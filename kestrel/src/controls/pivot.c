/**
 * Kestrel - Pivot and PivotItem, after the Windows Phone 8.1 template (generic.xaml of the phone SDK).
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
#define TITLE_FONT KST_81_FONT_LARGE
#define TITLE_X 19.0f
#define TITLE_TOP 38.0f
#define HEADER_MARGIN_LEFT 16.0f
#define HEADER_MARGIN_TOP (-6.5f)
#define HEADER_TEXT_X 19.0f
#define ITEM_MARGIN_LEFT 19.0f
#define ITEM_MARGIN_TOP 22.5f
#define ITEM_MARGIN_RIGHT 19.0f
#define SLIDE_MS 300.0f
#define DRAG_START 12.0f

typedef struct kst_pivot_item {
	kst_content_control_t base;
	char* header;
} kst_pivot_item_t;

typedef struct kst_pivot {
	kst_control_t base;
	char* title;
	kst_element_t** items;
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
	KPivotItemCallback loading, loaded;
	void *loading_data, *loaded_data;
} kst_pivot_t;

#define PIVOT(e) KST_AS(kst_pivot_t, e)
#define PITEM(e) KST_AS(kst_pivot_item_t, e)

/** PivotHeaderItemFontFamily, FontSize and CharacterSpacing of the theme resources of the root (an override applies). */
static kst_font_t header_font(const kst_pivot_t* p) {
	const kst_root_t* r = ((const kst_element_t*)p)->root;
	kst_font_t f = { K_FONT_SEMILIGHT, 57.0f, -25 };
	kst_res_font_family(r, K_RES_PIVOT_HEADER_ITEM_FONT_FAMILY, &f.family);
	kst_res_double(r, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE, &f.size);
	kst_res_int(r, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING, &f.spacing);
	return f;
}

static kst_font_t title_font(void) {
	kst_font_t f = { K_FONT_SEMIBOLD, TITLE_FONT, 0 };
	return f;
}

static const char* item_header(const kst_pivot_t* p, int i) {
	const char* h = PITEM(p->items[i])->header;
	return h ? h : "";
}

/** The width a header takes in the strip: its text and the 16 px of its margin. */
static float header_slot(const kst_pivot_t* p, int i) {
	const char* h = item_header(p, i);
	return kst_text_width(header_font(p), h, (int)strlen(h)) + HEADER_MARGIN_LEFT;
}

static float strip_length(const kst_pivot_t* p) {
	float l = 0;
	int i;
	for (i = 0; i < p->count; i++) l += header_slot(p, i);
	return l;
}

static int wrap_index(int index, int n) {
	int m = n > 0 ? index % n : 0;
	return m < 0 ? m + n : m;
}

/** `d` brought into [-n/2, n/2], a tie going forward. */
static float wrap_distance(float d, int n) {
	return d - (float)n * ceilf(d / (float)n - 0.5f);
}

/** How far the strip has gone round for a place between pages: the distance of the chosen header from the start of the ring. */
static float strip_shift(const kst_pivot_t* p, float offset) {
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

static float title_height(const kst_pivot_t* p) {
	return p->title && *p->title ? kst_font_line_height(title_font()) : 0;
}

/** Where the strip starts (the top of its panel) and how high it is: the line of the 57 px font, the margin and the padding cancel. */
static float strip_top(const kst_pivot_t* p) { return TITLE_TOP + title_height(p); }
static float strip_height(const kst_pivot_t* p) { return kst_font_line_height(header_font(p)); }
static float items_top(const kst_pivot_t* p) { return strip_top(p) + strip_height(p); }

/** A header as it stands in the strip: the item, the text's left edge, and how near it is to being the chosen one (0..1). */
typedef struct header_place {
	int item;
	float x, width, near;
} header_place_t;

static int header_places(const kst_pivot_t* p, header_place_t* out, int max, float view_width) {
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
			float slot = header_slot(p, i), x = HEADER_TEXT_X + c + (float)m * ring - shift;
			float r = (float)(i + m * n) - p->offset;
			c += slot;
			/* the header before the chosen one is not seen at rest: its text ends where the margin of the strip starts */
			if (x >= view_width || x + slot - HEADER_MARGIN_LEFT <= HEADER_TEXT_X - HEADER_MARGIN_LEFT + 0.5f) continue;
			if (count == max) return count;
			out[count].item = i;
			out[count].x = x;
			out[count].width = slot - HEADER_MARGIN_LEFT;
			out[count].near = kst_clampf(1.0f - fabsf(r), 0, 1);
			count++;
		}
	}
	return count;
}

