/**
 * Kestrel - controls/range.c
 * RangeBase, Slider, ProgressBar, ProgressRing, ScrollBar and Thumb. Slider,
 * ProgressBar and ProgressRing follow the Windows Phone 8.1 templates; the
 * ScrollBar and the Thumb are still the WP8 ones.
 */

#include "../internal.h"

#define RANGE(e) KST_AS(kst_range_base_t, e)


/* ============================================================================
 *  RangeBase
 * ============================================================================ */

static double coerce(const kst_range_base_t* r, double v) {
	if (v > r->maximum) v = r->maximum;
	if (v < r->minimum) v = r->minimum;
	return v;
}

/* Sets the value; fires ValueChanged. @return K_TRUE when the value changed. */
static int range_assign(kst_range_base_t* r, double value) {
	double old = r->value;
	value = coerce(r, value);
	if (value == old) return 0;
	r->value = value;
	kst_changed_paint(&r->base.base);
	if (r->changed) r->changed(r->base.base.id, old, value, r->changed_data);
	return 1;
}

static void range_init(kst_element_t* e) {
	kst_range_base_t* r = RANGE(e);
	r->minimum = 0;
	r->maximum = 1;
	r->small_change = 0.1;
	r->large_change = 1;
}

kst_class_t kst_class_range_base = {
	.base = &kst_class_control, .name = "RangeBase", .id = K_CLASS_RANGE_BASE, .size = sizeof(kst_range_base_t), .abstract_class = 1,
	.init = range_init
};

static kst_range_base_t* get_range(Kid id) { return RANGE(kst_get(id, K_CLASS_RANGE_BASE)); }

void Krange_base_set_value(Kid id, double value) {
	kst_range_base_t* r = get_range(id);
	if (r) range_assign(r, value);
}

double Krange_base_get_value(Kid id) { kst_range_base_t* r = get_range(id); return r ? r->value : 0; }
double Krange_base_get_minimum(Kid id) { kst_range_base_t* r = get_range(id); return r ? r->minimum : 0; }
double Krange_base_get_maximum(Kid id) { kst_range_base_t* r = get_range(id); return r ? r->maximum : 0; }

void Krange_base_set_range(Kid id, double minimum, double maximum) {
	kst_range_base_t* r = get_range(id);
	if (!r) return;
	if (maximum < minimum) maximum = minimum;
	r->minimum = minimum;
	r->maximum = maximum;
	{
		Kid eid = r->base.base.id;
		kst_changed_paint(&r->base.base);
		range_assign(r, r->value);
		if (kst_lookup(eid)) kst_changed_paint(&r->base.base);
	}
}

void Krange_base_set_changes(Kid id, double small_change, double large_change) {
	kst_range_base_t* r = get_range(id);
	if (!r) return;
	r->small_change = small_change;
	r->large_change = large_change;
}

void Krange_base_on_value_changed(Kid id, KValueChangedCallback callback, void* user_data) {
	kst_range_base_t* r = get_range(id);
	if (!r) return;
	r->changed = callback;
	r->changed_data = user_data;
}

static double range_fraction(const kst_range_base_t* r) {
	return r->maximum > r->minimum ? (r->value - r->minimum) / (r->maximum - r->minimum) : 0;
}


/* A cubic Bezier KeySpline (0,0)-(x1,y1)-(x2,y2)-(1,1): the y of an x in [0, 1]. */
static float key_spline(float x, float x1, float y1, float x2, float y2) {
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
		float u = 1 - t;
		return 3 * u * u * t * y1 + 3 * u * t * t * y2 + t * t * t;
	}
}

static float lerp(float a, float b, float t) { return a + (b - a) * t; }


/* ============================================================================
 *  Slider
 *  The 8.1 template: [Header] and under it a 57 px band: 17.5 px, the 9 px
 *  track, 30.5 px (ticks above and below); the thumb is 9.5 x 19, 13 px
 *  from the top of the band, and the accent fills the track up to it.
 *  Vertical: the same turned (36 px wide, the value grows upward, and the
 *  template gives its thumb 9 x 9). The Padding of the control is the margin
 *  of the template's grid.
 * ============================================================================ */

