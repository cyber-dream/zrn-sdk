/**
 * zrnui - internal.h
 * Structures shared by the sources. Nothing here is public.
 *
 * Every class is a struct that embeds the struct of its base class as its
 * first member, so a pointer to a ListBox is also a pointer to a Selector,
 * an ItemsControl, a Control and an element. Behavior is in a zrn_class_t
 * (a vtable); a class that leaves a slot empty inherits it from its base.
 */

#ifndef ZST_INTERNAL_H
#define ZST_INTERNAL_H

#include "zrnui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZST_UNUSED(x) ((void)(x))
static constexpr float ZST_PI = 3.14159265358979f;

/**
 * The safe area: what the status bar of the shell covers at the top of the screen, in logical
 * pixels, where the first element of a page starts (the 25 px of a Windows Phone page). A constant
 * for now; it is going to come from the shell, and change while the app runs.
 */
static constexpr float ZST_SAFE_AREA_TOP = 32.0f;

typedef struct zrn_element zrn_element_t;
typedef struct zrn_class zrn_class_t;
typedef struct zrn_painter zrn_painter_t;
typedef struct zrn_root zrn_root_t;
typedef struct zrn_image zrn_image_t;


/* ============================================================================
 *  small helpers
 * ============================================================================ */

static inline float zrn_minf(const float a, const float b) { return a < b ? a : b; }
static inline float zrn_maxf(const float a, const float b) { return a > b ? a : b; }
static inline float zrn_clampf(const float v, const float lo, const float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static inline int zrn_mini(const int a, const int b) { return a < b ? a : b; }
static inline int zrn_maxi(const int a, const int b) { return a > b ? a : b; }
static inline ZSize zrn_size(const float w, const float h) { const ZSize s = { .width = w, .height = h }; return s; }
static inline ZRect zrn_rect(const float x, const float y, const float w, const float h) {
 const ZRect r = { .x = x, .y = y, .width = w, .height = h }; return r; }
static inline ZColor zrn_color(const uint32_t argb) { return Zcolor_argb(argb); }
static inline ZColor zrn_color_alpha(ZColor c, const float a) { c.a *= a; return c; }
static inline ZThickness zrn_thickness_zero(void) { const ZThickness t = { 0, 0, 0, 0 }; return t; }
static inline float zrn_thickness_h(const ZThickness t) { return t.left + t.right; }
static inline float zrn_thickness_v(const ZThickness t) { return t.top + t.bottom; }

char* zrn_strdup(const char* s);
/** What the library was started with (Zinit_ex): the environment is read only by Zconfig_from_environment. */
const ZConfig* zrn_config(void);
void zrn_theme_reset(void);
void zrn_set_string(char** dest, const char* s);
/** Reports an error through Zset_error_callback and remembers it. */
[[gnu::format(printf, 2, 3)]] void zrn_error(ZError error, const char* fmt, ...);


/* ============================================================================
 *  input
 * ============================================================================ */

typedef enum zrn_input_type {
	ZST_IN_POINTER_DOWN = 1,
	ZST_IN_POINTER_UP,
	ZST_IN_POINTER_MOVE,
	ZST_IN_POINTER_ENTER,
	ZST_IN_POINTER_LEAVE,
	ZST_IN_WHEEL,
	ZST_IN_KEY_DOWN,
	ZST_IN_KEY_UP,
	ZST_IN_TEXT,
	ZST_IN_FOCUS_IN,
	ZST_IN_FOCUS_OUT,
	ZST_IN_CAPTURE_LOST,
	/** Sent to ancestors that observe the pointer before the target sees the event; they may take the capture (a scroll viewer taking a drag from a button). */
	ZST_IN_PREVIEW_DOWN,
	ZST_IN_PREVIEW_MOVE,
	ZST_IN_PREVIEW_UP
} zrn_input_type_t;

typedef struct zrn_input {
	zrn_input_type_t type;
	/** Pointer position in the coordinates of the element receiving it. */
	float x, y;
	int key;
	int mods;
	const char* text;
	float delta;
} zrn_input_t;


/* ============================================================================
 *  classes
 * ============================================================================ */

struct zrn_class {
	const zrn_class_t* base;
	const char* name;
	ZClass id;
	size_t size;
	int abstract_class;
	int resolved;

	/** Called base first, then derived. */
	void (*init)(zrn_element_t* e);
	/** Called derived first, then base. */
	void (*destroy)(zrn_element_t* e);

	/**
	 * Measure and arrange overrides. `available` has the margin removed and
	 * Width / Height / Min / Max applied; return the content size.
	 * `final` of arrange is the size the element will have; return the size used.
	 */
	ZSize (*measure)(zrn_element_t* e, ZSize available);
	ZSize (*arrange)(zrn_element_t* e, ZSize final);
	/** Draws the element itself in its own coordinates; the children are drawn after. */
	void (*paint)(zrn_element_t* e, zrn_painter_t* p);
	/** Draws over the children (scroll bars, focus marks). */
	void (*paint_over)(zrn_element_t* e, zrn_painter_t* p);
	/** Returns true when the input was handled and must not bubble. */
	int (*input)(zrn_element_t* e, zrn_input_t* in);
	void (*tick)(zrn_element_t* e, float delta_ms);
	/** A child element is about to be detached: drop pointers to it. */
	void (*child_removed)(zrn_element_t* e, zrn_element_t* child);
	/** Whether the element itself (not its children) catches the pointer. Default: yes. */
	int (*hit_self)(zrn_element_t* e);

	/** Extra room taken inside the element by the chrome a control draws (a Button's touch target). */
	ZThickness (*chrome)(zrn_element_t* e);
	/** What a TextBlock inside this control inherits. */
	ZColor (*content_foreground)(zrn_element_t* e);
	float (*content_font_size)(zrn_element_t* e);
	ZFontFamily (*content_font_family)(zrn_element_t* e);
};

const zrn_class_t* zrn_class_of(ZClass id);
int zrn_class_derives(const zrn_class_t* cls, const zrn_class_t* base);


/* ============================================================================
 *  element
 * ============================================================================ */

#define ZST_HANDLER_COUNT (Z_EVENT_LAST_ - Z_EVENT_CLICK)

typedef struct zrn_handler {
	ZRoutedCallback callback;
	void* user_data;
} zrn_handler_t;

struct zrn_element {
	Zid id;
	const zrn_class_t* cls;
	zrn_root_t* root;
	zrn_element_t* parent;
	zrn_element_t** children;
	int child_count;
	int child_cap;

	char* name;
	void* tag;
	char* tooltip;

	/* FrameworkElement */
	float width, height;
	float min_width, min_height, max_width, max_height;
	ZThickness margin;
	ZHorizontalAlignment halign;
	ZVerticalAlignment valign;
	ZVisibility visibility;
	float opacity;
	int enabled;
	int hit_visible;
	int clip_to_bounds;

	/* attached properties */
	int grid_row, grid_column, grid_row_span, grid_column_span;
	float canvas_left, canvas_top;
	int zindex;

	/* layout results */
	ZSize desired;
	/** Margin excluded, in the coordinates of the parent. */
	ZRect rect;
	ZSize last_available;
	int measure_valid;

	/* state */
	int pointer_over;
	int focusable;
	int tab_stop;
	int tab_index;
	int wants_tick;
	int observes_pointer;

	/* render transform (anim.c): rotation in degrees about the point (proj_cx, proj_cy) of the element, as fractions of its size */
	float rot_x, rot_y, rot_z, proj_cx, proj_cy, render_scale, translate_x, translate_y;
	/** true when any of the above is not the identity. */
	int transformed;
	int tilt;
	/** The ZTransition that plays when the element appears (Zelement_set_transition), after `appear_delay` ms. */
	int appear;
	float appear_delay;
	/** A ZTransition is running: Z_EVENT_TRANSITION_COMPLETED comes when its last animation ends. */
	int transition_active;
	/** A feathered transition moved the rows below this element (Zelement_clear_transition puts them back). */
	int transition_rows;
	/** The opacity the element had before a transition faded it (0: none is remembered). */
	float trans_opacity;
	/** Animations running on this element: one slot per ZAnimatedProperty, NULL when it has none. */
	struct zrn_tween* tweens;
	/** Laid out and painted by its parent itself (a popup's content, a Viewbox's child), not by the generic walk; not hit tested through the tree. */
	int is_popup_child;
	/** Set when zrn_destroy starts: what an element does as it goes must not destroy what is already going. */
	int destroying;

	zrn_handler_t handlers[ZST_HANDLER_COUNT];

	/** Properties bound to a ThemeResource (resources.c): a small list, NULL when there is none. */
	struct zrn_binding* bindings;
	int binding_count;
};

/* a few kinds of data hang off elements of the same names as their classes */

typedef struct zrn_panel {
	zrn_element_t base;
	ZColor background;
	int has_background;
} zrn_panel_t;

typedef struct zrn_stack_panel {
	zrn_panel_t base;
	ZOrientation orientation;
	int virtualizing;
} zrn_stack_panel_t;

typedef struct zrn_wrap_panel {
	zrn_panel_t base;
	ZOrientation orientation;
	float item_width, item_height;
} zrn_wrap_panel_t;

typedef struct zrn_grid_def {
	ZGridUnitType unit;
	float value;
	float min, max;
	/** Measured then arranged size. */
	float size;
} zrn_grid_def_t;

typedef struct zrn_grid {
	zrn_panel_t base;
	zrn_grid_def_t* columns;
	int column_count;
	zrn_grid_def_t* rows;
	int row_count;
} zrn_grid_t;

typedef struct zrn_border {
	zrn_element_t base;
	ZColor background, border_brush;
	int has_background, has_border_brush;
	ZThickness border_thickness, padding;
	ZCornerRadius corner_radius;
} zrn_border_t;

typedef struct zrn_viewbox {
	zrn_element_t base;
	ZStretch stretch;
	ZStretchDirection direction;
	float scale_x, scale_y;
} zrn_viewbox_t;

typedef struct zrn_text_block {
	zrn_element_t base;
	char* text;
	float font_size;
	ZFontFamily font_family;
	ZColor foreground;
	unsigned set_flags;
	/** A foreground that follows the theme: 0 none, 1 subtle, 2 contrast, 3 accent. */
	int fg_role;
	ZTextWrapping wrapping;
	ZTextAlignment alignment;
	ZTextTrimming trimming;
	int max_lines;
	float line_height;
	int character_spacing;
	/* cache of the last layout */
	void* layout;
	float layout_width;
} zrn_text_block_t;

enum {
	ZST_TB_FONT_SIZE = 1,
	ZST_TB_FONT_FAMILY = 2,
	ZST_TB_FOREGROUND = 4
};

struct zrn_image {
	int width, height;
	uint8_t* rgba;
	/** The texture of the OpenGL renderer that has drawn it (render/gles.c); 0 and NULL otherwise. */
	unsigned gl_texture;
	void* gl_owner;
};

typedef struct zrn_image_element {
	zrn_element_t base;
	zrn_image_t* image;
	ZStretch stretch;
} zrn_image_element_t;

typedef struct zrn_popup {
	zrn_element_t base;
	int is_open;
	int stays_open;
	float h_offset, v_offset;
	Zid target;
	ZPlacementMode placement;
	int placed;
	/** A modal popup (a ContentDialog) dims what is under it and blocks input to it. */
	int modal;
	/** As wide as the root, whatever its child asks for (a flyout on the phone). */
	int full_width;
} zrn_popup_t;


/* ---- Control and below ---- */

enum {
	ZST_C_FOREGROUND = 1,
	ZST_C_BACKGROUND = 2,
	ZST_C_BORDER_BRUSH = 4,
	ZST_C_BORDER_THICKNESS = 8,
	ZST_C_PADDING = 16,
	ZST_C_FONT_SIZE = 32,
	ZST_C_FONT_FAMILY = 64
};

typedef struct zrn_control {
	zrn_element_t base;
	ZColor foreground, background, border_brush;
	unsigned set_flags;
	ZThickness border_thickness, padding;
	float font_size;
	ZFontFamily font_family;
	ZHorizontalAlignment h_content;
	ZVerticalAlignment v_content;
} zrn_control_t;

typedef struct zrn_content_control {
	zrn_control_t base;
	zrn_element_t* content;
	/** true when `content` is the TextBlock made for a string. */
	int content_is_text;
} zrn_content_control_t;

/** A page a Frame shows (frame.c): the navigation parameter it was created for. */
typedef struct zrn_page {
	zrn_content_control_t base;
	void* parameter;
	/** How the frame reached the page now: what OnNavigatedTo and OnNavigatedFrom are told. */
	ZNavigationMode navigation_mode;
	ZNavigationCacheMode cache_mode;
	/** Page.TopAppBar and BottomAppBar: children of the page after its content. */
	zrn_element_t* top_bar;
	zrn_element_t* bottom_bar;
	/** The index in the history of the entry the page was built for. */
	int entry_index;
} zrn_page_t;

typedef struct zrn_frame_entry {
	ZPageFactory factory;
	void* parameter;
	/** The page of the entry, alive and out of the frame, when its NavigationCacheMode keeps it. */
	Zid cached;
} zrn_frame_entry_t;

/** A Frame: the navigation history, `current` is the entry shown. */
typedef struct zrn_frame {
	zrn_content_control_t base;
	zrn_frame_entry_t* entries;
	int count, capacity, current;
	/** true while the callbacks of a navigation run; another Navigate from them is refused. */
	int navigating;
	/** The transition (frame.c): 1 the old page turns away, 2 the new one swings in, 0 none. */
	ZPageTransition transition;
	/** Zframe_set_transitions: the transitions played on the pages themselves, [forward out, forward in, backward out, backward in], instead of `transition`. */
	int custom;
	ZTransition nav[4];
	/** The run in progress plays `nav` (not the feathered turnstile), and how long its out and in halves last, in ms. */
	int run_custom;
	float out_ms, in_ms;
	/** The first page went in before it was known whether animations run (the platform makes its surface later): Zroot_start_intro starts its swing-in. */
	int intro_pending;
	int phase, direction;
	/** The mode of the navigation in progress (the page that leaves is told it too). */
	ZNavigationMode mode;
	float elapsed;
	/** The new page, built but not yet in the frame, during phase 1. */
	Zid pending;
	void* pending_parameter;
	/** The rows being turned, and the opacity each had. */
	Zid* rows;
	float* row_opacity;
	int row_count, row_cap;
} zrn_frame_t;

typedef struct zrn_button_base {
	zrn_content_control_t base;
	ZClickMode click_mode;
	int pressed;
	int pointer_over;
	int keyboard_pressed;
	/** RepeatButton. */
	int repeat;
	/** RepeatButton.Delay and Interval, in ms. */
	int delay_ms, interval_ms;
	float repeat_timer;
	int repeating;
	/** HyperlinkButton. */
	char* uri;
	/** Button.Flyout: a child of the button, shown by a tap. */
	zrn_element_t* flyout;
} zrn_button_base_t;

typedef struct zrn_toggle_button {
	zrn_button_base_t base;
	ZToggleState state;
	int three_state;
	char* group_name;
} zrn_toggle_button_t;

typedef struct zrn_range_base {
	zrn_control_t base;
	double minimum, maximum, value;
	double small_change, large_change;
	ZValueChangedCallback changed;
	void* changed_data;
} zrn_range_base_t;

typedef struct zrn_slider {
	zrn_range_base_t base;
	ZOrientation orientation;
	int dragging;
	char* header;
	double step_frequency, tick_frequency;
	ZTickPlacement tick_placement;
	ZSliderSnapsTo snaps_to;
	int direction_reversed;
} zrn_slider_t;

typedef struct zrn_progress_bar {
	zrn_range_base_t base;
	int indeterminate;
	int show_error, show_paused;
	/** The clock of the indeterminate dots, in ms. */
	float time_ms;
} zrn_progress_bar_t;

/** ToggleSwitch (buttons.c): IsOn, the header and the two contents, and where the knob is. */
typedef struct zrn_toggle_switch {
	zrn_control_t base;
	int is_on;
	char *header, *on_content, *off_content;
	/** The knob and the curtain: 0 is off, 1 is on; `knob_goal` is where they are going. */
	float knob, knob_goal, knob_from, knob_elapsed;
	int knob_animating;
	int pressed, dragging, moved;
	float press_x, grab;
} zrn_toggle_switch_t;

/** ProgressRing (range.c): the dots run while IsActive. */
typedef struct zrn_progress_ring {
	zrn_control_t base;
	int is_active;
	float time_ms;
} zrn_progress_ring_t;

typedef struct zrn_scrollbar {
	zrn_range_base_t base;
	ZOrientation orientation;
	double viewport;
	int dragging;
	float drag_grab;
	ZScrollCallback on_scroll;
	void* on_scroll_data;
} zrn_scrollbar_t;

typedef struct zrn_thumb {
	zrn_control_t base;
	int dragging;
	float last_x, last_y;
	float total_x, total_y;
	ZDragCallback started, delta, completed;
	void *started_data, *delta_data, *completed_data;
} zrn_thumb_t;

typedef struct zrn_scroll_viewer {
	zrn_content_control_t base;
	ZScrollBarVisibility v_visibility, h_visibility;
	double offset_x, offset_y;
	double extent_w, extent_h, viewport_w, viewport_h;
	int show_v, show_h;
	/** Seconds the bars stay visible after a scroll (WP8 shows them only while scrolling). */
	float bar_visible;
	int dragging;
	float drag_start_y, drag_start_x;
	double drag_offset_y, drag_offset_x;
	int drag_moved;
	/** Velocity of a flick, px/s. */
	float velocity_x, velocity_y;
	float last_move_x, last_move_y;
	ZScrollCallback on_scroll;
	void* on_scroll_data;
} zrn_scroll_viewer_t;

typedef struct zrn_text_box {
	zrn_control_t base;
	char* text;
	int length_bytes;
	int caret;
	int anchor;
	int accepts_return;
	ZTextWrapping wrapping;
	int read_only;
	int max_length;
	char* placeholder;
	char* header;
	int password;
	uint32_t password_char;
	float caret_blink;
	float scroll_x, scroll_y;
	int selecting;
	/** Set while a program changes the text, to not fire TextChanged twice. */
	int quiet;
	/** true while TextChanged is raised for a change the user typed. */
	int from_user;
	/** Called first for a key (an AutoSuggestBox takes Up, Down, Enter and Escape); true when it took the key. */
	int (*key_hook)(struct zrn_text_box* box, zrn_input_t* in);
} zrn_text_box_t;

/** The selection modes of the items controls: ListBox and ListView have all four (a ListBox has no None). */
typedef enum zrn_sel_mode {
	ZST_SEL_NONE = 0,
	ZST_SEL_SINGLE,
	ZST_SEL_MULTIPLE,
	ZST_SEL_EXTENDED
} zrn_sel_mode_t;

/** SelectorItem (items.c): a container of an item, ListBoxItem, ComboBoxItem, ListViewItem, GridViewItem, FlipViewItem. */
typedef struct zrn_selector_item {
	zrn_content_control_t base;
	int selected;
	int pressed;
	float press_x, press_y;
} zrn_selector_item_t;

/**
 * ItemsControl and Selector (items.c). The items are containers in `host`, the items panel, with the header
 * items of the groups between them (only a ListView or a GridView has any). `scroll` is the ScrollViewer
 * a ListBox, a ListView and a GridView show the panel in; `presenter` is what it holds: the header, the panel and the footer.
 */
typedef struct zrn_items_control {
	zrn_control_t base;
	zrn_element_t* scroll;
	zrn_element_t* host;
	zrn_element_t* presenter;
	int selected_index;
	zrn_sel_mode_t sel_mode;
	/** The item the keyboard is on: its container, Z_INVALID_ID for none. */
	Zid focus_item;
	/** The keyboard moved it there: a tap gives the list the focus without showing where. */
	int focus_visible;
	/** The header items of the groups in `host`: when 0 an item index is the index of its child. */
	int group_count;
} zrn_items_control_t;

/** ListViewBase (listview.c). */
typedef struct zrn_list_view_base {
	zrn_items_control_t base;
	int item_click_enabled;
	int swipe_enabled;
	Zid header, footer;
	ZItemClickCallback on_item_click;
	void* on_item_click_data;
} zrn_list_view_base_t;

/** ComboBox (combo.c). */
typedef struct zrn_combo_box {
	zrn_items_control_t base;
	zrn_element_t* popup;
	int is_open;
	int pressed;
	float max_drop_down_height;
	char* header;
	char* placeholder;
} zrn_combo_box_t;

/** FlipView (flip.c): the items are pages side by side that slide as the pivot's do. */
typedef struct zrn_flip_view {
	zrn_items_control_t base;
	float offset, from, to, elapsed;
	int sliding, pressed, dragging;
	float press_x, press_y, last_x, velocity;
	int touch_animations_for_all;
	/** true while a finger or a key changes the selection. */
	int user_nav;
} zrn_flip_view_t;

/** AutoSuggestBox (combo.c): a TextBox and a popup with a ListView of the suggestions. */
typedef struct zrn_auto_suggest_box {
	zrn_items_control_t base;
	zrn_element_t* text_box;
	zrn_element_t* popup;
	zrn_element_t* list;
	int is_open;
	int update_text_on_select;
	float max_suggestion_list_height;
	/** The suggestion the keyboard is on, -1 for none. */
	int highlight;
	/** Set while the text is changed by the program or a suggestion. */
	ZAutoSuggestionBoxTextChangeReason reason;
	int reason_set;
	ZAutoSuggestTextChangedCallback on_text_changed;
	void* on_text_changed_data;
	ZAutoSuggestSuggestionChosenCallback on_suggestion_chosen;
	void* on_suggestion_chosen_data;
	ZAutoSuggestQuerySubmittedCallback on_query_submitted;
	void* on_query_submitted_data;
} zrn_auto_suggest_box_t;

/** ItemsWrapGrid (panels.c). */
typedef struct zrn_items_wrap_grid {
	zrn_panel_t base;
	ZOrientation orientation;
	int max_rows_or_columns;
	float item_width, item_height;
} zrn_items_wrap_grid_t;


/* ---- icons, app bars and flyouts (appbar.c, flyout.c) ---- */

/** IconElement: Foreground when the program sets one, else the one of the control the icon is in. */
typedef struct zrn_icon {
	zrn_element_t base;
	ZColor foreground;
	int has_foreground;
	/** SymbolIcon. */
	ZSymbol symbol;
	/** FontIcon. */
	char* glyph;
	float font_size;
	ZFontFamily family;
	/** PathIcon: closed polygons, each as a run of points of `path`, `path_runs` holds their lengths. */
	float* path;
	int* path_runs;
	int path_run_count;
	float path_bounds[4];
	/** BitmapIcon. */
	zrn_image_t* image;
} zrn_icon_t;

/** AppBar: closed it is as tall as ClosedDisplayMode says, open as tall as its content (a CommandBar: its commands with their labels, and the secondary ones above). */
typedef struct zrn_app_bar {
	zrn_content_control_t base;
	int is_open, is_sticky;
	ZAppBarClosedDisplayMode closed_mode;
	/** The opening animation: 0 closed, 1 open. */
	float open_t;
	/** Set by the Page that shows it as its TopAppBar. */
	int on_top;
	/** A press began on the three dots. */
	int ellipsis_pressed;
} zrn_app_bar_t;

typedef struct zrn_command_bar {
	zrn_app_bar_t base;
	zrn_element_t** primary;
	int primary_count, primary_cap;
	zrn_element_t** secondary;
	int secondary_count, secondary_cap;
} zrn_command_bar_t;

/** What AppBarButton and AppBarToggleButton add to their base. */
typedef struct zrn_app_bar_item {
	char* label;
	int is_compact;
	/** A secondary command: a row of text in the open menu. */
	int overflow;
} zrn_app_bar_item_t;

typedef struct zrn_app_bar_button {
	zrn_button_base_t base;
	zrn_app_bar_item_t item;
} zrn_app_bar_button_t;

typedef struct zrn_app_bar_toggle_button {
	zrn_toggle_button_t base;
	zrn_app_bar_item_t item;
} zrn_app_bar_toggle_button_t;

/** FlyoutBase: the Popup it shows in is its one child; the presenter is the child of the popup. */
typedef struct zrn_flyout_base {
	zrn_element_t base;
	zrn_element_t* popup;
	ZFlyoutPlacementMode placement;
	/** The element it was shown at. */
	Zid target;
	int is_open;
	/** What a flyout class does when it is about to show, and when it has been dismissed (pickers.c): called before Opening and before Closed. */
	void (*on_show)(zrn_element_t* flyout);
	void (*on_hide)(zrn_element_t* flyout);
} zrn_flyout_base_t;

typedef struct zrn_menu_flyout_item {
	zrn_control_t base;
	char* text;
	int pressed;
	/** ToggleMenuFlyoutItem. */
	int is_checked;
} zrn_menu_flyout_item_t;

typedef struct zrn_content_dialog {
	zrn_content_control_t base;
	zrn_element_t *title, *button1, *button2;
	char *primary_text, *secondary_text;
	int primary_enabled, secondary_enabled;
	int is_open;
	/** The popup that holds the dialog (its parent) once it has been shown. */
	Zid popup;
	Zid previous_focus;
	ZContentDialogResult result;
	ZContentDialogResultCallback callback;
	void* callback_data;
} zrn_content_dialog_t;

#define ZST_AS(type, e) ((type*)(e))


/* ============================================================================
 *  handles and elements (core.c)
 * ============================================================================ */

/** The handle table holds elements and roots. */
Zid zrn_handle_alloc(void* ptr, int is_root);
void zrn_handle_free(Zid id);
void* zrn_handle_get(Zid id, int is_root);
zrn_element_t* zrn_lookup(Zid id);
/** Children in paint order (Panel.ZIndex, then insertion). Free with zrn_ordered_free. */
zrn_element_t** zrn_ordered(zrn_element_t* e, int* count, int* heap);
void zrn_ordered_free(zrn_element_t** list, int heap);
/** Looks an element up and checks its class; reports and returns NULL on failure. */
zrn_element_t* zrn_get(Zid id, ZClass cls);
zrn_element_t* zrn_create(ZClass id);
void zrn_destroy(zrn_element_t* e);
int zrn_is_a(const zrn_element_t* e, ZClass id);
void zrn_add_child(zrn_element_t* parent, zrn_element_t* child, int index);
void zrn_remove_child(zrn_element_t* parent, zrn_element_t* child);
void zrn_clear_children(zrn_element_t* parent);
/** Replaces the one child of a single-child element: destroys the old one. */
void zrn_set_only_child(zrn_element_t* parent, zrn_element_t** slot, zrn_element_t* child);
int zrn_child_index(const zrn_element_t* parent, const zrn_element_t* child);

/* layout */
void zrn_measure(zrn_element_t* e, ZSize available);
void zrn_arrange(zrn_element_t* e, ZRect slot);
ZSize zrn_measure_children_max(zrn_element_t* e, ZSize available);
void zrn_changed_layout(zrn_element_t* e);
void zrn_changed_paint(zrn_element_t* e);
/** Arrange again without measuring again (a scroll offset changed). */
void zrn_changed_arrange(zrn_element_t* e);
int zrn_effectively_enabled(const zrn_element_t* e);
int zrn_effectively_visible(const zrn_element_t* e);
ZPoint zrn_to_root(const zrn_element_t* e, ZPoint p);
ZPoint zrn_from_root(const zrn_element_t* e, ZPoint p);
zrn_element_t* zrn_hit_test(zrn_element_t* e, ZPoint p);
/** Calls the handler of an event. @return false when the handler destroyed the element: the caller must then touch nothing of it. */
int zrn_raise(zrn_element_t* e, ZEvent ev);
/** A change of an inherited property (font) can change the measure of everything below. */
void zrn_invalidate_deep(zrn_element_t* e);
/** The ids of an element and its ancestors, innermost first: safe to walk while callbacks destroy things. */
int zrn_chain(const zrn_element_t* e, Zid* ids, int max);

/** Walks up for what a TextBlock inherits. */
ZColor zrn_inherited_foreground(const zrn_element_t* e);
float zrn_inherited_font_size(const zrn_element_t* e);
ZFontFamily zrn_inherited_font_family(const zrn_element_t* e);

/** Element rectangle in its own coordinates. */
static inline ZRect zrn_local_rect(const zrn_element_t* e) { return zrn_rect(0, 0, e->rect.width, e->rect.height); }

/* ---- the classes, one per source file ---- */

extern zrn_class_t zrn_class_element, zrn_class_panel, zrn_class_stack_panel, zrn_class_virtualizing_stack_panel,
	zrn_class_wrap_panel, zrn_class_grid, zrn_class_canvas, zrn_class_border, zrn_class_viewbox, zrn_class_text_block,
	zrn_class_image, zrn_class_popup, zrn_class_control, zrn_class_user_control, zrn_class_content_control,
	zrn_class_frame, zrn_class_page, zrn_class_pivot, zrn_class_tooltip, zrn_class_button_base, zrn_class_button, zrn_class_hyperlink_button, zrn_class_repeat_button,
	zrn_class_toggle_button, zrn_class_checkbox, zrn_class_radio_button, zrn_class_range_base, zrn_class_slider,
	zrn_class_progress_bar, zrn_class_scrollbar, zrn_class_thumb, zrn_class_separator, zrn_class_scroll_viewer,
	zrn_class_text_box, zrn_class_password_box, zrn_class_toggle_switch, zrn_class_progress_ring, zrn_class_items_control, zrn_class_selector, zrn_class_list_box,
	zrn_class_list_box_item, zrn_class_combo_box, zrn_class_combo_box_item, zrn_class_auto_suggest_box, zrn_class_selector_item,
	zrn_class_list_view_base, zrn_class_list_view, zrn_class_grid_view, zrn_class_list_view_item, zrn_class_grid_view_item,
	zrn_class_list_view_base_header_item, zrn_class_list_view_header_item, zrn_class_grid_view_header_item, zrn_class_flip_view,
	zrn_class_flip_view_item, zrn_class_items_stack_panel, zrn_class_items_wrap_grid, zrn_class_semantic_zoom,
	zrn_class_icon_element, zrn_class_symbol_icon, zrn_class_font_icon, zrn_class_path_icon, zrn_class_bitmap_icon, zrn_class_app_bar,
	zrn_class_command_bar, zrn_class_app_bar_button, zrn_class_app_bar_toggle_button, zrn_class_app_bar_separator, zrn_class_flyout_base,
	zrn_class_flyout, zrn_class_menu_flyout, zrn_class_menu_flyout_item_base, zrn_class_menu_flyout_item,
	zrn_class_toggle_menu_flyout_item, zrn_class_menu_flyout_separator, zrn_class_content_dialog, zrn_class_pivot_item, zrn_class_hub,
	zrn_class_hub_section, zrn_class_looping_selector, zrn_class_date_picker, zrn_class_time_picker, zrn_class_picker_flyout_base,
	zrn_class_date_picker_flyout, zrn_class_time_picker_flyout, zrn_class_rich_text_block;

void zrn_popup_close(zrn_element_t* popup);
void zrn_popup_set_open(zrn_element_t* popup, int open);
/** Closes what the Back button closes first: a dialog, a popup, an open app bar. true when it closed something. */
int zrn_root_dismiss_top(zrn_root_t* r);
/** A page (frame.c) asks the bars it shows how tall they are closed and now, and tells them they are on top. */
float zrn_app_bar_closed_height(zrn_element_t* bar);
float zrn_app_bar_height(zrn_element_t* bar, float width);
void zrn_app_bar_set_on_top(zrn_element_t* bar, int on_top);
/** Shows the flyout of a button at it (flyout.c). */
void zrn_flyout_show_at(zrn_element_t* flyout, zrn_element_t* target);
/** A command of an app bar was clicked (appbar.c): a secondary one closes the bar. */
void zrn_app_bar_item_clicked(zrn_element_t* item);
/** items.c: what the panel of an items control needs to tell it when its orientation changes. */
void zrn_items_panel_changed(zrn_element_t* panel);

/* the parts of the items controls that the sources of the classes share (items.c) */
zrn_items_control_t* zrn_item_owner(zrn_element_t* item);
int zrn_ic_count(const zrn_items_control_t* o);
zrn_element_t* zrn_ic_item(const zrn_items_control_t* o, int index);
int zrn_ic_index_of(const zrn_items_control_t* o, const zrn_element_t* container);
zrn_element_t* zrn_ic_add(zrn_items_control_t* o, zrn_element_t* item, const char* text);
const char* zrn_container_text(zrn_element_t* container);
/** Selection by the program: the item alone (-1 none). Raises SelectionChanged when the selection changed. */
void zrn_ic_select(zrn_items_control_t* o, int index);
/** Selection by a tap on an item, as the mode says. Raises SelectionChanged. */
void zrn_ic_tap(zrn_items_control_t* o, int index);
void zrn_ic_set_sel_mode(zrn_items_control_t* o, zrn_sel_mode_t mode);
void zrn_ic_select_all(zrn_items_control_t* o);
int zrn_ic_selected_indices(const zrn_items_control_t* o, int* out, int max);
void zrn_ic_set_host(zrn_items_control_t* o, zrn_element_t* panel);
void zrn_ic_scroll_into_view(zrn_items_control_t* o, int index, ZScrollIntoViewAlignment alignment);
/** The keys of a list: the arrows, Home and End move the item the keyboard is on, Space and Enter tap it. */
int zrn_ic_key(zrn_element_t* e, zrn_input_t* in);
/** SelectorItem.IsSelected without any event, for the controls that keep it in step with a state of their own. */
void zrn_item_set_selected(zrn_element_t* item, int selected);
/** flip.c: what a FlipView does when its items or its selection change. */
void zrn_flip_items_changed(zrn_items_control_t* o);
void zrn_flip_selected(zrn_items_control_t* o);
/** The tap on an item of a ComboBox (combo.c), of a ListView or a GridView (listview.c). */
void zrn_combo_item_tapped(zrn_items_control_t* o, int index);
void zrn_lvb_item_tapped(zrn_items_control_t* o, int index);
ZSize zrn_cc_measure(zrn_element_t* e, ZSize available);
/** A content control forgets its content when it is removed: the classes that have children of their own chain to it. */
void zrn_cc_child_removed(zrn_element_t* e, zrn_element_t* child);
ZSize zrn_cc_arrange(zrn_element_t* e, ZSize final);
/** Chrome + border + padding of a content control. */
ZThickness zrn_cc_insets(zrn_element_t* e);

/* content controls (basic.c) */
void zrn_content_set(zrn_content_control_t* c, zrn_element_t* content);
void zrn_content_set_text(zrn_content_control_t* c, const char* text);
/** The effective values of a control, theme defaults filled in. */
ZColor zrn_ctl_foreground(const zrn_control_t* c);
ZColor zrn_ctl_background(const zrn_control_t* c, ZColor fallback);
ZColor zrn_ctl_border_brush(const zrn_control_t* c, ZColor fallback);
ZColor zrn_ctl_color(const zrn_control_t* c, unsigned flag, ZColor stored, ZColor fallback);


/* ============================================================================
 *  theme (theme.c)
 * ============================================================================ */

typedef enum zrn_theme_color_id {
	ZST_TC_FOREGROUND,
	ZST_TC_BACKGROUND,
	ZST_TC_CONTRAST_FOREGROUND,
	ZST_TC_CONTRAST_BACKGROUND,
	ZST_TC_DISABLED,
	ZST_TC_PROGRESS_BAR_BACKGROUND,
	ZST_TC_TEXT_BOX,
	ZST_TC_TEXT_BOX_FOREGROUND,
	ZST_TC_TEXT_BOX_EDIT_BACKGROUND,
	ZST_TC_TEXT_BOX_READ_ONLY,
	ZST_TC_TEXT_CARET,
	ZST_TC_SUBTLE,
	ZST_TC_BUTTON_PRESSED_FOREGROUND,
	ZST_TC_CHROME,
	ZST_TC_INACTIVE,
	ZST_TC_BORDER,
	ZST_TC_SEMITRANSPARENT,
	ZST_TC_ACCENT,
	/* Windows Phone 8.1 (themeresources.xaml) */
	ZST_TC_BASE_MEDIUM_HIGH,
	ZST_TC_BASE_MID,
	ZST_TC_BASE_MEDIUM_MID,
	ZST_TC_BASE_MEDIUM_MID_SOLID,
	ZST_TC_BASE_LOW,
	ZST_TC_BASE_LOW_SOLID,
	ZST_TC_TEXT_OVER_ACCENT,
	ZST_TC_TEXT_BOX_BACKGROUND_81,
	ZST_TC_TEXT_BOX_BORDER_81,
	ZST_TC_TEXT_BOX_FOREGROUND_81,
	ZST_TC_TEXT_BOX_PLACEHOLDER_81,
	ZST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81,
	ZST_TC_SLIDER_THUMB,
	ZST_TC_SLIDER_THUMB_DISABLED,
	ZST_TC_SLIDER_TRACK,
	ZST_TC_SLIDER_DECREASE_DISABLED,
	/* PhoneBaseBlackColor, PhoneBaseWhiteColor, PhoneDisabledColor, PhoneTextMidColor, PhoneTextLowColor */
	ZST_TC_BASE_BLACK,
	ZST_TC_BASE_WHITE,
	ZST_TC_PHONE_DISABLED,
	ZST_TC_TEXT_MID,
	ZST_TC_TEXT_LOW,
	/* ContentDialogDimmingColor */
	ZST_TC_CONTENT_DIALOG_DIMMING,
	/* the text of a ListBox: PhoneBaseBlack on the white 80% box of Dark; the 8.1 Light dictionary has PhoneBaseBlack on a black 75% box, unreadable, so Light uses PhoneBaseWhite */
	ZST_TC_LIST_BOX_FOREGROUND,
	ZST_TC_COUNT
} zrn_theme_color_id_t;

typedef struct zrn_theme {
	ZTheme theme;
	ZColor colors[ZST_TC_COUNT];
	ZColor accent;
	ZColor background;
	int has_background;
} zrn_theme_t;

void zrn_theme_init(zrn_theme_t* t, ZTheme theme, ZColor accent);
/** The theme of the root an element is in, or the default theme for a detached element. */
ZColor zrn_theme_color(const zrn_element_t* e, zrn_theme_color_id_t id);
ZColor zrn_accent_color(ZAccent accent);

/* WP8 sizes (themes.md) */
static constexpr float ZST_FONT_SMALL = 18.667f;
static constexpr float ZST_FONT_NORMAL = 20.0f;
static constexpr float ZST_FONT_MEDIUM = 22.667f;
static constexpr float ZST_FONT_MEDIUM_LARGE = 25.333f;
static constexpr float ZST_FONT_LARGE = 32.0f;
static constexpr float ZST_FONT_EXTRA_LARGE = 42.667f;
static constexpr float ZST_FONT_EXTRA_EXTRA_LARGE = 72.0f;
static constexpr float ZST_FONT_HUGE = 186.667f;
static constexpr float ZST_BORDER_THICKNESS = 3.0f;
static constexpr float ZST_TOUCH_TARGET_OVERHANG = 12.0f;

/* Windows Phone 8.1 (themeresources.xaml and generic.xaml), taken as they are */
static constexpr float ZST_81_BORDER_THICKNESS = 2.5f;
/** PhoneTouchTargetOverhang is 0,9.5 and PhoneTouchTargetLargeOverhang 0,16: only above and below. */
static constexpr float ZST_81_TOUCH_OVERHANG = 9.5f;
static constexpr float ZST_81_TOUCH_LARGE_OVERHANG = 16.0f;
static constexpr float ZST_81_FONT_LARGE = 18.14f;
static constexpr float ZST_81_FONT_CONTROL_HEADER = 16.0f;
static constexpr float ZST_81_FONT_CONTENT = 20.26f;
static constexpr float ZST_81_FONT_EXTRA_LARGE = 25.5f;
static constexpr float ZST_81_FONT_EXTRA_LARGE_PLUS = 29.86f;


/* ============================================================================
 *  ThemeResource (resources.c, theme_resources.c)
 * ============================================================================ */

typedef enum zrn_res_type {
	ZST_RT_NONE = 0,
	ZST_RT_COLOR,
	ZST_RT_DOUBLE,
	ZST_RT_INT,
	ZST_RT_THICKNESS,
	ZST_RT_FONT_FAMILY
} zrn_res_type_t;

typedef union zrn_res_value {
	/** A color: ARGB; of an accent color only the alpha counts. */
	uint32_t argb;
	/** A double in f[0], a thickness (left, top, right, bottom) in f. */
	float f[4];
	/** An int, a font weight, a ZFontFamily. */
	int i;
} zrn_res_value_t;

/** One generated key: the table is indexed by key - Z_RES_FIRST_. */
typedef struct zrn_res_def {
	const char* name;
	uint8_t type;
	/** 1: the Dark color is the accent of the root (with the alpha of `dark`), 2: the Light one. */
	uint8_t accent;
	zrn_res_value_t dark, light;
} zrn_res_def_t;

extern const zrn_res_def_t zrn_res_table[];

typedef struct zrn_res_override {
	ZThemeResource key;
	zrn_res_type_t type;
	union { ZColor color; float d; int i; ZThickness thickness; } v;
} zrn_res_override_t;

/** The value of a key of a root (NULL: the default theme) as the type says; false when the key has no such value. */
int zrn_res_color(const zrn_root_t* r, ZThemeResource key, ZColor* out);
int zrn_res_double(const zrn_root_t* r, ZThemeResource key, float* out);
int zrn_res_int(const zrn_root_t* r, ZThemeResource key, int* out);
int zrn_res_thickness(const zrn_root_t* r, ZThemeResource key, ZThickness* out);
int zrn_res_font_family(const zrn_root_t* r, ZThemeResource key, ZFontFamily* out);

/** The properties of an element that can follow a key. */
typedef enum zrn_bind_prop {
	ZST_BP_ELEMENT_MARGIN = 1,
	ZST_BP_TEXT_FOREGROUND,
	ZST_BP_TEXT_FONT_SIZE,
	ZST_BP_TEXT_CHARACTER_SPACING,
	ZST_BP_CONTROL_FOREGROUND,
	ZST_BP_CONTROL_BACKGROUND,
	ZST_BP_CONTROL_BORDER_BRUSH,
	ZST_BP_CONTROL_BORDER_THICKNESS,
	ZST_BP_CONTROL_PADDING,
	ZST_BP_CONTROL_FONT_SIZE,
	ZST_BP_BORDER_BACKGROUND,
	ZST_BP_BORDER_BORDER_BRUSH,
	ZST_BP_BORDER_BORDER_THICKNESS,
	ZST_BP_BORDER_PADDING
} zrn_bind_prop_t;

typedef struct zrn_binding {
	zrn_bind_prop_t prop;
	ZThemeResource key;
} zrn_binding_t;

/** Binds `prop` of `e` to `key` (replacing its binding) and applies it; false when the key is not of the type the property takes. */
int zrn_bind(zrn_element_t* e, zrn_bind_prop_t prop, ZThemeResource key);
/** A direct setter was called: the property keeps its value and stops following a key. */
void zrn_unbind(zrn_element_t* e, zrn_bind_prop_t prop);
/** Applies every binding of `e` (when it enters a root). */
void zrn_bindings_apply(zrn_element_t* e);
void zrn_bindings_free(zrn_element_t* e);
/** The theme, the accent or an override changed: every bound property of the tree of the root is resolved again. */
void zrn_root_resources_changed(zrn_root_t* r);
void zrn_root_resources_free(zrn_root_t* r);


/* ============================================================================
 *  text (text.c)
 * ============================================================================ */

typedef struct zrn_font {
	ZFontFamily family;
	float size;
	int spacing;
} zrn_font_t;

typedef struct zrn_line {
	int start, end;
	float width;
} zrn_line_t;

typedef struct zrn_text_layout {
	zrn_font_t font;
	const char* text;
	zrn_line_t* lines;
	int line_count;
	int truncated;
	float width, height, line_height;
} zrn_text_layout_t;

bool zrn_text_init(void);
void zrn_text_shutdown(void);
/** Lays text out. `max_width` <= 0 is unlimited. The text must outlive the layout. */
zrn_text_layout_t* zrn_text_layout_new(zrn_font_t font, const char* text, int length, float max_width, int wrap, int max_lines, float line_height);
void zrn_text_layout_free(zrn_text_layout_t* l);
/** true when the font, or a fallback found on the system, has the glyph. */
int zrn_font_has_glyph(zrn_font_t font, uint32_t cp);
float zrn_font_line_height(zrn_font_t font);
float zrn_font_ascent(zrn_font_t font);
float zrn_text_width(zrn_font_t font, const char* s, int length);
/** Byte offset in [0, length] nearest to x. */
int zrn_text_index_at(zrn_font_t font, const char* s, int length, float x);
int zrn_utf8_next(const char* s, int length, int index);
int zrn_utf8_prev(const char* s, int index);
uint32_t zrn_utf8_decode(const char* s, int length, int* consumed);
int zrn_utf8_encode(uint32_t cp, char* out);


/* ============================================================================
 *  painter (render/painter.c)
 * ============================================================================ */

/**
 * A renderer. Every call has the signature of the zrn_* function of the same name, coordinates
 * logical and local as the painter's tx, ty, scale, alpha and clip say. The software backend
 * (render/software.c) draws into an RGBA8 buffer; others (OpenGL) draw on the GPU.
 */
typedef struct zrn_backend {
	const char* name;
	void (*paint_clear)(zrn_painter_t* p, ZColor c);
	void (*fill_rect)(zrn_painter_t* p, ZRect r, ZColor c);
	void (*fill_rrect)(zrn_painter_t* p, ZRect r, ZCornerRadius radius, ZColor c);
	void (*fill_ellipse)(zrn_painter_t* p, ZRect r, ZColor c);
	void (*stroke_ellipse)(zrn_painter_t* p, ZRect r, float thickness, ZColor c);
	void (*fill_polygon)(zrn_painter_t* p, const float* xy, int count, ZColor c);
	void (*draw_line)(zrn_painter_t* p, float x0, float y0, float x1, float y1, float thickness, ZColor c);
	/** One glyph bitmap: 8 bit coverage at a device position. */
	void (*blit_alpha)(zrn_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, ZColor c);
	void (*draw_image)(zrn_painter_t* p, const zrn_image_t* image, ZRect dest, ZRect source);
	void (*push_clip)(zrn_painter_t* p, ZRect r);
	void (*pop_clip)(zrn_painter_t* p);
	int (*clipped_out)(const zrn_painter_t* p, ZRect r);
	/** What follows is drawn under this affine transform (column-major 4x4, device pixels), NULL for none. Backends that cannot, draw it untransformed. */
	void (*set_transform)(zrn_painter_t* p, const float* m);
} zrn_backend_t;

extern const zrn_backend_t zrn_backend_software;
extern const zrn_backend_t zrn_backend_gles;
/** The clip is state of the painter, the same for every backend. */
void zrn_clip_push(zrn_painter_t* p, ZRect r);
void zrn_clip_pop(zrn_painter_t* p);
int zrn_clip_out(const zrn_painter_t* p, ZRect r);

/* render/gles.c: an OpenGL ES 2 context, in a window or off-screen */
typedef struct zrn_gl zrn_gl_t;
int zrn_gl_available(void);
/** The size the renderer last drew: 0 before the first frame. */
int zrn_gl_width(const zrn_gl_t* gl);
int zrn_gl_height(const zrn_gl_t* gl);
zrn_gl_t* zrn_gl_create_offscreen(int width, int height);
/** A renderer that draws in a context of the platform; `current` makes that context current (called before GL work). */
zrn_gl_t* zrn_gl_create_external(void (*current)(void* user), void* user);
void zrn_gl_resize(zrn_gl_t* gl, int width, int height);
/** Makes the context current and prepares `p` to draw on it; zrn_gl_end draws what is left and swaps. */
void zrn_gl_begin(zrn_gl_t* gl, zrn_painter_t* p, int width, int height, float scale);
void zrn_gl_end(zrn_gl_t* gl, int swap);
/** Swaps the buffer of an on-screen surface that the renderer made itself; nothing on an offscreen one or one of the platform. */
void zrn_gl_swap(zrn_gl_t* gl);
/** The density of the buffer against the logical size of the window, the painter draws the
 * grid of the UI scale into it. */
/** Off-screen only: the frame as RGBA8, rows top to bottom. */
int zrn_gl_read(zrn_gl_t* gl, uint8_t* rgba, int width, int height);
void zrn_gl_destroy(zrn_gl_t* gl);
void zrn_gl_forget_image(zrn_image_t* image);

struct zrn_painter {
	/** The transform of what is being painted, column-major 4x4 in device pixels (perspective is the renderer's), and whether it is not the identity. */
	float xf[16];
	int has_xf;
	const zrn_backend_t* be;
	void* backend_data;
	uint8_t* pixels;
	int width, height, stride;
	/** Device position of the local origin and device pixels per logical pixel. */
	float tx, ty, scale;
	float alpha;
	int clip[4];
	int clip_stack[32][4];
	int clip_depth;
};

void zrn_blit_alpha(zrn_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, ZColor c);
void zrn_painter_init(zrn_painter_t* p, uint8_t* pixels, int width, int height, float scale);
void zrn_paint_clear(zrn_painter_t* p, ZColor color);
void zrn_fill_rect(zrn_painter_t* p, ZRect r, ZColor c);
/** The outline of a rectangle, drawn inside it. */
void zrn_stroke_rect(zrn_painter_t* p, ZRect r, ZThickness t, ZColor c);
void zrn_fill_rrect(zrn_painter_t* p, ZRect r, ZCornerRadius radius, ZColor c);
void zrn_fill_ellipse(zrn_painter_t* p, ZRect r, ZColor c);
void zrn_stroke_ellipse(zrn_painter_t* p, ZRect r, float thickness, ZColor c);
void zrn_fill_polygon(zrn_painter_t* p, const float* xy, int count, ZColor c);
void zrn_draw_line(zrn_painter_t* p, float x0, float y0, float x1, float y1, float thickness, ZColor c);
void zrn_draw_text(zrn_painter_t* p, const zrn_text_layout_t* l, float x, float y, float width, ZTextAlignment align, ZTextTrimming trim, ZColor c);
/** One run of a single line, no wrapping. */
void zrn_draw_string(zrn_painter_t* p, zrn_font_t font, const char* s, int length, float x, float y, ZColor c);
void zrn_draw_image(zrn_painter_t* p, const zrn_image_t* image, ZRect dest, ZRect source);
void zrn_push_clip(zrn_painter_t* p, ZRect r);
void zrn_pop_clip(zrn_painter_t* p);
/** True when the local rectangle is entirely outside the clip. */
int zrn_clipped_out(const zrn_painter_t* p, ZRect r);

zrn_image_t* zrn_image_load_png(const char* path);
zrn_image_t* zrn_image_from_rgba(int w, int h, const uint8_t* rgba);
void zrn_image_free(zrn_image_t* image);


/* ============================================================================
 *  root (host.c)
 * ============================================================================ */

struct zrn_root {
	Zid id;
	int width, height;
	/** The painter's scale: the UI scale times the density of the buffer. */
	float scale;
	/** The UI scale: the layout grid is the window divided by it; the shell owns it. */
	float ui;
	/** The density of the buffer against the logical size: only crispness. */
	float density;
	zrn_theme_t theme;
	/** The theme we are leaving, the one we are going to, and the progress between them. */
	zrn_theme_t theme_from, theme_target;
	int theme_transitioning;
	float theme_t;
	zrn_element_t* visual;
	zrn_element_t** popups;
	int popup_count, popup_cap;
	zrn_element_t* focus;
	zrn_element_t* capture;
	zrn_element_t* hover;
	zrn_element_t* tooltip_target;
	float tooltip_timer;
	/** The AppBar that is open: a press outside it closes it unless it is sticky. */
	zrn_element_t* open_bar;
	/** Where the flyouts shown at an element are hosted while they show: a hidden Canvas that belongs to the root. */
	zrn_element_t* overlay;
	float pointer_x, pointer_y;
	int layout_dirty, paint_dirty;
	uint8_t* pixels;
	int pixels_w, pixels_h;
	int first_frame;
	ZRenderer renderer;
	ZAnimations animations;
	/** true while the platform presents the root with the OpenGL renderer (Zroot_report_presenter). */
	int gpu_window;
	/** The clock of animations, in ms: the sum of the Zroot_tick deltas, scaled by the animation speed. */
	double time_ms;
	/** Tilt in progress: the pressed element, how far the finger is from its centre (-1..1), how much it leans (0..1). */
	zrn_element_t* tilt_element;
	float tilt_fx, tilt_fy, tilt_amount, tilt_target;
	/** The off-screen GPU context of Zroot_render, made when it is first asked for. */
	struct zrn_gl* gpu;
	/** The overrides of ThemeResource keys the application set (resources.c). */
	struct zrn_res_override* overrides;
	int override_count, override_cap;
};

zrn_root_t* zrn_root_lookup(Zid id);
/**
 * Lays the tree out when it needs it and, when something changed, paints it with `p` (the clear
 * and every element). true when it painted. The software and the GPU renderer both go through it.
 */
int zrn_root_paint(zrn_root_t* r, zrn_painter_t* p);
/** Draws the debug overlay over the frame (debug.c): the bounds of the elements, their class names. */
/** The name Zroot_get_renderer_name reports. */
void zrn_note_renderer(const char* name);

/* ---- animation and transforms (anim.c) ---- */
/** One leg of a tween: it ends at `to` after `duration` ms, with `easing`. */
typedef struct zrn_leg {
	float to, duration;
	ZEasing easing;
} zrn_leg_t;

/** The animations of an element, one per property: after `delay` ms from `start` it runs its legs one after another, from where the property is then. */
typedef struct zrn_tween {
	struct { int active, started, count; float from, delay; double start; zrn_leg_t legs[3]; } p[Z_PROP_LAST_];
} zrn_tween_t;
int zrn_animations_enabled(const zrn_root_t* r);
/** The window has decided on its renderer: a first page that waited swings in now, when animations run. */
void zrn_root_start_intro(zrn_root_t* r);
float zrn_ease(ZEasing easing, float t);
void zrn_tweens_tick(zrn_element_t* e);
void zrn_tweens_free(zrn_element_t* e);
void zrn_update_transformed(zrn_element_t* e);
/** Plays the transition an element was given with Zelement_set_transition (it has appeared). */
void zrn_transition_appear(zrn_element_t* e);
/** How long a transition of an element takes, in ms: the delay is not in it (0 for NONE and for a kind an element cannot play). */
float zrn_transition_duration(ZTransition t);
/** A leaf of a page, for what turns row by row: whether `e` is a container whose children are the rows (frame.c). */
int zrn_is_row_container(const zrn_element_t* e);
void zrn_xf_identity(float m[16]);
/** out = a * b: b is applied first. */
void zrn_xf_mul(float out[16], const float a[16], const float b[16]);
/** The transform of an element, in device pixels, about its centre of projection; origin and size are the element's own, in device pixels. */
void zrn_xf_of_element(const zrn_element_t* e, float origin_x, float origin_y, float width, float height, float scale, float out[16]);
/** Sets the pressed tilt target of the root: the element leaning (NULL to let go) and where the finger is. */
void zrn_tilt_press(zrn_root_t* r, zrn_element_t* e, float x, float y);
void zrn_tilt_move(zrn_root_t* r, float x, float y);
void zrn_tilt_release(zrn_root_t* r);
void zrn_tilt_tick(zrn_root_t* r, float dt);
void zrn_root_attach(zrn_root_t* r, zrn_element_t* e);
void zrn_root_detach(zrn_element_t* e);
void zrn_root_register_popup(zrn_root_t* r, zrn_element_t* popup);
void zrn_root_unregister_popup(zrn_root_t* r, const zrn_element_t* popup);
void zrn_set_focus(zrn_root_t* r, zrn_element_t* e);
void zrn_set_capture(zrn_root_t* r, zrn_element_t* e);
void zrn_release_capture(zrn_root_t* r, zrn_element_t* e);
void zrn_element_forget(zrn_element_t* e);
int zrn_dispatch(zrn_element_t* e, zrn_input_t* in);
void zrn_paint_element(zrn_element_t* e, zrn_painter_t* p, zrn_root_t* r);

#endif
