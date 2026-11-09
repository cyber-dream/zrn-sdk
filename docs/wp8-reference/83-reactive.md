# Microsoft.Phone.Reactive / System.Observable

From the WP8 SDK IntelliSense XML.

## Contents

- [`Microsoft.Phone.Reactive.AsyncLock`](#microsoftphonereactiveasynclock)
- [`Microsoft.Phone.Reactive.AsyncSubject`1`](#microsoftphonereactiveasyncsubject1)
- [`Microsoft.Phone.Reactive.BehaviorSubject`1`](#microsoftphonereactivebehaviorsubject1)
- [`Microsoft.Phone.Reactive.BooleanDisposable`](#microsoftphonereactivebooleandisposable)
- [`Microsoft.Phone.Reactive.CompositeDisposable`](#microsoftphonereactivecompositedisposable)
- [`Microsoft.Phone.Reactive.ConnectableObservable`1`](#microsoftphonereactiveconnectableobservable1)
- [`Microsoft.Phone.Reactive.ContextDisposable`](#microsoftphonereactivecontextdisposable)
- [`Microsoft.Phone.Reactive.CurrentThreadScheduler`](#microsoftphonereactivecurrentthreadscheduler)
- [`Microsoft.Phone.Reactive.DispatcherObservableExtensions`](#microsoftphonereactivedispatcherobservableextensions)
- [`Microsoft.Phone.Reactive.DispatcherScheduler`](#microsoftphonereactivedispatcherscheduler)
- [`Microsoft.Phone.Reactive.Disposable`](#microsoftphonereactivedisposable)
- [`Microsoft.Phone.Reactive.Event`](#microsoftphonereactiveevent)
- [`Microsoft.Phone.Reactive.ExceptionExtensions`](#microsoftphonereactiveexceptionextensions)
- [`Microsoft.Phone.Reactive.IConnectableObservable`1`](#microsoftphonereactiveiconnectableobservable1)
- [`Microsoft.Phone.Reactive.IEvent`1`](#microsoftphonereactiveievent1)
- [`Microsoft.Phone.Reactive.IGroupedObservable`2`](#microsoftphonereactiveigroupedobservable2)
- [`Microsoft.Phone.Reactive.IObserver`2`](#microsoftphonereactiveiobserver2)
- [`Microsoft.Phone.Reactive.IScheduler`](#microsoftphonereactiveischeduler)
- [`Microsoft.Phone.Reactive.ISubject`1`](#microsoftphonereactiveisubject1)
- [`Microsoft.Phone.Reactive.ISubject`2`](#microsoftphonereactiveisubject2)
- [`Microsoft.Phone.Reactive.ImmediateScheduler`](#microsoftphonereactiveimmediatescheduler)
- [`Microsoft.Phone.Reactive.MutableDisposable`](#microsoftphonereactivemutabledisposable)
- [`Microsoft.Phone.Reactive.NewThreadScheduler`](#microsoftphonereactivenewthreadscheduler)
- [`Microsoft.Phone.Reactive.Notification`](#microsoftphonereactivenotification)
- [`Microsoft.Phone.Reactive.NotificationKind`](#microsoftphonereactivenotificationkind)
- [`Microsoft.Phone.Reactive.Notification`1`](#microsoftphonereactivenotification1)
- [`Microsoft.Phone.Reactive.Notification`1.OnCompleted`](#microsoftphonereactivenotification1oncompleted)
- [`Microsoft.Phone.Reactive.Notification`1.OnError`](#microsoftphonereactivenotification1onerror)
- [`Microsoft.Phone.Reactive.Notification`1.OnNext`](#microsoftphonereactivenotification1onnext)
- [`Microsoft.Phone.Reactive.Observable`](#microsoftphonereactiveobservable)
- [`Microsoft.Phone.Reactive.ObservableExtensions`](#microsoftphonereactiveobservableextensions)
- [`Microsoft.Phone.Reactive.Observer`](#microsoftphonereactiveobserver)
- [`Microsoft.Phone.Reactive.Pattern`](#microsoftphonereactivepattern)
- [`Microsoft.Phone.Reactive.Pattern`2`](#microsoftphonereactivepattern2)
- [`Microsoft.Phone.Reactive.Pattern`3`](#microsoftphonereactivepattern3)
- [`Microsoft.Phone.Reactive.Pattern`4`](#microsoftphonereactivepattern4)
- [`Microsoft.Phone.Reactive.Plan`1`](#microsoftphonereactiveplan1)
- [`Microsoft.Phone.Reactive.RefCountDisposable`](#microsoftphonereactiverefcountdisposable)
- [`Microsoft.Phone.Reactive.ReplaySubject`1`](#microsoftphonereactivereplaysubject1)
- [`Microsoft.Phone.Reactive.ScheduledDisposable`](#microsoftphonereactivescheduleddisposable)
- [`Microsoft.Phone.Reactive.Scheduler`](#microsoftphonereactivescheduler)
- [`Microsoft.Phone.Reactive.Subject`1`](#microsoftphonereactivesubject1)
- [`Microsoft.Phone.Reactive.SynchronizationContextScheduler`](#microsoftphonereactivesynchronizationcontextscheduler)
- [`Microsoft.Phone.Reactive.TaskObservableExtensions`](#microsoftphonereactivetaskobservableextensions)
- [`Microsoft.Phone.Reactive.ThreadPoolScheduler`](#microsoftphonereactivethreadpoolscheduler)
- [`Microsoft.Phone.Reactive.TimeInterval`1`](#microsoftphonereactivetimeinterval1)
- [`Microsoft.Phone.Reactive.Timestamped`1`](#microsoftphonereactivetimestamped1)
- [`Microsoft.Phone.Reactive.Unit`](#microsoftphonereactiveunit)
- [`System.IObservable`1`](#systemiobservable1)
- [`System.IObserver`1`](#systemiobserver1)

## Microsoft.Phone.Reactive.AsyncLock

Runs at most one action at a time.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `AsyncLock` class. |
| `Wait(Action)` | Sets an action to be run. |

## Microsoft.Phone.Reactive.AsyncSubject`1

Represents the result of an asynchronous operation.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a subject that can only receive one value and that value is cached for all future observations. |
| `Constructor(Microsoft.Phone.Reactive.IScheduler)` | Creates a subject that can only receive one value and that value is cached for all future observations. |
| `OnCompleted` | Notifies all subscribed observers of the end of the sequence. |
| `OnError(Exception)` | Notifies all subscribed observers with the exception. |
| `OnNext(`0)` | Notifies all subscribed observers with the value. |
| `Subscribe(IObserver{`0})` | Subscribes an observer to the subject. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.BehaviorSubject`1

Represents a value that changes over time.

### Methods

| Member | Description |
|---|---|
| `Constructor(`0)` | Creates a subject that caches its last value and starts with the specified value. |
| `Constructor(`0,Microsoft.Phone.Reactive.IScheduler)` | Creates a subject that caches its last value and starts with the specified value. |

## Microsoft.Phone.Reactive.BooleanDisposable

Represents an IDisposable that can be checked for status.

### Properties / fields

| Member | Description |
|---|---|
| `IsDisposed` | Gets a value indicating whether or not the object is disposed. <br>Returns: Returns `Boolean`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Constructs a new undisposed BooleanDisposable. |
| `Dispose` | Sets the status to Disposed. |

## Microsoft.Phone.Reactive.CompositeDisposable

Represents a group of Disposables that are disposed together.

### Properties / fields

| Member | Description |
|---|---|
| `Count` | Gets the number of disposables contained in the GroupDisposable. <br>Returns: Returns `Int32`. |
| `IsReadOnly` | Always returns false. <br>Returns: Returns `Boolean`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(IDisposable[])` | Constructs a GroupDisposable from a group of disposables. |
| `Add(IDisposable)` | Adds a disposable to the GroupDisposable or disposes the disposable if the GroupDisposable is disposed. |
| `Clear` | Removes and disposes all disposables from the GroupDisposable, but does not dispose the GroupDisposable. |
| `Contains(IDisposable)` | Determines whether the GroupDisposable contains a specific disposable. <br>Returns: Returns `Boolean`. |
| `CopyTo(IDisposable[],Int32)` | Copies the disposables contained in the GroupDisposable to an Array, starting at a particular Array index. |
| `Dispose` | Disposes all disposables in the group and removes them from the group. |
| `GetEnumerator` | Returns an enumerator that iterates through the GroupDisposable. <br>Returns: Returns `IEnumerator`1`. |
| `Remove(IDisposable)` | Removes and disposes the first occurrence of a disposable from the GroupDisposable. <br>Returns: Returns `Boolean`. |
| `System#Collections#IEnumerable#GetEnumerator` | Returns an enumerator that iterates through the GroupDisposable. <br>Returns: Returns IEnumerator. |

## Microsoft.Phone.Reactive.ConnectableObservable`1

Represents an observable that can be connected and disconnected from its source.

### Methods

| Member | Description |
|---|---|
| `Constructor(IObservable{`0})` | Creates an observable that can be connected and disconnected from its source. |
| `Constructor(IObservable{`0},Microsoft.Phone.Reactive.ISubject{`0})` | Creates an observable that can be connected and disconnected from its source. |
| `Connect` | Connects the observable to its source. <br>Returns: Returns `IDisposable`. |
| `Subscribe(IObserver{`0})` | Subscribes an observer to the observable sequence. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.ContextDisposable

Represents a thread-affine IDisposable.

### Properties / fields

| Member | Description |
|---|---|
| `Context` | Gets the provided SynchronizationContext. <br>Returns: Returns `SynchronizationContext`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Threading.SynchronizationContext,IDisposable)` | Constructs a ContextDisposable that uses a SynchronziationContext on which to dipose the disposable. |
| `Dispose` | Disposes the wrapped disposable on the provided SynchronizationContext. |

## Microsoft.Phone.Reactive.CurrentThreadScheduler

Represents an object that schedules units of work on the current thread.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `EnsureTrampoline(Action)` | Ensures action is surrounded by a trampoline. |
| `Schedule(Action)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.DispatcherObservableExtensions

Provides a set of static methods for subscribing to IObservables using Dispatchers.

### Methods

| Member | Description |
|---|---|
| `ObserveOn``1(IObservable{``0},Threading.Dispatcher)` | Asynchronously notify observers using the dispatcher. <br>Returns: Returns `IObservable`1`. |
| `SubscribeOn``1(IObservable{``0},Threading.Dispatcher)` | Asynchronously subscribes and unsubscribes observers using the dispatcher. <br>Returns: Returns `IObservable`1`. |

## Microsoft.Phone.Reactive.DispatcherScheduler

Represents an object that schedules units of work on a Dispatcher.

### Properties / fields

| Member | Description |
|---|---|
| `Dispatcher` | Gets the dispatcher associated with the DispatcherScheduler. <br>Returns: Returns `Dispatcher`. |
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Threading.Dispatcher)` | Constructs an DispatcherScheduler that schedules units of work on dispatcher. |
| `Schedule(Action)` | Schedules action to be executed on the dispatcher. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed on the dispatcher after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.Disposable

Provides a set of static methods for creating Disposables.

### Properties / fields

| Member | Description |
|---|---|
| `Empty` | Represents the disposable that does nothing when disposed. <br>Returns: Returns `IDisposable`. |

### Methods

| Member | Description |
|---|---|
| `Create(Action)` | Creates the disposable that invokes dispose when disposed. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.Event

Provides a set of static methods for creating events.

### Methods

| Member | Description |
|---|---|
| `Create``1(Object,``0)` | Creates an instance of the IEvent interface. <br>Returns: Returns `IEvent`1`. |

## Microsoft.Phone.Reactive.ExceptionExtensions

Provides a set of static methods for exceptions.

### Methods

| Member | Description |
|---|---|
| `PrepareForRethrow(Exception)` | Preserve callstack when rethrowing. <br>Returns: Returns `Exception`. |

## Microsoft.Phone.Reactive.IConnectableObservable`1

Represents an observable that can be connected and disconnected.

### Methods

| Member | Description |
|---|---|
| `Connect` | Connects the observable. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.IEvent`1

Represents the Sender and EventArg values of a .NET event.

### Properties / fields

| Member | Description |
|---|---|
| `EventArgs` | Gets the event arguments value of the event. |
| `Sender` | Gets the sender value of the event. <br>Returns: Returns `Object`. |

## Microsoft.Phone.Reactive.IGroupedObservable`2

Represents an observable sequence of values that have a common key.

### Properties / fields

| Member | Description |
|---|---|
| `Zey` | Gets the common key. |

## Microsoft.Phone.Reactive.IObserver`2

Supports push-style iteration over an observable sequence.

### Methods

| Member | Description |
|---|---|
| `OnCompleted` | Notifies the observer of the end of the sequence. <br>Returns: Returns `{0}`. |
| `OnError(Exception)` | Notifies the observer that an exception has occurred. <br>Returns: Returns `{0}`. |
| `OnNext(`0)` | Notifies the observer of a new value in the sequence. <br>Returns: Returns `{0}`. |

## Microsoft.Phone.Reactive.IScheduler

Represents an object that schedules units of work.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Schedule(Action)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.ISubject`1

Represents an object that is both an observable sequence as well as an observer.

## Microsoft.Phone.Reactive.ISubject`2

Represents an object that is both an observable sequence as well as an observer.

## Microsoft.Phone.Reactive.ImmediateScheduler

Represents an object that schedules units of work to run immediately on the current thread.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Schedule(Action)` | Schedules action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.MutableDisposable

Represents a disposable whose underlying disposable can be swapped for another disposable.

### Properties / fields

| Member | Description |
|---|---|
| `Disposable` | Gets a value indicating whether the MutableDisposable has an underlying disposable. <br>Returns: Returns `IDisposable`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Constructs a new MutableDisposable with no current underlying disposable. |
| `Dispose` | Disposes the underlying disposable as well as all future replacements. |

## Microsoft.Phone.Reactive.NewThreadScheduler

Represents an object that schedules units of work on the current thread.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a new instance of the `NewThreadScheduler` class. |
| `Schedule(Action)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.Notification

Represents a message sent to an observer.

### Methods

| Member | Description |
|---|---|
| `ToObservable``1(Microsoft.Phone.Reactive.Notification{``0})` | Converts the `Notification` to an `Observable`. <br>Returns: Returns `IObservable`1`. |
| `ToObservable``1(Microsoft.Phone.Reactive.Notification{``0},Microsoft.Phone.Reactive.IScheduler)` | Converts the `Notification` to an `Observable`. <br>Returns: Returns `IObservable`1`. |

## Microsoft.Phone.Reactive.NotificationKind

Enumerates the kinds of notifications that may be invoked.

### Fields (constants, dependency properties)

| Member | Description |
|---|---|
| `OnCompleted` | An OnCompleted notification. |
| `OnError` | An OnError notification. |
| `OnNext` | An OnNext notification. |

## Microsoft.Phone.Reactive.Notification`1

Represents a notification to an observer.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Returns the exception of an OnError notification or returns null. <br>Returns: Returns `Exception`. |
| `HasValue` | Returns whether the notification has a value. <br>Returns: Returns `Boolean`. |
| `Zind` | Gets the kind of notification that is represented. <br>Returns: Returns `NotificationKind`. |
| `Value` | Returns the value of an OnNext notification or throws an exception. |

### Methods

| Member | Description |
|---|---|
| `Accept(Action{`0},Action{Exception},Action)` | Invokes the delegate corresponding to the notification. |
| `Accept(IObserver{`0})` | Invokes the observer's method corresponding to the notification. |
| `Accept``1(Microsoft.Phone.Reactive.IObserver{`0,``0})` | Invokes the observer's method corresponding to the notification and returns the produced result. <br>Returns: N/A |
| `Accept``1(Func{`0,``0},Func{Exception,``0},Func{``0})` | Invokes the delegate corresponding to the notification and returns the produced result. <br>Returns: N/A |
| `Equals(Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether this instance and other are equal. <br>Returns: Returns `Boolean`. |
| `Equals(Object)` | Indicates whether this instance and a specified object are equal. <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this instance. <br>Returns: Returns `Int32`. |
| `op_Equality(Microsoft.Phone.Reactive.Notification{`0},Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether left and right are equal. <br>Returns: Returns `Boolean`. |
| `op_Inequality(Microsoft.Phone.Reactive.Notification{`0},Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether left and right are not equal. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Reactive.Notification`1.OnCompleted

Represents a OnCompleted notification to an observer.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Returns null. <br>Returns: Returns `Exception`. |
| `HasValue` | Returns false. <br>Returns: Returns `Boolean`. |
| `Zind` | Returns `OnCompleted`. <br>Returns: Returns `NotificationKind`. |
| `Value` | Throws an InvalidOperationException. |

### Methods

| Member | Description |
|---|---|
| `Constructor` | Constructs a notification of the end of a sequence. |
| `Accept(Action{`0},Action{Exception},Action)` | Invokes the observer’s method or delegate corresponding to the notification. |
| `Accept(IObserver{`0})` | Invokes the observer's method corresponding to the notification. (Inherited from `Notification`1`) |
| `Accept``1(Microsoft.Phone.Reactive.IObserver{`0,``0})` | Invokes the observer's method corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Accept``1(Func{`0,``0},Func{Exception,``0},Func{``0})` | Invokes the delegate corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Equals(Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether this instance and other are equal. (Inherited from `Notification`1`) <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this instance. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of this instance. <br>Returns: Returns `String`. |

## Microsoft.Phone.Reactive.Notification`1.OnError

Represents an OnError notification to an observer.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Returns the exception. <br>Returns: Returns `Exception`. |
| `HasValue` | Returns false. <br>Returns: Returns `Boolean`. |
| `Zind` | Returns `OnError`. <br>Returns: Returns `NotificationKind`. |
| `Value` | Throws the exception. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Exception)` | Constructs a notification of an exception. |
| `Accept(Action{`0},Action{Exception},Action)` | Invokes the delegate corresponding to the notification. (Inherited from `Notification`1`) |
| `Accept(IObserver{`0})` | Invokes the observer's method corresponding to the notification. (Inherited from `Notification`1`) |
| `Accept``1(Microsoft.Phone.Reactive.IObserver{`0,``0})` | Invokes the observer's method corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Accept``1(Func{`0,``0},Func{Exception,``0},Func{``0})` | Invokes the delegate corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Equals(Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether this instance and other are equal <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this instance. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of this instance. <br>Returns: Returns `String`. |

## Microsoft.Phone.Reactive.Notification`1.OnNext

Represents an OnNext notification to an observer.

### Properties / fields

| Member | Description |
|---|---|
| `Exception` | Returns null. <br>Returns: Returns `Exception`. |
| `HasValue` | Returns true. <br>Returns: Returns `Boolean`. |
| `Zind` | Returns `OnNext`. <br>Returns: Returns `NotificationKind`. |
| `Value` | Returns `OnNext`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(`0)` | Constructs a notification of a new value. |
| `Accept(Action{`0},Action{Exception},Action)` | Invokes the delegate corresponding to the notification. (Inherited from `Notification`1`) |
| `Accept(IObserver{`0})` | Invokes the observer's method corresponding to the notification. (Inherited from `Notification`1`) |
| `Accept``1(Microsoft.Phone.Reactive.IObserver{`0,``0})` | Invokes the observer's method corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Accept``1(Func{`0,``0},Func{Exception,``0},Func{``0})` | Invokes the delegate corresponding to the notification and returns the produced result. (Inherited from `Notification`1`) <br>Returns: Returns `{0}`. |
| `Equals(Microsoft.Phone.Reactive.Notification{`0})` | Indicates whether this instance and another are equal. <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this instance. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of this instance. <br>Returns: Returns `String`. |

## Microsoft.Phone.Reactive.Observable

Provides a set of static methods for query operations over observable sequences.

### Methods

| Member | Description |
|---|---|
| `Aggregate``1(IObservable{``0},Func{``0,``0,``0})` | Applies an accumulator function over an observable sequence. The specified seed value is used as the initial accumulator value. <br>Returns: Returns `IObservable`1`. |
| `Aggregate``2(IObservable{``0},``1,Func{``1,``0,``1})` | Applies an accumulator function over an observable sequence. The specified seed value is used as the initial accumulator value. <br>Returns: Returns `IObservable`1`. |
| `All``1(IObservable{``0},Func{``0,Boolean})` | Determines whether all values of an observable sequence satisfy a condition. <br>Returns: Returns `IObservable`1`. |
| `Amb``1(IEnumerable{IObservable{``0}})` | Returns the observable sequence that reacts first. <br>Returns: Returns `IObservable`1`. |
| `Amb``1(IObservable{``0},IObservable{``0})` | Returns the observable sequence that reacts first. <br>Returns: Returns `IObservable`1`. |
| `Amb``1(IObservable{``0}[])` | Returns the observable sequence that reacts first. <br>Returns: Returns `IObservable`1`. |
| `And``2(IObservable{``0},IObservable{``1})` | Matches when both observable sequences have an available value. <br>Returns: Returns `Pattern`2`. |
| `Any``1(IObservable{``0})` | Determines whether an observable sequence contains any values. <br>Returns: Returns `IObservable`1`. |
| `Any``1(IObservable{``0},Func{``0,Boolean})` | Determines whether an observable sequence contains any values. <br>Returns: Returns `IObservable`1`. |
| `AsObservable``1(IObservable{``0})` | Hides the identity of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Decimal})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Double})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Int32})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Int64})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Nullable{Decimal}})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Nullable{Double}})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Nullable{Int32}})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Nullable{Int64}})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Nullable{Single}})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `Average(IObservable{Single})` | Computes the average of an observable sequence of values. <br>Returns: Returns `IObservable`1`. |
| `BufferWithCount``1(IObservable{``0},Int32)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `BufferWithCount``1(IObservable{``0},Int32,Int32)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTimeOrCount``1(IObservable{``0},TimeSpan,Int32)` | Projects each value of an observable sequence into a buffer that's sent out when either it's full or a given amount of time has elapsed. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTimeOrCount``1(IObservable{``0},TimeSpan,Int32,Microsoft.Phone.Reactive.IScheduler)` | Projects each value of an observable sequence into a buffer that's sent out when either it's full or a given amount of time has elapsed. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTime``1(IObservable{``0},TimeSpan)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTime``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTime``1(IObservable{``0},TimeSpan,TimeSpan)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `BufferWithTime``1(IObservable{``0},TimeSpan,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Projects each value of an observable sequence into a buffer. <br>Returns: Returns `IObservable`1`. |
| `Case``2(Func{``0},IDictionary{``0,IObservable{``1}})` | Uses selector to determine which source in sources to use, empty if no match is found. <br>Returns: Returns `IObservable`1`. |
| `Case``2(Func{``0},IDictionary{``0,IObservable{``1}},Microsoft.Phone.Reactive.IScheduler)` | Uses selector to determine which source in sources to use, empty if no match is found. <br>Returns: Returns `IObservable`1`. |
| `Case``2(Func{``0},IDictionary{``0,IObservable{``1}},IObservable{``1})` | Uses selector to determine which source in sources to use, uses the specified default source if no match is found. <br>Returns: Returns `IObservable`1`. |
| `Cast``1(IObservable{Object})` | Casts values to the given type. <br>Returns: Returns `IObservable`1`. |
| `Catch``1(IEnumerable{IObservable{``0}})` | Continues an observable sequence that is terminated by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Catch``1(IObservable{``0},IObservable{``0})` | Continues an observable sequence that is terminated by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Catch``1(IObservable{``0}[])` | Continues an observable sequence that is terminated by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Catch``2(IObservable{``0},Func{``1,IObservable{``0}})` | Continues an observable sequence that is terminated by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `CombineLatest``3(IObservable{``0},IObservable{``1},Func{``0,``1,``2})` | Merges two observable sequences into one observable sequence by using the selector function whenever one of the observable sequences has a new value. <br>Returns: Returns `IObservable`1`. |
| `Concat``1(IEnumerable{IObservable{``0}})` | Concatenates all the observable sequences. <br>Returns: Returns `IObservable`1`. |
| `Concat``1(IObservable{``0},IObservable{``0})` | Concatenates two observable sequences. <br>Returns: Returns `IObservable`1`. |
| `Concat``1(IObservable{``0}[])` | Concatenates all the observable sequences. <br>Returns: Returns `IObservable`1`. |
| `Contains``1(IObservable{``0},``0)` | Determines whether an observable sequence contains a specified value by using the default comparer. <br>Returns: Returns `IObservable`1`. |
| `Contains``1(IObservable{``0},``0,IEqualityComparer{``0})` | Determines whether an observable sequence contains a specified value by using the specified comparer. <br>Returns: Returns `IObservable`1`. |
| `Count``1(IObservable{``0})` | Returns the number of elements in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `CreateWithDisposable``1(Func{IObserver{``0},IDisposable})` | Creates an observable sequence from the subscribe implementation. <br>Returns: Returns `IObservable`1`. |
| `Create``1(Func{IObserver{``0},Action})` | Creates an observable sequence from the subscribe implementation. <br>Returns: Returns `IObservable`1`. |
| `Defer``1(Func{IObservable{``0}})` | Returns an observable sequence that invokes the observableFactory function whenever a new observer subscribes. <br>Returns: Returns `IObservable`1`. |
| `Delay``1(IObservable{``0},DateTimeOffset)` | Time shifts the observable sequence by the specified time span. The relative time intervals between the values are preserved. <br>Returns: Returns `IObservable`1`. |
| `Delay``1(IObservable{``0},DateTimeOffset,Microsoft.Phone.Reactive.IScheduler)` | Time shifts the observable sequence by the specified time span. The relative time intervals between the values are preserved. <br>Returns: Returns `IObservable`1`. |
| `Delay``1(IObservable{``0},TimeSpan)` | Time shifts the observable sequence by the specified time span. The relative time intervals between the values are preserved. <br>Returns: Returns `IObservable`1`. |
| `Delay``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Time shifts the observable sequence by the specified time span. The relative time intervals between the values are preserved. <br>Returns: Returns `IObservable`1`. |
| `Dematerialize``1(IObservable{Microsoft.Phone.Reactive.Notification{``0}})` | Dematerializes the explicit notification values of an observable sequence as implicit notifications. <br>Returns: Returns `IObservable`1`. |
| `DistinctUntilChanged``1(IObservable{``0})` | Returns an observable sequence that contains only distinct contiguous values. <br>Returns: Returns `IObservable`1`. |
| `DistinctUntilChanged``1(IObservable{``0},IEqualityComparer{``0})` | Returns an observable sequence that contains only distinct contiguous values according to the comparer. <br>Returns: Returns `IObservable`1`. |
| `DistinctUntilChanged``2(IObservable{``0},Func{``0,``1})` | Returns an observable sequence that contains only distinct contiguous values according to the keySelector. <br>Returns: Returns `IObservable`1`. |
| `DistinctUntilChanged``2(IObservable{``0},Func{``0,``1},IEqualityComparer{``1})` | Returns an observable sequence that contains only distinct contiguous values according to the keySelector and comparer. <br>Returns: Returns `IObservable`1`. |
| `DoWhile``1(Func{Boolean},IObservable{``0})` | Repeats source as long as condition holds. <br>Returns: Returns `IObservable`1`. |
| `Do``1(IObservable{``0},Action{``0})` | Invokes the action for its side-effects on each value in the observable sequence <br>Returns: Returns `IObservable`1`. |
| `Do``1(IObservable{``0},Action{``0},Action)` | Invokes the action for its side-effects on each value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Do``1(IObservable{``0},Action{``0},Action{Exception})` | Invokes the action for its side-effects on each value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Do``1(IObservable{``0},Action{``0},Action{Exception},Action)` | Invokes the action for its side-effects on each value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Do``1(IObservable{``0},IObserver{``0})` | Invokes the action for its side-effects on each value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Empty``1` | Returns an empty observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Empty``1(Microsoft.Phone.Reactive.IScheduler)` | Returns an empty observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Finally``1(IObservable{``0},Action)` | Invokes finallyAction after source observable sequence terminates normally or by an exception. <br>Returns: Returns `IObservable`1`. |
| `FirstOrDefault``1(IObservable{``0})` | Returns the first value of an observable sequence, or a default value if no value is found. <br>Returns: Returns `IObservable`1`. |
| `First``1(IObservable{``0})` | Returns the first value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `For``2(IEnumerable{``0},Func{``0,IObservable{``1}})` | Concatenates the observable sequences obtained by running the specified result selector for each element in source. <br>Returns: Returns `IObservable`1`. |
| `ForkJoin``1(IEnumerable{IObservable{``0}})` | Runs all observable sequences in parallel and combines their last values. <br>Returns: Returns `IObservable`1`. |
| `ForkJoin``1(IObservable{``0}[])` | Runs all observable sequences in parallel and combines their first values. <br>Returns: Returns `IObservable`1`. |
| `ForkJoin``3(IObservable{``0},IObservable{``1},Func{``0,``1,``2})` | Runs all observable sequences in parallel and combines their last values. <br>Returns: Returns `IObservable`1`. |
| `FromAsyncPattern(Func{AsyncCallback,Object,IAsyncResult},Action{IAsyncResult})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `FromAsyncPattern``1(Func{AsyncCallback,Object,IAsyncResult},Func{IAsyncResult,``0})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `FromAsyncPattern``1(Func{``0,AsyncCallback,Object,IAsyncResult},Action{IAsyncResult})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `FromAsyncPattern``2(Func{``0,AsyncCallback,Object,IAsyncResult},Func{IAsyncResult,``1})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `FromAsyncPattern``2(Func{``0,``1,AsyncCallback,Object,IAsyncResult},Action{IAsyncResult})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `FromAsyncPattern``3(Func{``0,``1,AsyncCallback,Object,IAsyncResult},Func{IAsyncResult,``2})` | Converts a Begin/End invoke function pair into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `FromEvent``1(Action{EventHandler{``0}},Action{EventHandler{``0}})` | Returns an observable sequence that contains the values of the underlying .NET event. <br>Returns: Returns `IObservable`1`. |
| `FromEvent``1(Object,String)` | Returns an observable sequence that contains the values of the underlying .NET event. <br>Returns: Returns `IObservable`1`. |
| `FromEvent``2(Func{EventHandler{``1},``0},Action{``0},Action{``0})` | Returns an observable sequence that contains the values of the underlying .NET event. <br>Returns: Returns `IObservable`1`. |
| `GenerateWithTime``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,DateTimeOffset},Func{``0,``0})` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `GenerateWithTime``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,DateTimeOffset},Func{``0,``0},Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `GenerateWithTime``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,TimeSpan},Func{``0,``0})` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `GenerateWithTime``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,TimeSpan},Func{``0,``0},Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `Generate``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,``0})` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `Generate``2(``0,Func{``0,Boolean},Func{``0,``1},Func{``0,``0},Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence by iterating a state from an initial state until the condition fails. <br>Returns: Returns `IObservable`1`. |
| `GetEnumerator``1(IObservable{``0})` | Returns an enumerator that enumerates all values of the observable sequence. <br>Returns: Returns IEnumerator. |
| `GroupBy``2(IObservable{``0},Func{``0,``1})` | Groups the elements of an observable sequence according to a specified key selector function. <br>Returns: Returns `IObservable`1`. |
| `GroupBy``2(IObservable{``0},Func{``0,``1},IEqualityComparer{``1})` | Groups the elements of an observable sequence according to a specified key selector function and comparer. <br>Returns: Returns `IObservable`1`. |
| `GroupBy``3(IObservable{``0},Func{``0,``1},Func{``0,``2})` | Groups the elements of an observable sequence and selects the resulting elements by using a specified function. <br>Returns: Returns `IObservable`1`. |
| `GroupBy``3(IObservable{``0},Func{``0,``1},Func{``0,``2},IEqualityComparer{``1})` | Groups the elements of an observable sequence according to a specified key selector function and comparer and selects the resulting elements by using a specified function. <br>Returns: Returns `IObservable`1`. |
| `If``1(Func{Boolean},IObservable{``0},IObservable{``0})` | Determines if an observable collection contains values. <br>Returns: Returns `IObservable`1`. |
| `Interval(TimeSpan)` | Returns an observable sequence that produces a value after each period. <br>Returns: Returns `IObservable`1`. |
| `Interval(TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that produces a value after each period. <br>Returns: Returns `IObservable`1`. |
| `IsEmpty``1(IObservable{``0})` | Determines if an observable collection is empty. <br>Returns: Returns `IObservable`1`. |
| `Join``1(Microsoft.Phone.Reactive.Plan{``0}[])` | Joins together the results from several patterns. <br>Returns: Returns `IObservable`1`. |
| `Join``1(IEnumerable{Microsoft.Phone.Reactive.Plan{``0}})` | Joins together the results from several patterns. <br>Returns: Returns `IObservable`1`. |
| `LastOrDefault``1(IObservable{``0})` | Returns the last value of an observable sequence, or a default value if no value is found. <br>Returns: Returns `IObservable`1`. |
| `Last``1(IObservable{``0})` | Returns the last value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Latest``1(IObservable{``0})` | Samples the most recent value (buffer of size one with consumption) in an observable sequence. <br>Returns: Returns IEnumerable. |
| `Let``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}})` | Bind the source to the parameter without sharing subscription side-effects. <br>Returns: Returns `IObservable`1`. |
| `Let``2(``0,Func{``0,IObservable{``1}})` | Bind the source to the parameter without sharing subscription side-effects. <br>Returns: Returns `IObservable`1`. |
| `LongCount``1(IObservable{``0})` | Returns an Int64 that represents the total number of values in an observable sequence. Returns an Int64 that represents the total number of values in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Materialize``1(IObservable{``0})` | Materializes the implicit notifications of an observable sequence as explicit notification values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Decimal})` | Returns the maximum value in an observable sequence of System.Decimal. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Double})` | Returns the maximum value in an observable sequence of System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Int32})` | Returns the maximum value in an observable sequence of System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Int64})` | Returns the maximum value in an observable sequence of System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Nullable{Decimal}})` | Returns the maximum value in an observable sequence of nullable System.Decimal values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Nullable{Double}})` | Returns the maximum value in an observable sequence of nullable System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Nullable{Int32}})` | Returns the maximum value in an observable sequence of nullable System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Nullable{Int64}})` | Returns the maximum value in an observable sequence of nullable System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Nullable{Single}})` | Returns the maximum value in an observable sequence of System.Float values. <br>Returns: Returns `IObservable`1`. |
| `Max(IObservable{Single})` | Returns the maximum value in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `MaxBy``2(IObservable{``0},Func{``0,``1})` | Returns the element in an observable sequence with the maximum key value. <br>Returns: Returns `IObservable`1`. |
| `MaxBy``2(IObservable{``0},Func{``0,``1},IComparer{``1})` | Returns the element in an observable sequence with the maximum key value. <br>Returns: Returns `IObservable`1`. |
| `Max``1(IObservable{``0})` | Returns the maximum value in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Max``1(IObservable{``0},IComparer{``0})` | Returns the maximum value in an observable sequence as determined by the specified comparer function. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(Microsoft.Phone.Reactive.IScheduler,IObservable{``0}[])` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IEnumerable{IObservable{``0}})` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IEnumerable{IObservable{``0}},Microsoft.Phone.Reactive.IScheduler)` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IObservable{IObservable{``0}})` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IObservable{``0},IObservable{``0})` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IObservable{``0},IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Merge``1(IObservable{``0}[])` | Merges an observable sequence of observable sequences into an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Decimal})` | Returns the minimum value in an observable sequence of System.Decimal values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Double})` | Returns the minimum value in an observable sequence of System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Int32})` | Returns the minimum value in an observable sequence of System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Int64})` | Returns the minimum value in an observable sequence of System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Nullable{Decimal}})` | Returns the minimum value in an observable sequence of nullable System.Decimal values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Nullable{Double}})` | Returns the minimum value in an observable sequence of nullable System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Nullable{Int32}})` | Returns the minimum value in an observable sequence of Nullable System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Nullable{Int64}})` | Returns the minimum value in an observable sequence of nullable System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Nullable{Single}})` | Returns the minimum value in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Min(IObservable{Single})` | Returns the minimum value in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `MinBy``2(IObservable{``0},Func{``0,``1})` | Returns the element in an observable sequence with the minimum key value. <br>Returns: Returns `IObservable`1`. |
| `MinBy``2(IObservable{``0},Func{``0,``1},IComparer{``1})` | Returns the element in an observable sequence with the minimum key value. <br>Returns: Returns `IObservable`1`. |
| `Min``1(IObservable{``0})` | Returns the minimum value in an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Min``1(IObservable{``0},IComparer{``0})` | Returns the minimum value in an observable sequence as determined by the specified comparer function. <br>Returns: Returns `IObservable`1`. |
| `MostRecent``1(IObservable{``0},``0)` | Samples the most recent value (buffer of size one without consumption) in an observable sequence. <br>Returns: Returns IEnumerable. |
| `Never``1` | Returns a non-terminating observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Next``1(IObservable{``0})` | Samples the next value (blocking without buffering) from an observable sequence. <br>Returns: Returns IEnumerable. |
| `ObserveOnDispatcher``1(IObservable{``0})` | Asynchronously notify observers using the current dispatcher. <br>Returns: Returns `IObservable`1`. |
| `ObserveOn``1(IObservable{``0},Microsoft.Phone.Reactive.DispatcherScheduler)` | Asynchronously notify observers using the scheduler. <br>Returns: Returns `IObservable`1`. |
| `ObserveOn``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Asynchronously notify observers using the scheduler. <br>Returns: Returns `IObservable`1`. |
| `ObserveOn``1(IObservable{``0},Threading.SynchronizationContext)` | Asynchronously notify observers on the synchronization context. <br>Returns: Returns `IObservable`1`. |
| `OfType``1(IObservable{Object})` | Filters values of the given type. <br>Returns: Returns `IObservable`1`. |
| `OnErrorResumeNext``1(IEnumerable{IObservable{``0}})` | Continues an observable sequence that is terminated normally or by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `OnErrorResumeNext``1(IObservable{``0},IObservable{``0})` | Continues an observable sequence that is terminated normally or by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `OnErrorResumeNext``1(IObservable{``0}[])` | Continues an observable sequence that is terminated normally or by an exception with the next observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Prune``1(IObservable{``0})` | Returns a connectable observable sequence that shares a single subscription to the underlying source containing only the last notification. <br>Returns: Returns `IConnectableObservable`1`. |
| `Prune``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source containing only the last notification. <br>Returns: Returns `IConnectableObservable`1`. |
| `Prune``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}})` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source containing only the last notification. <br>Returns: Returns `IObservable`1`. |
| `Prune``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source containing only the last notification. <br>Returns: Returns `IObservable`1`. |
| `Publish``1(IObservable{``0})` | Returns a connectable observable sequence that shares a single subscription to the underlying source <br>Returns: Returns `IConnectableObservable`1`. |
| `Publish``1(IObservable{``0},``0)` | Returns a connectable observable sequence that shares a single subscription to the underlying source and starts with initialValue. <br>Returns: Returns `IConnectableObservable`1`. |
| `Publish``1(IObservable{``0},``0,Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source and starts with initialValue. <br>Returns: Returns `IConnectableObservable`1`. |
| `Publish``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}})` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source. <br>Returns: Returns `IObservable`1`. |
| `Publish``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},``0)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source and starts with initialValue. <br>Returns: Returns `IObservable`1`. |
| `Publish``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},``0,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source and starts with initialValue. <br>Returns: Returns `IObservable`1`. |
| `Range(Int32,Int32)` | Generates an observable sequence of integral numbers within a specified range. <br>Returns: Returns `IObservable`1`. |
| `Range(Int32,Int32,Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence of integral numbers within a specified range. <br>Returns: Returns `IObservable`1`. |
| `RefCount``1(Microsoft.Phone.Reactive.IConnectableObservable{``0})` | Returns an observable sequence that stays connected to the source as long as there is at least one subscription to the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `RemoveTimeInterval``1(IObservable{Microsoft.Phone.Reactive.TimeInterval{``0}})` | Removes the timestamp from each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `RemoveTimestamp``1(IObservable{Microsoft.Phone.Reactive.Timestamped{``0}})` | Removes the timestamp from each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(IObservable{``0})` | Repeats the observable sequence indefinitely. <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(IObservable{``0},Int32)` | Repeats the observable sequence the specified number of times <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(``0)` | Generates an observable sequence that contains one repeated value. <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(``0,Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence that contains one repeated value. <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(``0,Int32)` | Generates an observable sequence that contains one repeated value. <br>Returns: Returns `IObservable`1`. |
| `Repeat``1(``0,Int32,Microsoft.Phone.Reactive.IScheduler)` | Generates an observable sequence that contains one repeated value. <br>Returns: Returns `IObservable`1`. |
| `Replay``1(IObservable{``0})` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},Int32)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},Int32,Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications within window. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},Int32,TimeSpan)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications within window. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},Int32,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications within window. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},TimeSpan)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications within window. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications within window. <br>Returns: Returns `IConnectableObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}})` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Int32)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Int32,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Int32,TimeSpan)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications within window. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},Int32,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying bufferSize notifications within window. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},TimeSpan)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications within window. <br>Returns: Returns `IObservable`1`. |
| `Replay``2(IObservable{``0},Func{IObservable{``0},IObservable{``1}},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that is the result of invoking the selector on a connectable observable sequence that shares a single subscription to the underlying source replaying all notifications within window. <br>Returns: Returns `IObservable`1`. |
| `Retry``1(IObservable{``0})` | Repeats the source observable sequence until it successfully terminates. <br>Returns: Returns `IObservable`1`. |
| `Retry``1(IObservable{``0},Int32)` | Repeats the source observable sequence the retryCount times or until it successfully terminates. <br>Returns: Returns `IObservable`1`. |
| `Return``1(``0)` | Returns an observable sequence that contains a single value. <br>Returns: Returns `IObservable`1`. |
| `Return``1(``0,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that contains a single value. <br>Returns: Returns `IObservable`1`. |
| `Run``1(IObservable{``0})` | Invokes the observable sequence for its side-effects and blocks till the sequence is terminated. |
| `Run``1(IObservable{``0},Action{``0})` | Invokes the action for its side-effects on each value in the observable sequence and blocks till the sequence is terminated. |
| `Run``1(IObservable{``0},Action{``0},Action)` | Invokes the action for its side-effects on each value in the observable sequence and blocks till the sequence is terminated. |
| `Run``1(IObservable{``0},Action{``0},Action{Exception})` | Invokes the action for its side-effects on each value in the observable sequence and blocks till the sequence is terminated. |
| `Run``1(IObservable{``0},Action{``0},Action{Exception},Action)` | Invokes the action for its side-effects on each value in the observable sequence and blocks till the sequence is terminated. |
| `Run``1(IObservable{``0},IObserver{``0})` | Invokes the action for its side-effects on each value in the observable sequence and blocks till the sequence is terminated. |
| `Sample``1(IObservable{``0},TimeSpan)` | Samples the observable sequence at each interval. <br>Returns: Returns `IObservable`1`. |
| `Sample``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Samples the observable sequence at each interval. <br>Returns: Returns `IObservable`1`. |
| `Scan0``2(IObservable{``0},``1,Func{``1,``0,``1})` | Applies an accumulator function over an observable sequence and returns each intermediate result. The specified seed value is prepended to the sequence once a message comes in. <br>Returns: Returns `IObservable`1`. |
| `Scan``1(IObservable{``0},Func{``0,``0,``0})` | Applies an accumulator function over an observable sequence and returns each intermediate result. <br>Returns: Returns `IObservable`1`. |
| `Scan``2(IObservable{``0},``1,Func{``1,``0,``1})` | Applies an accumulator function over an observable sequence and returns each intermediate result. The specified seed value is used as the initial accumulator value. <br>Returns: Returns `IObservable`1`. |
| `SelectMany``2(IObservable{``0},Func{``0,IEnumerable{``1}})` | Projects each value of an observable sequence to an observable sequence and flattens the resulting observable sequences into one observable sequence. <br>Returns: Returns `IObservable`1`. |
| `SelectMany``2(IObservable{``0},Func{``0,IObservable{``1}})` | Projects each value of an observable sequence to an observable sequence and flattens the resulting observable sequences into one observable sequence. <br>Returns: Returns `IObservable`1`. |
| `SelectMany``2(IObservable{``0},Func{``0,IObservable{``1}},Func{Exception,IObservable{``1}},Func{IObservable{``1}})` | Projects each value of an observable sequence to an observable sequence, flattens the resulting observable sequences into one observable sequence, and invokes a result selector function on each value therein. <br>Returns: Returns `IObservable`1`. |
| `SelectMany``2(IObservable{``0},IObservable{``1})` | Projects each value of an observable sequence to an observable sequence and flattens the resulting observable sequences into one observable sequence. <br>Returns: Returns `IObservable`1`. |
| `SelectMany``3(IObservable{``0},Func{``0,IObservable{``1}},Func{``0,``1,``2})` | Projects each value of an observable sequence to an observable sequence, flattens the resulting observable sequences into one observable sequence, and invokes a result selector function on each value therein. <br>Returns: Returns `IObservable`1`. |
| `Select``2(IObservable{``0},Func{``0,``1})` | Projects each value of an observable sequence into a new form. <br>Returns: Returns `IObservable`1`. |
| `Select``2(IObservable{``0},Func{``0,Int32,``1})` | Projects each value of an observable sequence into a new form by incorporating the element's index. <br>Returns: Returns `IObservable`1`. |
| `SingleOrDefault``1(IObservable{``0})` | Returns the only value of an observable sequence, or a default value if the observable sequence is empty; this method throws an exception if there is more than one value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Single``1(IObservable{``0})` | Returns the only value of an observable sequence, and throws an exception if there is not exactly one value in the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `SkipLast``1(IObservable{``0},Int32)` | Bypasses a specified number of values at the end of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `SkipUntil``2(IObservable{``0},IObservable{``1})` | Returns the values from the source observable sequence only after the other observable sequence produces a value. <br>Returns: Returns `IObservable`1`. |
| `SkipWhile``1(IObservable{``0},Func{``0,Boolean})` | Bypasses values in an observable sequence as long as a specified condition is true and then returns the remaining values. <br>Returns: Returns `IObservable`1`. |
| `Skip``1(IObservable{``0},Int32)` | Bypasses a specified number of values in an observable sequence and then returns the remaining values. <br>Returns: Returns `IObservable`1`. |
| `Start(Action)` | Invokes the action asynchronously. <br>Returns: Returns `IObservable`1`. |
| `Start(Action,Microsoft.Phone.Reactive.IScheduler)` | Invokes the action asynchronously. <br>Returns: Returns `IObservable`1`. |
| `StartWith``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler,``0[])` | Prepends a sequence values to an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `StartWith``1(IObservable{``0},``0[])` | Prepends a sequence values to an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Start``1(Func{``0})` | Invokes the function asynchronously. <br>Returns: Returns `IObservable`1`. |
| `Start``1(Func{``0},Microsoft.Phone.Reactive.IScheduler)` | Invokes the function asynchronously. <br>Returns: Returns `IObservable`1`. |
| `SubscribeOnDispatcher``1(IObservable{``0})` | Asynchronously subscribes and unsubscribes observers using the current dispatcher. <br>Returns: Returns `IObservable`1`. |
| `SubscribeOn``1(IObservable{``0},Microsoft.Phone.Reactive.DispatcherScheduler)` | Asynchronously subscribes and unsubscribes observers using the scheduler. <br>Returns: Returns `IObservable`1`. |
| `SubscribeOn``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Asynchronously subscribes and unsubscribes observers using the scheduler. <br>Returns: Returns `IObservable`1`. |
| `SubscribeOn``1(IObservable{``0},Threading.SynchronizationContext)` | Asynchronously subscribes and unsubscribes observers on the synchronization context. <br>Returns: Returns `IObservable`1`. |
| `Subscribe``1(IEnumerable{``0},IObserver{``0})` | Subscribes an observer to an enumerable sequence. Returns an object that can be used to unsubscribe the observer from the enumerable. <br>Returns: Returns `IDisposable`. |
| `Subscribe``1(IEnumerable{``0},IObserver{``0},Microsoft.Phone.Reactive.IScheduler)` | Subscribes an observer to an enumerable sequence. Returns an object that can be used to unsubscribe the observer from the enumerable. <br>Returns: Returns `IDisposable`. |
| `Sum(IObservable{Decimal})` | Computes the sum of a sequence of System.Decimal values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Double})` | Computes the sum of a sequence of System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Int32})` | Computes the sum of a sequence of System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Int64})` | Computes the sum of a sequence of System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Nullable{Decimal}})` | Computes the sum of a sequence of nullable System.Decimal values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Nullable{Double}})` | Computes the sum of a sequence of nullable System.Double values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Nullable{Int32}})` | Computes the sum of a sequence of nullable System.Int32 values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Nullable{Int64}})` | Computes the sum of a sequence of nullable System.Int64 values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Nullable{Single}})` | Computes the sum of a sequence of nullable System.Float values. <br>Returns: Returns `IObservable`1`. |
| `Sum(IObservable{Single})` | Computes the sum of a sequence of System.Float values. <br>Returns: Returns `IObservable`1`. |
| `Switch``1(IObservable{IObservable{``0}})` | Transforms an observable sequence of observable sequences into an observable sequence producing values only from the most recent observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Synchronize``1(IObservable{``0})` | Synchronizes the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Synchronize``1(IObservable{``0},Object)` | Synchronizes the observable sequence. <br>Returns: Returns `IObservable`1`. |
| `TakeLast``1(IObservable{``0},Int32)` | Returns a specified number of contiguous values from the end of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `TakeUntil``2(IObservable{``0},IObservable{``1})` | Returns the values from the source observable sequence until the other observable sequence produces a value. <br>Returns: Returns `IObservable`1`. |
| `TakeWhile``1(IObservable{``0},Func{``0,Boolean})` | Returns values from an observable sequence as long as a specified condition is true, and then skips the remaining values. <br>Returns: Returns `IObservable`1`. |
| `Take``1(IObservable{``0},Int32)` | Returns a specified number of contiguous values from the start of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Take``1(IObservable{``0},Int32,Microsoft.Phone.Reactive.IScheduler)` | Returns a specified number of contiguous values from the start of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Then``2(IObservable{``0},Func{``0,``1})` | Matches when the observable sequence has an available value and projects the value. <br>Returns: Returns `Plan`1`. |
| `Throttle``1(IObservable{``0},TimeSpan)` | Ignores values from an observable sequence which are followed by another value before the specified time span. <br>Returns: Returns `IObservable`1`. |
| `Throttle``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Ignores values from an observable sequence which are followed by another value before the specified time span. <br>Returns: Returns `IObservable`1`. |
| `Throw``1(Exception)` | Returns an observable sequence that terminates with an exception. <br>Returns: Returns `IObservable`1`. |
| `Throw``1(Exception,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that terminates with an exception. <br>Returns: Returns `IObservable`1`. |
| `TimeInterval``1(IObservable{``0})` | Records the time interval for each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `TimeInterval``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Records the time interval for each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},DateTimeOffset)` | Returns either the observable sequence or an TimeoutException if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},DateTimeOffset,Microsoft.Phone.Reactive.IScheduler)` | Returns either the observable sequence or an TimeoutException if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},DateTimeOffset,IObservable{``0})` | Returns the source observable sequence or the other observable sequence if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},DateTimeOffset,IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Returns the source observable sequence or the other observable sequence if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},TimeSpan)` | Returns either the observable sequence or an TimeoutException if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns either the observable sequence or an TimeoutException if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},TimeSpan,IObservable{``0})` | Returns the source observable sequence or the other observable sequence if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timeout``1(IObservable{``0},TimeSpan,IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Returns the source observable sequence or the other observable sequence if dueTime elapses. <br>Returns: Returns `IObservable`1`. |
| `Timer(DateTimeOffset)` | Returns an observable sequence that produces a value at dueTime. <br>Returns: Returns `IObservable`1`. |
| `Timer(DateTimeOffset,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that produces a value after the dueTime has elapsed. <br>Returns: Returns `IObservable`1`. |
| `Timer(DateTimeOffset,TimeSpan)` | Returns an observable sequence that produces a value at dueTime and then after each period. <br>Returns: Returns `IObservable`1`. |
| `Timer(DateTimeOffset,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that produces a value at dueTime and then after each period. <br>Returns: Returns `IObservable`1`. |
| `Timer(TimeSpan)` | Returns an observable sequence that produces a value after the dueTime has elapsed. <br>Returns: Returns `IObservable`1`. |
| `Timer(TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that produces a value after the dueTime has elapsed. <br>Returns: Returns `IObservable`1`. |
| `Timer(TimeSpan,TimeSpan)` | Returns an observable sequence that produces a value after dueTime has elapsed and then after each period. <br>Returns: Returns `IObservable`1`. |
| `Timer(TimeSpan,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Returns an observable sequence that produces a value after dueTime has elapsed and then after each period. <br>Returns: Returns `IObservable`1`. |
| `Timestamp``1(IObservable{``0})` | Records the timestamp for each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Timestamp``1(IObservable{``0},Microsoft.Phone.Reactive.IScheduler)` | Records the timestamp for each value of an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `ToAsync(Action)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `ToAsync(Action,Microsoft.Phone.Reactive.IScheduler)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `ToAsync``1(Action{``0})` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `ToAsync``1(Action{``0},Microsoft.Phone.Reactive.IScheduler)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `ToAsync``1(Func{``0})` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `ToAsync``1(Func{``0},Microsoft.Phone.Reactive.IScheduler)` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`1`. |
| `ToAsync``2(Action{``0,``1})` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `ToAsync``2(Action{``0,``1},Microsoft.Phone.Reactive.IScheduler)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `ToAsync``2(Func{``0,``1})` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `ToAsync``2(Func{``0,``1},Microsoft.Phone.Reactive.IScheduler)` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`2`. |
| `ToAsync``3(Action{``0,``1,``2})` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`4`. |
| `ToAsync``3(Action{``0,``1,``2},Microsoft.Phone.Reactive.IScheduler)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`4`. |
| `ToAsync``3(Func{``0,``1,``2})` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `ToAsync``3(Func{``0,``1,``2},Microsoft.Phone.Reactive.IScheduler)` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`3`. |
| `ToAsync``4(Action{``0,``1,``2,``3})` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`5`. |
| `ToAsync``4(Action{``0,``1,``2,``3},Microsoft.Phone.Reactive.IScheduler)` | Converts the action into an asynchronous function. <br>Returns: Returns `Func`5`. |
| `ToAsync``4(Func{``0,``1,``2,``3})` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`4`. |
| `ToAsync``4(Func{``0,``1,``2,``3},Microsoft.Phone.Reactive.IScheduler)` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`4`. |
| `ToAsync``5(Func{``0,``1,``2,``3,``4})` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`5`. |
| `ToAsync``5(Func{``0,``1,``2,``3,``4},Microsoft.Phone.Reactive.IScheduler)` | Converts the function into an asynchronous function. <br>Returns: Returns `Func`5`. |
| `ToEnumerable``1(IObservable{``0})` | Converts an observable sequence to an enumerable sequence. <br>Returns: Returns IEnumerable. |
| `ToObservable``1(IEnumerable{``0})` | Converts an enumerable sequence to an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `ToObservable``1(IEnumerable{``0},Microsoft.Phone.Reactive.IScheduler)` | Converts an enumerable sequence to an observable sequence. <br>Returns: Returns `IObservable`1`. |
| `Using``2(Func{``1},Func{``1,IObservable{``0}})` | Retrieves resource from resourceSelector for use in resourceUsage and disposes the resource once the resulting observable sequence terminates. <br>Returns: Returns `IObservable`1`. |
| `Where``1(IObservable{``0},Func{``0,Boolean})` | Filters the values of an observable sequence based on a predicate. <br>Returns: Returns `IObservable`1`. |
| `Where``1(IObservable{``0},Func{``0,Int32,Boolean})` | Filters the values of an observable sequence based on a predicate by incorporating the element's index. <br>Returns: Returns `IObservable`1`. |
| `While``1(Func{Boolean},IObservable{``0})` | Repeats source as long as condition holds. <br>Returns: Returns `IObservable`1`. |
| `Zip``3(IObservable{``0},IEnumerable{``1},Func{``0,``1,``2})` | Merges an observable sequence and an enumerable sequence into one observable sequence by using the selector function. <br>Returns: Returns `IObservable`1`. |
| `Zip``3(IObservable{``0},IObservable{``1},Func{``0,``1,``2})` | Merges two observable sequences into one observable sequence by using the selector function. <br>Returns: Returns `IObservable`1`. |

## Microsoft.Phone.Reactive.ObservableExtensions

Provides a set of static methods for subscribing delegates to observables.

### Methods

| Member | Description |
|---|---|
| `Subscribe``1(IObservable{``0})` | Evaluates the observable sequence. <br>Returns: Returns `IDisposable`. |
| `Subscribe``1(IObservable{``0},Action{``0})` | Subscribes a value handler to an observable sequence. <br>Returns: Returns `IDisposable`. |
| `Subscribe``1(IObservable{``0},Action{``0},Action)` | Subscribes a value handler and a completion handler to an observable sequence. <br>Returns: Returns `IDisposable`. |
| `Subscribe``1(IObservable{``0},Action{``0},Action{Exception})` | Subscribes a value handler and an exception handler to an observable sequence. <br>Returns: Returns `IDisposable`. |
| `Subscribe``1(IObservable{``0},Action{``0},Action{Exception},Action)` | Subscribes a value handler, an exception handler, and a completion handler to an observable sequence. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.Observer

Provides a set of static methods for creating observers.

### Methods

| Member | Description |
|---|---|
| `AsObserver``1(IObserver{``0})` | Hides the identity of an observer. <br>Returns: Returns `IObserver`1`. |
| `Create``1(Action{``0})` | Creates an observer from the specified OnNext action. <br>Returns: Returns `IObserver`1`. |
| `Create``1(Action{``0},Action)` | Creates an observer from the specified OnNext and OnCompleted actions. <br>Returns: Returns `IObserver`1`. |
| `Create``1(Action{``0},Action{Exception})` | Creates an observer from the specified OnNext and OnError actions. <br>Returns: Returns `IObserver`1`. |
| `Create``1(Action{``0},Action{Exception},Action)` | Creates an observer from the specified OnNext, OnError, and OnCompleted actions. <br>Returns: Returns `IObserver`1`. |
| `ToNotifier``1(IObserver{``0})` | Creates a notification callback from an observer. <br>Returns: Returns `Action`1`. |
| `ToObserver``1(Action{Microsoft.Phone.Reactive.Notification{``0}})` | Creates an observer from a notification callback. <br>Returns: Returns `IObserver`1`. |

## Microsoft.Phone.Reactive.Pattern

Represents a join pattern.

## Microsoft.Phone.Reactive.Pattern`2

Represents a join pattern.

### Methods

| Member | Description |
|---|---|
| `And``1(IObservable{``0})` | Matches when all observable sequences have an available value. <br>Returns: Returns `Pattern`3`. |
| `Then``1(Func{`0,`1,``0})` | Matches when all observable sequences have an available value and projects the values. <br>Returns: Returns `Plan`1`. |

## Microsoft.Phone.Reactive.Pattern`3

Represents a join pattern.

### Methods

| Member | Description |
|---|---|
| `And``1(IObservable{``0})` | Matches when all observable sequences have an available value. <br>Returns: Returns `Pattern`4`. |
| `Then``1(Func{`0,`1,`2,``0})` | Matches when all observable sequences have an available value and projects the values. <br>Returns: Returns `Plan`1`. |

## Microsoft.Phone.Reactive.Pattern`4

Represents a join pattern.

### Methods

| Member | Description |
|---|---|
| `Then``1(Func{`0,`1,`2,`3,``0})` | Matches when all observable sequences have an available value and projects the values. <br>Returns: Returns `Plan`1`. |

## Microsoft.Phone.Reactive.Plan`1

Represents an execution plan for join patterns.

## Microsoft.Phone.Reactive.RefCountDisposable

Represents a disposable that only disposes its underlying disposable when all dependent disposables have been disposed.

### Methods

| Member | Description |
|---|---|
| `Constructor(IDisposable)` | Creates a disposable that only disposes its underlying disposable when all dependent disposables have been disposed |
| `Dispose` | Disposes the underlying disposable only when all dependent disposables have been disposed. |
| `GetDisposable` | Returns a disposable that when disposed decreases the refcount on the underlying disposable. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.ReplaySubject`1

Represents an object that is both an observable sequence as well as an observer.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a replayable subject. |
| `Constructor(Microsoft.Phone.Reactive.IScheduler)` | Creates a replayable subject. |
| `Constructor(Int32)` | Creates a replayable subject. |
| `Constructor(Int32,Microsoft.Phone.Reactive.IScheduler)` | Creates a replayable subject. |
| `Constructor(Int32,TimeSpan)` | Creates a replayable subject. |
| `Constructor(Int32,TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Creates a replayable subject. |
| `Constructor(TimeSpan)` | Creates a replayable subject. |
| `Constructor(TimeSpan,Microsoft.Phone.Reactive.IScheduler)` | Creates a replayable subject. |
| `OnCompleted` | Notifies all subscribed observers of the end of the sequence. |
| `OnError(Exception)` | Notifies all subscribed observers with the exception. |
| `OnNext(`0)` | Notifies all subscribed observers with the value. |
| `Subscribe(IObserver{`0})` | Subscribes an observer to the subject. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.ScheduledDisposable

Represents an object that schedules units of work on a provided scheduler.

### Properties / fields

| Member | Description |
|---|---|
| `Disposable` | Gets a value indicating the underlying disposable. <br>Returns: Returns `IDisposable`. |
| `Scheduler` | Gets a value indicating the scheduler. <br>Returns: Returns `IScheduler`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Microsoft.Phone.Reactive.IScheduler,IDisposable)` | Constructs a ScheduledDisposable that uses a scheduler on which to dipose the disposable. |
| `Dispose` | Disposes the wrapped disposable on the provided scheduler. |

## Microsoft.Phone.Reactive.Scheduler

Provides a set of static methods for creating Schedulers.

### Properties / fields

| Member | Description |
|---|---|
| `CurrentThread` | Returns a CurrentThreadScheduler object which schedules work for processing as soon as possible. <br>Returns: Returns `CurrentThreadScheduler`. |
| `Dispatcher` | Gets the scheduler that schedules work on the current Dispatcher. <br>Returns: Returns `DispatcherScheduler`. |
| `Immediate` | Returns an ImmediateScheduler object which schedules work for immediate processing on the current thread. <br>Returns: Returns `ImmediateScheduler`. |
| `NewThread` | Gets the scheduler that schedules work on a new thread. <br>Returns: Returns `NewThreadScheduler`. |
| `ThreadPool` | Gets the scheduler that schedules work on the ThreadPool. <br>Returns: Returns `ThreadPoolScheduler`. |

### Methods

| Member | Description |
|---|---|
| `Schedule(Microsoft.Phone.Reactive.IScheduler,Action,DateTimeOffset)` | Schedules an to be executed recursively at time . <br>Returns: Returns `IDisposable`. |
| `Schedule(Microsoft.Phone.Reactive.IScheduler,Action{Action{DateTimeOffset}},DateTimeOffset)` | Schedules action to be executed recursively at each specified time interval . <br>Returns: Returns `IDisposable`. |
| `Schedule(Microsoft.Phone.Reactive.IScheduler,Action{Action{TimeSpan}},TimeSpan)` | Schedules action to be executed recursively after each specified time interval . <br>Returns: Returns `IDisposable`. |
| `Schedule(Microsoft.Phone.Reactive.IScheduler,Action{Action})` | Schedules an to be executed recursively. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.Subject`1

Represents an object that is both an observable sequence as well as an observer.

### Methods

| Member | Description |
|---|---|
| `Constructor` | Creates a Subject. |
| `OnCompleted` | Notifies all subscribed observers of the end of the sequence. |
| `OnError(Exception)` | Notifies all subscribed observers with the exception. |
| `OnNext(`0)` | Notifies all subscribed observers with the value. |
| `Subscribe(IObserver{`0})` | Subscribes an observer to the subject. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.SynchronizationContextScheduler

Represents an object that schedules units of work on a provided SynchronizationContext.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Constructor(Threading.SynchronizationContext)` | Creates an object that schedules units of work on the provided SynchronizationContext. |
| `Schedule(Action)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules action to be executed after the specified time span. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.TaskObservableExtensions

Provides a set of static methods for converting Tasks to IObservables.

## Microsoft.Phone.Reactive.ThreadPoolScheduler

Represents an object that schedules units of work on the threadpool.

### Properties / fields

| Member | Description |
|---|---|
| `Now` | Gets the scheduler's notion of current time. <br>Returns: Returns `DateTimeOffset`. |

### Methods

| Member | Description |
|---|---|
| `Schedule(Action)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |
| `Schedule(Action,TimeSpan)` | Schedules an action to be executed. <br>Returns: Returns `IDisposable`. |

## Microsoft.Phone.Reactive.TimeInterval`1

Represents a time interval value.

### Properties / fields

| Member | Description |
|---|---|
| `Interval` | Gets the interval. <br>Returns: Returns `TimeSpan`. |
| `Value` | Gets the value. |

### Methods

| Member | Description |
|---|---|
| `Constructor(`0,TimeSpan)` | Constructs a time interval value. |
| `Equals(Object)` | Determines if the specified object is equal to the current `TimeInterval`1`. <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this object. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of this object. <br>Returns: Returns `String`. |
| `op_Equality(Microsoft.Phone.Reactive.TimeInterval{`0},Microsoft.Phone.Reactive.TimeInterval{`0})` | Returns true if time interval equals time interval , otherwise returns false. <br>Returns: Returns `Boolean`. |
| `op_Inequality(Microsoft.Phone.Reactive.TimeInterval{`0},Microsoft.Phone.Reactive.TimeInterval{`0})` | Returns true if time interval does not equal time interval , otherwise returns false. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Reactive.Timestamped`1

Represents a timestamped value.

### Properties / fields

| Member | Description |
|---|---|
| `Timestamp` | Gets the timestamp. <br>Returns: Returns `DateTimeOffset`. |
| `Value` | Gets the value. |

### Methods

| Member | Description |
|---|---|
| `Constructor(`0,DateTimeOffset)` | Constructs a timestamped value. |
| `Equals(Object)` | Determines if the specified object is equal to this one. <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Gets the hash code for this object. <br>Returns: Returns `Int32`. |
| `ToString` | Returns a string representation of this object. <br>Returns: Returns `String`. |
| `op_Equality(Microsoft.Phone.Reactive.Timestamped{`0},Microsoft.Phone.Reactive.Timestamped{`0})` | Returns true if timestamped value is equal to timestamped value, false otherwise. <br>Returns: Returns `Boolean`. |
| `op_Inequality(Microsoft.Phone.Reactive.Timestamped{`0},Microsoft.Phone.Reactive.Timestamped{`0})` | Returns true if timestamped value is not equal to timestamped value, false otherwise. <br>Returns: Returns `Boolean`. |

## Microsoft.Phone.Reactive.Unit

Represents void.

### Methods

| Member | Description |
|---|---|
| `Equals(Microsoft.Phone.Reactive.Unit)` | Determines equality of instances. <br>Returns: Returns `Boolean`. |
| `Equals(Object)` | Determines equality of instances. <br>Returns: Returns `Boolean`. |
| `GetHashCode` | Returns the hash code for this object. <br>Returns: Returns `Int32`. |
| `op_Equality(Microsoft.Phone.Reactive.Unit,Microsoft.Phone.Reactive.Unit)` | Always returns true. <br>Returns: Returns `Boolean`. |
| `op_Inequality(Microsoft.Phone.Reactive.Unit,Microsoft.Phone.Reactive.Unit)` | Always returns false. <br>Returns: Returns `Boolean`. |

## System.IObservable`1

Defines a provider for push-based notification.

### Methods

| Member | Description |
|---|---|
| `Subscribe(IObserver{`0})` | Subscribes to receive notifications. <br>`observer`: The object that is to receive notifications. <br>Returns: A reference to an interface that allows observers to stop receiving notifications before the provider has finished sending them. |

## System.IObserver`1

Provides a mechanism for receiving push-based notifications.

### Methods

| Member | Description |
|---|---|
| `OnCompleted` | Notifies the observer of the end of the observable sequence. No further values or notifications will be made available for this sequence. |
| `OnError(Exception)` | Notifies the observer that an exception has occurred in processing the observable sequence. <br>`exception`: An object that provides additional information about the error. |
| `OnNext(`0)` | Notifies the observer of a new value in the observable sequence. <br>`value`: The current notification information. |
