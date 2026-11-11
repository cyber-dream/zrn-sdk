/**
 * zrnui - host.c
 * The root: owns a tree, lays it out, paints it with its popups and
 * tooltip, routes input (hit testing, hover, capture, focus), and keeps
 * time.
 */

#include "internal.h"

zrn_root_t* zrn_root_lookup(const Zid id) { return zrn_handle_get(id, 1); }

static zrn_root_t* get_root(const Zid id) {
	zrn_root_t* const r = zrn_root_lookup(id);
	if (!r) zrn_error(Z_ERROR_INVALID_ID, "invalid root handle 0x%x", (unsigned)id);
	return r;
}


/* ============================================================================
 *  attaching trees
 * ============================================================================ */

void zrn_root_register_popup(zrn_root_t* const r, zrn_element_t* const popup) {
	for (int i = 0; i < r->popup_count; i++) if (r->popups[i] == popup) return;
	if (r->popup_count == r->popup_cap) {
		const int cap = r->popup_cap ? r->popup_cap * 2 : 4;
		zrn_element_t** list = realloc(r->popups, (size_t)cap * sizeof *list);
		if (!list) return;
		r->popups = list;
		r->popup_cap = cap;
	}
	r->popups[r->popup_count++] = popup;
	zrn_changed_layout(popup);
}

void zrn_root_unregister_popup(zrn_root_t* const r, const zrn_element_t* const popup) {
	for (int i = 0; i < r->popup_count; i++) {
		if (r->popups[i] == popup) {
			memmove(r->popups + i, r->popups + i + 1, (size_t)(r->popup_count - i - 1) * sizeof *r->popups);
			r->popup_count--;
			r->paint_dirty = 1;
			return;
		}
	}
}

void zrn_root_attach(zrn_root_t* const r, zrn_element_t* const e) {
	e->root = r;
	zrn_bindings_apply(e);
	if (e->cls == &zrn_class_popup && ZST_AS(zrn_popup_t, e)->is_open) zrn_root_register_popup(r, e);
	for (int i = 0; i < e->child_count; i++) zrn_root_attach(r, e->children[i]);
	zrn_raise(e, Z_EVENT_LOADED);
}

void zrn_element_forget(zrn_element_t* const e) {
	zrn_root_t* const r = e->root;
	if (!r) return;
	if (r->focus == e) r->focus = nullptr;
	if (r->capture == e) r->capture = nullptr;
	if (r->hover == e) r->hover = nullptr;
	if (r->tilt_element == e) r->tilt_element = nullptr;
	if (r->tooltip_target == e) r->tooltip_target = nullptr;
	if (r->visual == e) r->visual = nullptr;
	if (r->open_bar == e) r->open_bar = nullptr;
	if (e->cls == &zrn_class_popup) zrn_root_unregister_popup(r, e);
}

void zrn_root_detach(zrn_element_t* const e) {
	for (int i = 0; i < e->child_count; i++) zrn_root_detach(e->children[i]);
	if (e->root) {
		zrn_root_t* const r = e->root;
		zrn_element_forget(e);
		e->pointer_over = 0;
		r->paint_dirty = 1;
		r->layout_dirty = 1;
		e->root = nullptr;
	}
}


/* ============================================================================
 *  focus and capture
 * ============================================================================ */

void zrn_set_focus(zrn_root_t* r, zrn_element_t* e) {
	zrn_element_t* old = r->focus;
	zrn_input_t in;
	if (old == e) return;
	memset(&in, 0, sizeof in);
	r->focus = e;
	r->paint_dirty = 1;
	{
		const Zid new_id = e ? e->id : Z_INVALID_ID, old_id = old ? old->id : Z_INVALID_ID;
		if (old) {
			in.type = ZST_IN_FOCUS_OUT;
			if (old->cls->input) old->cls->input(old, &in);
			old = zrn_lookup(old_id);
			if (old) zrn_raise(old, Z_EVENT_LOST_FOCUS);
		}
		/* the handlers above may have destroyed the new element or moved the focus elsewhere */
		e = new_id != Z_INVALID_ID ? zrn_lookup(new_id) : nullptr;
		if (e && r->focus == e) {
			in.type = ZST_IN_FOCUS_IN;
			if (e->cls->input) e->cls->input(e, &in);
			e = zrn_lookup(new_id);
			if (e) zrn_raise(e, Z_EVENT_GOT_FOCUS);
		}
	}
}

void zrn_set_capture(zrn_root_t* const r, zrn_element_t* const e) {
	zrn_element_t* const old = r->capture;
	if (old == e) return;
	r->capture = e;
	if (old && old->cls->input) {
		zrn_input_t in = { .type = ZST_IN_CAPTURE_LOST };
		old->cls->input(old, &in);
	}
}

void zrn_release_capture(zrn_root_t* const r, zrn_element_t* const e) {
	if (r->capture == e) r->capture = nullptr;
}

