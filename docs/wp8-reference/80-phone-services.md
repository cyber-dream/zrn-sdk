# Microsoft.Phone (non-widget services: tasks, info, net, notification, media…)

From the WP8 SDK IntelliSense XML.

## Contents

- [`Microsoft.Devices.Camera`](#microsoftdevicescamera)
- [`Microsoft.Devices.CameraButtons`](#microsoftdevicescamerabuttons)
- [`Microsoft.Devices.CameraOperationCompletedEventArgs`](#microsoftdevicescameraoperationcompletedeventargs)
- [`Microsoft.Devices.CameraType`](#microsoftdevicescameratype)
- [`Microsoft.Devices.CameraVideoBrushExtensions`](#microsoftdevicescameravideobrushextensions)
- [`Microsoft.Devices.ContentReadyEventArgs`](#microsoftdevicescontentreadyeventargs)
- [`Microsoft.Devices.DeviceType`](#microsoftdevicesdevicetype)
- [`Microsoft.Devices.Environment`](#microsoftdevicesenvironment)
- [`Microsoft.Devices.FlashMode`](#microsoftdevicesflashmode)
- [`Microsoft.Devices.MediaHistory`](#microsoftdevicesmediahistory)
- [`Microsoft.Devices.MediaHistoryItem`](#microsoftdevicesmediahistoryitem)
- [`Microsoft.Devices.PhotoCamera`](#microsoftdevicesphotocamera)
- [`Microsoft.Devices.Radio.FMRadio`](#microsoftdevicesradiofmradio)
- [`Microsoft.Devices.Radio.RadioDisabledException`](#microsoftdevicesradioradiodisabledexception)
- [`Microsoft.Devices.Radio.RadioPowerMode`](#microsoftdevicesradioradiopowermode)
- [`Microsoft.Devices.Radio.RadioRegion`](#microsoftdevicesradioradioregion)
- [`Microsoft.Devices.VibrateController`](#microsoftdevicesvibratecontroller)
- [`Microsoft.Devices.YCbCrPixelLayout`](#microsoftdevicesycbcrpixellayout)
- [`Microsoft.Phone.BackgroundAgent`](#microsoftphonebackgroundagent)
- [`Microsoft.Phone.BackgroundAudio.AudioPlayerAgent`](#microsoftphonebackgroundaudioaudioplayeragent)
- [`Microsoft.Phone.BackgroundAudio.AudioStreamer`](#microsoftphonebackgroundaudioaudiostreamer)
- [`Microsoft.Phone.BackgroundAudio.AudioStreamingAgent`](#microsoftphonebackgroundaudioaudiostreamingagent)
- [`Microsoft.Phone.BackgroundAudio.AudioTrack`](#microsoftphonebackgroundaudioaudiotrack)
- [`Microsoft.Phone.BackgroundAudio.BackgroundAudioPlayer`](#microsoftphonebackgroundaudiobackgroundaudioplayer)
- [`Microsoft.Phone.BackgroundAudio.EnabledPlayerControls`](#microsoftphonebackgroundaudioenabledplayercontrols)
- [`Microsoft.Phone.BackgroundAudio.PlayState`](#microsoftphonebackgroundaudioplaystate)
- [`Microsoft.Phone.BackgroundAudio.UserAction`](#microsoftphonebackgroundaudiouseraction)
- [`Microsoft.Phone.BackgroundTransfer.BackgroundTransferEventArgs`](#microsoftphonebackgroundtransferbackgroundtransfereventargs)
- [`Microsoft.Phone.BackgroundTransfer.BackgroundTransferInternalException`](#microsoftphonebackgroundtransferbackgroundtransferinternalexception)
- [`Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest`](#microsoftphonebackgroundtransferbackgroundtransferrequest)
- [`Microsoft.Phone.BackgroundTransfer.BackgroundTransferService`](#microsoftphonebackgroundtransferbackgroundtransferservice)
- [`Microsoft.Phone.BackgroundTransfer.TransferPreferences`](#microsoftphonebackgroundtransfertransferpreferences)
- [`Microsoft.Phone.BackgroundTransfer.TransferStatus`](#microsoftphonebackgroundtransfertransferstatus)
- [`Microsoft.Phone.Info.DeviceExtendedProperties`](#microsoftphoneinfodeviceextendedproperties)
- [`Microsoft.Phone.Info.DeviceStatus`](#microsoftphoneinfodevicestatus)
- [`Microsoft.Phone.Info.MediaCapabilities`](#microsoftphoneinfomediacapabilities)
- [`Microsoft.Phone.Info.PowerSource`](#microsoftphoneinfopowersource)
- [`Microsoft.Phone.Info.UserExtendedProperties`](#microsoftphoneinfouserextendedproperties)
- [`Microsoft.Phone.Marketplace.LicenseInformation`](#microsoftphonemarketplacelicenseinformation)
- [`Microsoft.Phone.Net.NetworkInformation.ConnectState`](#microsoftphonenetnetworkinformationconnectstate)
- [`Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation`](#microsoftphonenetnetworkinformationdevicenetworkinformation)
- [`Microsoft.Phone.Net.NetworkInformation.NameResolutionCallback`](#microsoftphonenetnetworkinformationnameresolutioncallback)
- [`Microsoft.Phone.Net.NetworkInformation.NameResolutionResult`](#microsoftphonenetnetworkinformationnameresolutionresult)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkCharacteristics`](#microsoftphonenetnetworkinformationnetworkcharacteristics)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkError`](#microsoftphonenetnetworkinformationnetworkerror)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkException`](#microsoftphonenetnetworkinformationnetworkexception)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkInterface`](#microsoftphonenetnetworkinformationnetworkinterface)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceInfo`](#microsoftphonenetnetworkinformationnetworkinterfaceinfo)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceList`](#microsoftphonenetnetworkinformationnetworkinterfacelist)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceSubType`](#microsoftphonenetnetworkinformationnetworkinterfacesubtype)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceType`](#microsoftphonenetnetworkinformationnetworkinterfacetype)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkNotificationEventArgs`](#microsoftphonenetnetworkinformationnetworknotificationeventargs)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkNotificationType`](#microsoftphonenetnetworkinformationnetworknotificationtype)
- [`Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics`](#microsoftphonenetnetworkinformationnetworkselectioncharacteristics)
- [`Microsoft.Phone.Net.NetworkInformation.SocketExtensions`](#microsoftphonenetnetworkinformationsocketextensions)
- [`Microsoft.Phone.Net.NetworkInformation.WebRequestExtensions`](#microsoftphonenetnetworkinformationwebrequestextensions)
- [`Microsoft.Phone.Notification.ChannelConnectionStatus`](#microsoftphonenotificationchannelconnectionstatus)
- [`Microsoft.Phone.Notification.ChannelErrorType`](#microsoftphonenotificationchannelerrortype)
- [`Microsoft.Phone.Notification.ChannelPowerLevel`](#microsoftphonenotificationchannelpowerlevel)
- [`Microsoft.Phone.Notification.HttpNotification`](#microsoftphonenotificationhttpnotification)
- [`Microsoft.Phone.Notification.HttpNotificationChannel`](#microsoftphonenotificationhttpnotificationchannel)
- [`Microsoft.Phone.Notification.HttpNotificationEventArgs`](#microsoftphonenotificationhttpnotificationeventargs)
- [`Microsoft.Phone.Notification.NotificationChannelConnectionEventArgs`](#microsoftphonenotificationnotificationchannelconnectioneventargs)
- [`Microsoft.Phone.Notification.NotificationChannelErrorEventArgs`](#microsoftphonenotificationnotificationchannelerroreventargs)
- [`Microsoft.Phone.Notification.NotificationChannelUriEventArgs`](#microsoftphonenotificationnotificationchannelurieventargs)
- [`Microsoft.Phone.Notification.NotificationEventArgs`](#microsoftphonenotificationnotificationeventargs)
- [`Microsoft.Phone.PictureDecoder`](#microsoftphonepicturedecoder)
- [`Microsoft.Phone.Scheduler.AgentExitReason`](#microsoftphonescheduleragentexitreason)
- [`Microsoft.Phone.Scheduler.Alarm`](#microsoftphonescheduleralarm)
- [`Microsoft.Phone.Scheduler.PeriodicTask`](#microsoftphoneschedulerperiodictask)
- [`Microsoft.Phone.Scheduler.RecurrenceInterval`](#microsoftphoneschedulerrecurrenceinterval)
- [`Microsoft.Phone.Scheduler.Reminder`](#microsoftphoneschedulerreminder)
- [`Microsoft.Phone.Scheduler.ResourceIntensiveTask`](#microsoftphoneschedulerresourceintensivetask)
- [`Microsoft.Phone.Scheduler.ScheduledAction`](#microsoftphoneschedulerscheduledaction)
- [`Microsoft.Phone.Scheduler.ScheduledActionService`](#microsoftphoneschedulerscheduledactionservice)
- [`Microsoft.Phone.Scheduler.ScheduledNotification`](#microsoftphoneschedulerschedulednotification)
- [`Microsoft.Phone.Scheduler.ScheduledTask`](#microsoftphoneschedulerscheduledtask)
- [`Microsoft.Phone.Scheduler.ScheduledTaskAgent`](#microsoftphoneschedulerscheduledtaskagent)
- [`Microsoft.Phone.Scheduler.SchedulerServiceException`](#microsoftphoneschedulerschedulerserviceexception)
- [`Microsoft.Phone.Tasks.AddressChooserTask`](#microsoftphonetasksaddresschoosertask)
- [`Microsoft.Phone.Tasks.AddressResult`](#microsoftphonetasksaddressresult)
- [`Microsoft.Phone.Tasks.BingMapsDirectionsTask`](#microsoftphonetasksbingmapsdirectionstask)
- [`Microsoft.Phone.Tasks.BingMapsTask`](#microsoftphonetasksbingmapstask)
- [`Microsoft.Phone.Tasks.CameraCaptureTask`](#microsoftphonetaskscameracapturetask)
- [`Microsoft.Phone.Tasks.ChooserBase`1`](#microsoftphonetaskschooserbase1)
- [`Microsoft.Phone.Tasks.EmailAddressChooserTask`](#microsoftphonetasksemailaddresschoosertask)
- [`Microsoft.Phone.Tasks.EmailComposeTask`](#microsoftphonetasksemailcomposetask)
- [`Microsoft.Phone.Tasks.EmailResult`](#microsoftphonetasksemailresult)
- [`Microsoft.Phone.Tasks.GameInviteTask`](#microsoftphonetasksgameinvitetask)
- [`Microsoft.Phone.Tasks.LabeledMapLocation`](#microsoftphonetaskslabeledmaplocation)
- [`Microsoft.Phone.Tasks.MarketplaceContentType`](#microsoftphonetasksmarketplacecontenttype)
- [`Microsoft.Phone.Tasks.MarketplaceDetailTask`](#microsoftphonetasksmarketplacedetailtask)
- [`Microsoft.Phone.Tasks.MarketplaceHubTask`](#microsoftphonetasksmarketplacehubtask)
- [`Microsoft.Phone.Tasks.MarketplaceReviewTask`](#microsoftphonetasksmarketplacereviewtask)
- [`Microsoft.Phone.Tasks.MarketplaceSearchTask`](#microsoftphonetasksmarketplacesearchtask)
- [`Microsoft.Phone.Tasks.MediaLocationType`](#microsoftphonetasksmedialocationtype)
- [`Microsoft.Phone.Tasks.MediaPlaybackControls`](#microsoftphonetasksmediaplaybackcontrols)
- [`Microsoft.Phone.Tasks.MediaPlayerLauncher`](#microsoftphonetasksmediaplayerlauncher)
- [`Microsoft.Phone.Tasks.MediaPlayerOrientation`](#microsoftphonetasksmediaplayerorientation)
- [`Microsoft.Phone.Tasks.PhoneCallTask`](#microsoftphonetasksphonecalltask)
- [`Microsoft.Phone.Tasks.PhoneNumberChooserTask`](#microsoftphonetasksphonenumberchoosertask)
- [`Microsoft.Phone.Tasks.PhoneNumberResult`](#microsoftphonetasksphonenumberresult)
- [`Microsoft.Phone.Tasks.PhotoChooserTask`](#microsoftphonetasksphotochoosertask)
- [`Microsoft.Phone.Tasks.PhotoResult`](#microsoftphonetasksphotoresult)
- [`Microsoft.Phone.Tasks.SaveContactResult`](#microsoftphonetaskssavecontactresult)
- [`Microsoft.Phone.Tasks.SaveContactTask`](#microsoftphonetaskssavecontacttask)
- [`Microsoft.Phone.Tasks.SaveEmailAddressTask`](#microsoftphonetaskssaveemailaddresstask)
- [`Microsoft.Phone.Tasks.SavePhoneNumberTask`](#microsoftphonetaskssavephonenumbertask)
- [`Microsoft.Phone.Tasks.SaveRingtoneTask`](#microsoftphonetaskssaveringtonetask)
- [`Microsoft.Phone.Tasks.SearchTask`](#microsoftphonetaskssearchtask)
- [`Microsoft.Phone.Tasks.ShareLinkTask`](#microsoftphonetaskssharelinktask)
- [`Microsoft.Phone.Tasks.ShareStatusTask`](#microsoftphonetaskssharestatustask)
- [`Microsoft.Phone.Tasks.ShareTaskBase`](#microsoftphonetaskssharetaskbase)
- [`Microsoft.Phone.Tasks.SmsComposeTask`](#microsoftphonetaskssmscomposetask)
- [`Microsoft.Phone.Tasks.TaskEventArgs`](#microsoftphonetaszrnaskeventargs)
- [`Microsoft.Phone.Tasks.TaskResult`](#microsoftphonetaszrnaskresult)
- [`Microsoft.Phone.Tasks.WebBrowserTask`](#microsoftphonetaskswebbrowsertask)
- [`Microsoft.Phone.UserData.Account`](#microsoftphoneuserdataaccount)
- [`Microsoft.Phone.UserData.AddressKind`](#microsoftphoneuserdataaddresskind)
- [`Microsoft.Phone.UserData.Appointment`](#microsoftphoneuserdataappointment)
- [`Microsoft.Phone.UserData.AppointmentStatus`](#microsoftphoneuserdataappointmentstatus)
- [`Microsoft.Phone.UserData.Appointments`](#microsoftphoneuserdataappointments)
- [`Microsoft.Phone.UserData.AppointmentsSearchEventArgs`](#microsoftphoneuserdataappointmentssearcheventargs)
- [`Microsoft.Phone.UserData.Attendee`](#microsoftphoneuserdataattendee)
- [`Microsoft.Phone.UserData.CompleteName`](#microsoftphoneuserdatacompletename)
- [`Microsoft.Phone.UserData.Contact`](#microsoftphoneuserdatacontact)
- [`Microsoft.Phone.UserData.ContactAddress`](#microsoftphoneuserdatacontactaddress)
- [`Microsoft.Phone.UserData.ContactCompanyInformation`](#microsoftphoneuserdatacontactcompanyinformation)
- [`Microsoft.Phone.UserData.ContactEmailAddress`](#microsoftphoneuserdatacontactemailaddress)
- [`Microsoft.Phone.UserData.ContactPhoneNumber`](#microsoftphoneuserdatacontactphonenumber)
- [`Microsoft.Phone.UserData.Contacts`](#microsoftphoneuserdatacontacts)
- [`Microsoft.Phone.UserData.ContactsSearchEventArgs`](#microsoftphoneuserdatacontactssearcheventargs)
- [`Microsoft.Phone.UserData.EmailAddressKind`](#microsoftphoneuserdataemailaddresskind)
- [`Microsoft.Phone.UserData.FilterKind`](#microsoftphoneuserdatafilterkind)
- [`Microsoft.Phone.UserData.PhoneDataSharingContext`](#microsoftphoneuserdataphonedatasharingcontext)
- [`Microsoft.Phone.UserData.PhoneNumberKind`](#microsoftphoneuserdataphonenumberkind)
- [`Microsoft.Phone.UserData.StorageKind`](#microsoftphoneuserdatastoragekind)
- [`System.Data.Common.DbException`](#systemdatacommondbexception)
- [`System.Data.DataException`](#systemdatadataexception)
- [`System.Diagnostics.Stopwatch`](#systemdiagnosticsstopwatch)
- [`System.Windows.Automation.Peers.FrameAutomationPeer`](#systemwindowsautomationpeersframeautomationpeer)
- [`System.Windows.Controls.Frame`](#systemwindowscontrolsframe)
- [`System.Windows.Controls.Page`](#systemwindowscontrolspage)
- [`System.Windows.Media.Imaging.Extensions`](#systemwindowsmediaimagingextensions)
- [`System.Windows.Navigation.FragmentNavigationEventArgs`](#systemwindowsnavigationfragmentnavigationeventargs)
- [`System.Windows.Navigation.FragmentNavigationEventHandler`](#systemwindowsnavigationfragmentnavigationeventhandler)
- [`System.Windows.Navigation.JournalEntry`](#systemwindowsnavigationjournalentry)
- [`System.Windows.Navigation.JournalEntryRemovedEventArgs`](#systemwindowsnavigationjournalentryremovedeventargs)
- [`System.Windows.Navigation.JournalOwnership`](#systemwindowsnavigationjournalownership)
- [`System.Windows.Navigation.NavigationCacheMode`](#systemwindowsnavigationnavigationcachemode)
- [`System.Windows.Navigation.NavigationContext`](#systemwindowsnavigationnavigationcontext)
- [`System.Windows.Navigation.NavigationFailedEventArgs`](#systemwindowsnavigationnavigationfailedeventargs)
- [`System.Windows.Navigation.NavigationFailedEventHandler`](#systemwindowsnavigationnavigationfailedeventhandler)
- [`System.Windows.Navigation.NavigationService`](#systemwindowsnavigationnavigationservice)
- [`System.Windows.Navigation.NavigationStoppedEventHandler`](#systemwindowsnavigationnavigationstoppedeventhandler)
- [`System.Windows.Navigation.UriMapper`](#systemwindowsnavigationurimapper)
- [`System.Windows.Navigation.UriMapperBase`](#systemwindowsnavigationurimapperbase)
- [`System.Windows.Navigation.UriMapping`](#systemwindowsnavigationurimapping)

## Microsoft.Devices.Camera

The base class for camera application support.

### Properties / fields

| Member | Description |
|---|---|
| `AvailableResolutions` | Queries the camera for the available resolutions. <br>Returns: Returns `IEnumerable`1`. Collection of available resolutions on the camera. |
| `CameraType` | Gets the type of camera that the `Camera` object represents; indicates the location of the camera on the device. <br>Returns: The type of camera that the `Camera` object represents; indicates the location of the camera on the device. |
| `InternalLock` | This member is for internal use only. <br>Returns: Returns `Object`. |
| `Orientation` | Gets the number of degrees that the viewfinder brush needs to be rotated clockwise to align with the camera sensor. <br>Returns: The number of degrees that the viewfinder brush needs to be rotated clockwise to align with the camera sensor. |
| `PreviewResolution` | Gets the current resolution of the image provided by the `PhotoCamera`GetPreviewBuffer methods. This method is for use by the live viewfinder. <br>Returns: The preview resolution. |
| `Resolution` | Gets or sets the resolution of the image captured by the camera. <br>Returns: The resolution of the image captured by the camera. |
| `SequenceNumber` | This member is for internal use only. <br>Returns: Returns `Int32`. |

### Events

| Member | Description |
|---|---|
| `Initialized` | Occurs when the camera object has been initialized. |

### Methods

| Member | Description |
|---|---|
| `Dispose` | Disposes of the image source stream and frees resources used by the Camera object. |
| `Finalize` | Releases unmanaged resources and performs other cleanup operations before the Camera object is reclaimed by garbage collection. |
| `InvokeAndRemapExceptions(Action)` | This member is for internal use only. <br>`a`: Internal use only. |
| `IsCameraTypeSupported(Microsoft.Devices.CameraType)` | Determines whether the specified type of camera is supported on the device. <br>`type`: The type of camera represented by the `Camera` object; indicates the location of the camera on the device. <br>Returns: true if the specified type of camera is supported on the device; otherwise, false. |
| `SafeFireEvent(EventHandler)` | This member is for internal use only. <br>`callback`: Internal use only. <br>Returns: Returns `Boolean`. |
| `SafeFireEvent``1(EventHandler{``0},``0)` | This member is for internal use only. <br>`callback`: Internal use only. <br>`args`: Internal use only. <br>Returns: Returns `Boolean`. |

## Microsoft.Devices.CameraButtons

Provides events that are triggered by the device shutter button.

### Events

| Member | Description |
|---|---|
| `ShutterKeyHalfPressed` | Occurs when the hardware shutter button is pressed and held. |
| `ShutterKeyPressed` | Occurs when the hardware shutter button receives a full press. |
| `ShutterKeyReleased` | Occurs when the hardware shutter button is released, after the `ShutterKeyPressed` event occurs. |

## Microsoft.Devices.CameraOperationCompletedEventArgs

Provides data for camera events such as `Initialized`, `AutoFocusCompleted`, and `CaptureCompleted`.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Returns the error for the failed operation. <br>Returns: Returns `Exception`. The error for the failed operation. |
| `Succeeded` | Returns whether the operation succeeded or failed. <br>Returns: Returns `Boolean`. true if the operation succeeded; otherwise, false. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Boolean,Exception)` | Initializes a new instance of the CameraOperationCompletedEventArgs class. <br>`succeeded`: To determine whether the operation succeeded or failed. <br>`exception`: The error for the failed operation. |

## Microsoft.Devices.CameraType

Specifies the general location of the camera on the device.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FrontFacing` | The camera is located on the front of the device. |
| `Primary` | The camera is located on the back of the device. |

## Microsoft.Devices.CameraVideoBrushExtensions

Uses the VideoBrush to render a composited video source onto an object. Typical usage is to provide the background for elements such as a rectangle or container.

### Methods

| Member | Description |
|---|---|
| `SetSource(Media.VideoBrush,Microsoft.Devices.Camera)` | Sets the VideoBrush source to the camera. <br>`brush`: The VideoBrush object. <br>`camera`: Sets the VideoBrush source to the camera |

## Microsoft.Devices.ContentReadyEventArgs

Provides data for `PhotoCamera` events.

### Properties / fields

| Member | Description |
|---|---|
| `ImageStream` | Returns the image stream corresponding to the full-resolution and thumbnail images in the `CaptureImageAvailable` and `CaptureThumbnailAvailable` events, respectively. <br>Returns: The image stream. |

## Microsoft.Devices.DeviceType

Defines the device type values used by the `DeviceType` property.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Device` | The device type is an actual device. |
| `Emulator` | The device type is a device emulator. |

## Microsoft.Devices.Environment

Gives applications access to information about the environment in which they are running. In the current release, the only supported property of this class is `DeviceType`, which is used to determine if an application is running on an actual Windows Phone device or on the device emulator on a PC.

### Properties / fields

| Member | Description |
|---|---|
| `DeviceType` | Gets the type of device on which the application is running. Use this property to determine if your application is running on an actual device or on the device emulator. <br>Returns: Type: `DeviceType`. |

## Microsoft.Devices.FlashMode

Defines the operating mode of the camera flash.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Auto` | The camera flash is in auto flash mode. |
| `Off` | The camera flash is disabled. |
| `On` | The camera flash is enabled. |
| `RedEyeReduction` | The camera flash is in red-eye reduction mode. |

## Microsoft.Devices.MediaHistory

The `MediaHistory` class contains information about newly-acquired, recently-played or currently-playing media items.

### Properties / fields

| Member | Description |
|---|---|
| `NowPlaying` | Gets the most recent `MediaHistoryItem` in the media history, or sets the supplied `MediaHistoryItem` into the Now Playing section of the Music + Videos Hub. <br>Returns: Returns `MediaHistoryItem`. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Instance` | Returns a singleton that represents the MediaHistory class. |

### Methods

| Member | Description |
|---|---|
| `WriteAcquiredItem(Microsoft.Devices.MediaHistoryItem)` | Adds a `MediaHistoryItem` that an application recently acquired for the user. <br>`item`: The `MediaHistoryItem` that represents the newly-acquired content. |
| `WriteRecentPlay(Microsoft.Devices.MediaHistoryItem)` | Adds a `MediaHistoryItem` that an application has recently played for the user. <br>`item`: The `MediaHistoryItem` that represents the recently-played content. |

## Microsoft.Devices.MediaHistoryItem

Contains information about the media item that will show up in the user media history.

### Properties / fields

| Member | Description |
|---|---|
| `ImageStream` | Gets or sets a stream which contains the image that is displayed with this MediaHistoryItem in the media history area. <br>Returns: Returns `Stream`. |
| `MaxImageSize` | Represents the maximum size of `ImageStream`. <br>Returns: Returns `Int32`. |
| `PlayerContext` | A set of key/value pairs. <br>Returns: Returns `IDictionary`2`. |
| `Source` | This property is unsupported. <br>Returns: Returns `String`. |
| `Title` | Gets or sets the title of the MediaHistoryItem. <br>Returns: Returns `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a default MediaHistoryItem with no properties set. |

## Microsoft.Devices.PhotoCamera

Provides the basic camera functionality for a Windows Phone still camera application. This class includes members for enabling and configuring functionality such as image capture, focus, and flash mode. Also, it contains the primary focus events, image acquired event, and image saved event.

### Properties / fields

| Member | Description |
|---|---|
| `FlashMode` | Gets or sets the flash mode. Unsupported flash modes silently default to `Off`. <br>Returns: Returns `FlashMode`. The flash mode. |
| `IsFocusAtPointSupported` | Specifies whether the camera can programmatically auto focus on a specific point in the viewfinder. <br>Returns: true if the camera can auto focus on a specific point; otherwise, false. |
| `IsFocusSupported` | Specifies whether the camera can be auto-focused programmatically. <br>Returns: true if auto focus can be initiated programmatically; otherwise, false. |
| `YCbCrPixelLayout` | Gets the YCbCr pixel layout of the camera. <br>Returns: The YCbCr pixel layout of the camera. |

### Events

| Member | Description |
|---|---|
| `AutoFocusCompleted` | Provides data for the `AutoFocusCompleted` event. |
| `CaptureCompleted` | Occurs when the capture sequence is complete. |
| `CaptureImageAvailable` | Occurs when an image is available. |
| `CaptureStarted` | Occurs when the capture sequence has started. |
| `CaptureThumbnailAvailable` | Occurs when a thumbnail image is available. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PhotoCamera` class. |
| `Constructor(Microsoft.Devices.CameraType)` | Initializes a new instance of the `PhotoCamera` class for the specified type of camera. <br>`type`: The type of camera that the `PhotoCamera` object represents; indicates the location of the camera on the device. |
| `CancelFocus` | Cancels the current focus operation. |
| `CaptureImage` | Initiates a full-resolution capture of the current image displayed in the viewfinder. |
| `Focus` | Starts a camera auto focus operation. |
| `FocusAtPoint(Double,Double)` | Starts a camera auto focus operation on a specific point in the viewfinder, for those devices that support it. <br>`x`: The horizontal location in the viewfinder; a value between 0 (left) and 1.0 (right). <br>`y`: The vertical location in the viewfinder; a value between 0 (bottom) and 1.0 (top). |
| `GetPreviewBufferArgb32(Int32[])` | Copies the current viewfinder ARGB frame into a buffer for further manipulation. <br>`pixelData`: The ARGB pixel data. |
| `GetPreviewBufferY(Byte[])` | Copies the luminance data for the current viewfinder frame into a buffer for further manipulation. <br>`pixelData`: The YUV pixel data. |
| `GetPreviewBufferYCbCr(Byte[])` | Copies the current viewfinder frame into a buffer for further manipulation. <br>`pixelData`: The pixel data. |
| `IsFlashModeSupported(Microsoft.Devices.FlashMode)` | Determines whether a particular flash mode is supported on the device. <br>`mode`: The flash mode. <br>Returns: true if the specified flash mode is supported; otherwise, false. |

## Microsoft.Devices.Radio.FMRadio

This class acts as a singleton controller for the device radio.

### Properties / fields

| Member | Description |
|---|---|
| `CurrentRegion` | Sets the frequency region information for the `FMRadio` class. <br>Returns: Returns `RadioRegion`. |
| `Frequency` | Tunes the `FMRadio` to a new frequency. <br>Returns: Returns `Double`. |
| `Instance` | Returns a singleton that represents the `FMRadio` class. <br>Returns: Returns `FMRadio`. |
| `PowerMode` | Sets the `RadioPowerMode` of the device. Setting this property turns the device radio on or off. <br>Returns: Returns `RadioPowerMode`. |
| `SignalStrength` | Retrieves the received signal strength indicator (RSSI) value for the currently-tuned frequency. <br>Returns: Returns `Double`. |

## Microsoft.Devices.Radio.RadioDisabledException

This exception is thrown when the radio is not enabled due to policy on the device.

### Methods

| Member | Description |
|---|---|
| `Constructor` | This constructor is unsupported. |

## Microsoft.Devices.Radio.RadioPowerMode

An enumeration that contains the possible power modes of the radio.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Off` | The radio is off and not playing. |
| `On` | The radio is on and playing. |

## Microsoft.Devices.Radio.RadioRegion

An enumeration that describes the set of radio frequency bands available for `FMRadio` tuning.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Europe` | The radio frequency band of regions other than Japan and the United States. |
| `Japan` | The radio frequency band of Japan. |
| `UnitedStates` | The radio frequency band of the United States. |

## Microsoft.Devices.VibrateController

Allows Windows Phone applications to start and stop vibration on the device. Obtain an instance of this class using the Default method.

### Properties / fields

| Member | Description |
|---|---|
| `Default` | The static method used to obtain an instance of the `VibrateController` object. <br>Returns: Returns `VibrateController`. |

### Methods

| Member | Description |
|---|---|
| `Start(TimeSpan)` | Starts vibration on the device. <br>`duration`: A TimeSpan object specifying the amount of time, in seconds, for which the phone vibrates. Valid times are between 0 and 5 seconds. Values greater than 5 or less than 0 will generate an exception. |
| `Stop` | Stops vibration on the device. |

## Microsoft.Devices.YCbCrPixelLayout

Represents the current YCbCr plane layout for `Byte[])`.

### Properties / fields

| Member | Description |
|---|---|
| `CbOffset` | Gets the offset to the first Cb pixel in each row. <br>Returns: The offset to the first Cb pixel in each row. |
| `CbPitch` | Gets the pitch between rows of Cb pixels. <br>Returns: The pitch between rows of Cb pixels. |
| `CbXPitch` | Gets the pitch between Cb pixels in each row. <br>Returns: The pitch between Cb pixels in each row. |
| `CrOffset` | Gets the offset to the first Cr pixel in each row. <br>Returns: The offset to the first Cr pixel in each row. |
| `CrPitch` | Gets the pitch between rows of Cr pixels. <br>Returns: The pitch between rows of Cr pixels. |
| `CrXPitch` | Gets the pitch between Cr pixels in each row. <br>Returns: The pitch between Cr pixels in each row. |
| `RequiredBufferSize` | Gets the required buffer size for the image. <br>Returns: The required buffer size for the image. |
| `YOffset` | Gets the pitch between rows of Y pixels. <br>Returns: The pitch between rows of Y pixels. |
| `YPitch` | Gets the pitch, in bytes, to the next Y line. <br>Returns: The pitch, in bytes, to the next Y line. |
| `YXPitch` | Gets the pitch between Y pixels in each row. <br>Returns: The pitch between Y pixels in each row. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `YCbCrPixelLayout` class. |

## Microsoft.Phone.BackgroundAgent

This is the base class for `ScheduledTaskAgent`, `AudioPlayerAgent`, and `AudioStreamingAgent`. For more information about background audio agents, see `BackgroundAudio`. For information about Scheduled Tasks, see Scheduled Tasks Overview for Windows Phone.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BackgroundAgent` class. |
| `Abort` | Informs the operating system that the agent is unable to perform its intended task and that it should not be launched again until the foreground application mitigates the blocking issue and reenables the agent. |
| `NotifyComplete` | Informs the operating system that the agent has completed its intended task for the current invocation of the agent. |
| `OnCancel` | Called by the operating system to alert a background agent that it is going to be put into a dormant state or terminated. |

## Microsoft.Phone.BackgroundAudio.AudioPlayerAgent

An implementation of a `BackgroundAgent` specifically designed to play audio in the background.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of `AudioPlayerAgent`. |
| `OnError(Microsoft.Phone.BackgroundAudio.BackgroundAudioPlayer,Microsoft.Phone.BackgroundAudio.AudioTrack,Exception,Boolean)` | Called when there is an error with playback, such as an audio track not downloading correctly. <br>`player`: The `BackgroundAudioPlayer`. <br>`track`: The track that had the error. <br>`error`: The error that occurred. <br>`isFatal`: If true, playback cannot continue and playback of the track stops. |
| `OnPlayStateChanged(Microsoft.Phone.BackgroundAudio.BackgroundAudioPlayer,Microsoft.Phone.BackgroundAudio.AudioTrack,Microsoft.Phone.BackgroundAudio.PlayState)` | Called when the play state changes, except for the error state. <br>`player`: The `BackgroundAudioPlayer`. <br>`track`: The track playing at the time that the play state changed. <br>`playState`: The new state of the player. |
| `OnUserAction(Microsoft.Phone.BackgroundAudio.BackgroundAudioPlayer,Microsoft.Phone.BackgroundAudio.AudioTrack,Microsoft.Phone.BackgroundAudio.UserAction,Object)` | Called when the user requests an action using some application-provided UI or the Universal Volume Control (UVC) and the application has requested notification of the action. <br>`player`: The `BackgroundAudioPlayer`. <br>`track`: The track playing at the time of the user action. <br>`action`: The action that the user has requested. <br>`param`: The data associated with the requested action. |

## Microsoft.Phone.BackgroundAudio.AudioStreamer

Represents an audio stream that can be played back by the system audio player.

### Methods

| Member | Description |
|---|---|
| `SetSource(Media.MediaStreamSource)` | Sets a MediaStreamSource derived object that provides buffering and playback functionality for this audio stream. <br>`source`: The MediaStreamSource to use for this audio stream. |

## Microsoft.Phone.BackgroundAudio.AudioStreamingAgent

A background agent that performs per-track streaming for playback.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `AudioStreamingAgent` class. |
| `OnBeginStreaming(Microsoft.Phone.BackgroundAudio.AudioTrack,Microsoft.Phone.BackgroundAudio.AudioStreamer)` | Called when a new track requires audio streaming because it is about to start playing. <br>`track`: The track that needs audio streaming. <br>`streamer`: The `AudioStreamer` object to which a MediaStreamSource should be attached to start playback. |
| `OnCancel` | Tells `AudioStreamingAgent` that it has been cancelled. |

## Microsoft.Phone.BackgroundAudio.AudioTrack

Represents a single track of audio.

### Properties / fields

| Member | Description |
|---|---|
| `Album` | Text to display as the track’s album. <br>Returns: Returns `String`. |
| `AlbumArt` | The path to the album art for the track. <br>Returns: Returns `Uri`. |
| `Artist` | Text to display as the track's artist. <br>Returns: Returns `String`. |
| `Duration` | The length of the track. <br>Returns: Returns `TimeSpan`. |
| `PlayerControls` | Determines which playback controls are enabled in the system user interface. <br>Returns: Returns `EnabledPlayerControls`. |
| `Source` | The URI path to the track. <br>Returns: Returns `Uri`. |
| `Tag` | An arbitrary string associated with this track, used to store application-specific state. <br>Returns: Returns `String`. |
| `Title` | Text to display as the track's title. <br>Returns: Returns `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `AudioTrack` class. |
| `Constructor(Uri,String,String,String,Uri)` | Creates and initializes a new instance of the `AudioTrack` class. <br>`source`: The URI path to the track. This can be a `UriKind` value or null if using streaming. <br>`title`: The title of the track. This must be a non-empty string. <br>`artist`: The artist of the track. This string can be empty or null. <br>`album`: The album of the track. This string can be empty or null. <br>`albumArt`: The URI path to the image used in the user interface—for example, in the Now Playing tile. This can be null. |
| `Constructor(Uri,String,String,String,Uri,String,Microsoft.Phone.BackgroundAudio.EnabledPlayerControls)` | Creates and initializes a new instance of the `AudioTrack` class. <br>`source`: The URI path to the track. This can be a `UriKind` value or null if using streaming. <br>`title`: The title of the track. This must be a non-empty string. <br>`artist`: The artist of the track. This string can be empty or null. <br>`album`: The album of the track. This string can be empty or null. <br>`albumArt`: The URI path to the image used in the user interface—for example, in the Now Playing tile. This can be null. <br>`tag`: Application-specific context for the track. <br>`enabledControls`: Specifies which controls should be enabled in the system user interface. |
| `BeginEdit` | Sets the track object into edit mode. |
| `CancelEdit` | This method is not implemented for `AudioTrack` objects and always throws a `NotSupportedException`. |
| `EndEdit` | Ends an edit operation, saving all property changes to the underlying store. |

## Microsoft.Phone.BackgroundAudio.BackgroundAudioPlayer

Provides background access to audio playback functionality such as play, pause, fast-forward, and rewind.

### Properties / fields

| Member | Description |
|---|---|
| `BufferingProgress` | The amount of buffering that is completed for the media content. <br>Returns: Returns `Double`. |
| `CanPause` | Gets a value indicating whether the media can be paused when the `Pause` method is called. <br>Returns: Returns `Boolean`. |
| `CanSeek` | Gets a value indicating whether the media can be repositioned by setting the value of the `Position` property. <br>Returns: Returns `Boolean`. |
| `Error` | The last error, if any, to have occurred while playing the current `AudioTrack`. <br>Returns: Returns `Exception`. |
| `Instance` | Returns an instance of the `BackgroundAudioPlayer`. If this application already has background audio playback resources allocated, the `BackgroundAudioPlayer` returned will contain references to those resources. <br>Returns: Returns `BackgroundAudioPlayer`. |
| `PlayerState` | Gets the current `PlayState` of the player. <br>Returns: Returns `PlayState`. |
| `Position` | Gets or sets the current position within the current `Track`. <br>Returns: Returns `TimeSpan`. |
| `Track` | Gets or sets the current track for this application, whether it is currently playing or not. <br>Returns: Returns `AudioTrack`. |
| `Volume` | The media's volume represented on a linear scale between 0 and 1. The default is 0.85. <br>Returns: Returns `Double`. |

### Events

| Member | Description |
|---|---|
| `PlayStateChanged` | Occurs when the `PlayState` changes. |

### Methods

| Member | Description |
|---|---|
| `Close` | Closes the player and removes all resources reserved for it, including the current `AudioTrack`. |
| `FastForward` | Starts fast-forwarding through the current `AudioTrack`. |
| `Pause` | Pauses playback at the current position. |
| `Play` | Plays or resumes the current `AudioTrack` at its current position. |
| `Rewind` | Starts rewinding through the current `AudioTrack`. |
| `SkipNext` | Skips to the next track. |
| `SkipPrevious` | Skips to the previous track. |
| `Stop` | Stops and resets media to be played from the beginning. |

## Microsoft.Phone.BackgroundAudio.EnabledPlayerControls

Specifies which audio playback controls are enabled in the system user interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `All` | Enables all controls. |
| `FastForward` | Enables the Fast-Forward button. |
| `None` | All controls are disabled. |
| `Pause` | Enables the Pause button. |
| `Rewind` | Enables the Rewind button. |
| `SkipNext` | Enables the Next button. |
| `SkipPrevious` | Enables the Previous button. |

## Microsoft.Phone.BackgroundAudio.PlayState

Indicates the possible states that the player can be in, with respect to the current application's background audio.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `BufferingStarted` | The current track has started buffering. |
| `BufferingStopped` | The current track has stopped buffering. |
| `Error` | An error occurred. |
| `FastForwarding` | The current track is fast-forwarding. |
| `Paused` | Playback for this application is paused. |
| `Playing` | This application is currently playing a track. |
| `Rewinding` | The current track is rewinding. |
| `Shutdown` | The application has shutdown. |
| `Stopped` | No playback is occurring in this application. |
| `TrackEnded` | The last track has ended and this application is currently changing tracks. |
| `TrackReady` | The current track is ready for playback. |
| `Unknown` | The current `PlayState` is indeterminate. For example, a new `AudioTrack` has been set, but playback has not been initiated. |

## Microsoft.Phone.BackgroundAudio.UserAction

Indicates the possible user actions.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `FastForward` | Fast-forward the current track. |
| `Pause` | Pause playback. |
| `Play` | Play or resume playback. |
| `Rewind` | Rewind the current track. |
| `Seek` | Seek to a position within the current track. |
| `SkipNext` | Skip to the next track. |
| `SkipPrevious` | Skip to the previous track. |
| `Stop` | Stop playback. |

## Microsoft.Phone.BackgroundTransfer.BackgroundTransferEventArgs

Provides data for `TransferStatusChanged` and `TransferProgressChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `Request` | Gets the `BackgroundTransferRequest` for which the event was raised. <br>Returns: Type: `BackgroundTransferRequest`. The BackgroundTransferRequest for which the event was raised. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest)` | Initializes a new instance of the `BackgroundTransferEventArgs` class. <br>`request`: The `BackgroundTransferRequest` associated with the event args. |

## Microsoft.Phone.BackgroundTransfer.BackgroundTransferInternalException

Represents an exception thrown when the Background Transfer Service encounters an internal error.

## Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest

Represents a background file transfer request.

### Properties / fields

| Member | Description |
|---|---|
| `BytesReceived` | Gets the number of bytes that have been downloaded for the request. <br>Returns: The number of bytes that have been downloaded for the request. |
| `BytesSent` | Gets the number of bytes that have been uploaded for the request. <br>Returns: The number of bytes that have been uploaded for the request. |
| `DownloadLocation` | Gets or sets the location to which the requested file will be downloaded. <br>Returns: The location to which the requested file will be downloaded. |
| `Headers` | Gets the dictionary of HTTP headers for the request. <br>Returns: The dictionary of HTTP headers for the request. |
| `Method` | Gets or sets the HTTP method for the request. <br>Returns: The request method. |
| `RequestId` | Gets the unique identifier associated with the request. <br>Returns: The unique identifier of the request. |
| `RequestUri` | Gets the target URI associated with the request. <br>Returns: The target URI associated with the request. |
| `StatusCode` | The HTTP status code of the request <br>Returns: The HTTP status code of the request |
| `Tag` | Gets or sets additional data associated with the request. <br>Returns: Additional data associated with the request. |
| `TotalBytesToReceive` | Gets the total number of bytes that will be downloaded for the request. <br>Returns: The total number of bytes that will be downloaded for the request. |
| `TotalBytesToSend` | Gets the total number of bytes that will be uploaded for the request. <br>Returns: The total number of bytes that will be uploaded for the request. |
| `TransferError` | Gets the exception associated with a failed background transfer request. A transfer request can have a `TransferStatus` of `Completed` whether or not the transfer was completed successfully. If a completed transfer was successful, TransferError will be null. <br>Returns: The exception associated with a failed background transfer request. |
| `TransferPreferences` | Gets or sets the preferred conditions under which transfers are enabled. <br>Returns: The preferred conditions under which transfers are enabled. |
| `TransferStatus` | Gets the status of the request. <br>Returns: The status of the request. |
| `UploadLocation` | Gets or sets the location from which the requested file will be uploaded. <br>Returns: The location from which the requested file will be uploaded |

### Events

| Member | Description |
|---|---|
| `TransferProgressChanged` | Occurs when the progress of the transfer changes. |
| `TransferStatusChanged` | Occurs when the `TransferStatus` property of a request changes. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Uri)` | Initializes a new instance of the `BackgroundTransferRequest` class with the specified request URI. <br>`requestUri`: The URI of the requested file. |
| `Constructor(Uri,Uri)` | Initializes a new instance of the `BackgroundTransferRequest` class with the specified request URI and download file location. <br>`requestUri`: The URI of the requested file. <br>`downloadLocation`: The location to which the requested file will be downloaded. |
| `Dispose` | Releases all resources used by the current instance of the `BackgroundTransferRequest` class. |
| `Equals(Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest)` | Determines whether this `BackgroundTransferRequest` is equal to the specified object. <br>`other`: The `BackgroundTransferRequest` object to compare to the current instance. <br>Returns: Type: `Boolean`. true if this BackgroundTransferRequest object is equal to the specified object; otherwise, false. |
| `Equals(Object)` | Determines whether this `BackgroundTransferRequest` is equal to the specified object. <br>`other`: The object to compare to the current instance. <br>Returns: Returns `Boolean`. true if this BackgroundTransferRequest object is equal to the specified object; otherwise, false. |
| `GetHashCode` | Returns the hash code for this instance. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of the `BackgroundTransferRequest` object. <br>Returns: Returns `String`. |

## Microsoft.Phone.BackgroundTransfer.BackgroundTransferService

Enables the management of background transfer requests.

### Properties / fields

| Member | Description |
|---|---|
| `Requests` | Gets an enumeration of all active transfer requests associated with the calling application. <br>Returns: Type: `IEnumerable`1`. An enumeration of all active transfer requests associated with the calling application. |

### Methods

| Member | Description |
|---|---|
| `Add(Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest)` | Adds a background transfer request to the queue. <br>`request`: The `BackgroundTransferRequest` to be added to the queue. |
| `Find(String)` | Attempts to return the background transfer request with the specified ID. <br>`requestId`: The ID specifying the transfer request that is being sought. <br>Returns: Returns `BackgroundTransferRequest`. The transfer request associated with the specified ID. |
| `Remove(Microsoft.Phone.BackgroundTransfer.BackgroundTransferRequest)` | Attempts to remove the background transfer request with the specified ID. <br>`request`: The transfer request to be removed. |

## Microsoft.Phone.BackgroundTransfer.TransferPreferences

Defines the list of preferences for background transfer requests.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AllowBattery` | Allow transfers when there is a Wi-Fi connection and the device is using battery or external power. |
| `AllowCellular` | Allow transfers when the device is connected to external power and has a Wi-Fi or cellular connection. |
| `AllowCellularAndBattery` | Allow transfers when the device is using battery or external power and has a Wi-Fi or cellular connection. |
| `None` | Allow transfers only when the device is using external power and has a Wi-Fi connection. This is the default setting. |

## Microsoft.Phone.BackgroundTransfer.TransferStatus

Defines the list of states that a background transfer request can be.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Completed` | The request has completed. This means that the request is no longer actionable by the background transfer service regardless of whether the transfer was completed successfully. To confirm that a transfer was successful, confirm that the `TransferError` property is null. |
| `None` | The request has not yet been queued. |
| `Paused` | The request has been paused and waiting in the background transfer service queue. |
| `Transferring` | The requested file is currently being transferred. |
| `Unknown` | The Background Transfer Service could not retrieve the request status from the service. Once in this state, a `BackgroundTransferRequest` object is no longer useable. You may attempt to retrieve a new instance using the `String)` method. |
| `Waiting` | The request is waiting in the background transfer service queue. This status can indicate that the request is queued and waiting for previous transfers to complete or that the service is retrying the request due to a network error. |
| `WaitingForExternalPower` | The request is waiting in the background transfer service queue for external power to be connected. |
| `WaitingForExternalPowerDueToBatterySaverMode` | The request is waiting for the device to be connected to external power because the user has enabled Battery Saver mode on the device. |
| `WaitingForNonVoiceBlockingNetwork` | The Background Transfer Service does not run when the device is on a non-simultaneous voice and data network, including 2G, EDGE, and standard GPRS. This status indicates the service is waiting for a supported network connection. |
| `WaitingForWiFi` | The request is waiting in the background transfer service queue for a Wi-Fi connection. |

## Microsoft.Phone.Info.DeviceExtendedProperties

Allows an application to obtain information about the device on which it is running.

### Methods

| Member | Description |
|---|---|
| `GetValue(String)` | Retrieves an object representing the specified device property. <br>`propertyName`: The name of the device property to be retrieved. Property names are case-sensitive. <br>Returns: Returns `Object`. |
| `TryGetValue(String,Object@)` | Retrieves an object representing the specified device property. <br>`propertyName`: The name of the device property to be retrieved. Property names are case-sensitive <br>`propertyValue`: The output parameter object in which the value of the device property is stored. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Info.DeviceStatus

Allows an application to obtain information about the device on which it is running.

### Properties / fields

| Member | Description |
|---|---|
| `ApplicationCurrentMemoryUsage` | Returns the memory usage of the current application in bytes. <br>Returns: Returns `Int64`. |
| `ApplicationMemoryUsageLimit` | Returns the maximum additional amount of memory that your application process can allocate. <br>Returns: Returns `Int64`. |
| `ApplicationPeakMemoryUsage` | Returns the peak memory usage of the current application in bytes. <br>Returns: Returns `Int64`. |
| `DeviceFirmwareVersion` | Returns the firmware version running on the device. <br>Returns: Returns `String`. |
| `DeviceHardwareVersion` | Returns the hardware version running on the device. <br>Returns: Returns `String`. |
| `DeviceManufacturer` | Returns the device manufacturer name. <br>Returns: Returns `String`. |
| `DeviceName` | Returns the device name. <br>Returns: Returns `String`. |
| `DeviceTotalMemory` | Returns the physical RAM size of the device in bytes. <br>Returns: Returns `Int64`. |
| `IsKeyboardDeployed` | Indicates whether the user has deployed the physical hardware keyboard of the device. <br>Returns: Returns `Boolean`. |
| `IsKeyboardPresent` | Indicates whether the device contains a physical hardware keyboard. <br>Returns: Returns `Boolean`. |
| `PowerSource` | Indicates if the device is currently running on battery power or is plugged in to an external power supply. <br>Returns: Returns `PowerSource`. |

### Events

| Member | Description |
|---|---|
| `ZeyboardDeployedChanged` | This event occurs when the keyboard has been deployed or closed. |
| `PowerSourceChanged` | This event occurs when the power source of the device changes. |

## Microsoft.Phone.Info.MediaCapabilities

Allows an application to obtain information about the media capabilities of the device on which it is running.

### Properties / fields

| Member | Description |
|---|---|
| `IsMultiResolutionVideoSupported` | Indicates whether the current device supports smooth streaming of multi-resolution encoded video. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Info.PowerSource

Indicates if the device is currently running on battery power or is plugged in to an external power supply.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Battery` | The device is running on battery power. |
| `External` | The device is plugged in to an external power source, such as being docked to a computer or connected to a power supply. |

## Microsoft.Phone.Info.UserExtendedProperties

Allows an application to obtain an anonymous identifier for the user of the device.

### Methods

| Member | Description |
|---|---|
| `GetValue(String)` | Retrieves an object representing the specified user property. In the current release the only supported property is “ANID”. <br>`propertyName`: The name of the user property to be retrieved. Property names are case-sensitive. <br>Returns: Returns `Object`. |
| `TryGetValue(String,Object@)` | Retrieves an object representing the specified user property. In the current release the only supported property is “ANID”. <br>`propertyName`: The name of the user property to be retrieved. Property names are case-sensitive. <br>`propertyValue`: The output parameter object in which the value of the device property is stored. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Marketplace.LicenseInformation

The LicenseInformation class enables an application to determine if it is running under a trial license.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `LicenseInformation` class. |
| `IsTrial` | The IsTrial method is called to determine if an application is running under a trial license. <br>Returns: `Boolean`. Returns True if the application is running under a trial license. Otherwise, returns False. |

## Microsoft.Phone.Net.NetworkInformation.ConnectState

Specifies the connection state of a network interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Connected` | A network interface that is connected. |
| `Disconnected` | A network interface that is disconnected. |

## Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation

Contains network information for a specific Windows Phone device.

### Properties / fields

| Member | Description |
|---|---|
| `CellularMobileOperator` | Gets the name of the cellular mobile operator. <br>Returns: The name of the cellular mobile operator, or a null string. |
| `IsCellularDataEnabled` | Gets a value that indicates whether the network is cellular data enabled. <br>Returns: true if the network is cellular data enabled; otherwise, false. |
| `IsCellularDataRoamingEnabled` | Gets a value that indicates whether the network allows data roaming. <br>Returns: true if the network allows data roaming; otherwise, false. |
| `IsNetworkAvailable` | Gets a value that indicates whether the network is available. <br>Returns: true if there is at least one network interface available; otherwise, false. |
| `IsWiFiEnabled` | Gets a value that indicates whether the network is Wi-Fi enabled. <br>Returns: true if the network is Wi-Fi enabled; otherwise, false. |

### Events

| Member | Description |
|---|---|
| `NetworkAvailabilityChanged` | Occurs when the availability of the network changes. |

### Methods

| Member | Description |
|---|---|
| `ResolveHostNameAsync(Net.DnsEndPoint,Microsoft.Phone.Net.NetworkInformation.NameResolutionCallback,Object)` | Asynchronously resolves a host name on the first available network interface. <br>`endPoint`: A host name or IP address. <br>`callback`: A delegate that references the method to invoke when the operation is complete. <br>`context`: A user-defined object that contains information about the operation. |
| `ResolveHostNameAsync(Net.DnsEndPoint,Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceInfo,Microsoft.Phone.Net.NetworkInformation.NameResolutionCallback,Object)` | Asynchronously resolves a host name on a specified network interface. <br>`endPoint`: A host name or IP address. <br>`networkInterface`: The network interface on which to resolve the host name. <br>`callback`: A delegate that references the method to invoke when the operation is complete. <br>`context`: A user-defined object that contains information about the operation. |

## Microsoft.Phone.Net.NetworkInformation.NameResolutionCallback

Represents the method that is called when an asynchronous name resolution operation completes.

## Microsoft.Phone.Net.NetworkInformation.NameResolutionResult

Contains the result of an attempt to asynchronously resolve a host name.

### Properties / fields

| Member | Description |
|---|---|
| `AsyncState` | Gets a user-defined object that contains information about the operation. <br>Returns: The user-defined object. |
| `HostName` | Gets the host name that was submitted to be resolved. <br>Returns: The host name. |
| `IPEndPoints` | Gets IP addresses for the `HostName`, if the operation succeeded. <br>Returns: An array that contains IP addresses corresponding to the `HostName`. |
| `NetworkErrorCode` | Gets a value that indicates whether an error occurred. <br>Returns: One of the enumeration values that specifies the status of the network operation. |
| `NetworkInterface` | Gets the network interface that was used to resolve the host name. <br>Returns: The network interface that was used to resolve the host name. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `NameResolutionResult` class. |

## Microsoft.Phone.Net.NetworkInformation.NetworkCharacteristics

Specifies characteristics of the network interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `None` | A network that has no particular characteristics. |
| `Roaming` | A roaming network. |

## Microsoft.Phone.Net.NetworkInformation.NetworkError

Describes the status of a network operation, such as trying to connect to a network interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `NameResolutionNoHostAddresses` | The name resolution failed. The host address was incorrect or missing. |
| `NameResolutionOperationAborted` | The name resolution failed. The operation was aborted. |
| `NetworkNotificationRegistrationFailed` | The network notification registration failed. |
| `NetworkOperationFailedError` | The network operation failed. |
| `NetworkSelectionPreferencesFailed` | The network selection failed. A network with the specified preferences was not available. |
| `NetworkSelectionRequirementFailed` | The network selection failed. A network with the specified requirements was not available. |
| `SocketAlreadyBound` | The network connection failed. The socket is already bound. |
| `SocketNotConnected` | The network connection failed. The socket is not connected. |
| `Success` | The network operation succeeded. |
| `WebRequestAlreadyFinished` | The Web request is already finished. |

## Microsoft.Phone.Net.NetworkInformation.NetworkException

The exception that is thrown when a network interface error occurs.

### Properties / fields

| Member | Description |
|---|---|
| `NetworkErrorCode` | Gets the error code that is associated with this exception. <br>Returns: One of the enumeration values that specifies the type of error that occurred. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Microsoft.Phone.Net.NetworkInformation.NetworkError)` | Initializes a new instance of the `NetworkException` class using the supplied network error. <br>`error`: The error code that indicates the error that occurred. |

## Microsoft.Phone.Net.NetworkInformation.NetworkInterface

Provides configuration information for a network interface.

### Properties / fields

| Member | Description |
|---|---|
| `NetworkInterfaceType` | Gets the type of the network servicing Internet requests. <br>Returns: Returns `NetworkInterfaceType`. |

### Methods

| Member | Description |
|---|---|
| `GetInternetInterface` | Gets the NetworkInterface object. <br>Returns: Returns `NetworkInterface`. |

## Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceInfo

Contains all available information about an individual network interface.

### Properties / fields

| Member | Description |
|---|---|
| `Bandwidth` | Gets the speed of the network interface. <br>Returns: The speed of the network interface. |
| `Characteristics` | Gets the characteristics of the network interface. <br>Returns: The characteristics of the network interface. |
| `Description` | Gets a description of the network interface. <br>Returns: The description of the network interface. |
| `InterfaceName` | Gets the name of the network interface. <br>Returns: The name of the network interface. |
| `InterfaceState` | Gets the connection state of the network interface. <br>Returns: One of the enumeration values that specifies the connection state of the network interface. |
| `InterfaceSubtype` | Gets additional information about the type of the network interface. <br>Returns: One of the enumeration values that specifies the sub-type of the network interface. |
| `InterfaceType` | Gets the type of the network interface. <br>Returns: One of the enumeration values that specifies the type of the network interface. |

## Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceList

Contains an enumerable collection of `NetworkInterfaceInfo` objects.

### Properties / fields

| Member | Description |
|---|---|
| `Current` | Gets the current element in the collection. <br>Returns: The current element in the collection. |
| `System#Collections#IEnumerator#Current` | Gets the current element in the collection. <br>Returns: The current element in the collection. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `NetworkInterfaceList` class. |
| `Dispose` | Releases the resources used by the current instance of the `NetworkInterfaceList` class. |
| `Finalize` | Allows an object to attempt to free resources and perform other cleanup operations before it is reclaimed by the garbage collector. |
| `GetEnumerator` | Returns an enumerator that iterates through the `NetworkInterfaceList`. <br>Returns: An enumerator for the `NetworkInterfaceList`. |
| `MoveNext` | Advances the enumerator to the next element of the collection. <br>Returns: true if the enumerator was successfully advanced to the next element; otherwise, false. |
| `Reset` | Resets the `NetworkInterfaceList` to its initial state. |
| `System#Collections#IEnumerable#GetEnumerator` | Returns an enumerator that iterates through the `NetworkInterfaceList`. <br>Returns: An enumerator for the `NetworkInterfaceList`. |

## Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceSubType

Specifies additional information about the type of a network interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Cellular_1XRTT` | A network interface that uses Radio Transmission Technology (1xRTT) cellular technology. This is a 2.5G technology. |
| `Cellular_3G` | A network interface that uses third-generation (3G) cellular technology. |
| `Cellular_EDGE` | A network interface that uses Enhanced Data rates for GSM Evolution (EDGE) cellular technology. This is a 2.5G technology. |
| `Cellular_EVDO` | A network interface that uses Evolution Data Only (EV-DO) cellular technology. This is a 3G technology. |
| `Cellular_EVDV` | A network interface that uses Evolution Data and Voice (EV-DV) cellular technology. This is a 3G technology. |
| `Cellular_GPRS` | A network interface that uses General Packet Radio Service (GPRS) cellular technology. This is a 2.5G technology. |
| `Cellular_HSPA` | A network interface that uses High-Speed Downlink Packet Access (HSPA) cellular technology. This is a 3.5G technology. |
| `Desktop_PassThru` | A desktop pass-through network interface. |
| `Unknown` | There is no additional information about the type of the network interface. |
| `WiFi` | A network interface that uses wireless technology (802.11). |

## Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceType

Specifies the type of a network interface.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `AsymmetricDsl` | Windows Phone does not use this network interface type. |
| `Atm` | Windows Phone does not use this network interface type. |
| `BasicIsdn` | Windows Phone does not use this network interface type. |
| `Ethernet` | The network interface uses an Ethernet connection. Ethernet is defined in IEEE standard 802.3. This is used for desktop pass-through. |
| `Ethernet3Megabit` | Windows Phone does not use this network interface type. |
| `FastEthernetFx` | Windows Phone does not use this network interface type. |
| `FastEthernetT` | Windows Phone does not use this network interface type. |
| `Fddi` | Windows Phone does not use this network interface type. |
| `GenericModem` | Windows Phone does not use this network interface type. |
| `GigabitEthernet` | Windows Phone does not use this network interface type. |
| `HighPerformanceSerialBus` | Windows Phone does not use this network interface type. |
| `IPOverAtm` | Windows Phone does not use this network interface type. |
| `Isdn` | Windows Phone does not use this network interface type. |
| `Loopback` | Windows Phone does not use this network interface type. |
| `MobileBroadbandCdma` | The network interface uses a CDMA cellular network. |
| `MobileBroadbandGsm` | The network interface uses a GSM cellular network. |
| `MultiRateSymmetricDsl` | Windows Phone does not use this network interface type. |
| `None` | There is no network available for accessing the Internet. |
| `Ppp` | Windows Phone does not use this network interface type. |
| `PrimaryIsdn` | Windows Phone does not use this network interface type. |
| `RateAdaptDsl` | Windows Phone does not use this network interface type. |
| `Slip` | Windows Phone does not use this network interface type. |
| `SymmetricDsl` | Windows Phone does not use this network interface type. |
| `TokenRing` | Windows Phone does not use this network interface type. |
| `Tunnel` | Windows Phone does not use this network interface type. |
| `Unknown` | Windows Phone does not use this network interface type. |
| `VeryHighSpeedDsl` | Windows Phone does not use this network interface type. |
| `Wireless80211` | The network interface uses a wireless LAN connection (IEEE 802.11 standard). This is used for any Wi-Fi (802.11, Bluetooth, and so on). |

## Microsoft.Phone.Net.NetworkInformation.NetworkNotificationEventArgs

Provides data for the `NetworkAvailabilityChanged` event.

### Properties / fields

| Member | Description |
|---|---|
| `NetworkInterface` | Gets all available information about the network interface. <br>Returns: The network interface information associated with this notification. |
| `NotificationType` | Gets the type of change that occurred. <br>Returns: One of the enumeration values that indicates the type of change that occurred. |

## Microsoft.Phone.Net.NetworkInformation.NetworkNotificationType

Describes a notification from the network when a change occurs.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CharacteristicUpdate` | A notification indicating that the network interface has changed, such as when it changes to roaming. |
| `InterfaceConnected` | A notification indicating that the network interface has connected. |
| `InterfaceDisconnected` | A notification indicating that the network interface has disconnected. |

## Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics

Specifies the preferred network interface characteristic.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Cellular` | A network interface that uses cellular technology. |
| `NonCellular` | A network interface that does not use cellular technology. |

## Microsoft.Phone.Net.NetworkInformation.SocketExtensions

Defines extension methods for working with a network interface.

### Methods

| Member | Description |
|---|---|
| `AssociateToNetworkInterface(Net.Sockets.Socket,Microsoft.Phone.Net.NetworkInformation.NetworkInterfaceInfo)` | Sets information about a network interface by associating it with a new set of network interface information. <br>`socket`: The network interface for which to set information. <br>`networkInterface`: The new network interface information. |
| `GetCurrentNetworkInterface(Net.Sockets.Socket)` | Gets all available information about the network interface. <br>`socket`: The network interface for which to get the information. <br>Returns: All available information about the network interface. Use the `InterfaceType` and `InterfaceSubtype` properties to determine the network interface technology. |
| `SetNetworkPreference(Net.Sockets.Socket,Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics)` | Sets the preference for a network interface to use either cellular or non-cellular technology. <br>`socket`: The network interface for which to set the preference. <br>`preference`: One of the enumeration values that specify the preferred type of technology. |
| `SetNetworkRequirement(Net.Sockets.Socket,Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics)` | Sets the requirement for a network interface to use either cellular or non-cellular technology. <br>`socket`: The network interface for which to set the requirement. <br>`requirement`: One of the enumeration values that specify the required type of technology. |

## Microsoft.Phone.Net.NetworkInformation.WebRequestExtensions

Defines extension methods for working with a Web request.

### Methods

| Member | Description |
|---|---|
| `GetCurrentNetworkInterface(Net.WebRequest)` | Gets all available information about the network interface that corresponds to a Web request. <br>`request`: The Web request for which to get the information. <br>Returns: All available information about the network interface. Use the `InterfaceType` and `InterfaceSubtype` properties to determine the network interface technology. |
| `SetNetworkPreference(Net.WebRequest,Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics)` | Sets the preference for a Web request to use either cellular or non-cellular technology. <br>`request`: The Web request for which to set the preference. <br>`preference`: One of the enumeration values that specify the preferred type of technology. |
| `SetNetworkRequirement(Net.WebRequest,Microsoft.Phone.Net.NetworkInformation.NetworkSelectionCharacteristics)` | Sets the requirement for a Web request to use either cellular or non-cellular technology. <br>`request`: The Web request for which to set the requirement. <br>`requirement`: One of the enumeration values that specify the required type of technology. |

## Microsoft.Phone.Notification.ChannelConnectionStatus

Identifies the current connectivity status between the Push Client and the Microsoft Push Notification Service

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Connected` | The Push Client and the Microsoft Push Notification Service are connected. |
| `Disconnected` | The Push Client and the Microsoft Push Notification Service are disconnected. |

## Microsoft.Phone.Notification.ChannelErrorType

An enumeration that contains asynchronous errors sent from the Push Client to the application. These error values can occur regardless of if the application is running in the foreground or not.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `ChannelOpenFailed` | This error is returned when the Push Client and the Push Notification Service are unable to establish a connection. For more information on push notifications, see Push Notifications Overview for Windows Phone. |
| `MessageBadContent` | This error is returned when the image reference is pointing to an HTTP image, even though the notification channel is not currently bound to a list of URIs. |
| `NotificationRateTooHigh` | This error is returned when the Push Client is unable to receive messages because the web service is sending too many messages at too quick a rate to a certain device. |
| `PayloadFormatError` | This error is returned when the XML payload format or the HTTP header of the push notification is syntactically invalid. When this error is returned, the HTTP channel is disconnected and the channel must be reopened. |
| `PowerLevelChanged` | This error is returned when the Push Client's power policy changes due to a change in battery level. |
| `Unknown` | This error is returned when an unknown error occurs. |

## Microsoft.Phone.Notification.ChannelPowerLevel

The additional data associated with the `PowerLevelChanged` message.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CriticalLowPowerLevel` | The battery capacity is critically low. No push notifications of any type will be sent to the device. |
| `LowPowerLevel` | The battery capacity is low. Only raw notifications will be sent to the device. |
| `NormalPowerLevel` | The battery capacity is not low. All push notification types will be sent to the device. |

## Microsoft.Phone.Notification.HttpNotification

Contains the raw notification data that has been passed to the application from the Microsoft Push Notification Service.

### Properties / fields

| Member | Description |
|---|---|
| `Body` | The contents of the raw notification data. <br>Returns: Returns `Stream`. |
| `Channel` | The HttpNotificationChannel object that the raw notification is associated with. <br>Returns: Returns `HttpNotificationChannel`. |
| `Headers` | The HTTP headers associated with the raw notification. <br>Returns: Returns `WebHeaderCollection`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates an instance of the HttpNotification class. |

## Microsoft.Phone.Notification.HttpNotificationChannel

Creates a notification channel between the Microsoft Push Notification Service and the Push Client and creates a new subscription for raw notifications.

### Properties / fields

| Member | Description |
|---|---|
| `ChannelName` | The name of the notification channel. <br>Returns: Returns `String`. |
| `ChannelUri` | The current active notification channel URI. <br>Returns: Returns `Uri`. |
| `ConnectionStatus` | Get the current channel connection status. <br>Returns: Returns `ChannelConnectionStatus`. |
| `IsShellTileBound` | Returns true if the notification channel is currently bound to a tile notification subscription and false if not. <br>Returns: Returns `Boolean`. |
| `IsShellToastBound` | Returns true if the notification channel is currently bound to a toast notification subscription. <br>Returns: Returns `Boolean`. |

### Events

| Member | Description |
|---|---|
| `ChannelUriUpdated` | Returns the URI associated with the notification channel. |
| `ConnectionStatusChanged` | This event occurs when the connection status has changed. |
| `ErrorOccurred` | This event is raised when something unexpected happens when using the `HttpNotificationChannel` class. |
| `HttpNotificationReceived` | The event that is raised when the application receives a raw notification. |
| `ShellToastNotificationReceived` | The event that is raised when the application receives a toast notification. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Creates a notification channel. The application will identify the notification channel using the value. <br>`channelName`: The name the application uses to identify the notification channel instance. One channel name is supported per application. |
| `Constructor(String,String)` | Creates a notification channel. Use this constructor with an authenticated web service. The application will identify the notification channel using the value. The value identifies the Subject Name of the authenticated web service's certificate. <br>`channelName`: The name the application uses to identify the notification channel instance. <br>`serviceName`: The name that the web service uses to associate itself with the Push Notification Service. |
| `BindToShellTile` | The method the application uses to bind its default tile with a notification subscription. The tile can only contain local references for resources. |
| `BindToShellTile(Collections.ObjectModel.Collection{Uri})` | Binds the tile passed as the input parameter with a notification subscription. The tile can contain either a local or remote resource reference. <br>`baseUri`: Contains a collection of allowed domains from which to access remote images. Each URI in the collection is limited to 256 characters. |
| `BindToShellToast` | The method the application uses to bind a toast notification subscription to the `HttpNotificationChannel` class instance. |
| `Close` | Closes a notification channel and disassociates all of the subscriptions associated with this instance of the `HttpNotificationChannel` class. |
| `Dispose` | Disposes the class. |
| `Finalize` | Frees resources and performs cleanup operations before the class instance is reclaimed by garbage collection. |
| `Find(String)` | Used to find a previously created notification channel. <br>`channelName`: Name of the channel to search for. <br>Returns: Returns `HttpNotificationChannel`. |
| `Open` | Opens a notification channel with the Microsoft Push Notification Service. |
| `UnbindToShellTile` | Unbinds the active tile notification subscription from the notification channel. |
| `UnbindToShellToast` | Unbinds the active toast notification subscription from the notification channel. |

## Microsoft.Phone.Notification.HttpNotificationEventArgs

Contains the event data when the connectivity status between the Push Client and the Microsoft Push Notification Services changes.

### Properties / fields

| Member | Description |
|---|---|
| `Notification` | Contains the notification data regarding the status change. <br>Returns: Returns `HttpNotification`. |

## Microsoft.Phone.Notification.NotificationChannelConnectionEventArgs

Provides data for `ConnectionStatusChanged` events.

### Properties / fields

| Member | Description |
|---|---|
| `ConnectionStatus` | Contains the new connection status of the notification channel. <br>Returns: Returns `ChannelConnectionStatus`. |

## Microsoft.Phone.Notification.NotificationChannelErrorEventArgs

The argument returned with the `ErrorOccurred` event.

### Properties / fields

| Member | Description |
|---|---|
| `ErrorAdditionalData` | Additional information that is associated with the error type. <br>Returns: Returns `Int32`. |
| `ErrorCode` | Returns an HRESULT that corresponds with the error. <br>Returns: Returns `Int32`. |
| `ErrorType` | Contains an enumeration that corresponds with the error. For more information see `ChannelErrorType`. <br>Returns: Returns `ChannelErrorType`. |
| `Message` | The plain text message of the error. <br>Returns: Returns `String`. |

## Microsoft.Phone.Notification.NotificationChannelUriEventArgs

Contains the event data when a notification channel receives the channel’s URI.

### Properties / fields

| Member | Description |
|---|---|
| `ChannelUri` | Contains the URI associated with the notification channel. <br>Returns: Returns `Uri`. |

## Microsoft.Phone.Notification.NotificationEventArgs

Contains the event data for when a notification channel receives a tile or toast notification.

### Properties / fields

| Member | Description |
|---|---|
| `Collection` | A Dictionary type that contains the toast or tile data sent in the notification. This data is always presented as the name-value pair (<string>, <string>). <br>Returns: Returns `IDictionary`2`. |

## Microsoft.Phone.PictureDecoder

Decodes a JPEG file into a WriteableBitmap object.

### Methods

| Member | Description |
|---|---|
| `DecodeJpeg(IO.Stream)` | Decodes a JPEG into a WriteableBitmap object. <br>`source`: The image data stream. <br>Returns: Returns `WriteableBitmap`. |
| `DecodeJpeg(IO.Stream,Int32,Int32)` | Decodes a JPEG file into a WriteableBitmap object, with parameters for setting the maximum height and width of the output. <br>`source`: The image data stream. <br>`maxPixelWidth`: The maximum width of the output. <br>`maxPixelHeight`: The maximum height of the output. <br>Returns: Returns `WriteableBitmap`. |

## Microsoft.Phone.Scheduler.AgentExitReason

Lists the possible reasons for which a background agent exits. Use the `LastExitReason` property of the `ScheduledTask` class to determine why the task exited its most recent execution.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Aborted` | The agent exited by calling the Abort method. After this method is called, the `IsScheduled` property will be set to false and the agent will not be run again unless the foreground application reschedules it. |
| `Completed` | The agent exited by calling NotifyComplete. An agent should always call this method when it has completed its intended task. |
| `ExecutionTimeExceeded` | The agent was terminated by the system because the allotted time for execution has been exceeded. |
| `MemoryQuotaExceeded` | The agent was terminated by the system because it exceeded the memory cap for background agents. |
| `None` | The agent has never been executed. |
| `Other` | An internal system error caused the agent to be terminated. |
| `Terminated` | The agent was terminated by the system due to the state of the device or operating system. |
| `UnhandledException` | The agent was terminated because an unhandled exception occurred. |

## Microsoft.Phone.Scheduler.Alarm

A notification for which a custom sound can be specified.

### Properties / fields

| Member | Description |
|---|---|
| `Sound` | Gets or sets the sound file that plays when the `Alarm` is launched. <br>Returns: Type `Uri`. The sound file that plays when the alarm is launched. |
| `Title` | This property is inherited from the base `ScheduledNotification` class, but is not supported for alarms. The title that is displayed for all alarm notifications is “Alarm”. <br>Returns: Returns `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `Alarm` class. <br>`name`: The name of the alarm. |

## Microsoft.Phone.Scheduler.PeriodicTask

Represents a scheduled task that runs regularly for a small amount of time.

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `PeriodicTask` class. <br>`name`: The name of the periodic task. |

## Microsoft.Phone.Scheduler.RecurrenceInterval

Lists the recurrence options for the `ScheduledNotification` objects, `Alarm` and `Reminder`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Daily` | Daily recurrence. |
| `EndOfMonth` | Recurring at the end of each month. |
| `Monthly` | Monthly recurrence. |
| `None` | No recurrence. The notification is launched once at the time specified by `BeginTime`. |
| `Weekly` | Weekly recurrence. |
| `Yearly` | Yearly recurrence. |

## Microsoft.Phone.Scheduler.Reminder

A notification that can pass contextual information to the parent application.

### Properties / fields

| Member | Description |
|---|---|
| `NavigationUri` | Gets or sets the navigation URI that is passed to the application when it is launched from the reminder. <br>Returns: Type: `Uri`. The navigation URI that is passed to the application when launched from the reminder. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `Reminder` class. <br>`name`: The name of the reminder. |

## Microsoft.Phone.Scheduler.ResourceIntensiveTask

Represents a scheduled task that runs occasionally and is allowed to use substantial device resources.

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `ResourceIntensiveTask` class. <br>`name`: The name of the resource-intensive task. |

## Microsoft.Phone.Scheduler.ScheduledAction

The base class for all scheduled actions.

### Properties / fields

| Member | Description |
|---|---|
| `BeginTime` | Gets or sets the time at which the action’s schedule begins. <br>Returns: Type: `DateTime`. The time at which the action’s schedule begins. |
| `ExpirationTime` | Gets or sets the time at which the action’s schedule expires. <br>Returns: Type: `DateTime`. The time at which the action’s schedule expires. |
| `IsEnabled` | Gets the enabled status of the action. <br>Returns: Type: `Boolean`. The enabled status of the action. |
| `IsScheduled` | Gets the scheduled status of the action. <br>Returns: Type: `Boolean`. The scheduled status of the action. |
| `Name` | Gets the name of the scheduled action. <br>Returns: Type: `String`. The name of the scheduled action. |

## Microsoft.Phone.Scheduler.ScheduledActionService

Enables the management of scheduled actions.

### Methods

| Member | Description |
|---|---|
| `Add(Microsoft.Phone.Scheduler.ScheduledAction)` | Registers a scheduled action with the operating system. <br>`action`: The `ScheduledAction` to be registered. |
| `Find(String)` | Attempts to return the `ScheduledAction` with the specified name. <br>`name`: The unique identifier of the ScheduledAction to be retrieved. <br>Returns: The ScheduledAction associated with the provided name. If the requested name is not found, null is returned. |
| `GetActions``1` | Returns all `ScheduledAction` objects of the specified type that are registered with the system. <br>Returns: All objects of the specified type that are registered with the system. |
| `LaunchForTest(String,TimeSpan)` | Causes the background agent associated with a `ScheduledTask` to be run in the background after the specified delay. <br>`name`: The name that uniquely identifies the ScheduledTask object. <br>`delay`: The delay after which the associated background agent should be run. |
| `Remove(String)` | Removes the `ScheduledAction` with the specified name from the Scheduled Action Service. <br>`name`: The name uniquely identifying the ScheduledAction to be removed. |
| `Replace(Microsoft.Phone.Scheduler.ScheduledAction)` | Replaces an existing `ScheduledAction` with the one provided. The `Name` property uniquely identifies ScheduledAction objects and is used to determine which existing object, if any, is replaced. <br>`action`: The ScheduledAction object to replace. |

## Microsoft.Phone.Scheduler.ScheduledNotification

Represents a notification that is launched once or in a recurring pattern according to a time-based schedule.

### Properties / fields

| Member | Description |
|---|---|
| `Content` | Gets or sets the text content of the notification that is displayed to the user when the notification is launched. <br>Returns: Type: `String`. The text content of the notification. |
| `RecurrenceType` | Gets or sets the recurrence type for a notification. <br>Returns: Type: `RecurrenceInterval`. The recurrence type for a notification. |
| `Title` | Gets the title of the notification that is displayed to the user when the notification is launched. <br>Returns: Type: `String`. The title of the notification. |

## Microsoft.Phone.Scheduler.ScheduledTask

Represents a task that will be run periodically or when the phone is in a state that supports resource-intensive processing.

### Properties / fields

| Member | Description |
|---|---|
| `BeginTime` | This property is not supported for `ScheduledTask` objects in the current version. <br>Returns: Type: `DateTime`. Not supported. |
| `Description` | Gets or sets the description of the `ScheduledTask`. This string is displayed to the user in the Background Tasks settings page on the device. <br>Returns: Type: `String`. The description of the ScheduledTask. |
| `ExpirationTime` | Gets or sets the time at which the task’s schedule expires. <br>Returns: Type: `DateTime`. The time at which the task’s schedule expires. |
| `LastExitReason` | Gets the reason that the agent exited the last time it was run. <br>Returns: Returns `AgentExitReason`. The reason the agent exited. |
| `LastScheduledTime` | Gets the last time the agent was scheduled to be run, in the device’s local time. <br>Returns: The last time the agent was scheduled to be run. |

## Microsoft.Phone.Scheduler.ScheduledTaskAgent

Override this class to implement a Scheduled Task, also referred to as a background agent. This class contains one method, `ScheduledTask)`, which is called whenever the Scheduled Task is executed. For more information, see Scheduled Tasks Overview for Windows Phone.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ScheduledTaskAgent` class. |
| `OnCancel` | Not supported. |
| `OnInvoke(Microsoft.Phone.Scheduler.ScheduledTask)` | Called when the Scheduled Task is executed. Override this method and provide the code that should execute in the background. <br>`task`: The `ScheduledTask` object for which Invoke is being called. |

## Microsoft.Phone.Scheduler.SchedulerServiceException

Represents the exception that is thrown when the `ScheduledActionService` has an internal error.

### Methods

| Member | Description |
|---|---|
| `Constructor(Int32,String)` | Initializes a new instance of the `SchedulerServiceException` class. <br>`hResult`: The error code associated with the exception. <br>`message`: The message associated with the exception. |

## Microsoft.Phone.Tasks.AddressChooserTask

Allows an application to launch the Contacts application. Use this to obtain the physical address of a contact selected by the user.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AddressChooserTask` class. |
| `Show` | Shows the Contacts application, allowing the user to choose a contact for which the physical address is obtained. |

## Microsoft.Phone.Tasks.AddressResult

Represents a physical address returned from a call to the `Show` method.

### Properties / fields

| Member | Description |
|---|---|
| `Address` | Gets the physical address contained in the result. <br>Returns: Type: `String`. The physical address contained in the result. |
| `DisplayName` | Gets the display name associated with the result. <br>Returns: Returns `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `AddressResult` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `AddressResult` class with the specified `TaskResult`. <br>`taskResult`: The `TaskResult` associated with the new `AddressResult`. |

## Microsoft.Phone.Tasks.BingMapsDirectionsTask

Allows an application to launch the Bing Maps application, specifying a starting location or an ending location, or both, for which driving directions are displayed.

### Properties / fields

| Member | Description |
|---|---|
| `End` | The ending location for which driving directions are displayed. <br>Returns: Type: `LabeledMapLocation`. The ending location for which driving directions are displayed. |
| `Start` | The starting location for which driving directions are displayed. <br>Returns: Type: `LabeledMapLocation`. The starting location for which driving directions are displayed. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BingMapsDirectionsTask` class. |
| `Show` | Shows the Bing Maps application with driving directions displayed for the specified starting and ending locations. |

## Microsoft.Phone.Tasks.BingMapsTask

Allows an application to launch the Bing Maps application centered at the location specified with the `Center` property or at the user’s current location. If `SearchTerm` is set, locations matching the search term are tagged on the map.

### Properties / fields

| Member | Description |
|---|---|
| `Center` | Gets or sets the location that will be used as the center point for the map. <br>Returns: Type: `GeoCoordinate`. The center point of the map. |
| `SearchTerm` | Gets or sets the search term that is used to find and tag locations on the map. <br>Returns: Type: `String`. The search term used to find and tag locations on the map. |
| `ZoomLevel` | Gets or sets the initial zoom level of the map. <br>Returns: Type: `Double`. The zoom level of the map. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `BingMapsTask` class. |
| `Show` | Shows the Bing Maps application centered at the location specified with the `Center` property or at the user’s current location. If `SearchTerm` is set, locations matching the search term are tagged on the map. |

## Microsoft.Phone.Tasks.CameraCaptureTask

Allows an application to launch the Camera application. Use this to allow users to take a photo from your application.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `CameraCaptureTask` class. |
| `Show` | Shows the Camera application. |

## Microsoft.Phone.Tasks.ChooserBase`1

The base class from which all Choosers are derived. This class exposes a common function for showing the Choosers and an event for handling the Chooser result.

### Properties / fields

| Member | Description |
|---|---|
| `TaskEventArgs` | The EventArgs for the `Completed` event. |

### Events

| Member | Description |
|---|---|
| `Completed` | Occurs when a Chooser task is completed. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `ChooserBase`1` class. |
| `FireCompleted(Object,`0,Delegate)` | Raises the `Completed` event. <br>`sender`: The source of the event. <br>`e`: The EventArgs associated with the event. <br>`fireThisHandlerOnly`: The delegate to be called when the event is raised. |
| `Show` | Launches and displays the Chooser. |

## Microsoft.Phone.Tasks.EmailAddressChooserTask

Allows an application to launch the Contacts application. Use this to obtain the email address of a contact selected by the user.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EmailAddressChooserTask` class. |
| `Show` | Shows the email address chooser application. |

## Microsoft.Phone.Tasks.EmailComposeTask

Allows an application to launch the email application with a new message displayed. Use this to allow users to send email from your application.

### Properties / fields

| Member | Description |
|---|---|
| `Bcc` | Gets or sets the recipients on the Bcc line of the new email message. <br>Returns: Type: `String`. Gets or sets the recipients on the Bcc line of the new email message. |
| `Body` | Gets or sets the body of the new email message. <br>Returns: Type: `String`. The body of the new email message. |
| `Cc` | Gets or sets the recipients on the Cc line of the new email message. <br>Returns: Type: `String`. The recipients on the Cc line of the new email message. |
| `CodePage` | Gets or sets the character set that will be used to display the message content. <br>Returns: The codepage for the new email message. |
| `Subject` | Gets or sets the subject of the new email message. <br>Returns: Type: `String`. The subject of the new email message. |
| `To` | Gets or sets the recipients on the To line of the new email message. <br>Returns: Type: `String`. The recipients on the To line of the new email message. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EmailComposeTask` class. |
| `Show` | Shows the email application with a new message displayed. |

## Microsoft.Phone.Tasks.EmailResult

Represents an email address returned from a call to the Show method of a `EmailAddressChooserTask` object.

### Properties / fields

| Member | Description |
|---|---|
| `DisplayName` | Gets the display name associated with the result. <br>Returns: Type: `String` The display name associated with the result. |
| `Email` | Gets the email address contained in the result. <br>Returns: Type: `String`. The email address contained in the result. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `EmailResult` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `EmailResult` class with the specified `TaskResult`. <br>`taskResult`: The `TaskResult` associated with the new `EmailResult`. |

## Microsoft.Phone.Tasks.GameInviteTask

Allows an application to show a game invite screen that allows the user to invite players to a multiplayer game session.

### Properties / fields

| Member | Description |
|---|---|
| `SessionId` | Gets or sets a string identifying the network session. <br>Returns: Type: `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `GameInviteTask` class. |
| `Show` | Shows the game invite. |

## Microsoft.Phone.Tasks.LabeledMapLocation

Represents a geographic coordinate and an associated label that identifies it, such as the location of a business and the business name. This class is used with the `BingMapsDirectionsTask` class.

### Properties / fields

| Member | Description |
|---|---|
| `Label` | Gets or sets the text label that identifies the associated geographic location. <br>Returns: Type: `String`. The label that identifies the associated geographic location. |
| `Location` | Gets or sets the geographic coordinate associated with a labeled map location. <br>Returns: Type: `GeoCoordinate`. The geographic coordinate associated with a labeled map location. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `LabeledMapLocation` class. |
| `Constructor(String,Device.Location.GeoCoordinate)` | Creates a new instance of the `LabeledMapLocation` class. <br>`label`: The label that identifies the associated geographic location. <br>`location`: The geographic coordinate associated with a labeled map location. |

## Microsoft.Phone.Tasks.MarketplaceContentType

Defines the list of content types that can be shown by the Windows Phone Marketplace client application. This enumeration is used by `MarketplaceDetailTask`, `MarketplaceHubTask`, and `MarketplaceSearchTask`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Applications` | Application content. |
| `Music` | Music content. |

## Microsoft.Phone.Tasks.MarketplaceDetailTask

Allows an application to launch the Windows Phone Marketplace client application and display the details page for the specified product.

### Properties / fields

| Member | Description |
|---|---|
| `ContentIdentifier` | Gets or sets the unique identifier for the product to be displayed. <br>Returns: Type: `String`. |
| `ContentType` | Gets or sets the type of content displayed in the Windows Phone Marketplace client application. <br>Returns: Type: `MarketplaceContentType`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `MarketplaceDetailTask` class. |
| `Show` | Shows the Windows Phone Marketplace client application and displays the details page for the specified product. |

## Microsoft.Phone.Tasks.MarketplaceHubTask

Allows an application to launch the Windows Phone Marketplace client application.

### Properties / fields

| Member | Description |
|---|---|
| `ContentType` | Gets or sets the type of content displayed in the Windows Phone Marketplace client application. <br>Returns: Type: `MarketplaceContentType`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `MarketplaceHubTask` class. |
| `Show` | Shows the Windows Phone Marketplace client application. |

## Microsoft.Phone.Tasks.MarketplaceReviewTask

Allows an application to launch the Windows Phone Marketplace client application and display the review page for the specified product.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `MarketplaceReviewTask` class. |
| `Show` | Shows the Windows Phone Marketplace client application and displays the review page for the specified product. |

## Microsoft.Phone.Tasks.MarketplaceSearchTask

Allows an application to launch the Windows Phone Marketplace client application and display the search results from the specified search terms.

### Properties / fields

| Member | Description |
|---|---|
| `ContentType` | Gets or sets the type of content displayed in the Windows Phone Marketplace client application. <br>Returns: Type: `MarketplaceContentType`. |
| `SearchTerms` | Gets or sets the search terms. <br>Returns: Type: `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `MarketplaceSearchTask` class. |
| `Show` | Shows the Windows Phone Marketplace client application and displays the search results from the specified search terms. |

## Microsoft.Phone.Tasks.MediaLocationType

Lists the data stores in which a media file may be stored. Used by `MediaPlayerLauncher`.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Data` | The media file is in isolated storage. |
| `Install` | The media file is in the application’s installation directory. |
| `None` | The media item is in neither data store. The Show method will throw a FileNotFoundException if this value is used. |

## Microsoft.Phone.Tasks.MediaPlaybackControls

An enumeration defining the bitwise flags that are used with the Controls property of the `MediaPlayerLauncher` object to specify which controls should be displayed by the media player application.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `All` | All controls. The equivalent of using OR to combine all of the other members of the enumeration. |
| `FastForward` | The fast forward control. |
| `None` | No controls. |
| `Pause` | The pause control. |
| `Rewind` | The rewind control. |
| `Skip` | The skip control. |
| `Stop` | The stop control. |

## Microsoft.Phone.Tasks.MediaPlayerLauncher

Allows an application to launch the media player.

### Properties / fields

| Member | Description |
|---|---|
| `Controls` | Gets or sets the flags that determine which controls are displayed in the media player application. <br>Returns: Type `MediaPlaybackControls`. The bitwise combination of `MediaPlaybackControls` members that determine which controls are displayed in the media player application. |
| `Location` | Sets the location of the media file to be played. The `MediaLocationType` enumeration is used to specify either isolated storage or the application’s installation folder. <br>Returns: Type: `MediaLocationType`. |
| `Media` | Gets or sets the media played with the media player application. <br>Returns: Type: `Uri`. The URI of a media file. |
| `Orientation` | Gets or sets the orientation in which the media player will be displayed when launched. <br>Returns: Type: `MediaPlayerOrientation`. The orientation in which the media player will be displayed when launched. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the media player application. |
| `Show` | Shows the media player application. |

## Microsoft.Phone.Tasks.MediaPlayerOrientation

Defines the list of orientations in which the media player can be launched.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Landscape` | Landscape orientation. |
| `Portrait` | Portrait orientation. |

## Microsoft.Phone.Tasks.PhoneCallTask

Allows an application to launch the Phone application. Use this to allow users to make a phone call from your application.

### Properties / fields

| Member | Description |
|---|---|
| `DisplayName` | Gets or sets the name that is displayed when the Phone application is launched. <br>Returns: Type: `String`. The name that is displayed when the Phone application is launched. |
| `PhoneNumber` | Gets or sets the phone number that is dialed when the Phone application is launched. <br>Returns: Type: `String`. The phone number that is dialed when the Phone application is launched. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PhoneCallTask` class. |
| `Show` | Shows the Phone application. |

## Microsoft.Phone.Tasks.PhoneNumberChooserTask

Allows an application to launch the Contacts application. Use this to obtain the phone number of a contact selected by the user.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PhoneNumberChooserTask` class. |
| `Show` | Shows the Contacts application. |

## Microsoft.Phone.Tasks.PhoneNumberResult

Represents a phone number returned from a call to the Show method of a `PhoneNumberChooserTask` object.

### Properties / fields

| Member | Description |
|---|---|
| `DisplayName` | Gets the display name associated with the phone number selected by the user. <br>Returns: Type: `String`. |
| `PhoneNumber` | Gets the phone number selected by the user. <br>Returns: Type: `String`. The phone number selected by the user. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PhoneNumberResult` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `PhoneNumberResult` class with the specified `TaskResult`. <br>`taskResult`: The `TaskResult` associated with the new `PhoneNumberResult`. |

## Microsoft.Phone.Tasks.PhotoChooserTask

Allows an application to launch the Photo Chooser application. Use this to allow users to select a photo.

### Properties / fields

| Member | Description |
|---|---|
| `PixelHeight` | Gets or sets the maximum height and the height component of the aspect ratio for a cropping region set by the user during the photo choosing process. <br>Returns: Type: `Int32`. The height at which the image will be cropped. |
| `PixelWidth` | Gets or sets the maximum height and the height component of the aspect ratio for a cropping region set by the user during the photo choosing process. <br>Returns: Type: `Int32`. The width at which the image will be cropped. |
| `ShowCamera` | Gets or sets whether the user is presented with a button for launching the camera during the photo choosing process. <br>Returns: Type: `Boolean`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the Photo Chooser application. |
| `Show` | Shows the Photo Chooser application. |

## Microsoft.Phone.Tasks.PhotoResult

Represents a photo returned from a call to the Show method of a `PhotoChooserTask` object or a `CameraCaptureTask` object.

### Properties / fields

| Member | Description |
|---|---|
| `ChosenPhoto` | Gets the stream containing the data for the photo. <br>Returns: Type: `Stream`. The data for the photo. |
| `OriginalFileName` | Gets the file name of the photo. <br>Returns: Type `String`. The file name of the photo. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `PhotoResult` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `PhotoResult` class with the specified `TaskResult`. <br>`taskResult`: The `TaskResult` associated with the new `PhotoResult`. |

## Microsoft.Phone.Tasks.SaveContactResult

Contains the result of an attempt to save a contact.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SaveContactResult` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `SaveContactResult` class, with the specified `TaskResult`. <br>`taskResult`: One of the enumeration values that specifies the status of the Chooser operation. |

## Microsoft.Phone.Tasks.SaveContactTask

Provides methods and events for launching the contacts application and enabling a user to save a contact.

### Properties / fields

| Member | Description |
|---|---|
| `Company` | Gets or sets the name of the company associated with the contact. <br>Returns: The name of the company. |
| `FirstName` | Gets or sets the given name of the contact. <br>Returns: The given name of the contact. |
| `HomeAddressCity` | Gets or sets the city of the home address associated with the contact. <br>Returns: The city of the home address. |
| `HomeAddressCountry` | Gets or sets the country or region of the home address associated with the contact. <br>Returns: The country or region of the home address. |
| `HomeAddressState` | Gets or sets the state of the home address associated with the contact. <br>Returns: The state of the home address. |
| `HomeAddressStreet` | Gets or sets the street name and number of the home address associated with the contact. <br>Returns: The street name and number of the home address. |
| `HomeAddressZipCode` | Gets or sets the postal code of the home address associated with the contact. <br>Returns: The postal code of the home address. |
| `HomePhone` | Gets or sets the home phone number associated with the contact. <br>Returns: The home phone number. |
| `JobTitle` | Gets or sets the job title associated with the contact. <br>Returns: The job title of the contact. |
| `LastName` | Gets or sets the surname of the contact. <br>Returns: The surname of the contact. |
| `MiddleName` | Gets or sets the middle name of the contact. <br>Returns: The middle name of the contact. |
| `MobilePhone` | Gets or sets the mobile phone number associated with the contact. <br>Returns: The mobile phone number. |
| `Nickname` | Gets or sets the nickname of the contact. <br>Returns: The nickname of the contact. |
| `Notes` | Gets or sets a note associated with the contact. <br>Returns: The note associated with the contact. |
| `OtherEmail` | Gets or sets an additional email address associated with the contact. <br>Returns: The additional email address. |
| `PersonalEmail` | Gets or sets the personal email address associated with the contact. <br>Returns: The personal email address. |
| `Suffix` | Gets or sets the suffix of the name of the contact. <br>Returns: The suffix of the name of the contact. |
| `Title` | Gets or sets the title of the name of the contact. <br>Returns: The title of the name of the contact. |
| `Website` | Gets or sets the website associated with the contact. <br>Returns: The website associated with the contact. |
| `WorkAddressCity` | Gets or sets the city of the work address associated with the contact. <br>Returns: The city of the work address. |
| `WorkAddressCountry` | Gets or sets the country or region of the work address associated with the contact. <br>Returns: The country or region of the work address. |
| `WorkAddressState` | Gets or sets the state of the work address associated with the contact. <br>Returns: The state of the work address. |
| `WorkAddressStreet` | Gets or sets the street name and number of the work address associated with the contact. <br>Returns: The street name and number of the work address. |
| `WorkAddressZipCode` | Gets or sets the postal code of the work address associated with the contact. <br>Returns: The postal code of the work address. |
| `WorkEmail` | Gets or sets the work email address associated with the contact. <br>Returns: The work email address. |
| `WorkPhone` | Gets or sets the work phone number associated with the contact. <br>Returns: The work phone number. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SaveContactTask` class. |
| `Show` | Launches the contacts application and enables the user to save a contact. |

## Microsoft.Phone.Tasks.SaveEmailAddressTask

Allows an application to launch the contacts application. Use this to allow users to save an email address from your application to a new or existing contact.

### Properties / fields

| Member | Description |
|---|---|
| `Email` | Gets or sets the email address that can be saved to a contact. <br>Returns: Type `String`. The email address that can be saved to a contact. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SaveEmailAddressTask` class. |
| `Show` | Shows the contacts application. |

## Microsoft.Phone.Tasks.SavePhoneNumberTask

Allows an application to launch the contacts application. Use this to allow users to save a phone number from your application to a new or existing contact.

### Properties / fields

| Member | Description |
|---|---|
| `PhoneNumber` | Gets or sets the phone number that can be saved to a contact. <br>Returns: Type: `String`. The phone number that can be saved to a contact. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SavePhoneNumberTask` class. |
| `Show` | Shows the contacts application. |

## Microsoft.Phone.Tasks.SaveRingtoneTask

Enables an application to launch the ringtones application. Use the `SaveRingtoneTask` to allow users to save a ringtone from your application to the system ringtones list.

### Properties / fields

| Member | Description |
|---|---|
| `DisplayName` | Name used to identify the ringtone in the picker. <br>Returns: Returns `String`. |
| `IsShareable` | Flag that indicates if the ringtone should be accessible to other applications. <br>Returns: Returns `Boolean`. |
| `Source` | File path to the local copy of the audio file to be added to the ringtone directory. <br>Returns: Returns `Uri`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SaveRingtoneTask` class. |
| `Show` | Shows the ringtones application. |

## Microsoft.Phone.Tasks.SearchTask

Allows an application to launch the Web Search application.

### Properties / fields

| Member | Description |
|---|---|
| `SearchQuery` | Gets or sets the search query that the Web Search application will use when it is launched. <br>Returns: Type: `String`. The search query that the Web Search application will use when it is launched. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `SearchTask` class. |
| `Show` | Shows the Web Search application. |

## Microsoft.Phone.Tasks.ShareLinkTask

Allows an application to launch a dialog that enables the user to share a link on the social networks of their choice.

### Properties / fields

| Member | Description |
|---|---|
| `LinkUri` | Gets or sets the link URI that will be displayed in the link sharing dialog. <br>Returns: The URI of the link to be shared. |
| `Message` | Gets or sets the message that will accompany the link when it is shared. <br>Returns: The message that will accompany the shared link. |
| `Title` | Gets or sets the title of the link to be shared. <br>Returns: The title of the shared link. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ShareLinkTask` class. |

## Microsoft.Phone.Tasks.ShareStatusTask

Allows an application to launch a dialog that enables the user to share a status message on the social networks of their choice.

### Properties / fields

| Member | Description |
|---|---|
| `Status` | Gets or sets the status message to be shared. <br>Returns: The shared status message. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `ShareStatusTask`. |

## Microsoft.Phone.Tasks.ShareTaskBase

The base class for the social network sharing APIs, `ShareLinkTask` and `ShareStatusTask`.

### Methods

| Member | Description |
|---|---|
| `Constructor` | This is an abstract base class and cannot be instatiated directly. Use the derived classes `ShareLinkTask` and `ShareStatusTask` instead. |
| `Show` | Causes the sharing dialog to be displayed to the user. |

## Microsoft.Phone.Tasks.SmsComposeTask

Launches the Messaging application with a new SMS message displayed.

### Properties / fields

| Member | Description |
|---|---|
| `Body` | Gets or sets the body text of the new SMS message. <br>Returns: Type: `String`. |
| `To` | Gets or sets the recipient list for the new SMS message. <br>Returns: Type: `String`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `SmsComposeTask` class. |
| `Show` | Shows the Messaging application. |

## Microsoft.Phone.Tasks.TaskEventArgs

The EventArgs used by the `Completed` event for all Choosers.

### Properties / fields

| Member | Description |
|---|---|
| `Error` | The exception associated with the `TaskEventArgs`. <br>Returns: Type: `Exception`. |
| `TaskResult` | The `TaskResult` associated with the `TaskEventArgs`. This indicates whether the task was completed successfully, if the user cancelled the task, or if no result information is available. <br>Returns: One of the enumeration values that specifies the status of the task. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `TaskEventArgs` class. |
| `Constructor(Microsoft.Phone.Tasks.TaskResult)` | Initializes a new instance of the `TaskEventArgs` class with the specified `TaskResult`. <br>`taskResult`: One of the enumeration values that specifies the status of the task. |

## Microsoft.Phone.Tasks.TaskResult

Describes the success status of a chooser operation.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Cancel` | The chooser operation was cancelled by the user. |
| `None` | No success status was returned from the chooser operation. |
| `OK` | The chooser operation was successful. |

## Microsoft.Phone.Tasks.WebBrowserTask

Allows an application to launch the web browser application.

### Properties / fields

| Member | Description |
|---|---|
| `URL` | Gets or sets the URL to which the web browser application will navigate when it is launched. <br>Returns: Type: `String`. The URL to which the web browser application will navigate when it is launched. |
| `Uri` | Gets or sets the URI to which the web browser application will navigate when it is launched. <br>Returns: Type: `Uri`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `WebBrowserTask` class. |
| `Show` | Shows the web browser application. |

## Microsoft.Phone.UserData.Account

Contains information about a user’s account with a data provider, such as Facebook or Microsoft Outlook, that the user has set up on the phone.

### Properties / fields

| Member | Description |
|---|---|
| `Zind` | Gets the kind of account, such as `Phone` or `Outlook`. <br>Returns: One of the enumeration values that specifies the kind of account this is. |
| `Name` | Gets the name of the account, such as “Work Email.” <br>Returns: The name of the account. |

## Microsoft.Phone.UserData.AddressKind

Specifies information about a physical address.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Home` | A home address. |
| `Other` | A different kind of address, such as school, or the kind of address is unknown. |
| `Work` | A work address. |

## Microsoft.Phone.UserData.Appointment

Contains all available information about an individual appointment.

### Properties / fields

| Member | Description |
|---|---|
| `Account` | Gets the data source associated with this appointment. <br>Returns: The data source associated with this appointment. |
| `Attendees` | Gets the attendees associated with this appointment. <br>Returns: The attendees associated with this appointment. |
| `Details` | Gets a detailed description of the appointment. <br>Returns: A detailed description of the appointment. |
| `EndTime` | Gets the date and time that the appointment ends. <br>Returns: The date and time that the appointment ends. |
| `IsAllDayEvent` | Gets a value that indicates whether the appointment is an all-day event. <br>Returns: true if the appointment is an all-day event; otherwise, false. |
| `IsPrivate` | Gets a value that indicates whether the appointment is private. <br>Returns: true if the appointment is private; otherwise, false. |
| `Location` | Gets the location of the appointment. <br>Returns: The location of the appointment. |
| `Organizer` | Gets the organizer of the appointment. <br>Returns: The organizer of the appointment. |
| `StartTime` | Gets the date and time that the appointment starts. <br>Returns: The date and time that the appointment starts. |
| `Status` | Gets information about how to treat the block of time of this appointment, such as busy or out of the office. <br>Returns: One of the enumeration values that specifies how to treat the block of time of this appointment. |
| `Subject` | Gets the subject of the appointment. <br>Returns: The subject of the appointment. |

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Determines whether the specified appointment is equal to the current appointment. <br>`obj`: The appointment to compare with the current appointment. <br>Returns: true if the specified appointment is equal to the current appointment; otherwise, false. |
| `GetHashCode` | Serves as a hash function for the appointment type. <br>Returns: A hash code for the current appointment. |

## Microsoft.Phone.UserData.AppointmentStatus

Specifies information about how to treat the block of time of an appointment.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Busy` | The attendee is busy during this appointment. |
| `Free` | The attendee is free during this appointment. |
| `OutOfOffice` | The attendee is out of the office during this appointment. |
| `Tentative` | The attendee is tentatively busy during this appointment. |

## Microsoft.Phone.UserData.Appointments

Provides methods and events for interacting with a user’s appointment data.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources from which appointments are available. <br>Returns: The data sources from which appointments are available. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DefaultMaximumItems` | Represents the maximum number of appointments that are returned from a search, by default. |

### Events

| Member | Description |
|---|---|
| `SearchCompleted` | Occurs when a search for appointments completes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Appointments` class. |
| `SearchAsync(DateTime,DateTime,Microsoft.Phone.UserData.Account,Object)` | Asynchronously searches for appointments that occur between the specified start date and time and end date and time, from the specified data source. <br>`startTimeInclusive`: The start date and time to use to search for appointments. <br>`endTimeInclusive`: The end date and time to use to search for appointments. <br>`account`: The data source to search for appointments. <br>`state`: A user-defined object that contains information about the operation. |
| `SearchAsync(DateTime,DateTime,Int32,Microsoft.Phone.UserData.Account,Object)` | Asynchronously searches for appointments that occur between the specified start date and time and end date and time, from the specified data source, returning no more than the specified number of appointments. <br>`startTimeInclusive`: The start date and time to use to search for appointments. <br>`endTimeInclusive`: The end date and time to use to search for appointments. <br>`maximumItems`: The maximum number of appointments to return. <br>`account`: The data source to search for appointments. <br>`state`: A user-defined object that contains information about the operation. |
| `SearchAsync(DateTime,DateTime,Int32,Object)` | Asynchronously searches for appointments that occur between the specified start date and time and end date and time, returning no more than the specified number of appointments. <br>`startTimeInclusive`: The start date and time to use to search for appointments. <br>`endTimeInclusive`: The end date and time to use to search for appointments. <br>`maximumItems`: The maximum number of appointments to return. <br>`state`: A user-defined object that contains information about the operation. |
| `SearchAsync(DateTime,DateTime,Object)` | Asynchronously searches for appointments that occur between the specified start date and time and end date and time. <br>`startTimeInclusive`: The start date and time to use to search for appointments. <br>`endTimeInclusive`: The end date and time to use to search for appointments. <br>`state`: A user-defined object that contains information about the operation. |

## Microsoft.Phone.UserData.AppointmentsSearchEventArgs

Provides data for the `SearchCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `EndTimeInclusive` | Gets the end date and time that was used for a search. <br>Returns: The end date and time that was used for the search. |
| `Results` | Gets the results of a search for appointments. <br>Returns: The results of the search. |
| `StartTimeInclusive` | Gets the start date and time that was used for a search. <br>Returns: The starting date and time that was used for the search. |
| `State` | Gets a user-defined object that contains information about the operation. <br>Returns: The user-defined object. |

## Microsoft.Phone.UserData.Attendee

Contains information about an attendee associated with an appointment.

### Properties / fields

| Member | Description |
|---|---|
| `DisplayName` | Gets the name of the attendee. <br>Returns: The name of the attendee. |
| `EmailAddress` | Gets the email address of the attendee. <br>Returns: The email address of the attendee. |

## Microsoft.Phone.UserData.CompleteName

Contains information about the name of a contact.

### Properties / fields

| Member | Description |
|---|---|
| `FirstName` | Gets the given name of a contact. <br>Returns: The given name of the contact. |
| `LastName` | Gets the surname of a contact. <br>Returns: The surname of a contact. |
| `MiddleName` | Gets the middle name of a contact. <br>Returns: The middle name of the contact. |
| `Nickname` | Gets the nickname of the contact. <br>Returns: The nickname of the contact. |
| `Suffix` | Gets the suffix of the name of a contact. <br>Returns: The suffix of the name of the contact. |
| `Title` | Gets the title of the name of a contact. <br>Returns: The title of the name of the contact. |
| `YomiFirstName` | Gets the Yomi given name of a contact. <br>Returns: The Yomi given name of a contact. |
| `YomiLastName` | Gets the Yomigana surname of a contact. <br>Returns: The Yomigana surname of the contact. |

## Microsoft.Phone.UserData.Contact

Contains all available information about an individual contact.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources associated with this contact. <br>Returns: The data sources associated with this contact. |
| `Addresses` | Gets the addresses associated with this contact. <br>Returns: The addresses associated with this contact. |
| `Birthdays` | Gets the birthdays associated with this contact. <br>Returns: The birthdays associated with this contact. |
| `Children` | Gets the children associated with this contact. <br>Returns: The children associated with this contact. |
| `Companies` | Gets the companies associated with this contact. <br>Returns: The companies associated with this contact. |
| `CompleteName` | Gets the complete name associated with this contact. <br>Returns: The complete name associated with this contact. |
| `DisplayName` | Gets the display name of the contact. <br>Returns: The display name of the contact. |
| `EmailAddresses` | Gets the email addresses associated with this contact. <br>Returns: The email addresses associated with this contact. |
| `IsPinnedToStart` | Gets a value that indicates whether the contact is pinned to the Start screen. <br>Returns: true if the contact is pinned to Start; otherwise, false. |
| `Notes` | Gets the notes associated with this contact. <br>Returns: The notes associated with this contact. |
| `PhoneNumbers` | Gets the phone numbers associated with this contact. <br>Returns: The phone numbers associated with this contact. |
| `SignificantOthers` | Gets the significant others associated with this contact. <br>Returns: The significant others associated with this contact. |
| `Websites` | Gets the websites associated with this contact. <br>Returns: The websites associated with this contact. |

### Methods

| Member | Description |
|---|---|
| `Equals(Object)` | Determines whether the specified contact is equal to the current contact. <br>`obj`: The contact to compare with the current contact. <br>Returns: true if the specified contact is equal to the current contact; otherwise, false. |
| `GetHashCode` | Serves as a hash function for the contact type. <br>Returns: A hash code for the current contact. |
| `GetPicture` | Gets a picture of the contact. <br>Returns: A stream that contains the data for the picture. |
| `ToString` | Returns a string that represents the current contact. <br>Returns: A string that represents the current contact. |

## Microsoft.Phone.UserData.ContactAddress

Contains information about an address associated with a contact.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources associated with this address. <br>Returns: The data sources associated with this address. |
| `Zind` | Gets the kind of address, such as home or work. <br>Returns: One of the enumeration values that specifies the kind of address this is. |
| `PhysicalAddress` | Gets the details of the physical address. <br>Returns: The details of the physical address. |

## Microsoft.Phone.UserData.ContactCompanyInformation

Contains information for a company associated with a contact, such as the contact’s title and office location.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources associated with this company information. <br>Returns: The data sources associated with this company information. |
| `CompanyName` | Gets the name of the company. <br>Returns: The name of the company. |
| `JobTitle` | Gets the contact’s job title at the company. <br>Returns: The contact’s job title at the company. |
| `OfficeLocation` | Gets the contact’s office location at the company. <br>Returns: The contact’s office location at the company. |
| `YomiCompanyName` | Gets the Yomigana name of the company. <br>Returns: The Yomigana name of the company. |

## Microsoft.Phone.UserData.ContactEmailAddress

Contains information about an email address associated with a contact.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources associated with this email address. <br>Returns: The data sources associated with this email address. |
| `EmailAddress` | Gets the email address. <br>Returns: The email address. |
| `Zind` | Gets the kind of email address, such as personal or work. <br>Returns: One of the enumeration values that specifies the kind of email address this is. |

## Microsoft.Phone.UserData.ContactPhoneNumber

Contains information about a phone number associated with a contact.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the data sources associated with this phone number. <br>Returns: The data sources associated with this phone number. |
| `Zind` | The kind of phone number, such as home or work. <br>Returns: One of the enumeration values that specifies the kind of phone number this is. |
| `PhoneNumber` | Gets the phone number. <br>Returns: The phone number. |

### Methods

| Member | Description |
|---|---|
| `ToString` | Returns a string that represents the current phone number. <br>Returns: A string that represents the current phone number. |

## Microsoft.Phone.UserData.Contacts

Provides methods and events for interacting with a user’s contact data.

### Properties / fields

| Member | Description |
|---|---|
| `Accounts` | Gets the accounts from which contacts are available. <br>Returns: The accounts from which contacts are available. |

### Events

| Member | Description |
|---|---|
| `SearchCompleted` | Occurs when a search for contacts completes. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `Contacts` class. |
| `SearchAsync(String,Microsoft.Phone.UserData.FilterKind,Object)` | Asynchronously searches for contacts in the user’s contact data. <br>`filter`: The filter to use to search for contacts. <br>`filterKind`: The kind of filter to use when searching for contacts. <br>`state`: A user-defined object that contains information about the operation. |

## Microsoft.Phone.UserData.ContactsSearchEventArgs

Provides data for the `SearchCompleted` event.

### Properties / fields

| Member | Description |
|---|---|
| `Filter` | Gets the filter that was used for a search. <br>Returns: The filter that was used for the search. |
| `FilterKind` | Gets the kind of filter that was used for the search. <br>Returns: The kind of filter that was used for a search. |
| `Results` | Gets the results of a search for contacts. <br>Returns: The results of the search. |
| `State` | Gets a user-defined object that contains information about the operation. <br>Returns: The user-defined object. |

## Microsoft.Phone.UserData.EmailAddressKind

Specifies information about an email address.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Other` | A different kind of email address, such as school, or the kind of email address is unknown. |
| `Personal` | A personal email address. |
| `Work` | A work email address. |

## Microsoft.Phone.UserData.FilterKind

Specifies the kind of filter to use when searching for contacts.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `DisplayName` | A filter that returns contacts by display name. |
| `EmailAddress` | A filter that returns contacts by email address. |
| `None` | A filter that returns all contacts. |
| `PhoneNumber` | A filter that returns contacts by phone number. |
| `PinnedToStart` | A filter that returns all contacts that are pinned to the Start screen. |

## Microsoft.Phone.UserData.PhoneDataSharingContext

The base class for data sharing contexts such as `Contacts` and `Appointments`.

### Properties / fields

| Member | Description |
|---|---|
| `BaseUri` | Gets the string used to indicate the data context. <br>Returns: The string used to indicate the data context. |

### Methods

| Member | Description |
|---|---|
| `Constructor(String)` | Initializes a new instance of the `PhoneDataSharingContext` class. <br>`baseUri`: The string used to indicate the data context. |
| `ExecuteRequest``1(String)` | Performs a search for objects in a data context. <br>`query`: The search to perform. <br>Returns: The results of the search if possible; otherwise, null. |

## Microsoft.Phone.UserData.PhoneNumberKind

Specifies information about a phone number.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Company` | A company phone number. |
| `Home` | A home phone number. |
| `HomeFax` | A home fax number. |
| `Mobile` | A mobile phone number. |
| `Pager` | A pager phone number. |
| `Work` | A work phone number. |
| `WorkFax` | A work fax number. |

## Microsoft.Phone.UserData.StorageKind

Specifies the possible sources of user contact and calendar data.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Facebook` | Data comes from a Facebook account. |
| `Other` | Data comes from some other location. |
| `Outlook` | Data comes from a Microsoft Outlook account. |
| `Phone` | Data comes from the phone itself. |
| `WindowsLive` | Data comes from a Windows Live account. |

## System.Data.Common.DbException

The exception that is thrown by Windows Phone structured data storage operations that involve COM interop.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DbException` class. |
| `Constructor(String)` | Initializes a new instance of the `DbException` class with the specified error message. <br>`message`: The message that describes the error. |
| `Constructor(String,Exception)` | Initializes a new instance of the `DbException` class with the specified error message and a reference to the inner exception that is the cause of this exception. <br>`message`: The message that describes the error. <br>`innerException`: The exception that is the cause of the current exception. |
| `Constructor(String,Int32)` | Initializes a new instance of the `DbException` class with the specified error message and error code. <br>`message`: The message that describes the error. <br>`errorCode`: The HRESULT of the error. |

## System.Data.DataException

The exception that is thrown by local database operations that result in error.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the `DataException` class. |
| `Constructor(String)` | Initializes a new instance of the `DataException` class with the specified error message. <br>`message`: The message that describes the error. |
| `Constructor(String,Exception)` | Initializes a new instance of the `DataException` class with the specified error message and a reference to the inner exception that is the cause of this exception. <br>`message`: The message that describes the error. <br>`innerException`: The exception that is the cause of the current exception. |

## System.Diagnostics.Stopwatch

Provides a set of methods and properties that you can use to accurately measure elapsed time.

### Properties / fields

| Member | Description |
|---|---|
| `Elapsed` | Gets the total elapsed time measured by the current instance. <br>Returns: Returns `TimeSpan`. |
| `ElapsedMilliseconds` | Gets the total elapsed time measured by the current instance, in milliseconds. <br>Returns: Returns `Int64`. |
| `ElapsedTicks` | Gets the total elapsed time measured by the current instance, in timer ticks. <br>Returns: Returns `Int64`. |
| `IsRunning` | Gets a value indicating whether the Stopwatch timer is running. <br>Returns: Returns `Boolean`.true if the Stopwatch instance is currently running and measuring elapsed time for an interval; otherwise, false. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Frequency` | Gets the frequency of the timer as the number of ticks per second. This field is read-only. |
| `IsHighResolution` | Indicates whether the timer is based on a high-resolution performance counter. This field is read-only. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `Stopwatch` class. |
| `GetElapsedDateTimeTicks` | Gets the number of ticks that represent the date and time of this instance. <br>Returns: Returns `Int64`. |
| `GetRawElapsedTicks` | Gets the total elapsed time measured by the current instance, in timer ticks. <br>Returns: Returns `Int64`. |
| `GetTimestamp` | Gets the current number of ticks in the timer mechanism. <br>Returns: Returns `Int64`. |
| `Reset` | Stops time interval measurement and resets the elapsed time to zero. |
| `Start` | Starts, or resumes, measuring elapsed time for an interval. |
| `StartNew` | Initializes a new Stopwatch instance, sets the elapsed time property to zero, and starts measuring elapsed time. <br>Returns: Returns `Stopwatch`. |
| `Stop` | Stops measuring elapsed time for an interval. |

## System.Windows.Automation.Peers.FrameAutomationPeer

Exposes Frame types to UI automation.

### Methods

| Member | Description |
|---|---|
| `Constructor(Frame)` | Initializes a new instance of the FrameAutomationPeer class. <br>`owner`: The Frame to associate with this FrameAutomationPeer. |
| `GetAutomationControlTypeCore` | Gets the control type for the element that is associated with the UI automation peer. <br>Returns: Returns `AutomationControlType`. |
| `GetClassNameCore` | Gets the name of the Frame that is associated with this FrameAutomationPeer. This method is called by GetClassName. <br>Returns: Returns `String`. The control type. |
| `GetNameCore` | Gets the text label of the Frame that is associated with this FrameAutomationPeer. Called by GetName. <br>Returns: Returns `String`. The text label of the element that is associated with this automation peer, if not null a reference; otherwise, the name of the owning frame, or the name of the frame class. |

## System.Windows.Controls.Frame

Represents a control that supports navigation to and from Silverlight pages. The Frame class presented here is only a base class for `PhoneApplicationFrame`.

### Properties / fields

| Member | Description |
|---|---|
| `CacheSize` | Gets or sets the number of pages that can be cached for the frame. <br>Returns: Returns `Int32`. The number of pages that can be cached for the frame. The default value is 10. |
| `CanGoBack` | Gets a value that indicates whether there is at least one entry in the back navigation history. <br>Returns: Returns `Boolean`. True if there is at least one entry in the back navigation history; otherwise, false. |
| `CanGoForward` | Gets a value that indicates whether there is at least one entry in the forward navigation history. <br>Returns: Returns `Boolean`. For Windows Phone this value will always be false as there is no forward navigation stack on the phone |
| `CurrentSource` | Gets the uniform resource identifier (URI) of the content that is currently displayed. <br>Returns: Returns `Uri`. A value that represents the URI of content that is currently displayed. |
| `JournalOwnership` | Gets or sets whether a frame is responsible for managing its own navigation history, or whether it integrates with the Web browser journal. <br>Returns: Returns `JournalOwnership`. A value that specifies whether frame manages its own journal. The default value is Automatic. |
| `Source` | Gets or sets the uniform resource identifier (URI) of the current content or the content that is being navigated to. <br>Returns: Returns `Uri`. A value that represents the URI of the current content or the content that is being navigated to. |
| `UriMapper` | Gets or sets the object to manage converting a uniform resource identifier (URI) to another URI for this frame. <br>Returns: Returns `UriMapperBase`. The object to convert URIs for this frame. |

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `CacheSizeProperty` | Identifies the CacheSize dependency property. |
| `CanGoBackProperty` | Identifies the CanGoBack dependency property. |
| `CanGoForwardProperty` | Identifies the CanGoForward dependency property. |
| `CurrentSourceProperty` | Identifies the CurrentSource dependency property. |
| `JournalOwnershipProperty` | Identifies the JournalOwnership dependency property. |
| `SourceProperty` | Identifies the Source dependency property. |
| `UriMapperProperty` | Identifies the UriMapper dependency property. |

### Events

| Member | Description |
|---|---|
| `FragmentNavigation` | Occurs when navigation to a content fragment begins. |
| `Navigated` | Occurs when the content that is being navigated to has been found and is available. |
| `Navigating` | Occurs when a new navigation is requested. |
| `NavigationFailed` | Occurs when an error is encountered while navigating to the requested content. |
| `NavigationStopped` | Occurs when a navigation is terminated by either calling the StopLoading method, or when a new navigation is requested while the current navigation is in progress. |

### Methods

| Member | Description |
|---|---|
| `GoBack` | Navigates to the most recent entry in the back navigation history, or throws an exception if no entry exists in back navigation. |
| `GoForward` | Navigates to the most recent entry in the forward navigation history, or throws an exception if no entry exists in forward navigation. For Windows Phone this method will always throw an exception because there is no forward navigation stack. |
| `Navigate(Uri)` | Navigates to the content specified by the uniform resource identifier (URI). <br>`source`: A Uri object initialized with the URI for the desired content. <br>Returns: Returns `Boolean`. True if the navigation started successfully; otherwise, false. |
| `OnApplyTemplate` | Builds the visual tree for the Frame when a new template is applied. (Overrides FrameworkElement OnApplyTemplate().) |
| `OnCreateAutomationPeer` | Returns a FrameAutomationPeer for use by the Silverlight automation infrastructure. (Overrides UIElement OnCreateAutomationPeer().) <br>Returns: Returns `AutomationPeer`. A FrameAutomationPeer for the Frame object. |
| `StopLoading` | Stops asynchronous navigations that have not yet been processed. |

## System.Windows.Controls.Page

Encapsulates content that can be navigated to by a Frame. The Page class presented here is only a base class for `PhoneApplicationPage`. The constructor is internal.

### Properties / fields

| Member | Description |
|---|---|
| `NavigationCacheMode` | Gets or sets a value that indicates whether this page is cached and whether it is cached indefinitely. <br>Returns: Returns `NavigationCacheMode`. The value that specifies the caching behavior for this page. |
| `NavigationContext` | Gets an object that contains information about the navigation request. <br>Returns: Returns `NavigationContext`. An object that contains information about the navigation request. |
| `NavigationService` | Gets the service that the host used to navigate to this page. <br>Returns: Returns `NavigationService`. The service the host used to navigate to this page. |
| `Title` | Gets or sets the name for the page. <br>Returns: Returns `String`. The name for the page. |

### Methods

| Member | Description |
|---|---|
| `OnFragmentNavigation(Navigation.FragmentNavigationEventArgs)` | Called when navigating to a fragment on a page. <br>`e`: An object that contains the event data. |
| `OnNavigatedFrom(Navigation.NavigationEventArgs)` | Called when a page is no longer the active page in a frame. <br>`e`: An object that contains the event data. |
| `OnNavigatedTo(Navigation.NavigationEventArgs)` | Called when a page becomes the active page in a frame. <br>`e`: An object that contains the event data. |
| `OnNavigatingFrom(Navigation.NavigatingCancelEventArgs)` | Called just before a page is no longer the active page in a frame. <br>`e`: An object that contains the event data. |

## System.Windows.Media.Imaging.Extensions

Either encode a WriteableBitmap object into a JPEG stream, or decode a JPEG stream into a WriteableBitmap object.

### Methods

| Member | Description |
|---|---|
| `LoadJpeg(Media.Imaging.WriteableBitmap,IO.Stream)` | Decodes a JPEG stream into a WriteableBitmap object. <br>`bitmap`: The WriteableBitmap object. <br>`sourceStream`: The image data stream. |
| `SaveJpeg(Media.Imaging.WriteableBitmap,IO.Stream,Int32,Int32,Int32,Int32)` | Encodes a WriteableBitmap object into a JPEG stream, with parameters for setting the target width and height of the JPEG file. <br>`bitmap`: The WriteableBitmap object. <br>`targetStream`: The image data stream. <br>`targetWidth`: The target width of the file. <br>`targetHeight`: The target height of the file. <br>`orientation`: This parameter is not currently used by this method. Use a value of 0 as a placeholder. <br>`quality`: This parameter represents the quality of the JPEG photo with a range between 0 and 100, with 100 being the best photo quality. We recommend that you do not fall lower than a value of 70. because JPEG picture quality diminishes significantly below that level. |

## System.Windows.Navigation.FragmentNavigationEventArgs

Provides data for the OnFragmentNavigation method and FragmentNavigation event.

### Properties / fields

| Member | Description |
|---|---|
| `Fragment` | Gets the uniform resource identifier (URI) fragment. <br>Returns: Returns `String`. The URI fragment. |

## System.Windows.Navigation.FragmentNavigationEventHandler

Represents the method that will handle the FragmentNavigation event.

## System.Windows.Navigation.JournalEntry

Represents an entry in either back or forward navigation history.

### Properties / fields

| Member | Description |
|---|---|
| `Source` | Gets or sets the URI of the content that was navigated to. <br>Returns: Returns `Uri`. The URI of the content that was navigated to, or a null reference if no URI is associated with the entry. |

## System.Windows.Navigation.JournalEntryRemovedEventArgs

Provides data when removing journal entries from the back navigation history.

### Properties / fields

| Member | Description |
|---|---|
| `Entry` | Gets the journal entry of the item being removed. <br>Returns: Returns `JournalEntry`. Journal entry. |

## System.Windows.Navigation.JournalOwnership

Used to specify the type of journal that should be used by the Frame.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Automatic` | Indicates that the Frame should first determine its position in the visual tree. If it is at top level then this option is equivalent to `UsesParentJournal`. If it is not at top level then this option is equivalent to `OwnsJournal`. |
| `OwnsJournal` | Indicates that the Frame should keep its own journal and not integrate with the browser. This option can be set on any Frame. |
| `UsesParentJournal` | Indicates that the Frame should integrate with the browser journal. If the Frame is not a top-level Frame then this is not valid option and an exception will be thrown. |

## System.Windows.Navigation.NavigationCacheMode

Specifies how the page is cached when used within a frame.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `Disabled` | The page is never cached and a new instance of the page is created on each visit. |
| `Enabled` | The page is cached, but the cached instance is discarded when the size of the cache for the frame is exceeded. |
| `Required` | The page is cached and the cached instance is reused for every visit regardless of the cache size for the frame. |

## System.Windows.Navigation.NavigationContext

Represents the state of a navigation operation.

### Properties / fields

| Member | Description |
|---|---|
| `QueryString` | Gets a collection of query string values. <br>Returns: Returns `IDictionary`2`. A collection that contains the query string values. |

## System.Windows.Navigation.NavigationFailedEventArgs

Provides data for the NavigationFailed event of the NavigationService class and the NavigationFailed event of the Frame class.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Gets the error from the failed navigation. <br>Returns: Returns `Exception`. A value that represents the error. |
| `Handled` | Gets or sets a value that indicates whether the failure event has been handled. <br>Returns: Returns `Boolean`. True if the event has been handled; otherwise, false. |
| `Uri` | Gets the uniform resource identifier (URI) for the content that could not be navigated to. <br>Returns: Returns `Uri`. A value that represents the URI. |

## System.Windows.Navigation.NavigationFailedEventHandler

Represents the method that will handle the NavigationFailed event.

## System.Windows.Navigation.NavigationService

Provides methods, properties, and events to support navigation within a Silverlight application.

### Properties / fields

| Member | Description |
|---|---|
| `BackStack` | Gets an IEnumerable that you use to enumerate the entries in back navigation history. <br>Returns: Returns `IEnumerable`1`. List of entries in the back stack. |
| `CanGoBack` | Gets a value that indicates whether there is at least one entry in the back navigation history. <br>Returns: Returns `Boolean`. True if there is at least one entry in the back navigation history; otherwise, false. |
| `CanGoForward` | Gets a value that indicates whether there is at least one entry in the forward navigation history. <br>Returns: Returns `Boolean`. True if there is at least one entry in the forward navigation history; otherwise, false. |
| `CurrentSource` | Gets the uniform resource identifier (URI) of the content that is currently displayed. <br>Returns: Returns `Uri`. A value that represents the URI of content that is currently displayed. |
| `Source` | Gets or sets the uniform resource identifier (URI) of the current content or the content that is being navigated to. <br>Returns: Returns `Uri`. A value that represents the URI of the current content or the content that is being navigated to. |

### Events

| Member | Description |
|---|---|
| `FragmentNavigation` | Occurs when navigation to a content fragment begins. |
| `JournalEntryRemoved` | This event is raised during a `RemoveBackEntry` operation or during a normal back navigation after the `Navigated` event has been raised. |
| `Navigated` | Occurs when the content that is being navigated to has been found and is available. |
| `Navigating` | Occurs when a new navigation is requested. |
| `NavigationFailed` | Occurs when an error is encountered while navigating to the requested content. |
| `NavigationStopped` | Occurs when the StopLoading method is called, or when a new navigation is requested while the current navigation is in progress. |

### Methods

| Member | Description |
|---|---|
| `GoBack` | Navigates to the most recent entry in the back navigation history, or throws an exception if no entry exists in back navigation. |
| `GoForward` | Navigates to the most recent entry in the forward navigation history, or throws an exception if no entry exists in forward navigation. For Windows Phone this method will always throw an exception because there is no forward navigation stack. |
| `Navigate(Uri)` | Navigates to the content specified by the uniform resource identifier (URI). <br>`source`: The URI of the content to navigate to. <br>Returns: Returns `Boolean`. True if the navigation started successfully; otherwise, false. |
| `RemoveBackEntry` | This method is used to remove the most recent entry from the back stack, or throws an InvalidOperationException if there are no more entries to remove. If you want to remove more than one item, you call this method multiple times. This API is synchronous and must be called from the UI thread. <br>Returns: Returns `JournalEntry`. The list of `JournalEntry` items. |
| `StopLoading` | Stops asynchronous navigations that have not yet been processed. |

## System.Windows.Navigation.NavigationStoppedEventHandler

Represents the method that will handle the NavigationStopped event.

## System.Windows.Navigation.UriMapper

Converts a uniform resource identifier (URI) into a new URI based on the rules of a matching object specified in a collection of mapping objects.

### Properties / fields

| Member | Description |
|---|---|
| `UriMappings` | Gets a collection of objects that are used to convert a uniform resource identifier (URI) into a new URI. <br>Returns: Returns `Collection`1`. A collection of objects that are used to convert URI values. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the UriMapper class. |
| `MapUri(Uri)` | Converts a specified uniform resource identifier (URI) into a new URI based on the rules of a matching object in the UriMappings collection. <br>`uri`: Original URI value to be converted to a new URI. <br>Returns: Returns `Uri`. A URI to use for handling the request instead of the value of the uri parameter. If no object in the UriMappings collection matches uri, the original value for uri is returned. |

## System.Windows.Navigation.UriMapperBase

Represents the base class for classes that convert a requested uniform resource identifier (URI) into a new URI based on mapping rules.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the UriMapperBase class. |
| `MapUri(Uri)` | When overridden in a derived class, converts a requested uniform resource identifier (URI) to a new URI. <br>`uri`: The original URI value to be mapped to a new URI. <br>Returns: Returns `Uri`. A URI to use for the request instead of the value in the uri parameter. |

## System.Windows.Navigation.UriMapping

Defines the pattern for converting a requested uniform resource identifier (URI) into a new URI.

### Properties / fields

| Member | Description |
|---|---|
| `MappedUri` | Gets or sets the uniform resource identifier (URI) that is navigated to instead of the originally requested URI. <br>Returns: Returns `Uri`. The URI that the requested URI is converted to. |
| `Uri` | Gets or sets the pattern to match when determining if the requested uniform resource identifier (URI) is converted to a mapped URI. <br>Returns: Returns `Uri`. The URI that will be converted. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Initializes a new instance of the UriMapping class. |
| `MapUri(Uri)` | Converts the specified uniform resource identifier (URI) to a new URI, if the specified URI matches the defined template for converting. <br>`uri`: The URI to convert. <br>Returns: Returns `Uri`. The URI that has been converted, or a null reference if the URI cannot be converted. |
