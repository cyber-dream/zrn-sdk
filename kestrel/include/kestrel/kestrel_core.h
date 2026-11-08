/**
 * Kestrel
 * Windows Phone 8 Silverlight controls in C.
 *
 * kestrel_core.h : base types, enums, lifecycle, errors.
 *
 * Kestrel follows System.Windows.Controls of the Windows Phone 8 SDK
 * (docs/wp8-reference/10-silverlight-controls.md): the same class
 * hierarchy, the same properties and events, the same Measure / Arrange
 * layout, drawn the way the WP8 templates draw them (Metro, Dark or Light,
 * with an accent color).
 *
 * Conventions:
 *   - every object is an opaque Kid handle, K_INVALID_ID on failure
 *   - functions are K<class>_<action>: Kbutton_create, Kcontent_control_set_text,
 *     Krange_base_set_value... A function of a base class works on every class
 *     that derives from it: Kcontrol_set_padding takes a Button, a ListBox
 *     or a TextBox; Krange_base_set_value takes a Slider, a ProgressBar or a
 *     ScrollBar (the hierarchy of the SDK is in kestrel_element.h and
 *     kestrel_controls.h)
 *   - types are KName, callbacks are KNameCallback, constants and enum
 *     values are K_NAME
 *   - booleans are ints (K_TRUE / K_FALSE), colors are KColor
 *   - strings are UTF-8 and always copied by the library
 *   - lengths are in logical pixels of a 480 x 800 phone screen
 *
 * Threading: nothing is thread-safe. Call everything from one thread.
 */

#ifndef KESTREL_CORE_H
#define KESTREL_CORE_H

#define KESTREL_VERSION_MAJOR 0
#define KESTREL_VERSION_MINOR 1
#define KESTREL_VERSION_PATCH 0

#ifdef __cplusplus
 #include <cstdint>
 #include <cstddef>
#else
 #include <stdint.h>
 #include <stddef.h>
#endif

#if defined(_WIN32) && defined(KESTREL_BUILD_DLL)
	#define KESTREL_API __declspec(dllexport)
#elif defined(_WIN32) && defined(KESTREL_USE_DLL)
	#define KESTREL_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(KESTREL_BUILD_DLL)
	#define KESTREL_API __attribute__((visibility("default")))
#else
	#define KESTREL_API
#endif


/**
 * Opaque handle to any Kestrel object: a root, an element, a control. A
 * handle carries an index and a generation counter, so using a destroyed
 * handle is detected and reported instead of crashing.
 * Both K_INVALID_ID and 0 are never valid.
 */
typedef uint32_t Kid;

#define K_INVALID_ID ((Kid)0)
#define K_TRUE 1
#define K_FALSE 0

/** "Not set" for a length (Width, Height, Canvas.Left...): Double.NaN of Silverlight. */
#define K_AUTO (-1.0f)
/** No limit for MaxWidth / MaxHeight: Double.PositiveInfinity of Silverlight. */
#define K_INFINITE (1.0e30f)


typedef struct KColor {
	float r, g, b, a;
} KColor;

/** Four edge sizes (left, top, right, bottom): Margin, Padding, BorderThickness. Windows.UI.Xaml.Thickness of 8.1. */
typedef struct KThickness {
	float left, top, right, bottom;
} KThickness;

typedef struct KCornerRadius {
	float top_left, top_right, bottom_right, bottom_left;
} KCornerRadius;

typedef struct KSize {
	float width, height;
} KSize;

typedef struct KPoint {
	float x, y;
} KPoint;

typedef struct KRect {
	float x, y, width, height;
} KRect;


typedef enum KHorizontalAlignment {
	K_HALIGN_LEFT = 0x0100,
	K_HALIGN_CENTER,
	K_HALIGN_RIGHT,
	/** The default of an element: takes all the space of its slot. */
	K_HALIGN_STRETCH
} KHorizontalAlignment;

typedef enum KVerticalAlignment {
	K_VALIGN_TOP = 0x0110,
	K_VALIGN_CENTER,
	K_VALIGN_BOTTOM,
	K_VALIGN_STRETCH
} KVerticalAlignment;

typedef enum KVisibility {
	K_VISIBLE = 0x0120,
	/** Not drawn and takes no space. Silverlight has no "hidden". */
	K_COLLAPSED
} KVisibility;