Zid Zroot_get_focus(const Zid root) {
	const zrn_root_t* const r = get_root(root);
	return r && r->focus ? r->focus->id : Z_INVALID_ID;
}

Zid Zroot_get_pointer_over(const Zid root) {
	const zrn_root_t* const r = get_root(root);
	return r && r->hover ? r->hover->id : Z_INVALID_ID;
}

static void collect_tab_stops(zrn_element_t* const e, zrn_element_t*** list, int* const count, int* const cap) {
	if (e->visibility != Z_VISIBLE || !e->enabled) return;
	/* what a popup holds is a tab stop only while it is open, and then the list of the root has it */
	if (e->cls == &zrn_class_popup) return;
	if (e->focusable && e->tab_stop) {
		if (*count == *cap) {
			const int ncap = *cap ? *cap * 2 : 16;
			zrn_element_t** more = realloc(*list, (size_t)ncap * sizeof *more);
			if (!more) return;
			*list = more;
			*cap = ncap;
		}
		(*list)[(*count)++] = e;
	}
	for (int i = 0; i < e->child_count; i++) collect_tab_stops(e->children[i], list, count, cap);
}

void Zroot_move_focus(const Zid root, const int forward) {
	zrn_root_t* const r = get_root(root);
	zrn_element_t** list = nullptr;
	int count = 0, cap = 0, i, j, current = -1, first_popup;
	if (!r || !r->visual) return;
	/* a modal popup is all there is to reach while it is open */
	first_popup = 0;
	for (i = 0; i < r->popup_count; i++) if (ZST_AS(zrn_popup_t, r->popups[i])->modal) first_popup = i + 1;
	if (first_popup == 0) collect_tab_stops(r->visual, &list, &count, &cap);
	for (i = first_popup > 0 ? first_popup - 1 : 0; i < r->popup_count; i++) if (r->popups[i]->child_count) collect_tab_stops(r->popups[i]->children[0], &list, &count, &cap);
	if (count == 0) { free(list); return; }
	/* stable sort by TabIndex */
	for (i = 1; i < count; i++) {
		zrn_element_t* const x = list[i];
		for (j = i - 1; j >= 0 && list[j]->tab_index > x->tab_index; j--) list[j + 1] = list[j];
		list[j + 1] = x;
	}
	for (i = 0; i < count; i++) if (list[i] == r->focus) current = i;
	if (current < 0) current = forward ? -1 : count;
	current += forward ? 1 : -1;
	current = (current + count) % count;
	zrn_set_focus(r, list[current]);
	free(list);
}


/* ============================================================================
 *  public: lifecycle and properties
 * ============================================================================ */

Zid Zroot_create(void) {
	zrn_root_t* r;
	if (!Zinit()) return Z_INVALID_ID;
	r = (zrn_root_t*)calloc(1, sizeof *r);
	if (!r) {
		zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
		return Z_INVALID_ID;
	}
	r->width = 0;
	r->height = 0;
	/* the scale divides into the UI scale (of the layout grid) and the density of the buffer (crispness) */
	r->ui = r->density = r->scale = 1.0f;
	r->first_frame = 1;
	r->layout_dirty = r->paint_dirty = 1;
	r->renderer = zrn_config()->renderer;
	r->animations = zrn_config()->animations;
	zrn_theme_init(&r->theme, Z_THEME_DARK, zrn_accent_color(Z_ACCENT_COBALT));
	r->theme_from = r->theme_target = r->theme;
	r->theme_transitioning = 0;
	r->theme_t = 0;
	r->id = zrn_handle_alloc(r, 1);
	r->overlay = zrn_create(Z_CLASS_CANVAS);
	if (r->overlay) zrn_root_attach(r, r->overlay);
	return r->id;
}

void Zroot_destroy(const Zid root) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	if (r->visual) zrn_destroy(r->visual);
	if (r->overlay) {
		/* what is shown at an element belongs to the program: it only stops being shown */
		while (r->overlay->child_count > 0) zrn_remove_child(r->overlay, r->overlay->children[r->overlay->child_count - 1]);
		zrn_destroy(r->overlay);
	}
	free(r->popups);
	zrn_root_resources_free(r);
	free(r->pixels);
	if (r->gpu) zrn_gl_destroy(r->gpu);
	zrn_handle_free(r->id);
	free(r);
	Zshutdown();
}

int Zroot_is_valid(const Zid root) { return zrn_root_lookup(root) != nullptr; }

void Zroot_set_visual(const Zid root, const Zid visual) {
	zrn_root_t* const r = get_root(root);
	zrn_element_t* e = nullptr;
	if (!r) return;
	if (visual != Z_INVALID_ID) {
		e = zrn_get(visual, Z_CLASS_ELEMENT);
		if (!e) return;
		if (e->parent) zrn_remove_child(e->parent, e);
	}
	if (r->visual && r->visual != e) zrn_destroy(r->visual);
	r->visual = e;
	if (e) zrn_root_attach(r, e);
	r->layout_dirty = r->paint_dirty = 1;
}

