/**
 * zrnui - Frame and Page: the navigation of a Windows Phone app.
 */

#include "../internal.h"

#define PAGE(e) ZST_AS(zrn_page_t, e)
#define FRAME(e) ZST_AS(zrn_frame_t, e)


/* ============================================================================
 *  Page
 * ============================================================================ */

static void page_init(zrn_element_t* const e) {
	e->focusable = 0;
	ZST_AS(zrn_control_t, e)->h_content = Z_HALIGN_STRETCH;
	ZST_AS(zrn_control_t, e)->v_content = Z_VALIGN_STRETCH;
	PAGE(e)->navigation_mode = Z_NAVIGATION_NEW;
	PAGE(e)->cache_mode = Z_NAVIGATION_CACHE_DISABLED;
	PAGE(e)->entry_index = -1;
}

/* The page makes room for its app bars: what is left is the content's, and an open bar grows over the content. */
static void page_bar_room(zrn_page_t* const p, float* const top, float* const bottom) {
	*top = p->top_bar && p->top_bar->visibility == Z_VISIBLE ? zrn_app_bar_closed_height(p->top_bar) : 0;
	*bottom = p->bottom_bar && p->bottom_bar->visibility == Z_VISIBLE ? zrn_app_bar_closed_height(p->bottom_bar) : 0;
}

static ZSize page_measure(zrn_element_t* const e, const ZSize available) {
	zrn_page_t* const p = PAGE(e);
	const zrn_content_control_t* const cc = &p->base;
	float top, bottom;
	ZSize content = { 0, 0 };
	page_bar_room(p, &top, &bottom);
	if (cc->content) {
		zrn_measure(cc->content, zrn_size(available.width, zrn_maxf(0, available.height - top - bottom)));
		content = cc->content->desired;
	}
	if (p->top_bar) zrn_measure(p->top_bar, zrn_size(available.width, available.height));
	if (p->bottom_bar) zrn_measure(p->bottom_bar, zrn_size(available.width, available.height));
	return zrn_size(content.width, content.height + top + bottom);
}

static ZSize page_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_page_t* const p = PAGE(e);
	float top, bottom;
	page_bar_room(p, &top, &bottom);
	if (p->base.content) zrn_arrange(p->base.content, zrn_rect(0, top, final.width, zrn_maxf(0, final.height - top - bottom)));
	if (p->top_bar) zrn_arrange(p->top_bar, zrn_rect(0, 0, final.width, p->top_bar->desired.height));
	if (p->bottom_bar) zrn_arrange(p->bottom_bar, zrn_rect(0, final.height - p->bottom_bar->desired.height, final.width, p->bottom_bar->desired.height));
	return final;
}

static void page_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	zrn_page_t* const p = PAGE(e);
	if (p->top_bar == child) p->top_bar = nullptr;
	else if (p->bottom_bar == child) p->bottom_bar = nullptr;
	else zrn_cc_child_removed(e, child);
}

zrn_class_t zrn_class_page = {
	.base = &zrn_class_user_control, .name = "Page", .id = Z_CLASS_PAGE, .size = sizeof(zrn_page_t),
	.init = page_init, .measure = page_measure, .arrange = page_arrange, .child_removed = page_child_removed
};

Zid Zpage_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_PAGE);
	return e ? e->id : Z_INVALID_ID;
}

void Zpage_set_content(Zid id, Zid child) { Zuser_control_set_content(id, child); }

Zid Zpage_get_content(Zid id) { return Zuser_control_get_content(id); }

void* Zpage_get_parameter(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e ? PAGE(e)->parameter : nullptr;
}

ZNavigationMode Zpage_get_navigation_mode(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e ? PAGE(e)->navigation_mode : Z_NAVIGATION_NEW;
}

void Zpage_set_navigation_cache_mode(const Zid id, const ZNavigationCacheMode mode) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	if (e) PAGE(e)->cache_mode = mode;
}

ZNavigationCacheMode Zpage_get_navigation_cache_mode(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e ? PAGE(e)->cache_mode : Z_NAVIGATION_CACHE_DISABLED;
}

/* The bars are children of the page after its content: z-order puts them over it, and an open one grows over the content. */
static void page_set_bar(const Zid page, const Zid bar, const int top) {
	zrn_element_t* const e = zrn_get(page, Z_CLASS_PAGE);
	zrn_element_t* const b = bar != Z_INVALID_ID ? zrn_get(bar, Z_CLASS_APP_BAR) : nullptr;
	zrn_element_t** slot;
	if (!e || (bar != Z_INVALID_ID && !b)) return;
	slot = top ? &PAGE(e)->top_bar : &PAGE(e)->bottom_bar;
	zrn_set_only_child(e, slot, b);
	if (b && *slot == b) {
		b->zindex = 1;
		zrn_app_bar_set_on_top(b, top);
	}
}

