/**
 * Kestrel - internal.h
 * Structures shared by the sources. Nothing here is public.
 *
 * Every class is a struct that embeds the struct of its base class as its
 * first member, so a pointer to a ListBox is also a pointer to a Selector,
 * an ItemsControl, a Control and an element. Behavior is in a kst_class_t
 * (a vtable); a class that leaves a slot empty inherits it from its base.
 */

#ifndef KST_INTERNAL_H
#define KST_INTERNAL_H

#include "kestrel.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KST_UNUSED(x) ((void)(x))
#define KST_PI 3.14159265358979f

/**
 * The safe area: what the status bar of the shell covers at the top of the screen, in logical
 * pixels, where the first element of a page starts (the 25 px of a Windows Phone page). A constant
 * for now; it is going to come from the shell, and change while the app runs.
 */
#define KST_SAFE_AREA_TOP 32.0f

typedef struct kst_element kst_element_t;
typedef struct kst_class kst_class_t;
typedef struct kst_painter kst_painter_t;
typedef struct kst_root kst_root_t;
typedef struct kst_image kst_image_t;


/* ============================================================================
 *  small helpers
 * ============================================================================ */

static inline float kst_minf(float a, float b) { return a < b ? a : b; }
static inline float kst_maxf(float a, float b) { return a > b ? a : b; }
static inline float kst_clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static inline int kst_mini(int a, int b) { return a < b ? a : b; }
static inline int kst_maxi(int a, int b) { return a > b ? a : b; }
static inline KSize kst_size(float w, float h) { KSize s = { w, h }; return s; }
static inline KRect kst_rect(float x, float y, float w, float h) { KRect r = { x, y, w, h }; return r; }
static inline KColor kst_color(uint32_t argb) { return Kcolor_argb(argb); }
static inline KColor kst_color_alpha(KColor c, float a) { c.a *= a; return c; }
static inline KThickness kst_thickness_zero(void) { KThickness t = { 0, 0, 0, 0 }; return t; }
static inline float kst_thickness_h(KThickness t) { return t.left + t.right; }
static inline float kst_thickness_v(KThickness t) { return t.top + t.bottom; }

char* kst_strdup(const char* s);
void kst_set_string(char** dest, const char* s);
/** Reports an error through Kset_error_callback and remembers it. */
void kst_error(KError error, const char* fmt, ...);


/* ============================================================================
 *  input
 * ============================================================================ */

typedef enum kst_input_type {
	KST_IN_POINTER_DOWN = 1,
	KST_IN_POINTER_UP,
	KST_IN_POINTER_MOVE,
	KST_IN_POINTER_ENTER,
	KST_IN_POINTER_LEAVE,
	KST_IN_WHEEL,
	KST_IN_KEY_DOWN,
	KST_IN_KEY_UP,
	KST_IN_TEXT,
	KST_IN_FOCUS_IN,
	KST_IN_FOCUS_OUT,
	KST_IN_CAPTURE_LOST,
	/** Sent to ancestors that observe the pointer before the target sees the event; they may take the capture (a scroll viewer taking a drag from a button). */
	KST_IN_PREVIEW_DOWN,
	KST_IN_PREVIEW_MOVE,
	KST_IN_PREVIEW_UP
} kst_input_type_t;

typedef struct kst_input {
	kst_input_type_t type;
	/** Pointer position in the coordinates of the element receiving it. */
	float x, y;
	int key;
	int mods;
	const char* text;
	float delta;
} kst_input_t;


/* ============================================================================
 *  classes
 * ============================================================================ */

struct kst_class {
	const kst_class_t* base;
	const char* name;
	KClass id;
	size_t size;
	int abstract_class;
	int resolved;

	/** Called base first, then derived. */
	void (*init)(kst_element_t* e);
	/** Called derived first, then base. */
	void (*destroy)(kst_element_t* e);

	/**
	 * Measure and arrange overrides. `available` has the margin removed and
	 * Width / Height / Min / Max applied; return the content size.
	 * `final` of arrange is the size the element will have; return the size used.
	 */
	KSize (*measure)(kst_element_t* e, KSize available);
	KSize (*arrange)(kst_element_t* e, KSize final);
	/** Draws the element itself in its own coordinates; the children are drawn after. */
	void (*paint)(kst_element_t* e, kst_painter_t* p);
	/** Draws over the children (scroll bars, focus marks). */
	void (*paint_over)(kst_element_t* e, kst_painter_t* p);
	/** Returns K_TRUE when the input was handled and must not bubble. */
	int (*input)(kst_element_t* e, kst_input_t* in);
	void (*tick)(kst_element_t* e, float delta_ms);
	/** A child element is about to be detached: drop pointers to it. */
	void (*child_removed)(kst_element_t* e, kst_element_t* child);
	/** Whether the element itself (not its children) catches the pointer. Default: yes. */
	int (*hit_self)(kst_element_t* e);

	/** Extra room taken inside the element by the chrome a control draws (a Button's touch target). */
	KThickness (*chrome)(kst_element_t* e);
	/** What a TextBlock inside this control inherits. */
	KColor (*content_foreground)(kst_element_t* e);
	float (*content_font_size)(kst_element_t* e);
	KFontFamily (*content_font_family)(kst_element_t* e);
};

const kst_class_t* kst_class_of(KClass id);
int kst_class_derives(const kst_class_t* cls, const kst_class_t* base);


/* ============================================================================
 *  element
 * ============================================================================ */

#define KST_HANDLER_COUNT (K_EVENT_LAST_ - K_EVENT_CLICK)

typedef struct kst_handler {
	KRoutedCallback callback;
	void* user_data;
} kst_handler_t;

struct kst_element {
	Kid id;
	const kst_class_t* cls;
	kst_root_t* root;
	kst_element_t* parent;
	kst_element_t** children;
	int child_count;
	int child_cap;

	char* name;
	void* tag;
	char* tooltip;

	/* FrameworkElement */
	float width, height;
	float min_width, min_height, max_width, max_height;
	KThickness margin;
	KHorizontalAlignment halign;
	KVerticalAlignment valign;
	KVisibility visibility;
	float opacity;
	int enabled;
	int hit_visible;
	int clip_to_bounds;

	/* attached properties */
	int grid_row, grid_column, grid_row_span, grid_column_span;
	float canvas_left, canvas_top;
	int zindex;

	/* layout results */
	KSize desired;
	/** Margin excluded, in the coordinates of the parent. */
	KRect rect;
	KSize last_available;
	int measure_valid;

