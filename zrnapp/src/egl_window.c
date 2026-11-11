/**
 * zrnapp - egl_window.c
 * The EGL window of a Wayland surface. The context is made here; the drawing is the ZGlTarget of the core (zrnui/platform.h),
 * which paints in the context that is current and is told how to make it current again.
 */

#include "egl_window.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <wayland-egl.h>

static constexpr EGLint MSAA_SAMPLES = 4;

struct zrnapp_gl {
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	struct wl_egl_window* wl_window;
	ZGlTarget* target;
	int width, height;
};

static void report(const char* const message) {
	Zreport_error(Z_ERROR_IO, message);
}

/* A config with 4x MSAA; a driver without one gets a plain config, so that the GPU still draws (without anti-aliased edges). */
static bool choose_config(zrnapp_gl_t* const gl) {
	const EGLint multisampled[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1, EGL_SAMPLES, MSAA_SAMPLES, EGL_NONE
	};
	const EGLint plain[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE
	};
	EGLint n = 0;
	if (eglChooseConfig(gl->display, multisampled, &gl->config, 1, &n) && n > 0) return true;
	Z_DEBUG_LOG("no 4x MSAA config: the edges are not anti-aliased\n");
	if (eglChooseConfig(gl->display, plain, &gl->config, 1, &n) && n > 0) return true;
	report("no usable EGL config");
	return false;
}

static void make_current(void* const user) {
	const zrnapp_gl_t* const gl = (const zrnapp_gl_t*)user;
	eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context);
}

void zrnapp_gl_destroy(zrnapp_gl_t* const gl) {
	if (!gl) return;
	if (gl->display != EGL_NO_DISPLAY) {
		if (gl->target) {
			make_current(gl);
			Zgl_target_destroy(gl->target);
		}
		eglMakeCurrent(gl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (gl->surface != EGL_NO_SURFACE) eglDestroySurface(gl->display, gl->surface);
		if (gl->context != EGL_NO_CONTEXT) eglDestroyContext(gl->display, gl->context);
		eglTerminate(gl->display);
	}
	if (gl->wl_window) wl_egl_window_destroy(gl->wl_window);
	free(gl);
}

zrnapp_gl_t* zrnapp_gl_create(void* const wl_display, void* const wl_surface, const int width, const int height) {
	zrnapp_gl_t* const gl = (zrnapp_gl_t*)calloc(1, sizeof *gl);
	if (!gl) return nullptr;
	const PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	EGLint major, minor;
	gl->display = get_platform ? get_platform(EGL_PLATFORM_WAYLAND_KHR, wl_display, nullptr) : EGL_NO_DISPLAY;
	if (gl->display == EGL_NO_DISPLAY || !eglInitialize(gl->display, &major, &minor)) {
		report("no EGL display");
		gl->display = EGL_NO_DISPLAY;
		goto fail;
	}
	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
		report("no OpenGL ES");
		goto fail;
	}
	if (!choose_config(gl)) goto fail;
	{
		constexpr EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		gl->context = eglCreateContext(gl->display, gl->config, EGL_NO_CONTEXT, context_attribs);
	}
	if (gl->context == EGL_NO_CONTEXT) {
		report("cannot create an OpenGL ES 2 context");
		goto fail;
	}
	gl->wl_window = wl_egl_window_create((struct wl_surface*)wl_surface, width, height);
	if (!gl->wl_window) goto fail;
	gl->surface = eglCreateWindowSurface(gl->display, gl->config, (EGLNativeWindowType)gl->wl_window, nullptr);
	if (gl->surface == EGL_NO_SURFACE || !eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context)) {
		report("cannot make the OpenGL context current");
		goto fail;
	}
	eglSwapInterval(gl->display, 0);
	gl->target = Zgl_target_create(make_current, gl);
	if (!gl->target) {
		report("the OpenGL ES 2 renderer did not start");
		goto fail;
	}
	gl->width = width;
	gl->height = height;
#ifdef Z_DEBUG
	{
		GLint sample_buffers = 0, samples = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
		glGetIntegerv(GL_SAMPLES, &samples);
		Z_DEBUG_LOG("window framebuffer: %d sample buffers, %d samples\n", sample_buffers, samples);
	}
#endif
	return gl;
fail:
	zrnapp_gl_destroy(gl);
	return nullptr;
}

void zrnapp_gl_resize(zrnapp_gl_t* const gl, const int width, const int height) {
	if (!gl || !gl->wl_window) return;
	wl_egl_window_resize(gl->wl_window, width, height, 0, 0);
	gl->width = width;
	gl->height = height;
}

void zrnapp_gl_present(zrnapp_gl_t* const gl, const Zid root, const int width, const int height) {
	if (width != gl->width || height != gl->height) zrnapp_gl_resize(gl, width, height);
	(void)Zgl_target_paint(root, gl->target, width, height);
	eglSwapBuffers(gl->display, gl->surface);
}