#define SLIDER(e) KST_AS(kst_slider_t, e)
#define SL_THUMB_W 9.5f
#define SL_THUMB_H 19.0f
#define SL_TRACK 9.0f
#define SL_BAND_ABOVE 17.5f
#define SL_BAND_BELOW 30.5f
#define SL_TICK 5.0f
#define SL_VERTICAL_W 36.0f
#define SL_VERTICAL_SIDE 13.5f
#define SL_VERTICAL_THUMB 9.0f

static kst_font_t slider_header_font(void) { kst_font_t f = { K_FONT_NORMAL, KST_81_FONT_CONTROL_HEADER, 0 }; return f; }

static float slider_header_height(const kst_slider_t* s) {
	return s->header && *s->header ? kst_font_line_height(slider_header_font()) : 0;
}

static KSize slider_measure(kst_element_t* e, KSize available) {
	kst_slider_t* s = SLIDER(e);
	KThickness pad = s->base.base.padding;
	float along = s->orientation == K_HORIZONTAL ? available.width : available.height;
	float header = slider_header_height(s);
	if (along >= K_INFINITE * 0.5f) along = 200.0f + (s->orientation == K_HORIZONTAL ? kst_thickness_h(pad) : kst_thickness_v(pad) + header);
	if (s->orientation == K_HORIZONTAL) return kst_size(along, kst_thickness_v(pad) + header + SL_BAND_ABOVE + SL_TRACK + SL_BAND_BELOW);
	return kst_size(kst_thickness_h(pad) + SL_VERTICAL_W, along);
}

/* Track, thumb and the accent part of the track, in local coordinates; the band is the area under the header. */
static void slider_geometry(kst_element_t* e, KRect* track, KRect* thumb, KRect* fill) {
	kst_slider_t* s = SLIDER(e);
	KThickness pad = s->base.base.padding;
	double f = range_fraction(&s->base);
	float top = pad.top + slider_header_height(s);
	if (s->orientation == K_HORIZONTAL) {
		float length = kst_maxf(0, e->rect.width - kst_thickness_h(pad));
		float x = pad.left + (float)f * kst_maxf(0, length - SL_THUMB_W);
		*track = kst_rect(pad.left, top + SL_BAND_ABOVE, length, SL_TRACK);
		*thumb = kst_rect(x, top + 13.0f, SL_THUMB_W, SL_THUMB_H);
		*fill = kst_rect(pad.left, track->y, x - pad.left, SL_TRACK);
	} else {
		float length = kst_maxf(0, e->rect.height - top - pad.bottom);
		float y = top + (1.0f - (float)f) * kst_maxf(0, length - SL_VERTICAL_THUMB);
		float tx = pad.left + SL_VERTICAL_SIDE;
		*track = kst_rect(tx, top, SL_TRACK, length);
		*thumb = kst_rect(tx, y, SL_VERTICAL_THUMB, SL_VERTICAL_THUMB);
		*fill = kst_rect(tx, y + SL_VERTICAL_THUMB, SL_TRACK, top + length - y - SL_VERTICAL_THUMB);
	}
}

static void slider_paint_ticks(kst_element_t* e, kst_painter_t* p, KRect track) {
	kst_slider_t* s = SLIDER(e);
	double range = s->base.maximum - s->base.minimum, v;
	KColor outside = kst_theme_color(e, KST_TC_BASE_MEDIUM_MID);
	KColor inline_color = kst_theme_color(e, KST_TC_BACKGROUND);
	int count = 0;
	if (s->tick_frequency <= 0 || range <= 0 || s->tick_placement == K_TICK_NONE) return;
	for (v = s->base.minimum; v <= s->base.maximum + 1e-9 && count < 1000; v += s->tick_frequency, count++) {
		float f = (float)((v - s->base.minimum) / range);
		int top = s->tick_placement == K_TICK_TOP_LEFT || s->tick_placement == K_TICK_OUTSIDE;
		int bottom = s->tick_placement == K_TICK_BOTTOM_RIGHT || s->tick_placement == K_TICK_OUTSIDE;
		if (s->orientation == K_HORIZONTAL) {
			float x = track.x + SL_THUMB_W * 0.5f + f * kst_maxf(0, track.width - SL_THUMB_W);
			if (top) kst_fill_rect(p, kst_rect(x, track.y - (SL_BAND_ABOVE - SL_TICK) * 0.5f - SL_TICK, 1, SL_TICK), outside);
			if (bottom) kst_fill_rect(p, kst_rect(x, track.y + SL_TRACK + (SL_BAND_BELOW - SL_TICK) * 0.5f, 1, SL_TICK), outside);
			if (s->tick_placement == K_TICK_INLINE) kst_fill_rect(p, kst_rect(x, track.y, 1, SL_TRACK), inline_color);
		} else {
			float y = track.y + track.height - SL_VERTICAL_THUMB * 0.5f - f * kst_maxf(0, track.height - SL_VERTICAL_THUMB);
			if (top) kst_fill_rect(p, kst_rect(track.x - 1.5f - SL_TICK, y, SL_TICK, 1), outside);
			if (bottom) kst_fill_rect(p, kst_rect(track.x + SL_TRACK + 1.5f, y, SL_TICK, 1), outside);
			if (s->tick_placement == K_TICK_INLINE) kst_fill_rect(p, kst_rect(track.x, y, SL_TRACK, 1), inline_color);
		}
	}
}