#define MAX_PLACES 64

static KRect header_rect_of(const header_place_t* h, const kst_pivot_t* p) {
	return kst_rect(h->x, strip_top(p), h->width, strip_height(p));
}

static KSize pivot_measure(kst_element_t* e, KSize available) {
	kst_pivot_t* p = PIVOT(e);
	float w = available.width < K_INFINITE * 0.5f ? available.width : 480.0f;
	float h = available.height < K_INFINITE * 0.5f ? available.height : 800.0f;
	float top = items_top(p);
	int i;
	for (i = 0; i < p->count; i++) kst_measure(p->items[i], kst_size(w, kst_maxf(0, h - top)));
	return kst_size(w, h);
}

static KSize pivot_arrange(kst_element_t* e, KSize final) {
	kst_pivot_t* p = PIVOT(e);
	float top = items_top(p);
	int i, n = p->count;
	for (i = 0; i < n; i++) {
		kst_element_t* it = p->items[i];
		float d = wrap_distance((float)i - p->offset, n);
		/* a page is in view while the strip is less than a page away from it */
		it->visibility = fabsf(d) < 1.0f ? K_VISIBLE : K_COLLAPSED;
		it->hit_visible = fabsf(d) < 0.5f || (n == 1);
		kst_arrange(it, kst_rect(d * final.width, top, final.width, kst_maxf(0, final.height - top)));
	}
	return final;
}

static void pivot_paint(kst_element_t* e, kst_painter_t* painter) {
	kst_pivot_t* p = PIVOT(e);
	KColor fg = kst_ctl_foreground(&p->base), off = kst_theme_color(e, KST_TC_PHONE_DISABLED);
	header_place_t places[MAX_PLACES];
	int i, n = header_places(p, places, MAX_PLACES, e->rect.width);
	if (p->title && *p->title) kst_draw_string(painter, title_font(), p->title, (int)strlen(p->title), TITLE_X, TITLE_TOP, fg);
	for (i = 0; i < n; i++) {
		const char* h = item_header(p, places[i].item);
		float t = places[i].near;
		KColor c = { off.r + (fg.r - off.r) * t, off.g + (fg.g - off.g) * t, off.b + (fg.b - off.b) * t, off.a + (fg.a - off.a) * t };
		/* the margin top of -6.5 pulls the text up by as much as the padding pushes the box down: it starts where the strip does */
		kst_draw_string(painter, header_font(p), h, (int)strlen(h), places[i].x, strip_top(p) + HEADER_MARGIN_TOP, c);
	}
}

static void pivot_loaded_now(kst_pivot_t* p) {
	kst_element_t* e = &p->base.base;
	Kid id = e->id, item;
	if (!p->loaded_pending) return;
	p->loaded_pending = 0;
	if (!p->loaded || p->selected >= p->count) return;
	item = p->items[p->selected]->id;
	p->loaded(id, item, p->loaded_data);
}

/* The strip goes to a place on the ring; the item is the chosen one from here. */
static void pivot_go(kst_pivot_t* p, float to, int animate) {
	kst_element_t* e = &p->base.base;
	Kid id = e->id;
	int n = p->count, index = wrap_index((int)floorf(to + 0.5f), n), changed = index != p->selected;
	p->selected = index;
	p->from = p->offset;
	p->to = to;
	p->elapsed = 0;
	if (changed) p->loaded_pending = 1;
	if (changed && p->loading) {
		p->loading(id, p->items[index]->id, p->loading_data);
		if (!kst_lookup(id)) return;
		p = PIVOT(kst_lookup(id));
		e = &p->base.base;
	}
	if (animate && e->root && kst_animations_enabled(e->root) && p->offset != p->to) {
		p->sliding = 1;
		e->wants_tick = 1;
	} else {
		p->sliding = 0;
		p->offset = (float)index;
	}
	kst_changed_arrange(e);
	if (changed) {
		if (!kst_raise(e, K_EVENT_SELECTION_CHANGED)) return;
	}
	if (!p->sliding) pivot_loaded_now(p);
}

