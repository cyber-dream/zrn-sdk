/**
 * Kestrel - host.c
 * The root: owns a tree, lays it out, paints it with its popups and
 * tooltip, routes input (hit testing, hover, capture, focus), and keeps
 * time.
 */

#include "internal.h"

kst_root_t* kst_root_lookup(Kid id) { return (kst_root_t*)kst_handle_get(id, 1); }

static kst_root_t* get_root(Kid id) {
	kst_root_t* r = kst_root_lookup(id);
	if (!r) kst_error(K_ERROR_INVALID_ID, "invalid root handle 0x%x", (unsigned)id);
	return r;
}


/* ============================================================================
 *  attaching trees
 * ============================================================================ */

void kst_root_register_popup(kst_root_t* r, kst_element_t* popup) {
	int i;
	for (i = 0; i < r->popup_count; i++) if (r->popups[i] == popup) return;
	if (r->popup_count == r->popup_cap) {
		int cap = r->popup_cap ? r->popup_cap * 2 : 4;
		kst_element_t** list = (kst_element_t**)realloc(r->popups, (size_t)cap * sizeof *list);
		if (!list) return;
		r->popups = list;
		r->popup_cap = cap;
	}
	r->popups[r->popup_count++] = popup;
	kst_changed_layout(popup);
}

void kst_root_unregister_popup(kst_root_t* r, kst_element_t* popup) {
	int i;
	for (i = 0; i < r->popup_count; i++) {
		if (r->popups[i] == popup) {
			memmove(r->popups + i, r->popups + i + 1, (size_t)(r->popup_count - i - 1) * sizeof *r->popups);
			r->popup_count--;
			r->paint_dirty = 1;
			return;
		}
	}
}

void kst_root_attach(kst_root_t* r, kst_element_t* e) {
	int i;
	e->root = r;
	kst_bindings_apply(e);
	if (e->cls == &kst_class_popup && KST_AS(kst_popup_t, e)->is_open) kst_root_register_popup(r, e);
	for (i = 0; i < e->child_count; i++) kst_root_attach(r, e->children[i]);
	kst_raise(e, K_EVENT_LOADED);
}

void kst_element_forget(kst_element_t* e) {
	kst_root_t* r = e->root;
	if (!r) return;
	if (r->focus == e) r->focus = NULL;
	if (r->capture == e) r->capture = NULL;
	if (r->hover == e) r->hover = NULL;
	if (r->tilt_element == e) r->tilt_element = NULL;
	if (r->tooltip_target == e) r->tooltip_target = NULL;
	if (r->visual == e) r->visual = NULL;
	if (r->open_bar == e) r->open_bar = NULL;
	if (e->cls == &kst_class_popup) kst_root_unregister_popup(r, e);
}

void kst_root_detach(kst_element_t* e) {
	int i;
	for (i = 0; i < e->child_count; i++) kst_root_detach(e->children[i]);
	if (e->root) {
		kst_root_t* r = e->root;
		kst_element_forget(e);
		e->pointer_over = 0;
		r->paint_dirty = 1;
		r->layout_dirty = 1;
		e->root = NULL;
	}
}


/* ============================================================================
 *  focus and capture
 * ============================================================================ */

void kst_set_focus(kst_root_t* r, kst_element_t* e) {
	kst_element_t* old = r->focus;
	kst_input_t in;
	if (old == e) return;
	memset(&in, 0, sizeof in);
	r->focus = e;
	r->paint_dirty = 1;
	{
		Kid new_id = e ? e->id : K_INVALID_ID, old_id = old ? old->id : K_INVALID_ID;
		if (old) {
			in.type = KST_IN_FOCUS_OUT;
			if (old->cls->input) old->cls->input(old, &in);
			old = kst_lookup(old_id);
			if (old) kst_raise(old, K_EVENT_LOST_FOCUS);
		}
		/* the handlers above may have destroyed the new element or moved the focus elsewhere */
		e = new_id != K_INVALID_ID ? kst_lookup(new_id) : NULL;
		if (e && r->focus == e) {
			in.type = KST_IN_FOCUS_IN;
			if (e->cls->input) e->cls->input(e, &in);
			e = kst_lookup(new_id);
			if (e) kst_raise(e, K_EVENT_GOT_FOCUS);
		}
	}
}

void kst_set_capture(kst_root_t* r, kst_element_t* e) {
	kst_element_t* old = r->capture;
	if (old == e) return;
	r->capture = e;
	if (old && old->cls->input) {
		kst_input_t in;
		memset(&in, 0, sizeof in);
		in.type = KST_IN_CAPTURE_LOST;
		old->cls->input(old, &in);
	}
}

void kst_release_capture(kst_root_t* r, kst_element_t* e) {
	if (r->capture == e) r->capture = NULL;
}

Kid Kroot_get_focus(Kid root) {
	kst_root_t* r = get_root(root);
	return r && r->focus ? r->focus->id : K_INVALID_ID;
}