	/* state */
	int pointer_over;
	int focusable;
	int tab_stop;
	int tab_index;
	int wants_tick;
	int observes_pointer;

	/* render transform (anim.c): rotation in degrees about the point (proj_cx, proj_cy) of the element, as fractions of its size */
	float rot_x, rot_y, proj_cx, proj_cy, render_scale, translate_x, translate_y;
	/** K_TRUE when any of the above is not the identity. */
	int transformed;
	int tilt;
	/** Animations running on this element: one slot per KAnimatedProperty, NULL when it has none. */
	struct kst_tween* tweens;
	/** Laid out and painted by its parent itself (a popup's content, a Viewbox's child), not by the generic walk; not hit tested through the tree. */
	int is_popup_child;
	/** Set when kst_destroy starts: what an element does as it goes must not destroy what is already going. */
	int destroying;

	kst_handler_t handlers[KST_HANDLER_COUNT];

	/** Properties bound to a ThemeResource (resources.c): a small list, NULL when there is none. */
	struct kst_binding* bindings;
	int binding_count;
};

/* a few kinds of data hang off elements of the same names as their classes */

typedef struct kst_panel {
	kst_element_t base;
	KColor background;
	int has_background;
} kst_panel_t;

typedef struct kst_stack_panel {
	kst_panel_t base;
	KOrientation orientation;
	int virtualizing;
} kst_stack_panel_t;

typedef struct kst_wrap_panel {
	kst_panel_t base;
	KOrientation orientation;
	float item_width, item_height;
} kst_wrap_panel_t;

typedef struct kst_grid_def {
	KGridUnitType unit;
	float value;
	float min, max;
	/** Measured then arranged size. */
	float size;
} kst_grid_def_t;

typedef struct kst_grid {
	kst_panel_t base;
	kst_grid_def_t* columns;
	int column_count;
	kst_grid_def_t* rows;
	int row_count;
} kst_grid_t;

typedef struct kst_border {
	kst_element_t base;
	KColor background, border_brush;
	int has_background, has_border_brush;
	KThickness border_thickness, padding;
	KCornerRadius corner_radius;
} kst_border_t;

typedef struct kst_viewbox {
	kst_element_t base;
	KStretch stretch;
	KStretchDirection direction;
	float scale_x, scale_y;
} kst_viewbox_t;

typedef struct kst_text_block {
	kst_element_t base;
	char* text;
	float font_size;
	KFontFamily font_family;
	KColor foreground;
	unsigned set_flags;
	/** A foreground that follows the theme: 0 none, 1 subtle, 2 contrast, 3 accent. */
	int fg_role;
	KTextWrapping wrapping;
	KTextAlignment alignment;
	KTextTrimming trimming;
	int max_lines;
	float line_height;
	int character_spacing;
	/* cache of the last layout */
	void* layout;
	float layout_width;
} kst_text_block_t;

enum {
	KST_TB_FONT_SIZE = 1,
	KST_TB_FONT_FAMILY = 2,
	KST_TB_FOREGROUND = 4
};

struct kst_image {
	int width, height;
	uint8_t* rgba;
	/** The texture of the OpenGL renderer that has drawn it (render/gles.c); 0 and NULL otherwise. */
	unsigned gl_texture;
	void* gl_owner;
};

typedef struct kst_image_element {
	kst_element_t base;
	kst_image_t* image;
	KStretch stretch;
} kst_image_element_t;

typedef struct kst_popup {
	kst_element_t base;
	int is_open;
	int stays_open;
	float h_offset, v_offset;
	Kid target;
	KPlacementMode placement;
	int placed;
	/** A modal popup (a ContentDialog) dims what is under it and blocks input to it. */
	int modal;
	/** As wide as the root, whatever its child asks for (a flyout on the phone). */
	int full_width;
} kst_popup_t;


/* ---- Control and below ---- */

enum {
	KST_C_FOREGROUND = 1,
	KST_C_BACKGROUND = 2,
	KST_C_BORDER_BRUSH = 4,
	KST_C_BORDER_THICKNESS = 8,
	KST_C_PADDING = 16,
	KST_C_FONT_SIZE = 32,
	KST_C_FONT_FAMILY = 64
};

typedef struct kst_control {
	kst_element_t base;
	KColor foreground, background, border_brush;
	unsigned set_flags;
	KThickness border_thickness, padding;
	float font_size;
	KFontFamily font_family;
	KHorizontalAlignment h_content;
	KVerticalAlignment v_content;
} kst_control_t;

typedef struct kst_content_control {
	kst_control_t base;
	kst_element_t* content;
	/** K_TRUE when `content` is the TextBlock made for a string. */
	int content_is_text;
} kst_content_control_t;

/** A page a Frame shows (frame.c): the navigation parameter it was created for. */
typedef struct kst_page {
	kst_content_control_t base;
	void* parameter;
	/** How the frame reached the page now: what OnNavigatedTo and OnNavigatedFrom are told. */
	KNavigationMode navigation_mode;
	KNavigationCacheMode cache_mode;
	/** Page.TopAppBar and BottomAppBar: children of the page after its content. */
	kst_element_t* top_bar;
	kst_element_t* bottom_bar;
	/** The index in the history of the entry the page was built for. */
	int entry_index;
} kst_page_t;

typedef struct kst_frame_entry {
	KPageFactory factory;
	void* parameter;
	/** The page of the entry, alive and out of the frame, when its NavigationCacheMode keeps it. */
	Kid cached;
} kst_frame_entry_t;

/** A Frame: the navigation history, `current` is the entry shown. */
typedef struct kst_frame {
	kst_content_control_t base;
	kst_frame_entry_t* entries;
	int count, capacity, current;
	/** K_TRUE while the callbacks of a navigation run; another Navigate from them is refused. */
	int navigating;
	/** The transition (frame.c): 1 the old page turns away, 2 the new one swings in, 0 none. */
	KPageTransition transition;
	/** The first page went in before it was known whether animations run (the window is made later): Kwindow_run starts its swing-in. */
	int intro_pending;
	int phase, direction;
	/** The mode of the navigation in progress (the page that leaves is told it too). */
	KNavigationMode mode;
	float elapsed;
	/** The new page, built but not yet in the frame, during phase 1. */
	Kid pending;
	void* pending_parameter;
	/** The rows being turned, and the opacity each had. */
	Kid* rows;
	float* row_opacity;
	int row_count, row_cap;
} kst_frame_t;

