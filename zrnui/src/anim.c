/**
 * zrnui - anim.c
 * Animation: easing, the render transform of an element (PlaneProjection,
 * ScaleTransform, TranslateTransform), Zelement_animate, and the lean of a
 * pressed element (Tilt).
 *
 * The clock is the root's: the sum of the deltas of Zroot_tick, so a test can
 * move time by hand and an app that pauses stops its animations. An animation
 * that is running is advanced by Zroot_tick for the element that has it.
 */

#include "internal.h"

static constexpr float TILT_DEG = 14.0f;
static constexpr float TILT_LIFT = 40.0f;
static constexpr float PRESS_SINK = 0.045f;
static constexpr float TILT_IN_MS = 90.0f;
static constexpr float TILT_OUT_MS = 160.0f;


/* ============================================================================
 *  easing
 * ============================================================================ */

/* (0.2, 0.8, 0.2, 1) as a cubic Bezier */
static float ease_emphasized(const float t) {
	constexpr float x1 = 0.2f;
	constexpr float y1 = 0.8f;
	constexpr float x2 = 0.2f;
	constexpr float y2 = 1.0f;
	float s = t, u;
	for (int i = 0; i < 8; i++) {
		u = 1 - s;
		float dx = 3 * u * u * x1 + 6 * u * s * (x2 - x1) + 3 * s * s * (1 - x2);
		if (fabsf(dx) < 1e-5f) break;
		s -= (3 * u * u * s * x1 + 3 * u * s * s * x2 + s * s * s - t) / dx;
		s = zrn_clampf(s, 0, 1);
	}
	u = 1 - s;
	return 3 * u * u * s * y1 + 3 * u * s * s * y2 + s * s * s;
}

float zrn_ease(const ZEasing easing, float t) {
	t = zrn_clampf(t, 0, 1);
	switch (easing) {
	case Z_EASE_OUT: { const float u = 1 - t; return 1 - u * u * u; }
	case Z_EASE_IN: return t * t * t;
	case Z_EASE_EMPHASIZED: return ease_emphasized(t);
	default: return t;
	}
}

float Zeasing(const ZEasing easing, const float t) { return zrn_ease(easing, t); }


/* ============================================================================
 *  transforms: column-major 4x4, m[column * 4 + row]
 * ============================================================================ */

