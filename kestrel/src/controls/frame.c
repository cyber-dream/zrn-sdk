/**
 * Kestrel - Frame and Page: the navigation of a Windows Phone app.
 */

#include "../internal.h"

#define PAGE(e) KST_AS(kst_page_t, e)
#define FRAME(e) KST_AS(kst_frame_t, e)


/* ============================================================================
 *  Page
 * ============================================================================ */

static void page_init(kst_element_t* e) {
	e->focusable = 0;
	KST_AS(kst_control_t, e)->h_content = K_HALIGN_STRETCH;
	KST_AS(kst_control_t, e)->v_content = K_VALIGN_STRETCH;
	PAGE(e)->navigation_mode = K_NAVIGATION_NEW;
	PAGE(e)->cache_mode = K_NAVIGATION_CACHE_DISABLED;
	PAGE(e)->entry_index = -1;
}

/* The page makes room for its app bars: what is left is the content's, and an open bar grows over the content. */
static void page_bar_room(kst_page_t* p, float* top, float* bottom) {
	*top = p->top_bar && p->top_bar->visibility == K_VISIBLE ? kst_app_bar_closed_height(p->top_bar) : 0;
	*bottom = p->bottom_bar && p->bottom_bar->visibility == K_VISIBLE ? kst_app_bar_closed_height(p->bottom_bar) : 0;
}

static KSize page_measure(kst_element_t* e, KSize available) {
	kst_page_t* p = PAGE(e);
	kst_content_control_t* cc = &p->base;
	float top, bottom;
	KSize content = { 0, 0 };
	page_bar_room(p, &top, &bottom);
	if (cc->content) {
		kst_measure(cc->content, kst_size(available.width, kst_maxf(0, available.height - top - bottom)));
		content = cc->content->desired;
	}
	if (p->top_bar) kst_measure(p->top_bar, kst_size(available.width, available.height));
	if (p->bottom_bar) kst_measure(p->bottom_bar, kst_size(available.width, available.height));
	return kst_size(content.width, content.height + top + bottom);
}

static KSize page_arrange(kst_element_t* e, KSize final) {
	kst_page_t* p = PAGE(e);
	float top, bottom;
	page_bar_room(p, &top, &bottom);
	if (p->base.content) kst_arrange(p->base.content, kst_rect(0, top, final.width, kst_maxf(0, final.height - top - bottom)));
	if (p->top_bar) kst_arrange(p->top_bar, kst_rect(0, 0, final.width, p->top_bar->desired.height));
	if (p->bottom_bar) kst_arrange(p->bottom_bar, kst_rect(0, final.height - p->bottom_bar->desired.height, final.width, p->bottom_bar->desired.height));
	return final;
}

static void page_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_page_t* p = PAGE(e);
	if (p->top_bar == child) p->top_bar = NULL;
	else if (p->bottom_bar == child) p->bottom_bar = NULL;
	else kst_cc_child_removed(e, child);
}

kst_class_t kst_class_page = {
	.base = &kst_class_user_control, .name = "Page", .id = K_CLASS_PAGE, .size = sizeof(kst_page_t),
	.init = page_init, .measure = page_measure, .arrange = page_arrange, .child_removed = page_child_removed
};

Kid Kpage_create(void) {
	kst_element_t* e = kst_create(K_CLASS_PAGE);
	return e ? e->id : K_INVALID_ID;
}

void Kpage_set_content(Kid id, Kid child) { Kuser_control_set_content(id, child); }

Kid Kpage_get_content(Kid id) { return Kuser_control_get_content(id); }

void* Kpage_get_parameter(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e ? PAGE(e)->parameter : NULL;
}

KNavigationMode Kpage_get_navigation_mode(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e ? PAGE(e)->navigation_mode : K_NAVIGATION_NEW;
}

void Kpage_set_navigation_cache_mode(Kid id, KNavigationCacheMode mode) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	if (e) PAGE(e)->cache_mode = mode;
}

