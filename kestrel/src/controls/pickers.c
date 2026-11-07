/**
 * Kestrel - controls/pickers.c
 * LoopingSelector, DatePicker, TimePicker and their flyouts (DatePickerFlyout, TimePickerFlyout), after the
 * Windows Phone 8.1 templates (generic.xaml of the phone SDK).
 *
 * A LoopingSelector is a column of items that goes round, the chosen one in the middle: at rest only that
 * one shows, filled with the accent; while a finger is on the column the others come out as outlined boxes
 * (the Expanded state, 0.33 s). A drag has inertia and ends on an item. A DatePicker and a TimePicker are a
 * small header over a Button with the value; the Button shows a flyout that fills the screen: a title, the
 * selectors side by side, and Done and Cancel buttons at the bottom, as the bar of the shell has them.
 *
 * The calendar is the Gregorian one, in plain arithmetic: no libc time zones or locales. "Today" is read once, from the UTC clock.
 */

#include "../internal.h"

#include <time.h>

#define LS(e) KST_AS(kst_looping_selector_t, e)
#define PICKER(e) KST_AS(kst_picker_t, e)
#define PF(e) KST_AS(kst_picker_flyout_t, e)

/* generic.xaml, LoopingSelector and LoopingSelectorItem: Margin 5, a Border of 2.5, the item template's StackPanel margin 5,5,0,5 with
 * the TextBlocks of 45 px (spacing -24, margin 0.8,-6.5) and 17 px (margin 0.8,-0.8); the time picker's has a single 43 px line, margin 0.8,-5 */
#define ITEM_MARGIN 5.0f
#define ITEM_BORDER 2.5f
#define ITEM_PAD 5.0f
#define TEXT_X 0.8f
#define PRIMARY_SIZE 45.0f
#define PRIMARY_SPACING (-24)
#define PRIMARY_MARGIN (-6.5f)
#define SECONDARY_SIZE 17.0f
#define SECONDARY_MARGIN (-0.8f)
/** DatePickerSelectorThemeMinWidth. */
#define SELECTOR_MIN_WIDTH 64.0f
/** The Expanded transition. */
#define EXPAND_MS 330.0f
/** This library's: how far a finger must go to drag, how fast the inertia dies, how long the snap to an item takes. */
#define DRAG_START 10.0f
#define INERTIA_TAU 320.0f
#define SNAP_MS 160.0f
#define MIN_SPEED 0.0004f


/* ============================================================================
 *  the calendar
 * ============================================================================ */

static int is_leap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

static int days_in_month(int y, int m) {
	static const int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (m < 1) m = 1;
	if (m > 12) m = 12;
	return m == 2 && is_leap(y) ? 29 : days[m - 1];
}

