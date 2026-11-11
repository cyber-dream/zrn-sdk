/**
 * zrnui
 * zrnui_host.h : the root that hosts a visual tree (Application.RootVisual
 * and its PhoneApplicationFrame), the theme, input, time and rendering.
 *
 * zrnui opens no window by itself. A root owns a tree, lays it out, draws
 * it into an RGBA pixel buffer, and takes input that the program injects.
 * A program shows the buffer in whatever window or framebuffer it has
 * (Wayland shm, SDL, a test that writes a PNG) and forwards its events:
 *
 *   Zid root = Zroot_create();
 *   Zroot_set_visual(root, page);
 *   for (;;) {
 *       Zroot_inject_pointer(root, Z_POINTER_DOWN, x, y);   // from your events
 *       Zroot_tick(root, dt_ms);                            // animation, repeat, caret
 *       if (Zroot_render(root, &frame)) present(frame.pixels, frame.width, frame.height);
 *   }
 */

#ifndef ZRNUI_HOST_H
#define ZRNUI_HOST_H

#include "zrnui_controls.h"

/**
 * What the library is told when it starts, instead of reading the environment itself. Zero fields mean the default.
 * The strings are copied.
 */
typedef struct ZConfig {
	/** Where the fonts are (the bundled Selawik and any Segoe). NULL: the directory compiled in. */
	const char* font_directory;
	/** The theme.ini that overrides the colors. NULL: beside the binary, then the one compiled in; "": none. */
	const char* theme_file;
	/** The renderer of a new root (Zroot_set_renderer changes it). */
	ZRenderer renderer;
	/** Whether the animations of a new root run (Zroot_set_animations changes it). */
	ZAnimations animations;
	/** The speed of every animation: 0.5 half as fast, 2 twice as fast. 1 when not above 0. */
	float animation_speed;
} ZConfig;


typedef enum ZPointerEvent {
	Z_POINTER_DOWN = 0x0500,
	Z_POINTER_UP,
	Z_POINTER_MOVE,
	/** The pointer left the screen (a finger lifted without an up, a window left). */
	Z_POINTER_LEAVE
} ZPointerEvent;

typedef enum ZKey {
	Z_KEY_NONE = 0,
	Z_KEY_BACKSPACE = 0x0600,
	Z_KEY_TAB,
	Z_KEY_ENTER,
	Z_KEY_ESCAPE,
	Z_KEY_SPACE,
	Z_KEY_PAGE_UP,
	Z_KEY_PAGE_DOWN,
	Z_KEY_END,
	Z_KEY_HOME,
	Z_KEY_LEFT,
	Z_KEY_UP,
	Z_KEY_RIGHT,
	Z_KEY_DOWN,
	Z_KEY_DELETE,
	Z_KEY_A,
	Z_KEY_C,
	Z_KEY_V,
	Z_KEY_X
} ZKey;

typedef enum ZModifier {
	Z_MOD_SHIFT = 1,
	Z_MOD_CTRL = 2,
	Z_MOD_ALT = 4
} ZModifier;

/** A frame: 8 bit RGBA, rows top to bottom, `stride` bytes per row. Owned by the root, valid until the next render. */
typedef struct ZFrame {
	const uint8_t* pixels;
	int width;
	int height;
	int stride;
} ZFrame;