KNavigationCacheMode Kpage_get_navigation_cache_mode(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e ? PAGE(e)->cache_mode : K_NAVIGATION_CACHE_DISABLED;
}

/* The bars are children of the page after its content: z-order puts them over it, and an open one grows over the content. */
static void page_set_bar(Kid page, Kid bar, int top) {
	kst_element_t* e = kst_get(page, K_CLASS_PAGE);
	kst_element_t* b = bar != K_INVALID_ID ? kst_get(bar, K_CLASS_APP_BAR) : NULL;
	kst_element_t** slot;
	if (!e || (bar != K_INVALID_ID && !b)) return;
	slot = top ? &PAGE(e)->top_bar : &PAGE(e)->bottom_bar;
	kst_set_only_child(e, slot, b);
	if (b && *slot == b) {
		b->zindex = 1;
		kst_app_bar_set_on_top(b, top);
	}
}

void Kpage_set_top_app_bar(Kid page, Kid app_bar) { page_set_bar(page, app_bar, 1); }
void Kpage_set_bottom_app_bar(Kid page, Kid app_bar) { page_set_bar(page, app_bar, 0); }

Kid Kpage_get_top_app_bar(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e && PAGE(e)->top_bar ? PAGE(e)->top_bar->id : K_INVALID_ID;
}

Kid Kpage_get_bottom_app_bar(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e && PAGE(e)->bottom_bar ? PAGE(e)->bottom_bar->id : K_INVALID_ID;
}

Kid Kpage_get_frame(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PAGE);
	return e && e->parent && kst_is_a(e->parent, K_CLASS_FRAME) ? e->parent->id : K_INVALID_ID;
}

void Kpage_on_navigated_to(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_PAGE)) Kelement_on(id, K_EVENT_NAVIGATED_TO, callback, user_data);
}

void Kpage_on_navigated_from(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_PAGE)) Kelement_on(id, K_EVENT_NAVIGATED_FROM, callback, user_data);
}


/* ============================================================================
 *  Frame
 * ============================================================================ */

/* ---- the turnstile, as ZrnMetroCore times it from the Lumia: the rows of the page that leaves turn away
 * round the left edge one after another (128 ms each, 28 ms apart), a moment of nothing, then the rows
 * of the new page swing in from behind the screen (180 ms each). Going back the order is reversed. */
#define OUT_MS 128.0f
#define IN_MS 180.0f
#define STAGGER 28.0f
#define GAP_AT 330.0f
#define MAX_ROWS 64
#define MAX_STAGGER_ROWS 7

static void frame_init(kst_element_t* e) {
	e->focusable = 0;
	FRAME(e)->current = -1;
	FRAME(e)->transition = K_TRANSITION_TURNSTILE;
	FRAME(e)->mode = K_NAVIGATION_NEW;
	KST_AS(kst_control_t, e)->h_content = K_HALIGN_STRETCH;
	KST_AS(kst_control_t, e)->v_content = K_VALIGN_STRETCH;
}

/* The pages kept alive for the entries from `first` on go with them. */
static void drop_cached(kst_frame_t* f, int first) {
	int i;
	for (i = first; i < f->count; i++) {
		kst_element_t* page = f->entries[i].cached != K_INVALID_ID ? kst_lookup(f->entries[i].cached) : NULL;
		f->entries[i].cached = K_INVALID_ID;
		if (page && !page->parent) kst_destroy(page);
	}
}

static void frame_destroy(kst_element_t* e) {
	kst_frame_t* f = FRAME(e);
	kst_element_t* pending = f->pending != K_INVALID_ID ? kst_lookup(f->pending) : NULL;
	if (pending && !pending->parent) kst_destroy(pending);
	drop_cached(f, 0);
	free(f->entries);
	free(f->rows);
	free(f->row_opacity);
	f->entries = NULL;
	f->rows = NULL;
	f->row_opacity = NULL;
}

static void frame_tick(kst_element_t* e, float dt);

