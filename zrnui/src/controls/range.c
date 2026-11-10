/**
 * zrnui - controls/range.c
 * RangeBase, Slider, ProgressBar, ProgressRing, ScrollBar and Thumb. Slider,
 * ProgressBar and ProgressRing follow the Windows Phone 8.1 templates; the
 * ScrollBar and the Thumb are still the WP8 ones.
 */

#include "../internal.h"

#define RANGE(e) ZST_AS(zrn_range_base_t, e)


/* ============================================================================
 *  RangeBase
 * ============================================================================ */

static double coerce(const zrn_range_base_t* r, double v) {
	if (v > r->maximum) v = r->maximum;
	if (v < r->minimum) v = r->minimum;
	return v;
}

/* Sets the value; fires ValueChanged. @return true when the value changed. */
static int range_assign(zrn_range_base_t* r, double value) {
	const double old = r->value;
	value = coerce(r, value);
	if (value == old) return 0;
	r->value = value;
	zrn_changed_paint(&r->base.base);
	if (r->changed) r->changed(r->base.base.id, old, value, r->changed_data);
	return 1;
}

static void range_init(zrn_element_t* const e) {
	const auto r = RANGE(e);
	r->minimum = 0;
	r->maximum = 1;
	r->small_change = 0.1;
	r->large_change = 1;
}

zrn_class_t zrn_class_range_base = {
	.base = &zrn_class_control, .name = "RangeBase", .id = Z_CLASS_RANGE_BASE, .size = sizeof(zrn_range_base_t), .abstract_class = 1,
	.init = range_init
};

static zrn_range_base_t* get_range(const Zid id) { return RANGE(zrn_get(id, Z_CLASS_RANGE_BASE)); }

void Zrange_base_set_value(const Zid id, const double value) {
	zrn_range_base_t* const r = get_range(id);
	if (r) range_assign(r, value);
}

double Zrange_base_get_value(Zid id) { const zrn_range_base_t* r = get_range(id); return r ? r->value : 0; }
double Zrange_base_get_minimum(Zid id) { const zrn_range_base_t* r = get_range(id); return r ? r->minimum : 0; }
double Zrange_base_get_maximum(Zid id) { const zrn_range_base_t* r = get_range(id); return r ? r->maximum : 0; }

void Zrange_base_set_range(Zid id, const double minimum, double maximum) {
	zrn_range_base_t* const r = get_range(id);
	if (!r) return;
	if (maximum < minimum) maximum = minimum;
	r->minimum = minimum;
	r->maximum = maximum;
	{
		const Zid eid = r->base.base.id;
		zrn_changed_paint(&r->base.base);
		range_assign(r, r->value);
		if (zrn_lookup(eid)) zrn_changed_paint(&r->base.base);
	}
}

void Zrange_base_set_changes(const Zid id, const double small_change, const double large_change) {
	zrn_range_base_t* const r = get_range(id);
	if (!r) return;
	r->small_change = small_change;
	r->large_change = large_change;
}

void Zrange_base_on_value_changed(const Zid id, const ZValueChangedCallback callback, void* const user_data) {
	zrn_range_base_t* const r = get_range(id);
	if (!r) return;
	r->changed = callback;
	r->changed_data = user_data;
}

static double range_fraction(const zrn_range_base_t* const r) {
	return r->maximum > r->minimum ? (r->value - r->minimum) / (r->maximum - r->minimum) : 0;
}


/* A cubic Bezier ZeySpline (0,0)-(x1,y1)-(x2,y2)-(1,1): the y of an x in [0, 1]. */
static float key_spline(const float x, const float x1, const float y1, const float x2, const float y2) {
	float lo = 0, hi = 1, t = x;
	int i;
	if (x <= 0) return 0;
	if (x >= 1) return 1;
	for (i = 0; i < 24; i++) {
		float u = 1 - t, bx = 3 * u * u * t * x1 + 3 * u * t * t * x2 + t * t * t;
		if (bx < x) lo = t; else hi = t;
		t = (lo + hi) * 0.5f;
	}
	{
		const float u = 1 - t;
		return 3 * u * u * t * y1 + 3 * u * t * t * y2 + t * t * t;
	}
}

static float lerp(const float a, const float b, const float t) { return a + (b - a) * t; }


/* ============================================================================
 *  Slider
 *  The 8.1 template: [Header] and under it a 57 px band: 17.5 px, the 9 px
 *  track, 30.5 px (ticks above and below); the thumb is 9.5 x 19, 13 px
 *  from the top of the band, and the accent fills the track up to it.
 *  Vertical: the same turned (36 px wide, the value grows upward, and the
 *  template gives its thumb 9 x 9). The Padding of the control is the margin
 *  of the template's grid.
 * ============================================================================ */

