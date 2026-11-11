/**
 * zrnapp
 * zrnapp.h : the app cycle of a zrnui application and its integration in the OS.
 *
 * zrnui (the UI core) draws a tree and knows no window. zrnapp owns the rest: a window on Wayland, the event loop,
 * pointer, touch, wheel, keyboard and text going into the Zroot_inject_* functions, the scale of the shell, and the
 * frames of the root going to the compositor (wl_shm buffers, or OpenGL ES 2 through EGL). It talks to the core
 * only through zrnui/platform.h, so a shell that embeds zrnui does what zrnapp does, and links zrnui alone.
 *
 * Built when wayland-client and xkbcommon are found. Without them the functions exist and fail.
 */

#ifndef ZRNAPP_H
#define ZRNAPP_H

#include <zrnui.h>

#if defined(_WIN32) && defined(ZRNAPP_BUILD_DLL)
	#define ZRNAPP_API __declspec(dllexport)
#elif defined(_WIN32) && defined(ZRNAPP_USE_DLL)
	#define ZRNAPP_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(ZRNAPP_BUILD_DLL)
	#define ZRNAPP_API __attribute__((visibility("default")))
#else
	#define ZRNAPP_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Opens a window for `root` and runs its event loop until the window
	 * is closed, the compositor goes away, or Zwindow_quit is called.
	 * The window is as big as the root (in logical pixels); resizing it
	 * resizes the root. The scale is the one of the compositor ($ZRNUI_SCALE overrides it, for debugging).
	 * Zroot_tick is called every ~16 ms: animations, caret, repeat buttons.
	 * @param title Window title, also the app id.
	 * @return true after a normal close, false when no window could be opened (see Zget_error).
	 */
	ZRNAPP_API int Zwindow_run(Zid root, const char* title);

	/** @brief Ends the loop of Zwindow_run, from a callback. */
	ZRNAPP_API void Zwindow_quit(void);

#ifdef __cplusplus
}
#endif

#endif
