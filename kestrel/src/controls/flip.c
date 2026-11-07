/**
 * Kestrel - controls/flip.c
 * FlipView and SemanticZoom.
 *
 * FlipView is the Pivot of the items controls: the items are pages as wide and as high as the view,
 * side by side, and a finger drags them; the drag, the release and the 300 ms slide are the pivot's
 * (pivot.c). The template has a Border with Margin 2.5 around the ScrollViewer, so the pages are 5 px
 * smaller than the view. The pages are put where they are by layout, not by a render transform.
 */

#include "../internal.h"

#define IC(e) KST_AS(kst_items_control_t, e)
#define FV(e) KST_AS(kst_flip_view_t, e)

#define FLIP_MARGIN 2.5f
#define SLIDE_MS 300.0f
#define DRAG_START 12.0f

static float clampf(float v, float lo, float hi) { return v < lo ? lo : v > hi ? hi : v; }

static KRect flip_viewport(const kst_element_t* e) {
	return kst_rect(FLIP_MARGIN, FLIP_MARGIN, kst_maxf(0, e->rect.width - 2 * FLIP_MARGIN), kst_maxf(0, e->rect.height - 2 * FLIP_MARGIN));
}

static KSize flip_measure(kst_element_t* e, KSize available) {
	kst_items_control_t* o = IC(e);
	float w = available.width < K_INFINITE * 0.5f ? available.width : 0, h = available.height < K_INFINITE * 0.5f ? available.height : 0;
	int i, n = kst_ic_count(o);
	float vw = kst_maxf(0, w - 2 * FLIP_MARGIN), vh = kst_maxf(0, h - 2 * FLIP_MARGIN);
	for (i = 0; i < n; i++) {
		kst_element_t* item = kst_ic_item(o, i);
		kst_measure(item, kst_size(available.width < K_INFINITE * 0.5f ? vw : K_INFINITE, available.height < K_INFINITE * 0.5f ? vh : K_INFINITE));
		if (available.width >= K_INFINITE * 0.5f) w = kst_maxf(w, item->desired.width + 2 * FLIP_MARGIN);
		if (available.height >= K_INFINITE * 0.5f) h = kst_maxf(h, item->desired.height + 2 * FLIP_MARGIN);
	}
	return kst_size(w, h);
}

static KSize flip_arrange(kst_element_t* e, KSize final) {
	kst_flip_view_t* f = FV(e);
	kst_items_control_t* o = &f->base;
	KRect view = flip_viewport(e);
	int i, n = kst_ic_count(o);
	if (!o->host) return final;
	/* the host is the viewport, clipped; its items are laid out by hand, one page apart */
	kst_arrange(o->host, view);
	o->host->clip_to_bounds = 1;
	for (i = 0; i < n; i++) kst_arrange(kst_ic_item(o, i), kst_rect(((float)i - f->offset) * view.width, 0, view.width, view.height));
	return final;
}

static void flip_slide_to(kst_flip_view_t* f, int index, int animate) {
	kst_element_t* e = &f->base.base.base;
	f->from = f->offset;
	f->to = (float)index;
	f->elapsed = 0;
	if (animate && e->root && kst_animations_enabled(e->root) && f->offset != f->to) {
		f->sliding = 1;
		e->wants_tick = 1;
	} else {
		f->sliding = 0;
		f->offset = f->to;
	}
	kst_changed_arrange(e);
}

/* The selection changed (by the program or by the finger): the pages go to it. */
void kst_flip_selected(kst_items_control_t* o) {
	kst_flip_view_t* f = (kst_flip_view_t*)o;
	if (o->selected_index < 0) return;
	flip_slide_to(f, o->selected_index, f->user_nav || f->touch_animations_for_all);
}