static void slider_paint(kst_element_t* e, kst_painter_t* p) {
	kst_slider_t* s = SLIDER(e);
	kst_control_t* c = &s->base.base;
	KRect track, thumb, fill;
	int enabled = kst_effectively_enabled(e);
	KColor track_color = kst_ctl_background(c, kst_theme_color(e, KST_TC_SLIDER_TRACK));
	KColor fill_color = kst_ctl_color(c, KST_C_FOREGROUND, c->foreground, kst_theme_color(e, KST_TC_ACCENT));
	KColor thumb_color = kst_theme_color(e, KST_TC_SLIDER_THUMB);
	if (!enabled) {
		fill_color = kst_theme_color(e, KST_TC_SLIDER_DECREASE_DISABLED);
		thumb_color = kst_theme_color(e, KST_TC_SLIDER_THUMB_DISABLED);
	}
	slider_geometry(e, &track, &thumb, &fill);
	if (slider_header_height(s) > 0) {
		kst_draw_string(p, slider_header_font(), s->header, (int)strlen(s->header), c->padding.left, c->padding.top, kst_theme_color(e, KST_TC_BASE_MID));
	}
	kst_fill_rect(p, track, track_color);
	if (fill.width > 0 && fill.height > 0) kst_fill_rect(p, fill, fill_color);
	slider_paint_ticks(e, p, track);
	kst_fill_rect(p, thumb, thumb_color);
}

/* The value a touch at (x, y) means: the thumb is centered on it, then snapped to StepFrequency or TickFrequency. */
static void slider_set_from(kst_element_t* e, float x, float y) {
	kst_slider_t* s = SLIDER(e);
	KThickness pad = s->base.base.padding;
	float top = pad.top + slider_header_height(s);
	double f, v, step = s->snaps_to == K_SNAPS_TO_TICKS && s->tick_frequency > 0 ? s->tick_frequency : s->step_frequency;
	if (s->orientation == K_HORIZONTAL) {
		float length = e->rect.width - kst_thickness_h(pad) - SL_THUMB_W;
		f = length > 0 ? (x - pad.left - SL_THUMB_W * 0.5f) / length : 0;
	} else {
		float length = e->rect.height - top - pad.bottom - SL_VERTICAL_THUMB;
		f = length > 0 ? 1.0 - (y - top - SL_VERTICAL_THUMB * 0.5f) / length : 0;
	}
	if (f < 0) f = 0;
	if (f > 1) f = 1;
	v = s->base.minimum + f * (s->base.maximum - s->base.minimum);
	if (step > 0) v = s->base.minimum + floor((v - s->base.minimum) / step + 0.5) * step;
	range_assign(&s->base, v);
}

/* A key moves by SmallChange (arrows) or LargeChange (page keys), snapped to the step. */
static void slider_step(kst_slider_t* s, double delta) {
	double v = s->base.value + delta, step = s->snaps_to == K_SNAPS_TO_TICKS && s->tick_frequency > 0 ? s->tick_frequency : s->step_frequency;
	if (step > 0) v = s->base.minimum + floor((v - s->base.minimum) / step + 0.5) * step;
	range_assign(&s->base, v);
}

