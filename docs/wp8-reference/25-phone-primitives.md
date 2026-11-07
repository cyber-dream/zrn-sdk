# Phone control primitives

## Contents

- [`Microsoft.Phone.Controls.Primitives.AnimationDirection`](#microsoftphonecontrolsprimitivesanimationdirection)
- [`Microsoft.Phone.Controls.Primitives.ClipToBounds`](#microsoftphonecontrolsprimitivescliptobounds)
- [`Microsoft.Phone.Controls.Primitives.DateTimePickerPageBase`](#microsoftphonecontrolsprimitivesdatetimepickerpagebase)
- [`Microsoft.Phone.Controls.Primitives.DateTimeWrapper`](#microsoftphonecontrolsprimitivesdatetimewrapper)
- [`Microsoft.Phone.Controls.Primitives.IDateTimePickerPage`](#microsoftphonecontrolsprimitivesidatetimepickerpage)
- [`Microsoft.Phone.Controls.Primitives.ILoopingSelectorDataSource`](#microsoftphonecontrolsprimitivesiloopingselectordatasource)
- [`Microsoft.Phone.Controls.Primitives.LoopingSelector`](#microsoftphonecontrolsprimitivesloopingselector)
- [`Microsoft.Phone.Controls.Primitives.LoopingSelectorItem`](#microsoftphonecontrolsprimitivesloopingselectoritem)
- [`Microsoft.Phone.Controls.Primitives.PanningBackgroundLayer`](#microsoftphonecontrolsprimitivespanningbackgroundlayer)
- [`Microsoft.Phone.Controls.Primitives.PanningLayer`](#microsoftphonecontrolsprimitivespanninglayer)
- [`Microsoft.Phone.Controls.Primitives.PanningTitleLayer`](#microsoftphonecontrolsprimitivespanningtitlelayer)
- [`Microsoft.Phone.Controls.Primitives.PanoramaPanel`](#microsoftphonecontrolsprimitivespanoramapanel)
- [`Microsoft.Phone.Controls.Primitives.PivotHeaderItem`](#microsoftphonecontrolsprimitivespivotheaderitem)
- [`Microsoft.Phone.Controls.Primitives.PivotHeadersControl`](#microsoftphonecontrolsprimitivespivotheaderscontrol)
- [`Microsoft.Phone.Controls.Primitives.RelativeAnimatingContentControl`](#microsoftphonecontrolsprimitivesrelativeanimatingcontentcontrol)
- [`Microsoft.Phone.Controls.Primitives.TemplatedItemsControl`1`](#microsoftphonecontrolsprimitivestemplateditemscontrol1)
- [`Microsoft.Phone.Controls.Primitives.TemplatedListBox`](#microsoftphonecontrolsprimitivestemplatedlistbox)
- [`Microsoft.Phone.Controls.Primitives.ToggleSwitchButton`](#microsoftphonecontrolsprimitivestoggleswitchbutton)

## Microsoft.Phone.Controls.Primitives.AnimationDirection

Source: SDK

Describes the desired direction of animation.

### Fields

| Member | Description | Src |
|---|---|---|
| `Center` | Animates to the center direction. | SDK |
| `Left` | Animates to the left direction. | SDK |
| `Right` | Animates to the right direction. | SDK |

## Microsoft.Phone.Controls.Primitives.ClipToBounds

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Primitives/ClipToBounds.cs`

Provides an attachable property that clips a FrameworkElement to its bounds, e.g. clip the element when it is translated outside of the panel it is placed in.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty IsEnabledProperty` | Identifies the IsEnabled dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static bool GetIsEnabled(DependencyObject obj)` | Gets the IsEnabled dependency property from an object. <br>`obj`: The object to get the value from. <br>Returns: The property's value. | TK |
| `static void SetIsEnabled(DependencyObject obj, bool value)` | Sets the IsEnabled dependency property on an object. <br>`obj`: The object to set the value on. <br>`value`: The value to set. | TK |

## Microsoft.Phone.Controls.Primitives.DateTimePickerPageBase

Source: Toolkit · class : PhoneApplicationPage, IDateTimePickerPage · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DateTimePickerPageBase.cs`

Represents a base class for pages that work with DateTimePickerBase to allow users to choose a date/time.

### Properties

| Member | Description | Src |
|---|---|---|
| `DateTime? Value` | Gets or sets the DateTime to show in the picker page and to set when the user makes a selection. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void SetFlowDirection(FlowDirection flowDirection)` | Sets the selectors and title flow direction. <br>`flowDirection`: Flow direction to set. | TK |

## Microsoft.Phone.Controls.Primitives.DateTimeWrapper

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/DateTimeWrapper.cs`

Implements a wrapper for DateTime that provides formatted strings for DatePicker.

### Properties

| Member | Description | Src |
|---|---|---|
| `string AmPmString` | Gets the AM/PM designator as a string. | TK |
| `DateTime DateTime` | Gets the DateTime being wrapped. | TK |
| `string DayName` | Gets the day name as a string. | TK |
| `string DayNumber` | Gets the 2-digit day as a string. | TK |
| `string HourNumber` | Gets the hour as a string. | TK |
| `string MinuteNumber` | Gets the 2-digit minute as a string. | TK |
| `string MonthName` | Gets the month name as a string. | TK |
| `string MonthNumber` | Gets the 2-digit month as a string. | TK |
| `string YearNumber` | Gets the 4-digit year as a string. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static bool CurrentCultureUsesTwentyFourHourClock()` | Returns a value indicating whether the current culture uses a 24-hour clock. <br>Returns: True if it uses a 24-hour clock; false otherwise. | TK |
| `«ctor» DateTimeWrapper(DateTime dateTime)` | Initializes a new instance of the DateTimeWrapper class. <br>`dateTime`: DateTime to wrap. | TK |

## Microsoft.Phone.Controls.Primitives.IDateTimePickerPage

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/DateTimePickers/IDateTimePickerPage.cs`

Represents an interface for DatePicker/TimePicker to use for communicating with a picker page.

## Microsoft.Phone.Controls.Primitives.ILoopingSelectorDataSource

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Primitives/LoopingSelectorDataSource.cs`

Defines how the LoopingSelector communicates with data source.

## Microsoft.Phone.Controls.Primitives.LoopingSelector

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/Primitives/LoopingSelector.cs`

An infinitely scrolling, UI- and data-virtualizing selection control.

### Properties

| Member | Description | Src |
|---|---|---|
| `ILoopingSelectorDataSource DataSource` | The data source that the this control is the view for. | TK |
| `bool IsExpanded` | The IsExpanded property controls the expanded state of this control. | TK |
| `Thickness ItemMargin` | The margin around the items, to be a part of the touchable area. | TK |
| `Size ItemSize` | The size of the items, excluding the ItemMargin. | TK |
| `DataTemplate ItemTemplate` | The ItemTemplate property | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty DataSourceProperty` | The DataSource DependencyProperty | TK |
| `static DependencyProperty IsExpandedProperty` | The IsExpanded DependencyProperty. | TK |
| `static DependencyProperty ItemTemplateProperty` | The ItemTemplate DependencyProperty | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `DependencyPropertyChangedEventHandler IsExpandedChanged` | The IsExpandedChanged event will be raised whenever the IsExpanded state changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | When overridden in a derived class, is invoked whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate()`. In simplest terms, this means the method is called just before a UI element displays in an application. For more information, see Remarks. | TK |
| `«ctor» LoopingSelector()` | Creates a new LoopingSelector. | TK |

## Microsoft.Phone.Controls.Primitives.LoopingSelectorItem

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/Primitives/LoopingSelectorItem.cs`

The items that will be contained in the LoopingSelector.

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<EventArgs> Click` | The Click event. This is needed because there is no gesture for touch-down, pause longer than the Hold time, and touch-up. Tap will not be raise, and Hold is not adequate. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Override of OnApplyTemplate | TK |
| `«ctor» LoopingSelectorItem()` | Create a new LoopingSelectorItem. | TK |

## Microsoft.Phone.Controls.Primitives.PanningBackgroundLayer

Source: SDK

This override provides specific behaviors for the background.

### Properties

| Member | Description | Src |
|---|---|---|
| `PanRate` | Gets the pan rate. <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PanningBackgroundLayer. | SDK |
| `UpdateWrappingRectangles` | Ensures the wrapping rectangles have been created, and updates them as needed | SDK |

## Microsoft.Phone.Controls.Primitives.PanningLayer

Source: SDK

Implements a surface the Panorama control uses to pan content horizontally.

### Properties

| Member | Description | Src |
|---|---|---|
| `ContentPresenter` | ContentPresenter property <br>Returns: Returns `ContentPresenter`. | SDK |
| `LeftWraparound` | This will be used to display the right-most item on the left side for wraparound purposes. <br>Returns: Returns `Rectangle`. | SDK |
| `LocalTransform` | The localTransform adjusts the zero point if it changes due to the wraparounds. <br>Returns: Returns `TranslateTransform`. | SDK |
| `PanRate` | PanRate controls how much this pans relative to the content <br>Returns: Returns `Double`. | SDK |
| `PanningTransform` | The panningTransform is responsible for the panning. <br>Returns: Returns `TranslateTransform`. | SDK |
| `RightWraparound` | This will be used to display the left-most item on the right side for wraparound purposes. <br>Returns: Returns `Rectangle`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `Immediately` | Represents a zero duration. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PanningLayer class. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `GoTo(Int32,Duration,Action)` | Goes to an offset. <br>`targetOffset`: Target offset (in ItemsWidth coordinates). <br>`duration`: Duration. <br>`completionAction`: Action to perform at completion. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. <br>Returns: Returns `Size`. The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |
| `OnApplyTemplate` | OnApplyTemplate override | SDK |
| `UpdateWrappingRectangles` | Ensures the wrapping rectangles have been created, and updates them as needed | SDK |
| `Wraparound(Int32)` | Wraps the content around in preparation for post-wrap animation. <br>`direction`: Direction. | SDK |

## Microsoft.Phone.Controls.Primitives.PanningTitleLayer

Source: SDK

This override provides specific behaviors for the title text.

### Properties

| Member | Description | Src |
|---|---|---|
| `PanRate` | Get the rate at which this will pan compared to the items <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PanningTitleLayer type. | SDK |
| `UpdateWrappingRectangles` | Ensures the wrapping rectangles have been created, and updates them as needed | SDK |
| `Wraparound(Int32)` | This is called when the display is looping around. <br>`direction`: The direction of the movement. | SDK |

## Microsoft.Phone.Controls.Primitives.PanoramaPanel

Source: SDK

Implements a custom Panel for the Panorama control.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PanoramaPanel class. | SDK |
| `ArrangeOverride(Size)` | Handles the arrange pass for the control. <br>`finalSize`: Final size. <br>Returns: Render size. | SDK |
| `MeasureOverride(Size)` | Handles the measure pass for the control. <br>`availableSize`: Available size. <br>Returns: Desired size. | SDK |

## Microsoft.Phone.Controls.Primitives.PivotHeaderItem

Source: SDK

Represents a header item in the specialized pivot header items control.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsSelected` | Gets or sets a value indicating whether the item is selected. <br>Returns: Returns `Boolean`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsSelectedProperty` | Identifies the IsSelected dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PivotHeaderItem control. | SDK |
| `OnApplyTemplate` | Overrides the on apply template method. | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Handles the button up event. <br>`e`: The event arguments. | SDK |

## Microsoft.Phone.Controls.Primitives.PivotHeadersControl

Source: SDK

A primitive used by the Pivot control to manage the wrapping header experience for the control.

### Properties

| Member | Description | Src |
|---|---|---|
| `VisualFirstIndex` | The current VisualFirstIndex value <br>Returns: Returns `Int32`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `VisualFirstIndexProperty` | Identifies the VisualFirstIndex dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the PivotHeadersControl class. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for a FrameworkElement derived class. <br>`finalSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: Returns `Size`. The actual size used. | SDK |
| `ClearContainerForItemOverride(DependencyObject,Object)` | Undoes the effects of the PrepareContainerForItemOverride method. <br>`element`: The container element. <br>`item`: The item. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the FrameworkElement-derived class. <br>`availableSize`: The available size that this element can give to child elements. <br>Returns: Returns `Size`. The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |
| `OnApplyTemplate` | Overrides the on apply template method to connect template parts. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Handles the items changed event, re-creating the visuals for the control. <br>`e`: The event arguments. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the container to display the specified item. <br>`element`: The container element used to display the specified item. <br>`item`: The item to display. | SDK |

## Microsoft.Phone.Controls.Primitives.RelativeAnimatingContentControl

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit/ProgressBar/RelativeAnimatingContentControl.cs`

A very specialized primitive control that works around a specific visual state manager issue. The platform does not support relative sized translation values, and this special control walks through visual state animation storyboards looking for magic numbers to use as percentages. This control is not supported, unofficial, and is a hack in many ways. It is used to enable a Windows Phone native platform-style progress bar experience in indeterminate mode that remains performant.

### Properties

| Member | Description | Src |
|---|---|---|
| `DoubleAnimationDimension Dimension` | Gets the dimension of interest for the control. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `double StripMagicNumberOff(double number)` | Approximately removes the magic number state from a value. <br>`number`: The initial number. <br>Returns: Returns a double with an adjustment for the magic portion of the number. | TK |
| `static DoubleAnimationDimension? GetDimensionFromMagicNumber(double number)` | Retrieves the dimension, if any, from the number. If the number is not magic, null is returned instead. <br>`number`: The double value. <br>Returns: Returs a double animation dimension, if the number was partially magic; otherwise, returns null. | TK |
| `void UpdateWithNewDimension(double width, double height)` | Updates the original instance based on new dimension information from the control. Takes both and allows the subclass to make the decision on which ratio, values, and dimension to use. <br>`width`: The width of the control. <br>`height`: The height of the control. | TK |
| `«ctor» AnimationValueAdapter(DoubleAnimationDimension dimension)` | Initializes a new instance of the AnimationValueAdapter type. <br>`dimension`: The dimension of interest for updates. | TK |
| `«ctor» DoubleAnimationFrameAdapter(DoubleAnimationDimension dimension, DoubleKeyFrame frame) : base(dimension, frame)` | Initializes a new instance of the DoubleAnimationFrameAdapter type. <br>`dimension`: The dimension of interest. <br>`frame`: The instance of the animation type. | TK |
| `«ctor» DoubleAnimationFromAdapter(DoubleAnimationDimension dimension, DoubleAnimation instance) : base(dimension, instance)` | Initializes a new instance of the DoubleAnimationFromAdapter type. <br>`dimension`: The dimension of interest. <br>`instance`: The instance of the animation type. | TK |
| `«ctor» DoubleAnimationToAdapter(DoubleAnimationDimension dimension, DoubleAnimation instance) : base(dimension, instance)` | Initializes a new instance of the DoubleAnimationToAdapter type. <br>`dimension`: The dimension of interest. <br>`instance`: The instance of the animation type. | TK |
| `«ctor» GeneralAnimationValueAdapter(DoubleAnimationDimension d, T instance) : base(d)` | Initializes a new instance of the GeneralAnimationValueAdapter type. <br>`d`: The dimension of interest. <br>`instance`: The animation type instance. | TK |
| `«ctor» RelativeAnimatingContentControl()` | Initializes a new instance of the RelativeAnimatingContentControl type. | TK |

## Microsoft.Phone.Controls.Primitives.TemplatedItemsControl`1

Source: SDK

Subclasses ItemsControl to provide the typical support for using a custom container type.

### Properties

| Member | Description | Src |
|---|---|---|
| `ItemContainerStyle` | Gets or sets the ItemContainerStyle. <br>Returns: Returns `Style`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ItemContainerStyleProperty` | Identifies the ItemContainerStyle dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the TemplatedItemsControl class. | SDK |
| `ApplyItemContainerStyle(DependencyObject)` | Apply the ItemContainerStyle <br>`container`: The container | SDK |
| `ClearContainerForItemOverride(DependencyObject,Object)` | When overridden in a derived class, undoes the effects of the PrepareContainerForItemOverride method. <br>`element`: The container element. <br>`item`: The item. | SDK |
| `GetContainer(Object)` | Gets the container associated with the specified item. <br>`item`: Specified item. <br>Returns: Associated container. | SDK |
| `GetContainerForItemOverride` | Creates or identifies the element that is used to display the given item. <br>Returns: Returns the element for the container. | SDK |
| `GetItem(`0)` | Gets the item associated with the specified container. <br>`container`: Specified container. <br>Returns: Associated item. | SDK |
| `IsItemItsOwnContainerOverride(Object)` | Determines if the specified item is (or is eligible to be) its own container. <br>`item`: The item instance. <br>Returns: Returns a value indicating whether the item is its own container. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the container to display the specified item. <br>`element`: The container element used to display the specified item. <br>`item`: The item to display. | SDK |

## Microsoft.Phone.Controls.Primitives.TemplatedListBox

Source: Toolkit · class : ListBox · `Microsoft.Phone.Controls.Toolkit/LongListSelector/TemplatedListBox.cs`

Represents a ListBox with item-specific templates.

### Properties

| Member | Description | Src |
|---|---|---|
| `DataTemplate GroupFooterTemplate` | Gets or sets the footer template. | TK |
| `DataTemplate GroupHeaderTemplate` | Gets or sets the group header template. | TK |
| `DataTemplate ListFooterTemplate` | Gets or sets the list footer template. | TK |
| `DataTemplate ListHeaderTemplate` | Gets or sets the list header template. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler<LinkUnlinkEventArgs> Link` | Occurs when an item is about to be "realized". | TK |
| `EventHandler<LinkUnlinkEventArgs> Unlink` | Occurs when an item is about to be "un-realized". | TK |

## Microsoft.Phone.Controls.Primitives.ToggleSwitchButton

Source: Toolkit · class : ToggleButton · `Microsoft.Phone.Controls.Toolkit/Primitives/ToggleSwitchButton.cs`

Represents a switch that can be toggled between two states.

### Properties

| Member | Description | Src |
|---|---|---|
| `Brush SwitchForeground` | Gets or sets the switch foreground. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty SwitchForegroundProperty` | Identifies the SwitchForeground dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Gets all the template parts and initializes the corresponding state. | TK |
| `«ctor» ToggleSwitchButton()` | Initializes a new instance of the ToggleSwitch class. | TK |
