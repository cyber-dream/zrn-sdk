# Page transitions

## Contents

- [`Microsoft.Phone.Controls.FeatheredTransition`](#microsoftphonecontrolsfeatheredtransition)
- [`Microsoft.Phone.Controls.ITransition`](#microsoftphonecontrolsitransition)
- [`Microsoft.Phone.Controls.NavigationInTransition`](#microsoftphonecontrolsnavigationintransition)
- [`Microsoft.Phone.Controls.NavigationOutTransition`](#microsoftphonecontrolsnavigationouttransition)
- [`Microsoft.Phone.Controls.NavigationTransition`](#microsoftphonecontrolsnavigationtransition)
- [`Microsoft.Phone.Controls.RollTransition`](#microsoftphonecontrolsrolltransition)
- [`Microsoft.Phone.Controls.RotateTransition`](#microsoftphonecontrolsrotatetransition)
- [`Microsoft.Phone.Controls.RotateTransitionMode`](#microsoftphonecontrolsrotatetransitionmode)
- [`Microsoft.Phone.Controls.SlideTransition`](#microsoftphonecontrolsslidetransition)
- [`Microsoft.Phone.Controls.SlideTransitionMode`](#microsoftphonecontrolsslidetransitionmode)
- [`Microsoft.Phone.Controls.SwivelTransition`](#microsoftphonecontrolsswiveltransition)
- [`Microsoft.Phone.Controls.SwivelTransitionMode`](#microsoftphonecontrolsswiveltransitionmode)
- [`Microsoft.Phone.Controls.Transition`](#microsoftphonecontrolstransition)
- [`Microsoft.Phone.Controls.TransitionElement`](#microsoftphonecontrolstransitionelement)
- [`Microsoft.Phone.Controls.TransitionFrame`](#microsoftphonecontrolstransitionframe)
- [`Microsoft.Phone.Controls.TransitionService`](#microsoftphonecontrolstransitionservice)
- [`Microsoft.Phone.Controls.TurnstileFeatherTransition`](#microsoftphonecontrolsturnstilefeathertransition)
- [`Microsoft.Phone.Controls.TurnstileFeatherTransitionMode`](#microsoftphonecontrolsturnstilefeathertransitionmode)
- [`Microsoft.Phone.Controls.TurnstileTransition`](#microsoftphonecontrolsturnstiletransition)
- [`Microsoft.Phone.Controls.TurnstileTransitionMode`](#microsoftphonecontrolsturnstiletransitionmode)
- [Storyboard values](#storyboard-values)

## Microsoft.Phone.Controls.FeatheredTransition

Source: Toolkit · class : Transition · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/FeatheredTransition.cs`

Controls an `ITransition` in order to produce a feathered animation on a set of `UIElement`.

### Methods

| Member | Description | Src |
|---|---|---|
| `void Begin()` | Composes the `Storyboard` and mirrors `Begin`. | TK |
| `«ctor» FeatheredTransition(UIElement element, Storyboard storyboard, TurnstileFeatherTransitionMode mode, TimeSpan? beginTime) : base(element, storyboard)` | Constructs a `FeatheredTransition` for a `UIElement` and a `Storyboard`, based on a `TurnstileFeatherTransitionMode` <br>`element`: The `UIElement`. <br>`storyboard`: The `Storyboard`. <br>`mode`: The `TurnstileFeatherTransitionMode`. <br>`beginTime`: The time at which the transition should begin. | TK |

## Microsoft.Phone.Controls.ITransition

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/ITransition.cs`

Controls the behavior of transitions.

## Microsoft.Phone.Controls.NavigationInTransition

Source: Toolkit · class : NavigationTransition · `Microsoft.Phone.Controls.Toolkit/Transitions/NavigationInTransition.cs`

Has navigation-in `TransitionElement`s for the designer experiences.

## Microsoft.Phone.Controls.NavigationOutTransition

Source: Toolkit · class : NavigationTransition · `Microsoft.Phone.Controls.Toolkit/Transitions/NavigationOutTransition.cs`

Has navigation-out `TransitionElement`s for the designer experiences.

## Microsoft.Phone.Controls.NavigationTransition

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Transitions/NavigationTransition.cs`

Has `TransitionElement`s for the designer experiences.

### Properties

| Member | Description | Src |
|---|---|---|
| `TransitionElement Backward` | Gets or sets the backward `NavigationTransition`. | TK |
| `TransitionElement Forward` | Gets or sets the forward `NavigationTransition`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty BackwardProperty` | The `DependencyProperty` for the backward `NavigationTransition`. | TK |
| `static DependencyProperty ForwardProperty` | The `DependencyProperty` for the forward `NavigationTransition`. | TK |

### Events

| Member | Description | Src |
|---|---|---|
| `RoutedEventHandler BeginTransition` | The navigation transition will begin. | TK |
| `RoutedEventHandler EndTransition` | The navigation transition has ended. | TK |

## Microsoft.Phone.Controls.RollTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/RollTransition.cs`

Provides roll `ITransition`s.

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.RotateTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/RotateTransition.cs`

Provides rotate `ITransition`s.

### Properties

| Member | Description | Src |
|---|---|---|
| `RotateTransitionMode Mode` | The `RotateTransitionMode`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ModeProperty` | The `DependencyProperty` for the `RotateTransitionMode`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Saves and clears the existing `RenderTransformProperty` value before the start of the transition, then restores it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.RotateTransitionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/RotateTransitionMode.cs`

The rotate transition modes.

### Values

| Member | Description | Src |
|---|---|---|
| `In90Clockwise` | The rotate in 90 degrees clockwise transition mode. | TK |
| `In90Counterclockwise` | The rotate in 90 degrees counterclockwise transition mode. | TK |
| `In180Clockwise` | The rotate in 180 degrees clockwise transition mode. | TK |
| `In180Counterclockwise` | The rotate in 180 degrees counterclockwise transition mode. | TK |
| `Out90Clockwise` | The rotate out 90 degrees clockwise transition mode. | TK |
| `Out90Counterclockwise` | The rotate out 90 degrees counterclockwise transition mode. | TK |
| `Out180Clockwise` | The rotate out 180 degrees clockwise transition mode. | TK |
| `Out180Counterclockwise` | The rotate out 180 degrees counterclockwise transition mode. | TK |

## Microsoft.Phone.Controls.SlideTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/SlideTransition.cs`

Provides slide `ITransition`s.

### Properties

| Member | Description | Src |
|---|---|---|
| `SlideTransitionMode Mode` | The `SlideTransitionMode`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ModeProperty` | The `DependencyProperty` for the `SlideTransitionMode`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Saves and clears the existing `RenderTransformProperty` value before the start of the transition, then restores it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.SlideTransitionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/SlideTransitionMode.cs`

The slide transition modes.

### Values

| Member | Description | Src |
|---|---|---|
| `SlideUpFadeIn` | The slide up, fade in transition mode. | TK |
| `SlideUpFadeOut` | The slide up, fade out transition mode. | TK |
| `SlideDownFadeIn` | The slide down, fade in transition mode. | TK |
| `SlideDownFadeOut` | The slide down, fade out transition mode. | TK |
| `SlideLeftFadeIn` | The slide left, fade in transition mode. | TK |
| `SlideLeftFadeOut` | The slide left, fade out transition mode. | TK |
| `SlideRightFadeIn` | The slide right, fade in transition mode. | TK |
| `SlideRightFadeOut` | The slide right, fade out transition mode. | TK |

## Microsoft.Phone.Controls.SwivelTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/SwivelTransition.cs`

Provides swivel `ITransition`s.

### Properties

| Member | Description | Src |
|---|---|---|
| `SwivelTransitionMode Mode` | The `SwivelTransitionMode`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ModeProperty` | The `DependencyProperty` for the `SwivelTransitionMode`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Saves and clears the existing `ProjectionProperty` value before the start of the transition, then restores it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.SwivelTransitionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/SwivelTransitionMode.cs`

The swivel transition modes.

### Values

| Member | Description | Src |
|---|---|---|
| `FullScreenIn` | The swivel full screen in transition mode. | TK |
| `FullScreenOut` | The swivel full screen out transition mode. | TK |
| `ForwardIn` | The swivel forward in transition mode. | TK |
| `ForwardOut` | The swivel forward out transition mode. | TK |
| `BackwardIn` | The swivel backward in transition mode. | TK |
| `BackwardOut` | The swivel backward out transition mode. | TK |

## Microsoft.Phone.Controls.Transition

Source: Toolkit · class : ITransition · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/Transition.cs`

Mirrors the `Storyboard` interface to control an `ITransition` for a `UIElement`. Saves and restores the `CacheMode` and `IsHitTestVisible` values for the `UIElement`.

### Events

| Member | Description | Src |
|---|---|---|
| `EventHandler Completed` | Mirrors `Completed`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ClockState GetCurrentState()` | Mirrors `GetCurrentState`. | TK |
| `TimeSpan GetCurrentTime()` | Mirrors `GetCurrentTime`. | TK |
| `void Begin()` | Mirrors `Begin`. | TK |
| `void Pause()` | Mirrors `Pause`. | TK |
| `void Resume()` | Mirrors `Resume`. | TK |
| `void Seek(TimeSpan offset)` | Mirrors `Seek`. <br>`offset`: The time offset. | TK |
| `void SeekAlignedToLastTick(TimeSpan offset)` | Mirrors `SeekAlignedToLastTick`. <br>`offset`: The time offset. | TK |
| `void SkipToFill()` | Mirrors `SkipToFill`. | TK |
| `void Stop()` | Mirrors `Stop`. | TK |
| `«ctor» Transition(UIElement element, Storyboard storyboard)` | Constructs a `Transition` for a `UIElement` and a `Storyboard`. <br>`element`: The `UIElement`. <br>`storyboard`: The `Storyboard`. | TK |

## Microsoft.Phone.Controls.TransitionElement

Source: Toolkit · class : DependencyObject · `Microsoft.Phone.Controls.Toolkit/Transitions/TransitionElement.cs`

Transition factory for a particular transition family.

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Existing `RenderTransformProperty` or `ProjectionProperty` values may be saved and cleared before the start of the transition, then restored it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.TransitionFrame

Source: Toolkit · class : PhoneApplicationFrame · `Microsoft.Phone.Controls.Toolkit/Transitions/TransitionFrame.cs`

Enables navigation transitions for `PhoneApplicationPage`s.

### Methods

| Member | Description | Src |
|---|---|---|
| `void OnApplyTemplate()` | When overridden in a derived class, is invoked whenever application code or internal processes (such as a rebuilding layout pass) call `ApplyTemplate`. In simplest terms, this means the method is called just before a UI element displays in an application. | TK |
| `«ctor» TransitionFrame() : base()` | Initialzies a new instance of the TransitionFrame class. | TK |

## Microsoft.Phone.Controls.TransitionService

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/TransitionService.cs`

Provides attached properties for navigation `ITransition`s.

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty NavigationInTransitionProperty` | The `DependencyProperty` for the in `ITransition`s. | TK |
| `static DependencyProperty NavigationOutTransitionProperty` | The `DependencyProperty` for the in `ITransition`s. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `static NavigationInTransition GetNavigationInTransition(UIElement element)` | Gets the `NavigationTransition`s of `NavigationInTransitionProperty` for a `UIElement`. <br>`element`: The `UIElement`. <br>Returns: The | TK |
| `static NavigationOutTransition GetNavigationOutTransition(UIElement element)` | Gets the `NavigationTransition`s of `NavigationOutTransitionProperty` for a `UIElement`. <br>`element`: The `UIElement`. <br>Returns: The | TK |
| `static void SetNavigationInTransition(UIElement element, NavigationInTransition value)` | Sets a `NavigationTransition` to `NavigationInTransitionProperty` for a `UIElement`. <br>`element`: The `UIElement`. <br>`value`: The `NavigationTransition`. <br>Returns: The | TK |
| `static void SetNavigationOutTransition(UIElement element, NavigationOutTransition value)` | Sets a `NavigationTransition`s to `NavigationOutTransitionProperty` for a `UIElement`. <br>`element`: The `UIElement`. <br>`value`: The `NavigationTransition`. <br>Returns: The | TK |

## Microsoft.Phone.Controls.TurnstileFeatherTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/TurnstileFeatherTransition.cs`

Provides turnstile feather `ITransition`s.

### Properties

| Member | Description | Src |
|---|---|---|
| `TimeSpan? BeginTime` | The time at which the transition should begin. | TK |
| `TurnstileFeatherTransitionMode Mode` | The `TurnstileTransitionMode`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty BeginTimeProperty` | The `DependencyProperty` for the time at which the transition should begin. | TK |
| `static DependencyProperty ModeProperty` | The `DependencyProperty` for the `TurnstileTransitionMode`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Saves and clears the existing `ProjectionProperty` value before the start of the transition, then restores it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.TurnstileFeatherTransitionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/TurnstileFeatherTransitionMode.cs`

The turnstile feather transition modes.

### Values

| Member | Description | Src |
|---|---|---|
| `ForwardIn` | The turnstile feather forward in transition mode. | TK |
| `ForwardOut` | The turnstile feather forward out transition mode. | TK |
| `BackwardIn` | The turnstile feather backward in transition mode. | TK |
| `BackwardOut` | The turnstile feather backward out transition mode. | TK |

## Microsoft.Phone.Controls.TurnstileTransition

Source: Toolkit · class : TransitionElement · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/TurnstileTransition.cs`

Provides turnstile `ITransition`s.

### Properties

| Member | Description | Src |
|---|---|---|
| `TurnstileTransitionMode Mode` | The `TurnstileTransitionMode`. | TK |

### Fields

| Member | Description | Src |
|---|---|---|
| `static DependencyProperty ModeProperty` | The `DependencyProperty` for the `TurnstileTransitionMode`. | TK |

### Methods

| Member | Description | Src |
|---|---|---|
| `ITransition GetTransition(UIElement element)` | Creates a new `ITransition` for a `UIElement`. Saves and clears the existing `ProjectionProperty` value before the start of the transition, then restores it after it is stopped or completed. <br>`element`: The `UIElement`. <br>Returns: The `ITransition`. | TK |

## Microsoft.Phone.Controls.TurnstileTransitionMode

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Transitions/TurnstileTransitionMode.cs`

The turnstile transition modes.

### Values

| Member | Description | Src |
|---|---|---|
| `ForwardIn` | The turnstile forward in transition mode. | TK |
| `ForwardOut` | The turnstile forward out transition mode. | TK |
| `BackwardIn` | The turnstile backward in transition mode. | TK |
| `BackwardOut` | The turnstile backward out transition mode. | TK |

## Storyboard values

Source: Toolkit · `Microsoft.Phone.Controls.Toolkit/Transitions/Storyboards/*.xaml`, `Microsoft.Phone.Controls.Toolkit/Effects/TurnstileFeatherEffect.cs` (microsoftarchive/WindowsPhoneToolkit, `master`)

The numbers of the modes above, which the member tables do not give. Angles are the Toolkit's own (`PlaneProjection.RotationX/Y/Z`); zrnui negates every one of them, because its axes turn the other way round (`zrnui/src/anim.c`, `TDEFS`). Times are in ms, from the first keyframe. An easing is on the keyframe that ends the leg. `Exp n` is `ExponentialEase` with `Exponent` n, `In` or `Out` its `EasingMode`; `Sine out` is `SineEase`. "Opacity step" is `1` until 10 ms before the end and `0` at the end (the element stays opaque while it moves, then goes).

### Slide (`RenderTransform` `TranslateTransform`; the opacity has the same time and easing as the move)

| Mode | Property | From → to | Time | Easing | Opacity |
|---|---|---|---|---|---|
| `SlideUpFadeIn` | Y | 200 → 0 | 350 | Exp 6 out | 0 → 1 |
| `SlideUpFadeOut` | Y | 0 → -200 | 250 | Exp 6 out | 1 → 0 |
| `SlideDownFadeIn` | Y | -200 → 0 | 350 | Exp 6 out | 0 → 1 |
| `SlideDownFadeOut` | Y | 0 → 200 | 250 | Exp 6 out | 1 → 0 |
| `SlideLeftFadeIn` | X | 200 → 0 | 500 | Exp 6 out | 0 → 1 |
| `SlideLeftFadeOut` | X | 0 → -200 | 300 | Exp 6 out | 1 → 0 |
| `SlideRightFadeIn` | X | -200 → 0 | 500 | Exp 6 out | 0 → 1 |
| `SlideRightFadeOut` | X | 0 → 200 | 300 | Exp 6 out | 1 → 0 |

### Turnstile (`PlaneProjection`, `CenterOfRotationX` 0, `RotationY`)

| Mode | From → to | Time | Easing | Opacity |
|---|---|---|---|---|
| `ForwardIn` | -80 → 0 | 350 | Exp 6 out | 0 → 1 in 10 |
| `ForwardOut` | 0 → 50 | 250 | Exp 6 in | step |
| `BackwardIn` | 50 → 0 | 350 | Exp 6 out | 0 → 1 in 10 |
| `BackwardOut` | 0 → -80 | 250 | Exp 6 in | step |

### TurnstileFeather (`TurnstileFeatherEffect`)

Each element of the page that is on the screen turns by itself: `PlaneProjection` with `CenterOfRotationX` -0.2 (a fifth of its width to the left of it), `RotationY`. The n-th element (0 for the first, in `FeatheringIndex` order) starts `delay × n` ms after the transition. The elements of a `ListBox` and `LongListSelector` that are in view count one by one; a `Pivot` counts its title and its headers. Going in, an element is hidden until its own start, then appears at once; going out, it goes at the end of its turn.

| Mode | Angle, from → to | Time | Delay per element | Easing |
|---|---|---|---|---|
| `ForwardIn` | -80 → 0 | 350 | 40 | Exp 6 out |
| `ForwardOut` | 0 → 50 | 250 | 50 | Exp 6 in |
| `BackwardIn` | 50 → 0 | 350 | 50 | Exp 6 out |
| `BackwardOut` | 0 → -80 | 250 | 40 | Exp 6 in |

### Swivel (`PlaneProjection`, `RotationX`, centre of rotation 0.5, 0.5)

| Mode | Angle | Time | Easing | Opacity |
|---|---|---|---|---|
| `ForwardIn` | -45 → 0 | 350 | Exp 6 out | not touched |
| `ForwardOut` | 0 → 90 | 250 | Exp 15 in | step |
| `BackwardIn` | -45 → -30 at 200 (linear) → 0 at 350 | 350 | Exp 6 out (the last leg) | 0 until 200, 1 at 210 |
| `BackwardOut` | 0 → 60 | 250 | Exp 6 in | step |
| `FullScreenIn` | -30 → 0 | 350 | Exp 6 out | not touched |
| `FullScreenOut` | 0 → 30 | 250 | Exp 1 in | step |

### Rotate (`PlaneProjection`, `RotationZ`, centre 0.5, 0.5; 250 ms; the opacity is Sine out)

| Mode | Angle | Easing | Opacity |
|---|---|---|---|
| `In90Clockwise` | 90 → 0 | Exp 1 out | 0 → 1 |
| `In90Counterclockwise` | -90 → 0 | Exp 1 out | 0 → 1 |
| `In180Clockwise` | 180 → 0 | Exp 1 out | 0 → 1 |
| `In180Counterclockwise` | -180 → 0 | Exp 1 out | 0 → 1 |
| `Out90Clockwise` | 0 → -90 | Exp 1 in | 1 → 0 |
| `Out90Counterclockwise` | 0 → 90 | Exp 1 in | 1 → 0 |
| `Out180Clockwise` | 0 → -180 | Exp 1 in | 1 → 0 |
| `Out180Counterclockwise` | 0 → 180 | Exp 1 in | 1 → 0 |

### Roll (`RollTransition`, no modes; `PlaneProjection`, centre 0.5, 0.5, `RotationZ`)

0 → 45 at 300 (Exp 6 out), → 90 at 600 (linear). The opacity is not touched.