#define SLIDER(e) ZST_AS(zrn_slider_t, e)
static constexpr float SL_THUMB_W = 9.5f;
static constexpr float SL_THUMB_H = 19.0f;
static constexpr float SL_TRACK = 9.0f;
static constexpr float SL_BAND_ABOVE = 17.5f;
static constexpr float SL_BAND_BELOW = 30.5f;
static constexpr float SL_TICK = 5.0f;
static constexpr float SL_VERTICAL_W = 36.0f;
static constexpr float SL_VERTICAL_SIDE = 13.5f;
static constexpr float SL_VERTICAL_THUMB = 9.0f;

static zrn_font_t slider_header_font(void) { const zrn_font_t f = { Z_FONT_NORMAL, ZST_81_FONT_CONTROL_HEADER, 0 }; return f; }

static float slider_header_height(const zrn_slider_t* const s) {
	return s->header && *s->header ? zrn_font_line_height(slider_header_font()) : 0;
}

static ZSize slider_measure(zrn_element_t* const e, const ZSize available) {
	const auto s = SLIDER(e);
	const ZThickness pad = s->base.base.padding;
	float along = s->orientation == Z_HORIZONTAL ? available.width : available.height;
	const float header = slider_header_height(s);
	if (along >= Z_INFINITE * 0.5f) along = 200.0f + (s->orientation == Z_HORIZONTAL ? zrn_thickness_h(pad) : zrn_thickness_v(pad) + header);
	if (s->orientation == Z_HORIZONTAL) return zrn_size(along, zrn_thickness_v(pad) + header + SL_BAND_ABOVE + SL_TRACK + SL_BAND_BELOW);
	return zrn_size(zrn_thickness_h(pad) + SL_VERTICAL_W, along);
}

/* Track, thumb and the accent part of the track, in local coordinates; the band is the area under the header. */
static void slider_geometry(zrn_element_t* const e, ZRect* const track, ZRect* const thumb, ZRect* const fill) {
	const auto s = SLIDER(e);
	const ZThickness pad = s->base.base.padding;
	const double f = range_fraction(&s->base);
	const float top = pad.top + slider_header_height(s);
	if (s->orientation == Z_HORIZONTAL) {
		const float length = zrn_maxf(0, e->rect.width - zrn_thickness_h(pad));
		const float x = pad.left + (float)f * zrn_maxf(0, length - SL_THUMB_W);
		*track = zrn_rect(pad.left, top + SL_BAND_ABOVE, length, SL_TRACK);
		*thumb = zrn_rect(x, top + 13.0f, SL_THUMB_W, SL_THUMB_H);
		*fill = zrn_rect(pad.left, track->y, x - pad.left, SL_TRACK);
	} else {
		const float length = zrn_maxf(0, e->rect.height - top - pad.bottom);
		const float y = top + (1.0f - (float)f) * zrn_maxf(0, length - SL_VERTICAL_THUMB);
		const float tx = pad.left + SL_VERTICAL_SIDE;
		*track = zrn_rect(tx, top, SL_TRACK, length);
		*thumb = zrn_rect(tx, y, SL_VERTICAL_THUMB, SL_VERTICAL_THUMB);
		*fill = zrn_rect(tx, y + SL_VERTICAL_THUMB, SL_TRACK, top + length - y - SL_VERTICAL_THUMB);
	}
}

