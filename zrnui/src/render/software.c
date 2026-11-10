/**
 * zrnui - render/software.c
 * The software renderer: the painter on an RGBA8 buffer in memory, with
 * coverage-based anti-aliased shapes. It needs no GPU and no display, draws
 * what Zroot_render returns and what the window shows through wl_shm, and is the
 * safe mode when there is no OpenGL.
 *
 * Coordinates given to the backend functions are logical and local to the
 * element being painted (painter->tx, ty is its origin in device pixels).
 */

#include "../internal.h"

static void sw_paint_clear(zrn_painter_t* const p, const ZColor c) {
	int x, y;
	uint8_t px[4];
	px[0] = (uint8_t)(zrn_clampf(c.r, 0, 1) * 255.0f + 0.5f);
	px[1] = (uint8_t)(zrn_clampf(c.g, 0, 1) * 255.0f + 0.5f);
	px[2] = (uint8_t)(zrn_clampf(c.b, 0, 1) * 255.0f + 0.5f);
	px[3] = 255;
	for (y = 0; y < p->height; y++) {
		uint8_t* const row = p->pixels + (size_t)y * (size_t)p->stride;
		for (x = 0; x < p->width; x++) memcpy(row + x * 4, px, 4);
	}
}

/* Blends `c` with coverage `cov` (0..1) onto one pixel. The buffer is always opaque. */
static inline void blend(uint8_t* const d, const ZColor c, const float cov, const float alpha) {
	float a = c.a * alpha * cov;
	if (a <= 0.0f) return;
	if (a > 1.0f) a = 1.0f;
	d[0] = (uint8_t)((float)d[0] + (c.r * 255.0f - (float)d[0]) * a + 0.5f);
	d[1] = (uint8_t)((float)d[1] + (c.g * 255.0f - (float)d[1]) * a + 0.5f);
	d[2] = (uint8_t)((float)d[2] + (c.b * 255.0f - (float)d[2]) * a + 0.5f);
	d[3] = 255;
}

static inline float to_dev_x(const zrn_painter_t* p, const float x) { return p->tx + x * p->scale; }
static inline float to_dev_y(const zrn_painter_t* p, const float y) { return p->ty + y * p->scale; }


/* Fills a device rectangle with fractional edges: partial pixels get partial coverage. */
static void fill_device_rect(const zrn_painter_t* const p, const float x0, const float y0, const float x1, const float y1, const ZColor c) {
	int ix0, iy0, ix1, iy1, x, y;
	if (x1 <= x0 || y1 <= y0) return;
	ix0 = (int)floorf(x0);
	iy0 = (int)floorf(y0);
	ix1 = (int)ceilf(x1);
	iy1 = (int)ceilf(y1);
	if (ix0 < p->clip[0]) ix0 = p->clip[0];
	if (iy0 < p->clip[1]) iy0 = p->clip[1];
	if (ix1 > p->clip[2]) ix1 = p->clip[2];
	if (iy1 > p->clip[3]) iy1 = p->clip[3];
	for (y = iy0; y < iy1; y++) {
		const float cy = zrn_minf((float)y + 1.0f, y1) - zrn_maxf((float)y, y0);
		uint8_t* const row = p->pixels + (size_t)y * (size_t)p->stride;
		for (x = ix0; x < ix1; x++) {
			const float cx = zrn_minf((float)x + 1.0f, x1) - zrn_maxf((float)x, x0);
			blend(row + x * 4, c, cx * cy, p->alpha);
		}
	}
}

static void sw_fill_rect(zrn_painter_t* const p, const ZRect r, const ZColor c) {
	/* rectangle edges land on whole device pixels: crisp like Silverlight's layout rounding */
	float x0 = floorf(to_dev_x(p, r.x) + 0.5f), y0 = floorf(to_dev_y(p, r.y) + 0.5f);
	float x1 = floorf(to_dev_x(p, r.x + r.width) + 0.5f), y1 = floorf(to_dev_y(p, r.y + r.height) + 0.5f);
	fill_device_rect(p, x0, y0, x1, y1, c);
}

/* ---- shapes through a distance function ---- */

typedef float (*coverage_fn)(const void* shape, float x, float y);

