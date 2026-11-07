# Other

## Contents

- [`Microsoft.Phone.Maps.Services.QueryExtensions`](#microsoftphonemapsservicesqueryextensions)
- [`Microsoft.Phone.Maps.Toolkit.MapChild`](#microsoftphonemapstoolkitmapchild)
- [`Microsoft.Phone.Maps.Toolkit.MapChildCollection`](#microsoftphonemapstoolkitmapchildcollection)
- [`Microsoft.Phone.Maps.Toolkit.MapChildControl`](#microsoftphonemapstoolkitmapchildcontrol)
- [`Microsoft.Phone.Maps.Toolkit.MapExtensions`](#microsoftphonemapstoolkitmapextensions)
- [`Microsoft.Phone.Maps.Toolkit.MapItemsControl`](#microsoftphonemapstoolkitmapitemscontrol)
- [`Microsoft.Phone.Maps.Toolkit.Pushpin`](#microsoftphonemapstoolkitpushpin)
- [`Microsoft.Phone.Maps.Toolkit.UserLocationMarker`](#microsoftphonemapstoolkituserlocationmarker)
- [`System.Windows.HierarchicalDataTemplate`](#systemwindowshierarchicaldatatemplate)
- [`Windows.Devices.Geolocation.GeocoordinateExtensions`](#windowsdevicesgeolocationgeocoordinateextensions)

## Microsoft.Phone.Maps.Services.QueryExtensions

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/QueryExtensions.cs`

Represents a class that extends map queries.

### Methods

| Member | Description | Src |
|---|---|---|
| `static Task<IList<MapLocation>> GetMapLocationsAsync(this GeocodeQuery geocodeQuery)` | Get MapLocations as an asynchronous operation. <br>`geocodeQuery`: The `GeocodeQuery` the operation is performed on. <br>Returns: Returns `Task&lt;TResult&gt;`. The task object representing the asynchronous operation. | TK |
| `static Task<IList<MapLocation>> GetMapLocationsAsync(this ReverseGeocodeQuery reverseGeocodeQuery)` | Get MapLocations as an asynchronous operation. <br>`reverseGeocodeQuery`: The `ReverseGeocodeQuery` the operation is performed on. <br>Returns: Returns `Task&lt;TResult&gt;`. The task object representing the asynchronous operation. | TK |
| `static Task<Route> GetRouteAsync(this RouteQuery routeQuery)` | Get Route as an asynchronous operation. <br>`routeQuery`: The `RouteQuery` the operation is performed on. <br>Returns: Returns `Task&lt;TResult&gt;`. The task object representing the asynchronous operation. | TK |

## Microsoft.Phone.Maps.Toolkit.MapChild

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/MapChild.cs`

Represents a child of a map, which uses geographic coordinates to position itself.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty GeoCoordinateProperty` | Gets or sets the child's geographic coordinate position | TK |
| `static DependencyProperty PositionOriginProperty` | Gets or sets the child's position origin. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static GeoCoordinate GetGeoCoordinate(DependencyObject element)` | Gets the geographic coordinate position of the child. <br>`element`: The dependency object <br>Returns: Returns `GeoCoordinate` | TK |
| `static Point GetPositionOrigin(DependencyObject element)` | Gets the position origin of the child. <br>`element`: The dependency object <br>Returns: Returns `GeoCoordinate` | TK |
| `static void SetGeoCoordinate(DependencyObject element, GeoCoordinate value)` | Sets the geographic coordinate position of the child. <br>`element`: The dependency object <br>`value`: The coordinate to use to position the child | TK |
| `static void SetPositionOrigin(DependencyObject element, Point value)` | Sets the position origin of the child. <br>`element`: The dependency object <br>`value`: The position origin of the child | TK |

## Microsoft.Phone.Maps.Toolkit.MapChildCollection

Source: Toolkit · class : ObservableCollection<object>, IList, ICollection<object> · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/MapChildCollection.cs`

Holds the list of items that represent the content of a `MapItemsControl`.

### Methods

| Member | Description | Src |
|---|---|---|
| `void Add(DependencyObject dependencyObject, GeoCoordinate geoCoordinate)` | Adds an object at the specified GeoCoordinate. <br>`dependencyObject`: The dependency object to add. <br>`geoCoordinate`: The geographic coordinate at which to add the dependency object. | TK |
| `void Add(DependencyObject dependencyObject, GeoCoordinate geoCoordinate, Point positionOrigin)` | Adds an object at the specified GeoCoordinate and PositionOrigin <br>`dependencyObject`: The dependency object to add. <br>`geoCoordinate`: The geographic coordinate at which to add the dependency object. <br>`positionOrigin`: The position origin to use. | TK |
| `«ctor» MapChildCollection()` | Initializes a new instance of the `MapChildCollection` class | TK |

## Microsoft.Phone.Maps.Toolkit.MapChildControl

Source: Toolkit · class : ContentControl · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/MapChildControl.cs`

Represents a child of a map, which uses geographic coordinates to position itself.

### Properties

| Member | Description | Src |
|---|---|---|
| `GeoCoordinate GeoCoordinate` | Gets or sets the geographic coordinate of the control on the map. | TK |
| `Point PositionOrigin` | Gets or sets the position origin of the control, which defines the position on the control to anchor to the map. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty GeoCoordinateProperty` | Identifies the `GeoCoordinate` dependency property. | TK |
| `static DependencyProperty PositionOriginProperty` | Identifies the `PositionOrigin` dependency property. | TK |

## Microsoft.Phone.Maps.Toolkit.MapExtensions

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/MapExtensions.cs`

Represents a class that can extend the capabilities of the `Map` class.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ChildrenProperty` | Identifies the Children dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static ObservableCollection<DependencyObject> GetChildren(Map element)` | Gets the Children collection of a map. <br>`element`: The dependency object <br>Returns: Returns `ObservableCollection&lt;DependencyObject&gt;` | TK |
| `static void Add(this ObservableCollection<DependencyObject> childrenCollection, DependencyObject dependencyObject, GeoCoordinate geoCoordinate)` | Adds a dependency object to the map at the specified location. <br>`childrenCollection`: An `ObservableCollection&lt;DependencyObject&gt;` to add to. <br>`dependencyObject`: The dependency object to add. <br>`geoCoordinate`: The geographic coordinate at which to add the dependency object. | TK |
| `static void Add(this ObservableCollection<DependencyObject> childrenCollection, DependencyObject dependencyObject, GeoCoordinate geoCoordinate, Point positionOrigin)` | Adds a dependency object to the map at the specified location. <br>`childrenCollection`: An `ObservableCollection&lt;DependencyObject&gt;` to add to. <br>`dependencyObject`: The dependency object to add. <br>`geoCoordinate`: The geographic coordinate at which to add the dependency object. <br>`positionOrigin`: The position origin to use. | TK |

## Microsoft.Phone.Maps.Toolkit.MapItemsControl

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/MapItemsControl.cs`

Represents a control that can be used to present a collection of items on a map.

### Properties

| Member | Description | Src |
|---|---|---|
| `DataTemplate ItemTemplate` | Gets or sets the `DataTemplate` used to display each item. | TK |
| `MapChildCollection Items` | Gets the collection used to generate the content of the control. | TK |
| `IEnumerable ItemsSource` | Gets or sets a collection used to generate the content of the `ItemsControl`. | TK |
| `string Name` | Gets or sets the name | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ItemTemplateProperty` | Identifies the `ItemTemplate` dependency property. | TK |
| `static DependencyProperty ItemsSourceProperty` | Identifies the `ItemsSource` dependency property. | TK |
| `static DependencyProperty NameProperty` | Identifies the `Name` dependency property. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» MapItemsControl()` | Initializes a new instance of the `MapItemsControl` class | TK |

## Microsoft.Phone.Maps.Toolkit.Pushpin

Source: Toolkit · class : MapChildControl · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/PushPin.cs`

Represents a pushpin on the map.

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» Pushpin()` | Initializes a new instance of the `Pushpin` class. | TK |

## Microsoft.Phone.Maps.Toolkit.UserLocationMarker

Source: Toolkit · class : MapChildControl · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/UserLocationMarker.cs`

Represents a marker for the location of a user on the map.

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» UserLocationMarker()` | Initializes a new instance of the `UserLocationMarker` class. | TK |

## System.Windows.HierarchicalDataTemplate

Source: Toolkit · class : DataTemplate · `Microsoft.Phone.Controls.Toolkit/Data/HierarchicalDataTemplate.cs`

Represents a `DataTemplate` that supports `HeaderedItemsControl` objects, such as `TreeViewItem`.

### Properties

| Member | Description | Src |
|---|---|---|
| `Style ItemContainerStyle` | Gets or sets the `Style` that is applied to the item container for each child item. <br>Value: The style that is applied to the item container for each child item. | TK |
| `DataTemplate ItemTemplate` | Gets or sets the `DataTemplate` to apply to the `ItemTemplate` property on a generated `HeaderedItemsControl`, such as a `TreeViewItem`, to indicate how to display items from the next sublevel in the data hierarchy. <br>Value: The `DataTemplate` to apply to the `ItemTemplate` property on a generated `HeaderedItemsControl`, such as a `TreeViewItem`, to indicate how to display items from the next sublevel in the data hierarchy. | TK |
| `Binding ItemsSource` | Gets or sets the collection that is used to generate content for the next sublevel in the data hierarchy. <br>Value: The collection that is used to generate content for the next sublevel in the data hierarchy. The default value is null. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `«ctor» HierarchicalDataTemplate()` | Initializes a new instance of the `HierarchicalDataTemplate` class. | TK |

## Windows.Devices.Geolocation.GeocoordinateExtensions

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit.WP8/Maps/GeocoordinateExtensions.cs`

Represents a class that extends Geocoordinate.

### Methods

| Member | Description | Src |
|---|---|---|
| `static GeoCoordinate ToGeoCoordinate(this Geocoordinate geocoordinate)` | Creates a `GeoCoordinate` from a `Geocoordinate`. <br>`geocoordinate`: A `Geocoordinate` to create a `GeoCoordinate` from. <br>Returns: Returns `GeoCoordinate` | TK |