static void slider_paint_ticks(zrn_element_t* const e, zrn_painter_t* const p, const ZRect track) {
	const auto s = SLIDER(e);
	const double range = s->base.maximum - s->base.minimum;
	const ZColor outside = zrn_theme_color(e, ZST_TC_BASE_MEDIUM_MID);
	const ZColor inline_color = zrn_theme_color(e, ZST_TC_BACKGROUND);
	int count = 0;
	if (s->tick_frequency <= 0 || range <= 0 || s->tick_placement == Z_TICK_NONE) return;
	for (count = 0; count < 1000; count++) {
		const double v = s->base.minimum + (double)count * s->tick_frequency;
		if (v > s->base.maximum + 1e-9) break;
		const float f = (float)((v - s->base.minimum) / range);
		const int top = s->tick_placement == Z_TICK_TOP_LEFT || s->tick_placement == Z_TICK_OUTSIDE;
		const int bottom = s->tick_placement == Z_TICK_BOTTOM_RIGHT || s->tick_placement == Z_TICK_OUTSIDE;
		if (s->orientation == Z_HORIZONTAL) {
			const float x = track.x + SL_THUMB_W * 0.5f + f * zrn_maxf(0, track.width - SL_THUMB_W);
			if (top) zrn_fill_rect(p, zrn_rect(x, track.y - (SL_BAND_ABOVE - SL_TICK) * 0.5f - SL_TICK, 1, SL_TICK), outside);
			if (bottom) zrn_fill_rect(p, zrn_rect(x, track.y + SL_TRACK + (SL_BAND_BELOW - SL_TICK) * 0.5f, 1, SL_TICK), outside);
			if (s->tick_placement == Z_TICK_INLINE) zrn_fill_rect(p, zrn_rect(x, track.y, 1, SL_TRACK), inline_color);
		} else {
			const float y = track.y + track.height - SL_VERTICAL_THUMB * 0.5f - f * zrn_maxf(0, track.height - SL_VERTICAL_THUMB);
			if (top) zrn_fill_rect(p, zrn_rect(track.x - 1.5f - SL_TICK, y, SL_TICK, 1), outside);
			if (bottom) zrn_fill_rect(p, zrn_rect(track.x + SL_TRACK + 1.5f, y, SL_TICK, 1), outside);
			if (s->tick_placement == Z_TICK_INLINE) zrn_fill_rect(p, zrn_rect(track.x, y, SL_TRACK, 1), inline_color);
		}
	}
}

static void slider_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto s = SLIDER(e);
	const zrn_control_t* const c = &s->base.base;
	ZRect track, thumb, fill;
	const int enabled = zrn_effectively_enabled(e);
	const ZColor track_color = zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_SLIDER_TRACK));
	ZColor fill_color = zrn_ctl_color(c, ZST_C_FOREGROUND, c->foreground, zrn_theme_color(e, ZST_TC_ACCENT));
	ZColor thumb_color = zrn_theme_color(e, ZST_TC_SLIDER_THUMB);
	if (!enabled) {
		fill_color = zrn_theme_color(e, ZST_TC_SLIDER_DECREASE_DISABLED);
		thumb_color = zrn_theme_color(e, ZST_TC_SLIDER_THUMB_DISABLED);
	}
	slider_geometry(e, &track, &thumb, &fill);
	if (slider_header_height(s) > 0) {
		zrn_draw_string(p, slider_header_font(), s->header, (int)strlen(s->header), c->padding.left, c->padding.top, zrn_theme_color(e, ZST_TC_BASE_MID));
	}
	zrn_fill_rect(p, track, track_color);
	if (fill.width > 0 && fill.height > 0) zrn_fill_rect(p, fill, fill_color);
	slider_paint_ticks(e, p, track);
	zrn_fill_rect(p, thumb, thumb_color);
}

/* The value a touch at (x, y) means: the thumb is centered on it, then snapped to StepFrequency or TickFrequency. */
static void slider_set_from(zrn_element_t* const e, const float x, const float y) {
	const auto s = SLIDER(e);
	const ZThickness pad = s->base.base.padding;
	const float top = pad.top + slider_header_height(s);
	double f, v, step = s->snaps_to == Z_SNAPS_TO_TICKS && s->tick_frequency > 0 ? s->tick_frequency : s->step_frequency;
	if (s->orientation == Z_HORIZONTAL) {
		const float length = e->rect.width - zrn_thickness_h(pad) - SL_THUMB_W;
		f = length > 0 ? (x - pad.left - SL_THUMB_W * 0.5f) / length : 0;
	} else {
		const float length = e->rect.height - top - pad.bottom - SL_VERTICAL_THUMB;
		f = length > 0 ? 1.0 - (y - top - SL_VERTICAL_THUMB * 0.5f) / length : 0;
	}
	if (f < 0) f = 0;
	if (f > 1) f = 1;
	v = s->base.minimum + f * (s->base.maximum - s->base.minimum);
	if (step > 0) v = s->base.minimum + floor((v - s->base.minimum) / step + 0.5) * step;
	range_assign(&s->base, v);
}

/* A key moves by SmallChange (arrows) or LargeChange (page keys), snapped to the step. */
static void slider_step(zrn_slider_t* const s, const double delta) {
	double v = s->base.value + delta, step = s->snaps_to == Z_SNAPS_TO_TICKS && s->tick_frequency > 0 ? s->tick_frequency : s->step_frequency;
	if (step > 0) v = s->base.minimum + floor((v - s->base.minimum) / step + 0.5) * step;
	range_assign(&s->base, v);
}

