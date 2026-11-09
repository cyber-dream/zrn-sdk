# Silverlight controls (System.Windows.Controls)

## Contents

- [`System.Windows.Controls.AutoCompleteBox`](#systemwindowscontrolsautocompletebox)
- [`System.Windows.Controls.AutoCompleteFilterMode`](#systemwindowscontrolsautocompletefiltermode)
- [`System.Windows.Controls.Border`](#systemwindowscontrolsborder)
- [`System.Windows.Controls.Button`](#systemwindowscontrolsbutton)
- [`System.Windows.Controls.Canvas`](#systemwindowscontrolscanvas)
- [`System.Windows.Controls.CheckBox`](#systemwindowscontrolscheckbox)
- [`System.Windows.Controls.CleanUpVirtualizedItemEventArgs`](#systemwindowscontrolscleanupvirtualizeditemeventargs)
- [`System.Windows.Controls.CleanUpVirtualizedItemEventHandler`](#systemwindowscontrolscleanupvirtualizeditemeventhandler)
- [`System.Windows.Controls.ClickMode`](#systemwindowscontrolsclickmode)
- [`System.Windows.Controls.ColumnDefinition`](#systemwindowscontrolscolumndefinition)
- [`System.Windows.Controls.ColumnDefinitionCollection`](#systemwindowscontrolscolumndefinitioncollection)
- [`System.Windows.Controls.ComboBox`](#systemwindowscontrolscombobox)
- [`System.Windows.Controls.ComboBoxItem`](#systemwindowscontrolscomboboxitem)
- [`System.Windows.Controls.ContentChangedEventArgs`](#systemwindowscontrolscontentchangedeventargs)
- [`System.Windows.Controls.ContentChangedEventHandler`](#systemwindowscontrolscontentchangedeventhandler)
- [`System.Windows.Controls.ContentControl`](#systemwindowscontrolscontentcontrol)
- [`System.Windows.Controls.ContentPresenter`](#systemwindowscontrolscontentpresenter)
- [`System.Windows.Controls.ContextMenuService`](#systemwindowscontrolscontextmenuservice)
- [`System.Windows.Controls.Control`](#systemwindowscontrolscontrol)
- [`System.Windows.Controls.ControlTemplate`](#systemwindowscontrolscontroltemplate)
- [`System.Windows.Controls.DrawEventArgs`](#systemwindowscontrolsdraweventargs)
- [`System.Windows.Controls.DrawingSurface`](#systemwindowscontrolsdrawingsurface)
- [`System.Windows.Controls.Grid`](#systemwindowscontrolsgrid)
- [`System.Windows.Controls.HeaderedItemsControl`](#systemwindowscontrolsheadereditemscontrol)
- [`System.Windows.Controls.HyperlinkButton`](#systemwindowscontrolshyperlinkbutton)
- [`System.Windows.Controls.INavigate`](#systemwindowscontrolsinavigate)
- [`System.Windows.Controls.ISelectionAdapter`](#systemwindowscontrolsiselectionadapter)
- [`System.Windows.Controls.Image`](#systemwindowscontrolsimage)
- [`System.Windows.Controls.InkPresenter`](#systemwindowscontrolsinkpresenter)
- [`System.Windows.Controls.ItemCollection`](#systemwindowscontrolsitemcollection)
- [`System.Windows.Controls.ItemContainerGenerator`](#systemwindowscontrolsitemcontainergenerator)
- [`System.Windows.Controls.ItemsControl`](#systemwindowscontrolsitemscontrol)
- [`System.Windows.Controls.ItemsPanelTemplate`](#systemwindowscontrolsitemspaneltemplate)
- [`System.Windows.Controls.ItemsPresenter`](#systemwindowscontrolsitemspresenter)
- [`System.Windows.Controls.LengthConverter`](#systemwindowscontrolslengthconverter)
- [`System.Windows.Controls.ListBox`](#systemwindowscontrolslistbox)
- [`System.Windows.Controls.ListBoxItem`](#systemwindowscontrolslistboxitem)
- [`System.Windows.Controls.ManipulationMode`](#systemwindowscontrolsmanipulationmode)
- [`System.Windows.Controls.MediaElement`](#systemwindowscontrolsmediaelement)
- [`System.Windows.Controls.MultiScaleImage`](#systemwindowscontrolsmultiscaleimage)
- [`System.Windows.Controls.MultiScaleSubImage`](#systemwindowscontrolsmultiscalesubimage)
- [`System.Windows.Controls.NotifyEventArgs`](#systemwindowscontrolsnotifyeventargs)
- [`System.Windows.Controls.OpenFileDialog`](#systemwindowscontrolsopenfiledialog)
- [`System.Windows.Controls.Orientation`](#systemwindowscontrolsorientation)
- [`System.Windows.Controls.Panel`](#systemwindowscontrolspanel)
- [`System.Windows.Controls.PasswordBox`](#systemwindowscontrolspasswordbox)
- [`System.Windows.Controls.PopulatedEventArgs`](#systemwindowscontrolspopulatedeventargs)
- [`System.Windows.Controls.PopulatingEventArgs`](#systemwindowscontrolspopulatingeventargs)
- [`System.Windows.Controls.Primitives.ButtonBase`](#systemwindowscontrolsprimitivesbuttonbase)
- [`System.Windows.Controls.Primitives.DragCompletedEventArgs`](#systemwindowscontrolsprimitivesdragcompletedeventargs)
- [`System.Windows.Controls.Primitives.DragCompletedEventHandler`](#systemwindowscontrolsprimitivesdragcompletedeventhandler)
- [`System.Windows.Controls.Primitives.DragDeltaEventArgs`](#systemwindowscontrolsprimitivesdragdeltaeventargs)
- [`System.Windows.Controls.Primitives.DragDeltaEventHandler`](#systemwindowscontrolsprimitivesdragdeltaeventhandler)
- [`System.Windows.Controls.Primitives.DragStartedEventArgs`](#systemwindowscontrolsprimitivesdragstartedeventargs)
- [`System.Windows.Controls.Primitives.DragStartedEventHandler`](#systemwindowscontrolsprimitivesdragstartedeventhandler)
- [`System.Windows.Controls.Primitives.GeneratorDirection`](#systemwindowscontrolsprimitivesgeneratordirection)
- [`System.Windows.Controls.Primitives.GeneratorPosition`](#systemwindowscontrolsprimitivesgeneratorposition)
- [`System.Windows.Controls.Primitives.IItemContainerGenerator`](#systemwindowscontrolsprimitivesiitemcontainergenerator)
- [`System.Windows.Controls.Primitives.IRecyclingItemContainerGenerator`](#systemwindowscontrolsprimitivesirecyclingitemcontainergenerator)
- [`System.Windows.Controls.Primitives.IScrollInfo`](#systemwindowscontrolsprimitivesiscrollinfo)
- [`System.Windows.Controls.Primitives.ItemsChangedEventArgs`](#systemwindowscontrolsprimitivesitemschangedeventargs)
- [`System.Windows.Controls.Primitives.ItemsChangedEventHandler`](#systemwindowscontrolsprimitivesitemschangedeventhandler)
- [`System.Windows.Controls.Primitives.LayoutInformation`](#systemwindowscontrolsprimitiveslayoutinformation)
- [`System.Windows.Controls.Primitives.MenuBase`](#systemwindowscontrolsprimitivesmenubase)
- [`System.Windows.Controls.Primitives.PlacementMode`](#systemwindowscontrolsprimitivesplacementmode)
- [`System.Windows.Controls.Primitives.Popup`](#systemwindowscontrolsprimitivespopup)
- [`System.Windows.Controls.Primitives.RangeBase`](#systemwindowscontrolsprimitivesrangebase)
- [`System.Windows.Controls.Primitives.RepeatButton`](#systemwindowscontrolsprimitivesrepeatbutton)
- [`System.Windows.Controls.Primitives.ScrollBar`](#systemwindowscontrolsprimitivesscrollbar)
- [`System.Windows.Controls.Primitives.ScrollEventArgs`](#systemwindowscontrolsprimitivesscrolleventargs)
- [`System.Windows.Controls.Primitives.ScrollEventHandler`](#systemwindowscontrolsprimitivesscrolleventhandler)
- [`System.Windows.Controls.Primitives.ScrollEventType`](#systemwindowscontrolsprimitivesscrolleventtype)
- [`System.Windows.Controls.Primitives.Selector`](#systemwindowscontrolsprimitivesselector)
- [`System.Windows.Controls.Primitives.Thumb`](#systemwindowscontrolsprimitivesthumb)
- [`System.Windows.Controls.Primitives.ToggleButton`](#systemwindowscontrolsprimitivestogglebutton)
- [`System.Windows.Controls.ProgressBar`](#systemwindowscontrolsprogressbar)
- [`System.Windows.Controls.RadioButton`](#systemwindowscontrolsradiobutton)
- [`System.Windows.Controls.RichTextBox`](#systemwindowscontrolsrichtextbox)
- [`System.Windows.Controls.RichTextBoxOverflow`](#systemwindowscontrolsrichtextboxoverflow)
- [`System.Windows.Controls.RoutedPropertyChangingEventArgs<T>`](#systemwindowscontrolsroutedpropertychangingeventargst)
- [`System.Windows.Controls.RowDefinition`](#systemwindowscontrolsrowdefinition)
- [`System.Windows.Controls.RowDefinitionCollection`](#systemwindowscontrolsrowdefinitioncollection)
- [`System.Windows.Controls.SaveFileDialog`](#systemwindowscontrolssavefiledialog)
- [`System.Windows.Controls.ScrollBarVisibility`](#systemwindowscontrolsscrollbarvisibility)
- [`System.Windows.Controls.ScrollContentPresenter`](#systemwindowscontrolsscrollcontentpresenter)
- [`System.Windows.Controls.ScrollViewer`](#systemwindowscontrolsscrollviewer)
- [`System.Windows.Controls.SelectionChangedEventArgs`](#systemwindowscontrolsselectionchangedeventargs)
- [`System.Windows.Controls.SelectionChangedEventHandler`](#systemwindowscontrolsselectionchangedeventhandler)
- [`System.Windows.Controls.SelectionMode`](#systemwindowscontrolsselectionmode)
- [`System.Windows.Controls.SelectorSelectionAdapter`](#systemwindowscontrolsselectorselectionadapter)
- [`System.Windows.Controls.Separator`](#systemwindowscontrolsseparator)
- [`System.Windows.Controls.Slider`](#systemwindowscontrolsslider)
- [`System.Windows.Controls.StackPanel`](#systemwindowscontrolsstackpanel)
- [`System.Windows.Controls.StretchDirection`](#systemwindowscontrolsstretchdirection)
- [`System.Windows.Controls.TemplatedVisualTreeExtensions`](#systemwindowscontrolstemplatedvisualtreeextensions)
- [`System.Windows.Controls.TextBlock`](#systemwindowscontrolstextblock)
- [`System.Windows.Controls.TextBox`](#systemwindowscontrolstextbox)
- [`System.Windows.Controls.TextChangedEventArgs`](#systemwindowscontrolstextchangedeventargs)
- [`System.Windows.Controls.TextChangedEventHandler`](#systemwindowscontrolstextchangedeventhandler)
- [`System.Windows.Controls.TextSearch`](#systemwindowscontrolstextsearch)
- [`System.Windows.Controls.TimeTypeConverter`](#systemwindowscontrolstimetypeconverter)
- [`System.Windows.Controls.ToolTip`](#systemwindowscontrolstooltip)
- [`System.Windows.Controls.ToolTipService`](#systemwindowscontrolstooltipservice)
- [`System.Windows.Controls.UIElementCollection`](#systemwindowscontrolsuielementcollection)
- [`System.Windows.Controls.UserControl`](#systemwindowscontrolsusercontrol)
- [`System.Windows.Controls.Validation`](#systemwindowscontrolsvalidation)
- [`System.Windows.Controls.ValidationError`](#systemwindowscontrolsvalidationerror)
- [`System.Windows.Controls.ValidationErrorEventAction`](#systemwindowscontrolsvalidationerroreventaction)
- [`System.Windows.Controls.ValidationErrorEventArgs`](#systemwindowscontrolsvalidationerroreventargs)
- [`System.Windows.Controls.Viewbox`](#systemwindowscontrolsviewbox)
- [`System.Windows.Controls.VirtualizationMode`](#systemwindowscontrolsvirtualizationmode)
- [`System.Windows.Controls.VirtualizingPanel`](#systemwindowscontrolsvirtualizingpanel)
- [`System.Windows.Controls.VirtualizingStackPanel`](#systemwindowscontrolsvirtualizingstackpanel)
- [`System.Windows.Controls.WebBrowser`](#systemwindowscontrolswebbrowser)
- [`System.Windows.Controls.WebBrowserBrush`](#systemwindowscontrolswebbrowserbrush)
- [`System.Windows.Controls.WrapPanel`](#systemwindowscontrolswrappanel)

## System.Windows.Controls.AutoCompleteBox

Source: Toolkit · class : Control, IUpdateVisualState · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/AutoCompleteBox.cs`

Represents a control that provides a text box for user input and a drop-down that contains possible matches based on the input in the text box.

### Properties

| Member | Description | Src |
|---|---|---|
| `AutoCompleteFilterMode FilterMode` | Gets or sets how the text in the text box is used to filter items specified by the `ItemsSource` property for display in the drop-down. <br>Value: One of the `AutoCompleteFilterMode` values The default is `StartsWith`. | TK |
| `InputScope InputScope` | Gets or sets the `InputScope` used by the Text template part. | TK |
| `bool IsDropDownOpen` | Gets or sets a value indicating whether the drop-down portion of the control is open. <br>Value: True if the drop-down is open; otherwise, false. The default is false. | TK |
| `bool IsTextCompletionEnabled` | Gets or sets a value indicating whether the first possible match found during the filtering process will be displayed automatically in the text box. <br>Value: True if the first possible match found will be displayed automatically in the text box; otherwise, false. The default is false. | TK |
| `Style ItemContainerStyle` | Gets or sets the `Style` that is applied to the selection adapter contained in the drop-down portion of the `AutoCompleteBox` control. <br>Value: The `Style` applied to the selection adapter contained in the drop-down portion of the `AutoCompleteBox` control. The default is null. | TK |
| `AutoCompleteFilterPredicate<object> ItemFilter` | Gets or sets the custom method that uses user-entered text to filter the items specified by the `ItemsSource` property for display in the drop-down. <br>Value: The custom method that uses the user-entered text to filter the items specified by the `ItemsSource` property. The default is null. | TK |
| `DataTemplate ItemTemplate` | Gets or sets the `DataTemplate` used to display each item in the drop-down portion of the control. <br>Value: The `DataTemplate` used to display each item in the drop-down. The default is null. | TK |
| `IEnumerable ItemsSource` | Gets or sets a collection that is used to generate the items for the drop-down portion of the `AutoCompleteBox` control. <br>Value: The collection that is used to generate the items of the drop-down portion of the `AutoCompleteBox` control. | TK |
| `double MaxDropDownHeight` | Gets or sets the maximum height of the drop-down portion of the `AutoCompleteBox` control. <br>Value: The maximum height of the drop-down portion of the `AutoCompleteBox` control. The default is `PositiveInfinity`. | TK |
| `int MinimumPopulateDelay` | Gets or sets the minimum delay, in milliseconds, after text is typed in the text box before the `AutoCompleteBox` control populates the list of possible matches in the drop-down. <br>Value: The minimum delay, in milliseconds, after text is typed in the text box, but before the `AutoCompleteBox` populates the list of possible matches in the drop-down. The default is 0. | TK |
| `int MinimumPrefixLength` | Gets or sets the minimum number of characters required to be entered in the text box before the `AutoCompleteBox` displays possible matches. matches. <br>Value: The minimum number of characters to be entered in the text box before the `AutoCompleteBox` displays possible matches. The default is 1. | TK |
| `string SearchText` | Gets the text that is used to filter items in the `ItemsSource` item collection. <br>Value: The text that is used to filter items in the `ItemsSource` item collection. | TK |
| `object SelectedItem` | Gets or sets the selected item in the drop-down. <br>Value: The selected item in the drop-down. | TK |
| `string Text` | Gets or sets the text in the text box portion of the `AutoCompleteBox` control. <br>Value: The text in the text box portion of the `AutoCompleteBox` control. | TK |
| `Style TextBoxStyle` | Gets or sets the `Style` applied to the text box portion of the `AutoCompleteBox` control. <br>Value: The `Style` applied to the text box portion of the `AutoCompleteBox` control. The default is null. | TK |
| `AutoCompleteFilterPredicate<string> TextFilter` | Gets or sets the custom method that uses the user-entered text to filter items specified by the `ItemsSource` property in a text-based way for display in the drop-down. <br>Value: The custom method that uses the user-entered text to filter items specified by the `ItemsSource` property in a text-based way for display in the drop-down. | TK |
| `Binding ValueMemberBinding` | Gets or sets the `Binding` that is used to get the values for display in the text portion of the `AutoCompleteBox` control. <br>Value: The `Binding` object used when binding to a collection property. | TK |
| `string ValueMemberPath` | Gets or sets the property path that is used to get values for display in the text portion of the `AutoCompleteBox` control. <br>Value: The property path that is used to get values for display in the text portion of the `AutoCompleteBox` control. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty FilterModeProperty` | Gets the identifier for the `FilterMode` dependency property. | TK |
| `static DependencyProperty InputScopeProperty` | Identifies the `InputScope` dependency property. | TK |
| `static DependencyProperty IsDropDownOpenProperty` | Identifies the `IsDropDownOpen` dependency property. <br>Value: The identifier for the `IsDropDownOpen` dependency property. | TK |
| `static DependencyProperty IsTextCompletionEnabledProperty` | Identifies the `IsTextCompletionEnabled` dependency property. <br>Value: The identifier for the `IsTextCompletionEnabled` dependency property. | TK |
| `static DependencyProperty ItemContainerStyleProperty` | Identifies the `ItemContainerStyle` dependency property. <br>Value: The identifier for the `ItemContainerStyle` dependency property. | TK |
| `static DependencyProperty ItemFilterProperty` | Identifies the `ItemFilter` dependency property. <br>Value: The identifier for the `ItemFilter` dependency property. | TK |
| `static DependencyProperty ItemTemplateProperty` | Identifies the `ItemTemplate` dependency property. <br>Value: The identifier for the `ItemTemplate` dependency property. | TK |
| `static DependencyProperty ItemsSourceProperty` | Identifies the `ItemsSource` dependency property. <br>Value: The identifier for the `ItemsSource` dependency property. | TK |
| `static DependencyProperty MaxDropDownHeightProperty` | Identifies the `MaxDropDownHeight` dependency property. <br>Value: The identifier for the `MaxDropDownHeight` dependency property. | TK |
| `static DependencyProperty MinimumPopulateDelayProperty` | Identifies the `MinimumPopulateDelay` dependency property. <br>Value: The identifier for the `MinimumPopulateDelay` dependency property. | TK |
| `static DependencyProperty MinimumPrefixLengthProperty` | Identifies the `MinimumPrefixLength` dependency property. <br>Value: The identifier for the `MinimumPrefixLength` dependency property. | TK |
| `static DependencyProperty SearchTextProperty` | Identifies the `SearchText` dependency property. <br>Value: The identifier for the `SearchText` dependency property. | TK |
| `static DependencyProperty SelectedItemProperty` | Identifies the `SelectedItem` dependency property. <br>Value: The identifier the `SelectedItem` dependency property. | TK |
| `static RoutedEvent SelectionChangedEvent` | Declares the routed event for SelectionChanged. | TK |
| `static DependencyProperty TextBoxStyleProperty` | Identifies the `TextBoxStyle` dependency property. <br>Value: The identifier for the `TextBoxStyle` dependency property. | TK |
| `static DependencyProperty TextFilterProperty` | Identifies the `TextFilter` dependency property. <br>Value: The identifier for the `TextFilter` dependency property. | TK |
| `static DependencyProperty TextProperty` | Identifies the `Text` dependency property. <br>Value: The identifier for the `Text` dependency property. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedPropertyChangedEventHandler<bool> DropDownClosed` | Occurs when the `IsDropDownOpen` property was changed from true to false and the drop-down is open. | TK |
| `RoutedPropertyChangingEventHandler<bool> DropDownClosing` | Occurs when the `IsDropDownOpen` property is changing from true to false. | TK |
| `RoutedPropertyChangedEventHandler<bool> DropDownOpened` | Occurs when the value of the `IsDropDownOpen` property has changed from false to true and the drop-down is open. | TK |
| `RoutedPropertyChangingEventHandler<bool> DropDownOpening` | Occurs when the value of the `IsDropDownOpen` property is changing from false to true. | TK |
| `PopulatedEventHandler Populated` | Occurs when the `AutoCompleteBox` has populated the drop-down with possible matches based on the `Text` property. | TK |
| `PopulatingEventHandler Populating` | Occurs when the `AutoCompleteBox` is populating the drop-down with possible matches based on the `Text` property. | TK |
| `SelectionChangedEventHandler SelectionChanged` | Occurs when the selected item in the drop-down portion of the `AutoCompleteBox` has changed. | TK |
| `RoutedEventHandler TextChanged` | Occurs when the text in the text box portion of the `AutoCompleteBox` changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Builds the visual tree for the `AutoCompleteBox` control when a new template is applied. | TK |
| `void PopulateComplete()` | Notifies the `AutoCompleteBox` that the `ItemsSource` property has been set and the data can be filtered to provide possible matches in the drop-down. | TK |
| `«ctor» AutoCompleteBox()` | Initializes a new instance of the `AutoCompleteBox` class. | TK |

## System.Windows.Controls.AutoCompleteFilterMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/AutoCompleteFilterMode.cs`

Specifies how text in the text box portion of the `AutoCompleteBox` control is used to filter items specified by the `ItemsSource` property for display in the drop-down.

### Values

| Member | Description | Src |
|---|---|---|
| `None = 0` | Specifies that no filter is used. All items are returned. | TK |
| `StartsWith = 1` | Specifies a culture-sensitive, case-insensitive filter where the returned items start with the specified text. The filter uses the `StringComparison)` method, specifying `CurrentCultureIgnoreCase` as the string comparison criteria. | TK |
| `StartsWithCaseSensitive = 2` | Specifies a culture-sensitive, case-sensitive filter where the returned items start with the specified text. The filter uses the `StringComparison)` method, specifying `CurrentCulture` as the string comparison criteria. | TK |
| `StartsWithOrdinal = 3` | Specifies an ordinal, case-insensitive filter where the returned items start with the specified text. The filter uses the `StringComparison)` method, specifying `OrdinalIgnoreCase` as the string comparison criteria. | TK |
| `StartsWithOrdinalCaseSensitive = 4` | Specifies an ordinal, case-sensitive filter where the returned items start with the specified text. The filter uses the `StringComparison)` method, specifying `Ordinal` as the string comparison criteria. | TK |
| `Contains = 5` | Specifies a culture-sensitive, case-insensitive filter where the returned items contain the specified text. | TK |
| `ContainsCaseSensitive = 6` | Specifies a culture-sensitive, case-sensitive filter where the returned items contain the specified text. | TK |
| `ContainsOrdinal = 7` | Specifies an ordinal, case-insensitive filter where the returned items contain the specified text. | TK |
| `ContainsOrdinalCaseSensitive = 8` | Specifies an ordinal, case-sensitive filter where the returned items contain the specified text. | TK |
| `Equals = 9` | Specifies a culture-sensitive, case-insensitive filter where the returned items equal the specified text. The filter uses the `StringComparison)` method, specifying `CurrentCultureIgnoreCase` as the search comparison criteria. | TK |
| `EqualsCaseSensitive = 10` | Specifies a culture-sensitive, case-sensitive filter where the returned items equal the specified text. The filter uses the `StringComparison)` method, specifying `CurrentCulture` as the string comparison criteria. | TK |
| `EqualsOrdinal = 11` | Specifies an ordinal, case-insensitive filter where the returned items equal the specified text. The filter uses the `StringComparison)` method, specifying `OrdinalIgnoreCase` as the string comparison criteria. | TK |
| `EqualsOrdinalCaseSensitive = 12` | Specifies an ordinal, case-sensitive filter where the returned items equal the specified text. The filter uses the `StringComparison)` method, specifying `Ordinal` as the string comparison criteria. | TK |
| `Custom = 13` | Specifies that a custom filter is used. This mode is used when the `TextFilter` or `ItemFilter` properties are set. | TK |

## System.Windows.Controls.Border

Source: SDK

Draws a border, background, or both around another object.

### Properties

| Member | Description | Src |
|---|---|---|
| `Background` | Gets or sets the `Brush` that fills the background of the border. <br>Returns: The brush that fills the background. | SDK |
| `BorderBrush` | Gets or sets the `Brush` that is used to create the border. <br>Returns: The brush that fills the border. | SDK |
| `BorderThickness` | Gets or sets the thickness of the border. <br>Returns: The thickness of the border in pixels. The default is 0 on all four sides. | SDK |
| `Child` | Gets or sets the child element to draw the border around. <br>Returns: The `UIElement` to apply the border to. | SDK |
| `CornerRadius` | Gets or sets the radius for the corners of the border. <br>Returns: The degree to which the corners are rounded. | SDK |
| `Padding` | Gets or sets the distance between the border and its child object. <br>Returns: The dimensions of the space between the border and its child as a `Thickness` value. The `Thickness` values are in pixels. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BackgroundProperty` | Identifies the `Background` dependency property. <br>Returns: The identifier for the `Background` dependency property. | SDK |
| `BorderBrushProperty` | Identifies the `BorderBrush` dependency property. <br>Returns: The identifier for the `BorderBrush` dependency property. | SDK |
| `BorderThicknessProperty` | Identifies the `BorderThickness` dependency property. <br>Returns: The identifier for the `BorderThickness` dependency property. | SDK |
| `CornerRadiusProperty` | Identifies the `CornerRadius` dependency property. <br>Returns: The identifier for the `CornerRadius` dependency property. | SDK |
| `PaddingProperty` | Identifies the `Padding` dependency property. <br>Returns: The identifier for the `Padding` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Border` class. | SDK |

## System.Windows.Controls.Button

Source: SDK

Represents a button control.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Button` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `Button` when a new template is applied. | SDK |
| `OnClick` | Raises the `Click` event. | SDK |
| `OnCreateAutomationPeer` | Returns a `ButtonAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: `ButtonAutomationPeer` for the `Button` object. | SDK |

## System.Windows.Controls.Canvas

Source: SDK

Defines an area within which you can explicitly position child objects by using coordinates that are relative to the area.

### Properties

| Member | Description | Src |
|---|---|---|
| `Left` | Gets or sets the distance between the left side of an object and the left side of its parent `Canvas`. <br>Returns: The offset position from the left side of a parent `Canvas`, in pixels. The default is 0. | SDK |
| `Top` | Gets or sets the distance between the top of an element and the top of its parent `Canvas`. <br>Returns: The offset position from the top of a parent `Canvas`, in pixels. The default is 0. | SDK |
| `ZIndex` | Gets or sets the z-order rendering behavior of objects in a `Canvas`. <br>Returns: The z-order of the object within the `Canvas`. The default is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `LeftProperty` | Identifies the `Left` attached property. <br>Returns: The identifier for the `Left` attached property. | SDK |
| `TopProperty` | Identifies the `Top` attached property. <br>Returns: The identifier for the `Top` attached property. | SDK |
| `ZIndexProperty` | Identifies the `ZIndex` attached property. <br>Returns: The identifier for the `ZIndex` attached property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Canvas` class. | SDK |
| `ArrangeOverride(Size)` | Provides the behavior for the "arrange" pass of Silverlight layout. <br>`arrangeSize`: The size that this object should use to arrange its child objects. <br>Returns: The actual size used. | SDK |
| `GetLeft(UIElement)` | Gets the value of the `Left` attached property for a given object. <br>`element`: The object from which the property value is read. <br>Returns: The `Left` value of the specified object. | SDK |
| `GetTop(UIElement)` | Gets the value of the `Top` attached property for a given object. <br>`element`: The object from which the property value is read. <br>Returns: The `Top` value of the specified object. | SDK |
| `GetZIndex(UIElement)` | Gets the value of the `ZIndex` attached property for a given object. <br>`element`: The object from which the property value is read. <br>Returns: The `ZIndex` value of the requested object. The maximum allowed value for `ZIndex` is Int16.MaxValue – 1 = 32766. | SDK |
| `MeasureOverride(Size)` | Provides the behavior for the "measure" pass of Silverlight layout. <br>`constraint`: The available size that this object can give to child objects. Infinity can be specified as a value to indicate that the object will size to whatever content is available. <br>Returns: The size that this object determines it needs during layout, based on its calculations of child object allotted sizes. | SDK |
| `SetLeft(UIElement,Double)` | Sets the value of the `Left` attached property for a given object. <br>`element`: The object to which the property value is written. <br>`length`: The `Left` value of the specified element. | SDK |
| `SetTop(UIElement,Double)` | Sets the value of the `Top` attached property for a given object. <br>`element`: The object to which the property value is written. <br>`length`: The `Top` value of the specified object. | SDK |
| `SetZIndex(UIElement,Int32)` | Sets the value of the `ZIndex` attached property for a given object. <br>`element`: The object to which the property value is written. The maximum allowed value for `ZIndex` is Int16.MaxValue – 1 = 32766. <br>`value`: The `ZIndex` value of the specified object. | SDK |

## System.Windows.Controls.CheckBox

Source: SDK

Represents a control that a user can select (check) or clear (uncheck).

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `CheckBox` class. | SDK |
| `OnCreateAutomationPeer` | Returns a `CheckBoxAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `CheckBoxAutomationPeer` for the check box object. | SDK |

## System.Windows.Controls.CleanUpVirtualizedItemEventArgs

Source: SDK

Provides data for the `CleanUpVirtualizedItemEvent` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Cancel` | Gets or sets a value that indicates whether this item should not be re-virtualized. <br>Returns: true if you want to prevent revirtualization of this item; otherwise false. | SDK |
| `UIElement` | Gets an instance of the visual element that represents the data value. <br>Returns: The `UIElement` that represents the data value. | SDK |
| `Value` | Gets an `Object` that represents the original data value. <br>Returns: The `Object` that represents the original data value. | SDK |

## System.Windows.Controls.CleanUpVirtualizedItemEventHandler

Source: SDK

Represents the method that handles the `CleanUpVirtualizedItemEvent` attached events.

## System.Windows.Controls.ClickMode

Source: SDK

Specifies when the `Click` event should be raised for a control.

### Fields

| Member | Description | Src |
|---|---|---|
| `Hover` | Specifies that the `Click` event should be raised when the mouse pointer moves over the control. | SDK |
| `Press` | Specifies that the `Click` event should be raised when the mouse button is pressed and the mouse pointer is over the control. If you are using the keyboard, specifies that the `Click` event should be raised when the SPACEBAR or ENTER is pressed and the control has keyboard focus. | SDK |
| `Release` | Specifies that the `Click` event should be raised when the left mouse button is pressed and released, and the mouse pointer is over the control. If you are using the keyboard, specifies that the `Click` event should be raised when the SPACEBAR or ENTER key is pressed and released, and the control has keyboard focus. | SDK |

## System.Windows.Controls.ColumnDefinition

Source: SDK

Defines column-specific properties that apply to `Grid` objects.

### Properties

| Member | Description | Src |
|---|---|---|
| `ActualWidth` | Gets a value that represents the actual calculated width of a `ColumnDefinition`. <br>Returns: A `Double` that represents the actual calculated width in pixels. The default is 0. | SDK |
| `MaxWidth` | Gets or sets a value that represents the maximum width of a `ColumnDefinition`. <br>Returns: A `Double` that represents the maximum width in pixels. The default is `PositiveInfinity`. | SDK |
| `MinWidth` | Gets or sets a value that represents the minimum width of a `ColumnDefinition`. <br>Returns: A `Double` that represents the minimum width in pixels. The default is 0. | SDK |
| `Width` | Gets the calculated width of a `ColumnDefinition` element, or sets the `GridLength` value of a column that is defined by the `ColumnDefinition`. <br>Returns: The `GridLength` that represents the width of the column. The default value is 1.0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `MaxWidthProperty` | Identifies the `MaxWidth` dependency property. <br>Returns: The identifier for the `MaxWidth` dependency property. | SDK |
| `MinWidthProperty` | Identifies the `MinWidth` dependency property. <br>Returns: The identifier for the `MinWidth` dependency property. | SDK |
| `WidthProperty` | Identifies the `Width` dependency property. <br>Returns: The identifier for the `Width` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ColumnDefinition` class. | SDK |

## System.Windows.Controls.ColumnDefinitionCollection

Source: SDK

Provides access to an ordered, strongly typed collection of `ColumnDefinition` objects.

## System.Windows.Controls.ComboBox

Source: SDK

Represents a selection control that combines a non-editable text box and a drop-down containing a list box that allows users to select an item from a list.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsDropDownOpen` | Gets or sets a value that indicates whether the drop-down portion of the combo box is currently open. <br>Returns: true if the drop-down portion is open; otherwise, false. The default is false. | SDK |
| `IsEditable` | Gets a value that indicates whether the user can edit text in the text box portion of a combo box. This property always returns false. <br>Returns: false in all cases. | SDK |
| `IsSelectionBoxHighlighted` | Gets a value that indicates whether the `SelectionBoxItem` is highlighted. <br>Returns: true if the `SelectionBoxItem` is highlighted; otherwise, false. The default is true. | SDK |
| `ItemContainerStyle` | Gets or sets the style applied to the container generated for each item in the combo box. <br>Returns: The style applied to the container generated for each item in the combo box. | SDK |
| `MaxDropDownHeight` | Gets or sets the maximum height of the drop-down that lists combo box items. <br>Returns: The maximum height of the drop-down that lists combo box items. The default is infinity. | SDK |
| `SelectionBoxItem` | Gets the item displayed in the selection box. <br>Returns: The item displayed in the selection box. | SDK |
| `SelectionBoxItemTemplate` | Gets the template applied to the selection box content. <br>Returns: The template applied to the selection box content. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsDropDownOpenProperty` | Identifies the `IsDropDownOpen` dependency property. <br>Returns: The identifier for the `IsDropDownOpen` dependency property. | SDK |
| `IsSelectionActiveProperty` | Identifies the IsSelectionActive attached property. <br>Returns: The identifier for the IsSelectionActive attached property. | SDK |
| `ItemContainerStyleProperty` | Identifies the `ItemContainerStyle` dependency property. <br>Returns: The identifier for the `ItemContainerStyle` dependency property. | SDK |
| `MaxDropDownHeightProperty` | Identifies the `MaxDropDownHeight` dependency property. <br>Returns: The identifier for the `MaxDropDownHeight` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `DropDownClosed` | Occurs when the drop-down portion of the combo box closes. | SDK |
| `DropDownOpened` | Occurs when the drop-down portion of the combo box opens. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ComboBox` class. . | SDK |
| `ArrangeOverride(Size)` | Arranges and sizes the combo box control and its contents. <br>`arrangeBounds`: The size allowed for the combo box control. <br>Returns: The actual size of the combo box. | SDK |
| `ClearContainerForItemOverride(DependencyObject,Object)` | Removes all templates, styles, and bindings for the object displayed as a `ComboBoxItem`. <br>`element`: The container element to clear. <br>`item`: The item that is contained in the `ComboBoxItem`. | SDK |
| `GetContainerForItemOverride` | Retrieves an empty combo box item. <br>Returns: An empty combo box item. | SDK |
| `IsItemItsOwnContainerOverride(Object)` | Determines whether the specified item is (or is eligible to be) its own item container. <br>`item`: The item to evaluate. <br>Returns: true if the item is a `ComboBoxItem`; otherwise, false. The default is false. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ComboBox` when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns a `ComboBoxAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `ComboBoxAutomationPeer` for the `ComboBox` object. | SDK |
| `OnDropDownClosed(EventArgs)` | Raises the `DropDownClosed` event. <br>`e`: The event data. | SDK |
| `OnDropDownOpened(EventArgs)` | Raises the `DropDownOpened` event. <br>`e`: The event data. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Provides handling for the `GotFocus` event. <br>`e`: The event data. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Provides handling for the `ZeyDown` event. <br>`e`: The event data | SDK |
| `OnLostFocus(RoutedEventArgs)` | Provides handling for the `LostFocus` event. <br>`e`: The event data. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Provides handling for the `MouseEnter` event that occurs when the mouse pointer enters this control. <br>`e`: The event data. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Provides handling for the `MouseLeave` event that occurs when the mouse pointer leaves the combo box. <br>`e`: The event data. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Provides handling for the `MouseLeftButtonDown` event that occurs when the left mouse button is pressed while the mouse pointer is over the combo box. <br>`e`: The event data. | SDK |
| `OnMouseWheel(Input.MouseWheelEventArgs)` | Provides handling for the `MouseWheel` event. <br>`e`: A `MouseWheelEventArgs`that contains the event data. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the specified element to display the specified item. <br>`element`: The container element used to display the specified item. <br>`item`: The item to display. | SDK |

## System.Windows.Controls.ComboBoxItem

Source: SDK

Represents a selectable item contained in a `ComboBox` control.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ComboBoxItem` class. | SDK |
| `OnManipulationCompleted(Input.ManipulationCompletedEventArgs)` | Called when the `ManipulationCompleted`event occurs. <br>`e`: The data for the event. | SDK |
| `OnManipulationStarted(Input.ManipulationStartedEventArgs)` | Called when the `ManipulationStarted` event occurs. <br>`e`: The data for the event | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Provides handling for the `MouseLeftButtonUp`event. <br>`e`: The event data. | SDK |

## System.Windows.Controls.ContentChangedEventArgs

Source: SDK

Provides data for the `ContentChanged` event.

## System.Windows.Controls.ContentChangedEventHandler

Source: SDK

Represents the method that handles the `ContentChanged` event.

## System.Windows.Controls.ContentControl

Source: SDK

Represents a control with a single piece of content. Controls such as `Button`, `CheckBox`, and `ScrollViewer` directly or indirectly inherit from this class.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | Gets or sets the value of the `ContentControl` dependency property. <br>Returns: An object that contains the control's content. The default is null. | SDK |
| `ContentTemplate` | Gets or sets the data template that is used to display the content of the `ContentControl`. <br>Returns: The data template that is used to display the content of the `ContentControl`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ContentProperty` | Identifies the `Content` dependency property <br>Returns: The identifier for the `Content` dependency property. | SDK |
| `ContentTemplateProperty` | Identifies the `ContentTemplate` dependency property <br>Returns: The identifier for the `ContentTemplate` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ContentControl` class. | SDK |
| `OnContentChanged(Object,Object)` | Called when the value of the `Content` property changes. <br>`oldContent`: The old value of the `Content` property. <br>`newContent`: The new value of the `Content` property. | SDK |

## System.Windows.Controls.ContentPresenter

Source: SDK

Displays the content of a `ContentControl`.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | Gets or sets the data that is used to generate the child elements of a `ContentPresenter`. <br>Returns: The data that is used to generate the child elements. The default is null. | SDK |
| `ContentTemplate` | Gets or sets the template that is used to display the content of the control. <br>Returns: A `DataTemplate` that defines the visualization of the content. The default is null. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ContentProperty` | Identifies the `Content` dependency property <br>Returns: The identifier for the `Content` dependency property. | SDK |
| `ContentTemplateProperty` | Identifies the `ContentTemplate` dependency property. <br>Returns: The identifier for the `ContentTemplate` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ContentPresenter` class. | SDK |

## System.Windows.Controls.ContextMenuService

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/ContextMenu/ContextMenuService.cs`

Provides the system implementation for displaying a ContextMenu.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ContextMenuProperty` | Identifies the ContextMenu attached property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static ContextMenu GetContextMenu(DependencyObject element)` | Gets the value of the ContextMenu property of the specified object. <br>`element`: Object to query concerning the ContextMenu property. <br>Returns: Value of the ContextMenu property. | TK |
| `static void SetContextMenu(DependencyObject element, ContextMenu value)` | Sets the value of the ContextMenu property of the specified object. <br>`element`: Object to set the property on. <br>`value`: Value to set. | TK |

## System.Windows.Controls.Control

Source: SDK

Represents the base class for UI elements that use a `ControlTemplate` to define their appearance.

### Properties

| Member | Description | Src |
|---|---|---|
| `Background` | Gets or sets a brush that provides the background of the control. <br>Returns: The brush that provides the background of the control. The default is null. | SDK |
| `BorderBrush` | Gets or sets a brush that describes the border background of a control. <br>Returns: The brush that is used to fill the control's border; the default is null. | SDK |
| `BorderThickness` | Gets or sets the border thickness of a control. <br>Returns: A thickness value; the default is a thickness of 0 on all four sides. | SDK |
| `CharacterSpacing` | Gets or sets the distance between characters of text in the control measured in 1000ths of the font size. <br>Returns: The distance between characters of text in the control measured in 1000ths of the font size. The default is 0. | SDK |
| `DefaultStyleKey` | Gets or sets the key that references the default style for the control. <br>Returns: The key that references the default style for the control. To work correctly as part of theme style lookup, this value is expected to be the `Type` of the control being styled. | SDK |
| `FontFamily` | Gets or sets the font used to display text in the control. <br>Returns: The font used to display text in the control. The default is the "Portable User Interface". | SDK |
| `FontSize` | Gets or sets the size of the text in this control. <br>Returns: The size of the text in the `Control`. The default is 11 (in pixels). | SDK |
| `FontStretch` | Gets or sets the degree to which a font is condensed or expanded on the screen. <br>Returns: One of the values that specifies the degree to which a font is condensed or expanded on the screen. The default is `Normal`. | SDK |
| `FontStyle` | Gets or sets the style in which the text is rendered. <br>Returns: One of the values that specifies the style in which the text is rendered. The default is `Normal`. | SDK |
| `FontWeight` | Gets or sets the thickness of the specified font. <br>Returns: One of the values that specifies the thickness of the specified font. The default is `Normal`. | SDK |
| `Foreground` | Gets or sets a brush that describes the foreground color. <br>Returns: The brush that paints the foreground of the control. The default value is `Black`. | SDK |
| `HorizontalContentAlignment` | Gets or sets the horizontal alignment of the control's content. <br>Returns: One of the `HorizontalAlignment` values. The default is `Center`. | SDK |
| `IsEnabled` | Gets or sets a value indicating whether the user can interact with the control. <br>Returns: true if the user can interact with the control; otherwise, false. | SDK |
| `IsTabStop` | Gets or sets a value that indicates whether a control is included in tab navigation. <br>Returns: true if the control is included in tab navigation; otherwise, false. The default is true. | SDK |
| `Padding` | Gets or sets the padding inside a control. <br>Returns: The amount of space between the content of a `Control` and its `Margin` or `Border`. The default is a thickness of 0 on all four sides. | SDK |
| `TabIndex` | Gets or sets a value that determines the order in which elements receive focus when the user navigates through controls by using the TAB key. <br>Returns: A value that determines the order of logical navigation for a device. The default value is `MaxValue`. | SDK |
| `TabNavigation` | Gets or sets a value that modifies how tabbing and `TabIndex` work for this control. <br>Returns: A value of the enumeration. The default is `Local`. | SDK |
| `Template` | Gets or sets a control template. <br>Returns: The template that defines the appearance of the `Control`. | SDK |
| `VerticalContentAlignment` | Gets or sets the vertical alignment of the control's content. <br>Returns: One of the `VerticalAlignment` values. The default is `Center`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BackgroundProperty` | Identifies the `Background` dependency property <br>Returns: The identifier for the `Background` dependency property. | SDK |
| `BorderBrushProperty` | Identifies the `BorderBrush` dependency property. <br>Returns: The identifier for the `BorderBrush` dependency property. | SDK |
| `BorderThicknessProperty` | Identifies the `BorderThickness` dependency property. <br>Returns: The identifier for the `BorderThickness` dependency property. | SDK |
| `CharacterSpacingProperty` | Identifies the `CharacterSpacing` dependency property. <br>Returns: The identifier for the `CharacterSpacing`dependency property. | SDK |
| `DefaultStyleKeyProperty` | Identifies the `DefaultStyleKey` dependency property. <br>Returns: The identifier for the `DefaultStyleKey` dependency property. | SDK |
| `FontFamilyProperty` | Identifies the `FontFamily` dependency property. <br>Returns: The identifier for the `FontFamily` dependency property. | SDK |
| `FontSizeProperty` | Identifies the `FontStyle` dependency property. <br>Returns: The identifier for the `FontStyle` dependency property.f | SDK |
| `FontStretchProperty` | Identifies the `FontStretch` dependency property. <br>Returns: The identifier for the `FontStretch` dependency property. | SDK |
| `FontStyleProperty` | Identifies the `FontStyle` dependency property. <br>Returns: The identifier for the `FontStyle` dependency property. | SDK |
| `FontWeightProperty` | Identifies the `FontWeight` dependency property. <br>Returns: The identifier for the `FontWeight` dependency property. | SDK |
| `ForegroundProperty` | Identifies the `Foreground` dependency property. <br>Returns: The identifier for the `Foreground` dependency property. | SDK |
| `HorizontalContentAlignmentProperty` | Identifies the `HorizontalContentAlignment` dependency property. <br>Returns: The identifier for the `HorizontalContentAlignment` dependency property. | SDK |
| `IsEnabledProperty` | Identifies the `IsEnabled` dependency property. <br>Returns: The identifier for the `IsEnabled` dependency property. | SDK |
| `IsTabStopProperty` | Identifies the `IsTabStop` dependency property. <br>Returns: The identifier for the `IsTabStop` dependency property. | SDK |
| `PaddingProperty` | Identifies the `Padding` dependency property. <br>Returns: The identifier for the `Padding` dependency property. | SDK |
| `TabIndexProperty` | Identifies the `TabIndex` dependency property. <br>Returns: The identifier for the `TabIndex` dependency property. | SDK |
| `TabNavigationProperty` | Identifies the `TabNavigation` dependency property. <br>Returns: The identifier for the `TabNavigation` dependency property. | SDK |
| `TemplateProperty` | Identifies the `Template` dependency property. <br>Returns: The identifier for the `Template` dependency property. | SDK |
| `VerticalContentAlignmentProperty` | Identifies the `VerticalContentAlignment` dependency property. <br>Returns: The identifier for the `VerticalContentAlignment` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `IsEnabledChanged` | Occurs when the `IsEnabled` property changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Control` class. | SDK |
| `ApplyTemplate` | Loads the relevant control template so that its parts can be referenced. <br>Returns: Returns whether the visual tree was rebuilt by this call. true indicates the tree was rebuilt; false indicates that the previous visual tree was retained. | SDK |
| `Focus` | Attempts to set the focus on the control. <br>Returns: true if focus was set to the control, or focus was already on the control. false if the control is not focusable. | SDK |
| `GetTemplateChild(String)` | Retrieves the named element in the instantiated `ControlTemplate` visual tree. <br>`childName`: The name of element to find. <br>Returns: The named element from the template, if found. Can return null if no element with name was found in the template. | SDK |
| `OnDoubleTap(Input.GestureEventArgs)` | Called before the `DoubleTap` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnDragEnter(DragEventArgs)` | Called before the `DragEnter` event occurs. <br>`e`: A `DragEventArgs` that contains the event data. | SDK |
| `OnDragLeave(DragEventArgs)` | Called before the `DragLeave` event occurs. <br>`e`: A `DragEventArgs` that contains the event data. | SDK |
| `OnDragOver(DragEventArgs)` | Called before the `DragOver` event occurs. <br>`e`: A `DragEventArgs`that contains the event data. | SDK |
| `OnDrop(DragEventArgs)` | Called before the `Drop` event occurs. <br>`e`: A `DragEventArgs` that contains the event data. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Called before the `GotFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnHold(Input.GestureEventArgs)` | Called before the `Hold` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Called before the `ZeyDown` event occurs. <br>`e`: The data for the event. | SDK |
| `OnKeyUp(Input.ZeyEventArgs)` | Called before the `ZeyUp` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Called before the `LostFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostMouseCapture(Input.MouseEventArgs)` | Called before the `LostMouseCapture` event occurs to provide handling for the event in a derived class without attaching a delegate. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnManipulationCompleted(Input.ManipulationCompletedEventArgs)` | Called before the `ManipulationCompleted` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnManipulationDelta(Input.ManipulationDeltaEventArgs)` | Called before the `ManipulationDelta` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnManipulationStarted(Input.ManipulationStartedEventArgs)` | Called before the `ManipulationStarted` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Called before the `MouseEnter` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Called before the `MouseLeave` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Called before the `MouseLeftButtonDown` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Called before the `MouseLeftButtonUp` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseMove(Input.MouseEventArgs)` | Called before the `MouseMove` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseRightButtonDown(Input.MouseButtonEventArgs)` | Called before the `MouseRightButtonDown` event occurs. <br>`e`: A `MouseButtonEventArgs` that contains the event data. | SDK |
| `OnMouseRightButtonUp(Input.MouseButtonEventArgs)` | Called before the `MouseRightButtonUp` event occurs. <br>`e`: A `MouseButtonEventArgs` that contains the event data. | SDK |
| `OnMouseWheel(Input.MouseWheelEventArgs)` | Called before the `MouseWheel` event occurs to provide handling for the event in a derived class without attaching a delegate. <br>`e`: A `MouseWheelEventArgs` that contains the event data. | SDK |
| `OnTap(Input.GestureEventArgs)` | Called before the `Tap` event occurs. <br>`e`: Event data for the event. | SDK |
| `OnTextInput(Input.TextCompositionEventArgs)` | Called before the `TextInput` event occurs. <br>`e`: A `TextCompositionEventArgs` that contains the event data. | SDK |
| `OnTextInputStart(Input.TextCompositionEventArgs)` | Called before the `TextInputStart` event occurs. <br>`e`: A `TextCompositionEventArgs` that contains the event data. | SDK |
| `OnTextInputUpdate(Input.TextCompositionEventArgs)` | Called before the `TextInputUpdate` event occurs. <br>`e`: A `TextCompositionEventArgs` that contains the event data | SDK |

## System.Windows.Controls.ControlTemplate

Source: SDK

Defines the element tree that is applied as a control template.

### Properties

| Member | Description | Src |
|---|---|---|
| `TargetType` | Gets or sets the type to which the `ControlTemplate` is applied. <br>Returns: The type to which the `ControlTemplate` is applied. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ControlTemplate` class. | SDK |

## System.Windows.Controls.DrawEventArgs

Source: SDK

Provides data for the `Draw` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `DeltaTime` | Gets the difference between the value of `TotalTime` and the value of `TotalTime` at the last `Draw` event. <br>Returns: The delta between the current `TotalTime` and the previous `TotalTime`. | SDK |
| `GraphicsDevice` | Gets the `GraphicsDevice` associated with this event. <br>Returns: The graphics device associated with this event. | SDK |
| `TotalTime` | Gets the total time elapsed since the application was started. <br>Returns: The total time. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `InvalidateSurface` | Invalidates the `DrawingSurface` and requests a new `Draw` event. | SDK |

## System.Windows.Controls.DrawingSurface

Source: SDK

Defines an area within which 3-D content can be composed and rendered.

### Events

| Member | Description | Src |
|---|---|---|
| `Draw` | Occurs when the `DrawingSurface` has been invalidated. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `DrawingSurface` class. | SDK |
| `Invalidate` | Invalidates the `DrawingSurface` and requests a new `Draw` events. | SDK |

## System.Windows.Controls.Grid

Source: SDK

Defines a flexible grid area that consists of columns and rows.

### Properties

| Member | Description | Src |
|---|---|---|
| `Column` | Gets or sets a value that indicates which column child content within a `Grid` should appear in. <br>Returns: An `Int32` value that represents the column in which child content should appear. The default value is 0. | SDK |
| `ColumnDefinitions` | Gets a `ColumnDefinitionCollection` defined on this instance of `Grid`. <br>Returns: A `ColumnDefinitionCollection` defined on this instance of `Grid` | SDK |
| `ColumnSpan` | Gets or sets a value that indicates the total number of columns that child content spans within a `Grid`. <br>Returns: An `Int32` value that represents the number of columns that child content should span. The default value is 1. | SDK |
| `Row` | Gets or sets a value that indicates which row child content within a `Grid` should appear in. <br>Returns: An `Int32` value that represents the row in which child content should appear. The default value is 0. | SDK |
| `RowDefinitions` | Gets a `RowDefinitionCollection` defined on this instance of `Grid`. <br>Returns: A `RowDefinitionCollection` defined on this instance of `Grid`. | SDK |
| `RowSpan` | Gets or sets a value that indicates the total number of rows that child content spans within a `Grid`. <br>Returns: An `Int32` value that represents the number of rows that child content should span. The default value is 1. | SDK |
| `ShowGridLines` | Gets or sets a value that indicates whether grid lines are visible within this `Grid`. <br>Returns: true if grid lines are visible; otherwise, false. The default value is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ColumnProperty` | Identifies the `Column` dependency property. <br>Returns: The identifier for the `Column` dependency property. | SDK |
| `ColumnSpanProperty` | Identifies the `ColumnSpan` dependency property. <br>Returns: The identifier for the `ColumnSpan` dependency property. | SDK |
| `RowProperty` | Identifies the `Row` dependency property. <br>Returns: The identifier for the `Row` dependency property. | SDK |
| `RowSpanProperty` | Identifies the `RowSpan` dependency property. <br>Returns: The identifier for the `RowSpan` dependency property. | SDK |
| `ShowGridLinesProperty` | Identifies the `ShowGridLines` dependency property. <br>Returns: The identifier for the `ShowGridLines` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Grid` class. | SDK |
| `ArrangeOverride(Size)` | Arranges the content of a `Grid` element. <br>`arrangeSize`: Specifies the size this `Grid` element should use to arrange its child elements. <br>Returns: `Size` that represents the arranged size of this `Grid` element and its children. | SDK |
| `GetColumn(FrameworkElement)` | Gets the value of the `Column` attached property from the specified `FrameworkElement`. <br>`element`: The element from which to read the property value. <br>Returns: The value of the `Column` attached property. This is a zero-based index. | SDK |
| `GetColumnSpan(FrameworkElement)` | Gets the value of the `ColumnSpan` attached property from the specified `FrameworkElement`. <br>`element`: The element from which to read the property value. <br>Returns: The value of the `ColumnSpan` attached property. | SDK |
| `GetRow(FrameworkElement)` | Gets the value of the `Row` attached property from the specified `FrameworkElement`. <br>`element`: The element from which to read the property value. <br>Returns: The value of the `Row` attached property. | SDK |
| `GetRowSpan(FrameworkElement)` | Gets the value of the `RowSpan` attached property from the specified `FrameworkElement`. <br>`element`: The element from which to read the property value. <br>Returns: The value of the `RowSpan` attached property. | SDK |
| `MeasureOverride(Size)` | Measures the children of a `Grid` in anticipation of arranging them during the `Size)` pass. <br>`constraint`: Indicates an upper limit size that should not be exceeded. <br>Returns: `Size` that represents the maximum area available to the child content for layout. | SDK |
| `SetColumn(FrameworkElement,Int32)` | Sets the value of the `Column` attached property to the specified `FrameworkElement`. <br>`element`: The element on which to set the `Column` attached property. <br>`value`: The property value to set. | SDK |
| `SetColumnSpan(FrameworkElement,Int32)` | Sets the value of the `ColumnSpan` attached property to the specified `FrameworkElement`. <br>`element`: The element on which to set the `ColumnSpan` attached property. <br>`value`: The property value to set. | SDK |
| `SetRow(FrameworkElement,Int32)` | Sets the value of the `Row` attached property to the specified `FrameworkElement`. <br>`element`: The element on which to set the `Row` attached property. <br>`value`: The property value to set. | SDK |
| `SetRowSpan(FrameworkElement,Int32)` | Sets the value of the `RowSpan` attached property to the specified `FrameworkElement`. <br>`element`: The element on which to set the `RowSpan` attached property. <br>`value`: The property value to set. | SDK |

## System.Windows.Controls.HeaderedItemsControl

Source: Toolkit · class : ItemsControl · `Microsoft.Phone.Controls.Toolkit/HeaderedItemsControl/HeaderedItemsControl.cs`

Represents a control that contains a collection of items and a header.

### Properties

| Member | Description | Src |
|---|---|---|
| `object Header` | Gets or sets the item that labels the control. <br>Value: The item that labels the control. The default value is null. | TK |
| `DataTemplate HeaderTemplate` | Gets or sets a data template that is used to display the contents of the control's header. <br>Value: Gets or sets a data template that is used to display the contents of the control's header. The default is null. | TK |
| `Style ItemContainerStyle` | Gets or sets the `Style` that is applied to the container element generated for each item. <br>Value: The `Style` that is applied to the container element generated for each item. The default is null. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty HeaderProperty` | Identifies the `Header` dependency property. <br>Value: The identifier for the `Header` dependency property. | TK |
| `static DependencyProperty HeaderTemplateProperty` | Identifies the `HeaderTemplate` dependency property. <br>Value: The identifier for the `HeaderTemplate` dependency property. | TK |
| `static DependencyProperty ItemContainerStyleProperty` | Identifies the `ItemContainerStyle` dependency property. <br>Value: The identifier for the `ItemContainerStyle` dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | Builds the visual tree for the `HeaderedItemsControl` when a new template is applied. | TK |
| `«ctor» HeaderedItemsControl()` | Initializes a new instance of the `HeaderedItemsControl` class. | TK |

## System.Windows.Controls.HyperlinkButton

Source: SDK

Represents a button control that displays a hyperlink.

### Properties

| Member | Description | Src |
|---|---|---|
| `NavigateUri` | Gets or sets the URI to navigate to when the `HyperlinkButton` is clicked. <br>Returns: The URI to navigate to when the `HyperlinkButton` is clicked. | SDK |
| `TargetName` | Gets or sets the name of the target window or frame that the Web page should open in, or the name of the object within the Silverlight application to navigate to. <br>Returns: Gets or sets the name of the target window or frame that the Web page should open in, or the name of the object within the Silverlight application to navigate to. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `NavigateUriProperty` | Identifies the `NavigateUri` dependency property. <br>Returns: The identifier for the `NavigateUri` dependency property. | SDK |
| `TargetNameProperty` | Identifies the `TargetName` dependency property. <br>Returns: The identifier for the `TargetName` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `HyperlinkButton` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `HyperlinkButton` when a new template is applied. | SDK |
| `OnClick` | Provides handling for the `Click` event. | SDK |
| `OnCreateAutomationPeer` | Returns a `HyperlinkButtonAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `HyperlinkButtonAutomationPeer` for the hyperlink button object. | SDK |

## System.Windows.Controls.INavigate

Source: SDK

Defines a method for internal navigation in a Silverlight application.

### Methods

| Member | Description | Src |
|---|---|---|
| `Navigate(Uri)` | Displays the content located at the specified URI. <br>`source`: The URI of the content to display. <br>Returns: true if the content was successfully displayed; otherwise, false. | SDK |

## System.Windows.Controls.ISelectionAdapter

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/ISelectionAdapter.cs`

Defines an item collection, selection members, and key handling for the selection adapter contained in the drop-down portion of an `AutoCompleteBox` control.

## System.Windows.Controls.Image

Source: SDK

Represents a control that displays an image in the JPEG or PNG file formats.

### Properties

| Member | Description | Src |
|---|---|---|
| `Source` | Gets or sets the source for the image. <br>Returns: A source object for the drawn image. | SDK |
| `Stretch` | Gets or sets a value that describes how an `Image` should be stretched to fill the destination rectangle. <br>Returns: `Stretch`A value of the enumeration that specifies how the source image is applied if the `Height` and `Width` of the `Image` are specified and are different than the source image's height and width.The default value is Uniform. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. | SDK |
| `StretchProperty` | Identifies the `Stretch` dependency property. <br>Returns: The identifier for the `Stretch` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `ImageFailed` | Occurs when there is an error associated with image retrieval or format. | SDK |
| `ImageOpened` | Occurs when the image source is downloaded and decoded with no failure. You can use this event to determine the size of an image before rendering it. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Image` class. | SDK |

## System.Windows.Controls.InkPresenter

Source: SDK

Implements a rectangular surface that displays ink strokes.

### Properties

| Member | Description | Src |
|---|---|---|
| `Strokes` | Gets or sets the strokes that the `InkPresenter` displays. <br>Returns: The collection of ink strokes that are displayed by the `InkPresenter`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `StrokesProperty` | Identifies the `Strokes` dependency property. <br>Returns: The identifier for the `Strokes` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `InkPresenter` class. | SDK |

## System.Windows.Controls.ItemCollection

Source: SDK

Holds the list of items that represent the content of an `ItemsControl`.

### Events

| Member | Description | Src |
|---|---|---|
| `System#Collections#Specialized#INotifyCollectionChanged#CollectionChanged` | Occurs when the items list of the collection has changed, or the collection is reset. | SDK |

## System.Windows.Controls.ItemContainerGenerator

Source: SDK

Provides mappings between the items of an `ItemsControl` and their container elements.

### Events

| Member | Description | Src |
|---|---|---|
| `ItemsChanged` | Occurs when the contents of the items collection changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ContainerFromIndex(Int32)` | Returns the container for the item at the specified index within the `ItemCollection`. <br>`index`: The index of the item to retrieve. <br>Returns: The container for the item at the specified index within the item collection, if the item has a container; otherwise, null. | SDK |
| `ContainerFromItem(Object)` | Returns the container corresponding to the specified item. <br>`item`: The item to retrieve the container for. <br>Returns: A container that corresponds to the specified item, if the item has a container and exists in the collection; otherwise, null. | SDK |
| `GeneratorPositionFromIndex(Int32)` | Gets the generated position of the item at the specified index. <br>`itemIndex`: The index of the item to retrieve the position of. <br>Returns: The position of the item as generated by the `ItemContainerGenerator`. | SDK |
| `IndexFromContainer(DependencyObject)` | Returns the index to the item that has the specified, generated container. <br>`container`: The generated container to retrieve the item index for. <br>Returns: The index to the item that corresponds to the specified generated container. | SDK |
| `IndexFromGeneratorPosition(Primitives.GeneratorPosition)` | Returns the index that maps to the specified `GeneratorPosition`. <br>`position`: The `GeneratorPosition` for the desired index. <br>Returns: An `Int32` that is the index which maps to the specified `GeneratorPosition`. | SDK |
| `ItemFromContainer(DependencyObject)` | Returns the item that corresponds to the specified, generated container. <br>`container`: The `DependencyObject` that corresponds to the item to be returned. <br>Returns: The contained item, or the container if it does not contain an item. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#GenerateNext(Boolean@)` | Returns the container element used to display the next item, and whether the container element has been newly generated (realized). <br>`isNewlyRealized`: true if the returned `DependencyObject` is newly generated (realized); otherwise, false. <br>Returns: A `DependencyObject` that is the container element which is used to display the next item. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#GetItemContainerGeneratorForPanel(Panel)` | Returns the `ItemContainerGenerator` appropriate for use by the specified panel. <br>`panel`: The panel for which to return an appropriate `ItemContainerGenerator`. <br>Returns: An `ItemContainerGenerator` appropriate for use by the specified panel. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#PrepareItemContainer(DependencyObject)` | Prepares the specified element as the container for the corresponding item. <br>`container`: The container to prepare. Normally, `container` is the result of the previous call to `Boolean@)`. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#Remove(Primitives.GeneratorPosition,Int32)` | Removes one or more generated (realized) items. <br>`position`: The `Int32` index of the element to remove. `position` must refer to a previously generated (realized) item, which means its offset must be zero. <br>`count`: The `Int32` number of elements to remove, starting at `position`. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#RemoveAll` | Removes all generated (realized) items. | SDK |
| `System#Windows#Controls#Primitives#IItemContainerGenerator#StartAt(Primitives.GeneratorPosition,Primitives.GeneratorDirection,Boolean)` | Prepares the generator to generate items, starting at the specified `GeneratorPosition`, and in the specified `GeneratorDirection`, and controlling whether or not to start at a generated (realized) item. <br>`position`: A `GeneratorPosition`, that specifies the position of the item to start generating items at. <br>`direction`: Specifies the position of the item to start generating items at. <br>`allowStartAtRealizedItem`: A `Boolean` that specifies whether to start at a generated (realized) item. <br>Returns: An `IDisposable` object that tracks the lifetime of the generation process. | SDK |
| `System#Windows#Controls#Primitives#IRecyclingItemContainerGenerator#Recycle(Primitives.GeneratorPosition,Int32)` | Disassociates item containers from their data items and saves the containers so they can be reused later for other data items. <br>`position`: The zero-based index of the first element to reuse. must refer to a previously generated (realized) item. <br>`count`: The number of elements to reuse, starting at . | SDK |

## System.Windows.Controls.ItemsControl

Source: SDK

Represents a control that can be used to present a collection of items.

### Properties

| Member | Description | Src |
|---|---|---|
| `DisplayMemberPath` | Gets or sets the name or path of the property that is displayed for each data item. <br>Returns: The name or path of the property that is displayed for each the data item in the control. The default is an empty string (""). | SDK |
| `ItemContainerGenerator` | Gets the `ItemContainerGenerator` associated with this `ItemsControl`. <br>Returns: The `ItemContainerGenerator` associated with this `ItemsControl`. | SDK |
| `ItemTemplate` | Gets or sets the `DataTemplate` used to display each item. <br>Returns: The template that specifies the visualization of the data objects. The default is null. | SDK |
| `Items` | Gets the collection used to generate the content of the control. <br>Returns: The collection that is used to generate the content of the control, if it exists; otherwise, null. The default is an empty collection. | SDK |
| `ItemsPanel` | Gets or sets the template that defines the panel that controls the layout of items. <br>Returns: An `ItemsPanelTemplate` that defines the panel to use for the layout of the items. The default value for the `ItemsControl` is an `ItemsPanelTemplate` that specifies a `StackPanel`. | SDK |
| `ItemsSource` | Gets or sets a collection used to generate the content of the `ItemsControl`. <br>Returns: The object that is used to generate the content of the `ItemsControl`. The default is null. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `DisplayMemberPathProperty` | Identifies the `DisplayMemberPath` dependency property. <br>Returns: The identifier for the `DisplayMemberPath` dependency property. | SDK |
| `ItemTemplateProperty` | Identifies the `ItemTemplate` dependency property. <br>Returns: The identifier for the `ItemTemplate` dependency property. | SDK |
| `ItemsPanelProperty` | Identifies the `ItemsPanel` dependency property. <br>Returns: The identifier for the `ItemsPanel` dependency property. | SDK |
| `ItemsSourceProperty` | Identifies the `ItemsSource` dependency property. <br>Returns: The identifier for the `ItemsSource` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ItemsControl` class. | SDK |
| `ClearContainerForItemOverride(DependencyObject,Object)` | Undoes the effects of the `Object)` method. <br>`element`: The container element. <br>`item`: The item. | SDK |
| `GetContainerForItemOverride` | Creates or identifies the element that is used to display the given item. <br>Returns: The element that is used to display the given item. | SDK |
| `GetItemsOwner(DependencyObject)` | Returns the `ItemsControl` that the specified element hosts items for. <br>`element`: The host element. <br>Returns: The `ItemsControl` that the specified element hosts items for, or null. | SDK |
| `IsItemItsOwnContainerOverride(Object)` | Determines if the specified item is (or is eligible to be) its own container. <br>`item`: The item to check. <br>Returns: true if the item is (or is eligible to be) its own container; otherwise, false. | SDK |
| `ItemsControlFromItemContainer(DependencyObject)` | Returns the `ItemsControl` that owns the specified container element. <br>`container`: The container element to return the `ItemsControl` for. <br>Returns: The `ItemsControl` that owns the specified container element; otherwise, null. The `DependencyObject)` method returns null if `container` is not a `UIElement` or the parent is not an `ItemsControl`. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ItemsControl` when a new template is applied. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Called when the value of the `Items` property changes. <br>`e`: A `NotifyCollectionChangedEventArgs` that contains the event data | SDK |
| `OnTextInput(Input.TextCompositionEventArgs)` | Called before the `TextInput` event occurs. <br>`e`: Provides data about the event. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the specified element to display the specified item. <br>`element`: The element used to display the specified item. <br>`item`: The item to display. | SDK |

## System.Windows.Controls.ItemsPanelTemplate

Source: SDK

Specifies the panel that the `ItemsPresenter` creates for the layout of the items of an `ItemsControl`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ItemsPanelTemplate` class. | SDK |

## System.Windows.Controls.ItemsPresenter

Source: SDK

Specifies where items are placed in a control, usually an `ItemsControl`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ItemsPresenter` class. | SDK |

## System.Windows.Controls.LengthConverter

Source: Toolkit · class : TypeConverter · `Microsoft.Phone.Controls.Toolkit/Common/LengthConverter.cs`

Converts instances of other types to and from instances of a double that represent an object measurement such as a height or width.

### Methods

| Member | Description | Src |
|---|---|---|
| `bool CanConvertFrom(ITypeDescriptorContext typeDescriptorContext, Type sourceType)` | Determines whether conversion is possible from a specified type to a `Double` that represents an object measurement. <br>`typeDescriptorContext`: An `ITypeDescriptorContext` that provides a format context. <br>`sourceType`: A `Type` that represents the type you want to convert from. <br>Returns: True if this converter can perform the conversion; otherwise, false. | TK |
| `bool CanConvertTo(ITypeDescriptorContext typeDescriptorContext, Type destinationType)` | Returns whether the type converter can convert a measurement to the specified type. <br>`typeDescriptorContext`: An `ITypeDescriptorContext` that provides a format context. <br>`destinationType`: A `Type` that represents the type you want to convert to. <br>Returns: True if this converter can perform the conversion; otherwise, false. | TK |
| `object ConvertFrom(ITypeDescriptorContext typeDescriptorContext, CultureInfo cultureInfo, object source)` | Converts from the specified value to values of the `Double` type. <br>`typeDescriptorContext`: An `ITypeDescriptorContext` that provides a format context. <br>`cultureInfo`: The `CultureInfo` to use as the current culture. <br>`source`: The value to convert. <br>Returns: The converted value. | TK |
| `object ConvertTo(ITypeDescriptorContext typeDescriptorContext, CultureInfo cultureInfo, object value, Type destinationType)` | Converts the specified measurement to the specified type. <br>`typeDescriptorContext`: An object that provides a format context. <br>`cultureInfo`: The `CultureInfo` to use as the current culture. <br>`value`: The value to convert. <br>`destinationType`: A `Type` that represents the type you want to convert to. <br>Returns: The converted value. | TK |
| `«ctor» LengthConverter()` | Initializes a new instance of the `LengthConverter` class. | TK |

## System.Windows.Controls.ListBox

Source: SDK

Contains a list of selectable items.

### Properties

| Member | Description | Src |
|---|---|---|
| `ItemContainerStyle` | Gets or sets the style that is used when rendering the item containers. <br>Returns: The style applied to the item containers. The default is null. | SDK |
| `SelectedItems` | Gets the list of currently selected items for the `ListBox` control. <br>Returns: The list of currently selected items for the `ListBox`. | SDK |
| `SelectionMode` | Gets or sets the selection behavior for the `ListBox` control. <br>Returns: One of the `SelectionMode` values. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsSelectionActiveProperty` | Identifies the IsSelectionActive dependency property. <br>Returns: The identifier for the IsSelectionActive dependency property. | SDK |
| `ItemContainerStyleProperty` | Identifies the `ItemContainerStyle` dependency property. <br>Returns: The identifier for the `ItemContainerStyle` dependency property. | SDK |
| `SelectionModeProperty` | Identifies the `SelectionMode` dependency property. <br>Returns: The identifier for the `SelectionMode` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ListBox` class. | SDK |
| `GetContainerForItemOverride` | Creates or identifies the element used to display a specified item. <br>Returns: A `ListBoxItem` corresponding to a specified item. | SDK |
| `IsItemItsOwnContainerOverride(Object)` | Determines if the specified item is (or is eligible to be) its own item container. <br>`item`: The specified item. <br>Returns: true if the item is its own item container; otherwise, false. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ListBox` control when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns a `ListBoxAutomationPeer` for the Silverlight automation infrastructure. <br>Returns: A `ListBoxAutomationPeer` for the `ListBox` object. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Provides handling for the `GotFocus` event. <br>`e`: The event data. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Provides handling for the `ItemsChanged` event. <br>`e`: A `NotifyCollectionChangedEventArgs` that contains the event data. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Provides handling for the `ZeyDown` event that occurs when a key is pressed while the control has focus. <br>`e`: The event data. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Provides handling for the `LostFocus` event. <br>`e`: The event data. | SDK |
| `ScrollIntoView(Object)` | Causes the object to scroll into view. <br>`item`: The object to scroll. | SDK |
| `SelectAll` | Selects all the items in the `ListBox`. | SDK |

## System.Windows.Controls.ListBoxItem

Source: SDK

Represents a selectable item in a `ListBox`.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsSelected` | Gets or sets a value that indicates whether a `ListBoxItem` is selected. <br>Returns: true if the item is selected; otherwise, false. The default is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsSelectedProperty` | Identifies the `IsSelected` dependency property. <br>Returns: The identifier for the `IsSelected` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ListBoxItem` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ListBoxItem` control when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns a `ListBoxItemAutomationPeer` for the Silverlight automation infrastructure. <br>Returns: A `ListBoxItemAutomationPeer` for the `ListBoxItem`. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Provides handling for the `GotFocus` event. <br>`e`: The event data. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Provides handling for the `LostFocus` event. <br>`e`: The event data. | SDK |
| `OnManipulationCompleted(Input.ManipulationCompletedEventArgs)` | Called when the `ManipulationCompleted` event occurs. This member overrides `ManipulationCompletedEventArgs)`. <br>`e`: Event data for the event. | SDK |
| `OnManipulationStarted(Input.ManipulationStartedEventArgs)` | Called when the `ManipulationStarted` event occurs. This member overrides `ManipulationStartedEventArgs)`. <br>`e`: Event data for the event. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Provides handling for the `MouseEnter` event. <br>`e`: The event data. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Provides handling for the `MouseLeave` event. <br>`e`: The event data. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Provides handling for the `MouseLeftButtonDown` event. <br>`e`: The event data. | SDK |

## System.Windows.Controls.ManipulationMode

Source: SDK

Describes the manipulation mode used for `ScrollViewer` behavior.

### Fields

| Member | Description | Src |
|---|---|---|
| `Control` | Manipulation behavior for scrolling scrolls content in the control. | SDK |
| `System` | Manipulation behavior for scrolling should fall through to the system. | SDK |

## System.Windows.Controls.MediaElement

Source: SDK

Represents an object that contains audio, video, or both.

### Properties

| Member | Description | Src |
|---|---|---|
| `Attributes` | Gets the collection of attributes that corresponds to the current entry in the ASX file that `Source` is set to. <br>Returns: The collection of attributes that corresponds to the current entry in the ASX file that `Source` is set to. | SDK |
| `AudioStreamCount` | Gets the number of audio streams available in the current media file. <br>Returns: The number of audio streams that exist in the source media file. The default value is 0. | SDK |
| `AudioStreamIndex` | Gets or sets the index of the audio stream that plays along with the video component. The collection of audio streams is composed at run time and represents all audio streams available within the media file. <br>Returns: The index within the media file of the audio component that plays along with the video component. The index can be unspecified, in which case the value is null. The default value is null (see the "Remarks" section). | SDK |
| `AutoPlay` | Gets or sets a value that indicates whether media will begin playback automatically when the `Source` property is set. <br>Returns: true if playback is automatic; otherwise, false. The default value is true. | SDK |
| `Balance` | Gets or sets a ratio of volume across stereo speakers. <br>Returns: The ratio of volume across speakers in the range between -1 and 1. The default value is 0. | SDK |
| `BufferingProgress` | Gets a value that indicates the current buffering progress. <br>Returns: The amount of buffering that is completed for media content. The value ranges from 0 to 1. Multiply by 100 to obtain a percentage. The default value is 0. | SDK |
| `BufferingTime` | Gets or sets the amount of time to buffer. <br>Returns: The amount of time to buffer. The default value is a `TimeSpan` with value of 5 seconds (0:0:05). | SDK |
| `CanPause` | Gets a value indicating if media can be paused if the `Pause` method is called. <br>Returns: true if the media can be paused; otherwise, false. The default is false. | SDK |
| `CanSeek` | Gets a value indicating if media can be repositioned by setting the value of the `Position` property. <br>Returns: true if the media can be repositioned; otherwise, false.The default value is false. | SDK |
| `CurrentState` | Gets the status of the `MediaElement`. <br>Returns: The current state of the `MediaElement`. The state can be one of the following (as defined in the `MediaElementState` enumeration): `Buffering`, `Closed`, `Opening`, `Paused`, `Playing`, or `Stopped`.The default value is `Closed`. | SDK |
| `DownloadProgress` | Gets a percentage value indicating the amount of download completed for content located on a remote server. <br>Returns: A value that indicates the amount of download completed for content that is located on a remote server. The value ranges from 0 to 1. Multiply by 100 to obtain a percentage. The default value is 0. | SDK |
| `DownloadProgressOffset` | Gets the offset of the download progress. <br>Returns: The offset of the download progress. | SDK |
| `DroppedFramesPerSecond` | Gets the number of frames per second being dropped by the media. <br>Returns: The number of frames per second being dropped by the media. | SDK |
| `IsMuted` | Gets or sets a value indicating whether the audio is muted. <br>Returns: true if audio is muted; otherwise, false. The default is false. | SDK |
| `LicenseAcquirer` | Gets or sets the `LicenseAcquirer` associated with the `MediaElement`. The `LicenseAcquirer` handles acquiring licenses for DRM encrypted content. <br>Returns: The `LicenseAcquirer` associated with the `MediaElement`. The default is null. | SDK |
| `Markers` | Gets the collection of timeline markers associated with the currently loaded media file. <br>Returns: The collection of timeline markers (represented as `TimelineMarker` objects) associated with the currently loaded media file. The default value is an empty collection. | SDK |
| `NaturalDuration` | Gets the duration of the media file currently opened. <br>Returns: The natural duration of the media. The default value is `Automatic`, which is the value held if you query this property before `MediaOpened`. | SDK |
| `NaturalVideoHeight` | Gets the height of the video associated with the media. <br>Returns: The height of the video that is associated with the media, in pixels. Audio files will return 0. The default value is 0. | SDK |
| `NaturalVideoWidth` | Gets the width of the video associated with the media. <br>Returns: The width of the video associated with the media. The default value is 0. | SDK |
| `PlaybackRate` | Gets or sets the playback rate of the media. <br>Returns: The playback rate. | SDK |
| `Position` | Gets or sets the current position of progress through the media's playback time. <br>Returns: The amount of time since the beginning of the media. The default is a `TimeSpan` with value 0:0:0. | SDK |
| `RenderedFramesPerSecond` | Gets the number of frames per second being rendered by the media. <br>Returns: The number of frames per second being rendered by the media. | SDK |
| `Source` | Gets or sets a media source on the `MediaElement`. <br>Returns: A string that specifies the source of the element, as a Uniform Resource Identifier (URI). The default value is null. | SDK |
| `Stretch` | Gets or sets a `Stretch` value that describes how a `MediaElement` fills the destination rectangle. <br>Returns: A value of the enumeration that specifies the stretch behavior for the rendered media. The default value is Uniform. | SDK |
| `Volume` | Gets or sets the media's volume. <br>Returns: The media's volume represented on a linear scale between 0 and 1. The default is 0.5. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AttributesProperty` | Identifies the `Attributes` dependency property. <br>Returns: The identifier for the `Attributes` dependency property. | SDK |
| `AudioStreamCountProperty` | Identifies the `AudioStreamCount` dependency property. <br>Returns: The identifier for the `AudioStreamCount` dependency property. | SDK |
| `AudioStreamIndexProperty` | Identifies the `AudioStreamIndex` dependency property. <br>Returns: The identifier for the `AudioStreamIndex` dependency property. | SDK |
| `AutoPlayProperty` | Identifies the `AutoPlay` dependency property. <br>Returns: The identifier for the `AutoPlay` dependency property. | SDK |
| `BalanceProperty` | Identifies the `Balance` dependency property. <br>Returns: The identifier for the `Balance` dependency property. | SDK |
| `BufferingProgressProperty` | Identifies the `BufferingProgress` dependency property. <br>Returns: The identifier for the `BufferingProgress` dependency property. | SDK |
| `BufferingTimeProperty` | Identifies the `BufferingTime` dependency property. <br>Returns: The identifier for the `BufferingTime` dependency property. | SDK |
| `CanPauseProperty` | Identifies the `CanPause` dependency property. <br>Returns: The identifier for the `CanPause` dependency property. | SDK |
| `CanSeekProperty` | Identifies the `CanSeek` dependency property. <br>Returns: The identifier for the `CanSeek` dependency property. | SDK |
| `CurrentStateProperty` | Identifies the `CurrentState` dependency property. <br>Returns: The identifier for the `CurrentState` dependency property. | SDK |
| `DownloadProgressOffsetProperty` | Identifies the `DownloadProgressOffset` dependency property. <br>Returns: The identifier for the `DownloadProgressOffset` dependency property. | SDK |
| `DownloadProgressProperty` | Identifies the `DownloadProgress` dependency property. <br>Returns: The identifier for the `DownloadProgress` dependency property. | SDK |
| `DroppedFramesPerSecondProperty` | Identifies the `DroppedFramesPerSecond` dependency property. <br>Returns: The identifier for the `DroppedFramesPerSecond` dependency property. | SDK |
| `IsMutedProperty` | Identifies the `IsMuted` dependency property. <br>Returns: The identifier for the `IsMuted` dependency property. | SDK |
| `NaturalDurationProperty` | Identifies the `NaturalDuration` dependency property. <br>Returns: The identifier for the `NaturalDuration` dependency property. | SDK |
| `NaturalVideoHeightProperty` | Identifies the `NaturalVideoHeight` dependency property. <br>Returns: The identifier for the `NaturalVideoHeight` dependency property. | SDK |
| `NaturalVideoWidthProperty` | Identifies the `NaturalVideoWidth` dependency property. <br>Returns: The identifier for the `NaturalVideoWidth` dependency property. | SDK |
| `PlaybackRateProperty` | Identifies the `PlaybackRate` property. | SDK |
| `PositionProperty` | Identifies the `Position` dependency property. <br>Returns: The identifier the `Position` dependency property. | SDK |
| `RenderedFramesPerSecondProperty` | Identifies the `RenderedFramesPerSecond` dependency property. <br>Returns: The identifier for the `RenderedFramesPerSecond` dependency property. | SDK |
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. | SDK |
| `StretchProperty` | Identifies the `Stretch` dependency property. <br>Returns: The identifier for the `Stretch` dependency property. | SDK |
| `VolumeProperty` | Identifies the `Volume` dependency property. <br>Returns: The identifier for the `Volume` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `BufferingProgressChanged` | Occurs when the `BufferingProgress` property changes. | SDK |
| `CurrentStateChanged` | Occurs when the value of the `CurrentState` property changes. | SDK |
| `DownloadProgressChanged` | Occurs when the `DownloadProgress` property has changed. | SDK |
| `LogReady` | Occurs when the log is ready. | SDK |
| `MarkerReached` | Occurs when a timeline marker is encountered during media playback. | SDK |
| `MediaEnded` | Occurs when the `MediaElement` is no longer playing audio or video. | SDK |
| `MediaFailed` | Occurs when there is an error associated with the media `Source`. | SDK |
| `MediaOpened` | Occurs when the media stream has been validated and opened, and the file headers have been read. | SDK |
| `RateChanged` | Occurs when the `PlaybackRate` property changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Instantiates a new instance of the `MediaElement` class. | SDK |
| `Pause` | Pauses media at the current position. | SDK |
| `Play` | Plays media from the current position. | SDK |
| `RequestLog` | Sends a request to generate a log which will then be raised through the `LogReady` event. | SDK |
| `SetSource(IO.Stream)` | Sets the `Source` property using the supplied stream. <br>`stream`: A stream that contains a natively supported media source. | SDK |
| `SetSource(Media.MediaStreamSource)` | This sets the source of a `MediaElement` to a subclass of `MediaStreamSource`. <br>`mediaStreamSource`: A subclass of `MediaStreamSource`. | SDK |
| `Stop` | Stops and resets media to be played from the beginning. | SDK |

## System.Windows.Controls.MultiScaleImage

Source: SDK

Enables users to open a multi-resolution image, which can be zoomed in on and panned across.

### Properties

| Member | Description | Src |
|---|---|---|
| `AllowDownloading` | Gets or sets a value that indicates whether downloading is permitted by this `MultiScaleImage`. <br>Returns: true if downloading is permitted by this `MultiScaleImage`. false if downloading is not permitted by this `MultiScaleImage`. | SDK |
| `AspectRatio` | Gets the aspect ratio of the image used as the source of the `MultiScaleImage`. The aspect ratio is the width of the image divided by its height. <br>Returns: The aspect ratio of the image used as the source of the `MultiScaleImage`. The aspect ratio is the width of the image divided by its height. | SDK |
| `BlurFactor` | Gets or sets the extent that data is blurred while rendering. <br>Returns: The extent that data is blurred while rendering. A value of 2 means that data is twice as blurry (one level lower), while a value of 0.5 means that data is sharper (one level higher). The default is 1. | SDK |
| `IsDownloading` | Gets a value that indicates whether the image is still downloading. <br>Returns: true if the image is still downloading. false if all the needed tiles have been downloaded. If the image is moved, `IsDownloading` may become true again. | SDK |
| `IsIdle` | Gets a value that indicates whether Deep Zoom is done downloading, decoding, blending, and animating (if springs are being used) images. <br>Returns: true, if Deep Zoom is done downloading, decoding, blending, and animating (if springs are being used) images. Otherwise, false. | SDK |
| `SkipLevels` | Gets or sets a value that indicates levels to be skipped while loading a `MultiScaleImage`. <br>Returns: An integer that represents the levels that are skipped while loading a `MultiScaleImage`. | SDK |
| `Source` | Gets or sets the `MultiScaleTileSource` object that is used as the source for the `MultiScaleImage`. <br>Returns: The `MultiScaleTileSource` object that is used as the source for the `MultiScaleImage`. | SDK |
| `SubImages` | Gets the collection of `MultiScaleSubImage` objects within the multiresolution image that is used by the `MultiScaleImage`. <br>Returns: The collection of `MultiScaleSubImage` objects within the multiresolution image that is used by the `MultiScaleImage`. | SDK |
| `UseSprings` | Gets or sets a value that indicates whether the `MultiScaleImage` uses spring animations. <br>Returns: true if the `MultiScaleImage` uses spring animations; otherwise, false. The default value is true. | SDK |
| `ViewportOrigin` | Gets or sets the top-left corner of the area of the image to be displayed. <br>Returns: The top-left corner of the rectangular area of the image to be displayed. | SDK |
| `ViewportWidth` | Gets or sets the width of the area of the image displayed. <br>Returns: The width of the area of the image displayed. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AllowDownloadingProperty` | Identifies the `AllowDownloading` dependency property. <br>Returns: The identifier for the `AllowDownloading` dependency property. | SDK |
| `AspectRatioProperty` | Identifies the `AspectRatio` dependency property. <br>Returns: The identifier for the `AspectRatio` dependency property. | SDK |
| `BlurFactorProperty` | Identifies the `BlurFactor` dependency property. <br>Returns: The identifier for the `BlurFactor` dependency property. | SDK |
| `IsDownloadingProperty` | Identifies the `IsDownloading` dependency property. <br>Returns: The identifier for the `IsDownloading` dependency property. | SDK |
| `IsIdleProperty` | Identifies the `IsIdle` dependency property. <br>Returns: The identifier for the `IsIdle` dependency property. | SDK |
| `SkipLevelsProperty` | Identifies the `SkipLevels` dependency property. <br>Returns: The identifier for the `SkipLevels` dependency property. | SDK |
| `SourceProperty` | Identifies the `Source` dependency property. <br>Returns: The identifier for the `Source` dependency property. | SDK |
| `SubImagesProperty` | Identifies the `SubImages` dependency property. <br>Returns: The identifier for the `SubImages` dependency property. | SDK |
| `UseSpringsProperty` | Identifies the `UseSprings` dependency property. <br>Returns: The identifier for the `UseSprings` dependency property. | SDK |
| `ViewportOriginProperty` | Identifies the `ViewportOrigin` dependency property. <br>Returns: The identifier for the `ViewportOrigin` dependency property. | SDK |
| `ViewportWidthProperty` | Identifies the `ViewportWidth` dependency property. <br>Returns: The identifier for the `ViewportWidth` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `ImageFailed` | Occurs if the download of a tile times out or fails for another reason. | SDK |
| `ImageOpenFailed` | Occurs if the first piece of metadata used to open the image fails. If this event occurs no parts of the image will open successfully. | SDK |
| `ImageOpenSucceeded` | Occurs when the first piece of metadata that is needed to load the rest of the tiles opens. | SDK |
| `MotionFinished` | Occurs when the zoom or pan animation ends. | SDK |
| `ViewportChanged` | Occurs when the viewport (the area of the image displayed) changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MultiScaleImage` class. | SDK |
| `ElementToLogicalPoint(Point)` | Gets a point with logical coordinates (values between 0 and 1) from a point of the `MultiScaleImage`. <br>`elementPoint`: The point on the `MultiScaleImage` to translate into a point with logical coordinates (values between 0 and 1). <br>Returns: The logical point translated from the . | SDK |
| `LogicalToElementPoint(Point)` | Gets a point with pixel coordinates relative to the `MultiScaleImage` from a logical point (values between 0 and 1). <br>`logicalPoint`: The logical point to translate into pixel coordinates relative to the `MultiScaleImage`. <br>Returns: A point with pixel coordinates relative to the `MultiScaleImage` translated from `logicalPoint`. | SDK |
| `ZoomAboutLogicalPoint(Double,Double,Double)` | Enables a user to zoom in on a point of the `MultiScaleImage`. <br>`zoomIncrementFactor`: Specifies the zoom. This number is greater than 0. A value of 1 specifies that the image fit the allotted page size exactly. A number greater than 1 specifies to zoom in. If a value of 0 or less is used, failure is returned and no zoom changes are applied. <br>`zoomCenterLogicalX`: X coordinate for the point on the `MultiScaleImage` that is zoomed in on. This is a logical point (between 0 and 1). <br>`zoomCenterLogicalY`: Y coordinate for the point on the `MultiScaleImage` that is zoomed in on. This is a logical point (between 0 and 1). | SDK |

## System.Windows.Controls.MultiScaleSubImage

Source: SDK

This class holds the properties for each sub-image within the `MultiScaleImage`.

### Properties

| Member | Description | Src |
|---|---|---|
| `AspectRatio` | Gets the aspect ratio of the image used as the source of the `MultiScaleSubImage`. The aspect ratio is the width of the image divided by its height. <br>Returns: The aspect ratio of the image used as the source of the `MultiScaleSubImage`. The aspect ratio is the width of the image divided by its height. | SDK |
| `Opacity` | Gets or sets the degree of the `MultiScaleSubImage` opacity. <br>Returns: A value between 0 and 1.0 that declares the opacity, with 1.0 meaning full opacity and 0 meaning transparent. The default value is 1.0. | SDK |
| `ViewportOrigin` | Gets or sets the top-left corner of the area of the image to be displayed. <br>Returns: The point of the top-left corner of the rectangular area of the image to be displayed. | SDK |
| `ViewportWidth` | Gets or sets the width of the area of the image displayed. <br>Returns: The width of the area of the image displayed. | SDK |
| `ZIndex` | Gets or sets a value that represents the z-order rendering behavior of the `MultiScaleSubImage`. Z-order determines the relative rendering order of objects (which object is on top of which other objects). <br>Returns: The value that represents the z-order rendering behavior of the `MultiScaleSubImage`. The default value is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AspectRatioProperty` | Identifies the `AspectRatio` dependency property. <br>Returns: The identifier for the `AspectRatio` dependency property. | SDK |
| `OpacityProperty` | Identifies the `Opacity` dependency property. <br>Returns: The identifier for the `Opacity` dependency property. | SDK |
| `ViewportOriginProperty` | Identifies the `ViewportOrigin` dependency property. <br>Returns: The identifier for the `ViewportOrigin` dependency property. | SDK |
| `ViewportWidthProperty` | Identifies the `ViewportWidth` dependency property. <br>Returns: The identifier for the `ViewportWidth` dependency property. | SDK |
| `ZIndexProperty` | Identifies the `ZIndex` dependency property. <br>Returns: The identifier for the `ZIndex` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MultiScaleSubImage` class. | SDK |

## System.Windows.Controls.NotifyEventArgs

Source: SDK

Provides data for the `ScriptNotify` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Value` | Gets or sets the JavaScript method name that is passed to the Silverlight plug-in. <br>Returns: The JavaScript method name that is passed to the Silverlight plug-in. | SDK |

## System.Windows.Controls.OpenFileDialog

Source: SDK

Provides a dialog box that enables the user to select one or more files.

### Properties

| Member | Description | Src |
|---|---|---|
| `File` | Gets a `FileInfo` object for the selected file. If multiple files are selected, returns the first selected file. <br>Returns: The selected file. If multiple files are selected, returns the first selected file. | SDK |
| `Files` | Gets a collection of `FileInfo` objects for the selected files. <br>Returns: A collection of `FileInfo` objects for the selected files. | SDK |
| `Filter` | Gets or sets a filter string that specifies the file types and descriptions to display in the `OpenFileDialog`. <br>Returns: A filter string that specifies the file types and descriptions to display in the `OpenFileDialog`. The default is `Empty`. | SDK |
| `FilterIndex` | Gets or sets the index of the selected item in the `OpenFileDialog` filter drop-down list. <br>Returns: The index of the selected item in the `OpenFileDialog` filter drop-down list. The default is 1. | SDK |
| `Multiselect` | Gets or sets a value that indicates whether the `OpenFileDialog` allows users to select multiple files. <br>Returns: true if multiple selections are allowed; otherwise, false. The default is false. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `OpenFileDialog` class. | SDK |
| `ShowDialog` | Displays an `OpenFileDialog` that is modal to the Web browser window. <br>Returns: true if the user clicked OK; false if the user clicked Cancel or closed the dialog box. | SDK |

## System.Windows.Controls.Orientation

Source: SDK

Defines the different orientations that a control or layout can have.

### Fields

| Member | Description | Src |
|---|---|---|
| `Horizontal` | The control or layout should be horizontally oriented. | SDK |
| `Vertical` | The control or layout should be vertically oriented. | SDK |

## System.Windows.Controls.Panel

Source: SDK

Provides a base class for all `Panel` elements. Use `Panel` elements to position and arrange child objects in Silverlight-based applications.

### Properties

| Member | Description | Src |
|---|---|---|
| `Background` | Gets or sets a `Brush` that is used to fill the panel. <br>Returns: The brush used to fill the panel. The default is null. | SDK |
| `Children` | Gets the collection of child elements of the panel. <br>Returns: The collection of child objects. The default is an empty collection. | SDK |
| `IsItemsHost` | Gets a value that indicates whether this `Panel` is a container for UI items that are generated by an `ItemsControl`. <br>Returns: true if this instance of `Panel` is an items host; otherwise, false. The default value is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BackgroundProperty` | Identifies the `Background` dependency property. <br>Returns: The identifier for the `Background` dependency property. | SDK |
| `IsItemsHostProperty` | Identifies the `IsItemsHost` dependency property. <br>Returns: The identifier for the `IsItemsHost` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Panel` class. | SDK |

## System.Windows.Controls.PasswordBox

Source: SDK

Represents a control for entering passwords.

### Properties

| Member | Description | Src |
|---|---|---|
| `BaselineOffset` | Gets a value by which each line of text is offset from a baseline. <br>Returns: The amount by which each line of text is offset from the baseline, in device independent pixels. `NaN` indicates that an optimal baseline offset is automatically calculated from the current font characteristics. The default is `NaN`. | SDK |
| `CaretBrush` | Gets or sets the brush that is used to render the vertical bar that indicates the insertion point. <br>Returns: The brush that is used to render the vertical bar that indicates the insertion point. | SDK |
| `FontSource` | Gets or sets the font source that is applied to the password box for rendering content. <br>Returns: The font source used to render content in the text box. The default is null. | SDK |
| `MaxLength` | Gets or sets the maximum length for passwords to be handled by this `PasswordBox`. <br>Returns: An integer specifying the maximum length, in character, for passwords to be handled by this `PasswordBox`. A value of zero (0) means no limit.The default value is 0 (no length limit). | SDK |
| `Password` | Gets or sets the password currently held by the `PasswordBox`. <br>Returns: A string representing the password currently held by the `PasswordBox`.The default value is `Empty`. | SDK |
| `PasswordChar` | Gets or sets the masking character for the `PasswordBox`. <br>Returns: A masking character to echo when the user enters text into the `PasswordBox`. The default value is a bullet character (●). | SDK |
| `SelectionBackground` | Gets or sets the brush used to render the background for the selected text. <br>Returns: The brush that fills the background of the selected text. | SDK |
| `SelectionForeground` | Gets or sets the brush used for the selected text in the `PasswordBox`. <br>Returns: The brush used for the selected text in the `PasswordBox`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `CaretBrushProperty` | Identifies the `CaretBrush` dependency property. <br>Returns: The identifier for the `CaretBrush` dependency property. | SDK |
| `MaxLengthProperty` | Identifies the `MaxLength` dependency property. <br>Returns: The identifier for the `MaxLength` dependency property. | SDK |
| `PasswordCharProperty` | Identifies the `PasswordChar` dependency property. <br>Returns: The identifier for the `PasswordChar` dependency property. | SDK |
| `PasswordProperty` | Identifies the `Password` dependency property. <br>Returns: The identifier for the `Password` dependency property. | SDK |
| `SelectionBackgroundProperty` | Identifies the `SelectionBackground` dependency property. <br>Returns: The identifier for the `SelectionBackground` dependency property. | SDK |
| `SelectionForegroundProperty` | Identifies the `SelectionForeground` dependency property. <br>Returns: The identifier for the `SelectionForeground` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `PasswordChanged` | Occurs when the value of the `Password` property changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `PasswordBox` class | SDK |
| `SelectAll` | Selects all the character in the `PasswordBox`. | SDK |

## System.Windows.Controls.PopulatedEventArgs

Source: Toolkit · class : RoutedEventArgs · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/PopulatedEventArgs.cs`

Provides data for the `Populated` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `IEnumerable Data` | Gets the list of possible matches added to the drop-down portion of the `AutoCompleteBox` control. <br>Value: The list of possible matches added to the `AutoCompleteBox`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» PopulatedEventArgs(IEnumerable data)` | Initializes a new instance of the `PopulatedEventArgs`. <br>`data`: The list of possible matches added to the drop-down portion of the `AutoCompleteBox` control. | TK |

## System.Windows.Controls.PopulatingEventArgs

Source: Toolkit · class : RoutedEventArgs · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/PopulatingEventArgs.cs`

Provides data for the `Populating` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool Cancel` | Gets or sets a value indicating whether the `Populating` event should be canceled. <br>Value: True to cancel the event, otherwise false. The default is false. | TK |
| `string Parameter` | Gets the text that is used to determine which items to display in the `AutoCompleteBox` control. <br>Value: The text that is used to determine which items to display in the `AutoCompleteBox`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» PopulatingEventArgs(string parameter)` | Initializes a new instance of the `PopulatingEventArgs`. <br>`parameter`: The value of the `SearchText` property, which is used to filter items for the `AutoCompleteBox` control. | TK |

## System.Windows.Controls.Primitives.ButtonBase

Source: SDK

Represents the base class for all button controls, such as `Button`, `RepeatButton`, and `HyperlinkButton`.

### Properties

| Member | Description | Src |
|---|---|---|
| `ClickMode` | Gets or sets when the `Click` event occurs. <br>Returns: When the `Click` event occurs. The default value is `Release`. | SDK |
| `Command` | Gets or sets the command to invoke when this button is pressed. <br>Returns: The command to invoke when this button is pressed. The default is null. | SDK |
| `CommandParameter` | Gets or sets the parameter to pass to the `Command` property. <br>Returns: The parameter to pass to the `Command` property. The default is null. | SDK |
| `IsFocused` | Gets a value that determines whether the button has focus. <br>Returns: true if the control has focus; otherwise, false. The default is false. | SDK |
| `IsMouseOver` | Gets a value indicating whether the mouse pointer is located over this button control. <br>Returns: true to indicate the mouse pointer is over the button control, otherwise false. The default is false. | SDK |
| `IsPressed` | Gets a value that indicates whether a `ButtonBase` is currently in a pressed state. <br>Returns: true if the `ButtonBase` is in a pressed state; otherwise false. The default is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ClickModeProperty` | Identifies the `ClickMode` dependency property <br>Returns: The identifier for the `ClickMode` dependency property. | SDK |
| `CommandParameterProperty` | Identifier for the `CommandParameter` dependency property. <br>Returns: The identifier for the `CommandParameter` dependency property. | SDK |
| `CommandProperty` | Identifier for the `Command` dependency property. <br>Returns: The identifier for the `Command` dependency property. | SDK |
| `IsFocusedProperty` | Identifies the `IsFocused` dependency property. <br>Returns: The identifier for the `IsFocused` dependency property. | SDK |
| `IsMouseOverProperty` | Identifies the `IsMouseOver` dependency property. <br>Returns: The identifier for the `IsMouseOver` dependency property. | SDK |
| `IsPressedProperty` | Identifies the `IsPressed` dependency property. <br>Returns: The identifier for the `IsPressed` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Click` | Occurs when a `Button` is clicked. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ButtonBase` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `Slider` control when a new template is applied. | SDK |
| `OnClick` | Raises the `Click` event. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Provides handling for the `GotFocus` event. <br>`e`: The event data. | SDK |
| `OnIsPressedChanged(DependencyPropertyChangedEventArgs)` | Called when the value of the `IsPressed` property changes. <br>`e`: The data for `DependencyPropertyChangedEventArgs`. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Provides class handling for the `ZeyDown` event that occurs when the user presses a key while this control has focus. <br>`e`: The event data. | SDK |
| `OnKeyUp(Input.ZeyEventArgs)` | Provides class handling for the `ZeyUp` routed event that occurs when the user releases a key while this control has focus. <br>`e`: The event data. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Provides handling for the `LostFocus` event. <br>`e`: The event data for the `LostFocus` event. | SDK |
| `OnLostMouseCapture(Input.MouseEventArgs)` | Provides handling for the `LostMouseCapture` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Provides class handling for the `MouseEnter` event that occurs when the mouse enters this control. <br>`e`: The event data. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Provides class handling for the `MouseLeave` routed event that occurs when the mouse leaves an element. <br>`e`: The event data for the `MouseLeave` event. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Provides class handling for the `MouseLeftButtonDown` event that occurs when the left mouse button is pressed while the mouse pointer is over this control. <br>`e`: The event data. | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Provides class handling for the `MouseLeftButtonUp` event that occurs when the left mouse button is released while the mouse pointer is over this control. <br>`e`: The event data. | SDK |
| `OnMouseMove(Input.MouseEventArgs)` | Provides class handling for the `MouseMove` event that occurs when the mouse pointer moves while over this element. <br>`e`: The event data. | SDK |

## System.Windows.Controls.Primitives.DragCompletedEventArgs

Source: SDK

Provides data for the `DragCompleted` event that occurs when a user completes a drag operation with the mouse of a `Thumb` control.

### Properties

| Member | Description | Src |
|---|---|---|
| `Canceled` | Gets a value that indicates whether the drag operation was canceled. <br>Returns: true if the drag operation was canceled; otherwise, false. | SDK |
| `HorizontalChange` | Gets the horizontal distance between the current mouse position and the thumb coordinates. <br>Returns: The horizontal distance between the current mouse position and the thumb coordinates. | SDK |
| `VerticalChange` | Gets the vertical distance between the current mouse position and the thumb coordinates. <br>Returns: The vertical distance between the current mouse position and the thumb coordinates. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Double,Double,Boolean)` | Initializes a new instance of the `DragCompletedEventArgs` class. <br>`horizontalChange`: The horizontal change in position of the `Thumb` control, resulting from the drag operation. <br>`verticalChange`: The vertical change in position of the `Thumb` control, resulting from the drag operation. <br>`canceled`: A value that indicates whether the drag operation was canceled by a call to the `CancelDrag` method. | SDK |

## System.Windows.Controls.Primitives.DragCompletedEventHandler

Source: SDK

Represents the method that will handle the `DragCompleted` event of a `Thumb`.

## System.Windows.Controls.Primitives.DragDeltaEventArgs

Source: SDK

Provides data for the `DragDelta` event that occurs one or more times when a user drags a `Thumb` control with the mouse..

### Properties

| Member | Description | Src |
|---|---|---|
| `HorizontalChange` | Gets the horizontal change in the `Thumb` position since the last `DragDelta` event. <br>Returns: The horizontal change in the `Thumb` position since the last `DragDelta` event. | SDK |
| `VerticalChange` | Gets the vertical change in the `Thumb` position since the last `DragDelta` event. <br>Returns: The vertical change in the `Thumb` position since the last `DragDelta` event. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Double,Double)` | Initializes a new instance of the `DragDeltaEventArgs` class. <br>`horizontalChange`: The horizontal change in the `Thumb` position since the last `DragDelta` event. <br>`verticalChange`: The vertical change in the `Thumb` position since the last `DragDelta` event. | SDK |

## System.Windows.Controls.Primitives.DragDeltaEventHandler

Source: SDK

Represents the method that will handle the `DragDelta` event of a `Thumb`.

## System.Windows.Controls.Primitives.DragStartedEventArgs

Source: SDK

Provides data for the `DragStarted` event that occurs when a user drags a `Thumb` control with the mouse.

### Properties

| Member | Description | Src |
|---|---|---|
| `HorizontalOffset` | Gets the horizontal distance between the current mouse position and the thumb coordinates. <br>Returns: The horizontal distance between the current mouse position and the thumb coordinates. | SDK |
| `VerticalOffset` | Gets the vertical distance between the current mouse position and the thumb coordinates. <br>Returns: The vertical distance between the current mouse position and the thumb coordinates. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Double,Double)` | Initializes a new instance of the `DragStartedEventArgs` class. <br>`horizontalOffset`: The horizontal distance between the current mouse position and the thumb coordinates. <br>`verticalOffset`: The vertical distance between the current mouse position and the thumb coordinates. | SDK |

## System.Windows.Controls.Primitives.DragStartedEventHandler

Source: SDK

Represents the method that will handle the `DragStarted` event of a `Thumb`.

## System.Windows.Controls.Primitives.GeneratorDirection

Source: SDK

Specifies the direction in which item generation will occur.

### Fields

| Member | Description | Src |
|---|---|---|
| `Backward` | Specifies to generate items in a backward direction. | SDK |
| `Forward` | Specifies to generate items in a forward direction. | SDK |

## System.Windows.Controls.Primitives.GeneratorPosition

Source: SDK

`GeneratorPosition` is used to describe the position of an item that is managed by `ItemContainerGenerator`.

### Properties

| Member | Description | Src |
|---|---|---|
| `Index` | Gets or sets the `Int32` index that is relative to the generated (realized) items. <br>Returns: An `Int32` index that is relative to the generated (realized) items. | SDK |
| `Offset` | Gets or sets the `Int32` offset that is relative to the ungenerated (unrealized) items near the indexed item. <br>Returns: An `Int32` offset that is relative to the ungenerated (unrealized) items near the indexed item. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Int32,Int32)` | Initializes a new instance of `GeneratorPosition` with the specified index and offset. <br>`index`: An `Int32` index that is relative to the generated (realized) items. -1 is a special value that refers to a fictitious item at the beginning or the end of the items list. <br>`offset`: An `Int32` offset that is relative to the ungenerated (unrealized) items near the indexed item. An offset of 0 refers to the indexed element itself, an offset 1 refers to the next ungenerated (unrealized) item, and an offset of -1 refers to the previous item. | SDK |
| `Equals(Object)` | Compares the specified instance and the current instance of `GeneratorPosition` for value equality. <br>`o`: The `GeneratorPosition` instance to compare. <br>Returns: true if `o` and this instance of `GeneratorPosition` have the same values. | SDK |
| `GetHashCode` | Returns the hash code for this `GeneratorPosition`. <br>Returns: The hash code for this `GeneratorPosition`. | SDK |
| `ToString` | Returns a string representation of this instance of `GeneratorPosition`. <br>Returns: A string representation of this instance of `GeneratorPosition`. | SDK |
| `op_Equality(Primitives.GeneratorPosition,Primitives.GeneratorPosition)` | Compares two `GeneratorPosition` objects for value equality. <br>`gp1`: The first instance to compare. <br>`gp2`: The second instance to compare. <br>Returns: true if the two objects are equal; otherwise, false. | SDK |
| `op_Inequality(Primitives.GeneratorPosition,Primitives.GeneratorPosition)` | Compares two `GeneratorPosition` objects for value inequality. <br>`gp1`: The first instance to compare. <br>`gp2`: The second instance to compare. <br>Returns: true if the values are not equal; otherwise, false. | SDK |

## System.Windows.Controls.Primitives.IItemContainerGenerator

Source: SDK

An interface that is implemented by classes which are responsible for generating UI content on behalf of a host.

### Methods

| Member | Description | Src |
|---|---|---|
| `GenerateNext(Boolean@)` | Returns the container element used to display the next item, and whether the container element has been newly generated (realized). <br>`isNewlyRealized`: Is true if the returned `DependencyObject` is newly generated (realized); otherwise, false. <br>Returns: A `DependencyObject` that is the container element which is used to display the next item. | SDK |
| `GeneratorPositionFromIndex(Int32)` | Returns the `GeneratorPosition` object that maps to the item at the specified index. <br>`itemIndex`: The index of desired item. <br>Returns: An `GeneratorPosition` object that maps to the item at the specified index. | SDK |
| `GetItemContainerGeneratorForPanel(Panel)` | Returns the `ItemContainerGenerator` appropriate for use by the specified panel. <br>`panel`: The panel for which to return an appropriate `ItemContainerGenerator`. <br>Returns: An `ItemContainerGenerator` appropriate for use by the specified panel. | SDK |
| `IndexFromGeneratorPosition(Primitives.GeneratorPosition)` | Returns the index that maps to the specified `GeneratorPosition`. <br>`position`: The index of desired item.The `GeneratorPosition` for the desired index. <br>Returns: An `Int32` that is the index which maps to the specified `GeneratorPosition`. | SDK |
| `PrepareItemContainer(DependencyObject)` | Prepares the specified element as the container for the corresponding item. <br>`container`: The container to prepare. Normally, `container` is the result of the previous call to `Boolean@)`. | SDK |
| `Remove(Primitives.GeneratorPosition,Int32)` | Removes one or more generated (realized) items. <br>`position`: The `Int32` index of the element to remove. `position` must refer to a previously generated (realized) item, which means its offset must be zero. <br>`count`: The `Int32` number of elements to remove, starting at `position`. | SDK |
| `RemoveAll` | Removes all generated (realized) items. | SDK |
| `StartAt(Primitives.GeneratorPosition,Primitives.GeneratorDirection,Boolean)` | Prepares the generator to generate items, starting at the specified `GeneratorPosition`, and in the specified `GeneratorDirection`, and controlling whether or not to start at a generated (realized) item. <br>`position`: A `GeneratorPosition`, that specifies the position of the item to start generating items at. <br>`direction`: Specifies the position of the item to start generating items at. <br>`allowStartAtRealizedItem`: A `Boolean` that specifies whether to start at a generated (realized) item. <br>Returns: An `IDisposable` object that tracks the lifetime of the generation process. | SDK |

## System.Windows.Controls.Primitives.IRecyclingItemContainerGenerator

Source: SDK

Extends the `IItemContainerGenerator` interface to reuse the UI content it generates. Classes that are responsible for generating user interface (UI) content on behalf of a host implement this interface.

### Methods

| Member | Description | Src |
|---|---|---|
| `Recycle(Primitives.GeneratorPosition,Int32)` | Disassociates item containers from their data items and saves the containers so they can be reused later for other data items. <br>`position`: The zero-based index of the first element to reuse. must refer to a previously generated (realized) item. <br>`count`: The number of elements to reuse, starting at . | SDK |

## System.Windows.Controls.Primitives.IScrollInfo

Source: SDK

Represents the main scrollable region inside a `ScrollViewer` control.

### Properties

| Member | Description | Src |
|---|---|---|
| `CanHorizontallyScroll` | Gets or sets a value that indicates whether scrolling on the horizontal axis is possible. <br>Returns: true if scrolling is possible; otherwise false. This property has no default value. | SDK |
| `CanVerticallyScroll` | Gets or sets a value that indicates whether scrolling on the vertical axis is possible. <br>Returns: true if scrolling is possible; otherwise false. This property has no default value. | SDK |
| `ExtentHeight` | Gets the vertical size of the extent. <br>Returns: A `Double` that represents, in device independent pixels, the vertical size of the extent. This property has no default value. | SDK |
| `ExtentWidth` | Gets the horizontal size of the extent. <br>Returns: A `Double` that represents, in device independent pixels, the horizontal size of the extent. This property has no default value. | SDK |
| `HorizontalOffset` | Gets the horizontal offset of the scrolled content. <br>Returns: A `Double` that represents, in device independent pixels, the horizontal offset. Valid values are between zero and the `ExtentWidth` minus the `ViewportWidth`. This property has no default value. | SDK |
| `ScrollOwner` | Gets or sets a `ScrollViewer` element that controls scrolling behavior. <br>Returns: A `ScrollViewer` element that controls scrolling behavior. This property has no default value. | SDK |
| `VerticalOffset` | Gets the vertical offset of the scrolled content. <br>Returns: A `Double` that represents, in device independent pixels, the vertical offset of the scrolled content. Valid values are between zero and the `ExtentHeight` minus the `ViewportHeight`. This property has no default value. | SDK |
| `ViewportHeight` | Gets the vertical size of the viewport for this content. <br>Returns: A `Double` that represents, in device independent pixels, the vertical size of the viewport for this content. This property has no default value. | SDK |
| `ViewportWidth` | Gets the horizontal size of the viewport for this content. <br>Returns: A `Double` that represents, in device independent pixels, the horizontal size of the viewport for this content. This property has no default value. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `LineDown` | Scrolls down within the content by one logical unit. | SDK |
| `LineLeft` | Scrolls left within the content by one logical unit. | SDK |
| `LineRight` | Scrolls right within the content by one logical unit. | SDK |
| `LineUp` | Scrolls up within the content by one logical unit. | SDK |
| `MakeVisible(UIElement,Rect)` | Forces content to scroll until the coordinate space of a visual object is visible. <br>`visual`: A `UIElement` that becomes visible. <br>`rectangle`: A bounding rectangle that identifies the coordinate space to make visible. <br>Returns: A `Rect` that is visible. | SDK |
| `MouseWheelDown` | Scrolls down within the content after the user clicks the wheel button on a mouse. | SDK |
| `MouseWheelLeft` | Scrolls left within the content after the user clicks the wheel button on a mouse. | SDK |
| `MouseWheelRight` | Scrolls right within the content after the user clicks the wheel button on a mouse. | SDK |
| `MouseWheelUp` | Scrolls up within the content after the user clicks the wheel button on a mouse. | SDK |
| `PageDown` | Scrolls down within the content by one page. | SDK |
| `PageLeft` | Scrolls left within the content by one page. | SDK |
| `PageRight` | Scrolls right within the content by one page. | SDK |
| `PageUp` | Scrolls up within the content by one page. | SDK |
| `SetHorizontalOffset(Double)` | Sets the amount of horizontal offset. <br>`offset`: The amount that content is horizontally offset from the containing viewport. | SDK |
| `SetVerticalOffset(Double)` | Sets the amount of vertical offset. <br>`offset`: The amount that content is vertically offset from the containing viewport. | SDK |

## System.Windows.Controls.Primitives.ItemsChangedEventArgs

Source: SDK

Provides data for the `ItemsChanged` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Action` | Gets the action that occurred on the items collection. <br>Returns: Returns the action that occurred. | SDK |
| `ItemCount` | Gets the number of items that were involved in the change. <br>Returns: Integer that represents the number of items involved in the change. | SDK |
| `ItemUICount` | Gets the number of UI elements involved in the change. <br>Returns: Integer that represents the number of UI elements involved in the change. | SDK |
| `OldPosition` | Gets the position in the collection before the change occurred. <br>Returns: Returns a `GeneratorPosition`. | SDK |
| `Position` | Gets the position in the collection where the change occurred. <br>Returns: Returns a `GeneratorPosition`. | SDK |

## System.Windows.Controls.Primitives.ItemsChangedEventHandler

Source: SDK

Represents the method that will handle the `ItemsChanged` event.

## System.Windows.Controls.Primitives.LayoutInformation

Source: SDK

Defines methods that provide additional information about the layout of an element.

### Methods

| Member | Description | Src |
|---|---|---|
| `GetLayoutClip(FrameworkElement)` | Returns the visible region of the specified element. <br>`element`: The element whose layout clip geometry is desired. <br>Returns: The visible region of the clipped element, or null if the element was not clipped during layout. | SDK |
| `GetLayoutExceptionElement(Threading.Dispatcher)` | Returns the element that was being processed by the layout system at the moment of an unhandled exception. <br>`dispatcher`: The `Dispatcher` object that defines the scope of the operation. There is one dispatcher per layout engine instance. <br>Returns: The element being processed at the time of an unhandled exception. | SDK |
| `GetLayoutSlot(FrameworkElement)` | Returns the layout slot, or bounding box, that contains the specified element. <br>`element`: The element whose layout slot is desired. <br>Returns: The area assigned to the element for layout. | SDK |

## System.Windows.Controls.Primitives.MenuBase

Source: Toolkit · class : ItemsControl · `Microsoft.Phone.Controls.Toolkit/ContextMenu/MenuBase.cs`

Represents a control that defines choices for users to select.

### Properties

| Member | Description | Src |
|---|---|---|
| `Style ItemContainerStyle` | Gets or sets the Style that is applied to the container element generated for each item. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ItemContainerStyleProperty` | Identifies the ItemContainerStyle dependency property. | TK |

## System.Windows.Controls.Primitives.PlacementMode

Source: SDK

Specifies the preferred location for positioning a `ToolTip` relative to a visual element.

### Fields

| Member | Description | Src |
|---|---|---|
| `Bottom` | Indicates that the preferred location of the tooltip is at the bottom of the target element. | SDK |
| `Left` | Indicates that the preferred location of the tooltip is at the left of the target element. | SDK |
| `Mouse` | Indicates that the preferred location of the tooltip is at the mouse pointer location. | SDK |
| `Right` | Indicates that the preferred location of the tooltip is at the right of the target element. | SDK |
| `Top` | Indicates that the preferred location of the tooltip is at the top of the target element. | SDK |

## System.Windows.Controls.Primitives.Popup

Source: SDK

Displays content on top of existing Silverlight content, within the bounds of the Silverlight control.

### Properties

| Member | Description | Src |
|---|---|---|
| `Child` | Gets or sets the content to be hosted in the popup. <br>Returns: The content to be hosted in the popup. | SDK |
| `HorizontalOffset` | Gets or sets the distance between the left side of the Silverlight control and the left side of the popup. <br>Returns: The distance, in pixels, between the left side of the Silverlight control and the left side of the popup. | SDK |
| `IsOpen` | Gets or sets whether the popup is currently displaying on the screen. <br>Returns: true if the popup is currently displaying; otherwise, false. The default is false. | SDK |
| `VerticalOffset` | Gets or sets the distance between the top of the Silverlight control and the top of the popup. <br>Returns: The distance, in pixels, from the top of the Silverlight control and the top of the popup. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ChildProperty` | Gets the identifier for the `Child` dependency property. <br>Returns: The identifier for the `Child` dependency property. | SDK |
| `HorizontalOffsetProperty` | Gets the identifier for the `HorizontalOffset` dependency property. <br>Returns: The identifier for the `HorizontalOffset` dependency property. | SDK |
| `IsOpenProperty` | Gets the identifier for the `IsOpen` dependency property. <br>Returns: The identifier for the `IsOpen` dependency property. | SDK |
| `VerticalOffsetProperty` | Gets the identifier for the `VerticalOffset` dependency property. <br>Returns: The identifier for the `VerticalOffset` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Closed` | Occurs when the `IsOpen` property is set to false. | SDK |
| `Opened` | Occurs when the `IsOpen` property is set to true. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Popup` class. | SDK |

## System.Windows.Controls.Primitives.RangeBase

Source: SDK

Represents an element that has a value within a specific range, such as the `ProgressBar`, `ScrollBar`, and `Slider` controls.

### Properties

| Member | Description | Src |
|---|---|---|
| `LargeChange` | Gets or sets a value to be added to or subtracted from the `Value` of a `RangeBase` control. <br>Returns: `Value` to add to or subtract from the `Value` of the `RangeBase` element. The default is 1. | SDK |
| `Maximum` | Gets or sets the highest possible `Value` of the range element. <br>Returns: The highest possible `Value` of the range element. The default is 1. | SDK |
| `Minimum` | Gets or sets the `Minimum` possible `Value` of the range element. <br>Returns: `Minimum` possible `Value` of the range element. The default is 0. | SDK |
| `SmallChange` | Gets or sets a `Value` to be added to or subtracted from the `Value` of a `RangeBase` control. <br>Returns: `Value` to add to or subtract from the `Value` of the `RangeBase` element. The default is 0.1. | SDK |
| `Value` | Gets or sets the current setting of the range control, which may be coerced. <br>Returns: The current setting of the range control, which may be coerced. The default is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `LargeChangeProperty` | Identifies the `LargeChange` dependency property. <br>Returns: The identifier for the `LargeChange` dependency property. | SDK |
| `MaximumProperty` | Identifies the `Maximum` dependency property. <br>Returns: The identifier for the `Maximum` dependency property. | SDK |
| `MinimumProperty` | Identifies the `Minimum` dependency property. <br>Returns: The identifier for the `Minimum` dependency property. | SDK |
| `SmallChangeProperty` | Identifies the `SmallChange` dependency property. <br>Returns: The identifier for the `SmallChange` dependency property. | SDK |
| `ValueProperty` | Identifies the `Value` dependency property. <br>Returns: The identifier for the `Value` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `ValueChanged` | Occurs when the range value changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RangeBase` class. | SDK |
| `OnMaximumChanged(Double,Double)` | Called when the `Maximum` property changes. <br>`oldMaximum`: Old value of the `Maximum` property. <br>`newMaximum`: New value of the `Maximum` property. | SDK |
| `OnMinimumChanged(Double,Double)` | Called when the `Minimum` property changes. <br>`oldMinimum`: Old value of the `Minimum` property. <br>`newMinimum`: New value of the `Minimum` property. | SDK |
| `OnValueChanged(Double,Double)` | Raises the `ValueChanged` routed event. <br>`oldValue`: Old value of the `Value` property. <br>`newValue`: New value of the `Value` property. | SDK |
| `ToString` | Provides a string representation of a `RangeBase` object. <br>Returns: Returns the string representation of a `RangeBase` object. | SDK |

## System.Windows.Controls.Primitives.RepeatButton

Source: SDK

Represents a control that raises its `Click` event repeatedly from the time it is pressed until it is released.

### Properties

| Member | Description | Src |
|---|---|---|
| `Delay` | Gets or sets the time, in milliseconds, the `RepeatButton` waits when it is pressed before it starts repeating the click action. <br>Returns: The time, in milliseconds, the `RepeatButton` waits when it is pressed before it starts repeating the click action. The default is 250. | SDK |
| `Interval` | Gets or sets the time, in milliseconds, between repetitions of the click action, as soon as repeating starts. <br>Returns: The time, in milliseconds, between repetitions of the click action, as soon as repeating starts. The default is 250. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `DelayProperty` | Identifies the `Delay` dependency property. <br>Returns: The identifier for the `Delay` dependency property. | SDK |
| `IntervalProperty` | Identifies the `Interval` dependency property. <br>Returns: The identifier for the `Interval` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RepeatButton` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `RepeatButton` control when a new template is applied. | SDK |

## System.Windows.Controls.Primitives.ScrollBar

Source: SDK

Represents a control that provides a scroll bar that has a sliding `Thumb` whose position corresponds to a value.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation` | Gets or sets whether the `ScrollBar` is displayed horizontally or vertically. <br>Returns: An `Orientation` enumeration value that defines whether the `ScrollBar` is displayed horizontally or vertically. The default is `Horizontal`. | SDK |
| `ViewportSize` | Gets or sets the amount of the scrollable content that is currently visible. <br>Returns: The amount of the scrollable content that is currently visible. The default is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `OrientationProperty` | Identifies the `Orientation` dependency property. <br>Returns: The identifier for the `Orientation` dependency property. | SDK |
| `ViewportSizeProperty` | Identifies the `ViewportSize` dependency property. <br>Returns: The identifier for the `ViewportSize` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Scroll` | Occurs one or more times as content scrolls in a `ScrollBar` when the user moves the `Thumb` by using the mouse. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ScrollBar` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ScrollBar` control when a new template is applied. | SDK |

## System.Windows.Controls.Primitives.ScrollEventArgs

Source: SDK

Provides data for the `Scroll` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `NewValue` | Gets the new `Value` of the `ScrollBar`. <br>Returns: The `Value` of the `ScrollBar` after the event. | SDK |
| `ScrollEventType` | Gets a `ScrollEventType` describing the event. <br>Returns: A `ScrollEventType` describing the event. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Primitives.ScrollEventType,Double)` | Initializes a new instance of the `ScrollEventArgs` class. <br>`scrollEventType`: A `ScrollEventType` describing the event. <br>`newValue`: The new `Value` of the `ScrollBar`. | SDK |

## System.Windows.Controls.Primitives.ScrollEventHandler

Source: SDK

Represents the method that will handle the `Scroll` event of a `ScrollBar`.

## System.Windows.Controls.Primitives.ScrollEventType

Source: SDK

Specifies the type of `Scroll` event that occurred.

### Fields

| Member | Description | Src |
|---|---|---|
| `EndScroll` | Specifies that the `Thumb` was dragged to a new position and is now no longer being dragged by the user. | SDK |
| `First` | Specifies that the `Thumb` moved to the `Minimum` position of the `ScrollBar`. | SDK |
| `LargeDecrement` | Specifies that the `Thumb` moved a specified distance, as determined by the value of `LargeChange`. The `Thumb` moved to the left for a horizontal `ScrollBar` or upward for a vertical `ScrollBar`. | SDK |
| `LargeIncrement` | Specifies that the `Thumb` moved a specified distance, as determined by the value of `LargeChange`. The `Thumb` moved to the right for a horizontal `ScrollBar` or downward for a vertical `ScrollBar`. | SDK |
| `Last` | Specifies that the `Thumb` moved to the `Minimum` position of the `ScrollBar`. | SDK |
| `SmallDecrement` | Specifies that the `Thumb` moved a specified distance, as determined by the value of `SmallChange`. The `Thumb` moved to the left for a horizontal `ScrollBar` or upward for a vertical `ScrollBar`. | SDK |
| `SmallIncrement` | Specifies that the `Thumb` moved a specified distance, as determined by the value of `SmallChange`. The `Thumb` moved to the right for a horizontal `ScrollBar` or downward for a vertical `ScrollBar`. | SDK |
| `ThumbPosition` | Specifies that the `Thumb` moved to a new position because the user selected Scroll Here in the shortcut menu of the `ScrollBar`. | SDK |
| `ThumbTrack` | The `Thumb` was dragged and caused a `MouseMove` event. A `Scroll` event of this `ScrollEventType` may occur more than one time when the `Thumb` is dragged in the `ScrollBar`. | SDK |

## System.Windows.Controls.Primitives.Selector

Source: SDK

Represents a control that allows a user to select an item from a collection of items.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsSynchronizedWithCurrentItem` | Gets or sets a value that indicates whether the `Selector` should keep the `SelectedItem` synchronized with the current item in the `Items` property. <br>Returns: true if the `SelectedItem` is always synchronized with the current item; false if the `SelectedItem` is never synchronized with the current item; null if the `SelectedItem` is synchronized with the current item only if the `Selector` uses a `ICollectionView`. The default is null. | SDK |
| `SelectedIndex` | Gets or sets the index of the selected item. <br>Returns: The index of the selected item. The default is -1. | SDK |
| `SelectedItem` | Gets or sets the selected item. <br>Returns: The selected item. The default is null. | SDK |
| `SelectedValue` | Gets or sets the value of the selected item, obtained by using the `SelectedValuePath`. <br>Returns: The value of the selected item, obtained by using the `SelectedValuePath`, or null if no item is selected. The default value is null. | SDK |
| `SelectedValuePath` | Gets or sets the property path that is used to get the `SelectedValue` property of the `SelectedItem` property. <br>Returns: The property path that is used to get the `SelectedValue` property of the `SelectedItem` property. The default is `Empty`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsSynchronizedWithCurrentItemProperty` | Identifies the `IsSynchronizedWithCurrentItem` dependency property. <br>Returns: The identifier for the `IsSynchronizedWithCurrentItem` dependency property. | SDK |
| `SelectedIndexProperty` | Identifies the `SelectedIndex` dependency property. <br>Returns: The identifier for the `SelectedIndex` dependency property. | SDK |
| `SelectedItemProperty` | Identifies the `SelectedItem` dependency property. | SDK |
| `SelectedValuePathProperty` | Gets the identifier for the `SelectedValuePath` dependency property. <br>Returns: The identifier for the `SelectedValuePath` dependency property. | SDK |
| `SelectedValueProperty` | Gets the identifier for the `SelectedValue` dependency property. <br>Returns: The identifier for the `SelectedValue` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `SelectionChanged` | Occurs when the currently selected item changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ClearContainerForItemOverride(DependencyObject,Object)` | Removes any bindings and templates applied to the item container for the specified content. <br>`element`: The combo box item used to display the specified content. <br>`item`: The item content. | SDK |
| `GetIsSelectionActive(DependencyObject)` | Gets a value that indicates whether the specified `Selector` has the focus. <br>`element`: The `Selector` to evaluate. <br>Returns: true to indicate that the `Selector` has the focus; otherwise, false. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `Selector` control when a new template is applied. | SDK |
| `OnItemsChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Updates the current selected item when the `Items` collection has changed. <br>`e`: Contains data about changes in the items collection. | SDK |
| `PrepareContainerForItemOverride(DependencyObject,Object)` | Prepares the specified element to display the specified item. <br>`element`: The element used to display the specified item. <br>`item`: The item to display | SDK |
| `System#ComponentModel#ISupportInitialize#BeginInit` | Signals the object that initialization is starting. | SDK |
| `System#ComponentModel#ISupportInitialize#EndInit` | Signals the object that initialization is complete. | SDK |

## System.Windows.Controls.Primitives.Thumb

Source: SDK

Represents a control that can be dragged by the user.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsDragging` | Gets whether the `Thumb` control has focus and mouse capture. <br>Returns: true if the `Thumb` control has focus and mouse capture; otherwise false. The default is false. | SDK |
| `IsFocused` | Gets whether the thumb has focus. <br>Returns: true to indicate the thumb has focus; otherwise false. The default is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsDraggingProperty` | Identifies the `IsDragging` dependency property. <br>Returns: The identifier for the `IsDragging` dependency property. | SDK |
| `IsFocusedProperty` | Gets the identifier for the `IsFocused` dependency property. <br>Returns: The identifier for the `IsFocused` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `DragCompleted` | Occurs when the `Thumb` control loses mouse capture. | SDK |
| `DragDelta` | Occurs one or more times as the mouse pointer is moved when a `Thumb` control has logical focus and mouse capture. | SDK |
| `DragStarted` | Occurs when a `Thumb` control receives logical focus and mouse capture. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Thumb` class. | SDK |
| `CancelDrag` | Cancels a drag operation for the `Thumb`. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `Thumb` control when a new template is applied. | SDK |

## System.Windows.Controls.Primitives.ToggleButton

Source: SDK

Base class for controls that can switch states, such as `CheckBox` and `RadioButton`.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsChecked` | Gets or sets whether the `ToggleButton` is checked. <br>Returns: true if the `ToggleButton` is checked; false if the `ToggleButton` is unchecked; otherwise null. The default is false. | SDK |
| `IsThreeState` | Gets or sets whether the control supports two or three states. <br>Returns: true if the control supports three states; otherwise, false. The default is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsCheckedProperty` | Identifies the `IsChecked` dependency property. <br>Returns: The identifier for the `IsChecked` dependency property. | SDK |
| `IsThreeStateProperty` | Identifies the `IsThreeState` dependency property. <br>Returns: The identifier for the `IsThreeState` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Checked` | Occurs when a `ToggleButton` is checked. | SDK |
| `Indeterminate` | Occurs when the state of a `ToggleButton` is switched to the indeterminate state. | SDK |
| `Unchecked` | Occurs when a `ToggleButton` is unchecked. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ToggleButton` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ToggleButton` when a new template is applied. | SDK |
| `OnClick` | Called when the `ToggleButton` is clicked by the mouse or the keyboard. | SDK |
| `OnContentChanged(Object,Object)` | Called when the `ContentControl` property changes. <br>`oldContent`: The content to be replaced. <br>`newContent`: The new content to display. | SDK |
| `OnCreateAutomationPeer` | Returns a `ToggleButtonAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `ToggleButtonAutomationPeer` for the `ToggleButton` object. | SDK |
| `OnToggle` | Called by the `OnClick` method to implement toggle behavior. | SDK |
| `ToString` | Returns the string representation of a `ToggleButton` object. <br>Returns: String representation of a `ToggleButton` object. | SDK |

## System.Windows.Controls.ProgressBar

Source: SDK

Represents a control that indicates the progress of an operation.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsIndeterminate` | Gets or sets a value that indicates whether the progress bar reports generic progress with a repeating pattern or reports progress based on the `Value` property. <br>Returns: true if the progress bar reports generic progress with a repeating pattern; false if the progress bar reports progress based on the `Value` property. The default is false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsIndeterminateProperty` | Identifies the `IsIndeterminate` dependency property. <br>Returns: The identifier for the `IsIndeterminate` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ProgressBar` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ProgressBar` control when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns a `ProgressBarAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `ProgressBarAutomationPeer` for the `ProgressBar` object. | SDK |
| `OnMaximumChanged(Double,Double)` | Called when value of the `Maximum` property changes. <br>`oldMaximum`: The previous value. <br>`newMaximum`: The new value. | SDK |
| `OnMinimumChanged(Double,Double)` | Called when value of the `Minimum` property changes. <br>`oldMinimum`: The previous value. <br>`newMinimum`: The new value. | SDK |
| `OnValueChanged(Double,Double)` | Called when value of the `Value` property changes. <br>`oldValue`: The previous value. <br>`newValue`: The new value. | SDK |

## System.Windows.Controls.RadioButton

Source: SDK

Represents a button that allows a user to select a single option from a group of options.

### Properties

| Member | Description | Src |
|---|---|---|
| `GroupName` | Gets or sets the name that specifies which `RadioButton` controls are mutually exclusive. <br>Returns: The name that specifies which `RadioButton` controls are mutually exclusive. The default is null. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `GroupNameProperty` | Identifies the `GroupName` dependency property. <br>Returns: The identifier for `GroupName` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RadioButton` class. | SDK |
| `OnCreateAutomationPeer` | Returns a `RadioButtonAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: An `RadioButtonAutomationPeer` for the radio button object. | SDK |
| `OnToggle` | Sets the `IsChecked` property to true. | SDK |

## System.Windows.Controls.RichTextBox

Source: SDK

Represents a rich text editing control that supports formatted text, hyperlinks, inline images, and other rich content.

### Properties

| Member | Description | Src |
|---|---|---|
| `AcceptsReturn` | Gets or sets a value that determines whether the `RichTextBox` allows and displays the newline or return characters when the ENTER or RETURN keys are pressed. <br>Returns: true if the `RichTextBox` allows newline characters; otherwise, false. The default is true. | SDK |
| `BaselineOffset` | Gets a value that represents the offset in pixels from the top of the content to the baseline of the first paragraph. The baseline of the paragraph is the baseline of the first line in it. <br>Returns: The computed baseline for the first paragraph, or 0 if the `RichTextBox` is empty. | SDK |
| `Blocks` | Gets the contents of the `RichTextBox`. <br>Returns: A `BlockCollection` that contains the contents of the `RichTextBox`. | SDK |
| `CaretBrush` | Gets or sets the brush that is used to render the vertical bar that indicates the insertion point. <br>Returns: A brush that is used to render the vertical bar that indicates the insertion point. | SDK |
| `ContentEnd` | Gets a `TextPointer` that indicates the end of content in the `RichTextBox`. <br>Returns: A `TextPointer` that indicates the end of content in the `RichTextBox`. | SDK |
| `ContentStart` | Gets a `TextPointer` that indicates the start of content in the `RichTextBox`. <br>Returns: A `TextPointer` that indicates the start of content in the `RichTextBox`. | SDK |
| `HorizontalScrollBarVisibility` | Gets or sets the visibility of the horizontal scroll bar. <br>Returns: The visibility of the horizontal scroll bar. The default is `Hidden`. | SDK |
| `IsReadOnly` | Gets or sets a value that determines whether the user can change the text in the `RichTextBox`. <br>Returns: true if the `RichTextBox` is read-only; otherwise, false. The default is false. | SDK |
| `LineHeight` | Gets or sets the height of each line of content. <br>Returns: The height of each line in pixels. A value of 0 indicates that the line height is determined automatically from the current font characteristics. The default is 0. | SDK |
| `LineStackingStrategy` | Gets or sets a value that indicates how a line box is determined for each line of text in the `RichTextBox`. <br>Returns: A value that indicates how a line box is determined for each line of text in the `RichTextBox`. The default is `MaxHeight`. | SDK |
| `OverflowContentTarget` | Gets or sets the `RichTextBoxOverflow` that will consume the overflow content of this `RichTextBox`. <br>Returns: A `RichTextBoxOverflow` that consumes the overflow content of this `RichTextBox`. | SDK |
| `Selection` | Gets the `TextSelection` in the `RichTextBox`. <br>Returns: A `TextSelection` that represents the selected text in the `RichTextBox`. | SDK |
| `TextAlignment` | Gets or sets how the text should be aligned in the `RichTextBox`. <br>Returns: One of the `TextAlignment` enumeration values. The default is Left. | SDK |
| `TextWrapping` | Gets or sets how text wrapping occurs if a line of text extends beyond the available width of the `RichTextBox`. <br>Returns: One of the `TextWrapping` values. The default is `Wrap`. | SDK |
| `VerticalScrollBarVisibility` | Gets or sets the visibility of the vertical scroll bar. <br>Returns: The visibility of the vertical scroll bar. The default is `Auto`. | SDK |
| `Xaml` | Gets or sets a XAML representation of the content in the `RichTextBox`. <br>Returns: A `String` object that is a XAML representation of the content in the `RichTextBox`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AcceptsReturnProperty` | Identifies the `AcceptsReturn` dependency property. <br>Returns: The identifier for the `AcceptsReturn` dependency property. | SDK |
| `CaretBrushProperty` | Identifies the `CaretBrush` dependency property. <br>Returns: Identifier for the `CaretBrush` dependency property. | SDK |
| `IsReadOnlyProperty` | Identifies the `IsReadOnly` dependency property. <br>Returns: The identifier for the `IsReadOnly` dependency property. | SDK |
| `LineHeightProperty` | Identifies the `LineHeight` dependency property. <br>Returns: The identifier for the `LineHeight` dependency property. | SDK |
| `LineStackingStrategyProperty` | Identifies the `LineStackingStrategy` dependency property. <br>Returns: The identifier for the `LineStackingStrategy` dependency property. | SDK |
| `OverflowContentTargetProperty` | Identifies the `OverflowContentTarget` dependency property. <br>Returns: Identifier for the `OverflowContentTarget` dependency property. | SDK |
| `TextAlignmentProperty` | Identifies the `TextAlignment` dependency property. <br>Returns: Identifier for the `TextAlignment` dependency property. | SDK |
| `TextWrappingProperty` | Identifies the `TextWrapping` dependency property. <br>Returns: The identifier for the `TextWrapping` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `ContentChanged` | Occurs when the content changes in a `RichTextBox`. | SDK |
| `SelectionChanged` | Occurs when the text selection has changed. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RichTextBox` class. | SDK |
| `GetPositionFromPoint(Point)` | Returns a `TextPointer` that indicates the closest insertion position for the specified point. <br>`point`: A point in the coordinate space of the `RichTextBox` for which the closest insertion position is retrieved. <br>Returns: A `TextPointer` that indicates the closest insertion position for the specified point. | SDK |
| `OnCreateAutomationPeer` | Returns a `RichTextBoxAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `RichTextBoxAutomationPeer` for the `RichTextBox` object. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Called before the `GotFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Called when the `ZeyDown` event occurs. <br>`e`: The data for the event. | SDK |
| `OnKeyUp(Input.ZeyEventArgs)` | Called before the `ZeyUp` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Called before the `LostFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostMouseCapture(Input.MouseEventArgs)` | Provides handling for the `LostMouseCapture` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Called before the `MouseEnter` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Called before the `MouseLeave` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Called before the `MouseLeftButtonDown` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Called before the `MouseLeftButtonUp` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseMove(Input.MouseEventArgs)` | Called before the `MouseMove` event occurs. <br>`e`: The data for the event. | SDK |
| `OnTextInput(Input.TextCompositionEventArgs)` | Called before the `TextInput` event occurs. <br>`e`: The data for the event. | SDK |
| `OnTextInputStart(Input.TextCompositionEventArgs)` | Called before the `TextInputStart` event occurs. <br>`e`: The data for the event. | SDK |
| `OnTextInputUpdate(Input.TextCompositionEventArgs)` | Called before the `TextInputUpdate` event occurs. <br>`e`: The data for the event. | SDK |
| `SelectAll` | Selects the entire contents in the `RichTextBox`. | SDK |

## System.Windows.Controls.RichTextBoxOverflow

Source: SDK

Displays the content that does not fit in a `RichTextBox` or another `RichTextBoxOverflow` instance.

### Properties

| Member | Description | Src |
|---|---|---|
| `HorizontalScrollBarVisibility` | Gets or sets the visibility of the horizontal scroll bar. <br>Returns: The visibility of the horizontal scroll bar. The default is `Hidden`. | SDK |
| `OverflowContentTarget` | Gets or sets the `RichTextBoxOverflow` that will consume any overflow content from this linked container. <br>Returns: A `RichTextBoxOverflow` that will consume any overflow content from this linked container. | SDK |
| `VerticalScrollBarVisibility` | Gets or sets the visibility of the vertical scroll bar. <br>Returns: The visibility of the vertical scroll bar. The default is `Hidden`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `OverflowContentTargetProperty` | Identifies the `OverflowContentTarget` dependency property. <br>Returns: Identifier for the `OverflowContentTarget` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RichTextBoxOverflow` class. | SDK |

## System.Windows.Controls.RoutedPropertyChangingEventArgs<T>

Source: Toolkit · class : RoutedEventArgs · `Microsoft.Phone.Controls.Toolkit/Common/RoutedPropertyChangingEventArgs.cs`

Provides event data for various routed events that track property values changing. Typically the events denote a cancellable action.

### Properties

| Member | Description | Src |
|---|---|---|
| `bool Cancel` | Gets or sets a value indicating whether the property change that originated the RoutedPropertyChanging event should be cancelled. <br>Value: True to cancel the property change; this resets the property to `OldValue`. false to not cancel the property change; the value changes to `NewValue`. | TK |
| `bool InCoercion` | Gets or sets a value indicating whether internal value coercion is acting on the property change that originated the RoutedPropertyChanging event. <br>Value: True if coercion is active. false if coercion is not active. | TK |
| `bool IsCancelable` | Gets a value indicating whether the property change that originated the RoutedPropertyChanging event is cancellable. <br>Value: True if the property change is cancellable. false if the property change is not cancellable. | TK |
| `T NewValue` | Gets or sets a value that reports the new value of the changing property, assuming that the property change is not cancelled. <br>Value: The new value of the changing property. | TK |
| `T OldValue` | Gets a value that reports the previous value of the changing property. <br>Value: The previous value of the changing property. | TK |
| `DependencyProperty Property` | Gets the `DependencyProperty` identifier for the property that is changing. <br>Value: The `DependencyProperty` identifier for the property that is changing. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» RoutedPropertyChangingEventArgs( DependencyProperty property, T oldValue, T newValue, bool isCancelable)` | Initializes a new instance of the `RoutedPropertyChangingEventArgs`1` class. <br>`property`: The `DependencyProperty` identifier for the property that is changing. <br>`oldValue`: The previous value of the property. <br>`newValue`: The new value of the property, assuming that the property change is not cancelled. <br>`isCancelable`: True if the property change is cancellable by setting `Cancel` to true in event handling. false if the property change is not cancellable. | TK |

## System.Windows.Controls.RowDefinition

Source: SDK

Defines row-specific properties that apply to `Grid` elements.

### Properties

| Member | Description | Src |
|---|---|---|
| `ActualHeight` | Gets a value that represents the calculated height of the `RowDefinition`. <br>Returns: A `Double` that represents the calculated height in pixels. The default value is 0. | SDK |
| `Height` | Gets the calculated height of a `RowDefinition` element, or sets the `GridLength` value of a row that is defined by the `RowDefinition`. <br>Returns: The `GridLength` that represents the height of the row. The default value is 1.0. | SDK |
| `MaxHeight` | Gets or sets a value that represents the maximum height of a `RowDefinition`. <br>Returns: A `Double` that represents the maximum height. | SDK |
| `MinHeight` | Gets or sets a value that represents the minimum allowed height of a `RowDefinition`. <br>Returns: A `Double` that represents the minimum allowed height. The default value is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `HeightProperty` | Identifies the `Height` dependency property. <br>Returns: The identifier for the `Height` dependency property. | SDK |
| `MaxHeightProperty` | Identifies the `MaxHeight` dependency property. <br>Returns: The identifier for the `MaxHeight` dependency property. | SDK |
| `MinHeightProperty` | Identifies the `MinHeight` dependency property. <br>Returns: The identifier for the `MinHeight` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RowDefinition` class. | SDK |

## System.Windows.Controls.RowDefinitionCollection

Source: SDK

Provides access to an ordered, strongly typed collection of `RowDefinition` objects.

## System.Windows.Controls.SaveFileDialog

Source: SDK

Provides a dialog box that enables the user to specify options for saving a file.

### Properties

| Member | Description | Src |
|---|---|---|
| `DefaultExt` | Gets or sets the default file name extension applied to files that are saved with the `SaveFileDialog`. <br>Returns: The default file name extension applied to files that are saved with the `SaveFileDialog`, which can optionally include the dot character (.). | SDK |
| `DefaultFileName` | Gets or sets the file name used if a file name is not specified by the user. <br>Returns: The file name used if a file name is not specified by the user.`String`. | SDK |
| `Filter` | Gets or sets a filter string that specifies the files types and descriptions to display in the `SaveFileDialog`. <br>Returns: A filter string that specifies the file types and descriptions to display in the `SaveFileDialog`. The default is `Empty`. | SDK |
| `FilterIndex` | Gets or sets the index of the selected item in the Save as type drop-down list. <br>Returns: The index of the selected item in the Save as type filter drop-down list. The default is 1. | SDK |
| `SafeFileName` | Gets the file name for the selected file associated with the `SaveFileDialog`. <br>Returns: The file name for the selected file associated with the `SaveFileDialog`. The default is `Empty`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `SaveFileDialog` class. | SDK |
| `OpenFile` | Opens the file specified by the `SafeFileName` property. <br>Returns: A read-write stream for the file specified by the `SafeFileName` property. | SDK |
| `ShowDialog` | Displays a `SaveFileDialog` that is modal to the Web browser window. <br>Returns: true if the user clicked Save; false if the user clicked Cancel or closed the dialog box. | SDK |

## System.Windows.Controls.ScrollBarVisibility

Source: SDK

Specifies the visibility of a scrollbar within a `ScrollViewer` control.

### Fields

| Member | Description | Src |
|---|---|---|
| `Auto` | A `ScrollBar` appears and the dimension of the `ScrollViewer` is applied to the content when the viewport cannot display all of the content. For a horizontal `ScrollBar`, the width of the content is set to the `ViewportWidth` of the `ScrollViewer`. For a vertical `ScrollBar`, the height of the content is set to the `ViewportHeight` of the `ScrollViewer`. | SDK |
| `Disabled` | A `ScrollBar` does not appear even when the viewport cannot display all of the content. The dimension of the content is set to the corresponding dimension of the `ScrollViewer` parent. For a horizontal `ScrollBar`, the width of the content is set to the `ViewportWidth` of the `ScrollViewer`. For a vertical `ScrollBar`, the height of the content is set to the `ViewportHeight` of the `ScrollViewer`. | SDK |
| `Hidden` | A `ScrollBar` does not appear even when the viewport cannot display all of the content. The dimension of the `ScrollViewer` is not applied to the content. | SDK |
| `Visible` | A `ScrollBar` always appears. The dimension of the `ScrollViewer` is applied to the content. For a horizontal `ScrollBar`, the width of the content is set to the `ViewportWidth` of the `ScrollViewer`. For a vertical `ScrollBar`, the height of the content is set to the `ViewportHeight` of the `ScrollViewer`. | SDK |

## System.Windows.Controls.ScrollContentPresenter

Source: SDK

Displays the content of a `ScrollViewer` control.

### Properties

| Member | Description | Src |
|---|---|---|
| `CanHorizontallyScroll` | Gets or sets a value that indicates whether scrolling on the horizontal axis is possible. <br>Returns: true if scrolling is possible; otherwise, false. | SDK |
| `CanVerticallyScroll` | Gets or sets a value that indicates whether scrolling on the vertical axis is possible. <br>Returns: true if scrolling is possible; otherwise, false. | SDK |
| `ExtentHeight` | Gets the vertical size of the extent. <br>Returns: The vertical size of the extent. | SDK |
| `ExtentWidth` | Gets the horizontal size of the extent. <br>Returns: The horizontal size of the extent. | SDK |
| `HorizontalOffset` | Gets or sets the distance the content has been scrolled horizontally. <br>Returns: The distance the content has been scrolled horizontally. | SDK |
| `ScrollOwner` | Gets or sets the `ScrollViewer` element that controls scrolling behavior. <br>Returns: The `ScrollViewer` element that controls scrolling behavior. | SDK |
| `VerticalOffset` | Gets or sets the distance the content has been scrolled vertically. <br>Returns: The distance the content has been scrolled vertically. | SDK |
| `ViewportHeight` | Gets the vertical size of the viewport. <br>Returns: The vertical size of the viewport. | SDK |
| `ViewportWidth` | Gets the horizontal size of the viewport. <br>Returns: The horizontal size of the viewport. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ScrollContentPresenter` class. | SDK |
| `LineDown` | Scrolls the `ScrollContentPresenter` content downward by one line. | SDK |
| `LineLeft` | Scrolls the `ScrollContentPresenter` content to the left by a predetermined amount. | SDK |
| `LineRight` | Scrolls the `ScrollContentPresenter` content to the right by a predetermined amount. | SDK |
| `LineUp` | Scrolls the `ScrollContentPresenter` content upward by one line. | SDK |
| `MakeVisible(UIElement,Rect)` | Forces content to scroll until the coordinate space of a visual object is visible. <br>`visual`: A `UIElement` that becomes visible. <br>`rectangle`: The bounding rectangle that identifies the coordinate space to make visible. <br>Returns: A `Rect` that represents the visible region. | SDK |
| `MouseWheelDown` | Scrolls down within content after a user clicks the wheel button on a mouse. | SDK |
| `MouseWheelLeft` | Scrolls left within content after a user clicks the wheel button on a mouse. | SDK |
| `MouseWheelRight` | Scrolls right within content after a user clicks the wheel button on a mouse. | SDK |
| `MouseWheelUp` | Scrolls up within content after a user clicks the wheel button on a mouse. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ScrollContentPresenter` when a new template is applied. | SDK |
| `PageDown` | Scrolls down within the content by one page. | SDK |
| `PageLeft` | Scrolls left within the content by one page. | SDK |
| `PageRight` | Scrolls right within the content by one page. | SDK |
| `PageUp` | Scrolls up within the content by one page. | SDK |
| `SetHorizontalOffset(Double)` | Sets the distance the content has been scrolled horizontally. <br>`offset`: The distance the content has been scrolled horizontally. | SDK |
| `SetVerticalOffset(Double)` | Sets the distance the content has been scrolled vertically. <br>`offset`: The distance the content has been scrolled vertically. | SDK |

## System.Windows.Controls.ScrollViewer

Source: SDK

Represents a scrollable area that can contain other visible elements.

### Properties

| Member | Description | Src |
|---|---|---|
| `ComputedHorizontalScrollBarVisibility` | Gets a value that indicates whether the horizontal `ScrollBar` is visible. <br>Returns: A `Visibility` that indicates whether the horizontal scroll bar is visible. The default value is `Visible`. | SDK |
| `ComputedVerticalScrollBarVisibility` | Gets a value that indicates whether the vertical `ScrollBar` is visible. <br>Returns: A `Visibility` that indicates whether the vertical scroll bar is visible. The default value is `Visible`. | SDK |
| `ExtentHeight` | Gets the vertical size of all the content for display in the `ScrollViewer`. <br>Returns: The vertical size of all the content for display in the `ScrollViewer`. | SDK |
| `ExtentWidth` | Gets the horizontal size of all the content for display in the `ScrollViewer`. <br>Returns: The horizontal size of all the content for display in the `ScrollViewer`. | SDK |
| `HorizontalOffset` | Gets a value that contains the horizontal offset of the scrolled content. <br>Returns: The horizontal offset of the scrolled content. The default value is 0.0. | SDK |
| `HorizontalScrollBarVisibility` | Gets or sets a value that indicates whether a horizontal `ScrollBar` should be displayed. <br>Returns: A `ScrollBarVisibility` value that indicates whether a horizontal `ScrollBar` should be displayed. The default value is `Hidden`. | SDK |
| `ManipulationMode` | Gets or sets the manipulation mode used for `ScrollViewer` behavior. <br>Returns: The manipulation mode used for `ScrollViewer` behavior. | SDK |
| `ScrollableHeight` | Gets a value that represents the vertical size of the area that can be scrolled; the difference between the height of the extent and the height of the viewport. <br>Returns: The vertical size of the area that can be scrolled. This property has no default value. | SDK |
| `ScrollableWidth` | Gets a value that represents the horizontal size of the area that can be scrolled; the difference between the width of the extent and the width of the viewport.. <br>Returns: The horizontal size of the area that can be scrolled. This property has no default value. | SDK |
| `VerticalOffset` | Gets a value that contains the vertical offset of the scrolled content. <br>Returns: The vertical offset of the scrolled content. The default value is 0.0. | SDK |
| `VerticalScrollBarVisibility` | Gets or sets a value that indicates whether a vertical `ScrollBar` should be displayed. <br>Returns: A `ScrollBarVisibility` value that indicates whether a vertical `ScrollBar` should be displayed. The default value is `Visible`. | SDK |
| `ViewportHeight` | Gets a value that contains the vertical size of the viewable content. <br>Returns: The vertical size of the viewable content. This property has no default value. | SDK |
| `ViewportWidth` | Gets a value that contains the horizontal size of the viewable content. <br>Returns: The horizontal size of the viewable content. The default value is 0.0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ComputedHorizontalScrollBarVisibilityProperty` | Identifies the `ComputedHorizontalScrollBarVisibility` dependency property. <br>Returns: The identifier for the `ComputedHorizontalScrollBarVisibility` dependency property. | SDK |
| `ComputedVerticalScrollBarVisibilityProperty` | Identifies the `ComputedVerticalScrollBarVisibility` dependency property. <br>Returns: The identifier for the `ComputedVerticalScrollBarVisibility` dependency property. | SDK |
| `ExtentHeightProperty` | Identifier for the `ExtentHeight` dependency property. <br>Returns: The identifier for the `ExtentHeight` dependency property. | SDK |
| `ExtentWidthProperty` | Identifier for the `ExtentWidth` dependency property. <br>Returns: The identifier for the `ExtentWidth` dependency property. | SDK |
| `HorizontalOffsetProperty` | Identifies the `HorizontalOffset` dependency property. <br>Returns: The identifier for the `HorizontalOffset` dependency property. | SDK |
| `HorizontalScrollBarVisibilityProperty` | Identifies the `HorizontalScrollBarVisibility` dependency property. <br>Returns: The identifier for the `HorizontalScrollBarVisibility` dependency property. | SDK |
| `ManipulationModeProperty` | Identifies the `ManipulationMode` dependency property. <br>Returns: The identifier for the `ManipulationMode` dependency property. | SDK |
| `ScrollableHeightProperty` | Identifies the `ScrollableHeight` dependency property. <br>Returns: The identifier for the `ScrollableHeight` dependency property. | SDK |
| `ScrollableWidthProperty` | Identifies the `ScrollableWidth` dependency property. <br>Returns: The identifier for the `ScrollableWidth` dependency property. | SDK |
| `VerticalOffsetProperty` | Identifies the `VerticalOffset` dependency property. <br>Returns: The identifier for the `VerticalOffset` dependency property. | SDK |
| `VerticalScrollBarVisibilityProperty` | Identifies the `VerticalScrollBarVisibility` dependency property. <br>Returns: The identifier for the `VerticalScrollBarVisibility` dependency property. | SDK |
| `ViewportHeightProperty` | Identifies the `ViewportHeight` dependency property. <br>Returns: The identifier for the `ViewportHeight` dependency property. | SDK |
| `ViewportWidthProperty` | Identifies the `ViewportWidth` dependency property. <br>Returns: The identifier for the `ViewportWidth` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ScrollViewer` class. | SDK |
| `GetHorizontalScrollBarVisibility(DependencyObject)` | Gets the value of the `HorizontalScrollBarVisibility` dependency property from a specified element. <br>`element`: The element from which the property value is read. <br>Returns: The value of the `HorizontalScrollBarVisibility` dependency property. | SDK |
| `GetManipulationMode(DependencyObject)` | Gets the value of the `ManipulationMode` attached property, and supports the property's XAML usage. <br>`element`: The object to get the property value from. <br>Returns: The obtained `ManipulationMode` attached property value. | SDK |
| `GetVerticalScrollBarVisibility(DependencyObject)` | Gets the value of the `VerticalScrollBarVisibility` dependency property from a specified element. <br>`element`: The element from which the property value is read. <br>Returns: The value of the `VerticalScrollBarVisibility` dependency property. | SDK |
| `InvalidateScrollInfo` | Called when the value of properties that describe the size and location of the scroll area change. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ScrollViewer` control when a new template is applied. | SDK |
| `ScrollToHorizontalOffset(Double)` | Scrolls the content that is within the `ScrollViewer` to the specified horizontal offset position. <br>`offset`: The position that the content scrolls to. | SDK |
| `ScrollToVerticalOffset(Double)` | Scrolls the content that is within the `ScrollViewer` to the specified vertical offset position. <br>`offset`: The position that the content scrolls to. | SDK |
| `SetHorizontalScrollBarVisibility(DependencyObject,ScrollBarVisibility)` | Sets the value of the `HorizontalScrollBarVisibility` dependency property to a specified element. <br>`element`: The element on which to set the property value. <br>`horizontalScrollBarVisibility`: The property value to set. | SDK |
| `SetManipulationMode(DependencyObject,ManipulationMode)` | Sets the value of the `ManipulationMode` attached property, and supports the property's XAML usage. <br>`element`: The element to set the property value on. <br>`manipulationMode`: The value to set. | SDK |
| `SetVerticalScrollBarVisibility(DependencyObject,ScrollBarVisibility)` | Sets the value of the `VerticalScrollBarVisibility` dependency property to a specified element. <br>`element`: The element on which to set the property value. <br>`verticalScrollBarVisibility`: The property value to set. | SDK |

## System.Windows.Controls.SelectionChangedEventArgs

Source: SDK

Provides data for the `SelectionChanged` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `AddedItems` | Gets a list that contains the items that were selected. <br>Returns: The items that were selected in this event. | SDK |
| `RemovedItems` | Gets a list that contains the items that were unselected. <br>Returns: The items that were unselected in this event. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Collections.IList,Collections.IList)` | Initializes a new instance of the `SelectionChangedEventArgs` class. <br>`removedItems`: The items that were unselected. <br>`addedItems`: The items that were selected. | SDK |

## System.Windows.Controls.SelectionChangedEventHandler

Source: SDK

Represents the method that will handle the `SelectionChanged` event.

## System.Windows.Controls.SelectionMode

Source: SDK

Defines the selection behavior for a `ListBox`.

### Fields

| Member | Description | Src |
|---|---|---|
| `Extended` | The user can select multiple items by pressing a modifier key. | SDK |
| `Multiple` | The user can select multiple items without pressing a modifier key. | SDK |
| `Single` | The user can select only one item at a time. | SDK |

## System.Windows.Controls.SelectorSelectionAdapter

Source: Toolkit · class : ISelectionAdapter · `Microsoft.Phone.Controls.Toolkit/AutoCompleteBox/SelectorSelectionAdapter.cs`

Represents the selection adapter contained in the drop-down portion of an `AutoCompleteBox` control.

### Properties

| Member | Description | Src |
|---|---|---|
| `IEnumerable ItemsSource` | Gets or sets a collection that is used to generate the content of the selection adapter. <br>Value: The collection used to generate content for the selection adapter. | TK |
| `object SelectedItem` | Gets or sets the selected item of the selection adapter. <br>Value: The selected item of the underlying selection adapter. | TK |
| `Selector SelectorControl` | Gets or sets the underlying `Selector` control. <br>Value: The underlying `Selector` control. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler Cancel` | Occurs when a selection is canceled before it is committed. | TK |
| `RoutedEventHandler Commit` | Occurs when an item is selected and is committed to the underlying `Selector` control. | TK |
| `SelectionChangedEventHandler SelectionChanged` | Occurs when the `SelectedItem` property value changes. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `AutomationPeer CreateAutomationPeer()` | Returns an automation peer for the underlying `Selector` control, for use by the Silverlight automation infrastructure. <br>Returns: An automation peer for use by the Silverlight automation infrastructure. | TK |
| `void HandleKeyDown(ZeyEventArgs e)` | Provides handling for the `ZeyDown` event that occurs when a key is pressed while the drop-down portion of the `AutoCompleteBox` has focus. <br>`e`: A `ZeyEventArgs` that contains data about the `ZeyDown` event. | TK |
| `«ctor» SelectorSelectionAdapter()` | Initializes a new instance of the `SelectorSelectionAdapter` class. | TK |
| `«ctor» SelectorSelectionAdapter(Selector selector)` | Initializes a new instance of the `SelectorSelectionAdapter` class with the specified `Selector` control. <br>`selector`: The `Selector` control to wrap as a `SelectorSelectionAdapter`. | TK |

## System.Windows.Controls.Separator

Source: Toolkit · class : Control · `Microsoft.Phone.Controls.Toolkit/ContextMenu/Separator.cs`

Control that is used to separate items in items controls.

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» Separator()` | Initializes a new instance of the Separator class. | TK |

## System.Windows.Controls.Slider

Source: SDK

Represents a control that lets the user select from a range of values by moving a `Thumb` control along a track.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsDirectionReversed` | Gets or sets a value that indicates the direction of increasing value. <br>Returns: true if the direction of increasing value is to the left for a horizontal slider or down for a vertical slider; otherwise, false. The default is false. | SDK |
| `IsFocused` | Gets a value indicating whether the slider control has focus. <br>Returns: true if the slider control has focus; otherwise, false. The default is false. | SDK |
| `Orientation` | Gets or sets the orientation of a `Slider`. <br>Returns: One of the `Orientation` values. The default is `Horizontal`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsDirectionReversedProperty` | Identifies the `IsDirectionReversed` dependency property. | SDK |
| `IsFocusedProperty` | Identifies the `IsFocused` dependency property. <br>Returns: The identifier for the `IsFocused` dependency property. | SDK |
| `OrientationProperty` | Identifies the `Orientation` dependency property. <br>Returns: The identifier for the `Orientation` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Slider` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `Slider` control when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns a `SliderAutomationPeer` object for use by the automation infrastructure. <br>Returns: A `SliderAutomationPeer` object for use by the slider control. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Provides class handling for the `GotFocus` event. <br>`e`: A `RoutedEventArgs` that contains the event data. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Provides class handling for the `ZeyDown` event. <br>`e`: A `ZeyEventArgs` that contains the event data. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Provides class handling for the `LostFocus` event. <br>`e`: A `RoutedEventArgs` that contains the event data. | SDK |
| `OnLostMouseCapture(Input.MouseEventArgs)` | Provides handling for the `LostMouseCapture` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMaximumChanged(Double,Double)` | Called when the `Maximum` property changes. <br>`oldMaximum`: Old value of the `Maximum` property. <br>`newMaximum`: New value of the `Maximum` property. | SDK |
| `OnMinimumChanged(Double,Double)` | Called when the `Minimum` property changes. <br>`oldMinimum`: Old value of the `Minimum` property. <br>`newMinimum`: New value of the `Minimum` property. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Provides class handling for the `MouseEnter` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Provides class handling for the `MouseLeave` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Provides class handling for the `MouseLeftButtonDown` event. | SDK |
| `OnValueChanged(Double,Double)` | Updates the current position of the `Slider` when the `Value` property changes. <br>`oldValue`: The old `Value` of the `Slider`. <br>`newValue`: The new `Value` of the `Slider`. | SDK |

## System.Windows.Controls.StackPanel

Source: SDK

Arranges child elements into a single line that can be oriented horizontally or vertically.

### Properties

| Member | Description | Src |
|---|---|---|
| `Orientation` | Gets or sets the dimension by which child elements are stacked. <br>Returns: One of the enumeration values that specifies the orientation of child elements. The default is `Vertical`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `OrientationProperty` | Identifies the `Orientation` dependency property. <br>Returns: The identifier for the `Orientation` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `StackPanel` class. | SDK |
| `ArrangeOverride(Size)` | Positions child elements and determines a size for the derived class. <br>`arrangeSize`: The final area within the parent that this element should use to arrange itself and its children. <br>Returns: The actual size used. | SDK |
| `MeasureOverride(Size)` | Measures the size in layout required for child elements and determines a size for the derived class. <br>`constraint`: The available size that this element can give to child elements. <br>Returns: The size that this element determines it needs during layout, based on its calculations of child element sizes. | SDK |

## System.Windows.Controls.StretchDirection

Source: SDK

Describes the direction that content is scaled.

### Fields

| Member | Description | Src |
|---|---|---|
| `Both` | The content stretches to fit the parent according to the `Stretch` property. | SDK |
| `DownOnly` | The content scales downward only when it is larger than the parent. If the content is smaller, no scaling upward is performed. | SDK |
| `UpOnly` | The content scales upward only when it is smaller than the parent. If the content is larger, no scaling downward is performed. | SDK |

## System.Windows.Controls.TemplatedVisualTreeExtensions

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Common/TemplatedVisualTreeExtensions.cs`

A static class providing methods for working with the visual tree using generics.

## System.Windows.Controls.TextBlock

Source: SDK

Provides a lightweight control for displaying small amounts of text..

### Properties

| Member | Description | Src |
|---|---|---|
| `BaselineOffset` | Returns a value by which each line of text is offset from a baseline. <br>Returns: The amount by which each line of text is offset from the baseline, in device independent pixels. . `NaN` indicates that an optimal baseline offset is automatically calculated from the current font characteristics. The default is `NaN`. | SDK |
| `CharacterSpacing` | Gets or sets the distance between characters of text in the control measured in 1000ths of the font size. <br>Returns: The distance between characters of text in the control measured in 1000ths of the font size. The default is 0. | SDK |
| `FontFamily` | Gets or sets the preferred top-level font family for the text content in this element. <br>Returns: A `FontFamily` object that specifies the preferred font family, or a primary preferred font family with one or more fallback font families. For information about defaults, see the `FontFamily` class topic. | SDK |
| `FontSize` | Gets or sets the font size for the text content in this element. <br>Returns: A non-negative value that specifies the font size, measured in pixels. The default is 11. | SDK |
| `FontSource` | Gets or sets the font source that is applied to the text for rendering content. <br>Returns: The font source that is used to render content in the text box. The default is null. | SDK |
| `FontStretch` | Gets or sets the font stretch for the text content in this element. <br>Returns: The requested font stretch, which is a `FontStretch` that is obtained from one of the `FontStretches` property values. The default is `Normal`. | SDK |
| `FontStyle` | Gets or sets the font style for the content in this element. <br>Returns: The requested font style, which is a `FontStyle` that is obtained from one of the `FontStyles` property values. The default is `Normal`. | SDK |
| `FontWeight` | Gets or sets the top-level font weight for the `TextBlock`. <br>Returns: The requested font weight, which is a `FontWeight` that is obtained from one of the `FontWeights` property values. The default is `Normal`. | SDK |
| `Foreground` | Gets or sets the `Brush` to apply to the text contents of the `TextBlock`. <br>Returns: The brush used to apply to the text contents. The default is a `SolidColorBrush` with a `Color` value of `Black`. | SDK |
| `Inlines` | Gets the collection of inline text elements within a `TextBlock`. <br>Returns: A collection that holds all inline text elements from the `TextBlock`.The default is an empty collection. | SDK |
| `LineHeight` | Gets or sets the height of each line of content. <br>Returns: The height of each line in pixels. A value of 0 indicates that the line height is determined automatically from the current font characteristics. The default is 0. | SDK |
| `LineStackingStrategy` | Gets or sets a value that indicates how a line box is determined for each line of text in the `TextBlock`. <br>Returns: A value that indicates how a line box is determined for each line of text in the `TextBlock`. The default is `MaxHeight`. | SDK |
| `Padding` | Gets or sets a value that indicates the thickness of padding space between the boundaries of the content area and the content displayed by a `TextBlock`. <br>Returns: A `Thickness` structure that specifies the amount of padding to apply. | SDK |
| `Text` | Gets or sets the text contents of a `TextBlock`. <br>Returns: A string that specifies the text contents of this `TextBlock`. The default is an empty string. | SDK |
| `TextAlignment` | Gets or sets a value that indicates the horizontal alignment of text content. <br>Returns: The text alignment. The default is `Left`. | SDK |
| `TextDecorations` | Gets or sets a value that specifies the text decorations that are applied to the content in a `TextBlock` element. <br>Returns: A `TextDecorationCollection`, or null if no text decorations are applied. | SDK |
| `TextTrimming` | Gets or sets the text trimming behavior to employ when content overflows the content area. <br>Returns: One of the `TextTrimming` values that specifies the text trimming behavior to employ. The default is `None`. | SDK |
| `TextWrapping` | Gets or sets how the `TextBlock` wraps text. <br>Returns: A value that indicates how the `TextBlock` wraps text. The default is `NoWrap`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `CharacterSpacingProperty` | Identifies the `CharacterSpacing` dependency property. <br>Returns: The identifier for the `CharacterSpacing` dependency property. | SDK |
| `FontFamilyProperty` | Identifies the `FontFamily` dependency property. <br>Returns: The identifier for the `FontFamily` dependency property. | SDK |
| `FontSizeProperty` | Identifies the `FontSize` dependency property. <br>Returns: The identifier for the `FontSize` dependency property. | SDK |
| `FontStretchProperty` | Identifies the `FontStretch` dependency property. <br>Returns: The identifier for the `FontStretch` dependency property. | SDK |
| `FontStyleProperty` | Identifies the `FontStyle` dependency property. <br>Returns: The identifier for the `FontStyle` dependency property. | SDK |
| `FontWeightProperty` | Identifies the `FontWeight` dependency property. <br>Returns: The identifier for the `FontWeight` dependency property. | SDK |
| `ForegroundProperty` | Identifies the `Foreground` dependency property. <br>Returns: The identifier for the `Foreground` dependency property. | SDK |
| `LineHeightProperty` | Identifies the `LineHeight` dependency property. <br>Returns: The identifier for the `LineHeight` dependency property. | SDK |
| `LineStackingStrategyProperty` | Identifies the `LineStackingStrategy` dependency property. <br>Returns: The identifier for the `LineStackingStrategy` dependency property. | SDK |
| `PaddingProperty` | Identifies the `Padding` dependency property. <br>Returns: The identifier for the `Padding` dependency property. | SDK |
| `TextAlignmentProperty` | Identifies the `TextAlignment` dependency property. <br>Returns: The identifier for the `TextAlignment` dependency property. | SDK |
| `TextDecorationsProperty` | Identifies the `TextDecorations` dependency property. <br>Returns: The identifier for the `TextDecorations` dependency property. | SDK |
| `TextProperty` | Identifies the `Text` dependency property. <br>Returns: The identifier of the `Text` dependency property. | SDK |
| `TextTrimmingProperty` | Identifies the `TextTrimming` dependency property. <br>Returns: The identifier of the `TextTrimming` dependency property. | SDK |
| `TextWrappingProperty` | Identifies the `TextWrapping` dependency property. <br>Returns: The identifier of the `TextWrapping` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `TextBlock` class. | SDK |

## System.Windows.Controls.TextBox

Source: SDK

Represents a control that can be used to display single-format, multi-line text.

### Properties

| Member | Description | Src |
|---|---|---|
| `AcceptsReturn` | Gets or sets the value that determines whether the text box allows and displays the newline or return characters. <br>Returns: true if the text box allows newline characters; otherwise, false. The default is false. | SDK |
| `BaselineOffset` | Gets a value by which each line of text is offset from a baseline. <br>Returns: The amount by which each line of text is offset from the baseline, in device independent pixels. `NaN` indicates that an optimal baseline offset is automatically calculated from the current font characteristics. The default is `NaN`. | SDK |
| `CaretBrush` | Gets or sets the brush that is used to render the vertical bar that indicates the insertion point. <br>Returns: A brush that is used to render the vertical bar that indicates the insertion point. | SDK |
| `FontSource` | Gets or sets the font source that is applied to the `TextBox` for rendering content. <br>Returns: The font source used to render content in the text box. The default is null. | SDK |
| `HorizontalScrollBarVisibility` | Gets or sets the visibility of the horizontal scroll bar. <br>Returns: The visibility of the horizontal scroll bar. The default is `Hidden`. | SDK |
| `InputScope` | Gets or sets the context for input used by this `TextBox`. <br>Returns: The input scope, which modifies how input from alternative input methods is interpreted. The default value is a null reference (Nothing in Visual Basic). | SDK |
| `IsReadOnly` | Gets or sets the value that determines if the user can change the text in the text box. <br>Returns: true if the text box is read-only; otherwise, false. The default is false. | SDK |
| `LineHeight` | Gets or sets the height of each line of content. <br>Returns: The height of each line in pixels. A value of 0 indicates that the line height is determined automatically from the current font characteristics. The default is 0. | SDK |
| `LineStackingStrategy` | Gets or sets a value that indicates how a line box is determined for each line of text in the `TextBox`. <br>Returns: A value that indicates how a line box is determined for each line of text in the `TextBox`. The default is `MaxHeight`.. | SDK |
| `MaxLength` | Gets or sets the value that determines the maximum number of characters allowed for user input. <br>Returns: The maximum number of characters allowed for user input. The default is 0. | SDK |
| `SelectedText` | Gets or sets the content of the current selection in the text box. <br>Returns: The currently selected text in the text box. If no text is selected, the value is `Empty`. | SDK |
| `SelectionBackground` | Gets or sets the brush that fills the background of the selected text. <br>Returns: The brush that fills the background of the selected text. | SDK |
| `SelectionForeground` | Gets or sets the brush used for the selected text in the text box. <br>Returns: The brush used for the selected text in the text box. | SDK |
| `SelectionLength` | Gets or sets the number of characters in the current selection in the text box. <br>Returns: The number of characters in the current selection in the text box, or 0 if there is no selection. | SDK |
| `SelectionStart` | Gets or sets the starting position of the text selected in the text box. <br>Returns: The starting position of the current selection. | SDK |
| `Text` | Gets or sets the text contents of the text box. <br>Returns: A string containing the text contents of the text box. The default is an empty string (""). | SDK |
| `TextAlignment` | Gets or sets how the text should be aligned in the text box. <br>Returns: One of the `TextAlignment` enumeration values. The default is `Left`. | SDK |
| `TextWrapping` | Gets or sets how line breaking occurs if a line of text extends beyond the available width of the text box. <br>Returns: One of the `TextWrapping` values. The default is `NoWrap`. | SDK |
| `VerticalScrollBarVisibility` | Gets or sets the visibility of the vertical scroll bar. <br>Returns: The visibility of the vertical scroll bar. The default is `Hidden`. | SDK |
| `Watermark` | Gets or sets the content displayed as a watermark in the `TextBox` when it is empty. <br>Returns: The content displayed as a watermark in the `TextBox` when it is empty. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AcceptsReturnProperty` | Identifies the `AcceptsReturn` dependency property. <br>Returns: The identifier for the `AcceptsReturn` dependency property. | SDK |
| `CaretBrushProperty` | Identifies the `CaretBrush` dependency property. <br>Returns: The identifier for the `CaretBrush` dependency property. | SDK |
| `InputScopeProperty` | Identifies the `InputScope` dependency property. <br>Returns: Identifier for the `InputScope` dependency property. | SDK |
| `IsReadOnlyProperty` | Identifies the `IsReadOnly` dependency property. <br>Returns: The identifier for the `IsReadOnly` dependency property. | SDK |
| `LineHeightProperty` | Identifies the `LineHeight` dependency property. <br>Returns: Identifier for the `LineHeight` dependency property. | SDK |
| `LineStackingStrategyProperty` | Identifies the `LineStackingStrategy` dependency property. <br>Returns: Identifier for the `LineStackingStrategy` dependency property. | SDK |
| `MaxLengthProperty` | Identifies the `MaxLength` dependency property. <br>Returns: The identifier for the `MaxLength` dependency property. | SDK |
| `SelectionBackgroundProperty` | Identifies the `SelectionBackground` dependency property. <br>Returns: The identifier for the `SelectionBackground` dependency property. | SDK |
| `SelectionForegroundProperty` | Identifies the `SelectionForeground` dependency property. <br>Returns: The identifier for the `SelectionForeground` dependency property. | SDK |
| `TextAlignmentProperty` | Identifies the `TextAlignment` dependency property. <br>Returns: The identifier for the `TextAlignment` dependency property. | SDK |
| `TextProperty` | Identifies the `Text` dependency property. <br>Returns: The identifier for the `Text` dependency property. | SDK |
| `TextWrappingProperty` | Identifies the `TextWrapping` dependency property. <br>Returns: The identifier for the `TextWrapping` dependency property. | SDK |
| `WatermarkProperty` | Identifies the `Watermark` dependency property. <br>Returns: Identifier for the `Watermark` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `SelectionChanged` | Occurs when the text selection has changed. | SDK |
| `TextChanged` | Occurs when content changes in the text box. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `TextBox` class. | SDK |
| `GetRectFromCharacterIndex(Int32)` | Returns a rectangle for the leading edge of the character at the specified index. <br>`charIndex`: A zero-based character index of the character for which to retrieve the rectangle. <br>Returns: A rectangle for the leading edge of the character at the specified character index, or `Empty` if a bounding rectangle cannot be determined. | SDK |
| `GetRectFromCharacterIndex(Int32,Boolean)` | Returns a rectangle for the leading or trailing edge of the character at the specified index. <br>`charIndex`: A zero-based character index of the character for which to retrieve the rectangle. <br>`trailingEdge`: true to get the trailing edge of the character; false to get the leading edge of the character. <br>Returns: A rectangle for an edge of the character at the specified character index, or `Empty` if a bounding rectangle cannot be determined. | SDK |
| `OnCreateAutomationPeer` | Returns a `TextBoxAutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: A `TextBoxAutomationPeer` for the `TextBox` object. | SDK |
| `OnGotFocus(RoutedEventArgs)` | Called before `GotFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnKeyDown(Input.ZeyEventArgs)` | Called when `ZeyDown` event occurs. <br>`e`: The data for the event. | SDK |
| `OnKeyUp(Input.ZeyEventArgs)` | Called before `ZeyUp` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostFocus(RoutedEventArgs)` | Called before `LostFocus` event occurs. <br>`e`: The data for the event. | SDK |
| `OnLostMouseCapture(Input.MouseEventArgs)` | Provides handling for the `LostMouseCapture` event. <br>`e`: A `MouseEventArgs` that contains the event data. | SDK |
| `OnMouseEnter(Input.MouseEventArgs)` | Called before `MouseEnter` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeave(Input.MouseEventArgs)` | Called before `MouseLeave` event occurs. <br>`e`: The data for the event. | SDK |
| `OnMouseLeftButtonDown(Input.MouseButtonEventArgs)` | Called before `MouseLeftButtonDown` event occurs. <br>`e`: The data for the event. The event data reports that the left mouse button was pressed. | SDK |
| `OnMouseLeftButtonUp(Input.MouseButtonEventArgs)` | Called before `MouseLeftButtonUp` event occurs. <br>`e`: The data for the event. The event data reports that the left mouse button was released. | SDK |
| `OnMouseMove(Input.MouseEventArgs)` | Called before `MouseMove` event occurs. <br>`e`: The data for the event. | SDK |
| `OnTextInput(Input.TextCompositionEventArgs)` | Called before the `TextInput` event occurs. <br>`e`: Provides data about the event. | SDK |
| `OnTextInputStart(Input.TextCompositionEventArgs)` | Called before the `TextInputStart` event occurs. <br>`e`: Provides data about the event. | SDK |
| `OnTextInputUpdate(Input.TextCompositionEventArgs)` | Called before the `TextInputUpdate` event occurs. <br>`e`: Provides data about the event. | SDK |
| `Select(Int32,Int32)` | Selects a range of text in the text box. <br>`start`: The zero-based index of the first character in the selection. <br>`length`: The length of the selection, in characters. | SDK |
| `SelectAll` | Selects the entire contents of the text box. | SDK |

## System.Windows.Controls.TextChangedEventArgs

Source: SDK

Provides data for the `TextChanged` event.

## System.Windows.Controls.TextChangedEventHandler

Source: SDK

Represents the method that will handle the `TextChanged` event.

## System.Windows.Controls.TextSearch

Source: SDK

Enables the user to search a list of items in an `ItemsControl` using keyboard input.

### Properties

| Member | Description | Src |
|---|---|---|
| `TextPath` | Gets or sets the name of the items' property that identifies each item in a control's collection. <br>Returns: The name of the property that contains the text that identifies the item to the user. The default value is an empty string. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `TextPathProperty` | Identifies the `TextPath` attached property. <br>Returns: The identifier for the `TextPath` attached property | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `GetTextPath(DependencyObject)` | Returns the name of the property that identifies an item in the specified element's collection. <br>`element`: The element from which the property value is read. <br>Returns: The name of the property that identifies the item to the user. | SDK |
| `SetTextPath(DependencyObject,String)` | Writes the `TextPath` attached property to the specified element. <br>`element`: The element to which the property value is written. <br>`path`: The name of the property that identifies an item. | SDK |

## System.Windows.Controls.TimeTypeConverter

Source: Toolkit · class : TypeConverter · `Microsoft.Phone.Controls.Toolkit/Common/TimeTypeConverter.cs`

Allows time to be set from xaml.

### Methods

| Member | Description | Src |
|---|---|---|
| `bool CanConvertFrom(ITypeDescriptorContext typeDescriptorContext, Type sourceType)` | Determines whether this instance can convert from the specified type descriptor context. <br>`typeDescriptorContext`: The type descriptor context. <br>`sourceType`: Type of the source. <br>Returns: `True` if this instance can convert from the specified type descriptor context; otherwise, `false`. | TK |
| `bool CanConvertTo(ITypeDescriptorContext typeDescriptorContext, Type destinationType)` | Determines whether this instance can convert to the specified type descriptor context. <br>`typeDescriptorContext`: The type descriptor context. <br>`destinationType`: Type of the destination. <br>Returns: `True` if this instance can convert to the specified type descriptor context; otherwise, `false`. | TK |
| `object ConvertFrom(ITypeDescriptorContext typeDescriptorContext, CultureInfo cultureInfo, object source)` | Converts instances of other data types into instances of DateTime that represent a time. <br>`typeDescriptorContext`: The type descriptor context. <br>`cultureInfo`: The culture used to convert. This culture is not used during conversion, but a specific set of formats is used. <br>`source`: The string being converted to the DateTime. <br>Returns: A DateTime that is the value of the conversion. | TK |
| `object ConvertTo(ITypeDescriptorContext typeDescriptorContext, CultureInfo cultureInfo, object value, Type destinationType)` | Converts a DateTime into a string. <br>`typeDescriptorContext`: The type descriptor context. <br>`cultureInfo`: The culture used to convert. <br>`value`: The value that is being converted to a specified type. <br>`destinationType`: The type to convert the value to. <br>Returns: The value of the conversion to the specified type. | TK |

## System.Windows.Controls.ToolTip

Source: SDK

Represents a control that creates a pop-up window that displays information for an element in the UI.

### Properties

| Member | Description | Src |
|---|---|---|
| `HorizontalOffset` | Gets or sets the horizontal distance between the target origin and the pop-up alignment point. <br>Returns: The horizontal distance between the target origin and the pop-up alignment point. The default is 0. | SDK |
| `IsOpen` | Gets or sets a value that indicates whether the `ToolTip` is visible. <br>Returns: true if the `ToolTip` is visible; otherwise, false. The default is false. | SDK |
| `Placement` | Gets or sets how the `ToolTip` should be positioned in relation to the `PlacementTarget`. <br>Returns: One of the `PlacementMode` values. The default is `Mouse`. | SDK |
| `PlacementTarget` | Gets or sets the visual element or control that the tool tip should be positioned in relation to when opened by the `ToolTipService`. <br>Returns: The visual element or control that the tool tip should be positioned in relation to when opened by the `ToolTipService`. The default is null. | SDK |
| `VerticalOffset` | Gets or sets the vertical distance between the target origin and the pop-up alignment point. <br>Returns: The vertical distance between the target origin and the pop-up alignment point. The default is 0. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `HorizontalOffsetProperty` | Identifies the `HorizontalOffset` dependency property. <br>Returns: The identifier for the `HorizontalOffset` dependency property. | SDK |
| `IsOpenProperty` | Identifies the `IsOpen` dependency property. <br>Returns: The identifier for the `IsOpen` dependency property. | SDK |
| `PlacementProperty` | Identifies the `Placement` dependency property. <br>Returns: The identifier for the `Placement`dependency property. | SDK |
| `PlacementTargetProperty` | Identifies the `PlacementTarget` dependency property. <br>Returns: The identifier for the `PlacementTarget` dependency property. | SDK |
| `VerticalOffsetProperty` | Identifies the `VerticalOffset` dependency property. <br>Returns: The identifier for the `VerticalOffset` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Closed` | Occurs when a `ToolTip` is closed and is no longer visible. | SDK |
| `Opened` | Occurs when a `ToolTip` becomes visible. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ToolTip` class. | SDK |
| `OnApplyTemplate` | Builds the visual tree for the `ToolTip` when a new template is applied. | SDK |
| `OnCreateAutomationPeer` | Returns an `AutomationPeer` for use by the Silverlight automation infrastructure. <br>Returns: An `AutomationPeer` for the `ToolTip` object. | SDK |

## System.Windows.Controls.ToolTipService

Source: SDK

Represents a service that provides static methods to display a tooltip.

### Properties

| Member | Description | Src |
|---|---|---|
| `Placement` | Gets or sets the position of the tooltip in relation to its target visual element. <br>Returns: The position of the tooltip in relation to its target visual element. | SDK |
| `PlacementTarget` | Gets or sets the visual element that the tooltip should be positioned in relation to. <br>Returns: The visual element that the tooltip should be displayed in relation to. | SDK |
| `ToolTip` | Gets or sets a tooltip to be attached to a control. <br>Returns: The tooltip to display for a control. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `PlacementProperty` | Identifies the `Placement` dependency property. <br>Returns: The identifier for the `Placement` dependency property. | SDK |
| `PlacementTargetProperty` | Identifies the `PlacementTarget` dependency property. <br>Returns: The identifier for the `PlacementTarget` dependency property. | SDK |
| `ToolTipProperty` | Identifies the `ToolTip` dependency property. <br>Returns: The identifier for the `ToolTip` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `GetPlacement(DependencyObject)` | Gets the relative position of the specified tooltip. <br>`element`: The tooltip retrieve the position of. <br>Returns: The relative position of the specified tooltip. | SDK |
| `GetPlacementTarget(DependencyObject)` | Gets the visual element that the tooltip is positioned relative to. <br>`element`: The tooltip to retrieve the visual element of. <br>Returns: The visual element that the tooltip is positioned relative to. | SDK |
| `GetToolTip(DependencyObject)` | Gets the tooltip for an object. <br>`element`: The object from which the property value is read. <br>Returns: The object's tooltip value. | SDK |
| `SetPlacement(DependencyObject,Primitives.PlacementMode)` | Sets the position of the specified `ToolTip` using the specified placement mode. <br>`element`: The tooltip to set the position of. <br>`value`: One of the `PlacementMode` values that specify where the tooltip should appear relative to the control that is the placement target. | SDK |
| `SetPlacementTarget(DependencyObject,UIElement)` | Sets the position of the specified `ToolTip` relative to the specified value element. <br>`element`: The tooltip to set the position of. <br>`value`: The visual element to set the tooltip for. | SDK |
| `SetToolTip(DependencyObject,Object)` | Sets the tooltip for an object. <br>`element`: The object to which the attached property is written. <br>`value`: The value to set. | SDK |

## System.Windows.Controls.UIElementCollection

Source: SDK

Represents an ordered collection of `UIElement` objects.

## System.Windows.Controls.UserControl

Source: SDK

Provides the base class for defining a new control that encapsulates related existing controls and provides its own logic.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | Gets or sets the content that is contained within a user control. <br>Returns: The content of the user control. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ContentProperty` | Identifies the `Content` dependency property. <br>Returns: The identifier for the `Content` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `UserControl` class. | SDK |

## System.Windows.Controls.Validation

Source: SDK

Provides methods and attached properties that support data validation and govern the visual state of the control.

### Properties

| Member | Description | Src |
|---|---|---|
| `Errors` | Gets a collection of `ValidationError` objects associated with the binding target element. <br>Returns: A collection of `ValidationError` objects associated with the binding target element. | SDK |
| `HasError` | Gets a value that indicates whether any binding on the target element has a validation error. <br>Returns: true if any binding on the target element has a validation error; otherwise, false. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ErrorsProperty` | Identifies the `Errors` attached property. | SDK |
| `HasErrorProperty` | Identifies the `HasError` attached property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `GetErrors(DependencyObject)` | Gets the value of the `Errors` attached property for the specified element. <br>`element`: The element to retrieve the errors for. <br>Returns: A collection of `ValidationError` objects associated with the specified element. | SDK |
| `GetHasError(DependencyObject)` | Gets the value of the `HasError` attached property for the specified element. <br>`element`: The element to check for errors. <br>Returns: true if any binding on the specified element has a validation error; otherwise, false. | SDK |

## System.Windows.Controls.ValidationError

Source: SDK

Represents a validation error that is generated by the binding engine when an exception occurs during target-to-source updates.

### Properties

| Member | Description | Src |
|---|---|---|
| `ErrorContent` | Gets the error message. <br>Returns: The error message. | SDK |
| `Exception` | Gets the exception that caused the validation error. <br>Returns: The exception that caused the validation error. | SDK |

## System.Windows.Controls.ValidationErrorEventAction

Source: SDK

Describes the reason a `BindingValidationError` event has occurred.

### Fields

| Member | Description | Src |
|---|---|---|
| `Added` | A new `ValidationError` has occurred. | SDK |
| `Removed` | An existing `ValidationError` has been removed. | SDK |

## System.Windows.Controls.ValidationErrorEventArgs

Source: SDK

Provides data for the `BindingValidationError` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Action` | Gets the state of the validation error. <br>Returns: One of the enumeration values that indicates the state of the validation error. | SDK |
| `Error` | Gets the validation error that caused the `BindingValidationError` event. <br>Returns: A `ValidationError` object that contains the exception that caused the validation error. | SDK |
| `Handled` | Gets or sets the value that marks the routed event as handled. <br>Returns: true if the event is handled; otherwise, false. | SDK |

## System.Windows.Controls.Viewbox

Source: SDK

Defines a content decorator that can stretch and scale a single child to fill the available space.

### Properties

| Member | Description | Src |
|---|---|---|
| `Child` | Gets or sets the single child element of a `Viewbox` element. <br>Returns: The single child element of a `Viewbox` element. | SDK |
| `Stretch` | Gets or sets the `Stretch` mode, which determines how content fits into the available space. <br>Returns: A `Stretch` mode, which determines how content fits in the available space. The default is `Uniform`. | SDK |
| `StretchDirection` | Gets or sets the `StretchDirection`, which determines how scaling is applied to the contents of a `Viewbox`. <br>Returns: A `StretchDirection`, which determines how scaling is applied to the contents of a `Viewbox`. The default is `Both`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `StretchDirectionProperty` | Identifies the `StretchDirection` dependency property. <br>Returns: The identifier for the `StretchDirection` dependency property. | SDK |
| `StretchProperty` | Identifies the `Stretch` dependency property. <br>Returns: The identifier for the `Stretch` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Viewbox` class. | SDK |

## System.Windows.Controls.VirtualizationMode

Source: SDK

Specifies the method the `VirtualizingStackPanel` uses to manage virtualizing its child items.

### Fields

| Member | Description | Src |
|---|---|---|
| `Recycling` | Reuse the item containers. | SDK |
| `Standard` | Create and discard the item containers. | SDK |

## System.Windows.Controls.VirtualizingPanel

Source: SDK

Provides a framework for `Panel` elements that virtualize their visual children.

### Properties

| Member | Description | Src |
|---|---|---|
| `ItemContainerGenerator` | Gets a value that identifies the `ItemContainerGenerator` for this `VirtualizingPanel`. <br>Returns: The `ItemContainerGenerator` for this `VirtualizingPanel`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `VirtualizingPanel` class. | SDK |
| `AddInternalChild(UIElement)` | Adds the specified `UIElement` to the `Children` collection of a `VirtualizingPanel` element. <br>`child`: The `UIElement` child to add to the collection. | SDK |
| `BringIndexIntoView(Int32)` | When implemented in a derived class, generates the item at the specified index location and makes it visible. <br>`index`: The index position of the item that is generated and made visible. | SDK |
| `InsertInternalChild(Int32,UIElement)` | Adds the specified `UIElement` to the collection of a `VirtualizingPanel` element at the specified index position. <br>`index`: The index position within the collection at which the child element is inserted. <br>`child`: The `UIElement` child to add to the collection. | SDK |
| `OnClearChildren` | Called when the collection of child elements is cleared by the base `Panel` class. | SDK |
| `OnItemsChanged(Object,Primitives.ItemsChangedEventArgs)` | Called when the `Items` collection that is associated with the `ItemsControl` for this `Panel` changes. <br>`sender`: The `Object` that raised the event. <br>`args`: Provides data for the `ItemsChanged` event. | SDK |
| `RemoveInternalChildRange(Int32,Int32)` | Removes child elements from the `Children` collection. <br>`index`: The beginning index position within the collection at which the first child element is removed. <br>`range`: The total number of child elements to remove from the collection. | SDK |

## System.Windows.Controls.VirtualizingStackPanel

Source: SDK

Arranges and virtualizes content on a single line that is oriented either horizontally or vertically.

### Properties

| Member | Description | Src |
|---|---|---|
| `CanHorizontallyScroll` | Gets or sets a value that indicates whether a `VirtualizingStackPanel` can scroll in the horizontal dimension. <br>Returns: true if content can scroll in the horizontal dimension; otherwise, false. The default is false. | SDK |
| `CanVerticallyScroll` | Gets or sets a value that indicates whether content can scroll in the vertical dimension. <br>Returns: true if content can scroll in the vertical dimension; otherwise, false. The default is false. | SDK |
| `ExtentHeight` | Gets a value that contains the vertical size of the extent. <br>Returns: A `Double` that represents the vertical size of the extent, in pixels. The default is 0. | SDK |
| `ExtentWidth` | Gets a value that contains the horizontal size of the extent. <br>Returns: A `Double` that represents the horizontal size of the extent, in pixels. The default is 0. | SDK |
| `HorizontalOffset` | Gets a value that contains the horizontal offset of the scrolled content. <br>Returns: A `Double` that represents the horizontal offset of the scrolled content, in pixels. The default is 0. | SDK |
| `IsVirtualizing` | Gets a value that indicates whether this `VirtualizingStackPanel` is virtualizing its child collection. <br>Returns: true if this instance of `VirtualizingStackPanel` is virtualizing its child collection; otherwise, false. The default is true. | SDK |
| `Orientation` | Gets or sets a value that describes the horizontal or vertical orientation of stacked content. <br>Returns: The `Orientation` of child content. The default is `Vertical`. | SDK |
| `ScrollOwner` | Gets or sets a value that identifies the container that controls scrolling behavior in this `VirtualizingStackPanel`. <br>Returns: The `ScrollViewer` that owns scrolling for this `VirtualizingStackPanel`. | SDK |
| `VerticalOffset` | Gets a value that represents how far down the content is currently scrolled. <br>Returns: A `Double` that represents the vertical offset of the scrolled content, in pixels. The default is 0. | SDK |
| `ViewportHeight` | Gets a value that contains the vertical size of the viewport (visible area) of the content. <br>Returns: A `Double` that represents the vertical size of the viewport (visible area) of the content, in pixels. The default is 0. | SDK |
| `ViewportWidth` | Gets a value that contains the horizontal size of the viewport (visible area) of the content. <br>Returns: A `Double` that represents the vertical size of the viewport (visible area) of the content, in pixels. The default is 0. | SDK |
| `VirtualizationMode` | Gets or sets how a panel in an `ItemsControl` virtualizes its child items. <br>Returns: One of the `VirtualizationMode` values. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsVirtualizingProperty` | A value that indicates whether the `VirtualizingStackPanel` is using virtualization. <br>Returns: A value that indicates whether the `VirtualizingStackPanel` is using virtualization. | SDK |
| `OrientationProperty` | Identifies the `Orientation` dependency property. <br>Returns: The identifier for the `Orientation` dependency property. | SDK |
| `VirtualizationModeProperty` | The current virtualization mode of the `VirtualizingStackPanel` (whether it is `Recycling` or not). | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `CleanUpVirtualizedItemEvent` | Occurs when an item that is hosted by the `VirtualizingStackPanel` is re-virtualized. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `VirtualizingStackPanel` class. | SDK |
| `ArrangeOverride(Size)` | Arranges the content of a `VirtualizingStackPanel` element. <br>`arrangeSize`: The `Size` that this element should use to arrange its child elements. <br>Returns: The `Size` that represents the arranged size of this `VirtualizingStackPanel` element and its child elements. | SDK |
| `GetIsVirtualizing(DependencyObject)` | Gets a value that determines whether the `VirtualizingStackPanel` is virtualizing its content. <br>`o`: The object being virtualized. <br>Returns: true if the `VirtualizingStackPanel` is virtualizing its content; otherwise false. | SDK |
| `GetVirtualizationMode(DependencyObject)` | Returns the `VirtualizationMode` for the specified object. <br>`element`: The object from which the `VirtualizationMode` is read. <br>Returns: One of the enumeration values that specifies whether the object uses container recycling. | SDK |
| `LineDown` | Scrolls content downward by one logical unit. | SDK |
| `LineLeft` | Scrolls content to the left by one logical unit. | SDK |
| `LineRight` | Scrolls content to the right by one logical unit. | SDK |
| `LineUp` | Scrolls content upward by one logical unit. | SDK |
| `MakeVisible(UIElement,Rect)` | Scrolls to the specified coordinates and makes that portion of a `UIElement` visible. <br>`visual`: The `UIElement` that becomes visible. <br>`rectangle`: A `Rect` that represents the coordinate space within a `UIElement`. <br>Returns: Rectangular area of the `UIElement` now visible. | SDK |
| `MeasureOverride(Size)` | Measures the child elements of a `VirtualizingStackPanel` in anticipation of arranging them during the `Size)` pass. <br>`constraint`: An upper limit `Size` that should not be exceeded. <br>Returns: The `Size` that represents the desired size of the element. | SDK |
| `MouseWheelDown` | Scrolls content logically downward in response to a downward click of the mouse wheel button. | SDK |
| `MouseWheelLeft` | Scrolls content logically to the left in response to a left click of the mouse wheel button. | SDK |
| `MouseWheelRight` | Scrolls content logically to the right in response to a right click of the mouse wheel button. | SDK |
| `MouseWheelUp` | Scrolls content logically upward in response to an upward click of the mouse wheel button. | SDK |
| `OnCleanUpVirtualizedItem(CleanUpVirtualizedItemEventArgs)` | Called when an item that is hosted by the `VirtualizingStackPanel` is re-virtualized. <br>`e`: Data about the event. | SDK |
| `OnClearChildren` | Called when the collection of child elements is cleared by the base `Panel` class. | SDK |
| `OnItemsChanged(Object,Primitives.ItemsChangedEventArgs)` | Called when the `Items` collection that is associated with the `ItemsControl` for this `Panel` changes. <br>`sender`: The `Object` that raised the event. <br>`args`: Provides data for the `ItemsChanged` event. | SDK |
| `PageDown` | Scrolls content downward by one page. | SDK |
| `PageLeft` | Scrolls content to the left by one page. | SDK |
| `PageRight` | Scrolls content to the right by one page. | SDK |
| `PageUp` | Scrolls content upward by one page. | SDK |
| `SetHorizontalOffset(Double)` | Sets the value of the `HorizontalOffset` property. <br>`offset`: The value of the `HorizontalOffset` property. | SDK |
| `SetVerticalOffset(Double)` | Sets the value of the `VerticalOffset` property. <br>`offset`: The value of the `VerticalOffset` property. | SDK |
| `SetVirtualizationMode(DependencyObject,VirtualizationMode)` | Sets the `VirtualizationMode` on the specified object. <br>`element`: The element on which to set the `VirtualizationMode`. <br>`value`: One of the enumeration values that specifies whether uses container recycling. | SDK |

## System.Windows.Controls.WebBrowser

Source: SDK

Hosts HTML content within the Silverlight plug-in.

### Properties

| Member | Description | Src |
|---|---|---|
| `Source` | Gets or sets the URI source of the HTML content to display in the `WebBrowser` control. <br>Returns: The URI source of the HTML content to display in the `WebBrowser` control. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `LoadCompleted` | Occurs when top-level navigation completes and the content loads into the `WebBrowser` control or when an error occurs during loading. | SDK |
| `ScriptNotify` | Occurs when the content contained in the `WebBrowser` control passes a string to the Silverlight plug-in by using JavaScript. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `WebBrowser` class. | SDK |
| `InvokeScript(String)` | Executes the specified script, which is defined in the currently loaded HTML. <br>`scriptName`: The name of the script to execute. <br>Returns: The result of the script invocation. | SDK |
| `InvokeScript(String,String[])` | Executes the specified script function, which is defined in the currently loaded HTML, with the specified arguments. <br>`scriptName`: The name of the script to execute. <br>`args`: The arguments to pass to the script function. <br>Returns: The result of the script invocation. | SDK |
| `Navigate(Uri)` | Loads the HTML content at the specified URI. <br>`source`: The URI of the HTML content to load. | SDK |
| `NavigateToString(String)` | Displays the specified HTML content. <br>`text`: The HTML content to display in the `WebBrowser` control. | SDK |
| `SaveToString` | Saves the source for the HTML content currently displayed in the `WebBrowser` as a string. <br>Returns: The string representation of the source for the currently displayed HTML content. | SDK |

## System.Windows.Controls.WebBrowserBrush

Source: SDK

Provides a brush that renders the currently hosted HTML.

### Properties

| Member | Description | Src |
|---|---|---|
| `SourceName` | Gets the name of the source `WebBrowser` control that provides the HTML content. <br>Returns: The `Name` of the `WebBrowser` that provides the HTML content. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `SourceNameProperty` | Identifies the `SourceName` dependency property. <br>Returns: The identifier for the `SourceName` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `WebBrowserBrush` class. | SDK |
| `Redraw` | Forces the brush to asynchronously redraw itself. | SDK |
| `SetSource(WebBrowser)` | Sets the source of the content for the `WebBrowserBrush`. <br>`source`: The `WebBrowser` hosting the HTML content that is the source for the brush. | SDK |

## System.Windows.Controls.WrapPanel

Source: Toolkit · class : Panel · `Microsoft.Phone.Controls.Toolkit/WrapPanel/WrapPanel.cs`

Positions child elements sequentially from left to right or top to bottom. When elements extend beyond the panel edge, elements are positioned in the next row or column.

### Properties

| Member | Description | Src |
|---|---|---|
| `double ItemHeight` | Gets or sets the height of the layout area for each item that is contained in a `WrapPanel`. <br>Value: The height applied to the layout area of each item that is contained within a `WrapPanel`. The default value is `NaN`. | TK |
| `double ItemWidth` | Gets or sets the width of the layout area for each item that is contained in a `WrapPanel`. <br>Value: The width that applies to the layout area of each item that is contained in a `WrapPanel`. The default value is `NaN`. | TK |
| `Orientation Orientation` | Gets or sets the direction in which child elements are arranged. <br>Value: One of the `Orientation` values. The default is `Horizontal`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ItemHeightProperty` | Identifies the `ItemHeight` dependency property. <br>Value: The identifier for the `ItemHeight` dependency property | TK |
| `static DependencyProperty ItemWidthProperty` | Identifies the `ItemWidth` dependency property. <br>Value: The identifier for the `ItemWidth` dependency property. | TK |
| `static DependencyProperty OrientationProperty` | Identifies the `Orientation` dependency property. <br>Value: The identifier for the `Orientation` dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» WrapPanel()` | Initializes a new instance of the `WrapPanel` class. | TK |
