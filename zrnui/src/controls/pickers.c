/**
 * zrnui - controls/pickers.c
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

#define LS(e) ZST_AS(zrn_looping_selector_t, e)
#define PICKER(e) ZST_AS(zrn_picker_t, e)
#define PF(e) ZST_AS(zrn_picker_flyout_t, e)

/* generic.xaml, LoopingSelector and LoopingSelectorItem: Margin 5, a Border of 2.5, the item template's StackPanel margin 5,5,0,5 with
 * the TextBlocks of 45 px (spacing -24, margin 0.8,-6.5) and 17 px (margin 0.8,-0.8); the time picker's has a single 43 px line, margin 0.8,-5 */
static constexpr float ITEM_MARGIN = 5.0f;
static constexpr float ITEM_BORDER = 2.5f;
static constexpr float ITEM_PAD = 5.0f;
static constexpr float TEXT_X = 0.8f;
static constexpr float PRIMARY_SIZE = 45.0f;
static constexpr int PRIMARY_SPACING = (-24);
static constexpr float PRIMARY_MARGIN = (-6.5f);
static constexpr float SECONDARY_SIZE = 17.0f;
static constexpr float SECONDARY_MARGIN = (-0.8f);
/** DatePickerSelectorThemeMinWidth. */
static constexpr float SELECTOR_MIN_WIDTH = 64.0f;
/** The Expanded transition. */
static constexpr float EXPAND_MS = 330.0f;
/** This library's: how far a finger must go to drag, how fast the inertia dies, how long the snap to an item takes. */
static constexpr float DRAG_START = 10.0f;
static constexpr float INERTIA_TAU = 320.0f;
static constexpr float SNAP_MS = 160.0f;
static constexpr float MIN_SPEED = 0.0004f;


/* ============================================================================
 *  the calendar
 * ============================================================================ */