static void fill_shape(const zrn_painter_t* const p, const float bx0, const float by0, const float bx1, const float by1, const coverage_fn fn, const void* const shape, const ZColor c) {
	int x0 = (int)floorf(bx0) - 1, y0 = (int)floorf(by0) - 1, x1 = (int)ceilf(bx1) + 1, y1 = (int)ceilf(by1) + 1, x, y;
	if (x0 < p->clip[0]) x0 = p->clip[0];
	if (y0 < p->clip[1]) y0 = p->clip[1];
	if (x1 > p->clip[2]) x1 = p->clip[2];
	if (y1 > p->clip[3]) y1 = p->clip[3];
	for (y = y0; y < y1; y++) {
		uint8_t* const row = p->pixels + (size_t)y * (size_t)p->stride;
		for (x = x0; x < x1; x++) {
			const float cov = fn(shape, (float)x + 0.5f, (float)y + 0.5f);
			if (cov > 0.0f) blend(row + x * 4, c, cov, p->alpha);
		}
	}
}

typedef struct rrect_shape {
	float cx, cy, hw, hh;
	float tl, tr, br, bl;
} rrect_shape_t;

/* Signed distance to a rounded rectangle, negative inside. */
static float rrect_distance(const rrect_shape_t* const s, const float x, const float y) {
	const float px = x - s->cx, py = y - s->cy;
	const float r = px < 0 ? (py < 0 ? s->tl : s->bl) : (py < 0 ? s->tr : s->br);
	const float qx = fabsf(px) - s->hw + r, qy = fabsf(py) - s->hh + r;
	const float ox = zrn_maxf(qx, 0), oy = zrn_maxf(qy, 0);
	return sqrtf(ox * ox + oy * oy) + zrn_minf(zrn_maxf(qx, qy), 0) - r;
}

static float rrect_coverage(const void* const shape, const float x, const float y) {
	return zrn_clampf(0.5f - rrect_distance((const rrect_shape_t*)shape, x, y), 0.0f, 1.0f);
}

static void sw_fill_rrect(zrn_painter_t* const p, const ZRect r, const ZCornerRadius radius, const ZColor c) {
	rrect_shape_t s;
	float x0, y0, x1, y1;
	if (radius.top_left <= 0 && radius.top_right <= 0 && radius.bottom_right <= 0 && radius.bottom_left <= 0) {
		zrn_fill_rect(p, r, c);
		return;
	}
	x0 = to_dev_x(p, r.x);
	y0 = to_dev_y(p, r.y);
	x1 = to_dev_x(p, r.x + r.width);
	y1 = to_dev_y(p, r.y + r.height);
	s.cx = (x0 + x1) * 0.5f;
	s.cy = (y0 + y1) * 0.5f;
	s.hw = (x1 - x0) * 0.5f;
	s.hh = (y1 - y0) * 0.5f;
	s.tl = radius.top_left * p->scale;
	s.tr = radius.top_right * p->scale;
	s.br = radius.bottom_right * p->scale;
	s.bl = radius.bottom_left * p->scale;
	fill_shape(p, x0, y0, x1, y1, rrect_coverage, &s, c);
}

typedef struct ring_shape {
	float cx, cy, rx, ry;
	/** Stroke width in device pixels; 0 fills. */
	float stroke;
} ring_shape_t;

static float ellipse_coverage(const void* const shape, const float x, const float y) {
	const ring_shape_t* s = (const ring_shape_t*)shape;
	float nx = (x - s->cx) / s->rx, ny = (y - s->cy) / s->ry;
	float len = sqrtf(nx * nx + ny * ny);
	const float r = zrn_minf(s->rx, s->ry);
	/* distance in pixels to the outline, approximately */
	const float d = (len - 1.0f) * r;
	if (s->stroke <= 0.0f) return zrn_clampf(0.5f - d, 0.0f, 1.0f);
	return zrn_clampf(0.5f - (fabsf(d + s->stroke * 0.5f) - s->stroke * 0.5f), 0.0f, 1.0f);
}

