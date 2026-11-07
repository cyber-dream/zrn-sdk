/**
 * Kestrel
 * kestrel_window.h : a window for a root, on Wayland.
 *
 * Built when wayland-client and xkbcommon are found (KESTREL_WITH_WAYLAND).
 * The window draws the frames of the root into wl_shm buffers (no GPU) and
 * forwards pointer, touch, wheel, keyboard and text to the Kroot_inject_*
 * functions. Without Wayland support the function exists and fails.
 */

#ifndef KESTREL_WINDOW_H
#define KESTREL_WINDOW_H

#include "kestrel_host.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Opens a window for `root` and runs its event loop until the window
	 * is closed, the compositor goes away, or Kwindow_quit is called.
	 * The window is as big as the root (in logical pixels); resizing it
	 * resizes the root. Integer scale: $KESTREL_SCALE (default 1).
	 * Kroot_tick is called every ~16 ms: animations, caret, repeat buttons.
	 * @param title Window title, also the app id.
	 * @return K_TRUE after a normal close, K_FALSE when no window could be opened (see Kget_error).
	 */
	KESTREL_API int Kwindow_run(Kid root, const char* title);

	/** @brief Ends the loop of Kwindow_run, from a callback. */
	KESTREL_API void Kwindow_quit(void);

#ifdef __cplusplus
}
#endif

#endif
