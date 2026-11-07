# Windows Phone 8 widget API reference

Merged from the Windows Phone 8 SDK IntelliSense XML (`wpsdk_en.cab`, preferred) and the Windows Phone Toolkit sources (github.com/microsoftarchive/WindowsPhoneToolkit). When a type or member exists in both, the SDK description wins; Toolkit only adds what the SDK lacks. Each type and member is tagged with its source: **SDK** or **TK**.



- [Silverlight controls (System.Windows.Controls)](10-silverlight-controls.md) — 116 types
- [Phone controls (Microsoft.Phone.Controls, incl. Toolkit)](20-phone-controls.md) — 86 types
- [Phone control primitives](25-phone-primitives.md) — 18 types
- [Shell: ApplicationBar, SystemTray, tiles](30-shell.md) — 24 types
- [Maps](60-maps.md) — 78 types
- [Other](70-other.md) — 10 types
- [Page transitions](90-transitions.md) — 20 types
- [Microsoft.Phone (non-widget services: tasks, info, net, notification, media…)](80-phone-services.md) — 158 types
- [Microsoft.Devices.Sensors](81-sensors.md) — 17 types
- [System.Device (location)](82-location.md) — 13 types
- [Microsoft.Phone.Reactive / System.Observable](83-reactive.md) — 50 types
- [System.Windows (core types: FrameworkElement, UIElement, Visual states, input, media…)](05-silverlight-core.md) — 550 types

## Non-API material

- [themes.md](themes.md) — WP8 theme constants: accent colors, Dark/Light colors, font sizes, margins, text styles.
- `theme-xaml/` — original WP8 theme resources and control templates (`wp8_*`), plus WP 7.1 `generic.xaml` with full Pivot/Panorama/ApplicationBar templates (`wp71_*`).
- `appbar-icons/dark`, `appbar-icons/light` — 48x48 ApplicationBar icons (PNG).
- `intellisense-xml/` — the raw IntelliSense XML from the WP8 SDK that the Markdown files are generated from.

Not included: the SDK/Toolkit sources, and the Segoe WP fonts (Microsoft-licensed binaries).