static int slider_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto s = SLIDER(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			if (!zrn_effectively_enabled(e)) return 1;
			s->dragging = 1;
			if (e->root) zrn_set_capture(e->root, e);
			slider_set_from(e, in->x, in->y);
			return 1;
		case ZST_IN_POINTER_MOVE:
			if (!s->dragging) return 0;
			slider_set_from(e, in->x, in->y);
			return 1;
		case ZST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) zrn_release_capture(e->root, e);
			return 1;
		case ZST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		case ZST_IN_KEY_DOWN: {
			zrn_range_base_t* const r = &s->base;
			const int horizontal = s->orientation == Z_HORIZONTAL;
			const int forward = horizontal ? Z_KEY_RIGHT : Z_KEY_UP, back = horizontal ? Z_KEY_LEFT : Z_KEY_DOWN;
			if (in->key == forward) slider_step(s, r->small_change);
			else if (in->key == Z_KEY_PAGE_UP) slider_step(s, r->large_change);
			else if (in->key == back) slider_step(s, -r->small_change);
			else if (in->key == Z_KEY_PAGE_DOWN) slider_step(s, -r->large_change);
			else if (in->key == Z_KEY_HOME) range_assign(r, r->minimum);
			else if (in->key == Z_KEY_END) range_assign(r, r->maximum);
			else return 0;
			return 1;
		}
		default:
			return 0;
	}
}

static void slider_init(zrn_element_t* const e) {
	const auto s = SLIDER(e);
	s->orientation = Z_HORIZONTAL;
	s->base.maximum = 100;
	s->base.small_change = 1;
	s->base.large_change = 10;
	s->step_frequency = 1;
	s->tick_placement = Z_TICK_OUTSIDE;
	s->snaps_to = Z_SNAPS_TO_STEP_VALUES;
}

static void slider_destroy(zrn_element_t* e) { free(SLIDER(e)->header); }

zrn_class_t zrn_class_slider = {
	.base = &zrn_class_range_base, .name = "Slider", .id = Z_CLASS_SLIDER, .size = sizeof(zrn_slider_t),
	.init = slider_init, .destroy = slider_destroy, .measure = slider_measure, .paint = slider_paint, .input = slider_input
};

Zid Zslider_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_SLIDER);
	if (!e) return Z_INVALID_ID;
	SLIDER(e)->orientation = orientation;
	return e->id;
}

static zrn_slider_t* get_slider(const Zid id) { return SLIDER(zrn_get(id, Z_CLASS_SLIDER)); }

void Zslider_set_orientation(const Zid id, const ZOrientation orientation) {
	zrn_slider_t* const s = get_slider(id);
	if (!s) return;
	s->orientation = orientation;
	zrn_changed_layout(&s->base.base.base);
}

void Zslider_set_header(const Zid id, const char* const header) {
	zrn_slider_t* const s = get_slider(id);
	if (!s) return;
	zrn_set_string(&s->header, header && *header ? header : nullptr);
	zrn_changed_layout(&s->base.base.base);
}

const char* Zslider_get_header(const Zid id) {
	const zrn_slider_t* const s = get_slider(id);
	return s && s->header ? s->header : "";
}

void Zslider_set_step_frequency(const Zid id, const double step_frequency) {
	zrn_slider_t* const s = get_slider(id);
	if (s) s->step_frequency = step_frequency < 0 ? 0 : step_frequency;
}

double Zslider_get_step_frequency(Zid id) { const zrn_slider_t* s = get_slider(id); return s ? s->step_frequency : 0; }

void Zslider_set_tick_frequency(const Zid id, const double tick_frequency) {
	zrn_slider_t* const s = get_slider(id);
	if (!s) return;
	s->tick_frequency = tick_frequency < 0 ? 0 : tick_frequency;
	zrn_changed_paint(&s->base.base.base);
}

double Zslider_get_tick_frequency(Zid id) { const zrn_slider_t* s = get_slider(id); return s ? s->tick_frequency : 0; }

void Zslider_set_tick_placement(const Zid id, const ZTickPlacement placement) {
	zrn_slider_t* const s = get_slider(id);
	if (!s) return;
	s->tick_placement = placement;
	zrn_changed_paint(&s->base.base.base);
}

void Zslider_set_snaps_to(const Zid id, const ZSliderSnapsTo snaps_to) {
	zrn_slider_t* const s = get_slider(id);
	if (s) s->snaps_to = snaps_to;
}


/* ============================================================================
 *  ProgressBar
 *  3.5 px high (MinHeight), no padding. Determinate: the track is the accent
 *  at 0.16 opacity and the indicator the accent; ShowPaused halves the
 *  indicator, ShowError takes it away. IsIndeterminate: the track goes and
 *  five 3.5 px squares run across, 3.917 s a round, one 0.167 s after the
 *  other (the storyboard of the template). The ProgressBarTemplateSettings
 *  (where the squares wait and where they leave to) are computed by the
 *  control in 8.1 and are not in the SDK: they are a tenth and a seventh of
 *  the width here.
 * ============================================================================ */

