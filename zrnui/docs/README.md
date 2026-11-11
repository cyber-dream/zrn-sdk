# zrnui

zrnui is a C UI library inspired by Windows Phone 8.1 XAML controls. It provides
elements, controls, layout, input, animation and theming. It draws a tree and knows no window:
the window and the app cycle are `zrnapp`, the OS integration layer built on top of it (`../zrnapp`).

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Options:
- `ZRNUI_BUILD_SHARED` — build as shared library (default OFF)
- `ZRNUI_BUILD_TESTS` — build tests (default ON when top-level)
- `ZRNUI_WITH_GL` — OpenGL ES 2 renderer

Dependencies: freetype2, libpng, fontconfig (optional), egl/glesv2 (optional).

## Usage

Include `zrnui.h` and link against `zrnui`. zrnui has no window: a program that embeds it (a shell) passes input and
time in through `Zroot_inject_*` / `Zroot_tick` and takes pixels out through `Zroot_render` or a `ZGlTarget` in its own
OpenGL ES 2 context (`zrnui/platform.h`). An application that is an ordinary Wayland client uses zrnapp (`../zrnapp`). Controls are created with `Z...` factories and attached
to a parent element.

## Fonts

At `Zinit` the font directory (`Zset_font_directory`, `$ZRNUI_FONT_DIR`, then `ZRNUI_FONT_DIR`) and all system
fonts (fontconfig, optional) are registered. The five faces are resolved from that registry in this order:
1. Exact Segoe files in the font directory.
2. System Segoe ("Segoe WP", then "Segoe UI"), nearest weight.
3. Stand-in files in the font directory (Segoe Regular/Bold, then the bundled Selawik).

A code point a face lacks is drawn with the best system font that has it (upright, proportional, nearest weight).

## Navigation

`Zframe_navigate` manages a page stack. Back/forward reuse cached pages when
available; otherwise rebuild. `drop_cached` releases entries ahead.

## Memory

Manual memory management. Pages may be cached for navigation; destroyed when
evicted or on invalid input. No reference counting.