static void sw_fill_ellipse(zrn_painter_t* const p, const ZRect r, const ZColor c) {
	ring_shape_t s;
	float x0 = to_dev_x(p, r.x), y0 = to_dev_y(p, r.y), x1 = to_dev_x(p, r.x + r.width), y1 = to_dev_y(p, r.y + r.height);
	s.cx = (x0 + x1) * 0.5f;
	s.cy = (y0 + y1) * 0.5f;
	s.rx = zrn_maxf((x1 - x0) * 0.5f, 0.01f);
	s.ry = zrn_maxf((y1 - y0) * 0.5f, 0.01f);
	s.stroke = 0;
	fill_shape(p, x0, y0, x1, y1, ellipse_coverage, &s, c);
}

static void sw_stroke_ellipse(zrn_painter_t* const p, const ZRect r, const float thickness, const ZColor c) {
	ring_shape_t s;
	float x0 = to_dev_x(p, r.x), y0 = to_dev_y(p, r.y), x1 = to_dev_x(p, r.x + r.width), y1 = to_dev_y(p, r.y + r.height);
	s.cx = (x0 + x1) * 0.5f;
	s.cy = (y0 + y1) * 0.5f;
	s.rx = zrn_maxf((x1 - x0) * 0.5f, 0.01f);
	s.ry = zrn_maxf((y1 - y0) * 0.5f, 0.01f);
	s.stroke = thickness * p->scale;
	fill_shape(p, x0, y0, x1, y1, ellipse_coverage, &s, c);
}

/* ---- polygon by scanlines, 4 sub-scanlines and exact horizontal coverage ---- */

static void sw_fill_polygon(zrn_painter_t* const p, const float* const xy, const int count, const ZColor c) {
	enum { SUB = 4 };
	float miny = 1e30f, maxy = -1e30f, *cover;
	int y, y0, y1, x0 = p->clip[0], x1 = p->clip[2], width, i, s;
	if (count < 3) return;
	for (i = 0; i < count; i++) {
		const float py = to_dev_y(p, xy[i * 2 + 1]);
		if (py < miny) miny = py;
		if (py > maxy) maxy = py;
	}
	y0 = (int)floorf(miny);
	y1 = (int)ceilf(maxy);
	if (y0 < p->clip[1]) y0 = p->clip[1];
	if (y1 > p->clip[3]) y1 = p->clip[3];
	width = x1 - x0;
	if (width <= 0 || y1 <= y0) return;
	cover = (float*)malloc((size_t)width * sizeof *cover);
	if (!cover) return;
	for (y = y0; y < y1; y++) {
		memset(cover, 0, (size_t)width * sizeof *cover);
		for (s = 0; s < SUB; s++) {
			const float sy = (float)y + ((float)s + 0.5f) / SUB;
			float xs[64];
			int n = 0, k, j;
			for (i = 0; i < count && n < 62; i++) {
				const int next = (i + 1) % count;
				const float ax = to_dev_x(p, xy[i * 2]), ay = to_dev_y(p, xy[i * 2 + 1]);
				const float bx = to_dev_x(p, xy[next * 2]), by = to_dev_y(p, xy[next * 2 + 1]);
				if ((ay <= sy && by > sy) || (by <= sy && ay > sy)) xs[n++] = ax + (sy - ay) / (by - ay) * (bx - ax);
			}
			for (k = 1; k < n; k++) {
				const float v = xs[k];
				for (j = k - 1; j >= 0 && xs[j] > v; j--) xs[j + 1] = xs[j];
				xs[j + 1] = v;
			}
			for (k = 0; k + 1 < n; k += 2) {
				const float a = zrn_maxf(xs[k], (float)x0), b = zrn_minf(xs[k + 1], (float)x1);
				int ia, ib, x;
				if (b <= a) continue;
				ia = (int)floorf(a);
				ib = (int)ceilf(b);
				for (x = ia; x < ib; x++) {
					const float l = zrn_maxf(a, (float)x), r = zrn_minf(b, (float)x + 1.0f);
					cover[x - x0] += (r - l) / SUB;
				}
			}
		}
		{
			uint8_t* const row = p->pixels + (size_t)y * (size_t)p->stride;
			for (int x = 0; x < width; x++) if (cover[x] > 0.0f) blend(row + (x0 + x) * 4, c, cover[x], p->alpha);
		}
	}
	free(cover);
}