static int is_leap(const int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

static int days_in_month(const int y, int m) {
	static const int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (m < 1) m = 1;
	if (m > 12) m = 12;
	return m == 2 && is_leap(y) ? 29 : days[m - 1];
}

/** 0 is Sunday. */
static int weekday_of(int y, const int m, const int d) {
	static const int t[12] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	if (m < 3) y--;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

/** The civil date of a day count from 1970-01-01 (the algorithm of Howard Hinnant's days_from_civil, inverted). */
static ZDate date_from_days(long z) {
	long era, doe, yoe, y, doy, mp;
	ZDate d;
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

static ZDate today(void) { return date_from_days((long)(time(nullptr) / 86400)); }

static const char* const MONTHS[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
static const char* const WEEKDAYS[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

/** A date in [min_year, max_year] with a day that the month has. */
static ZDate coerce_date(ZDate d, const int min_year, const int max_year) {
	if (d.year < min_year) d.year = min_year;
	if (d.year > max_year) d.year = max_year;
	if (d.month < 1) d.month = 1;
	if (d.month > 12) d.month = 12;
	if (d.day < 1) d.day = 1;
	if (d.day > days_in_month(d.year, d.month)) d.day = days_in_month(d.year, d.month);
	return d;
}

static int same_date(const ZDate a, const ZDate b) { return a.year == b.year && a.month == b.month && a.day == b.day; }

static ZTimeOfDay coerce_time(ZTimeOfDay t) {
	t.hour = zrn_mini(zrn_maxi(t.hour, 0), 23);
	t.minute = zrn_mini(zrn_maxi(t.minute, 0), 59);
	return t;
}

static int same_time(const ZTimeOfDay a, const ZTimeOfDay b) { return a.hour == b.hour && a.minute == b.minute; }


/* ============================================================================
 *  LoopingSelector
 * ============================================================================ */

typedef struct looping_item {
	char* primary;
	char* secondary;
} looping_item_t;

typedef struct zrn_looping_selector {
	zrn_control_t base;
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
} zrn_looping_selector_t;

static zrn_font_t primary_font(const zrn_looping_selector_t* const s) {
	const zrn_font_t f = { .family = Z_FONT_SEMIBOLD, .size = s->primary_size, .spacing = s->primary_spacing };
	return f;
}

static zrn_font_t secondary_font(void) {
	const zrn_font_t f = { .family = Z_FONT_SEMIBOLD, .size = SECONDARY_SIZE, .spacing = 0 };
	return f;
}

static float primary_height(const zrn_looping_selector_t* s) { return zrn_font_line_height(primary_font(s)) + 2 * s->primary_margin; }
static float secondary_height(void) { return zrn_font_line_height(secondary_font()) + 2 * SECONDARY_MARGIN; }

/** The height of what an item holds, the StackPanel with its margin: the text and 5 above and below. */
static float content_height(const zrn_looping_selector_t* const s) {
	return ITEM_PAD + primary_height(s) + (s->any_secondary || s->reserve_secondary ? secondary_height() : 0) + ITEM_PAD;
}

static float item_cell_height(const zrn_looping_selector_t* const s) {
	if (s->item_height > 0) return s->item_height;
	return 2 * ITEM_MARGIN + 2 * ITEM_BORDER + content_height(s);
}

static float item_cell_width(const zrn_looping_selector_t* const s) {
	float w = 0;
	if (s->item_width > 0) return s->item_width;
	for (int i = 0; i < s->count; i++) {
		const looping_item_t* it = &s->items[i];
		const float a = zrn_text_width(primary_font(s), it->primary, (int)strlen(it->primary)), b = it->secondary ? zrn_text_width(secondary_font(), it->secondary, (int)strlen(it->secondary)) : 0;
		w = zrn_maxf(w, zrn_maxf(a, b));
	}
	return zrn_maxf(SELECTOR_MIN_WIDTH, 2 * ITEM_MARGIN + 2 * ITEM_BORDER + ITEM_PAD + TEXT_X + w);
}

static int wrap_index(const int index, const int n) {
	const int m = n > 0 ? index % n : 0;
	return m < 0 ? m + n : m;
}

static ZSize ls_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_looping_selector_t* const s = LS(e);
	const float h = available.height < Z_INFINITE * 0.5f ? available.height : 5 * item_cell_height(s);
	return zrn_size(item_cell_width(s), h);
}

static void ls_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto s = LS(e);
	const float w = e->rect.width, hh = e->rect.height, ch = item_cell_height(s), center = hh * 0.5f;
	const ZColor accent = zrn_theme_color(e, ZST_TC_ACCENT), over = zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT), low = zrn_theme_color(e, ZST_TC_TEXT_LOW);
	int base, half, j, n = s->count;
	if (n == 0 || ch <= 0) return;
	base = (int)floorf(s->pos + 0.5f);
	half = (int)ceilf(center / ch) + 1;
	for (j = -half; j <= half; j++) {
		int k = base + j, idx;
		float top, alpha;
		int selected;
		if (!s->should_loop && (k < 0 || k >= n)) continue;
		idx = wrap_index(k, n);
		top = center + ((float)k - s->pos) * ch - ch * 0.5f;
		if (top >= hh || top + ch <= 0) continue;
		selected = idx == s->selected;
		alpha = selected ? 1.0f : s->expand;
		if (alpha <= 0.001f) continue;
		const looping_item_t* it = &s->items[idx];
		ZRect box = zrn_rect(ITEM_MARGIN, top + ITEM_MARGIN, w - 2 * ITEM_MARGIN, ch - 2 * ITEM_MARGIN);
		{
			const ZColor text = selected ? over : zrn_color_alpha(low, alpha), line = zrn_color_alpha(low, alpha);
			/* the StackPanel sits at the bottom of the box (VerticalContentAlignment Bottom) */
			const float inner_h = box.height - 2 * ITEM_BORDER, y0 = box.y + ITEM_BORDER + zrn_maxf(0, inner_h - content_height(s));
			const float x = box.x + ITEM_BORDER + ITEM_PAD + TEXT_X;
			if (selected) zrn_fill_rect(p, box, accent);
			else zrn_stroke_rect(p, box, Zthickness_uniform(ITEM_BORDER), line);
			zrn_draw_string(p, primary_font(s), it->primary, (int)strlen(it->primary), x, y0 + ITEM_PAD + s->primary_margin, text);
			if (it->secondary) {
				zrn_draw_string(p, secondary_font(), it->secondary, (int)strlen(it->secondary), x, y0 + ITEM_PAD + primary_height(s) + SECONDARY_MARGIN, text);
			}
		}
	}
}

static void ls_set_expand(zrn_looping_selector_t* const s, const float goal) {
	zrn_element_t* const e = &s->base.base;
	s->expand_goal = goal;
	if (!e->root || !zrn_animations_enabled(e->root)) {
		s->expand = goal;
		zrn_changed_paint(e);
	} else {
		e->wants_tick = 1;
	}
}

/* The column has come to rest on an item: the item is the chosen one. */
static void ls_finish(zrn_looping_selector_t* const s, const int target) {
	zrn_element_t* const e = &s->base.base;
	int n = s->count, index, changed;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	if (n == 0) return;
	index = s->should_loop ? wrap_index(target, n) : zrn_mini(zrn_maxi(target, 0), n - 1);
	changed = index != s->selected;
	s->selected = index;
	s->pos = (float)index;
	ls_set_expand(s, 0);
	zrn_changed_paint(e);
	if (changed) zrn_raise(e, Z_EVENT_SELECTION_CHANGED);
}

static int ls_nearest(const zrn_looping_selector_t* const s, const float pos) {
	int k = (int)floorf(pos + 0.5f);
	if (!s->should_loop) k = zrn_mini(zrn_maxi(k, 0), s->count - 1);
	return k;
}

/* To an item, which snaps in 160 ms when animations run. */
static void ls_snap_to(zrn_looping_selector_t* const s, const int target) {
	zrn_element_t* const e = &s->base.base;
	s->inertia = 0;
	s->velocity = 0;
	if (e->root && zrn_animations_enabled(e->root) && (float)target != s->pos) {
		s->settling = 1;
		s->snap_from = s->pos;
		s->snap_to = (float)target;
		s->snap_elapsed = 0;
		e->wants_tick = 1;
	} else {
		ls_finish(s, target);
	}
}

static void ls_tick(zrn_element_t* const e, const float dt) {
	auto s = LS(e);
	int busy = 0;
	if (s->inertia) {
		const float old = s->pos;
		s->pos += s->velocity * dt;
		s->velocity *= expf(-dt / INERTIA_TAU);
		if (!s->should_loop && (s->pos < 0 || s->pos > (float)(s->count - 1))) {
			s->pos = zrn_clampf(s->pos, 0, (float)(s->count - 1));
			s->velocity = 0;
		}
		zrn_changed_paint(e);
		if (fabsf(s->velocity) < MIN_SPEED || s->pos == old) {
			ls_snap_to(s, ls_nearest(s, s->pos));
			if (!zrn_lookup(e->id)) return;
		}
		busy = 1;
	} else if (s->settling) {
		s->snap_elapsed += dt;
		if (s->snap_elapsed >= SNAP_MS) {
			ls_finish(s, (int)floorf(s->snap_to + 0.5f));
		} else {
			s->pos = s->snap_from + (s->snap_to - s->snap_from) * zrn_ease(Z_EASE_OUT, s->snap_elapsed / SNAP_MS);
			zrn_changed_paint(e);
		}
		busy = 1;
	}
	if (s->expand != s->expand_goal) {
		const float step = dt / EXPAND_MS;
		s->expand = s->expand < s->expand_goal ? zrn_minf(s->expand + step, s->expand_goal) : zrn_maxf(s->expand - step, s->expand_goal);
		zrn_changed_paint(e);
		busy = 1;
	}
	if (!busy && !s->pressed) e->wants_tick = 0;
}

/* A finger lets go of a drag: the inertia goes on, or, without animations, ends where it would have ended. */
static void ls_release(zrn_looping_selector_t* const s) {
	zrn_element_t* const e = &s->base.base;
	if (s->count == 0) return;
	if (e->root && zrn_animations_enabled(e->root)) {
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
static void ls_step(zrn_looping_selector_t* const s, const int direction) {
	if (s->count == 0) return;
	int target = (int)floorf(s->pos + 0.5f) + direction;
	if (!s->should_loop) target = zrn_mini(zrn_maxi(target, 0), s->count - 1);
	ls_set_expand(s, 1);
	ls_snap_to(s, target);
	/* with no animations the column is at rest already; with them it lets go of the items when the snap ends */
}

static int ls_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto s = LS(e);
	const float ch = item_cell_height(s);
	switch (in->type) {
	case ZST_IN_POINTER_DOWN:
		if (!zrn_effectively_enabled(e) || s->count == 0) return 1;
		s->pressed = 1;
		s->dragging = 0;
		s->inertia = s->settling = 0;
		s->velocity = 0;
		s->press_y = s->last_y = in->y;
		s->press_pos = s->pos;
		if (e->root) zrn_set_capture(e->root, e);
		e->wants_tick = 1;
		ls_set_expand(s, 1);
		return 1;
	case ZST_IN_POINTER_MOVE:
		if (!s->pressed) return 0;
		if (!s->dragging) {
			if (fabsf(in->y - s->press_y) <= DRAG_START) return 1;
			s->dragging = 1;
		}
		s->pos = s->press_pos - (in->y - s->press_y) / ch;
		if (!s->should_loop) s->pos = zrn_clampf(s->pos, -0.4f, (float)s->count - 0.6f);
		/* items per ms: the root does not give the time of a move, a step is taken for 16 ms */
		s->velocity = s->velocity * 0.5f + (-(in->y - s->last_y) / ch / 16.0f) * 0.5f;
		s->last_y = in->y;
		zrn_changed_paint(e);
		return 1;
	case ZST_IN_POINTER_UP: {
		const int was_dragging = s->dragging, was_pressed = s->pressed;
		s->pressed = 0;
		s->dragging = 0;
		if (e->root) zrn_release_capture(e->root, e);
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
	case ZST_IN_CAPTURE_LOST:
		if (s->pressed) {
			const int was_dragging = s->dragging;
			s->pressed = 0;
			s->dragging = 0;
			if (was_dragging) ls_release(s);
			else ls_snap_to(s, ls_nearest(s, s->pos));
		}
		return 0;
	case ZST_IN_WHEEL:
		if (!zrn_effectively_enabled(e) || s->count == 0) return 0;
		ls_step(s, in->delta > 0 ? -1 : 1);
		return 1;
	case ZST_IN_KEY_DOWN:
		if (in->key == Z_KEY_UP) { ls_step(s, -1); return 1; }
		if (in->key == Z_KEY_DOWN) { ls_step(s, 1); return 1; }
		return 0;
	default:
		return 0;
	}
}

static void ls_init(zrn_element_t* const e) {
	const auto s = LS(e);
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

static void ls_clear(zrn_looping_selector_t* const s) {
	for (int i = 0; i < s->count; i++) {
		free(s->items[i].primary);
		free(s->items[i].secondary);
	}
	s->count = 0;
	s->any_secondary = 0;
}

static void ls_destroy(zrn_element_t* const e) {
	const auto s = LS(e);
	ls_clear(s);
	free(s->items);
	s->items = nullptr;
}

zrn_class_t zrn_class_looping_selector = {
	.base = &zrn_class_control, .name = "LoopingSelector", .id = Z_CLASS_LOOPING_SELECTOR, .size = sizeof(zrn_looping_selector_t),
	.init = ls_init, .destroy = ls_destroy, .measure = ls_measure, .paint = ls_paint, .input = ls_input, .tick = ls_tick
};

Zid Zlooping_selector_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_LOOPING_SELECTOR);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_looping_selector_t* get_ls(const Zid id) { return LS(zrn_get(id, Z_CLASS_LOOPING_SELECTOR)); }

int Zlooping_selector_add_item(const Zid looping_selector, const char* const primary_text, const char* const secondary_text) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s) return -1;
	if (s->count == s->cap) {
		const int cap = s->cap ? s->cap * 2 : 8;
		looping_item_t* items = (looping_item_t*)realloc(s->items, (size_t)cap * sizeof *items);
		if (!items) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		s->items = items;
		s->cap = cap;
	}
	looping_item_t* it = &s->items[s->count];
	it->primary = zrn_strdup(primary_text ? primary_text : "");
	it->secondary = secondary_text && *secondary_text ? zrn_strdup(secondary_text) : nullptr;
	if (it->secondary) s->any_secondary = 1;
	s->count++;
	if (s->count == 1) { s->selected = 0; s->pos = 0; }
	zrn_changed_layout(&s->base.base);
	return s->count - 1;
}

void Zlooping_selector_clear_items(const Zid looping_selector) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s) return;
	ls_clear(s);
	s->selected = -1;
	s->pos = 0;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	zrn_changed_layout(&s->base.base);
}