Kid Kroot_get_pointer_over(Kid root) {
	kst_root_t* r = get_root(root);
	return r && r->hover ? r->hover->id : K_INVALID_ID;
}

static void collect_tab_stops(kst_element_t* e, kst_element_t*** list, int* count, int* cap) {
	int i;
	if (e->visibility != K_VISIBLE || !e->enabled) return;
	/* what a popup holds is a tab stop only while it is open, and then the list of the root has it */
	if (e->cls == &kst_class_popup) return;
	if (e->focusable && e->tab_stop) {
		if (*count == *cap) {
			int ncap = *cap ? *cap * 2 : 16;
			kst_element_t** more = (kst_element_t**)realloc(*list, (size_t)ncap * sizeof *more);
			if (!more) return;
			*list = more;
			*cap = ncap;
		}
		(*list)[(*count)++] = e;
	}
	for (i = 0; i < e->child_count; i++) collect_tab_stops(e->children[i], list, count, cap);
}

void Kroot_move_focus(Kid root, int forward) {
	kst_root_t* r = get_root(root);
	kst_element_t** list = NULL;
	int count = 0, cap = 0, i, j, current = -1, first_popup;
	if (!r || !r->visual) return;
	/* a modal popup is all there is to reach while it is open */
	first_popup = 0;
	for (i = 0; i < r->popup_count; i++) if (KST_AS(kst_popup_t, r->popups[i])->modal) first_popup = i + 1;
	if (first_popup == 0) collect_tab_stops(r->visual, &list, &count, &cap);
	for (i = first_popup > 0 ? first_popup - 1 : 0; i < r->popup_count; i++) if (r->popups[i]->child_count) collect_tab_stops(r->popups[i]->children[0], &list, &count, &cap);
	if (count == 0) { free(list); return; }
	/* stable sort by TabIndex */
	for (i = 1; i < count; i++) {
		kst_element_t* x = list[i];
		for (j = i - 1; j >= 0 && list[j]->tab_index > x->tab_index; j--) list[j + 1] = list[j];
		list[j + 1] = x;
	}
	for (i = 0; i < count; i++) if (list[i] == r->focus) current = i;
	if (current < 0) current = forward ? -1 : count;
	current += forward ? 1 : -1;
	current = (current + count) % count;
	kst_set_focus(r, list[current]);
	free(list);
}


/* ============================================================================
 *  public: lifecycle and properties
 * ============================================================================ */

Kid Kroot_create(int width, int height) {
	kst_root_t* r;
	if (width < 0 || height < 0 || (width == 0) != (height == 0)) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "a root needs a positive size, or 0 x 0 when the window gives it");
		return K_INVALID_ID;
	}
	if (!Kinit()) return K_INVALID_ID;
	r = (kst_root_t*)calloc(1, sizeof *r);
	if (!r) {
		kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
		return K_INVALID_ID;
	}
	r->width = width;
	r->height = height;
	/* the scale divides into the UI scale (of the layout grid) and the density of the buffer (crispness) */
	r->ui = r->density = r->scale = 1.0f;
	r->first_frame = 1;
	r->layout_dirty = r->paint_dirty = 1;
	{
		const char* env = getenv("KESTREL_RENDERER");
		if (env && !strcmp(env, "software")) r->renderer = K_RENDERER_SOFTWARE;
		else if (env && !strcmp(env, "gpu")) r->renderer = K_RENDERER_GPU;
		env = getenv("KESTREL_ANIMATIONS");
		if (env && !strcmp(env, "off")) r->animations = K_ANIMATIONS_OFF;
		else if (env && !strcmp(env, "on")) r->animations = K_ANIMATIONS_ON;
	}
	kst_theme_init(&r->theme, K_THEME_DARK, kst_accent_color(K_ACCENT_COBALT));
	r->theme_from = r->theme_target = r->theme;
	r->theme_transitioning = 0;
	r->theme_t = 0;
	r->id = kst_handle_alloc(r, 1);
	r->overlay = kst_create(K_CLASS_CANVAS);
	if (r->overlay) kst_root_attach(r, r->overlay);
	return r->id;
}

void Kroot_destroy(Kid root) {
	kst_root_t* r = get_root(root);
	if (!r) return;
	if (r->visual) kst_destroy(r->visual);
	if (r->overlay) {
		/* what is shown at an element belongs to the program: it only stops being shown */
		while (r->overlay->child_count > 0) kst_remove_child(r->overlay, r->overlay->children[r->overlay->child_count - 1]);
		kst_destroy(r->overlay);
	}
	free(r->popups);
	kst_root_resources_free(r);
	free(r->pixels);
	if (r->gpu) kst_gl_destroy(r->gpu);
	kst_handle_free(r->id);
	free(r);
	Kshutdown();
}

int Kroot_is_valid(Kid root) { return kst_root_lookup(root) != NULL; }