/* Items came or went: there is always a selected item when there are any. */
void kst_flip_items_changed(kst_items_control_t* o) {
	kst_flip_view_t* f = (kst_flip_view_t*)o;
	int n = kst_ic_count(o), i, want;
	if (n == 0) {
		o->selected_index = -1;
		f->offset = f->to = f->from = 0;
		f->sliding = 0;
		return;
	}
	want = o->selected_index < 0 ? 0 : kst_mini(o->selected_index, n - 1);
	for (i = 0; i < n; i++) kst_item_set_selected(kst_ic_item(o, i), i == want);
	o->selected_index = want;
	f->offset = f->to = (float)want;
	f->sliding = 0;
	kst_changed_layout(&o->base.base);
}

static void flip_tick(kst_element_t* e, float dt) {
	kst_flip_view_t* f = FV(e);
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
		f->offset = f->from + (f->to - f->from) * kst_ease(K_EASE_OUT, f->elapsed / SLIDE_MS);
	}
	kst_changed_arrange(e);
}

/* To an item by the user: the slide always runs. */
static void flip_go(kst_flip_view_t* f, int index) {
	kst_items_control_t* o = &f->base;
	Kid id = o->base.base.id;
	int n = kst_ic_count(o);
	if (n == 0) return;
	index = kst_mini(kst_maxi(index, 0), n - 1);
	f->user_nav = 1;
	if (index != o->selected_index) kst_ic_select(o, index);
	if (!kst_lookup(id)) return;
	f->user_nav = 0;
	flip_slide_to(f, o->selected_index, 1);
}

/* The finger lets go: on the next page when it went a fifth of a page or flicked, else back. */
static void flip_release(kst_flip_view_t* f, float width) {
	kst_items_control_t* o = &f->base;
	float dx = (f->offset - (float)o->selected_index) * -width;
	int target = o->selected_index;
	if (dx < -width * 0.2f || f->velocity < -0.4f) target = o->selected_index + 1;
	else if (dx > width * 0.2f || f->velocity > 0.4f) target = o->selected_index - 1;
	flip_go(f, target);
}

static int flip_input(kst_element_t* e, kst_input_t* in) {
	kst_flip_view_t* f = FV(e);
	int n = kst_ic_count(&f->base);
	float width = e->rect.width - 2 * FLIP_MARGIN > 1 ? e->rect.width - 2 * FLIP_MARGIN : 1;
	switch (in->type) {
	case KST_IN_PREVIEW_DOWN:
		f->pressed = 1;
		f->dragging = 0;
		f->press_x = f->last_x = in->x;
		f->press_y = in->y;
		f->velocity = 0;
		return 0;
	case KST_IN_PREVIEW_MOVE:
		if (!f->pressed || f->dragging || !e->root || e->root->capture == e || n < 2) return 0;
		{
			float dx = in->x - f->press_x, dy = in->y - f->press_y;
			if (fabsf(dx) > DRAG_START && fabsf(dx) > fabsf(dy)) {
				/* a horizontal drag is the view's, whoever had the pointer loses it */
				f->dragging = 1;
				f->sliding = 0;
				kst_set_capture(e->root, e);
				e->wants_tick = 1;
			}
		}
		return 0;
	case KST_IN_PREVIEW_UP:
		return 0;
	case KST_IN_POINTER_DOWN:
		f->pressed = 1;
		f->dragging = 0;
		f->press_x = f->last_x = in->x;
		f->press_y = in->y;
		f->velocity = 0;
		if (e->root) kst_set_capture(e->root, e);
		return 1;
	case KST_IN_POINTER_MOVE:
		if (!f->pressed) return 0;
		if (!f->dragging) {
			if (fabsf(in->x - f->press_x) <= DRAG_START || n < 2) return 1;
			/* the move came up from an item that has the pointer: the drag is the view's from here, with the pointer */
			f->dragging = 1;
			f->sliding = 0;
			if (e->root) kst_set_capture(e->root, e);
			e->wants_tick = 1;
		}
		/* a quarter of a page of rubber at the ends */
		f->offset = clampf((float)f->base.selected_index - (in->x - f->press_x) / width, -0.25f, (float)n - 0.75f);
		f->velocity = f->velocity * 0.5f + ((in->x - f->last_x) / 16.0f) * 0.5f;
		f->last_x = in->x;
		kst_changed_arrange(e);
		return 1;
	case KST_IN_POINTER_UP: {
		int was_dragging = f->dragging;
		f->pressed = 0;
		f->dragging = 0;
		if (e->root) kst_release_capture(e->root, e);
		if (was_dragging) flip_release(f, width);
		return 1;
	}
	case KST_IN_CAPTURE_LOST:
		if (f->dragging) {
			f->dragging = 0;
			f->pressed = 0;
			flip_release(f, width);
		}
		return 0;
	case KST_IN_KEY_DOWN:
		if (in->key == K_KEY_RIGHT || in->key == K_KEY_DOWN || in->key == K_KEY_PAGE_DOWN) { flip_go(f, f->base.selected_index + 1); return 1; }
		if (in->key == K_KEY_LEFT || in->key == K_KEY_UP || in->key == K_KEY_PAGE_UP) { flip_go(f, f->base.selected_index - 1); return 1; }
		if (in->key == K_KEY_HOME) { flip_go(f, 0); return 1; }
		if (in->key == K_KEY_END) { flip_go(f, n - 1); return 1; }
		return 0;
	default:
		return 0;
	}
}

