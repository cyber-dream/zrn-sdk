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
	const float x1 = 0.2f, y1 = 0.8f, x2 = 0.2f, y2 = 1.0f;
	float s = t, u;
	int i;
	for (i = 0; i < 8; i++) {
		float dx;
		u = 1 - s;
		dx = 3 * u * u * x1 + 6 * u * s * (x2 - x1) + 3 * s * s * (1 - x2);
		if (fabsf(dx) < 1e-5f) break;
		s -= (3 * u * u * s * x1 + 3 * u * s * s * x2 + s * s * s - t) / dx;
		s = zrn_clampf(s, 0, 1);
	}
	u = 1 - s;
	return 3 * u * u * s * y1 + 3 * u * s * s * y2 + s * s * s;
}

float zrn_ease(ZEasing easing, float t) {
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

static void xf_scale(float out[16], const float s) {
	float r[16];
	zrn_xf_identity(r);
	r[0] = r[5] = r[10] = s;
	zrn_xf_mul(out, out, r);
}

void zrn_xf_of_element(const zrn_element_t* const e, const float ox, const float oy, const float width, const float height, const float scale, float out[16]) {
	const float cx = ox + e->proj_cx * width, cy = oy + e->proj_cy * height;
	zrn_xf_identity(out);
	/* applied last to first: the centre to the origin, the rotations, the scale, back, and the slide */
	xf_translate(out, cx + e->translate_x * scale, cy + e->translate_y * scale, 0);
	if (e->render_scale != 1.0f) xf_scale(out, e->render_scale);
	if (e->rot_x != 0.0f) xf_rotate_x(out, e->rot_x);
	if (e->rot_y != 0.0f) xf_rotate_y(out, e->rot_y);
	xf_translate(out, -cx, -cy, 0);
}

void zrn_update_transformed(zrn_element_t* const e) {
	e->transformed = e->rot_x != 0.0f || e->rot_y != 0.0f || e->render_scale != 1.0f || e->translate_x != 0.0f || e->translate_y != 0.0f;
}

void Zelement_set_projection(const Zid id, const float rotation_x, const float rotation_y, const float center_x, const float center_y) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->rot_x = rotation_x;
	e->rot_y = rotation_y;
	e->proj_cx = center_x;
	e->proj_cy = center_y;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_render_scale(const Zid id, const float scale) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->render_scale = scale;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_translate(const Zid id, const float x, const float y) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->translate_x = x;
	e->translate_y = y;
	zrn_update_transformed(e);
	zrn_changed_paint(e);
}

void Zelement_set_tilt(const Zid id, const int enabled) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
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

float Zelement_get_property(const Zid id, const ZAnimatedProperty property) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	const float* const slot = e ? property_slot(e, property) : nullptr;
	return slot ? *slot : 0.0f;
}

int zrn_animations_enabled(const zrn_root_t* const r) {
	if (!r) return 0;
	if (r->animations == Z_ANIMATIONS_ON) return 1;
	if (r->animations == Z_ANIMATIONS_OFF) return 0;
	return r->renderer == Z_RENDERER_GPU || r->gpu_window;
}

void Zelement_animate(const Zid id, const ZAnimatedProperty property, const float to, const float duration_ms, const ZEasing easing, const float delay_ms) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	float* slot;
	if (!e || property < 0 || property >= Z_PROP_LAST_) return;
	slot = property_slot(e, property);
	if (e->tweens) e->tweens->p[property].active = 0;
	if (!e->root || !zrn_animations_enabled(e->root) || (duration_ms <= 0 && delay_ms <= 0)) {
		property_set(e, property, to);
		return;
	}
	if (!e->tweens) {
		e->tweens = (zrn_tween_t*)calloc(1, sizeof *e->tweens);
		if (!e->tweens) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
	}
	e->tweens->p[property].active = 1;
	e->tweens->p[property].from = *slot;
	e->tweens->p[property].to = to;
	e->tweens->p[property].start = (float)e->root->time_ms;
	e->tweens->p[property].duration = duration_ms;
	e->tweens->p[property].delay = delay_ms;
	e->tweens->p[property].easing = easing;
	zrn_changed_paint(e);
}

int Zelement_is_animating(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	int i;
	if (!e || !e->tweens) return false;
	for (i = 0; i < Z_PROP_LAST_; i++) if (e->tweens->p[i].active) return true;
	return false;
}

void zrn_tweens_tick(zrn_element_t* const e) {
	int i, any = 0;
	float now;
	if (!e->tweens) return;
	now = e->root ? (float)e->root->time_ms : 1e30f;
	for (i = 0; i < Z_PROP_LAST_; i++) {
		zrn_tween_t* const t = e->tweens;
		float u;
		if (!t->p[i].active) continue;
		u = now - t->p[i].start - t->p[i].delay;
		if (u < 0) { any = 1; continue; }
		if (t->p[i].duration <= 0 || u >= t->p[i].duration) {
			t->p[i].active = 0;
			property_set(e, (ZAnimatedProperty)i, t->p[i].to);
		} else {
			property_set(e, (ZAnimatedProperty)i, t->p[i].from + (t->p[i].to - t->p[i].from) * zrn_ease(t->p[i].easing, u / t->p[i].duration));
			any = 1;
		}
	}
	if (!any) zrn_tweens_free(e);
}

void zrn_tweens_free(zrn_element_t* const e) {
	free(e->tweens);
	e->tweens = nullptr;
}


/* ============================================================================
 *  tilt
 * ============================================================================ */

static void tilt_aim(zrn_root_t* const r, const float x, const float y) {
	zrn_element_t* const e = r->tilt_element;
	ZPoint c;
	if (!e || e->rect.width <= 0 || e->rect.height <= 0) return;
	c = zrn_to_root(e, (ZPoint){ e->rect.width * 0.5f, e->rect.height * 0.5f });
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

int Zroot_animations_enabled(Zid root) { return zrn_animations_enabled(zrn_root_lookup(root)); }