typedef enum KOrientation {
	K_HORIZONTAL = 0x0130,
	K_VERTICAL
} KOrientation;

typedef enum KTextWrapping {
	K_NO_WRAP = 0x0140,
	K_WRAP
} KTextWrapping;

typedef enum KTextAlignment {
	K_TEXT_LEFT = 0x0150,
	K_TEXT_CENTER,
	K_TEXT_RIGHT
} KTextAlignment;

/**
 * The text styles of a TextBlock: the styles of the Windows Phone 8.1 theme
 * (HeaderTextBlockStyle, BodyTextBlockStyle...), and K_TEXT_STYLE_PHONE_* the
 * PhoneText*Style of Silverlight 8.0. Ktext_block_set_style applies one;
 * a property set after it wins.
 */
typedef enum KTextStyle {
	K_TEXT_STYLE_NONE = 0x0800,
	/* Windows Phone 8.1 */
	K_TEXT_STYLE_BASE,
	K_TEXT_STYLE_BODY,
	K_TEXT_STYLE_HEADER,
	K_TEXT_STYLE_SUBHEADER,
	K_TEXT_STYLE_TITLE,
	K_TEXT_STYLE_GROUP_HEADER,
	K_TEXT_STYLE_CONTROL_HEADER,
	K_TEXT_STYLE_CONTROL_CONTEXTUAL_INFO,
	K_TEXT_STYLE_FLYOUT_PICKER_TITLE,
	K_TEXT_STYLE_LIST_VIEW_ITEM,
	K_TEXT_STYLE_LIST_VIEW_EMPTY_STATIC,
	K_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER,
	K_TEXT_STYLE_LIST_VIEW_ITEM_CONTENT,
	K_TEXT_STYLE_MESSAGE_DIALOG_TITLE,
	K_TEXT_STYLE_MESSAGE_DIALOG_CONTENT,
	K_TEXT_STYLE_COMBO_BOX_PLACEHOLDER,
	/* Windows Phone 8.0 Silverlight */
	K_TEXT_STYLE_PHONE_NORMAL,
	K_TEXT_STYLE_PHONE_SUBTLE,
	K_TEXT_STYLE_PHONE_TITLE1,
	K_TEXT_STYLE_PHONE_TITLE2,
	K_TEXT_STYLE_PHONE_TITLE3,
	K_TEXT_STYLE_PHONE_EXTRA_LARGE,
	K_TEXT_STYLE_PHONE_GROUP_HEADER,
	K_TEXT_STYLE_PHONE_LARGE,
	K_TEXT_STYLE_PHONE_SMALL,
	K_TEXT_STYLE_PHONE_CONTRAST,
	K_TEXT_STYLE_PHONE_ACCENT,
	K_TEXT_STYLE_PHONE_HUGE,
	K_TEXT_STYLE_LAST_
} KTextStyle;

typedef enum KTextTrimming {
	K_TRIM_NONE = 0x0160,
	K_TRIM_WORD_ELLIPSIS
} KTextTrimming;

typedef enum KStretch {
	/** Original size. */
	K_STRETCH_NONE = 0x0170,
	/** Fills the space, the aspect ratio is not kept. */
	K_STRETCH_FILL,
	/** The largest size that fits, aspect ratio kept. */
	K_STRETCH_UNIFORM,
	/** The smallest size that covers, aspect ratio kept (cropped). */
	K_STRETCH_UNIFORM_TO_FILL
} KStretch;

typedef enum KStretchDirection {
	K_STRETCH_UP_ONLY = 0x0180,
	K_STRETCH_DOWN_ONLY,
	K_STRETCH_BOTH
} KStretchDirection;

typedef enum KScrollBarVisibility {
	K_SCROLLBAR_DISABLED = 0x0190,
	K_SCROLLBAR_AUTO,
	K_SCROLLBAR_HIDDEN,
	K_SCROLLBAR_VISIBLE
} KScrollBarVisibility;

typedef enum KClickMode {
	K_CLICK_RELEASE = 0x01A0,
	K_CLICK_PRESS,
	K_CLICK_HOVER
} KClickMode;

typedef enum KSelectionMode {
	K_SELECTION_SINGLE = 0x01B0,
	K_SELECTION_MULTIPLE,
	K_SELECTION_EXTENDED
} KSelectionMode;