static int slider_input(kst_element_t* e, kst_input_t* in) {
	kst_slider_t* s = SLIDER(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			if (!kst_effectively_enabled(e)) return 1;
			s->dragging = 1;
			if (e->root) kst_set_capture(e->root, e);
			slider_set_from(e, in->x, in->y);
			return 1;
		case KST_IN_POINTER_MOVE:
			if (!s->dragging) return 0;
			slider_set_from(e, in->x, in->y);
			return 1;
		case KST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) kst_release_capture(e->root, e);
			return 1;
		case KST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		case KST_IN_KEY_DOWN: {
			kst_range_base_t* r = &s->base;
			int horizontal = s->orientation == K_HORIZONTAL;
			int forward = horizontal ? K_KEY_RIGHT : K_KEY_UP, back = horizontal ? K_KEY_LEFT : K_KEY_DOWN;
			if (in->key == forward) slider_step(s, r->small_change);
			else if (in->key == K_KEY_PAGE_UP) slider_step(s, r->large_change);
			else if (in->key == back) slider_step(s, -r->small_change);
			else if (in->key == K_KEY_PAGE_DOWN) slider_step(s, -r->large_change);
			else if (in->key == K_KEY_HOME) range_assign(r, r->minimum);
			else if (in->key == K_KEY_END) range_assign(r, r->maximum);
			else return 0;
			return 1;
		}
		default:
			return 0;
	}
}

static void slider_init(kst_element_t* e) {
	kst_slider_t* s = SLIDER(e);
	s->orientation = K_HORIZONTAL;
	s->base.maximum = 100;
	s->base.small_change = 1;
	s->base.large_change = 10;
	s->step_frequency = 1;
	s->tick_placement = K_TICK_OUTSIDE;
	s->snaps_to = K_SNAPS_TO_STEP_VALUES;
}

static void slider_destroy(kst_element_t* e) { free(SLIDER(e)->header); }

kst_class_t kst_class_slider = {
	.base = &kst_class_range_base, .name = "Slider", .id = K_CLASS_SLIDER, .size = sizeof(kst_slider_t),
	.init = slider_init, .destroy = slider_destroy, .measure = slider_measure, .paint = slider_paint, .input = slider_input
};

Kid Kslider_create(KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_SLIDER);
	if (!e) return K_INVALID_ID;
	SLIDER(e)->orientation = orientation;
	return e->id;
}

static kst_slider_t* get_slider(Kid id) { return SLIDER(kst_get(id, K_CLASS_SLIDER)); }

void Kslider_set_orientation(Kid id, KOrientation orientation) {
	kst_slider_t* s = get_slider(id);
	if (!s) return;
	s->orientation = orientation;
	kst_changed_layout(&s->base.base.base);
}

void Kslider_set_header(Kid id, const char* header) {
	kst_slider_t* s = get_slider(id);
	if (!s) return;
	kst_set_string(&s->header, header && *header ? header : NULL);
	kst_changed_layout(&s->base.base.base);
}

const char* Kslider_get_header(Kid id) {
	kst_slider_t* s = get_slider(id);
	return s && s->header ? s->header : "";
}

void Kslider_set_step_frequency(Kid id, double step_frequency) {
	kst_slider_t* s = get_slider(id);
	if (s) s->step_frequency = step_frequency < 0 ? 0 : step_frequency;
}

double Kslider_get_step_frequency(Kid id) { kst_slider_t* s = get_slider(id); return s ? s->step_frequency : 0; }

void Kslider_set_tick_frequency(Kid id, double tick_frequency) {
	kst_slider_t* s = get_slider(id);
	if (!s) return;
	s->tick_frequency = tick_frequency < 0 ? 0 : tick_frequency;
	kst_changed_paint(&s->base.base.base);
}

double Kslider_get_tick_frequency(Kid id) { kst_slider_t* s = get_slider(id); return s ? s->tick_frequency : 0; }

void Kslider_set_tick_placement(Kid id, KTickPlacement placement) {
	kst_slider_t* s = get_slider(id);
	if (!s) return;
	s->tick_placement = placement;
	kst_changed_paint(&s->base.base.base);
}