void Zpage_set_top_app_bar(Zid page, Zid app_bar) { page_set_bar(page, app_bar, 1); }
void Zpage_set_bottom_app_bar(Zid page, Zid app_bar) { page_set_bar(page, app_bar, 0); }

Zid Zpage_get_top_app_bar(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e && PAGE(e)->top_bar ? PAGE(e)->top_bar->id : Z_INVALID_ID;
}

Zid Zpage_get_bottom_app_bar(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e && PAGE(e)->bottom_bar ? PAGE(e)->bottom_bar->id : Z_INVALID_ID;
}

Zid Zpage_get_frame(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_PAGE);
	return e && e->parent && zrn_is_a(e->parent, Z_CLASS_FRAME) ? e->parent->id : Z_INVALID_ID;
}

void Zpage_on_navigated_to(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(id, Z_CLASS_PAGE)) Zelement_on(id, Z_EVENT_NAVIGATED_TO, callback, user_data);
}

void Zpage_on_navigated_from(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(id, Z_CLASS_PAGE)) Zelement_on(id, Z_EVENT_NAVIGATED_FROM, callback, user_data);
}


/* ============================================================================
 *  Frame
 * ============================================================================ */

/* ---- the turnstile, as ZrnMetroCore times it from the Lumia: the rows of the page that leaves turn away
 * round the left edge one after another (128 ms each, 28 ms apart), a moment of nothing, then the rows
 * of the new page swing in from behind the screen (180 ms each). Going back the order is reversed. */
static constexpr float OUT_MS = 128.0f;
static constexpr float IN_MS = 180.0f;
static constexpr float STAGGER = 28.0f;
static constexpr float GAP_AT = 330.0f;
static constexpr int MAX_ROWS = 64;
static constexpr int MAX_STAGGER_ROWS = 7;

static void frame_init(zrn_element_t* const e) {
	e->focusable = 0;
	FRAME(e)->current = -1;
	FRAME(e)->transition = Z_TRANSITION_TURNSTILE;
	FRAME(e)->mode = Z_NAVIGATION_NEW;
	ZST_AS(zrn_control_t, e)->h_content = Z_HALIGN_STRETCH;
	ZST_AS(zrn_control_t, e)->v_content = Z_VALIGN_STRETCH;
}

/* The pages kept alive for the entries from `first` on go with them. */
static void drop_cached(zrn_frame_t* const f, const int first) {
	int i;
	for (i = first; i < f->count; i++) {
		zrn_element_t* const page = f->entries[i].cached != Z_INVALID_ID ? zrn_lookup(f->entries[i].cached) : nullptr;
		f->entries[i].cached = Z_INVALID_ID;
		if (page && !page->parent) zrn_destroy(page);
	}
}

static void frame_destroy(zrn_element_t* const e) {
	zrn_frame_t* const f = FRAME(e);
	zrn_element_t* const pending = f->pending != Z_INVALID_ID ? zrn_lookup(f->pending) : nullptr;
	if (pending && !pending->parent) zrn_destroy(pending);
	drop_cached(f, 0);
	free(f->entries);
	free(f->rows);
	free(f->row_opacity);
	f->entries = nullptr;
	f->rows = nullptr;
	f->row_opacity = nullptr;
}

static void frame_tick(zrn_element_t* e, float dt);

/* The page starts under the status bar: the safe area is room the frame keeps, which is also where a turnstile turns. */
static ZThickness frame_chrome(zrn_element_t* const e) {
	ZST_UNUSED(e);
	return Zthickness(0, ZST_SAFE_AREA_TOP, 0, 0);
}

zrn_class_t zrn_class_frame = {
	.base = &zrn_class_content_control, .name = "Frame", .id = Z_CLASS_FRAME, .size = sizeof(zrn_frame_t),
	.init = frame_init, .destroy = frame_destroy, .tick = frame_tick, .chrome = frame_chrome
};

Zid Zframe_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_FRAME);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_frame_t* get_frame(Zid id) { return FRAME(zrn_get(id, Z_CLASS_FRAME)); }

