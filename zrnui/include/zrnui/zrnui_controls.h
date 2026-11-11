/**
 * zrnui
 * zrnui_controls.h : Control and everything that derives from it.
 *
 *   Control                              Foreground, Background, BorderBrush,
 *                                        BorderThickness, Padding, Font*
 *     UserControl
 *     ContentControl                     Content: a string or an element
 *       ToolTip
 *       ButtonBase                       ClickMode, IsPressed, Click
 *         Button                         Flyout
 *           AppBarButton                 Label, Icon, IsCompact
 *         HyperlinkButton, RepeatButton
 *         ToggleButton                   IsChecked, IsThreeState
 *           CheckBox, RadioButton        GroupName
 *           AppBarToggleButton           Label, Icon, IsCompact
 *       ContentDialog                    Title, PrimaryButtonText, SecondaryButtonText, Show, Hide
 *       PivotItem                        Header: the page of a Pivot
 *       HubSection                       Header, IsHeaderInteractive: a section of a Hub
 *       SelectorItem                     IsSelected
 *         ListBoxItem, ComboBoxItem, ListViewItem, GridViewItem, FlipViewItem
 *       ListViewBaseHeaderItem           the header of a group
 *         ListViewHeaderItem, GridViewHeaderItem
 *     AppBar                             IsOpen, IsSticky, ClosedDisplayMode, Opened, Closed
 *       CommandBar                       PrimaryCommands, SecondaryCommands
 *     AppBarSeparator
 *     MenuFlyoutItemBase                 (an item of a MenuFlyout)
 *       MenuFlyoutItem, MenuFlyoutSeparator
 *         ToggleMenuFlyoutItem
 *     ToggleSwitch                       IsOn, Header, OnContent, OffContent, Toggled
 *     ProgressRing                       IsActive
 *     RangeBase                          Minimum, Maximum, Value
 *       Slider, ProgressBar, ScrollBar
 *     Thumb                              dragging
 *     Separator
 *     ScrollViewer
 *     TextBox, PasswordBox
 *     ItemsControl                       Items, ItemsPanel
 *       Selector                         SelectedIndex, SelectionChanged
 *         ListBox                        SelectionMode
 *         ComboBox                       Header, PlaceholderText, IsDropDownOpen
 *         FlipView                       a page at a time
 *         ListViewBase                   SelectionMode, IsItemClickEnabled, Header, Footer
 *           ListView, GridView
 *       AutoSuggestBox                   Text, PlaceholderText, the suggestions
 *     SemanticZoom                       two views of the same data
 *     Pivot                              Title, SelectedIndex, IsLocked: PivotItems under a strip of headers
 *     Hub                                Header, Orientation, ScrollToSection: HubSections side by side
 *     LoopingSelector                    Items, SelectedIndex, ShouldLoop
 *     DatePicker, TimePicker             Header, Date / Time: a button that shows the flyout below
 *   IconElement (not a control)          SymbolIcon, FontIcon, PathIcon, BitmapIcon
 *   FlyoutBase (not an element)          Placement, ShowAt, Hide, Opening, Opened, Closed
 *     Flyout                             Content
 *     MenuFlyout                         Items
 *     PickerFlyoutBase                   Title, with the selectors and Done / Cancel
 *       DatePickerFlyout, TimePickerFlyout
 *
 * Button, HyperlinkButton, RepeatButton, ToggleButton, CheckBox, RadioButton,
 * Slider, ProgressBar, ProgressRing, ToggleSwitch, TextBox, PasswordBox, ListBox,
 * ComboBox, ListView, GridView, FlipView, AutoSuggestBox, Pivot, Hub, LoopingSelector, DatePicker and TimePicker look and behave like the controls of the Windows Phone 8.1 templates (generic.xaml
 * of the phone SDK), with the numbers of the 8.1 XAML as they are: a Button is a
 * 2.5 px outline, at least 109 x 57.5, that fills with the accent while pressed;
 * a CheckBox is a 25.5 px square with a check path; a TextBox a header over a
 * 34 px box; a ListView item is a row with no padding of 12 px text. The other
 * controls here are the WP8 ones still. The templates are
 * not exposed: a control draws itself, and the properties below are the ones
 * the templates bind to.
 *
 * Colors that were not set follow the theme (zrnui_host.h): changing the
 * theme or the accent of the root restyles every control that has none.
 */

#ifndef ZRNUI_CONTROLS_H
#define ZRNUI_CONTROLS_H

#include "zrnui_element.h"


/** ValueChanged of a RangeBase. */
typedef void (*ZValueChangedCallback)(Zid sender, double old_value, double new_value, void* user_data);
/** Scroll of a ScrollBar (and of a ScrollViewer). */
typedef void (*ZScrollCallback)(Zid sender, ZScrollEventType type, double new_value, void* user_data);
/** DragStarted / DragDelta / DragCompleted of a Thumb. */
typedef void (*ZDragCallback)(Zid sender, float horizontal_change, float vertical_change, void* user_data);
/** ItemClick of a ListView or a GridView: `index` is the index of the item that was tapped (ItemClickEventArgs.ClickedItem). */
typedef void (*ZItemClickCallback)(Zid sender, int index, void* user_data);
/** TextChanged of an AutoSuggestBox: why the text changed. A suggestion that was chosen comes as Z_SUGGEST_SUGGESTION_CHOSEN. */
typedef void (*ZAutoSuggestTextChangedCallback)(Zid sender, ZAutoSuggestionBoxTextChangeReason reason, void* user_data);
/** SuggestionChosen of an AutoSuggestBox: `index` of the suggestion in the list (SelectedItem). */
typedef void (*ZAutoSuggestSuggestionChosenCallback)(Zid sender, int index, void* user_data);
/** QuerySubmitted of an AutoSuggestBox (Enter, or a tap on a suggestion): the text and the suggestion chosen, -1 when none. */
typedef void (*ZAutoSuggestQuerySubmittedCallback)(Zid sender, const char* query_text, int chosen_index, void* user_data);
/** ContentDialog.ShowAsync has finished: what closed the dialog. */
typedef void (*ZContentDialogResultCallback)(Zid dialog, ZContentDialogResult result, void* user_data);
/** ViewChangeStarted / ViewChangeCompleted of a SemanticZoom: whether the view that is being left (was left) is the zoomed in one. */
typedef void (*ZSemanticZoomCallback)(Zid sender, int is_source_zoomed_in_view, void* user_data);


/** A calendar date, Gregorian: `month` 1 to 12, `day` 1 to the days of the month. */
typedef struct ZDate {
	int year, month, day;
} ZDate;

/** A time of day, 24 hour clock: `hour` 0 to 23, `minute` 0 to 59. */
typedef struct ZTimeOfDay {
	int hour, minute;
} ZTimeOfDay;

/** ClockIdentifier of a TimePicker. */
typedef enum ZClockIdentifier {
	Z_CLOCK_12_HOUR = 0x0700,
	Z_CLOCK_24_HOUR
} ZClockIdentifier;

/** DateChanged of a DatePicker (DatePickerValueChangedEventArgs): the old and the new date. */
typedef void (*ZDateChangedCallback)(Zid sender, ZDate old_date, ZDate new_date, void* user_data);
/** DatePicked of a DatePickerFlyout (DatePickedEventArgs). */
typedef void (*ZDatePickedCallback)(Zid sender, ZDate old_date, ZDate new_date, void* user_data);
/** TimeChanged of a TimePicker. */
typedef void (*ZTimeChangedCallback)(Zid sender, ZTimeOfDay old_time, ZTimeOfDay new_time, void* user_data);
/** TimePicked of a TimePickerFlyout. */
typedef void (*ZTimePickedCallback)(Zid sender, ZTimeOfDay old_time, ZTimeOfDay new_time, void* user_data);