int Zlooping_selector_get_item_count(const Zid looping_selector) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s ? s->count : 0;
}

const char* Zlooping_selector_get_item_text(const Zid looping_selector, const int index) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s && index >= 0 && index < s->count ? s->items[index].primary : "";
}

const char* Zlooping_selector_get_item_secondary_text(const Zid looping_selector, const int index) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s && index >= 0 && index < s->count && s->items[index].secondary ? s->items[index].secondary : "";
}

void Zlooping_selector_set_selected_index(Zid looping_selector, int index) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s || s->count == 0) return;
	index = zrn_mini(zrn_maxi(index, 0), s->count - 1);
	int changed = index != s->selected;
	s->selected = index;
	s->pos = (float)index;
	s->inertia = s->settling = 0;
	s->velocity = 0;
	zrn_changed_paint(&s->base.base);
	if (changed) zrn_raise(&s->base.base, Z_EVENT_SELECTION_CHANGED);
}

int Zlooping_selector_get_selected_index(const Zid looping_selector) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s ? s->selected : -1;
}

void Zlooping_selector_set_should_loop(const Zid looping_selector, const int should_loop) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s) return;
	s->should_loop = should_loop != 0;
	s->pos = s->selected >= 0 ? (float)s->selected : 0;
	zrn_changed_paint(&s->base.base);
}

int Zlooping_selector_get_should_loop(const Zid looping_selector) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s ? s->should_loop : 0;
}

void Zlooping_selector_set_item_width(const Zid looping_selector, const float width) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s) return;
	s->item_width = width > 0 ? width : 0;
	zrn_changed_layout(&s->base.base);
}

float Zlooping_selector_get_item_width(const Zid looping_selector) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s ? item_cell_width(s) : 0;
}

void Zlooping_selector_set_item_height(const Zid looping_selector, const float height) {
	zrn_looping_selector_t* const s = get_ls(looping_selector);
	if (!s) return;
	s->item_height = height > 0 ? height : 0;
	zrn_changed_layout(&s->base.base);
}

float Zlooping_selector_get_item_height(const Zid looping_selector) {
	const zrn_looping_selector_t* const s = get_ls(looping_selector);
	return s ? item_cell_height(s) : 0;
}

void Zlooping_selector_on_selection_changed(const Zid looping_selector, const ZRoutedCallback callback, void* const user_data) {
	if (get_ls(looping_selector)) Zelement_on(looping_selector, Z_EVENT_SELECTION_CHANGED, callback, user_data);
}


/* ============================================================================
 *  the flyouts
 * ============================================================================ */

typedef struct zrn_picker_flyout {
	zrn_flyout_base_t base;
	char* title;
	zrn_element_t *title_block, *selectors_grid, *done_button, *cancel_button;
	zrn_element_t* selector[3];
	/** The picker (DatePicker, TimePicker) the flyout belongs to, Z_INVALID_ID for none: it gives the title. */
	Zid owner;
	/** Set while the selectors are changed by the program. */
	int syncing;
	/* DatePickerFlyout */
	ZDate date;
	int min_year, max_year;
	int day_visible, month_visible, year_visible;
	ZDatePickedCallback date_picked;
	void* date_picked_data;
	/* TimePickerFlyout */
	ZTimeOfDay time;
	int minute_increment;
	ZClockIdentifier clock;
	ZTimePickedCallback time_picked;
	void* time_picked_data;
} zrn_picker_flyout_t;

/* DatePicker and TimePicker (below) */
struct zrn_picker {
	zrn_control_t base;
	char* header;
	/** The FlyoutButton of the template, and the flyout it shows (the button's child). */
	zrn_element_t *button, *flyout;
	ZDate date;
	int min_year, max_year;
	int day_visible, month_visible, year_visible;
	ZDateChangedCallback date_changed;
	void* date_changed_data;
	ZTimeOfDay time;
	int minute_increment;
	ZClockIdentifier clock;
	ZTimeChangedCallback time_changed;
	void* time_changed_data;
};

static void set_handler(zrn_element_t* const e, const ZEvent ev, const ZRoutedCallback cb, void* const data) {
	e->handlers[ev - Z_EVENT_CLICK].callback = cb;
	e->handlers[ev - Z_EVENT_CLICK].user_data = data;
}

static zrn_picker_flyout_t* flyout_of(const Zid id) { return PF(zrn_lookup(id)); }

static int selector_index(zrn_element_t* sel) { return sel ? LS(sel)->selected : 0; }

static void selector_fill(const zrn_element_t* const sel, const int count, const char* (*text)(int index, char* const buf, void* data), void* const data) {
	char buf[32];
	Zlooping_selector_clear_items(sel->id);
	for (int i = 0; i < count; i++) {
		const char* secondary = text(i, buf, data);
		Zlooping_selector_add_item(sel->id, buf, secondary);
	}
}