/* A row is what turns: a leaf of the tree, or a control that is not a container. */
static int is_container(const zrn_element_t* const e) {
	return zrn_is_a(e, Z_CLASS_PANEL) || zrn_is_a(e, Z_CLASS_BORDER) || zrn_is_a(e, Z_CLASS_VIEWBOX) ||
		zrn_is_a(e, Z_CLASS_SCROLL_VIEWER) || zrn_is_a(e, Z_CLASS_USER_CONTROL) || zrn_is_a(e, Z_CLASS_ITEMS_CONTROL);
}

static void collect_rows(zrn_frame_t* const f, zrn_element_t* const e) {
	int i;
	if (e->visibility != Z_VISIBLE || f->row_count >= MAX_ROWS || zrn_is_a(e, Z_CLASS_APP_BAR)) return;
	if (is_container(e) && e->child_count > 0) {
		for (i = 0; i < e->child_count; i++) collect_rows(f, e->children[i]);
		return;
	}
	if (f->row_count == f->row_cap) {
		const int cap = f->row_cap ? f->row_cap * 2 : 16;
		Zid* rows = (Zid*)realloc(f->rows, (size_t)cap * sizeof *rows);
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

static void rows_of(zrn_frame_t* const f, zrn_element_t* const page) {
	f->row_count = 0;
	if (page) collect_rows(f, page);
}

/* The rows back as they were. */
static void rows_reset(zrn_frame_t* const f) {
	int i;
	for (i = 0; i < f->row_count; i++) {
		zrn_element_t* const row = zrn_lookup(f->rows[i]);
		if (!row) continue;
		row->rot_x = row->rot_y = 0;
		row->proj_cx = row->proj_cy = 0.5f;
		row->opacity = f->row_opacity[i];
		zrn_update_transformed(row);
		zrn_changed_paint(row);
	}
	f->row_count = 0;
}

static float clamp01(float v) { return v < 0 ? 0 : v > 1 ? 1 : v; }

/* One frame of the turnstile: row `i` of `n`, `t` ms into this half. */
static void rows_turn(zrn_frame_t* const f, zrn_element_t* const frame, const float t, const int incoming) {
	int i, n = f->row_count;
	const float left = zrn_to_root(frame, (ZPoint){ 0, 0 }).x;
	for (i = 0; i < n; i++) {
		zrn_element_t* const row = zrn_lookup(f->rows[i]);
		int order = f->direction >= 0 ? i : n - 1 - i;
		float u, angle, alpha;
		ZPoint origin;
		if (!row) continue;
		if (order > MAX_STAGGER_ROWS) order = MAX_STAGGER_ROWS;
		/* a row not laid out yet has no width to turn about: it is only kept clear until the first tick lays it out */
		if (row->rect.width > 0) {
			origin = zrn_to_root(row, (ZPoint){ 0, 0 });
			row->proj_cx = (left - origin.x) / row->rect.width;
			row->proj_cy = 0.5f;
		}
		if (!incoming) {
			u = clamp01((t - (float)order * STAGGER) / OUT_MS);
			angle = (f->direction >= 0 ? -50.0f : 50.0f) * zrn_ease(Z_EASE_IN, u);
			alpha = u >= 1 ? 0 : 1 - u * u;
		} else {
			u = clamp01((t - (float)order * STAGGER) / IN_MS);
			angle = 80.0f * (1 - zrn_ease(Z_EASE_OUT, u));
			alpha = u <= 0 ? 0 : clamp01(u * 3);
		}
		row->rot_y = angle;
		row->opacity = f->row_opacity[i] * alpha;
		zrn_update_transformed(row);
		zrn_changed_paint(row);
	}
}

/* The page goes into the frame: its parameter, the content, NavigatedTo, Navigated. */
static void attach(zrn_frame_t* const f, const Zid frame_id, const Zid page_id, void* const parameter) {
	zrn_element_t* const page = zrn_lookup(page_id);
	f->navigating = 1;
	if (!zrn_lookup(frame_id)) {
		if (page && !page->parent) zrn_destroy(page);
		return;
	}
	if (!page) { f->navigating = 0; return; }
	PAGE(page)->parameter = parameter;
	PAGE(page)->navigation_mode = f->mode;
	PAGE(page)->entry_index = f->current;
	page->hit_visible = 1;
	/* a page that keeps its NavigationCacheMode stays alive, out of the frame, for the entry it was built for */
	{
		zrn_element_t* const old = f->base.content;
		if (old && old != page && zrn_is_a(old, Z_CLASS_PAGE) && PAGE(old)->cache_mode != Z_NAVIGATION_CACHE_DISABLED) {
			const int index = PAGE(old)->entry_index;
			if (index >= 0 && index < f->count && f->entries[index].cached == Z_INVALID_ID) {
				zrn_remove_child(&f->base.base.base, old);
				f->entries[index].cached = old->id;
			}
		}
	}
	zrn_content_set(&f->base, page);
	if (zrn_lookup(page_id)) zrn_raise(zrn_lookup(page_id), Z_EVENT_NAVIGATED_TO);
	if (zrn_lookup(frame_id)) zrn_raise(zrn_lookup(frame_id), Z_EVENT_NAVIGATED);
	if (zrn_lookup(frame_id)) f->navigating = 0;
}

/* Ends a transition at once: the new page is in, nothing turns. */
static void finish(zrn_frame_t* const f, const Zid frame_id) {
	if (f->phase == 0) return;
	if (f->phase == 1) {
		const Zid page = f->pending;
		rows_reset(f);
		f->pending = Z_INVALID_ID;
		f->phase = 0;
		attach(f, frame_id, page, f->pending_parameter);
		if (!zrn_lookup(frame_id)) return;
	} else {
		rows_reset(f);
	}
	f->phase = 0;
	f->elapsed = 0;
}

static void frame_tick(zrn_element_t* e, float dt) {
	zrn_frame_t* f = FRAME(e);
	const Zid id = e->id;
	if (f->phase == 0) { e->wants_tick = 0; return; }
	f->elapsed += dt;
	if (e->root && e->root->layout_dirty) Zroot_update_layout(e->root->id);
	if (f->phase == 1) {
		if (f->elapsed < GAP_AT) {
			rows_turn(f, e, f->elapsed, 0);
			return;
		}
		/* the old page is gone, the new one comes in */
		{
			const Zid page = f->pending;
			const float carry = f->elapsed - GAP_AT;
			rows_reset(f);
			f->pending = Z_INVALID_ID;
			f->phase = 2;
			attach(f, id, page, f->pending_parameter);
			if (!zrn_lookup(id)) return;
			f = FRAME(zrn_lookup(id));
			e = &f->base.base.base;
			if (e->root && e->root->layout_dirty) Zroot_update_layout(e->root->id);
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
 * is called on, so the elements are found again by Zid. With animations the old page stays until
 * it has turned away, and the new one is held back till then.
 */
static void start_intro(zrn_frame_t* const f, const int direction) {
	zrn_element_t* const fe = &f->base.base.base;
	f->intro_pending = 0;
	if (!f->base.content) return;
	rows_of(f, f->base.content);
	f->direction = direction;
	f->phase = 2;
	f->elapsed = 0;
	fe->wants_tick = 1;
	rows_turn(f, fe, 0, 1);
}

void zrn_root_start_intro(zrn_root_t* const r) {
	zrn_frame_t* f;
	if (!r || !r->visual || !zrn_is_a(r->visual, Z_CLASS_FRAME)) return;
	f = FRAME(r->visual);
	if (f->intro_pending && f->phase == 0 && zrn_animations_enabled(r)) start_intro(f, 1);
}

static void show(zrn_frame_t* f, Zid frame_id, Zid page_id, void* parameter, int direction, ZNavigationMode mode) {
	f->intro_pending = 0;
	f->mode = mode;
	zrn_element_t* old = f->base.content;
	zrn_element_t* const frame = &f->base.base.base;
	const int enabled = f->transition != Z_TRANSITION_NONE && frame->root && zrn_animations_enabled(frame->root);
	const int animate = enabled && old && zrn_is_a(old, Z_CLASS_PAGE);
	f->navigating = 1;
	if (old && zrn_is_a(old, Z_CLASS_PAGE)) {
		/* the page that leaves is told how the next one is reached */
		PAGE(old)->navigation_mode = mode;
		zrn_raise(old, Z_EVENT_NAVIGATED_FROM);
	}
	if (!zrn_lookup(frame_id)) {
		zrn_element_t* const page = zrn_lookup(page_id);
		if (page && !page->parent) zrn_destroy(page);
		return;
	}
	if (!animate || !f->base.content) {
		const int first = old == nullptr;
		attach(f, frame_id, page_id, parameter);
		if (!zrn_lookup(frame_id)) return;
		/* the first page has nothing to turn away from, but it swings in: the app opens as the second half of a turn.
		 * Before the window exists nobody knows yet whether animations run: Zwindow_run starts it then. */
		f = FRAME(zrn_lookup(frame_id));
		f->intro_pending = first && f->transition != Z_TRANSITION_NONE;
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

/* Builds the page of an entry; Z_INVALID_ID, and the page destroyed, when it is not a Page. */
static Zid build(const Zid frame_id, const zrn_frame_entry_t entry) {
	const Zid page = entry.factory(frame_id, entry.parameter);
	zrn_element_t* e;
	if (page == Z_INVALID_ID) return Z_INVALID_ID;
	e = zrn_lookup(page);
	if (!e) return Z_INVALID_ID;
	if (!zrn_is_a(e, Z_CLASS_PAGE) || e->parent) {
		zrn_error(Z_ERROR_WRONG_CLASS, "a page factory must return a new Page");
		if (!e->parent) zrn_destroy(e);
		return Z_INVALID_ID;
	}
	return page;
}

int Zframe_navigate(const Zid id, const ZPageFactory factory, void* const parameter) {
	zrn_frame_t* f = get_frame(id);
	zrn_frame_entry_t entry = { factory, parameter, Z_INVALID_ID };
	Zid page;
	if (!f) return false;
	if (!factory) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "Navigate needs a page factory");
		return false;
	}
	if (f->navigating) return false;
	finish(f, id);
	f = get_frame(id);
	if (!f) return false;
	f->navigating = 1;
	page = build(id, entry);
	f = get_frame(id);
	if (!f) return false;
	f->navigating = 0;
	if (page == Z_INVALID_ID) return false;
	if (f->current + 1 >= f->capacity) {
		const int capacity = f->capacity ? f->capacity * 2 : 8;
		zrn_frame_entry_t* entries = (zrn_frame_entry_t*)realloc(f->entries, (size_t)capacity * sizeof *entries);
		if (!entries) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			zrn_destroy(zrn_lookup(page));
			return false;
		}
		f->entries = entries;
		f->capacity = capacity;
	}
	/* the entries ahead are dropped, and the pages kept for them */
	drop_cached(f, f->current + 1);
	f->current++;
	entry.cached = Z_INVALID_ID;
	f->entries[f->current] = entry;
	f->count = f->current + 1;
	show(f, id, page, parameter, 1, Z_NAVIGATION_NEW);
	return true;
}

static int move(const Zid id, const int step) {
	zrn_frame_t* f = get_frame(id);
	int target;
	Zid page;
	if (!f || f->navigating) return false;
	finish(f, id);
	f = get_frame(id);
	if (!f) return false;
	target = f->current + step;
	if (target < 0 || target >= f->count) return false;
	f->navigating = 1;
	if (f->entries[target].cached != Z_INVALID_ID && zrn_lookup(f->entries[target].cached)) {
		page = f->entries[target].cached;
		f->entries[target].cached = Z_INVALID_ID;
	} else {
		page = build(id, f->entries[target]);
		f = get_frame(id);
		if (!f) return false;
	}
	f->navigating = 0;
	if (page == Z_INVALID_ID) return false;
	f->current = target;
	show(f, id, page, f->entries[target].parameter, step, step < 0 ? Z_NAVIGATION_BACK : Z_NAVIGATION_FORWARD);
	return true;
}

int Zframe_go_back(Zid id) { return move(id, -1); }
int Zframe_go_forward(Zid id) { return move(id, 1); }

int Zframe_can_go_back(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	return f && f->current > 0;
}

int Zframe_can_go_forward(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	return f && f->current + 1 < f->count;
}

int Zframe_get_back_stack_depth(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	return f && f->current > 0 ? f->current : 0;
}

Zid Zframe_get_page(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	if (f && f->phase == 1 && f->pending != Z_INVALID_ID) return f->pending;
	return f && f->base.content ? f->base.content->id : Z_INVALID_ID;
}

ZPageFactory Zframe_get_current_source_page_type(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	return f && f->current >= 0 && f->current < f->count ? f->entries[f->current].factory : nullptr;
}

void Zframe_on_navigated(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(id, Z_CLASS_FRAME)) Zelement_on(id, Z_EVENT_NAVIGATED, callback, user_data);
}

void Zframe_set_transition(const Zid id, const ZPageTransition transition) {
	zrn_frame_t* const f = get_frame(id);
	if (f) f->transition = transition;
}

int Zframe_is_transitioning(const Zid id) {
	const zrn_frame_t* const f = get_frame(id);
	return f && f->phase != 0;
}
