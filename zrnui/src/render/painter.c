/**
 * zrnui - render/painter.c
 * The painter, which is the same for every renderer: the state that drawing
 * has (origin, scale, opacity, clip), the drawing calls that go to the
 * renderer of the painter, the shapes made of other shapes, and images with
 * PNG reading (libpng). The renderers draw; nothing here writes a file.
 *
 * The renderers are in this directory: software.c (the buffer in memory)
 * and gles.c (OpenGL ES 2). A renderer is a zrn_backend_t, internal.h.
 */

#include "../internal.h"

#include <png.h>

void zrn_painter_init(zrn_painter_t* const p, uint8_t* const pixels, const int width, const int height, const float scale) {
	memset(p, 0, sizeof *p);
	p->be = &zrn_backend_software;
	zrn_xf_identity(p->xf);
	p->pixels = pixels;
	p->width = width;
	p->height = height;
	p->stride = width * 4;
	p->scale = scale;
	p->alpha = 1.0f;
	p->clip[0] = 0;
	p->clip[1] = 0;
	p->clip[2] = width;
	p->clip[3] = height;
}

static inline float to_dev_x(const zrn_painter_t* p, float x) { return p->tx + x * p->scale; }
static inline float to_dev_y(const zrn_painter_t* p, float y) { return p->ty + y * p->scale; }

void zrn_clip_push(zrn_painter_t* const p, const ZRect r) {
	static int warned = 0;
	int x0, y0, x1, y1;
	/* a full stack cannot store the clip, so the push is ignored: the clip stays as it is (a draw may exceed the new clip, which is the lesser evil) and the matching pop restores one level early */
	if (p->clip_depth >= 32) {
		if (!warned) {
			warned = 1;
			zrn_error(Z_ERROR_CYCLE, "the clip stack is full: a clip push was ignored");
		}
		return;
	}
	/* a transformed element is drawn where the clip rectangle is not: it clips nothing then */
	if (p->has_xf) {
		memcpy(p->clip_stack[p->clip_depth], p->clip, sizeof p->clip);
		p->clip_depth++;
		return;
	}
	x0 = (int)floorf(to_dev_x(p, r.x) + 0.5f);
	y0 = (int)floorf(to_dev_y(p, r.y) + 0.5f);
	x1 = (int)floorf(to_dev_x(p, r.x + r.width) + 0.5f);
	y1 = (int)floorf(to_dev_y(p, r.y + r.height) + 0.5f);
	memcpy(p->clip_stack[p->clip_depth], p->clip, sizeof p->clip);
	p->clip_depth++;
	if (x0 > p->clip[0]) p->clip[0] = x0;
	if (y0 > p->clip[1]) p->clip[1] = y0;
	if (x1 < p->clip[2]) p->clip[2] = x1;
	if (y1 < p->clip[3]) p->clip[3] = y1;
}

void zrn_clip_pop(zrn_painter_t* const p) {
	if (p->clip_depth <= 0) return;
	p->clip_depth--;
	if (p->clip_depth < 32) memcpy(p->clip, p->clip_stack[p->clip_depth], sizeof p->clip);
}

int zrn_clip_out(const zrn_painter_t* const p, const ZRect r) {
	if (p->has_xf) return 0;
	const float x0 = to_dev_x(p, r.x), y0 = to_dev_y(p, r.y);
	const float x1 = to_dev_x(p, r.x + r.width), y1 = to_dev_y(p, r.y + r.height);
	return x1 <= (float)p->clip[0] || y1 <= (float)p->clip[1] || x0 >= (float)p->clip[2] || y0 >= (float)p->clip[3];
}

void zrn_stroke_rect(zrn_painter_t* const p, const ZRect r, const ZThickness t, const ZColor c) {
	if (t.top > 0) zrn_fill_rect(p, zrn_rect(r.x, r.y, r.width, t.top), c);
	if (t.bottom > 0) zrn_fill_rect(p, zrn_rect(r.x, r.y + r.height - t.bottom, r.width, t.bottom), c);
	if (t.left > 0) zrn_fill_rect(p, zrn_rect(r.x, r.y + t.top, t.left, r.height - t.top - t.bottom), c);
	if (t.right > 0) zrn_fill_rect(p, zrn_rect(r.x + r.width - t.right, r.y + t.top, t.right, r.height - t.top - t.bottom), c);
}

/* ---- the painter: every drawing call goes to the backend of the painter ---- */

