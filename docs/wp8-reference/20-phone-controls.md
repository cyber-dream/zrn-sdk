# Phone controls (Microsoft.Phone.Controls, incl. Toolkit)

## Contents

- [`Microsoft.Phone.Controls.ContextMenu`](#microsoftphonecontrolscontextmenu)
- [`Microsoft.Phone.Controls.CultureInfoExtensions`](#microsoftphonecontrolscultureinfoextensions)
- [`Microsoft.Phone.Controls.CustomMessageBox`](#microsoftphonecontrolscustommessagebox)
- [`Microsoft.Phone.Controls.CustomMessageBoxResult`](#microsoftphonecontrolscustommessageboxresult)
- [`Microsoft.Phone.Controls.DailyDateTimeConverter`](#microsoftphonecontrolsdailydatetimeconverter)
- [`Microsoft.Phone.Controls.DatePicker`](#microsoftphonecontrolsdatepicker)
- [`Microsoft.Phone.Controls.DatePickerPage`](#microsoftphonecontrolsdatepickerpage)
- [`Microsoft.Phone.Controls.DateTimePickerBase`](#microsoftphonecontrolsdatetimepickerbase)
- [`Microsoft.Phone.Controls.DateTimePickerResources`](#microsoftphonecontrolsdatetimepickerresources)
- [`Microsoft.Phone.Controls.DateTimeValueChangedEventArgs`](#microsoftphonecontrolsdatetimevaluechangedeventargs)
- [`Microsoft.Phone.Controls.DismissedEventArgs`](#microsoftphonecontrolsdismissedeventargs)
- [`Microsoft.Phone.Controls.DismissingEventArgs`](#microsoftphonecontrolsdismissingeventargs)
- [`Microsoft.Phone.Controls.DragCompletedGestureEventArgs`](#microsoftphonecontrolsdragcompletedgestureeventargs)
- [`Microsoft.Phone.Controls.DragDeltaGestureEventArgs`](#microsoftphonecontrolsdragdeltagestureeventargs)
- [`Microsoft.Phone.Controls.DragStartedGestureEventArgs`](#microsoftphonecontrolsdragstartedgestureeventargs)
- [`Microsoft.Phone.Controls.ExpanderView`](#microsoftphonecontrolsexpanderview)
- [`Microsoft.Phone.Controls.ExpansionMode`](#microsoftphonecontrolsexpansionmode)
- [`Microsoft.Phone.Controls.FlickGestureEventArgs`](#microsoftphonecontrolsflickgestureeventargs)
- [`Microsoft.Phone.Controls.FullViewDateTimeConverter`](#microsoftphonecontrolsfullviewdatetimeconverter)
- [`Microsoft.Phone.Controls.GestureEventArgs`](#microsoftphonecontrolsgestureeventargs)
- [`Microsoft.Phone.Controls.GestureListener`](#microsoftphonecontrolsgesturelistener)
- [`Microsoft.Phone.Controls.GestureService`](#microsoftphonecontrolsgestureservice)
- [`Microsoft.Phone.Controls.GroupViewClosingEventArgs`](#microsoftphonecontrolsgroupviewclosingeventargs)
- [`Microsoft.Phone.Controls.GroupViewOpenedEventArgs`](#microsoftphonecontrolsgroupviewopenedeventargs)
- [`Microsoft.Phone.Controls.HourlyDateTimeConverter`](#microsoftphonecontrolshourlydatetimeconverter)
- [`Microsoft.Phone.Controls.HubTile`](#microsoftphonecontrolshubtile)
- [`Microsoft.Phone.Controls.HubTileService`](#microsoftphonecontrolshubtileservice)
- [`Microsoft.Phone.Controls.LinkUnlinkEventArgs`](#microsoftphonecontrolslinkunlinkeventargs)
- [`Microsoft.Phone.Controls.ListPicker`](#microsoftphonecontrolslistpicker)
- [`Microsoft.Phone.Controls.ListPickerItem`](#microsoftphonecontrolslistpickeritem)
- [`Microsoft.Phone.Controls.ListPickerMode`](#microsoftphonecontrolslistpickermode)
- [`Microsoft.Phone.Controls.ListPickerPage`](#microsoftphonecontrolslistpickerpage)
- [`Microsoft.Phone.Controls.ListViewDateTimeConverter`](#microsoftphonecontrolslistviewdatetimeconverter)
- [`Microsoft.Phone.Controls.LockablePivot`](#microsoftphonecontrolslockablepivot)
- [`Microsoft.Phone.Controls.LongListMultiSelector`](#microsoftphonecontrolslonglistmultiselector)
- [`Microsoft.Phone.Controls.LongListMultiSelectorItem`](#microsoftphonecontrolslonglistmultiselectoritem)
- [`Microsoft.Phone.Controls.LongListSelector`](#microsoftphonecontrolslonglistselector)
- [`Microsoft.Phone.Controls.LongListSelectorItem`](#microsoftphonecontrolslonglistselectoritem)
- [`Microsoft.Phone.Controls.LongListSelectorItemType`](#microsoftphonecontrolslonglistselectoritemtype)
- [`Microsoft.Phone.Controls.MenuItem`](#microsoftphonecontrolsmenuitem)
- [`Microsoft.Phone.Controls.MultiTouchGestureEventArgs`](#microsoftphonecontrolsmultitouchgestureeventargs)
- [`Microsoft.Phone.Controls.MultiselectItem`](#microsoftphonecontrolsmultiselectitem)
- [`Microsoft.Phone.Controls.MultiselectList`](#microsoftphonecontrolsmultiselectlist)
- [`Microsoft.Phone.Controls.NavigatingEventArgs`](#microsoftphonecontrolsnavigatingeventargs)
- [`Microsoft.Phone.Controls.NotifyEventArgs`](#microsoftphonecontrolsnotifyeventargs)
- [`Microsoft.Phone.Controls.NullToVisibilityConverter`](#microsoftphonecontrolsnulltovisibilityconverter)
- [`Microsoft.Phone.Controls.ObscuredEventArgs`](#microsoftphonecontrolsobscuredeventargs)
- [`Microsoft.Phone.Controls.OffOnConverter`](#microsoftphonecontrolsoffonconverter)
- [`Microsoft.Phone.Controls.OrientationChangedEventArgs`](#microsoftphonecontrolsorientationchangedeventargs)
- [`Microsoft.Phone.Controls.PageOrientation`](#microsoftphonecontrolspageorientation)
- [`Microsoft.Phone.Controls.Panorama`](#microsoftphonecontrolspanorama)
- [`Microsoft.Phone.Controls.PanoramaItem`](#microsoftphonecontrolspanoramaitem)
- [`Microsoft.Phone.Controls.PerformanceProgressBar`](#microsoftphonecontrolsperformanceprogressbar)
- [`Microsoft.Phone.Controls.PhoneApplicationFrame`](#microsoftphonecontrolsphoneapplicationframe)
- [`Microsoft.Phone.Controls.PhoneApplicationPage`](#microsoftphonecontrolsphoneapplicationpage)
- [`Microsoft.Phone.Controls.PhoneTextBox`](#microsoftphonecontrolsphonetextbox)
- [`Microsoft.Phone.Controls.PinchGestureEventArgs`](#microsoftphonecontrolspinchgestureeventargs)
- [`Microsoft.Phone.Controls.PinchStartedGestureEventArgs`](#microsoftphonecontrolspinchstartedgestureeventargs)
- [`Microsoft.Phone.Controls.Pivot`](#microsoftphonecontrolspivot)
- [`Microsoft.Phone.Controls.PivotItem`](#microsoftphonecontrolspivotitem)
- [`Microsoft.Phone.Controls.PivotItemEventArgs`](#microsoftphonecontrolspivotitemeventargs)
- [`Microsoft.Phone.Controls.Rating`](#microsoftphonecontrolsrating)
- [`Microsoft.Phone.Controls.RatingItem`](#microsoftphonecontrolsratingitem)
- [`Microsoft.Phone.Controls.RecurringDaysPicker`](#microsoftphonecontrolsrecurringdayspicker)
- [`Microsoft.Phone.Controls.RelativeTimeConverter`](#microsoftphonecontrolsrelativetimeconverter)
- [`Microsoft.Phone.Controls.SlideInEffect`](#microsoftphonecontrolsslideineffect)
- [`Microsoft.Phone.Controls.SpeechRecognizedEventArgs`](#microsoftphonecontrolsspeechrecognizedeventargs)
- [`Microsoft.Phone.Controls.SpeechTextBox`](#microsoftphonecontrolsspeechtextbox)
- [`Microsoft.Phone.Controls.SupportedPageOrientation`](#microsoftphonecontrolssupportedpageorientation)
- [`Microsoft.Phone.Controls.ThreadDateTimeConverter`](#microsoftphonecontrolsthreaddatetimeconverter)
- [`Microsoft.Phone.Controls.TileSize`](#microsoftphonecontrolstilesize)
- [`Microsoft.Phone.Controls.TileSizeToHeightConverter`](#microsoftphonecontrolstilesizetoheightconverter)
- [`Microsoft.Phone.Controls.TileSizeToWidthConverter`](#microsoftphonecontrolstilesizetowidthconverter)
- [`Microsoft.Phone.Controls.TiltEffect`](#microsoftphonecontrolstilteffect)
- [`Microsoft.Phone.Controls.TimePicker`](#microsoftphonecontrolstimepicker)
- [`Microsoft.Phone.Controls.TimePickerPage`](#microsoftphonecontrolstimepickerpage)
- [`Microsoft.Phone.Controls.ToggleSwitch`](#microsoftphonecontrolstoggleswitch)
- [`Microsoft.Phone.Controls.TransferControl`](#microsoftphonecontrolstransfercontrol)
- [`Microsoft.Phone.Controls.TransferMonitor`](#microsoftphonecontrolstransfermonitor)
- [`Microsoft.Phone.Controls.TransferRequestState`](#microsoftphonecontrolstransferrequeststate)
- [`Microsoft.Phone.Controls.TransferType`](#microsoftphonecontrolstransfertype)
- [`Microsoft.Phone.Controls.TurnstileFeatherEffect`](#microsoftphonecontrolsturnstilefeathereffect)
- [`Microsoft.Phone.Controls.WeakReferenceHelper`](#microsoftphonecontrolsweakreferencehelper)
- [`Microsoft.Phone.Controls.WebBrowser`](#microsoftphonecontrolswebbrowser)
- [`Microsoft.Phone.Controls.WebBrowserExtensions`](#microsoftphonecontrolswebbrowserextensions)
- [`Microsoft.Phone.Controls.WebBrowserNavigationException`](#microsoftphonecontrolswebbrowsernavigationexception)

## Microsoft.Phone.Controls.ContextMenu

Source: Toolkit · class : MenuBase · `Microsoft.Phone.Controls.Toolkit/ContextMenu/ContextMenu.cs`

Represents a pop-up menu that enables a control to expose functionality that is specific to the context of the control.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool IsFadeEnabled` | Gets or sets a value indicating whether the background will fade when the ContextMenu is open. IsZoomEnabled must be true for this value to take effect. | TK |
| `bool IsOpen` | Gets or sets a value indicating whether the ContextMenu is visible. | TK |
| `bool IsZoomEnabled` | Gets or sets a value indicating whether the background will zoom out when the ContextMenu is open. | TK |
| `DependencyObject Owner` | Gets or sets the owning object for the ContextMenu. | TK |
| `Rect? RegionOfInterest` | Gets or sets the region of interest expressed in the coordinate system of the root visual. A context menu will try to position itself outside the region of interest. If null, the owner's bounding box is considered the region of interest. | TK |
| `double VerticalOffset` | Gets or sets the vertical distance between the target origin and the popup alignment point. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty IsFadeEnabledProperty` | Identifies the IsFadeEnabled dependency property. | TK |
| `static DependencyProperty IsOpenProperty` | Identifies the IsOpen dependency property. | TK |
| `static DependencyProperty IsZoomEnabledProperty` | Identifies the IsZoomEnabled dependency property. | TK |
| `static DependencyProperty RegionOfInterestProperty` | Identifies the RegionOfInterest dependency property. | TK |
| `static DependencyProperty VerticalOffsetProperty` | Identifies the VerticalOffset dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler Closed` | Occurs when a particular instance of a ContextMenu closes. | TK |
| `RoutedEventHandler Opened` | Occurs when a particular instance of a ContextMenu opens. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Called when a new Template is applied. | TK |
| `«ctor» ContextMenu()` | Initializes a new instance of the ContextMenu class. | TK |

## Microsoft.Phone.Controls.CultureInfoExtensions

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Common/CultureInfoExtensions.cs`

Extends the CultureInfo class to add Weekdays and Weekends methods.

### Methods

| Member | Description | Src |
|---|---|---|
| `static ReadOnlyCollection<string> Weekdays(this CultureInfo culture)` | Returns a list of days that are weekdays in the given culture. <br>`culture`: The culture to lookup. | TK |
| `static ReadOnlyCollection<string> Weekends(this CultureInfo culture)` | Returns a list of days that are weekends in the given culture. <br>`culture`: The culture to lookup. | TK |

## Microsoft.Phone.Controls.CustomMessageBox

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/CustomMessageBox/CustomMessageBox.cs`

Represents a popup dialog with one or two buttons.

### Properties

| Member | Description | Src |
|---|---|---|
| `string Caption` | Gets or sets the caption. | TK |
| `bool IsFullScreen` | Gets or sets whether the message box occupies the whole screen. | TK |
| `bool IsLeftButtonEnabled` | Gets or sets whether the left button is enabled. | TK |
| `bool IsRightButtonEnabled` | Gets or sets whether the right button is enabled. | TK |
| `Object LeftButtonContent` | Gets or sets the left button content. | TK |
| `string Message` | Gets or sets the message. | TK |
| `Object RightButtonContent` | Gets or sets the right button content. | TK |
| `string Title` | Gets or sets the title. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty CaptionProperty` | Identifies the Caption dependency property. | TK |
| `static DependencyProperty IsFullScreenProperty` | Identifies the IsFullScreen dependency property. | TK |
| `static DependencyProperty IsLeftButtonEnabledProperty` | Identifies the IsLeftButtonEnabled dependency property. | TK |
| `static DependencyProperty IsRightButtonEnabledProperty` | Identifies the IsRightButtonEnabled dependency property. | TK |
| `static DependencyProperty LeftButtonContentProperty` | Identifies the LeftButtonContent dependency property. | TK |
| `static DependencyProperty MessageProperty` | Identifies the Message dependency property. | TK |
| `static DependencyProperty RightButtonContentProperty` | Identifies the RightButtonContent dependency property. | TK |
| `static DependencyProperty TitleProperty` | Identifies the Title dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<DismissedEventArgs> Dismissed` | Called when the message box is dismissed. | TK |
| `EventHandler<DismissingEventArgs> Dismissing` | Called when the message box is about to be dismissed. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void Dismiss()` | Dismisses the message box. | TK |
| `void OnApplyTemplate()` | Gets the template parts and attaches event handlers. Animates the message box when the template is applied to it. | TK |
| `void Show()` | Reveals the message box by inserting it into a popup and opening it. | TK |
| `«ctor» CustomMessageBox() : base()` | Initializes a new instance of the CustomMessageBox class. | TK |

## Microsoft.Phone.Controls.CustomMessageBoxResult

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/CustomMessageBox/CustomMessageBoxResult.cs`

Describes the different possible results for the dismissal of a CustomMessageBox.

### Values

| Member | Description | Src |
|---|---|---|
| `LeftButton` | The left button contains the result value of the custom message box. | TK |
| `RightButton` | The right button contains the result value of custom the message box. | TK |
| `None` | The custom message box has no result. | TK |

## Microsoft.Phone.Controls.DailyDateTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/DailyDateTimeConverter.cs`

Date and time converter for daily feeds.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string appropriately formatted for daily feeds. This format can be found in the call history. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.DatePicker

Source: Toolkit · class : DateTimePickerBase · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DatePicker.cs`

Represents a control that allows the user to choose a date (day/month/year).

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» DatePicker()` | Initializes a new instance of the DatePicker control. | TK |

## Microsoft.Phone.Controls.DatePickerPage

Source: Toolkit · class : DateTimePickerPageBase · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DatePickerPage.xaml.cs`

Represents a page used by the DatePicker control that allows the user to choose a date (day/month/year).

### Methods

| Member | Description | Src |
|---|---|---|
| `void SetFlowDirection(FlowDirection flowDirection)` | Sets the selectors and title flow direction. <br>`flowDirection`: Flow direction to set. | TK |
| `«ctor» DatePickerPage()` | Initializes a new instance of the DatePickerPage control. | TK |

## Microsoft.Phone.Controls.DateTimePickerBase

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DateTimePickerBase.cs`

Represents a base class for controls that allow the user to choose a date/time.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Header` | Gets or sets the header of the control. | TK |
| `DataTemplate HeaderTemplate` | Gets or sets the template used to display the control's header. | TK |
| `Uri PickerPageUri` | Gets or sets the Uri to use for loading the IDateTimePickerPage instance when the control is clicked. | TK |
| `DateTime? Value` | Gets or sets the DateTime value. | TK |
| `string ValueString` | Gets the string representation of the selected value. | TK |
| `string ValueStringFormat` | Gets or sets the format string to use when converting the Value property to a string. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty HeaderProperty` | Identifies the Header DependencyProperty. | TK |
| `static DependencyProperty HeaderTemplateProperty` | Identifies the HeaderTemplate DependencyProperty. | TK |
| `static DependencyProperty PickerPageUriProperty` | Identifies the PickerPageUri DependencyProperty. | TK |
| `static DependencyProperty ValueProperty` | Identifies the Value DependencyProperty. | TK |
| `static DependencyProperty ValueStringFormatProperty` | Identifies the ValueStringFormat DependencyProperty. | TK |
| `static DependencyProperty ValueStringProperty` | Identifies the ValueString DependencyProperty. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<DateTimeValueChangedEventArgs> ValueChanged` | Event that is invoked when the Value property changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Called when the control's Template is expanded. | TK |
| `«ctor» DateTimePickerBase()` | Initializes a new instance of the DateTimePickerBase control. | TK |

## Microsoft.Phone.Controls.DateTimePickerResources

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DateTimePickerResources.cs`

Provides access to the localized resources used by the DatePicker and TimePicker.

### Properties

| Member | Description | Src |
|---|---|---|
| `static string DatePickerTitle` | Gets the localized DatePicker title string. | TK |
| `static string TimePickerTitle` | Gets the localized TimePicker title string. | TK |

## Microsoft.Phone.Controls.DateTimeValueChangedEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DateTimeValueChangedEventArgs.cs`

Provides data for the DatePicker and TimePicker's ValueChanged event.

### Properties

| Member | Description | Src |
|---|---|---|
| `DateTime? NewDateTime` | Gets or sets the new DateTime value. | TK |
| `DateTime? OldDateTime` | Gets or sets the old DateTime value. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» DateTimeValueChangedEventArgs(DateTime? oldDateTime, DateTime? newDateTime)` | Initializes a new instance of the DateTimeValueChangedEventArgs class. <br>`oldDateTime`: Old DateTime value. <br>`newDateTime`: New DateTime value. | TK |

## Microsoft.Phone.Controls.DismissedEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/CustomMessageBox/DismissedEventArgs.cs`

Provides data for the CustomMessageBox's Dismissed event.

### Properties

| Member | Description | Src |
|---|---|---|
| `CustomMessageBoxResult Result` | Gets or sets the result value. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» DismissedEventArgs(CustomMessageBoxResult result)` | Initializes a new instance of the DismissedEventArgs class. <br>`result`: The result value. | TK |

## Microsoft.Phone.Controls.DismissingEventArgs

Source: Toolkit · class : CancelEventArgs · `Microsoft.Phone.Controls.Toolkit/CustomMessageBox/DismissingEventArgs.cs`

Provides data for the CustomMessageBox's Dismissing event.

### Properties

| Member | Description | Src |
|---|---|---|
| `CustomMessageBoxResult Result` | Gets or sets the result value. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» DismissingEventArgs(CustomMessageBoxResult result)` | Initializes a new instance of the DismissingEventArgs class. <br>`result`: The result value. | TK |

## Microsoft.Phone.Controls.DragCompletedGestureEventArgs

Source: Toolkit · class : GestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used by the DragCompleted event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation Direction` | The direction of the drag gesture, as determined by the initial drag change. | TK |
| `double HorizontalChange` | The total horizontal (X) change of the drag event. | TK |
| `double HorizontalVelocity` | The final horizontal (X) velocity of the drag, if the drag was inertial. | TK |
| `double VerticalChange` | The total vertical (Y) change of the drag event. | TK |
| `double VerticalVelocity` | The final vertical (Y) velocity of the drag, if the drag was inertial. | TK |

## Microsoft.Phone.Controls.DragDeltaGestureEventArgs

Source: Toolkit · class : GestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used by the DragDelta event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation Direction` | The direction of the drag gesture, as determined by the initial drag change. | TK |
| `double HorizontalChange` | The horizontal (X) change for this drag event. | TK |
| `double VerticalChange` | The vertical (Y) change for this drag event. | TK |

## Microsoft.Phone.Controls.DragStartedGestureEventArgs

Source: Toolkit · class : GestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used in the DragStarted event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation Direction` | The direction of the drag gesture, as determined by the initial drag change. | TK |

## Microsoft.Phone.Controls.ExpanderView

Source: Toolkit · class : HeaderedItemsControl · `Microsoft.Phone.Controls.Toolkit/ExpanderView/ExpanderView.cs`

Represents a collection of items that can be expanded or collapsed.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Expander` | Gets or sets the expander object. | TK |
| `DataTemplate ExpanderTemplate` | Gets or sets the data template that defines the expander. | TK |
| `bool HasItems` | Gets or sets the flag that indicates whether the Expander View has items. | TK |
| `bool IsExpanded` | Gets or sets the flag that indicates whether the Expander View is expanded. | TK |
| `bool IsNonExpandable` | Gets or sets the flag that indicates whether the Expander View is non-expandable. | TK |
| `object NonExpandableHeader` | Gets or sets the header object that is displayed when the Expander View is non-expandable. | TK |
| `DataTemplate NonExpandableHeaderTemplate` | Gets or sets the data template that defines the non-expandable header. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `string CollapsedState` | Collapsed visual state. | TK |
| `string ExpandabilityStates` | Expandability visual states. | TK |
| `string ExpandableState` | Expandable visual state. | TK |
| `string ExpandedState` | Expanded visual state. | TK |
| `static DependencyProperty ExpanderProperty` | Identifies the Expander dependency property. | TK |
| `static DependencyProperty ExpanderTemplateProperty` | Identifies the ExpanderTemplate dependency property. | TK |
| `string ExpansionStates` | Expansion visual states. | TK |
| `static DependencyProperty HasItemsProperty` | Identifies the HasItems dependency property. | TK |
| `static DependencyProperty IsExpandedProperty` | Identifies the IsExpanded dependency property. | TK |
| `static DependencyProperty IsNonExpandableProperty` | Identifies the NonExpandable dependency property. | TK |
| `static DependencyProperty NonExpandableHeaderProperty` | Identifies the NonExpandableHeader dependency property. | TK |
| `static DependencyProperty NonExpandableHeaderTemplateProperty` | Identifies the NonExpandableHeaderTemplate dependency property. | TK |
| `string NonExpandableState` | NonExpandable visual state. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler Collapsed` | Occurs when the Expander View closes to hide its content. | TK |
| `RoutedEventHandler Expanded` | Occurs when the Expander View opens to display its content. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Gets the template parts and sets event handlers. | TK |
| `«ctor» ExpanderView()` | Initializes a new instance of the ExpanderView class. | TK |

## Microsoft.Phone.Controls.ExpansionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/ListPicker/ExpansionMode.cs`

The expansion mode of a ListPicker.

### Values

| Member | Description | Src |
|---|---|---|
| `ExpansionAllowed` | Allows all items to be visible on the page. | TK |
| `FullScreenOnly` | Requires all items to be displayed in a full screen popup. | TK |

## Microsoft.Phone.Controls.FlickGestureEventArgs

Source: Toolkit · class : GestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used by the Flick event.

### Properties

| Member | Description | Src |
|---|---|---|
| `double Angle` | The angle of the flick. | TK |
| `Orientation Direction` | The direction of the flick gesture, as determined by the flick velocities. | TK |
| `double HorizontalVelocity` | The horizontal (X) velocity of the flick. | TK |
| `double VerticalVelocity` | The vertical (Y) velocity of the flick. | TK |

## Microsoft.Phone.Controls.FullViewDateTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/FullViewDateTimeConverter.cs`

Date and time converter to display information in full format.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string appropriately formatted to display full date and time information. This format can be found in email. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.GestureEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The base class for all gesture events. Also used by Tap, DoubleTap and Hold.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool Handled` | If an event handler sets this to true, it stops event bubbling. | TK |
| `object OriginalSource` | The first hit-testable item under the touch point. Determined by a combination of order in the tree and Z-order. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Point GetPosition(UIElement relativeTo)` | Returns the position of the gesture's starting point relative to a given UIElement. <br>`relativeTo`: The return value will be relative to this element. <br>Returns: The gesture's starting point relative to the given UIElement. | TK |

## Microsoft.Phone.Controls.GestureListener

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Input/GestureListener.cs`

_No description._

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<GestureEventArgs> DoubleTap` | The DoubleTap event is raised instead of Tap if the time between two taps is short eonugh. | TK |
| `EventHandler<DragCompletedGestureEventArgs> DragCompleted` | The DragCompleted event. Will be raised on touch release after a drag, or when a second touch point is added. | TK |
| `EventHandler<DragDeltaGestureEventArgs> DragDelta` | The DragDelta event. | TK |
| `EventHandler<DragStartedGestureEventArgs> DragStarted` | The DragStarted event. | TK |
| `EventHandler<FlickGestureEventArgs> Flick` | The Flick event. Raised when a drag that was fast enough ends with a release. | TK |
| `EventHandler<GestureEventArgs> GestureBegin` | The GestureBegin event. | TK |
| `EventHandler<GestureEventArgs> GestureCompleted` | The GestureCompleted event. | TK |
| `EventHandler<GestureEventArgs> Hold` | The Hold event (touch and hold for one second) | TK |
| `EventHandler<PinchGestureEventArgs> PinchCompleted` | The end of a pinch operation. | TK |
| `EventHandler<PinchGestureEventArgs> PinchDelta` | Any two-touch point (two finger) operation. | TK |
| `EventHandler<PinchStartedGestureEventArgs> PinchStarted` | The PinchStarted event. | TK |
| `EventHandler<GestureEventArgs> Tap` | The Tap event (touch, release, no movement). | TK |

## Microsoft.Phone.Controls.GestureService

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit.WP8/Input/GestureService.cs`

The GestureService class is the helper for getting and setting GestureListeners on elements.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty GestureListenerProperty` | The definition of the GestureListener attached DependencyProperty. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static GestureListener GetGestureListener(DependencyObject obj)` | Gets a GestureListener for the new element. Will create a new one if necessary. <br>`obj`: The object to get the GestureListener from. <br>Returns: Either the previously existing GestureListener, or a new one. | TK |
| `static void SetGestureListener(DependencyObject obj, GestureListener value)` | Sets the GestureListener on an element. Needed for XAML, but should not be used in code. Use GetGestureListener instead, which will create a new instance if one is not already set, to add your handlers to an element. <br>`obj`: The object to set the GestureListener on. <br>`value`: The GestureListener. | TK |

## Microsoft.Phone.Controls.GroupViewClosingEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorEventArgs.cs`

The GroupPopupClosing event args.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool Cancel` | Set this to true if the application will handle the popup closing and scrolling to the group. | TK |
| `ItemsControl ItemsControl` | The ItemsControl containing the groups. | TK |
| `object SelectedGroup` | The selected group. Will be null if the back button was pressed. | TK |

## Microsoft.Phone.Controls.GroupViewOpenedEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorEventArgs.cs`

The GroupPopupOpened event args.

### Properties

| Member | Description | Src |
|---|---|---|
| `ItemsControl ItemsControl` | The ItemsControl containing the groups. | TK |

## Microsoft.Phone.Controls.HourlyDateTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/HourlyDateTimeConverter.cs`

Date and time converter for hourly feeds.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string appropriately formatted for hourly feeds. This format can be found in messaging. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.HubTile

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/HubTile/HubTile.cs`

Represents an animated tile that supports an image and a title. Furthermore, it can also be associated with a message or a notification.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool DisplayNotification` | Gets or sets the flag for new notifications. | TK |
| `string GroupTag` | Gets or sets the group tag. | TK |
| `bool IsFrozen` | Gets or sets the flag for images that do not animate. | TK |
| `string Message` | Gets or sets the message. | TK |
| `string Notification` | Gets or sets the notification alert. | TK |
| `TileSize Size` | Gets or sets the visual state. | TK |
| `ImageSource Source` | Gets or sets the image source. | TK |
| `string Title` | Gets or sets the title. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty DisplayNotificationProperty` | Identifies the DisplayNotification dependency property. | TK |
| `static DependencyProperty GroupTagProperty` | Identifies the GroupTag dependency property. | TK |
| `static DependencyProperty IsFrozenProperty` | Identifies the IsFrozen dependency property. | TK |
| `static DependencyProperty MessageProperty` | Identifies the Message dependency property. | TK |
| `static DependencyProperty NotificationProperty` | Identifies the Notification dependency property. | TK |
| `static DependencyProperty SizeProperty` | Identifies the State dependency property. | TK |
| `static DependencyProperty SourceProperty` | Identifies the Source dependency property. | TK |
| `static DependencyProperty TitleProperty` | Identifies the Title dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Gets the template parts and sets binding. | TK |
| `«ctor» HubTile()` | Initializes a new instance of the HubTile class. | TK |

## Microsoft.Phone.Controls.HubTileService

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/HubTile/HubTileService.cs`

Provides organized animations for the hub tiles.

### Methods

| Member | Description | Src |
|---|---|---|
| `static void FreezeGroup(string group)` | Freezes all the hub tiles with the specified group tag that are not already frozen. <br>`group`: The group tag representing the hub tiles that should be frozen. | TK |
| `static void FreezeHubTile(HubTile tile)` | Freeze a hub tile. <br>`tile`: The hub tile to be frozen. | TK |
| `static void UnfreezeGroup(string group)` | Unfreezes all the hub tiles with the specified group tag that are currently frozen and restarts the timer if needed. <br>`group`: The group tag representing the hub tiles that should be unfrozen. | TK |
| `static void UnfreezeHubTile(HubTile tile)` | Unfreezes a hub tile and restarts the timer if needed. <br>`tile`: The hub tile to be unfrozen. | TK |

## Microsoft.Phone.Controls.LinkUnlinkEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorEventArgs.cs`

The event args for the Link/Unlink events.

### Properties

| Member | Description | Src |
|---|---|---|
| `ContentPresenter ContentPresenter` | The ContentPresenter which is displaying the item. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» LinkUnlinkEventArgs(ContentPresenter contentPresenter)` | Create new LinkUnlinkEventArgs. <br>`contentPresenter`: The ContentPresenter. | TK |

## Microsoft.Phone.Controls.ListPicker

Source: Toolkit · class : ItemsControl · `Microsoft.Phone.Controls.Toolkit/ListPicker/ListPicker.cs`

Class that implements a flexible list-picking experience with a custom interface for few/many items.

### Properties

| Member | Description | Src |
|---|---|---|
| `ExpansionMode ExpansionMode` | Gets or sets how the list picker expands when tapped. This property has an effect only when SelectionMode is Single. When SelectionMode is Multiple, the ExpansionMode will be treated as FullScreenOnly. ExpansionAllowed will only expand when the number of items is less than or equalt to ItemCountThreshold Single by default. | TK |
| `object FullModeHeader` | Gets or sets the header to use when ListPickerMode is set to Full. | TK |
| `DataTemplate FullModeItemTemplate` | Gets or sets the DataTemplate used to display each item when ListPickerMode is set to Full. | TK |
| `object Header` | Gets or sets the header of the control. | TK |
| `DataTemplate HeaderTemplate` | Gets or sets the template used to display the control's header. | TK |
| `int ItemCountThreshold` | Gets the maximum number of items for which Expanded mode will be used, 5. | TK |
| `ListPickerMode ListPickerMode` | Gets or sets the ListPickerMode (ex: Normal/Expanded/Full). | TK |
| `Uri PickerPageUri` | Gets or sets the Uri to use for loading the ListPickerPage instance when the control is tapped. | TK |
| `int SelectedIndex` | Gets or sets the index of the selected item. | TK |
| `object SelectedItem` | Gets or sets the selected item. | TK |
| `IList SelectedItems` | Gets the selected items. | TK |
| `SelectionMode SelectionMode` | Gets or sets the SelectionMode. Extended is treated as Multiple. Single by default. | TK |
| `Func<IList, string> SummaryForSelectedItemsDelegate` | Gets or sets the delegate, which is called to summarize a list of selections into a string. If not implemented, the default summarizing behavior will be used. If this delegate is implemented, default summarizing behavior can be achieved by returning null instead of a string. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ExpansionModeProperty` | Identifies the ExpansionMode DependencyProperty. | TK |
| `static DependencyProperty FullModeHeaderProperty` | Identifies the FullModeHeader DependencyProperty. | TK |
| `static DependencyProperty FullModeItemTemplateProperty` | Identifies the FullModeItemTemplate DependencyProperty. | TK |
| `static DependencyProperty HeaderProperty` | Identifies the Header DependencyProperty. | TK |
| `static DependencyProperty HeaderTemplateProperty` | Identifies the HeaderTemplate DependencyProperty. | TK |
| `static DependencyProperty ItemCountThresholdProperty` | Identifies the ItemCountThreshold DependencyProperty. | TK |
| `static DependencyProperty ListPickerModeProperty` | Identifies the ListPickerMode DependencyProperty. | TK |
| `static DependencyProperty PickerPageUriProperty` | Identifies the PickerPageUri DependencyProperty. | TK |
| `static DependencyProperty SelectedIndexProperty` | Identifies the SelectedIndex DependencyProperty. | TK |
| `static DependencyProperty SelectedItemProperty` | Identifies the SelectedItem DependencyProperty. | TK |
| `static DependencyProperty SelectedItemsProperty` | Identifies the SelectedItems DependencyProperty. | TK |
| `static DependencyProperty SelectionModeProperty` | Identifies the SelectionMode DependencyProperty. | TK |
| `static DependencyProperty SummaryForSelectedItemsDelegateProperty` | Identifies the SummaryForSelectedItemsDelegate DependencyProperty. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `SelectionChangedEventHandler SelectionChanged` | Event that is raised when the selection changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `bool Open()` | Opens the picker for selection either into Expanded or Full mode depending on the picker's state. <br>Returns: Whether the picker was succesfully opened. | TK |
| `void OnApplyTemplate()` | Builds the visual tree for the control when a new template is applied. | TK |
| `«ctor» ListPicker()` | Initializes a new instance of the ListPicker class. | TK |

## Microsoft.Phone.Controls.ListPickerItem

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/ListPicker/ListPickerItem.cs`

Class that implements a container for the ListPicker control.

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Builds the visual tree for the control when a new template is applied. | TK |
| `«ctor» ListPickerItem()` | Initializes a new instance of the ListPickerItem class. | TK |

## Microsoft.Phone.Controls.ListPickerMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/ListPicker/ListPickerMode.cs`

An enumeration defining the supported ListPicker modes.

### Values

| Member | Description | Src |
|---|---|---|
| `Normal` | Normal mode; only the selected item is visible on the original page. | TK |
| `Expanded` | Expanded mode; all items are visible on the original page. | TK |
| `Full` | Full mode; all items are visible in a separate Popup. | TK |

## Microsoft.Phone.Controls.ListPickerPage

Source: Toolkit · class : PhoneApplicationPage · `Microsoft.Phone.Controls.Toolkit.WP8/ListPicker/ListPickerPage.xaml.cs`

Displays the list of items and allows single or multiple selection.

### Properties

| Member | Description | Src |
|---|---|---|
| `DataTemplate FullModeItemTemplate` | Gets or sets the item template | TK |
| `string HeaderText` | Gets or sets the string of text to display as the header of the page. | TK |
| `IList Items` | Gets or sets the list of items to display. | TK |
| `object SelectedItem` | Gets or sets the selected item. | TK |
| `IList SelectedItems` | Gets or sets the list of items to select. | TK |
| `SelectionMode SelectionMode` | Gets or sets the selection mode. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» ListPickerPage()` | Creates a list picker page. | TK |

## Microsoft.Phone.Controls.ListViewDateTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/ListViewDateTimeConverter.cs`

Date and time converter to be used in list-views.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string appropriately formatted for list-views. This format can be found in email. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.LockablePivot

Source: Toolkit · class : Pivot · `Microsoft.Phone.Controls.Toolkit/Pivot/LockablePivot.cs`

The lockable Pivot extends the base Pivot control with a property that disables navigation between Pivot items.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool IsLocked` | Gets or sets a value indicating whether the control is locked. | TK |
| `PivotHeaderItem headerItem` | — | TK |
| `double opacity` | — | TK |
| `OpacityAnimator opacityAnimator` | — | TK |
| `double originalX` | — | TK |
| `TranslateTransform transform` | — | TK |
| `TransformAnimator transformAnimator` | — | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty IsLockedProperty` | Identifies the IsLocked dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» LockablePivot()` | Initializes a new instance of the LockablePivot type. | TK |

## Microsoft.Phone.Controls.LongListMultiSelector

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit.WP8/LongListMultiSelector/LongListMultiSelector.cs`

Extension of the standard LongListSelector control which allows multiple selection of items

### Properties

| Member | Description | Src |
|---|---|---|
| `bool EnforceIsSelectionEnabled` | Gets or sets the EnforceIsSelectionEnabled property | TK |
| `Size GridCellSize` | Gets or sets the size used when displaying an item in the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `DataTemplate GroupFooterTemplate` | Gets or sets the template for the group footer in the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `DataTemplate GroupHeaderTemplate` | Gets or sets the template for the group header in the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `bool HideEmptyGroups` | Gets or sets a value that indicates whether to hide empty groups in the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `bool IsGroupingEnabled` | Gets or sets a value that indicates whether grouping is enabled in the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `bool IsSelectionEnabled` | Gets or sets the flag that indicates if the list is in selection mode or not. | TK |
| `Style ItemContainerStyle` | Gets or sets the template for items hosting in the Microsoft.Phone.Controls.LongListMultiSelector, targeted to customize selection highlighting | TK |
| `DataTemplate ItemInfoTemplate` | Gets or sets the data template that is to be right align in default template and will not move when selection is opened | TK |
| `DataTemplate ItemTemplate` | Gets or sets the template for the items in the items view | TK |
| `IList ItemsSource` | Gets or sets a collection used to generate the content of the Microsoft.Phone.Controls.LongListMultiSelector. | TK |
| `Style JumpListStyle` | Gets or sets the System.Windows.Style for jump list in the Microsoft.Phone.Controls.LongListSelector. | TK |
| `LongListSelectorLayoutMode LayoutMode` | Gets or sets a value that specifies if the Microsoft.Phone.Controls.LongListSelector is in a list mode or grid mode from the Microsoft.Phone.Controls.LongListSelectorLayoutMode enum. | TK |
| `object ListFooter` | Gets or sets the object that is displayed at the foot of the Microsoft.Phone.Controls.LongListSelector. | TK |
| `DataTemplate ListFooterTemplate` | Gets or sets the System.Windows.DataTemplatefor an item to display at the foot of the Microsoft.Phone.Controls.LongListSelector. | TK |
| `object ListHeader` | Gets or sets the object to display at the head of the Microsoft.Phone.Controls.LongListSelector. | TK |
| `DataTemplate ListHeaderTemplate` | Gets or sets the System.Windows.DataTemplatefor an item to display at the head of the Microsoft.Phone.Controls.LongListSelector. | TK |
| `ManipulationState ManipulationState` | Gets the state of manipulation handling on the Microsoft.Phone.Controls.LongListSelector control. | TK |
| `IList OldItems` | Items removed from the list | TK |
| `IList SelectedItems` | Gets the currently selected items in the Microsoft.Phone.Controls.LongListSelector. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty EnforceIsSelectionEnabledProperty` | Identifies the EnforceIsSelectionEnabled dependency property. | TK |
| `static DependencyProperty GridCellSizeProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.GridCellSize dependency property. | TK |
| `static DependencyProperty GroupFooterTemplateProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.GroupFooterTemplate dependency property. | TK |
| `static DependencyProperty GroupHeaderTemplateProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.GroupHeaderTemplate dependency property. | TK |
| `static DependencyProperty HideEmptyGroupsProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.HideEmptyGroups dependency property. | TK |
| `static DependencyProperty IsGroupingEnabledProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.IsGroupingEnabled dependency property. | TK |
| `static DependencyProperty IsSelectionEnabledProperty` | Identifies the IsSelectionEnabled dependency property. | TK |
| `static DependencyProperty ItemContainerStyleProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ItemContainerStyle dependency property. | TK |
| `static DependencyProperty ItemInfoTemplateProperty` | Identifies the ItemInfoTemplate dependency property. | TK |
| `static DependencyProperty ItemTemplateProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ItemTemplate dependency property. | TK |
| `static DependencyProperty ItemsSourceProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ItemsSource dependency property. | TK |
| `static DependencyProperty JumpListStyleProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.JumpListStyle dependency property. | TK |
| `static DependencyProperty ListFooterProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ListFooter dependency property. | TK |
| `static DependencyProperty ListFooterTemplateProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ListFooterTemplate dependency property. | TK |
| `static DependencyProperty ListHeaderProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ListHeader dependency property. | TK |
| `static DependencyProperty ListHeaderTemplateProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.ListHeaderTemplate dependency property. | TK |
| `static DependencyProperty SelectedItemsProperty` | Identifies the Microsoft.Phone.Controls.LongListMultiSelector.SelectedItems dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<ClearedChangedArgs> CollectionCleared` | Event indicating that collection has changed | TK |
| `DependencyPropertyChangedEventHandler IsSelectionEnabledChanged` | Occurs when the selection mode is opened or closed. | TK |
| `EventHandler<ItemRealizationEventArgs> ItemRealized` | Occurs when a new item is realized. | TK |
| `EventHandler<ItemRealizationEventArgs> ItemUnrealized` | Occurs when an item in the Microsoft.Phone.Controls.LongListMultiSelector is unrealized. | TK |
| `EventHandler JumpListClosed` | Occurs when the jump list is closed. | TK |
| `EventHandler JumpListOpening` | Occurs when a jump list is opened. | TK |
| `EventHandler ManipulationStateChanged` | Occurs when Microsoft.Phone.Controls.ManipulationState changes. | TK |
| `PropertyChangedEventHandler PropertyChanged` | Occurs when a property value changes. | TK |
| `SelectionChangedEventHandler SelectionChanged` | Occurs when the currently selected item changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `object ContainerFromItem(object item)` | Returns the LongListMultiSelectorItem corresponding to the given item <br>`item`: Item whose container has to be returned | TK |
| `void OnApplyTemplate()` | Template application : gets and hooks the inner LongListSelector | TK |
| `void ScrollTo(object item)` | Scrolls to a specified item in the Microsoft.Phone.Controls.LongListSelector. <br>`item`: The list item to scroll to. | TK |
| `«ctor» ClearedChangedArgs(IList items)` | Constructs a NotifyItemsChangedArgs from an action and a list of items <br>`items`:  | TK |
| `«ctor» LongListMultiSelector()` | Initializes a new instance of the Microsoft.Phone.Controls.LongListMultiSelector | TK |

## Microsoft.Phone.Controls.LongListMultiSelectorItem

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit.WP8/LongListMultiSelector/LongListMultiSelectorItem.cs`

Class for LongListMultiSelector items

### Properties

| Member | Description | Src |
|---|---|---|
| `object ContentInfo` | Gets or sets the content information. | TK |
| `DataTemplate ContentInfoTemplate` | Gets or sets the data template that defines the content information. | TK |
| `double HintPanelHeight` | Gets or sets the height of the hint panel. | TK |
| `bool IsSelected` | Gets or sets the flag indicating that the item is selected | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ContentInfoProperty` | Identifies the ContentInfo dependency property. | TK |
| `static DependencyProperty ContentInfoTemplateProperty` | Identifies the ContentInfoTemplate dependency property. | TK |
| `static DependencyProperty HintPanelHeightProperty` | Identifies the HintPanelHeight dependency property. | TK |
| `static DependencyProperty IsSelectedProperty` | Identifies the IsSelected dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler IsSelectedChanged` | Triggered when the IsSelected property has changed | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Template application : hooks events which need to be handled | TK |
| `«ctor» LongListMultiSelectorItem()` | Constructor | TK |

## Microsoft.Phone.Controls.LongListSelector

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorItemsControl.cs`

Partial definition of LongListSelector. Includes ItemsControl subclass.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Group` | — | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `GroupSelectedEventHandler GroupSelected` | — | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» GroupSelectedEventArgs(object group)` | — | TK |

## Microsoft.Phone.Controls.LongListSelectorItem

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorItem.cs`

Holds information about an item for use in the LongListSelector.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Group` | Gets or sets the associated group for the item. | TK |
| `object Item` | Gets or sets the underlying item instance. | TK |
| `LongListSelectorItemType ItemType` | Gets or sets the item type. | TK |

## Microsoft.Phone.Controls.LongListSelectorItemType

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/LongListSelector/LongListSelectorItemType.cs`

Describes different items.

### Values

| Member | Description | Src |
|---|---|---|
| `Unknown` | Indicates an unknown item type. | TK |
| `Item` | Represents a standard list item. | TK |
| `GroupHeader` | Represents a group header. | TK |
| `GroupFooter` | Represents a group footer. | TK |
| `ListHeader` | Represents a list header. | TK |
| `ListFooter` | Represents a list footer. | TK |

## Microsoft.Phone.Controls.MenuItem

Source: Toolkit · class : HeaderedItemsControl // , ICommandSource // ICommandSource not defined by Silverlight 4 · `Microsoft.Phone.Controls.Toolkit/ContextMenu/MenuItem.cs`

Represents a selectable item inside a Menu or ContextMenu.

### Properties

| Member | Description | Src |
|---|---|---|
| `ICommand Command` | Gets or sets the command associated with the menu item. | TK |
| `object CommandParameter` | Gets or sets the parameter to pass to the Command property of a MenuItem. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty CommandParameterProperty` | Identifies the CommandParameter dependency property. | TK |
| `static DependencyProperty CommandProperty` | Identifies the Command dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler Click` | Occurs when a MenuItem is clicked. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Called when the template's tree is generated. | TK |
| `«ctor» MenuItem()` | Initializes a new instance of the MenuItem class. | TK |

## Microsoft.Phone.Controls.MultiTouchGestureEventArgs

Source: Toolkit · class : GestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The base class for multi-touch gesture event args. Currently used only for two-finger (pinch) operations.

### Methods

| Member | Description | Src |
|---|---|---|
| `Point GetPosition(UIElement relativeTo, int index)` | Returns the position of either of the two touch points (0 or 1) relative to the UIElement provided. <br>`relativeTo`: The return value will be relative to this element. <br>`index`: The touchpoint to use (0 or 1). <br>Returns: The gesture's starting point relative to the given UIElement. | TK |

## Microsoft.Phone.Controls.MultiselectItem

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/MultiselectList/MultiselectItem.cs`

An item container for a Multiselect List.

### Properties

| Member | Description | Src |
|---|---|---|
| `object ContentInfo` | Gets or sets the content information. | TK |
| `DataTemplate ContentInfoTemplate` | Gets or sets the data template that defines the content information. | TK |
| `double HintPanelHeight` | Gets or sets the height of the hint panel. | TK |
| `bool IsSelected` | Gets or sets the flag that indicates if the item is currently selected. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ContentInfoProperty` | Identifies the ContentInfo dependency property. | TK |
| `static DependencyProperty ContentInfoTemplateProperty` | Identifies the ContentInfoTemplate dependency property. | TK |
| `static DependencyProperty HintPanelHeightProperty` | Identifies the HintPanelHeight dependency property. | TK |
| `static DependencyProperty IsSelectedProperty` | Identifies the IsSelected dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler Selected` | Occurs when the multiselect item is selected. | TK |
| `RoutedEventHandler Unselected` | Occurs when the multiselect item is unselected. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Gets the template parts and sets event handlers. | TK |
| `«ctor» MultiselectItem()` | Initializes a new instance of the MultiselectItem class. | TK |

## Microsoft.Phone.Controls.MultiselectList

Source: Toolkit · class : ItemsControl · `Microsoft.Phone.Controls.Toolkit/MultiselectList/MultiselectList.cs`

A collection of items that supports multiple selection.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool IsSelectionEnabled` | Gets or sets the flag that indicates if the list is in selection mode or not. | TK |
| `Style ItemContainerStyle` | Gets or sets the item container style. | TK |
| `DataTemplate ItemInfoTemplate` | Gets or sets the data template that is to be used on the item information field of the MultiselectItems. | TK |
| `IList SelectedItems` | Collection of items that are currently selected. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty IsInSelectionModeProperty` | Identifies the IsSelectionEnabled dependency property. | TK |
| `static DependencyProperty ItemContainerStyleProperty` | Identifies the ItemContainerStyle dependency property. | TK |
| `static DependencyProperty ItemInfoTemplateProperty` | Identifies the ItemInfoTemplate dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `DependencyPropertyChangedEventHandler IsSelectionEnabledChanged` | Occurs when the selection mode is opened or closed. | TK |
| `SelectionChangedEventHandler SelectionChanged` | Occurs when there is a change to the SelectedItems collection. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» MultiselectList()` | Initializes a new instance of the MultiselectList class. | TK |

## Microsoft.Phone.Controls.NavigatingEventArgs

Source: SDK

Provides data for the WebBrowser control `Navigating` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Uri` | The URI being navigated to. <br>Returns: Type: `Uri`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the NavigatingEventArgs class. | SDK |

## Microsoft.Phone.Controls.NotifyEventArgs

Source: SDK

Provides data for the WebBrowser control `ScriptNotify` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Value` | Gets argument data passed from JavaScript. <br>Returns: Type `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `NotifyEventArgs` class. | SDK |

## Microsoft.Phone.Controls.NullToVisibilityConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/TransferControl/NullToVisibilityConverter.cs`

Converts a null value to collapsed visibility

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Modifies the source data before passing it to the target for display in the UI. <br>`value`: The source data being passed to the target. <br>`targetType`: The `Type` of data expected by the target dependency property. <br>`parameter`: An optional parameter to be used in the converter logic. <br>`culture`: The culture of the conversion. <br>Returns: The value to be passed to the target dependency property. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Modifies the target data before passing it to the source object. This method is called only in `TwoWay` bindings. <br>`value`: The target data being passed to the source. <br>`targetType`: The `Type` of data expected by the source object. <br>`parameter`: An optional parameter to be used in the converter logic. <br>`culture`: The culture of the conversion. <br>Returns: The value to be passed to the source object. | TK |

## Microsoft.Phone.Controls.ObscuredEventArgs

Source: SDK

The `ObscuredEventArgs` class handles obscurity changes.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsLocked` | This property returns the phone lock state. <br>Returns: Returns `Boolean`. A value of true equals locked and a value of false is unlocked. If you have set the `PhoneApplicationService` property `ApplicationIdleDetectionMode` to Disabled, your application will run underneath the lock screen. If the IsLocked property is set to true, for better performance turn of all UI functionality as it cannot be viewed. The application should stop all operations that are not critical such as animations, media, or sensors. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Boolean)` | Creates a new instance of the `ObscuredEventArgs` class. <br>`isLocked`: This property returns the phone lock state. | SDK |

## Microsoft.Phone.Controls.OffOnConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/ToggleSwitch/OffOnConverter.cs`

Converts bool? values to "Off" and "On" strings.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a boolean to the equivalent On or Off string. <br>`value`: The given boolean. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: A converted value. If the method returns null, the valid null value is used. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.OrientationChangedEventArgs

Source: SDK

Provides data to the OrientationChanged event of the Page and Frame classes.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation` | A member of the `PageOrientation` enumeration indicating the device orientation associated with this event. <br>Returns: Type: `PageOrientation`. The page orientation associated with the event. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Microsoft.Phone.Controls.PageOrientation)` | Creates a new instance of the `OrientationChangedEventArgs` class. <br>`orientation`: A member of the `PageOrientation` enumeration indicating the device orientation associated with the event. | SDK |

## Microsoft.Phone.Controls.PageOrientation

Source: SDK

An enumeration defining the possible orientations of a page.

### Fields

| Member | Description | Src |
|---|---|---|
| `Landscape` | Landscape orientation. | SDK |
| `LandscapeLeft` | Landscape orientation with the top of the page rotated to the left. | SDK |
| `LandscapeRight` | Landscape orientation with the top of the page rotated to the right. | SDK |
| `None` | No orientation is specified. | SDK |
| `Portrait` | Portrait orientation. | SDK |
| `PortraitDown` | Portrait orientation. This orientation is never used. | SDK |
| `PortraitUp` | Portrait orientation. | SDK |

## Microsoft.Phone.Controls.Panorama

Source: SDK

Creates a panoramic view of items that can be panned side-to-side.

### Properties

| Member | Description | Src |
|---|---|---|
| `DefaultItem` | Gets or sets the default item for the Panorama. <br>Returns: Returns `Object`. | SDK |
| `HeaderTemplate` | Gets or sets the template for the Header property of PanoramaItem children. <br>Returns: Returns `DataTemplate`. | SDK |
| `SelectedIndex` | Gets the selected index. <br>Returns: Returns `Int32`. | SDK |
| `SelectedItem` | Gets the selected item. <br>Returns: Returns `Object`. | SDK |
| `Title` | Gets or sets the title for the Panorama. <br>Returns: Returns `Object`. | SDK |
| `TitleTemplate` | Gets or sets the template for the Title property. <br>Returns: Returns `DataTemplate`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `DefaultItemProperty` | Identifies the DefaultItem dependency property. | SDK |
| `HeaderTemplateProperty` | Identifies the HeaderTemplate dependency property. | SDK |
| `SelectedIndexProperty` | Identifies the SelectedIndex dependency property. | SDK |
| `SelectedItemProperty` | Identifies the SelectedItem dependency property. | SDK |
| `TitleProperty` | Identifies the Title dependency property. | SDK |
| `TitleTemplateProperty` | Identifies the TitleTemplate dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `SelectionChanged` | Event that is invoked when selection changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the Panorama class. | SDK |
| `ArrangeOverride(Size)` | Handles the arrange pass for the control. <br>`finalSize`: Final size. <br>Returns: Render size. | SDK |
| `MeasureOverride(Size)` | Handles the measurement of the control. <br>`availableSize`: Available size. <br>Returns: Desired size. | SDK |
| `OnApplyTemplate` | Handles the application of a new Template. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | When the items have changed, we need to adjust the selection. <br>`e`: The changed item. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Handles the preparation of a new container. <br>`element`: Container instance. <br>`item`: Container content. | SDK |

## Microsoft.Phone.Controls.PanoramaItem

Source: SDK

Represents an item in a Panorama control.

### Properties

| Member | Description | Src |
|---|---|---|
| `Header` | Gets or sets the header for the PanoramaItem. <br>Returns: Returns `Object`. | SDK |
| `HeaderTemplate` | Gets or sets the template for the Header property. <br>Returns: Returns `DataTemplate`. | SDK |
| `Orientation` | Gets or sets the primary (scrolling) orientation for the PanoramaItem. <br>Returns: Returns `Orientation`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `HeaderProperty` | Identifies the Header dependency property. | SDK |
| `HeaderTemplateProperty` | Identifies the HeaderTemplate dependency property. | SDK |
| `OrientationProperty` | Identifies the Orientation dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PanoramaItem class. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. <br>Returns: Returns `Size`. The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |

## Microsoft.Phone.Controls.PerformanceProgressBar

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/ProgressBar/PerformanceProgressBar.cs`

A progress bar implementation for a smoother appearance of the indeterminate states, with the added behavior that after the behavior is no longer needed, it smoothly fades out the dots for a less jarring experience. No exposed Value property.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool ActualIsIndeterminate` | Gets or sets the value indicating whether the actual indeterminate property should be reflecting a particular value. | TK |
| `bool IsIndeterminate` | Gets or sets a value indicating whether the control is in the indeterminate state. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ActualIsIndeterminateProperty` | Identifies the ActualIsIndeterminate dependency property. | TK |
| `static DependencyProperty IsIndeterminateProperty` | Identifies the IsIndeterminate dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Applies the template and extracts both a visual state and a template part. | TK |
| `«ctor» PerformanceProgressBar() : base()` | Initializes a new instance of the PerformanceProgressBar type. | TK |

## Microsoft.Phone.Controls.PhoneApplicationFrame

Source: SDK

The class for the PhoneApplicationFrame control for Windows Phone Silverlight projects. For more information on this control see Frame and Page Navigation Overview for Windows Phone.

### Properties

| Member | Description | Src |
|---|---|---|
| `BackStack` | Gets an IEnumerable that you use to enumerate the entries in back navigation history. <br>Returns: Returns `IEnumerable`1` List of entries in the back stack.. | SDK |
| `Orientation` | This property indicates the current orientation of the frame that is decided by the hosted page. <br>Returns: Returns `PageOrientation`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `OrientationProperty` | The dependency property for `Orientation`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `BackKeyPress` | This event is raised when the hardware Back button is pressed. | SDK |
| `JournalEntryRemoved` | This event is raised during a `RemoveBackEntry` operation or during a normal back navigation after the `Navigated` event has been raised. | SDK |
| `Obscured` | This event is raised when the shell chrome is covering the frame. | SDK |
| `OrientationChanged` | Raised when the Orientation property has changed. | SDK |
| `Unobscured` | This event is raised when the shell chrome is no longer covering the frame. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PhoneApplicationFrame class. | SDK |
| `ArrangeOverride(Size)` | When overridden in a derived class, positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `Finalize` | Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection. Finalize is protected and, therefore, is accessible only through this class or a derived class. | SDK |
| `MeasureOverride(Size)` | When overridden in a derived class, measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. Infinity can be specified as a value to indicate that the element will size to whatever content is available. <br>Returns: Returns `Size`.The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |
| `OnApplyTemplate` | This method applies a template to the PhoneApplicationFrame. | SDK |
| `RemoveBackEntry` | This method is used to remove the most recent entry from the back stack, or throws an InvalidOperationException if there are no more entries to remove. If you want to remove more than one item, you call this method multiple times. This API is synchronous and must be called from the UI thread. <br>Returns: Returns `JournalEntry`. The list of `JournalEntry` items. | SDK |

## Microsoft.Phone.Controls.PhoneApplicationPage

Source: SDK

The class for the PhoneApplicationPage control for Windows Phone Silverlight projects.

### Properties

| Member | Description | Src |
|---|---|---|
| `ApplicationBar` | Gets or sets the application bar UI. <br>Returns: Returns `ApplicationBar`. | SDK |
| `Orientation` | Gets the current orientation. <br>Returns: Returns `PageOrientation`. | SDK |
| `State` | This property allows you to save transient state data on your page. <br>Returns: Returns `IDictionary`2`.Represents a generic collection of key/value pairs. | SDK |
| `SupportedOrientations` | Gets or sets the supported phone orientations. <br>Returns: Returns `SupportedPageOrientation`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ApplicationBarProperty` | The dependency property for `ApplicationBar`. | SDK |
| `OrientationProperty` | The dependency property for `Orientation`. | SDK |
| `SupportedOrientationsProperty` | The dependency property for `SupportedOrientations`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `BackKeyPress` | This event is raised when the hardware Back button is pressed. | SDK |
| `BeginLayoutChanged` | This event is raised when the layout orientation should change. This event is for internal use only. | SDK |
| `OrientationChanged` | This event is raised when the Orientation property has changed. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the of the PhoneApplicationPage class. | SDK |
| `Finalize` | Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection. Finalize is protected and, therefore, is accessible only through this class or a derived class. | SDK |
| `OnBackKeyPress(ComponentModel.CancelEventArgs)` | This method is called when the hardware Back button is pressed. <br>`e`: Set e.Cancel to true to indicate that the request was handled by the application. | SDK |
| `OnOrientationChanged(Microsoft.Phone.Controls.OrientationChangedEventArgs)` | This method is called after the Orientation property has been changed. <br>`e`: The event arguments. | SDK |
| `OnRemovedFromJournal(Navigation.JournalEntryRemovedEventArgs)` | This method is called on a page after it has been removed from the journal through the `RemoveBackEntry` method. It is also called after the `NavigationEventArgs)` override in a normal back navigation. <br>`e`: The removed journal entry. | SDK |

## Microsoft.Phone.Controls.PhoneTextBox

Source: Toolkit · class : TextBox · `Microsoft.Phone.Controls.Toolkit/PhoneTextBox/PhoneTextBox.cs`

An extended TextBox for Windows Phone which implements hint text, an action icon, and a length indicator.

### Properties

| Member | Description | Src |
|---|---|---|
| `ImageSource ActionIcon` | Gets or sets the ActionIcon. | TK |
| `Visibility ActualHintVisibility` | Gets or sets whether the hint is actually visible. | TK |
| `int DisplayedMaxLength` | The displayed value for the maximum length of the input. | TK |
| `bool HidesActionItemWhenEmpty` | Gets or sets whether the ActionItem is hidden when there is not text entered in the PhoneTextBox. | TK |
| `string Hint` | Gets or sets the Hint | TK |
| `Style HintStyle` | Gets or sets the Hint style | TK |
| `int LengthIndicatorThreshold` | Threshold after which the length indicator will appear. | TK |
| `Boolean LengthIndicatorVisible` | Boolean that determines if the length indicator should be visible. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ActionIconProperty` | Identifies the ActionIcon DependencyProperty. | TK |
| `static DependencyProperty ActualHintVisibilityProperty` | Identifies the HintVisibility DependencyProperty | TK |
| `static DependencyProperty DisplayedMaxLengthProperty` | The displayed maximum length of text that can be entered. This value takes priority over the MaxLength property in the Length Indicator display. | TK |
| `static DependencyProperty HidesActionItemWhenEmptyProperty` | Identifies the HidesActionItemWhenEmpty DependencyProperty. | TK |
| `static DependencyProperty HintProperty` | Identifies the Hint DependencyProperty. | TK |
| `static DependencyProperty HintStyleProperty` | Identifies the HintStyle DependencyProperty. | TK |
| `static DependencyProperty LengthIndicatorThresholdProperty` | Length Indicator Visibility Threshold Dependency Property. | TK |
| `static DependencyProperty LengthIndicatorVisibleProperty` | Length Indicator Visibile Dependency Property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler ActionIconTapped` | Action Icon Tapped event. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Applies the template and checks to see if the Hint should be shown when the page is first loaded. | TK |
| `«ctor» PhoneTextBox()` | Initializes a new instance of the PhoneTextBox class. | TK |

## Microsoft.Phone.Controls.PinchGestureEventArgs

Source: Toolkit · class : MultiTouchGestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used by the PinchDelta and PinchCompleted events.

### Properties

| Member | Description | Src |
|---|---|---|
| `double DistanceRatio` | Returns the ratio of the current distance between touchpoints / the original distance between the touchpoints. | TK |
| `double TotalAngleDelta` | Returns the difference in angle between the current touch positions and the original touch positions. | TK |

## Microsoft.Phone.Controls.PinchStartedGestureEventArgs

Source: Toolkit · class : MultiTouchGestureEventArgs · `Microsoft.Phone.Controls.Toolkit/Input/GestureHelperEventArgs.cs`

The event args used by the PinchStarted event.

### Properties

| Member | Description | Src |
|---|---|---|
| `double Angle` | The angle defined by the two touch points. | TK |
| `double Distance` | The distance between the two touch points. | TK |

## Microsoft.Phone.Controls.Pivot

Source: SDK

The Pivot control provides a quick way to manage the navigation of views within an application. The control can be used as a navigation interface for filtering large sets or switching between views.

### Properties

| Member | Description | Src |
|---|---|---|
| `HeaderTemplate` | Gets or sets the template for the Header property of PivotItem children. <br>Returns: Returns `DataTemplate`. | SDK |
| `SelectedIndex` | Gets or sets the index of the selected (current) PivotItem. <br>Returns: Returns `Int32`. | SDK |
| `SelectedItem` | Gets or sets the selected (current) PivotItem. <br>Returns: Returns `Object`. | SDK |
| `Title` | Gets or sets the title to be optionally set above the headers. <br>Returns: Returns `Object`. | SDK |
| `TitleTemplate` | Gets or sets the title template used for displaying the title above the headers area. <br>Returns: Returns `DataTemplate`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `HeaderTemplateProperty` | Identifies the HeaderTemplate dependency property. | SDK |
| `SelectedIndexProperty` | Identifies the SelectedIndex dependency property. | SDK |
| `SelectedItemProperty` | Identifies the SelectedItem dependency property. | SDK |
| `TitleProperty` | Identifies the Title dependency property. | SDK |
| `TitleTemplateProperty` | Identifies the TitleTemplate dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `LoadedPivotItem` | Event for indicating that an item has fully loaded. | SDK |
| `LoadingPivotItem` | Event for offering an opportunity to dynamically load or change the content of a pivot item before it is displayed. | SDK |
| `SelectionChanged` | This event occurs when the currently selected item changes. | SDK |
| `UnloadedPivotItem` | Event for notifying that the pivot item has been completely unloaded from the visual pivot. | SDK |
| `UnloadingPivotItem` | Event for offering an opportunity to dynamically load, change or remove the content of a pivot item as it is removed. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the Pivot type. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. <br>Returns: Returns `Size`. The size that this element determines it needs during layout, based on its calculations of child element sizes | SDK |
| `OnApplyTemplate` | Overrides the on apply template method to connect template parts. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Update the header items when there are changes in the pivot items collection. <br>`e`: The event arguments. | SDK |
| `OnLoadedPivotItem(Microsoft.Phone.Controls.PivotItem)` | Override for notifying that an item has loaded. <br>`item`: The pivot item instance. | SDK |
| `OnLoadingPivotItem(Microsoft.Phone.Controls.PivotItem)` | Overrride for creating content for a given item. <br>`item`: The pivot item instance. | SDK |
| `OnSelectionChanged(SelectionChangedEventArgs)` | Responds to a pivot selection change by raising a SelectionChanged event. <br>`e`: Provides data for SelectionChangedEventArgs. | SDK |
| `OnUnloadedPivotItem(Microsoft.Phone.Controls.PivotItemEventArgs)` | The unloaded event handler. <br>`e`: Provides data for PivotItemEventArgs. | SDK |
| `OnUnloadingPivotItem(Microsoft.Phone.Controls.PivotItemEventArgs)` | The unloading event handler. <br>`e`: The pivot item event arguments. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the container to display the specified item. <br>`element`: The container element used to display the specified item. <br>`item`: The item to display. | SDK |
| `UpdateItemVisibility(UIElement,Boolean)` | Shows or hides an item. <br>`element`: The user interface element. <br>`toVisible`: A value indicating whether to make the item visible or not. | SDK |

## Microsoft.Phone.Controls.PivotItem

Source: SDK

PivotItem is the container for items in the Pivot control. The item has both a set of visual states for position and a Header, effectively a HeaderedContentControl.

### Properties

| Member | Description | Src |
|---|---|---|
| `Header` | Gets or sets the header for the PivotItem. <br>Returns: Returns `Object`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `HeaderProperty` | Identifies the Header dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PivotItem type. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. <br>Returns: Returns `Size`. The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |
| `OnApplyTemplate` | Overrides the on apply template method. | SDK |

## Microsoft.Phone.Controls.PivotItemEventArgs

Source: SDK

Event arguments for dynamically interacting with the PivotItem before use, allowing for delay load scenarios.

### Properties

| Member | Description | Src |
|---|---|---|
| `Item` | Gets the pivot item instance. <br>Returns: Returns `PivotItem`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PivotItemEventArgs type. | SDK |
| `Constructor(Microsoft.Phone.Controls.PivotItem)` | Initializes a new instance of the PivotItemEventArgs type. <br>`item`: The pivot item instance. | SDK |

## Microsoft.Phone.Controls.Rating

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/RatingControl/Rating.cs`

A rating control implementation that allows for a parameterized number of RatingItems, which can be arbitrarily styled using the FilledItemStyle and UnfilledItemStyle properties, or templated using an applied ControlTemplate.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool AllowHalfItemIncrement` | Gets or sets the value indicating whether this control will allow users to select half items when dragging or touching. | TK |
| `bool AllowSelectingZero` | Gets or sets the value indicating whether this control will allow users to drag the selected items to zero. | TK |
| `Style FilledItemStyle` | Gets or sets the style that will be applied to filled RatingItems. This style can only be applied once to a RatingItem. | TK |
| `Orientation Orientation` | Gets or sets the value indicating the current orientation of the control. | TK |
| `int RatingItemCount` | Gets or sets the number of RatingItems that will be displayed by the control. Changing this property will cause elements to be added or removed from the FilledItemsCollection and UnfilledItemsCollection. | TK |
| `bool ReadOnly` | Gets or sets the value indicating whether this control will process user input and update the Value property. | TK |
| `bool ShowSelectionHelper` | Gets or sets the value indicating whether this control will display the drag selection helper. | TK |
| `Style UnfilledItemStyle` | Gets or sets the style that will be applied to unfilled RatingItems. This style can only be applied once to a RatingItem. | TK |
| `double Value` | — | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty AllowHalfItemIncrementProperty` | Identifies the AllowHalfItemIncrement dependency property. | TK |
| `static DependencyProperty AllowSelectingZeroProperty` | Identifies the AllowNoItemsSelected dependency property. | TK |
| `static DependencyProperty FilledItemStyleProperty` | Identifies the FilledItemStyle dependency property. | TK |
| `static DependencyProperty OrientationProperty` | Identifies the Orientation dependency property. | TK |
| `static DependencyProperty RatingItemCountProperty` | Identifies the RatingItemCount dependency property. | TK |
| `static DependencyProperty ReadOnlyProperty` | Identifies the ReadOnly dependency property. | TK |
| `static DependencyProperty ShowSelectionHelperProperty` | Identifies the AllowNoItemsSelected dependency property. | TK |
| `static DependencyProperty UnfilledItemStyleProperty` | Identifies the UnfilledItemStyle dependency property. | TK |
| `static DependencyProperty ValueProperty` | Identifies the Value dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler ValueChanged` | Occures when the value of this control changes, either from user interaction or by directly setting the property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Applies the template and extracts both a visual state and template parts. | TK |
| `«ctor» Rating()` | Initializes a new instance of the Rating type. | TK |

## Microsoft.Phone.Controls.RatingItem

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/RatingControl/RatingItem.cs`

A Control-derived class used by the Rating control to uniquely identify the objects used by the collections that contain the items displayed by the control.

### Properties

| Member | Description | Src |
|---|---|---|
| `double StrokeThickness` | Gets or sets the value indicating the thickness of a stroke around the path object. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty StrokeThicknessProperty` | Identifies the StrokeThickness dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» RatingItem()` | Initializes a new instance of the RatingItem type. | TK |

## Microsoft.Phone.Controls.RecurringDaysPicker

Source: Toolkit · class : ListPicker · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/RecurringDaysPicker.cs`

Represents a control that allows the user to choose days of the week.

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» RecurringDaysPicker()` | Initializes a new instance of the RecurringDaysPicker control. | TK |

## Microsoft.Phone.Controls.RelativeTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/RelativeTimeConverter.cs`

Time converter to display elapsed time relatively to the present.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string the represents the elapsed time relatively to the present. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: The culture to use in the converter. When not specified, the converter uses the current culture as specified by the system locale. <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.SlideInEffect

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Effects/SlideInEffect.cs`

Provides attached properties to make FrameworkElements inside PivotItems responsive to SelectionChanged events by Pivots. The result is a 'slide in' effect added to the selected elements.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty LineIndexProperty` | Identifies the line index of the current element, which is proportional to its initial offset before sliding in. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static int GetLineIndex(DependencyObject obj)` | Gets the line index of the specified dependency object. <br>`obj`: The dependency object. <br>Returns: The line index. | TK |
| `static void SetLineIndex(DependencyObject obj, int value)` | Sets the line index of the specified dependency object. <br>`obj`: The dependency object. <br>`value`: The line index. | TK |

## Microsoft.Phone.Controls.SpeechRecognizedEventArgs

Source: Toolkit · class : EventArgs · `Microsoft.Phone.Controls.Toolkit.WP8/SpeechTextBox/SpeechRecognizedEventArgs.cs`

Event arguments for the SpeechTextBox SpeechRecognized event.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool Canceled` | Gets or sets the canceled property. | TK |
| `SpeechRecognitionResult Result` | Gets the speech recognition results for processing. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» SpeechRecognizedEventArgs(SpeechRecognitionResult result)` | Creates a new instance of the SpeechRecognizedEventArgs class. | TK |

## Microsoft.Phone.Controls.SpeechTextBox

Source: Toolkit · class : PhoneTextBox · `Microsoft.Phone.Controls.Toolkit.WP8/SpeechTextBox/SpeechTextBox.cs`

An extended PhoneTextBox that behaves as a speech-enabled textbox, with an optional event handler to override default behavior.

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<SpeechRecognizedEventArgs> SpeechRecognized` | Speech Recognized event to optionally override default speech recognition processing. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Applies the template and adds a ManipulationStarted handler to the ActionIconBorder, if it exists. | TK |
| `«ctor» SpeechTextBox()` | Creates a new instance of the SpeechTextBox class. | TK |

## Microsoft.Phone.Controls.SupportedPageOrientation

Source: SDK

An enumeration defining the orientations that may be supported by an application.

### Fields

| Member | Description | Src |
|---|---|---|
| `Landscape` | Landscape orientation. Landscape supports both left and right views but there is no way programmatically to specify one or the other. | SDK |
| `Portrait` | Portrait orientation. | SDK |
| `PortraitOrLandscape` | Landscape or portrait orientation. | SDK |

## Microsoft.Phone.Controls.ThreadDateTimeConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/DateTimeConverters/ThreadDateTimeConverter.cs`

Date and time converter for threads.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts a `DateTime` object into a string appropriately formatted for threads. This format can be found in messaging. <br>`value`: The given date and time. <br>`targetType`: The type corresponding to the binding property, which must be of `String`. <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: The given date and time as a string. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not implemented. <br>`value`: (Not used). <br>`targetType`: (Not used). <br>`parameter`: (Not used). <br>`culture`: (Not used). <br>Returns: null | TK |

## Microsoft.Phone.Controls.TileSize

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/HubTile/HubTile.cs`

Represents the size of a Hub tile.

### Values

| Member | Description | Src |
|---|---|---|
| `Default` | Default size (173 px x 173 px). | TK |
| `Small` | Small size (99 px x 99 px). | TK |
| `Medium` | Medium size (210 px x 210 px). | TK |
| `Large` | Large size (432 px x 210 px). | TK |

## Microsoft.Phone.Controls.TileSizeToHeightConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/HubTile/HubTileConverters.cs`

Returns the Hub tile height corresponding to a tile size.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts from a tile size to the corresponding height. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not used. | TK |

## Microsoft.Phone.Controls.TileSizeToWidthConverter

Source: Toolkit · class : IValueConverter · `Microsoft.Phone.Controls.Toolkit/HubTile/HubTileConverters.cs`

Returns the Hub tile width corresponding to a tile size.

### Methods

| Member | Description | Src |
|---|---|---|
| `object Convert(object value, Type targetType, object parameter, CultureInfo culture)` | Converts from a tile size to the corresponding width. | TK |
| `object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)` | Not used. | TK |

## Microsoft.Phone.Controls.TiltEffect

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Effects/TiltEffect.cs`

This code provides attached properties for adding a 'tilt' effect to all controls within a container.

### Properties

| Member | Description | Src |
|---|---|---|
| `static List<Type> TiltableItems` | Default list of items that are tiltable. | TK |
| `static bool UseLogarithmicEase` | Whether to use a slightly more accurate (but slightly slower) tilt animation easing function. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty IsTiltEnabledProperty` | Whether the tilt effect is enabled on a container (and all its children). | TK |
| `static DependencyProperty SuppressTiltProperty` | Suppresses the tilt effect on a single control that would otherwise be tilted. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static bool GetIsTiltEnabled(DependencyObject source)` | Gets the IsTiltEnabled dependency property from an object. <br>`source`: The object to get the property from. <br>Returns: The property's value. | TK |
| `static bool GetSuppressTilt(DependencyObject source)` | Gets the SuppressTilt dependency property from an object. <br>`source`: The object to get the property from. <br>Returns: The property's value. | TK |
| `static void SetIsTiltEnabled(DependencyObject source, bool value)` | Sets the IsTiltEnabled dependency property on an object. <br>`source`: The object to set the property on. <br>`value`: The value to set. | TK |
| `static void SetSuppressTilt(DependencyObject source, bool value)` | Sets the SuppressTilt dependency property from an object. <br>`source`: The object to get the property from. <br>`value`: The property's value. | TK |

## Microsoft.Phone.Controls.TimePicker

Source: Toolkit · class : DateTimePickerBase · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/TimePicker.cs`

Represents a control that allows the user to choose a time (hour/minute/am/pm).

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» TimePicker()` | Initializes a new instance of the TimePicker control. | TK |

## Microsoft.Phone.Controls.TimePickerPage

Source: Toolkit · class : DateTimePickerPageBase · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/TimePickerPage.xaml.cs`

Represents a page used by the TimePicker control that allows the user to choose a time (hour/minute/am/pm).

### Methods

| Member | Description | Src |
|---|---|---|
| `void SetFlowDirection(FlowDirection flowDirection)` | Sets the selectors and title flow direction. <br>`flowDirection`: Flow direction to set. | TK |
| `«ctor» TimePickerPage()` | Initializes a new instance of the TimePickerPage control. | TK |

## Microsoft.Phone.Controls.ToggleSwitch

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/ToggleSwitch/ToggleSwitch.cs`

Represents a switch that can be toggled between two states.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Header` | Gets or sets the header. | TK |
| `DataTemplate HeaderTemplate` | Gets or sets the template used to display the control's header. | TK |
| `bool? IsChecked` | Gets or sets whether the ToggleSwitch is checked. | TK |
| `Brush SwitchForeground` | Gets or sets the switch foreground. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty HeaderProperty` | Identifies the Header DependencyProperty. | TK |
| `static DependencyProperty HeaderTemplateProperty` | Identifies the HeaderTemplate DependencyProperty. | TK |
| `static DependencyProperty IsCheckedProperty` | Identifies the IsChecked DependencyProperty. | TK |
| `static DependencyProperty SwitchForegroundProperty` | Identifies the SwitchForeground DependencyProperty. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<RoutedEventArgs> Checked` | Occurs when the `ToggleSwitch` is checked. | TK |
| `EventHandler<RoutedEventArgs> Click` | Occurs when the `ToggleButton` is clicked. | TK |
| `EventHandler<RoutedEventArgs> Indeterminate` | Occurs when the `ToggleSwitch` is indeterminate. | TK |
| `EventHandler<RoutedEventArgs> Unchecked` | Occurs when the `ToggleSwitch` is unchecked. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `string ToString()` | Returns a `String` that represents the current `Object` . | TK |
| `void OnApplyTemplate()` | Gets all the template parts and initializes the corresponding state. | TK |
| `«ctor» ToggleSwitch()` | Initializes a new instance of the ToggleSwitch class. | TK |

## Microsoft.Phone.Controls.TransferControl

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/TransferControl/TransferControl.cs`

This control displays the progress and status of a download or upload

### Properties

| Member | Description | Src |
|---|---|---|
| `bool AutoHide` | If set to true, the control will collapse when the transfer completes or is cancelled. False by default. | TK |
| `object Header` | This controls what appears above the progress bar. | TK |
| `DataTemplate HeaderTemplate` | This provides the template for the header | TK |
| `Uri Icon` | This icon can be set to any type of control and appears to the left of the progress bar | TK |
| `bool IsContextMenuEnabled` | Defines if the context menu is enabled | TK |
| `TransferMonitor Monitor` | This binds to a TransferMonitor which wraps around a background transfer | TK |
| `Style ProgressBarStyle` | The style used for the progress bar | TK |
| `Brush StatusTextBrush` | The foreground brush used to the color the status text | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Applies the context menu actions | TK |
| `«ctor» TransferControl()` | The default constructor assigns the object a DataTemplate style and adds event listeners. | TK |

## Microsoft.Phone.Controls.TransferMonitor

Source: Toolkit · class : INotifyPropertyChanged · `Microsoft.Phone.Controls.Toolkit/TransferControl/TransferMonitor.cs`

This class is a wrapper around BackgroundTransferRequest.

### Properties

| Member | Description | Src |
|---|---|---|
| `long BytesTransferred` | The number of bytes sent or received | TK |
| `string ErrorMessage` | Provides an error message when the transfer fails | TK |
| `bool IsProgressIndeterminate` | True if the total size of the transfer is not known | TK |
| `string Name` | The name of the transfer. If not specified in the constructor, it attempts to parse the filename from the request. | TK |
| `double PercentComplete` | How much of the transfer has completed. | TK |
| `TransferRequestState State` | Returns the current state of the transfer | TK |
| `string StatusText` | This is the text that is displayed along with the progress bar | TK |
| `long TotalBytesToTransfer` | The number of bytes that will be sent or received | TK |
| `TransferType TransferType` | Identifies which type of transfer this is | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<BackgroundTransferEventArgs> Complete` | Event occurs when the file has finished transferring successfully | TK |
| `EventHandler<BackgroundTransferEventArgs> Failed` | Event occurs when the transfer has failed to transfer | TK |
| `EventHandler<BackgroundTransferEventArgs> ProgressChanged` | Event occurs when the transfer progress updates | TK |
| `PropertyChangedEventHandler PropertyChanged` | Occurs when a property of the helper changes | TK |
| `EventHandler<BackgroundTransferEventArgs> Started` | Event ocurs when the transfer has been added to the queue This is not necessarily when the file begans downloading and uploading | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void RequestCancel()` | Requests cancellation of the transfer | TK |
| `void RequestStart()` | Requests the transfer be added to the BackgroundTransferService queue The transfer may not begin immediately, but this attempts add the request to the queue | TK |
| `«ctor» TransferMonitor(BackgroundTransferRequest request) : this(request, "")` | This helper wraps around an instance of BackgroundTransferRequest <br>`request`: The background request to display | TK |
| `«ctor» TransferMonitor(BackgroundTransferRequest request, string name)` | This helper wraps around an instance of BackgroundTransferRequest <br>`request`: The background request to display <br>`name`: The name to display | TK |

## Microsoft.Phone.Controls.TransferRequestState

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/TransferControl/TransferRequestState.cs`

Defines all possible states for the transfer request

### Values

| Member | Description | Src |
|---|---|---|
| `Pending` | The request is waiting to begin | TK |
| `Downloading` | The request is downloading a file | TK |
| `Uploading` | The request is uploading a file | TK |
| `Paused` | The request is paused | TK |
| `Waiting` | The request is waiting for the system | TK |
| `Complete` | The request is complete | TK |
| `Failed` | The request has failed | TK |
| `Unknown` | The request is in an unknown state | TK |

## Microsoft.Phone.Controls.TransferType

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/TransferControl/TransferType.cs`

Defines the type of transfer

### Values

| Member | Description | Src |
|---|---|---|
| `Upload` | File uploaded to a server | TK |
| `Download` | File download | TK |

## Microsoft.Phone.Controls.TurnstileFeatherEffect

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Effects/TurnstileFeatherEffect.cs`

Provides attached properties to feather FrameworkElements in and out during page transitions. The result is a 'turnstile feather' effect added to the select elements.

### Properties

| Member | Description | Src |
|---|---|---|
| `static IList<Type> NonPermittedTypes` | Default list of types that cannot be feathered. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty FeatheringIndexProperty` | Identifies the feathering index of the current element, which represents its place in the feathering order sequence. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static int GetFeatheringIndex(DependencyObject obj)` | Gets the feathering index of the specified dependency object. <br>`obj`: The dependency object. <br>Returns: The feathering index. | TK |
| `static void SetFeatheringIndex(DependencyObject obj, int value)` | Sets the feathering index of the specified dependency object. <br>`obj`: The dependency object. <br>`value`: The feathering index. | TK |

## Microsoft.Phone.Controls.WeakReferenceHelper

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Common/WeakReferenceHelper.cs`

A helper class that provides useful utilities to work with weak references.

### Methods

| Member | Description | Src |
|---|---|---|
| `static bool ContainsTarget(IEnumerable<WeakReference> references, object target)` | Determine if there is a reference to a known target in an object that implements IEnumerable. <br>`references`: The object that implements IEnumerable. <br>`target`: The known target. <br>Returns: True if a reference to the known target exists in the list. False otherwise. | TK |
| `static bool TryRemoveTarget(IList<WeakReference> references, object target)` | Remove a reference to a known target from an object that implement IList. <br>`references`: The list to be examined. <br>`target`: The known target. <br>Returns: Try if the target was successsfully removed. | TK |
| `static void RemoveNullTargetReferences(IList<WeakReference> references)` | Removes all the weak references with null targets from an object that implements IList. <br>`references`: The object that implements IList. | TK |

## Microsoft.Phone.Controls.WebBrowser

Source: SDK

Allows HTML rendering and navigation functionality to be embedded in an application.

### Properties

| Member | Description | Src |
|---|---|---|
| `Base` | Sets the base directory within isolated storage that is used to resolve relative references within the WebBrowser control. <br>Returns: Returns `String`. | SDK |
| `IsGeolocationEnabled` | Determines if a website that is hosted in the WebBrowser control can use location services on the device. <br>Returns: Returns `Boolean`. | SDK |
| `IsScriptEnabled` | Enables or disables scripting. Applies to the next document that is navigated to, not the current document. This property is false by default. Set to true to enable scripting, or false to disable scripting. <br>Returns: Returns `Boolean`. | SDK |
| `Source` | Gets or sets the URI location of the WebBrowser control. <br>Returns: Returns `Uri`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BaseProperty` | Identifies the `Base` dependency property. | SDK |
| `IsGeolocationEnabledProperty` | Identifies the `IsGeolocationEnabled` dependency property. | SDK |
| `IsScriptEnabledProperty` | Identifies the `IsScriptEnabled` dependency property. | SDK |
| `SourceProperty` | Identifies the `Source` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `LoadCompleted` | Event that fires after the WebBrowser control has loaded content. | SDK |
| `Navigated` | Event that fires after the WebBrowser control successfully navigates. | SDK |
| `Navigating` | Event that fires when the browser control is navigating, including from a redirect. | SDK |
| `NavigationFailed` | Event that fires after the WebBrowser control fails to navigate. | SDK |
| `ScriptNotify` | Event that fires when Javascript calls window.external.notify(<data>) | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the WebBrowser class. | SDK |
| `InvokeScript(String)` | Executes a scripting function defined in the currently loaded document. <br>`scriptName`: The scripting function to execute. <br>Returns: Returns `String`.Value returned by the scripting function. | SDK |
| `InvokeScript(String,String[])` | Executes a scripting function defined in the currently loaded document, and passes the function an array of string parameters. <br>`scriptName`: The scripting function to execute. <br>`args`: A variable number of strings to pass to the function as parameters. <br>Returns: Returns `String`.Value returned by the scripting function. | SDK |
| `Navigate(Uri)` | Initiates a navigate request to the provided URI. <br>`uri`: The URI to navigate to. | SDK |
| `Navigate(Uri,Byte[],String)` | Initiates a navigate request to the provided URI. Allows customizable options for including posted form data and HTTP headers. <br>`uri`: The URI to navigate to. <br>`postData`: The posted form data. <br>`additionalHeaders`: The additional HTTP headers. | SDK |
| `NavigateToString(String)` | Injects an HTML string into the web browser control for rendering. <br>`html`: The HTML to display in the browser | SDK |
| `OnApplyTemplate` | Do not use this method. It is unsupported. | SDK |
| `SaveToString` | Returns a string containing the HTML content of the web page. <br>Returns: Returns `String`. | SDK |

## Microsoft.Phone.Controls.WebBrowserExtensions

Source: SDK

Contains extension methods for the `WebBrowser` control.

### Methods

| Member | Description | Src |
|---|---|---|
| `GetCookies(Microsoft.Phone.Controls.WebBrowser)` | Returns a CookieCollection of Cookie objects retrieved from the `Source` URI of a `WebBrowser` control. <br>`browserControl`: The `WebBrowser` class instance whose `Source` property is used to retrieve the CookieCollection object. <br>Returns: Returns a collection of Cookie objects. | SDK |

## Microsoft.Phone.Controls.WebBrowserNavigationException

Source: SDK

Included with the NavigationFailedEventArgs object, when the NavigationFailed event is raised.

### Properties

| Member | Description | Src |
|---|---|---|
| `StatusCode` | The status code of the navigation failure. <br>Returns: Returns `HttpStatusCode`. | SDK |