/** 0 is Sunday. */
static int weekday_of(int y, int m, int d) {
	static const int t[12] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	if (m < 3) y--;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

/** The civil date of a day count from 1970-01-01 (the algorithm of Howard Hinnant's days_from_civil, inverted). */
static KDate date_from_days(long z) {
	long era, doe, yoe, y, doy, mp;
	KDate d;
	z += 719468;
	era = (z >= 0 ? z : z - 146096) / 146097;
	doe = z - era * 146097;
	yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
	y = yoe + era * 400;
	doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
	mp = (5 * doy + 2) / 153;
	d.day = (int)(doy - (153 * mp + 2) / 5 + 1);
	d.month = (int)(mp < 10 ? mp + 3 : mp - 9);
	d.year = (int)(y + (d.month <= 2));
	return d;
}

static KDate today(void) { return date_from_days((long)(time(NULL) / 86400)); }

static const char* const MONTHS[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
static const char* const WEEKDAYS[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

/** A date in [min_year, max_year] with a day that the month has. */
static KDate coerce_date(KDate d, int min_year, int max_year) {
	if (d.year < min_year) d.year = min_year;
	if (d.year > max_year) d.year = max_year;
	if (d.month < 1) d.month = 1;
	if (d.month > 12) d.month = 12;
	if (d.day < 1) d.day = 1;
	if (d.day > days_in_month(d.year, d.month)) d.day = days_in_month(d.year, d.month);
	return d;
}

static int same_date(KDate a, KDate b) { return a.year == b.year && a.month == b.month && a.day == b.day; }

static KTimeOfDay coerce_time(KTimeOfDay t) {
	t.hour = kst_mini(kst_maxi(t.hour, 0), 23);
	t.minute = kst_mini(kst_maxi(t.minute, 0), 59);
	return t;
}

static int same_time(KTimeOfDay a, KTimeOfDay b) { return a.hour == b.hour && a.minute == b.minute; }


/* ============================================================================
 *  LoopingSelector
 * ============================================================================ */

typedef struct looping_item {
	char* primary;
	char* secondary;
} looping_item_t;

typedef struct kst_looping_selector {
	kst_control_t base;
	looping_item_t* items;
	int count, cap, any_secondary;
	/** The item template always has the line of the secondary text, empty or not (the date flyout). */
	int reserve_secondary;
	int selected;
	int should_loop;
	float item_width, item_height;
	float primary_size, primary_margin;
	int primary_spacing;
	/** The place of the column, in items: the item in the middle is `pos`; with ShouldLoop it can go below 0 or past the count. */
	float pos;
	/** Items per ms while the inertia runs. */
	float velocity;
	int pressed, dragging, inertia, settling;
	float press_y, press_pos, last_y;
	float snap_from, snap_to, snap_elapsed;
	/** 0 at rest, 1 while a finger is on the column or it moves: how much of the other items shows. */
	float expand, expand_goal;
} kst_looping_selector_t;

static kst_font_t primary_font(const kst_looping_selector_t* s) {
	kst_font_t f = { K_FONT_SEMIBOLD, s->primary_size, s->primary_spacing };
	return f;
}

static kst_font_t secondary_font(void) {
	kst_font_t f = { K_FONT_SEMIBOLD, SECONDARY_SIZE, 0 };
	return f;
}

static float primary_height(const kst_looping_selector_t* s) { return kst_font_line_height(primary_font(s)) + 2 * s->primary_margin; }
static float secondary_height(void) { return kst_font_line_height(secondary_font()) + 2 * SECONDARY_MARGIN; }

/** The height of what an item holds, the StackPanel with its margin: the text and 5 above and below. */
static float content_height(const kst_looping_selector_t* s) {
	return ITEM_PAD + primary_height(s) + (s->any_secondary || s->reserve_secondary ? secondary_height() : 0) + ITEM_PAD;
}

static float item_cell_height(const kst_looping_selector_t* s) {
	if (s->item_height > 0) return s->item_height;
	return 2 * ITEM_MARGIN + 2 * ITEM_BORDER + content_height(s);
}

static float item_cell_width(const kst_looping_selector_t* s) {
	float w = 0;
	int i;
	if (s->item_width > 0) return s->item_width;
	for (i = 0; i < s->count; i++) {
		const looping_item_t* it = &s->items[i];
		float a = kst_text_width(primary_font(s), it->primary, (int)strlen(it->primary)), b = it->secondary ? kst_text_width(secondary_font(), it->secondary, (int)strlen(it->secondary)) : 0;
		w = kst_maxf(w, kst_maxf(a, b));
	}
	return kst_maxf(SELECTOR_MIN_WIDTH, 2 * ITEM_MARGIN + 2 * ITEM_BORDER + ITEM_PAD + TEXT_X + w);
}

static int wrap_index(int index, int n) {
	int m = n > 0 ? index % n : 0;
	return m < 0 ? m + n : m;
}

static KSize ls_measure(kst_element_t* e, KSize available) {
	kst_looping_selector_t* s = LS(e);
	float h = available.height < K_INFINITE * 0.5f ? available.height : 5 * item_cell_height(s);
	return kst_size(item_cell_width(s), h);
}

static void ls_paint(kst_element_t* e, kst_painter_t* p) {
	kst_looping_selector_t* s = LS(e);
	float w = e->rect.width, hh = e->rect.height, ch = item_cell_height(s), center = hh * 0.5f;
	KColor accent = kst_theme_color(e, KST_TC_ACCENT), over = kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT), low = kst_theme_color(e, KST_TC_TEXT_LOW);
	int base, half, j, n = s->count;
	if (n == 0 || ch <= 0) return;
	base = (int)floorf(s->pos + 0.5f);
	half = (int)ceilf(center / ch) + 1;
	for (j = -half; j <= half; j++) {
		int k = base + j, idx;
		float top, alpha;
		KRect box;
		int selected;
		const looping_item_t* it;
		if (!s->should_loop && (k < 0 || k >= n)) continue;
		idx = wrap_index(k, n);
		top = center + ((float)k - s->pos) * ch - ch * 0.5f;
		if (top >= hh || top + ch <= 0) continue;
		selected = idx == s->selected;
		alpha = selected ? 1.0f : s->expand;
		if (alpha <= 0.001f) continue;
		it = &s->items[idx];
		box = kst_rect(ITEM_MARGIN, top + ITEM_MARGIN, w - 2 * ITEM_MARGIN, ch - 2 * ITEM_MARGIN);
		{
			KColor text = selected ? over : kst_color_alpha(low, alpha), line = kst_color_alpha(low, alpha);
			/* the StackPanel sits at the bottom of the box (VerticalContentAlignment Bottom) */
			float inner_h = box.height - 2 * ITEM_BORDER, y0 = box.y + ITEM_BORDER + kst_maxf(0, inner_h - content_height(s));
			float x = box.x + ITEM_BORDER + ITEM_PAD + TEXT_X;
			if (selected) kst_fill_rect(p, box, accent);
			else kst_stroke_rect(p, box, Kthickness_uniform(ITEM_BORDER), line);
			kst_draw_string(p, primary_font(s), it->primary, (int)strlen(it->primary), x, y0 + ITEM_PAD + s->primary_margin, text);
			if (it->secondary) {
				kst_draw_string(p, secondary_font(), it->secondary, (int)strlen(it->secondary), x, y0 + ITEM_PAD + primary_height(s) + SECONDARY_MARGIN, text);
			}
		}
	}
}

static void ls_set_expand(kst_looping_selector_t* s, float goal) {
	kst_element_t* e = &s->base.base;
	s->expand_goal = goal;
	if (!e->root || !kst_animations_enabled(e->root)) {
		s->expand = goal;
		kst_changed_paint(e);
	} else {
		e->wants_tick = 1;
	}
}

/* The column has come to rest on an item: the item is the chosen one. */
static void ls_finish(kst_looping_selector_t* s, int target) {
	kst_element_t* e = &s->base.base;
	int n = s->count, index, changed;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	if (n == 0) return;
	index = s->should_loop ? wrap_index(target, n) : kst_mini(kst_maxi(target, 0), n - 1);
	changed = index != s->selected;
	s->selected = index;
	s->pos = (float)index;
	ls_set_expand(s, 0);
	kst_changed_paint(e);
	if (changed) kst_raise(e, K_EVENT_SELECTION_CHANGED);
}

static int ls_nearest(const kst_looping_selector_t* s, float pos) {
	int k = (int)floorf(pos + 0.5f);
	if (!s->should_loop) k = kst_mini(kst_maxi(k, 0), s->count - 1);
	return k;
}

/* To an item, which snaps in 160 ms when animations run. */
static void ls_snap_to(kst_looping_selector_t* s, int target) {
	kst_element_t* e = &s->base.base;
	s->inertia = 0;
	s->velocity = 0;
	if (e->root && kst_animations_enabled(e->root) && (float)target != s->pos) {
		s->settling = 1;
		s->snap_from = s->pos;
		s->snap_to = (float)target;
		s->snap_elapsed = 0;
		e->wants_tick = 1;
	} else {
		ls_finish(s, target);
	}
}

static void ls_tick(kst_element_t* e, float dt) {
	kst_looping_selector_t* s = LS(e);
	int busy = 0;
	if (s->inertia) {
		float old = s->pos;
		s->pos += s->velocity * dt;
		s->velocity *= expf(-dt / INERTIA_TAU);
		if (!s->should_loop && (s->pos < 0 || s->pos > (float)(s->count - 1))) {
			s->pos = kst_clampf(s->pos, 0, (float)(s->count - 1));
			s->velocity = 0;
		}
		kst_changed_paint(e);
		if (fabsf(s->velocity) < MIN_SPEED || s->pos == old) {
			ls_snap_to(s, ls_nearest(s, s->pos));
			if (!kst_lookup(e->id)) return;
		}
		busy = 1;
	} else if (s->settling) {
		s->snap_elapsed += dt;
		if (s->snap_elapsed >= SNAP_MS) {
			ls_finish(s, (int)floorf(s->snap_to + 0.5f));
		} else {
			s->pos = s->snap_from + (s->snap_to - s->snap_from) * kst_ease(K_EASE_OUT, s->snap_elapsed / SNAP_MS);
			kst_changed_paint(e);
		}
		busy = 1;
	}
	if (s->expand != s->expand_goal) {
		float step = dt / EXPAND_MS;
		s->expand = s->expand < s->expand_goal ? kst_minf(s->expand + step, s->expand_goal) : kst_maxf(s->expand - step, s->expand_goal);
		kst_changed_paint(e);
		busy = 1;
	}
	if (!busy && !s->pressed) e->wants_tick = 0;
}

/* A finger lets go of a drag: the inertia goes on, or, without animations, ends where it would have ended. */
static void ls_release(kst_looping_selector_t* s) {
	kst_element_t* e = &s->base.base;
	if (s->count == 0) return;
	if (e->root && kst_animations_enabled(e->root)) {
		if (fabsf(s->velocity) >= MIN_SPEED * 4) {
			s->inertia = 1;
			e->wants_tick = 1;
		} else {
			ls_snap_to(s, ls_nearest(s, s->pos));
		}
	} else {
		ls_finish(s, ls_nearest(s, s->pos + s->velocity * INERTIA_TAU));
	}
}

/* One item up or down, from the keys and the wheel. */
static void ls_step(kst_looping_selector_t* s, int direction) {
	int target;
	if (s->count == 0) return;
	target = (int)floorf(s->pos + 0.5f) + direction;
	if (!s->should_loop) target = kst_mini(kst_maxi(target, 0), s->count - 1);
	ls_set_expand(s, 1);
	ls_snap_to(s, target);
	/* with no animations the column is at rest already; with them it lets go of the items when the snap ends */
}

static int ls_input(kst_element_t* e, kst_input_t* in) {
	kst_looping_selector_t* s = LS(e);
	float ch = item_cell_height(s);
	switch (in->type) {
	case KST_IN_POINTER_DOWN:
		if (!kst_effectively_enabled(e) || s->count == 0) return 1;
		s->pressed = 1;
		s->dragging = 0;
		s->inertia = s->settling = 0;
		s->velocity = 0;
		s->press_y = s->last_y = in->y;
		s->press_pos = s->pos;
		if (e->root) kst_set_capture(e->root, e);
		e->wants_tick = 1;
		ls_set_expand(s, 1);
		return 1;
	case KST_IN_POINTER_MOVE:
		if (!s->pressed) return 0;
		if (!s->dragging) {
			if (fabsf(in->y - s->press_y) <= DRAG_START) return 1;
			s->dragging = 1;
		}
		s->pos = s->press_pos - (in->y - s->press_y) / ch;
		if (!s->should_loop) s->pos = kst_clampf(s->pos, -0.4f, (float)s->count - 0.6f);
		/* items per ms: the root does not give the time of a move, a step is taken for 16 ms */
		s->velocity = s->velocity * 0.5f + (-(in->y - s->last_y) / ch / 16.0f) * 0.5f;
		s->last_y = in->y;
		kst_changed_paint(e);
		return 1;
	case KST_IN_POINTER_UP: {
		int was_dragging = s->dragging, was_pressed = s->pressed;
		s->pressed = 0;
		s->dragging = 0;
		if (e->root) kst_release_capture(e->root, e);
		if (!was_pressed) return 0;
		if (was_dragging) {
			ls_release(s);
		} else {
			/* a tap on an item chooses it */
			int k = (int)floorf((in->y - e->rect.height * 0.5f) / ch + s->pos + 0.5f);
			if (!s->should_loop && (k < 0 || k >= s->count)) k = ls_nearest(s, s->pos);
			ls_snap_to(s, k);
		}
		return 1;
	}
	case KST_IN_CAPTURE_LOST:
		if (s->pressed) {
			int was_dragging = s->dragging;
			s->pressed = 0;
			s->dragging = 0;
			if (was_dragging) ls_release(s);
			else ls_snap_to(s, ls_nearest(s, s->pos));
		}
		return 0;
	case KST_IN_WHEEL:
		if (!kst_effectively_enabled(e) || s->count == 0) return 0;
		ls_step(s, in->delta > 0 ? -1 : 1);
		return 1;
	case KST_IN_KEY_DOWN:
		if (in->key == K_KEY_UP) { ls_step(s, -1); return 1; }
		if (in->key == K_KEY_DOWN) { ls_step(s, 1); return 1; }
		return 0;
	default:
		return 0;
	}
}

static void ls_init(kst_element_t* e) {
	kst_looping_selector_t* s = LS(e);
	/* the style: ShouldLoop True; IsTabStop is false in the pickers, the keys work while it has the focus */
	s->should_loop = 1;
	s->selected = -1;
	s->primary_size = PRIMARY_SIZE;
	s->primary_margin = PRIMARY_MARGIN;
	s->primary_spacing = PRIMARY_SPACING;
	e->focusable = 1;
	e->tab_stop = 0;
	e->clip_to_bounds = 1;
	e->observes_pointer = 0;
}

static void ls_clear(kst_looping_selector_t* s) {
	int i;
	for (i = 0; i < s->count; i++) {
		free(s->items[i].primary);
		free(s->items[i].secondary);
	}
	s->count = 0;
	s->any_secondary = 0;
}

static void ls_destroy(kst_element_t* e) {
	kst_looping_selector_t* s = LS(e);
	ls_clear(s);
	free(s->items);
	s->items = NULL;
}

kst_class_t kst_class_looping_selector = {
	.base = &kst_class_control, .name = "LoopingSelector", .id = K_CLASS_LOOPING_SELECTOR, .size = sizeof(kst_looping_selector_t),
	.init = ls_init, .destroy = ls_destroy, .measure = ls_measure, .paint = ls_paint, .input = ls_input, .tick = ls_tick
};

Kid Klooping_selector_create(void) {
	kst_element_t* e = kst_create(K_CLASS_LOOPING_SELECTOR);
	return e ? e->id : K_INVALID_ID;
}

static kst_looping_selector_t* get_ls(Kid id) { return LS(kst_get(id, K_CLASS_LOOPING_SELECTOR)); }

int Klooping_selector_add_item(Kid id, const char* primary_text, const char* secondary_text) {
	kst_looping_selector_t* s = get_ls(id);
	looping_item_t* it;
	if (!s) return -1;
	if (s->count == s->cap) {
		int cap = s->cap ? s->cap * 2 : 8;
		looping_item_t* items = (looping_item_t*)realloc(s->items, (size_t)cap * sizeof *items);
		if (!items) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		s->items = items;
		s->cap = cap;
	}
	it = &s->items[s->count];
	it->primary = kst_strdup(primary_text ? primary_text : "");
	it->secondary = secondary_text && *secondary_text ? kst_strdup(secondary_text) : NULL;
	if (it->secondary) s->any_secondary = 1;
	s->count++;
	if (s->count == 1) { s->selected = 0; s->pos = 0; }
	kst_changed_layout(&s->base.base);
	return s->count - 1;
}

void Klooping_selector_clear_items(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	if (!s) return;
	ls_clear(s);
	s->selected = -1;
	s->pos = 0;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	kst_changed_layout(&s->base.base);
}

int Klooping_selector_get_item_count(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	return s ? s->count : 0;
}

const char* Klooping_selector_get_item_text(Kid id, int index) {
	kst_looping_selector_t* s = get_ls(id);
	return s && index >= 0 && index < s->count ? s->items[index].primary : "";
}

const char* Klooping_selector_get_item_secondary_text(Kid id, int index) {
	kst_looping_selector_t* s = get_ls(id);
	return s && index >= 0 && index < s->count && s->items[index].secondary ? s->items[index].secondary : "";
}

void Klooping_selector_set_selected_index(Kid id, int index) {
	kst_looping_selector_t* s = get_ls(id);
	int changed;
	if (!s || s->count == 0) return;
	index = kst_mini(kst_maxi(index, 0), s->count - 1);
	changed = index != s->selected;
	s->selected = index;
	s->pos = (float)index;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	kst_changed_paint(&s->base.base);
	if (changed) kst_raise(&s->base.base, K_EVENT_SELECTION_CHANGED);
}

int Klooping_selector_get_selected_index(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	return s ? s->selected : -1;
}

void Klooping_selector_set_should_loop(Kid id, int should_loop) {
	kst_looping_selector_t* s = get_ls(id);
	if (!s) return;
	s->should_loop = should_loop != 0;
	s->pos = s->selected >= 0 ? (float)s->selected : 0;
	kst_changed_paint(&s->base.base);
}

int Klooping_selector_get_should_loop(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	return s ? s->should_loop : 0;
}

void Klooping_selector_set_item_width(Kid id, float width) {
	kst_looping_selector_t* s = get_ls(id);
	if (!s) return;
	s->item_width = width > 0 ? width : 0;
	kst_changed_layout(&s->base.base);
}

float Klooping_selector_get_item_width(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	return s ? item_cell_width(s) : 0;
}

void Klooping_selector_set_item_height(Kid id, float height) {
	kst_looping_selector_t* s = get_ls(id);
	if (!s) return;
	s->item_height = height > 0 ? height : 0;
	kst_changed_layout(&s->base.base);
}

float Klooping_selector_get_item_height(Kid id) {
	kst_looping_selector_t* s = get_ls(id);
	return s ? item_cell_height(s) : 0;
}

void Klooping_selector_on_selection_changed(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_ls(id)) Kelement_on(id, K_EVENT_SELECTION_CHANGED, callback, user_data);
}


/* ============================================================================
 *  the flyouts
 * ============================================================================ */

typedef struct kst_picker_flyout {
	kst_flyout_base_t base;
	char* title;
	kst_element_t *title_block, *selectors_grid, *done_button, *cancel_button;
	kst_element_t* selector[3];
	/** The picker (DatePicker, TimePicker) the flyout belongs to, K_INVALID_ID for none: it gives the title. */
	Kid owner;
	/** Set while the selectors are changed by the program. */
	int syncing;
	/* DatePickerFlyout */
	KDate date;
	int min_year, max_year;
	int day_visible, month_visible, year_visible;
	KDatePickedCallback date_picked;
	void* date_picked_data;
	/* TimePickerFlyout */
	KTimeOfDay time;
	int minute_increment;
	KClockIdentifier clock;
	KTimePickedCallback time_picked;
	void* time_picked_data;
} kst_picker_flyout_t;

/* DatePicker and TimePicker (below) */
struct kst_picker {
	kst_control_t base;
	char* header;
	/** The FlyoutButton of the template, and the flyout it shows (the button's child). */
	kst_element_t *button, *flyout;
	KDate date;
	int min_year, max_year;
	int day_visible, month_visible, year_visible;
	KDateChangedCallback date_changed;
	void* date_changed_data;
	KTimeOfDay time;
	int minute_increment;
	KClockIdentifier clock;
	KTimeChangedCallback time_changed;
	void* time_changed_data;
};

static void set_handler(kst_element_t* e, KEvent ev, KRoutedCallback cb, void* data) {
	e->handlers[ev - K_EVENT_CLICK].callback = cb;
	e->handlers[ev - K_EVENT_CLICK].user_data = data;
}

static kst_picker_flyout_t* flyout_of(Kid id) { return PF(kst_lookup(id)); }

static int selector_index(kst_element_t* sel) { return sel ? LS(sel)->selected : 0; }

static void selector_fill(kst_element_t* sel, int count, const char* (*text)(int index, char* buf, void* data), void* data) {
	char buf[32];
	int i;
	Klooping_selector_clear_items(sel->id);
	for (i = 0; i < count; i++) {
		const char* secondary = text(i, buf, data);
		Klooping_selector_add_item(sel->id, buf, secondary);
	}
}

static void flyout_set_title_text(kst_picker_flyout_t* f) {
	const char* t = f->title ? f->title : "";
	if (!f->title_block) return;
	Ktext_block_set_text(f->title_block->id, t);
	f->title_block->visibility = *t ? K_VISIBLE : K_COLLAPSED;
	kst_changed_layout(f->title_block);
}

/* ---- the date ---- */

static const char* month_text(int i, char* buf, void* data) { (void)data; snprintf(buf, 32, "%s", MONTHS[i]); return NULL; }
static const char* year_text(int i, char* buf, void* data) { snprintf(buf, 32, "%d", *(int*)data + i); return NULL; }
static const char* day_text(int i, char* buf, void* data) {
	KDate* d = (KDate*)data;
	snprintf(buf, 32, "%d", i + 1);
	return WEEKDAYS[weekday_of(d->year, d->month, i + 1)];
}

/* The days of the month the month and year selectors show now, with the chosen day kept as far as the month goes. */
static void date_fill_days(kst_picker_flyout_t* f, int year, int month, int day_index) {
	KDate d = { year, month, 1 };
	int n = days_in_month(year, month);
	selector_fill(f->selector[1], n, day_text, &d);
	Klooping_selector_set_selected_index(f->selector[1]->id, kst_mini(day_index, n - 1));
}

static void date_sync(kst_picker_flyout_t* f) {
	int years = f->max_year - f->min_year + 1;
	if (!f->selector[0] || years < 1) return;
	f->syncing = 1;
	selector_fill(f->selector[0], 12, month_text, NULL);
	Klooping_selector_set_selected_index(f->selector[0]->id, f->date.month - 1);
	selector_fill(f->selector[2], years, year_text, &f->min_year);
	Klooping_selector_set_selected_index(f->selector[2]->id, f->date.year - f->min_year);
	date_fill_days(f, f->date.year, f->date.month, f->date.day - 1);
	f->syncing = 0;
}

/* What the selectors show as a date. */
static KDate date_of_selectors(const kst_picker_flyout_t* f) {
	KDate d = { f->min_year + selector_index(f->selector[2]), selector_index(f->selector[0]) + 1, selector_index(f->selector[1]) + 1 };
	return coerce_date(d, f->min_year, f->max_year);
}

/* The month or the year turned: the days are those of the new month. */
static void date_selector_changed(Kid sender, void* data) {
	kst_picker_flyout_t* f = flyout_of((Kid)(uintptr_t)data);
	KST_UNUSED(sender);
	if (!f || f->syncing || sender == f->selector[1]->id) return;
	f->syncing = 1;
	date_fill_days(f, f->min_year + selector_index(f->selector[2]), selector_index(f->selector[0]) + 1, selector_index(f->selector[1]));
	f->syncing = 0;
}

/* ---- the time ---- */

static const char* hour_text(int i, char* buf, void* data) {
	snprintf(buf, 32, "%d", *(int*)data == K_CLOCK_12_HOUR ? i + 1 : i);
	return NULL;
}
static const char* minute_text(int i, char* buf, void* data) { snprintf(buf, 32, "%02d", i * *(int*)data); return NULL; }
static const char* period_text(int i, char* buf, void* data) { (void)data; snprintf(buf, 32, "%s", i ? "PM" : "AM"); return NULL; }

static int hour_index(KTimeOfDay t, KClockIdentifier clock) {
	if (clock == K_CLOCK_24_HOUR) return t.hour;
	return (t.hour % 12 == 0 ? 12 : t.hour % 12) - 1;
}

static void time_sync(kst_picker_flyout_t* f) {
	int clock = (int)f->clock, inc = f->minute_increment, minutes = (59 / inc) + 1;
	if (!f->selector[0]) return;
	f->syncing = 1;
	selector_fill(f->selector[0], clock == K_CLOCK_12_HOUR ? 12 : 24, hour_text, &clock);
	Klooping_selector_set_selected_index(f->selector[0]->id, hour_index(f->time, f->clock));
	selector_fill(f->selector[1], minutes, minute_text, &inc);
	/* a minute that is not on the grid of MinuteIncrement is shown on the nearest one that is */
	Klooping_selector_set_selected_index(f->selector[1]->id, (f->time.minute + inc / 2) / inc >= minutes ? minutes - 1 : (f->time.minute + inc / 2) / inc);
	selector_fill(f->selector[2], 2, period_text, NULL);
	Klooping_selector_set_selected_index(f->selector[2]->id, f->time.hour >= 12);
	Klooping_selector_set_should_loop(f->selector[2]->id, 0);
	f->selector[2]->visibility = clock == K_CLOCK_12_HOUR ? K_VISIBLE : K_COLLAPSED;
	f->syncing = 0;
	kst_changed_layout(f->selectors_grid);
}

static KTimeOfDay time_of_selectors(const kst_picker_flyout_t* f) {
	KTimeOfDay t;
	int h = selector_index(f->selector[0]);
	if (f->clock == K_CLOCK_24_HOUR) t.hour = h;
	else t.hour = ((h + 1) % 12) + (selector_index(f->selector[2]) ? 12 : 0);
	t.minute = selector_index(f->selector[1]) * f->minute_increment;
	return coerce_time(t);
}

/* ---- both ---- */

static void flyout_sync(kst_picker_flyout_t* f) {
	if (f->base.base.cls == &kst_class_date_picker_flyout) date_sync(f);
	else time_sync(f);
}

typedef struct kst_picker kst_picker_t;
static void date_picker_set(kst_picker_t* p, KDate date, int raise);
static void time_picker_set(kst_picker_t* p, KTimeOfDay t, int raise);

/* Done: the selectors are the value; the handler hears of it, and the flyout goes. */
static void flyout_done(Kid button, void* data) {
	kst_picker_flyout_t* f = flyout_of((Kid)(uintptr_t)data);
	Kid id = (Kid)(uintptr_t)data;
	KST_UNUSED(button);
	if (!f) return;
	if (f->base.base.cls == &kst_class_date_picker_flyout) {
		KDate old = f->date;
		KDatePickedCallback cb = f->date_picked;
		void* user = f->date_picked_data;
		f->date = date_of_selectors(f);
		if (cb) cb(id, old, f->date, user);
		/* the picker the flyout belongs to takes the date */
		{
			kst_element_t* o = kst_lookup(f->owner);
			if (kst_lookup(id) && o && o->cls == &kst_class_date_picker) date_picker_set(PICKER(o), f->date, 1);
		}
	} else {
		KTimeOfDay old = f->time;
		KTimePickedCallback cb = f->time_picked;
		void* user = f->time_picked_data;
		f->time = time_of_selectors(f);
		if (cb) cb(id, old, f->time, user);
		{
			kst_element_t* o = kst_lookup(f->owner);
			if (kst_lookup(id) && o && o->cls == &kst_class_time_picker) time_picker_set(PICKER(o), f->time, 1);
		}
	}
	f = flyout_of(id);
	if (f && f->base.is_open && f->base.popup) kst_popup_close(f->base.popup);
}

static void flyout_cancel(Kid button, void* data) {
	Kid id = (Kid)(uintptr_t)data;
	KST_UNUSED(button);
	Kflyout_base_hide(id);
}

static void picker_header_of(Kid owner, kst_picker_flyout_t* f);

/* About to show: the selectors begin on the value, and the title is the header of the picker. */
static void flyout_on_show(kst_element_t* e) {
	kst_picker_flyout_t* f = PF(e);
	picker_header_of(f->owner, f);
	flyout_set_title_text(f);
	flyout_sync(f);
}

/* Dismissed: what the selectors show goes back to the value. */
static void flyout_on_hide(kst_element_t* e) { flyout_sync(PF(e)); }

static kst_element_t* make_bar_button(const char* label, KSymbol symbol, kst_picker_flyout_t* f) {
	Kid icon = Ksymbol_icon_create(symbol), b = Kapp_bar_button_create(label, icon);
	kst_element_t* e = kst_lookup(b);
	if (e) {
		Kelement_set_horizontal_alignment(b, K_HALIGN_CENTER);
		Kbutton_on_click(b, symbol == K_SYMBOL_ACCEPT ? flyout_done : flyout_cancel, (void*)(uintptr_t)f->base.base.id);
	}
	return e;
}

/* The presenter's content: the title, the selectors in the middle and the bar of Done and Cancel at the bottom. */
static void picker_flyout_init(kst_element_t* e) {
	kst_picker_flyout_t* f = PF(e);
	kst_element_t* presenter = f->base.popup && f->base.popup->child_count ? f->base.popup->children[0] : NULL;
	Kid grid, inner, title, bar;
	int i;
	f->base.placement = K_FLYOUT_PLACEMENT_FULL;
	f->base.on_show = flyout_on_show;
	f->base.on_hide = flyout_on_hide;
	f->owner = K_INVALID_ID;
	f->day_visible = f->month_visible = f->year_visible = 1;
	f->minute_increment = 1;
	f->clock = K_CLOCK_12_HOUR;
	if (!presenter) return;
	/* DateTimeFlyoutContentPanelPortraitThemeMargin 0,37,0,0 and DateTimeFlyoutTitleThemeMargin 19,0,19,17.5; the title is the TitleTextBlockStyle */
	grid = Kgrid_create();
	Kgrid_add_row(grid, K_GRID_AUTO, 0);
	Kgrid_add_row(grid, K_GRID_STAR, 1);
	Kgrid_add_row(grid, K_GRID_AUTO, 0);
	Kelement_set_margin(grid, Kthickness(0, 37, 0, 0));
	title = Ktext_block_create("");
	Ktext_block_set_style(title, K_TEXT_STYLE_TITLE);
	Kelement_set_margin(title, Kthickness(19, 0, 19, 17.5f));
	Kgrid_place(grid, title, 0, 0);
	kst_lookup(title)->visibility = K_COLLAPSED;
	f->title_block = kst_lookup(title);
	inner = Kgrid_create();
	Kgrid_add_column(inner, K_GRID_STAR, 1);
	Kgrid_add_column(inner, K_GRID_AUTO, 0);
	Kgrid_add_column(inner, K_GRID_AUTO, 0);
	Kgrid_add_column(inner, K_GRID_AUTO, 0);
	Kgrid_add_column(inner, K_GRID_STAR, 1);
	for (i = 0; i < 3; i++) {
		Kid sel = Klooping_selector_create();
		Kgrid_place(inner, sel, i + 1, 0);
		f->selector[i] = kst_lookup(sel);
	}
	f->selectors_grid = kst_lookup(inner);
	Kgrid_place(grid, inner, 0, 1);
	/* the bar: Done and Cancel, each in half the width */
	bar = Kgrid_create();
	Kgrid_add_column(bar, K_GRID_STAR, 1);
	Kgrid_add_column(bar, K_GRID_STAR, 1);
	f->done_button = make_bar_button("done", K_SYMBOL_ACCEPT, f);
	f->cancel_button = make_bar_button("cancel", K_SYMBOL_CANCEL, f);
	if (f->done_button) Kgrid_place(bar, f->done_button->id, 0, 0);
	if (f->cancel_button) Kgrid_place(bar, f->cancel_button->id, 1, 0);
	Kgrid_place(grid, bar, 0, 2);
	Kborder_set_child(presenter->id, grid);
}

static void picker_flyout_destroy(kst_element_t* e) {
	free(PF(e)->title);
	PF(e)->title = NULL;
}

kst_class_t kst_class_picker_flyout_base = {
	.base = &kst_class_flyout_base, .name = "PickerFlyoutBase", .id = K_CLASS_PICKER_FLYOUT_BASE, .size = sizeof(kst_picker_flyout_t), .abstract_class = 1,
	.init = picker_flyout_init, .destroy = picker_flyout_destroy
};

static void date_flyout_init(kst_element_t* e) {
	kst_picker_flyout_t* f = PF(e);
	KDate t = today();
	int i;
	f->date = t;
	f->min_year = t.year - 100;
	f->max_year = t.year + 100;
	for (i = 0; i < 3; i++) {
		if (!f->selector[i]) return;
		LS(f->selector[i])->reserve_secondary = 1;
		set_handler(f->selector[i], K_EVENT_SELECTION_CHANGED, date_selector_changed, (void*)(uintptr_t)e->id);
	}
	/* the years do not go round */
	Klooping_selector_set_should_loop(f->selector[2]->id, 0);
	date_sync(f);
}

static void time_flyout_init(kst_element_t* e) {
	kst_picker_flyout_t* f = PF(e);
	int i;
	f->time.hour = 0;
	f->time.minute = 0;
	for (i = 0; i < 3; i++) {
		if (!f->selector[i]) return;
		/* TimePickerFlyoutPresenter's item: one 43 px line, margin 0.8,-5, the weight of the presenter */
		LS(f->selector[i])->primary_size = 43.0f;
		LS(f->selector[i])->primary_margin = -5.0f;
		LS(f->selector[i])->primary_spacing = 0;
	}
	time_sync(f);
}

kst_class_t kst_class_date_picker_flyout = {
	.base = &kst_class_picker_flyout_base, .name = "DatePickerFlyout", .id = K_CLASS_DATE_PICKER_FLYOUT, .size = sizeof(kst_picker_flyout_t),
	.init = date_flyout_init
};

kst_class_t kst_class_time_picker_flyout = {
	.base = &kst_class_picker_flyout_base, .name = "TimePickerFlyout", .id = K_CLASS_TIME_PICKER_FLYOUT, .size = sizeof(kst_picker_flyout_t),
	.init = time_flyout_init
};

static kst_picker_flyout_t* get_flyout(Kid id) { return PF(kst_get(id, K_CLASS_PICKER_FLYOUT_BASE)); }
static kst_picker_flyout_t* get_date_flyout(Kid id) { return PF(kst_get(id, K_CLASS_DATE_PICKER_FLYOUT)); }
static kst_picker_flyout_t* get_time_flyout(Kid id) { return PF(kst_get(id, K_CLASS_TIME_PICKER_FLYOUT)); }

void Kpicker_flyout_base_set_title(Kid id, const char* title) {
	kst_picker_flyout_t* f = get_flyout(id);
	if (!f) return;
	kst_set_string(&f->title, title ? title : "");
	flyout_set_title_text(f);
}

const char* Kpicker_flyout_base_get_title(Kid id) {
	kst_picker_flyout_t* f = get_flyout(id);
	return f && f->title ? f->title : "";
}

Kid Kpicker_flyout_base_get_selector(Kid id, int index) {
	kst_picker_flyout_t* f = get_flyout(id);
	return f && index >= 0 && index < 3 && f->selector[index] ? f->selector[index]->id : K_INVALID_ID;
}

Kid Kpicker_flyout_base_get_done_button(Kid id) {
	kst_picker_flyout_t* f = get_flyout(id);
	return f && f->done_button ? f->done_button->id : K_INVALID_ID;
}

Kid Kpicker_flyout_base_get_cancel_button(Kid id) {
	kst_picker_flyout_t* f = get_flyout(id);
	return f && f->cancel_button ? f->cancel_button->id : K_INVALID_ID;
}

Kid Kdate_picker_flyout_create(void) {
	kst_element_t* e = kst_create(K_CLASS_DATE_PICKER_FLYOUT);
	return e ? e->id : K_INVALID_ID;
}

void Kdate_picker_flyout_set_date(Kid id, KDate date) {
	kst_picker_flyout_t* f = get_date_flyout(id);
	if (!f) return;
	f->date = coerce_date(date, f->min_year, f->max_year);
	date_sync(f);
}

KDate Kdate_picker_flyout_get_date(Kid id) {
	kst_picker_flyout_t* f = get_date_flyout(id);
	KDate none = { 0, 0, 0 };
	return f ? f->date : none;
}

void Kdate_picker_flyout_set_min_year(Kid id, int year) {
	kst_picker_flyout_t* f = get_date_flyout(id);
	if (!f) return;
	f->min_year = year;
	if (f->max_year < year) f->max_year = year;
	f->date = coerce_date(f->date, f->min_year, f->max_year);
	date_sync(f);
}

void Kdate_picker_flyout_set_max_year(Kid id, int year) {
	kst_picker_flyout_t* f = get_date_flyout(id);
	if (!f) return;
	f->max_year = year;
	if (f->min_year > year) f->min_year = year;
	f->date = coerce_date(f->date, f->min_year, f->max_year);
	date_sync(f);
}

int Kdate_picker_flyout_get_min_year(Kid id) { kst_picker_flyout_t* f = get_date_flyout(id); return f ? f->min_year : 0; }
int Kdate_picker_flyout_get_max_year(Kid id) { kst_picker_flyout_t* f = get_date_flyout(id); return f ? f->max_year : 0; }

static void date_apply_visibility(kst_picker_flyout_t* f) {
	f->selector[0]->visibility = f->month_visible ? K_VISIBLE : K_COLLAPSED;
	f->selector[1]->visibility = f->day_visible ? K_VISIBLE : K_COLLAPSED;
	f->selector[2]->visibility = f->year_visible ? K_VISIBLE : K_COLLAPSED;
	kst_changed_layout(f->selectors_grid);
}

void Kdate_picker_flyout_set_day_visible(Kid id, int visible) { kst_picker_flyout_t* f = get_date_flyout(id); if (f) { f->day_visible = visible != 0; date_apply_visibility(f); } }
void Kdate_picker_flyout_set_month_visible(Kid id, int visible) { kst_picker_flyout_t* f = get_date_flyout(id); if (f) { f->month_visible = visible != 0; date_apply_visibility(f); } }
void Kdate_picker_flyout_set_year_visible(Kid id, int visible) { kst_picker_flyout_t* f = get_date_flyout(id); if (f) { f->year_visible = visible != 0; date_apply_visibility(f); } }
int Kdate_picker_flyout_get_day_visible(Kid id) { kst_picker_flyout_t* f = get_date_flyout(id); return f ? f->day_visible : 0; }
int Kdate_picker_flyout_get_month_visible(Kid id) { kst_picker_flyout_t* f = get_date_flyout(id); return f ? f->month_visible : 0; }
int Kdate_picker_flyout_get_year_visible(Kid id) { kst_picker_flyout_t* f = get_date_flyout(id); return f ? f->year_visible : 0; }

void Kdate_picker_flyout_on_date_picked(Kid id, KDatePickedCallback callback, void* user_data) {
	kst_picker_flyout_t* f = get_date_flyout(id);
	if (!f) return;
	f->date_picked = callback;
	f->date_picked_data = user_data;
}

Kid Ktime_picker_flyout_create(void) {
	kst_element_t* e = kst_create(K_CLASS_TIME_PICKER_FLYOUT);
	return e ? e->id : K_INVALID_ID;
}

void Ktime_picker_flyout_set_time(Kid id, KTimeOfDay time_of_day) {
	kst_picker_flyout_t* f = get_time_flyout(id);
	if (!f) return;
	f->time = coerce_time(time_of_day);
	time_sync(f);
}

KTimeOfDay Ktime_picker_flyout_get_time(Kid id) {
	kst_picker_flyout_t* f = get_time_flyout(id);
	KTimeOfDay none = { 0, 0 };
	return f ? f->time : none;
}

void Ktime_picker_flyout_set_minute_increment(Kid id, int increment) {
	kst_picker_flyout_t* f = get_time_flyout(id);
	if (!f) return;
	f->minute_increment = kst_mini(kst_maxi(increment, 1), 59);
	time_sync(f);
}

int Ktime_picker_flyout_get_minute_increment(Kid id) { kst_picker_flyout_t* f = get_time_flyout(id); return f ? f->minute_increment : 1; }

void Ktime_picker_flyout_set_clock_identifier(Kid id, KClockIdentifier clock) {
	kst_picker_flyout_t* f = get_time_flyout(id);
	if (!f || (clock != K_CLOCK_12_HOUR && clock != K_CLOCK_24_HOUR)) return;
	f->clock = clock;
	time_sync(f);
}

KClockIdentifier Ktime_picker_flyout_get_clock_identifier(Kid id) { kst_picker_flyout_t* f = get_time_flyout(id); return f ? f->clock : K_CLOCK_12_HOUR; }

void Ktime_picker_flyout_on_time_picked(Kid id, KTimePickedCallback callback, void* user_data) {
	kst_picker_flyout_t* f = get_time_flyout(id);
	if (!f) return;
	f->time_picked = callback;
	f->time_picked_data = user_data;
}


/* ============================================================================
 *  DatePicker, TimePicker
 * ============================================================================ */


/* HeaderContentPresenterStyle: Normal, TextStyleMediumFontSize 16, PhoneMid, margin 0,0,0,-3 */
static kst_font_t header_font(void) {
	kst_font_t f = { K_FONT_NORMAL, KST_81_FONT_CONTROL_HEADER, 0 };
	return f;
}

static float header_height(const kst_picker_t* p) {
	return p->header && *p->header ? kst_font_line_height(header_font()) - 3.0f : 0;
}

static void picker_header_of(Kid owner, kst_picker_flyout_t* f) {
	kst_element_t* o = owner != K_INVALID_ID ? kst_lookup(owner) : NULL;
	if (o && (o->cls == &kst_class_date_picker || o->cls == &kst_class_time_picker) && PICKER(o)->header && *PICKER(o)->header)
		kst_set_string(&f->title, PICKER(o)->header);
}

static void picker_text(const kst_picker_t* p, int is_date, char* buf, size_t size) {
	if (is_date) {
		snprintf(buf, size, "%s %d, %d", MONTHS[p->date.month - 1], p->date.day, p->date.year);
	} else if (p->clock == K_CLOCK_24_HOUR) {
		snprintf(buf, size, "%d:%02d", p->time.hour, p->time.minute);
	} else {
		snprintf(buf, size, "%d:%02d %s", p->time.hour % 12 == 0 ? 12 : p->time.hour % 12, p->time.minute, p->time.hour >= 12 ? "PM" : "AM");
	}
}

static void picker_refresh_button(kst_picker_t* p) {
	char buf[64];
	picker_text(p, p->base.base.cls == &kst_class_date_picker, buf, sizeof buf);
	if (p->button) Kcontent_control_set_text(p->button->id, buf);
}

static KSize picker_measure(kst_element_t* e, KSize available) {
	kst_picker_t* p = PICKER(e);
	kst_control_t* c = &p->base;
	float w = available.width < K_INFINITE * 0.5f ? available.width : 480.0f;
	float pad_h = kst_thickness_h(c->padding), h = c->padding.top + c->padding.bottom + header_height(p);
	if (p->button) {
		/* the Foreground of the picker is what the button draws and outlines in */
		if (c->set_flags & KST_C_FOREGROUND) Kcontrol_set_foreground(p->button->id, c->foreground);
		kst_measure(p->button, kst_size(kst_maxf(0, w - pad_h), K_INFINITE));
		h += p->button->desired.height;
	}
	return kst_size(w, h);
}

static KSize picker_arrange(kst_element_t* e, KSize final) {
	kst_picker_t* p = PICKER(e);
	kst_control_t* c = &p->base;
	if (p->button) {
		kst_arrange(p->button, kst_rect(c->padding.left, c->padding.top + header_height(p), kst_maxf(0, final.width - kst_thickness_h(c->padding)), p->button->desired.height));
	}
	return final;
}

static void picker_paint(kst_element_t* e, kst_painter_t* painter) {
	kst_picker_t* p = PICKER(e);
	kst_control_t* c = &p->base;
	if (!p->header || !*p->header) return;
	kst_draw_string(painter, header_font(), p->header, (int)strlen(p->header), c->padding.left, c->padding.top,
		kst_effectively_enabled(e) ? kst_theme_color(e, KST_TC_TEXT_MID) : kst_theme_color(e, KST_TC_PHONE_DISABLED));
}

static void picker_init(kst_element_t* e) {
	kst_picker_t* p = PICKER(e);
	int is_date = e->cls == &kst_class_date_picker;
	Kid button = Kbutton_create(""), flyout = is_date ? Kdate_picker_flyout_create() : Ktime_picker_flyout_create();
	kst_element_t* b = kst_lookup(button);
	e->focusable = 0;
	p->day_visible = p->month_visible = p->year_visible = 1;
	p->minute_increment = 1;
	p->clock = K_CLOCK_12_HOUR;
	{
		KDate t = today();
		p->date = t;
		p->min_year = t.year - 100;
		p->max_year = t.year + 100;
	}
	if (!b) return;
	/* FlyoutButton: BorderThickness 2.5 (the Button's own), Padding 6.5,0,0,3, HorizontalAlignment Stretch, content at the left */
	Kcontrol_set_padding(button, Kthickness(6.5f, 0, 0, 3));
	Kelement_set_horizontal_alignment(button, K_HALIGN_STRETCH);
	Kcontrol_set_horizontal_content_alignment(button, K_HALIGN_LEFT);
	kst_add_child(e, b, -1);
	p->button = b;
	if (kst_lookup(flyout)) {
		kst_picker_flyout_t* f = PF(kst_lookup(flyout));
		p->flyout = kst_lookup(flyout);
		f->owner = e->id;
		if (is_date) f->date = p->date;
		Kbutton_set_flyout(button, flyout);
	}
	picker_refresh_button(p);
}

static void picker_destroy(kst_element_t* e) {
	free(PICKER(e)->header);
	PICKER(e)->header = NULL;
}

static void picker_child_removed(kst_element_t* e, kst_element_t* child) {
	if (PICKER(e)->button == child) {
		PICKER(e)->button = NULL;
		PICKER(e)->flyout = NULL;
	}
}

kst_class_t kst_class_date_picker = {
	.base = &kst_class_control, .name = "DatePicker", .id = K_CLASS_DATE_PICKER, .size = sizeof(kst_picker_t),
	.init = picker_init, .destroy = picker_destroy, .measure = picker_measure, .arrange = picker_arrange, .paint = picker_paint,
	.child_removed = picker_child_removed
};

kst_class_t kst_class_time_picker = {
	.base = &kst_class_control, .name = "TimePicker", .id = K_CLASS_TIME_PICKER, .size = sizeof(kst_picker_t),
	.init = picker_init, .destroy = picker_destroy, .measure = picker_measure, .arrange = picker_arrange, .paint = picker_paint,
	.child_removed = picker_child_removed
};

static kst_picker_t* get_date_picker(Kid id) { return PICKER(kst_get(id, K_CLASS_DATE_PICKER)); }
static kst_picker_t* get_time_picker(Kid id) { return PICKER(kst_get(id, K_CLASS_TIME_PICKER)); }

/* The flyout of a picker and the picker agree on the value. */
static void date_picker_set(kst_picker_t* p, KDate date, int raise) {
	kst_element_t* e = &p->base.base;
	Kid id = e->id;
	KDate old = p->date;
	p->date = coerce_date(date, p->min_year, p->max_year);
	if (p->flyout) {
		kst_picker_flyout_t* f = PF(p->flyout);
		f->min_year = p->min_year;
		f->max_year = p->max_year;
		f->date = p->date;
		date_sync(f);
	}
	picker_refresh_button(p);
	kst_changed_layout(e);
	if (raise && !same_date(old, p->date) && p->date_changed) {
		KDateChangedCallback cb = p->date_changed;
		void* user = p->date_changed_data;
		cb(id, old, p->date, user);
	}
}

static void time_picker_set(kst_picker_t* p, KTimeOfDay t, int raise) {
	kst_element_t* e = &p->base.base;
	Kid id = e->id;
	KTimeOfDay old = p->time;
	p->time = coerce_time(t);
	if (p->flyout) {
		kst_picker_flyout_t* f = PF(p->flyout);
		f->minute_increment = p->minute_increment;
		f->clock = p->clock;
		f->time = p->time;
		time_sync(f);
	}
	picker_refresh_button(p);
	kst_changed_layout(e);
	if (raise && !same_time(old, p->time) && p->time_changed) {
		KTimeChangedCallback cb = p->time_changed;
		void* user = p->time_changed_data;
		cb(id, old, p->time, user);
	}
}

Kid Kdate_picker_create(void) {
	kst_element_t* e = kst_create(K_CLASS_DATE_PICKER);
	if (!e) return K_INVALID_ID;
	return e->id;
}

Kid Ktime_picker_create(void) {
	kst_element_t* e = kst_create(K_CLASS_TIME_PICKER);
	if (!e) return K_INVALID_ID;
	return e->id;
}

void Kdate_picker_set_header(Kid id, const char* header) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	kst_set_string(&p->header, header ? header : "");
	kst_changed_layout(&p->base.base);
}

