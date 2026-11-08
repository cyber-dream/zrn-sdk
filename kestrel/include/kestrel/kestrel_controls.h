/**
 * Kestrel
 * kestrel_controls.h : Control and everything that derives from it.
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
 * Colors that were not set follow the theme (kestrel_host.h): changing the
 * theme or the accent of the root restyles every control that has none.
 */

#ifndef KESTREL_CONTROLS_H
#define KESTREL_CONTROLS_H

#include "kestrel_element.h"


/** ValueChanged of a RangeBase. */
typedef void (*KValueChangedCallback)(Kid sender, double old_value, double new_value, void* user_data);
/** Scroll of a ScrollBar (and of a ScrollViewer). */
typedef void (*KScrollCallback)(Kid sender, KScrollEventType type, double new_value, void* user_data);
/** DragStarted / DragDelta / DragCompleted of a Thumb. */
typedef void (*KDragCallback)(Kid sender, float horizontal_change, float vertical_change, void* user_data);
/** ItemClick of a ListView or a GridView: `index` is the index of the item that was tapped (ItemClickEventArgs.ClickedItem). */
typedef void (*KItemClickCallback)(Kid sender, int index, void* user_data);
/** TextChanged of an AutoSuggestBox: why the text changed. A suggestion that was chosen comes as K_SUGGEST_SUGGESTION_CHOSEN. */
typedef void (*KAutoSuggestTextChangedCallback)(Kid sender, KAutoSuggestionBoxTextChangeReason reason, void* user_data);
/** SuggestionChosen of an AutoSuggestBox: `index` of the suggestion in the list (SelectedItem). */
typedef void (*KAutoSuggestSuggestionChosenCallback)(Kid sender, int index, void* user_data);
/** QuerySubmitted of an AutoSuggestBox (Enter, or a tap on a suggestion): the text and the suggestion chosen, -1 when none. */
typedef void (*KAutoSuggestQuerySubmittedCallback)(Kid sender, const char* query_text, int chosen_index, void* user_data);
/** ContentDialog.ShowAsync has finished: what closed the dialog. */
typedef void (*KContentDialogResultCallback)(Kid dialog, KContentDialogResult result, void* user_data);
/** ViewChangeStarted / ViewChangeCompleted of a SemanticZoom: whether the view that is being left (was left) is the zoomed in one. */
typedef void (*KSemanticZoomCallback)(Kid sender, int is_source_zoomed_in_view, void* user_data);


/** A calendar date, Gregorian: `month` 1 to 12, `day` 1 to the days of the month. */
typedef struct KDate {
	int year, month, day;
} KDate;

/** A time of day, 24 hour clock: `hour` 0 to 23, `minute` 0 to 59. */
typedef struct KTimeOfDay {
	int hour, minute;
} KTimeOfDay;

/** ClockIdentifier of a TimePicker. */
typedef enum KClockIdentifier {
	K_CLOCK_12_HOUR = 0x0700,
	K_CLOCK_24_HOUR
} KClockIdentifier;

/** DateChanged of a DatePicker (DatePickerValueChangedEventArgs): the old and the new date. */
typedef void (*KDateChangedCallback)(Kid sender, KDate old_date, KDate new_date, void* user_data);
/** DatePicked of a DatePickerFlyout (DatePickedEventArgs). */
typedef void (*KDatePickedCallback)(Kid sender, KDate old_date, KDate new_date, void* user_data);
/** TimeChanged of a TimePicker. */
typedef void (*KTimeChangedCallback)(Kid sender, KTimeOfDay old_time, KTimeOfDay new_time, void* user_data);
/** TimePicked of a TimePickerFlyout. */
typedef void (*KTimePickedCallback)(Kid sender, KTimeOfDay old_time, KTimeOfDay new_time, void* user_data);