void Kslider_set_snaps_to(Kid id, KSliderSnapsTo snaps_to) {
	kst_slider_t* s = get_slider(id);
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

#define PROGRESS(e) KST_AS(kst_progress_bar_t, e)
#define PB_HEIGHT 3.5f
#define PB_DOT 3.5f
#define PB_CYCLE_MS 3917.0f

static KSize progress_measure(kst_element_t* e, KSize available) {
	kst_control_t* c = &PROGRESS(e)->base.base;
	float w = available.width >= K_INFINITE * 0.5f ? 100.0f + kst_thickness_h(c->padding) : available.width;
	return kst_size(w, PB_HEIGHT + kst_thickness_v(c->padding));
}

/* The position of the i-th square (0 is the one in front) `t` seconds into the round, relative to its place in the row. */
static float progress_dot_offset(int i, float t, float width) {
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

static void progress_paint(kst_element_t* e, kst_painter_t* p) {
	kst_progress_bar_t* b = PROGRESS(e);
	kst_control_t* c = &b->base.base;
	KColor accent = kst_theme_color(e, KST_TC_ACCENT);
	KColor fg = kst_ctl_color(c, KST_C_FOREGROUND, c->foreground, accent);
	if (b->indeterminate) {
		float pitch = 2 * PB_DOT, t = b->time_ms / 1000.0f, group = 4 * pitch + PB_DOT;
		float container = lerp(-group, e->rect.width, b->time_ms / PB_CYCLE_MS);
		float y = (e->rect.height - PB_DOT) * 0.5f;
		int i;
		kst_push_clip(p, kst_local_rect(e));
		for (i = 0; i < 5; i++) {
			float u = t - 0.167f * (float)i;
			if (u < 0 || u >= 3.0f) continue;
			kst_fill_rect(p, kst_rect(container + (float)(4 - i) * pitch + progress_dot_offset(i, t, e->rect.width), y, PB_DOT, PB_DOT), fg);
		}
		kst_pop_clip(p);
		return;
	}
	{
		KRect root = kst_local_rect(e);
		KRect area = kst_rect(c->padding.left, c->padding.top, kst_maxf(0, e->rect.width - kst_thickness_h(c->padding)), kst_maxf(0, e->rect.height - kst_thickness_v(c->padding)));
		KColor track = kst_ctl_background(c, kst_color_alpha(accent, 0.16f));
		KColor indicator = b->show_paused ? kst_color_alpha(fg, 0.5f) : fg;
		kst_fill_rect(p, root, track);
		if (!b->show_error) kst_fill_rect(p, kst_rect(area.x, area.y, area.width * (float)range_fraction(&b->base), area.height), indicator);
	}
}

static void progress_tick(kst_element_t* e, float dt) {
	kst_progress_bar_t* b = PROGRESS(e);
	if (!b->indeterminate) return;
	b->time_ms += dt;
	if (b->time_ms >= PB_CYCLE_MS) b->time_ms = fmodf(b->time_ms, PB_CYCLE_MS);
	kst_changed_paint(e);
}

static void progress_init(kst_element_t* e) {
	kst_progress_bar_t* b = PROGRESS(e);
	b->base.maximum = 100;
	e->min_height = PB_HEIGHT;
	e->valign = K_VALIGN_CENTER;
	e->hit_visible = 0;
	e->focusable = 0;
	e->tab_stop = 0;
}

kst_class_t kst_class_progress_bar = {
	.base = &kst_class_range_base, .name = "ProgressBar", .id = K_CLASS_PROGRESS_BAR, .size = sizeof(kst_progress_bar_t),
	.init = progress_init, .measure = progress_measure, .paint = progress_paint, .tick = progress_tick
};

Kid Kprogress_bar_create(void) {
	kst_element_t* e = kst_create(K_CLASS_PROGRESS_BAR);
	return e ? e->id : K_INVALID_ID;
}

void Kprogress_bar_set_is_indeterminate(Kid id, int is_indeterminate) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->indeterminate = is_indeterminate != 0;
	PROGRESS(e)->time_ms = 0;
	e->wants_tick = is_indeterminate != 0;
	kst_changed_paint(e);
}

int Kprogress_bar_get_is_indeterminate(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_BAR);
	return e ? PROGRESS(e)->indeterminate : 0;
}

void Kprogress_bar_set_show_error(Kid id, int show_error) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->show_error = show_error != 0;
	kst_changed_paint(e);
}

void Kprogress_bar_set_show_paused(Kid id, int show_paused) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_BAR);
	if (!e) return;
	PROGRESS(e)->show_paused = show_paused != 0;
	kst_changed_paint(e);
}


