/**
 * Kestrel - render/painter.c
 * The painter, which is the same for every renderer: the state that drawing
 * has (origin, scale, opacity, clip), the drawing calls that go to the
 * renderer of the painter, the shapes made of other shapes, and images with
 * PNG reading (libpng). The renderers draw; nothing here writes a file.
 *
 * The renderers are in this directory: software.c (the buffer in memory)
 * and gles.c (OpenGL ES 2). A renderer is a kst_backend_t, internal.h.
 */

#include "../internal.h"

#include <png.h>

void kst_painter_init(kst_painter_t* p, uint8_t* pixels, int width, int height, float scale) {
	memset(p, 0, sizeof *p);
	p->be = &kst_backend_software;
	kst_xf_identity(p->xf);
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

static inline float to_dev_x(const kst_painter_t* p, float x) { return p->tx + x * p->scale; }
static inline float to_dev_y(const kst_painter_t* p, float y) { return p->ty + y * p->scale; }

void kst_clip_push(kst_painter_t* p, KRect r) {
	/* a transformed element is drawn where the clip rectangle is not: it clips nothing then */
	if (p->has_xf) {
		if (p->clip_depth < 32) memcpy(p->clip_stack[p->clip_depth], p->clip, sizeof p->clip);
		p->clip_depth++;
		return;
	}
	int x0 = (int)floorf(to_dev_x(p, r.x) + 0.5f), y0 = (int)floorf(to_dev_y(p, r.y) + 0.5f);
	int x1 = (int)floorf(to_dev_x(p, r.x + r.width) + 0.5f), y1 = (int)floorf(to_dev_y(p, r.y + r.height) + 0.5f);
	if (p->clip_depth < 32) memcpy(p->clip_stack[p->clip_depth], p->clip, sizeof p->clip);
	p->clip_depth++;
	if (x0 > p->clip[0]) p->clip[0] = x0;
	if (y0 > p->clip[1]) p->clip[1] = y0;
	if (x1 < p->clip[2]) p->clip[2] = x1;
	if (y1 < p->clip[3]) p->clip[3] = y1;
}

void kst_clip_pop(kst_painter_t* p) {
	if (p->clip_depth <= 0) return;
	p->clip_depth--;
	if (p->clip_depth < 32) memcpy(p->clip, p->clip_stack[p->clip_depth], sizeof p->clip);
}

int kst_clip_out(const kst_painter_t* p, KRect r) {
	if (p->has_xf) return 0;
	float x0 = to_dev_x(p, r.x), y0 = to_dev_y(p, r.y);
	float x1 = to_dev_x(p, r.x + r.width), y1 = to_dev_y(p, r.y + r.height);
	return x1 <= (float)p->clip[0] || y1 <= (float)p->clip[1] || x0 >= (float)p->clip[2] || y0 >= (float)p->clip[3];
}

void kst_stroke_rect(kst_painter_t* p, KRect r, KThickness t, KColor c) {
	if (t.top > 0) kst_fill_rect(p, kst_rect(r.x, r.y, r.width, t.top), c);
	if (t.bottom > 0) kst_fill_rect(p, kst_rect(r.x, r.y + r.height - t.bottom, r.width, t.bottom), c);
	if (t.left > 0) kst_fill_rect(p, kst_rect(r.x, r.y + t.top, t.left, r.height - t.top - t.bottom), c);
	if (t.right > 0) kst_fill_rect(p, kst_rect(r.x + r.width - t.right, r.y + t.top, t.right, r.height - t.top - t.bottom), c);
}

/* ---- the painter: every drawing call goes to the backend of the painter ---- */

void kst_paint_clear(kst_painter_t* p, KColor c) { p->be->paint_clear(p, c); }
void kst_fill_rect(kst_painter_t* p, KRect r, KColor c) { p->be->fill_rect(p, r, c); }
void kst_fill_rrect(kst_painter_t* p, KRect r, KCornerRadius radius, KColor c) { p->be->fill_rrect(p, r, radius, c); }
void kst_fill_ellipse(kst_painter_t* p, KRect r, KColor c) { p->be->fill_ellipse(p, r, c); }
void kst_stroke_ellipse(kst_painter_t* p, KRect r, float thickness, KColor c) { p->be->stroke_ellipse(p, r, thickness, c); }
void kst_fill_polygon(kst_painter_t* p, const float* xy, int count, KColor c) { p->be->fill_polygon(p, xy, count, c); }
void kst_draw_line(kst_painter_t* p, float x0, float y0, float x1, float y1, float thickness, KColor c) { p->be->draw_line(p, x0, y0, x1, y1, thickness, c); }
void kst_blit_alpha(kst_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, KColor c) { p->be->blit_alpha(p, x, y, w, h, alpha, stride, c); }
void kst_draw_image(kst_painter_t* p, const kst_image_t* image, KRect dest, KRect source) { p->be->draw_image(p, image, dest, source); }
void kst_push_clip(kst_painter_t* p, KRect r) { p->be->push_clip(p, r); }
void kst_pop_clip(kst_painter_t* p) { p->be->pop_clip(p); }
int kst_clipped_out(const kst_painter_t* p, KRect r) { return p->be->clipped_out(p, r); }

/* ============================================================================
 *  images and PNG
 * ============================================================================ */


kst_image_t* kst_image_from_rgba(int w, int h, const uint8_t* rgba) {
	kst_image_t* image;
	if (w <= 0 || h <= 0 || !rgba) return NULL;
	image = (kst_image_t*)calloc(1, sizeof *image);
	if (!image) return NULL;
	image->rgba = (uint8_t*)malloc((size_t)w * (size_t)h * 4);
	if (!image->rgba) { free(image); return NULL; }
	memcpy(image->rgba, rgba, (size_t)w * (size_t)h * 4);
	image->width = w;
	image->height = h;
	return image;
}

void kst_image_free(kst_image_t* image) {
	if (image && image->gl_owner) kst_gl_forget_image(image);
	if (!image) return;
	free(image->rgba);
	free(image);
}

kst_image_t* kst_image_load_png(const char* path) {
	FILE* f = fopen(path, "rb");
	png_structp png;
	png_infop info;
	/* these live across a longjmp of libpng */
	png_bytep* volatile rows = NULL;
	kst_image_t* volatile image = NULL;
	volatile int w, h;
	int y;
	uint8_t* volatile pixels = NULL;
	if (!f) return NULL;
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png ? png_create_info_struct(png) : NULL;
	if (!png || !info) goto done;
	if (setjmp(png_jmpbuf(png))) goto done;
	png_init_io(png, f);
	png_read_info(png, info);
	w = (int)png_get_image_width(png, info);
	h = (int)png_get_image_height(png, info);
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
	image = kst_image_from_rgba(w, h, pixels);
done:
	free(rows);
	free(pixels);
	if (png) png_destroy_read_struct(&png, info ? &info : NULL, NULL);
	fclose(f);
	return image;
}
