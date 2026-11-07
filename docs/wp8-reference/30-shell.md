# Shell: ApplicationBar, SystemTray, tiles

## Contents

- [`Microsoft.Phone.Shell.ActivatedEventArgs`](#microsoftphoneshellactivatedeventargs)
- [`Microsoft.Phone.Shell.ApplicationBar`](#microsoftphoneshellapplicationbar)
- [`Microsoft.Phone.Shell.ApplicationBarIconButton`](#microsoftphoneshellapplicationbariconbutton)
- [`Microsoft.Phone.Shell.ApplicationBarMenuItem`](#microsoftphoneshellapplicationbarmenuitem)
- [`Microsoft.Phone.Shell.ApplicationBarMode`](#microsoftphoneshellapplicationbarmode)
- [`Microsoft.Phone.Shell.ApplicationBarStateChangedEventArgs`](#microsoftphoneshellapplicationbarstatechangedeventargs)
- [`Microsoft.Phone.Shell.ClosingEventArgs`](#microsoftphoneshellclosingeventargs)
- [`Microsoft.Phone.Shell.DeactivatedEventArgs`](#microsoftphoneshelldeactivatedeventargs)
- [`Microsoft.Phone.Shell.IApplicationBar`](#microsoftphoneshelliapplicationbar)
- [`Microsoft.Phone.Shell.IApplicationBarIconButton`](#microsoftphoneshelliapplicationbariconbutton)
- [`Microsoft.Phone.Shell.IApplicationBarMenuItem`](#microsoftphoneshelliapplicationbarmenuitem)
- [`Microsoft.Phone.Shell.IdleDetectionMode`](#microsoftphoneshellidledetectionmode)
- [`Microsoft.Phone.Shell.LaunchingEventArgs`](#microsoftphoneshelllaunchingeventargs)
- [`Microsoft.Phone.Shell.PhoneApplicationService`](#microsoftphoneshellphoneapplicationservice)
- [`Microsoft.Phone.Shell.ProgressIndicator`](#microsoftphoneshellprogressindicator)
- [`Microsoft.Phone.Shell.ShellTile`](#microsoftphoneshellshelltile)
- [`Microsoft.Phone.Shell.ShellTileData`](#microsoftphoneshellshelltiledata)
- [`Microsoft.Phone.Shell.ShellTileSchedule`](#microsoftphoneshellshelltileschedule)
- [`Microsoft.Phone.Shell.ShellToast`](#microsoftphoneshellshelltoast)
- [`Microsoft.Phone.Shell.StandardTileData`](#microsoftphoneshellstandardtiledata)
- [`Microsoft.Phone.Shell.StartupMode`](#microsoftphoneshellstartupmode)
- [`Microsoft.Phone.Shell.SystemTray`](#microsoftphoneshellsystemtray)
- [`Microsoft.Phone.Shell.UpdateInterval`](#microsoftphoneshellupdateinterval)
- [`Microsoft.Phone.Shell.UpdateRecurrence`](#microsoftphoneshellupdaterecurrence)

## Microsoft.Phone.Shell.ActivatedEventArgs

Source: SDK

Provides data for `Activated` events.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsApplicationInstancePreserved` | Indicates whether the application instance was preserved. <br>Returns: Whether the application instance was preserved. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ActivatedEventArgs` class. | SDK |

## Microsoft.Phone.Shell.ApplicationBar

Source: SDK

Represents an Application Bar in Windows Phone applications.

### Properties

| Member | Description | Src |
|---|---|---|
| `BackgroundColor` | Gets or sets the background color of the Application Bar. <br>Returns: The background color of the Application Bar. | SDK |
| `Buttons` | Gets the list of the buttons that appear on the Application Bar. <br>Returns: The Application Bar buttons. | SDK |
| `DefaultSize` | Gets the distance that the Application Bar extends into a page when the `Mode` property is set to `Default`. <br>Returns: The distance that the Application Bar extends into a page. | SDK |
| `ForegroundColor` | Gets or sets the foreground color of the Application Bar. <br>Returns: The foreground color of the Application Bar. | SDK |
| `IsMenuEnabled` | Gets or sets a value that indicates whether the user can open the menu. <br>Returns: true if the menu is enabled; otherwise, false. | SDK |
| `IsVisible` | Gets or sets a value that indicates whether the Application Bar is visible. <br>Returns: true if the Application Bar is visible; otherwise, false. | SDK |
| `MenuItems` | Gets the list of the menu items that appear on the Application Bar. <br>Returns: The list of menu items. | SDK |
| `MiniSize` | Gets the distance that the Application Bar extends into a page when the `Mode` property is set to `Minimized`. <br>Returns: The distance that the Application Bar extends into a page. | SDK |
| `Mode` | Gets or sets the size of the Application Bar. <br>Returns: One of the enumeration values that indicates the size of the Application Bar. | SDK |
| `Opacity` | Gets or sets the opacity of the Application Bar. <br>Returns: The opacity of the Application Bar. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `StateChanged` | Occurs when the user opens or closes the menu. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ApplicationBar` class. | SDK |

## Microsoft.Phone.Shell.ApplicationBarIconButton

Source: SDK

An Application Bar button with an icon.

### Properties

| Member | Description | Src |
|---|---|---|
| `IconUri` | The URI of the icon to use for the button. <br>Returns: The URI of the icon to use for the button. | SDK |
| `IsEnabled` | Gets or sets the enabled status of the button. <br>Returns: true if the button is enabled; otherwise, false. | SDK |
| `Text` | Gets or sets the text string that is displayed as a label for the button. The label is displayed when the user taps the ellipses symbol on the Application Bar. <br>Returns: Type: `String`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Click` | Occurs when the user taps a button in the Application Bar. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ApplicationBarIconButton` class. | SDK |
| `Constructor(Uri)` | Creates a new instance of the `ApplicationBarIconButton` class with the specified icon. <br>`iconUri`: The URI of the icon to use for the button. | SDK |

## Microsoft.Phone.Shell.ApplicationBarMenuItem

Source: SDK

An item that can be added to the menu of an `ApplicationBar`.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsEnabled` | Gets or sets the enabled status of the menu item. <br>Returns: true if the menu item is enabled; otherwise, false. | SDK |
| `Text` | The string to display on the menu item. <br>Returns: Type: `String`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Click` | Occurs when the user taps the menu item. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ApplicationBarMenuItem` class. | SDK |
| `Constructor(String)` | Creates a new instance of the `ApplicationBarMenuItem` class that displays the specified string. <br>`text`: The string to display on the menu item. | SDK |

## Microsoft.Phone.Shell.ApplicationBarMode

Source: SDK

Specifies the size of an Application Bar.

### Fields

| Member | Description | Src |
|---|---|---|
| `Default` | The Application Bar default size. A row of icon buttons and an ellipsis appear. | SDK |
| `Minimized` | The Application Bar minimized size. Only an ellipsis appears. | SDK |

## Microsoft.Phone.Shell.ApplicationBarStateChangedEventArgs

Source: SDK

Provides data for `StateChanged` events that occur when the user opens or closes the menu.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsMenuVisible` | Whether or not the Application Bar menu is visible. <br>Returns: true if the Application Bar menu is visible; otherwise, false. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Boolean)` | Creates a new instance of the `ApplicationBarStateChangedEventArgs` class. <br>`isMenuVisible`: Whether or not the Application Bar menu is visible. This property is set when the user opens or closes the menu. It cannot be set by the application to show the menu programmatically. | SDK |

## Microsoft.Phone.Shell.ClosingEventArgs

Source: SDK

Provides data for the `Closing` event.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ClosingEventArgs` class. | SDK |

## Microsoft.Phone.Shell.DeactivatedEventArgs

Source: SDK

Provides data for the `Deactivated` event.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `DeactivatedEventArgs` class. | SDK |

## Microsoft.Phone.Shell.IApplicationBar

Source: SDK

Defines events and properties for the Application Bar in Windows Phone applications.

### Properties

| Member | Description | Src |
|---|---|---|
| `BackgroundColor` | Gets or sets the background color of the Application Bar. <br>Returns: The background color of the Application Bar. | SDK |
| `Buttons` | Gets the list of the buttons that appear on the Application Bar. <br>Returns: The Application Bar buttons. | SDK |
| `DefaultSize` | Gets the distance that the Application Bar extends into a page when the `Mode` property is set to `Default`. <br>Returns: The distance that the Application Bar extends into a page. | SDK |
| `ForegroundColor` | Gets or sets the foreground color of the Application Bar. <br>Returns: The foreground color of the Application Bar. | SDK |
| `IsMenuEnabled` | Gets or sets a value that indicates whether the user can open the menu. <br>Returns: true if the menu is enabled; otherwise, false. | SDK |
| `IsVisible` | Gets or sets a value that indicates whether the Application Bar is visible. <br>Returns: true if the Application Bar is visible; otherwise, false. | SDK |
| `MenuItems` | Gets the list of the menu items that appear on the Application Bar. <br>Returns: The list of menu items. | SDK |
| `MiniSize` | Gets the distance that the Application Bar extends into a page when the `Mode` property is set to `Minimized`. <br>Returns: The distance that the Application Bar extends into a page. | SDK |
| `Mode` | Gets or sets the size of the Application Bar. <br>Returns: One of the enumeration values that indicates the size of the Application Bar. | SDK |
| `Opacity` | Gets or sets the opacity of the Application Bar. <br>Returns: The opacity of the Application Bar. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `StateChanged` | Occurs when the user opens or closes the menu. | SDK |

## Microsoft.Phone.Shell.IApplicationBarIconButton

Source: SDK

The interface from which `ApplicationBarIconButton` inherits.

### Properties

| Member | Description | Src |
|---|---|---|
| `IconUri` | The URI of the icon to use for the button. <br>Returns: Type: `Uri`. | SDK |

## Microsoft.Phone.Shell.IApplicationBarMenuItem

Source: SDK

The interface from which `ApplicationBarMenuItem` inherits.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsEnabled` | Gets or sets the enabled status of the menu item. <br>Returns: true if the menu item is enabled; otherwise, false. | SDK |
| `Text` | The string to display on the menu item. <br>Returns: Type: `String`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Click` | Occurs when the user taps the menu item. | SDK |

## Microsoft.Phone.Shell.IdleDetectionMode

Source: SDK

Defines the status for idle detection of users and applications. Used with the `UserIdleDetectionMode` and `ApplicationIdleDetectionMode` properties.

### Fields

| Member | Description | Src |
|---|---|---|
| `Disabled` | When `UserIdleDetectionMode` is set to disabled, the operating system will not consider an application to be idle when no touch events are detected. When `ApplicationIdleDetectionMode` is set to disabled, the active application will continue to run when the lock screen is engaged. | SDK |
| `Enabled` | When `UserIdleDetectionMode` is set to enabled, the operating system will consider an application to be idle when no touch events are detected. When `ApplicationIdleDetectionMode` is set to enabled, the operating system will pause the active application when the lock screen is engaged. | SDK |

## Microsoft.Phone.Shell.LaunchingEventArgs

Source: SDK

Provides data for `Launching` events.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `LaunchingEventArgs` class. | SDK |

## Microsoft.Phone.Shell.PhoneApplicationService

Source: SDK

The `PhoneApplicationService` class provides access to various aspects of the application’s lifetime. This includes management of the application’s idle behavior and management of the application’s state when it becomes active or inactive.

### Properties

| Member | Description | Src |
|---|---|---|
| `ApplicationIdleDetectionMode` | Gets or sets whether application idle detection is enabled. <br>Returns: Type: `IdleDetectionMode`. | SDK |
| `Current` | Gets the `PhoneApplicationService` object associated with the current application. <br>Returns: Type: `PhoneApplicationService`. | SDK |
| `StartupMode` | Gets the mode in which the application was started. <br>Returns: Type:`StartupMode`. | SDK |
| `State` | Gets the dictionary used for passing an application’s state between invocations. <br>Returns: Type: `IDictionary`2`. | SDK |
| `UserIdleDetectionMode` | Gets or sets whether user idle detection is enabled. <br>Returns: Type: `IdleDetectionMode`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `Activated` | Occurs when the application is being made active after previously being put into a dormant state or tombstoned. | SDK |
| `Closing` | Occurs when the application is exiting. | SDK |
| `Deactivated` | Occurs when the application is being deactivated. | SDK |
| `Launching` | Occurs when the application is being launched. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Instantiates a new instance of the `PhoneApplicationService` class. | SDK |
| `StartService(ApplicationServiceContext)` | This method is used by the underlying implementation of the `PhoneApplicationService` and should not be used by applications. <br>`context`: This parameter is not used. | SDK |
| `StopService` | This method is used by the underlying implementation of the `PhoneApplicationService` and should not be used by applications. | SDK |

## Microsoft.Phone.Shell.ProgressIndicator

Source: SDK

Provides methods and properties for interacting with the progress indicator on the system tray on an application page.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsIndeterminate` | Gets or sets a value that indicates whether the progress indicator on the system tray on the current application page is determinate or indeterminate. <br>Returns: true if the progress indicator is indeterminate; false if the progress bar is determinate. | SDK |
| `IsVisible` | Gets or sets the visibility of the progress indicator on the system tray on the current application page. <br>Returns: true if the progress indicator is visible; otherwise, false. | SDK |
| `Text` | Gets or sets the text of the progress indicator on the system tray on the current application page. <br>Returns: The text of the progress indicator on the system tray. | SDK |
| `Value` | Gets or sets the value of the progress indicator on the system tray on the current application page. <br>Returns: The value of the progress indicator on the system tray. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `IsIndeterminateProperty` | The dependency property for `IsIndeterminate`. | SDK |
| `IsVisibleProperty` | The dependency property for `IsVisible`. | SDK |
| `TextProperty` | The dependency property for `Text`. | SDK |
| `ValueProperty` | The dependency property for `Value`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `ProgressIndicator` class. | SDK |

## Microsoft.Phone.Shell.ShellTile

Source: SDK

Class for managing the Application Tile and secondary Tiles for an application.

### Properties

| Member | Description | Src |
|---|---|---|
| `ActiveTiles` | Contains the collection of an applications tiles pinned to Start. <br>Returns: Returns `IEnumerable`1`. | SDK |
| `NavigationUri` | Contains the URI and launch parameter this is navigated to when a secondary tile is tapped. <br>Returns: Returns `Uri`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Create(Uri,Microsoft.Phone.Shell.ShellTileData)` | Creates a new secondary tile. <br>`navigationUri`: URI for the tile being created. The URI can contain custom launch parameters. <br>`initialData`: Text and image information for the tile being created. | SDK |
| `Delete` | Unpins and deletes a secondary tile. | SDK |
| `Update(Microsoft.Phone.Shell.ShellTileData)` | Updates an Application Tile or secondary Tile. <br>`data`: New text and image data for the tile. | SDK |

## Microsoft.Phone.Shell.ShellTileData

Source: SDK

Base class for tile data.

### Properties

| Member | Description | Src |
|---|---|---|
| `Title` | Text to display in the tile. <br>Returns: Returns `String`. | SDK |

## Microsoft.Phone.Shell.ShellTileSchedule

Source: SDK

Creates an object which allows an application to schedule updates of its tile's background image.

### Properties

| Member | Description | Src |
|---|---|---|
| `Interval` | Sets the interval of how often the background image of the tile is updated. <br>Returns: Returns `UpdateInterval`. | SDK |
| `MaxUpdateCount` | Defines the number of times the schedule will run. If this value is not set or set to a number less than 1, then the schedule will run indefinitely. <br>Returns: Returns `Int32`. | SDK |
| `Recurrence` | Defines whether the schedule will be executed one time or multiple times. The recurrence values are defined in `UpdateRecurrence`. <br>Returns: Returns `UpdateRecurrence`. | SDK |
| `RemoteImageUri` | The fully qualified URI of the background image. <br>Returns: Returns `Uri`. | SDK |
| `StartTime` | Defines the start time of the schedule. <br>Returns: Returns `DateTime`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates an instance of the ShellTileSchedule class. | SDK |
| `Constructor(Microsoft.Phone.Shell.ShellTile)` | Creates a ShellTileSchedule for the given ShellTile. <br>`tileId`: ShellTile to create schedule for. | SDK |
| `Start` | Starts the schedule. | SDK |
| `Stop` | Cancels an existing tile schedule. | SDK |

## Microsoft.Phone.Shell.ShellToast

Source: SDK

A class for displaying a toast to the user. While the `Show` method can be called from any application, a toast will not be displayed if the current foreground application is the caller of the ShellToast.Show method. This means that this should primarily be used by a background agent.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | The contents of the toast message. <br>Returns: Returns `String`. | SDK |
| `NavigationUri` | Uri to navigate to if the user taps the toast message. <br>Returns: Returns `Uri`. | SDK |
| `Title` | The title of the toast message. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Class constructor. | SDK |
| `Show` | Display the toast message. While the Show method can be called from any application, a toast will not be displayed if the current foreground application is the caller of the Show method. This means that this should primarily be used by a background agent. | SDK |

## Microsoft.Phone.Shell.StandardTileData

Source: SDK

Data for a tile pinned to Start. Tiles have a ‘front’ and ‘back’ to them and this class holds all this data.

### Properties

| Member | Description | Src |
|---|---|---|
| `BackBackgroundImage` | Background image of the back of the Tile. If this property is an empty Uri, the background image of the back of the tile will not change during an update. <br>Returns: Returns `Uri`. | SDK |
| `BackContent` | Text to display on the back of the tile, above the title. If this property is an empty string, the content on the back of the tile will not change during an update. <br>Returns: Returns `String`. | SDK |
| `BackTitle` | Title to display at the bottom of the back of the Tile. If this property is an empty string, the title of the back of the tile will not change during an update. <br>Returns: Returns `String`. | SDK |
| `BackgroundImage` | Background image of the front of the Tile. If this property is an empty Uri, the background image of the front of the tile will not change during an update. <br>Returns: Returns `Uri`. | SDK |
| `Count` | A value between 1 and 99 will be displayed in the Count field of the tile. A value of 0 means the Count will not be displayed. If this property is not set, the Count display will not change during an update. <br>Returns: Returns `Nullable`1`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Class constructor. | SDK |

## Microsoft.Phone.Shell.StartupMode

Source: SDK

Lists the modes in which an application can be started. Used by the `StartupMode` property.

### Fields

| Member | Description | Src |
|---|---|---|
| `Activate` | The application was launched by the user returning to the application by pressing the Back button. | SDK |
| `Launch` | The application was launched by the user from the Start experience. | SDK |

## Microsoft.Phone.Shell.SystemTray

Source: SDK

Provides methods and properties for interacting with the system tray on an application page.

### Properties

| Member | Description | Src |
|---|---|---|
| `BackgroundColor` | Gets or sets the background color of the system tray on the current application page. <br>Returns: The background color of the system tray on the current application page. | SDK |
| `ForegroundColor` | Gets or sets the foreground color of the system tray on the current application page. <br>Returns: The foreground color of the system tray on the current application page. | SDK |
| `IsVisible` | Gets or sets the visibility of the system tray on the current application page. <br>Returns: true if the system tray is visible; otherwise, false. | SDK |
| `Opacity` | Gets or sets the opacity factor of the system tray on the current application page. <br>Returns: The opacity factor of the system tray on the current application page. | SDK |
| `ProgressIndicator` | Gets or sets the progress indicator on the system tray on the current application page. <br>Returns: The progress indicator on the system tray on the current application page. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BackgroundColorProperty` | The dependency property for `BackgroundColor`. | SDK |
| `ForegroundColorProperty` | The dependency property for `ForegroundColor`. | SDK |
| `IsVisibleProperty` | The dependency property for `IsVisible`. | SDK |
| `OpacityProperty` | The dependency property for `Opacity`. | SDK |
| `ProgressIndicatorProperty` | The dependency property for `ProgressIndicator`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Creates a new instance of the `SystemTray` class. | SDK |
| `GetBackgroundColor(DependencyObject)` | Gets the value of the `BackgroundColor` attached property for a specified phone application page. <br>`element`: The page for which to get the `BackgroundColor` attached property. <br>Returns: The background color of the system tray on the current application page. | SDK |
| `GetForegroundColor(DependencyObject)` | Gets the value of the `ForegroundColor` attached property for a specified phone application page. <br>`element`: The page for which to get the `ForegroundColor` attached property. <br>Returns: The foreground color of the system tray on the current application page. | SDK |
| `GetIsVisible(DependencyObject)` | Gets the value of the `IsVisible` attached property for a specified phone application page. <br>`element`: The page for which to get the `IsVisible` attached property. <br>Returns: true if the system tray is visible; otherwise, false. | SDK |
| `GetOpacity(DependencyObject)` | Gets the value of the `Opacity` attached property for a specified phone application page. <br>`element`: The page for which to get the `Opacity` attached property. <br>Returns: The opacity factor of the system tray on the current application page. | SDK |
| `GetProgressIndicator(DependencyObject)` | Gets the value of the `ProgressIndicator` attached property for a specified phone application page. <br>`element`: The page for which to get the `ProgressIndicator` attached property. <br>Returns: The progress indicator on the system tray on the current application page. | SDK |
| `SetBackgroundColor(DependencyObject,Media.Color)` | Sets the value of the `BackgroundColor` attached property for a specified phone application page. <br>`element`: The page for which to set the `BackgroundColor` attached property. <br>`color`: The background color to set for the system tray. | SDK |
| `SetForegroundColor(DependencyObject,Media.Color)` | Gets the value of the `ForegroundColor` attached property for a specified phone application page.. <br>`element`: The page for which to set the `ForegroundColor` attached property. <br>`color`: The foreground color to set for the system tray. | SDK |
| `SetIsVisible(DependencyObject,Boolean)` | Sets the value of the `IsVisible` attached property for a given phone application page. <br>`element`: The page for which to set the `IsVisible` attached property. <br>`isVisible`: true to display the system tray; false to hide the system tray. | SDK |
| `SetOpacity(DependencyObject,Double)` | Sets the value of the `Opacity` attached property for a specified phone application page. <br>`element`: The page for which to set the `Opacity` attached property. <br>`opacity`: The opacity factor to set for the system tray. | SDK |
| `SetProgressIndicator(DependencyObject,Microsoft.Phone.Shell.ProgressIndicator)` | Sets the value of the `ProgressIndicator` attached property for a specified phone application page. <br>`element`: The page for which to set the `ProgressIndicator` attached property. <br>`progressIndicator`: The progress indicator to set on the system tray. | SDK |

## Microsoft.Phone.Shell.UpdateInterval

Source: SDK

Defines which interval will be used with the schedule if `UpdateRecurrence` has been set to Interval.

### Fields

| Member | Description | Src |
|---|---|---|
| `EveryDay` | The schedule will be run every day. | SDK |
| `EveryHour` | The schedule will be run every hour. | SDK |
| `EveryMonth` | The schedule will be run every month. | SDK |
| `EveryWeek` | The schedule will be run every week. | SDK |

## Microsoft.Phone.Shell.UpdateRecurrence

Source: SDK

Defines whether the schedule will be executed one time or multiple times.

### Fields

| Member | Description | Src |
|---|---|---|
| `Interval` | Specifies that the schedule will be executed more than one time. | SDK |
| `Onetime` | Specifies that the schedule will be executed one time. | SDK |
