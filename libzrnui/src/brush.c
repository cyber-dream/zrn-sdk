// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Brushes, as XAML has them: one thing that fills an area, whatever it is filled with. A solid
 * colour, a linear or a radial gradient, a picture. Any node can have one as its background
 * (zui_set_background); the page's, a ContentPanel's, a hub section's.
 *
 * A gradient is baked once into a small picture on the unit square (as XAML's default
 * RelativeToBoundingBox maps it) and drawn as a picture stretched over the area: it is smooth, so a
 * small one stretched looks right, and it goes through the same path as any picture: rounded
 * corners, opacity, and DotMatrix turning it into dots by its brightness.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zui-internal.h"

#define GRAD_N 128		/* the baked gradient's side */
#define MAX_STOPS 16

enum { BRUSH_SOLID, BRUSH_LINEAR, BRUSH_RADIAL, BRUSH_IMAGE, BRUSH_SHADER };

#define MAX_UNIFORMS 8
struct uniform { char name[48]; float v[4]; };

struct stop { float at, a; uint32_t rgb; };

struct zui_brush {
	int refs, kind, stretch;
	uint32_t rgb;			/* solid */
	float opacity, ax, ay;		/* ax, ay: where a picture that does not fill sits, 0..1 */
	float x0, y0, x1, y1;		/* linear: from, to; radial: x0, y0 the centre, x1, y1 the radii (unit square) */
	struct stop stop[MAX_STOPS];
	int nstops;
	bool dirty;			/* a gradient to bake again */
	zui_pixmap *pix;		/* the picture, or the baked gradient */
	/* a shader: the app's source and what GL made of it, for this context */
	char *src, error[1024];
	unsigned prog, gen;
	bool failed, animated;
	uint32_t started;
	struct uniform uniform[MAX_UNIFORMS];
	int nuniforms;
};

static zui_brush *brush_new(int kind) {
	zui_brush *b = calloc(1, sizeof(*b));
	b->refs = 1;
	b->kind = kind;
	b->opacity = 1;
	b->stretch = ZUI_STRETCH_FILL;	/* as in XAML */
	b->ax = b->ay = 0.5f;
	return b;
}

ZUI_EXPORT zui_brush *zui_brush_solid(uint32_t rgb) {
	zui_brush *b = brush_new(BRUSH_SOLID);
	b->rgb = rgb & 0xffffff;
	return b;
}

/* a gradient along the line from (x0, y0) to (x1, y1), in 0..1 of the area it paints */
ZUI_EXPORT zui_brush *zui_brush_linear(double x0, double y0, double x1, double y1) {
	zui_brush *b = brush_new(BRUSH_LINEAR);
	b->x0 = (float)x0;
	b->y0 = (float)y0;
	b->x1 = (float)x1;
	b->y1 = (float)y1;
	b->dirty = true;
	return b;
}

/* a gradient from the centre (cx, cy) out to the radii (rx, ry), in 0..1 of the area it paints */
ZUI_EXPORT zui_brush *zui_brush_radial(double cx, double cy, double rx, double ry) {
	zui_brush *b = brush_new(BRUSH_RADIAL);
	b->x0 = (float)cx;
	b->y0 = (float)cy;
	b->x1 = (float)(rx > 0 ? rx : 0.5);
	b->y1 = (float)(ry > 0 ? ry : 0.5);
	b->dirty = true;
	return b;
}

ZUI_EXPORT void zui_brush_stop(zui_brush *b, double offset, uint32_t rgb, double alpha) {
	if (!b || b->nstops >= MAX_STOPS || (b->kind != BRUSH_LINEAR && b->kind != BRUSH_RADIAL)) return;
	struct stop *s = &b->stop[b->nstops++];
	s->at = offset < 0 ? 0 : offset > 1 ? 1 : (float)offset;
	s->rgb = rgb & 0xffffff;
	s->a = alpha < 0 ? 0 : alpha > 1 ? 1 : (float)alpha;
	b->dirty = true;
}

ZUI_EXPORT zui_brush *zui_brush_image(zui_pixmap *pix) {
	zui_brush *b = brush_new(BRUSH_IMAGE);
	if (pix) pix->refs++;
	b->pix = pix;
	return b;
}