typedef enum KGridUnitType {
	K_GRID_AUTO = 0x01C0,
	K_GRID_PIXEL,
	K_GRID_STAR
} KGridUnitType;

typedef enum KScrollEventType {
	K_SCROLL_SMALL_DECREMENT = 0x01D0,
	K_SCROLL_SMALL_INCREMENT,
	K_SCROLL_LARGE_DECREMENT,
	K_SCROLL_LARGE_INCREMENT,
	K_SCROLL_THUMB_POSITION,
	K_SCROLL_THUMB_TRACK,
	K_SCROLL_FIRST,
	K_SCROLL_LAST,
	K_SCROLL_END
} KScrollEventType;

typedef enum KPlacementMode {
	K_PLACE_BOTTOM = 0x01E0,
	K_PLACE_TOP,
	K_PLACE_LEFT,
	K_PLACE_RIGHT,
	K_PLACE_MOUSE
} KPlacementMode;

/** ListViewBase.SelectionMode (Windows.UI.Xaml.Controls.ListViewSelectionMode). ListBox has the three-valued KSelectionMode above. */
typedef enum KListViewSelectionMode {
	K_LIST_VIEW_SELECTION_NONE = 0x0280,
	K_LIST_VIEW_SELECTION_SINGLE,
	K_LIST_VIEW_SELECTION_MULTIPLE,
	K_LIST_VIEW_SELECTION_EXTENDED
} KListViewSelectionMode;

/** Why the text of an AutoSuggestBox changed (AutoSuggestionBoxTextChangeReason). */
typedef enum KAutoSuggestionBoxTextChangeReason {
	K_SUGGEST_USER_INPUT = 0x0290,
	K_SUGGEST_PROGRAMMATIC_CHANGE,
	K_SUGGEST_SUGGESTION_CHOSEN
} KAutoSuggestionBoxTextChangeReason;

/** ListViewBase.ScrollIntoView: where the item ends up. Default only brings it into view, Leading puts it at the start of the viewport. */
typedef enum KScrollIntoViewAlignment {
	K_SCROLL_INTO_VIEW_DEFAULT = 0x02A0,
	K_SCROLL_INTO_VIEW_LEADING
} KScrollIntoViewAlignment;

/** Slider.TickPlacement: where the tick marks are drawn (with a TickFrequency above 0). */
typedef enum KTickPlacement {
	K_TICK_NONE = 0x0260,
	K_TICK_TOP_LEFT,
	K_TICK_BOTTOM_RIGHT,
	K_TICK_OUTSIDE,       /* both sides of the track: the default */
	K_TICK_INLINE         /* over the track */
} KTickPlacement;

/** Slider.SnapsTo: what the value of a touch or a key lands on. */
typedef enum KSliderSnapsTo {
	K_SNAPS_TO_STEP_VALUES = 0x0270,
	K_SNAPS_TO_TICKS
} KSliderSnapsTo;

/** Frame.Navigate, GoBack, GoForward: how the page was reached (Windows.UI.Xaml.Navigation.NavigationMode). */
typedef enum KNavigationMode {
	K_NAVIGATION_NEW = 0x02B0,
	K_NAVIGATION_BACK,
	K_NAVIGATION_FORWARD,
	K_NAVIGATION_REFRESH
} KNavigationMode;

/** Page.NavigationCacheMode: whether a page is kept alive when the frame leaves it (Windows.UI.Xaml.Navigation.NavigationCacheMode). */
typedef enum KNavigationCacheMode {
	K_NAVIGATION_CACHE_DISABLED = 0x02C0,
	K_NAVIGATION_CACHE_REQUIRED,
	K_NAVIGATION_CACHE_ENABLED
} KNavigationCacheMode;

/** AppBar.ClosedDisplayMode (Windows.UI.Xaml.Controls.AppBarClosedDisplayMode: the 8.1 enum has these two values only). */
typedef enum KAppBarClosedDisplayMode {
	K_APP_BAR_CLOSED_COMPACT = 0x02D0,
	K_APP_BAR_CLOSED_MINIMAL
} KAppBarClosedDisplayMode;

/** FlyoutBase.Placement (Windows.UI.Xaml.Controls.Primitives.FlyoutPlacementMode). */
typedef enum KFlyoutPlacementMode {
	K_FLYOUT_PLACEMENT_TOP = 0x02E0,
	K_FLYOUT_PLACEMENT_BOTTOM,
	K_FLYOUT_PLACEMENT_LEFT,
	K_FLYOUT_PLACEMENT_RIGHT,
	K_FLYOUT_PLACEMENT_FULL
} KFlyoutPlacementMode;