#define PROGRESS(e) ZST_AS(zrn_progress_bar_t, e)
static constexpr float PB_HEIGHT = 3.5f;
static constexpr float PB_DOT = 3.5f;
static constexpr float PB_CYCLE_MS = 3917.0f;

static ZSize progress_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_control_t* const c = &PROGRESS(e)->base.base;
	const float w = available.width >= Z_INFINITE * 0.5f ? 100.0f + zrn_thickness_h(c->padding) : available.width;
	return zrn_size(w, PB_HEIGHT + zrn_thickness_v(c->padding));
}

/* The position of the i-th square (0 is the one in front) `t` seconds into the round, relative to its place in the row. */
static float progress_dot_offset(const int i, const float t, const float width) {
	float well = width * 0.1f, end = width * 0.15f;
	float u = t - 0.167f * (float)i, e = 0, b;
	float ta = 0.5f + 0.167f * (float)i, tb = 2.0f + 0.167f * (float)i, tc = 3.0f + 0.167f * (float)i;
	if (u <= 0) e = 0;
	else if (u < 1) e = well * key_spline(u, 0.4f, 0, 0.6f, 1);
	else if (u < 2) e = well;
	else if (u < 3) e = well + (end - well) * key_spline(u - 2, 0.4f, 0, 0.6f, 1);
	else e = end;
	if (t < ta) b = lerp(-40, 0, t / ta);
	else if (t < tb) b = 0;
	else if (t < tc) b = lerp(0, 80, (t - tb) / (tc - tb));
	else b = 80;
	return e + b;
}

static void progress_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto b = PROGRESS(e);
	const zrn_control_t* const c = &b->base.base;
	const ZColor accent = zrn_theme_color(e, ZST_TC_ACCENT);
	const ZColor fg = zrn_ctl_color(c, ZST_C_FOREGROUND, c->foreground, accent);
	if (b->indeterminate) {
		const float pitch = 2 * PB_DOT, t = b->time_ms / 1000.0f, group = 4 * pitch + PB_DOT;
		const float container = lerp(-group, e->rect.width, b->time_ms / PB_CYCLE_MS);
		const float y = (e->rect.height - PB_DOT) * 0.5f;
		int i;
		zrn_push_clip(p, zrn_local_rect(e));
		for (i = 0; i < 5; i++) {
			const float u = t - 0.167f * (float)i;
			if (u < 0 || u >= 3.0f) continue;
			zrn_fill_rect(p, zrn_rect(container + (float)(4 - i) * pitch + progress_dot_offset(i, t, e->rect.width), y, PB_DOT, PB_DOT), fg);
		}
		zrn_pop_clip(p);
		return;
	}
	{
		const ZRect root = zrn_local_rect(e);
		const ZRect area = zrn_rect(c->padding.left, c->padding.top, zrn_maxf(0, e->rect.width - zrn_thickness_h(c->padding)), zrn_maxf(0, e->rect.height - zrn_thickness_v(c->padding)));
		const ZColor track = zrn_ctl_background(c, zrn_color_alpha(accent, 0.16f));
		const ZColor indicator = b->show_paused ? zrn_color_alpha(fg, 0.5f) : fg;
		zrn_fill_rect(p, root, track);
		if (!b->show_error) zrn_fill_rect(p, zrn_rect(area.x, area.y, area.width * (float)range_fraction(&b->base), area.height), indicator);
	}
}

static void progress_tick(zrn_element_t* const e, const float dt) {
	const auto b = PROGRESS(e);
	if (!b->indeterminate) return;
	b->time_ms += dt;
	if (b->time_ms >= PB_CYCLE_MS) b->time_ms = fmodf(b->time_ms, PB_CYCLE_MS);
	zrn_changed_paint(e);
}

static void progress_init(zrn_element_t* const e) {
	const auto b = PROGRESS(e);
	b->base.maximum = 100;
	e->min_height = PB_HEIGHT;
	e->valign = Z_VALIGN_CENTER;
	e->hit_visible = 0;
	e->focusable = 0;
	e->tab_stop = 0;
}

zrn_class_t zrn_class_progress_bar = {
	.base = &zrn_class_range_base, .name = "ProgressBar", .id = Z_CLASS_PROGRESS_BAR, .size = sizeof(zrn_progress_bar_t),
	.init = progress_init, .measure = progress_measure, .paint = progress_paint, .tick = progress_tick
};

Zid Zprogress_bar_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_PROGRESS_BAR);
	return e ? e->id : Z_INVALID_ID;
}

