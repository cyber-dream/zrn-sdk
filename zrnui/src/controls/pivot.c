/**
 * zrnui - Pivot and PivotItem, after the Windows Phone 8.1 template (generic.xaml of the phone SDK).
 *
 * A title (SemiBold, 18.14 px, at 19,38), under it the strip of the headers (57 px SemiLight, character
 * spacing -25, margin 16,-6.5, padding 0,0,0,6.5) with the chosen one in PivotHeaderForegroundSelected
 * and the others in PivotHeaderForegroundUnselected (PhoneControlDisabled), and under it the page of the
 * chosen item, a PivotItem with the margin 19,22.5,19,0. A finger drags the pages and the strip together
 * and lets go on the next page when it went a fifth of a page or flicked.
 *
 * The strip is a ring, as the PivotHeaderPanel makes it: with two items or more the headers go round, the
 * chosen one at the left and the next ones after it; the one that leaves at the left comes back at the end,
 * and a swipe past the last page goes to the first (the template is silent about it, the native PivotPanel
 * does it). IsLocked ends the swipes and the taps on the headers, and hides the headers of the others.
 *
 * The pages are put where they are by layout (not by a render transform), so the pivot looks the same with
 * the software renderer; the headers and the title are drawn by the pivot itself.
 */

#include "../internal.h"

/* themeresources.xaml: PivotHeaderItem*, PivotItemMargin, PivotPortraitThemePadding; TextStyleLargeFontSize for the title */
#define TITLE_FONT ZST_81_FONT_LARGE
static constexpr float TITLE_X = 19.0f;
static constexpr float TITLE_TOP = 38.0f;
static constexpr float HEADER_MARGIN_LEFT = 16.0f;
static constexpr float HEADER_MARGIN_TOP = (-6.5f);
static constexpr float HEADER_TEXT_X = 19.0f;
static constexpr float ITEM_MARGIN_LEFT = 19.0f;
static constexpr float ITEM_MARGIN_TOP = 22.5f;
static constexpr float ITEM_MARGIN_RIGHT = 19.0f;
static constexpr float SLIDE_MS = 300.0f;
static constexpr float DRAG_START = 12.0f;

typedef struct zrn_pivot_item {
	zrn_content_control_t base;
	char* header;
} zrn_pivot_item_t;

typedef struct zrn_pivot {
	zrn_control_t base;
	char* title;
	zrn_element_t** items;
	int count, cap;
	int selected;
	/** Where the strip is, in pages, on a ring: 1.25 is a quarter of the way from the second page to the third; it can be below 0 or past the count. */
	float offset;
	/** The slide to `to`. */
	float from, to, elapsed;
	int sliding;
	int pressed, dragging;
	float press_x, press_y, last_x, velocity;
	int is_locked;
	/** The item that was chosen and waits for its slide to end to be Loaded. */
	int loaded_pending;
	ZPivotItemCallback loading, loaded;
	void *loading_data, *loaded_data;
} zrn_pivot_t;

#define PIVOT(e) ZST_AS(zrn_pivot_t, e)
#define PITEM(e) ZST_AS(zrn_pivot_item_t, e)

/** PivotHeaderItemFontFamily, FontSize and CharacterSpacing of the theme resources of the root (an override applies). */
static zrn_font_t header_font(const zrn_pivot_t* const p) {
	const zrn_root_t* r = ((const zrn_element_t*)p)->root;
	zrn_font_t f = { Z_FONT_SEMILIGHT, 57.0f, -25 };
	zrn_res_font_family(r, Z_RES_PIVOT_HEADER_ITEM_FONT_FAMILY, &f.family);
	zrn_res_double(r, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE, &f.size);
	zrn_res_int(r, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING, &f.spacing);
	return f;
}

static zrn_font_t title_font(void) {
	const zrn_font_t f = { Z_FONT_SEMIBOLD, TITLE_FONT, 0 };
	return f;
}

static const char* item_header(const zrn_pivot_t* const p, const int i) {
	const char* h = PITEM(p->items[i])->header;
	return h ? h : "";
}