/* ============================================================================
 *  ProgressRing
 *  Five 6.5 px dots in a 57.5 px ring, run by the storyboard of the template: each
 *  dot (0.167 s after the one before) is turned by the keyframes of E1R, spread
 *  6 degrees apart, for 3.21 s, and the whole round is 4.305 s. While it is active
 *  the control paints its Background (the page's) over its whole area at 0.8 opacity.
 *  The Large state of the template (a sixth dot above 60 px) is not made.
 * ============================================================================ */

#define RING(e) KST_AS(kst_progress_ring_t, e)
#define PR_SIZE 57.5f
#define PR_DOT 6.5f
#define PR_DOT_MARGIN 7.5f
#define PR_CYCLE_MS 4305.0f

static float progress_ring_angle(float u) {
	static const float times[7] = { 0, 0.433f, 1.2f, 1.617f, 2.017f, 2.783f, 3.217f };
	static const float angles[7] = { -110, 10, 93, 205, 357, 439, 585 };
	/* the KeySpline of the segment that ends at a keyframe; linear where the keyframe has none */
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

static void progress_ring_paint(kst_element_t* e, kst_painter_t* p) {
	kst_progress_ring_t* r = RING(e);
	kst_control_t* c = &r->base;
	KColor fg = kst_ctl_color(c, KST_C_FOREGROUND, c->foreground, kst_theme_color(e, KST_TC_ACCENT));
	float cx = e->rect.width * 0.5f, cy = e->rect.height * 0.5f, t = r->time_ms / 1000.0f, start = PR_DOT_MARGIN + PR_DOT * 0.5f - PR_SIZE * 0.5f;
	int i;
	if (!r->is_active) return;
	kst_fill_rect(p, kst_local_rect(e), kst_color_alpha(kst_ctl_background(c, kst_theme_color(e, KST_TC_BACKGROUND)), 0.8f));
	for (i = 0; i < 5; i++) {
		float u = t - 0.167f * (float)i, a, x, y;
		if (u < 0 || u >= 3.21f) continue;
		a = (progress_ring_angle(u) - 6.0f * (float)i) * KST_PI / 180.0f;
		/* the dot sits at the top left of the ring and turns about its center */
		x = start * cosf(a) - start * sinf(a);
		y = start * sinf(a) + start * cosf(a);
		kst_fill_ellipse(p, kst_rect(cx + x - PR_DOT * 0.5f, cy + y - PR_DOT * 0.5f, PR_DOT, PR_DOT), fg);
	}
}

static void progress_ring_tick(kst_element_t* e, float dt) {
	kst_progress_ring_t* r = RING(e);
	if (!r->is_active) return;
	r->time_ms += dt;
	if (r->time_ms >= PR_CYCLE_MS) r->time_ms = fmodf(r->time_ms, PR_CYCLE_MS);
	kst_changed_paint(e);
}

static void progress_ring_init(kst_element_t* e) {
	e->min_width = PR_SIZE;
	e->min_height = PR_SIZE;
	e->hit_visible = 0;
	e->focusable = 0;
	e->tab_stop = 0;
}

static KSize progress_ring_measure(kst_element_t* e, KSize available) {
	KST_UNUSED(e); KST_UNUSED(available);
	return kst_size(PR_SIZE, PR_SIZE);
}

kst_class_t kst_class_progress_ring = {
	.base = &kst_class_control, .name = "ProgressRing", .id = K_CLASS_PROGRESS_RING, .size = sizeof(kst_progress_ring_t),
	.init = progress_ring_init, .measure = progress_ring_measure, .paint = progress_ring_paint, .tick = progress_ring_tick
};

Kid Kprogress_ring_create(void) {
	kst_element_t* e = kst_create(K_CLASS_PROGRESS_RING);
	return e ? e->id : K_INVALID_ID;
}

void Kprogress_ring_set_is_active(Kid id, int is_active) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_RING);
	if (!e) return;
	is_active = is_active != 0;
	if (RING(e)->is_active == is_active) return;
	RING(e)->is_active = is_active;
	RING(e)->time_ms = 0;
	e->wants_tick = is_active;
	kst_changed_paint(e);
}