static void flyout_set_title_text(const zrn_picker_flyout_t* const f) {
	const char* t = f->title ? f->title : "";
	if (!f->title_block) return;
	Ztext_block_set_text(f->title_block->id, t);
	f->title_block->visibility = *t ? Z_VISIBLE : Z_COLLAPSED;
	zrn_changed_layout(f->title_block);
}

/* ---- the date ---- */

static const char* month_text(const int i, char* buf, void* data) { (void)data; snprintf(buf, 32, "%s", MONTHS[i]); return nullptr; }
static const char* year_text(const int i, char* buf, void* data) { snprintf(buf, 32, "%d", *(int*)data + i); return nullptr; }
static const char* day_text(const int i, char* const buf, void* const data) {
	const auto d = (ZDate*)data;
	snprintf(buf, 32, "%d", i + 1);
	return WEEKDAYS[weekday_of(d->year, d->month, i + 1)];
}

/* The days of the month the month and year selectors show now, with the chosen day kept as far as the month goes. */
static void date_fill_days(const zrn_picker_flyout_t* const f, const int year, const int month, const int day_index) {
	ZDate d = { .year = year, .month = month, .day = 1 };
	const int n = days_in_month(year, month);
	selector_fill(f->selector[1], n, day_text, &d);
	Zlooping_selector_set_selected_index(f->selector[1]->id, zrn_mini(day_index, n - 1));
}

static void date_sync(zrn_picker_flyout_t* const f) {
	const int years = f->max_year - f->min_year + 1;
	if (!f->selector[0] || years < 1) return;
	f->syncing = 1;
	selector_fill(f->selector[0], 12, month_text, nullptr);
	Zlooping_selector_set_selected_index(f->selector[0]->id, f->date.month - 1);
	selector_fill(f->selector[2], years, year_text, &f->min_year);
	Zlooping_selector_set_selected_index(f->selector[2]->id, f->date.year - f->min_year);
	date_fill_days(f, f->date.year, f->date.month, f->date.day - 1);
	f->syncing = 0;
}

/* What the selectors show as a date. */
static ZDate date_of_selectors(const zrn_picker_flyout_t* const f) {
	const ZDate d = { .year = f->min_year + selector_index(f->selector[2]), .month = selector_index(f->selector[0]) + 1, .day = selector_index(f->selector[1]) + 1 };
	return coerce_date(d, f->min_year, f->max_year);
}

/* The month or the year turned: the days are those of the new month. */
static void date_selector_changed(const Zid sender, void* const data) {
	zrn_picker_flyout_t* const f = flyout_of((Zid)(uintptr_t)data);
	ZST_UNUSED(sender);
	if (!f || f->syncing || sender == f->selector[1]->id) return;
	f->syncing = 1;
	date_fill_days(f, f->min_year + selector_index(f->selector[2]), selector_index(f->selector[0]) + 1, selector_index(f->selector[1]));
	f->syncing = 0;
}

/* ---- the time ---- */

static const char* hour_text(const int i, char* const buf, void* const data) {
	snprintf(buf, 32, "%d", *(int*)data == Z_CLOCK_12_HOUR ? i + 1 : i);
	return nullptr;
}
static const char* minute_text(const int i, char* buf, void* data) { snprintf(buf, 32, "%02d", i * *(int*)data); return nullptr; }
static const char* period_text(const int i, char* buf, void* data) { (void)data; snprintf(buf, 32, "%s", i ? "PM" : "AM"); return nullptr; }

static int hour_index(const ZTimeOfDay t, const ZClockIdentifier clock) {
	if (clock == Z_CLOCK_24_HOUR) return t.hour;
	return (t.hour % 12 == 0 ? 12 : t.hour % 12) - 1;
}

static void time_sync(zrn_picker_flyout_t* const f) {
	int clock = (int)f->clock, inc = f->minute_increment, minutes = (59 / inc) + 1;
	if (!f->selector[0]) return;
	f->syncing = 1;
	selector_fill(f->selector[0], clock == Z_CLOCK_12_HOUR ? 12 : 24, hour_text, &clock);
	Zlooping_selector_set_selected_index(f->selector[0]->id, hour_index(f->time, f->clock));
	selector_fill(f->selector[1], minutes, minute_text, &inc);
	/* a minute that is not on the grid of MinuteIncrement is shown on the nearest one that is */
	Zlooping_selector_set_selected_index(f->selector[1]->id, (f->time.minute + inc / 2) / inc >= minutes ? minutes - 1 : (f->time.minute + inc / 2) / inc);
	selector_fill(f->selector[2], 2, period_text, nullptr);
	Zlooping_selector_set_selected_index(f->selector[2]->id, f->time.hour >= 12);
	Zlooping_selector_set_should_loop(f->selector[2]->id, 0);
	f->selector[2]->visibility = clock == Z_CLOCK_12_HOUR ? Z_VISIBLE : Z_COLLAPSED;
	f->syncing = 0;
	zrn_changed_layout(f->selectors_grid);
}

static ZTimeOfDay time_of_selectors(const zrn_picker_flyout_t* const f) {
	ZTimeOfDay t;
	const int h = selector_index(f->selector[0]);
	if (f->clock == Z_CLOCK_24_HOUR) t.hour = h;
	else t.hour = ((h + 1) % 12) + (selector_index(f->selector[2]) ? 12 : 0);
	t.minute = selector_index(f->selector[1]) * f->minute_increment;
	return coerce_time(t);
}

/* ---- both ---- */

static void flyout_sync(zrn_picker_flyout_t* const f) {
	if (f->base.base.cls == &zrn_class_date_picker_flyout) date_sync(f);
	else time_sync(f);
}

typedef struct zrn_picker zrn_picker_t;
static void date_picker_set(zrn_picker_t* p, ZDate date, int raise);
static void time_picker_set(zrn_picker_t* p, ZTimeOfDay t, int raise);

/* Done: the selectors are the value; the handler hears of it, and the flyout goes. */
static void flyout_done(const Zid button, void* const data) {
	zrn_picker_flyout_t* f = flyout_of((Zid)(uintptr_t)data);
	const Zid id = (Zid)(uintptr_t)data;
	ZST_UNUSED(button);
	if (!f) return;
	if (f->base.base.cls == &zrn_class_date_picker_flyout) {
		const ZDate old = f->date;
		const ZDatePickedCallback cb = f->date_picked;
		void* const user = f->date_picked_data;
		f->date = date_of_selectors(f);
		if (cb) cb(id, old, f->date, user);
		/* the picker the flyout belongs to takes the date */
		{
			zrn_element_t* const o = zrn_lookup(f->owner);
			if (zrn_lookup(id) && o && o->cls == &zrn_class_date_picker) date_picker_set(PICKER(o), f->date, 1);
		}
	} else {
		const ZTimeOfDay old = f->time;
		const ZTimePickedCallback cb = f->time_picked;
		void* const user = f->time_picked_data;
		f->time = time_of_selectors(f);
		if (cb) cb(id, old, f->time, user);
		{
			zrn_element_t* const o = zrn_lookup(f->owner);
			if (zrn_lookup(id) && o && o->cls == &zrn_class_time_picker) time_picker_set(PICKER(o), f->time, 1);
		}
	}
	f = flyout_of(id);
	if (f && f->base.is_open && f->base.popup) zrn_popup_close(f->base.popup);
}

