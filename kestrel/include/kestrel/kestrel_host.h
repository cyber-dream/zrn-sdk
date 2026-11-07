/**
 * Kestrel
 * kestrel_host.h : the root that hosts a visual tree (Application.RootVisual
 * and its PhoneApplicationFrame), the theme, input, time and rendering.
 *
 * Kestrel opens no window by itself. A root owns a tree, lays it out, draws
 * it into an RGBA pixel buffer, and takes input that the program injects.
 * A program shows the buffer in whatever window or framebuffer it has
 * (Wayland shm, SDL, a test that writes a PNG) and forwards its events:
 *
 *   Kid root = Kroot_create(0, 0);
 *   Kroot_set_visual(root, page);
 *   for (;;) {
 *       Kroot_inject_pointer(root, K_POINTER_DOWN, x, y);   // from your events
 *       Kroot_tick(root, dt_ms);                            // animation, repeat, caret
 *       if (Kroot_render(root, &frame)) present(frame.pixels, frame.width, frame.height);
 *   }
 */

#ifndef KESTREL_HOST_H
#define KESTREL_HOST_H

#include "kestrel_controls.h"


typedef enum KPointerEvent {
	K_POINTER_DOWN = 0x0500,
	K_POINTER_UP,
	K_POINTER_MOVE,
	/** The pointer left the screen (a finger lifted without an up, a window left). */
	K_POINTER_LEAVE
} KPointerEvent;

typedef enum KKey {
	K_KEY_NONE = 0,
	K_KEY_BACKSPACE = 0x0600,
	K_KEY_TAB,
	K_KEY_ENTER,
	K_KEY_ESCAPE,
	K_KEY_SPACE,
	K_KEY_PAGE_UP,
	K_KEY_PAGE_DOWN,
	K_KEY_END,
	K_KEY_HOME,
	K_KEY_LEFT,
	K_KEY_UP,
	K_KEY_RIGHT,
	K_KEY_DOWN,
	K_KEY_DELETE,
	K_KEY_A,
	K_KEY_C,
	K_KEY_V,
	K_KEY_X
} KKey;

typedef enum KModifier {
	K_MOD_SHIFT = 1,
	K_MOD_CTRL = 2,
	K_MOD_ALT = 4
} KModifier;

/** A frame: 8 bit RGBA, rows top to bottom, `stride` bytes per row. Owned by the root, valid until the next render. */
typedef struct KFrame {
	const uint8_t* pixels;
	int width;
	int height;
	int stride;
} KFrame;