/** The width a header takes in the strip: its text and the 16 px of its margin. */
static float header_slot(const zrn_pivot_t* const p, const int i) {
	const char* h = item_header(p, i);
	return zrn_text_width(header_font(p), h, (int)strlen(h)) + HEADER_MARGIN_LEFT;
}

static float strip_length(const zrn_pivot_t* const p) {
	float l = 0;
	int i;
	for (i = 0; i < p->count; i++) l += header_slot(p, i);
	return l;
}

static int wrap_index(const int index, const int n) {
	const int m = n > 0 ? index % n : 0;
	return m < 0 ? m + n : m;
}

/** `d` brought into [-n/2, n/2], a tie going forward. */
static float wrap_distance(const float d, const int n) {
	return d - (float)n * ceilf(d / (float)n - 0.5f);
}

/** How far the strip has gone round for a place between pages: the distance of the chosen header from the start of the ring. */
static float strip_shift(const zrn_pivot_t* const p, const float offset) {
	int a, i, n = p->count;
	float c = 0, f;
	if (n < 2) return 0;
	a = (int)floorf(offset);
	f = offset - (float)a;
	i = wrap_index(a, n);
	{
		int k;
		for (k = 0; k < i; k++) c += header_slot(p, k);
	}
	return c + f * header_slot(p, i) + (float)((a - i) / n) * strip_length(p);
}

static float title_height(const zrn_pivot_t* const p) {
	return p->title && *p->title ? zrn_font_line_height(title_font()) : 0;
}

/** Where the strip starts (the top of its panel) and how high it is: the line of the 57 px font, the margin and the padding cancel. */
static float strip_top(const zrn_pivot_t* p) { return TITLE_TOP + title_height(p); }
static float strip_height(const zrn_pivot_t* p) { return zrn_font_line_height(header_font(p)); }
static float items_top(const zrn_pivot_t* p) { return strip_top(p) + strip_height(p); }

/** A header as it stands in the strip: the item, the text's left edge, and how near it is to being the chosen one (0..1). */
typedef struct header_place {
	int item;
	float x, width, near;
} header_place_t;

static int header_places(const zrn_pivot_t* const p, header_place_t* const out, const int max, const float view_width) {
	int count = 0, i, m, n = p->count;
	float shift, ring;
	if (n == 0) return 0;
	if (p->is_locked) {
		out[0].item = p->selected;
		out[0].x = HEADER_TEXT_X;
		out[0].width = header_slot(p, p->selected) - HEADER_MARGIN_LEFT;
		out[0].near = 1.0f;
		return 1;
	}
	shift = strip_shift(p, p->offset);
	ring = strip_length(p);
	for (m = n < 2 ? 0 : -2; m <= (n < 2 ? 0 : 2); m++) {
		float c = 0;
		for (i = 0; i < n; i++) {
			const float slot = header_slot(p, i), x = HEADER_TEXT_X + c + (float)m * ring - shift;
			const float r = (float)(i + m * n) - p->offset;
			c += slot;
			/* the header before the chosen one is not seen at rest: its text ends where the margin of the strip starts */
			if (x >= view_width || x + slot - HEADER_MARGIN_LEFT <= HEADER_TEXT_X - HEADER_MARGIN_LEFT + 0.5f) continue;
			if (count == max) return count;
			out[count].item = i;
			out[count].x = x;
			out[count].width = slot - HEADER_MARGIN_LEFT;
			out[count].near = zrn_clampf(1.0f - fabsf(r), 0, 1);
			count++;
		}
	}
	return count;
}

static constexpr int MAX_PLACES = 64;

static ZRect header_rect_of(const header_place_t* const h, const zrn_pivot_t* const p) {
	return zrn_rect(h->x, strip_top(p), h->width, strip_height(p));
}

static ZSize pivot_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_pivot_t* const p = PIVOT(e);
	const float w = available.width < Z_INFINITE * 0.5f ? available.width : 480.0f;
	const float h = available.height < Z_INFINITE * 0.5f ? available.height : 800.0f;
	const float top = items_top(p);
	int i;
	for (i = 0; i < p->count; i++) zrn_measure(p->items[i], zrn_size(w, zrn_maxf(0, h - top)));
	return zrn_size(w, h);
}