static void flyout_cancel(const Zid button, void* const data) {
	const Zid id = (Zid)(uintptr_t)data;
	ZST_UNUSED(button);
	Zflyout_base_hide(id);
}

static void picker_header_of(Zid owner, zrn_picker_flyout_t* f);

/* About to show: the selectors begin on the value, and the title is the header of the picker. */
static void flyout_on_show(zrn_element_t* const e) {
	const auto f = PF(e);
	picker_header_of(f->owner, f);
	flyout_set_title_text(f);
	flyout_sync(f);
}

/* Dismissed: what the selectors show goes back to the value. */
static void flyout_on_hide(zrn_element_t* e) { flyout_sync(PF(e)); }

static zrn_element_t* make_bar_button(const char* const label, const ZSymbol symbol, const zrn_picker_flyout_t* const f) {
	const Zid icon = Zsymbol_icon_create(symbol), b = Zapp_bar_button_create(label, icon);
	zrn_element_t* const e = zrn_lookup(b);
	if (e) {
		Zelement_set_horizontal_alignment(b, Z_HALIGN_CENTER);
		Zbutton_on_click(b, symbol == Z_SYMBOL_ACCEPT ? flyout_done : flyout_cancel, (void*)(uintptr_t)f->base.base.id);
	}
	return e;
}

/* The presenter's content: the title, the selectors in the middle and the bar of Done and Cancel at the bottom. */
static void picker_flyout_init(zrn_element_t* const e) {
	const auto f = PF(e);
	zrn_element_t* const presenter = f->base.popup && f->base.popup->child_count ? f->base.popup->children[0] : nullptr;
	Zid grid, inner, title, bar;
	f->base.placement = Z_FLYOUT_PLACEMENT_FULL;
	f->base.on_show = flyout_on_show;
	f->base.on_hide = flyout_on_hide;
	f->owner = Z_INVALID_ID;
	f->day_visible = f->month_visible = f->year_visible = 1;
	f->minute_increment = 1;
	f->clock = Z_CLOCK_12_HOUR;
	if (!presenter) return;
	/* DateTimeFlyoutContentPanelPortraitThemeMargin 0,37,0,0 and DateTimeFlyoutTitleThemeMargin 19,0,19,17.5; the title is the TitleTextBlockStyle */
	grid = Zgrid_create();
	Zgrid_add_row(grid, Z_GRID_AUTO, 0);
	Zgrid_add_row(grid, Z_GRID_STAR, 1);
	Zgrid_add_row(grid, Z_GRID_AUTO, 0);
	Zelement_set_margin(grid, Zthickness(0, 37, 0, 0));
	title = Ztext_block_create("");
	Ztext_block_set_style(title, Z_TEXT_STYLE_TITLE);
	Zelement_set_margin(title, Zthickness(19, 0, 19, 17.5f));
	Zgrid_place(grid, title, 0, 0);
	zrn_lookup(title)->visibility = Z_COLLAPSED;
	f->title_block = zrn_lookup(title);
	inner = Zgrid_create();
	Zgrid_add_column(inner, Z_GRID_STAR, 1);
	Zgrid_add_column(inner, Z_GRID_AUTO, 0);
	Zgrid_add_column(inner, Z_GRID_AUTO, 0);
	Zgrid_add_column(inner, Z_GRID_AUTO, 0);
	Zgrid_add_column(inner, Z_GRID_STAR, 1);
	for (int i = 0; i < 3; i++) {
		const Zid sel = Zlooping_selector_create();
		Zgrid_place(inner, sel, i + 1, 0);
		f->selector[i] = zrn_lookup(sel);
	}
	f->selectors_grid = zrn_lookup(inner);
	Zgrid_place(grid, inner, 0, 1);
	/* the bar: Done and Cancel, each in half the width */
	bar = Zgrid_create();
	Zgrid_add_column(bar, Z_GRID_STAR, 1);
	Zgrid_add_column(bar, Z_GRID_STAR, 1);
	f->done_button = make_bar_button("done", Z_SYMBOL_ACCEPT, f);
	f->cancel_button = make_bar_button("cancel", Z_SYMBOL_CANCEL, f);
	if (f->done_button) Zgrid_place(bar, f->done_button->id, 0, 0);
	if (f->cancel_button) Zgrid_place(bar, f->cancel_button->id, 1, 0);
	Zgrid_place(grid, bar, 0, 2);
	Zborder_set_child(presenter->id, grid);
}

static void picker_flyout_destroy(zrn_element_t* const e) {
	free(PF(e)->title);
	PF(e)->title = nullptr;
}

zrn_class_t zrn_class_picker_flyout_base = {
	.base = &zrn_class_flyout_base, .name = "PickerFlyoutBase", .id = Z_CLASS_PICKER_FLYOUT_BASE, .size = sizeof(zrn_picker_flyout_t), .abstract_class = 1,
	.init = picker_flyout_init, .destroy = picker_flyout_destroy
};

static void date_flyout_init(zrn_element_t* const e) {
	const auto f = PF(e);
	const ZDate t = today();
	f->date = t;
	f->min_year = t.year - 100;
	f->max_year = t.year + 100;
	for (int i = 0; i < 3; i++) {
		if (!f->selector[i]) return;
		LS(f->selector[i])->reserve_secondary = 1;
		set_handler(f->selector[i], Z_EVENT_SELECTION_CHANGED, date_selector_changed, (void*)(uintptr_t)e->id);
	}
	/* the years do not go round */
	Zlooping_selector_set_should_loop(f->selector[2]->id, 0);
	date_sync(f);
}

static void time_flyout_init(zrn_element_t* const e) {
	const auto f = PF(e);
	f->time.hour = 0;
	f->time.minute = 0;
	for (int i = 0; i < 3; i++) {
		if (!f->selector[i]) return;
		/* TimePickerFlyoutPresenter's item: one 43 px line, margin 0.8,-5, the weight of the presenter */
		LS(f->selector[i])->primary_size = 43.0f;
		LS(f->selector[i])->primary_margin = -5.0f;
		LS(f->selector[i])->primary_spacing = 0;
	}
	time_sync(f);
}

zrn_class_t zrn_class_date_picker_flyout = {
	.base = &zrn_class_picker_flyout_base, .name = "DatePickerFlyout", .id = Z_CLASS_DATE_PICKER_FLYOUT, .size = sizeof(zrn_picker_flyout_t),
	.init = date_flyout_init
};

zrn_class_t zrn_class_time_picker_flyout = {
	.base = &zrn_class_picker_flyout_base, .name = "TimePickerFlyout", .id = Z_CLASS_TIME_PICKER_FLYOUT, .size = sizeof(zrn_picker_flyout_t),
	.init = time_flyout_init
};