/** What closed a ContentDialog (Windows.UI.Xaml.Controls.ContentDialogResult). */
typedef enum KContentDialogResult {
	K_CONTENT_DIALOG_RESULT_NONE = 0x02F0,
	K_CONTENT_DIALOG_RESULT_PRIMARY,
	K_CONTENT_DIALOG_RESULT_SECONDARY
} KContentDialogResult;

/** SymbolIcon.Symbol (Windows.UI.Xaml.Controls.Symbol): the values are the code points of the 8.1 symbol font. */
typedef enum KSymbol {
	K_SYMBOL_PREVIOUS = 57600,
	K_SYMBOL_NEXT = 57601,
	K_SYMBOL_PLAY = 57602,
	K_SYMBOL_PAUSE = 57603,
	K_SYMBOL_EDIT = 57604,
	K_SYMBOL_SAVE = 57605,
	K_SYMBOL_CLEAR = 57606,
	K_SYMBOL_DELETE = 57607,
	K_SYMBOL_REMOVE = 57608,
	K_SYMBOL_ADD = 57609,
	K_SYMBOL_CANCEL = 57610,
	K_SYMBOL_ACCEPT = 57611,
	K_SYMBOL_MORE = 57612,
	K_SYMBOL_REDO = 57613,
	K_SYMBOL_UNDO = 57614,
	K_SYMBOL_HOME = 57615,
	K_SYMBOL_UP = 57616,
	K_SYMBOL_FORWARD = 57617,
	K_SYMBOL_BACK = 57618,
	K_SYMBOL_FAVORITE = 57619,
	K_SYMBOL_CAMERA = 57620,
	K_SYMBOL_SETTING = 57621,
	K_SYMBOL_VIDEO = 57622,
	K_SYMBOL_SYNC = 57623,
	K_SYMBOL_DOWNLOAD = 57624,
	K_SYMBOL_MAIL = 57625,
	K_SYMBOL_FIND = 57626,
	K_SYMBOL_HELP = 57627,
	K_SYMBOL_UPLOAD = 57628,
	K_SYMBOL_EMOJI = 57629,
	K_SYMBOL_TWO_PAGE = 57630,
	K_SYMBOL_LEAVE_CHAT = 57631,
	K_SYMBOL_MAIL_FORWARD = 57632,
	K_SYMBOL_CLOCK = 57633,
	K_SYMBOL_SEND = 57634,
	K_SYMBOL_CROP = 57635,
	K_SYMBOL_ROTATE_CAMERA = 57636,
	K_SYMBOL_PEOPLE = 57637,
	K_SYMBOL_OPEN_PANE = 57638,
	K_SYMBOL_CLOSE_PANE = 57639,
	K_SYMBOL_WORLD = 57640,
	K_SYMBOL_FLAG = 57641,
	K_SYMBOL_PREVIEW_LINK = 57642,
	K_SYMBOL_GLOBE = 57643,
	K_SYMBOL_TRIM = 57644,
	K_SYMBOL_ATTACH_CAMERA = 57645,
	K_SYMBOL_ZOOM_IN = 57646,
	K_SYMBOL_BOOKMARKS = 57647,
	K_SYMBOL_DOCUMENT = 57648,
	K_SYMBOL_PROTECTED_DOCUMENT = 57649,
	K_SYMBOL_PAGE = 57650,
	K_SYMBOL_BULLETS = 57651,
	K_SYMBOL_COMMENT = 57652,
	K_SYMBOL_MAIL_FILLED = 57653,
	K_SYMBOL_CONTACT_INFO = 57654,
	K_SYMBOL_HANG_UP = 57655,
	K_SYMBOL_VIEW_ALL = 57656,
	K_SYMBOL_MAP_PIN = 57657,
	K_SYMBOL_PHONE = 57658,
	K_SYMBOL_VIDEO_CHAT = 57659,
	K_SYMBOL_SWITCH = 57660,
	K_SYMBOL_CONTACT = 57661,
	K_SYMBOL_RENAME = 57662,
	K_SYMBOL_PIN = 57665,
	K_SYMBOL_MUSIC_INFO = 57666,
	K_SYMBOL_GO = 57667,
	K_SYMBOL_KEYBOARD = 57668,
	K_SYMBOL_DOCK_LEFT = 57669,
	K_SYMBOL_DOCK_RIGHT = 57670,
	K_SYMBOL_DOCK_BOTTOM = 57671,
	K_SYMBOL_REMOTE = 57672,
	K_SYMBOL_REFRESH = 57673,
	K_SYMBOL_ROTATE = 57674,
	K_SYMBOL_SHUFFLE = 57675,
	K_SYMBOL_LIST = 57676,
	K_SYMBOL_SHOP = 57677,
	K_SYMBOL_SELECT_ALL = 57678,
	K_SYMBOL_ORIENTATION = 57679,
	K_SYMBOL_IMPORT = 57680,
	K_SYMBOL_IMPORT_ALL = 57681,
	K_SYMBOL_BROWSE_PHOTOS = 57685,
	K_SYMBOL_WEB_CAM = 57686,
	K_SYMBOL_PICTURES = 57688,
	K_SYMBOL_SAVE_LOCAL = 57689,
	K_SYMBOL_CAPTION = 57690,
	K_SYMBOL_STOP = 57691,
	K_SYMBOL_SHOW_RESULTS = 57692,
	K_SYMBOL_VOLUME = 57693,
	K_SYMBOL_REPAIR = 57694,
	K_SYMBOL_MESSAGE = 57695,
	K_SYMBOL_PAGE2 = 57696,
	K_SYMBOL_CALENDAR_DAY = 57697,
	K_SYMBOL_CALENDAR_WEEK = 57698,
	K_SYMBOL_CALENDAR = 57699,
	K_SYMBOL_CHARACTER = 57700,
	K_SYMBOL_MAIL_REPLY_ALL = 57701,
	K_SYMBOL_READ = 57702,
	K_SYMBOL_LINK = 57703,
	K_SYMBOL_ACCOUNT = 57704,
	K_SYMBOL_SHOW_BCC = 57705,
	K_SYMBOL_HIDE_BCC = 57706,
	K_SYMBOL_CUT = 57707,
	K_SYMBOL_ATTACH = 57708,
	K_SYMBOL_PASTE = 57709,
	K_SYMBOL_FILTER = 57710,
	K_SYMBOL_COPY = 57711,
	K_SYMBOL_EMOJI2 = 57712,
	K_SYMBOL_IMPORTANT = 57713,
	K_SYMBOL_MAIL_REPLY = 57714,
	K_SYMBOL_SLIDE_SHOW = 57715,
	K_SYMBOL_SORT = 57716,
	K_SYMBOL_MANAGE = 57720,
	K_SYMBOL_ALL_APPS = 57721,
	K_SYMBOL_DISCONNECT_DRIVE = 57722,
	K_SYMBOL_MAP_DRIVE = 57723,
	K_SYMBOL_NEW_WINDOW = 57724,
	K_SYMBOL_OPEN_WITH = 57725,
	K_SYMBOL_CONTACT_PRESENCE = 57729,
	K_SYMBOL_PRIORITY = 57730,
	K_SYMBOL_GO_TO_TODAY = 57732,
	K_SYMBOL_FONT = 57733,
	K_SYMBOL_FONT_COLOR = 57734,
	K_SYMBOL_CONTACT2 = 57735,
	K_SYMBOL_FOLDER = 57736,
	K_SYMBOL_AUDIO = 57737,
	K_SYMBOL_PLACEHOLDER = 57738,
	K_SYMBOL_VIEW = 57739,
	K_SYMBOL_SET_LOCK_SCREEN = 57740,
	K_SYMBOL_SET_TILE = 57741,
	K_SYMBOL_CLOSED_CAPTION = 57744,
	K_SYMBOL_STOP_SLIDE_SHOW = 57745,
	K_SYMBOL_PERMISSIONS = 57746,
	K_SYMBOL_HIGHLIGHT = 57747,
	K_SYMBOL_DISABLE_UPDATES = 57748,
	K_SYMBOL_UN_FAVORITE = 57749,
	K_SYMBOL_UN_PIN = 57750,
	K_SYMBOL_OPEN_LOCAL = 57751,
	K_SYMBOL_MUTE = 57752,
	K_SYMBOL_ITALIC = 57753,
	K_SYMBOL_UNDERLINE = 57754,
	K_SYMBOL_BOLD = 57755,
	K_SYMBOL_MOVE_TO_FOLDER = 57756,
	K_SYMBOL_LIKE_DISLIKE = 57757,
	K_SYMBOL_DISLIKE = 57758,
	K_SYMBOL_LIKE = 57759,
	K_SYMBOL_ALIGN_RIGHT = 57760,
	K_SYMBOL_ALIGN_CENTER = 57761,
	K_SYMBOL_ALIGN_LEFT = 57762,
	K_SYMBOL_ZOOM = 57763,
	K_SYMBOL_ZOOM_OUT = 57764,
	K_SYMBOL_OPEN_FILE = 57765,
	K_SYMBOL_OTHER_USER = 57766,
	K_SYMBOL_ADMIN = 57767,
	K_SYMBOL_STREET = 57795,
	K_SYMBOL_MAP = 57796,
	K_SYMBOL_CLEAR_SELECTION = 57797,
	K_SYMBOL_FONT_DECREASE = 57798,
	K_SYMBOL_FONT_INCREASE = 57799,
	K_SYMBOL_FONT_SIZE = 57800,
	K_SYMBOL_CELL_PHONE = 57801,
	K_SYMBOL_RE_SHARE = 57802,
	K_SYMBOL_TAG = 57803,
	K_SYMBOL_REPEAT_ONE = 57804,
	K_SYMBOL_REPEAT_ALL = 57805,
	K_SYMBOL_OUTLINE_STAR = 57806,
	K_SYMBOL_SOLID_STAR = 57807,
	K_SYMBOL_CALCULATOR = 57808,
	K_SYMBOL_DIRECTIONS = 57809,
	K_SYMBOL_TARGET = 57810,
	K_SYMBOL_LIBRARY = 57811,
	K_SYMBOL_PHONE_BOOK = 57812,
	K_SYMBOL_MEMO = 57813,
	K_SYMBOL_MICROPHONE = 57814,
	K_SYMBOL_POST_UPDATE = 57815,
	K_SYMBOL_BACK_TO_WINDOW = 57816,
	K_SYMBOL_FULL_SCREEN = 57817,
	K_SYMBOL_NEW_FOLDER = 57818,
	K_SYMBOL_CALENDAR_REPLY = 57819,
	K_SYMBOL_UN_SYNC_FOLDER = 57821,
	K_SYMBOL_REPORT_HACKED = 57822,
	K_SYMBOL_SYNC_FOLDER = 57823,
	K_SYMBOL_BLOCK_CONTACT = 57824,
	K_SYMBOL_SWITCH_APPS = 57825,
	K_SYMBOL_ADD_FRIEND = 57826,
	K_SYMBOL_TOUCH_POINTER = 57827,
	K_SYMBOL_GO_TO_START = 57828,
	K_SYMBOL_ZERO_BARS = 57829,
	K_SYMBOL_ONE_BAR = 57830,
	K_SYMBOL_TWO_BARS = 57831,
	K_SYMBOL_THREE_BARS = 57832,
	K_SYMBOL_FOUR_BARS = 57833,
	K_SYMBOL_SCAN = 58004,
	K_SYMBOL_PREVIEW = 58005
} KSymbol;