Zid Zroot_get_visual(const Zid root) {
	const zrn_root_t* const r = get_root(root);
	return r && r->visual ? r->visual->id : Z_INVALID_ID;
}

void Zroot_set_size(const Zid root, const int width, const int height) {
	zrn_root_t* const r = get_root(root);
	if (!r || width <= 0 || height <= 0) return;
	r->width = width;
	r->height = height;
	r->layout_dirty = r->paint_dirty = 1;
	if (r->visual) zrn_changed_layout(r->visual);
}

/** The UI scale: the size of the layout grid relative to the window (the painter draws it
 * back up); at 720 x 1280 and a scale of 1.5 the page is laid out in the 480 x 800 grid.
 * The scale belongs to the shell, the platform layer passes it on; it is not a zoom. */
void Zroot_set_scale(const Zid root, const float scale) {
	zrn_root_t* const r = get_root(root);
	if (!r || scale <= 0) return;
	r->ui = scale;
	r->scale = scale * r->density;
	r->paint_dirty = 1;
}

/** The density of the buffer against the logical size of the window: only crispness,
 * what the shell needs to show the content sharply; the size of the content is the job of the UI scale. */
void Zroot_set_density(const Zid root, const float density) {
	zrn_root_t* const r = get_root(root);
	if (!r || density <= 0) return;
	r->density = density;
	r->scale = density * r->ui;
	r->paint_dirty = 1;
}

void Zroot_set_theme(const Zid root, const ZTheme theme) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	ZColor bg = r->theme.background;
	int has = r->theme.has_background;
	if (!zrn_animations_enabled(r)) {
		/* instant, as before: no cross-fade when animations are off */
		zrn_theme_init(&r->theme, theme, r->theme.accent);
		if (has) { r->theme.background = bg; r->theme.has_background = 1; }
		r->theme_from = r->theme_target = r->theme;
		r->theme_transitioning = 0;
		r->theme_t = 0;
		r->paint_dirty = 1;
		zrn_root_resources_changed(r);
		return;
	}
	r->theme_from = r->theme;
	zrn_theme_init(&r->theme_target, theme, r->theme.accent);
	if (has) { r->theme_target.background = bg; r->theme_target.has_background = 1; }
	r->theme_transitioning = 1;
	r->theme_t = 0;
	r->paint_dirty = 1;
	zrn_root_resources_changed(r);
}

ZTheme Zroot_get_theme(const Zid root) {
	zrn_root_t* const r = get_root(root);
	return r ? r->theme.theme : Z_THEME_DARK;
}

void Zroot_set_accent_color(const Zid root, const ZColor color) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	r->theme.accent = color;
	r->theme.colors[ZST_TC_ACCENT] = color;
	r->paint_dirty = 1;
	zrn_root_resources_changed(r);
}

void Zroot_set_accent(const Zid root, const ZAccent accent) { Zroot_set_accent_color(root, zrn_accent_color(accent)); }

ZColor Zroot_get_accent_color(const Zid root) {
	zrn_root_t* const r = get_root(root);
	return r ? r->theme.accent : Zcolor_rgba(0, 0, 0, 0);
}

void Zroot_set_background(const Zid root, const ZColor color) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	r->theme.background = color;
	r->theme.has_background = 1;
	r->paint_dirty = 1;
}


/* ============================================================================
 *  layout
 * ============================================================================ */

static void layout_popups(const zrn_root_t* const r) {
	const float lw = (float)r->width / r->ui, lh = (float)r->height / r->ui;
	for (int i = 0; i < r->popup_count; i++) {
		const zrn_popup_t* const popup = ZST_AS(zrn_popup_t, r->popups[i]);
		zrn_element_t* const child = popup->base.child_count ? popup->base.children[0] : nullptr;
		ZPoint origin, pos;
		float width;
		if (!child) continue;
		zrn_measure(child, zrn_size(lw, lh));
		width = popup->full_width ? lw : child->desired.width;
		zrn_arrange(child, zrn_rect(0, 0, width, child->desired.height));
		origin = zrn_to_root(&popup->base, (ZPoint){ .x = 0, .y = 0 });
		pos.x = popup->h_offset;
		pos.y = popup->v_offset;
		if (popup->target != Z_INVALID_ID) {
			const zrn_element_t* const target = zrn_lookup(popup->target);
			if (target) {
				const ZPoint t = zrn_to_root(target, (ZPoint){ .x = 0, .y = 0 });
				switch (popup->placement) {
					case Z_PLACE_TOP: pos.x += t.x; pos.y += t.y - child->desired.height; break;
					case Z_PLACE_LEFT: pos.x += t.x - width; pos.y += t.y; break;
					case Z_PLACE_RIGHT: pos.x += t.x + target->rect.width; pos.y += t.y; break;
					default: pos.x += t.x; pos.y += t.y + target->rect.height; break;
				}
				/* flip to the other side when it would leave the screen */
				if (popup->placement == Z_PLACE_BOTTOM && pos.y + child->desired.height > lh && t.y - child->desired.height >= 0) pos.y = t.y - child->desired.height;
				if (popup->placement == Z_PLACE_TOP && pos.y < 0 && t.y + target->rect.height + child->desired.height <= lh) pos.y = t.y + target->rect.height;
			}
		}
		if (popup->full_width) pos.x = 0;
		pos.x = zrn_clampf(pos.x, 0, zrn_maxf(0, lw - width));
		pos.y = zrn_clampf(pos.y, 0, zrn_maxf(0, lh - child->desired.height));
		child->rect.x += pos.x - origin.x;
		child->rect.y += pos.y - origin.y;
	}
}

