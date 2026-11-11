# zrnui - a wp8.1 UI lib

## Build

From sources:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Arch Linux package:

```sh
makepkg -s
```

## Layers

- **zrnui** (this directory): the UI core. The tree, layout, controls, theme, text and the painters (software, OpenGL ES 2).
  No window, no event loop; the seam to the platform is `include/zrnui/platform.h`.
- **zrnapp** (`../zrnapp`): the app cycle and the OS integration. A Wayland window, the event loop, pointer / touch /
  keyboard into `Zroot_inject_*`, the scale of the shell, EGL for the GPU window. Link `zrnapp::zrnapp` for an
  application; a shell that embeds zrnui links `zrnui::zrnui` alone.

`CMAKE_BUILD_TYPE=Debug` (or `-DZRNUI_DEBUG=ON`) defines `Z_DEBUG` on the `zrnui` target and on every target
that links it. Debug features are compiled under `#ifdef Z_DEBUG` (`Z_DEBUG_ONLY(...)`, `Z_DEBUG_LOG(...)`
in `zrnui_core.h`) and are not in a release binary.

## Fonts

`Zinit` scans the font directory and, with fontconfig, every system font, and resolves the five faces from
that inventory; nothing is searched for while text is drawn (a Debug build prints what was found).
The font directory is `Zset_font_directory`, then `$ZRNUI_FONT_DIR`, then the compiled-in `ZRNUI_FONT_DIR`
(`fonts/selawik` of the sources).

## Environment

| variable | effect |
|---|---|
| `ZRNUI_SCALE` | (read by zrnapp) overrides the UI scale (debug only): the layout grid is the window / scale; without it the `preferred_scale` of the compositor is used |
| `ZRNUI_ANIMATION_SPEED` | multiplier of the speed of every animation (0.5 half as fast, 2 twice as fast); default 1.0 |
| `ZRNUI_ANIMATIONS` | `on` / `off`: forces animations on or off for a new root |
| `ZRNUI_RENDERER` | `auto` / `software` / `gpu`: the renderer of a new root |
| `ZRNUI_THEME` | path of a `theme.ini` that replaces the compiled-in one |
| `ZRNUI_FONT_DIR` | the font directory, when `Zset_font_directory` was not called |

## Fuzzing and coverage

```sh
# libFuzzer (clang): text layout and input, PNG files, theme.ini
cmake -S . -B build-fuzz -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DZRNUI_BUILD_FUZZERS=ON -DZRNUI_BUILD_TESTS=OFF
cmake --build build-fuzz
ASAN_OPTIONS=detect_leaks=0 build-fuzz/fuzz/fuzz_text corpus_dir -max_total_time=60

# gcov: build with -DZRNUI_COVERAGE=ON, run ctest, then gcov the .gcda files
```