/** The tri-state of ToggleButton.IsChecked. */
typedef enum KToggleState {
	K_UNCHECKED = 0x0200,
	K_CHECKED,
	K_INDETERMINATE
} KToggleState;

/** The WP8 font families (themes.md): "Segoe WP" and its weights. */
typedef enum KFontFamily {
	K_FONT_NORMAL = 0x0210,
	K_FONT_LIGHT,
	K_FONT_SEMILIGHT,
	K_FONT_SEMIBOLD,
	K_FONT_BOLD
} KFontFamily;

typedef enum KTheme {
	K_THEME_DARK = 0x0220,
	K_THEME_LIGHT
} KTheme;

/** The 20 accent colors of Windows Phone 8. */
typedef enum KAccent {
	K_ACCENT_LIME = 0x0230,
	K_ACCENT_GREEN,
	K_ACCENT_EMERALD,
	K_ACCENT_TEAL,
	K_ACCENT_CYAN,
	K_ACCENT_COBALT,
	K_ACCENT_INDIGO,
	K_ACCENT_VIOLET,
	K_ACCENT_PINK,
	K_ACCENT_MAGENTA,
	K_ACCENT_CRIMSON,
	K_ACCENT_RED,
	K_ACCENT_ORANGE,
	K_ACCENT_AMBER,
	K_ACCENT_YELLOW,
	K_ACCENT_BROWN,
	K_ACCENT_OLIVE,
	K_ACCENT_STEEL,
	K_ACCENT_MAUVE,
	K_ACCENT_TAUPE
} KAccent;