void Zroot_update_layout(const Zid root) {
	zrn_root_t* const r = get_root(root);
	int pass;
	float lw, lh;
	if (!r) return;
	/* the layout grid is the window divided by the scale: at 720 x 1280 and a scale of 1.5
	 * the page is laid out in the phone grid of 480 x 800, the painter draws it x 1.5 */
	lw = (float)r->width / r->ui;
	lh = (float)r->height / r->ui;
	/* a pass can change what the next one needs (scroll bars appear): up to a few */
	for (pass = 0; pass < 4; pass++) {
		r->layout_dirty = 0;
		if (r->visual) {
			zrn_measure(r->visual, zrn_size(lw, lh));
			zrn_arrange(r->visual, zrn_rect(0, 0, lw, lh));
		}
		layout_popups(r);
		if (!r->layout_dirty) break;
	}
}


/* ============================================================================
 *  painting
 * ============================================================================ */

void zrn_paint_element(zrn_element_t* const e, zrn_painter_t* const p, zrn_root_t* const r) {
	float tx = p->tx, ty = p->ty, alpha = p->alpha;
	float saved_xf[16] = {};
	int saved_has_xf = p->has_xf, count, heap, i;
	if (e->visibility != Z_VISIBLE) return;
	/* an element is skipped when it is out of view, unless it holds things that may be in view: a zero-sized Canvas whose children overflow */
	if (zrn_clipped_out(p, e->rect) && (e->clip_to_bounds || e->child_count == 0)) return;
	p->tx += e->rect.x * p->scale;
	p->ty += e->rect.y * p->scale;
	p->alpha *= e->opacity;
	/* read once: the transform is pushed and popped as a pair, whatever the paint handlers do to the element */
	const bool b_is_transformed = e->transformed;
	if (b_is_transformed) {
		/* the transform of this element, about its centre of projection, under the one of its parent */
		float own[16];
		memcpy(saved_xf, p->xf, sizeof saved_xf);
		zrn_xf_of_element(e, p->tx, p->ty, e->rect.width * p->scale, e->rect.height * p->scale, p->scale, own);
		zrn_xf_mul(p->xf, p->xf, own);
		p->has_xf = 1;
		p->be->set_transform(p, p->xf);
	}
	if (e->clip_to_bounds) zrn_push_clip(p, zrn_local_rect(e));
	if (e->cls->paint) e->cls->paint(e, p);
	zrn_element_t** list = zrn_ordered(e, &count, &heap);
	for (i = 0; i < count; i++) if (!list[i]->is_popup_child) zrn_paint_element(list[i], p, r);
	zrn_ordered_free(list, heap);
	if (e->cls->paint_over) e->cls->paint_over(e, p);
	if (e->clip_to_bounds) zrn_pop_clip(p);
	if (b_is_transformed) {
		memcpy(p->xf, saved_xf, sizeof saved_xf);
		p->has_xf = saved_has_xf;
		p->be->set_transform(p, saved_has_xf ? p->xf : nullptr);
	}
	p->tx = tx;
	p->ty = ty;
	p->alpha = alpha;
}

static void paint_tooltip(const zrn_root_t* const r, zrn_painter_t* const p) {
	const zrn_font_t font = { .family = Z_FONT_NORMAL, .size = ZST_FONT_SMALL, .spacing = 0 };
	float w, h, x, y;
	if (!r->tooltip_target || !r->tooltip_target->tooltip || r->tooltip_timer < 500.0f) return;
	zrn_text_layout_t* l = zrn_text_layout_new(font, r->tooltip_target->tooltip, (int)strlen(r->tooltip_target->tooltip), 300.0f, 1, 0, 0);
	if (!l) return;
	w = l->width + 16;
	h = l->height + 12;
	x = zrn_clampf(r->pointer_x, 0, (float)r->width / r->ui - w);
	y = r->pointer_y + 28;
	if (y + h > (float)r->height / r->ui) y = r->pointer_y - h - 8;
	p->tx = 0; p->ty = 0; p->alpha = 1;
	zrn_fill_rect(p, zrn_rect(x, y, w, h), zrn_theme_color(nullptr, ZST_TC_CHROME));
	zrn_stroke_rect(p, zrn_rect(x, y, w, h), Zthickness_uniform(1), r->theme.colors[ZST_TC_BORDER]);
	zrn_draw_text(p, l, x + 8, y + 6, l->width, Z_TEXT_LEFT, Z_TRIM_NONE, r->theme.colors[ZST_TC_FOREGROUND]);
	zrn_text_layout_free(l);
}