typedef struct kst_button_base {
	kst_content_control_t base;
	KClickMode click_mode;
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
	kst_element_t* flyout;
} kst_button_base_t;

typedef struct kst_toggle_button {
	kst_button_base_t base;
	KToggleState state;
	int three_state;
	char* group_name;
} kst_toggle_button_t;

typedef struct kst_range_base {
	kst_control_t base;
	double minimum, maximum, value;
	double small_change, large_change;
	KValueChangedCallback changed;
	void* changed_data;
} kst_range_base_t;

typedef struct kst_slider {
	kst_range_base_t base;
	KOrientation orientation;
	int dragging;
	char* header;
	double step_frequency, tick_frequency;
	KTickPlacement tick_placement;
	KSliderSnapsTo snaps_to;
	int direction_reversed;
} kst_slider_t;

typedef struct kst_progress_bar {
	kst_range_base_t base;
	int indeterminate;
	int show_error, show_paused;
	/** The clock of the indeterminate dots, in ms. */
	float time_ms;
} kst_progress_bar_t;

/** ToggleSwitch (buttons.c): IsOn, the header and the two contents, and where the knob is. */
typedef struct kst_toggle_switch {
	kst_control_t base;
	int is_on;
	char *header, *on_content, *off_content;
	/** The knob and the curtain: 0 is off, 1 is on; `knob_goal` is where they are going. */
	float knob, knob_goal;
	int pressed, dragging, moved;
	float press_x, grab;
} kst_toggle_switch_t;

/** ProgressRing (range.c): the dots run while IsActive. */
typedef struct kst_progress_ring {
	kst_control_t base;
	int is_active;
	float time_ms;
} kst_progress_ring_t;

typedef struct kst_scrollbar {
	kst_range_base_t base;
	KOrientation orientation;
	double viewport;
	int dragging;
	float drag_grab;
	KScrollCallback on_scroll;
	void* on_scroll_data;
} kst_scrollbar_t;

typedef struct kst_thumb {
	kst_control_t base;
	int dragging;
	float last_x, last_y;
	float total_x, total_y;
	KDragCallback started, delta, completed;
	void *started_data, *delta_data, *completed_data;
} kst_thumb_t;

typedef struct kst_scroll_viewer {
	kst_content_control_t base;
	KScrollBarVisibility v_visibility, h_visibility;
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
	KScrollCallback on_scroll;
	void* on_scroll_data;
} kst_scroll_viewer_t;

typedef struct kst_text_box {
	kst_control_t base;
	char* text;
	int length_bytes;
	int caret;
	int anchor;
	int accepts_return;
	KTextWrapping wrapping;
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
	/** K_TRUE while TextChanged is raised for a change the user typed. */
	int from_user;
	/** Called first for a key (an AutoSuggestBox takes Up, Down, Enter and Escape); K_TRUE when it took the key. */
	int (*key_hook)(struct kst_text_box* box, kst_input_t* in);
} kst_text_box_t;

/** The selection modes of the items controls: ListBox and ListView have all four (a ListBox has no None). */
typedef enum kst_sel_mode {
	KST_SEL_NONE = 0,
	KST_SEL_SINGLE,
	KST_SEL_MULTIPLE,
	KST_SEL_EXTENDED
} kst_sel_mode_t;

/** SelectorItem (items.c): a container of an item, ListBoxItem, ComboBoxItem, ListViewItem, GridViewItem, FlipViewItem. */
typedef struct kst_selector_item {
	kst_content_control_t base;
	int selected;
	int pressed;
	float press_x, press_y;
} kst_selector_item_t;

/**
 * ItemsControl and Selector (items.c). The items are containers in `host`, the items panel, with the header
 * items of the groups between them (only a ListView or a GridView has any). `scroll` is the ScrollViewer
 * a ListBox, a ListView and a GridView show the panel in; `presenter` is what it holds: the header, the panel and the footer.
 */
typedef struct kst_items_control {
	kst_control_t base;
	kst_element_t* scroll;
	kst_element_t* host;
	kst_element_t* presenter;
	int selected_index;
	kst_sel_mode_t sel_mode;
	/** The item the keyboard is on: its container, K_INVALID_ID for none. */
	Kid focus_item;
	/** The keyboard moved it there: a tap gives the list the focus without showing where. */
	int focus_visible;
	/** The header items of the groups in `host`: when 0 an item index is the index of its child. */
	int group_count;
} kst_items_control_t;

/** ListViewBase (listview.c). */
typedef struct kst_list_view_base {
	kst_items_control_t base;
	int item_click_enabled;
	int swipe_enabled;
	Kid header, footer;
	KItemClickCallback on_item_click;
	void* on_item_click_data;
} kst_list_view_base_t;

/** ComboBox (combo.c). */
typedef struct kst_combo_box {
	kst_items_control_t base;
	kst_element_t* popup;
	int is_open;
	int pressed;
	float max_drop_down_height;
	char* header;
	char* placeholder;
} kst_combo_box_t;

/** FlipView (flip.c): the items are pages side by side that slide as the pivot's do. */
typedef struct kst_flip_view {
	kst_items_control_t base;
	float offset, from, to, elapsed;
	int sliding, pressed, dragging;
	float press_x, press_y, last_x, velocity;
	int touch_animations_for_all;
	/** K_TRUE while a finger or a key changes the selection. */
	int user_nav;
} kst_flip_view_t;

/** AutoSuggestBox (combo.c): a TextBox and a popup with a ListView of the suggestions. */
typedef struct kst_auto_suggest_box {
	kst_items_control_t base;
	kst_element_t* text_box;
	kst_element_t* popup;
	kst_element_t* list;
	int is_open;
	int update_text_on_select;
	float max_suggestion_list_height;
	/** The suggestion the keyboard is on, -1 for none. */
	int highlight;
	/** Set while the text is changed by the program or a suggestion. */
	KAutoSuggestionBoxTextChangeReason reason;
	int reason_set;
	KAutoSuggestTextChangedCallback on_text_changed;
	void* on_text_changed_data;
	KAutoSuggestSuggestionChosenCallback on_suggestion_chosen;
	void* on_suggestion_chosen_data;
	KAutoSuggestQuerySubmittedCallback on_query_submitted;
	void* on_query_submitted_data;
} kst_auto_suggest_box_t;

