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

	/** What the window says about itself. Zero fields mean the default. */
	typedef struct ZWindowOptions {
		/** The title in the title bar and the task switcher. NULL: the app id. */
		const char* title;
		/**
		 * The app id: the identity by which the shell finds the .desktop file, the icon and the grouping of windows, so it is the
		 * name of the .desktop file without the suffix (`org.example.Notes` for `org.example.Notes.desktop`). NULL or empty:
		 * the file name of the executable, as GTK does; a program installed as `notes` with `notes.desktop` needs nothing here.
		 */
		const char* app_id;
	} ZWindowOptions;

	/**
	 * @brief Opens a window for `root` and runs its event loop until the window
	 * is closed, the compositor goes away, or Zwindow_quit is called.
	 * The window is as big as the root (in logical pixels); resizing it
	 * resizes the root. The scale is the one of the compositor ($ZRNUI_SCALE overrides it, for debugging).
	 * Zroot_tick is called every ~16 ms: animations, caret, repeat buttons.
	 * @param options NULL: the defaults of ZWindowOptions.
	 * @return true after a normal close, false when no window could be opened (see Zget_error).
	 */
	ZRNAPP_API int Zwindow_run_ex(Zid root, const ZWindowOptions* options);

	/**
	 * @brief Zwindow_run_ex with only a title. The app id is the one of the executable (see ZWindowOptions.app_id): the title
	 * is no longer used as the app id, which it was before and which made "Hello World" the identity of a window.
	 */
	ZRNAPP_API int Zwindow_run(Zid root, const char* title);

	/** @brief Ends the loop of Zwindow_run, from a callback. */
	ZRNAPP_API void Zwindow_quit(void);

#ifdef __cplusplus
}
#endif

#endif