static void sw_draw_line(zrn_painter_t* const p, const float x0, const float y0, const float x1, const float y1, const float thickness, const ZColor c) {
	float dx = x1 - x0, dy = y1 - y0, len = sqrtf(dx * dx + dy * dy), nx, ny, h = thickness * 0.5f, q[8];
	if (len <= 0.0001f) return;
	nx = -dy / len * h;
	ny = dx / len * h;
	q[0] = x0 + nx; q[1] = y0 + ny;
	q[2] = x1 + nx; q[3] = y1 + ny;
	q[4] = x1 - nx; q[5] = y1 - ny;
	q[6] = x0 - nx; q[7] = y0 - ny;
	zrn_fill_polygon(p, q, 4, c);
}

/* ---- text glyphs and images ---- */

static void sw_blit_alpha(zrn_painter_t* const p, const int x, const int y, const int w, const int h, const uint8_t* const alpha, const int stride, const ZColor c) {
	int ix0 = x, iy0 = y, ix1 = x + w, iy1 = y + h, ix, iy;
	if (ix0 < p->clip[0]) ix0 = p->clip[0];
	if (iy0 < p->clip[1]) iy0 = p->clip[1];
	if (ix1 > p->clip[2]) ix1 = p->clip[2];
	if (iy1 > p->clip[3]) iy1 = p->clip[3];
	for (iy = iy0; iy < iy1; iy++) {
		uint8_t* const row = p->pixels + (size_t)iy * (size_t)p->stride;
		const uint8_t* src = alpha + (size_t)(iy - y) * (size_t)stride;
		for (ix = ix0; ix < ix1; ix++) {
			const uint8_t a = src[ix - x];
			if (a) blend(row + ix * 4, c, (float)a / 255.0f, p->alpha);
		}
	}
}

static void sw_draw_image(zrn_painter_t* const p, const zrn_image_t* const image, const ZRect dest, const ZRect source) {
	const float x0 = to_dev_x(p, dest.x), y0 = to_dev_y(p, dest.y), x1 = to_dev_x(p, dest.x + dest.width), y1 = to_dev_y(p, dest.y + dest.height);
	int ix0 = (int)floorf(x0), iy0 = (int)floorf(y0), ix1 = (int)ceilf(x1), iy1 = (int)ceilf(y1), x, y;
	if (!image || x1 <= x0 || y1 <= y0) return;
	if (ix0 < p->clip[0]) ix0 = p->clip[0];
	if (iy0 < p->clip[1]) iy0 = p->clip[1];
	if (ix1 > p->clip[2]) ix1 = p->clip[2];
	if (iy1 > p->clip[3]) iy1 = p->clip[3];
	for (y = iy0; y < iy1; y++) {
		uint8_t* const row = p->pixels + (size_t)y * (size_t)p->stride;
		const float v = source.y + ((float)y + 0.5f - y0) / (y1 - y0) * source.height;
		const int sy = (int)zrn_clampf(floorf(v), 0, (float)image->height - 1);
		for (x = ix0; x < ix1; x++) {
			const float u = source.x + ((float)x + 0.5f - x0) / (x1 - x0) * source.width;
			const int sx = (int)zrn_clampf(floorf(u), 0, (float)image->width - 1);
			const uint8_t* s = image->rgba + ((size_t)sy * (size_t)image->width + (size_t)sx) * 4;
			const ZColor c = { (float)s[0] / 255.0f, (float)s[1] / 255.0f, (float)s[2] / 255.0f, (float)s[3] / 255.0f };
			blend(row + x * 4, c, 1.0f, p->alpha);
		}
	}
}

/* ---- the software backend ---- */

/* a buffer in memory has no GPU to project with: what is transformed is drawn where it lies */
static void sw_set_transform(zrn_painter_t* p, const float* m) { (void)p; (void)m; }

const zrn_backend_t zrn_backend_software = {
	.name = "software",
	.paint_clear = sw_paint_clear,
	.fill_rect = sw_fill_rect,
	.fill_rrect = sw_fill_rrect,
	.fill_ellipse = sw_fill_ellipse,
	.stroke_ellipse = sw_stroke_ellipse,
	.fill_polygon = sw_fill_polygon,
	.draw_line = sw_draw_line,
	.blit_alpha = sw_blit_alpha,
	.draw_image = sw_draw_image,
	.push_clip = zrn_clip_push,
	.pop_clip = zrn_clip_pop,
	.clipped_out = zrn_clip_out,
	.set_transform = sw_set_transform,
};