void Kroot_set_visual(Kid root, Kid visual) {
	kst_root_t* r = get_root(root);
	kst_element_t* e = NULL;
	if (!r) return;
	if (visual != K_INVALID_ID) {
		e = kst_get(visual, K_CLASS_ELEMENT);
		if (!e) return;
		if (e->parent) kst_remove_child(e->parent, e);
	}
	if (r->visual && r->visual != e) kst_destroy(r->visual);
	r->visual = e;
	if (e) kst_root_attach(r, e);
	r->layout_dirty = r->paint_dirty = 1;
}

Kid Kroot_get_visual(Kid root) {
	kst_root_t* r = get_root(root);
	return r && r->visual ? r->visual->id : K_INVALID_ID;
}

void Kroot_set_size(Kid root, int width, int height) {
	kst_root_t* r = get_root(root);
	if (!r || width <= 0 || height <= 0) return;
	r->width = width;
	r->height = height;
	r->layout_dirty = r->paint_dirty = 1;
	if (r->visual) kst_changed_layout(r->visual);
}

/** The UI scale: the size of the layout grid relative to the window (the painter draws it
 * back up); at 720 x 1280 and a scale of 1.5 the page is laid out in the 480 x 800 grid.
 * The scale belongs to the shell; this is the override of KESTREL_SCALE, not a zoom. */
void Kroot_set_scale(Kid root, float scale) {
	kst_root_t* r = get_root(root);
	if (!r || scale <= 0) return;
	r->ui = scale;
	r->scale = scale * r->density;
	r->paint_dirty = 1;
}

/** The density of the buffer against the logical size of the window: only crispness,
 * what the compositor needs to draw the window sharply; the size of the content is the job of the UI scale. */
void kst_root_set_density(Kid root, float density) {
	kst_root_t* r = get_root(root);
	if (!r || density <= 0) return;
	r->density = density;
	r->scale = density * r->ui;
	r->paint_dirty = 1;
}

void Kroot_set_debug(Kid root, int flags) {
	kst_root_t* r = get_root(root);
	if (!r) return;
	r->debug_flags = flags ? K_DEBUG_BOUNDS | (flags & 2 ? 2 : 0) : 0;
	r->paint_dirty = 1;
}

int Kroot_get_debug(Kid root) {
	kst_root_t* r = get_root(root);
	return r ? r->debug_flags : 0;
}

void Kroot_set_theme(Kid root, KTheme theme) {
	kst_root_t* r = get_root(root);
	KColor bg;
	int has;
	if (!r) return;
	bg = r->theme.background;
	has = r->theme.has_background;
	if (!kst_animations_enabled(r)) {
		/* instant, as before: no cross-fade when animations are off */
		kst_theme_init(&r->theme, theme, r->theme.accent);
		if (has) { r->theme.background = bg; r->theme.has_background = 1; }
		r->theme_from = r->theme_target = r->theme;
		r->theme_transitioning = 0;
		r->theme_t = 0;
		r->paint_dirty = 1;
		kst_root_resources_changed(r);
		return;
	}
	r->theme_from = r->theme;
	kst_theme_init(&r->theme_target, theme, r->theme.accent);
	if (has) { r->theme_target.background = bg; r->theme_target.has_background = 1; }
	r->theme_transitioning = 1;
	r->theme_t = 0;
	r->paint_dirty = 1;
	kst_root_resources_changed(r);
}

KTheme Kroot_get_theme(Kid root) {
	kst_root_t* r = get_root(root);
	return r ? r->theme.theme : K_THEME_DARK;
}

void Kroot_set_accent_color(Kid root, KColor color) {
	kst_root_t* r = get_root(root);
	if (!r) return;
	r->theme.accent = color;
	r->theme.colors[KST_TC_ACCENT] = color;
	r->paint_dirty = 1;
	kst_root_resources_changed(r);
}

void Kroot_set_accent(Kid root, KAccent accent) { Kroot_set_accent_color(root, kst_accent_color(accent)); }

KColor Kroot_get_accent_color(Kid root) {
	kst_root_t* r = get_root(root);
	return r ? r->theme.accent : Kcolor_rgba(0, 0, 0, 0);
}

void Kroot_set_background(Kid root, KColor color) {
	kst_root_t* r = get_root(root);
	if (!r) return;
	r->theme.background = color;
	r->theme.has_background = 1;
	r->paint_dirty = 1;
}


/* ============================================================================
 *  layout
 * ============================================================================ */