/** ItemsWrapGrid (panels.c). */
typedef struct kst_items_wrap_grid {
	kst_panel_t base;
	KOrientation orientation;
	int max_rows_or_columns;
	float item_width, item_height;
} kst_items_wrap_grid_t;


/* ---- icons, app bars and flyouts (appbar.c, flyout.c) ---- */

/** IconElement: Foreground when the program sets one, else the one of the control the icon is in. */
typedef struct kst_icon {
	kst_element_t base;
	KColor foreground;
	int has_foreground;
	/** SymbolIcon. */
	KSymbol symbol;
	/** FontIcon. */
	char* glyph;
	float font_size;
	KFontFamily family;
	/** PathIcon: closed polygons, each as a run of points of `path`, `path_runs` holds their lengths. */
	float* path;
	int* path_runs;
	int path_run_count;
	float path_bounds[4];
	/** BitmapIcon. */
	kst_image_t* image;
} kst_icon_t;

/** AppBar: closed it is as tall as ClosedDisplayMode says, open as tall as its content (a CommandBar: its commands with their labels, and the secondary ones above). */
typedef struct kst_app_bar {
	kst_content_control_t base;
	int is_open, is_sticky;
	KAppBarClosedDisplayMode closed_mode;
	/** The opening animation: 0 closed, 1 open. */
	float open_t;
	/** Set by the Page that shows it as its TopAppBar. */
	int on_top;
	/** A press began on the three dots. */
	int ellipsis_pressed;
} kst_app_bar_t;

typedef struct kst_command_bar {
	kst_app_bar_t base;
	kst_element_t** primary;
	int primary_count, primary_cap;
	kst_element_t** secondary;
	int secondary_count, secondary_cap;
} kst_command_bar_t;

/** What AppBarButton and AppBarToggleButton add to their base. */
typedef struct kst_app_bar_item {
	char* label;
	int is_compact;
	/** A secondary command: a row of text in the open menu. */
	int overflow;
} kst_app_bar_item_t;

typedef struct kst_app_bar_button {
	kst_button_base_t base;
	kst_app_bar_item_t item;
} kst_app_bar_button_t;

typedef struct kst_app_bar_toggle_button {
	kst_toggle_button_t base;
	kst_app_bar_item_t item;
} kst_app_bar_toggle_button_t;

/** FlyoutBase: the Popup it shows in is its one child; the presenter is the child of the popup. */
typedef struct kst_flyout_base {
	kst_element_t base;
	kst_element_t* popup;
	KFlyoutPlacementMode placement;
	/** The element it was shown at. */
	Kid target;
	int is_open;
	/** What a flyout class does when it is about to show, and when it has been dismissed (pickers.c): called before Opening and before Closed. */
	void (*on_show)(kst_element_t* flyout);
	void (*on_hide)(kst_element_t* flyout);
} kst_flyout_base_t;

typedef struct kst_menu_flyout_item {
	kst_control_t base;
	char* text;
	int pressed;
	/** ToggleMenuFlyoutItem. */
	int is_checked;
} kst_menu_flyout_item_t;

typedef struct kst_content_dialog {
	kst_content_control_t base;
	kst_element_t *title, *button1, *button2;
	char *primary_text, *secondary_text;
	int primary_enabled, secondary_enabled;
	int is_open;
	/** The popup that holds the dialog (its parent) once it has been shown. */
	Kid popup;
	Kid previous_focus;
	KContentDialogResult result;
	KContentDialogResultCallback callback;
	void* callback_data;
} kst_content_dialog_t;

#define KST_AS(type, e) ((type*)(e))


/* ============================================================================
 *  handles and elements (core.c)
 * ============================================================================ */

/** The handle table holds elements and roots. */
Kid kst_handle_alloc(void* ptr, int is_root);
void kst_handle_free(Kid id);
void* kst_handle_get(Kid id, int is_root);
kst_element_t* kst_lookup(Kid id);
/** Children in paint order (Panel.ZIndex, then insertion). Free with kst_ordered_free. */
kst_element_t** kst_ordered(kst_element_t* e, int* count, int* heap);
void kst_ordered_free(kst_element_t** list, int heap);
/** Looks an element up and checks its class; reports and returns NULL on failure. */
kst_element_t* kst_get(Kid id, KClass cls);
kst_element_t* kst_create(KClass cls);
void kst_destroy(kst_element_t* e);
int kst_is_a(const kst_element_t* e, KClass cls);
void kst_add_child(kst_element_t* parent, kst_element_t* child, int index);
void kst_remove_child(kst_element_t* parent, kst_element_t* child);
void kst_clear_children(kst_element_t* parent);
/** Replaces the one child of a single-child element: destroys the old one. */
void kst_set_only_child(kst_element_t* parent, kst_element_t** slot, kst_element_t* child);
int kst_child_index(const kst_element_t* parent, const kst_element_t* child);

/* layout */
void kst_measure(kst_element_t* e, KSize available);
void kst_arrange(kst_element_t* e, KRect final_rect);
KSize kst_measure_children_max(kst_element_t* e, KSize available);
void kst_changed_layout(kst_element_t* e);
void kst_changed_paint(kst_element_t* e);
/** Arrange again without measuring again (a scroll offset changed). */
void kst_changed_arrange(kst_element_t* e);
int kst_effectively_enabled(const kst_element_t* e);
int kst_effectively_visible(const kst_element_t* e);
KPoint kst_to_root(const kst_element_t* e, KPoint p);
KPoint kst_from_root(const kst_element_t* e, KPoint p);
kst_element_t* kst_hit_test(kst_element_t* e, KPoint p);
/** Calls the handler of an event. @return K_FALSE when the handler destroyed the element: the caller must then touch nothing of it. */
int kst_raise(kst_element_t* e, KEvent ev);
/** A change of an inherited property (font) can change the measure of everything below. */
void kst_invalidate_deep(kst_element_t* e);
/** The ids of an element and its ancestors, innermost first: safe to walk while callbacks destroy things. */
int kst_chain(const kst_element_t* e, Kid* ids, int max);

/** Walks up for what a TextBlock inherits. */
KColor kst_inherited_foreground(const kst_element_t* e);
float kst_inherited_font_size(const kst_element_t* e);
KFontFamily kst_inherited_font_family(const kst_element_t* e);

/** Element rectangle in its own coordinates. */
static inline KRect kst_local_rect(const kst_element_t* e) { return kst_rect(0, 0, e->rect.width, e->rect.height); }

/* ---- the classes, one per source file ---- */

