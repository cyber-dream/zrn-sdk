/**
 * zrnui - controls/flip.c
 * FlipView and SemanticZoom.
 *
 * FlipView is the Pivot of the items controls: the items are pages as wide and as high as the view,
 * side by side, and a finger drags them; the drag, the release and the 300 ms slide are the pivot's
 * (pivot.c). The template has a Border with Margin 2.5 around the ScrollViewer, so the pages are 5 px
 * smaller than the view. The pages are put where they are by layout, not by a render transform.
 */

#include "../internal.h"

#define IC(e) ZST_AS(zrn_items_control_t, e)
#define FV(e) ZST_AS(zrn_flip_view_t, e)

static constexpr float FLIP_MARGIN = 2.5f;
static constexpr float SLIDE_MS = 300.0f;
static constexpr float DRAG_START = 12.0f;

static float clampf(const float v, const float lo, const float hi) { return v < lo ? lo : v > hi ? hi : v; }

static ZRect flip_viewport(const zrn_element_t* const e) {
	return zrn_rect(FLIP_MARGIN, FLIP_MARGIN, zrn_maxf(0, e->rect.width - 2 * FLIP_MARGIN), zrn_maxf(0, e->rect.height - 2 * FLIP_MARGIN));
}

static ZSize flip_measure(zrn_element_t* const e, const ZSize available) {
	const auto o = IC(e);
	float w = available.width < Z_INFINITE * 0.5f ? available.width : 0, h = available.height < Z_INFINITE * 0.5f ? available.height : 0;
	int i, n = zrn_ic_count(o);
	const float vw = zrn_maxf(0, w - 2 * FLIP_MARGIN), vh = zrn_maxf(0, h - 2 * FLIP_MARGIN);
	for (i = 0; i < n; i++) {
		zrn_element_t* item = zrn_ic_item(o, i);
		zrn_measure(item, zrn_size(available.width < Z_INFINITE * 0.5f ? vw : Z_INFINITE, available.height < Z_INFINITE * 0.5f ? vh : Z_INFINITE));
		if (available.width >= Z_INFINITE * 0.5f) w = zrn_maxf(w, item->desired.width + 2 * FLIP_MARGIN);
		if (available.height >= Z_INFINITE * 0.5f) h = zrn_maxf(h, item->desired.height + 2 * FLIP_MARGIN);
	}
	return zrn_size(w, h);
}

static ZSize flip_arrange(zrn_element_t* const e, const ZSize final) {
	const auto f = FV(e);
	zrn_items_control_t* o = &f->base;
	const ZRect view = flip_viewport(e);
	int i, n = zrn_ic_count(o);
	if (!o->host) return final;
	/* the host is the viewport, clipped; its items are laid out by hand, one page apart */
	zrn_arrange(o->host, view);
	o->host->clip_to_bounds = 1;
	for (i = 0; i < n; i++) zrn_arrange(zrn_ic_item(o, i), zrn_rect(((float)i - f->offset) * view.width, 0, view.width, view.height));
	return final;
}

static void flip_slide_to(zrn_flip_view_t* const f, const int index, const int animate) {
	zrn_element_t* const e = &f->base.base.base;
	f->from = f->offset;
	f->to = (float)index;
	f->elapsed = 0;
	if (animate && e->root && zrn_animations_enabled(e->root) && f->offset != f->to) {
		f->sliding = 1;
		e->wants_tick = 1;
	} else {
		f->sliding = 0;
		f->offset = f->to;
	}
	zrn_changed_arrange(e);
}

/* The selection changed (by the program or by the finger): the pages go to it. */
void zrn_flip_selected(zrn_items_control_t* const o) {
	const auto f = (zrn_flip_view_t*)o;
	if (o->selected_index < 0) return;
	flip_slide_to(f, o->selected_index, f->user_nav || f->touch_animations_for_all);
}

