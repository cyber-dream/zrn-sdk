# System.Device (location)

From the WP8 SDK IntelliSense XML.

## Contents

- [`System.Device.Location.CivicAddress`](#systemdevicelocationcivicaddress)
- [`System.Device.Location.CivicAddressResolver`](#systemdevicelocationcivicaddressresolver)
- [`System.Device.Location.GeoCoordinate`](#systemdevicelocationgeocoordinate)
- [`System.Device.Location.GeoCoordinateWatcher`](#systemdevicelocationgeocoordinatewatcher)
- [`System.Device.Location.GeoPositionAccuracy`](#systemdevicelocationgeopositionaccuracy)
- [`System.Device.Location.GeoPositionChangedEventArgs`1`](#systemdevicelocationgeopositionchangedeventargs1)
- [`System.Device.Location.GeoPositionPermission`](#systemdevicelocationgeopositionpermission)
- [`System.Device.Location.GeoPositionStatus`](#systemdevicelocationgeopositionstatus)
- [`System.Device.Location.GeoPositionStatusChangedEventArgs`](#systemdevicelocationgeopositionstatuschangedeventargs)
- [`System.Device.Location.GeoPosition`1`](#systemdevicelocationgeoposition1)
- [`System.Device.Location.ICivicAddressResolver`](#systemdevicelocationicivicaddressresolver)
- [`System.Device.Location.IGeoPositionWatcher`1`](#systemdevicelocationigeopositionwatcher1)
- [`System.Device.Location.ResolveAddressCompletedEventArgs`](#systemdevicelocationresolveaddresscompletedeventargs)

## System.Device.Location.CivicAddress

Represents a location expressed as a civic address.

### Properties / fields

| Member | Description |
|---|---|
| `AddressLine1` | The first line of the civic address, generally used for street name and number. <br>Returns: Type: `String`. |
| `AddressLine2` | The second line of the civic address. <br>Returns: Type: `String`. The second line of the civic address. |
| `Building` | The building name of the civic address. <br>Returns: Type: `String`. The building name of the civic address. |
| `City` | The city of the civic address. <br>Returns: Type: `String`. The city of the civic address. |
| `CountryRegion` | The country or region of the civic address. <br>Returns: Type: `String`. The country or region of the civic address. |
| `FloorLevel` | The floor level of the civic address. <br>Returns: Type: `String`. The floor level of the civic address. |
| `IsUnknown` | Gets a value indicating whether the `CivicAddress` object contains any address data. <br>Returns: Type: `Boolean`. True if any of the fields of the `CivicAddress` object are not null or empty. |
| `PostalCode` | The postal code of the civic address. <br>Returns: Type: `String`. The postal code of the civic address. |
| `StateProvince` | The state or province of the civic address. <br>Returns: Type: `String`. The state or province of the civic address. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Unknown` | Represents a `CivicAddress` that contains no data. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CivicAddress` where the value of each field is an empty string. |
| `Constructor(String,String,String,String,String,String,String,String)` | Initializes a new instance of the `CivicAddress` with the provided address data. <br>`addressLine1`: The first line of the civic address, generally used for street name and number. <br>`addressLine2`: The second line of the civic address. <br>`building`: The building name of the civic address. <br>`city`: The city of the civic address. <br>`countryRegion`: The country or region of the civic address. <br>`floorLevel`: The floor level of the civic address. <br>`postalCode`: The postal code of the civic address. <br>`stateProvince`: The state or provice of the civic address. |

## System.Device.Location.CivicAddressResolver

Attempts to obtain the civic address for a provided geographical coordinate.

### Events

| Member | Description |
|---|---|
| `ResolveAddressCompleted` | Occurs when an asynchronous request for civic address resolution is completed. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CivicAddressResolver` class. |
| `ResolveAddress(Device.Location.GeoCoordinate)` | Attempts to synchronously obtain a civic address from the provided geographic coordinate. This method is not implemented in the current release. <br>`coordinate`: The geographic coordinate from which a civic address is resolved. <br>Returns: Returns `CivicAddress`. The civic address associated with the provided geographic coordinate. |
| `ResolveAddressAsync(Device.Location.GeoCoordinate)` | Attempts to asynchronously obtain a civic address from the provided geographic coordinate. This method is not implemented in the current release. <br>`coordinate`: The geographic coordinate from which a civic address is resolved. |

## System.Device.Location.GeoCoordinate

Represents a location expressed as a geographic coordinate. The values exposed by this class use the WGS 84 standard.

### Properties / fields

| Member | Description |
|---|---|
| `Altitude` | Gets or sets the altitude of the `GeoCoordinate` in meters. <br>Returns: Type: `Double`. The altitude of the `GeoCoordinate`. |
| `Course` | Gets or sets the heading in degrees, relative to true north. `GeoCoordinate`. <br>Returns: Type: `Double`. The course of the `GeoCoordinate`. |
| `HorizontalAccuracy` | Gets or sets the horizontal accuracy of the `GeoCoordinate`, in meters. <br>Returns: Type: `Double`. The horizontal accuracy of the `GeoCoordinate`. |
| `IsUnknown` | Gets a value indicating whether the `GeoCoordinate` contains any geographic coordinate data. <br>Returns: Returns `Boolean`. true if both the latitude and longitude of the current instance are not a number. |
| `Latitude` | Gets or sets the latitude of the `GeoCoordinate`. <br>Returns: Type: `Double`. The latitude of the `GeoCoordinate`. |
| `Longitude` | Gets or sets the longitude of the `GeoCoordinate`. <br>Returns: Type: `Double`. The longitude of the `GeoCoordinate`. |
| `Speed` | Gets the speed of the `GeoCoordinate` in meters per second. <br>Returns: Type: `Double`. The speed of the `GeoCoordinate`. |
| `VerticalAccuracy` | Gets or sets the vertical accuracy of the `GeoCoordinate`, in meters. <br>Returns: Type: `Double`. The vertical accuracy of the `GeoCoordinate`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Unknown` | Represents a `GeoCoordinate` that contains no data. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GeoCoordinate` class where the value of each fields is not a number. |
| `Constructor(Double,Double)` | Initializes a new instance of the `GeoCoordinate` class with the provided latitude and longitude. <br>`latitude`: The latitude of the geographic coordinate. <br>`longitude`: The longitude of the geographic coordinate. |
| `Constructor(Double,Double,Double)` | Initializes a new instance of the `GeoCoordinate` class with the provided latitude, longitude, and altitude. <br>`latitude`: The latitude of the geographic coordinate. <br>`longitude`: The longitude of the geographic coordinate. <br>`altitude`: The altiltude of the geographic coordinate. |
| `Constructor(Double,Double,Double,Double,Double,Double,Double)` | Initializes a new instance of the `GeoCoordinate` class with the provided latitude, longitude, altitude, accuracy, speed, and course. <br>`latitude`: The latitude of the geographic coordinate. <br>`longitude`: The longitude of the geographic coordinate. <br>`altitude`: The altitude of the geographic coordinate. <br>`horizontalAccuracy`: The horizontal accuracy of the geographic coordinate. <br>`verticalAccuracy`: The vertical accuracy of the geographic coordinate. <br>`speed`: The speed of the geographic coordinate. <br>`course`: The course of the geographic coordinate. |
| `Equals(Device.Location.GeoCoordinate)` | Determines whether the specified `GeoCoordinate` has the same latitude and longitude values as this one. <br>`other`: The `GeoCoordinate` object to compare with the current instance. <br>Returns: Returns `Boolean`. true if the latitude and longitude properties of both objects have the same value. |
| `Equals(Object)` | Determines whether the specified object is a `GeoCoordinate` that has the same latitude and longitude values as this one. <br>`obj`: The object to compare with the current instance. <br>Returns: Returns `Boolean`. true if the latitude and longitude properties of both objects have the same value. |
| `GetDistanceTo(Device.Location.GeoCoordinate)` | Returns the calculated distance, in meters, from the current `GeoCoordinate` to the provided `GeoCoordinate`. <br>`other`: The `GeoCoordinate` used to calculate the distance from the current `GeoCoordinate`. <br>Returns: Returns `Double`. The distance between the two geographic coordinates. |
| `GetHashCode` | Returns a hash code for this instance. <br>Returns: Returns `Int32`. A hash code for the current `GeoCoordinate`. |
| `ToString` | Returns a string representation of the current `GeoCoordinate`. <br>Returns: Returns `String`.. |
| `op_Equality(Device.Location.GeoCoordinate,Device.Location.GeoCoordinate)` | Determines if two `GeoCoordinate` objects are equal. <br>`left`: The first `GeoCoordinate` to test for equality. <br>`right`: The second `GeoCoordinate` to test for equality. <br>Returns: Returns `Boolean`. true if the latitude and longitude values of both `GeoCoordinate` objects are equal. |
| `op_Inequality(Device.Location.GeoCoordinate,Device.Location.GeoCoordinate)` | Determines if two `GeoCoordinate` objects are not equal. <br>`left`: The first `GeoCoordinate` to test for inequality. <br>`right`: The second `GeoCoordinate` to test for inequality. <br>Returns: Returns `Boolean`. true if the latitude and longitude values of both `GeoCoordinate` objects are not equal. |

## System.Device.Location.GeoCoordinateWatcher

A provider class exposing the Windows Phone location service.

### Properties / fields

| Member | Description |
|---|---|
| `DesiredAccuracy` | The desired accuracy for data returned from the location service. <br>Returns: Type: `GeoPositionAccuracy`. The desired accuracy for location data. |
| `MovementThreshold` | The minimum distance that must be travelled between successive `PositionChanged` events. <br>Returns: Type: `Double`. The distance, in meters, that must be travelled between successive `PositionChanged` events. The default value for this property is 0 meters. |
| `Permission` | The application’s level of access to the location service. <br>Returns: Type: `GeoPositionPermission`. The application’s access to the location service. |
| `Position` | The most recent position obtained from the location service. <br>Returns: Type: `GeoPosition`1`. The most recent position obtained from the location service. |
| `Status` | The status of the location service. <br>Returns: Type: `GeoPositionStatus` The status of the location service.. |

### Events

| Member | Description |
|---|---|
| `PositionChanged` | Occurs when the location service detects a change in position. |
| `StatusChanged` | Occurs when the status of the location service changes. |
| `System#ComponentModel#INotifyPropertyChanged#PropertyChanged` | Occurs when a `GeoCoordinateWatcher` property changes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Instantiates a new instance of the `GeoCoordinateWatcher` class with the `DesiredAccuracy` value of `Low`. |
| `Constructor(Device.Location.GeoPositionAccuracy)` | Instantiates a new instance of the `GeoCoordinateWatcher` class with the provided `DesiredAccuracy` value. <br>`desiredAccuracy`: A member of the `GeoPositionAccuracy` enumeration specifying the `DesiredAccuracy` value for the `GeoCoordinateWatcher`. |
| `Dispose` | Releases managed and unmanaged resources used by the `GeoCoordinateWatcher` and stops the acquisition of data from the location service. |
| `Dispose(Boolean)` | Releases unmanaged and optionally releases the managed resources used by the `GeoCoordinateWatcher` and stops the acquisition of data from the location service. <br>`disposing`: true to release both managed and unmanaged resources; false to release only managed resources. |
| `Finalize` | Releases unmanaged resources and performs other cleanup operations before the `GeoCoordinateWatcher` is reclaimed by garbage collection. |
| `OnPositionChanged(Device.Location.GeoPositionChangedEventArgs{Device.Location.GeoCoordinate})` | Raises the `PositionChanged` event. <br>`e`: A `GeoPositionChangedEventArgs`1` containing the event data. |
| `OnPositionStatusChanged(Device.Location.GeoPositionStatusChangedEventArgs)` | Raises the `PositionChanged` event. <br>`e`: A `GeoPositionStatusChangedEventArgs` object containing the event data. |
| `OnPropertyChanged(String)` | Raises the PropertyChanged event for the `GeoCoordinateWatcher` class. <br>`propertyName`: The name of the property that has changed. |
| `Start` | Starts the acquisition of data from the location service. |
| `Start(Boolean)` | Starts the acquisition of data from the location service. <br>`suppressPermissionPrompt`: This parameter is not used. |
| `Stop` | Stops the acquisition of data from the location service. |
| `TryStart(Boolean,TimeSpan)` | Attempts to start the acquisition of data from the location service. If the provided timeout interval is exceeded before the location service responds, the request for location is stopped and the method returns false. <br>`suppressPermissionPrompt`: This parameter is not used. <br>`timeout`: A TimeSpan object specifying the amount of time to wait for location data acquisition to begin. <br>Returns: Returns `Boolean`. true if the location service responds within the timeout window. Otherwise, false. |

## System.Device.Location.GeoPositionAccuracy

Defines the levels of accuracy that can be configured for the location service by using the `GeoCoordinateWatcher` constructor.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Default` | Default accuracy. |
| `High` | High accuracy. |

## System.Device.Location.GeoPositionChangedEventArgs`1

Provides data for `PositionChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `Position` | A `GeoPosition`1` object containing the location and time stamp data for the `PositionChanged` event. <br>Returns: Type: `GeoPosition`1`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Device.Location.GeoPosition{`0})` | Initializes a new instance of the `GeoPositionChangedEventArgs`1` class. <br>`position`: A `GeoPosition`1` object containing the location and time stamp data for the event. |

## System.Device.Location.GeoPositionPermission

Defines the level of access an application has to the location service. Used by the `Permission` property.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Denied` | Access to the location service is denied. |
| `Granted` | Access to the location service is granted. |
| `Unknown` | Access to the location service is unknown. |

## System.Device.Location.GeoPositionStatus

Defines the status values for the location service. Used by the `Status` property.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Disabled` | The location service is disabled or unsupported. |
| `Initializing` | The location service is attempting to acquire data. |
| `NoData` | The location sensors are accessible but the service is unable to resolve location data. |
| `Ready` | The location service is enabled and ready. |

## System.Device.Location.GeoPositionStatusChangedEventArgs

Provides data for `StatusChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `Status` | The GeoPositionStatus value reflecting the updated status of the location service. <br>Returns: Type: `GeoPositionStatus`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Device.Location.GeoPositionStatus)` | Initializes a new instance of the `GeoPositionStatusChangedEventArgs` class. <br>`status`: A member of the GeoPositionStatus enumeration describing the current status of the location service. |

## System.Device.Location.GeoPosition`1

Represents a geographic position consisting of a location and a time stamp.

### Properties / fields

| Member | Description |
|---|---|
| `Location` | Gets or sets the location of the `GeoPosition`1` object. |
| `Timestamp` | Gets or sets the time stamp of the `GeoPosition`1` object. <br>Returns: Type: `DateTimeOffset`. The point in time associated with the `GeoPosition`1` object. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GeoPosition`1` class with no data. |
| `Constructor(DateTimeOffset,`0)` | Initializes a new instance of the `GeoPosition`1` class with the provided time stamp and location. <br>`timestamp`: A DateTimeOffset object specifying the point in time associated with the `GeoPosition`1` object. |

## System.Device.Location.ICivicAddressResolver

Defines the interface for resolving civic addresses. Implemented by `CivicAddressResolver`.

### Events

| Member | Description |
|---|---|
| `ResolveAddressCompleted` | Occurs when the `GeoCoordinate)` request is completed. |

### Methods

| Member | Description |
|---|---|
| `ResolveAddress(Device.Location.GeoCoordinate)` | Attempts to synchronously obtain a civic address from the provided geographic coordinate. <br>`coordinate`: The geographic coordinate from which a civic address is resolved. <br>Returns: Returns `CivicAddress`. The civic address associated with the provided geographic coordinate. |
| `ResolveAddressAsync(Device.Location.GeoCoordinate)` | Attempts to asynchronously obtain a civic address from the provided geographic coordinate. <br>`coordinate`: Attempts to asynchronously obtain a civic address from the provided geographic coordinate. |

## System.Device.Location.IGeoPositionWatcher`1

Defines the interface for resolving geographic positions. Implemented by the `GeoCoordinateWatcher` class.

### Properties / fields

| Member | Description |
|---|---|
| `Position` | The most recent position obtained from the location service. <br>Returns: Type: `GeoPosition`1`. |
| `Status` | The status of the location service. <br>Returns: Type: `GeoPositionStatus`. |

### Events

| Member | Description |
|---|---|
| `PositionChanged` | Occurs when the location service detects a change in position. |
| `StatusChanged` | Occurs when the status of the location service changes. |

### Methods

| Member | Description |
|---|---|
| `Start` | Starts the acquisition of data from the location service. |
| `Start(Boolean)` | Starts the acquisition of data from the location service. <br>`suppressPermissionPrompt`: This parameter is not used. |
| `Stop` | Stops the acquisition of data from the location service. |
| `TryStart(Boolean,TimeSpan)` | Attempts to start the acquisition of data from the location service. <br>`suppressPermissionPrompt`: This parameter is not used. <br>`timeout`: A TimeSpan object specifying the amount of time to wait for location data acquisition to begin. <br>Returns: Returns `Boolean`. |

## System.Device.Location.ResolveAddressCompletedEventArgs

Provides data for `ResolveAddressCompleted` events.

### Properties / fields

| Member | Description |
|---|---|
| `Address` | The resolved civic address. <br>Returns: Type: `CivicAddress`. The resolved civic address. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Device.Location.CivicAddress,Exception,Boolean,Object)` | Initializes a new instance of the `ResolveAddressCompletedEventArgs` class. <br>`address`: A `CivicAddress` object containing the resolved address. <br>`error`: This parameter is not used. <br>`cancelled`: This parameter is not used. <br>`userState`: This parameter is not used. |
