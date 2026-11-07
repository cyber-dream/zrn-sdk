# Microsoft.Devices.Sensors

From the WP8 SDK IntelliSense XML.

## Contents

- [`Microsoft.Devices.Sensors.Accelerometer`](#microsoftdevicessensorsaccelerometer)
- [`Microsoft.Devices.Sensors.AccelerometerFailedException`](#microsoftdevicessensorsaccelerometerfailedexception)
- [`Microsoft.Devices.Sensors.AccelerometerReading`](#microsoftdevicessensorsaccelerometerreading)
- [`Microsoft.Devices.Sensors.AccelerometerReadingEventArgs`](#microsoftdevicessensorsaccelerometerreadingeventargs)
- [`Microsoft.Devices.Sensors.AttitudeReading`](#microsoftdevicessensorsattitudereading)
- [`Microsoft.Devices.Sensors.CalibrationEventArgs`](#microsoftdevicessensorscalibrationeventargs)
- [`Microsoft.Devices.Sensors.Compass`](#microsoftdevicessensorscompass)
- [`Microsoft.Devices.Sensors.CompassReading`](#microsoftdevicessensorscompassreading)
- [`Microsoft.Devices.Sensors.Gyroscope`](#microsoftdevicessensorsgyroscope)
- [`Microsoft.Devices.Sensors.GyroscopeReading`](#microsoftdevicessensorsgyroscopereading)
- [`Microsoft.Devices.Sensors.ISensorReading`](#microsoftdevicessensorsisensorreading)
- [`Microsoft.Devices.Sensors.Motion`](#microsoftdevicessensorsmotion)
- [`Microsoft.Devices.Sensors.MotionReading`](#microsoftdevicessensorsmotionreading)
- [`Microsoft.Devices.Sensors.SensorBase`1`](#microsoftdevicessensorssensorbase1)
- [`Microsoft.Devices.Sensors.SensorFailedException`](#microsoftdevicessensorssensorfailedexception)
- [`Microsoft.Devices.Sensors.SensorReadingEventArgs`1`](#microsoftdevicessensorssensorreadingeventargs1)
- [`Microsoft.Devices.Sensors.SensorState`](#microsoftdevicessensorssensorstate)

## Microsoft.Devices.Sensors.Accelerometer

Provides Windows Phone applications access to the device’s accelerometer sensor.

### Properties / fields

| Member | Description |
|---|---|
| `IsSupported` | Gets or sets whether the device on which the application is running supports the accelerometer sensor. <br>Returns: Type: `Boolean`. true if the device on which the application is running supports the accelerometer sensor; otherwise, false. |
| `State` | Gets the current state of the accelerometer. The value is a member of the `SensorState` enumeration. <br>Returns: Type: `SensorState`. |

### Events

| Member | Description |
|---|---|
| `ReadingChanged` | Occurs when new data arrives from the accelerometer. This method is deprecated in the current release. Applications should use the `CurrentValueChanged` event of the `SensorBase`1` class instead. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `Accelerometer` object. |
| `Dispose` | Releases the managed and unmanaged resources used by the `Accelerometer`. |
| `Start` | Starts data acquisition from the accelerometer. |
| `Stop` | Stops data acquisition from the accelerometer. |

## Microsoft.Devices.Sensors.AccelerometerFailedException

The exception that may be thrown during a call to `Start` or `Stop`. The Message field describes the reason for the exception and the `ErrorId` field contains the error code from the underlying native code implementation of the accelerometer framework.

### Properties / fields

| Member | Description |
|---|---|
| `ErrorId` | The error code from the underlying native code implementation of the accelerometer framework. <br>Returns: Tpe `Int32`. |

## Microsoft.Devices.Sensors.AccelerometerReading

Contains the acceleration and timestamp for an accelerometer reading, indicating the magnitude and direction of forces applied to the device.

### Properties / fields

| Member | Description |
|---|---|
| `Acceleration` | Gets the acceleration vector of the device, in gravitational units. <br>Returns: Type: `Vector3`. The acceleration of the device. |
| `Timestamp` | Gets a timestamp indicating the time at which the accelerometer reading was taken. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the accelerometer reading was taken. |

## Microsoft.Devices.Sensors.AccelerometerReadingEventArgs

Provides data for `ReadingChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `Timestamp` | A timestamp indicating the time at which the accelerometer reading was taken. <br>Returns: Type: `DateTimeOffset`. |
| `X` | The acceleration in the X direction, in gravitational units. <br>Returns: Type: `Double`. |
| `Y` | The acceleration in the Y direction, in gravitational units. <br>Returns: Type: `Double`. |
| `Z` | The acceleration in the Z direction, in gravitational units. <br>Returns: Type: `Double`. |

## Microsoft.Devices.Sensors.AttitudeReading

Contains information about the orientation of the device in space.

### Properties / fields

| Member | Description |
|---|---|
| `Pitch` | Gets the pitch of the attitude reading in radians. <br>Returns: Type: `Double`. The pitch of the attitude reading. |
| `Quaternion` | Gets the quaternion representation of the attitude reading. <br>Returns: Type: `Quaternion`. The quaternion representation of the attitude reading. |
| `Roll` | Gets the roll of the attitude reading in radians. <br>Returns: Type: `Double`. The roll of the attitude reading. |
| `RotationMatrix` | Gets the matrix representation of the attitude reading. <br>Returns: Type: `Matrix`. The matrix representation of the attitude reading. |
| `Timestamp` | Gets a timestamp indicating the time at which the accelerometer reading was taken. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the attitude reading was taken. |
| `Yaw` | Gets the yaw of the attitude reading in radians. <br>Returns: Type: `Double`. The yaw of the attitude reading. |

## Microsoft.Devices.Sensors.CalibrationEventArgs

Provides data for `Calibrate` and events.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CalibrationEventArgs` class. |

## Microsoft.Devices.Sensors.Compass

Provides Windows Phone applications access to the device’s compass sensor.

### Properties / fields

| Member | Description |
|---|---|
| `IsSupported` | Gets or sets whether the device on which the application is running supports the compass sensor. <br>Returns: Type: `Boolean`. true if the device on which the application is running supports the compass sensor; otherwise, false. |

### Events

| Member | Description |
|---|---|
| `Calibrate` | Occurs when the operating system detects that the compass needs calibration. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Compass` class. |

## Microsoft.Devices.Sensors.CompassReading

Contains information about the magnetic field detected by the device’s compass sensor, which can be used to determine the orientation of the device relative to the Earth’s magnetic field.

### Properties / fields

| Member | Description |
|---|---|
| `HeadingAccuracy` | Gets the accuracy, in degrees, of the compass heading readings. <br>Returns: Type: `Double`. The accuracy of the compass heading readings. |
| `MagneticHeading` | Gets the heading, in degrees, measured clockwise from the Earth’s magnetic north. <br>Returns: Type: `Double`. The heading relative to the Earth’s magnetic north. |
| `MagnetometerReading` | Gets the raw magnetometer reading in microteslas. <br>Returns: Type: `Vector3`. The raw magnetometer reading. |
| `Timestamp` | Gets a timestamp indicating the time at which the accelerometer reading was taken. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the compass reading was taken. |
| `TrueHeading` | Gets the heading, in degrees, measured clockwise from the Earth’s geographic north. <br>Returns: Type: `Double`. The heading relative to the Earth’s geographic north. |

## Microsoft.Devices.Sensors.Gyroscope

Provides Windows Phone applications access to the device’s gyroscope sensor.

### Properties / fields

| Member | Description |
|---|---|
| `IsSupported` | Gets or sets whether the device on which the application is running supports the gyroscope sensor. <br>Returns: Type: `Boolean`. true if the device on which the application is running supports the gyroscope sensor; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the Gyroscope class. |

## Microsoft.Devices.Sensors.GyroscopeReading

Contains information about the rotation acceleration of the device.

### Properties / fields

| Member | Description |
|---|---|
| `RotationRate` | Gets the rotational velocity around each axis of the device, in radians per second. <br>Returns: Type: `Vector3`. The rotational velocity around each axis of the device. |
| `Timestamp` | Gets a timestamp indicating the time at which the accelerometer reading was taken. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the gyroscope reading was taken. |

## Microsoft.Devices.Sensors.ISensorReading

The interface that is implemented by all sensor reading classes.

### Properties / fields

| Member | Description |
|---|---|
| `Timestamp` | Gets a timestamp indicating the time at which the sensor reading was taken. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the sensor reading was taken. |

## Microsoft.Devices.Sensors.Motion

Provides Windows Phone applications information about the device’s orientation and motion.

### Properties / fields

| Member | Description |
|---|---|
| `IsSupported` | Gets or sets whether the device on which the application is running supports the sensors required by the `Motion` class. <br>Returns: Type: `Boolean`. true if the device on which the application is running supports the sensors required by the `Motion` class; otherwise, false. |

### Events

| Member | Description |
|---|---|
| `Calibrate` | Occurs when the operating detects that the compass needs calibration. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Motion` class. |

## Microsoft.Devices.Sensors.MotionReading

Contains information about the orientation and movement of the device.

### Properties / fields

| Member | Description |
|---|---|
| `Attitude` | Gets the attitude (yaw, pitch, and roll) of the device, in radians. <br>Returns: The attitude of the device. |
| `DeviceAcceleration` | Gets the linear acceleration of the device, in gravitational units. <br>Returns: The linear acceleration of the device. |
| `DeviceRotationRate` | Gets the rotational velocity of the device, in radians per second. <br>Returns: The rotational velocity of the device. |
| `Gravity` | Gets the gravity vector associated with the `MotionReading`. <br>Returns: The gravity vector associated with the MotionReading. |
| `Timestamp` | Gets a timestamp indicating the time at which the `MotionReading` was calculated. This can be used to correlate readings across sensors and provide additional input to algorithms that process raw sensor data. <br>Returns: Type: `DateTimeOffset`. The time at which the MotionReading was calculated. |

## Microsoft.Devices.Sensors.SensorBase`1

The base class for all Windows Phone sensor classes.

### Properties / fields

| Member | Description |
|---|---|
| `CurrentValue` | Gets an object that implements `ISensorReading` that contains the current value of the sensor. <br>Returns: Type: `{0}`. An object that implements `ISensorReading` that contains the current value of the sensor. |
| `IsDataValid` | Gets the validity of the sensor’s data. <br>Returns: Type: `Boolean`. true if the sensor’s data is valid; otherwise, false. |
| `TimeBetweenUpdates` | Gets or sets the preferred time between `CurrentValueChanged` events. <br>Returns: Type: `TimeSpan`. The preferred time between CurrentValueChanged events. |

### Events

| Member | Description |
|---|---|
| `CurrentValueChanged` | Occurs when new data arrives from the sensor. |

### Methods

| Member | Description |
|---|---|
| `Dispose` | Releases the managed and unmanaged resources used by the sensor. |
| `Finalize` | Allows an object to try to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection. |
| `Start` | Starts acquisition of data from the sensor. |
| `Stop` | Stops acquisition of data from the sensor. |

## Microsoft.Devices.Sensors.SensorFailedException

Represents the exception that is thrown when a sensor operation fails.

### Properties / fields

| Member | Description |
|---|---|
| `ErrorId` | The error ID indicating the cause of the exception. <br>Returns: Type: `Int32`. |

## Microsoft.Devices.Sensors.SensorReadingEventArgs`1

Provides data for `CurrentValueChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `SensorReading` | Gets the sensor reading data associated with the SensorReadingEventArgs. <br>Returns: The sensor reading data associated with the SensorReadingEventArgs. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SensorReadingEventArgs`1` class. |

## Microsoft.Devices.Sensors.SensorState

An enumeration of values for expressing the current state of the accelerometer. The state of a sensor can be obtained using its `State` property.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Disabled` | The sensor is disabled. |
| `Initializing` | The sensor is available and initializing. |
| `NoData` | The sensor is unable to obtain data. |
| `NoPermissions` | The caller does not have permission to access the sensor’s data. |
| `NotSupported` | The sensor hardware is unavailable. |
| `Ready` | The sensor is available and resolving data. |