static void layout_popups(kst_root_t* r) {
	int i;
	const float lw = (float)r->width / r->ui, lh = (float)r->height / r->ui;
	for (i = 0; i < r->popup_count; i++) {
		kst_popup_t* popup = KST_AS(kst_popup_t, r->popups[i]);
		kst_element_t* child = popup->base.child_count ? popup->base.children[0] : NULL;
		KPoint origin, pos;
		float width;
		if (!child) continue;
		kst_measure(child, kst_size(lw, lh));
		width = popup->full_width ? lw : child->desired.width;
		kst_arrange(child, kst_rect(0, 0, width, child->desired.height));
		origin = kst_to_root(&popup->base, (KPoint){ 0, 0 });
		pos.x = popup->h_offset;
		pos.y = popup->v_offset;
		if (popup->target != K_INVALID_ID) {
			kst_element_t* target = kst_lookup(popup->target);
			if (target) {
				KPoint t = kst_to_root(target, (KPoint){ 0, 0 });
				switch (popup->placement) {
					case K_PLACE_TOP: pos.x += t.x; pos.y += t.y - child->desired.height; break;
					case K_PLACE_LEFT: pos.x += t.x - width; pos.y += t.y; break;
					case K_PLACE_RIGHT: pos.x += t.x + target->rect.width; pos.y += t.y; break;
					default: pos.x += t.x; pos.y += t.y + target->rect.height; break;
				}
				/* flip to the other side when it would leave the screen */
				if (popup->placement == K_PLACE_BOTTOM && pos.y + child->desired.height > lh && t.y - child->desired.height >= 0) pos.y = t.y - child->desired.height;
				if (popup->placement == K_PLACE_TOP && pos.y < 0 && t.y + target->rect.height + child->desired.height <= lh) pos.y = t.y + target->rect.height;
			}
		}
		if (popup->full_width) pos.x = 0;
		pos.x = kst_clampf(pos.x, 0, kst_maxf(0, lw - width));
		pos.y = kst_clampf(pos.y, 0, kst_maxf(0, lh - child->desired.height));
		child->rect.x += pos.x - origin.x;
		child->rect.y += pos.y - origin.y;
	}
}

void Kroot_update_layout(Kid root) {
	kst_root_t* r = get_root(root);
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
			kst_measure(r->visual, kst_size(lw, lh));
			kst_arrange(r->visual, kst_rect(0, 0, lw, lh));
		}
		layout_popups(r);
		if (!r->layout_dirty) break;
	}
}


/* ============================================================================
 *  painting
 * ============================================================================ */

void kst_paint_element(kst_element_t* e, kst_painter_t* p, kst_root_t* r) {
	float tx = p->tx, ty = p->ty, alpha = p->alpha;
	float saved_xf[16];
	int saved_has_xf = p->has_xf, count, heap, i;
	kst_element_t** list;
	if (e->visibility != K_VISIBLE) return;
	/* an element is skipped when it is out of view, unless it holds things that may be in view: a zero-sized Canvas whose children overflow */
	if (kst_clipped_out(p, e->rect) && (e->clip_to_bounds || e->child_count == 0)) return;
	p->tx += e->rect.x * p->scale;
	p->ty += e->rect.y * p->scale;
	p->alpha *= e->opacity;
	if (e->transformed) {
		/* the transform of this element, about its centre of projection, under the one of its parent */
		float own[16];
		memcpy(saved_xf, p->xf, sizeof saved_xf);
		kst_xf_of_element(e, p->tx, p->ty, e->rect.width * p->scale, e->rect.height * p->scale, p->scale, own);
		kst_xf_mul(p->xf, p->xf, own);
		p->has_xf = 1;
		p->be->set_transform(p, p->xf);
	}
	if (e->clip_to_bounds) kst_push_clip(p, kst_local_rect(e));
	if (e->cls->paint) e->cls->paint(e, p);
	list = kst_ordered(e, &count, &heap);
	for (i = 0; i < count; i++) if (!list[i]->is_popup_child) kst_paint_element(list[i], p, r);
	kst_ordered_free(list, heap);
	if (e->cls->paint_over) e->cls->paint_over(e, p);
	if (e->clip_to_bounds) kst_pop_clip(p);
	kst_debug_element(e, p, r);
	if (e->transformed) {
		memcpy(p->xf, saved_xf, sizeof saved_xf);
		p->has_xf = saved_has_xf;
		p->be->set_transform(p, saved_has_xf ? p->xf : NULL);
	}
	p->tx = tx;
	p->ty = ty;
	p->alpha = alpha;
}

static void paint_tooltip(kst_root_t* r, kst_painter_t* p) {
	kst_font_t font = { K_FONT_NORMAL, KST_FONT_SMALL, 0 };
	kst_text_layout_t* l;
	float w, h, x, y;
	if (!r->tooltip_target || !r->tooltip_target->tooltip || r->tooltip_timer < 500.0f) return;
	l = kst_text_layout_new(font, r->tooltip_target->tooltip, (int)strlen(r->tooltip_target->tooltip), 300.0f, 1, 0, 0);
	if (!l) return;
	w = l->width + 16;
	h = l->height + 12;
	x = kst_clampf(r->pointer_x, 0, (float)r->width / r->ui - w);
	y = r->pointer_y + 28;
	if (y + h > (float)r->height / r->ui) y = r->pointer_y - h - 8;
	p->tx = 0; p->ty = 0; p->alpha = 1;
	kst_fill_rect(p, kst_rect(x, y, w, h), kst_theme_color(NULL, KST_TC_CHROME));
	kst_stroke_rect(p, kst_rect(x, y, w, h), Kthickness_uniform(1), r->theme.colors[KST_TC_BORDER]);
	kst_draw_text(p, l, x + 8, y + 6, l->width, K_TEXT_LEFT, K_TRIM_NONE, r->theme.colors[KST_TC_FOREGROUND]);
	kst_text_layout_free(l);
}

