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

/* the ExponentialEase of Silverlight: (e^(n t) - 1) / (e^n - 1) going in, its mirror going out */
static float ease_exp_in(const float t, const float n) { return (expf(n * t) - 1.0f) / (expf(n) - 1.0f); }
static float ease_exp_out(const float t, const float n) { return 1.0f - ease_exp_in(1.0f - t, n); }

float zrn_ease(const ZEasing easing, float t) {
	t = zrn_clampf(t, 0, 1);
	switch (easing) {
	case Z_EASE_SINE_OUT: return sinf(t * ZST_PI * 0.5f);
	case Z_EASE_EXP1_IN: return ease_exp_in(t, 1);
	case Z_EASE_EXP1_OUT: return ease_exp_out(t, 1);
	case Z_EASE_EXP6_IN: return ease_exp_in(t, 6);
	case Z_EASE_EXP6_OUT: return ease_exp_out(t, 6);
	case Z_EASE_EXP15_IN: return ease_exp_in(t, 15);
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

/* Starts the tween of one property through `nleg` legs, one after another. The value it moves from is the one it has when the delay is over. */
static void tween_start_legs(zrn_element_t* const e, const ZAnimatedProperty property, const zrn_leg_t* const legs, const int nleg, const float delay_ms) {
	float total = 0;
	for (int i = 0; i < nleg; i++) total += legs[i].duration;
	if (e->tweens) e->tweens->p[property].active = 0;
	if (!e->root || !zrn_animations_enabled(e->root) || (total <= 0 && delay_ms <= 0)) {
		property_set(e, property, legs[nleg - 1].to);
		return;
	}
	if (!e->tweens) {
		e->tweens = (zrn_tween_t*)calloc(1, sizeof *e->tweens);
		if (!e->tweens) {
			/* the value still gets where it was going */
			property_set(e, property, legs[nleg - 1].to);
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
	}
	e->tweens->p[property].active = 1;
	e->tweens->p[property].started = 0;
	e->tweens->p[property].from = 0;
	e->tweens->p[property].delay = delay_ms;
	e->tweens->p[property].start = e->root->time_ms;
	e->tweens->p[property].count = nleg;
	for (int i = 0; i < nleg; i++) e->tweens->p[property].legs[i] = legs[i];
	zrn_changed_paint(e);
}

static void tween_start(zrn_element_t* const e, const ZAnimatedProperty property, const float to, const float duration_ms, const ZEasing easing, const float delay_ms) {
	const zrn_leg_t leg = { .to = to, .duration = duration_ms, .easing = easing };
	tween_start_legs(e, property, &leg, 1, delay_ms);
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
		double total = 0, acc = 0;
		float v0, v;
		int leg;
		if (u < 0) { any = 1; continue; }
		if (!t->p[i].started) {
			const float* const slot = property_slot(e, (ZAnimatedProperty)i);
			t->p[i].started = 1;
			t->p[i].from = slot ? *slot : 0.0f;
		}
		for (leg = 0; leg < t->p[i].count; leg++) total += (double)t->p[i].legs[leg].duration;
		if (u >= total) {
			t->p[i].active = 0;
			property_set(e, (ZAnimatedProperty)i, t->p[i].legs[t->p[i].count - 1].to);
			continue;
		}
		/* the leg that holds the time now, from the end of the one before */
		v0 = t->p[i].from;
		for (leg = 0; leg < t->p[i].count - 1 && u >= acc + (double)t->p[i].legs[leg].duration; leg++) {
			acc += (double)t->p[i].legs[leg].duration;
			v0 = t->p[i].legs[leg].to;
		}
		v = t->p[i].legs[leg].duration > 0
			? v0 + (t->p[i].legs[leg].to - v0) * zrn_ease(t->p[i].legs[leg].easing, (float)((u - acc) / (double)t->p[i].legs[leg].duration))
			: t->p[i].legs[leg].to;
		property_set(e, (ZAnimatedProperty)i, v);
		any = 1;
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
 *  Every kind is a few properties moving at once, each through keyframes: the first is where it
 *  starts, each next one is reached at its time with the easing it has (as in the XAML of the
 *  Toolkit, where the easing is on the frame that ends the leg). Opacity is in fractions of what
 *  the element has (so a dimmed element fades back to its own opacity). The element shows the
 *  start at once, then the tweens run after the delay.
 *
 *  The numbers are the Toolkit's: Microsoft.Phone.Controls.Toolkit/Transitions/Storyboards/ (the .xaml of each kind)
 *  and Effects/TurnstileFeatherEffect.cs (microsoftarchive/WindowsPhoneToolkit), with every angle
 *  negated: its RotationX, RotationY and RotationZ turn the other way round than ours (its
 *  ForwardOut turns a page +50 about Y, which is the -50 of the turnstile of the Lumia here).
 *  Only the Fade is not the Toolkit's.
 * ============================================================================ */

typedef struct { float t, value; ZEasing easing; } tkey_t;

typedef struct {
	ZAnimatedProperty prop;
	int count;
	tkey_t key[3];
} tseg_t;

typedef struct {
	/** The centre of the rotations, fractions of the size; cx < -1: it stays as it is. */
	float cx, cy;
	int count;
	tseg_t seg[3];
	/** Feathered kinds: the delay between one row and the next, in ms. */
	float stagger;
} tdef_t;

#define L Z_EASE_LINEAR
#define K(t, v, e) { (t), (v), (e) }
/* one move from `a` to `b` in `ms` with easing `e` */
#define MV(p, a, b, ms, e) { (p), 2, { K(0, a, L), K(ms, b, e) } }
/* the opacity: there at once / held and gone at the end / the same through the time of the move */
#define OP_ON { Z_PROP_OPACITY, 2, { K(0, 0, L), K(10, 1, L) } }
#define OP_ON_AT_ONCE { Z_PROP_OPACITY, 2, { K(0, 0, L), K(1, 1, L) } }
#define OP_OFF_AT_END(ms) { Z_PROP_OPACITY, 3, { K(0, 1, L), K((ms) - 10, 1, L), K(ms, 0, L) } }
#define OP_FADE(a, b, ms, e) MV(Z_PROP_OPACITY, a, b, ms, e)
#define NOP -2.0f, -2.0f

static constexpr float SLIDE_DIST = 200.0f;
static constexpr int ROW_MAX = 64;

static const tdef_t TDEFS[Z_TRANSITION_LAST_] = {
	/* not the Toolkit's */
	[Z_TRANSITION_FADE_IN] = { NOP, 1, { OP_FADE(0, 1, 350, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_FADE_OUT] = { NOP, 1, { OP_FADE(1, 0, 250, Z_EASE_EXP6_OUT) } },

	[Z_TRANSITION_SLIDE_UP_FADE_IN] = { NOP, 2, { MV(Z_PROP_TRANSLATE_Y, SLIDE_DIST, 0, 350, Z_EASE_EXP6_OUT), OP_FADE(0, 1, 350, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_UP_FADE_OUT] = { NOP, 2, { MV(Z_PROP_TRANSLATE_Y, 0, -SLIDE_DIST, 250, Z_EASE_EXP6_OUT), OP_FADE(1, 0, 250, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_DOWN_FADE_IN] = { NOP, 2, { MV(Z_PROP_TRANSLATE_Y, -SLIDE_DIST, 0, 350, Z_EASE_EXP6_OUT), OP_FADE(0, 1, 350, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_DOWN_FADE_OUT] = { NOP, 2, { MV(Z_PROP_TRANSLATE_Y, 0, SLIDE_DIST, 250, Z_EASE_EXP6_OUT), OP_FADE(1, 0, 250, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_LEFT_FADE_IN] = { NOP, 2, { MV(Z_PROP_TRANSLATE_X, SLIDE_DIST, 0, 500, Z_EASE_EXP6_OUT), OP_FADE(0, 1, 500, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_LEFT_FADE_OUT] = { NOP, 2, { MV(Z_PROP_TRANSLATE_X, 0, -SLIDE_DIST, 300, Z_EASE_EXP6_OUT), OP_FADE(1, 0, 300, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_RIGHT_FADE_IN] = { NOP, 2, { MV(Z_PROP_TRANSLATE_X, -SLIDE_DIST, 0, 500, Z_EASE_EXP6_OUT), OP_FADE(0, 1, 500, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SLIDE_RIGHT_FADE_OUT] = { NOP, 2, { MV(Z_PROP_TRANSLATE_X, 0, SLIDE_DIST, 300, Z_EASE_EXP6_OUT), OP_FADE(1, 0, 300, Z_EASE_EXP6_OUT) } },

	/* a swivel turns about the middle; the full screen and forward ones in keep the opacity */
	[Z_TRANSITION_SWIVEL_FULL_SCREEN_IN] = { 0.5f, 0.5f, 1, { MV(Z_PROP_ROTATION_X, 30, 0, 350, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SWIVEL_FULL_SCREEN_OUT] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_X, 0, -30, 250, Z_EASE_EXP1_IN), OP_OFF_AT_END(250) } },
	[Z_TRANSITION_SWIVEL_FORWARD_IN] = { 0.5f, 0.5f, 1, { MV(Z_PROP_ROTATION_X, 45, 0, 350, Z_EASE_EXP6_OUT) } },
	[Z_TRANSITION_SWIVEL_FORWARD_OUT] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_X, 0, -90, 250, Z_EASE_EXP15_IN), OP_OFF_AT_END(250) } },
	[Z_TRANSITION_SWIVEL_BACKWARD_IN] = { 0.5f, 0.5f, 2, {
		{ Z_PROP_ROTATION_X, 3, { K(0, 45, L), K(200, 30, L), K(350, 0, Z_EASE_EXP6_OUT) } },
		{ Z_PROP_OPACITY, 3, { K(0, 0, L), K(200, 0, L), K(210, 1, L) } } } },
	[Z_TRANSITION_SWIVEL_BACKWARD_OUT] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_X, 0, -60, 250, Z_EASE_EXP6_IN), OP_OFF_AT_END(250) } },

	/* a turnstile turns about the left edge; it is there as soon as it starts, and gone when it has turned */
	[Z_TRANSITION_TURNSTILE_FORWARD_IN] = { 0, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 80, 0, 350, Z_EASE_EXP6_OUT), OP_ON } },
	[Z_TRANSITION_TURNSTILE_FORWARD_OUT] = { 0, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 0, -50, 250, Z_EASE_EXP6_IN), OP_OFF_AT_END(250) } },
	[Z_TRANSITION_TURNSTILE_BACKWARD_IN] = { 0, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, -50, 0, 350, Z_EASE_EXP6_OUT), OP_ON } },
	[Z_TRANSITION_TURNSTILE_BACKWARD_OUT] = { 0, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 0, 80, 250, Z_EASE_EXP6_IN), OP_OFF_AT_END(250) } },

	/* TurnstileFeatherEffect: every row turns about a point a fifth of its width to the left of it, the rows 40 or 50 ms apart */
	[Z_TRANSITION_FEATHER_FORWARD_IN] = { -0.2f, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 80, 0, 350, Z_EASE_EXP6_OUT), OP_ON_AT_ONCE }, 40.0f },
	[Z_TRANSITION_FEATHER_FORWARD_OUT] = { -0.2f, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 0, -50, 250, Z_EASE_EXP6_IN), OP_OFF_AT_END(250) }, 50.0f },
	[Z_TRANSITION_FEATHER_BACKWARD_IN] = { -0.2f, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, -50, 0, 350, Z_EASE_EXP6_OUT), OP_ON_AT_ONCE }, 50.0f },
	[Z_TRANSITION_FEATHER_BACKWARD_OUT] = { -0.2f, 0.5f, 2, { MV(Z_PROP_ROTATION_Y, 0, 80, 250, Z_EASE_EXP6_IN), OP_OFF_AT_END(250) }, 40.0f },

	[Z_TRANSITION_ROTATE_IN_90_CLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, -90, 0, 250, Z_EASE_EXP1_OUT), OP_FADE(0, 1, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_IN_90_COUNTERCLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 90, 0, 250, Z_EASE_EXP1_OUT), OP_FADE(0, 1, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_IN_180_CLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, -180, 0, 250, Z_EASE_EXP1_OUT), OP_FADE(0, 1, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_IN_180_COUNTERCLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 180, 0, 250, Z_EASE_EXP1_OUT), OP_FADE(0, 1, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_OUT_90_CLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 0, 90, 250, Z_EASE_EXP1_IN), OP_FADE(1, 0, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_OUT_90_COUNTERCLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 0, -90, 250, Z_EASE_EXP1_IN), OP_FADE(1, 0, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_OUT_180_CLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 0, 180, 250, Z_EASE_EXP1_IN), OP_FADE(1, 0, 250, Z_EASE_SINE_OUT) } },
	[Z_TRANSITION_ROTATE_OUT_180_COUNTERCLOCKWISE] = { 0.5f, 0.5f, 2, { MV(Z_PROP_ROTATION_Z, 0, -180, 250, Z_EASE_EXP1_IN), OP_FADE(1, 0, 250, Z_EASE_SINE_OUT) } },

	/* a quarter turn in 600 ms: the first half quick with a long settle, the second even; the opacity is not touched */
	[Z_TRANSITION_ROLL] = { 0.5f, 0.5f, 1, { { Z_PROP_ROTATION_Z, 3, { K(0, 0, L), K(300, -45, Z_EASE_EXP6_OUT), K(600, -90, L) } } } },
};