int zrn_root_paint(zrn_root_t* const r, zrn_painter_t* const p) {
	if (r->layout_dirty || r->first_frame) Zroot_update_layout(r->id);
	if (!(r->paint_dirty || r->first_frame)) return false;
	zrn_paint_clear(p, r->theme.has_background ? r->theme.background : r->theme.colors[ZST_TC_BACKGROUND]);
	if (r->visual) zrn_paint_element(r->visual, p, r);
	for (int i = 0; i < r->popup_count; i++) {
		zrn_element_t* const popup = r->popups[i];
		const ZPoint origin = zrn_to_root(popup, (ZPoint){ .x = 0, .y = 0 });
		if (!popup->child_count) continue;
		if (ZST_AS(zrn_popup_t, popup)->modal) {
			/* ContentDialogDimmingThemeBrush over everything under the dialog */
			p->tx = 0;
			p->ty = 0;
			p->alpha = 1.0f;
			zrn_fill_rect(p, zrn_rect(0, 0, (float)r->width / r->ui, (float)r->height / r->ui), r->theme.colors[ZST_TC_CONTENT_DIALOG_DIMMING]);
		}
		p->tx = origin.x * r->scale;
		p->ty = origin.y * r->scale;
		p->alpha = 1.0f;
		zrn_paint_element(popup->children[0], p, r);
	}
	paint_tooltip(r, p);
	r->paint_dirty = 0;
	r->first_frame = 0;
	return true;
}

static const char* g_last_renderer = "software";

void zrn_note_renderer(const char* name) { g_last_renderer = name; }

/* A frame on the off-screen GPU context, read back into the pixels of the root. false when there is none. */
static int render_gpu(zrn_root_t* const r, const int w, const int h) {
	zrn_painter_t p;
	const int sized = r->pixels_w == w && r->pixels_h == h && r->pixels;
	if (!r->gpu) r->gpu = zrn_gl_create_offscreen(w, h);
	if (!r->gpu) return false;
	if (!sized) {
		free(r->pixels);
		r->pixels = (uint8_t*)calloc((size_t)w * (size_t)h, 4);
		r->pixels_w = w;
		r->pixels_h = h;
		if (!r->pixels) return false;
	}
	zrn_gl_begin(r->gpu, &p, w, h, r->scale);
	zrn_root_paint(r, &p);
	zrn_gl_end(r->gpu, 0);
	if (!zrn_gl_read(r->gpu, r->pixels, w, h)) return false;
	g_last_renderer = "gles2";
	return true;
}

int Zroot_render(const Zid root, ZFrame* const frame) {
	zrn_root_t* const r = get_root(root);
	zrn_painter_t p;
	int w, h, changed;
	if (!r) return false;
	/* the buffer follows the density of the window, not the UI scale: the painter's scale (ui times density) fills it exactly */
	w = (int)ceilf((float)r->width * r->density);
	h = (int)ceilf((float)r->height * r->density);
	if (r->layout_dirty || r->first_frame) Zroot_update_layout(root);
	changed = r->paint_dirty || r->first_frame || r->pixels_w != w || r->pixels_h != h;
	if (changed) {
		if (r->renderer == Z_RENDERER_GPU) {
			r->paint_dirty = 1;
			if (!render_gpu(r, w, h)) {
				zrn_error(Z_ERROR_IO, "no OpenGL ES 2 context for Z_RENDERER_GPU");
				return false;
			}
		} else {
			if (r->pixels_w != w || r->pixels_h != h || !r->pixels) {
				free(r->pixels);
				r->pixels = (uint8_t*)calloc((size_t)w * (size_t)h, 4);
				r->pixels_w = w;
				r->pixels_h = h;
				if (!r->pixels) {
					zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
					return false;
				}
			}
			zrn_painter_init(&p, r->pixels, w, h, r->scale);
			r->paint_dirty = 1;
			zrn_root_paint(r, &p);
			g_last_renderer = "software";
		}
	}
	if (frame) {
		frame->pixels = r->pixels;
		frame->width = r->pixels_w;
		frame->height = r->pixels_h;
		frame->stride = r->pixels_w * 4;
	}
	return changed;
}

void Zroot_set_renderer(const Zid root, const ZRenderer renderer) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	r->renderer = renderer;
	r->paint_dirty = 1;
}

ZRenderer Zroot_get_renderer(const Zid root) {
	const zrn_root_t* const r = get_root(root);
	return r ? r->renderer : Z_RENDERER_AUTO;
}

const char* Zroot_get_renderer_name(const Zid root) {
	(void)root;
	return g_last_renderer;
}


