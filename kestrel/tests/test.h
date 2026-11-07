/**
 * Kestrel tests - minimal test harness: no dependency, one executable per file.
 */

#ifndef KST_TEST_H
#define KST_TEST_H

#include <kestrel.h>

#include <png.h>
#include <stdlib.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

static int g_test_checks = 0;
static int g_test_failures = 0;

#define CHECK(condition) do { \
	g_test_checks++; \
	if (!(condition)) { \
		g_test_failures++; \
		printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
	} \
} while (0)

#define CHECK_NEAR(actual, expected) do { \
	double test_a_ = (double)(actual), test_e_ = (double)(expected); \
	g_test_checks++; \
	if (fabs(test_a_ - test_e_) > 0.51) { \
		g_test_failures++; \
		printf("  FAIL %s:%d: %s = %g, expected %g\n", __FILE__, __LINE__, #actual, test_a_, test_e_); \
	} \
} while (0)

#define CHECK_STR(actual, expected) do { \
	const char* test_a_ = (actual); \
	const char* test_e_ = (expected); \
	g_test_checks++; \
	if (!test_a_ || strcmp(test_a_, test_e_) != 0) { \
		g_test_failures++; \
		printf("  FAIL %s:%d: %s = \"%s\", expected \"%s\"\n", __FILE__, __LINE__, #actual, test_a_ ? test_a_ : "(null)", test_e_); \
	} \
} while (0)

#define RUN(test) do { printf("%s\n", #test); test(); } while (0)

static int test_finish(const char* name) {
	printf("%s: %d checks, %d failed\n", name, g_test_checks, g_test_failures);
	return g_test_failures ? 1 : 0;
}

/* Renders the root and writes the frame as a PNG: the library draws, a test is what keeps pictures. */
static int __attribute__((unused)) test_save_png(Kid root, const char* path) {
	KFrame f;
	FILE* file;
	png_structp png;
	png_infop info;
	png_bytep* volatile rows = NULL;
	volatile int ok = 0;
	int y;
	Kroot_render(root, &f);
	file = fopen(path, "wb");
	if (!file || !f.pixels) { if (file) fclose(file); return 0; }
	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png ? png_create_info_struct(png) : NULL;
	rows = (png_bytep*)malloc((size_t)f.height * sizeof *rows);
	if (!png || !info || !rows) goto done;
	if (setjmp(png_jmpbuf(png))) goto done;
	png_init_io(png, file);
	png_set_IHDR(png, info, (png_uint_32)f.width, (png_uint_32)f.height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);
	for (y = 0; y < f.height; y++) rows[y] = (png_bytep)(f.pixels + (size_t)y * (size_t)f.stride);
	png_write_image(png, rows);
	png_write_end(png, NULL);
	ok = 1;
done:
	free(rows);
	if (png) png_destroy_write_struct(&png, info ? &info : NULL);
	fclose(file);
	return ok;
}

/* Reads one pixel of the last frame. */
static void __attribute__((unused)) test_pixel(Kid root, int x, int y, int* r, int* g, int* b) {
	KFrame f;
	const unsigned char* px;
	Kroot_render(root, &f);
	px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
	*r = px[0];
	*g = px[1];
	*b = px[2];
}

#endif