/* The page starts under the status bar: the safe area is room the frame keeps, which is also where a turnstile turns. */
static KThickness frame_chrome(kst_element_t* e) {
	KST_UNUSED(e);
	return Kthickness(0, KST_SAFE_AREA_TOP, 0, 0);
}

kst_class_t kst_class_frame = {
	.base = &kst_class_content_control, .name = "Frame", .id = K_CLASS_FRAME, .size = sizeof(kst_frame_t),
	.init = frame_init, .destroy = frame_destroy, .tick = frame_tick, .chrome = frame_chrome
};

Kid Kframe_create(void) {
	kst_element_t* e = kst_create(K_CLASS_FRAME);
	return e ? e->id : K_INVALID_ID;
}

static kst_frame_t* get_frame(Kid id) { return FRAME(kst_get(id, K_CLASS_FRAME)); }

/* A row is what turns: a leaf of the tree, or a control that is not a container. */
static int is_container(const kst_element_t* e) {
	return kst_is_a(e, K_CLASS_PANEL) || kst_is_a(e, K_CLASS_BORDER) || kst_is_a(e, K_CLASS_VIEWBOX) ||
		kst_is_a(e, K_CLASS_SCROLL_VIEWER) || kst_is_a(e, K_CLASS_USER_CONTROL) || kst_is_a(e, K_CLASS_ITEMS_CONTROL);
}

static void collect_rows(kst_frame_t* f, kst_element_t* e) {
	int i;
	if (e->visibility != K_VISIBLE || f->row_count >= MAX_ROWS || kst_is_a(e, K_CLASS_APP_BAR)) return;
	if (is_container(e) && e->child_count > 0) {
		for (i = 0; i < e->child_count; i++) collect_rows(f, e->children[i]);
		return;
	}
	if (f->row_count == f->row_cap) {
		int cap = f->row_cap ? f->row_cap * 2 : 16;
		Kid* rows = (Kid*)realloc(f->rows, (size_t)cap * sizeof *rows);
		float* ops;
		if (!rows) return;
		f->rows = rows;
		ops = (float*)realloc(f->row_opacity, (size_t)cap * sizeof *ops);
		if (!ops) return;
		f->row_opacity = ops;
		f->row_cap = cap;
	}
	f->rows[f->row_count] = e->id;
	f->row_opacity[f->row_count] = e->opacity;
	f->row_count++;
}

static void rows_of(kst_frame_t* f, kst_element_t* page) {
	f->row_count = 0;
	if (page) collect_rows(f, page);
}

/* The rows back as they were. */
static void rows_reset(kst_frame_t* f) {
	int i;
	for (i = 0; i < f->row_count; i++) {
		kst_element_t* row = kst_lookup(f->rows[i]);
		if (!row) continue;
		row->rot_x = row->rot_y = 0;
		row->proj_cx = row->proj_cy = 0.5f;
		row->opacity = f->row_opacity[i];
		kst_update_transformed(row);
		kst_changed_paint(row);
	}
	f->row_count = 0;
}

static float clamp01(float v) { return v < 0 ? 0 : v > 1 ? 1 : v; }