/* Items came or went: there is always a selected item when there are any. */
void zrn_flip_items_changed(zrn_items_control_t* const o) {
	const auto f = (zrn_flip_view_t*)o;
	int n = zrn_ic_count(o), i, want;
	if (n == 0) {
		o->selected_index = -1;
		f->offset = f->to = f->from = 0;
		f->sliding = 0;
		return;
	}
	want = o->selected_index < 0 ? 0 : zrn_mini(o->selected_index, n - 1);
	for (i = 0; i < n; i++) zrn_item_set_selected(zrn_ic_item(o, i), i == want);
	o->selected_index = want;
	f->offset = f->to = (float)want;
	f->sliding = 0;
	zrn_changed_layout(&o->base.base);
}

static void flip_tick(zrn_element_t* const e, const float dt) {
	const auto f = FV(e);
	if (!f->sliding) {
		if (!f->dragging) e->wants_tick = 0;
		return;
	}
	f->elapsed += dt;
	if (f->elapsed >= SLIDE_MS) {
		f->offset = f->to;
		f->sliding = 0;
		if (!f->dragging) e->wants_tick = 0;
	} else {
		f->offset = f->from + (f->to - f->from) * zrn_ease(Z_EASE_OUT, f->elapsed / SLIDE_MS);
	}
	zrn_changed_arrange(e);
}

/* To an item by the user: the slide always runs. */
static void flip_go(zrn_flip_view_t* f, int index) {
	zrn_items_control_t* const o = &f->base;
	const Zid id = o->base.base.id;
	const int n = zrn_ic_count(o);
	if (n == 0) return;
	index = zrn_mini(zrn_maxi(index, 0), n - 1);
	f->user_nav = 1;
	if (index != o->selected_index) zrn_ic_select(o, index);
	if (!zrn_lookup(id)) return;
	f->user_nav = 0;
	flip_slide_to(f, o->selected_index, 1);
}

/* The finger lets go: on the next page when it went a fifth of a page or flicked, else back. */
static void flip_release(zrn_flip_view_t* const f, const float width) {
	const zrn_items_control_t* const o = &f->base;
	const float dx = (f->offset - (float)o->selected_index) * -width;
	int target = o->selected_index;
	if (dx < -width * 0.2f || f->velocity < -0.4f) target = o->selected_index + 1;
	else if (dx > width * 0.2f || f->velocity > 0.4f) target = o->selected_index - 1;
	flip_go(f, target);
}

static int flip_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto f = FV(e);
	const int n = zrn_ic_count(&f->base);
	const float width = e->rect.width - 2 * FLIP_MARGIN > 1 ? e->rect.width - 2 * FLIP_MARGIN : 1;
	switch (in->type) {
	case ZST_IN_PREVIEW_DOWN:
		f->pressed = 1;
		f->dragging = 0;
		f->press_x = f->last_x = in->x;
		f->press_y = in->y;
		f->velocity = 0;
		return 0;
	case ZST_IN_PREVIEW_MOVE:
		if (!f->pressed || f->dragging || !e->root || e->root->capture == e || n < 2) return 0;
		{
			const float dx = in->x - f->press_x, dy = in->y - f->press_y;
			if (fabsf(dx) > DRAG_START && fabsf(dx) > fabsf(dy)) {
				/* a horizontal drag is the view's, whoever had the pointer loses it */
				f->dragging = 1;
				f->sliding = 0;
				zrn_set_capture(e->root, e);
				e->wants_tick = 1;
			}
		}
		return 0;
	case ZST_IN_PREVIEW_UP:
		return 0;
	case ZST_IN_POINTER_DOWN:
		f->pressed = 1;
		f->dragging = 0;
		f->press_x = f->last_x = in->x;
		f->press_y = in->y;
		f->velocity = 0;
		if (e->root) zrn_set_capture(e->root, e);
		return 1;
	case ZST_IN_POINTER_MOVE:
		if (!f->pressed) return 0;
		if (!f->dragging) {
			if (fabsf(in->x - f->press_x) <= DRAG_START || n < 2) return 1;
			/* the move came up from an item that has the pointer: the drag is the view's from here, with the pointer */
			f->dragging = 1;
			f->sliding = 0;
			if (e->root) zrn_set_capture(e->root, e);
			e->wants_tick = 1;
		}
		/* a quarter of a page of rubber at the ends */
		f->offset = clampf((float)f->base.selected_index - (in->x - f->press_x) / width, -0.25f, (float)n - 0.75f);
		f->velocity = f->velocity * 0.5f + ((in->x - f->last_x) / 16.0f) * 0.5f;
		f->last_x = in->x;
		zrn_changed_arrange(e);
		return 1;
	case ZST_IN_POINTER_UP: {
		const int was_dragging = f->dragging;
		f->pressed = 0;
		f->dragging = 0;
		if (e->root) zrn_release_capture(e->root, e);
		if (was_dragging) flip_release(f, width);
		return 1;
	}
	case ZST_IN_CAPTURE_LOST:
		if (f->dragging) {
			f->dragging = 0;
			f->pressed = 0;
			flip_release(f, width);
		}
		return 0;
	case ZST_IN_KEY_DOWN:
		if (in->key == Z_KEY_RIGHT || in->key == Z_KEY_DOWN || in->key == Z_KEY_PAGE_DOWN) { flip_go(f, f->base.selected_index + 1); return 1; }
		if (in->key == Z_KEY_LEFT || in->key == Z_KEY_UP || in->key == Z_KEY_PAGE_UP) { flip_go(f, f->base.selected_index - 1); return 1; }
		if (in->key == Z_KEY_HOME) { flip_go(f, 0); return 1; }
		if (in->key == Z_KEY_END) { flip_go(f, n - 1); return 1; }
		return 0;
	default:
		return 0;
	}
}