const char* Kdate_picker_get_header(Kid id) {
	kst_picker_t* p = get_date_picker(id);
	return p && p->header ? p->header : "";
}

void Kdate_picker_set_date(Kid id, KDate date) {
	kst_picker_t* p = get_date_picker(id);
	if (p) date_picker_set(p, date, 1);
}

KDate Kdate_picker_get_date(Kid id) {
	kst_picker_t* p = get_date_picker(id);
	KDate none = { 0, 0, 0 };
	return p ? p->date : none;
}

void Kdate_picker_set_min_year(Kid id, int year) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->min_year = year;
	if (p->max_year < year) p->max_year = year;
	date_picker_set(p, p->date, 1);
}

void Kdate_picker_set_max_year(Kid id, int year) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->max_year = year;
	if (p->min_year > year) p->min_year = year;
	date_picker_set(p, p->date, 1);
}

int Kdate_picker_get_min_year(Kid id) { kst_picker_t* p = get_date_picker(id); return p ? p->min_year : 0; }
int Kdate_picker_get_max_year(Kid id) { kst_picker_t* p = get_date_picker(id); return p ? p->max_year : 0; }

void Kdate_picker_set_day_visible(Kid id, int visible) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->day_visible = visible != 0;
	if (p->flyout) Kdate_picker_flyout_set_day_visible(p->flyout->id, visible);
}