void zrn_xf_identity(float m[16]) {
	memset(m, 0, 16 * sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1;
}

void zrn_xf_mul(float out[16], const float a[16], const float b[16]) {
	float r[16];
	for (int c = 0; c < 4; c++) {
		for (int row = 0; row < 4; row++) {
			float s = 0;
			for (int k = 0; k < 4; k++) s += a[k * 4 + row] * b[c * 4 + k];
			r[c * 4 + row] = s;
		}
	}
	memcpy(out, r, sizeof r);
}

static void xf_translate(float out[16], const float x, const float y, const float z) {
	float t[16];
	zrn_xf_identity(t);
	t[12] = x; t[13] = y; t[14] = z;
	zrn_xf_mul(out, out, t);
}

/* positive takes the right-hand side away from the viewer; z grows toward the viewer */
static void xf_rotate_y(float out[16], const float deg) {
	const float a = deg * ZST_PI / 180.0f;
	float r[16];
	const float s = sinf(a);
	const float c = cosf(a);
	zrn_xf_identity(r);
	r[0] = c; r[8] = s; r[2] = -s; r[10] = c;
	zrn_xf_mul(out, out, r);
}

/* positive takes the bottom away from the viewer */
static void xf_rotate_x(float out[16], const float deg) {
	const float a = deg * ZST_PI / 180.0f;
	float r[16];
	const float c = cosf(a);
	const float s = sinf(a);
	zrn_xf_identity(r);
	r[5] = c; r[9] = s; r[6] = -s; r[10] = c;
	zrn_xf_mul(out, out, r);
}

/* positive turns clockwise on the screen (y grows down) */
static void xf_rotate_z(float out[16], const float deg) {
	const float a = deg * ZST_PI / 180.0f;
	float r[16];
	const float c = cosf(a);
	const float s = sinf(a);
	zrn_xf_identity(r);
	r[0] = c; r[1] = s; r[4] = -s; r[5] = c;
	zrn_xf_mul(out, out, r);
}

static void xf_scale(float out[16], const float s) {
	float r[16];
	zrn_xf_identity(r);
	r[0] = r[5] = r[10] = s;
	zrn_xf_mul(out, out, r);
}

void zrn_xf_of_element(const zrn_element_t* const e, const float origin_x, const float origin_y, const float width, const float height, const float scale, float out[16]) {
	const float cx = origin_x + e->proj_cx * width, cy = origin_y + e->proj_cy * height;
	zrn_xf_identity(out);
	/* applied last to first: the centre to the origin, the rotations (in the plane last), the scale, back, and the slide */
	xf_translate(out, cx + e->translate_x * scale, cy + e->translate_y * scale, 0);
	if (e->render_scale != 1.0f) xf_scale(out, e->render_scale);
	if (e->rot_z != 0.0f) xf_rotate_z(out, e->rot_z);
	if (e->rot_x != 0.0f) xf_rotate_x(out, e->rot_x);
	if (e->rot_y != 0.0f) xf_rotate_y(out, e->rot_y);
	xf_translate(out, -cx, -cy, 0);
}

void zrn_update_transformed(zrn_element_t* const e) {
	e->transformed = e->rot_x != 0.0f || e->rot_z != 0.0f || e->rot_y != 0.0f || e->render_scale != 1.0f || e->translate_x != 0.0f || e->translate_y != 0.0f;
}

void Zelement_set_projection(const Zid element, const float rotation_x, const float rotation_y, const float center_x, const float center_y) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	e->rot_x = rotation_x;
	e->rot_y = rotation_y;
	e->proj_cx = center_x;
	e->proj_cy = center_y;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_render_scale(const Zid element, const float scale) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	e->render_scale = scale;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_translate(const Zid element, const float x, const float y) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	e->translate_x = x;
	e->translate_y = y;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_rotation_z(const Zid element, const float degrees) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	e->rot_z = degrees;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_tilt(const Zid element, const int enabled) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (e) e->tilt = enabled != 0;
}


/* ============================================================================
 *  Zelement_animate
 * ============================================================================ */

static float* property_slot(zrn_element_t* const e, const ZAnimatedProperty p) {
	switch (p) {
	case Z_PROP_OPACITY: return &e->opacity;
	case Z_PROP_ROTATION_X: return &e->rot_x;
	case Z_PROP_ROTATION_Y: return &e->rot_y;
	case Z_PROP_ROTATION_Z: return &e->rot_z;
	case Z_PROP_SCALE: return &e->render_scale;
	case Z_PROP_TRANSLATE_X: return &e->translate_x;
	case Z_PROP_TRANSLATE_Y: return &e->translate_y;
	default: return nullptr;
	}
}

static void property_set(zrn_element_t* const e, const ZAnimatedProperty p, const float v) {
	float* const slot = property_slot(e, p);
	if (!slot) return;
	*slot = v;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

float Zelement_get_property(const Zid element, const ZAnimatedProperty property) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	const float* const slot = e ? property_slot(e, property) : nullptr;
	return slot ? *slot : 0.0f;
}

int zrn_animations_enabled(const zrn_root_t* const r) {
	if (!r) return 0;
	if (r->animations == Z_ANIMATIONS_ON) return 1;
	if (r->animations == Z_ANIMATIONS_OFF) return 0;
	return r->renderer == Z_RENDERER_GPU || r->gpu_window;
}

static void transition_ended(zrn_element_t* e);

/* Starts the tween of one property. The value it moves from is the one it has when the delay is over. */
static void tween_start(zrn_element_t* const e, const ZAnimatedProperty property, const float to, const float duration_ms, const ZEasing easing, const float delay_ms) {
	if (e->tweens) e->tweens->p[property].active = 0;
	if (!e->root || !zrn_animations_enabled(e->root) || (duration_ms <= 0 && delay_ms <= 0)) {
		property_set(e, property, to);
		return;
	}
	if (!e->tweens) {
		e->tweens = (zrn_tween_t*)calloc(1, sizeof *e->tweens);
		if (!e->tweens) {
			/* the value still gets where it was going */
			property_set(e, property, to);
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
	}
	e->tweens->p[property].active = 1;
	e->tweens->p[property].started = 0;
	e->tweens->p[property].from = 0;
	e->tweens->p[property].to = to;
	e->tweens->p[property].start = e->root->time_ms;
	e->tweens->p[property].duration = duration_ms;
	e->tweens->p[property].delay = delay_ms;
	e->tweens->p[property].easing = easing;
	zrn_changed_paint(e);
}

void Zelement_animate(const Zid element, const ZAnimatedProperty property, const float to, const float duration_ms, const ZEasing easing, const float delay_ms) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e || property < 0 || property >= Z_PROP_LAST_) return;
	tween_start(e, property, to, duration_ms, easing, delay_ms);
}

