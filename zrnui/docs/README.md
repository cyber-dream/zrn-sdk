# zrnui

zrnui is a C UI library inspired by Windows Phone 8.1 XAML controls. It provides
elements, controls, layout, input, animation, theming and a Wayland host.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Options:
- `ZRNUI_BUILD_SHARED` — build as shared library (default OFF)
- `ZRNUI_BUILD_TESTS` — build tests (default ON when top-level)
- `ZRNUI_WITH_GL` — OpenGL ES 2 renderer
- `ZRNUI_WITH_WAYLAND` — Wayland window (`Zwindow_run`)

Dependencies: freetype2, libpng, fontconfig (optional), wayland-client, xkbcommon, egl/glesv2 (optional).

## Usage

Include `zrnui.h` and link against `zrnui`. Create a window with `Zwindow_run` or
use the software renderer. Controls are created with `Z...` factories and attached
to a parent element.

## Fonts

Font search order:
1. Exact Segoe files in `ZRNUI_FONT_DIR` / `$ZRNUI_FONT_DIR`.
2. System Segoe via fontconfig.
3. Stand-in files in the same directory.

Bundled Selawik fonts are used as stand-ins.

## Navigation

`Zframe_navigate` manages a page stack. Back/forward reuse cached pages when
available; otherwise rebuild. `drop_cached` releases entries ahead.

## Memory

Manual memory management. Pages may be cached for navigation; destroyed when
evicted or on invalid input. No reference counting.