#ifdef __cplusplus
extern "C" {
#endif

	/* ============================================================================
	 *  CONTROL
	 * ============================================================================ */

	/**
	 * Visual properties of every control. A color or a size that was never
	 * set comes from the template: Foreground white in Dark, the accent for
	 * a ProgressBar... Zcontrol_reset_* is not needed: set a color to take
	 * it, create the control again to drop it.
	 */
	ZRNUI_API void Zcontrol_set_foreground(Zid control, ZColor color);
	ZRNUI_API void Zcontrol_set_background(Zid control, ZColor color);
	ZRNUI_API void Zcontrol_set_border_brush(Zid control, ZColor color);
	ZRNUI_API void Zcontrol_set_border_thickness(Zid control, ZThickness thickness);
	ZRNUI_API void Zcontrol_set_padding(Zid control, ZThickness padding);
	/**
	 * @brief {ThemeResource key} for a property of a control: Foreground, Background and BorderBrush take a color key,
	 * Padding and BorderThickness a Thickness key, FontSize a Double key. Follows the theme, the accent and the overrides
	 * of the root; the direct setter removes the binding. @return false for a Zid that is not a control or a key of another type.
	 */
	ZRNUI_API int Zcontrol_bind_foreground(Zid control, ZThemeResource key);
	ZRNUI_API int Zcontrol_bind_background(Zid control, ZThemeResource key);
	ZRNUI_API int Zcontrol_bind_border_brush(Zid control, ZThemeResource key);
	ZRNUI_API int Zcontrol_bind_border_thickness(Zid control, ZThemeResource key);
	ZRNUI_API int Zcontrol_bind_padding(Zid control, ZThemeResource key);
	ZRNUI_API int Zcontrol_bind_font_size(Zid control, ZThemeResource key);
	ZRNUI_API ZThickness Zcontrol_get_padding(Zid control);
	ZRNUI_API void Zcontrol_set_font_size(Zid control, float size);
	ZRNUI_API float Zcontrol_get_font_size(Zid control);
	ZRNUI_API void Zcontrol_set_font_family(Zid control, ZFontFamily family);
	ZRNUI_API void Zcontrol_set_horizontal_content_alignment(Zid control, ZHorizontalAlignment alignment);
	ZRNUI_API void Zcontrol_set_vertical_content_alignment(Zid control, ZVerticalAlignment alignment);
	/** @brief IsTabStop: false keeps the control out of the Tab order. */
	ZRNUI_API void Zcontrol_set_tab_stop(Zid control, int is_tab_stop);
	ZRNUI_API void Zcontrol_set_tab_index(Zid control, int tab_index);

	ZRNUI_API Zid Zuser_control_create(void);
	/** @brief UserControl.Content: its one child. Replacing it destroys the old one. */
	ZRNUI_API void Zuser_control_set_content(Zid user_control, Zid child);
	/** @return The content, Z_INVALID_ID when empty. */
	ZRNUI_API Zid Zuser_control_get_content(Zid user_control);


	/* ============================================================================
	 *  FRAME and PAGE
	 *
	 *  The navigation of a Windows Phone app: Window.Content is a Frame, and
	 *  Frame.Navigate(typeof(MainPage), parameter) shows a Page. C has no
	 *  typeof, so a page is made by a factory: a function that builds the
	 *  page for a parameter. The Frame keeps the history as (factory,
	 *  parameter) and builds the page again on GoBack (CacheSize 0).
	 *
	 *    static Zid main_page(Zid frame, void* parameter) {
	 *        Zid page = Zpage_create();
	 *        Zpage_set_content(page, ...);
	 *        Zpage_on_navigated_to(page, loaded, NULL);   // LoadState
	 *        return page;
	 *    }
	 *    Zid frame = Zframe_create();
	 *    Zroot_set_visual(root, frame);
	 *    Zframe_navigate(frame, main_page, NULL);
	 *
	 *  A Click handler may call Zframe_navigate: the page, and the button
	 *  that is running, are destroyed after the handler returns to the
	 *  router, which checks for it. The `parameter` is a pointer the program
	 *  owns; the Frame only keeps it, so it must outlive the history.
	 *  The page starts below the safe area, 25 px at the top for the status bar of the shell
	 *  (a constant for now, to come from the shell later).
	 *
	 *  Windows.UI.Xaml.Navigation.NavigationMode comes with the callbacks: Zpage_get_navigation_mode
	 *  tells, inside OnNavigatedTo and OnNavigatedFrom, whether the frame Navigate d (Z_NAVIGATION_NEW),
	 *  went back or went forward; the page that leaves is told the mode of the navigation that leaves it.
	 *  A page with a NavigationCacheMode other than Disabled is not destroyed when the frame leaves it:
	 *  it is kept for its entry of the history and comes back, as it was, on GoBack or GoForward (its
	 *  factory does not run again); it is destroyed with the frame or when Navigate drops its entry.
	 *  Required and Enabled do the same (there is no CacheSize: every cached page is kept).
	 *  A page may show an AppBar over its top or its bottom (Zpage_set_top_app_bar); the content
	 *  gets what the closed bar leaves, and an open bar grows over the content.
	 *  Not included: SuspensionManager, GetNavigationState, Navigating (it cannot be cancelled here).
	 * ============================================================================ */

	/** @brief Builds the page of a navigation. Returns a Zpage_create element, or Z_INVALID_ID to refuse. */
	typedef Zid (*ZPageFactory)(Zid frame, void* parameter);

	ZRNUI_API Zid Zpage_create(void);
	/** @brief Page.Content: its one child, as UserControl. Replacing it destroys the old one. */
	ZRNUI_API void Zpage_set_content(Zid page, Zid child);
	ZRNUI_API Zid Zpage_get_content(Zid page);
	/** @brief The parameter passed to Zframe_navigate for this page (NavigationEventArgs.Parameter). */
	ZRNUI_API void* Zpage_get_parameter(Zid page);
	/** @brief The Frame showing the page, Z_INVALID_ID when it is not shown. */
	ZRNUI_API Zid Zpage_get_frame(Zid page);
	/** @brief The NavigationMode of the navigation that brings the page, or takes it away, valid in its OnNavigatedTo and OnNavigatedFrom. */
	ZRNUI_API ZNavigationMode Zpage_get_navigation_mode(Zid page);
	/** @brief NavigationCacheMode, Z_NAVIGATION_CACHE_DISABLED by default. Set it before the frame leaves the page. */
	ZRNUI_API void Zpage_set_navigation_cache_mode(Zid page, ZNavigationCacheMode mode);
	ZRNUI_API ZNavigationCacheMode Zpage_get_navigation_cache_mode(Zid page);
	/** @brief TopAppBar and BottomAppBar: an AppBar (a CommandBar), owned by the page; replacing it destroys the old one. Z_INVALID_ID takes it away. */
	ZRNUI_API void Zpage_set_top_app_bar(Zid page, Zid app_bar);
	ZRNUI_API void Zpage_set_bottom_app_bar(Zid page, Zid app_bar);
	ZRNUI_API Zid Zpage_get_top_app_bar(Zid page);
	ZRNUI_API Zid Zpage_get_bottom_app_bar(Zid page);
	/** @brief OnNavigatedTo: the Frame has shown the page; read the parameter from it. */
	ZRNUI_API void Zpage_on_navigated_to(Zid page, ZRoutedCallback callback, void* user_data);
	/** @brief OnNavigatedFrom: the Frame is about to replace the page; it is still alive in the callback. */
	ZRNUI_API void Zpage_on_navigated_from(Zid page, ZRoutedCallback callback, void* user_data);

	/**
	 * What shows between two pages when animations run (Zroot_set_animations; with them off the page is
	 * replaced at once). The turnstile of Windows Phone: the rows of the page that leaves turn away round
	 * the left edge one after another, a moment of nothing, then the rows of the new page swing in;
	 * 720 ms. NavigatedFrom comes when it starts, NavigatedTo and Navigated when the new page appears.
	 * The transition of a Frame is a ZTransition: Z_TRANSITION_NONE, or Z_TRANSITION_TURNSTILE (the default, as above);
	 * Zframe_set_transitions plays any other kinds on the pages themselves.
	 */
	typedef ZTransition ZPageTransition;

	ZRNUI_API Zid Zframe_create(void);


	/* ============================================================================
	 *  PIVOT, PIVOTITEM
	 *
	 *  The Pivot of the Windows Phone 8.1 template: a title (18.14 px SemiBold at 19,38), under it the
	 *  strip of the headers in a big light type (57 px SemiLight) with the chosen one in the foreground and
	 *  the others in PhoneControlDisabled, and the page of the chosen item below, with the margin 19,22.5,19,0.
	 *  A finger drags the pages and the strip together; it lets go on the next page when it went a fifth of
	 *  a page or flicked; a tap on a header goes to its page. With animations the slide takes 300 ms, without
	 *  them it is at once. SelectionChanged (Z_EVENT_SELECTION_CHANGED, Zpivot_on_selection_changed) comes when
	 *  the chosen item changes, PivotItemLoading when an item is chosen and PivotItemLoaded when its page has
	 *  arrived (at once without animations). The first item is not announced as it is added.
	 *
	 *    Zid pivot = Zpivot_create();
	 *    Zpivot_set_title(pivot, "SETTINGS");
	 *    Zid item = Zpivot_item_create("system");
	 *    Zcontent_control_set_content(item, system_list);
	 *    Zpivot_add_item(pivot, item);
	 *
	 *  The strip is a ring, as the PivotHeaderPanel makes it: with two items or more the headers go round (the
	 *  one that leaves at the left comes back at the end), and a swipe past the last page goes to the first. The
	 *  template does not say it, the native panel does. IsLocked stops the swipes and the taps on the headers and
	 *  hides the headers of the other items; the program can still set SelectedIndex. A Pivot is not an ItemsControl
	 *  here: its items are PivotItems, added with Zpivot_add_item.
	 * ============================================================================ */

	/** PivotItemLoading and PivotItemLoaded: the pivot and the PivotItem (PivotItemEventArgs.Item). */
	typedef void (*ZPivotItemCallback)(Zid sender, Zid item, void* user_data);

	ZRNUI_API Zid Zpivot_create(void);
	/** @brief Title above the headers (the name of the application); "" or NULL takes it away. */
	ZRNUI_API void Zpivot_set_title(Zid pivot, const char* title);
	ZRNUI_API const char* Zpivot_get_title(Zid pivot);
	/**
	 * @brief A PivotItem with its Header. Its Content is the page: Zcontent_control_set_content or set_text.
	 * The item is given to a pivot with Zpivot_add_item, which owns it from then on.
	 */
	ZRNUI_API Zid Zpivot_item_create(const char* header);
	ZRNUI_API void Zpivot_item_set_header(Zid pivot_item, const char* header);
	ZRNUI_API const char* Zpivot_item_get_header(Zid pivot_item);
	/** @brief Items.Append: the pivot takes the item. @return its index, -1 on failure (the item is in a pivot already). */
	ZRNUI_API int Zpivot_add_item(Zid pivot, Zid pivot_item);
	/** @brief Items.Insert; an index out of range appends. */
	ZRNUI_API int Zpivot_insert_item(Zid pivot, int index, Zid pivot_item);
	/** @brief Items.RemoveAt: the item is destroyed. */
	ZRNUI_API void Zpivot_remove_item_at(Zid pivot, int index);
	ZRNUI_API void Zpivot_clear_items(Zid pivot);
	ZRNUI_API int Zpivot_get_item_count(Zid pivot);
	ZRNUI_API Zid Zpivot_get_item(Zid pivot, int index);
	/** @brief SelectedItem, Z_INVALID_ID when there are no items. */
	ZRNUI_API Zid Zpivot_get_selected_item(Zid pivot);
	/** @brief SelectedIndex: goes to an item, the shortest way round, sliding when animations run. */
	ZRNUI_API void Zpivot_set_selected_index(Zid pivot, int index);
	/** @return The index of the chosen item, -1 when there are no items. */
	ZRNUI_API int Zpivot_get_selected_index(Zid pivot);
	/** @brief IsLocked (default false). */
	ZRNUI_API void Zpivot_set_is_locked(Zid pivot, int is_locked);
	ZRNUI_API int Zpivot_get_is_locked(Zid pivot);
	/** @brief Where the header of an item is now in the strip, in the coordinates of the pivot (an empty rectangle when it is out of view). */
	ZRNUI_API ZRect Zpivot_get_header_rect(Zid pivot, int index);
	ZRNUI_API void Zpivot_on_selection_changed(Zid pivot, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zpivot_on_pivot_item_loading(Zid pivot, ZPivotItemCallback callback, void* user_data);
	ZRNUI_API void Zpivot_on_pivot_item_loaded(Zid pivot, ZPivotItemCallback callback, void* user_data);

	ZRNUI_API void Zframe_set_transition(Zid frame, ZPageTransition transition);
	/**
	 * @brief The NavigationInTransition and NavigationOutTransition of the Toolkit, one per direction: what the page
	 * that leaves does and what the new page does, going forward (Navigate, GoForward) and going back (GoBack).
	 * Each is a ZTransition played on the page itself; the new page comes in when the old one has gone. Use the "Out"
	 * kinds for the first and third and the "In" kinds for the second and fourth (the others are not refused, but
	 * play as they are). A feathered turnstile is Zframe_set_transition's.
	 */
	ZRNUI_API void Zframe_set_transitions(Zid frame, ZTransition forward_out, ZTransition forward_in, ZTransition backward_out, ZTransition backward_in);
	/** @brief true while a transition runs. A Navigate or GoBack in that time ends it at once. */
	ZRNUI_API int Zframe_is_transitioning(Zid frame);
	/**
	 * @brief Frame.Navigate: builds the page, shows it and puts it on the history; the pages ahead of the current one are dropped.
	 * Order: NavigatedFrom of the old page, the old page destroyed, NavigatedTo of the new one, then Navigated of the Frame.
	 * @return false, nothing changed, when the factory refuses or does not return a Page, or a callback of a navigation navigates.
	 */
	ZRNUI_API int Zframe_navigate(Zid frame, ZPageFactory factory, void* parameter);
	ZRNUI_API int Zframe_can_go_back(Zid frame);
	ZRNUI_API int Zframe_can_go_forward(Zid frame);
	/** @brief Frame.GoBack / GoForward: shows the previous (next) page, built again from its entry. false when there is none. */
	ZRNUI_API int Zframe_go_back(Zid frame);
	ZRNUI_API int Zframe_go_forward(Zid frame);
	/** @brief Frame.BackStackDepth: the pages behind the current one. */
	ZRNUI_API int Zframe_get_back_stack_depth(Zid frame);
	/** @brief The page shown, Z_INVALID_ID before the first Navigate. */
	ZRNUI_API Zid Zframe_get_page(Zid frame);
	/** @brief Frame.CurrentSourcePageType: the factory of the page shown, NULL before the first Navigate. */
	ZRNUI_API ZPageFactory Zframe_get_current_source_page_type(Zid frame);
	/** @brief Frame.Navigated: the page is shown (after its OnNavigatedTo). */
	ZRNUI_API void Zframe_on_navigated(Zid frame, ZRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  CONTENTCONTROL
	 * ============================================================================ */

	ZRNUI_API Zid Zcontent_control_create(void);

	/**
	 * @brief Content as an element. The control takes ownership; the old
	 * content is destroyed. NULL (Z_INVALID_ID) empties it.
	 */
	ZRNUI_API void Zcontent_control_set_content(Zid control, Zid content);
	/** @brief Content as a string: the control makes a TextBlock that shows it with the font of the control. */
	ZRNUI_API void Zcontent_control_set_text(Zid control, const char* text);
	/** @return The element that is the content (the TextBlock of a string), Z_INVALID_ID when empty. */
	ZRNUI_API Zid Zcontent_control_get_content(Zid control);
	/** @return The text of a string content, "" for an element content or none. */
	ZRNUI_API const char* Zcontent_control_get_text(Zid control);

	ZRNUI_API Zid Ztool_tip_create(const char* text);


	/* ============================================================================
	 *  BUTTONS
	 * ============================================================================ */

	/** @brief Button with a string content. */
	ZRNUI_API Zid Zbutton_create(const char* content);
	ZRNUI_API Zid Zhyperlink_button_create(const char* content);
	ZRNUI_API Zid Zrepeat_button_create(const char* content);
	/**
	 * Default sizes of the 8.1 styles (the control is left aligned, vertically centered): Button, RepeatButton
	 * and ToggleButton MinWidth 109, MinHeight 57.5 with 9.5 px of touch overhang above and below the outline;
	 * CheckBox and RadioButton MinWidth 168 with 16 px above and below; HyperlinkButton has none of it.
	 */

	/** @brief ButtonBase.Click. Fired on release inside the button, or per ClickMode. */
	ZRNUI_API void Zbutton_on_click(Zid button, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zbutton_set_click_mode(Zid button, ZClickMode mode);
	ZRNUI_API int Zbutton_is_pressed(Zid button);
	ZRNUI_API int Zbutton_is_pointer_over(Zid button);
	/** @brief Fires Click as if the user had pressed it (Automation Invoke). */
	ZRNUI_API void Zbutton_click(Zid button);

	/** @brief HyperlinkButton.NavigateUri: only stored; the program opens it in Click. */
	ZRNUI_API void Zhyperlink_button_set_uri(Zid button, const char* uri);
	ZRNUI_API const char* Zhyperlink_button_get_uri(Zid button);

	/** @brief RepeatButton.Delay: milliseconds from the press to the first repeat (default 250). */
	ZRNUI_API void Zrepeat_button_set_delay(Zid button, int delay_ms);
	ZRNUI_API int Zrepeat_button_get_delay(Zid button);
	/** @brief RepeatButton.Interval: milliseconds between repeats (default 250). */
	ZRNUI_API void Zrepeat_button_set_interval(Zid button, int interval_ms);
	ZRNUI_API int Zrepeat_button_get_interval(Zid button);

	ZRNUI_API Zid Ztoggle_button_create(const char* content);
	ZRNUI_API Zid Zcheck_box_create(const char* content);
	/** @param group_name RadioButton.GroupName; radio buttons of one group under one parent exclude each other. NULL or "" groups by parent. */
	ZRNUI_API Zid Zradio_button_create(const char* content, const char* group_name);
	ZRNUI_API void Zradio_button_set_group_name(Zid radio_button, const char* group_name);

	/** @brief IsChecked. A plain bool for two states; Ztoggle_button_set_state for the third. */
	ZRNUI_API void Ztoggle_button_set_is_checked(Zid toggle, int is_checked);
	ZRNUI_API int Ztoggle_button_get_is_checked(Zid toggle);
	ZRNUI_API void Ztoggle_button_set_state(Zid toggle, ZToggleState state);
	ZRNUI_API ZToggleState Ztoggle_button_get_state(Zid toggle);
	/** @brief IsThreeState: a click cycles unchecked, checked, indeterminate. */
	ZRNUI_API void Ztoggle_button_set_is_three_state(Zid toggle, int is_three_state);
	/** @brief Checked, Unchecked and Indeterminate events: Zelement_on(Z_EVENT_CHECKED...). */


	/* ============================================================================
	 *  TOGGLESWITCH
	 *
	 *  A header, the content of the state ("Off" / "On") and a 77 x 36 switch at the right
	 *  that is off at first: a tap anywhere or Space flips it, a drag that starts on the
	 *  knob carries it. Toggled comes whenever IsOn changes (also when the program sets it);
	 *  read the state from the sender. The knob slides for 200 ms when animations run.
	 *  Header and contents are strings here (no templates).
	 * ============================================================================ */

	ZRNUI_API Zid Ztoggle_switch_create(void);
	ZRNUI_API void Ztoggle_switch_set_is_on(Zid toggle_switch, int is_on);
	ZRNUI_API int Ztoggle_switch_get_is_on(Zid toggle_switch);
	/** @brief ToggleSwitch.Header: a small text above the content; "" or NULL takes it away. */
	ZRNUI_API void Ztoggle_switch_set_header(Zid toggle_switch, const char* header);
	ZRNUI_API const char* Ztoggle_switch_get_header(Zid toggle_switch);
	/** @brief OnContent and OffContent (default "On" and "Off"): shown while IsOn is true / false. */
	ZRNUI_API void Ztoggle_switch_set_on_content(Zid toggle_switch, const char* content);
	ZRNUI_API const char* Ztoggle_switch_get_on_content(Zid toggle_switch);
	ZRNUI_API void Ztoggle_switch_set_off_content(Zid toggle_switch, const char* content);
	ZRNUI_API const char* Ztoggle_switch_get_off_content(Zid toggle_switch);
	/** @brief ToggleSwitch.Toggled. */
	ZRNUI_API void Ztoggle_switch_on_toggled(Zid toggle_switch, ZRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  PROGRESSRING
	 *
	 *  Five dots run round a 57.5 px ring while IsActive (needs Zroot_tick), over the
	 *  Background of the page at 0.8 opacity that covers the control. Nothing shows
	 *  while it is not active. It takes no input.
	 * ============================================================================ */

	ZRNUI_API Zid Zprogress_ring_create(void);
	ZRNUI_API void Zprogress_ring_set_is_active(Zid progress_ring, int is_active);
	ZRNUI_API int Zprogress_ring_get_is_active(Zid progress_ring);


	/* ============================================================================
	 *  RANGEBASE: SLIDER, PROGRESSBAR, SCROLLBAR
	 * ============================================================================ */

	/**
	 * @brief Slider with Minimum 0, Maximum 100, Value 0, StepFrequency 1, SmallChange 1 and LargeChange 10
	 * (the 8.1 defaults, as far as they are known). It is a 57 px band (plus the header) as wide as its room.
	 */
	ZRNUI_API Zid Zslider_create(ZOrientation orientation);
	/** @brief ProgressBar with Minimum 0, Maximum 100: 3.5 px high. */
	ZRNUI_API Zid Zprogress_bar_create(void);
	ZRNUI_API Zid Zscroll_bar_create(ZOrientation orientation);

	/** @brief Value is coerced into [Minimum, Maximum]; ValueChanged fires on change. */
	ZRNUI_API void Zrange_base_set_value(Zid range, double value);
	ZRNUI_API double Zrange_base_get_value(Zid range);
	/** @brief Minimum and Maximum; the value is coerced again. */
	ZRNUI_API void Zrange_base_set_range(Zid range, double minimum, double maximum);
	ZRNUI_API double Zrange_base_get_minimum(Zid range);
	ZRNUI_API double Zrange_base_get_maximum(Zid range);
	/** @brief SmallChange (arrows, wheel) and LargeChange (clicks in the track). */
	ZRNUI_API void Zrange_base_set_changes(Zid range, double small_change, double large_change);
	ZRNUI_API void Zrange_base_on_value_changed(Zid range, ZValueChangedCallback callback, void* user_data);

	ZRNUI_API void Zslider_set_orientation(Zid slider, ZOrientation orientation);
	/** @brief Slider.Header: a text above the slider; "" or NULL takes it away. */
	ZRNUI_API void Zslider_set_header(Zid slider, const char* header);
	ZRNUI_API const char* Zslider_get_header(Zid slider);
	/** @brief StepFrequency (default 1): what a touch or a key snaps the value to; 0 for no snapping. A value the program sets is not snapped. */
	ZRNUI_API void Zslider_set_step_frequency(Zid slider, double step_frequency);
	ZRNUI_API double Zslider_get_step_frequency(Zid slider);
	/** @brief TickFrequency: the distance between tick marks; 0 (the default) draws none. */
	ZRNUI_API void Zslider_set_tick_frequency(Zid slider, double tick_frequency);
	ZRNUI_API double Zslider_get_tick_frequency(Zid slider);
	/** @brief TickPlacement, Z_TICK_OUTSIDE by default. */
	ZRNUI_API void Zslider_set_tick_placement(Zid slider, ZTickPlacement placement);
	/** @brief SnapsTo: with Z_SNAPS_TO_TICKS a touch or a key lands on the tick marks instead of the steps. */
	ZRNUI_API void Zslider_set_snaps_to(Zid slider, ZSliderSnapsTo snaps_to);
	/** @brief ProgressBar.IsIndeterminate: five squares run along the bar and the track is hidden (needs Zroot_tick). */
	ZRNUI_API void Zprogress_bar_set_is_indeterminate(Zid progress_bar, int is_indeterminate);
	ZRNUI_API int Zprogress_bar_get_is_indeterminate(Zid progress_bar);
	/** @brief ShowError: the indicator disappears. ShowPaused: the indicator is half transparent. */
	ZRNUI_API void Zprogress_bar_set_show_error(Zid progress_bar, int show_error);
	ZRNUI_API void Zprogress_bar_set_show_paused(Zid progress_bar, int show_paused);

	/** @brief ScrollBar.ViewportSize: the size of the thumb relative to the range. */
	ZRNUI_API void Zscroll_bar_set_viewport_size(Zid scrollbar, double viewport_size);
	ZRNUI_API void Zscroll_bar_on_scroll(Zid scrollbar, ZScrollCallback callback, void* user_data);

	ZRNUI_API Zid Zthumb_create(void);
	ZRNUI_API void Zthumb_on_drag_started(Zid thumb, ZDragCallback callback, void* user_data);
	ZRNUI_API void Zthumb_on_drag_delta(Zid thumb, ZDragCallback callback, void* user_data);
	ZRNUI_API void Zthumb_on_drag_completed(Zid thumb, ZDragCallback callback, void* user_data);
	ZRNUI_API int Zthumb_is_dragging(Zid thumb);

	/** @brief A thin line between items: horizontal in a vertical stack and the other way. */
	ZRNUI_API Zid Zseparator_create(void);


	/* ============================================================================
	 *  SCROLLVIEWER
	 * ============================================================================ */

	ZRNUI_API Zid Zscroll_viewer_create(void);
	/** @brief ScrollViewer.Content: any element; it is measured with unlimited room along the scrolling axes. */
	ZRNUI_API void Zscroll_viewer_set_content(Zid scroll_viewer, Zid content);
	ZRNUI_API void Zscroll_viewer_set_vertical_scrollbar_visibility(Zid scroll_viewer, ZScrollBarVisibility visibility);
	ZRNUI_API void Zscroll_viewer_set_horizontal_scrollbar_visibility(Zid scroll_viewer, ZScrollBarVisibility visibility);
	ZRNUI_API void Zscroll_viewer_scroll_to_vertical_offset(Zid scroll_viewer, double offset);
	ZRNUI_API void Zscroll_viewer_scroll_to_horizontal_offset(Zid scroll_viewer, double offset);
	ZRNUI_API double Zscroll_viewer_get_vertical_offset(Zid scroll_viewer);
	ZRNUI_API double Zscroll_viewer_get_horizontal_offset(Zid scroll_viewer);
	ZRNUI_API double Zscroll_viewer_get_extent_height(Zid scroll_viewer);
	ZRNUI_API double Zscroll_viewer_get_extent_width(Zid scroll_viewer);
	ZRNUI_API double Zscroll_viewer_get_viewport_height(Zid scroll_viewer);
	ZRNUI_API double Zscroll_viewer_get_viewport_width(Zid scroll_viewer);
	ZRNUI_API void Zscroll_viewer_on_scroll(Zid scroll_viewer, ZScrollCallback callback, void* user_data);


	/* ============================================================================
	 *  TEXTBOX, PASSWORDBOX
	 * ============================================================================ */

	ZRNUI_API Zid Ztext_box_create(const char* text);
	ZRNUI_API Zid Zpassword_box_create(void);

	/**
	 * A TextBox has the margin 0,9.5 and MinWidth 51 of its style, and its box is 34 px high at least.
	 * @brief TextBox.Text / PasswordBox.Password. TextChanged fires when it changes, not when the program sets the same text.
	 */
	ZRNUI_API void Ztext_box_set_text(Zid text_box, const char* text);
	ZRNUI_API const char* Ztext_box_get_text(Zid text_box);
	/** @brief AcceptsReturn: Enter makes a new line (multi-line box). Wraps with Ztext_box_set_wrapping. */
	ZRNUI_API void Ztext_box_set_accepts_return(Zid text_box, int accepts_return);
	/** @brief TextWrapping: Z_WRAP wraps the lines at the width of the box (the default is Z_NO_WRAP). */
	ZRNUI_API void Ztext_box_set_text_wrapping(Zid text_box, ZTextWrapping wrapping);
	ZRNUI_API void Ztext_box_set_is_read_only(Zid text_box, int is_read_only);
	ZRNUI_API int Ztext_box_get_is_read_only(Zid text_box);
	/** @brief MaxLength in characters; 0 is no limit. */
	ZRNUI_API void Ztext_box_set_max_length(Zid text_box, int max_length);
	ZRNUI_API int Ztext_box_get_max_length(Zid text_box);
	/** @brief Selection in bytes of the UTF-8 text. */
	ZRNUI_API void Ztext_box_select(Zid text_box, int start, int length);
	ZRNUI_API void Ztext_box_select_all(Zid text_box);
	ZRNUI_API int Ztext_box_get_selection_start(Zid text_box);
	ZRNUI_API int Ztext_box_get_selection_length(Zid text_box);
	/** @brief PlaceholderText: shown in the box while it is empty and has no focus; "" or NULL takes it away. Also for a PasswordBox. */
	ZRNUI_API void Ztext_box_set_placeholder_text(Zid text_box, const char* placeholder_text);
	ZRNUI_API const char* Ztext_box_get_placeholder_text(Zid text_box);
	/** @brief Header: a small text above the box (16 px, PhoneMid); "" or NULL takes it away. Also for a PasswordBox. */
	ZRNUI_API void Ztext_box_set_header(Zid text_box, const char* header);
	ZRNUI_API const char* Ztext_box_get_header(Zid text_box);
	/** @brief PasswordBox.Password: the same text Ztext_box_get_text returns. */
	ZRNUI_API void Zpassword_box_set_password(Zid password_box, const char* password);
	ZRNUI_API const char* Zpassword_box_get_password(Zid password_box);
	ZRNUI_API void Zpassword_box_set_header(Zid password_box, const char* header);
	ZRNUI_API void Zpassword_box_set_placeholder_text(Zid password_box, const char* placeholder_text);
	/** @brief PasswordBox.PasswordChar, the bullet by default. */
	ZRNUI_API void Zpassword_box_set_password_char(Zid password_box, uint32_t codepoint);


	/* ============================================================================
	 *  ITEMSCONTROL, SELECTOR
	 *
	 *  An item is a container (ListBoxItem, ListViewItem...) in the items panel; an item that is
	 *  a string gets a TextBlock for content. A ListBox, a ListView and a GridView show the panel
	 *  in a ScrollViewer. Items are not virtualized: every container exists (thousands are fine),
	 *  but only what is in view is painted.
	 *
	 *    Zid list = Zlist_view_create();
	 *    Zlist_view_set_selection_mode(list, Z_LIST_VIEW_SELECTION_NONE);
	 *    Zlist_view_set_is_item_click_enabled(list, true);
	 *    Zlist_view_on_item_click(list, opened, NULL);
	 *    Zitems_control_add_text(list, "Wi-Fi");
	 *
	 *  The text of a string item is 12 px, the ControlContentThemeFontSize of the ListBox and
	 *  ListView styles; set Zcontrol_set_font_size on the control or on the container to change it.
	 * ============================================================================ */

	/** @brief A plain ItemsControl: its items are the elements themselves, in a StackPanel, with no container around them. */
	ZRNUI_API Zid Zitems_control_create(void);

	/** @brief Adds an item that is a string: the control makes its container. @return The container. */
	ZRNUI_API Zid Zitems_control_add_text(Zid items_control, const char* text);
	/**
	 * @brief Adds an element as an item (Items.Add). Wrapped in the container of the control (ListBoxItem,
	 * ListViewItem, GridViewItem, ComboBoxItem, FlipViewItem) unless it already is a SelectorItem.
	 * @return The container.
	 */
	ZRNUI_API Zid Zitems_control_add(Zid items_control, Zid item);
	ZRNUI_API void Zitems_control_remove_at(Zid items_control, int index);
	ZRNUI_API void Zitems_control_clear(Zid items_control);
	ZRNUI_API int Zitems_control_get_count(Zid items_control);
	/** @return The container (ListBoxItem...) at an index; the header items of groups are not counted. */
	ZRNUI_API Zid Zitems_control_get_container(Zid items_control, int index);
	/** @brief ItemContainerGenerator.IndexFromContainer: the index of a container, -1 when it is not one of the items. */
	ZRNUI_API int Zitems_control_index_from_container(Zid items_control, Zid container);
	/** @brief The text of an item that was added as a string, or the text content of its container. */
	ZRNUI_API const char* Zitems_control_get_text(Zid items_control, int index);
	/**
	 * @brief ItemsPanel: the panel the containers are laid out by, Zitems_stack_panel_create or Zitems_wrap_grid_create
	 * (or a StackPanel, a VirtualizingStackPanel, a WrapPanel). The control takes ownership and moves the containers
	 * to it. A ListView has an ItemsStackPanel and a GridView an ItemsWrapGrid at first. The view scrolls along the
	 * orientation of the panel: sideways for a horizontal stack and for a vertical wrap grid.
	 */
	ZRNUI_API void Zitems_control_set_items_panel(Zid items_control, Zid panel);
	ZRNUI_API Zid Zitems_control_get_items_panel(Zid items_control);
	/** @return The ScrollViewer of a ListBox, ListView or GridView (the template part named ScrollViewer), Z_INVALID_ID for others. */
	ZRNUI_API Zid Zitems_control_get_scroll_viewer(Zid items_control);

	/** @brief SelectedIndex, -1 for none; with several items selected the first. SelectionChanged (Z_EVENT_SELECTION_CHANGED) fires on change. */
	ZRNUI_API void Zselector_set_selected_index(Zid selector, int index);
	ZRNUI_API int Zselector_get_selected_index(Zid selector);
	/** @brief The container of SelectedItem, Z_INVALID_ID for none; its content is the item. */
	ZRNUI_API Zid Zselector_get_selected_container(Zid selector);

	/** @brief SelectorItem.IsSelected: selecting a container selects it in its control the way SelectionMode lets it. */
	ZRNUI_API void Zselector_item_set_is_selected(Zid selector_item, int is_selected);
	ZRNUI_API int Zselector_item_get_is_selected(Zid selector_item);


	/* ============================================================================
	 *  LISTBOX
	 *
	 *  The 8.1 ListBox: a PhoneBaseMediumHigh box (white at 80%) with black text; a row is
	 *  6.5,8 padded, leans while pressed, and fills with the accent when it is selected.
	 *  Single: a tap selects. Multiple: a tap toggles. Extended: a tap selects that one only.
	 *  In the Light theme the 8.1 dictionary gives the box black 75% and the text PhoneBaseBlack, black on black: here the
	 *  text of a ListBox row is PhoneBaseWhite in Light (the one departure from themeresources.xaml).
	 * ============================================================================ */

	ZRNUI_API Zid Zlist_box_create(void);
	/** @brief An item container by itself, to give to Zitems_control_add. */
	ZRNUI_API Zid Zlist_box_item_create(const char* content);
	/** @brief ListBox.SelectionMode, Z_SELECTION_SINGLE by default. */
	ZRNUI_API void Zlist_box_set_selection_mode(Zid list_box, ZSelectionMode mode);
	ZRNUI_API ZSelectionMode Zlist_box_get_selection_mode(Zid list_box);
	/** @brief SelectedItems: writes up to `max` indices, ascending, into `indices`. @return How many items are selected (also when more than `max`). */
	ZRNUI_API int Zlist_box_get_selected_indices(Zid list_box, int* indices, int max);
	/** @brief ListBox.SelectAll (Multiple and Extended). */
	ZRNUI_API void Zlist_box_select_all(Zid list_box);
	/** @brief ListBox.ScrollIntoView. */
	ZRNUI_API void Zlist_box_scroll_into_view(Zid list_box, int index);


	/* ============================================================================
	 *  LISTVIEW, GRIDVIEW (ListViewBase)
	 *
	 *  The 8.1 containers of the phone. A ListView stacks its items; a GridView wraps them in an
	 *  ItemsWrapGrid, rows left to right. Selection looks as in the templates: Single shows nothing
	 *  (the program reads SelectedIndex), Multiple and Extended show a check box 40.5 px wide at the
	 *  left of a list item, and an accent border with a check in the corner of a grid item; a
	 *  pressed item leans and a disabled one is at 40%.
	 *
	 *  A tap: with SelectionMode Single it selects, Multiple toggles, Extended selects that one
	 *  alone, None does nothing; then, when IsItemClickEnabled, ItemClick is raised. (The order of
	 *  the two is not in the SDK; this is what the library does.) The keyboard: arrow keys move
	 *  through the items and Space or Enter acts as a tap; Ctrl+A selects all in Multiple and Extended.
	 * ============================================================================ */

	ZRNUI_API Zid Zlist_view_create(void);
	ZRNUI_API Zid Zgrid_view_create(void);
	/** @brief An item container by itself, to give to Zitems_control_add (a ListViewItem or a GridViewItem). */
	ZRNUI_API Zid Zlist_view_item_create(const char* content);
	ZRNUI_API Zid Zgrid_view_item_create(const char* content);

	/** @brief ListViewBase.SelectionMode, Single by default. Changing it clears the selection that the new mode cannot hold. */
	ZRNUI_API void Zlist_view_set_selection_mode(Zid list_view, ZListViewSelectionMode mode);
	ZRNUI_API ZListViewSelectionMode Zlist_view_get_selection_mode(Zid list_view);
	/** @brief IsItemClickEnabled, false by default. */
	ZRNUI_API void Zlist_view_set_is_item_click_enabled(Zid list_view, int enabled);
	ZRNUI_API int Zlist_view_get_is_item_click_enabled(Zid list_view);
	/** @brief ItemClick: a tap on an item while IsItemClickEnabled. One handler; NULL removes it. */
	ZRNUI_API void Zlist_view_on_item_click(Zid list_view, ZItemClickCallback callback, void* user_data);
	/** @brief IsSwipeEnabled (default true): only stored, a swipe does not select. */
	ZRNUI_API void Zlist_view_set_is_swipe_enabled(Zid list_view, int enabled);
	ZRNUI_API int Zlist_view_get_is_swipe_enabled(Zid list_view);
	/** @brief SelectedItems: writes up to `max` indices, ascending. @return How many items are selected. */
	ZRNUI_API int Zlist_view_get_selected_indices(Zid list_view, int* indices, int max);
	/** @brief ListViewBase.SelectAll (Multiple and Extended). */
	ZRNUI_API void Zlist_view_select_all(Zid list_view);

	/** @brief Header as a string, shown above the items inside the scrolling area; "" or NULL takes it away. */
	ZRNUI_API void Zlist_view_set_header(Zid list_view, const char* header);
	/** @brief Header as an element; the view takes ownership and destroys the old header. Z_INVALID_ID takes it away. */
	ZRNUI_API void Zlist_view_set_header_content(Zid list_view, Zid header);
	/** @brief Footer, below the items: a string, or an element. */
	ZRNUI_API void Zlist_view_set_footer(Zid list_view, const char* footer);
	ZRNUI_API void Zlist_view_set_footer_content(Zid list_view, Zid footer);

	/** @brief ScrollIntoView: scrolls the least that brings the item into view. */
	ZRNUI_API void Zlist_view_scroll_into_view(Zid list_view, int index);
	ZRNUI_API void Zlist_view_scroll_into_view_with_alignment(Zid list_view, int index, ZScrollIntoViewAlignment alignment);

	/**
	 * @brief A group in its simplest form: a ListViewHeaderItem (GridViewHeaderItem) with `title`, added to the items
	 * as they come; the items that follow belong to it. Indices count the items only, not the headers. In a GridView a
	 * header takes a row of its own. There are no collapsing groups, no sticky headers.
	 * @return The header item.
	 */
	ZRNUI_API Zid Zlist_view_add_group_header(Zid list_view, const char* title);
	ZRNUI_API int Zlist_view_get_group_count(Zid list_view);


	/* ============================================================================
	 *  COMBOBOX
	 *
	 *  The inline ComboBox of the phone: a 2.5 px outlined box (33.5 high, with the 9.5 px of touch
	 *  target above and below) with the selected item in it, or the PlaceholderText. A tap opens
	 *  the list over the box, white with an accent border, the selected item in the accent over
	 *  the place of the box; a tap on an item, a tap outside or Escape closes it. The full screen
	 *  picker the phone shows for a list of more than five items is not made: the list is always inline.
	 * ============================================================================ */

	ZRNUI_API Zid Zcombo_box_create(void);
	ZRNUI_API Zid Zcombo_box_item_create(const char* content);
	/** @brief IsDropDownOpen. DropDownOpened and DropDownClosed: Zelement_on(box, Z_EVENT_DROP_DOWN_OPENED...). */
	ZRNUI_API void Zcombo_box_set_is_drop_down_open(Zid combo_box, int is_open);
	ZRNUI_API int Zcombo_box_get_is_drop_down_open(Zid combo_box);
	/** @brief MaxDropDownHeight in pixels; the list scrolls past it. Z_INFINITE (the default) keeps it to the screen. */
	ZRNUI_API void Zcombo_box_set_max_drop_down_height(Zid combo_box, float height);
	ZRNUI_API float Zcombo_box_get_max_drop_down_height(Zid combo_box);
	/** @brief Header: a small text above the box; "" or NULL takes it away. */
	ZRNUI_API void Zcombo_box_set_header(Zid combo_box, const char* header);
	ZRNUI_API const char* Zcombo_box_get_header(Zid combo_box);
	/** @brief PlaceholderText: shown in the box while nothing is selected. */
	ZRNUI_API void Zcombo_box_set_placeholder_text(Zid combo_box, const char* placeholder_text);
	ZRNUI_API const char* Zcombo_box_get_placeholder_text(Zid combo_box);


	/* ============================================================================
	 *  FLIPVIEW
	 *
	 *  One item at a time, as wide and as high as the view. A finger drags the items sideways and
	 *  lets go on the next one when it went a fifth of the width or flicked; the left and right keys
	 *  go to the previous and the next. With animations the snap takes 300 ms (as in a Pivot),
	 *  without them it is at once. SelectionChanged comes when the item in view changes. It does not wrap round.
	 *  The flip buttons of the desktop FlipView are not drawn on the phone.
	 * ============================================================================ */

	ZRNUI_API Zid Zflip_view_create(void);
	ZRNUI_API Zid Zflip_view_item_create(const char* content);
	/** @brief UseTouchAnimationsForAllNavigation (default true): false makes a change of SelectedIndex by the program jump instead of slide. */
	ZRNUI_API void Zflip_view_set_use_touch_animations_for_all_navigation(Zid flip_view, int use);
	ZRNUI_API int Zflip_view_get_use_touch_animations_for_all_navigation(Zid flip_view);


	/* ============================================================================
	 *  AUTOSUGGESTBOX
	 *
	 *  A TextBox with PlaceholderText and Header and, under it, a white list with an accent border
	 *  of suggestions while IsSuggestionListOpen. Nothing is filtered: the program listens to
	 *  TextChanged, with the reason, and gives the suggestions for the text with
	 *  Zauto_suggest_box_set_items; a non empty list opens, an empty one closes. A tap on a suggestion
	 *  (or Enter on the one the Up and Down keys are on) puts it in the text (UpdateTextOnSelect), raises
	 *  SuggestionChosen, then TextChanged with Z_SUGGEST_SUGGESTION_CHOSEN, then QuerySubmitted.
	 *  Enter in the text raises QuerySubmitted with the text; QuerySubmitted and QueryIcon belong to
	 *  Windows 10: they are not in the 8.1 interface, QuerySubmitted is here for the Enter key.
	 *  The suggestions are items: Zitems_control_get_count and Zitems_control_get_text work on the box.
	 * ============================================================================ */

	ZRNUI_API Zid Zauto_suggest_box_create(void);
	/** @brief The suggestions (ItemsSource as strings), copied; the list opens when there are any and the box has focus. */
	ZRNUI_API void Zauto_suggest_box_set_items(Zid box, const char* const* items, int count);
	/** @brief Text. Changing it by the program raises TextChanged with Z_SUGGEST_PROGRAMMATIC_CHANGE and does not open the list. */
	ZRNUI_API void Zauto_suggest_box_set_text(Zid box, const char* text);
	ZRNUI_API const char* Zauto_suggest_box_get_text(Zid box);
	ZRNUI_API void Zauto_suggest_box_set_placeholder_text(Zid box, const char* placeholder_text);
	ZRNUI_API const char* Zauto_suggest_box_get_placeholder_text(Zid box);
	ZRNUI_API void Zauto_suggest_box_set_header(Zid box, const char* header);
	ZRNUI_API const char* Zauto_suggest_box_get_header(Zid box);
	ZRNUI_API void Zauto_suggest_box_set_is_suggestion_list_open(Zid box, int is_open);
	ZRNUI_API int Zauto_suggest_box_get_is_suggestion_list_open(Zid box);
	/** @brief MaxSuggestionListHeight in pixels, 300 by default. */
	ZRNUI_API void Zauto_suggest_box_set_max_suggestion_list_height(Zid box, float height);
	/** @brief UpdateTextOnSelect (default true): a chosen suggestion goes into the text. */
	ZRNUI_API void Zauto_suggest_box_set_update_text_on_select(Zid box, int update);
	ZRNUI_API void Zauto_suggest_box_on_text_changed(Zid box, ZAutoSuggestTextChangedCallback callback, void* user_data);
	ZRNUI_API void Zauto_suggest_box_on_suggestion_chosen(Zid box, ZAutoSuggestSuggestionChosenCallback callback, void* user_data);
	ZRNUI_API void Zauto_suggest_box_on_query_submitted(Zid box, ZAutoSuggestQuerySubmittedCallback callback, void* user_data);


	/* ============================================================================
	 *  SEMANTICKOOM
	 *
	 *  Two views of the same data, the zoomed in one (a list of items) and the zoomed out one (a list
	 *  of its groups); one shows at a time, filling the control. There is no pinch: the program toggles the
	 *  view, usually from the ItemClick of the zoomed out list. (The 8.1 phone template has no zoom out
	 *  button, and the zoomed out view is shown in the place of the zoomed in one, not in a Popup.)
	 * ============================================================================ */

	ZRNUI_API Zid Zsemantic_zoom_create(void);
	/** @brief ZoomedInView and ZoomedOutView: any elements (a ListView, a GridView); the control takes ownership. */
	ZRNUI_API void Zsemantic_zoom_set_zoomed_in_view(Zid semantic_zoom, Zid view);
	ZRNUI_API void Zsemantic_zoom_set_zoomed_out_view(Zid semantic_zoom, Zid view);
	/** @brief IsZoomedInViewActive, true at first. Setting it is a view change, with the events. */
	ZRNUI_API void Zsemantic_zoom_set_is_zoomed_in_view_active(Zid semantic_zoom, int active);
	ZRNUI_API int Zsemantic_zoom_get_is_zoomed_in_view_active(Zid semantic_zoom);
	/** @brief ToggleActiveView. */
	ZRNUI_API void Zsemantic_zoom_toggle_active_view(Zid semantic_zoom);
	/** @brief CanChangeViews (default true): false keeps the view that shows; the button and the toggle do nothing. */
	ZRNUI_API void Zsemantic_zoom_set_can_change_views(Zid semantic_zoom, int can_change);
	/** @brief IsZoomOutButtonEnabled (default true): only stored, the phone template has no button. */
	ZRNUI_API void Zsemantic_zoom_set_is_zoom_out_button_enabled(Zid semantic_zoom, int enabled);
	ZRNUI_API void Zsemantic_zoom_on_view_change_started(Zid semantic_zoom, ZSemanticZoomCallback callback, void* user_data);
	ZRNUI_API void Zsemantic_zoom_on_view_change_completed(Zid semantic_zoom, ZSemanticZoomCallback callback, void* user_data);


	/* ============================================================================
	 *  HUB, HUBSECTION
	 *
	 *  The Hub of the Windows Phone 8.1 template: a big header (78 px Light) over sections that stand side by
	 *  side and scroll sideways with a finger, with inertia and no snap points (no scroll bars). A HubSection is
	 *  a header (19 px SemiBold, margin -1,5,0,31.5) over its content, with the margin 19,0,0,0; its content is its
	 *  Content (Zcontent_control_set_content), where XAML has a template. Each section keeps room for the header of the
	 *  hub above its own, so that they begin together under it.
	 *
	 *    Zid hub = Zhub_create();
	 *    Zhub_set_header(hub, "my app");
	 *    Zid news = Zhub_section_create("news");
	 *    Zcontent_control_set_content(news, list);
	 *    Zhub_add_section(hub, news);
	 *
	 *  A section is as wide as its content wants, never more than the screen less its margin, or as wide as its Width
	 *  (Zelement_set_width); the templates do not say. The header of the hub goes at half the speed of the sections
	 *  (parallax; this is the library's number). Orientation Vertical stacks the sections in a vertical scroller; there
	 *  the header scrolls away with them. Not included: the parallax of the Background, SemanticZoomOwner, section header
	 *  templates, DefaultSectionIndex, an arrow on an interactive header (the phone template draws none).
	 * ============================================================================ */

	/** SectionHeaderClick of a Hub: the hub and the HubSection whose header was tapped. */
	typedef void (*ZHubSectionHeaderClickCallback)(Zid sender, Zid section, void* user_data);

	ZRNUI_API Zid Zhub_create(void);
	/** @brief Header: a string, "" or NULL takes it away. */
	ZRNUI_API void Zhub_set_header(Zid hub, const char* header);
	ZRNUI_API const char* Zhub_get_header(Zid hub);
	/** @brief Orientation, Z_HORIZONTAL by default. Setting it scrolls back to the start. */
	ZRNUI_API void Zhub_set_orientation(Zid hub, ZOrientation orientation);
	ZRNUI_API ZOrientation Zhub_get_orientation(Zid hub);
	/** @return The ScrollViewer of the template (the template part named ScrollViewer): its offsets tell where the hub is. */
	ZRNUI_API Zid Zhub_get_scroll_viewer(Zid hub);
	/** @brief Sections.Append: the hub owns the section. @return its index, -1 on failure (the section is in a hub already). */
	ZRNUI_API int Zhub_add_section(Zid hub, Zid hub_section);
	ZRNUI_API int Zhub_insert_section(Zid hub, int index, Zid hub_section);
	/** @brief Sections.RemoveAt: the section is destroyed. */
	ZRNUI_API void Zhub_remove_section_at(Zid hub, int index);
	ZRNUI_API void Zhub_clear_sections(Zid hub);
	ZRNUI_API int Zhub_get_section_count(Zid hub);
	ZRNUI_API Zid Zhub_get_section(Zid hub, int index);
	/** @brief ScrollToSection: puts the section at the start of the view (at once, as far as the extent lets it go). */
	ZRNUI_API void Zhub_scroll_to_section(Zid hub, int index);
	/** @brief SectionsInView: the sections that show at least half a pixel, in order. */
	ZRNUI_API int Zhub_get_sections_in_view_count(Zid hub);
	ZRNUI_API Zid Zhub_get_section_in_view(Zid hub, int index);
	/** @brief SectionsInViewChanged: raised from the scroll that makes other sections show (a finger, the inertia, ScrollToSection). */
	ZRNUI_API void Zhub_on_sections_in_view_changed(Zid hub, ZRoutedCallback callback, void* user_data);
	/** @brief SectionHeaderClick: a tap on the header of a section with IsHeaderInteractive. One handler; NULL removes it. */
	ZRNUI_API void Zhub_on_section_header_click(Zid hub, ZHubSectionHeaderClickCallback callback, void* user_data);

	ZRNUI_API Zid Zhub_section_create(const char* header);
	ZRNUI_API void Zhub_section_set_header(Zid hub_section, const char* header);
	ZRNUI_API const char* Zhub_section_get_header(Zid hub_section);
	/** @brief IsHeaderInteractive (default false): the header can be tapped, for SectionHeaderClick. */
	ZRNUI_API void Zhub_section_set_is_header_interactive(Zid hub_section, int is_interactive);
	ZRNUI_API int Zhub_section_get_is_header_interactive(Zid hub_section);


	/* ============================================================================
	 *  LOOPINGSELECTOR
	 *
	 *  The column of the date and time pickers: items that go round, one in the middle. At rest only the
	 *  chosen item shows, filled with the accent; while a finger is on the column (and while it moves) the
	 *  others come out as boxes outlined in LoopingSelectorForeground, in 330 ms. A drag has inertia (a
	 *  third of the speed is left after 0.32 s) and ends on an item, which becomes the chosen one when the
	 *  column stops; a tap on an item goes to it, the arrow keys and the wheel move by one. The items are a
	 *  line of 45 px SemiBold text (spacing -24) with, under it, a smaller one of 17 px (the weekday of a day).
	 *  SelectionChanged (Z_EVENT_SELECTION_CHANGED) comes when the chosen item changes. Without animations the
	 *  column ends at once where the inertia would have taken it.
	 *  ItemWidth and ItemHeight are not in the XAML: the width is the widest text (64 px at least) in its margin
	 *  5 and border 2.5, the height the text with its paddings; the column is as high as its room.
	 * ============================================================================ */

	ZRNUI_API Zid Zlooping_selector_create(void);
	/** @brief Items.Append, with the text of the item and a smaller one under it (NULL for none). @return the index. */
	ZRNUI_API int Zlooping_selector_add_item(Zid looping_selector, const char* primary_text, const char* secondary_text);
	ZRNUI_API void Zlooping_selector_clear_items(Zid looping_selector);
	ZRNUI_API int Zlooping_selector_get_item_count(Zid looping_selector);
	ZRNUI_API const char* Zlooping_selector_get_item_text(Zid looping_selector, int index);
	ZRNUI_API const char* Zlooping_selector_get_item_secondary_text(Zid looping_selector, int index);
	/** @brief SelectedIndex, -1 without items. A change by the program is at once, and raises SelectionChanged. */
	ZRNUI_API void Zlooping_selector_set_selected_index(Zid looping_selector, int index);
	ZRNUI_API int Zlooping_selector_get_selected_index(Zid looping_selector);
	/** @brief ShouldLoop (default true): the items go round. false stops the column at the first and the last item. */
	ZRNUI_API void Zlooping_selector_set_should_loop(Zid looping_selector, int should_loop);
	ZRNUI_API int Zlooping_selector_get_should_loop(Zid looping_selector);
	/** @brief ItemWidth and ItemHeight: 0 (the default) is the size of the items as they come. The getters tell the size in use. */
	ZRNUI_API void Zlooping_selector_set_item_width(Zid looping_selector, float width);
	ZRNUI_API float Zlooping_selector_get_item_width(Zid looping_selector);
	ZRNUI_API void Zlooping_selector_set_item_height(Zid looping_selector, float height);
	ZRNUI_API float Zlooping_selector_get_item_height(Zid looping_selector);
	ZRNUI_API void Zlooping_selector_on_selection_changed(Zid looping_selector, ZRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  DATEPICKER, TIMEPICKER, DATEPICKERFLYOUT, TIMEPICKERFLYOUT, PICKERFLYOUTBASE
	 *
	 *  A DatePicker (TimePicker) is a Header (16 px, PhoneMid) over a Button as wide as its room, 2.5 px outlined, with
	 *  the value in it ("November 7, 2026", "3:07 PM" or "15:07"; the formats are the library's: DayFormat and the like are not
	 *  here). A tap on the button shows the flyout, which fills the screen: the Header as its title, the selectors side by side
	 *  (month, day with its weekday, year; hour, minute, AM / PM) with the value as it is, and under them "done" and "cancel"
	 *  buttons. Done makes the selectors the value: the picker's Date changes and DateChanged comes (also, with the old and the new
	 *  date, DatePicked of the flyout, before the flyout closes). Cancel, Back and a tap outside close the flyout and leave the
	 *  value; the selectors go back to it. A change of the value by the program raises DateChanged too, when it is another date.
	 *  Choosing another month or year makes the days those of that month (a day past its end becomes the last one), February
	 *  of a leap year has 29; the calendar is the Gregorian, in plain arithmetic.
	 *
	 *    Zid picker = Zdate_picker_create();
	 *    Zdate_picker_set_header(picker, "birthday");
	 *    Zdate_picker_set_date(picker, (ZDate){ 1999, 12, 31 });
	 *    Zdate_picker_on_date_changed(picker, changed, NULL);
	 *
	 *  The flyouts can also be used by themselves: made, given a value, shown with Zflyout_base_show_at, and listened to
	 *  with DatePicked / TimePicked. They are FlyoutBase (Opening, Opened, Closed, Hide work) with the placement Full.
	 *  Defaults the SDK does not give: a DatePicker begins on today (UTC) and its years are 100 either side of this year;
	 *  a TimePicker on 0:00 with the 12 hour clock and MinuteIncrement 1; a time whose minute is not on the grid of
	 *  MinuteIncrement shows on the nearest minute in the flyout. Not included: CalendarIdentifier, Day, Month and YearFormat,
	 *  Orientation, the header and flyout templates.
	 * ============================================================================ */

	ZRNUI_API Zid Zdate_picker_create(void);
	ZRNUI_API void Zdate_picker_set_header(Zid date_picker, const char* header);
	ZRNUI_API const char* Zdate_picker_get_header(Zid date_picker);
	/** @brief Date: coerced into MinYear..MaxYear and to a day the month has. DateChanged comes when it is another date. */
	ZRNUI_API void Zdate_picker_set_date(Zid date_picker, ZDate date);
	ZRNUI_API ZDate Zdate_picker_get_date(Zid date_picker);
	/** @brief MinYear and MaxYear of the year selector (the date is coerced again). */
	ZRNUI_API void Zdate_picker_set_min_year(Zid date_picker, int year);
	ZRNUI_API void Zdate_picker_set_max_year(Zid date_picker, int year);
	ZRNUI_API int Zdate_picker_get_min_year(Zid date_picker);
	ZRNUI_API int Zdate_picker_get_max_year(Zid date_picker);
	/** @brief DayVisible, MonthVisible, YearVisible (default true): which selectors the flyout has. */
	ZRNUI_API void Zdate_picker_set_day_visible(Zid date_picker, int visible);
	ZRNUI_API void Zdate_picker_set_month_visible(Zid date_picker, int visible);
	ZRNUI_API void Zdate_picker_set_year_visible(Zid date_picker, int visible);
	ZRNUI_API int Zdate_picker_get_day_visible(Zid date_picker);
	ZRNUI_API int Zdate_picker_get_month_visible(Zid date_picker);
	ZRNUI_API int Zdate_picker_get_year_visible(Zid date_picker);
	/** @return The flyout of the picker (a DatePickerFlyout the picker owns), and the Button that shows it (the template part FlyoutButton). */
	ZRNUI_API Zid Zdate_picker_get_flyout(Zid date_picker);
	ZRNUI_API Zid Zdate_picker_get_flyout_button(Zid date_picker);
	ZRNUI_API void Zdate_picker_on_date_changed(Zid date_picker, ZDateChangedCallback callback, void* user_data);

	ZRNUI_API Zid Ztime_picker_create(void);
	ZRNUI_API void Ztime_picker_set_header(Zid time_picker, const char* header);
	ZRNUI_API const char* Ztime_picker_get_header(Zid time_picker);
	/** @brief Time. TimeChanged comes when it is another time. */
	ZRNUI_API void Ztime_picker_set_time(Zid time_picker, ZTimeOfDay time_of_day);
	ZRNUI_API ZTimeOfDay Ztime_picker_get_time(Zid time_picker);
	/** @brief MinuteIncrement (1 to 59, default 1): the minute selector has 0, the increment, twice it... */
	ZRNUI_API void Ztime_picker_set_minute_increment(Zid time_picker, int increment);
	ZRNUI_API int Ztime_picker_get_minute_increment(Zid time_picker);
	/** @brief ClockIdentifier: Z_CLOCK_12_HOUR (the default, with an AM / PM selector) or Z_CLOCK_24_HOUR. */
	ZRNUI_API void Ztime_picker_set_clock_identifier(Zid time_picker, ZClockIdentifier clock);
	ZRNUI_API ZClockIdentifier Ztime_picker_get_clock_identifier(Zid time_picker);
	ZRNUI_API Zid Ztime_picker_get_flyout(Zid time_picker);
	ZRNUI_API Zid Ztime_picker_get_flyout_button(Zid time_picker);
	ZRNUI_API void Ztime_picker_on_time_changed(Zid time_picker, ZTimeChangedCallback callback, void* user_data);

	/** @brief The flyouts by themselves; the Date of a flyout that was not shown yet is today. */
	ZRNUI_API Zid Zdate_picker_flyout_create(void);
	ZRNUI_API void Zdate_picker_flyout_set_date(Zid flyout, ZDate date);
	ZRNUI_API ZDate Zdate_picker_flyout_get_date(Zid flyout);
	ZRNUI_API void Zdate_picker_flyout_set_min_year(Zid flyout, int year);
	ZRNUI_API void Zdate_picker_flyout_set_max_year(Zid flyout, int year);
	ZRNUI_API int Zdate_picker_flyout_get_min_year(Zid flyout);
	ZRNUI_API int Zdate_picker_flyout_get_max_year(Zid flyout);
	ZRNUI_API void Zdate_picker_flyout_set_day_visible(Zid flyout, int visible);
	ZRNUI_API void Zdate_picker_flyout_set_month_visible(Zid flyout, int visible);
	ZRNUI_API void Zdate_picker_flyout_set_year_visible(Zid flyout, int visible);
	ZRNUI_API int Zdate_picker_flyout_get_day_visible(Zid flyout);
	ZRNUI_API int Zdate_picker_flyout_get_month_visible(Zid flyout);
	ZRNUI_API int Zdate_picker_flyout_get_year_visible(Zid flyout);
	/** @brief DatePicked: Done was tapped; the new date is already the flyout's Date. One handler; NULL removes it. */
	ZRNUI_API void Zdate_picker_flyout_on_date_picked(Zid flyout, ZDatePickedCallback callback, void* user_data);

	ZRNUI_API Zid Ztime_picker_flyout_create(void);
	ZRNUI_API void Ztime_picker_flyout_set_time(Zid flyout, ZTimeOfDay time_of_day);
	ZRNUI_API ZTimeOfDay Ztime_picker_flyout_get_time(Zid flyout);
	ZRNUI_API void Ztime_picker_flyout_set_minute_increment(Zid flyout, int increment);
	ZRNUI_API int Ztime_picker_flyout_get_minute_increment(Zid flyout);
	ZRNUI_API void Ztime_picker_flyout_set_clock_identifier(Zid flyout, ZClockIdentifier clock);
	ZRNUI_API ZClockIdentifier Ztime_picker_flyout_get_clock_identifier(Zid flyout);
	ZRNUI_API void Ztime_picker_flyout_on_time_picked(Zid flyout, ZTimePickedCallback callback, void* user_data);

	/** @brief PickerFlyoutBase.Title: a string over the selectors, "" takes it away (a picker's flyout takes its Header when it shows). */
	ZRNUI_API void Zpicker_flyout_base_set_title(Zid picker_flyout, const char* title);
	ZRNUI_API const char* Zpicker_flyout_base_get_title(Zid picker_flyout);
	/**
	 * @return A LoopingSelector of the flyout, 0 to 2, left to right: month, day and year of a DatePickerFlyout; hour, minute and
	 * AM / PM of a TimePickerFlyout. The Done and Cancel buttons are AppBarButtons.
	 */
	ZRNUI_API Zid Zpicker_flyout_base_get_selector(Zid picker_flyout, int index);
	ZRNUI_API Zid Zpicker_flyout_base_get_done_button(Zid picker_flyout);
	ZRNUI_API Zid Zpicker_flyout_base_get_cancel_button(Zid picker_flyout);


	/* ============================================================================
	 *  IconElement
	 *
	 *  The icon of an app bar button: a SymbolIcon draws a Symbol as vector shapes, a FontIcon a glyph
	 *  with the text engine, a PathIcon a geometry, a BitmapIcon a PNG. An icon draws in the foreground of
	 *  the control it is in (the white outline of an AppBarButton, the background color while it is
	 *  pressed) unless Zicon_element_set_foreground gives it one. In the circle of an AppBarButton an icon is 20 x 20.
	 *  Vector drawings exist for Accept, Add, Back, Cancel, Clear, Delete, Download, Edit, Favorite, Find,
	 *  Forward, Home, Mail, MailFilled, More, Next, OutlineStar, Pause, Pin, Play, Previous, Refresh, Remove, Save,
	 *  SaveLocal, Send, Setting, SolidStar, Stop, Sync, Up and Upload; any other Symbol is an empty box.
	 *  (The 8.1 Symbol has Find and Setting, no Search and Settings, and has no Share and no Down.)
	 * ============================================================================ */

	/** @brief IconElement.Foreground. */
	ZRNUI_API void Zicon_element_set_foreground(Zid icon, ZColor color);

	ZRNUI_API Zid Zsymbol_icon_create(ZSymbol symbol);
	ZRNUI_API void Zsymbol_icon_set_symbol(Zid symbol_icon, ZSymbol symbol);
	ZRNUI_API ZSymbol Zsymbol_icon_get_symbol(Zid symbol_icon);

	/** @brief A FontIcon: `glyph` is UTF-8, drawn in the font of the text engine (Selawik, FontSize 20 by default). */
	ZRNUI_API Zid Zfont_icon_create(const char* glyph);
	ZRNUI_API void Zfont_icon_set_glyph(Zid font_icon, const char* glyph);
	ZRNUI_API const char* Zfont_icon_get_glyph(Zid font_icon);
	ZRNUI_API void Zfont_icon_set_font_size(Zid font_icon, float font_size);
	ZRNUI_API void Zfont_icon_set_font_family(Zid font_icon, ZFontFamily family);

	/**
	 * @brief A PathIcon. `data` is Path.Data in the mini language "M x,y L x,y H x V y Z" (also m, l, h, v, z):
	 * every sub-path is a filled polygon, and the whole fits the icon uniformly. NULL or "" makes an empty one.
	 * @return Z_INVALID_ID when the data is not understood.
	 */
	ZRNUI_API Zid Zpath_icon_create(const char* data);
	/** @return false, the icon unchanged, when the data is not understood. */
	ZRNUI_API int Zpath_icon_set_data(Zid path_icon, const char* data);

	/** @brief A BitmapIcon from a PNG file, drawn as it is (it is not tinted with the foreground). Z_INVALID_ID on failure. */
	ZRNUI_API Zid Zbitmap_icon_create(const char* png_path);
	ZRNUI_API int Zbitmap_icon_set_uri_source(Zid bitmap_icon, const char* png_path);


	/* ============================================================================
	 *  APPBAR, COMMANDBAR, APPBARBUTTON, APPBARTOGGLEBUTTON, APPBARSEPARATOR
	 *
	 *  A bar a Page shows over its top or its bottom. Closed it is 57.5 px tall (the 54.5 of
	 *  AppBarThemeMinHeight and a 1.5 px border and padding), or 35 px as ClosedDisplayMode Minimal
	 *  (the XAML has no number for it: 35 is this library's); the page gives its content what is left.
	 *  Three dots at the right edge (all of a Minimal bar) open it: a tap, or IsOpen. Open, a bar
	 *  grows over the page (up for a bottom bar, down for a top bar) in 167 ms, the buttons show
	 *  their labels and a CommandBar lists its secondary commands as rows of text; a press outside
	 *  an open bar that is not sticky closes it, and is not given to what is under it; so does Back.
	 *  A secondary command that is tapped closes the bar. The phone template of CommandBar is only
	 *  a stub (the shell draws the real bar): the dots, the size of a Minimal bar, the rows of the secondary
	 *  commands and the separator are this library's own, in the measures of the other 8.1 templates.
	 *  The Compact bar is 57.5 px, not the 68 of other sources.
	 *
	 *  An AppBarButton is a 38.5 px circle with a 2.5 px outline in the foreground, 12 px under the top
	 *  (11 for a toggle button), the icon in it, and its label under it in 9.5 px type, 80 px wide;
	 *  IsCompact (a CommandBar sets it while it is closed) hides the label and makes it 48 wide. Pressed it fills
	 *  with the foreground and the icon takes the background color; a checked AppBarToggleButton looks so too.
	 *  AppBarButton is a Button: Zbutton_on_click and Zbutton_set_flyout work on it, and the AppBarToggleButton
	 *  is a ToggleButton (Ztoggle_button_*). The Icon is the Content of the button.
	 * ============================================================================ */

	ZRNUI_API Zid Zapp_bar_create(void);
	/** @brief IsOpen. Opened and Closed come when it changes (not when the animation ends). */
	ZRNUI_API void Zapp_bar_set_is_open(Zid app_bar, int is_open);
	ZRNUI_API int Zapp_bar_get_is_open(Zid app_bar);
	/** @brief IsSticky: true keeps an open bar open through a press outside it and Back. */
	ZRNUI_API void Zapp_bar_set_is_sticky(Zid app_bar, int is_sticky);
	ZRNUI_API int Zapp_bar_get_is_sticky(Zid app_bar);
	/** @brief ClosedDisplayMode: Z_APP_BAR_CLOSED_COMPACT (the default) or Z_APP_BAR_CLOSED_MINIMAL. */
	ZRNUI_API void Zapp_bar_set_closed_display_mode(Zid app_bar, ZAppBarClosedDisplayMode mode);
	ZRNUI_API ZAppBarClosedDisplayMode Zapp_bar_get_closed_display_mode(Zid app_bar);
	ZRNUI_API void Zapp_bar_on_opened(Zid app_bar, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zapp_bar_on_closed(Zid app_bar, ZRoutedCallback callback, void* user_data);

	/** @brief A CommandBar. Its Content (Zcontent_control_set_content) is at the left, the primary commands at the right. */
	ZRNUI_API Zid Zcommand_bar_create(void);
	/**
	 * @brief PrimaryCommands.Append and SecondaryCommands.Append: an AppBarButton, an AppBarToggleButton or an AppBarSeparator;
	 * the bar takes ownership. @return the index, -1 on failure.
	 */
	ZRNUI_API int Zcommand_bar_add_primary_command(Zid command_bar, Zid command);
	ZRNUI_API int Zcommand_bar_add_secondary_command(Zid command_bar, Zid command);
	ZRNUI_API int Zcommand_bar_get_primary_command_count(Zid command_bar);
	ZRNUI_API int Zcommand_bar_get_secondary_command_count(Zid command_bar);
	ZRNUI_API Zid Zcommand_bar_get_primary_command(Zid command_bar, int index);
	ZRNUI_API Zid Zcommand_bar_get_secondary_command(Zid command_bar, int index);
	/** @brief Clear: the commands are destroyed. */
	ZRNUI_API void Zcommand_bar_clear_primary_commands(Zid command_bar);
	ZRNUI_API void Zcommand_bar_clear_secondary_commands(Zid command_bar);

	/** @brief An AppBarButton with a Label and an Icon (an IconElement the button takes, or Z_INVALID_ID). */
	ZRNUI_API Zid Zapp_bar_button_create(const char* label, Zid icon);
	ZRNUI_API Zid Zapp_bar_toggle_button_create(const char* label, Zid icon);
	/** @brief Label, Icon and IsCompact of an AppBarButton or an AppBarToggleButton. */
	ZRNUI_API void Zapp_bar_button_set_label(Zid app_bar_button, const char* label);
	ZRNUI_API const char* Zapp_bar_button_get_label(Zid app_bar_button);
	ZRNUI_API void Zapp_bar_button_set_icon(Zid app_bar_button, Zid icon);
	ZRNUI_API Zid Zapp_bar_button_get_icon(Zid app_bar_button);
	ZRNUI_API void Zapp_bar_button_set_is_compact(Zid app_bar_button, int is_compact);
	ZRNUI_API int Zapp_bar_button_get_is_compact(Zid app_bar_button);

	ZRNUI_API Zid Zapp_bar_separator_create(void);
	ZRNUI_API void Zapp_bar_separator_set_is_compact(Zid separator, int is_compact);
	ZRNUI_API int Zapp_bar_separator_get_is_compact(Zid separator);


	/* ============================================================================
	 *  FLYOUTBASE, FLYOUT, MENUFLYOUT, BUTTON.FLYOUT
	 *
	 *  A flyout shows above the page, in a Popup, next to the element it is shown at; a tap outside
	 *  it, Back, an item of a menu or Hide closes it. On the phone it is as wide as the screen. Placement
	 *  Top (the default) puts it above the element and Bottom under it (each flips to the other side
	 *  when it does not fit); Left and Right are Bottom here; Full fills the screen.
	 *  A Flyout is a FlyoutPresenter: the chrome color, at least 43 px tall, its content 25.5 px under its top
	 *  edge. A MenuFlyout is a MenuFlyoutPresenter: the page background with a 1.5 px line in the foreground above and below,
	 *  its items 25.5 px SemiLight, padded 8.5 px above and below, at 19 px from the left.
	 *  Opening comes before the flyout shows, Opened after, Closed when it has gone.
	 *  A flyout is owned by the program (Zelement_destroy) until Zbutton_set_flyout gives it to a button, which
	 *  shows it when it is tapped, after its Click.
	 * ============================================================================ */

	ZRNUI_API void Zflyout_base_set_placement(Zid flyout, ZFlyoutPlacementMode placement);
	ZRNUI_API ZFlyoutPlacementMode Zflyout_base_get_placement(Zid flyout);
	/** @brief ShowAt: `placement_target` must be in a tree that belongs to a root. */
	ZRNUI_API void Zflyout_base_show_at(Zid flyout, Zid placement_target);
	ZRNUI_API void Zflyout_base_hide(Zid flyout);
	/** @brief Whether it shows now (the property of the later flyouts, here for the program to ask). */
	ZRNUI_API int Zflyout_base_get_is_open(Zid flyout);
	ZRNUI_API void Zflyout_base_on_opening(Zid flyout, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zflyout_base_on_opened(Zid flyout, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zflyout_base_on_closed(Zid flyout, ZRoutedCallback callback, void* user_data);

	ZRNUI_API Zid Zflyout_create(void);
	/** @brief Flyout.Content: any element, owned by the flyout. FlyoutPresenterStyle is not supported. */
	ZRNUI_API void Zflyout_set_content(Zid flyout, Zid content);
	ZRNUI_API Zid Zflyout_get_content(Zid flyout);

	ZRNUI_API Zid Zmenu_flyout_create(void);
	/** @brief Items.Append: a MenuFlyoutItem, a ToggleMenuFlyoutItem or a MenuFlyoutSeparator, owned by the menu. @return the index, -1 on failure. */
	ZRNUI_API int Zmenu_flyout_add_item(Zid menu_flyout, Zid item);
	ZRNUI_API int Zmenu_flyout_get_item_count(Zid menu_flyout);
	ZRNUI_API Zid Zmenu_flyout_get_item(Zid menu_flyout, int index);
	ZRNUI_API void Zmenu_flyout_clear_items(Zid menu_flyout);

	/** @brief A MenuFlyoutItem. Click runs on a tap, and the menu closes. */
	ZRNUI_API Zid Zmenu_flyout_item_create(const char* text);
	ZRNUI_API void Zmenu_flyout_item_set_text(Zid menu_flyout_item, const char* text);
	ZRNUI_API const char* Zmenu_flyout_item_get_text(Zid menu_flyout_item);
	ZRNUI_API void Zmenu_flyout_item_on_click(Zid menu_flyout_item, ZRoutedCallback callback, void* user_data);
	/** @brief A ToggleMenuFlyoutItem: a tap flips IsChecked before Click runs; a check is drawn at the right edge when it is on (the template has none). */
	ZRNUI_API Zid Ztoggle_menu_flyout_item_create(const char* text, int is_checked);
	ZRNUI_API void Ztoggle_menu_flyout_item_set_is_checked(Zid toggle_menu_flyout_item, int is_checked);
	ZRNUI_API int Ztoggle_menu_flyout_item_get_is_checked(Zid toggle_menu_flyout_item);
	ZRNUI_API Zid Zmenu_flyout_separator_create(void);

	/** @brief Button.Flyout: the button takes ownership; a tap shows it at the button. Z_INVALID_ID takes it away (and destroys it). */
	ZRNUI_API void Zbutton_set_flyout(Zid button, Zid flyout);
	ZRNUI_API Zid Zbutton_get_flyout(Zid button);


	/* ============================================================================
	 *  CONTENTDIALOG
	 *
	 *  A dialog at the top of the screen, as wide as it, in the chrome color: the title (25.5 px SemiBold),
	 *  the content (18.14 px) and under them the buttons, side by side in two halves (the primary one at the left,
	 *  and a button without text is not there). It shows in a modal Popup: what is under it is dimmed
	 *  (ContentDialogDimmingThemeBrush: black 60% in Dark, white 60% in Light) and gets no input, Tab stays in the dialog,
	 *  and Back or Hide closes it with the result None. The program owns the dialog (it is not put in a tree).
	 *
	 *    Zid dialog = Zcontent_dialog_create();
	 *    Zcontent_dialog_set_title(dialog, "Delete?");
	 *    Zcontent_control_set_text(dialog, "The file will be gone.");
	 *    Zcontent_dialog_set_primary_button_text(dialog, "delete");
	 *    Zcontent_dialog_set_secondary_button_text(dialog, "cancel");
	 *    Zcontent_dialog_show(dialog, root, done, NULL);   // done(dialog, Z_CONTENT_DIALOG_RESULT_PRIMARY, NULL)
	 *
	 *  There is no await in C: ShowAsync is Zcontent_dialog_show with a callback that gets the result. Closed
	 *  comes before it. The keyboard focus goes to the primary button and comes back when the dialog closes.
	 *  Not included: Closing (it cannot be cancelled), FullSizeDesired, the buttons' Command, TitleTemplate.
	 * ============================================================================ */

	ZRNUI_API Zid Zcontent_dialog_create(void);
	/** @brief Title as a string; "" or NULL takes it away. The Content is Zcontent_control_set_content / set_text. */
	ZRNUI_API void Zcontent_dialog_set_title(Zid content_dialog, const char* title);
	ZRNUI_API const char* Zcontent_dialog_get_title(Zid content_dialog);
	ZRNUI_API void Zcontent_dialog_set_primary_button_text(Zid content_dialog, const char* text);
	ZRNUI_API const char* Zcontent_dialog_get_primary_button_text(Zid content_dialog);
	ZRNUI_API void Zcontent_dialog_set_secondary_button_text(Zid content_dialog, const char* text);
	ZRNUI_API const char* Zcontent_dialog_get_secondary_button_text(Zid content_dialog);
	ZRNUI_API void Zcontent_dialog_set_is_primary_button_enabled(Zid content_dialog, int is_enabled);
	ZRNUI_API int Zcontent_dialog_get_is_primary_button_enabled(Zid content_dialog);
	ZRNUI_API void Zcontent_dialog_set_is_secondary_button_enabled(Zid content_dialog, int is_enabled);
	ZRNUI_API int Zcontent_dialog_get_is_secondary_button_enabled(Zid content_dialog);
	ZRNUI_API void Zcontent_dialog_on_primary_button_click(Zid content_dialog, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zcontent_dialog_on_secondary_button_click(Zid content_dialog, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zcontent_dialog_on_opened(Zid content_dialog, ZRoutedCallback callback, void* user_data);
	ZRNUI_API void Zcontent_dialog_on_closed(Zid content_dialog, ZRoutedCallback callback, void* user_data);
	/**
	 * @brief ShowAsync: shows the dialog over `root`. `callback` (may be NULL) gets what closed it.
	 * @return false when it already shows or `root` is not a root.
	 */
	ZRNUI_API int Zcontent_dialog_show(Zid content_dialog, Zid root, ZContentDialogResultCallback callback, void* user_data);
	/** @brief Hide: closes the dialog with the result None. */
	ZRNUI_API void Zcontent_dialog_hide(Zid content_dialog);

#ifdef __cplusplus
}
#endif

#endif