int Zelement_is_animating(const Zid element) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e || !e->tweens) return false;
	for (int i = 0; i < Z_PROP_LAST_; i++) if (e->tweens->p[i].active) return true;
	return false;
}

void zrn_tweens_tick(zrn_element_t* const e) {
	int i, any = 0;
	if (!e->tweens) return;
	const double now = e->root ? e->root->time_ms : 1e300;
	for (i = 0; i < Z_PROP_LAST_; i++) {
		zrn_tween_t* const t = e->tweens;
		if (!t->p[i].active) continue;
		const double u = now - t->p[i].start - (double)t->p[i].delay;
		if (u < 0) { any = 1; continue; }
		if (!t->p[i].started) {
			const float* const slot = property_slot(e, (ZAnimatedProperty)i);
			t->p[i].started = 1;
			t->p[i].from = slot ? *slot : 0.0f;
		}
		if (t->p[i].duration <= 0 || u >= (double)t->p[i].duration) {
			t->p[i].active = 0;
			property_set(e, (ZAnimatedProperty)i, t->p[i].to);
		} else {
			property_set(e, (ZAnimatedProperty)i, t->p[i].from + (t->p[i].to - t->p[i].from) * zrn_ease(t->p[i].easing, (float)(u / (double)t->p[i].duration)));
			any = 1;
		}
	}
	if (!any) {
		zrn_tweens_free(e);
		transition_ended(e);
	}
}

void zrn_tweens_free(zrn_element_t* const e) {
	free(e->tweens);
	e->tweens = nullptr;
}


/* ============================================================================
 *  transitions: the Windows Phone Toolkit's, by kind
 *
 *  Every kind is a few properties moving at once, each from a start to an end. Opacity is in
 *  fractions of what the element has (so a dimmed element fades back to its own opacity). The
 *  element shows the start at once, then the tweens run after the delay.
 *
 *  The numbers are estimates of the Toolkit's (docs/wp8-reference/90-transitions.md lists the
 *  modes and nothing else), except the feathered turnstile, which is as the Lumia does it.
 * ============================================================================ */

typedef struct {
	ZAnimatedProperty prop;
	float from, to;
	/** 0: the duration and easing of the kind. */
	float duration;
	ZEasing easing;
} tseg_t;

typedef struct {
	float duration;
	ZEasing easing;
	/** The centre of the rotations, fractions of the size; -1: it stays as it is. */
	float cx, cy;
	int n;
	tseg_t seg[3];
} tdef_t;

static constexpr float SLIDE_DIST = 150.0f;
static constexpr float ROW_STAGGER_MS = 28.0f;
static constexpr int ROW_STAGGER_MAX = 7;
static constexpr int ROW_MAX = 64;
static constexpr float NO_PIVOT = -1e9f;

#define OP(a, b) { Z_PROP_OPACITY, (a), (b), 0, Z_EASE_LINEAR }
#define MOVE(prop, a, b) { (prop), (a), (b), 0, Z_EASE_LINEAR }
#define IN_MS 350.0f, Z_EASE_OUT
#define OUT_MS 250.0f, Z_EASE_IN