void zrn_paint_clear(zrn_painter_t* p, ZColor c) { p->be->paint_clear(p, c); }
void zrn_fill_rect(zrn_painter_t* p, ZRect r, ZColor c) { p->be->fill_rect(p, r, c); }
void zrn_fill_rrect(zrn_painter_t* p, ZRect r, ZCornerRadius radius, ZColor c) { p->be->fill_rrect(p, r, radius, c); }
void zrn_fill_ellipse(zrn_painter_t* p, ZRect r, ZColor c) { p->be->fill_ellipse(p, r, c); }
void zrn_stroke_ellipse(zrn_painter_t* p, ZRect r, float thickness, ZColor c) { p->be->stroke_ellipse(p, r, thickness, c); }
void zrn_fill_polygon(zrn_painter_t* p, const float* xy, int count, ZColor c) { p->be->fill_polygon(p, xy, count, c); }
void zrn_draw_line(zrn_painter_t* p, float x0, float y0, float x1, float y1, float thickness, ZColor c) { p->be->draw_line(p, x0, y0, x1, y1, thickness, c); }
void zrn_blit_alpha(zrn_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, ZColor c) { p->be->blit_alpha(p, x, y, w, h, alpha, stride, c); }
void zrn_draw_image(zrn_painter_t* p, const zrn_image_t* image, ZRect dest, ZRect source) { p->be->draw_image(p, image, dest, source); }
void zrn_push_clip(zrn_painter_t* p, ZRect r) { p->be->push_clip(p, r); }
void zrn_pop_clip(zrn_painter_t* p) { p->be->pop_clip(p); }
int zrn_clipped_out(const zrn_painter_t* p, ZRect r) { return p->be->clipped_out(p, r); }

/* ============================================================================
 *  images and PNG
 * ============================================================================ */


zrn_image_t* zrn_image_from_rgba(const int w, const int h, const uint8_t* const rgba) {
	zrn_image_t* image;
	if (w <= 0 || h <= 0 || !rgba) return nullptr;
	image = (zrn_image_t*)calloc(1, sizeof *image);
	if (!image) return nullptr;
	image->rgba = (uint8_t*)malloc((size_t)w * (size_t)h * 4);
	if (!image->rgba) { free(image); return nullptr; }
	memcpy(image->rgba, rgba, (size_t)w * (size_t)h * 4);
	image->width = w;
	image->height = h;
	return image;
}

void zrn_image_free(zrn_image_t* const image) {
	if (image && image->gl_owner) zrn_gl_forget_image(image);
	if (!image) return;
	free(image->rgba);
	free(image);
}

zrn_image_t* zrn_image_load_png(const char* const path) {
	FILE* f = fopen(path, "rb");
	png_structp png;
	png_infop info;
	/* these live across a longjmp of libpng */
	png_bytep* volatile rows = nullptr;
	zrn_image_t* volatile image = nullptr;
	volatile int w, h;
	int y;
	uint8_t* volatile pixels = nullptr;
	if (!f) return nullptr;
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	info = png ? png_create_info_struct(png) : nullptr;
	if (!png || !info) goto done;
	if (setjmp(png_jmpbuf(png))) goto done;
	png_init_io(png, f);
	png_read_info(png, info);
	{
		const png_uint_32 pw = png_get_image_width(png, info);
		const png_uint_32 ph = png_get_image_height(png, info);
		/* a sane bound: it also guarantees the values fit an int and the allocation below cannot overflow */
		if (pw == 0 || ph == 0 || pw > 16384 || ph > 16384 || (size_t)pw > SIZE_MAX / 4 / (size_t)ph) {
			zrn_error(Z_ERROR_IO, "PNG image %s is empty or too large (%lux%lu)", path, (unsigned long)pw, (unsigned long)ph);
			goto done;
		}
		w = (int)pw;
		h = (int)ph;
	}
	if (png_get_bit_depth(png, info) == 16) png_set_strip_16(png);
	if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
	if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY && png_get_bit_depth(png, info) < 8) png_set_expand_gray_1_2_4_to_8(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
	if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY || png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);
	png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	png_read_update_info(png, info);
	pixels = (uint8_t*)malloc((size_t)w * (size_t)h * 4);
	rows = (png_bytep*)malloc((size_t)h * sizeof *rows);
	if (!pixels || !rows) goto done;
	for (y = 0; y < h; y++) rows[y] = pixels + (size_t)y * (size_t)w * 4;
	png_read_image(png, rows);
	image = zrn_image_from_rgba(w, h, pixels);
done:
	free(rows);
	free(pixels);
	if (png) png_destroy_read_struct(&png, info ? &info : nullptr, nullptr);
	fclose(f);
	return image;
}