/**
 * Every class of the library. Kelement_get_class returns the class of an
 * object; Kelement_is_a tells whether it is, or derives from, a class.
 */
typedef enum KAnimations {
	K_ANIMATIONS_AUTO = 0,
	K_ANIMATIONS_OFF,
	K_ANIMATIONS_ON
} KAnimations;

typedef enum KRenderer {
	K_RENDERER_AUTO = 0,
	K_RENDERER_SOFTWARE,
	K_RENDERER_GPU
} KRenderer;

typedef enum KClass {
	K_CLASS_ELEMENT = 0x0300,          /* UIElement + FrameworkElement */
	K_CLASS_PANEL,                     /* abstract */
	K_CLASS_STACK_PANEL,
	K_CLASS_VIRTUALIZING_STACK_PANEL,
	K_CLASS_WRAP_PANEL,
	K_CLASS_GRID,
	K_CLASS_CANVAS,
	K_CLASS_BORDER,
	K_CLASS_VIEWBOX,
	K_CLASS_TEXT_BLOCK,
	K_CLASS_IMAGE,
	K_CLASS_POPUP,
	K_CLASS_CONTROL,                   /* abstract */
	K_CLASS_USER_CONTROL,
	K_CLASS_CONTENT_CONTROL,
	K_CLASS_TOOLTIP,
	K_CLASS_BUTTON_BASE,               /* abstract */
	K_CLASS_BUTTON,
	K_CLASS_HYPERLINK_BUTTON,
	K_CLASS_REPEAT_BUTTON,
	K_CLASS_TOGGLE_BUTTON,
	K_CLASS_CHECKBOX,
	K_CLASS_RADIO_BUTTON,
	K_CLASS_RANGE_BASE,                /* abstract */
	K_CLASS_SLIDER,
	K_CLASS_PROGRESS_BAR,
	K_CLASS_SCROLLBAR,
	K_CLASS_THUMB,
	K_CLASS_SEPARATOR,
	K_CLASS_SCROLL_VIEWER,
	K_CLASS_TEXT_BOX,
	K_CLASS_PASSWORD_BOX,
	K_CLASS_ITEMS_CONTROL,
	K_CLASS_SELECTOR,                  /* abstract */
	K_CLASS_SELECTOR_ITEM,             /* abstract: IsSelected */
	K_CLASS_LIST_BOX,
	K_CLASS_LIST_BOX_ITEM,
	K_CLASS_COMBO_BOX,
	K_CLASS_COMBO_BOX_ITEM,
	K_CLASS_AUTO_SUGGEST_BOX,
	K_CLASS_FRAME,                     /* a ContentControl that navigates between pages */
	K_CLASS_PAGE,                      /* a UserControl a Frame shows */
	K_CLASS_PIVOT,                     /* headers over pages that slide */
	K_CLASS_TOGGLE_SWITCH,             /* an on/off switch with a header */
	K_CLASS_PROGRESS_RING,             /* the dots that run round while something loads */
	K_CLASS_LIST_VIEW_BASE,            /* abstract: SelectionMode, ItemClick, Header, Footer */
	K_CLASS_LIST_VIEW,
	K_CLASS_GRID_VIEW,
	K_CLASS_LIST_VIEW_ITEM,
	K_CLASS_GRID_VIEW_ITEM,
	K_CLASS_LIST_VIEW_BASE_HEADER_ITEM, /* abstract: the header of a group */
	K_CLASS_LIST_VIEW_HEADER_ITEM,
	K_CLASS_GRID_VIEW_HEADER_ITEM,
	K_CLASS_FLIP_VIEW,
	K_CLASS_FLIP_VIEW_ITEM,
	K_CLASS_ITEMS_STACK_PANEL,
	K_CLASS_ITEMS_WRAP_GRID,
	K_CLASS_SEMANTIC_ZOOM,
	K_CLASS_ICON_ELEMENT,              /* abstract: the icon of an app bar button */
	K_CLASS_SYMBOL_ICON,
	K_CLASS_FONT_ICON,
	K_CLASS_PATH_ICON,
	K_CLASS_BITMAP_ICON,
	K_CLASS_APP_BAR,                   /* a ContentControl the page shows at its top or bottom: IsOpen, IsSticky */
	K_CLASS_COMMAND_BAR,
	K_CLASS_APP_BAR_BUTTON,
	K_CLASS_APP_BAR_TOGGLE_BUTTON,
	K_CLASS_APP_BAR_SEPARATOR,
	K_CLASS_FLYOUT_BASE,               /* abstract: a thing that shows above the page next to an element */
	K_CLASS_FLYOUT,
	K_CLASS_MENU_FLYOUT,
	K_CLASS_MENU_FLYOUT_ITEM_BASE,     /* abstract */
	K_CLASS_MENU_FLYOUT_ITEM,
	K_CLASS_TOGGLE_MENU_FLYOUT_ITEM,
	K_CLASS_MENU_FLYOUT_SEPARATOR,
	K_CLASS_CONTENT_DIALOG,
	K_CLASS_PIVOT_ITEM,                /* a ContentControl with a Header: the page of a Pivot */
	K_CLASS_HUB,                       /* sections side by side in a scroller, over a big header */
	K_CLASS_HUB_SECTION,               /* a section of a Hub: a header over its content */
	K_CLASS_LOOPING_SELECTOR,          /* a column of items that goes round, with the chosen one in the middle */
	K_CLASS_DATE_PICKER,               /* a header over a button with the date; a tap opens the DatePickerFlyout */
	K_CLASS_TIME_PICKER,
	K_CLASS_PICKER_FLYOUT_BASE,        /* abstract: a full screen flyout with Done and Cancel */
	K_CLASS_DATE_PICKER_FLYOUT,
	K_CLASS_TIME_PICKER_FLYOUT,
	K_CLASS_RICH_TEXT_BLOCK,           /* paragraphs of plain text */
	K_CLASS_ROOT,                      /* the host of a visual tree, kestrel_host.h */
	K_CLASS_LAST_
} KClass;