#ifdef __cplusplus
extern "C" {
#endif

	/* ============================================================================
	 *  CONTROL
	 * ============================================================================ */

	/**
	 * Visual properties of every control. A color or a size that was never
	 * set comes from the template: Foreground white in Dark, the accent for
	 * a ProgressBar... Kcontrol_reset_* is not needed: set a color to take
	 * it, create the control again to drop it.
	 */
	KESTREL_API void Kcontrol_set_foreground(Kid control, KColor color);
	KESTREL_API void Kcontrol_set_background(Kid control, KColor color);
	KESTREL_API void Kcontrol_set_border_brush(Kid control, KColor color);
	KESTREL_API void Kcontrol_set_border_thickness(Kid control, KThickness thickness);
	KESTREL_API void Kcontrol_set_padding(Kid control, KThickness padding);
	/**
	 * @brief {ThemeResource key} for a property of a control: Foreground, Background and BorderBrush take a color key,
	 * Padding and BorderThickness a Thickness key, FontSize a Double key. Follows the theme, the accent and the overrides
	 * of the root; the direct setter removes the binding. @return K_FALSE for a Kid that is not a control or a key of another type.
	 */
	KESTREL_API int Kcontrol_bind_foreground(Kid control, KThemeResource key);
	KESTREL_API int Kcontrol_bind_background(Kid control, KThemeResource key);
	KESTREL_API int Kcontrol_bind_border_brush(Kid control, KThemeResource key);
	KESTREL_API int Kcontrol_bind_border_thickness(Kid control, KThemeResource key);
	KESTREL_API int Kcontrol_bind_padding(Kid control, KThemeResource key);
	KESTREL_API int Kcontrol_bind_font_size(Kid control, KThemeResource key);
	KESTREL_API KThickness Kcontrol_get_padding(Kid control);
	KESTREL_API void Kcontrol_set_font_size(Kid control, float size);
	KESTREL_API float Kcontrol_get_font_size(Kid control);
	KESTREL_API void Kcontrol_set_font_family(Kid control, KFontFamily family);
	KESTREL_API void Kcontrol_set_horizontal_content_alignment(Kid control, KHorizontalAlignment alignment);
	KESTREL_API void Kcontrol_set_vertical_content_alignment(Kid control, KVerticalAlignment alignment);
	/** @brief IsTabStop: K_FALSE keeps the control out of the Tab order. */
	KESTREL_API void Kcontrol_set_tab_stop(Kid control, int is_tab_stop);
	KESTREL_API void Kcontrol_set_tab_index(Kid control, int tab_index);

	KESTREL_API Kid Kuser_control_create(void);
	/** @brief UserControl.Content: its one child. Replacing it destroys the old one. */
	KESTREL_API void Kuser_control_set_content(Kid user_control, Kid child);
	/** @return The content, K_INVALID_ID when empty. */
	KESTREL_API Kid Kuser_control_get_content(Kid user_control);


	/* ============================================================================
	 *  FRAME and PAGE
	 *
	 *  The navigation of a Windows Phone app: Window.Content is a Frame, and
	 *  Frame.Navigate(typeof(MainPage), parameter) shows a Page. C has no
	 *  typeof, so a page is made by a factory: a function that builds the
	 *  page for a parameter. The Frame keeps the history as (factory,
	 *  parameter) and builds the page again on GoBack (CacheSize 0).
	 *
	 *    static Kid main_page(Kid frame, void* parameter) {
	 *        Kid page = Kpage_create();
	 *        Kpage_set_content(page, ...);
	 *        Kpage_on_navigated_to(page, loaded, NULL);   // LoadState
	 *        return page;
	 *    }
	 *    Kid frame = Kframe_create();
	 *    Kroot_set_visual(root, frame);
	 *    Kframe_navigate(frame, main_page, NULL);
	 *
	 *  A Click handler may call Kframe_navigate: the page, and the button
	 *  that is running, are destroyed after the handler returns to the
	 *  router, which checks for it. The `parameter` is a pointer the program
	 *  owns; the Frame only keeps it, so it must outlive the history.
	 *  The page starts below the safe area, 25 px at the top for the status bar of the shell
	 *  (a constant for now, to come from the shell later).
	 *
	 *  Windows.UI.Xaml.Navigation.NavigationMode comes with the callbacks: Kpage_get_navigation_mode
	 *  tells, inside OnNavigatedTo and OnNavigatedFrom, whether the frame Navigate d (K_NAVIGATION_NEW),
	 *  went back or went forward; the page that leaves is told the mode of the navigation that leaves it.
	 *  A page with a NavigationCacheMode other than Disabled is not destroyed when the frame leaves it:
	 *  it is kept for its entry of the history and comes back, as it was, on GoBack or GoForward (its
	 *  factory does not run again); it is destroyed with the frame or when Navigate drops its entry.
	 *  Required and Enabled do the same (there is no CacheSize: every cached page is kept).
	 *  A page may show an AppBar over its top or its bottom (Kpage_set_top_app_bar); the content
	 *  gets what the closed bar leaves, and an open bar grows over the content.
	 *  Not included: SuspensionManager, GetNavigationState, Navigating (it cannot be cancelled here).
	 * ============================================================================ */

	/** @brief Builds the page of a navigation. Returns a Kpage_create element, or K_INVALID_ID to refuse. */
	typedef Kid (*KPageFactory)(Kid frame, void* parameter);

	KESTREL_API Kid Kpage_create(void);
	/** @brief Page.Content: its one child, as UserControl. Replacing it destroys the old one. */
	KESTREL_API void Kpage_set_content(Kid page, Kid child);
	KESTREL_API Kid Kpage_get_content(Kid page);
	/** @brief The parameter passed to Kframe_navigate for this page (NavigationEventArgs.Parameter). */
	KESTREL_API void* Kpage_get_parameter(Kid page);
	/** @brief The Frame showing the page, K_INVALID_ID when it is not shown. */
	KESTREL_API Kid Kpage_get_frame(Kid page);
	/** @brief The NavigationMode of the navigation that brings the page, or takes it away, valid in its OnNavigatedTo and OnNavigatedFrom. */
	KESTREL_API KNavigationMode Kpage_get_navigation_mode(Kid page);
	/** @brief NavigationCacheMode, K_NAVIGATION_CACHE_DISABLED by default. Set it before the frame leaves the page. */
	KESTREL_API void Kpage_set_navigation_cache_mode(Kid page, KNavigationCacheMode mode);
	KESTREL_API KNavigationCacheMode Kpage_get_navigation_cache_mode(Kid page);
	/** @brief TopAppBar and BottomAppBar: an AppBar (a CommandBar), owned by the page; replacing it destroys the old one. K_INVALID_ID takes it away. */
	KESTREL_API void Kpage_set_top_app_bar(Kid page, Kid app_bar);
	KESTREL_API void Kpage_set_bottom_app_bar(Kid page, Kid app_bar);
	KESTREL_API Kid Kpage_get_top_app_bar(Kid page);
	KESTREL_API Kid Kpage_get_bottom_app_bar(Kid page);
	/** @brief OnNavigatedTo: the Frame has shown the page; read the parameter from it. */
	KESTREL_API void Kpage_on_navigated_to(Kid page, KRoutedCallback callback, void* user_data);
	/** @brief OnNavigatedFrom: the Frame is about to replace the page; it is still alive in the callback. */
	KESTREL_API void Kpage_on_navigated_from(Kid page, KRoutedCallback callback, void* user_data);

	/**
	 * What shows between two pages when animations run (Kroot_set_animations; with them off the page is
	 * replaced at once). The turnstile of Windows Phone: the rows of the page that leaves turn away round
	 * the left edge one after another, a moment of nothing, then the rows of the new page swing in;
	 * 720 ms. NavigatedFrom comes when it starts, NavigatedTo and Navigated when the new page appears.
	 */
	typedef enum KPageTransition {
		K_TRANSITION_NONE = 0,
		K_TRANSITION_TURNSTILE
	} KPageTransition;

	KESTREL_API Kid Kframe_create(void);


	/* ============================================================================
	 *  PIVOT, PIVOTITEM
	 *
	 *  The Pivot of the Windows Phone 8.1 template: a title (18.14 px SemiBold at 19,38), under it the
	 *  strip of the headers in a big light type (57 px SemiLight) with the chosen one in the foreground and
	 *  the others in PhoneControlDisabled, and the page of the chosen item below, with the margin 19,22.5,19,0.
	 *  A finger drags the pages and the strip together; it lets go on the next page when it went a fifth of
	 *  a page or flicked; a tap on a header goes to its page. With animations the slide takes 300 ms, without
	 *  them it is at once. SelectionChanged (K_EVENT_SELECTION_CHANGED, Kpivot_on_selection_changed) comes when
	 *  the chosen item changes, PivotItemLoading when an item is chosen and PivotItemLoaded when its page has
	 *  arrived (at once without animations). The first item is not announced as it is added.
	 *
	 *    Kid pivot = Kpivot_create();
	 *    Kpivot_set_title(pivot, "SETTINGS");
	 *    Kid item = Kpivot_item_create("system");
	 *    Kcontent_control_set_content(item, system_list);
	 *    Kpivot_add_item(pivot, item);
	 *
	 *  The strip is a ring, as the PivotHeaderPanel makes it: with two items or more the headers go round (the
	 *  one that leaves at the left comes back at the end), and a swipe past the last page goes to the first. The
	 *  template does not say it, the native panel does. IsLocked stops the swipes and the taps on the headers and
	 *  hides the headers of the other items; the program can still set SelectedIndex. A Pivot is not an ItemsControl
	 *  here: its items are PivotItems, added with Kpivot_add_item.
	 * ============================================================================ */

	/** PivotItemLoading and PivotItemLoaded: the pivot and the PivotItem (PivotItemEventArgs.Item). */
	typedef void (*KPivotItemCallback)(Kid sender, Kid item, void* user_data);

	KESTREL_API Kid Kpivot_create(void);
	/** @brief Title above the headers (the name of the application); "" or NULL takes it away. */
	KESTREL_API void Kpivot_set_title(Kid pivot, const char* title);
	KESTREL_API const char* Kpivot_get_title(Kid pivot);
	/**
	 * @brief A PivotItem with its Header. Its Content is the page: Kcontent_control_set_content or set_text.
	 * The item is given to a pivot with Kpivot_add_item, which owns it from then on.
	 */
	KESTREL_API Kid Kpivot_item_create(const char* header);
	KESTREL_API void Kpivot_item_set_header(Kid pivot_item, const char* header);
	KESTREL_API const char* Kpivot_item_get_header(Kid pivot_item);
	/** @brief Items.Append: the pivot takes the item. @return its index, -1 on failure (the item is in a pivot already). */
	KESTREL_API int Kpivot_add_item(Kid pivot, Kid pivot_item);
	/** @brief Items.Insert; an index out of range appends. */
	KESTREL_API int Kpivot_insert_item(Kid pivot, int index, Kid pivot_item);
	/** @brief Items.RemoveAt: the item is destroyed. */
	KESTREL_API void Kpivot_remove_item_at(Kid pivot, int index);
	KESTREL_API void Kpivot_clear_items(Kid pivot);
	KESTREL_API int Kpivot_get_item_count(Kid pivot);
	KESTREL_API Kid Kpivot_get_item(Kid pivot, int index);
	/** @brief SelectedItem, K_INVALID_ID when there are no items. */
	KESTREL_API Kid Kpivot_get_selected_item(Kid pivot);
	/** @brief SelectedIndex: goes to an item, the shortest way round, sliding when animations run. */
	KESTREL_API void Kpivot_set_selected_index(Kid pivot, int index);
	/** @return The index of the chosen item, -1 when there are no items. */
	KESTREL_API int Kpivot_get_selected_index(Kid pivot);
	/** @brief IsLocked (default K_FALSE). */
	KESTREL_API void Kpivot_set_is_locked(Kid pivot, int is_locked);
	KESTREL_API int Kpivot_get_is_locked(Kid pivot);
	/** @brief Where the header of an item is now in the strip, in the coordinates of the pivot (an empty rectangle when it is out of view). */
	KESTREL_API KRect Kpivot_get_header_rect(Kid pivot, int index);
	KESTREL_API void Kpivot_on_selection_changed(Kid pivot, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kpivot_on_pivot_item_loading(Kid pivot, KPivotItemCallback callback, void* user_data);
	KESTREL_API void Kpivot_on_pivot_item_loaded(Kid pivot, KPivotItemCallback callback, void* user_data);

	KESTREL_API void Kframe_set_transition(Kid frame, KPageTransition transition);
	/** @brief K_TRUE while a transition runs. A Navigate or GoBack in that time ends it at once. */
	KESTREL_API int Kframe_is_transitioning(Kid frame);
	/**
	 * @brief Frame.Navigate: builds the page, shows it and puts it on the history; the pages ahead of the current one are dropped.
	 * Order: NavigatedFrom of the old page, the old page destroyed, NavigatedTo of the new one, then Navigated of the Frame.
	 * @return K_FALSE, nothing changed, when the factory refuses or does not return a Page, or a callback of a navigation navigates.
	 */
	KESTREL_API int Kframe_navigate(Kid frame, KPageFactory factory, void* parameter);
	KESTREL_API int Kframe_can_go_back(Kid frame);
	KESTREL_API int Kframe_can_go_forward(Kid frame);
	/** @brief Frame.GoBack / GoForward: shows the previous (next) page, built again from its entry. K_FALSE when there is none. */
	KESTREL_API int Kframe_go_back(Kid frame);
	KESTREL_API int Kframe_go_forward(Kid frame);
	/** @brief Frame.BackStackDepth: the pages behind the current one. */
	KESTREL_API int Kframe_get_back_stack_depth(Kid frame);
	/** @brief The page shown, K_INVALID_ID before the first Navigate. */
	KESTREL_API Kid Kframe_get_page(Kid frame);
	/** @brief Frame.CurrentSourcePageType: the factory of the page shown, NULL before the first Navigate. */
	KESTREL_API KPageFactory Kframe_get_current_source_page_type(Kid frame);
	/** @brief Frame.Navigated: the page is shown (after its OnNavigatedTo). */
	KESTREL_API void Kframe_on_navigated(Kid frame, KRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  CONTENTCONTROL
	 * ============================================================================ */

	KESTREL_API Kid Kcontent_control_create(void);

	/**
	 * @brief Content as an element. The control takes ownership; the old
	 * content is destroyed. NULL (K_INVALID_ID) empties it.
	 */
	KESTREL_API void Kcontent_control_set_content(Kid control, Kid content);
	/** @brief Content as a string: the control makes a TextBlock that shows it with the font of the control. */
	KESTREL_API void Kcontent_control_set_text(Kid control, const char* text);
	/** @return The element that is the content (the TextBlock of a string), K_INVALID_ID when empty. */
	KESTREL_API Kid Kcontent_control_get_content(Kid control);
	/** @return The text of a string content, "" for an element content or none. */
	KESTREL_API const char* Kcontent_control_get_text(Kid control);

	KESTREL_API Kid Ktool_tip_create(const char* text);


	/* ============================================================================
	 *  BUTTONS
	 * ============================================================================ */

	/** @brief Button with a string content. */
	KESTREL_API Kid Kbutton_create(const char* content);
	KESTREL_API Kid Khyperlink_button_create(const char* content);
	KESTREL_API Kid Krepeat_button_create(const char* content);
	/**
	 * Default sizes of the 8.1 styles (the control is left aligned, vertically centered): Button, RepeatButton
	 * and ToggleButton MinWidth 109, MinHeight 57.5 with 9.5 px of touch overhang above and below the outline;
	 * CheckBox and RadioButton MinWidth 168 with 16 px above and below; HyperlinkButton has none of it.
	 */

	/** @brief ButtonBase.Click. Fired on release inside the button, or per ClickMode. */
	KESTREL_API void Kbutton_on_click(Kid button, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kbutton_set_click_mode(Kid button, KClickMode mode);
	KESTREL_API int Kbutton_is_pressed(Kid button);
	KESTREL_API int Kbutton_is_pointer_over(Kid button);
	/** @brief Fires Click as if the user had pressed it (Automation Invoke). */
	KESTREL_API void Kbutton_click(Kid button);

	/** @brief HyperlinkButton.NavigateUri: only stored; the program opens it in Click. */
	KESTREL_API void Khyperlink_button_set_uri(Kid button, const char* uri);
	KESTREL_API const char* Khyperlink_button_get_uri(Kid button);

	/** @brief RepeatButton.Delay: milliseconds from the press to the first repeat (default 250). */
	KESTREL_API void Krepeat_button_set_delay(Kid button, int delay_ms);
	KESTREL_API int Krepeat_button_get_delay(Kid button);
	/** @brief RepeatButton.Interval: milliseconds between repeats (default 250). */
	KESTREL_API void Krepeat_button_set_interval(Kid button, int interval_ms);
	KESTREL_API int Krepeat_button_get_interval(Kid button);

	KESTREL_API Kid Ktoggle_button_create(const char* content);
	KESTREL_API Kid Kcheck_box_create(const char* content);
	/** @param group_name RadioButton.GroupName; radio buttons of one group under one parent exclude each other. NULL or "" groups by parent. */
	KESTREL_API Kid Kradio_button_create(const char* content, const char* group_name);
	KESTREL_API void Kradio_button_set_group_name(Kid radio_button, const char* group_name);

	/** @brief IsChecked. A plain bool for two states; Ktoggle_button_set_state for the third. */
	KESTREL_API void Ktoggle_button_set_is_checked(Kid toggle, int is_checked);
	KESTREL_API int Ktoggle_button_get_is_checked(Kid toggle);
	KESTREL_API void Ktoggle_button_set_state(Kid toggle, KToggleState state);
	KESTREL_API KToggleState Ktoggle_button_get_state(Kid toggle);
	/** @brief IsThreeState: a click cycles unchecked, checked, indeterminate. */
	KESTREL_API void Ktoggle_button_set_is_three_state(Kid toggle, int is_three_state);
	/** @brief Checked, Unchecked and Indeterminate events: Kelement_on(K_EVENT_CHECKED...). */


	/* ============================================================================
	 *  TOGGLESWITCH
	 *
	 *  A header, the content of the state ("Off" / "On") and a 77 x 36 switch at the right
	 *  that is off at first: a tap anywhere or Space flips it, a drag that starts on the
	 *  knob carries it. Toggled comes whenever IsOn changes (also when the program sets it);
	 *  read the state from the sender. The knob slides for 200 ms when animations run.
	 *  Header and contents are strings here (no templates).
	 * ============================================================================ */

	KESTREL_API Kid Ktoggle_switch_create(void);
	KESTREL_API void Ktoggle_switch_set_is_on(Kid toggle_switch, int is_on);
	KESTREL_API int Ktoggle_switch_get_is_on(Kid toggle_switch);
	/** @brief ToggleSwitch.Header: a small text above the content; "" or NULL takes it away. */
	KESTREL_API void Ktoggle_switch_set_header(Kid toggle_switch, const char* header);
	KESTREL_API const char* Ktoggle_switch_get_header(Kid toggle_switch);
	/** @brief OnContent and OffContent (default "On" and "Off"): shown while IsOn is true / false. */
	KESTREL_API void Ktoggle_switch_set_on_content(Kid toggle_switch, const char* content);
	KESTREL_API const char* Ktoggle_switch_get_on_content(Kid toggle_switch);
	KESTREL_API void Ktoggle_switch_set_off_content(Kid toggle_switch, const char* content);
	KESTREL_API const char* Ktoggle_switch_get_off_content(Kid toggle_switch);
	/** @brief ToggleSwitch.Toggled. */
	KESTREL_API void Ktoggle_switch_on_toggled(Kid toggle_switch, KRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  PROGRESSRING
	 *
	 *  Five dots run round a 57.5 px ring while IsActive (needs Kroot_tick), over the
	 *  Background of the page at 0.8 opacity that covers the control. Nothing shows
	 *  while it is not active. It takes no input.
	 * ============================================================================ */

	KESTREL_API Kid Kprogress_ring_create(void);
	KESTREL_API void Kprogress_ring_set_is_active(Kid progress_ring, int is_active);
	KESTREL_API int Kprogress_ring_get_is_active(Kid progress_ring);


	/* ============================================================================
	 *  RANGEBASE: SLIDER, PROGRESSBAR, SCROLLBAR
	 * ============================================================================ */

	/**
	 * @brief Slider with Minimum 0, Maximum 100, Value 0, StepFrequency 1, SmallChange 1 and LargeChange 10
	 * (the 8.1 defaults, as far as they are known). It is a 57 px band (plus the header) as wide as its room.
	 */
	KESTREL_API Kid Kslider_create(KOrientation orientation);
	/** @brief ProgressBar with Minimum 0, Maximum 100: 3.5 px high. */
	KESTREL_API Kid Kprogress_bar_create(void);
	KESTREL_API Kid Kscroll_bar_create(KOrientation orientation);

	/** @brief Value is coerced into [Minimum, Maximum]; ValueChanged fires on change. */
	KESTREL_API void Krange_base_set_value(Kid range, double value);
	KESTREL_API double Krange_base_get_value(Kid range);
	/** @brief Minimum and Maximum; the value is coerced again. */
	KESTREL_API void Krange_base_set_range(Kid range, double minimum, double maximum);
	KESTREL_API double Krange_base_get_minimum(Kid range);
	KESTREL_API double Krange_base_get_maximum(Kid range);
	/** @brief SmallChange (arrows, wheel) and LargeChange (clicks in the track). */
	KESTREL_API void Krange_base_set_changes(Kid range, double small_change, double large_change);
	KESTREL_API void Krange_base_on_value_changed(Kid range, KValueChangedCallback callback, void* user_data);

	KESTREL_API void Kslider_set_orientation(Kid slider, KOrientation orientation);
	/** @brief Slider.Header: a text above the slider; "" or NULL takes it away. */
	KESTREL_API void Kslider_set_header(Kid slider, const char* header);
	KESTREL_API const char* Kslider_get_header(Kid slider);
	/** @brief StepFrequency (default 1): what a touch or a key snaps the value to; 0 for no snapping. A value the program sets is not snapped. */
	KESTREL_API void Kslider_set_step_frequency(Kid slider, double step_frequency);
	KESTREL_API double Kslider_get_step_frequency(Kid slider);
	/** @brief TickFrequency: the distance between tick marks; 0 (the default) draws none. */
	KESTREL_API void Kslider_set_tick_frequency(Kid slider, double tick_frequency);
	KESTREL_API double Kslider_get_tick_frequency(Kid slider);
	/** @brief TickPlacement, K_TICK_OUTSIDE by default. */
	KESTREL_API void Kslider_set_tick_placement(Kid slider, KTickPlacement placement);
	/** @brief SnapsTo: with K_SNAPS_TO_TICKS a touch or a key lands on the tick marks instead of the steps. */
	KESTREL_API void Kslider_set_snaps_to(Kid slider, KSliderSnapsTo snaps_to);
	/** @brief ProgressBar.IsIndeterminate: five squares run along the bar and the track is hidden (needs Kroot_tick). */
	KESTREL_API void Kprogress_bar_set_is_indeterminate(Kid progress_bar, int is_indeterminate);
	KESTREL_API int Kprogress_bar_get_is_indeterminate(Kid progress_bar);
	/** @brief ShowError: the indicator disappears. ShowPaused: the indicator is half transparent. */
	KESTREL_API void Kprogress_bar_set_show_error(Kid progress_bar, int show_error);
	KESTREL_API void Kprogress_bar_set_show_paused(Kid progress_bar, int show_paused);

	/** @brief ScrollBar.ViewportSize: the size of the thumb relative to the range. */
	KESTREL_API void Kscroll_bar_set_viewport_size(Kid scrollbar, double viewport_size);
	KESTREL_API void Kscroll_bar_on_scroll(Kid scrollbar, KScrollCallback callback, void* user_data);

	KESTREL_API Kid Kthumb_create(void);
	KESTREL_API void Kthumb_on_drag_started(Kid thumb, KDragCallback callback, void* user_data);
	KESTREL_API void Kthumb_on_drag_delta(Kid thumb, KDragCallback callback, void* user_data);
	KESTREL_API void Kthumb_on_drag_completed(Kid thumb, KDragCallback callback, void* user_data);
	KESTREL_API int Kthumb_is_dragging(Kid thumb);

	/** @brief A thin line between items: horizontal in a vertical stack and the other way. */
	KESTREL_API Kid Kseparator_create(void);


	/* ============================================================================
	 *  SCROLLVIEWER
	 * ============================================================================ */

	KESTREL_API Kid Kscroll_viewer_create(void);
	/** @brief ScrollViewer.Content: any element; it is measured with unlimited room along the scrolling axes. */
	KESTREL_API void Kscroll_viewer_set_content(Kid scroll_viewer, Kid content);
	KESTREL_API void Kscroll_viewer_set_vertical_scrollbar_visibility(Kid scroll_viewer, KScrollBarVisibility visibility);
	KESTREL_API void Kscroll_viewer_set_horizontal_scrollbar_visibility(Kid scroll_viewer, KScrollBarVisibility visibility);
	KESTREL_API void Kscroll_viewer_scroll_to_vertical_offset(Kid scroll_viewer, double offset);
	KESTREL_API void Kscroll_viewer_scroll_to_horizontal_offset(Kid scroll_viewer, double offset);
	KESTREL_API double Kscroll_viewer_get_vertical_offset(Kid scroll_viewer);
	KESTREL_API double Kscroll_viewer_get_horizontal_offset(Kid scroll_viewer);
	KESTREL_API double Kscroll_viewer_get_extent_height(Kid scroll_viewer);
	KESTREL_API double Kscroll_viewer_get_extent_width(Kid scroll_viewer);
	KESTREL_API double Kscroll_viewer_get_viewport_height(Kid scroll_viewer);
	KESTREL_API double Kscroll_viewer_get_viewport_width(Kid scroll_viewer);
	KESTREL_API void Kscroll_viewer_on_scroll(Kid scroll_viewer, KScrollCallback callback, void* user_data);


	/* ============================================================================
	 *  TEXTBOX, PASSWORDBOX
	 * ============================================================================ */

	KESTREL_API Kid Ktext_box_create(const char* text);
	KESTREL_API Kid Kpassword_box_create(void);

	/**
	 * A TextBox has the margin 0,9.5 and MinWidth 51 of its style, and its box is 34 px high at least.
	 * @brief TextBox.Text / PasswordBox.Password. TextChanged fires when it changes, not when the program sets the same text.
	 */
	KESTREL_API void Ktext_box_set_text(Kid text_box, const char* text);
	KESTREL_API const char* Ktext_box_get_text(Kid text_box);
	/** @brief AcceptsReturn: Enter makes a new line (multi-line box). Wraps with Ktext_box_set_wrapping. */
	KESTREL_API void Ktext_box_set_accepts_return(Kid text_box, int accepts_return);
	/** @brief TextWrapping: K_WRAP wraps the lines at the width of the box (the default is K_NO_WRAP). */
	KESTREL_API void Ktext_box_set_text_wrapping(Kid text_box, KTextWrapping wrapping);
	KESTREL_API void Ktext_box_set_is_read_only(Kid text_box, int is_read_only);
	KESTREL_API int Ktext_box_get_is_read_only(Kid text_box);
	/** @brief MaxLength in characters; 0 is no limit. */
	KESTREL_API void Ktext_box_set_max_length(Kid text_box, int max_length);
	KESTREL_API int Ktext_box_get_max_length(Kid text_box);
	/** @brief Selection in bytes of the UTF-8 text. */
	KESTREL_API void Ktext_box_select(Kid text_box, int start, int length);
	KESTREL_API void Ktext_box_select_all(Kid text_box);
	KESTREL_API int Ktext_box_get_selection_start(Kid text_box);
	KESTREL_API int Ktext_box_get_selection_length(Kid text_box);
	/** @brief PlaceholderText: shown in the box while it is empty and has no focus; "" or NULL takes it away. Also for a PasswordBox. */
	KESTREL_API void Ktext_box_set_placeholder_text(Kid text_box, const char* placeholder_text);
	KESTREL_API const char* Ktext_box_get_placeholder_text(Kid text_box);
	/** @brief Header: a small text above the box (16 px, PhoneMid); "" or NULL takes it away. Also for a PasswordBox. */
	KESTREL_API void Ktext_box_set_header(Kid text_box, const char* header);
	KESTREL_API const char* Ktext_box_get_header(Kid text_box);
	/** @brief PasswordBox.Password: the same text Ktext_box_get_text returns. */
	KESTREL_API void Kpassword_box_set_password(Kid password_box, const char* password);
	KESTREL_API const char* Kpassword_box_get_password(Kid password_box);
	KESTREL_API void Kpassword_box_set_header(Kid password_box, const char* header);
	KESTREL_API void Kpassword_box_set_placeholder_text(Kid password_box, const char* placeholder_text);
	/** @brief PasswordBox.PasswordChar, the bullet by default. */
	KESTREL_API void Kpassword_box_set_password_char(Kid password_box, uint32_t codepoint);


	/* ============================================================================
	 *  ITEMSCONTROL, SELECTOR
	 *
	 *  An item is a container (ListBoxItem, ListViewItem...) in the items panel; an item that is
	 *  a string gets a TextBlock for content. A ListBox, a ListView and a GridView show the panel
	 *  in a ScrollViewer. Items are not virtualized: every container exists (thousands are fine),
	 *  but only what is in view is painted.
	 *
	 *    Kid list = Klist_view_create();
	 *    Klist_view_set_selection_mode(list, K_LIST_VIEW_SELECTION_NONE);
	 *    Klist_view_set_is_item_click_enabled(list, K_TRUE);
	 *    Klist_view_on_item_click(list, opened, NULL);
	 *    Kitems_control_add_text(list, "Wi-Fi");
	 *
	 *  The text of a string item is 12 px, the ControlContentThemeFontSize of the ListBox and
	 *  ListView styles; set Kcontrol_set_font_size on the control or on the container to change it.
	 * ============================================================================ */

	/** @brief A plain ItemsControl: its items are the elements themselves, in a StackPanel, with no container around them. */
	KESTREL_API Kid Kitems_control_create(void);

	/** @brief Adds an item that is a string: the control makes its container. @return The container. */
	KESTREL_API Kid Kitems_control_add_text(Kid items_control, const char* text);
	/**
	 * @brief Adds an element as an item (Items.Add). Wrapped in the container of the control (ListBoxItem,
	 * ListViewItem, GridViewItem, ComboBoxItem, FlipViewItem) unless it already is a SelectorItem.
	 * @return The container.
	 */
	KESTREL_API Kid Kitems_control_add(Kid items_control, Kid item);
	KESTREL_API void Kitems_control_remove_at(Kid items_control, int index);
	KESTREL_API void Kitems_control_clear(Kid items_control);
	KESTREL_API int Kitems_control_get_count(Kid items_control);
	/** @return The container (ListBoxItem...) at an index; the header items of groups are not counted. */
	KESTREL_API Kid Kitems_control_get_container(Kid items_control, int index);
	/** @brief ItemContainerGenerator.IndexFromContainer: the index of a container, -1 when it is not one of the items. */
	KESTREL_API int Kitems_control_index_from_container(Kid items_control, Kid container);
	/** @brief The text of an item that was added as a string, or the text content of its container. */
	KESTREL_API const char* Kitems_control_get_text(Kid items_control, int index);
	/**
	 * @brief ItemsPanel: the panel the containers are laid out by, Kitems_stack_panel_create or Kitems_wrap_grid_create
	 * (or a StackPanel, a VirtualizingStackPanel, a WrapPanel). The control takes ownership and moves the containers
	 * to it. A ListView has an ItemsStackPanel and a GridView an ItemsWrapGrid at first. The view scrolls along the
	 * orientation of the panel: sideways for a horizontal stack and for a vertical wrap grid.
	 */
	KESTREL_API void Kitems_control_set_items_panel(Kid items_control, Kid panel);
	KESTREL_API Kid Kitems_control_get_items_panel(Kid items_control);
	/** @return The ScrollViewer of a ListBox, ListView or GridView (the template part named ScrollViewer), K_INVALID_ID for others. */
	KESTREL_API Kid Kitems_control_get_scroll_viewer(Kid items_control);

	/** @brief SelectedIndex, -1 for none; with several items selected the first. SelectionChanged (K_EVENT_SELECTION_CHANGED) fires on change. */
	KESTREL_API void Kselector_set_selected_index(Kid selector, int index);
	KESTREL_API int Kselector_get_selected_index(Kid selector);
	/** @brief The container of SelectedItem, K_INVALID_ID for none; its content is the item. */
	KESTREL_API Kid Kselector_get_selected_container(Kid selector);

	/** @brief SelectorItem.IsSelected: selecting a container selects it in its control the way SelectionMode lets it. */
	KESTREL_API void Kselector_item_set_is_selected(Kid selector_item, int is_selected);
	KESTREL_API int Kselector_item_get_is_selected(Kid selector_item);


	/* ============================================================================
	 *  LISTBOX
	 *
	 *  The 8.1 ListBox: a PhoneBaseMediumHigh box (white at 80%) with black text; a row is
	 *  6.5,8 padded, leans while pressed, and fills with the accent when it is selected.
	 *  Single: a tap selects. Multiple: a tap toggles. Extended: a tap selects that one only.
	 *  In the Light theme the 8.1 dictionary gives the box black 75% and the text PhoneBaseBlack, black on black: here the
	 *  text of a ListBox row is PhoneBaseWhite in Light (the one departure from themeresources.xaml).
	 * ============================================================================ */

	KESTREL_API Kid Klist_box_create(void);
	/** @brief An item container by itself, to give to Kitems_control_add. */
	KESTREL_API Kid Klist_box_item_create(const char* content);
	/** @brief ListBox.SelectionMode, K_SELECTION_SINGLE by default. */
	KESTREL_API void Klist_box_set_selection_mode(Kid list_box, KSelectionMode mode);
	KESTREL_API KSelectionMode Klist_box_get_selection_mode(Kid list_box);
	/** @brief SelectedItems: writes up to `max` indices, ascending, into `indices`. @return How many items are selected (also when more than `max`). */
	KESTREL_API int Klist_box_get_selected_indices(Kid list_box, int* indices, int max);
	/** @brief ListBox.SelectAll (Multiple and Extended). */
	KESTREL_API void Klist_box_select_all(Kid list_box);
	/** @brief ListBox.ScrollIntoView. */
	KESTREL_API void Klist_box_scroll_into_view(Kid list_box, int index);


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

	KESTREL_API Kid Klist_view_create(void);
	KESTREL_API Kid Kgrid_view_create(void);
	/** @brief An item container by itself, to give to Kitems_control_add (a ListViewItem or a GridViewItem). */
	KESTREL_API Kid Klist_view_item_create(const char* content);
	KESTREL_API Kid Kgrid_view_item_create(const char* content);

	/** @brief ListViewBase.SelectionMode, Single by default. Changing it clears the selection that the new mode cannot hold. */
	KESTREL_API void Klist_view_set_selection_mode(Kid list_view, KListViewSelectionMode mode);
	KESTREL_API KListViewSelectionMode Klist_view_get_selection_mode(Kid list_view);
	/** @brief IsItemClickEnabled, K_FALSE by default. */
	KESTREL_API void Klist_view_set_is_item_click_enabled(Kid list_view, int enabled);
	KESTREL_API int Klist_view_get_is_item_click_enabled(Kid list_view);
	/** @brief ItemClick: a tap on an item while IsItemClickEnabled. One handler; NULL removes it. */
	KESTREL_API void Klist_view_on_item_click(Kid list_view, KItemClickCallback callback, void* user_data);
	/** @brief IsSwipeEnabled (default K_TRUE): only stored, a swipe does not select. */
	KESTREL_API void Klist_view_set_is_swipe_enabled(Kid list_view, int enabled);
	KESTREL_API int Klist_view_get_is_swipe_enabled(Kid list_view);
	/** @brief SelectedItems: writes up to `max` indices, ascending. @return How many items are selected. */
	KESTREL_API int Klist_view_get_selected_indices(Kid list_view, int* indices, int max);
	/** @brief ListViewBase.SelectAll (Multiple and Extended). */
	KESTREL_API void Klist_view_select_all(Kid list_view);

	/** @brief Header as a string, shown above the items inside the scrolling area; "" or NULL takes it away. */
	KESTREL_API void Klist_view_set_header(Kid list_view, const char* header);
	/** @brief Header as an element; the view takes ownership and destroys the old header. K_INVALID_ID takes it away. */
	KESTREL_API void Klist_view_set_header_content(Kid list_view, Kid header);
	/** @brief Footer, below the items: a string, or an element. */
	KESTREL_API void Klist_view_set_footer(Kid list_view, const char* footer);
	KESTREL_API void Klist_view_set_footer_content(Kid list_view, Kid footer);

	/** @brief ScrollIntoView: scrolls the least that brings the item into view. */
	KESTREL_API void Klist_view_scroll_into_view(Kid list_view, int index);
	KESTREL_API void Klist_view_scroll_into_view_with_alignment(Kid list_view, int index, KScrollIntoViewAlignment alignment);

	/**
	 * @brief A group in its simplest form: a ListViewHeaderItem (GridViewHeaderItem) with `title`, added to the items
	 * as they come; the items that follow belong to it. Indices count the items only, not the headers. In a GridView a
	 * header takes a row of its own. There are no collapsing groups, no sticky headers.
	 * @return The header item.
	 */
	KESTREL_API Kid Klist_view_add_group_header(Kid list_view, const char* title);
	KESTREL_API int Klist_view_get_group_count(Kid list_view);


	/* ============================================================================
	 *  COMBOBOX
	 *
	 *  The inline ComboBox of the phone: a 2.5 px outlined box (33.5 high, with the 9.5 px of touch
	 *  target above and below) with the selected item in it, or the PlaceholderText. A tap opens
	 *  the list over the box, white with an accent border, the selected item in the accent over
	 *  the place of the box; a tap on an item, a tap outside or Escape closes it. The full screen
	 *  picker the phone shows for a list of more than five items is not made: the list is always inline.
	 * ============================================================================ */

	KESTREL_API Kid Kcombo_box_create(void);
	KESTREL_API Kid Kcombo_box_item_create(const char* content);
	/** @brief IsDropDownOpen. DropDownOpened and DropDownClosed: Kelement_on(box, K_EVENT_DROP_DOWN_OPENED...). */
	KESTREL_API void Kcombo_box_set_is_drop_down_open(Kid combo_box, int is_open);
	KESTREL_API int Kcombo_box_get_is_drop_down_open(Kid combo_box);
	/** @brief MaxDropDownHeight in pixels; the list scrolls past it. K_INFINITE (the default) keeps it to the screen. */
	KESTREL_API void Kcombo_box_set_max_drop_down_height(Kid combo_box, float height);
	KESTREL_API float Kcombo_box_get_max_drop_down_height(Kid combo_box);
	/** @brief Header: a small text above the box; "" or NULL takes it away. */
	KESTREL_API void Kcombo_box_set_header(Kid combo_box, const char* header);
	KESTREL_API const char* Kcombo_box_get_header(Kid combo_box);
	/** @brief PlaceholderText: shown in the box while nothing is selected. */
	KESTREL_API void Kcombo_box_set_placeholder_text(Kid combo_box, const char* placeholder_text);
	KESTREL_API const char* Kcombo_box_get_placeholder_text(Kid combo_box);


	/* ============================================================================
	 *  FLIPVIEW
	 *
	 *  One item at a time, as wide and as high as the view. A finger drags the items sideways and
	 *  lets go on the next one when it went a fifth of the width or flicked; the left and right keys
	 *  go to the previous and the next. With animations the snap takes 300 ms (as in a Pivot),
	 *  without them it is at once. SelectionChanged comes when the item in view changes. It does not wrap round.
	 *  The flip buttons of the desktop FlipView are not drawn on the phone.
	 * ============================================================================ */

	KESTREL_API Kid Kflip_view_create(void);
	KESTREL_API Kid Kflip_view_item_create(const char* content);
	/** @brief UseTouchAnimationsForAllNavigation (default K_TRUE): K_FALSE makes a change of SelectedIndex by the program jump instead of slide. */
	KESTREL_API void Kflip_view_set_use_touch_animations_for_all_navigation(Kid flip_view, int use);
	KESTREL_API int Kflip_view_get_use_touch_animations_for_all_navigation(Kid flip_view);


	/* ============================================================================
	 *  AUTOSUGGESTBOX
	 *
	 *  A TextBox with PlaceholderText and Header and, under it, a white list with an accent border
	 *  of suggestions while IsSuggestionListOpen. Nothing is filtered: the program listens to
	 *  TextChanged, with the reason, and gives the suggestions for the text with
	 *  Kauto_suggest_box_set_items; a non empty list opens, an empty one closes. A tap on a suggestion
	 *  (or Enter on the one the Up and Down keys are on) puts it in the text (UpdateTextOnSelect), raises
	 *  SuggestionChosen, then TextChanged with K_SUGGEST_SUGGESTION_CHOSEN, then QuerySubmitted.
	 *  Enter in the text raises QuerySubmitted with the text; QuerySubmitted and QueryIcon belong to
	 *  Windows 10: they are not in the 8.1 interface, QuerySubmitted is here for the Enter key.
	 *  The suggestions are items: Kitems_control_get_count and Kitems_control_get_text work on the box.
	 * ============================================================================ */

	KESTREL_API Kid Kauto_suggest_box_create(void);
	/** @brief The suggestions (ItemsSource as strings), copied; the list opens when there are any and the box has focus. */
	KESTREL_API void Kauto_suggest_box_set_items(Kid box, const char* const* items, int count);
	/** @brief Text. Changing it by the program raises TextChanged with K_SUGGEST_PROGRAMMATIC_CHANGE and does not open the list. */
	KESTREL_API void Kauto_suggest_box_set_text(Kid box, const char* text);
	KESTREL_API const char* Kauto_suggest_box_get_text(Kid box);
	KESTREL_API void Kauto_suggest_box_set_placeholder_text(Kid box, const char* placeholder_text);
	KESTREL_API const char* Kauto_suggest_box_get_placeholder_text(Kid box);
	KESTREL_API void Kauto_suggest_box_set_header(Kid box, const char* header);
	KESTREL_API const char* Kauto_suggest_box_get_header(Kid box);
	KESTREL_API void Kauto_suggest_box_set_is_suggestion_list_open(Kid box, int is_open);
	KESTREL_API int Kauto_suggest_box_get_is_suggestion_list_open(Kid box);
	/** @brief MaxSuggestionListHeight in pixels, 300 by default. */
	KESTREL_API void Kauto_suggest_box_set_max_suggestion_list_height(Kid box, float height);
	/** @brief UpdateTextOnSelect (default K_TRUE): a chosen suggestion goes into the text. */
	KESTREL_API void Kauto_suggest_box_set_update_text_on_select(Kid box, int update);
	KESTREL_API void Kauto_suggest_box_on_text_changed(Kid box, KAutoSuggestTextChangedCallback callback, void* user_data);
	KESTREL_API void Kauto_suggest_box_on_suggestion_chosen(Kid box, KAutoSuggestSuggestionChosenCallback callback, void* user_data);
	KESTREL_API void Kauto_suggest_box_on_query_submitted(Kid box, KAutoSuggestQuerySubmittedCallback callback, void* user_data);


	/* ============================================================================
	 *  SEMANTICZOOM
	 *
	 *  Two views of the same data, the zoomed in one (a list of items) and the zoomed out one (a list
	 *  of its groups); one shows at a time, filling the control. There is no pinch: the program toggles the
	 *  view, usually from the ItemClick of the zoomed out list. (The 8.1 phone template has no zoom out
	 *  button, and the zoomed out view is shown in the place of the zoomed in one, not in a Popup.)
	 * ============================================================================ */

	KESTREL_API Kid Ksemantic_zoom_create(void);
	/** @brief ZoomedInView and ZoomedOutView: any elements (a ListView, a GridView); the control takes ownership. */
	KESTREL_API void Ksemantic_zoom_set_zoomed_in_view(Kid semantic_zoom, Kid view);
	KESTREL_API void Ksemantic_zoom_set_zoomed_out_view(Kid semantic_zoom, Kid view);
	/** @brief IsZoomedInViewActive, K_TRUE at first. Setting it is a view change, with the events. */
	KESTREL_API void Ksemantic_zoom_set_is_zoomed_in_view_active(Kid semantic_zoom, int active);
	KESTREL_API int Ksemantic_zoom_get_is_zoomed_in_view_active(Kid semantic_zoom);
	/** @brief ToggleActiveView. */
	KESTREL_API void Ksemantic_zoom_toggle_active_view(Kid semantic_zoom);
	/** @brief CanChangeViews (default K_TRUE): K_FALSE keeps the view that shows; the button and the toggle do nothing. */
	KESTREL_API void Ksemantic_zoom_set_can_change_views(Kid semantic_zoom, int can_change);
	/** @brief IsZoomOutButtonEnabled (default K_TRUE): only stored, the phone template has no button. */
	KESTREL_API void Ksemantic_zoom_set_is_zoom_out_button_enabled(Kid semantic_zoom, int enabled);
	KESTREL_API void Ksemantic_zoom_on_view_change_started(Kid semantic_zoom, KSemanticZoomCallback callback, void* user_data);
	KESTREL_API void Ksemantic_zoom_on_view_change_completed(Kid semantic_zoom, KSemanticZoomCallback callback, void* user_data);


	/* ============================================================================
	 *  HUB, HUBSECTION
	 *
	 *  The Hub of the Windows Phone 8.1 template: a big header (78 px Light) over sections that stand side by
	 *  side and scroll sideways with a finger, with inertia and no snap points (no scroll bars). A HubSection is
	 *  a header (19 px SemiBold, margin -1,5,0,31.5) over its content, with the margin 19,0,0,0; its content is its
	 *  Content (Kcontent_control_set_content), where XAML has a template. Each section keeps room for the header of the
	 *  hub above its own, so that they begin together under it.
	 *
	 *    Kid hub = Khub_create();
	 *    Khub_set_header(hub, "my app");
	 *    Kid news = Khub_section_create("news");
	 *    Kcontent_control_set_content(news, list);
	 *    Khub_add_section(hub, news);
	 *
	 *  A section is as wide as its content wants, never more than the screen less its margin, or as wide as its Width
	 *  (Kelement_set_width); the templates do not say. The header of the hub goes at half the speed of the sections
	 *  (parallax; this is the library's number). Orientation Vertical stacks the sections in a vertical scroller; there
	 *  the header scrolls away with them. Not included: the parallax of the Background, SemanticZoomOwner, section header
	 *  templates, DefaultSectionIndex, an arrow on an interactive header (the phone template draws none).
	 * ============================================================================ */

	/** SectionHeaderClick of a Hub: the hub and the HubSection whose header was tapped. */
	typedef void (*KHubSectionHeaderClickCallback)(Kid sender, Kid section, void* user_data);

	KESTREL_API Kid Khub_create(void);
	/** @brief Header: a string, "" or NULL takes it away. */
	KESTREL_API void Khub_set_header(Kid hub, const char* header);
	KESTREL_API const char* Khub_get_header(Kid hub);
	/** @brief Orientation, K_HORIZONTAL by default. Setting it scrolls back to the start. */
	KESTREL_API void Khub_set_orientation(Kid hub, KOrientation orientation);
	KESTREL_API KOrientation Khub_get_orientation(Kid hub);
	/** @return The ScrollViewer of the template (the template part named ScrollViewer): its offsets tell where the hub is. */
	KESTREL_API Kid Khub_get_scroll_viewer(Kid hub);
	/** @brief Sections.Append: the hub owns the section. @return its index, -1 on failure (the section is in a hub already). */
	KESTREL_API int Khub_add_section(Kid hub, Kid hub_section);
	KESTREL_API int Khub_insert_section(Kid hub, int index, Kid hub_section);
	/** @brief Sections.RemoveAt: the section is destroyed. */
	KESTREL_API void Khub_remove_section_at(Kid hub, int index);
	KESTREL_API void Khub_clear_sections(Kid hub);
	KESTREL_API int Khub_get_section_count(Kid hub);
	KESTREL_API Kid Khub_get_section(Kid hub, int index);
	/** @brief ScrollToSection: puts the section at the start of the view (at once, as far as the extent lets it go). */
	KESTREL_API void Khub_scroll_to_section(Kid hub, int index);
	/** @brief SectionsInView: the sections that show at least half a pixel, in order. */
	KESTREL_API int Khub_get_sections_in_view_count(Kid hub);
	KESTREL_API Kid Khub_get_section_in_view(Kid hub, int index);
	/** @brief SectionsInViewChanged: raised from the scroll that makes other sections show (a finger, the inertia, ScrollToSection). */
	KESTREL_API void Khub_on_sections_in_view_changed(Kid hub, KRoutedCallback callback, void* user_data);
	/** @brief SectionHeaderClick: a tap on the header of a section with IsHeaderInteractive. One handler; NULL removes it. */
	KESTREL_API void Khub_on_section_header_click(Kid hub, KHubSectionHeaderClickCallback callback, void* user_data);

	KESTREL_API Kid Khub_section_create(const char* header);
	KESTREL_API void Khub_section_set_header(Kid hub_section, const char* header);
	KESTREL_API const char* Khub_section_get_header(Kid hub_section);
	/** @brief IsHeaderInteractive (default K_FALSE): the header can be tapped, for SectionHeaderClick. */
	KESTREL_API void Khub_section_set_is_header_interactive(Kid hub_section, int is_interactive);
	KESTREL_API int Khub_section_get_is_header_interactive(Kid hub_section);


	/* ============================================================================
	 *  LOOPINGSELECTOR
	 *
	 *  The column of the date and time pickers: items that go round, one in the middle. At rest only the
	 *  chosen item shows, filled with the accent; while a finger is on the column (and while it moves) the
	 *  others come out as boxes outlined in LoopingSelectorForeground, in 330 ms. A drag has inertia (a
	 *  third of the speed is left after 0.32 s) and ends on an item, which becomes the chosen one when the
	 *  column stops; a tap on an item goes to it, the arrow keys and the wheel move by one. The items are a
	 *  line of 45 px SemiBold text (spacing -24) with, under it, a smaller one of 17 px (the weekday of a day).
	 *  SelectionChanged (K_EVENT_SELECTION_CHANGED) comes when the chosen item changes. Without animations the
	 *  column ends at once where the inertia would have taken it.
	 *  ItemWidth and ItemHeight are not in the XAML: the width is the widest text (64 px at least) in its margin
	 *  5 and border 2.5, the height the text with its paddings; the column is as high as its room.
	 * ============================================================================ */

	KESTREL_API Kid Klooping_selector_create(void);
	/** @brief Items.Append, with the text of the item and a smaller one under it (NULL for none). @return the index. */
	KESTREL_API int Klooping_selector_add_item(Kid looping_selector, const char* primary_text, const char* secondary_text);
	KESTREL_API void Klooping_selector_clear_items(Kid looping_selector);
	KESTREL_API int Klooping_selector_get_item_count(Kid looping_selector);
	KESTREL_API const char* Klooping_selector_get_item_text(Kid looping_selector, int index);
	KESTREL_API const char* Klooping_selector_get_item_secondary_text(Kid looping_selector, int index);
	/** @brief SelectedIndex, -1 without items. A change by the program is at once, and raises SelectionChanged. */
	KESTREL_API void Klooping_selector_set_selected_index(Kid looping_selector, int index);
	KESTREL_API int Klooping_selector_get_selected_index(Kid looping_selector);
	/** @brief ShouldLoop (default K_TRUE): the items go round. K_FALSE stops the column at the first and the last item. */
	KESTREL_API void Klooping_selector_set_should_loop(Kid looping_selector, int should_loop);
	KESTREL_API int Klooping_selector_get_should_loop(Kid looping_selector);
	/** @brief ItemWidth and ItemHeight: 0 (the default) is the size of the items as they come. The getters tell the size in use. */
	KESTREL_API void Klooping_selector_set_item_width(Kid looping_selector, float width);
	KESTREL_API float Klooping_selector_get_item_width(Kid looping_selector);
	KESTREL_API void Klooping_selector_set_item_height(Kid looping_selector, float height);
	KESTREL_API float Klooping_selector_get_item_height(Kid looping_selector);
	KESTREL_API void Klooping_selector_on_selection_changed(Kid looping_selector, KRoutedCallback callback, void* user_data);


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
	 *    Kid picker = Kdate_picker_create();
	 *    Kdate_picker_set_header(picker, "birthday");
	 *    Kdate_picker_set_date(picker, (KDate){ 1999, 12, 31 });
	 *    Kdate_picker_on_date_changed(picker, changed, NULL);
	 *
	 *  The flyouts can also be used by themselves: made, given a value, shown with Kflyout_base_show_at, and listened to
	 *  with DatePicked / TimePicked. They are FlyoutBase (Opening, Opened, Closed, Hide work) with the placement Full.
	 *  Defaults the SDK does not give: a DatePicker begins on today (UTC) and its years are 100 either side of this year;
	 *  a TimePicker on 0:00 with the 12 hour clock and MinuteIncrement 1; a time whose minute is not on the grid of
	 *  MinuteIncrement shows on the nearest minute in the flyout. Not included: CalendarIdentifier, Day, Month and YearFormat,
	 *  Orientation, the header and flyout templates.
	 * ============================================================================ */

	KESTREL_API Kid Kdate_picker_create(void);
	KESTREL_API void Kdate_picker_set_header(Kid date_picker, const char* header);
	KESTREL_API const char* Kdate_picker_get_header(Kid date_picker);
	/** @brief Date: coerced into MinYear..MaxYear and to a day the month has. DateChanged comes when it is another date. */
	KESTREL_API void Kdate_picker_set_date(Kid date_picker, KDate date);
	KESTREL_API KDate Kdate_picker_get_date(Kid date_picker);
	/** @brief MinYear and MaxYear of the year selector (the date is coerced again). */
	KESTREL_API void Kdate_picker_set_min_year(Kid date_picker, int year);
	KESTREL_API void Kdate_picker_set_max_year(Kid date_picker, int year);
	KESTREL_API int Kdate_picker_get_min_year(Kid date_picker);
	KESTREL_API int Kdate_picker_get_max_year(Kid date_picker);
	/** @brief DayVisible, MonthVisible, YearVisible (default K_TRUE): which selectors the flyout has. */
	KESTREL_API void Kdate_picker_set_day_visible(Kid date_picker, int visible);
	KESTREL_API void Kdate_picker_set_month_visible(Kid date_picker, int visible);
	KESTREL_API void Kdate_picker_set_year_visible(Kid date_picker, int visible);
	KESTREL_API int Kdate_picker_get_day_visible(Kid date_picker);
	KESTREL_API int Kdate_picker_get_month_visible(Kid date_picker);
	KESTREL_API int Kdate_picker_get_year_visible(Kid date_picker);
	/** @return The flyout of the picker (a DatePickerFlyout the picker owns), and the Button that shows it (the template part FlyoutButton). */
	KESTREL_API Kid Kdate_picker_get_flyout(Kid date_picker);
	KESTREL_API Kid Kdate_picker_get_flyout_button(Kid date_picker);
	KESTREL_API void Kdate_picker_on_date_changed(Kid date_picker, KDateChangedCallback callback, void* user_data);

	KESTREL_API Kid Ktime_picker_create(void);
	KESTREL_API void Ktime_picker_set_header(Kid time_picker, const char* header);
	KESTREL_API const char* Ktime_picker_get_header(Kid time_picker);
	/** @brief Time. TimeChanged comes when it is another time. */
	KESTREL_API void Ktime_picker_set_time(Kid time_picker, KTimeOfDay time_of_day);
	KESTREL_API KTimeOfDay Ktime_picker_get_time(Kid time_picker);
	/** @brief MinuteIncrement (1 to 59, default 1): the minute selector has 0, the increment, twice it... */
	KESTREL_API void Ktime_picker_set_minute_increment(Kid time_picker, int increment);
	KESTREL_API int Ktime_picker_get_minute_increment(Kid time_picker);
	/** @brief ClockIdentifier: K_CLOCK_12_HOUR (the default, with an AM / PM selector) or K_CLOCK_24_HOUR. */
	KESTREL_API void Ktime_picker_set_clock_identifier(Kid time_picker, KClockIdentifier clock);
	KESTREL_API KClockIdentifier Ktime_picker_get_clock_identifier(Kid time_picker);
	KESTREL_API Kid Ktime_picker_get_flyout(Kid time_picker);
	KESTREL_API Kid Ktime_picker_get_flyout_button(Kid time_picker);
	KESTREL_API void Ktime_picker_on_time_changed(Kid time_picker, KTimeChangedCallback callback, void* user_data);

	/** @brief The flyouts by themselves; the Date of a flyout that was not shown yet is today. */
	KESTREL_API Kid Kdate_picker_flyout_create(void);
	KESTREL_API void Kdate_picker_flyout_set_date(Kid flyout, KDate date);
	KESTREL_API KDate Kdate_picker_flyout_get_date(Kid flyout);
	KESTREL_API void Kdate_picker_flyout_set_min_year(Kid flyout, int year);
	KESTREL_API void Kdate_picker_flyout_set_max_year(Kid flyout, int year);
	KESTREL_API int Kdate_picker_flyout_get_min_year(Kid flyout);
	KESTREL_API int Kdate_picker_flyout_get_max_year(Kid flyout);
	KESTREL_API void Kdate_picker_flyout_set_day_visible(Kid flyout, int visible);
	KESTREL_API void Kdate_picker_flyout_set_month_visible(Kid flyout, int visible);
	KESTREL_API void Kdate_picker_flyout_set_year_visible(Kid flyout, int visible);
	KESTREL_API int Kdate_picker_flyout_get_day_visible(Kid flyout);
	KESTREL_API int Kdate_picker_flyout_get_month_visible(Kid flyout);
	KESTREL_API int Kdate_picker_flyout_get_year_visible(Kid flyout);
	/** @brief DatePicked: Done was tapped; the new date is already the flyout's Date. One handler; NULL removes it. */
	KESTREL_API void Kdate_picker_flyout_on_date_picked(Kid flyout, KDatePickedCallback callback, void* user_data);

	KESTREL_API Kid Ktime_picker_flyout_create(void);
	KESTREL_API void Ktime_picker_flyout_set_time(Kid flyout, KTimeOfDay time_of_day);
	KESTREL_API KTimeOfDay Ktime_picker_flyout_get_time(Kid flyout);
	KESTREL_API void Ktime_picker_flyout_set_minute_increment(Kid flyout, int increment);
	KESTREL_API int Ktime_picker_flyout_get_minute_increment(Kid flyout);
	KESTREL_API void Ktime_picker_flyout_set_clock_identifier(Kid flyout, KClockIdentifier clock);
	KESTREL_API KClockIdentifier Ktime_picker_flyout_get_clock_identifier(Kid flyout);
	KESTREL_API void Ktime_picker_flyout_on_time_picked(Kid flyout, KTimePickedCallback callback, void* user_data);

	/** @brief PickerFlyoutBase.Title: a string over the selectors, "" takes it away (a picker's flyout takes its Header when it shows). */
	KESTREL_API void Kpicker_flyout_base_set_title(Kid picker_flyout, const char* title);
	KESTREL_API const char* Kpicker_flyout_base_get_title(Kid picker_flyout);
	/**
	 * @return A LoopingSelector of the flyout, 0 to 2, left to right: month, day and year of a DatePickerFlyout; hour, minute and
	 * AM / PM of a TimePickerFlyout. The Done and Cancel buttons are AppBarButtons.
	 */
	KESTREL_API Kid Kpicker_flyout_base_get_selector(Kid picker_flyout, int index);
	KESTREL_API Kid Kpicker_flyout_base_get_done_button(Kid picker_flyout);
	KESTREL_API Kid Kpicker_flyout_base_get_cancel_button(Kid picker_flyout);


	/* ============================================================================
	 *  IconElement
	 *
	 *  The icon of an app bar button: a SymbolIcon draws a Symbol as vector shapes, a FontIcon a glyph
	 *  with the text engine, a PathIcon a geometry, a BitmapIcon a PNG. An icon draws in the foreground of
	 *  the control it is in (the white outline of an AppBarButton, the background color while it is
	 *  pressed) unless Kicon_element_set_foreground gives it one. In the circle of an AppBarButton an icon is 20 x 20.
	 *  Vector drawings exist for Accept, Add, Back, Cancel, Clear, Delete, Download, Edit, Favorite, Find,
	 *  Forward, Home, Mail, MailFilled, More, Next, OutlineStar, Pause, Pin, Play, Previous, Refresh, Remove, Save,
	 *  SaveLocal, Send, Setting, SolidStar, Stop, Sync, Up and Upload; any other Symbol is an empty box.
	 *  (The 8.1 Symbol has Find and Setting, no Search and Settings, and has no Share and no Down.)
	 * ============================================================================ */

	/** @brief IconElement.Foreground. */
	KESTREL_API void Kicon_element_set_foreground(Kid icon, KColor color);

	KESTREL_API Kid Ksymbol_icon_create(KSymbol symbol);
	KESTREL_API void Ksymbol_icon_set_symbol(Kid symbol_icon, KSymbol symbol);
	KESTREL_API KSymbol Ksymbol_icon_get_symbol(Kid symbol_icon);

	/** @brief A FontIcon: `glyph` is UTF-8, drawn in the font of the text engine (Selawik, FontSize 20 by default). */
	KESTREL_API Kid Kfont_icon_create(const char* glyph);
	KESTREL_API void Kfont_icon_set_glyph(Kid font_icon, const char* glyph);
	KESTREL_API const char* Kfont_icon_get_glyph(Kid font_icon);
	KESTREL_API void Kfont_icon_set_font_size(Kid font_icon, float font_size);
	KESTREL_API void Kfont_icon_set_font_family(Kid font_icon, KFontFamily family);

	/**
	 * @brief A PathIcon. `data` is Path.Data in the mini language "M x,y L x,y H x V y Z" (also m, l, h, v, z):
	 * every sub-path is a filled polygon, and the whole fits the icon uniformly. NULL or "" makes an empty one.
	 * @return K_INVALID_ID when the data is not understood.
	 */
	KESTREL_API Kid Kpath_icon_create(const char* data);
	/** @return K_FALSE, the icon unchanged, when the data is not understood. */
	KESTREL_API int Kpath_icon_set_data(Kid path_icon, const char* data);

	/** @brief A BitmapIcon from a PNG file, drawn as it is (it is not tinted with the foreground). K_INVALID_ID on failure. */
	KESTREL_API Kid Kbitmap_icon_create(const char* png_path);
	KESTREL_API int Kbitmap_icon_set_uri_source(Kid bitmap_icon, const char* png_path);


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
	 *  AppBarButton is a Button: Kbutton_on_click and Kbutton_set_flyout work on it, and the AppBarToggleButton
	 *  is a ToggleButton (Ktoggle_button_*). The Icon is the Content of the button.
	 * ============================================================================ */

	KESTREL_API Kid Kapp_bar_create(void);
	/** @brief IsOpen. Opened and Closed come when it changes (not when the animation ends). */
	KESTREL_API void Kapp_bar_set_is_open(Kid app_bar, int is_open);
	KESTREL_API int Kapp_bar_get_is_open(Kid app_bar);
	/** @brief IsSticky: K_TRUE keeps an open bar open through a press outside it and Back. */
	KESTREL_API void Kapp_bar_set_is_sticky(Kid app_bar, int is_sticky);
	KESTREL_API int Kapp_bar_get_is_sticky(Kid app_bar);
	/** @brief ClosedDisplayMode: K_APP_BAR_CLOSED_COMPACT (the default) or K_APP_BAR_CLOSED_MINIMAL. */
	KESTREL_API void Kapp_bar_set_closed_display_mode(Kid app_bar, KAppBarClosedDisplayMode mode);
	KESTREL_API KAppBarClosedDisplayMode Kapp_bar_get_closed_display_mode(Kid app_bar);
	KESTREL_API void Kapp_bar_on_opened(Kid app_bar, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kapp_bar_on_closed(Kid app_bar, KRoutedCallback callback, void* user_data);

	/** @brief A CommandBar. Its Content (Kcontent_control_set_content) is at the left, the primary commands at the right. */
	KESTREL_API Kid Kcommand_bar_create(void);
	/**
	 * @brief PrimaryCommands.Append and SecondaryCommands.Append: an AppBarButton, an AppBarToggleButton or an AppBarSeparator;
	 * the bar takes ownership. @return the index, -1 on failure.
	 */
	KESTREL_API int Kcommand_bar_add_primary_command(Kid command_bar, Kid command);
	KESTREL_API int Kcommand_bar_add_secondary_command(Kid command_bar, Kid command);
	KESTREL_API int Kcommand_bar_get_primary_command_count(Kid command_bar);
	KESTREL_API int Kcommand_bar_get_secondary_command_count(Kid command_bar);
	KESTREL_API Kid Kcommand_bar_get_primary_command(Kid command_bar, int index);
	KESTREL_API Kid Kcommand_bar_get_secondary_command(Kid command_bar, int index);
	/** @brief Clear: the commands are destroyed. */
	KESTREL_API void Kcommand_bar_clear_primary_commands(Kid command_bar);
	KESTREL_API void Kcommand_bar_clear_secondary_commands(Kid command_bar);

	/** @brief An AppBarButton with a Label and an Icon (an IconElement the button takes, or K_INVALID_ID). */
	KESTREL_API Kid Kapp_bar_button_create(const char* label, Kid icon);
	KESTREL_API Kid Kapp_bar_toggle_button_create(const char* label, Kid icon);
	/** @brief Label, Icon and IsCompact of an AppBarButton or an AppBarToggleButton. */
	KESTREL_API void Kapp_bar_button_set_label(Kid app_bar_button, const char* label);
	KESTREL_API const char* Kapp_bar_button_get_label(Kid app_bar_button);
	KESTREL_API void Kapp_bar_button_set_icon(Kid app_bar_button, Kid icon);
	KESTREL_API Kid Kapp_bar_button_get_icon(Kid app_bar_button);
	KESTREL_API void Kapp_bar_button_set_is_compact(Kid app_bar_button, int is_compact);
	KESTREL_API int Kapp_bar_button_get_is_compact(Kid app_bar_button);

	KESTREL_API Kid Kapp_bar_separator_create(void);
	KESTREL_API void Kapp_bar_separator_set_is_compact(Kid separator, int is_compact);
	KESTREL_API int Kapp_bar_separator_get_is_compact(Kid separator);


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
	 *  A flyout is owned by the program (Kelement_destroy) until Kbutton_set_flyout gives it to a button, which
	 *  shows it when it is tapped, after its Click.
	 * ============================================================================ */

	KESTREL_API void Kflyout_base_set_placement(Kid flyout, KFlyoutPlacementMode placement);
	KESTREL_API KFlyoutPlacementMode Kflyout_base_get_placement(Kid flyout);
	/** @brief ShowAt: `placement_target` must be in a tree that belongs to a root. */
	KESTREL_API void Kflyout_base_show_at(Kid flyout, Kid placement_target);
	KESTREL_API void Kflyout_base_hide(Kid flyout);
	/** @brief Whether it shows now (the property of the later flyouts, here for the program to ask). */
	KESTREL_API int Kflyout_base_get_is_open(Kid flyout);
	KESTREL_API void Kflyout_base_on_opening(Kid flyout, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kflyout_base_on_opened(Kid flyout, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kflyout_base_on_closed(Kid flyout, KRoutedCallback callback, void* user_data);

	KESTREL_API Kid Kflyout_create(void);
	/** @brief Flyout.Content: any element, owned by the flyout. FlyoutPresenterStyle is not supported. */
	KESTREL_API void Kflyout_set_content(Kid flyout, Kid content);
	KESTREL_API Kid Kflyout_get_content(Kid flyout);

	KESTREL_API Kid Kmenu_flyout_create(void);
	/** @brief Items.Append: a MenuFlyoutItem, a ToggleMenuFlyoutItem or a MenuFlyoutSeparator, owned by the menu. @return the index, -1 on failure. */
	KESTREL_API int Kmenu_flyout_add_item(Kid menu_flyout, Kid item);
	KESTREL_API int Kmenu_flyout_get_item_count(Kid menu_flyout);
	KESTREL_API Kid Kmenu_flyout_get_item(Kid menu_flyout, int index);
	KESTREL_API void Kmenu_flyout_clear_items(Kid menu_flyout);

	/** @brief A MenuFlyoutItem. Click runs on a tap, and the menu closes. */
	KESTREL_API Kid Kmenu_flyout_item_create(const char* text);
	KESTREL_API void Kmenu_flyout_item_set_text(Kid menu_flyout_item, const char* text);
	KESTREL_API const char* Kmenu_flyout_item_get_text(Kid menu_flyout_item);
	KESTREL_API void Kmenu_flyout_item_on_click(Kid menu_flyout_item, KRoutedCallback callback, void* user_data);
	/** @brief A ToggleMenuFlyoutItem: a tap flips IsChecked before Click runs; a check is drawn at the right edge when it is on (the template has none). */
	KESTREL_API Kid Ktoggle_menu_flyout_item_create(const char* text, int is_checked);
	KESTREL_API void Ktoggle_menu_flyout_item_set_is_checked(Kid toggle_menu_flyout_item, int is_checked);
	KESTREL_API int Ktoggle_menu_flyout_item_get_is_checked(Kid toggle_menu_flyout_item);
	KESTREL_API Kid Kmenu_flyout_separator_create(void);

	/** @brief Button.Flyout: the button takes ownership; a tap shows it at the button. K_INVALID_ID takes it away (and destroys it). */
	KESTREL_API void Kbutton_set_flyout(Kid button, Kid flyout);
	KESTREL_API Kid Kbutton_get_flyout(Kid button);


	/* ============================================================================
	 *  CONTENTDIALOG
	 *
	 *  A dialog at the top of the screen, as wide as it, in the chrome color: the title (25.5 px SemiBold),
	 *  the content (18.14 px) and under them the buttons, side by side in two halves (the primary one at the left,
	 *  and a button without text is not there). It shows in a modal Popup: what is under it is dimmed
	 *  (ContentDialogDimmingThemeBrush: black 60% in Dark, white 60% in Light) and gets no input, Tab stays in the dialog,
	 *  and Back or Hide closes it with the result None. The program owns the dialog (it is not put in a tree).
	 *
	 *    Kid dialog = Kcontent_dialog_create();
	 *    Kcontent_dialog_set_title(dialog, "Delete?");
	 *    Kcontent_control_set_text(dialog, "The file will be gone.");
	 *    Kcontent_dialog_set_primary_button_text(dialog, "delete");
	 *    Kcontent_dialog_set_secondary_button_text(dialog, "cancel");
	 *    Kcontent_dialog_show(dialog, root, done, NULL);   // done(dialog, K_CONTENT_DIALOG_RESULT_PRIMARY, NULL)
	 *
	 *  There is no await in C: ShowAsync is Kcontent_dialog_show with a callback that gets the result. Closed
	 *  comes before it. The keyboard focus goes to the primary button and comes back when the dialog closes.
	 *  Not included: Closing (it cannot be cancelled), FullSizeDesired, the buttons' Command, TitleTemplate.
	 * ============================================================================ */

	KESTREL_API Kid Kcontent_dialog_create(void);
	/** @brief Title as a string; "" or NULL takes it away. The Content is Kcontent_control_set_content / set_text. */
	KESTREL_API void Kcontent_dialog_set_title(Kid content_dialog, const char* title);
	KESTREL_API const char* Kcontent_dialog_get_title(Kid content_dialog);
	KESTREL_API void Kcontent_dialog_set_primary_button_text(Kid content_dialog, const char* text);
	KESTREL_API const char* Kcontent_dialog_get_primary_button_text(Kid content_dialog);
	KESTREL_API void Kcontent_dialog_set_secondary_button_text(Kid content_dialog, const char* text);
	KESTREL_API const char* Kcontent_dialog_get_secondary_button_text(Kid content_dialog);
	KESTREL_API void Kcontent_dialog_set_is_primary_button_enabled(Kid content_dialog, int is_enabled);
	KESTREL_API int Kcontent_dialog_get_is_primary_button_enabled(Kid content_dialog);
	KESTREL_API void Kcontent_dialog_set_is_secondary_button_enabled(Kid content_dialog, int is_enabled);
	KESTREL_API int Kcontent_dialog_get_is_secondary_button_enabled(Kid content_dialog);
	KESTREL_API void Kcontent_dialog_on_primary_button_click(Kid content_dialog, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kcontent_dialog_on_secondary_button_click(Kid content_dialog, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kcontent_dialog_on_opened(Kid content_dialog, KRoutedCallback callback, void* user_data);
	KESTREL_API void Kcontent_dialog_on_closed(Kid content_dialog, KRoutedCallback callback, void* user_data);
	/**
	 * @brief ShowAsync: shows the dialog over `root`. `callback` (may be NULL) gets what closed it.
	 * @return K_FALSE when it already shows or `root` is not a root.
	 */
	KESTREL_API int Kcontent_dialog_show(Kid content_dialog, Kid root, KContentDialogResultCallback callback, void* user_data);
	/** @brief Hide: closes the dialog with the result None. */
	KESTREL_API void Kcontent_dialog_hide(Kid content_dialog);

#ifdef __cplusplus
}
#endif

#endif