static zrn_picker_flyout_t* get_flyout(const Zid id) { return PF(zrn_get(id, Z_CLASS_PICKER_FLYOUT_BASE)); }
static zrn_picker_flyout_t* get_date_flyout(const Zid id) { return PF(zrn_get(id, Z_CLASS_DATE_PICKER_FLYOUT)); }
static zrn_picker_flyout_t* get_time_flyout(const Zid id) { return PF(zrn_get(id, Z_CLASS_TIME_PICKER_FLYOUT)); }

void Zpicker_flyout_base_set_title(const Zid picker_flyout, const char* const title) {
	zrn_picker_flyout_t* const f = get_flyout(picker_flyout);
	if (!f) return;
	zrn_set_string(&f->title, title ? title : "");
	flyout_set_title_text(f);
}

const char* Zpicker_flyout_base_get_title(const Zid picker_flyout) {
	const zrn_picker_flyout_t* const f = get_flyout(picker_flyout);
	return f && f->title ? f->title : "";
}

Zid Zpicker_flyout_base_get_selector(const Zid picker_flyout, const int index) {
	const zrn_picker_flyout_t* const f = get_flyout(picker_flyout);
	return f && index >= 0 && index < 3 && f->selector[index] ? f->selector[index]->id : Z_INVALID_ID;
}

Zid Zpicker_flyout_base_get_done_button(const Zid picker_flyout) {
	const zrn_picker_flyout_t* const f = get_flyout(picker_flyout);
	return f && f->done_button ? f->done_button->id : Z_INVALID_ID;
}

Zid Zpicker_flyout_base_get_cancel_button(const Zid picker_flyout) {
	const zrn_picker_flyout_t* const f = get_flyout(picker_flyout);
	return f && f->cancel_button ? f->cancel_button->id : Z_INVALID_ID;
}

Zid Zdate_picker_flyout_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_DATE_PICKER_FLYOUT);
	return e ? e->id : Z_INVALID_ID;
}

void Zdate_picker_flyout_set_date(const Zid flyout, const ZDate date) {
	zrn_picker_flyout_t* const f = get_date_flyout(flyout);
	if (!f) return;
	f->date = coerce_date(date, f->min_year, f->max_year);
	date_sync(f);
}

ZDate Zdate_picker_flyout_get_date(const Zid flyout) {
	const zrn_picker_flyout_t* const f = get_date_flyout(flyout);
	const ZDate none = { .year = 0, .month = 0, .day = 0 };
	return f ? f->date : none;
}

void Zdate_picker_flyout_set_min_year(const Zid flyout, const int year) {
	zrn_picker_flyout_t* const f = get_date_flyout(flyout);
	if (!f) return;
	f->min_year = year;
	if (f->max_year < year) f->max_year = year;
	f->date = coerce_date(f->date, f->min_year, f->max_year);
	date_sync(f);
}

void Zdate_picker_flyout_set_max_year(const Zid flyout, const int year) {
	zrn_picker_flyout_t* const f = get_date_flyout(flyout);
	if (!f) return;
	f->max_year = year;
	if (f->min_year > year) f->min_year = year;
	f->date = coerce_date(f->date, f->min_year, f->max_year);
	date_sync(f);
}

int Zdate_picker_flyout_get_min_year(Zid flyout) { const zrn_picker_flyout_t* f = get_date_flyout(flyout); return f ? f->min_year : 0; }
int Zdate_picker_flyout_get_max_year(Zid flyout) { const zrn_picker_flyout_t* f = get_date_flyout(flyout); return f ? f->max_year : 0; }

static void date_apply_visibility(const zrn_picker_flyout_t* const f) {
	f->selector[0]->visibility = f->month_visible ? Z_VISIBLE : Z_COLLAPSED;
	f->selector[1]->visibility = f->day_visible ? Z_VISIBLE : Z_COLLAPSED;
	f->selector[2]->visibility = f->year_visible ? Z_VISIBLE : Z_COLLAPSED;
	zrn_changed_layout(f->selectors_grid);
}

void Zdate_picker_flyout_set_day_visible(Zid id, const int visible) { zrn_picker_flyout_t* f = get_date_flyout(id); if (f) { f->day_visible = visible != 0; date_apply_visibility(f); } }
void Zdate_picker_flyout_set_month_visible(Zid flyout, const int visible) { zrn_picker_flyout_t* f = get_date_flyout(flyout); if (f) { f->month_visible = visible != 0; date_apply_visibility(f); } }
void Zdate_picker_flyout_set_year_visible(Zid flyout, const int visible) { zrn_picker_flyout_t* f = get_date_flyout(flyout); if (f) { f->year_visible = visible != 0; date_apply_visibility(f); } }
int Zdate_picker_flyout_get_day_visible(Zid flyout) { const zrn_picker_flyout_t* f = get_date_flyout(flyout); return f ? f->day_visible : 0; }
int Zdate_picker_flyout_get_month_visible(Zid flyout) { const zrn_picker_flyout_t* f = get_date_flyout(flyout); return f ? f->month_visible : 0; }
int Zdate_picker_flyout_get_year_visible(Zid flyout) { const zrn_picker_flyout_t* f = get_date_flyout(flyout); return f ? f->year_visible : 0; }

void Zdate_picker_flyout_on_date_picked(const Zid flyout, const ZDatePickedCallback callback, void* const user_data) {
	zrn_picker_flyout_t* const f = get_date_flyout(flyout);
	if (!f) return;
	f->date_picked = callback;
	f->date_picked_data = user_data;
}

Zid Ztime_picker_flyout_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_TIME_PICKER_FLYOUT);
	return e ? e->id : Z_INVALID_ID;
}

void Ztime_picker_flyout_set_time(const Zid flyout, const ZTimeOfDay time_of_day) {
	zrn_picker_flyout_t* const f = get_time_flyout(flyout);
	if (!f) return;
	f->time = coerce_time(time_of_day);
	time_sync(f);
}

ZTimeOfDay Ztime_picker_flyout_get_time(const Zid flyout) {
	const zrn_picker_flyout_t* const f = get_time_flyout(flyout);
	const ZTimeOfDay none = { .hour = 0, .minute = 0 };
	return f ? f->time : none;
}

void Ztime_picker_flyout_set_minute_increment(const Zid flyout, const int increment) {
	zrn_picker_flyout_t* const f = get_time_flyout(flyout);
	if (!f) return;
	f->minute_increment = zrn_mini(zrn_maxi(increment, 1), 59);
	time_sync(f);
}

int Ztime_picker_flyout_get_minute_increment(Zid id) { const zrn_picker_flyout_t* f = get_time_flyout(id); return f ? f->minute_increment : 1; }

void Ztime_picker_flyout_set_clock_identifier(const Zid flyout, const ZClockIdentifier clock) {
	zrn_picker_flyout_t* const f = get_time_flyout(flyout);
	if (!f || (clock != Z_CLOCK_12_HOUR && clock != Z_CLOCK_24_HOUR)) return;
	f->clock = clock;
	time_sync(f);
}

ZClockIdentifier Ztime_picker_flyout_get_clock_identifier(Zid id) { const zrn_picker_flyout_t* f = get_time_flyout(id); return f ? f->clock : Z_CLOCK_12_HOUR; }

void Ztime_picker_flyout_on_time_picked(const Zid flyout, const ZTimePickedCallback callback, void* const user_data) {
	zrn_picker_flyout_t* const f = get_time_flyout(flyout);
	if (!f) return;
	f->time_picked = callback;
	f->time_picked_data = user_data;
}