static void flip_init(kst_element_t* e) {
	kst_flip_view_t* f = FV(e);
	kst_items_control_t* o = &f->base;
	kst_element_t* host = kst_create(K_CLASS_STACK_PANEL);
	/* FlipView: IsTabStop False; a touch gives it the keyboard */
	e->focusable = 1;
	e->tab_stop = 0;
	e->observes_pointer = 1;
	e->clip_to_bounds = 1;
	f->touch_animations_for_all = 1;
	if (o->host) kst_destroy(o->host);
	o->host = NULL;
	if (!host) return;
	Kstack_panel_set_orientation(host->id, K_HORIZONTAL);
	kst_add_child(e, host, -1);
	o->host = host;
}

kst_class_t kst_class_flip_view = {
	.base = &kst_class_selector, .name = "FlipView", .id = K_CLASS_FLIP_VIEW, .size = sizeof(kst_flip_view_t),
	.init = flip_init, .measure = flip_measure, .arrange = flip_arrange, .input = flip_input, .tick = flip_tick
};

Kid Kflip_view_create(void) {
	kst_element_t* e = kst_create(K_CLASS_FLIP_VIEW);
	return e ? e->id : K_INVALID_ID;
}

void Kflip_view_set_use_touch_animations_for_all_navigation(Kid id, int use) {
	kst_element_t* e = kst_get(id, K_CLASS_FLIP_VIEW);
	if (e) FV(e)->touch_animations_for_all = use != 0;
}

int Kflip_view_get_use_touch_animations_for_all_navigation(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_FLIP_VIEW);
	return e ? FV(e)->touch_animations_for_all : 0;
}


/* ============================================================================
 *  SemanticZoom
 * ============================================================================ */

typedef struct kst_semantic_zoom {
	kst_control_t base;
	kst_element_t *zoomed_in, *zoomed_out;
	int in_active, can_change, button_enabled;
	KSemanticZoomCallback started, completed;
	void *started_data, *completed_data;
} kst_semantic_zoom_t;

#define SZ(e) KST_AS(kst_semantic_zoom_t, e)

static kst_element_t* sz_active(const kst_semantic_zoom_t* z) { return z->in_active ? z->zoomed_in : z->zoomed_out; }

static void sz_apply(kst_semantic_zoom_t* z) {
	if (z->zoomed_in) z->zoomed_in->visibility = z->in_active ? K_VISIBLE : K_COLLAPSED;
	if (z->zoomed_out) z->zoomed_out->visibility = z->in_active ? K_COLLAPSED : K_VISIBLE;
	kst_changed_layout(&z->base.base);
}

static KSize sz_measure(kst_element_t* e, KSize available) {
	kst_element_t* view = sz_active(SZ(e));
	KSize s = { 0, 0 };
	if (view) {
		kst_measure(view, available);
		s = view->desired;
	}
	return s;
}