void Kdate_picker_set_month_visible(Kid id, int visible) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->month_visible = visible != 0;
	if (p->flyout) Kdate_picker_flyout_set_month_visible(p->flyout->id, visible);
}

void Kdate_picker_set_year_visible(Kid id, int visible) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->year_visible = visible != 0;
	if (p->flyout) Kdate_picker_flyout_set_year_visible(p->flyout->id, visible);
}

int Kdate_picker_get_day_visible(Kid id) { kst_picker_t* p = get_date_picker(id); return p ? p->day_visible : 0; }
int Kdate_picker_get_month_visible(Kid id) { kst_picker_t* p = get_date_picker(id); return p ? p->month_visible : 0; }
int Kdate_picker_get_year_visible(Kid id) { kst_picker_t* p = get_date_picker(id); return p ? p->year_visible : 0; }

Kid Kdate_picker_get_flyout(Kid id) {
	kst_picker_t* p = get_date_picker(id);
	return p && p->flyout ? p->flyout->id : K_INVALID_ID;
}

Kid Kdate_picker_get_flyout_button(Kid id) {
	kst_picker_t* p = get_date_picker(id);
	return p && p->button ? p->button->id : K_INVALID_ID;
}

void Kdate_picker_on_date_changed(Kid id, KDateChangedCallback callback, void* user_data) {
	kst_picker_t* p = get_date_picker(id);
	if (!p) return;
	p->date_changed = callback;
	p->date_changed_data = user_data;
}