/* To an item by its index: the shortest way round. */
static void pivot_select(kst_pivot_t* p, int index, int animate) {
	if (p->count == 0) return;
	index = kst_mini(kst_maxi(index, 0), p->count - 1);
	pivot_go(p, p->offset + wrap_distance((float)index - p->offset, p->count), animate);
}

static void pivot_tick(kst_element_t* e, float dt) {
	kst_pivot_t* p = PIVOT(e);
	if (!p->sliding) {
		if (!p->dragging) e->wants_tick = 0;
		return;
	}
	p->elapsed += dt;
	if (p->elapsed >= SLIDE_MS) {
		p->offset = (float)p->selected;
		p->sliding = 0;
		if (!p->dragging) e->wants_tick = 0;
		kst_changed_arrange(e);
		pivot_loaded_now(p);
		return;
	}
	p->offset = p->from + (p->to - p->from) * kst_ease(K_EASE_OUT, p->elapsed / SLIDE_MS);
	kst_changed_arrange(e);
}

static int header_at(const kst_pivot_t* p, float x, float y, float view_width) {
	header_place_t places[MAX_PLACES];
	int i, n = header_places(p, places, MAX_PLACES, view_width);
	for (i = 0; i < n; i++) {
		KRect r = header_rect_of(&places[i], p);
		if (x >= r.x - HEADER_MARGIN_LEFT && x < r.x + r.width && y >= r.y + HEADER_MARGIN_TOP && y < r.y + r.height) return places[i].item;
	}
	return -1;
}

/* The finger lets go: to the next page when it went a fifth of a page or flicked, else back. */
static void pivot_release(kst_pivot_t* p, float width) {
	float dx = (p->offset - (float)p->selected) * -width;
	float target = (float)p->selected;
	if (dx < -width * 0.2f || p->velocity < -0.4f) target += 1.0f;
	else if (dx > width * 0.2f || p->velocity > 0.4f) target -= 1.0f;
	/* the slide goes from where the finger left the strip, back to the same page too */
	pivot_go(p, target, 1);
}

static void pivot_begin_drag(kst_pivot_t* p) {
	kst_element_t* e = &p->base.base;
	p->dragging = 1;
	p->sliding = 0;
	if (e->root) kst_set_capture(e->root, e);
	e->wants_tick = 1;
}

static int pivot_input(kst_element_t* e, kst_input_t* in) {
	kst_pivot_t* p = PIVOT(e);
	float width = e->rect.width > 0 ? e->rect.width : 1;
	int can_drag = p->count >= 2 && !p->is_locked;
	switch (in->type) {
	case KST_IN_PREVIEW_DOWN:
		p->pressed = 1;
		p->dragging = 0;
		p->press_x = p->last_x = in->x;
		p->press_y = in->y;
		p->velocity = 0;
		return 0;
	case KST_IN_PREVIEW_MOVE:
		if (!p->pressed || p->dragging || !e->root || e->root->capture == e || !can_drag) return 0;
		{
			float dx = in->x - p->press_x, dy = in->y - p->press_y, before = p->last_x;
			p->last_x = in->x;
			if (fabsf(dx) > DRAG_START && fabsf(dx) > fabsf(dy)) {
				/* a horizontal drag is the pivot's, whoever had the pointer loses it; its first move is the step from the last one */
				pivot_begin_drag(p);
				p->last_x = before;
			}
		}
		return 0;
	case KST_IN_PREVIEW_UP:
		return 0;
	case KST_IN_POINTER_DOWN:
		p->pressed = 1;
		p->dragging = 0;
		p->press_x = p->last_x = in->x;
		p->press_y = in->y;
		p->velocity = 0;
		if (e->root) kst_set_capture(e->root, e);
		return 1;
	case KST_IN_POINTER_MOVE:
		if (!p->pressed) return 0;
		if (!p->dragging) {
			float dx = in->x - p->press_x, dy = in->y - p->press_y, before = p->last_x;
			p->last_x = in->x;
			if (fabsf(dx) <= DRAG_START || fabsf(dx) <= fabsf(dy) || !can_drag) return 1;
			/* the move came up from an element that has the pointer: the drag is the pivot's from here, with the pointer */
			pivot_begin_drag(p);
			p->last_x = before;
		}
		{
			float dx = in->x - p->press_x;
			/* a page of rubber on each side: past it the next page would be the one chosen */
			p->offset = kst_clampf((float)p->selected - dx / width, (float)p->selected - 1.0f, (float)p->selected + 1.0f);
			p->velocity = p->velocity * 0.5f + ((in->x - p->last_x) / 16.0f) * 0.5f;
			p->last_x = in->x;
			kst_changed_arrange(e);
		}
		return 1;
	case KST_IN_POINTER_UP: {
		int was_dragging = p->dragging, tap = p->pressed && !p->dragging;
		float x = in->x, y = in->y;
		p->pressed = 0;
		p->dragging = 0;
		if (e->root) kst_release_capture(e->root, e);
		if (was_dragging) {
			pivot_release(p, width);
		} else if (tap && !p->is_locked) {
			int h = header_at(p, x, y, e->rect.width);
			if (h >= 0) pivot_select(p, h, 1);
		}
		return 1;
	}
	case KST_IN_CAPTURE_LOST:
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

static void pivot_init(kst_element_t* e) {
	e->focusable = 0;
	e->observes_pointer = 1;
	e->clip_to_bounds = 1;
}

static void pivot_destroy(kst_element_t* e) {
	free(PIVOT(e)->items);
	PIVOT(e)->items = NULL;
	free(PIVOT(e)->title);
	PIVOT(e)->title = NULL;
}

static void pivot_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_pivot_t* p = PIVOT(e);
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
		kst_changed_layout(e);
		return;
	}
}