ZUI_EXPORT void zui_brush_set_stretch(zui_brush *b, int stretch) { if (b) b->stretch = stretch; }
ZUI_EXPORT void zui_brush_set_opacity(zui_brush *b, double opacity) {
	if (b) b->opacity = opacity < 0 ? 0 : opacity > 1 ? 1 : (float)opacity;
}
ZUI_EXPORT void zui_brush_set_align(zui_brush *b, double ax, double ay) {
	if (!b) return;
	b->ax = ax < 0 ? 0 : ax > 1 ? 1 : (float)ax;
	b->ay = ay < 0 ? 0 : ay > 1 ? 1 : (float)ay;
}

/* A brush that paints with an OpenGL ES 1.00 fragment shader the app writes: it defines
 * `vec4 zui_shade(vec2 uv)` (uv 0..1 over the area, y down), and may read u_time (seconds since the first
 * frame it was painted), u_res (the area's size in points) and the uniforms it declares and
 * zui_brush_set_uniform gives. Compile errors are in zui_brush_error, and nothing is painted. */
ZUI_EXPORT zui_brush *zui_brush_shader(const char *source) {
	zui_brush *b = brush_new(BRUSH_SHADER);
	b->src = strdup(source ? source : "");
	b->animated = true;
	return b;
}

ZUI_EXPORT void zui_brush_set_uniform(zui_brush *b, const char *name, double x, double y, double z, double w) {
	if (!b || b->kind != BRUSH_SHADER || !name) return;
	struct uniform *u = NULL;
	for (int i = 0; i < b->nuniforms; i++)
		if (strcmp(b->uniform[i].name, name) == 0) u = &b->uniform[i];
	if (!u && b->nuniforms < MAX_UNIFORMS) {
		u = &b->uniform[b->nuniforms++];
		snprintf(u->name, sizeof(u->name), "%s", name);
	}
	if (!u) return;
	u->v[0] = (float)x;
	u->v[1] = (float)y;
	u->v[2] = (float)z;
	u->v[3] = (float)w;
}

/* a shader that does not use u_time need not have a frame after frame: false stops them */
ZUI_EXPORT void zui_brush_shader_set_animated(zui_brush *b, bool animated) { if (b) b->animated = animated; }

ZUI_EXPORT const char *zui_brush_error(const zui_brush *b) { return b && b->failed ? b->error : NULL; }

ZUI_EXPORT void zui_brush_ref(zui_brush *b) { if (b) b->refs++; }

ZUI_EXPORT void zui_brush_free(zui_brush *b) {
	if (!b || --b->refs > 0) return;
	zui_pixmap_free(b->pix);
	if (b->prog && b->gen == zg_main_generation()) zg_shader_free(b->prog);
	free(b->src);
	free(b);
}

/* the colour of the gradient at t: between its two stops, premultiplied, as the drawing wants it */
static void stop_colour(const zui_brush *b, float t, uint8_t out[4]) {
	if (b->nstops == 0) {
		memset(out, 0, 4);
		return;
	}
	const struct stop *lo = &b->stop[0], *hi = &b->stop[b->nstops - 1];
	for (int i = 0; i < b->nstops; i++) {	/* the stops need not be given in order */
		const struct stop *s = &b->stop[i];
		if (s->at <= t && s->at >= lo->at) lo = s;
	}
	hi = NULL;
	for (int i = 0; i < b->nstops; i++) {
		const struct stop *s = &b->stop[i];
		if (s->at >= t && (!hi || s->at < hi->at)) hi = s;
	}
	if (!hi) hi = lo;
	if (lo->at > t) lo = hi;
	float k = hi->at > lo->at ? (t - lo->at) / (hi->at - lo->at) : 0;
	k = k < 0 ? 0 : k > 1 ? 1 : k;
	float a = lo->a + (hi->a - lo->a) * k;
	for (int c = 0; c < 3; c++) {
		float f = (float)((lo->rgb >> (16 - 8 * c)) & 0xff) * (1 - k) + (float)((hi->rgb >> (16 - 8 * c)) & 0xff) * k;
		out[c] = (uint8_t)lroundf(f * a);
	}
	out[3] = (uint8_t)lroundf(a * 255);
}