static const tdef_t TDEFS[Z_TRANSITION_LAST_] = {
	[Z_TRANSITION_FADE_IN] = { 250.0f, Z_EASE_OUT, -1, -1, 1, { OP(0, 1) } },
	[Z_TRANSITION_FADE_OUT] = { 250.0f, Z_EASE_IN, -1, -1, 1, { OP(1, 0) } },

	[Z_TRANSITION_SLIDE_UP_FADE_IN] = { IN_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_Y, SLIDE_DIST, 0), OP(0, 1) } },
	[Z_TRANSITION_SLIDE_UP_FADE_OUT] = { OUT_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_Y, 0, -SLIDE_DIST), OP(1, 0) } },
	[Z_TRANSITION_SLIDE_DOWN_FADE_IN] = { IN_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_Y, -SLIDE_DIST, 0), OP(0, 1) } },
	[Z_TRANSITION_SLIDE_DOWN_FADE_OUT] = { OUT_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_Y, 0, SLIDE_DIST), OP(1, 0) } },
	[Z_TRANSITION_SLIDE_LEFT_FADE_IN] = { IN_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_X, SLIDE_DIST, 0), OP(0, 1) } },
	[Z_TRANSITION_SLIDE_LEFT_FADE_OUT] = { OUT_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_X, 0, -SLIDE_DIST), OP(1, 0) } },
	[Z_TRANSITION_SLIDE_RIGHT_FADE_IN] = { IN_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_X, -SLIDE_DIST, 0), OP(0, 1) } },
	[Z_TRANSITION_SLIDE_RIGHT_FADE_OUT] = { OUT_MS, -1, -1, 2, { MOVE(Z_PROP_TRANSLATE_X, 0, SLIDE_DIST), OP(1, 0) } },

	[Z_TRANSITION_SWIVEL_FULL_SCREEN_IN] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, -35, 0), OP(0, 1) } },
	[Z_TRANSITION_SWIVEL_FULL_SCREEN_OUT] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, 0, 35), OP(1, 0) } },
	[Z_TRANSITION_SWIVEL_FORWARD_IN] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, -45, 0), OP(0, 1) } },
	[Z_TRANSITION_SWIVEL_FORWARD_OUT] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, 0, 45), OP(1, 0) } },
	[Z_TRANSITION_SWIVEL_BACKWARD_IN] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, 45, 0), OP(0, 1) } },
	[Z_TRANSITION_SWIVEL_BACKWARD_OUT] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_X, 0, -45), OP(1, 0) } },

	/* about the left edge; the angles are those of the feathered turnstile (out -50 or +50, in from 80) */
	[Z_TRANSITION_TURNSTILE_FORWARD_IN] = { IN_MS, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 80, 0), OP(0, 1) } },
	[Z_TRANSITION_TURNSTILE_FORWARD_OUT] = { OUT_MS, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 0, -50), OP(1, 0) } },
	[Z_TRANSITION_TURNSTILE_BACKWARD_IN] = { IN_MS, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 80, 0), OP(0, 1) } },
	[Z_TRANSITION_TURNSTILE_BACKWARD_OUT] = { OUT_MS, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 0, 50), OP(1, 0) } },

	/* what one row does in the feathered turnstile: out 128 ms (the angle eased in, the opacity falling with it), in 180 ms
	 * (the angle eased out, the opacity up in the first third) */
	[Z_TRANSITION_FEATHER_FORWARD_IN] = { 180.0f, Z_EASE_OUT, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 80, 0), { Z_PROP_OPACITY, 0, 1, 60.0f, Z_EASE_LINEAR } } },
	[Z_TRANSITION_FEATHER_FORWARD_OUT] = { 128.0f, Z_EASE_IN, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 0, -50), OP(1, 0) } },
	[Z_TRANSITION_FEATHER_BACKWARD_IN] = { 180.0f, Z_EASE_OUT, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 80, 0), { Z_PROP_OPACITY, 0, 1, 60.0f, Z_EASE_LINEAR } } },
	[Z_TRANSITION_FEATHER_BACKWARD_OUT] = { 128.0f, Z_EASE_IN, 0, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Y, 0, 50), OP(1, 0) } },

	[Z_TRANSITION_ROTATE_IN_90_CLOCKWISE] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, -90, 0), OP(0, 1) } },
	[Z_TRANSITION_ROTATE_IN_90_COUNTERCLOCKWISE] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 90, 0), OP(0, 1) } },
	[Z_TRANSITION_ROTATE_IN_180_CLOCKWISE] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, -180, 0), OP(0, 1) } },
	[Z_TRANSITION_ROTATE_IN_180_COUNTERCLOCKWISE] = { IN_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 180, 0), OP(0, 1) } },
	[Z_TRANSITION_ROTATE_OUT_90_CLOCKWISE] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 0, 90), OP(1, 0) } },
	[Z_TRANSITION_ROTATE_OUT_90_COUNTERCLOCKWISE] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 0, -90), OP(1, 0) } },
	[Z_TRANSITION_ROTATE_OUT_180_CLOCKWISE] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 0, 180), OP(1, 0) } },
	[Z_TRANSITION_ROTATE_OUT_180_COUNTERCLOCKWISE] = { OUT_MS, 0.5f, 0.5f, 2, { MOVE(Z_PROP_ROTATION_Z, 0, -180), OP(1, 0) } },

	/* it rolls like a wheel: the slide and the turn together */
	[Z_TRANSITION_ROLL_IN] = { 450.0f, Z_EASE_OUT, 0.5f, 0.5f, 3, { MOVE(Z_PROP_TRANSLATE_X, SLIDE_DIST, 0), MOVE(Z_PROP_ROTATION_Z, 120, 0), OP(0, 1) } },
	[Z_TRANSITION_ROLL_OUT] = { 300.0f, Z_EASE_IN, 0.5f, 0.5f, 3, { MOVE(Z_PROP_TRANSLATE_X, 0, -SLIDE_DIST), MOVE(Z_PROP_ROTATION_Z, 0, -120), OP(1, 0) } },
};