static ZSize pivot_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_pivot_t* const p = PIVOT(e);
	const float top = items_top(p);
	int i, n = p->count;
	for (i = 0; i < n; i++) {
		zrn_element_t* const it = p->items[i];
		const float d = wrap_distance((float)i - p->offset, n);
		/* a page is in view while the strip is less than a page away from it */
		it->visibility = fabsf(d) < 1.0f ? Z_VISIBLE : Z_COLLAPSED;
		it->hit_visible = fabsf(d) < 0.5f || (n == 1);
		zrn_arrange(it, zrn_rect(d * final.width, top, final.width, zrn_maxf(0, final.height - top)));
	}
	return final;
}

static void pivot_paint(zrn_element_t* const e, zrn_painter_t* const painter) {
	const zrn_pivot_t* const p = PIVOT(e);
	const ZColor fg = zrn_ctl_foreground(&p->base), off = zrn_theme_color(e, ZST_TC_PHONE_DISABLED);
	header_place_t places[MAX_PLACES];
	int i, n = header_places(p, places, MAX_PLACES, e->rect.width);
	if (p->title && *p->title) zrn_draw_string(painter, title_font(), p->title, (int)strlen(p->title), TITLE_X, TITLE_TOP, fg);
	for (i = 0; i < n; i++) {
		const char* h = item_header(p, places[i].item);
		const float t = places[i].near;
		const ZColor c = { off.r + (fg.r - off.r) * t, off.g + (fg.g - off.g) * t, off.b + (fg.b - off.b) * t, off.a + (fg.a - off.a) * t };
		/* the margin top of -6.5 pulls the text up by as much as the padding pushes the box down: it starts where the strip does */
		zrn_draw_string(painter, header_font(p), h, (int)strlen(h), places[i].x, strip_top(p) + HEADER_MARGIN_TOP, c);
	}
}

static void pivot_loaded_now(zrn_pivot_t* const p) {
	const zrn_element_t* const e = &p->base.base;
	Zid id = e->id, item;
	if (!p->loaded_pending) return;
	p->loaded_pending = 0;
	if (!p->loaded || p->selected >= p->count) return;
	item = p->items[p->selected]->id;
	p->loaded(id, item, p->loaded_data);
}

/* The strip goes to a place on the ring; the item is the chosen one from here. */
static void pivot_go(zrn_pivot_t* p, const float to, const int animate) {
	zrn_element_t* e = &p->base.base;
	const Zid id = e->id;
	const int n = p->count, index = wrap_index((int)floorf(to + 0.5f), n), changed = index != p->selected;
	p->selected = index;
	p->from = p->offset;
	p->to = to;
	p->elapsed = 0;
	if (changed) p->loaded_pending = 1;
	if (changed && p->loading) {
		p->loading(id, p->items[index]->id, p->loading_data);
		if (!zrn_lookup(id)) return;
		p = PIVOT(zrn_lookup(id));
		e = &p->base.base;
	}
	if (animate && e->root && zrn_animations_enabled(e->root) && p->offset != p->to) {
		p->sliding = 1;
		e->wants_tick = 1;
	} else {
		p->sliding = 0;
		p->offset = (float)index;
	}
	zrn_changed_arrange(e);
	if (changed) {
		if (!zrn_raise(e, Z_EVENT_SELECTION_CHANGED)) return;
	}
	if (!p->sliding) pivot_loaded_now(p);
}

/* To an item by its index: the shortest way round. */
static void pivot_select(zrn_pivot_t* p, int index, const int animate) {
	if (p->count == 0) return;
	index = zrn_mini(zrn_maxi(index, 0), p->count - 1);
	pivot_go(p, p->offset + wrap_distance((float)index - p->offset, p->count), animate);
}

static void pivot_tick(zrn_element_t* const e, const float dt) {
	const auto p = PIVOT(e);
	if (!p->sliding) {
		if (!p->dragging) e->wants_tick = 0;
		return;
	}
	p->elapsed += dt;
	if (p->elapsed >= SLIDE_MS) {
		p->offset = (float)p->selected;
		p->sliding = 0;
		if (!p->dragging) e->wants_tick = 0;
		zrn_changed_arrange(e);
		pivot_loaded_now(p);
		return;
	}
	p->offset = p->from + (p->to - p->from) * zrn_ease(Z_EASE_OUT, p->elapsed / SLIDE_MS);
	zrn_changed_arrange(e);
}