typedef enum KError {
	K_OK = 0,
	K_ERROR_INVALID_ID = 1,
	K_ERROR_WRONG_CLASS,
	K_ERROR_INVALID_ARGUMENT,
	K_ERROR_OUT_OF_MEMORY,
	K_ERROR_IO,
	K_ERROR_CYCLE,
	K_ERROR_HAS_PARENT
} KError;


#ifdef __cplusplus
extern "C" {
#endif

	/** @brief Starts the library (fonts). Safe to call twice; nested with Kshutdown. */
	KESTREL_API int Kinit(void);

	/** @brief Destroys every root and every element, frees the fonts. */
	KESTREL_API void Kshutdown(void);

	/** @brief Version as "major.minor.patch". */
	KESTREL_API const char* Kget_version(void);

	/** @brief The last error of the calling thread: K_OK when there was none. */
	KESTREL_API KError Kget_error(void);
	KESTREL_API const char* Kget_error_message(void);

	/**
	 * @brief Called when a function fails (invalid handle, wrong class...).
	 * By default the message is printed to stderr.
	 */
	typedef void (*KErrorCallback)(KError error, const char* message, void* user_data);
	KESTREL_API void Kset_error_callback(KErrorCallback callback, void* user_data);

	/**
	 * @brief Directory of the Selawik fonts (Selawik-Regular.ttf,
	 * Selawik-Semilight.ttf, Selawik-Semibold.ttf), the free fonts that
	 * metric-match Segoe UI and stand in for Segoe WP. Call before Kinit.
	 * Default: $KESTREL_FONT_DIR, then the directory compiled in.
	 */
	KESTREL_API void Kset_font_directory(const char* path);

	KESTREL_API KColor Kcolor_rgba(float r, float g, float b, float a);
	/** @brief 0xAARRGGBB, the notation of the WP8 theme files. */
	KESTREL_API KColor Kcolor_argb(uint32_t argb);
	KESTREL_API KThickness Kthickness(float left, float top, float right, float bottom);
	KESTREL_API KThickness Kthickness_uniform(float all);

#ifdef __cplusplus
}
#endif

#endif