void Ktime_picker_set_header(Kid id, const char* header) {
	kst_picker_t* p = get_time_picker(id);
	if (!p) return;
	kst_set_string(&p->header, header ? header : "");
	kst_changed_layout(&p->base.base);
}

const char* Ktime_picker_get_header(Kid id) {
	kst_picker_t* p = get_time_picker(id);
	return p && p->header ? p->header : "";
}

void Ktime_picker_set_time(Kid id, KTimeOfDay time_of_day) {
	kst_picker_t* p = get_time_picker(id);
	if (p) time_picker_set(p, time_of_day, 1);
}

KTimeOfDay Ktime_picker_get_time(Kid id) {
	kst_picker_t* p = get_time_picker(id);
	KTimeOfDay none = { 0, 0 };
	return p ? p->time : none;
}

void Ktime_picker_set_minute_increment(Kid id, int increment) {
	kst_picker_t* p = get_time_picker(id);
	if (!p) return;
	p->minute_increment = kst_mini(kst_maxi(increment, 1), 59);
	time_picker_set(p, p->time, 1);
}

int Ktime_picker_get_minute_increment(Kid id) { kst_picker_t* p = get_time_picker(id); return p ? p->minute_increment : 1; }

void Ktime_picker_set_clock_identifier(Kid id, KClockIdentifier clock) {
	kst_picker_t* p = get_time_picker(id);
	if (!p || (clock != K_CLOCK_12_HOUR && clock != K_CLOCK_24_HOUR)) return;
	p->clock = clock;
	time_picker_set(p, p->time, 1);
}

KClockIdentifier Ktime_picker_get_clock_identifier(Kid id) { kst_picker_t* p = get_time_picker(id); return p ? p->clock : K_CLOCK_12_HOUR; }

Kid Ktime_picker_get_flyout(Kid id) {
	kst_picker_t* p = get_time_picker(id);
	return p && p->flyout ? p->flyout->id : K_INVALID_ID;
}

Kid Ktime_picker_get_flyout_button(Kid id) {
	kst_picker_t* p = get_time_picker(id);
	return p && p->button ? p->button->id : K_INVALID_ID;
}

void Ktime_picker_on_time_changed(Kid id, KTimeChangedCallback callback, void* user_data) {
	kst_picker_t* p = get_time_picker(id);
	if (!p) return;
	p->time_changed = callback;
	p->time_changed_data = user_data;
}