static int header_at(const zrn_pivot_t* const p, const float x, const float y, const float view_width) {
	header_place_t places[MAX_PLACES];
	int i, n = header_places(p, places, MAX_PLACES, view_width);
	for (i = 0; i < n; i++) {
		const ZRect r = header_rect_of(&places[i], p);
		if (x >= r.x - HEADER_MARGIN_LEFT && x < r.x + r.width && y >= r.y + HEADER_MARGIN_TOP && y < r.y + r.height) return places[i].item;
	}
	return -1;
}

/* The finger lets go: to the next page when it went a fifth of a page or flicked, else back. */
static void pivot_release(zrn_pivot_t* const p, const float width) {
	const float dx = (p->offset - (float)p->selected) * -width;
	float target = (float)p->selected;
	if (dx < -width * 0.2f || p->velocity < -0.4f) target += 1.0f;
	else if (dx > width * 0.2f || p->velocity > 0.4f) target -= 1.0f;
	/* the slide goes from where the finger left the strip, back to the same page too */
	pivot_go(p, target, 1);
}

static void pivot_begin_drag(zrn_pivot_t* const p) {
	zrn_element_t* const e = &p->base.base;
	p->dragging = 1;
	p->sliding = 0;
	if (e->root) zrn_set_capture(e->root, e);
	e->wants_tick = 1;
}

static int pivot_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto p = PIVOT(e);
	const float width = e->rect.width > 0 ? e->rect.width : 1;
	const int can_drag = p->count >= 2 && !p->is_locked;
	switch (in->type) {
	case ZST_IN_PREVIEW_DOWN:
		p->pressed = 1;
		p->dragging = 0;
		p->press_x = p->last_x = in->x;
		p->press_y = in->y;
		p->velocity = 0;
		return 0;
	case ZST_IN_PREVIEW_MOVE:
		if (!p->pressed || p->dragging || !e->root || e->root->capture == e || !can_drag) return 0;
		{
			const float dx = in->x - p->press_x, dy = in->y - p->press_y, before = p->last_x;
			p->last_x = in->x;
			if (fabsf(dx) > DRAG_START && fabsf(dx) > fabsf(dy)) {
				/* a horizontal drag is the pivot's, whoever had the pointer loses it; its first move is the step from the last one */
				pivot_begin_drag(p);
				p->last_x = before;
			}
		}
		return 0;
	case ZST_IN_PREVIEW_UP:
		return 0;
	case ZST_IN_POINTER_DOWN:
		p->pressed = 1;
		p->dragging = 0;
		p->press_x = p->last_x = in->x;
		p->press_y = in->y;
		p->velocity = 0;
		if (e->root) zrn_set_capture(e->root, e);
		return 1;
	case ZST_IN_POINTER_MOVE:
		if (!p->pressed) return 0;
		if (!p->dragging) {
			const float dx = in->x - p->press_x, dy = in->y - p->press_y, before = p->last_x;
			p->last_x = in->x;
			if (fabsf(dx) <= DRAG_START || fabsf(dx) <= fabsf(dy) || !can_drag) return 1;
			/* the move came up from an element that has the pointer: the drag is the pivot's from here, with the pointer */
			pivot_begin_drag(p);
			p->last_x = before;
		}
		{
			const float dx = in->x - p->press_x;
			/* a page of rubber on each side: past it the next page would be the one chosen */
			p->offset = zrn_clampf((float)p->selected - dx / width, (float)p->selected - 1.0f, (float)p->selected + 1.0f);
			p->velocity = p->velocity * 0.5f + ((in->x - p->last_x) / 16.0f) * 0.5f;
			p->last_x = in->x;
			zrn_changed_arrange(e);
		}
		return 1;
	case ZST_IN_POINTER_UP: {
		const int was_dragging = p->dragging, tap = p->pressed && !p->dragging;
		const float x = in->x, y = in->y;
		p->pressed = 0;
		p->dragging = 0;
		if (e->root) zrn_release_capture(e->root, e);
		if (was_dragging) {
			pivot_release(p, width);
		} else if (tap && !p->is_locked) {
			const int h = header_at(p, x, y, e->rect.width);
			if (h >= 0) pivot_select(p, h, 1);
		}
		return 1;
	}
	case ZST_IN_CAPTURE_LOST:
		if (p->dragging) {
			p->dragging = 0;
			p->pressed = 0;
			pivot_release(p, width);
		}
		return 0;
	default:
		return 0;
	}
}