extern kst_class_t kst_class_element, kst_class_panel, kst_class_stack_panel, kst_class_virtualizing_stack_panel,
	kst_class_wrap_panel, kst_class_grid, kst_class_canvas, kst_class_border, kst_class_viewbox, kst_class_text_block,
	kst_class_image, kst_class_popup, kst_class_control, kst_class_user_control, kst_class_content_control,
	kst_class_frame, kst_class_page, kst_class_pivot, kst_class_tooltip, kst_class_button_base, kst_class_button, kst_class_hyperlink_button, kst_class_repeat_button,
	kst_class_toggle_button, kst_class_checkbox, kst_class_radio_button, kst_class_range_base, kst_class_slider,
	kst_class_progress_bar, kst_class_scrollbar, kst_class_thumb, kst_class_separator, kst_class_scroll_viewer,
	kst_class_text_box, kst_class_password_box, kst_class_toggle_switch, kst_class_progress_ring, kst_class_items_control, kst_class_selector, kst_class_list_box,
	kst_class_list_box_item, kst_class_combo_box, kst_class_combo_box_item, kst_class_auto_suggest_box, kst_class_selector_item,
	kst_class_list_view_base, kst_class_list_view, kst_class_grid_view, kst_class_list_view_item, kst_class_grid_view_item,
	kst_class_list_view_base_header_item, kst_class_list_view_header_item, kst_class_grid_view_header_item, kst_class_flip_view,
	kst_class_flip_view_item, kst_class_items_stack_panel, kst_class_items_wrap_grid, kst_class_semantic_zoom,
	kst_class_icon_element, kst_class_symbol_icon, kst_class_font_icon, kst_class_path_icon, kst_class_bitmap_icon, kst_class_app_bar,
	kst_class_command_bar, kst_class_app_bar_button, kst_class_app_bar_toggle_button, kst_class_app_bar_separator, kst_class_flyout_base,
	kst_class_flyout, kst_class_menu_flyout, kst_class_menu_flyout_item_base, kst_class_menu_flyout_item,
	kst_class_toggle_menu_flyout_item, kst_class_menu_flyout_separator, kst_class_content_dialog, kst_class_pivot_item, kst_class_hub,
	kst_class_hub_section, kst_class_looping_selector, kst_class_date_picker, kst_class_time_picker, kst_class_picker_flyout_base,
	kst_class_date_picker_flyout, kst_class_time_picker_flyout, kst_class_rich_text_block;

void kst_popup_close(kst_element_t* popup);
void kst_popup_set_open(kst_element_t* popup, int open);
/** Closes what the Back button closes first: a dialog, a popup, an open app bar. K_TRUE when it closed something. */
int kst_root_dismiss_top(kst_root_t* r);
/** A page (frame.c) asks the bars it shows how tall they are closed and now, and tells them they are on top. */
float kst_app_bar_closed_height(kst_element_t* bar);
float kst_app_bar_height(kst_element_t* bar, float width);
void kst_app_bar_set_on_top(kst_element_t* bar, int on_top);
/** Shows the flyout of a button at it (flyout.c). */
void kst_flyout_show_at(kst_element_t* flyout, kst_element_t* target);
/** A command of an app bar was clicked (appbar.c): a secondary one closes the bar. */
void kst_app_bar_item_clicked(kst_element_t* item);
/** items.c: what the panel of an items control needs to tell it when its orientation changes. */
void kst_items_panel_changed(kst_element_t* panel);

/* the parts of the items controls that the sources of the classes share (items.c) */
kst_items_control_t* kst_item_owner(kst_element_t* item);
int kst_ic_count(const kst_items_control_t* o);
kst_element_t* kst_ic_item(const kst_items_control_t* o, int index);
int kst_ic_index_of(const kst_items_control_t* o, const kst_element_t* container);
kst_element_t* kst_ic_add(kst_items_control_t* o, kst_element_t* item, const char* text);
const char* kst_container_text(kst_element_t* container);
/** Selection by the program: the item alone (-1 none). Raises SelectionChanged when the selection changed. */
void kst_ic_select(kst_items_control_t* o, int index);
/** Selection by a tap on an item, as the mode says. Raises SelectionChanged. */
void kst_ic_tap(kst_items_control_t* o, int index);
void kst_ic_set_sel_mode(kst_items_control_t* o, kst_sel_mode_t mode);
void kst_ic_select_all(kst_items_control_t* o);
int kst_ic_selected_indices(const kst_items_control_t* o, int* out, int max);
void kst_ic_set_host(kst_items_control_t* o, kst_element_t* panel);
void kst_ic_scroll_into_view(kst_items_control_t* o, int index, KScrollIntoViewAlignment alignment);
/** The keys of a list: the arrows, Home and End move the item the keyboard is on, Space and Enter tap it. */
int kst_ic_key(kst_element_t* e, kst_input_t* in);
/** SelectorItem.IsSelected without any event, for the controls that keep it in step with a state of their own. */
void kst_item_set_selected(kst_element_t* item, int selected);
/** flip.c: what a FlipView does when its items or its selection change. */
void kst_flip_items_changed(kst_items_control_t* o);
void kst_flip_selected(kst_items_control_t* o);
/** The tap on an item of a ComboBox (combo.c), of a ListView or a GridView (listview.c). */
void kst_combo_item_tapped(kst_items_control_t* o, int index);
void kst_lvb_item_tapped(kst_items_control_t* o, int index);
KSize kst_cc_measure(kst_element_t* e, KSize available);
/** A content control forgets its content when it is removed: the classes that have children of their own chain to it. */
void kst_cc_child_removed(kst_element_t* e, kst_element_t* child);
KSize kst_cc_arrange(kst_element_t* e, KSize final);
/** Chrome + border + padding of a content control. */
KThickness kst_cc_insets(kst_element_t* e);

/* content controls (basic.c) */
void kst_content_set(kst_content_control_t* c, kst_element_t* content);
void kst_content_set_text(kst_content_control_t* c, const char* text);
/** The effective values of a control, theme defaults filled in. */
KColor kst_ctl_foreground(const kst_control_t* c);
KColor kst_ctl_background(const kst_control_t* c, KColor fallback);
KColor kst_ctl_border_brush(const kst_control_t* c, KColor fallback);
KColor kst_ctl_color(const kst_control_t* c, unsigned flag, KColor stored, KColor fallback);


/* ============================================================================
 *  theme (theme.c)
 * ============================================================================ */