static void bake(zui_brush *b) {
	uint8_t *px = malloc((size_t)GRAD_N * GRAD_N * 4);
	float dx = b->x1 - b->x0, dy = b->y1 - b->y0, len2 = dx * dx + dy * dy;
	for (int y = 0; y < GRAD_N; y++)
		for (int x = 0; x < GRAD_N; x++) {
			float u = ((float)x + 0.5f) / GRAD_N, v = ((float)y + 0.5f) / GRAD_N, t;
			if (b->kind == BRUSH_LINEAR) t = len2 > 0 ? ((u - b->x0) * dx + (v - b->y0) * dy) / len2 : 0;
			else t = hypotf((u - b->x0) / b->x1, (v - b->y0) / b->y1);
			t = t < 0 ? 0 : t > 1 ? 1 : t;
			stop_colour(b, t, px + ((size_t)y * GRAD_N + x) * 4);
		}
	zui_pixmap *old = b->pix;
	b->pix = zui_pixmap_rgba(GRAD_N, GRAD_N, px);
	free(px);
	zui_pixmap_free(old);
	b->dirty = false;
}

/* Paint the brush over x, y, w, h (corners round by r). A picture is placed as XAML's Stretch says:
 * None at its own size (cut at the area's edge), Fill over all of it, Uniform whole inside it,
 * UniformToFill covering all of it (cut). Where one that does not fill sits is the alignment's;
 * a cut one is cut evenly from both sides. */
void zui_brush_paint(zui_app *app, zui_brush *b, float x, float y, float w, float h, float r) {
	if (!b || w <= 0 || h <= 0 || b->opacity <= 0) return;
	float old = zg_get_alpha();
	zg_set_alpha(old * b->opacity);
	if (b->kind == BRUSH_SOLID) {
		zg_rect(x, y, w, h, r, zc_rgb(b->rgb));
	} else if (b->kind == BRUSH_SHADER) {
		if ((!b->prog || b->gen != zg_main_generation()) && !b->failed) {
			b->prog = zg_shader_compile(b->src, b->error, sizeof(b->error));
			b->gen = zg_main_generation();
			if (!b->prog) {
				b->failed = true;
				fprintf(stderr, "zrnui: a shader brush did not compile: %s\n", b->error);
			}
		}
		if (b->prog) {
			uint32_t now = zui_now();
			if (!b->started) b->started = now ? now : 1;
			const char *names[MAX_UNIFORMS];
			float values[MAX_UNIFORMS][4];
			for (int i = 0; i < b->nuniforms; i++) {
				names[i] = b->uniform[i].name;
				memcpy(values[i], b->uniform[i].v, sizeof(values[i]));
			}
			zg_shader_rect(b->prog, x, y, w, h, r, (float)(int32_t)(now - b->started) / 1000.0f, b->nuniforms, names, (const float (*)[4])values);
			if (b->animated) app->dirty = true;
		}
	} else {
		if ((b->kind == BRUSH_LINEAR || b->kind == BRUSH_RADIAL) && (b->dirty || !b->pix)) bake(b);
		if (b->pix && zui_pixmap_ready(app, b->pix, false)) {
			float iw = (float)b->pix->w, ih = (float)b->pix->h;
			float dw = w, dh = h, cx = 1, cy = 1;	/* what is drawn, and how much of the picture */
			int stretch = b->kind == BRUSH_IMAGE ? b->stretch : ZUI_STRETCH_FILL;
			if (stretch == ZUI_STRETCH_NONE) {
				if (iw < w) dw = iw; else cx = w / iw;
				if (ih < h) dh = ih; else cy = h / ih;
			} else if (stretch == ZUI_STRETCH_UNIFORM) {
				float k = fminf(w / iw, h / ih);
				dw = iw * k;
				dh = ih * k;
			} else if (stretch == ZUI_STRETCH_UNIFORM_TO_FILL) {
				float sa = w / h, ia = iw / ih;
				if (ia > sa) cx = sa / ia;
				else cy = ia / sa;
			}
			zg_image(b->pix->tex, x + (w - dw) * b->ax, y + (h - dh) * b->ay, dw, dh, dw == w && dh == h ? r : 0, cx, cy);
		}
	}
	zg_set_alpha(old);
}

/* ---- a node's background ---- */
/* A pivot made from the app is a root, and what is set on a root is set on all of it: the page under it
 * is what the screen shows, titles and all (zui_page_of, as everywhere a page is meant). A hub is the
 * whole screen itself. */
ZUI_EXPORT void zui_set_background(zui_node *n, zui_brush *brush) {
	if (n->kind == ZK_PIVOT) n = zui_page_of(n);
	if (brush) brush->refs++;
	zui_brush_free(n->bg);
	n->bg = brush;
	n->app->dirty = true;
}