int kst_root_paint(kst_root_t* r, kst_painter_t* p) {
	int i;
	if (r->layout_dirty || r->first_frame) Kroot_update_layout(r->id);
	if (!(r->paint_dirty || r->first_frame)) return K_FALSE;
	kst_paint_clear(p, r->theme.has_background ? r->theme.background : r->theme.colors[KST_TC_BACKGROUND]);
	if (r->visual) kst_paint_element(r->visual, p, r);
	for (i = 0; i < r->popup_count; i++) {
		kst_element_t* popup = r->popups[i];
		KPoint origin = kst_to_root(popup, (KPoint){ 0, 0 });
		if (!popup->child_count) continue;
		if (KST_AS(kst_popup_t, popup)->modal) {
			/* ContentDialogDimmingThemeBrush over everything under the dialog */
			p->tx = 0;
			p->ty = 0;
			p->alpha = 1.0f;
			kst_fill_rect(p, kst_rect(0, 0, (float)r->width / r->ui, (float)r->height / r->ui), r->theme.colors[KST_TC_CONTENT_DIALOG_DIMMING]);
		}
		p->tx = origin.x * r->scale;
		p->ty = origin.y * r->scale;
		p->alpha = 1.0f;
		kst_paint_element(popup->children[0], p, r);
	}
	paint_tooltip(r, p);
	r->paint_dirty = 0;
	r->first_frame = 0;
	return K_TRUE;
}

static const char* g_last_renderer = "software";

void kst_note_renderer(const char* name) { g_last_renderer = name; }

/* A frame on the off-screen GPU context, read back into the pixels of the root. K_FALSE when there is none. */
static int render_gpu(kst_root_t* r, int w, int h) {
	kst_painter_t p;
	int sized = r->pixels_w == w && r->pixels_h == h && r->pixels;
	if (!r->gpu) r->gpu = kst_gl_create_offscreen(w, h);
	if (!r->gpu) return K_FALSE;
	if (!sized) {
		free(r->pixels);
		r->pixels = (uint8_t*)calloc((size_t)w * (size_t)h, 4);
		r->pixels_w = w;
		r->pixels_h = h;
		if (!r->pixels) return K_FALSE;
	}
	kst_gl_begin(r->gpu, &p, w, h, r->scale);
	kst_root_paint(r, &p);
	kst_gl_end(r->gpu, 0);
	if (!kst_gl_read(r->gpu, r->pixels, w, h)) return K_FALSE;
	g_last_renderer = "gles2";
	return K_TRUE;
}