typedef enum kst_theme_color_id {
	KST_TC_FOREGROUND,
	KST_TC_BACKGROUND,
	KST_TC_CONTRAST_FOREGROUND,
	KST_TC_CONTRAST_BACKGROUND,
	KST_TC_DISABLED,
	KST_TC_PROGRESS_BAR_BACKGROUND,
	KST_TC_TEXT_BOX,
	KST_TC_TEXT_BOX_FOREGROUND,
	KST_TC_TEXT_BOX_EDIT_BACKGROUND,
	KST_TC_TEXT_BOX_READ_ONLY,
	KST_TC_TEXT_CARET,
	KST_TC_SUBTLE,
	KST_TC_BUTTON_PRESSED_FOREGROUND,
	KST_TC_CHROME,
	KST_TC_INACTIVE,
	KST_TC_BORDER,
	KST_TC_SEMITRANSPARENT,
	KST_TC_ACCENT,
	/* Windows Phone 8.1 (themeresources.xaml) */
	KST_TC_BASE_MEDIUM_HIGH,
	KST_TC_BASE_MID,
	KST_TC_BASE_MEDIUM_MID,
	KST_TC_BASE_MEDIUM_MID_SOLID,
	KST_TC_BASE_LOW,
	KST_TC_BASE_LOW_SOLID,
	KST_TC_TEXT_OVER_ACCENT,
	KST_TC_TEXT_BOX_BACKGROUND_81,
	KST_TC_TEXT_BOX_BORDER_81,
	KST_TC_TEXT_BOX_FOREGROUND_81,
	KST_TC_TEXT_BOX_PLACEHOLDER_81,
	KST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81,
	KST_TC_SLIDER_THUMB,
	KST_TC_SLIDER_THUMB_DISABLED,
	KST_TC_SLIDER_TRACK,
	KST_TC_SLIDER_DECREASE_DISABLED,
	/* PhoneBaseBlackColor, PhoneBaseWhiteColor, PhoneDisabledColor, PhoneTextMidColor, PhoneTextLowColor */
	KST_TC_BASE_BLACK,
	KST_TC_BASE_WHITE,
	KST_TC_PHONE_DISABLED,
	KST_TC_TEXT_MID,
	KST_TC_TEXT_LOW,
	/* ContentDialogDimmingColor */
	KST_TC_CONTENT_DIALOG_DIMMING,
	/* the text of a ListBox: PhoneBaseBlack on the white 80% box of Dark; the 8.1 Light dictionary has PhoneBaseBlack on a black 75% box, unreadable, so Light uses PhoneBaseWhite */
	KST_TC_LIST_BOX_FOREGROUND,
	KST_TC_COUNT
} kst_theme_color_id_t;

typedef struct kst_theme {
	KTheme theme;
	KColor colors[KST_TC_COUNT];
	KColor accent;
	KColor background;
	int has_background;
} kst_theme_t;

void kst_theme_init(kst_theme_t* t, KTheme theme, KColor accent);
/** The theme of the root an element is in, or the default theme for a detached element. */
KColor kst_theme_color(const kst_element_t* e, kst_theme_color_id_t id);
KColor kst_accent_color(KAccent accent);

/* WP8 sizes (themes.md) */
#define KST_FONT_SMALL 18.667f
#define KST_FONT_NORMAL 20.0f
#define KST_FONT_MEDIUM 22.667f
#define KST_FONT_MEDIUM_LARGE 25.333f
#define KST_FONT_LARGE 32.0f
#define KST_FONT_EXTRA_LARGE 42.667f
#define KST_FONT_EXTRA_EXTRA_LARGE 72.0f
#define KST_FONT_HUGE 186.667f
#define KST_BORDER_THICKNESS 3.0f
#define KST_TOUCH_TARGET_OVERHANG 12.0f

/* Windows Phone 8.1 (themeresources.xaml and generic.xaml), taken as they are */
#define KST_81_BORDER_THICKNESS 2.5f
/** PhoneTouchTargetOverhang is 0,9.5 and PhoneTouchTargetLargeOverhang 0,16: only above and below. */
#define KST_81_TOUCH_OVERHANG 9.5f
#define KST_81_TOUCH_LARGE_OVERHANG 16.0f
#define KST_81_FONT_LARGE 18.14f
#define KST_81_FONT_CONTROL_HEADER 16.0f
#define KST_81_FONT_CONTENT 20.26f
#define KST_81_FONT_EXTRA_LARGE 25.5f
#define KST_81_FONT_EXTRA_LARGE_PLUS 29.86f


/* ============================================================================
 *  ThemeResource (resources.c, theme_resources.c)
 * ============================================================================ */

typedef enum kst_res_type {
	KST_RT_NONE = 0,
	KST_RT_COLOR,
	KST_RT_DOUBLE,
	KST_RT_INT,
	KST_RT_THICKNESS,
	KST_RT_FONT_FAMILY
} kst_res_type_t;

typedef union kst_res_value {
	/** A color: ARGB; of an accent color only the alpha counts. */
	uint32_t argb;
	/** A double in f[0], a thickness (left, top, right, bottom) in f. */
	float f[4];
	/** An int, a font weight, a KFontFamily. */
	int i;
} kst_res_value_t;

/** One generated key: the table is indexed by key - K_RES_FIRST_. */
typedef struct kst_res_def {
	const char* name;
	uint8_t type;
	/** 1: the Dark color is the accent of the root (with the alpha of `dark`), 2: the Light one. */
	uint8_t accent;
	kst_res_value_t dark, light;
} kst_res_def_t;

extern const kst_res_def_t kst_res_table[];

typedef struct kst_res_override {
	KThemeResource key;
	kst_res_type_t type;
	union { KColor color; float d; int i; KThickness thickness; } v;
} kst_res_override_t;

/** The value of a key of a root (NULL: the default theme) as the type says; K_FALSE when the key has no such value. */
int kst_res_color(const kst_root_t* r, KThemeResource key, KColor* out);
int kst_res_double(const kst_root_t* r, KThemeResource key, float* out);
int kst_res_int(const kst_root_t* r, KThemeResource key, int* out);
int kst_res_thickness(const kst_root_t* r, KThemeResource key, KThickness* out);
int kst_res_font_family(const kst_root_t* r, KThemeResource key, KFontFamily* out);