int Kprogress_ring_get_is_active(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PROGRESS_RING);
	return e ? RING(e)->is_active : 0;
}


/* ============================================================================
 *  ScrollBar
 *  The WP8 bar is a thin, 5 px thumb of at least 72 px with a 1 px margin,
 *  and it does not take the pointer (IsHitTestVisible False); here it does
 *  once Kelement_set_hit_test_visible turns that on.
 * ============================================================================ */

#define SCROLLBAR(e) KST_AS(kst_scrollbar_t, e)

static KSize scrollbar_measure(kst_element_t* e, KSize available) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	KST_UNUSED(available);
	return s->orientation == K_VERTICAL ? kst_size(5, 0) : kst_size(0, 5);
}

static KRect scrollbar_thumb(kst_element_t* e) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	float length = s->orientation == K_VERTICAL ? e->rect.height : e->rect.width;
	double range = s->base.maximum - s->base.minimum;
	float thumb = range + s->viewport > 0 ? (float)(s->viewport / (range + s->viewport)) * length : length;
	float pos;
	thumb = kst_clampf(thumb, kst_minf(72.0f, length), length);
	pos = range > 0 ? (float)range_fraction(&s->base) * (length - thumb) : 0;
	if (s->orientation == K_VERTICAL) return kst_rect(1, pos + 1, e->rect.width - 2, thumb - 2);
	return kst_rect(pos + 1, 1, thumb - 2, e->rect.height - 2);
}

static void scrollbar_paint(kst_element_t* e, kst_painter_t* p) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	kst_control_t* c = &s->base.base;
	if (s->base.maximum <= s->base.minimum) return;
	kst_fill_rect(p, scrollbar_thumb(e), kst_ctl_background(c, kst_theme_color(e, KST_TC_DISABLED)));
}

static void scrollbar_scroll(kst_element_t* e, KScrollEventType type, double value) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	Kid id = e->id;
	range_assign(&s->base, value);
	if (!kst_lookup(id)) return; /* ValueChanged destroyed the bar */
	if (s->on_scroll) s->on_scroll(id, type, s->base.value, s->on_scroll_data);
}

static int scrollbar_input(kst_element_t* e, kst_input_t* in) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	int vertical = s->orientation == K_VERTICAL;
	float pos = vertical ? in->y : in->x;
	switch (in->type) {
		case KST_IN_POINTER_DOWN: {
			KRect t = scrollbar_thumb(e);
			float start = vertical ? t.y : t.x, size = vertical ? t.height : t.width;
			if (pos >= start && pos <= start + size) {
				s->dragging = 1;
				s->drag_grab = pos - start;
				if (e->root) kst_set_capture(e->root, e);
			} else {
				scrollbar_scroll(e, pos < start ? K_SCROLL_LARGE_DECREMENT : K_SCROLL_LARGE_INCREMENT, s->base.value + (pos < start ? -s->base.large_change : s->base.large_change));
			}
			return 1;
		}
		case KST_IN_POINTER_MOVE:
			if (s->dragging) {
				float length = vertical ? e->rect.height : e->rect.width;
				KRect t = scrollbar_thumb(e);
				float room = length - (vertical ? t.height : t.width) - 2;
				double f = room > 0 ? (pos - s->drag_grab - 1) / room : 0;
				f = f < 0 ? 0 : (f > 1 ? 1 : f);
				scrollbar_scroll(e, K_SCROLL_THUMB_TRACK, s->base.minimum + f * (s->base.maximum - s->base.minimum));
				return 1;
			}
			return 0;
		case KST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) kst_release_capture(e->root, e);
			scrollbar_scroll(e, K_SCROLL_END, s->base.value);
			return 1;
		case KST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		default:
			return 0;
	}
}

static void scrollbar_init(kst_element_t* e) {
	kst_scrollbar_t* s = SCROLLBAR(e);
	s->orientation = K_VERTICAL;
	s->base.maximum = 1;
	s->base.base.padding = Kthickness_uniform(1);
	s->viewport = 0.1;
	e->hit_visible = 0;
	e->focusable = 0;
}

kst_class_t kst_class_scrollbar = {
	.base = &kst_class_range_base, .name = "ScrollBar", .id = K_CLASS_SCROLLBAR, .size = sizeof(kst_scrollbar_t),
	.init = scrollbar_init, .measure = scrollbar_measure, .paint = scrollbar_paint, .input = scrollbar_input
};