#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Creates a root of `width` x `height` logical pixels (480 x 800 is a WP8 screen).
	 * 0 x 0 leaves the size to the window: Kwindow_run sizes the root by what the compositor
	 * gives (480 x 800 until the first configure), so an application does not manage it.
	 * Dark theme, Cobalt accent as the system default of a Windows Phone 8 test device.
	 */
	KESTREL_API Kid Kroot_create(int width, int height);
	KESTREL_API void Kroot_destroy(Kid root);
	KESTREL_API int Kroot_is_valid(Kid root);

	/** @brief The tree shown. The root owns it; the previous visual is destroyed. */
	KESTREL_API void Kroot_set_visual(Kid root, Kid visual);
	KESTREL_API Kid Kroot_get_visual(Kid root);

	KESTREL_API void Kroot_set_size(Kid root, int width, int height);

	/**
	 * @brief Which renderer draws the root. K_RENDERER_AUTO (the default; $KESTREL_RENDERER=auto|software|gpu
	 * sets it for a new root) lets the window use OpenGL ES 2 when it can and Kroot_render use the software
	 * painter; K_RENDERER_SOFTWARE never touches the GPU (the safe mode: no animations that need one);
	 * K_RENDERER_GPU draws Kroot_render frames on an off-screen OpenGL context, and Kwindow_run fails
	 * when there is none. Choose before the first Kroot_render / Kwindow_run.
	 */
	KESTREL_API void Kroot_set_renderer(Kid root, KRenderer renderer);
	KESTREL_API KRenderer Kroot_get_renderer(Kid root);

	/**
	 * @brief Whether animations run. K_ANIMATIONS_AUTO (the default; $KESTREL_ANIMATIONS=on|off|auto) runs them
	 * when the frames go through the OpenGL renderer and leaves them off in the software renderer, the safe mode,
	 * where every change is at once: the page a Frame shows, the pressed look, a dropped list.
	 */
	KESTREL_API void Kroot_set_animations(Kid root, KAnimations mode);
	KESTREL_API KAnimations Kroot_get_animations(Kid root);
	/** @return K_TRUE when animations run now: the mode, and for AUTO the renderer, decide. */
	KESTREL_API int Kroot_animations_enabled(Kid root);
	/** @return "software" or "gles2": the renderer of the last frame. */
	KESTREL_API const char* Kroot_get_renderer_name(Kid root);
	/**
	 * @brief The Back button: when the visual of the root is a Frame that can go back, goes back.
	 * K_FALSE on the first page (or with no Frame): on Windows Phone Back there leaves the app, so the window quits.
	 * Escape does this in Kroot_inject_key.
	 */
	KESTREL_API int Kroot_back(Kid root);
	/** @brief Device pixels per logical pixel (a 720p WP8 phone draws 1.5). The frame is size * scale. */
	KESTREL_API void Kroot_set_scale(Kid root, float scale);

	/**
	 * Theme. Dark: white on black. Light: black on white. The accent colors
	 * the pressed buttons, the sliders, the progress bars and the selected
	 * items. Everything that was not given a color of its own follows.
	 */
	KESTREL_API void Kroot_set_theme(Kid root, KTheme theme);
	KESTREL_API KTheme Kroot_get_theme(Kid root);
	KESTREL_API void Kroot_set_accent(Kid root, KAccent accent);
	KESTREL_API void Kroot_set_accent_color(Kid root, KColor color);
	KESTREL_API KColor Kroot_get_accent_color(Kid root);
	/** @brief The color behind the tree: PhoneBackgroundBrush of the theme unless set. */
	KESTREL_API void Kroot_set_background(Kid root, KColor color);
	/**
	 * @return A color of the theme by its name, transparent black for an unknown name. The WP8 names
	 * ("PhoneForegroundColor", "PhoneSubtleColor"...) and, of Windows Phone 8.1 (themeresources.xaml),
	 * "PhoneBaseLowColor", "PhoneBaseMidColor", "PhoneBaseMediumMidColor", "PhoneBaseMediumMidSolidColor",
	 * "PhoneBaseMediumHighColor", "PhoneBaseLowSolidColor", "PhoneTextOverAccentColor", "TextBoxBackgroundColor",
	 * "TextBoxBorderColor", "TextBoxForegroundColor", "TextBoxPlaceholderColor", "TextBoxFocusedBackgroundThemeBrush",
	 * "SliderThumbBackgroundThemeBrush", "SliderThumbDisabledBackgroundThemeBrush", "SliderTrackBackgroundThemeBrush",
	 * "SliderTrackDecreaseDisabledBackgroundThemeBrush".
	 */
	KESTREL_API KColor Kroot_get_theme_color(Kid root, const char* name);

	/** @brief Measure and Arrange of the whole tree if anything changed. */
	KESTREL_API void Kroot_update_layout(Kid root);

	/**
	 * @brief Draws the tree. @return K_TRUE when something changed since the
	 * last frame (and the first time): `frame` holds a new picture. K_FALSE
	 * when the last picture is still right; `frame` is filled all the same.
	 * `frame` may be NULL.
	 */
	KESTREL_API int Kroot_render(Kid root, KFrame* frame);

	/** @brief Advances time: RepeatButton repeats, caret blink, indeterminate progress, tooltip delay. */
	KESTREL_API void Kroot_tick(Kid root, float delta_ms);

	/** @brief A pointer or a finger at a point of the root, in logical pixels. @return K_TRUE when an element handled it. */
	KESTREL_API int Kroot_inject_pointer(Kid root, KPointerEvent event, float x, float y);
	/** @brief The wheel at the last pointer position; positive scrolls up. */
	KESTREL_API int Kroot_inject_wheel(Kid root, float delta);
	KESTREL_API int Kroot_inject_key(Kid root, KKey key, int modifiers, int pressed);
	/** @brief Typed text, UTF-8, to the focused element. */
	KESTREL_API int Kroot_inject_text(Kid root, const char* text);

	KESTREL_API Kid Kroot_get_focus(Kid root);
	/** @brief The element under the pointer, or K_INVALID_ID. */
	KESTREL_API Kid Kroot_get_pointer_over(Kid root);
	/** @brief Moves the focus to the next / previous tab stop (Tab, Shift+Tab). */
	KESTREL_API void Kroot_move_focus(Kid root, int forward);

#ifdef __cplusplus
}
#endif

#endif
