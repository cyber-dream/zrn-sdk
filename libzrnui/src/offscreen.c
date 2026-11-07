// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * A page drawn with no compositor: an EGL context on Mesa's surfaceless
 * platform (or a GBM render node), a framebuffer object, glReadPixels, PNG.
 * The same GLES2 path as on screen, so what this writes is what the app
 * shows. Used by `zrn render`, the tests and the evidence pictures.
 */
#define _GNU_SOURCE
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <fcntl.h>
#include <gbm.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "zui-internal.h"

#ifndef EGL_PLATFORM_SURFACELESS_MESA
#define EGL_PLATFORM_SURFACELESS_MESA 0x31DD
#endif
#ifndef EGL_PLATFORM_GBM_KHR
#define EGL_PLATFORM_GBM_KHR 0x31D7
#endif

int zui_png_write(const char *path, const uint8_t *rgba, int w, int h, bool flip) {
	FILE *f = fopen(path, "wb");
	if (!f) {
		fprintf(stderr, "zrnui: cannot write %s\n", path);
		return -1;
	}
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	if (setjmp(png_jmpbuf(png))) {
		png_destroy_write_struct(&png, &info);
		fclose(f);
		return -1;
	}
	png_init_io(png, f);
	png_set_IHDR(png, info, (png_uint_32)w, (png_uint_32)h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);
	for (int y = 0; y < h; y++) {
		int row = flip ? h - 1 - y : y;
		png_write_row(png, (png_const_bytep)(rgba + (size_t)row * w * 4));
	}
	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &info);
	fclose(f);
	return 0;
}

static EGLDisplay open_display(int *drm_fd, struct gbm_device **gbm) {
	PFNEGLGETPLATFORMDISPLAYEXTPROC get = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	if (!get) return EGL_NO_DISPLAY;
	EGLDisplay d = get(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL);
	if (d != EGL_NO_DISPLAY && eglInitialize(d, NULL, NULL)) return d;
	/* a render node through GBM */
	const char *nodes[] = { "/dev/dri/renderD128", "/dev/dri/renderD129" };
	for (size_t i = 0; i < 2; i++) {
		int fd = open(nodes[i], O_RDWR | O_CLOEXEC);
		if (fd < 0) continue;
		struct gbm_device *g = gbm_create_device(fd);
		if (g) {
			d = get(EGL_PLATFORM_GBM_KHR, g, NULL);
			if (d != EGL_NO_DISPLAY && eglInitialize(d, NULL, NULL)) {
				*drm_fd = fd;
				*gbm = g;
				return d;
			}
			gbm_device_destroy(g);
		}
		close(fd);
	}
	return EGL_NO_DISPLAY;
}

int zui_offscreen_png(zui_app *app, zui_node *page, const char *path, int w, int h, int scale) {
	if (!page) {
		fprintf(stderr, "zrnui: no page to draw\n");
		return -1;
	}
	int drm_fd = -1;
	struct gbm_device *gbm = NULL;
	EGLDisplay dpy = open_display(&drm_fd, &gbm);
	if (dpy == EGL_NO_DISPLAY) {
		fprintf(stderr, "zrnui: no EGL display for offscreen drawing (Mesa surfaceless or a render node)\n");
		return -1;
	}
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint attrs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
	EGLConfig config = NULL;
	EGLint count = 0;
	eglChooseConfig(dpy, attrs, &config, 1, &count);
	EGLint ctx_attrs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	EGLContext ctx = eglCreateContext(dpy, count ? config : EGL_NO_CONFIG_KHR, EGL_NO_CONTEXT, ctx_attrs);
	int rc = -1;
	if (ctx == EGL_NO_CONTEXT || !eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, ctx)) {
		fprintf(stderr, "zrnui: no surfaceless GLES2 context\n");
		goto out;
	}
	int fw = w * scale, fh = h * scale;
	GLuint tex, fbo;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fw, fh, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "zrnui: offscreen framebuffer incomplete\n");
		goto out_ctx;
	}
	if (!zg_init()) goto out_ctx;
	zui_node *was = app->page;
	app->page = page;
	app->width = (float)w;
	app->height = (float)h;
	app->scale = scale;
	app->laid_out = false;
	const char *scroll = getenv("ZUI_RENDER_SCROLL");
	if (scroll) {
		zui_do_layout(app, page);
		app->laid_out = true;
		struct zui_scroll *sv = zui_scroller(page);
		if (sv) sv->scroll = (float)atof(scroll);
	}
	uint32_t now = zui_now();
	struct zui_palette pal = zui_pal(app);
	zg_begin(fw, fh, (float)scale, pal.bg);
	zui_draw(app, now);
	zg_end();
	uint8_t *px = malloc((size_t)fw * fh * 4);
	glReadPixels(0, 0, fw, fh, GL_RGBA, GL_UNSIGNED_BYTE, px);
	/* the page's own background is opaque: no alpha left in the picture */
	for (size_t i = 3; i < (size_t)fw * fh * 4; i += 4) px[i] = 255;
	rc = zui_png_write(path, px, fw, fh, true);
	free(px);
	zg_fini();
	app->page = was ? was : page;
out_ctx:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(dpy, ctx);
out:
	eglTerminate(dpy);
	if (gbm) gbm_device_destroy(gbm);
	if (drm_fd >= 0) close(drm_fd);
	return rc;
}