/* ============================================================================
 *  DatePicker, TimePicker
 * ============================================================================ */


/* HeaderContentPresenterStyle: Normal, TextStyleMediumFontSize 16, PhoneMid, margin 0,0,0,-3 */
static zrn_font_t header_font(void) {
	const zrn_font_t f = { .family = Z_FONT_NORMAL, .size = ZST_81_FONT_CONTROL_HEADER, .spacing = 0 };
	return f;
}

static float header_height(const zrn_picker_t* const p) {
	return p->header && *p->header ? zrn_font_line_height(header_font()) - 3.0f : 0;
}

static void picker_header_of(const Zid owner, zrn_picker_flyout_t* const f) {
	zrn_element_t* const o = owner != Z_INVALID_ID ? zrn_lookup(owner) : nullptr;
	if (o && (o->cls == &zrn_class_date_picker || o->cls == &zrn_class_time_picker) && PICKER(o)->header && *PICKER(o)->header)
		zrn_set_string(&f->title, PICKER(o)->header);
}

static void picker_text(const zrn_picker_t* const p, const int is_date, char* const buf, const size_t size) {
	if (is_date) {
		snprintf(buf, size, "%s %d, %d", MONTHS[p->date.month - 1], p->date.day, p->date.year);
	} else if (p->clock == Z_CLOCK_24_HOUR) {
		snprintf(buf, size, "%d:%02d", p->time.hour, p->time.minute);
	} else {
		snprintf(buf, size, "%d:%02d %s", p->time.hour % 12 == 0 ? 12 : p->time.hour % 12, p->time.minute, p->time.hour >= 12 ? "PM" : "AM");
	}
}

static void picker_refresh_button(const zrn_picker_t* const p) {
	char buf[64];
	picker_text(p, p->base.base.cls == &zrn_class_date_picker, buf, sizeof buf);
	if (p->button) Zcontent_control_set_text(p->button->id, buf);
}

static ZSize picker_measure(zrn_element_t* const e, const ZSize available) {
	const auto p = PICKER(e);
	zrn_control_t* const c = &p->base;
	const float w = available.width < Z_INFINITE * 0.5f ? available.width : 480.0f;
	float pad_h = zrn_thickness_h(c->padding), h = c->padding.top + c->padding.bottom + header_height(p);
	if (p->button) {
		/* the Foreground of the picker is what the button draws and outlines in */
		if (c->set_flags & ZST_C_FOREGROUND) Zcontrol_set_foreground(p->button->id, c->foreground);
		zrn_measure(p->button, zrn_size(zrn_maxf(0, w - pad_h), Z_INFINITE));
		h += p->button->desired.height;
	}
	return zrn_size(w, h);
}

static ZSize picker_arrange(zrn_element_t* const e, const ZSize final) {
	const auto p = PICKER(e);
	const zrn_control_t* const c = &p->base;
	if (p->button) {
		zrn_arrange(p->button, zrn_rect(c->padding.left, c->padding.top + header_height(p), zrn_maxf(0, final.width - zrn_thickness_h(c->padding)), p->button->desired.height));
	}
	return final;
}

static void picker_paint(zrn_element_t* const e, zrn_painter_t* const painter) {
	const auto p = PICKER(e);
	const zrn_control_t* const c = &p->base;
	if (!p->header || !*p->header) return;
	zrn_draw_string(painter, header_font(), p->header, (int)strlen(p->header), c->padding.left, c->padding.top,
		zrn_effectively_enabled(e) ? zrn_theme_color(e, ZST_TC_TEXT_MID) : zrn_theme_color(e, ZST_TC_PHONE_DISABLED));
}

static void picker_init(zrn_element_t* const e) {
	const auto p = PICKER(e);
	const int is_date = e->cls == &zrn_class_date_picker;
	const Zid button = Zbutton_create(""), flyout = is_date ? Zdate_picker_flyout_create() : Ztime_picker_flyout_create();
	zrn_element_t* const b = zrn_lookup(button);
	e->focusable = 0;
	p->day_visible = p->month_visible = p->year_visible = 1;
	p->minute_increment = 1;
	p->clock = Z_CLOCK_12_HOUR;
	{
		const ZDate t = today();
		p->date = t;
		p->min_year = t.year - 100;
		p->max_year = t.year + 100;
	}
	if (!b) return;
	/* FlyoutButton: BorderThickness 2.5 (the Button's own), Padding 6.5,0,0,3, HorizontalAlignment Stretch, content at the left */
	Zcontrol_set_padding(button, Zthickness(6.5f, 0, 0, 3));
	Zelement_set_horizontal_alignment(button, Z_HALIGN_STRETCH);
	Zcontrol_set_horizontal_content_alignment(button, Z_HALIGN_LEFT);
	zrn_add_child(e, b, -1);
	p->button = b;
	if (zrn_lookup(flyout)) {
		const auto f = PF(zrn_lookup(flyout));
		p->flyout = zrn_lookup(flyout);
		f->owner = e->id;
		if (is_date) f->date = p->date;
		Zbutton_set_flyout(button, flyout);
	}
	picker_refresh_button(p);
}

static void picker_destroy(zrn_element_t* const e) {
	free(PICKER(e)->header);
	PICKER(e)->header = nullptr;
}

static void picker_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	if (PICKER(e)->button == child) {
		PICKER(e)->button = nullptr;
		PICKER(e)->flyout = nullptr;
	}
}

zrn_class_t zrn_class_date_picker = {
	.base = &zrn_class_control, .name = "DatePicker", .id = Z_CLASS_DATE_PICKER, .size = sizeof(zrn_picker_t),
	.init = picker_init, .destroy = picker_destroy, .measure = picker_measure, .arrange = picker_arrange, .paint = picker_paint,
	.child_removed = picker_child_removed
};

zrn_class_t zrn_class_time_picker = {
	.base = &zrn_class_control, .name = "TimePicker", .id = Z_CLASS_TIME_PICKER, .size = sizeof(zrn_picker_t),
	.init = picker_init, .destroy = picker_destroy, .measure = picker_measure, .arrange = picker_arrange, .paint = picker_paint,
	.child_removed = picker_child_removed
};

static zrn_picker_t* get_date_picker(const Zid id) { return PICKER(zrn_get(id, Z_CLASS_DATE_PICKER)); }
static zrn_picker_t* get_time_picker(const Zid id) { return PICKER(zrn_get(id, Z_CLASS_TIME_PICKER)); }

/* The flyout of a picker and the picker agree on the value. */
static void date_picker_set(zrn_picker_t* const p, const ZDate date, const int raise) {
	zrn_element_t* const e = &p->base.base;
	const Zid id = e->id;
	const ZDate old = p->date;
	p->date = coerce_date(date, p->min_year, p->max_year);
	if (p->flyout) {
		const auto f = PF(p->flyout);
		f->min_year = p->min_year;
		f->max_year = p->max_year;
		f->date = p->date;
		date_sync(f);
	}
	picker_refresh_button(p);
	zrn_changed_layout(e);
	if (raise && !same_date(old, p->date) && p->date_changed) {
		const ZDateChangedCallback cb = p->date_changed;
		void* const user = p->date_changed_data;
		cb(id, old, p->date, user);
	}
}

