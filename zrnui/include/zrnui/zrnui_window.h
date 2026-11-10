/**
 * zrnui
 * zrnui_window.h : a window for a root, on Wayland.
 *
 * Built when wayland-client and xkbcommon are found (ZRNUI_WITH_WAYLAND).
 * The window draws the frames of the root into wl_shm buffers (no GPU) and
 * forwards pointer, touch, wheel, keyboard and text to the Zroot_inject_*
 * functions. Without Wayland support the function exists and fails.
 */

#ifndef ZRNUI_WINDOW_H
#define ZRNUI_WINDOW_H

#include "host.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Opens a window for `root` and runs its event loop until the window
	 * is closed, the compositor goes away, or Zwindow_quit is called.
	 * The window is as big as the root (in logical pixels); resizing it
	 * resizes the root. Integer scale: $ZRNUI_SCALE (default 1).
	 * Zroot_tick is called every ~16 ms: animations, caret, repeat buttons.
	 * @param title Window title, also the app id.
	 * @return true after a normal close, false when no window could be opened (see Zget_error).
	 */
	ZRNUI_API int Zwindow_run(Zid root, const char* title);

	/** @brief Ends the loop of Zwindow_run, from a callback. */
	ZRNUI_API void Zwindow_quit(void);

#ifdef __cplusplus
}
#endif

#endif