/** The properties of an element that can follow a key. */
typedef enum kst_bind_prop {
	KST_BP_ELEMENT_MARGIN = 1,
	KST_BP_TEXT_FOREGROUND,
	KST_BP_TEXT_FONT_SIZE,
	KST_BP_TEXT_CHARACTER_SPACING,
	KST_BP_CONTROL_FOREGROUND,
	KST_BP_CONTROL_BACKGROUND,
	KST_BP_CONTROL_BORDER_BRUSH,
	KST_BP_CONTROL_BORDER_THICKNESS,
	KST_BP_CONTROL_PADDING,
	KST_BP_CONTROL_FONT_SIZE,
	KST_BP_BORDER_BACKGROUND,
	KST_BP_BORDER_BORDER_BRUSH,
	KST_BP_BORDER_BORDER_THICKNESS,
	KST_BP_BORDER_PADDING
} kst_bind_prop_t;

typedef struct kst_binding {
	kst_bind_prop_t prop;
	KThemeResource key;
} kst_binding_t;

/** Binds `prop` of `e` to `key` (replacing its binding) and applies it; K_FALSE when the key is not of the type the property takes. */
int kst_bind(kst_element_t* e, kst_bind_prop_t prop, KThemeResource key);
/** A direct setter was called: the property keeps its value and stops following a key. */
void kst_unbind(kst_element_t* e, kst_bind_prop_t prop);
/** Applies every binding of `e` (when it enters a root). */
void kst_bindings_apply(kst_element_t* e);
void kst_bindings_free(kst_element_t* e);
/** The theme, the accent or an override changed: every bound property of the tree of the root is resolved again. */
void kst_root_resources_changed(kst_root_t* r);
void kst_root_resources_free(kst_root_t* r);


/* ============================================================================
 *  text (text.c)
 * ============================================================================ */

typedef struct kst_font {
	KFontFamily family;
	float size;
	int spacing;
} kst_font_t;

typedef struct kst_line {
	int start, end;
	float width;
} kst_line_t;

typedef struct kst_text_layout {
	kst_font_t font;
	const char* text;
	kst_line_t* lines;
	int line_count;
	int truncated;
	float width, height, line_height;
} kst_text_layout_t;

int kst_text_init(void);
void kst_text_shutdown(void);
/** Lays text out. `max_width` <= 0 is unlimited. The text must outlive the layout. */
kst_text_layout_t* kst_text_layout_new(kst_font_t font, const char* text, int length, float max_width, int wrap, int max_lines, float line_height);
void kst_text_layout_free(kst_text_layout_t* l);
/** K_TRUE when the font, or a fallback found on the system, has the glyph. */
int kst_font_has_glyph(kst_font_t font, uint32_t cp);
float kst_font_line_height(kst_font_t font);
float kst_font_ascent(kst_font_t font);
float kst_text_width(kst_font_t font, const char* s, int length);
/** Byte offset in [0, length] nearest to x. */
int kst_text_index_at(kst_font_t font, const char* s, int length, float x);
int kst_utf8_next(const char* s, int length, int index);
int kst_utf8_prev(const char* s, int index);
uint32_t kst_utf8_decode(const char* s, int length, int* consumed);
int kst_utf8_encode(uint32_t cp, char* out);


/* ============================================================================
 *  painter (render/painter.c)
 * ============================================================================ */

/**
 * A renderer. Every call has the signature of the kst_* function of the same name, coordinates
 * logical and local as the painter's tx, ty, scale, alpha and clip say. The software backend
 * (render/software.c) draws into an RGBA8 buffer; others (OpenGL) draw on the GPU.
 */
typedef struct kst_backend {
	const char* name;
	void (*paint_clear)(kst_painter_t* p, KColor c);
	void (*fill_rect)(kst_painter_t* p, KRect r, KColor c);
	void (*fill_rrect)(kst_painter_t* p, KRect r, KCornerRadius radius, KColor c);
	void (*fill_ellipse)(kst_painter_t* p, KRect r, KColor c);
	void (*stroke_ellipse)(kst_painter_t* p, KRect r, float thickness, KColor c);
	void (*fill_polygon)(kst_painter_t* p, const float* xy, int count, KColor c);
	void (*draw_line)(kst_painter_t* p, float x0, float y0, float x1, float y1, float thickness, KColor c);
	/** One glyph bitmap: 8 bit coverage at a device position. */
	void (*blit_alpha)(kst_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, KColor c);
	void (*draw_image)(kst_painter_t* p, const kst_image_t* image, KRect dest, KRect source);
	void (*push_clip)(kst_painter_t* p, KRect r);
	void (*pop_clip)(kst_painter_t* p);
	int (*clipped_out)(const kst_painter_t* p, KRect r);
	/** What follows is drawn under this affine transform (column-major 4x4, device pixels), NULL for none. Backends that cannot, draw it untransformed. */
	void (*set_transform)(kst_painter_t* p, const float* m);
} kst_backend_t;

extern const kst_backend_t kst_backend_software;
extern const kst_backend_t kst_backend_gles;
/** The clip is state of the painter, the same for every backend. */
void kst_clip_push(kst_painter_t* p, KRect r);
void kst_clip_pop(kst_painter_t* p);
int kst_clip_out(const kst_painter_t* p, KRect r);

/* render/gles.c: an OpenGL ES 2 context, in a window or off-screen */
typedef struct kst_gl kst_gl_t;
int kst_gl_available(void);
kst_gl_t* kst_gl_create_offscreen(int width, int height);
kst_gl_t* kst_gl_create_window(void* wl_display, void* wl_surface, int width, int height);
void kst_gl_resize(kst_gl_t* gl, int width, int height);
/** Makes the context current and prepares `p` to draw on it; kst_gl_end draws what is left and swaps. */
void kst_gl_begin(kst_gl_t* gl, kst_painter_t* p, int width, int height, float scale);
void kst_gl_end(kst_gl_t* gl, int swap);
/** Swaps the buffer of an on-screen window to the compositor; nothing on an offscreen one. */
void kst_gl_swap(kst_gl_t* gl);
/** The density of the buffer against the logical size of the window, the painter draws the
 * grid of the UI scale into it. */
void kst_root_set_density(Kid root, float density);
/** Off-screen only: the frame as RGBA8, rows top to bottom. */
int kst_gl_read(kst_gl_t* gl, uint8_t* rgba, int width, int height);
void kst_gl_destroy(kst_gl_t* gl);
void kst_gl_forget_image(kst_image_t* image);