static void flip_init(zrn_element_t* const e) {
	const auto f = FV(e);
	zrn_items_control_t* const o = &f->base;
	zrn_element_t* const host = zrn_create(Z_CLASS_STACK_PANEL);
	/* FlipView: IsTabStop False; a touch gives it the keyboard */
	e->focusable = 1;
	e->tab_stop = 0;
	e->observes_pointer = 1;
	e->clip_to_bounds = 1;
	f->touch_animations_for_all = 1;
	if (o->host) zrn_destroy(o->host);
	o->host = nullptr;
	if (!host) return;
	Zstack_panel_set_orientation(host->id, Z_HORIZONTAL);
	zrn_add_child(e, host, -1);
	o->host = host;
}

zrn_class_t zrn_class_flip_view = {
	.base = &zrn_class_selector, .name = "FlipView", .id = Z_CLASS_FLIP_VIEW, .size = sizeof(zrn_flip_view_t),
	.init = flip_init, .measure = flip_measure, .arrange = flip_arrange, .input = flip_input, .tick = flip_tick
};

Zid Zflip_view_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_FLIP_VIEW);
	return e ? e->id : Z_INVALID_ID;
}

void Zflip_view_set_use_touch_animations_for_all_navigation(const Zid id, const int use) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_FLIP_VIEW);
	if (e) FV(e)->touch_animations_for_all = use != 0;
}

int Zflip_view_get_use_touch_animations_for_all_navigation(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_FLIP_VIEW);
	return e ? FV(e)->touch_animations_for_all : 0;
}


/* ============================================================================
 *  SemanticZoom
 * ============================================================================ */

typedef struct zrn_semantic_zoom {
	zrn_control_t base;
	zrn_element_t *zoomed_in, *zoomed_out;
	int in_active, can_change, button_enabled;
	ZSemanticZoomCallback started, completed;
	void *started_data, *completed_data;
} zrn_semantic_zoom_t;

#define SZ(e) ZST_AS(zrn_semantic_zoom_t, e)

static zrn_element_t* sz_active(const zrn_semantic_zoom_t* z) { return z->in_active ? z->zoomed_in : z->zoomed_out; }

static void sz_apply(zrn_semantic_zoom_t* const z) {
	if (z->zoomed_in) z->zoomed_in->visibility = z->in_active ? Z_VISIBLE : Z_COLLAPSED;
	if (z->zoomed_out) z->zoomed_out->visibility = z->in_active ? Z_COLLAPSED : Z_VISIBLE;
	zrn_changed_layout(&z->base.base);
}