/* One frame of the turnstile: row `i` of `n`, `t` ms into this half. */
static void rows_turn(kst_frame_t* f, kst_element_t* frame, float t, int incoming) {
	int i, n = f->row_count;
	float left = kst_to_root(frame, (KPoint){ 0, 0 }).x;
	for (i = 0; i < n; i++) {
		kst_element_t* row = kst_lookup(f->rows[i]);
		int order = f->direction >= 0 ? i : n - 1 - i;
		float u, angle, alpha;
		KPoint origin;
		if (!row) continue;
		if (order > MAX_STAGGER_ROWS) order = MAX_STAGGER_ROWS;
		/* a row not laid out yet has no width to turn about: it is only kept clear until the first tick lays it out */
		if (row->rect.width > 0) {
			origin = kst_to_root(row, (KPoint){ 0, 0 });
			row->proj_cx = (left - origin.x) / row->rect.width;
			row->proj_cy = 0.5f;
		}
		if (!incoming) {
			u = clamp01((t - (float)order * STAGGER) / OUT_MS);
			angle = (f->direction >= 0 ? -50.0f : 50.0f) * kst_ease(K_EASE_IN, u);
			alpha = u >= 1 ? 0 : 1 - u * u;
		} else {
			u = clamp01((t - (float)order * STAGGER) / IN_MS);
			angle = 80.0f * (1 - kst_ease(K_EASE_OUT, u));
			alpha = u <= 0 ? 0 : clamp01(u * 3);
		}
		row->rot_y = angle;
		row->opacity = f->row_opacity[i] * alpha;
		kst_update_transformed(row);
		kst_changed_paint(row);
	}
}

/* The page goes into the frame: its parameter, the content, NavigatedTo, Navigated. */
static void attach(kst_frame_t* f, Kid frame_id, Kid page_id, void* parameter) {
	kst_element_t* page = kst_lookup(page_id);
	f->navigating = 1;
	if (!kst_lookup(frame_id)) {
		if (page && !page->parent) kst_destroy(page);
		return;
	}
	if (!page) { f->navigating = 0; return; }
	PAGE(page)->parameter = parameter;
	PAGE(page)->navigation_mode = f->mode;
	PAGE(page)->entry_index = f->current;
	page->hit_visible = 1;
	/* a page that keeps its NavigationCacheMode stays alive, out of the frame, for the entry it was built for */
	{
		kst_element_t* old = f->base.content;
		if (old && old != page && kst_is_a(old, K_CLASS_PAGE) && PAGE(old)->cache_mode != K_NAVIGATION_CACHE_DISABLED) {
			int index = PAGE(old)->entry_index;
			if (index >= 0 && index < f->count && f->entries[index].cached == K_INVALID_ID) {
				kst_remove_child(&f->base.base.base, old);
				f->entries[index].cached = old->id;
			}
		}
	}
	kst_content_set(&f->base, page);
	if (kst_lookup(page_id)) kst_raise(kst_lookup(page_id), K_EVENT_NAVIGATED_TO);
	if (kst_lookup(frame_id)) kst_raise(kst_lookup(frame_id), K_EVENT_NAVIGATED);
	if (kst_lookup(frame_id)) f->navigating = 0;
}

/* Ends a transition at once: the new page is in, nothing turns. */
static void finish(kst_frame_t* f, Kid frame_id) {
	if (f->phase == 0) return;
	if (f->phase == 1) {
		Kid page = f->pending;
		rows_reset(f);
		f->pending = K_INVALID_ID;
		f->phase = 0;
		attach(f, frame_id, page, f->pending_parameter);
		if (!kst_lookup(frame_id)) return;
	} else {
		rows_reset(f);
	}
	f->phase = 0;
	f->elapsed = 0;
}

static void frame_tick(kst_element_t* e, float dt) {
	kst_frame_t* f = FRAME(e);
	Kid id = e->id;
	if (f->phase == 0) { e->wants_tick = 0; return; }
	f->elapsed += dt;
	if (e->root && e->root->layout_dirty) Kroot_update_layout(e->root->id);
	if (f->phase == 1) {
		if (f->elapsed < GAP_AT) {
			rows_turn(f, e, f->elapsed, 0);
			return;
		}
		/* the old page is gone, the new one comes in */
		{
			Kid page = f->pending;
			float carry = f->elapsed - GAP_AT;
			rows_reset(f);
			f->pending = K_INVALID_ID;
			f->phase = 2;
			attach(f, id, page, f->pending_parameter);
			if (!kst_lookup(id)) return;
			f = FRAME(kst_lookup(id));
			e = &f->base.base.base;
			if (e->root && e->root->layout_dirty) Kroot_update_layout(e->root->id);
			rows_of(f, f->base.content);
			f->elapsed = carry;
			rows_turn(f, e, f->elapsed, 1);
		}
		return;
	}
	if (f->elapsed >= IN_MS + (float)MAX_STAGGER_ROWS * STAGGER) {
		rows_reset(f);
		f->phase = 0;
		e->wants_tick = 0;
		return;
	}
	rows_turn(f, e, f->elapsed, 1);
}