#undef L
#undef K
#undef MV
#undef OP_ON
#undef OP_ON_AT_ONCE
#undef OP_OFF_AT_END
#undef OP_FADE
#undef NOP

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
static float transition_run(zrn_element_t* const e, const ZTransition t, const float delay_ms) {
	const tdef_t* const d = &TDEFS[t];
	const float base = base_opacity(e);
	const int animate = e->root && zrn_animations_enabled(e->root);
	float end = 0;
	if (d->cx >= -1.0f) {
		e->proj_cx = d->cx;
		e->proj_cy = d->cy;
	}
	for (int i = 0; i < d->count; i++) {
		const tseg_t* const sg = &d->seg[i];
		const float k = sg->prop == Z_PROP_OPACITY ? base : 1.0f;
		zrn_leg_t legs[2];
		if (!animate) {
			if (e->tweens) e->tweens->p[sg->prop].active = 0;
			property_set(e, sg->prop, sg->key[sg->count - 1].value * k);
			continue;
		}
		for (int j = 1; j < sg->count; j++) {
			legs[j - 1].to = sg->key[j].value * k;
			legs[j - 1].duration = sg->key[j].t - sg->key[j - 1].t;
			legs[j - 1].easing = sg->key[j].easing;
		}
		property_set(e, sg->prop, sg->key[0].value * k);
		tween_start_legs(e, sg->prop, legs, sg->count - 1, delay_ms);
		if (delay_ms + sg->key[sg->count - 1].t > end) end = delay_ms + sg->key[sg->count - 1].t;
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

/* The rows go one after another in the order they are in, going in or out, forward or back. */
static void transition_feather(zrn_element_t* const e, const ZTransition t, const float delay_ms) {
	Zid rows[ROW_MAX];
	if (e->root && e->root->layout_dirty) Zroot_update_layout(e->root->id);
	const int n = collect_rows(e, rows, 0);
	e->transition_rows = 1;
	for (int i = 0; i < n; i++) {
		zrn_element_t* const row = zrn_lookup(rows[i]);
		if (!row) continue;
		row->transition_active = 1;
		if (transition_run(row, t, delay_ms + (float)i * TDEFS[t].stagger) <= 0) transition_ended(row);
	}
}

static void transition_play(zrn_element_t* const e, const ZTransition t, const float delay_ms) {
	if (transition_is_feather(t)) {
		transition_feather(e, t, delay_ms);
		return;
	}
	e->transition_active = 1;
	if (transition_run(e, t, delay_ms) <= 0) transition_ended(e);
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
	if (!transition_valid(t) || transition_is_feather(t)) return 0;
	for (int i = 0; i < TDEFS[t].count; i++) {
		const tseg_t* const sg = &TDEFS[t].seg[i];
		if (sg->key[sg->count - 1].t > end) end = sg->key[sg->count - 1].t;
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