/* ============================================================================
 *  time
 * ============================================================================ */

static void zrn_tick_tree(zrn_element_t* const e, const float dt) {
	if (e->tweens) zrn_tweens_tick(e);
	if (e->wants_tick && e->cls->tick) e->cls->tick(e, dt);
	for (int i = 0; i < e->child_count; i++) zrn_tick_tree(e->children[i], dt);
}

/* ZRNUI_ANIMATION_SPEED: a float multiplier for every animation (0.5 = half speed,
 * 2 = twice as fast; 0 or negative disables the speed override and leaves 1). */
static ZColor zrn_lerp_color(const ZColor a, const ZColor b, const float t) {
	const ZColor c = { a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t };
	return c;
}

static void zrn_theme_lerp(zrn_theme_t* const out, const zrn_theme_t* const a, const zrn_theme_t* const b, const float t) {
	out->theme = t >= 0.5f ? b->theme : a->theme;
	out->accent = zrn_lerp_color(a->accent, b->accent, t);
	out->background = zrn_lerp_color(a->background, b->background, t);
	out->has_background = b->has_background;
	for (int i = 0; i < ZST_TC_COUNT; i++) out->colors[i] = zrn_lerp_color(a->colors[i], b->colors[i], t);
}

static float zrn_animation_speed(void) {
	const float speed = zrn_config()->animation_speed;
	return speed > 0 ? speed : 1.0f;
}
void Zroot_tick(const Zid root, const float delta_ms) {
	zrn_root_t* const r = get_root(root);
	if (!r) return;
	r->time_ms += delta_ms;
	float speed = zrn_animation_speed();
	/* cross-fade of the theme: the displayed theme moves from theme_from to theme_target */
	if (r->theme_transitioning) {
		r->theme_t += (delta_ms * speed) / 250.0f;
		if (r->theme_t >= 1.0f) {
			r->theme_t = 1.0f;
			r->theme_transitioning = 0;
			r->theme = r->theme_target;
		} else {
			zrn_theme_lerp(&r->theme, &r->theme_from, &r->theme_target, r->theme_t);
		}
		r->paint_dirty = 1;
	}
	zrn_tilt_tick(r, delta_ms * speed);
	if (r->visual) zrn_tick_tree(r->visual, delta_ms * speed);
	if (r->overlay) zrn_tick_tree(r->overlay, delta_ms * speed);
	if (r->tooltip_target && r->tooltip_target->tooltip) {
		const float before = r->tooltip_timer;
		r->tooltip_timer += delta_ms;
		if (before < 500.0f && r->tooltip_timer >= 500.0f) r->paint_dirty = 1;
	}
}


/* ============================================================================
 *  input
 * ============================================================================ */

/* Bubbles an input from `e` up. Pointer positions are given in root coordinates in `in`.
 * The elements are walked by id: a handler may destroy any of them (a tap that opens another page). */
int zrn_dispatch(zrn_element_t* const e, zrn_input_t* const in) {
	Zid ids[64];
	int n = zrn_chain(e, ids, 64), i;
	float rx = in->x, ry = in->y;
	const int pointer = in->type >= ZST_IN_POINTER_DOWN && in->type <= ZST_IN_WHEEL;
	for (i = 0; i < n; i++) {
		zrn_element_t* const a = zrn_lookup(ids[i]);
		zrn_input_t local = *in;
		if (!a) return 1;
		if (!a->cls->input) continue;
		if (pointer) {
			ZPoint q = zrn_from_root(a, (ZPoint){ .x = rx, .y = ry });
			local.x = q.x;
			local.y = q.y;
			if (!a->enabled) continue;
		}
		if (a->cls->input(a, &local)) return 1;
	}
	return 0;
}

static zrn_element_t* pick(const zrn_root_t* const r, const float x, const float y) {
	for (int i = r->popup_count - 1; i >= 0; i--) {
		zrn_element_t* const popup = r->popups[i];
		zrn_element_t* const child = popup->child_count ? popup->children[0] : nullptr;
		ZPoint origin, q;
		if (!child) continue;
		origin = zrn_to_root(popup, (ZPoint){ .x = 0, .y = 0 });
		q.x = x - origin.x - child->rect.x;
		q.y = y - origin.y - child->rect.y;
		zrn_element_t* hit = zrn_hit_test(child, q);
		if (hit) return hit;
		/* a modal popup is all there is to reach under it */
		if (ZST_AS(zrn_popup_t, popup)->modal) return nullptr;
	}
	if (r->visual) return zrn_hit_test(r->visual, (ZPoint){ x - r->visual->rect.x, y - r->visual->rect.y });
	return nullptr;
}

static int is_inside(const zrn_element_t* e, const zrn_element_t* ancestor) {
	for (; e; e = e->parent) if (e == ancestor) return 1;
	return 0;
}