static void time_picker_set(zrn_picker_t* const p, const ZTimeOfDay t, const int raise) {
	zrn_element_t* const e = &p->base.base;
	const Zid id = e->id;
	const ZTimeOfDay old = p->time;
	p->time = coerce_time(t);
	if (p->flyout) {
		const auto f = PF(p->flyout);
		f->minute_increment = p->minute_increment;
		f->clock = p->clock;
		f->time = p->time;
		time_sync(f);
	}
	picker_refresh_button(p);
	zrn_changed_layout(e);
	if (raise && !same_time(old, p->time) && p->time_changed) {
		const ZTimeChangedCallback cb = p->time_changed;
		void* const user = p->time_changed_data;
		cb(id, old, p->time, user);
	}
}

Zid Zdate_picker_create(void) {
	zrn_element_t* const e = zrn_create(Z_CLASS_DATE_PICKER);
	if (!e) return Z_INVALID_ID;
	return e->id;
}

Zid Ztime_picker_create(void) {
	zrn_element_t* const e = zrn_create(Z_CLASS_TIME_PICKER);
	if (!e) return Z_INVALID_ID;
	return e->id;
}

void Zdate_picker_set_header(const Zid date_picker, const char* const header) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	zrn_set_string(&p->header, header ? header : "");
	zrn_changed_layout(&p->base.base);
}

const char* Zdate_picker_get_header(const Zid date_picker) {
	const zrn_picker_t* const p = get_date_picker(date_picker);
	return p && p->header ? p->header : "";
}

void Zdate_picker_set_date(const Zid date_picker, const ZDate date) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (p) date_picker_set(p, date, 1);
}

ZDate Zdate_picker_get_date(const Zid date_picker) {
	const zrn_picker_t* const p = get_date_picker(date_picker);
	const ZDate none = { .year = 0, .month = 0, .day = 0 };
	return p ? p->date : none;
}

void Zdate_picker_set_min_year(const Zid date_picker, const int year) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->min_year = year;
	if (p->max_year < year) p->max_year = year;
	date_picker_set(p, p->date, 1);
}

void Zdate_picker_set_max_year(const Zid date_picker, const int year) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->max_year = year;
	if (p->min_year > year) p->min_year = year;
	date_picker_set(p, p->date, 1);
}

int Zdate_picker_get_min_year(Zid date_picker) { const zrn_picker_t* p = get_date_picker(date_picker); return p ? p->min_year : 0; }
int Zdate_picker_get_max_year(Zid date_picker) { const zrn_picker_t* p = get_date_picker(date_picker); return p ? p->max_year : 0; }

void Zdate_picker_set_day_visible(const Zid date_picker, const int visible) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->day_visible = visible != 0;
	if (p->flyout) Zdate_picker_flyout_set_day_visible(p->flyout->id, visible);
}

void Zdate_picker_set_month_visible(const Zid date_picker, const int visible) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->month_visible = visible != 0;
	if (p->flyout) Zdate_picker_flyout_set_month_visible(p->flyout->id, visible);
}

void Zdate_picker_set_year_visible(const Zid date_picker, const int visible) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->year_visible = visible != 0;
	if (p->flyout) Zdate_picker_flyout_set_year_visible(p->flyout->id, visible);
}

int Zdate_picker_get_day_visible(Zid id) { const zrn_picker_t* p = get_date_picker(id); return p ? p->day_visible : 0; }
int Zdate_picker_get_month_visible(Zid date_picker) { const zrn_picker_t* p = get_date_picker(date_picker); return p ? p->month_visible : 0; }
int Zdate_picker_get_year_visible(Zid date_picker) { const zrn_picker_t* p = get_date_picker(date_picker); return p ? p->year_visible : 0; }

Zid Zdate_picker_get_flyout(const Zid date_picker) {
	const zrn_picker_t* const p = get_date_picker(date_picker);
	return p && p->flyout ? p->flyout->id : Z_INVALID_ID;
}

Zid Zdate_picker_get_flyout_button(const Zid date_picker) {
	const zrn_picker_t* const p = get_date_picker(date_picker);
	return p && p->button ? p->button->id : Z_INVALID_ID;
}

void Zdate_picker_on_date_changed(const Zid date_picker, const ZDateChangedCallback callback, void* const user_data) {
	zrn_picker_t* const p = get_date_picker(date_picker);
	if (!p) return;
	p->date_changed = callback;
	p->date_changed_data = user_data;
}

void Ztime_picker_set_header(const Zid date_picker, const char* const header) {
	zrn_picker_t* const p = get_time_picker(date_picker);
	if (!p) return;
	zrn_set_string(&p->header, header ? header : "");
	zrn_changed_layout(&p->base.base);
}

const char* Ztime_picker_get_header(const Zid time_picker) {
	const zrn_picker_t* const p = get_time_picker(time_picker);
	return p && p->header ? p->header : "";
}

void Ztime_picker_set_time(const Zid time_picker, const ZTimeOfDay time_of_day) {
	zrn_picker_t* const p = get_time_picker(time_picker);
	if (p) time_picker_set(p, time_of_day, 1);
}

ZTimeOfDay Ztime_picker_get_time(const Zid time_picker) {
	const zrn_picker_t* const p = get_time_picker(time_picker);
	const ZTimeOfDay none = { .hour = 0, .minute = 0 };
	return p ? p->time : none;
}

void Ztime_picker_set_minute_increment(const Zid time_picker, const int increment) {
	zrn_picker_t* const p = get_time_picker(time_picker);
	if (!p) return;
	p->minute_increment = zrn_mini(zrn_maxi(increment, 1), 59);
	time_picker_set(p, p->time, 1);
}

int Ztime_picker_get_minute_increment(Zid time_picker) { const zrn_picker_t* p = get_time_picker(time_picker); return p ? p->minute_increment : 1; }

void Ztime_picker_set_clock_identifier(const Zid time_picker, const ZClockIdentifier clock) {
	zrn_picker_t* const p = get_time_picker(time_picker);
	if (!p || (clock != Z_CLOCK_12_HOUR && clock != Z_CLOCK_24_HOUR)) return;
	p->clock = clock;
	time_picker_set(p, p->time, 1);
}

ZClockIdentifier Ztime_picker_get_clock_identifier(Zid time_picker) { const zrn_picker_t* p = get_time_picker(time_picker); return p ? p->clock : Z_CLOCK_12_HOUR; }

Zid Ztime_picker_get_flyout(const Zid time_picker) {
	const zrn_picker_t* const p = get_time_picker(time_picker);
	return p && p->flyout ? p->flyout->id : Z_INVALID_ID;
}

Zid Ztime_picker_get_flyout_button(const Zid time_picker) {
	const zrn_picker_t* const p = get_time_picker(time_picker);
	return p && p->button ? p->button->id : Z_INVALID_ID;
}

void Ztime_picker_on_time_changed(const Zid time_picker, const ZTimeChangedCallback callback, void* const user_data) {
	zrn_picker_t* const p = get_time_picker(time_picker);
	if (!p) return;
	p->time_changed = callback;
	p->time_changed_data = user_data;
}