void Zprogress_bar_set_is_indeterminate(const Zid id, const int is_indeterminate) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->indeterminate = is_indeterminate != 0;
	PROGRESS(e)->time_ms = 0;
	e->wants_tick = is_indeterminate != 0;
	zrn_changed_paint(e);
}

int Zprogress_bar_get_is_indeterminate(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_BAR);
	return e ? PROGRESS(e)->indeterminate : 0;
}

void Zprogress_bar_set_show_error(const Zid id, const int show_error) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->show_error = show_error != 0;
	zrn_changed_paint(e);
}

void Zprogress_bar_set_show_paused(const Zid id, const int show_paused) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->show_paused = show_paused != 0;
	zrn_changed_paint(e);
}


/* ============================================================================
 *  ProgressRing
 *  Five 6.5 px dots in a 57.5 px ring, run by the storyboard of the template: each
 *  dot (0.167 s after the one before) is turned by the keyframes of E1R, spread
 *  6 degrees apart, for 3.21 s, and the whole round is 4.305 s. While it is active
 *  the control paints its Background (the page's) over its whole area at 0.8 opacity.
 *  The Large state of the template (a sixth dot above 60 px) is not made.
 * ============================================================================ */

#define RING(e) ZST_AS(zrn_progress_ring_t, e)
static constexpr float PR_SIZE = 57.5f;
static constexpr float PR_DOT = 6.5f;
static constexpr float PR_DOT_MARGIN = 7.5f;
static constexpr float PR_CYCLE_MS = 4305.0f;

static float progress_ring_angle(const float u) {
	static const float times[7] = { 0, 0.433f, 1.2f, 1.617f, 2.017f, 2.783f, 3.217f };
	static const float angles[7] = { -110, 10, 93, 205, 357, 439, 585 };
	/* the ZeySpline of the segment that ends at a keyframe; linear where the keyframe has none */
	static const float splines[7][4] = { { 0, 0, 1, 1 }, { 0.02f, 0.33f, 0.38f, 0.77f }, { 0, 0, 1, 1 }, { 0.57f, 0.17f, 0.95f, 0.75f },
		{ 0, 0.19f, 0.07f, 0.72f }, { 0, 0, 1, 1 }, { 0, 0, 0.95f, 0.37f } };
	int i;
	if (u <= 0) return angles[0];
	for (i = 1; i < 7; i++) {
		if (u < times[i]) {
			float f = (u - times[i - 1]) / (times[i] - times[i - 1]);
			const float* k = splines[i];
			if (k[2] != 1 || k[3] != 1) f = key_spline(f, k[0], k[1], k[2], k[3]);
			return lerp(angles[i - 1], angles[i], f);
		}
	}
	return angles[6];
}

static void progress_ring_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto r = RING(e);
	zrn_control_t* const c = &r->base;
	const ZColor fg = zrn_ctl_color(c, ZST_C_FOREGROUND, c->foreground, zrn_theme_color(e, ZST_TC_ACCENT));
	float cx = e->rect.width * 0.5f, cy = e->rect.height * 0.5f, t = r->time_ms / 1000.0f, start = PR_DOT_MARGIN + PR_DOT * 0.5f - PR_SIZE * 0.5f;
	int i;
	if (!r->is_active) return;
	zrn_fill_rect(p, zrn_local_rect(e), zrn_color_alpha(zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_BACKGROUND)), 0.8f));
	for (i = 0; i < 5; i++) {
		float u = t - 0.167f * (float)i, a, x, y;
		if (u < 0 || u >= 3.21f) continue;
		a = (progress_ring_angle(u) - 6.0f * (float)i) * ZST_PI / 180.0f;
		/* the dot sits at the top left of the ring and turns about its center */
		x = start * cosf(a) - start * sinf(a);
		y = start * sinf(a) + start * cosf(a);
		zrn_fill_ellipse(p, zrn_rect(cx + x - PR_DOT * 0.5f, cy + y - PR_DOT * 0.5f, PR_DOT, PR_DOT), fg);
	}
}

static void progress_ring_tick(zrn_element_t* const e, const float dt) {
	const auto r = RING(e);
	if (!r->is_active) return;
	r->time_ms += dt;
	if (r->time_ms >= PR_CYCLE_MS) r->time_ms = fmodf(r->time_ms, PR_CYCLE_MS);
	zrn_changed_paint(e);
}

static void progress_ring_init(zrn_element_t* const e) {
	e->min_width = PR_SIZE;
	e->min_height = PR_SIZE;
	e->hit_visible = 0;
	e->focusable = 0;
	e->tab_stop = 0;
}