#undef OP
#undef MOVE
#undef IN_MS
#undef OUT_MS

static int transition_is_feather(const ZTransition t) {
	return t >= Z_TRANSITION_FEATHER_FORWARD_IN && t <= Z_TRANSITION_FEATHER_BACKWARD_OUT;
}

static int transition_valid(const ZTransition t) {
	return t > Z_TRANSITION_NONE && t < Z_TRANSITION_LAST_ && t != Z_TRANSITION_TURNSTILE;
}

/* The opacity a transition fades to and from: the element's own, remembered from the first. */
static float base_opacity(zrn_element_t* const e) {
	if (e->trans_opacity <= 0.0f) e->trans_opacity = e->opacity > 0.0f ? e->opacity : 1.0f;
	return e->trans_opacity;
}

/* Plays `t` on one element. Returns when its last animation ends, in ms after now; 0 when nothing runs. */
static float transition_run(zrn_element_t* const e, const ZTransition t, const float delay_ms, const float pivot_x) {
	const tdef_t* const d = &TDEFS[t];
	const float base = base_opacity(e);
	const int animate = e->root && zrn_animations_enabled(e->root);
	float end = 0;
	if (d->cx >= 0.0f) {
		e->proj_cx = pivot_x > NO_PIVOT ? pivot_x : d->cx;
		e->proj_cy = d->cy;
	}
	for (int i = 0; i < d->n; i++) {
		const tseg_t* const sg = &d->seg[i];
		const float k = sg->prop == Z_PROP_OPACITY ? base : 1.0f;
		const float dur = sg->duration > 0 ? sg->duration : d->duration;
		const ZEasing ease = sg->duration > 0 ? sg->easing : d->easing;
		if (!animate) {
			if (e->tweens) e->tweens->p[sg->prop].active = 0;
			property_set(e, sg->prop, sg->to * k);
			continue;
		}
		property_set(e, sg->prop, sg->from * k);
		tween_start(e, sg->prop, sg->to * k, dur, ease, delay_ms);
		if (delay_ms + dur > end) end = delay_ms + dur;
	}
	zrn_update_transformed(e);
	return end;
}

/* The rows of a container, as the Frame finds them. */
static int collect_rows(const zrn_element_t* const e, Zid* const rows, int n) {
	if (e->visibility != Z_VISIBLE || n >= ROW_MAX || zrn_is_a(e, Z_CLASS_APP_BAR)) return n;
	if (zrn_is_row_container(e) && e->child_count > 0) {
		for (int i = 0; i < e->child_count; i++) n = collect_rows(e->children[i], rows, n);
		return n;
	}
	rows[n++] = e->id;
	return n;
}