/*
 * Shows `page` (just built from the entry at `index`) in place of the current one. The history is
 * already settled, so a callback may ask the frame anything. Every callback can destroy what it
 * is called on, so the elements are found again by Kid. With animations the old page stays until
 * it has turned away, and the new one is held back till then.
 */
static void start_intro(kst_frame_t* f, int direction) {
	kst_element_t* fe = &f->base.base.base;
	f->intro_pending = 0;
	if (!f->base.content) return;
	rows_of(f, f->base.content);
	f->direction = direction;
	f->phase = 2;
	f->elapsed = 0;
	fe->wants_tick = 1;
	rows_turn(f, fe, 0, 1);
}

void kst_root_start_intro(kst_root_t* r) {
	kst_frame_t* f;
	if (!r || !r->visual || !kst_is_a(r->visual, K_CLASS_FRAME)) return;
	f = FRAME(r->visual);
	if (f->intro_pending && f->phase == 0 && kst_animations_enabled(r)) start_intro(f, 1);
}

static void show(kst_frame_t* f, Kid frame_id, Kid page_id, void* parameter, int direction, KNavigationMode mode) {
	f->intro_pending = 0;
	f->mode = mode;
	kst_element_t* old = f->base.content;
	kst_element_t* frame = &f->base.base.base;
	int enabled = f->transition != K_TRANSITION_NONE && frame->root && kst_animations_enabled(frame->root);
	int animate = enabled && old && kst_is_a(old, K_CLASS_PAGE);
	f->navigating = 1;
	if (old && kst_is_a(old, K_CLASS_PAGE)) {
		/* the page that leaves is told how the next one is reached */
		PAGE(old)->navigation_mode = mode;
		kst_raise(old, K_EVENT_NAVIGATED_FROM);
	}
	if (!kst_lookup(frame_id)) {
		kst_element_t* page = kst_lookup(page_id);
		if (page && !page->parent) kst_destroy(page);
		return;
	}
	if (!animate || !f->base.content) {
		int first = old == NULL;
		attach(f, frame_id, page_id, parameter);
		if (!kst_lookup(frame_id)) return;
		/* the first page has nothing to turn away from, but it swings in: the app opens as the second half of a turn.
		 * Before the window exists nobody knows yet whether animations run: Kwindow_run starts it then. */
		f = FRAME(kst_lookup(frame_id));
		f->intro_pending = first && f->transition != K_TRANSITION_NONE;
		if (enabled) start_intro(f, direction);
		return;
	}
	old = f->base.content;
	rows_of(f, old);
	old->hit_visible = 0;
	f->pending = page_id;
	f->pending_parameter = parameter;
	f->direction = direction;
	f->phase = 1;
	f->elapsed = 0;
	f->navigating = 0;
	frame->wants_tick = 1;
	rows_turn(f, frame, 0, 0);
}

/* Builds the page of an entry; K_INVALID_ID, and the page destroyed, when it is not a Page. */
static Kid build(Kid frame_id, kst_frame_entry_t entry) {
	Kid page = entry.factory(frame_id, entry.parameter);
	kst_element_t* e;
	if (page == K_INVALID_ID) return K_INVALID_ID;
	e = kst_lookup(page);
	if (!e) return K_INVALID_ID;
	if (!kst_is_a(e, K_CLASS_PAGE) || e->parent) {
		kst_error(K_ERROR_WRONG_CLASS, "a page factory must return a new Page");
		if (!e->parent) kst_destroy(e);
		return K_INVALID_ID;
	}
	return page;
}