static KSize sz_arrange(kst_element_t* e, KSize final) {
	kst_semantic_zoom_t* z = SZ(e);
	if (sz_active(z)) kst_arrange(sz_active(z), kst_rect(0, 0, final.width, final.height));
	return final;
}

static void sz_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_semantic_zoom_t* z = SZ(e);
	if (z->zoomed_in == child) z->zoomed_in = NULL;
	if (z->zoomed_out == child) z->zoomed_out = NULL;
}

static void sz_init(kst_element_t* e) {
	kst_semantic_zoom_t* z = SZ(e);
	z->in_active = 1;
	z->can_change = 1;
	z->button_enabled = 1;
	e->focusable = 0;
}

kst_class_t kst_class_semantic_zoom = {
	.base = &kst_class_control, .name = "SemanticZoom", .id = K_CLASS_SEMANTIC_ZOOM, .size = sizeof(kst_semantic_zoom_t),
	.init = sz_init, .measure = sz_measure, .arrange = sz_arrange, .child_removed = sz_child_removed
};

Kid Ksemantic_zoom_create(void) {
	kst_element_t* e = kst_create(K_CLASS_SEMANTIC_ZOOM);
	return e ? e->id : K_INVALID_ID;
}

static kst_semantic_zoom_t* get_sz(Kid id) { return SZ(kst_get(id, K_CLASS_SEMANTIC_ZOOM)); }

static void sz_set_view(kst_semantic_zoom_t* z, kst_element_t** slot, Kid view) {
	kst_element_t* e = &z->base.base;
	kst_element_t* v = view != K_INVALID_ID ? kst_get(view, K_CLASS_ELEMENT) : NULL;
	kst_element_t* old = *slot;
	if (view != K_INVALID_ID && !v) return;
	if (old == v) return;
	*slot = NULL;
	if (old) kst_destroy(old);
	if (v) {
		kst_add_child(e, v, -1);
		if (v->parent == e) *slot = v;
	}
	sz_apply(z);
}

void Ksemantic_zoom_set_zoomed_in_view(Kid id, Kid view) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) sz_set_view(z, &z->zoomed_in, view);
}

void Ksemantic_zoom_set_zoomed_out_view(Kid id, Kid view) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) sz_set_view(z, &z->zoomed_out, view);
}

static void sz_change(kst_semantic_zoom_t* z, int zoomed_in_active) {
	kst_element_t* e = &z->base.base;
	Kid id = e->id;
	int source = z->in_active;
	zoomed_in_active = zoomed_in_active != 0;
	if (!z->can_change || z->in_active == zoomed_in_active) return;
	if (z->started) {
		z->started(id, source, z->started_data);
		if (!kst_lookup(id)) return;
	}
	z->in_active = zoomed_in_active;
	sz_apply(z);
	if (z->completed) z->completed(id, source, z->completed_data);
}

void Ksemantic_zoom_set_is_zoomed_in_view_active(Kid id, int active) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) sz_change(z, active);
}

int Ksemantic_zoom_get_is_zoomed_in_view_active(Kid id) {
	kst_semantic_zoom_t* z = get_sz(id);
	return z ? z->in_active : 0;
}

void Ksemantic_zoom_toggle_active_view(Kid id) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) sz_change(z, !z->in_active);
}

void Ksemantic_zoom_set_can_change_views(Kid id, int can_change) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) z->can_change = can_change != 0;
}

void Ksemantic_zoom_set_is_zoom_out_button_enabled(Kid id, int enabled) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (z) z->button_enabled = enabled != 0;
}

void Ksemantic_zoom_on_view_change_started(Kid id, KSemanticZoomCallback callback, void* user_data) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (!z) return;
	z->started = callback;
	z->started_data = user_data;
}

void Ksemantic_zoom_on_view_change_completed(Kid id, KSemanticZoomCallback callback, void* user_data) {
	kst_semantic_zoom_t* z = get_sz(id);
	if (!z) return;
	z->completed = callback;
	z->completed_data = user_data;
}