static ZSize progress_ring_measure(zrn_element_t* const e, const ZSize available) {
	ZST_UNUSED(e); ZST_UNUSED(available);
	return zrn_size(PR_SIZE, PR_SIZE);
}

zrn_class_t zrn_class_progress_ring = {
	.base = &zrn_class_control, .name = "ProgressRing", .id = Z_CLASS_PROGRESS_RING, .size = sizeof(zrn_progress_ring_t),
	.init = progress_ring_init, .measure = progress_ring_measure, .paint = progress_ring_paint, .tick = progress_ring_tick
};

Zid Zprogress_ring_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_PROGRESS_RING);
	return e ? e->id : Z_INVALID_ID;
}

void Zprogress_ring_set_is_active(Zid id, int is_active) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_RING);
	if (!e) return;
	is_active = is_active != 0;
	if (RING(e)->is_active == is_active) return;
	RING(e)->is_active = is_active;
	RING(e)->time_ms = 0;
	e->wants_tick = is_active;
	zrn_changed_paint(e);
}

int Zprogress_ring_get_is_active(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_PROGRESS_RING);
	return e ? RING(e)->is_active : 0;
}


/* ============================================================================
 *  ScrollBar
 *  The WP8 bar is a thin, 5 px thumb of at least 72 px with a 1 px margin,
 *  and it does not take the pointer (IsHitTestVisible False); here it does
 *  once Zelement_set_hit_test_visible turns that on.
 * ============================================================================ */

#define SCROLLBAR(e) ZST_AS(zrn_scrollbar_t, e)

static ZSize scrollbar_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_scrollbar_t* const s = SCROLLBAR(e);
	ZST_UNUSED(available);
	return s->orientation == Z_VERTICAL ? zrn_size(5, 0) : zrn_size(0, 5);
}

static ZRect scrollbar_thumb(zrn_element_t* const e) {
	const zrn_scrollbar_t* const s = SCROLLBAR(e);
	const float length = s->orientation == Z_VERTICAL ? e->rect.height : e->rect.width;
	const double range = s->base.maximum - s->base.minimum;
	float thumb = range + s->viewport > 0 ? (float)(s->viewport / (range + s->viewport)) * length : length;
	float pos;
	thumb = zrn_clampf(thumb, zrn_minf(72.0f, length), length);
	pos = range > 0 ? (float)range_fraction(&s->base) * (length - thumb) : 0;
	if (s->orientation == Z_VERTICAL) return zrn_rect(1, pos + 1, e->rect.width - 2, thumb - 2);
	return zrn_rect(pos + 1, 1, thumb - 2, e->rect.height - 2);
}

static void scrollbar_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto s = SCROLLBAR(e);
	const zrn_control_t* const c = &s->base.base;
	if (s->base.maximum <= s->base.minimum) return;
	zrn_fill_rect(p, scrollbar_thumb(e), zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_DISABLED)));
}

static void scrollbar_scroll(zrn_element_t* const e, const ZScrollEventType type, const double value) {
	const auto s = SCROLLBAR(e);
	const Zid id = e->id;
	range_assign(&s->base, value);
	if (!zrn_lookup(id)) return; /* ValueChanged destroyed the bar */
	if (s->on_scroll) s->on_scroll(id, type, s->base.value, s->on_scroll_data);
}

static int scrollbar_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto s = SCROLLBAR(e);
	const int vertical = s->orientation == Z_VERTICAL;
	const float pos = vertical ? in->y : in->x;
	switch (in->type) {
		case ZST_IN_POINTER_DOWN: {
			const ZRect t = scrollbar_thumb(e);
			const float start = vertical ? t.y : t.x, size = vertical ? t.height : t.width;
			if (pos >= start && pos <= start + size) {
				s->dragging = 1;
				s->drag_grab = pos - start;
				if (e->root) zrn_set_capture(e->root, e);
			} else {
				scrollbar_scroll(e, pos < start ? Z_SCROLL_LARGE_DECREMENT : Z_SCROLL_LARGE_INCREMENT, s->base.value + (pos < start ? -s->base.large_change : s->base.large_change));
			}
			return 1;
		}
		case ZST_IN_POINTER_MOVE:
			if (s->dragging) {
				const float length = vertical ? e->rect.height : e->rect.width;
				const ZRect t = scrollbar_thumb(e);
				const float room = length - (vertical ? t.height : t.width) - 2;
				double f = room > 0 ? (pos - s->drag_grab - 1) / room : 0;
				f = f < 0 ? 0 : (f > 1 ? 1 : f);
				scrollbar_scroll(e, Z_SCROLL_THUMB_TRACK, s->base.minimum + f * (s->base.maximum - s->base.minimum));
				return 1;
			}
			return 0;
		case ZST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) zrn_release_capture(e->root, e);
			scrollbar_scroll(e, Z_SCROLL_END, s->base.value);
			return 1;
		case ZST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		default:
			return 0;
	}
}