/* A press outside the open popups closes the ones that do not stay open. Returns 1 when the press was on the owner of a closed popup. */
static int close_outside(const zrn_root_t* const r, const zrn_element_t* const target) {
	int i, swallow = 0;
	for (i = r->popup_count - 1; i >= 0; i--) {
		const auto popup = ZST_AS(zrn_popup_t, r->popups[i]);
		if (popup->stays_open) continue;
		if (target && is_inside(target, &popup->base)) continue;
		if (target && popup->base.parent && is_inside(target, popup->base.parent)) swallow = 1;
		/* a tap on the element a flyout is shown at closes it: it does not open it again */
		if (target && popup->target != Z_INVALID_ID && zrn_lookup(popup->target) && is_inside(target, zrn_lookup(popup->target))) swallow = 1;
		zrn_popup_close(&popup->base);
		i = r->popup_count;
	}
	return swallow;
}

static int in_a_popup(const zrn_root_t* const r, const zrn_element_t* const e) {
	for (int i = 0; i < r->popup_count; i++) if (e && is_inside(e, r->popups[i])) return 1;
	return 0;
}

/* The Back button and Escape: the topmost thing that can be dismissed closes, and nothing else happens. */
int zrn_root_dismiss_top(zrn_root_t* const r) {
	for (int i = r->popup_count - 1; i >= 0; i--) {
		const auto popup = ZST_AS(zrn_popup_t, r->popups[i]);
		/* a modal popup (a ContentDialog) closes as if it were told to hide; a popup that stays open is the program's own */
		if (popup->stays_open && !popup->modal) continue;
		zrn_popup_close(&popup->base);
		return 1;
	}
	if (r->open_bar) {
		zrn_element_t* const bar = r->open_bar;
		if (!ZST_AS(zrn_app_bar_t, bar)->is_sticky) {
			Zapp_bar_set_is_open(bar->id, false);
			return 1;
		}
	}
	return 0;
}

static void update_hover(zrn_root_t* const r, zrn_element_t* const target, const float x, const float y) {
	Zid old_ids[64], new_ids[64];
	int old_n, new_n, i, j;
	zrn_input_t in;
	if (target == r->hover) return;
	memset(&in, 0, sizeof in);
	in.x = x;
	in.y = y;
	old_n = r->hover ? zrn_chain(r->hover, old_ids, 64) : 0;
	new_n = target ? zrn_chain(target, new_ids, 64) : 0;
	r->hover = target;
	r->paint_dirty = 1;
	/* the elements are walked by id: a Click-on-hover handler may destroy them */
	for (i = 0; i < old_n; i++) {
		int stays = 0;
		zrn_element_t* const a = zrn_lookup(old_ids[i]);
		for (j = 0; j < new_n; j++) if (new_ids[j] == old_ids[i]) stays = 1;
		if (!a || stays) continue;
		a->pointer_over = 0;
		in.type = ZST_IN_POINTER_LEAVE;
		if (a->cls->input) a->cls->input(a, &in);
	}
	for (i = 0; i < new_n; i++) {
		zrn_element_t* const a = zrn_lookup(new_ids[i]);
		if (!a || a->pointer_over) continue;
		a->pointer_over = 1;
		in.type = ZST_IN_POINTER_ENTER;
		if (a->cls->input) a->cls->input(a, &in);
	}
}

static zrn_element_t* tooltip_owner(zrn_element_t* e) {
	for (; e; e = e->parent) if (e->tooltip) return e;
	return nullptr;
}

/* Lets the ancestors that observe the pointer see an event first. */
static void preview(const zrn_element_t* const target, const zrn_input_type_t type, const float x, const float y) {
	Zid ids[64];
	int n = zrn_chain(target, ids, 64), i;
	for (i = 0; i < n; i++) {
		zrn_input_t in;
		zrn_element_t* const a = zrn_lookup(ids[i]);
		if (!a) return;
		if (!a->observes_pointer || !a->enabled || !a->cls->input) continue;
		memset(&in, 0, sizeof in);
		ZPoint q = zrn_from_root(a, (ZPoint){ .x = x, .y = y });
		in.type = type;
		in.x = q.x;
		in.y = q.y;
		a->cls->input(a, &in);
	}
}

/* The target again after something that can run user code: NULL if it is gone. */
#define RELOOK(id) ((id) != Z_INVALID_ID ? zrn_lookup(id) : nullptr)