Kid Kscroll_bar_create(KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_SCROLLBAR);
	if (!e) return K_INVALID_ID;
	SCROLLBAR(e)->orientation = orientation;
	return e->id;
}

void Kscroll_bar_set_viewport_size(Kid id, double viewport_size) {
	kst_element_t* e = kst_get(id, K_CLASS_SCROLLBAR);
	if (!e) return;
	SCROLLBAR(e)->viewport = viewport_size < 0 ? 0 : viewport_size;
	kst_changed_paint(e);
}

void Kscroll_bar_on_scroll(Kid id, KScrollCallback callback, void* user_data) {
	kst_element_t* e = kst_get(id, K_CLASS_SCROLLBAR);
	if (!e) return;
	SCROLLBAR(e)->on_scroll = callback;
	SCROLLBAR(e)->on_scroll_data = user_data;
}


/* ============================================================================
 *  Thumb
 * ============================================================================ */

#define THUMB(e) KST_AS(kst_thumb_t, e)

static void thumb_paint(kst_element_t* e, kst_painter_t* p) {
	kst_control_t* c = &THUMB(e)->base;
	KColor fill = kst_ctl_background(c, kst_theme_color(e, KST_TC_FOREGROUND));
	if (THUMB(e)->dragging) fill = kst_theme_color(e, KST_TC_ACCENT);
	kst_fill_rect(p, kst_local_rect(e), fill);
}

static int thumb_input(kst_element_t* e, kst_input_t* in) {
	kst_thumb_t* t = THUMB(e);
	KPoint root;
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			root = kst_to_root(e, (KPoint){ in->x, in->y });
			t->dragging = 1;
			t->last_x = root.x;
			t->last_y = root.y;
			t->total_x = t->total_y = 0;
			if (e->root) kst_set_capture(e->root, e);
			kst_changed_paint(e);
			if (t->started) t->started(e->id, in->x, in->y, t->started_data);
			return 1;
		case KST_IN_POINTER_MOVE: {
			float dx, dy;
			if (!t->dragging) return 0;
			root = kst_to_root(e, (KPoint){ in->x, in->y });
			dx = root.x - t->last_x;
			dy = root.y - t->last_y;
			t->last_x = root.x;
			t->last_y = root.y;
			t->total_x += dx;
			t->total_y += dy;
			if (t->delta && (dx != 0 || dy != 0)) t->delta(e->id, dx, dy, t->delta_data);
			return 1;
		}
		case KST_IN_POINTER_UP:
			if (!t->dragging) return 0;
			t->dragging = 0;
			if (e->root) kst_release_capture(e->root, e);
			kst_changed_paint(e);
			if (t->completed) t->completed(e->id, t->total_x, t->total_y, t->completed_data);
			return 1;
		case KST_IN_CAPTURE_LOST:
			t->dragging = 0;
			return 0;
		default:
			return 0;
	}
}

static void thumb_init(kst_element_t* e) { e->focusable = 0; }

kst_class_t kst_class_thumb = {
	.base = &kst_class_control, .name = "Thumb", .id = K_CLASS_THUMB, .size = sizeof(kst_thumb_t),
	.init = thumb_init, .paint = thumb_paint, .input = thumb_input
};

Kid Kthumb_create(void) {
	kst_element_t* e = kst_create(K_CLASS_THUMB);
	return e ? e->id : K_INVALID_ID;
}

void Kthumb_on_drag_started(Kid id, KDragCallback callback, void* user_data) {
	kst_element_t* e = kst_get(id, K_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->started = callback;
	THUMB(e)->started_data = user_data;
}

void Kthumb_on_drag_delta(Kid id, KDragCallback callback, void* user_data) {
	kst_element_t* e = kst_get(id, K_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->delta = callback;
	THUMB(e)->delta_data = user_data;
}

void Kthumb_on_drag_completed(Kid id, KDragCallback callback, void* user_data) {
	kst_element_t* e = kst_get(id, K_CLASS_THUMB);
	if (!e) return;
	THUMB(e)->completed = callback;
	THUMB(e)->completed_data = user_data;
}

int Kthumb_is_dragging(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_THUMB);
	return e ? THUMB(e)->dragging : 0;
}