static void scrollbar_init(zrn_element_t* const e) {
	const auto s = SCROLLBAR(e);
	s->orientation = Z_VERTICAL;
	s->base.maximum = 1;
	s->base.base.padding = Zthickness_uniform(1);
	s->viewport = 0.1;
	e->hit_visible = 0;
	e->focusable = 0;
}

zrn_class_t zrn_class_scrollbar = {
	.base = &zrn_class_range_base, .name = "ScrollBar", .id = Z_CLASS_SCROLLBAR, .size = sizeof(zrn_scrollbar_t),
	.init = scrollbar_init, .measure = scrollbar_measure, .paint = scrollbar_paint, .input = scrollbar_input
};

Zid Zscroll_bar_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_SCROLLBAR);
	if (!e) return Z_INVALID_ID;
	SCROLLBAR(e)->orientation = orientation;
	return e->id;
}

void Zscroll_bar_set_viewport_size(const Zid id, const double viewport_size) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_SCROLLBAR);
	if (!e) return;
	SCROLLBAR(e)->viewport = viewport_size < 0 ? 0 : viewport_size;
	zrn_changed_paint(e);
}

void Zscroll_bar_on_scroll(const Zid id, const ZScrollCallback callback, void* const user_data) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_SCROLLBAR);
	if (!e) return;
	SCROLLBAR(e)->on_scroll = callback;
	SCROLLBAR(e)->on_scroll_data = user_data;
}


/* ============================================================================
 *  Thumb
 * ============================================================================ */

#define THUMB(e) ZST_AS(zrn_thumb_t, e)

static void thumb_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_control_t* const c = &THUMB(e)->base;
	ZColor fill = zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_FOREGROUND));
	if (THUMB(e)->dragging) fill = zrn_theme_color(e, ZST_TC_ACCENT);
	zrn_fill_rect(p, zrn_local_rect(e), fill);
}

static int thumb_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto t = THUMB(e);
	ZPoint root;
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			root = zrn_to_root(e, (ZPoint){ in->x, in->y });
			t->dragging = 1;
			t->last_x = root.x;
			t->last_y = root.y;
			t->total_x = t->total_y = 0;
			if (e->root) zrn_set_capture(e->root, e);
			zrn_changed_paint(e);
			if (t->started) t->started(e->id, in->x, in->y, t->started_data);
			return 1;
		case ZST_IN_POINTER_MOVE: {
			float dx, dy;
			if (!t->dragging) return 0;
			root = zrn_to_root(e, (ZPoint){ in->x, in->y });
			dx = root.x - t->last_x;
			dy = root.y - t->last_y;
			t->last_x = root.x;
			t->last_y = root.y;
			t->total_x += dx;
			t->total_y += dy;
			if (t->delta && (dx != 0 || dy != 0)) t->delta(e->id, dx, dy, t->delta_data);
			return 1;
		}
		case ZST_IN_POINTER_UP:
			if (!t->dragging) return 0;
			t->dragging = 0;
			if (e->root) zrn_release_capture(e->root, e);
			zrn_changed_paint(e);
			if (t->completed) t->completed(e->id, t->total_x, t->total_y, t->completed_data);
			return 1;
		case ZST_IN_CAPTURE_LOST:
			t->dragging = 0;
			return 0;
		default:
			return 0;
	}
}

static void thumb_init(zrn_element_t* e) { e->focusable = 0; }

zrn_class_t zrn_class_thumb = {
	.base = &zrn_class_control, .name = "Thumb", .id = Z_CLASS_THUMB, .size = sizeof(zrn_thumb_t),
	.init = thumb_init, .paint = thumb_paint, .input = thumb_input
};

Zid Zthumb_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_THUMB);
	return e ? e->id : Z_INVALID_ID;
}

void Zthumb_on_drag_started(const Zid id, const ZDragCallback callback, void* const user_data) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->started = callback;
	THUMB(e)->started_data = user_data;
}

void Zthumb_on_drag_delta(const Zid id, const ZDragCallback callback, void* const user_data) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->delta = callback;
	THUMB(e)->delta_data = user_data;
}

void Zthumb_on_drag_completed(const Zid id, const ZDragCallback callback, void* const user_data) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->completed = callback;
	THUMB(e)->completed_data = user_data;
}

int Zthumb_is_dragging(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_THUMB);
	return e ? THUMB(e)->dragging : 0;
}