struct kst_painter {
	/** The transform of what is being painted, column-major 4x4 in device pixels (perspective is the renderer's), and whether it is not the identity. */
	float xf[16];
	int has_xf;
	const kst_backend_t* be;
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

void kst_blit_alpha(kst_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, KColor c);
void kst_painter_init(kst_painter_t* p, uint8_t* pixels, int width, int height, float scale);
void kst_paint_clear(kst_painter_t* p, KColor color);
void kst_fill_rect(kst_painter_t* p, KRect r, KColor c);
/** The outline of a rectangle, drawn inside it. */
void kst_stroke_rect(kst_painter_t* p, KRect r, KThickness t, KColor c);
void kst_fill_rrect(kst_painter_t* p, KRect r, KCornerRadius radius, KColor c);
void kst_fill_ellipse(kst_painter_t* p, KRect r, KColor c);
void kst_stroke_ellipse(kst_painter_t* p, KRect r, float thickness, KColor c);
void kst_fill_polygon(kst_painter_t* p, const float* xy, int count, KColor c);
void kst_draw_line(kst_painter_t* p, float x0, float y0, float x1, float y1, float thickness, KColor c);
void kst_draw_text(kst_painter_t* p, const kst_text_layout_t* l, float x, float y, float width, KTextAlignment align, KTextTrimming trim, KColor c);
/** One run of a single line, no wrapping. */
void kst_draw_string(kst_painter_t* p, kst_font_t font, const char* s, int length, float x, float y, KColor c);
void kst_draw_image(kst_painter_t* p, const kst_image_t* image, KRect dest, KRect source);
void kst_push_clip(kst_painter_t* p, KRect r);
void kst_pop_clip(kst_painter_t* p);
/** True when the local rectangle is entirely outside the clip. */
int kst_clipped_out(const kst_painter_t* p, KRect r);

kst_image_t* kst_image_load_png(const char* path);
kst_image_t* kst_image_from_rgba(int w, int h, const uint8_t* rgba);
void kst_image_free(kst_image_t* image);


/* ============================================================================
 *  root (host.c)
 * ============================================================================ */

struct kst_root {
	Kid id;
	int width, height;
	/** The painter's scale: the UI scale times the density of the buffer. */
	float scale;
	/** The UI scale: the layout grid is the window divided by it; the shell owns it. */
	float ui;
	/** The density of the buffer against the logical size: only crispness. */
	float density;
	kst_theme_t theme;
	kst_element_t* visual;
	kst_element_t** popups;
	int popup_count, popup_cap;
	kst_element_t* focus;
	kst_element_t* capture;
	kst_element_t* hover;
	kst_element_t* tooltip_target;
	float tooltip_timer;
	/** The AppBar that is open: a press outside it closes it unless it is sticky. */
	kst_element_t* open_bar;
	/** Where the flyouts shown at an element are hosted while they show: a hidden Canvas that belongs to the root. */
	kst_element_t* overlay;
	float pointer_x, pointer_y;
	int layout_dirty, paint_dirty;
	uint8_t* pixels;
	int pixels_w, pixels_h;
	int first_frame;
	KRenderer renderer;
	KAnimations animations;
	/** K_TRUE while the window of Kwindow_run draws with the OpenGL renderer. */
	int gpu_window;
	/** The clock of animations, in ms: the sum of the Kroot_tick deltas. */
	double time_ms;
	/** Tilt in progress: the pressed element, how far the finger is from its centre (-1..1), how much it leans (0..1). */
	kst_element_t* tilt_element;
	float tilt_fx, tilt_fy, tilt_amount, tilt_target;
	/** The off-screen GPU context of Kroot_render, made when it is first asked for. */
	struct kst_gl* gpu;
	/** The overrides of ThemeResource keys the application set (resources.c). */
	struct kst_res_override* overrides;
	int override_count, override_cap;
	/** The debug overlay: zero, or a mix of K_DEBUG_BOUNDS and K_DEBUG_LABELS (debug.c). */
	int debug_flags;
};

kst_root_t* kst_root_lookup(Kid id);
/**
 * Lays the tree out when it needs it and, when something changed, paints it with `p` (the clear
 * and every element). K_TRUE when it painted. The software and the GPU renderer both go through it.
 */
int kst_root_paint(kst_root_t* r, kst_painter_t* p);
/** Draws the debug overlay over the frame (debug.c): the bounds of the elements, their class names. */
void kst_debug_element(const kst_element_t* e, kst_painter_t* p, const kst_root_t* r);
/** The name Kroot_get_renderer_name reports. */
void kst_note_renderer(const char* name);

/* ---- animation and transforms (anim.c) ---- */
typedef struct kst_tween {
	struct { int active; float from, to, start, duration, delay; KEasing easing; } p[K_PROP_LAST_];
} kst_tween_t;
int kst_animations_enabled(const kst_root_t* r);
/** The window has decided on its renderer: a first page that waited swings in now, when animations run. */
void kst_root_start_intro(kst_root_t* r);
float kst_ease(KEasing easing, float t);
void kst_tweens_tick(kst_element_t* e);
void kst_tweens_free(kst_element_t* e);
void kst_update_transformed(kst_element_t* e);
void kst_xf_identity(float m[16]);
/** out = a * b: b is applied first. */
void kst_xf_mul(float out[16], const float a[16], const float b[16]);
/** The transform of an element, in device pixels, about its centre of projection; origin and size are the element's own, in device pixels. */
void kst_xf_of_element(const kst_element_t* e, float origin_x, float origin_y, float width, float height, float scale, float out[16]);
/** Sets the pressed tilt target of the root: the element leaning (NULL to let go) and where the finger is. */
void kst_tilt_press(kst_root_t* r, kst_element_t* e, float x, float y);
void kst_tilt_move(kst_root_t* r, float x, float y);
void kst_tilt_release(kst_root_t* r);
void kst_tilt_tick(kst_root_t* r, float dt);
void kst_root_attach(kst_root_t* r, kst_element_t* e);
void kst_root_detach(kst_element_t* e);
void kst_root_register_popup(kst_root_t* r, kst_element_t* popup);
void kst_root_unregister_popup(kst_root_t* r, kst_element_t* popup);
void kst_set_focus(kst_root_t* r, kst_element_t* e);
void kst_set_capture(kst_root_t* r, kst_element_t* e);
void kst_release_capture(kst_root_t* r, kst_element_t* e);
void kst_element_forget(kst_element_t* e);
int kst_dispatch(kst_element_t* e, kst_input_t* in);
void kst_paint_element(kst_element_t* e, kst_painter_t* p, kst_root_t* r);

#endif