int Zroot_inject_pointer(const Zid root, const ZPointerEvent event, float x, float y) {
	zrn_root_t* r = get_root(root);
	zrn_input_t in;
	int handled = 0;
	if (!r) return false;
	if (r->layout_dirty) Zroot_update_layout(root);
	/* the window gives the pointer in its logical pixels; the layout thinks in the grid of the UI scale */
	x /= r->ui;
	y /= r->ui;
	r->pointer_x = x;
	r->pointer_y = y;
	memset(&in, 0, sizeof in);
	in.x = x;
	in.y = y;
	if (event == Z_POINTER_LEAVE) {
		update_hover(r, nullptr, x, y);
		return false;
	}
	zrn_element_t* target = r->capture ? r->capture : pick(r, x, y);
	Zid tid = target ? target->id : Z_INVALID_ID;
	if (event == Z_POINTER_MOVE) {
		zrn_element_t* hit = r->capture ? pick(r, x, y) : target;
		zrn_element_t* owner = tooltip_owner(hit);
		update_hover(r, hit, x, y);
		if (!zrn_root_lookup(root)) return false;
		if (owner != r->tooltip_target) {
			if (r->tooltip_timer >= 500.0f) r->paint_dirty = 1;
			r->tooltip_target = owner && zrn_lookup(owner->id) ? owner : nullptr;
			r->tooltip_timer = 0;
		}
		zrn_tilt_move(r, x, y);
		target = RELOOK(tid);
		if (target) preview(target, ZST_IN_PREVIEW_MOVE, x, y);
		if (r->capture) target = r->capture;
		else target = RELOOK(tid);
		in.type = ZST_IN_POINTER_MOVE;
		if (target) handled = zrn_dispatch(target, &in);
		return handled;
	}
	if (event == Z_POINTER_DOWN) {
		r->tooltip_timer = 0;
		r->tooltip_target = nullptr;
		if (!r->capture && r->open_bar && !ZST_AS(zrn_app_bar_t, r->open_bar)->is_sticky && !is_inside(target, r->open_bar) && !in_a_popup(r, target)) {
			/* light dismiss: the press that closes the bar is not given to what is under it */
			Zapp_bar_set_is_open(r->open_bar->id, false);
			close_outside(r, target);
			return true;
		}
		if (!r->capture && close_outside(r, target)) return true;
		target = RELOOK(tid);
		if (tid != Z_INVALID_ID && !target) return true;
		update_hover(r, target, x, y);
		target = RELOOK(tid);
		if (tid != Z_INVALID_ID && !target) return true;
		for (zrn_element_t* f = target; f; f = f->parent) {
			if (f->focusable && zrn_effectively_enabled(f)) {
				zrn_set_focus(r, f);
				break;
			}
		}
		target = RELOOK(tid);
		if (tid != Z_INVALID_ID && !target) return true;
		if (target) preview(target, ZST_IN_PREVIEW_DOWN, x, y);
		target = RELOOK(tid);
		in.type = ZST_IN_POINTER_DOWN;
		{
			for (zrn_element_t* f2 = target; f2; f2 = f2->parent) {
				if (f2->tilt && zrn_effectively_enabled(f2)) {
					zrn_tilt_press(r, f2, x, y);
					break;
				}
			}
		}
		if (target) handled = zrn_dispatch(target, &in);
		return handled;
	}
	zrn_tilt_release(r);
	if (target) preview(target, ZST_IN_PREVIEW_UP, x, y);
	target = r->capture ? r->capture : RELOOK(tid);
	in.type = ZST_IN_POINTER_UP;
	if (target) handled = zrn_dispatch(target, &in);
	return handled;
}

int Zroot_inject_wheel(const Zid root, const float delta) {
	zrn_root_t* const r = get_root(root);
	zrn_input_t in;
	if (!r) return false;
	zrn_element_t* target = pick(r, r->pointer_x, r->pointer_y);
	if (!target) return false;
	memset(&in, 0, sizeof in);
	in.type = ZST_IN_WHEEL;
	in.x = r->pointer_x;
	in.y = r->pointer_y;
	in.delta = delta;
	return zrn_dispatch(target, &in);
}

int Zroot_back(const Zid root) {
	zrn_root_t* const r = get_root(root);
	if (r && zrn_root_dismiss_top(r)) return true;
	if (!r || !r->visual || !zrn_is_a(r->visual, Z_CLASS_FRAME)) return false;
	return Zframe_go_back(r->visual->id);
}

int Zroot_inject_key(const Zid root, const ZKey key, const int modifiers, const int pressed) {
	zrn_root_t* const r = get_root(root);
	zrn_input_t in;
	int handled = 0;
	if (!r) return false;
	memset(&in, 0, sizeof in);
	in.type = pressed ? ZST_IN_KEY_DOWN : ZST_IN_KEY_UP;
	in.key = key;
	in.mods = modifiers;
	if (r->focus) handled = zrn_dispatch(r->focus, &in);
	if (!handled && pressed && key == Z_KEY_TAB) {
		Zroot_move_focus(root, !(modifiers & Z_MOD_SHIFT));
		handled = 1;
	}
	if (!handled && pressed && key == Z_KEY_ESCAPE) handled = Zroot_back(root);
	return handled;
}

int Zroot_inject_text(const Zid root, const char* const text) {
	zrn_root_t* const r = get_root(root);
	zrn_input_t in;
	if (!r || !r->focus || !text) return false;
	memset(&in, 0, sizeof in);
	in.type = ZST_IN_TEXT;
	in.text = text;
	return zrn_dispatch(r->focus, &in);
}