static void pivot_init(zrn_element_t* const e) {
	e->focusable = 0;
	e->observes_pointer = 1;
	e->clip_to_bounds = 1;
}

static void pivot_destroy(zrn_element_t* const e) {
	free(PIVOT(e)->items);
	PIVOT(e)->items = nullptr;
	free(PIVOT(e)->title);
	PIVOT(e)->title = nullptr;
}

static void pivot_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	const auto p = PIVOT(e);
	int i;
	for (i = 0; i < p->count; i++) {
		if (p->items[i] != child) continue;
		memmove(&p->items[i], &p->items[i + 1], (size_t)(p->count - i - 1) * sizeof *p->items);
		p->count--;
		if (i < p->selected) p->selected--;
		if (p->selected >= p->count) p->selected = p->count > 0 ? p->count - 1 : 0;
		p->offset = (float)p->selected;
		p->to = p->from = p->offset;
		p->sliding = 0;
		zrn_changed_layout(e);
		return;
	}
}

zrn_class_t zrn_class_pivot = {
	.base = &zrn_class_control, .name = "Pivot", .id = Z_CLASS_PIVOT, .size = sizeof(zrn_pivot_t),
	.init = pivot_init, .destroy = pivot_destroy, .measure = pivot_measure, .arrange = pivot_arrange, .paint = pivot_paint,
	.input = pivot_input, .tick = pivot_tick, .child_removed = pivot_child_removed
};


/* ============================================================================
 *  PivotItem
 * ============================================================================ */

static void pivot_item_init(zrn_element_t* const e) {
	const auto c = ZST_AS(zrn_control_t, e);
	/* PivotItem: Margin 19,22.5,19,0 (PivotItemMargin), the content stretches */
	e->margin = Zthickness(ITEM_MARGIN_LEFT, ITEM_MARGIN_TOP, ITEM_MARGIN_RIGHT, 0);
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
	e->focusable = 0;
}

static void pivot_item_destroy(zrn_element_t* const e) {
	free(PITEM(e)->header);
	PITEM(e)->header = nullptr;
}

zrn_class_t zrn_class_pivot_item = {
	.base = &zrn_class_content_control, .name = "PivotItem", .id = Z_CLASS_PIVOT_ITEM, .size = sizeof(zrn_pivot_item_t),
	.init = pivot_item_init, .destroy = pivot_item_destroy
};

Zid Zpivot_item_create(const char* const header) {
	zrn_element_t* const e = zrn_create(Z_CLASS_PIVOT_ITEM);
	if (!e) return Z_INVALID_ID;
	zrn_set_string(&PITEM(e)->header, header ? header : "");
	return e->id;
}

void Zpivot_item_set_header(const Zid id, const char* const header) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PIVOT_ITEM);
	if (!e) return;
	zrn_set_string(&PITEM(e)->header, header ? header : "");
	if (e->parent) zrn_changed_layout(e->parent);
}

const char* Zpivot_item_get_header(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PIVOT_ITEM);
	return e && PITEM(e)->header ? PITEM(e)->header : "";
}


/* ============================================================================
 *  Pivot, the functions
 * ============================================================================ */

Zid Zpivot_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_PIVOT);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_pivot_t* get_pivot(const Zid id) { return PIVOT(zrn_get(id, Z_CLASS_PIVOT)); }

void Zpivot_set_title(const Zid id, const char* const title) {
	zrn_pivot_t* const p = get_pivot(id);
	if (!p) return;
	zrn_set_string(&p->title, title ? title : "");
	zrn_changed_layout(&p->base.base);
}

