# Maps

## Contents

- [`Microsoft.Phone.Controls.Maps.AerialMode`](#microsoftphonecontrolsmapsaerialmode)
- [`Microsoft.Phone.Controls.Maps.AnimationLevel`](#microsoftphonecontrolsmapsanimationlevel)
- [`Microsoft.Phone.Controls.Maps.ApplicationIdCredentialsProvider`](#microsoftphonecontrolsmapsapplicationidcredentialsprovider)
- [`Microsoft.Phone.Controls.Maps.AttributionCollection`](#microsoftphonecontrolsmapsattributioncollection)
- [`Microsoft.Phone.Controls.Maps.AttributionInfo`](#microsoftphonecontrolsmapsattributioninfo)
- [`Microsoft.Phone.Controls.Maps.AutomationPeers.BaseAutomationPeer`](#microsoftphonecontrolsmapsautomationpeersbaseautomationpeer)
- [`Microsoft.Phone.Controls.Maps.AutomationPeers.MapAutomationPeer`](#microsoftphonecontrolsmapsautomationpeersmapautomationpeer)
- [`Microsoft.Phone.Controls.Maps.AutomationPeers.MapBaseAutomationPeer`](#microsoftphonecontrolsmapsautomationpeersmapbaseautomationpeer)
- [`Microsoft.Phone.Controls.Maps.AutomationPeers.MapTileLayerAutomationPeer`](#microsoftphonecontrolsmapsautomationpeersmaptilelayerautomationpeer)
- [`Microsoft.Phone.Controls.Maps.Core.ConfigurationNotLoadedException`](#microsoftphonecontrolsmapscoreconfigurationnotloadedexception)
- [`Microsoft.Phone.Controls.Maps.Core.CopyrightManager`](#microsoftphonecontrolsmapscorecopyrightmanager)
- [`Microsoft.Phone.Controls.Maps.Core.CredentialsInvalidException`](#microsoftphonecontrolsmapscorecredentialsinvalidexception)
- [`Microsoft.Phone.Controls.Maps.Core.FlatMapMode`](#microsoftphonecontrolsmapscoreflatmapmode)
- [`Microsoft.Phone.Controls.Maps.Core.IProjectable`](#microsoftphonecontrolsmapscoreiprojectable)
- [`Microsoft.Phone.Controls.Maps.Core.MapBase`](#microsoftphonecontrolsmapscoremapbase)
- [`Microsoft.Phone.Controls.Maps.Core.MapConfiguration`](#microsoftphonecontrolsmapscoremapconfiguration)
- [`Microsoft.Phone.Controls.Maps.Core.MapConfigurationCallback`](#microsoftphonecontrolsmapscoremapconfigurationcallback)
- [`Microsoft.Phone.Controls.Maps.Core.MapConfigurationLoadedEventArgs`](#microsoftphonecontrolsmapscoremapconfigurationloadedeventargs)
- [`Microsoft.Phone.Controls.Maps.Core.MapConfigurationSection`](#microsoftphonecontrolsmapscoremapconfigurationsection)
- [`Microsoft.Phone.Controls.Maps.Core.MapCore`](#microsoftphonecontrolsmapscoremapcore)
- [`Microsoft.Phone.Controls.Maps.Core.MapMode`](#microsoftphonecontrolsmapscoremapmode)
- [`Microsoft.Phone.Controls.Maps.Core.MapShapeBase`](#microsoftphonecontrolsmapscoremapshapebase)
- [`Microsoft.Phone.Controls.Maps.Core.MatrixHelper`](#microsoftphonecontrolsmapscorematrixhelper)
- [`Microsoft.Phone.Controls.Maps.Core.MercatorMode`](#microsoftphonecontrolsmapscoremercatormode)
- [`Microsoft.Phone.Controls.Maps.Core.MercatorUtility`](#microsoftphonecontrolsmapscoremercatorutility)
- [`Microsoft.Phone.Controls.Maps.Core.ModeBackground`](#microsoftphonecontrolsmapscoremodebackground)
- [`Microsoft.Phone.Controls.Maps.Core.NullMode`](#microsoftphonecontrolsmapscorenullmode)
- [`Microsoft.Phone.Controls.Maps.Core.TileSourceCollection`](#microsoftphonecontrolsmapscoretilesourcecollection)
- [`Microsoft.Phone.Controls.Maps.Core.TileSourcePropertyChangedEventArgs`](#microsoftphonecontrolsmapscoretilesourcepropertychangedeventargs)
- [`Microsoft.Phone.Controls.Maps.Core.UriSchemeNotSupportedException`](#microsoftphonecontrolsmapscoreurischemenotsupportedexception)
- [`Microsoft.Phone.Controls.Maps.Credentials`](#microsoftphonecontrolsmapscredentials)
- [`Microsoft.Phone.Controls.Maps.CredentialsProvider`](#microsoftphonecontrolsmapscredentialsprovider)
- [`Microsoft.Phone.Controls.Maps.Design.ApplicationIdCredentialsProviderConverter`](#microsoftphonecontrolsmapsdesignapplicationidcredentialsproviderconverter)
- [`Microsoft.Phone.Controls.Maps.Design.LocationCollectionConverter`](#microsoftphonecontrolsmapsdesignlocationcollectionconverter)
- [`Microsoft.Phone.Controls.Maps.Design.LocationConverter`](#microsoftphonecontrolsmapsdesignlocationconverter)
- [`Microsoft.Phone.Controls.Maps.Design.LocationRectConverter`](#microsoftphonecontrolsmapsdesignlocationrectconverter)
- [`Microsoft.Phone.Controls.Maps.Design.MapModeConverter`](#microsoftphonecontrolsmapsdesignmapmodeconverter)
- [`Microsoft.Phone.Controls.Maps.Design.PositionOriginConverter`](#microsoftphonecontrolsmapsdesignpositionoriginconverter)
- [`Microsoft.Phone.Controls.Maps.Design.RangeConverter`](#microsoftphonecontrolsmapsdesignrangeconverter)
- [`Microsoft.Phone.Controls.Maps.LoadingErrorEventArgs`](#microsoftphonecontrolsmapsloadingerroreventargs)
- [`Microsoft.Phone.Controls.Maps.LocationCollection`](#microsoftphonecontrolsmapslocationcollection)
- [`Microsoft.Phone.Controls.Maps.LocationRect`](#microsoftphonecontrolsmapslocationrect)
- [`Microsoft.Phone.Controls.Maps.LocationRectTileSource`](#microsoftphonecontrolsmapslocationrecttilesource)
- [`Microsoft.Phone.Controls.Maps.Map`](#microsoftphonecontrolsmapsmap)
- [`Microsoft.Phone.Controls.Maps.MapDragEventArgs`](#microsoftphonecontrolsmapsmapdrageventargs)
- [`Microsoft.Phone.Controls.Maps.MapEventArgs`](#microsoftphonecontrolsmapsmapeventargs)
- [`Microsoft.Phone.Controls.Maps.MapFlickEventArgs`](#microsoftphonecontrolsmapsmapflickeventargs)
- [`Microsoft.Phone.Controls.Maps.MapInputEventArgs`](#microsoftphonecontrolsmapsmapinputeventargs)
- [`Microsoft.Phone.Controls.Maps.MapItemsControl`](#microsoftphonecontrolsmapsmapitemscontrol)
- [`Microsoft.Phone.Controls.Maps.MapLayer`](#microsoftphonecontrolsmapsmaplayer)
- [`Microsoft.Phone.Controls.Maps.MapLayerBase`](#microsoftphonecontrolsmapsmaplayerbase)
- [`Microsoft.Phone.Controls.Maps.MapPolygon`](#microsoftphonecontrolsmapsmappolygon)
- [`Microsoft.Phone.Controls.Maps.MapPolyline`](#microsoftphonecontrolsmapsmappolyline)
- [`Microsoft.Phone.Controls.Maps.MapTileLayer`](#microsoftphonecontrolsmapsmaptilelayer)
- [`Microsoft.Phone.Controls.Maps.MapZoomEventArgs`](#microsoftphonecontrolsmapsmapzoomeventargs)
- [`Microsoft.Phone.Controls.Maps.Overlays.Copyright`](#microsoftphonecontrolsmapsoverlayscopyright)
- [`Microsoft.Phone.Controls.Maps.Overlays.DistanceUnit`](#microsoftphonecontrolsmapsoverlaysdistanceunit)
- [`Microsoft.Phone.Controls.Maps.Overlays.LoadingErrorMessage`](#microsoftphonecontrolsmapsoverlaysloadingerrormessage)
- [`Microsoft.Phone.Controls.Maps.Overlays.Logo`](#microsoftphonecontrolsmapsoverlayslogo)
- [`Microsoft.Phone.Controls.Maps.Overlays.MapCommandBase`](#microsoftphonecontrolsmapsoverlaysmapcommandbase)
- [`Microsoft.Phone.Controls.Maps.Overlays.MapCommandEventArgs`](#microsoftphonecontrolsmapsoverlaysmapcommandeventargs)
- [`Microsoft.Phone.Controls.Maps.Overlays.MapForeground`](#microsoftphonecontrolsmapsoverlaysmapforeground)
- [`Microsoft.Phone.Controls.Maps.Overlays.Overlay`](#microsoftphonecontrolsmapsoverlaysoverlay)
- [`Microsoft.Phone.Controls.Maps.Overlays.Scale`](#microsoftphonecontrolsmapsoverlaysscale)
- [`Microsoft.Phone.Controls.Maps.Overlays.ShadowText`](#microsoftphonecontrolsmapsoverlaysshadowtext)
- [`Microsoft.Phone.Controls.Maps.Overlays.ZoomBar`](#microsoftphonecontrolsmapsoverlayszoombar)
- [`Microsoft.Phone.Controls.Maps.Overlays.ZoomMapCommand`](#microsoftphonecontrolsmapsoverlayszoommapcommand)
- [`Microsoft.Phone.Controls.Maps.Platform.Location`](#microsoftphonecontrolsmapsplatformlocation)
- [`Microsoft.Phone.Controls.Maps.Platform.Rectangle`](#microsoftphonecontrolsmapsplatformrectangle)
- [`Microsoft.Phone.Controls.Maps.Platform.ShapeBase`](#microsoftphonecontrolsmapsplatformshapebase)
- [`Microsoft.Phone.Controls.Maps.PositionOrigin`](#microsoftphonecontrolsmapspositionorigin)
- [`Microsoft.Phone.Controls.Maps.ProjectionChangedEventArgs`](#microsoftphonecontrolsmapsprojectionchangedeventargs)
- [`Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel`](#microsoftphonecontrolsmapsprojectionupdatelevel)
- [`Microsoft.Phone.Controls.Maps.Pushpin`](#microsoftphonecontrolsmapspushpin)
- [`Microsoft.Phone.Controls.Maps.QuadKey`](#microsoftphonecontrolsmapsquadkey)
- [`Microsoft.Phone.Controls.Maps.Range`1`](#microsoftphonecontrolsmapsrange1)
- [`Microsoft.Phone.Controls.Maps.RoadMode`](#microsoftphonecontrolsmapsroadmode)
- [`Microsoft.Phone.Controls.Maps.TileSource`](#microsoftphonecontrolsmapstilesource)

## Microsoft.Phone.Controls.Maps.AerialMode

Source: SDK

Represents the aerial map mode.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | Gets the map as a UI Element. <br>Returns: Returns `UIElement`. | SDK |
| `IsDownloading` | Gets a bool indicating whether map data is still downloading. <br>Returns: Returns `Boolean`. | SDK |
| `IsIdle` | Gets whether the mode is idle. <br>Returns: Returns `Boolean`. | SDK |
| `ShouldDisplayFadingLabels` | Gets or sets whether or not to fade map labels when the map pans or zooms. <br>Returns: Returns `Boolean`. | SDK |
| `ShouldDisplayLabels` | Gets or sets whether to show map labels. <br>Returns: Returns `Boolean`. | SDK |
| `ShouldRestrictMaxZoomLevel` | Gets or sets whether the maximum zoom level is restricted by a coverage map. <br>Returns: Returns `Boolean`. | SDK |
| `UpSampleLevelDelta` | Gets or sets the map tile layers sample level delta. <br>Returns: Returns `Int32`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `AerialMode` class. | SDK |
| `Activating(Microsoft.Phone.Controls.Maps.Core.MapMode,Microsoft.Phone.Controls.Maps.MapLayerBase,Microsoft.Phone.Controls.Maps.MapLayerBase)` | Notifies a mode that is being activated by the map. <br>`previousMode`: The previous map mode. <br>`modeLayer`: The map mode layer. <br>`modeForegroundLayer`: The foreground layer. | SDK |
| `Constructor(Boolean)` | Initializes a new instance of the `AerialMode` class. <br>`labels`: A bool indicating whether to show map labels. | SDK |
| `GetZoomRange(Device.Location.GeoCoordinate)` | When overridden in a derived class, retrieves the valid zoom range for the specified location. <br>`center`: The location to use. <br>Returns: Returns `Range`1`. | SDK |
| `OnCultureChanged` | Enables derived classes to provide custom handling when the culture of the mode changes. | SDK |

## Microsoft.Phone.Controls.Maps.AnimationLevel

Source: SDK

Determines when map navigation is animated.

### Fields

| Member | Description | Src |
|---|---|---|
| `Full` | Map navigation is always animated. | SDK |
| `None` | The map navigates to a new view by snapping to it instead of using animation. | SDK |
| `UserInput` | Map navigation is only animated in response to user input. | SDK |

## Microsoft.Phone.Controls.Maps.ApplicationIdCredentialsProvider

Source: SDK

Provides credentials in the form of an application ID.

### Properties

| Member | Description | Src |
|---|---|---|
| `ApplicationId` | Gets or sets the application ID. The application ID is your Bing Maps Key. Information about getting a Bing Maps Key is found in Getting a Bing Maps Key. <br>Returns: Returns `String`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `PropertyChanged` | Occurs when a property of this class changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ApplicationIdCredentialsProvider` class. | SDK |
| `Constructor(String)` | Initializes a new instance of the `ApplicationIdCredentialsProvider` class. <br>`applicationId`: The application ID, or Bing Maps Key, to use. | SDK |
| `GetCredentials(Action{Microsoft.Phone.Controls.Maps.Credentials})` | Retrieves the credentials for the map control. This method can be used to pass your Bing Maps Key to other Bing Maps APIs, such as the Bing Maps REST Services. <br>`callback`: The function to call when credentials are retrieved. | SDK |

## Microsoft.Phone.Controls.Maps.AttributionCollection

Source: SDK

Contains a collection of `AttributionInfo` objects.

### Events

| Member | Description | Src |
|---|---|---|
| `CollectionChanged` | Occurs when any item in the attribution collection changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Collections.ObjectModel.ObservableCollection{Microsoft.Phone.Controls.Maps.AttributionInfo})` | Initializes a new instance of the `AttributionCollection` class. <br>`list`: The list of `AttributionInfo` items. | SDK |
| `OnCollectionChanged(Collections.Specialized.NotifyCollectionChangedEventArgs)` | Raises the `CollectionChanged` event. <br>`args`: The event data. | SDK |

## Microsoft.Phone.Controls.Maps.AttributionInfo

Source: SDK

Contains map data and imagery attribution information.

### Properties

| Member | Description | Src |
|---|---|---|
| `Text` | Gets or sets the text which contains the map data and imagery attribution. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `AttributionInfo` class. | SDK |
| `Equals(Object)` | Determines whether the specified object is equal to this attribution info object. <br>`obj`: The object to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `GetHashCode` | Retrieves the hash code for this attribution info object. <br>Returns: Returns `Int32`. | SDK |

## Microsoft.Phone.Controls.Maps.AutomationPeers.BaseAutomationPeer

Source: SDK

Represents the base automation peer class, which is used to automate map control testing.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(FrameworkElement,String)` | Initializes a new instance of the `BaseAutomationPeer` class. <br>`element`: The element for which to create the automation peer. <br>`className`: The name of the class. | SDK |
| `GetAutomationControlTypeCore` | Retrieves the type of the control. <br>Returns: Returns `AutomationControlType`. | SDK |
| `GetClassNameCore` | Retrieves the name of the class. <br>Returns: Returns `String`. | SDK |
| `GetNameCore` | Retrieves the name of the element. <br>Returns: Returns `String`. | SDK |

## Microsoft.Phone.Controls.Maps.AutomationPeers.MapAutomationPeer

Source: SDK

Contains methods used to automate the testing of a `Map`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Microsoft.Phone.Controls.Maps.Map)` | Initializes a new instance of the `MapAutomationPeer` class. <br>`element`: The map to test. | SDK |
| `DoubleTap(Point)` | Simulates a double tap on the phone screen. <br>`viewportPoint`: The tapped point on the screen. | SDK |
| `Drag(Point,Point)` | Simulates a pan drag on the phone screen. <br>`viewportPoint`: The tapped point on the screen that began the drag. <br>`dragDelta`: The amount of the pan drag. | SDK |
| `Flick(Point,Point)` | Simulates a touch on the phone screen. <br>`viewportPoint`: The touched point on the screen. <br>`velocity`: The speed of the touch. | SDK |
| `Scale(Point,Double)` | Simulates a pinch stretch on the phone screen. <br>`viewportPoint`: The point on the screen where the pinch began. <br>`scaleDelta`: The amount the screen was pinched. | SDK |
| `Zoom(Point,Double)` | Simulates a zoom on the phone screen. <br>`viewportPoint`: The tapped point on the screen. <br>`zoomDelta`: The amount the map was zoomed. | SDK |

## Microsoft.Phone.Controls.Maps.AutomationPeers.MapBaseAutomationPeer

Source: SDK

Contains methods used to automate the testing of a `Map`.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsReadOnly` | Gets whether the automation peer is read-only. <br>Returns: Returns `Boolean`. | SDK |
| `Map` | Gets the map associated with this automation peer. <br>Returns: Returns `MapBase`. | SDK |
| `Value` | Gets a string with map properties embedded in it. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Microsoft.Phone.Controls.Maps.Core.MapBase)` | Initializes a new instance of the `MapBaseAutomationPeer` class. <br>`element`: The map to test. | SDK |
| `Constructor(Microsoft.Phone.Controls.Maps.Core.MapBase,String)` | Initializes a new instance of the `MapBaseAutomationPeer` class. <br>`element`: The map to test. <br>`className`: The class name of the map. | SDK |
| `GetPattern(Automation.Peers.PatternInterface)` | Retrieves the value pattern that is supported. <br>`patternInterface`: The pattern interface. <br>Returns: Returns `Object`. | SDK |
| `SetValue(String)` | Sets the specified value. <br>`value`: The value to set. | SDK |

## Microsoft.Phone.Controls.Maps.AutomationPeers.MapTileLayerAutomationPeer

Source: SDK

Contains methods used to automate the testing of a map tile layer.

### Properties

| Member | Description | Src |
|---|---|---|
| `IsReadOnly` | Gets whether or not the automation peer is read only. <br>Returns: Returns `Boolean`. | SDK |
| `Value` | Gets a string with map tile layer properties embedded in it. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(FrameworkElement)` | Initializes a new instance of the `MapTileLayerAutomationPeer` class. <br>`element`: The map tile layer to test. | SDK |
| `GetPattern(Automation.Peers.PatternInterface)` | Retrieves the pattern interface as an object. <br>`patternInterface`: The pattern interface. <br>Returns: Returns `Object`. | SDK |
| `SetValue(String)` | Sets the specified value. <br>`value`: The value to set. | SDK |

## Microsoft.Phone.Controls.Maps.Core.ConfigurationNotLoadedException

Source: SDK

The exception that is thrown when a configuration has not loaded.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ConfigurationNotLoadedException` class. | SDK |
| `Constructor(String)` | Initializes a new instance of the `ConfigurationNotLoadedException` class. <br>`message`: The error message that explains the reason for the exception. | SDK |
| `Constructor(String,Exception)` | Initializes a new instance of the `ConfigurationNotLoadedException` class. <br>`message`: The error message that explains the reason for the exception. <br>`innerException`: The error type that is the cause of the current exception, or null. | SDK |

## Microsoft.Phone.Controls.Maps.Core.CopyrightManager

Source: SDK

Manages the copyright string.

### Methods

| Member | Description | Src |
|---|---|---|
| `GetInstance` | Returns an instance of the copyright manager. <br>Returns: Returns `CopyrightManager`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.CredentialsInvalidException

Source: SDK

The exception that is thrown when the specified credentials are invalid.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `CredentialsInvalidException` class. | SDK |
| `Constructor(String)` | Initializes a new instance of the `CredentialsInvalidException` class. <br>`message`: The error message that explains the reason for the exception. | SDK |
| `Constructor(String,Exception)` | Initializes a new instance of the `CredentialsInvalidException` class. <br>`message`: The error message that explains the reason for the exception. <br>`innerException`: The error type that is the cause of the current exception, or null. | SDK |

## Microsoft.Phone.Controls.Maps.Core.FlatMapMode

Source: SDK

Represents a flattened map projection of the Earth. This class must be inherited.

### Properties

| Member | Description | Src |
|---|---|---|
| `Center` | Gets or sets the location of the center of the map view. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `TargetCenter` | Gets the location of the center of the map view towards which the map is animating. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `TargetZoomLevel` | Gets the zoom level of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `ZoomLevel` | Gets or sets the zoom level of the map view. <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ConstrainView(Device.Location.GeoCoordinate,Double@,Double@,Double@)` | When overridden in a derived class, limits the view values to valid margins. <br>`center`: The location of the center of the view. <br>`zoomLevel`: The zoom level of the view. <br>`heading`: The heading of the view. <br>`pitch`: The pitch of the view. <br>Returns: Returns a `Boolean` indicating whether or not any of the values were changed. | SDK |
| `Constructor(Size)` | Initializes a new instance of the `FlatMapMode` class. <br>`logicalAreaSizeInScreenSpaceAtLevel1`: The size of the available screen space at the most zoomed out level. | SDK |
| `LocationRectToLogicalRect(Microsoft.Phone.Controls.Maps.LocationRect)` | When overridden in a derived class, converts a `LocationRect` object to a `Rect`. <br>`boundingRectangle`: The location rectangle to convert. <br>Returns: Returns `Rect`. | SDK |
| `LocationToLogicalPoint(Device.Location.GeoCoordinate)` | When overridden in a derived class, converts a `GeoCoordinate` object to a point on the map. <br>`location`: The `GeoCoordinate` to convert. <br>Returns: Returns `Point`. | SDK |
| `LocationToLogicalPoint(IEnumerable{Device.Location.GeoCoordinate})` | When overridden in a derived class, converts a collection of `GeoCoordinate` objects to a collection of points on the map. <br>`locations`: The collection of `GeoCoordinate` objects to convert. <br>Returns: Returns `IEnumerable`1`. | SDK |
| `LocationToViewportPoint(Device.Location.GeoCoordinate)` | Converts a location to a viewport point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `LocationToViewportPoint(IEnumerable{Device.Location.GeoCoordinate})` | Converts a collection of locations to a collection of viewport points. <br>`locations`: The locations to convert. <br>Returns: Returns `IEnumerable`1`. | SDK |
| `LocationToViewportPoint(Microsoft.Phone.Controls.Maps.LocationRect)` | Converts a location rectangle to a viewport point. <br>`boundingRectangle`: The location rectangle to convert. <br>Returns: Returns `Rect`. | SDK |
| `LogicalPointToLocation(Point)` | When overridden in a derived class, converts a `Point` to a `GeoCoordinate`. <br>`logicalPoint`: The point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `LogicalPointToViewportPoint(Point)` | Converts a point on the screen to a point in the map view. <br>`logicalPoint`: The point to convert. <br>Returns: Returns `Point`. | SDK |
| `OnMapDrag(Microsoft.Phone.Controls.Maps.MapDragEventArgs)` | Raises the `MapPan` event. <br>`e`: The event arguments. | SDK |
| `OnMapFlick(Microsoft.Phone.Controls.Maps.MapFlickEventArgs)` | Raises the flick events. <br>`e`: The event arguments. | SDK |
| `OnMapZoom(Microsoft.Phone.Controls.Maps.MapZoomEventArgs)` | Raises the `MapZoom` event. <br>`e`: The event arguments. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double,Double,Boolean)` | Sets the map view. <br>`center`: The location of the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. <br>`pitch`: The pitch of the map view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `SetView(Microsoft.Phone.Controls.Maps.LocationRect,Boolean)` | Sets the map view using the specified location rectangle. <br>`boundingRectangle`: The location rectangle to use to define the borders of the map view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `TryLocationToViewportPoint(Device.Location.GeoCoordinate,Point@)` | Tries to convert the specified `GeoCoordinate` to a viewport point. <br>`location`: The `GeoCoordinate` to convert. <br>`viewportPoint`: The viewport point which represents the specified location. <br>Returns: Returns `Boolean` if the conversion was successful. | SDK |
| `TryViewportPointToLocation(Point,Device.Location.GeoCoordinate@)` | Tries to convert a viewport point to a `GeoCoordinate`. <br>`viewportPoint`: The viewport point to convert. <br>`location`: The `GeoCoordinate` which represents the specified viewport point. <br>Returns: Returns a `Boolean` if the conversion was successful. | SDK |
| `ViewportPointToLocation(Point)` | Converts a viewport point to a `GeoCoordinate`. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `ViewportPointToLogicalPoint(Point)` | Converts a viewport point to a point on the screen. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `Point`. | SDK |
| `ViewportSizeChanged(Size)` | Represents the method that will handle a viewport size change. <br>`viewportSize`: The new viewport size. | SDK |

## Microsoft.Phone.Controls.Maps.Core.IProjectable

Source: SDK

Defines the base interface for all `UIElement` objects that use a projection to render themselves on a map.

### Properties

| Member | Description | Src |
|---|---|---|
| `ParentMap` | Gets the parent map of this projectable. <br>Returns: Returns `MapBase`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Executes when the projection is changed and the conversion methods (ViewportPointToLocation and LocationToViewportPoint) return different results. <br>`updateLevel`: The projection update level. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapBase

Source: SDK

Represents the abstract base map class. This class must be inherited.

### Properties

| Member | Description | Src |
|---|---|---|
| `AnimationLevel` | When overridden in a derived class, gets or sets the animation level of map navigation. <br>Returns: Returns `AnimationLevel`. | SDK |
| `BoundingRectangle` | When overridden in a derived class, gets the bounding rectangle that defines the boundaries of the map view. <br>Returns: Returns `LocationRect`. | SDK |
| `Center` | Gets the center location of the map view. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `Children` | Gets the child elements of the map. <br>Returns: Returns `UIElementCollection`. | SDK |
| `CopyrightVisibility` | Gets or sets the visibility of the copyright on the map. <br>Returns: Returns `Visibility`. | SDK |
| `CredentialsProvider` | Gets or sets the credentials object. <br>Returns: Returns `CredentialsProvider`. | SDK |
| `Culture` | Gets or sets the culture used by the map. <br>Returns: Returns `CultureInfo`. | SDK |
| `Heading` | Gets or sets the directional heading of the map. <br>Returns: Returns `Double`. | SDK |
| `IsDownloading` | When overridden in a derived class, gets a bool indicating whether map data is downloading. <br>Returns: Returns `Boolean`. | SDK |
| `IsIdle` | When overridden in a derived class, gets whether the map is idle. <br>Returns: Returns `Boolean`. | SDK |
| `LogoVisibility` | Gets or sets the visibility of the logo on the map. <br>Returns: Returns `Visibility`. | SDK |
| `Mode` | When overridden in a derived class, gets or sets the map mode. <br>Returns: Returns `MapMode`. | SDK |
| `Pitch` | Gets or sets the pitch of the map. <br>Returns: Returns `Double`. | SDK |
| `RootLayer` | Gets the base map layer. <br>Returns: Returns `MapLayer`. | SDK |
| `ScaleVisibility` | Gets or sets the visibility of the scale bar. <br>Returns: Returns `Visibility`. | SDK |
| `TargetBoundingRectangle` | When overridden in a derived class, gets the bounding rectangle that defines the view towards which the map is animating. <br>Returns: Returns `LocationRect`. | SDK |
| `TargetCenter` | When overridden in a derived class, gets the center location of the view towards which the map is animating. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `TargetHeading` | When overridden in a derived class, gets the heading of the view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetPitch` | When overridden in a derived class, gets the pitch of the view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetZoomLevel` | When overridden in a derived class, gets the zoom level of the view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `ViewportSize` | When overridden in a derived class, gets the size of the viewport. <br>Returns: Returns `Size`. | SDK |
| `ZoomBarVisibility` | Gets or sets whether the map zoom level bar displays. <br>Returns: Returns `Visibility`. | SDK |
| `ZoomLevel` | When overridden in a derived class, gets or sets the zoom level of the map. <br>Returns: Returns `Double`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `AnimationLevelProperty` | Identifies the `AnimationLevel` dependency property. | SDK |
| `CenterProperty` | Identifies the `Center` dependency property. | SDK |
| `CopyrightVisibilityProperty` | Identifies the `CopyrightVisibility` dependency property. | SDK |
| `CredentialsProviderProperty` | Identifies the `CredentialsProvider` dependency property. | SDK |
| `CultureProperty` | Identifies the `Culture` dependency property. | SDK |
| `HeadingProperty` | Identifies the `Heading` dependency property. | SDK |
| `LogoVisibilityProperty` | Identifies the `LogoVisibility` dependency property. | SDK |
| `ModeProperty` | Identifies the `Mode` dependency property. | SDK |
| `PitchProperty` | Identifies the `Pitch` dependency property. | SDK |
| `ScaleVisibilityProperty` | Identifies the `ScaleVisibility` dependency property. | SDK |
| `ZoomBarVisibilityProperty` | Identifies the `ZoomBarVisibility` dependency property. | SDK |
| `ZoomLevelProperty` | Identifies the `ZoomLevel` dependency property. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `LoadingError` | When overridden in a derived class, occurs when there is an error loading the map. | SDK |
| `MapPan` | When overridden in a derived class, occurs when the screen is touched and dragged to pan the map. This event occurs every frame. | SDK |
| `MapResolved` | When overridden in a derived class, occurs when the map tiles are fully downloaded, regardless of whether the user is still interacting with the map. | SDK |
| `MapZoom` | When overridden in a derived class, occurs when the map is touched to zoom the map. | SDK |
| `ModeChanged` | When overridden in a derived class, occurs when the map mode changes. | SDK |
| `TargetViewChanged` | When overridden in a derived class, occurs when the view towards which the map is animating changes. | SDK |
| `ViewChangeEnd` | When overridden in a derived class, occurs when the view is done changing. | SDK |
| `ViewChangeOnFrame` | When overridden in a derived class, occurs when the map view is changing. | SDK |
| `ViewChangeStart` | When overridden in a derived class, occurs when the map view starts changing. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapBase` class. | SDK |
| `LocationToViewportPoint(Device.Location.GeoCoordinate)` | When overridden in a derived class, converts a location to a viewport point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `OnAnimationLevelChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the animation level changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnCenterChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the center of the map changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `OnCredentialsProviderChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the credentials provider changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnCultureChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the culture of the map changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnHeadingChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the heading of the map changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnModeChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the map mode changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnOverlayVisibilityChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the copyright, scale bar, or zoom bar visibility changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnPitchChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the pitch of the map changes. <br>`eventArgs`: The event arguments. | SDK |
| `OnZoomLevelChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the zoom level of the map changes. <br>`eventArgs`: The event arguments. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling when the projection of the map changes. <br>`updateLevel`: The projection update level. | SDK |
| `SetMode(Microsoft.Phone.Controls.Maps.Core.MapMode,Boolean)` | When overridden in a derived class, sets the map mode. <br>`newMode`: The new map mode. <br>`transferSettings`: A bool indicating whether to transfer view and animation level settings from the old map mode. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double)` | When overridden in a derived class, sets the map view based on a specified center and zoom level. <br>`center`: The location that defines the center of the map view. <br>`zoomLevel`: The zoom level of the view. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double)` | When overridden in a derived class, sets the map view based on a specified center, zoom level, and heading. <br>`center`: The location that defines the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double,Double)` | When overridden in a derived class, sets the map view based on a specified center, zoom level, heading, and pitch. <br>`center`: The location that defines the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the view. <br>`pitch`: The pitch of the view. | SDK |
| `SetView(Microsoft.Phone.Controls.Maps.LocationRect)` | When overridden in a derived class, sets the map view to the specified location rectangle. <br>`boundingRectangle`: The location rectangle that defines the borders of the view. | SDK |
| `TryLocationToViewportPoint(Device.Location.GeoCoordinate,Point@)` | When overridden in a derived class, tries to convert a location to a viewport point. <br>`location`: The location to convert. <br>`viewportPoint`: The location as a viewport point.i <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `TryViewportPointToLocation(Point,Device.Location.GeoCoordinate@)` | When overridden in a derived class, tries to convert a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>`location`: The viewport point as a location. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `ViewportPointToLocation(Point)` | When overridden in a derived class, converts a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapConfiguration

Source: SDK

Contains asynchronous configuration settings. The configuration settings are loaded when an event handler is added to AsynchronousConfigurationLoaded.

### Events

| Member | Description | Src |
|---|---|---|
| `Loaded` | Occurs when the configuration is loaded. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `GetSection(String,String,String,Microsoft.Phone.Controls.Maps.Core.MapConfigurationCallback)` | Loads the configuration section. If configuration is already loaded, this method calls the callback function without trying to load the configuration. <br>`version`: The version string of the configuration. <br>`sectionName`: The name of the configuration section to load. <br>`culture`: The culture of the configuration. <br>`callback`: The callback function to call after the configuration is loaded. | SDK |
| `GetSection(String,String,String,Microsoft.Phone.Controls.Maps.Core.MapConfigurationCallback,Object)` | Loads the configuration section. If configuration is already loaded, this method calls the callback function without trying to load the configuration. <br>`version`: The version string of the configuration. <br>`sectionName`: The name of the configuration section to load. <br>`culture`: The culture of the configuration. <br>`callback`: The callback function to call after the configuration is loaded. <br>`userState`: An object that stores user defined information. This object is returned in the callback function. | SDK |
| `Load(Uri)` | Loads the configuration from the configuration service. <br>`address`: The URI that contains the configuration. | SDK |
| `Load(Xml.XmlReader)` | Loads the configuration using the xml provided. <br>`config`: The configuration xml to use. | SDK |
| `SetServiceUri(Uri)` | Sets the URI of the configuration service. <br>`address`: The URI of the configuration service. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapConfigurationCallback

Source: SDK

Represents the function to call when the map configuration has loaded.

## Microsoft.Phone.Controls.Maps.Core.MapConfigurationLoadedEventArgs

Source: SDK

Provides data for the `Loaded` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `Error` | Gets the error that occurred when the configuration was loaded. <br>Returns: Returns `Exception`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Exception)` | Initializes a new instance of the `MapConfigurationLoadedEventArgs` class. <br>`error`: The error that occurred when the configuration was loaded. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapConfigurationSection

Source: SDK

Represents a section of a map configuration file.

### Properties

| Member | Description | Src |
|---|---|---|
| `Item` | Gets the configuration setting identified by the specified key. <br>`key`: The key that identifies the configuration setting. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Contains(String)` | Returns a bool indicating if the configuration setting identified by the specified key is in the map configuration. <br>`key`: The key that identifies the configuration setting. <br>Returns: Returns `Boolean`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapCore

Source: SDK

Represents the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `BoundingRectangle` | Gets the rectangle that defines the boundaries of the map view. <br>Returns: Returns `LocationRect`. | SDK |
| `IsDownloading` | Gets a bool indicating whether map data is downloading. <br>Returns: Returns `Boolean`. | SDK |
| `IsIdle` | Gets whether the map is idle. <br>Returns: Returns `Boolean`. | SDK |
| `LoadingException` | Gets the exception that occurred while loading the map. <br>Returns: Returns `Exception`. | SDK |
| `TargetBoundingRectangle` | Gets the bounding rectangle that defines the boundaries of the map view towards which the map is animating. <br>Returns: Returns `LocationRect`. | SDK |
| `TargetCenter` | Gets the location that defines the center of the map view towards which the map is animating. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `TargetHeading` | Gets the heading of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetPitch` | Gets the pitch of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetZoomLevel` | Gets the zoom level of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `ViewportSize` | Gets the size of the viewport. <br>Returns: Returns `Size`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `LoadingError` | Occurs when there is an error loading the map. | SDK |
| `MapPan` | Occurs when the screen is touched and dragged to pan the map. This event occurs every frame. | SDK |
| `MapResolved` | Occurs when the map tiles are fully downloaded, regardless of whether the user is still interacting with the map. | SDK |
| `MapZoom` | Occurs when the map is touched to zoom the map. | SDK |
| `ModeChanged` | Occurs when the map mode changes. | SDK |
| `TargetViewChanged` | Occurs when the view towards which the map is animating changes. | SDK |
| `ViewChangeEnd` | Occurs when the view is done changing. | SDK |
| `ViewChangeOnFrame` | Occurs for every frame of a view change. | SDK |
| `ViewChangeStart` | Occurs when the view starts changing. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapCore` class. | SDK |
| `ArrangeOverride(Size)` | Arranges the elements to fit the given window size. <br>`finalSize`: The final size of the window. <br>Returns: Returns `Size`. | SDK |
| `LocationToViewportPoint(Device.Location.GeoCoordinate)` | Converts a location to a viewport point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `MeasureOverride(Size)` | Measures the map elements so that they fit within the map size. <br>`availableSize`: The available size for the map elements. <br>Returns: Returns `Size`. | SDK |
| `OnAnimationLevelChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the animation level of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnCenterChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the center of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnCredentialsProviderChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the map credentials provider object changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnCultureChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the culture of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnFirstFrame` | When overridden in a derived class, executes on the first map view frame. | SDK |
| `OnHeadingChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the heading of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnModeChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the mode of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnPitchChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the pitch of the map changes. <br>`eventArgs`: The event data to use. | SDK |
| `OnZoomLevelChanged(DependencyPropertyChangedEventArgs)` | Enables derived classes to provide custom handling when the pitch of the map view changes. <br>`eventArgs`: The event data to use. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling when the map projection is updated. <br>`updateLevel`: The projection update level. | SDK |
| `SetMode(Microsoft.Phone.Controls.Maps.Core.MapMode,Boolean)` | Sets the mode of the map. <br>`newMode`: The new map mode. <br>`transferSettings`: A bool indicating whether to transfer view and animation level settings from the old map mode. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double)` | Sets the map view based on the specified center and zoom level. <br>`center`: The location of the center of the map view. <br>`zoomLevel`: The zoom level of the map view. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double)` | Sets the map view based on the specified center, zoom level, and heading. <br>`center`: The location of the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double,Double)` | Sets the map view based on the specified center, zoom level, heading, and pitch. <br>`center`: The location of the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. <br>`pitch`: The pitch of the map view. | SDK |
| `SetView(Microsoft.Phone.Controls.Maps.LocationRect)` | Sets the map view using the specified location rectangle. <br>`boundingRectangle`: The location rectangle that defines the borders of the map view. | SDK |
| `TryLocationToViewportPoint(Device.Location.GeoCoordinate,Point@)` | Tries to convert a location to a viewport point. <br>`location`: The location to convert. <br>`viewportPoint`: The location as a viewport point. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `TryViewportPointToLocation(Point,Device.Location.GeoCoordinate@)` | Tries to convert a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>`location`: The viewport point as a location. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `ViewportPointToLocation(Point)` | Converts a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapMode

Source: SDK

Defines the base class for a map mode. This class must be inherited.

### Properties

| Member | Description | Src |
|---|---|---|
| `AnimationLevel` | Gets or sets the animation level of the mode. <br>Returns: Returns `AnimationLevel`. | SDK |
| `Attributions` | When overridden in a derived class, gets the imagery attributions for the mode. <br>Returns: Returns `AttributionCollection`. | SDK |
| `BoundingRectangle` | When overridden in a derived class, gets the rectangle that defines the boundaries of the map view. <br>Returns: Returns `LocationRect`. | SDK |
| `Center` | When overridden in a derived class, gets or sets the center of the map view. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `Content` | When overridden in a derived class, gets the UIElement that represents the map mode. <br>Returns: Returns `UIElement`. | SDK |
| `CredentialsProvider` | When overridden in a derived class, gets or sets the credentials provider for the mode. <br>Returns: Returns `CredentialsProvider`. | SDK |
| `Culture` | Gets or sets the culture of the map mode. <br>Returns: Returns `CultureInfo`. | SDK |
| `ForegroundContent` | When overridden in a derived class, gets the map foreground content as a UIElement. <br>Returns: Returns `UIElement`. | SDK |
| `Heading` | When overridden in a derived class, gets or sets the heading of the map view. <br>Returns: Returns `Double`. | SDK |
| `HeadingRange` | When overridden in a derived class, gets the range of valid heading values. <br>Returns: Returns `Range`1`. | SDK |
| `IsDownloading` | Gets or sets whether map data is downloading. <br>Returns: Returns `Boolean`. | SDK |
| `IsIdle` | Gets whether the map is idle. <br>Returns: Returns `Boolean`. | SDK |
| `ModeBackground` | When overridden in a derived class, gets the mode of the background. <br>Returns: Returns `ModeBackground`. | SDK |
| `Pitch` | When overridden in a derived class, gets or sets the pitch of the map view. <br>Returns: Returns `Double`. | SDK |
| `PitchRange` | When overridden in a derived class, gets the range of the valid pitch values. <br>Returns: Returns `Range`1`. | SDK |
| `Scale` | When overridden in a derived class, gets or sets the scale of the map. <br>Returns: Returns `Double`. | SDK |
| `TargetBoundingRectangle` | When overridden in a derived class, gets the location rectangle that defines the boundaries of the map view towards which the map is animating. <br>Returns: Returns `LocationRect`. | SDK |
| `TargetCenter` | When overridden in a derived class, gets the location of the center of the map view towards which the map is animating. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `TargetHeading` | When overridden in a derived class, gets the heading of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetPitch` | When overridden in a derived class, gets the pitch of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetScale` | When overridden in a derived class, gets the scale of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `TargetZoomLevel` | When overridden in a derived class, gets the zoom level of the map view towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `ViewportSize` | Gets or sets the size of the viewport. <br>Returns: Returns `Size`. | SDK |
| `ZoomLevel` | When overridden in a derived class, gets or sets the zoom level. <br>Returns: Returns `Double`. | SDK |
| `ZoomRange` | When overridden in a derived class, gets the range of valid zoom levels for the mode. <br>Returns: Returns `Range`1`. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `ProjectionChanged` | Occurs when the map projection changes. | SDK |
| `TargetViewChanged` | Occurs when the map view towards which the map is animating changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapMode` class. | SDK |
| `Activated(Microsoft.Phone.Controls.Maps.MapLayerBase,Microsoft.Phone.Controls.Maps.MapLayerBase)` | When overridden in a derived class, executes when the map mode is activated. <br>`modeLayer`: The map mode layer. <br>`modeForegroundLayer`: The map mode foreground layer. | SDK |
| `Activating(Microsoft.Phone.Controls.Maps.Core.MapMode,Microsoft.Phone.Controls.Maps.MapLayerBase,Microsoft.Phone.Controls.Maps.MapLayerBase)` | When overridden in a derived class, executes when the map mode is activating. <br>`previousMode`: The previous map mode. <br>`modeLayer`: The map mode layer. <br>`modeForegroundLayer`: The map mode foreground layer. | SDK |
| `Deactivating` | When overridden in a derived class, executes when the map mode is deactivating. | SDK |
| `LocationToViewportPoint(Device.Location.GeoCoordinate)` | When overridden in a derived class, converts a location to a viewport point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `LocationToViewportPoint(IEnumerable{Device.Location.GeoCoordinate})` | When overridden in a derived class, converts a collection of locations viewport points. <br>`locations`: The locations to convert. <br>Returns: Returns `IEnumerable`1`. | SDK |
| `LocationToViewportPoint(Microsoft.Phone.Controls.Maps.LocationRect)` | When overridden in a derived class, converts a location rectangle to a viewport point rectangle. <br>`boundingRectangle`: The location rectangle to convert. <br>Returns: Returns `Rect`. | SDK |
| `OnCultureChanged` | Enables derived classes to provide custom handling when the culture of the map mode changes. | SDK |
| `OnMapDrag(Microsoft.Phone.Controls.Maps.MapDragEventArgs)` | Raises the `MapPan` event. <br>`e`: The event data. | SDK |
| `OnMapFlick(Microsoft.Phone.Controls.Maps.MapFlickEventArgs)` | Raises the `MapPan` event. <br>`e`: The event data. | SDK |
| `OnMapZoom(Microsoft.Phone.Controls.Maps.MapZoomEventArgs)` | Raises the `MapZoom` event. <br>`e`: The event data. | SDK |
| `OnProjectionChanged(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling when the projection of the map mode changes. <br>`updateLevel`: The projection update level. | SDK |
| `OnTargetViewChanged` | Enables derived classes to provide custom handling when the target view of the map mode changes. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double,Double,Boolean)` | When overridden in a derived class, sets the map view to the specified center, zoom level, heading, and pitch. <br>`center`: The location of the center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. <br>`pitch`: The pitch of the map view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `SetView(Microsoft.Phone.Controls.Maps.LocationRect,Boolean)` | When overridden in a derived class, sets the map view to the specified location rectangle. <br>`boundingRectangle`: The location rectangle that defines the boundaries of the map view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `TryLocationToViewportPoint(Device.Location.GeoCoordinate,Point@)` | When overridden in a derived class, tries to convert a location to a viewport point. <br>`location`: The location to convert. <br>`viewportPoint`: The location as a viewport point. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `TryViewportPointToLocation(Point,Device.Location.GeoCoordinate@)` | When overridden in a derived class, tries to convert a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>`location`: The viewport point as a location. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `ViewportPointToLocation(Point)` | When overridden in a derived class, converts a viewport point to a location in this map mode. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `ViewportSizeChanged(Size)` | When overridden in a derived class, executes when the size of the viewport changes. <br>`viewportSize`: The new viewport size. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MapShapeBase

Source: SDK

Represents the base class for a shape on the map. This class must be inherited.

### Properties

| Member | Description | Src |
|---|---|---|
| `EmbeddedShape` | Gets the shape. <br>Returns: Returns `Shape`. | SDK |
| `Fill` | Gets or sets the fill of the shape. <br>Returns: Returns `Brush`. | SDK |
| `Locations` | Gets or sets the locations that define the vertices of the shape. <br>Returns: Returns `LocationCollection`. | SDK |
| `ParentMap` | Gets the parent map of the shape. <br>Returns: Returns `MapBase`. | SDK |
| `ProjectedPoints` | When overridden in a derived class, gets or sets the projected points of the shape. Use this property to arrange the points of derived classes. <br>Returns: Returns `PointCollection`. | SDK |
| `Stroke` | Gets or sets the stroke color of the shape. <br>Returns: Returns `Brush`. | SDK |
| `StrokeDashArray` | Gets or sets the StrokeDashArray value of the shape. <br>Returns: Returns `DoubleCollection`. | SDK |
| `StrokeDashCap` | Gets or sets the StrokeDashCap value for the shape. <br>Returns: Returns `PenLineCap`. | SDK |
| `StrokeDashOffset` | Gets or sets the StrokeDashOffset value for the shape. <br>Returns: Returns `Double`. | SDK |
| `StrokeEndLineCap` | Gets or sets the StrokeEndLineCap value for the shape. <br>Returns: Returns `PenLineCap`. | SDK |
| `StrokeLineJoin` | Gets or sets the StrokeLineJoin value for the shape. <br>Returns: Returns `PenLineJoin`. | SDK |
| `StrokeMiterLimit` | Gets or sets the StrokeMiterLimit value for the shape. <br>Returns: Returns `Double`. | SDK |
| `StrokeStartLineCap` | Gets or sets the StrokeStartLineCap value for the shape. <br>Returns: Returns `PenLineCap`. | SDK |
| `StrokeThickness` | Gets or sets the StrokeThickness value for the shape. <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapShapeBase` class. | SDK |
| `ArrangeOverride(Size)` | Arranges the shape to fit on the map. <br>`finalSize`: The size in which to fit the shape. <br>Returns: Returns the actual `Size` used. | SDK |
| `MeasureOverride(Size)` | Measures the shape to fit the available size. <br>`availableSize`: The window size available. <br>Returns: Returns the actual `Size` needed by the shape and its children. | SDK |
| `OnApplyTemplate` | Executes whenever application code or internal processes (such as a rebuilding layout pass) call System.Windows.Controls.Control.ApplyTemplate(). | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling of the shape when the projection is updated. <br>`updateLevel`: The projection update level. | SDK |
| `SetEmbeddedShape(Shapes.Shape)` | Sets the new embedded shape object. <br>`newShape`: The new shape. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MatrixHelper

Source: SDK

Contains helper methods for working with the `Matrix` class.

### Methods

| Member | Description | Src |
|---|---|---|
| `Rotate(Media.Matrix@,Double)` | Rotates the given matrix to the specified angle. <br>`matrix`: The matrix to rotate. <br>`angle`: The angle to which to rotate. | SDK |
| `RotateAt(Media.Matrix@,Double,Point)` | Rotates the given matrix to the specified angle around the specified center. <br>`matrix`: The matrix to rotate. <br>`angle`: The angle to which to rotate. <br>`center`: The center point around which to rotate the matrix. | SDK |
| `Translate(Media.Matrix@,Double,Double)` | Translates the given matrix based on the given X and Y values. <br>`matrix`: The matrix to translate. <br>`translateX`: The X value. <br>`translateY`: The Y value. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MercatorMode

Source: SDK

Represents a Mercator map mode.

### Properties

| Member | Description | Src |
|---|---|---|
| `Attributions` | Gets the map mode imagery attributions. <br>Returns: Returns `AttributionCollection`. | SDK |
| `BoundingRectangle` | Gets the location rectangle that defines the boundaries of the mode. <br>Returns: Returns `LocationRect`. | SDK |
| `Heading` | Gets the directional heading of the map view. <br>Returns: Returns `Double`. | SDK |
| `HeadingRange` | Gets the heading range supported for the Mercator map mode. <br>Returns: Returns `Range`1`. | SDK |
| `Pitch` | Gets the pitch of the current map view. <br>Returns: Returns `Double`. | SDK |
| `PitchRange` | Gets the valid range of the pitch. <br>Returns: Returns `Range`1`. | SDK |
| `Scale` | Gets or sets the scale of the map. <br>Returns: Returns `Double`. | SDK |
| `TargetBoundingRectangle` | Gets the rectangle that defines the boundaries of the map view towards which the map is animating. <br>Returns: Returns `LocationRect`. | SDK |
| `TargetScale` | Gets the scale of the map towards which the map is animating. <br>Returns: Returns `Double`. | SDK |
| `ZoomRange` | Gets the range of the valid zoom levels. <br>Returns: Returns `Range`1`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MercatorMode` class. | SDK |
| `ConstrainView(Device.Location.GeoCoordinate,Double@,Double@,Double@)` | Modifies the given parameters to valid values based on the mode limitations. <br>`center`: The center of the view. <br>`zoomLevel`: The zoom level of the view to modify. <br>`heading`: The heading of the view to modify. <br>`pitch`: The pitch of the view to modify. <br>Returns: Returns `Boolean`. | SDK |
| `GetZoomRange(Device.Location.GeoCoordinate)` | When overridden in a derived class, gets the zoom range of the specified location in this mode. <br>`center`: The center location to use when retrieving valid zoom levels. <br>Returns: Returns `Range`1`. | SDK |
| `LocationRectToLogicalRect(Microsoft.Phone.Controls.Maps.LocationRect)` | Converts a location rectangle to a rectangle in logical space. <br>`boundingRectangle`: The rectangle to convert. <br>Returns: Returns `Rect`. | SDK |
| `LocationToLogicalPoint(Device.Location.GeoCoordinate)` | Converts a location to a logical point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `LocationToLogicalPoint(IEnumerable{Device.Location.GeoCoordinate})` | Converts a location collection to logical points. <br>`locations`: The locations to convert. <br>Returns: Returns `IEnumerable`1`. | SDK |
| `LogicalPointToLocation(Point)` | Converts a logical point to a location in the Mercator map mode. <br>`logicalPoint`: The logical point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `OnProjectionChanged(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling when the projection changes. <br>`updateLevel`: The projection update level. | SDK |

## Microsoft.Phone.Controls.Maps.Core.MercatorUtility

Source: SDK

Contains helper methods for Mercator projection calculations.

### Fields

| Member | Description | Src |
|---|---|---|
| `EarthCircumferenceInMeters` | Specifies the circumference of the Earth, in meters. | SDK |
| `EarthRadiusInMeters` | Specifies the radius of the Earth, in meters. | SDK |
| `MercatorLatitudeLimit` | Specifies the maximum allowed latitude in a Mercator projection. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `DegreesToRadians(Double)` | Converts degrees to radians. <br>`deg`: The degrees to convert. <br>Returns: Returns `Double`. | SDK |
| `LocationToLogicalPoint(Device.Location.GeoCoordinate)` | Converts a location to a logical point. <br>`location`: The `GeoCoordinate` to convert. <br>Returns: Returns `Point`. | SDK |
| `LogicalPointToLocation(Point)` | Converts a `Point` to a `GeoCoordinate` in a Mercator projection. <br>`logicalPoint`: The point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `NormalizeLatitude(Double)` | Modifies a given latitude to an equivalent latitude within the valid latitude range (-85.051128 to 85.051128). <br>`latitude`: The latitude to normalize. <br>Returns: Returns `Double`. | SDK |
| `NormalizeLocation(Device.Location.GeoCoordinate)` | Modifies the latitude and longitude values of a given location to be within valid latitude (-85.051128 to 85.051128) and longitude (-180 to 180) ranges. <br>`location`: The `GeoCoordinate` that contains the latitude and longitude to normalize. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `NormalizeLogicalPoint(Point,Point)` | Modifies the logical point to be within the range (-0.5 to 0.5) of the center logical point. <br>`logicalPoint`: The logical point to normalize. <br>`centerLogicalPoint`: The center logical point to use in the normalization. <br>Returns: Returns `Point`. | SDK |
| `NormalizeLongitude(Double)` | Modifies a given longitude to an equivalent longitude within the valid longitude range (-180 to 180). <br>`longitude`: The longitude to normalize. <br>Returns: Returns `Double`. | SDK |
| `ScaleToZoom(Size,Double,Device.Location.GeoCoordinate)` | Calculates the zoom level of the specified location. <br>`logicalAreaSizeInScreenSpaceAtLevel1`: The available screen size. This parameter specifies the width and height (in pixels) of zoom level 1. This should be 512x512, unless you are using your own non-standard tiles. <br>`scale`: The scale of the map, in meters per pixel. <br>`location`: The center location to use. The scale changes depending on the latitude, so you need to specify what location the scale is in reference to. <br>Returns: Returns `Double`. | SDK |
| `ZoomToScale(Size,Double,Device.Location.GeoCoordinate)` | Calculates the scale based on the specified location and zoom level. <br>`logicalAreaSizeInScreenSpaceAtLevel1`: The available screen size. This parameter specifies the width and height (in pixels) of zoom level 1. This should be 512x512, unless you are using your own non-standard tiles. <br>`zoomLevel`: The zoom level of the map. <br>`location`: The center location to use. The scale changes depending on the latitude, so you need to specify what location the scale is in reference to. <br>Returns: Returns `Double`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.ModeBackground

Source: SDK

Contains background styles that allow the foreground controls to choose appropriate foreground styles.

### Fields

| Member | Description | Src |
|---|---|---|
| `Dark` | The background is more typically dark (black). | SDK |
| `Light` | The background is more typically light (white). | SDK |

## Microsoft.Phone.Controls.Maps.Core.NullMode

Source: SDK

Represents the null map mode.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `NullMode` class. | SDK |
| `LocationToViewportPoint(Device.Location.GeoCoordinate)` | Converts a location to a viewport point. <br>`location`: The location to convert. <br>Returns: Returns `Point`. | SDK |
| `SetView(Device.Location.GeoCoordinate,Double,Double,Double,Boolean)` | Sets the map view. <br>`center`: The center of the map view. <br>`zoomLevel`: The zoom level of the map view. <br>`heading`: The heading of the map view. <br>`pitch`: The pitch of the map view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `SetView(Microsoft.Phone.Controls.Maps.LocationRect,Boolean)` | Sets the map view to the specified location rectangle. <br>`boundingRectangle`: The location rectangle to use to define the borders of the view. <br>`animate`: A bool indicating whether to animate map navigation. | SDK |
| `TryLocationToViewportPoint(Device.Location.GeoCoordinate,Point@)` | Tries to convert a location to a viewport point. <br>`location`: The location to convert. <br>`viewportPoint`: The `GeoCoordinate` converted to a viewport point. <br>Returns: Returns `Boolean` indicating if the conversion was successful. | SDK |
| `TryViewportPointToLocation(Point,Device.Location.GeoCoordinate@)` | Tries to convert a viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>`location`: The viewport point converted to a `GeoCoordinate`. <br>Returns: Returns a `Boolean` indicating if the conversion was successful. | SDK |
| `ViewportPointToLocation(Point)` | Converts the specified viewport point to a location. <br>`viewportPoint`: The viewport point to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |

## Microsoft.Phone.Controls.Maps.Core.TileSourceCollection

Source: SDK

Contains a collection of `TileSource` objects.

### Events

| Member | Description | Src |
|---|---|---|
| `ItemPropertyChanged` | Occurs when a property of one of the collection item has changed. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `TileSourceCollection` class. | SDK |

## Microsoft.Phone.Controls.Maps.Core.TileSourcePropertyChangedEventArgs

Source: SDK

Provides data for the `PropertyChanged` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `PropertyName` | Gets the name of the property that changed. <br>Returns: Returns `String`. | SDK |
| `TileSource` | Gets the name of the tile source. <br>Returns: Returns `TileSource`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Microsoft.Phone.Controls.Maps.TileSource,String)` | Initializes a new instance of the `TileSourcePropertyChangedEventArgs` class. <br>`source`: The tile source that contains the property that changed. <br>`property`: The name of the property that changed. | SDK |

## Microsoft.Phone.Controls.Maps.Core.UriSchemeNotSupportedException

Source: SDK

Represents the exception that is thrown when the URI scheme used to host the map control is not supported.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `UriSchemeNotSupportedException` class. | SDK |
| `Constructor(String)` | Initializes a new instance of the `UriSchemeNotSupportedException` class. <br>`message`: The error message. | SDK |
| `Constructor(String,Exception)` | Initializes a new instance of the `UriSchemeNotSupportedException` class. <br>`message`: The error message. <br>`innerException`: The inner exception that caused the error. | SDK |

## Microsoft.Phone.Controls.Maps.Credentials

Source: SDK

Contains information used to authenticate map requests.

### Properties

| Member | Description | Src |
|---|---|---|
| `ApplicationId` | Gets or sets the application ID for these credentials. <br>Returns: Returns `String`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Credentials` class. | SDK |
| `Equals(Object)` | Determines whether the credentials are equal to the specified object. <br>`obj`: The object to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `GetHashCode` | Retrieves the hash code for the credentials object. <br>Returns: Returns `Int32`. | SDK |
| `ToString` | Converts the credentials to a string. <br>Returns: Returns `String`. | SDK |

## Microsoft.Phone.Controls.Maps.CredentialsProvider

Source: SDK

Provides credentials used to authenticate map requests.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `CredentialsProvider` class. | SDK |
| `GetCredentials(Action{Microsoft.Phone.Controls.Maps.Credentials})` | When overridden in a derived class, retrieves the credentials used by the map control. <br>`callback`: The function to call when the credentials are retrieved. | SDK |

## Microsoft.Phone.Controls.Maps.Design.ApplicationIdCredentialsProviderConverter

Source: SDK

Provides the ability to convert an object to a `ApplicationIdCredentialsProvider`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ApplicationIdCredentialsProviderConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the given type can be converted to an `ApplicationIdCredentialsProvider`. <br>`context`: The context of the conversion. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to an `ApplicationIdCredentialsProvider`. <br>`context`: The context to use in the conversion. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.LocationCollectionConverter

Source: SDK

Provides the ability to convert an object to a `LocationCollection`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationCollectionConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `LocationCollection`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `LocationCollection`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.LocationConverter

Source: SDK

Provides the ability to convert an object to a `GeoCoordinate`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `GeoCoordinate`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `GeoCoordinate`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.LocationRectConverter

Source: SDK

Provides the ability to convert an object to a `LocationRect`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationRectConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `LocationRect`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `LocationRect`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.MapModeConverter

Source: SDK

Provides the ability to convert an object to a `MapMode`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapModeConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `MapMode`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `MapMode`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.PositionOriginConverter

Source: SDK

Provides the ability to convert an object to a `PositionOrigin`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `PositionOriginConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `PositionOrigin`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `PositionOrigin`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.Design.RangeConverter

Source: SDK

Provides the ability to convert an object to a `Range`1`.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RangeConverter` class. | SDK |
| `CanConvertFrom(ComponentModel.ITypeDescriptorContext,Type)` | Determines whether the specified type can be converted to a `Range`1`. <br>`context`: The format context provider of the type. <br>`sourceType`: The type of the object to convert. <br>Returns: Returns `Boolean`. | SDK |
| `ConvertFrom(ComponentModel.ITypeDescriptorContext,Globalization.CultureInfo,Object)` | Converts the specified object to a `Range`1`. <br>`context`: The format context provider of the type. <br>`culture`: The culture to use in the conversion. <br>`value`: The object to convert. <br>Returns: Returns `Object`. | SDK |

## Microsoft.Phone.Controls.Maps.LoadingErrorEventArgs

Source: SDK

Provides data for a `LoadingError` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `LoadingException` | Gets the exception that caused the map loading error. <br>Returns: Returns `Exception`. | SDK |

## Microsoft.Phone.Controls.Maps.LocationCollection

Source: SDK

Contains a collection of `GeoCoordinate` items.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationCollection` class. | SDK |

## Microsoft.Phone.Controls.Maps.LocationRect

Source: SDK

Represents a rectangle on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `Center` | Gets the location of the center of the rectangle. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `East` | Gets or sets the longitude of the eastern border of the rectangle. <br>Returns: Returns `Double`. | SDK |
| `Height` | Gets the height of the location rectangle. <br>Returns: Returns `Double`. | SDK |
| `North` | Gets or sets the latitude of the northern border of the rectangle. <br>Returns: Returns `Double`. | SDK |
| `Northeast` | Gets or sets the location of the northeast corner of the rectangle. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `Northwest` | Gets the location of the northwest corner of the rectangle. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `South` | Gets or sets the latitude of the southern border of the rectangle. <br>Returns: Returns `Double`. | SDK |
| `Southeast` | Gets the location of the southeast corner of the rectangle. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `Southwest` | Gets or sets the location of the southwest corner of the rectangle. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `West` | Gets or sets the longitude of the western border of the rectangle. <br>Returns: Returns `Double`. | SDK |
| `Width` | Gets or sets the width of the rectangle. <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationRect` class. | SDK |
| `Constructor(Device.Location.GeoCoordinate,Double,Double)` | Initializes a new instance of the `LocationRect` class, centered on the specified location. <br>`center`: The location of the center of the rectangle. <br>`width`: The width of the rectangle. <br>`height`: The height of the rectangle. | SDK |
| `Constructor(Double,Double,Double,Double)` | Initializes a new instance of the `LocationRect` class, using the specified borders. <br>`north`: The latitude of the northern border of the rectangle. <br>`west`: The longitude of the western border of the rectangle. <br>`south`: The latitude of the southern border of the rectangle. <br>`east`: The longitude of the eastern border of the rectangle. | SDK |
| `Constructor(Microsoft.Phone.Controls.Maps.LocationRect)` | Initializes a new instance of the `LocationRect` class. <br>`rect`: The location rectangle to use. | SDK |
| `CreateLocationRect(Device.Location.GeoCoordinate[])` | Creates a location rectangle based on an array of locations. <br>`locations`: The array of locations to use as the corners of the location rectangle. <br>Returns: Returns `LocationRect`. | SDK |
| `CreateLocationRect(IEnumerable{Device.Location.GeoCoordinate})` | Creates a location rectangle based on a collection of locations. <br>`locations`: A collection of locations that define the corners of the location rectangle. <br>Returns: Returns `LocationRect`. | SDK |
| `Equals(Microsoft.Phone.Controls.Maps.LocationRect)` | Determines whether this location rectangle is equal to the specified object. <br>`value`: The location rectangle to compare to <br>Returns: Returns `Boolean`. | SDK |
| `Equals(Object)` | Determines whether this location rectangle is equal to the specified object. <br>`obj`: The object to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `GetHashCode` | Retrieves the hash code for this location rectangle. <br>Returns: Returns `Int32`. | SDK |
| `Intersection(Microsoft.Phone.Controls.Maps.LocationRect)` | Retrieves the intersection rectangle of this location rectangle and the specified location rectangle. <br>`rect`: The location rectangle to use. <br>Returns: Returns `LocationRect`. | SDK |
| `Intersects(Microsoft.Phone.Controls.Maps.LocationRect)` | Determines whether this location rectangle intersects with the specified location rectangle. <br>`rect`: The location rectangle to use. <br>Returns: Returns `Boolean`. | SDK |
| `System#IFormattable#ToString(String,IFormatProvider)` | Converts the location rectangle to a formatted string containing the latitude, longitude, and altitude values of its corners using a given format provider. <br>`format`: The string format. <br>`provider`: A System.IFormatProvider implementation that supplies culture-specific formatting information. <br>Returns: Returns `String`. | SDK |
| `ToString` | Converts the location rectangle to a formatted string containing the latitude, longitude, and altitude values of its corners. <br>Returns: Returns `String`. | SDK |
| `ToString(IFormatProvider)` | Converts the location rectangle to a formatted string containing the latitude, longitude, and altitude values of its corners using a given format provider. <br>`provider`: A System.IFormatProvider implementation that supplies culture-specific formatting information. <br>Returns: Returns `String`. | SDK |

## Microsoft.Phone.Controls.Maps.LocationRectTileSource

Source: SDK

Represents map tiles contained within a location rectangle that are available on servers.

### Properties

| Member | Description | Src |
|---|---|---|
| `BoundingRectangle` | Gets or sets the rectangle which contains the tile source. <br>Returns: Returns `LocationRect`. | SDK |
| `ZoomRange` | Gets or sets the zoom range of the tile source. <br>Returns: Returns `Range`1`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LocationRectTileSource` class. | SDK |
| `Constructor(String,Microsoft.Phone.Controls.Maps.LocationRect,Microsoft.Phone.Controls.Maps.Range{Double})` | Initializes a new instance of the `LocationRectTileSource` class with the specified tile source URI, the bounding rectangle within which tiles can be displayed, and the valid zoom range.. <br>`uriFormat`: The URI of the tile source. <br>`boundingRectangle`: The area on the map within which tiles can be displayed. <br>`zoomRange`: The zoom range where tiles are visible. | SDK |
| `GetUri(Int32,Int32,Int32)` | Retrieves the URI of a tile at the given point and zoom level. <br>`x`: The horizontal position of the tile. <br>`y`: The vertical position of the tile. <br>`zoomLevel`: The zoom level of the tile. <br>Returns: Returns `Uri`. | SDK |

## Microsoft.Phone.Controls.Maps.Map

Source: SDK

Represents the default map class.

### Properties

| Member | Description | Src |
|---|---|---|
| `CredentialsProvider` | Gets or sets the credentials provider. <br>Returns: Returns `CredentialsProvider`. | SDK |
| `MapForeground` | Gets the map foreground. <br>Returns: Returns `MapForeground`. | SDK |
| `Mode` | Gets or sets the map mode. <br>Returns: Returns `MapMode`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Map` class. | SDK |

## Microsoft.Phone.Controls.Maps.MapDragEventArgs

Source: SDK

Provides data for the `MapPan` event. This class inherits from the `MapInputEventArgs` class.

### Properties

| Member | Description | Src |
|---|---|---|
| `DragDelta` | Gets the screen amount from the current viewport point that was touched. <br>Returns: Returns `Point`. | SDK |

## Microsoft.Phone.Controls.Maps.MapEventArgs

Source: SDK

Provides data for `Map` events.

### Properties

| Member | Description | Src |
|---|---|---|
| `Handled` | Gets or sets whether the map event is handled. <br>Returns: Returns `Boolean`. | SDK |

## Microsoft.Phone.Controls.Maps.MapFlickEventArgs

Source: SDK

Provides data for the flick events. This class inherits from the `MapInputEventArgs` class.

### Properties

| Member | Description | Src |
|---|---|---|
| `Velocity` | Gets the screen amount from the current viewport point that was touched. <br>Returns: Returns `Point`. | SDK |

## Microsoft.Phone.Controls.Maps.MapInputEventArgs

Source: SDK

Provides data for map input events.

### Properties

| Member | Description | Src |
|---|---|---|
| `Handled` | Gets or sets whether the input event is handled. <br>Returns: Returns `Boolean`. | SDK |
| `ViewportPoint` | Gets the viewport point on the screen where the map event occurred. <br>Returns: Returns `Point`. | SDK |

## Microsoft.Phone.Controls.Maps.MapItemsControl

Source: SDK

Represents the class that uses a `MapLayer` as an ItemsPanel. This enables data binding using an ItemsSource and an ItemTemplate. This class inherits from the ItemsControl class.

### Properties

| Member | Description | Src |
|---|---|---|
| `ParentMap` | Gets the map that owns this map items control. <br>Returns: Returns `MapBase`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapItemsControl` class. | SDK |
| `ArrangeOverride(Size)` | Arranges the elements so that they fit into the control. <br>`finalSize`: The final size available. <br>Returns: Returns `Size`. | SDK |
| `MeasureOverride(Size)` | Measures the map items control to see if it will fit into the available size. <br>`availableSize`: The size available into which the map items control must fit. <br>Returns: Returns `Size` that is the size needed to fit the map items control. | SDK |
| `OnApplyTemplate` | Executes whenever application code or internal processes (such as a rebuilding layout pass) call System.Windows.Controls.Control.ApplyTemplate(). In simplest terms, this means the method is called just before the UI element displays in an application. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Updates the elements if the projection is updated. <br>`updateLevel`: The projection update level as a `ProjectionUpdateLevel`. | SDK |

## Microsoft.Phone.Controls.Maps.MapLayer

Source: SDK

Represents a map layer, which positions its child `UIElement` objects using geographic coordinates.

### Properties

| Member | Description | Src |
|---|---|---|
| `ParentMap` | Gets the parent map of the map layer. <br>Returns: Returns `MapBase`. | SDK |
| `Position` | Gets or sets the position of the map layer. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `PositionOffset` | Gets or sets the map layer’s position offset. <br>Returns: Returns `Point`. | SDK |
| `PositionOrigin` | Gets or sets the map layer’s position origin. <br>Returns: Returns `PositionOrigin`. | SDK |
| `PositionRectangle` | Gets or sets a `LocationRect` that UIElement objects of the layer are fit into (also known as “arranging”). <br>Returns: Returns `LocationRect`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `PositionOffsetProperty` | Identifies the `PositionOffset` dependency property. | SDK |
| `PositionOriginProperty` | Identifies the `PositionOrigin` dependency property. | SDK |
| `PositionProperty` | Identifies the `Position` dependency property. | SDK |
| `PositionRectangleProperty` | Identifies the `PositionRectangle` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapLayer` class. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate)` | Adds a UIElement to the map layer at the specified location. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate,Microsoft.Phone.Controls.Maps.PositionOrigin)` | Adds a UIElement to the map layer at the specified location and origin. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. <br>`origin`: The position origin to use. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate,Point)` | Adds a UIElement to the map layer at the specified location using the given pixel offset. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. <br>`offset`: The pixel offset to use. | SDK |
| `AddChild(UIElement,Microsoft.Phone.Controls.Maps.LocationRect)` | Adds a UIElement to the map layer at the specified location rectangle. <br>`element`: The UIElement to add. <br>`locationRect`: The location rectangle at which to add the UIElement. | SDK |
| `GetPosition(DependencyObject)` | Retrieves the position of the map layer. <br>`dependencyObject`: The dependency object. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `GetPositionOffset(DependencyObject)` | Retrieves the offset of a layer from the position calculated by the map projection, in pixels. <br>`dependencyObject`: The dependency object. <br>Returns: Returns `Point`. | SDK |
| `GetPositionOrigin(DependencyObject)` | Retrieves the map layer’s position origin. <br>`dependencyObject`: The dependency object. <br>Returns: Returns `PositionOrigin`. | SDK |
| `GetPositionRectangle(DependencyObject)` | Retrieves a `LocationRect` that UIElement objects of the layer are fit into (also known as “arranging”). <br>`dependencyObject`: The dependency object. <br>Returns: Returns `LocationRect`. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Updates the map layer elements if the map projection is updated. <br>`updateLevel`: The projection update level. | SDK |
| `SetPosition(DependencyObject,Device.Location.GeoCoordinate)` | Sets the position of the map layer. <br>`dependencyObject`: The dependency object. <br>`position`: The coordinate to use to position the map layer. | SDK |
| `SetPositionOffset(DependencyObject,Point)` | Sets the offset of a layer from the position calculated by the map projection, in pixels. <br>`dependencyObject`: The dependency object. <br>`point`: The position offset. | SDK |
| `SetPositionOrigin(DependencyObject,Microsoft.Phone.Controls.Maps.PositionOrigin)` | Sets the position origin of the map layer. <br>`dependencyObject`: The dependency object. <br>`origin`: The position origin of the map layer. | SDK |
| `SetPositionRectangle(DependencyObject,Microsoft.Phone.Controls.Maps.LocationRect)` | Sets a `LocationRect` that UIElement objects of the layer are fit into (also known as “arranging”). <br>`dependencyObject`: The dependency object. <br>`rect`: The `LocationRect` that UIElement objects of the layer are fit into. | SDK |

## Microsoft.Phone.Controls.Maps.MapLayerBase

Source: SDK

Represents the base map layer.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapLayerBase` class. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate)` | When overridden in a derived class, adds a UIElement to the map layer at the specified location. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate,Microsoft.Phone.Controls.Maps.PositionOrigin)` | When overridden in a derived class, adds a UIElement to the map layer at the specified location using the given position origin. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. <br>`origin`: The position origin to use. | SDK |
| `AddChild(UIElement,Device.Location.GeoCoordinate,Point)` | When overridden in a derived class, adds a UIElement to the map layer at the specified location using the given pixel offset. <br>`element`: The UIElement to add. <br>`location`: The location at which to add the UIElement. <br>`offset`: The pixel offset to use. | SDK |
| `AddChild(UIElement,Microsoft.Phone.Controls.Maps.LocationRect)` | When overridden in a derived class, adds a UIElement to the map layer at the specified location rectangle. <br>`element`: The UIElement to add. <br>`locationRect`: The location rectangle at which to add the UIElement. | SDK |

## Microsoft.Phone.Controls.Maps.MapPolygon

Source: SDK

Represents a polygon on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `FillRule` | Gets or sets the fill rule for the polygon. <br>Returns: Returns `FillRule`. | SDK |
| `ProjectedPoints` | Gets or sets the vertices of the polygon. <br>Returns: Returns `PointCollection`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapPolygon` class. | SDK |
| `SetEmbeddedShape(Shapes.Shape)` | Sets the new embedded shape object. <br>`newShape`: The new shape. | SDK |

## Microsoft.Phone.Controls.Maps.MapPolyline

Source: SDK

Represents a polyline on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `FillRule` | Gets or sets the fill rule for the polyline. <br>Returns: Returns `FillRule`. | SDK |
| `ProjectedPoints` | Gets or sets the points that define the polyline. <br>Returns: Returns `PointCollection`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapPolyline` class. | SDK |
| `SetEmbeddedShape(Shapes.Shape)` | Sets the new embedded shape object. <br>`newShape`: The new shape. | SDK |

## Microsoft.Phone.Controls.Maps.MapTileLayer

Source: SDK

Represents a layer of image tiles on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `ParentMap` | Gets the parent map of the map tile layer. <br>Returns: Returns `MapBase`. | SDK |
| `TileHeight` | Gets or sets the height, in pixels, of a tile in the map tile layer. <br>Returns: Returns `Int32`. | SDK |
| `TileSources` | Gets or sets a collection of tile sources associated with the map tile layer. <br>Returns: Returns `TileSourceCollection`. | SDK |
| `TileWidth` | Gets or sets the width, in pixels, of a tile in the map tile layer. <br>Returns: Returns `Int32`. | SDK |
| `UpSampleLevelDelta` | Gets or sets the sample level delta. <br>Returns: Returns `Int32`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `UpSampleLevelDeltaProperty` | Identifies the `UpSampleLevelDelta` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapTileLayer` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `ProjectionUpdated(Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel)` | Enables derived classes to provide custom handling when the projection of the tile layer changes. <br>`updateLevel`: The projection update level. | SDK |

## Microsoft.Phone.Controls.Maps.MapZoomEventArgs

Source: SDK

Provides data for zoom events.

### Properties

| Member | Description | Src |
|---|---|---|
| `ZoomDelta` | Gets the direction and extent of the zoom movement. This property returns, as a multiplier, the amount the touch manipulation has resized the map. <br>Returns: Returns `Double`. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.Copyright

Source: SDK

Represents the copyright control on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `Attributions` | Gets the copyright imagery attributions. <br>Returns: Returns `ObservableCollection`1`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Copyright` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.DistanceUnit

Source: SDK

Represents the units of distance used by the scale bar.

### Fields

| Member | Description | Src |
|---|---|---|
| `Default` | The scale bar uses the default unit of distance of the current country or region. | SDK |
| `KilometersMeters` | The scale bar uses kilometers for long distances and meters for shorter distances. | SDK |
| `MilesFeet` | The scale bar uses miles for long distances and feet for short distances. | SDK |
| `MilesYards` | The scale bar uses miles for long distances and yards for short distances. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.LoadingErrorMessage

Source: SDK

Represents a Silverlight control that shows an error message.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `LoadingErrorMessage` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling whenever an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `SetConfigurationError(Globalization.CultureInfo)` | Displays a configuration loading error. <br>`culture`: The culture to use to localize the error message or to use the default culture. | SDK |
| `SetCredentialsError(Globalization.CultureInfo)` | Displays an error if credentials are invalid. <br>`culture`: The culture to use to localize the error message or to use the default culture. | SDK |
| `SetUriSchemeError(Globalization.CultureInfo)` | Displays an error for a bad URL. <br>`culture`: The culture to use to localize the error message or to use the default culture. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.Logo

Source: SDK

Represents a logo on the map.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Logo` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling whenever an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.MapCommandBase

Source: SDK

Represents the command contract between the map control and the buttons on it.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `MapCommandBase` class. | SDK |
| `Execute(Microsoft.Phone.Controls.Maps.Core.MapBase)` | When overridden in a derived class, executes when a button on the map is invoked. <br>`map`: The map containing the navigation button. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.MapCommandEventArgs

Source: SDK

Provides data for the map button command events.

### Properties

| Member | Description | Src |
|---|---|---|
| `Command` | Gets the command associated with the event. <br>Returns: Returns `MapCommandBase`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Microsoft.Phone.Controls.Maps.Overlays.MapCommandBase)` | Initializes a new instance of the `MapCommandEventArgs` class. <br>`command`: The map command associated with the event. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.MapForeground

Source: SDK

Represents the control that displays the scale bar, the logo, the copyright, and map navigation bar on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `Copyright` | Gets the current copyright control. <br>Returns: Returns `Copyright`. | SDK |
| `Culture` | Gets or sets the culture of the map foreground controls. <br>Returns: Returns `String`. | SDK |
| `Logo` | Gets the current logo control. <br>Returns: Returns `Logo`. | SDK |
| `Scale` | Gets the current scale bar control. <br>Returns: Returns `Scale`. | SDK |
| `ZoomBar` | Gets the zoom bar overlay object on the map. <br>Returns: Returns `ZoomBar`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `CultureProperty` | Identifies the `Culture` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `OnCultureChanged(Globalization.CultureInfo,Globalization.CultureInfo)` | Enables derived classes to provide custom handling when the culture of the map changes. <br>`oldValue`: The old culture. <br>`newValue`: The new culture. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.Overlay

Source: SDK

Represents the abstract base class for a control on top of the map.

### Events

| Member | Description | Src |
|---|---|---|
| `TemplateApplied` | Occurs when the template is applied to the overlay after OnApplyTemplate executes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Overlay` class. | SDK |
| `FireTemplateApplied` | Raises the `TemplateApplied` event. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.Scale

Source: SDK

Represents the scale bar control on a map.

### Properties

| Member | Description | Src |
|---|---|---|
| `Culture` | Gets or sets the culture of the scale bar, which determines the language and default units used by the scale bar. <br>Returns: Returns `CultureInfo`. | SDK |
| `DistanceUnit` | Gets or sets the distance unit used by the scale bar. <br>Returns: Returns `DistanceUnit`. | SDK |
| `MetersPerPixel` | Gets or sets the meters per pixel to display on the scale bar. <br>Returns: Returns `Double`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `CultureProperty` | Identifies the `Culture` dependency property. | SDK |
| `DistanceUnitProperty` | Identifies the `DistanceUnit` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Scale` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `OnCultureChanged` | Enables derived classes to provide custom handling when the culture of the scale bar changes. | SDK |
| `OnPerPixelChanged` | Enables derived classes to provide custom handling when the meters per pixel of the scale bar changes. | SDK |
| `OnUnitChanged` | Enables derived classes to provide custom handling when the distance unit of the scale bar changes. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.ShadowText

Source: SDK

Represents a shadowed text label that can be added to a map.

### Properties

| Member | Description | Src |
|---|---|---|
| `ForegroundBottom` | Gets or sets the color of the shadow behind the text. <br>Returns: Returns `Brush`. | SDK |
| `ForegroundTop` | Gets or sets the color of the text. <br>Returns: Returns `Brush`. | SDK |
| `Text` | Gets or sets the text of the shadow text control on the map. <br>Returns: Returns `String`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `ForegroundBottomProperty` | Identifies the `ForegroundBottom` dependency property. | SDK |
| `ForegroundTopProperty` | Identifies the `ForegroundTop` dependency property. | SDK |
| `TextProperty` | Identifies the `Text` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ShadowText` class. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `SetForegroundColors(Media.Brush,Media.Brush)` | Sets the color of the text and its shadow. <br>`top`: The color of the text. <br>`bottom`: The color of the shadow behind the text. | SDK |
| `SetForegroundColorsForDarkBackground` | Sets colors that display best on a dark background. | SDK |
| `SetForegroundColorsForLightBackground` | Sets colors that display best on a light background. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.ZoomBar

Source: SDK

Represents the zoom control.

### Events

| Member | Description | Src |
|---|---|---|
| `ZoomMap` | Occurs when the map zooms in or out. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ZoomBar` class. | SDK |
| `Finalize` | Deconstructs the zoom bar. | SDK |
| `OnApplyTemplate` | Enables derived classes to provide custom handling whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. | SDK |
| `OnCreateAutomationPeer` | Enables derived classes to provide custom handling when an automation peer is created. <br>Returns: Returns `AutomationPeer`. | SDK |
| `OnZoomMap(Microsoft.Phone.Controls.Maps.Overlays.ZoomMapCommand)` | Raises the `ZoomMap` event. <br>`command`: The zoom command that occurred. | SDK |

## Microsoft.Phone.Controls.Maps.Overlays.ZoomMapCommand

Source: SDK

Represents the command that adjusts the position of the slider on the zoom bar.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Boolean)` | Initializes a new instance of the `ZoomMapCommand` class. <br>`zoomIn`: A bool indicating whether the command zooms in to the map. This value is true if the command zooms in and false if the command zooms out. | SDK |
| `Execute(Microsoft.Phone.Controls.Maps.Core.MapBase)` | Executes a zoom in or zoom out command on the specified map. <br>`map`: The map to zoom. | SDK |

## Microsoft.Phone.Controls.Maps.Platform.Location

Source: SDK

Represents a geographical location determined by latitude and longitude coordinates. This class contains the same members as the `GeoCoordinate` class.

### Properties

| Member | Description | Src |
|---|---|---|
| `Altitude` | Gets or sets the altitude of the location. <br>Returns: Returns `Double`. | SDK |
| `Latitude` | Gets or sets the latitude of the location. <br>Returns: Returns `Double`. | SDK |
| `Longitude` | Gets or sets the longitude of the location. <br>Returns: Returns `Double`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Location` class. | SDK |
| `System#IFormattable#ToString(String,IFormatProvider)` | Converts the location to a formatted string based on the given provider. <br>`format`: The string format. <br>`provider`: An IFormatProvider implementation that supplies culture-specific formatting information. <br>Returns: Returns `String`. | SDK |
| `ToString` | Converts a location to a string. <br>Returns: Returns `String`. | SDK |
| `ToString(IFormatProvider)` | Converts the location to a string based on the specified format provider. <br>`provider`: An IFormatProvider implementation that supplies culture-specific formatting information. <br>Returns: Returns `String`. | SDK |
| `op_Implicit(Device.Location.GeoCoordinate)` | Converts a `GeoCoordinate` to a `Location`. <br>`obj`: The `GeoCoordinate` to convert. <br>Returns: Returns `Location`. | SDK |
| `op_Implicit(Microsoft.Phone.Controls.Maps.Platform.Location)` | Converts a `Location` to a `GeoCoordinate`. <br>`obj`: The `Location` to convert. <br>Returns: Returns `GeoCoordinate`. | SDK |

## Microsoft.Phone.Controls.Maps.Platform.Rectangle

Source: SDK

Represents a location rectangle defined by northeast and southwest corners.

### Properties

| Member | Description | Src |
|---|---|---|
| `Northeast` | Gets or sets the northeast corner of the rectangle. <br>Returns: Returns `Location`. | SDK |
| `Southwest` | Gets or sets the southwest corner of the rectangle. <br>Returns: Returns `Location`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Rectangle` class. | SDK |
| `op_Implicit(Microsoft.Phone.Controls.Maps.LocationRect)` | Converts a `LocationRect` to a `Rectangle`. <br>`obj`: The `LocationRect` to convert. <br>Returns: Returns `Rectangle`. | SDK |
| `op_Implicit(Microsoft.Phone.Controls.Maps.Platform.Rectangle)` | Converts a `Rectangle` to a `LocationRect`. <br>`obj`: The `Rectangle` to convert. <br>Returns: Returns `LocationRect`. | SDK |

## Microsoft.Phone.Controls.Maps.Platform.ShapeBase

Source: SDK

Represents the base shape class. This class is used for serialization.

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `ShapeBase` class. | SDK |

## Microsoft.Phone.Controls.Maps.PositionOrigin

Source: SDK

Represents the anchor point of the Position property on a UIElement.

### Properties

| Member | Description | Src |
|---|---|---|
| `X` | Gets or sets the x-axis position of the position origin. <br>Returns: Returns `Double`. | SDK |
| `Y` | Gets or sets the y-axis position of the position origin. <br>Returns: Returns `Double`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `BottomCenter` | Specifies the bottom center of the position origin. | SDK |
| `BottomLeft` | Specifies the bottom left of the position origin. | SDK |
| `BottomRight` | Specifies the bottom right of the position origin. | SDK |
| `Center` | Specifies the center of the position origin. | SDK |
| `CenterLeft` | Specifies the center left of the position origin. | SDK |
| `CenterRight` | Specifies the center right of the position origin. | SDK |
| `TopCenter` | Specifies the top center of the position origin. | SDK |
| `TopLeft` | Specifies the top left of the position origin. | SDK |
| `TopRight` | Specifies the top right of the position origin. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Double,Double)` | Initializes a new instance of the `PositionOrigin` structure. <br>`horizontalOrigin`: The position of the origin along the x-axis. <br>`verticalOrigin`: The position of the origin along the y-axis. | SDK |
| `Equals(Microsoft.Phone.Controls.Maps.PositionOrigin)` | Determines whether the specified object is equal to this position origin. <br>`origin`: The position origin to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `Equals(Object)` | Determines whether the specified object is equal to this position origin. <br>`obj`: The object to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `GetHashCode` | Retrieves the hash code for this position origin. <br>Returns: Returns `Int32`. | SDK |
| `op_Equality(Microsoft.Phone.Controls.Maps.PositionOrigin,Microsoft.Phone.Controls.Maps.PositionOrigin)` | Determines whether two position origin objects are equal. <br>`origin1`: The first position origin to compare. <br>`origin2`: The second position origin to compare. <br>Returns: Returns `Boolean`. | SDK |
| `op_Inequality(Microsoft.Phone.Controls.Maps.PositionOrigin,Microsoft.Phone.Controls.Maps.PositionOrigin)` | Determines whether two position origins are not equal. <br>`origin1`: The first position origin to compare. <br>`origin2`: The second position origin nto compare. <br>Returns: Returns `Boolean`. | SDK |

## Microsoft.Phone.Controls.Maps.ProjectionChangedEventArgs

Source: SDK

Provides data for the `ProjectionChanged` event.

### Properties

| Member | Description | Src |
|---|---|---|
| `UpdateLevel` | Gets the new projection update level. <br>Returns: Returns `ProjectionUpdateLevel`. | SDK |

## Microsoft.Phone.Controls.Maps.ProjectionUpdateLevel

Source: SDK

Contains projection update levels, which are based on the difference between the return values of the `GeoCoordinate)` and `Point)` methods.

### Fields

| Member | Description | Src |
|---|---|---|
| `Full` | The `GeoCoordinate)` and `Point)` methods return different results. For example, this occurs when the zoom level or bird’s eye scene is changed. | SDK |
| `Linear` | The `GeoCoordinate)` and `Point)` methods return different results, but the points stay at the same location relative to each other. For example, this occurs if you pan the map in Road view but do not change the zoom level. | SDK |
| `None` | The `GeoCoordinate)` and `Point)` methods return the same results. | SDK |

## Microsoft.Phone.Controls.Maps.Pushpin

Source: SDK

Represents a pushpin on the map.

### Properties

| Member | Description | Src |
|---|---|---|
| `Location` | Gets or sets the location of the pushpin on the map. <br>Returns: Returns `GeoCoordinate`. | SDK |
| `PositionOrigin` | Gets or sets the position origin of the pushpin, which defines the position on the pushpin to anchor to the map. <br>Returns: Returns `PositionOrigin`. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `LocationDependencyProperty` | Identifies the `Location` dependency property. | SDK |
| `PositionOriginDependencyProperty` | Identifies the `PositionOrigin` dependency property. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `Pushpin` class. | SDK |

## Microsoft.Phone.Controls.Maps.QuadKey

Source: SDK

Represents a Bing Maps quad key.

### Properties

| Member | Description | Src |
|---|---|---|
| `Key` | Gets the quadkey string of the tile. <br>Returns: Returns `String`. | SDK |
| `X` | Gets the x-axis position of the tile. <br>Returns: Returns `Int32`. | SDK |
| `Y` | Gets the y-axis position of the tile. <br>Returns: Returns `Int32`. | SDK |
| `ZoomLevel` | Gets or sets the zoom level of the tile. <br>Returns: Returns `Int32`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(Int32,Int32,Int32)` | Initializes a new instance of the `QuadKey` structure. <br>`x`: The x-axis position of the tile. <br>`y`: The y-axis position of the tile. <br>`zoomLevel`: The zoom level of the tile. | SDK |
| `Constructor(String)` | Initializes a new instance of the `QuadKey` structure. <br>`quadKey`: A string containing the quadkey of a tile. | SDK |
| `Equals(Object)` | Determines whether this quadkey is equal to the specified object. <br>`obj`: The object to compare to. <br>Returns: Returns `Boolean`. | SDK |
| `GetHashCode` | Retrieves the hash code for this quad key. <br>Returns: Returns `Int32`. | SDK |
| `op_Equality(Microsoft.Phone.Controls.Maps.QuadKey,Microsoft.Phone.Controls.Maps.QuadKey)` | Determines whether two quadkeys are equal. <br>`tile1`: The first quadkey to compare. <br>`tile2`: The second quadkey to compare. <br>Returns: Returns `Boolean`. | SDK |
| `op_Inequality(Microsoft.Phone.Controls.Maps.QuadKey,Microsoft.Phone.Controls.Maps.QuadKey)` | Determines whether two quadkeys are not equal. <br>`tile1`: The first quadkey to compare. <br>`tile2`: The second quadkey to compare. <br>Returns: Returns `Boolean`. | SDK |

## Microsoft.Phone.Controls.Maps.Range`1

Source: SDK

Defines a generic range class for a given minimum and maximum value.

### Properties

| Member | Description | Src |
|---|---|---|
| `From` | Gets the minimum value of the range. <br>Returns: Returns the minimum value of the range. | SDK |
| `To` | Gets the maximum value of the range. <br>Returns: Returns the maximum value of the range. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor(`0,`0)` | Initializes a new instance of the `Range`1` class. <br>`fromValue`: The minimum value of the range. <br>`toValue`: The maximum value of the range. | SDK |

## Microsoft.Phone.Controls.Maps.RoadMode

Source: SDK

Represents the Road map mode.

### Properties

| Member | Description | Src |
|---|---|---|
| `Content` | Gets the map mode as a UIElement. <br>Returns: Returns `UIElement`. | SDK |
| `IsDownloading` | Gets a bool indicating whether map data is still downloading. <br>Returns: Returns `Boolean`. | SDK |
| `IsIdle` | Gets whether the mode is idle. <br>Returns: Returns `Boolean`. | SDK |
| `ModeBackground` | Gets the background style of the mode. <br>Returns: Returns `ModeBackground`. | SDK |
| `ShouldRestrictMaxZoomLevel` | Gets or sets whether the maximum zoom level is restricted by a coverage map. <br>Returns: Returns `Boolean`. | SDK |
| `UpSampleLevelDelta` | Gets or sets the map tile layers sample level delta. <br>Returns: Returns `Int32`. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `RoadMode` class. | SDK |
| `Activating(Microsoft.Phone.Controls.Maps.Core.MapMode,Microsoft.Phone.Controls.Maps.MapLayerBase,Microsoft.Phone.Controls.Maps.MapLayerBase)` | Notifies a mode that it is being activated by the map. <br>`previousMode`: The mode from which the map is transitioning. <br>`modeLayer`: The map mode layer. <br>`modeForegroundLayer`: The mode foreground layer. | SDK |
| `GetZoomRange(Device.Location.GeoCoordinate)` | Retrieves the valid zoom range for the specified location in the Road map mode. <br>`center`: The center location to use. <br>Returns: Returns `Range`1`. | SDK |
| `OnCultureChanged` | Enables derived classes to provide custom handling when the culture of the mode changes. | SDK |

## Microsoft.Phone.Controls.Maps.TileSource

Source: SDK

Retrieves the URI for a tile based on its zoom level and tile position.

### Properties

| Member | Description | Src |
|---|---|---|
| `UriFormat` | Gets or sets the format used to create the URI. <br>Returns: Returns a `String`. The following markers may be present in the string.{quadkey} The string in the UriFormat that will be replaced with a quad key.{UriScheme} The string in the UriFormat that will be replaced with a URI scheme.{subdomain} The string in the UriFormat that will be replaced with the sub domain. | SDK |

### Fields

| Member | Description | Src |
|---|---|---|
| `QuadKeyUriFragment` | Specifies the piece of the URI that is the quad key. | SDK |
| `SubdomainUriFragment` | Specifies the piece of the URI that is the sub-domain. | SDK |
| `UriSchemeUriFragment` | Specifies the piece of the URI that is the URI scheme. | SDK |

### Events

| Member | Description | Src |
|---|---|---|
| `PropertyChanged` | Occurs when a property of the tile changes. | SDK |

### Methods

| Member | Description | Src |
|---|---|---|
| `Constructor` | Initializes a new instance of the `TileSource` class. | SDK |
| `Constructor(String)` | Initializes a new instance of the `TileSource` class. <br>`uriFormat`: The format used to create the URI. | SDK |
| `GetSubdomain(Microsoft.Phone.Controls.Maps.QuadKey)` | Retrieves the sub-domain for the tile identified by the specified quad key. <br>`quadKey`: The quad key that identifies the tile. <br>Returns: Returns `String`. | SDK |
| `GetUri(Int32,Int32,Int32)` | Retrieves the URI for the tile specified by the given x,y coordinates and zoom level. <br>`x`: The horizontal position of the tile. <br>`y`: The vertical position of the tile. <br>`zoomLevel`: The zoom level of the tile. <br>Returns: Returns `Uri` for the tile. | SDK |
| `OnPropertyChanged(String)` | Raises the `PropertyChanged` event. <br>`propertyName`: The name of the property that changed. | SDK |
| `SetSubdomains(String[][])` | Sets a matrix that represents sub-domains. <br>`subdomains`: A string matrix that specifies the sub-domains. | SDK |