#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Creates a root. Its size comes from the window (Zwindow_run sizes it by what the compositor
	 * gives, 480 x 800 until the first configure); a program without a window sets it with Zroot_set_size.
	 * Dark theme, Cobalt accent as the system default of a Windows Phone 8 test device.
	 */
	ZRNUI_API Zid Zroot_create(void);

	/** @brief The defaults: every field zero. */
	ZRNUI_API void Zconfig_default(ZConfig* config);

	/**
	 * @brief The defaults overridden by the environment: $ZRNUI_FONT_DIR, $ZRNUI_THEME, $ZRNUI_RENDERER (auto|software|gpu),
	 * $ZRNUI_ANIMATIONS (on|off), $ZRNUI_ANIMATION_SPEED. The strings point into the environment. This is the only place
	 * where the library reads it, and only Zinit calls it.
	 */
	ZRNUI_API void Zconfig_from_environment(ZConfig* config);

	/** @brief Starts the library with `config` (NULL: the defaults). Same nesting rules as Zinit. */
	ZRNUI_API int Zinit_ex(const ZConfig* config);
	ZRNUI_API void Zroot_destroy(Zid root);
	ZRNUI_API int Zroot_is_valid(Zid root);

	/** @brief The tree shown. The root owns it; the previous visual is destroyed. */
	ZRNUI_API void Zroot_set_visual(Zid root, Zid visual);
	ZRNUI_API Zid Zroot_get_visual(Zid root);

	ZRNUI_API void Zroot_set_size(Zid root, int width, int height);

	/**
	 * @brief Which renderer draws the root. Z_RENDERER_AUTO (the default; ZConfig.renderer, $ZRNUI_RENDERER=auto|software|gpu with Zinit
	 * sets it for a new root) lets the window use OpenGL ES 2 when it can and Zroot_render use the software
	 * painter; Z_RENDERER_SOFTWARE never touches the GPU (the safe mode: no animations that need one);
	 * Z_RENDERER_GPU draws Zroot_render frames on an off-screen OpenGL context, and Zwindow_run fails
	 * when there is none. Choose before the first Zroot_render / Zwindow_run.
	 */
	ZRNUI_API void Zroot_set_renderer(Zid root, ZRenderer renderer);
	ZRNUI_API ZRenderer Zroot_get_renderer(Zid root);

	/**
	 * @brief Whether animations run. Z_ANIMATIONS_AUTO (the default; ZConfig.animations, $ZRNUI_ANIMATIONS=on|off|auto with Zinit) runs them
	 * when the frames go through the OpenGL renderer and leaves them off in the software renderer, the safe mode,
	 * where every change is at once: the page a Frame shows, the pressed look, a dropped list.
	 */
	ZRNUI_API void Zroot_set_animations(Zid root, ZAnimations mode);
	ZRNUI_API ZAnimations Zroot_get_animations(Zid root);
	/** @return true when animations run now: the mode, and for AUTO the renderer, decide. */
	ZRNUI_API int Zroot_animations_enabled(Zid root);
	/** @return "software" or "gles2": the renderer of the last frame. */
	ZRNUI_API const char* Zroot_get_renderer_name(Zid root);
	/**
	 * @brief The Back button: when the visual of the root is a Frame that can go back, goes back.
	 * false on the first page (or with no Frame): on Windows Phone Back there leaves the app, so the window quits.
	 * Escape does this in Zroot_inject_key.
	 */
	ZRNUI_API int Zroot_back(Zid root);
	/**
	 * @brief The UI scale: the layout grid is the window divided by it, the painter draws the grid
	 * back up (at 720 x 1280 and a scale of 1.5 the page is laid out in the 480 x 800 grid and
	 * comes out one and a half times as big). The scale belongs to the shell of a phone;
	 * a window sets it from the preferred_scale of the compositor (ZRNUI_SCALE overrides it
	 * only as a debug override), not from the app.
	 */
	ZRNUI_API void Zroot_set_scale(Zid root, float scale);

	/**
	 * Theme. Dark: white on black. Light: black on white. The accent colors
	 * the pressed buttons, the sliders, the progress bars and the selected
	 * items. Everything that was not given a color of its own follows.
	 */
	ZRNUI_API void Zroot_set_theme(Zid root, ZTheme theme);
	ZRNUI_API ZTheme Zroot_get_theme(Zid root);
	ZRNUI_API void Zroot_set_accent(Zid root, ZAccent accent);
	ZRNUI_API void Zroot_set_accent_color(Zid root, ZColor color);
	ZRNUI_API ZColor Zroot_get_accent_color(Zid root);
	/** @brief The color behind the tree: PhoneBackgroundBrush of the theme unless set. */
	ZRNUI_API void Zroot_set_background(Zid root, ZColor color);
	/**
	 * ThemeResource (themeresources.xaml of Windows Phone 8.1, zrnui_resources.h): the value of a key for the theme
	 * (Dark / Light) and the accent of the root, or the override the application set. A key of another type, an
	 * unknown key or an invalid root gives the zero value: transparent black, 0, 0, zeros, Z_FONT_NORMAL.
	 * Brushes are colors (Z_RES_PHONE_FOREGROUND_BRUSH); those that are the accent follow Zroot_set_accent. Font weights are Ints.
	 */
	ZRNUI_API ZColor Zroot_get_theme_color(Zid root, ZThemeResource key);
	ZRNUI_API float Zroot_get_theme_double(Zid root, ZThemeResource key);
	ZRNUI_API int Zroot_get_theme_int(Zid root, ZThemeResource key);
	ZRNUI_API ZThickness Zroot_get_theme_thickness(Zid root, ZThemeResource key);
	ZRNUI_API ZFontFamily Zroot_get_theme_font_family(Zid root, ZThemeResource key);

	/**
	 * @brief Overrides a key for the root, in both themes: the override wins over the theme until Zroot_clear_resource.
	 * A key of the theme keeps its type (an override of another type is refused); a key from Z_RES_USER_BASE up is the
	 * application's own and takes the type of the first set (or a set after a clear). Elements bound to the key follow.
	 * @return false for an invalid root, a key that is no key or the wrong type.
	 */
	ZRNUI_API int Zroot_set_resource_color(Zid root, ZThemeResource key, ZColor value);
	ZRNUI_API int Zroot_set_resource_double(Zid root, ZThemeResource key, float value);
	ZRNUI_API int Zroot_set_resource_int(Zid root, ZThemeResource key, int value);
	ZRNUI_API int Zroot_set_resource_thickness(Zid root, ZThemeResource key, ZThickness value);
	ZRNUI_API int Zroot_set_resource_font_family(Zid root, ZThemeResource key, ZFontFamily value);
	/** @brief Removes the override of a key: the theme's value is back (a user key is gone). */
	ZRNUI_API void Zroot_clear_resource(Zid root, ZThemeResource key);

	/** @brief Measure and Arrange of the whole tree if anything changed. */
	ZRNUI_API void Zroot_update_layout(Zid root);

	/**
	 * @brief Draws the tree. @return true when something changed since the
	 * last frame (and the first time): `frame` holds a new picture. false
	 * when the last picture is still right; `frame` is filled all the same.
	 * `frame` may be NULL.
	 */
	ZRNUI_API int Zroot_render(Zid root, ZFrame* frame);

	/** @brief Advances time: RepeatButton repeats, caret blink, indeterminate progress, tooltip delay. */
	ZRNUI_API void Zroot_tick(Zid root, float delta_ms);

	/** @brief A pointer or a finger at a point of the root, in logical pixels. @return true when an element handled it. */
	ZRNUI_API int Zroot_inject_pointer(Zid root, ZPointerEvent event, float x, float y);
	/** @brief The wheel at the last pointer position; positive scrolls up. */
	ZRNUI_API int Zroot_inject_wheel(Zid root, float delta);
	ZRNUI_API int Zroot_inject_key(Zid root, ZKey key, int modifiers, int pressed);
	/** @brief Typed text, UTF-8, to the focused element. */
	ZRNUI_API int Zroot_inject_text(Zid root, const char* text);

	ZRNUI_API Zid Zroot_get_focus(Zid root);
	/** @brief The element under the pointer, or Z_INVALID_ID. */
	ZRNUI_API Zid Zroot_get_pointer_over(Zid root);
	/** @brief Moves the focus to the next / previous tab stop (Tab, Shift+Tab). */
	ZRNUI_API void Zroot_move_focus(Zid root, int forward);

#ifdef __cplusplus
}
#endif

#endif