static ZSize sz_measure(zrn_element_t* const e, const ZSize available) {
	zrn_element_t* const view = sz_active(SZ(e));
	ZSize s = { 0, 0 };
	if (view) {
		zrn_measure(view, available);
		s = view->desired;
	}
	return s;
}

static ZSize sz_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_semantic_zoom_t* const z = SZ(e);
	if (sz_active(z)) zrn_arrange(sz_active(z), zrn_rect(0, 0, final.width, final.height));
	return final;
}

static void sz_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	const auto z = SZ(e);
	if (z->zoomed_in == child) z->zoomed_in = nullptr;
	if (z->zoomed_out == child) z->zoomed_out = nullptr;
}

static void sz_init(zrn_element_t* const e) {
	const auto z = SZ(e);
	z->in_active = 1;
	z->can_change = 1;
	z->button_enabled = 1;
	e->focusable = 0;
}

zrn_class_t zrn_class_semantic_zoom = {
	.base = &zrn_class_control, .name = "SemanticZoom", .id = Z_CLASS_SEMANTIC_ZOOM, .size = sizeof(zrn_semantic_zoom_t),
	.init = sz_init, .measure = sz_measure, .arrange = sz_arrange, .child_removed = sz_child_removed
};

Zid Zsemantic_zoom_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_SEMANTIC_ZOOM);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_semantic_zoom_t* get_sz(const Zid id) { return SZ(zrn_get(id, Z_CLASS_SEMANTIC_ZOOM)); }

static void sz_set_view(zrn_semantic_zoom_t* const z, zrn_element_t** slot, const Zid view) {
	zrn_element_t* const e = &z->base.base;
	zrn_element_t* const v = view != Z_INVALID_ID ? zrn_get(view, Z_CLASS_ELEMENT) : nullptr;
	zrn_element_t* const old = *slot;
	if (view != Z_INVALID_ID && !v) return;
	if (old == v) return;
	*slot = nullptr;
	if (old) zrn_destroy(old);
	if (v) {
		zrn_add_child(e, v, -1);
		if (v->parent == e) *slot = v;
	}
	sz_apply(z);
}

void Zsemantic_zoom_set_zoomed_in_view(const Zid id, const Zid view) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) sz_set_view(z, &z->zoomed_in, view);
}

void Zsemantic_zoom_set_zoomed_out_view(const Zid id, const Zid view) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) sz_set_view(z, &z->zoomed_out, view);
}

static void sz_change(zrn_semantic_zoom_t* z, int zoomed_in_active) {
	const zrn_element_t* const e = &z->base.base;
	const Zid id = e->id;
	const int source = z->in_active;
	zoomed_in_active = zoomed_in_active != 0;
	if (!z->can_change || z->in_active == zoomed_in_active) return;
	if (z->started) {
		z->started(id, source, z->started_data);
		if (!zrn_lookup(id)) return;
	}
	z->in_active = zoomed_in_active;
	sz_apply(z);
	if (z->completed) z->completed(id, source, z->completed_data);
}

void Zsemantic_zoom_set_is_zoomed_in_view_active(const Zid id, const int active) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) sz_change(z, active);
}

int Zsemantic_zoom_get_is_zoomed_in_view_active(const Zid id) {
	const zrn_semantic_zoom_t* const z = get_sz(id);
	return z ? z->in_active : 0;
}

void Zsemantic_zoom_toggle_active_view(const Zid id) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) sz_change(z, !z->in_active);
}

void Zsemantic_zoom_set_can_change_views(const Zid id, const int can_change) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) z->can_change = can_change != 0;
}

void Zsemantic_zoom_set_is_zoom_out_button_enabled(const Zid id, const int enabled) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (z) z->button_enabled = enabled != 0;
}

void Zsemantic_zoom_on_view_change_started(const Zid id, const ZSemanticZoomCallback callback, void* const user_data) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (!z) return;
	z->started = callback;
	z->started_data = user_data;
}

void Zsemantic_zoom_on_view_change_completed(const Zid id, const ZSemanticZoomCallback callback, void* const user_data) {
	zrn_semantic_zoom_t* const z = get_sz(id);
	if (!z) return;
	z->completed = callback;
	z->completed_data = user_data;
}