static void transition_feather(zrn_element_t* const e, const ZTransition t, const float delay_ms) {
	Zid rows[ROW_MAX];
	const int forward = t == Z_TRANSITION_FEATHER_FORWARD_IN || t == Z_TRANSITION_FEATHER_FORWARD_OUT;
	if (e->root && e->root->layout_dirty) Zroot_update_layout(e->root->id);
	const float left = zrn_to_root(e, (ZPoint){ .x = 0, .y = 0 }).x;
	const int n = collect_rows(e, rows, 0);
	e->transition_rows = 1;
	for (int i = 0; i < n; i++) {
		zrn_element_t* const row = zrn_lookup(rows[i]);
		int order = forward ? i : n - 1 - i;
		float pivot = NO_PIVOT;
		if (!row) continue;
		if (order > ROW_STAGGER_MAX) order = ROW_STAGGER_MAX;
		/* every row turns about the left edge of the container, not its own */
		if (row->rect.width > 0) pivot = (left - zrn_to_root(row, (ZPoint){ .x = 0, .y = 0 }).x) / row->rect.width;
		row->transition_active = 1;
		if (transition_run(row, t, delay_ms + (float)order * ROW_STAGGER_MS, pivot) <= 0) transition_ended(row);
	}
}

static void transition_play(zrn_element_t* const e, const ZTransition t, const float delay_ms) {
	if (transition_is_feather(t)) {
		transition_feather(e, t, delay_ms);
		return;
	}
	e->transition_active = 1;
	if (transition_run(e, t, delay_ms, NO_PIVOT) <= 0) transition_ended(e);
}

/* The element has nothing more to play. The opacity it had is remembered only while a transition has it faded. */
static void transition_ended(zrn_element_t* const e) {
	if (!e->transition_active) return;
	e->transition_active = 0;
	if (e->trans_opacity > 0.0f && fabsf(e->opacity - e->trans_opacity) < 1e-4f) e->trans_opacity = 0;
	zrn_raise(e, Z_EVENT_TRANSITION_COMPLETED);
}

float zrn_transition_duration(const ZTransition t) {
	float end = 0;
	if (!transition_valid(t)) return 0;
	for (int i = 0; i < TDEFS[t].n; i++) {
		const float dur = TDEFS[t].seg[i].duration > 0 ? TDEFS[t].seg[i].duration : TDEFS[t].duration;
		if (dur > end) end = dur;
	}
	return end;
}

void zrn_transition_appear(zrn_element_t* const e) {
	if (transition_valid((ZTransition)e->appear)) transition_play(e, (ZTransition)e->appear, e->appear_delay);
}

void Zelement_play_transition(const Zid element, const ZTransition transition, const float delay_ms) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	if (transition == Z_TRANSITION_NONE) {
		if (e->tweens) for (int i = 0; i < Z_PROP_LAST_; i++) e->tweens->p[i].active = 0;
		e->transition_active = 0;
		return;
	}
	if (!transition_valid(transition)) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "Zelement_play_transition: not a transition of an element");
		return;
	}
	transition_play(e, transition, delay_ms > 0 ? delay_ms : 0);
}

void Zelement_set_transition(const Zid element, const ZTransition transition, const float delay_ms) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	if (transition != Z_TRANSITION_NONE && !transition_valid(transition)) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "Zelement_set_transition: not a transition of an element");
		return;
	}
	e->appear = transition;
	e->appear_delay = delay_ms > 0 ? delay_ms : 0;
	/* an element that is already shown plays it now; one that is not plays it when it appears */
	if (transition != Z_TRANSITION_NONE && e->root && e->visibility == Z_VISIBLE) transition_play(e, transition, e->appear_delay);
}