kst_class_t kst_class_pivot = {
	.base = &kst_class_control, .name = "Pivot", .id = K_CLASS_PIVOT, .size = sizeof(kst_pivot_t),
	.init = pivot_init, .destroy = pivot_destroy, .measure = pivot_measure, .arrange = pivot_arrange, .paint = pivot_paint,
	.input = pivot_input, .tick = pivot_tick, .child_removed = pivot_child_removed
};


/* ============================================================================
 *  PivotItem
 * ============================================================================ */

static void pivot_item_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	/* PivotItem: Margin 19,22.5,19,0 (PivotItemMargin), the content stretches */
	e->margin = Kthickness(ITEM_MARGIN_LEFT, ITEM_MARGIN_TOP, ITEM_MARGIN_RIGHT, 0);
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
	e->focusable = 0;
}

static void pivot_item_destroy(kst_element_t* e) {
	free(PITEM(e)->header);
	PITEM(e)->header = NULL;
}

kst_class_t kst_class_pivot_item = {
	.base = &kst_class_content_control, .name = "PivotItem", .id = K_CLASS_PIVOT_ITEM, .size = sizeof(kst_pivot_item_t),
	.init = pivot_item_init, .destroy = pivot_item_destroy
};

Kid Kpivot_item_create(const char* header) {
	kst_element_t* e = kst_create(K_CLASS_PIVOT_ITEM);
	if (!e) return K_INVALID_ID;
	kst_set_string(&PITEM(e)->header, header ? header : "");
	return e->id;
}

void Kpivot_item_set_header(Kid id, const char* header) {
	kst_element_t* e = kst_get(id, K_CLASS_PIVOT_ITEM);
	if (!e) return;
	kst_set_string(&PITEM(e)->header, header ? header : "");
	if (e->parent) kst_changed_layout(e->parent);
}

const char* Kpivot_item_get_header(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PIVOT_ITEM);
	return e && PITEM(e)->header ? PITEM(e)->header : "";
}


/* ============================================================================
 *  Pivot, the functions
 * ============================================================================ */

Kid Kpivot_create(void) {
	kst_element_t* e = kst_create(K_CLASS_PIVOT);
	return e ? e->id : K_INVALID_ID;
}

static kst_pivot_t* get_pivot(Kid id) { return PIVOT(kst_get(id, K_CLASS_PIVOT)); }

void Kpivot_set_title(Kid id, const char* title) {
	kst_pivot_t* p = get_pivot(id);
	if (!p) return;
	kst_set_string(&p->title, title ? title : "");
	kst_changed_layout(&p->base.base);
}

