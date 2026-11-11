/**
 * zrnapp - egl_window.h
 * An OpenGL ES 2 context on the EGL window of a Wayland surface, and the frames of a zrnui root drawn in it.
 * Without EGL in the build (ZRNAPP_HAVE_EGL) the window has no GPU path: the functions below are empty and
 * zrnapp_gl_create fails, so the window draws with wl_shm.
 */

#ifndef ZRNAPP_EGL_WINDOW_H
#define ZRNAPP_EGL_WINDOW_H

#include <zrnui.h>

typedef struct zrnapp_gl zrnapp_gl_t;

#ifdef ZRNAPP_HAVE_EGL

/** NULL when there is no EGL / OpenGL ES 2 for this surface (the message is in Zget_error_message). */
zrnapp_gl_t* zrnapp_gl_create(void* wl_display, void* wl_surface, int width, int height);
void zrnapp_gl_resize(zrnapp_gl_t* gl, int width, int height);
/** Paints the root when something changed and shows the frame. */
void zrnapp_gl_present(zrnapp_gl_t* gl, Zid root, int width, int height);
void zrnapp_gl_destroy(zrnapp_gl_t* gl);

#else

static inline zrnapp_gl_t* zrnapp_gl_create(void* wl_display, void* wl_surface, int width, int height) {
	(void)wl_display; (void)wl_surface; (void)width; (void)height;
	Zreport_error(Z_ERROR_IO, "zrnapp was built without EGL");
	return nullptr;
}
static inline void zrnapp_gl_resize(zrnapp_gl_t* gl, int width, int height) { (void)gl; (void)width; (void)height; }
static inline void zrnapp_gl_present(zrnapp_gl_t* gl, Zid root, int width, int height) { (void)gl; (void)root; (void)width; (void)height; }
static inline void zrnapp_gl_destroy(zrnapp_gl_t* gl) { (void)gl; }

#endif

#endif