int Kroot_render(Kid root, KFrame* frame) {
	kst_root_t* r = get_root(root);
	kst_painter_t p;
	int w, h, changed;
	if (!r) return K_FALSE;
	/* the buffer follows the density of the window, not the UI scale: the painter's scale (ui times density) fills it exactly */
	w = (int)ceilf((float)r->width * r->density);
	h = (int)ceilf((float)r->height * r->density);
	if (r->layout_dirty || r->first_frame) Kroot_update_layout(root);
	changed = r->paint_dirty || r->first_frame || r->pixels_w != w || r->pixels_h != h;
	if (changed) {
		if (r->renderer == K_RENDERER_GPU) {
			r->paint_dirty = 1;
			if (!render_gpu(r, w, h)) {
				kst_error(K_ERROR_IO, "no OpenGL ES 2 context for K_RENDERER_GPU");
				return K_FALSE;
			}
		} else {
			if (r->pixels_w != w || r->pixels_h != h || !r->pixels) {
				free(r->pixels);
				r->pixels = (uint8_t*)calloc((size_t)w * (size_t)h, 4);
				r->pixels_w = w;
				r->pixels_h = h;
				if (!r->pixels) {
					kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
					return K_FALSE;
				}
			}
			kst_painter_init(&p, r->pixels, w, h, r->scale);
			r->paint_dirty = 1;
			kst_root_paint(r, &p);
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

void Kroot_set_renderer(Kid root, KRenderer renderer) {
	kst_root_t* r = get_root(root);
	if (!r) return;
	r->renderer = renderer;
	r->paint_dirty = 1;
}

KRenderer Kroot_get_renderer(Kid root) {
	kst_root_t* r = get_root(root);
	return r ? r->renderer : K_RENDERER_AUTO;
}

const char* Kroot_get_renderer_name(Kid root) {
	(void)root;
	return g_last_renderer;
}


/* ============================================================================
 *  time
 * ============================================================================ */

static void tick_tree(kst_element_t* e, float dt) {
	int i;
	if (e->tweens) kst_tweens_tick(e);
	if (e->wants_tick && e->cls->tick) e->cls->tick(e, dt);
	for (i = 0; i < e->child_count; i++) tick_tree(e->children[i], dt);
}

/* KESTREL_ANIMATION_SPEED: a float multiplier for every animation (0.5 = half speed,
 * 2 = twice as fast; 0 or negative disables the speed override and leaves 1). */
static KColor kst_lerp_color(KColor a, KColor b, float t) {
	KColor c = { a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t };
	return c;
}

static void kst_theme_lerp(kst_theme_t* out, const kst_theme_t* a, const kst_theme_t* b, float t) {
	int i;
	out->theme = t >= 0.5f ? b->theme : a->theme;
	out->accent = kst_lerp_color(a->accent, b->accent, t);
	out->background = kst_lerp_color(a->background, b->background, t);
	out->has_background = b->has_background;
	for (i = 0; i < KST_TC_COUNT; i++) out->colors[i] = kst_lerp_color(a->colors[i], b->colors[i], t);
}

static float kst_animation_speed(void) {
	static int cached = 0;
	static float v = 1.0f;
	if (!cached) {
		const char* e = getenv("KESTREL_ANIMATION_SPEED");
		if (e && *e) { float f = strtof(e, NULL); if (f > 0) v = f; }
		cached = 1;
	}
	return v;
}
void Kroot_tick(Kid root, float delta_ms) {
	kst_root_t* r = get_root(root);
	float speed;
	if (!r) return;
	r->time_ms += delta_ms;
	speed = kst_animation_speed();
	/* cross-fade of the theme: the displayed theme moves from theme_from to theme_target */
	if (r->theme_transitioning) {
		r->theme_t += (delta_ms * speed) / 250.0f;
		if (r->theme_t >= 1.0f) {
			r->theme_t = 1.0f;
			r->theme_transitioning = 0;
			r->theme = r->theme_target;
		} else {
			kst_theme_lerp(&r->theme, &r->theme_from, &r->theme_target, r->theme_t);
		}
		r->paint_dirty = 1;
	}
	kst_tilt_tick(r, delta_ms * speed);
	if (r->visual) tick_tree(r->visual, delta_ms * speed);
	if (r->overlay) tick_tree(r->overlay, delta_ms * speed);
	if (r->tooltip_target && r->tooltip_target->tooltip) {
		float before = r->tooltip_timer;
		r->tooltip_timer += delta_ms;
		if (before < 500.0f && r->tooltip_timer >= 500.0f) r->paint_dirty = 1;
	}
}


/* ============================================================================
 *  input
 * ============================================================================ */

/* Bubbles an input from `e` up. Pointer positions are given in root coordinates in `in`.
 * The elements are walked by id: a handler may destroy any of them (a tap that opens another page). */
int kst_dispatch(kst_element_t* e, kst_input_t* in) {
	Kid ids[64];
	int n = kst_chain(e, ids, 64), i;
	float rx = in->x, ry = in->y;
	int pointer = in->type >= KST_IN_POINTER_DOWN && in->type <= KST_IN_WHEEL;
	for (i = 0; i < n; i++) {
		kst_element_t* a = kst_lookup(ids[i]);
		kst_input_t local = *in;
		if (!a) return 1;
		if (!a->cls->input) continue;
		if (pointer) {
			KPoint q = kst_from_root(a, (KPoint){ rx, ry });
			local.x = q.x;
			local.y = q.y;
			if (!a->enabled) continue;
		}
		if (a->cls->input(a, &local)) return 1;
	}
	return 0;
}

static kst_element_t* pick(kst_root_t* r, float x, float y) {
	int i;
	for (i = r->popup_count - 1; i >= 0; i--) {
		kst_element_t* popup = r->popups[i];
		kst_element_t* child = popup->child_count ? popup->children[0] : NULL;
		KPoint origin, q;
		kst_element_t* hit;
		if (!child) continue;
		origin = kst_to_root(popup, (KPoint){ 0, 0 });
		q.x = x - origin.x - child->rect.x;
		q.y = y - origin.y - child->rect.y;
		hit = kst_hit_test(child, q);
		if (hit) return hit;
		/* a modal popup is all there is to reach under it */
		if (KST_AS(kst_popup_t, popup)->modal) return NULL;
	}
	if (r->visual) return kst_hit_test(r->visual, (KPoint){ x - r->visual->rect.x, y - r->visual->rect.y });
	return NULL;
}

static int is_inside(const kst_element_t* e, const kst_element_t* ancestor) {
	for (; e; e = e->parent) if (e == ancestor) return 1;
	return 0;
}

void kst_popup_close(kst_element_t* popup);

/* A press outside the open popups closes the ones that do not stay open. Returns 1 when the press was on the owner of a closed popup. */
static int close_outside(kst_root_t* r, kst_element_t* target) {
	int i, swallow = 0;
	for (i = r->popup_count - 1; i >= 0; i--) {
		kst_popup_t* popup = KST_AS(kst_popup_t, r->popups[i]);
		if (popup->stays_open) continue;
		if (target && is_inside(target, &popup->base)) continue;
		if (target && popup->base.parent && is_inside(target, popup->base.parent)) swallow = 1;
		/* a tap on the element a flyout is shown at closes it: it does not open it again */
		if (target && popup->target != K_INVALID_ID && kst_lookup(popup->target) && is_inside(target, kst_lookup(popup->target))) swallow = 1;
		kst_popup_close(&popup->base);
		i = r->popup_count;
	}
	return swallow;
}

static int in_a_popup(const kst_root_t* r, const kst_element_t* e) {
	int i;
	for (i = 0; i < r->popup_count; i++) if (e && is_inside(e, r->popups[i])) return 1;
	return 0;
}

/* The Back button and Escape: the topmost thing that can be dismissed closes, and nothing else happens. */
int kst_root_dismiss_top(kst_root_t* r) {
	int i;
	for (i = r->popup_count - 1; i >= 0; i--) {
		kst_popup_t* popup = KST_AS(kst_popup_t, r->popups[i]);
		/* a modal popup (a ContentDialog) closes as if it were told to hide; a popup that stays open is the program's own */
		if (popup->stays_open && !popup->modal) continue;
		kst_popup_close(&popup->base);
		return 1;
	}
	if (r->open_bar) {
		kst_element_t* bar = r->open_bar;
		if (!KST_AS(kst_app_bar_t, bar)->is_sticky) {
			Kapp_bar_set_is_open(bar->id, K_FALSE);
			return 1;
		}
	}
	return 0;
}

static void update_hover(kst_root_t* r, kst_element_t* target, float x, float y) {
	Kid old_ids[64], new_ids[64];
	int old_n, new_n, i, j;
	kst_input_t in;
	if (target == r->hover) return;
	memset(&in, 0, sizeof in);
	in.x = x;
	in.y = y;
	old_n = r->hover ? kst_chain(r->hover, old_ids, 64) : 0;
	new_n = target ? kst_chain(target, new_ids, 64) : 0;
	r->hover = target;
	r->paint_dirty = 1;
	/* the elements are walked by id: a Click-on-hover handler may destroy them */
	for (i = 0; i < old_n; i++) {
		int stays = 0;
		kst_element_t* a = kst_lookup(old_ids[i]);
		for (j = 0; j < new_n; j++) if (new_ids[j] == old_ids[i]) stays = 1;
		if (!a || stays) continue;
		a->pointer_over = 0;
		in.type = KST_IN_POINTER_LEAVE;
		if (a->cls->input) a->cls->input(a, &in);
	}
	for (i = 0; i < new_n; i++) {
		kst_element_t* a = kst_lookup(new_ids[i]);
		if (!a || a->pointer_over) continue;
		a->pointer_over = 1;
		in.type = KST_IN_POINTER_ENTER;
		if (a->cls->input) a->cls->input(a, &in);
	}
}

static kst_element_t* tooltip_owner(kst_element_t* e) {
	for (; e; e = e->parent) if (e->tooltip) return e;
	return NULL;
}

/* Lets the ancestors that observe the pointer see an event first. */
static void preview(kst_element_t* target, kst_input_type_t type, float x, float y) {
	Kid ids[64];
	int n = kst_chain(target, ids, 64), i;
	for (i = 0; i < n; i++) {
		kst_input_t in;
		KPoint q;
		kst_element_t* a = kst_lookup(ids[i]);
		if (!a) return;
		if (!a->observes_pointer || !a->enabled || !a->cls->input) continue;
		memset(&in, 0, sizeof in);
		q = kst_from_root(a, (KPoint){ x, y });
		in.type = type;
		in.x = q.x;
		in.y = q.y;
		a->cls->input(a, &in);
	}
}

/* The target again after something that can run user code: NULL if it is gone. */
#define RELOOK(id) ((id) != K_INVALID_ID ? kst_lookup(id) : NULL)

int Kroot_inject_pointer(Kid root, KPointerEvent event, float x, float y) {
	kst_root_t* r = get_root(root);
	kst_element_t* target;
	Kid tid;
	kst_input_t in;
	int handled = 0;
	if (!r) return K_FALSE;
	if (r->layout_dirty) Kroot_update_layout(root);
	/* the window gives the pointer in its logical pixels; the layout thinks in the grid of the UI scale */
	x /= r->ui;
	y /= r->ui;
	r->pointer_x = x;
	r->pointer_y = y;
	memset(&in, 0, sizeof in);
	in.x = x;
	in.y = y;
	if (event == K_POINTER_LEAVE) {
		update_hover(r, NULL, x, y);
		return K_FALSE;
	}
	target = r->capture ? r->capture : pick(r, x, y);
	tid = target ? target->id : K_INVALID_ID;
	if (event == K_POINTER_MOVE) {
		kst_element_t* hit = r->capture ? pick(r, x, y) : target;
		kst_element_t* owner = tooltip_owner(hit);
		update_hover(r, hit, x, y);
		if (!kst_root_lookup(root)) return K_FALSE;
		if (owner != r->tooltip_target) {
			if (r->tooltip_timer >= 500.0f) r->paint_dirty = 1;
			r->tooltip_target = owner && kst_lookup(owner->id) ? owner : NULL;
			r->tooltip_timer = 0;
		}
		kst_tilt_move(r, x, y);
		target = RELOOK(tid);
		if (target) preview(target, KST_IN_PREVIEW_MOVE, x, y);
		if (r->capture) target = r->capture;
		else target = RELOOK(tid);
		in.type = KST_IN_POINTER_MOVE;
		if (target) handled = kst_dispatch(target, &in);
		return handled;
	}
	if (event == K_POINTER_DOWN) {
		kst_element_t* f;
		r->tooltip_timer = 0;
		r->tooltip_target = NULL;
		if (!r->capture && r->open_bar && !KST_AS(kst_app_bar_t, r->open_bar)->is_sticky && !is_inside(target, r->open_bar) && !in_a_popup(r, target)) {
			/* light dismiss: the press that closes the bar is not given to what is under it */
			Kapp_bar_set_is_open(r->open_bar->id, K_FALSE);
			close_outside(r, target);
			return K_TRUE;
		}
		if (!r->capture && close_outside(r, target)) return K_TRUE;
		target = RELOOK(tid);
		if (tid != K_INVALID_ID && !target) return K_TRUE;
		update_hover(r, target, x, y);
		target = RELOOK(tid);
		if (tid != K_INVALID_ID && !target) return K_TRUE;
		for (f = target; f; f = f->parent) {
			if (f->focusable && kst_effectively_enabled(f)) {
				kst_set_focus(r, f);
				break;
			}
		}
		target = RELOOK(tid);
		if (tid != K_INVALID_ID && !target) return K_TRUE;
		if (target) preview(target, KST_IN_PREVIEW_DOWN, x, y);
		target = RELOOK(tid);
		in.type = KST_IN_POINTER_DOWN;
		{
			kst_element_t* f2;
			for (f2 = target; f2; f2 = f2->parent) {
				if (f2->tilt && kst_effectively_enabled(f2)) {
					kst_tilt_press(r, f2, x, y);
					break;
				}
			}
		}
		if (target) handled = kst_dispatch(target, &in);
		return handled;
	}
	kst_tilt_release(r);
	if (target) preview(target, KST_IN_PREVIEW_UP, x, y);
	target = r->capture ? r->capture : RELOOK(tid);
	in.type = KST_IN_POINTER_UP;
	if (target) handled = kst_dispatch(target, &in);
	return handled;
}

int Kroot_inject_wheel(Kid root, float delta) {
	kst_root_t* r = get_root(root);
	kst_element_t* target;
	kst_input_t in;
	if (!r) return K_FALSE;
	target = pick(r, r->pointer_x, r->pointer_y);
	if (!target) return K_FALSE;
	memset(&in, 0, sizeof in);
	in.type = KST_IN_WHEEL;
	in.x = r->pointer_x;
	in.y = r->pointer_y;
	in.delta = delta;
	return kst_dispatch(target, &in);
}

int Kroot_back(Kid root) {
	kst_root_t* r = get_root(root);
	if (r && kst_root_dismiss_top(r)) return K_TRUE;
	if (!r || !r->visual || !kst_is_a(r->visual, K_CLASS_FRAME)) return K_FALSE;
	return Kframe_go_back(r->visual->id);
}

int Kroot_inject_key(Kid root, KKey key, int modifiers, int pressed) {
	kst_root_t* r = get_root(root);
	kst_input_t in;
	int handled = 0;
	if (!r) return K_FALSE;
	memset(&in, 0, sizeof in);
	in.type = pressed ? KST_IN_KEY_DOWN : KST_IN_KEY_UP;
	in.key = key;
	in.mods = modifiers;
	if (r->focus) handled = kst_dispatch(r->focus, &in);
	if (!handled && pressed && key == K_KEY_TAB) {
		Kroot_move_focus(root, !(modifiers & K_MOD_SHIFT));
		handled = 1;
	}
	if (!handled && pressed && key == K_KEY_ESCAPE) handled = Kroot_back(root);
	return handled;
}

int Kroot_inject_text(Kid root, const char* text) {
	kst_root_t* r = get_root(root);
	kst_input_t in;
	if (!r || !r->focus || !text) return K_FALSE;
	memset(&in, 0, sizeof in);
	in.type = KST_IN_TEXT;
	in.text = text;
	return kst_dispatch(r->focus, &in);
}