const char* Kpivot_get_title(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	return p && p->title ? p->title : "";
}

int Kpivot_insert_item(Kid id, int index, Kid item) {
	kst_pivot_t* p = get_pivot(id);
	kst_element_t* c = item != K_INVALID_ID ? kst_get(item, K_CLASS_PIVOT_ITEM) : NULL;
	if (!p || !c) return -1;
	if (c->parent) {
		kst_error(K_ERROR_HAS_PARENT, "the PivotItem is in a pivot already");
		return -1;
	}
	if (index < 0 || index > p->count) index = p->count;
	if (p->count == p->cap) {
		int cap = p->cap ? p->cap * 2 : 4;
		kst_element_t** items = (kst_element_t**)realloc(p->items, (size_t)cap * sizeof *items);
		if (!items) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		p->items = items;
		p->cap = cap;
	}
	memmove(&p->items[index + 1], &p->items[index], (size_t)(p->count - index) * sizeof *p->items);
	p->items[index] = c;
	p->count++;
	kst_add_child(&p->base.base, c, -1);
	/* the first item is the chosen one; one that comes before it moves the chosen one along */
	if (p->count == 1) { p->selected = 0; p->offset = p->from = p->to = 0; }
	else if (index <= p->selected) { p->selected++; p->offset = p->from = p->to = (float)p->selected; p->sliding = 0; }
	kst_changed_layout(&p->base.base);
	return index;
}

int Kpivot_add_item(Kid id, Kid item) {
	kst_pivot_t* p = get_pivot(id);
	return p ? Kpivot_insert_item(id, p->count, item) : -1;
}

void Kpivot_remove_item_at(Kid id, int index) {
	kst_pivot_t* p = get_pivot(id);
	if (!p || index < 0 || index >= p->count) return;
	kst_destroy(p->items[index]);
}

void Kpivot_clear_items(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	while (p && p->count > 0) kst_destroy(p->items[p->count - 1]);
}

int Kpivot_get_item_count(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	return p ? p->count : 0;
}

Kid Kpivot_get_item(Kid id, int index) {
	kst_pivot_t* p = get_pivot(id);
	return p && index >= 0 && index < p->count ? p->items[index]->id : K_INVALID_ID;
}

Kid Kpivot_get_selected_item(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	return p && p->count > 0 ? p->items[p->selected]->id : K_INVALID_ID;
}

void Kpivot_set_selected_index(Kid id, int index) {
	kst_pivot_t* p = get_pivot(id);
	if (p) pivot_select(p, index, 1);
}

int Kpivot_get_selected_index(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	return p && p->count > 0 ? p->selected : -1;
}

void Kpivot_set_is_locked(Kid id, int is_locked) {
	kst_pivot_t* p = get_pivot(id);
	if (!p) return;
	p->is_locked = is_locked != 0;
	kst_changed_arrange(&p->base.base);
}

int Kpivot_get_is_locked(Kid id) {
	kst_pivot_t* p = get_pivot(id);
	return p ? p->is_locked : 0;
}

KRect Kpivot_get_header_rect(Kid id, int index) {
	kst_pivot_t* p = get_pivot(id);
	header_place_t places[MAX_PLACES];
	int i, n;
	KRect none = { 0, 0, 0, 0 };
	if (!p || index < 0 || index >= p->count) return none;
	n = header_places(p, places, MAX_PLACES, p->base.base.rect.width);
	for (i = 0; i < n; i++) {
		if (places[i].item == index) {
			KRect r = header_rect_of(&places[i], p);
			return r;
		}
	}
	return none;
}

void Kpivot_on_selection_changed(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_pivot(id)) Kelement_on(id, K_EVENT_SELECTION_CHANGED, callback, user_data);
}

void Kpivot_on_pivot_item_loading(Kid id, KPivotItemCallback callback, void* user_data) {
	kst_pivot_t* p = get_pivot(id);
	if (!p) return;
	p->loading = callback;
	p->loading_data = user_data;
}

void Kpivot_on_pivot_item_loaded(Kid id, KPivotItemCallback callback, void* user_data) {
	kst_pivot_t* p = get_pivot(id);
	if (!p) return;
	p->loaded = callback;
	p->loaded_data = user_data;
}