const char* Zpivot_get_title(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p && p->title ? p->title : "";
}

int Zpivot_insert_item(Zid id, int index, Zid item) {
	zrn_pivot_t* const p = get_pivot(id);
	zrn_element_t* const c = item != Z_INVALID_ID ? zrn_get(item, Z_CLASS_PIVOT_ITEM) : nullptr;
	if (!p || !c) return -1;
	if (c->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "the PivotItem is in a pivot already");
		return -1;
	}
	if (index < 0 || index > p->count) index = p->count;
	if (p->count == p->cap) {
		const int cap = p->cap ? p->cap * 2 : 4;
		zrn_element_t** items = (zrn_element_t**)realloc(p->items, (size_t)cap * sizeof *items);
		if (!items) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		p->items = items;
		p->cap = cap;
	}
	memmove(&p->items[index + 1], &p->items[index], (size_t)(p->count - index) * sizeof *p->items);
	p->items[index] = c;
	p->count++;
	zrn_add_child(&p->base.base, c, -1);
	/* the first item is the chosen one; one that comes before it moves the chosen one along */
	if (p->count == 1) { p->selected = 0; p->offset = p->from = p->to = 0; }
	else if (index <= p->selected) { p->selected++; p->offset = p->from = p->to = (float)p->selected; p->sliding = 0; }
	zrn_changed_layout(&p->base.base);
	return index;
}

int Zpivot_add_item(const Zid id, const Zid item) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p ? Zpivot_insert_item(id, p->count, item) : -1;
}

void Zpivot_remove_item_at(const Zid id, const int index) {
	zrn_pivot_t* const p = get_pivot(id);
	if (!p || index < 0 || index >= p->count) return;
	zrn_destroy(p->items[index]);
}

void Zpivot_clear_items(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	while (p && p->count > 0) zrn_destroy(p->items[p->count - 1]);
}

int Zpivot_get_item_count(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p ? p->count : 0;
}

Zid Zpivot_get_item(const Zid id, const int index) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p && index >= 0 && index < p->count ? p->items[index]->id : Z_INVALID_ID;
}

Zid Zpivot_get_selected_item(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p && p->count > 0 ? p->items[p->selected]->id : Z_INVALID_ID;
}

void Zpivot_set_selected_index(const Zid id, const int index) {
	zrn_pivot_t* const p = get_pivot(id);
	if (p) pivot_select(p, index, 1);
}

int Zpivot_get_selected_index(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p && p->count > 0 ? p->selected : -1;
}

void Zpivot_set_is_locked(const Zid id, const int is_locked) {
	zrn_pivot_t* const p = get_pivot(id);
	if (!p) return;
	p->is_locked = is_locked != 0;
	zrn_changed_arrange(&p->base.base);
}

int Zpivot_get_is_locked(const Zid id) {
	const zrn_pivot_t* const p = get_pivot(id);
	return p ? p->is_locked : 0;
}

ZRect Zpivot_get_header_rect(const Zid id, const int index) {
	zrn_pivot_t* const p = get_pivot(id);
	header_place_t places[MAX_PLACES];
	int i, n;
	const ZRect none = { 0, 0, 0, 0 };
	if (!p || index < 0 || index >= p->count) return none;
	n = header_places(p, places, MAX_PLACES, p->base.base.rect.width);
	for (i = 0; i < n; i++) {
		if (places[i].item == index) {
			const ZRect r = header_rect_of(&places[i], p);
			return r;
		}
	}
	return none;
}

void Zpivot_on_selection_changed(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (get_pivot(id)) Zelement_on(id, Z_EVENT_SELECTION_CHANGED, callback, user_data);
}

void Zpivot_on_pivot_item_loading(const Zid id, const ZPivotItemCallback callback, void* const user_data) {
	zrn_pivot_t* const p = get_pivot(id);
	if (!p) return;
	p->loading = callback;
	p->loading_data = user_data;
}

void Zpivot_on_pivot_item_loaded(const Zid id, const ZPivotItemCallback callback, void* const user_data) {
	zrn_pivot_t* const p = get_pivot(id);
	if (!p) return;
	p->loaded = callback;
	p->loaded_data = user_data;
}