int Kframe_navigate(Kid id, KPageFactory factory, void* parameter) {
	kst_frame_t* f = get_frame(id);
	kst_frame_entry_t entry = { factory, parameter, K_INVALID_ID };
	Kid page;
	if (!f) return K_FALSE;
	if (!factory) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "Navigate needs a page factory");
		return K_FALSE;
	}
	if (f->navigating) return K_FALSE;
	finish(f, id);
	f = get_frame(id);
	if (!f) return K_FALSE;
	f->navigating = 1;
	page = build(id, entry);
	f = get_frame(id);
	if (!f) return K_FALSE;
	f->navigating = 0;
	if (page == K_INVALID_ID) return K_FALSE;
	if (f->current + 1 >= f->capacity) {
		int capacity = f->capacity ? f->capacity * 2 : 8;
		kst_frame_entry_t* entries = (kst_frame_entry_t*)realloc(f->entries, (size_t)capacity * sizeof *entries);
		if (!entries) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			kst_destroy(kst_lookup(page));
			return K_FALSE;
		}
		f->entries = entries;
		f->capacity = capacity;
	}
	/* the entries ahead are dropped, and the pages kept for them */
	drop_cached(f, f->current + 1);
	f->current++;
	entry.cached = K_INVALID_ID;
	f->entries[f->current] = entry;
	f->count = f->current + 1;
	show(f, id, page, parameter, 1, K_NAVIGATION_NEW);
	return K_TRUE;
}

static int move(Kid id, int step) {
	kst_frame_t* f = get_frame(id);
	int target;
	Kid page;
	if (!f || f->navigating) return K_FALSE;
	finish(f, id);
	f = get_frame(id);
	if (!f) return K_FALSE;
	target = f->current + step;
	if (target < 0 || target >= f->count) return K_FALSE;
	f->navigating = 1;
	if (f->entries[target].cached != K_INVALID_ID && kst_lookup(f->entries[target].cached)) {
		page = f->entries[target].cached;
		f->entries[target].cached = K_INVALID_ID;
	} else {
		page = build(id, f->entries[target]);
		f = get_frame(id);
		if (!f) return K_FALSE;
	}
	f->navigating = 0;
	if (page == K_INVALID_ID) return K_FALSE;
	f->current = target;
	show(f, id, page, f->entries[target].parameter, step, step < 0 ? K_NAVIGATION_BACK : K_NAVIGATION_FORWARD);
	return K_TRUE;
}

int Kframe_go_back(Kid id) { return move(id, -1); }
int Kframe_go_forward(Kid id) { return move(id, 1); }

int Kframe_can_go_back(Kid id) {
	kst_frame_t* f = get_frame(id);
	return f && f->current > 0;
}

int Kframe_can_go_forward(Kid id) {
	kst_frame_t* f = get_frame(id);
	return f && f->current + 1 < f->count;
}

int Kframe_get_back_stack_depth(Kid id) {
	kst_frame_t* f = get_frame(id);
	return f && f->current > 0 ? f->current : 0;
}

Kid Kframe_get_page(Kid id) {
	kst_frame_t* f = get_frame(id);
	if (f && f->phase == 1 && f->pending != K_INVALID_ID) return f->pending;
	return f && f->base.content ? f->base.content->id : K_INVALID_ID;
}

KPageFactory Kframe_get_current_source_page_type(Kid id) {
	kst_frame_t* f = get_frame(id);
	return f && f->current >= 0 && f->current < f->count ? f->entries[f->current].factory : NULL;
}

void Kframe_on_navigated(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_FRAME)) Kelement_on(id, K_EVENT_NAVIGATED, callback, user_data);
}

void Kframe_set_transition(Kid id, KPageTransition transition) {
	kst_frame_t* f = get_frame(id);
	if (f) f->transition = transition;
}

int Kframe_is_transitioning(Kid id) {
	kst_frame_t* f = get_frame(id);
	return f && f->phase != 0;
}