static void clear_one(zrn_element_t* const e) {
	if (e->tweens) for (int i = 0; i < Z_PROP_LAST_; i++) e->tweens->p[i].active = 0;
	e->rot_x = e->rot_y = e->rot_z = 0;
	e->render_scale = 1;
	e->translate_x = e->translate_y = 0;
	e->proj_cx = e->proj_cy = 0.5f;
	if (e->trans_opacity > 0.0f) e->opacity = e->trans_opacity;
	e->trans_opacity = 0;
	e->transition_active = 0;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_clear_transition(const Zid element) {
	zrn_element_t* const e = zrn_get(element, Z_CLASS_ELEMENT);
	if (!e) return;
	if (e->transition_rows) {
		Zid rows[ROW_MAX];
		const int n = collect_rows(e, rows, 0);
		e->transition_rows = 0;
		for (int i = 0; i < n; i++) {
			zrn_element_t* const row = zrn_lookup(rows[i]);
			if (row && row != e) clear_one(row);
		}
	}
	clear_one(e);
}


/* ============================================================================
 *  tilt
 * ============================================================================ */

static void tilt_aim(zrn_root_t* const r, const float x, const float y) {
	zrn_element_t* const e = r->tilt_element;
	if (!e || e->rect.width <= 0 || e->rect.height <= 0) return;
	ZPoint c = zrn_to_root(e, (ZPoint){ e->rect.width * 0.5f, e->rect.height * 0.5f });
	r->tilt_fx = zrn_clampf((x - c.x) / (e->rect.width * 0.5f), -1, 1);
	r->tilt_fy = zrn_clampf((y - c.y) / (e->rect.height * 0.5f), -1, 1);
}

void zrn_tilt_press(zrn_root_t* const r, zrn_element_t* const e, const float x, const float y) {
	if (!zrn_animations_enabled(r)) return;
	if (r->tilt_element && r->tilt_element != e) {
		r->tilt_element->rot_x = r->tilt_element->rot_y = 0;
		r->tilt_element->render_scale = 1;
		zrn_update_transformed(r->tilt_element);
	}
	/* the lean owns these while it lasts */
	if (e->tweens) {
		e->tweens->p[Z_PROP_ROTATION_X].active = e->tweens->p[Z_PROP_ROTATION_Y].active = e->tweens->p[Z_PROP_SCALE].active = 0;
	}
	r->tilt_element = e;
	r->tilt_amount = 0;
	r->tilt_target = 1;
	e->proj_cx = e->proj_cy = 0.5f;
	tilt_aim(r, x, y);
}

void zrn_tilt_move(zrn_root_t* const r, const float x, const float y) {
	if (r->tilt_element && r->tilt_target > 0) tilt_aim(r, x, y);
}

void zrn_tilt_release(zrn_root_t* const r) {
	if (r->tilt_element) r->tilt_target = 0;
}

void zrn_tilt_tick(zrn_root_t* const r, const float dt) {
	zrn_element_t* const e = r->tilt_element;
	float eased, cap_y, cap_x, deg_y, deg_x;
	if (!e) return;
	if (r->tilt_target > r->tilt_amount) r->tilt_amount = zrn_minf(1, r->tilt_amount + dt / TILT_IN_MS);
	else if (r->tilt_target < r->tilt_amount) r->tilt_amount = zrn_maxf(0, r->tilt_amount - dt / TILT_OUT_MS);
	eased = zrn_ease(Z_EASE_OUT, r->tilt_amount);
	/* its far end never rises more than TILT_LIFT points */
	cap_y = e->rect.width > 0 ? asinf(zrn_minf(1, TILT_LIFT / (e->rect.width * 0.5f))) * 180.0f / ZST_PI : TILT_DEG;
	cap_x = e->rect.height > 0 ? asinf(zrn_minf(1, TILT_LIFT / (e->rect.height * 0.5f))) * 180.0f / ZST_PI : TILT_DEG;
	deg_y = r->tilt_fx * zrn_minf(TILT_DEG, cap_y) * eased;
	deg_x = r->tilt_fy * zrn_minf(TILT_DEG, cap_x) * eased;
	e->rot_y = deg_y;
	e->rot_x = deg_x;
	e->render_scale = 1 - PRESS_SINK * eased;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
	if (r->tilt_amount <= 0 && r->tilt_target <= 0) {
		e->rot_x = e->rot_y = 0;
		e->render_scale = 1;
		zrn_update_transformed(e);
		r->tilt_element = nullptr;
	}
}

void Zroot_set_animations(const Zid root, const ZAnimations mode) {
	zrn_root_t* const r = zrn_root_lookup(root);
	if (r) r->animations = mode;
}

ZAnimations Zroot_get_animations(const Zid root) {
	const zrn_root_t* const r = zrn_root_lookup(root);
	return r ? r->animations : Z_ANIMATIONS_AUTO;
}

int Zroot_animations_enabled(const Zid root) { return zrn_animations_enabled(zrn_root_lookup(root)); }
