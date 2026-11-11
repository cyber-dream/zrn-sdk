# zrnapp

The window of an ordinary Wayland application on top of [zrnui](../zrnui). zrnui draws a tree and knows no window;
zrnapp is the Wayland client around it: `xdg-shell` window, pointer / touch / wheel / keyboard / text into the
`Zroot_inject_*` functions, the scale of the shell (`wp_fractional_scale` with `wp_viewporter`), and the frames of the
root on the compositor, through `wl_shm` or OpenGL ES 2 on EGL. It talks to zrnui only through `zrnui/platform.h`.
An application started this way is a normal Wayland process: a binary, a `.desktop` file, no launcher or runtime of its own.

A program that embeds zrnui in a shell does not use zrnapp.

## Use

```c
#include <zrnapp.h>

int main(void) {
    if (!Zinit()) return 1;
    const Zid root = Zroot_create();
    /* ... build the tree, Zroot_set_visual(root, ...) ... */
    const ZWindowOptions window = { .title = "Notes" };   /* the app id is the name of the executable */
    const int ok = Zwindow_run_ex(root, &window);
    Zroot_destroy(root);
    Zshutdown();
    return ok ? 0 : 1;
}
```

Link `zrnapp::zrnapp` (CMake) or `pkg-config --cflags --libs zrnapp`.

## Title and app id

A window has a title (for people) and an app id (for the shell: it finds `<app id>.desktop`, the icon and the grouping of
windows by it). `ZWindowOptions.app_id` is the name of the `.desktop` file without the suffix. Without it the app id is the
file name of the executable, as GTK does, so a program installed as `notes` with `notes.desktop` needs nothing.
`Zwindow_run(root, title)` sets the title only; the title is never used as the app id.

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build        # test_app_id; the window tests run on cage (headless) when it is installed
```

- zrnui is the one in `../zrnui`, or the installed one (`pkg-config zrnui`) with `-DZRNAPP_SYSTEM_ZRNUI=ON`.
- Needs wayland-client, xkbcommon, `wayland-scanner` and the XML of `wayland-protocols` (xdg-shell, viewporter,
  fractional-scale-v1; the protocol code is generated at build time). For the GPU window also egl, glesv2 and wayland-egl;
  without them the window draws with `wl_shm`.
- `-DZRNAPP_BUILD_SHARED=ON` builds a shared library; `cmake --install` installs it with `zrnapp.h` and `zrnapp.pc`.

## Environment

| variable | effect |
|---|---|
| `ZRNAPP_SCALE` | overrides the UI scale (debug only): the layout grid is the window / scale; without it the `preferred_scale` of the compositor is used |

The variables of zrnui itself (`ZRNUI_RENDERER`, `ZRNUI_ANIMATIONS`, ...) are read by `Zinit`; see `../zrnui/Readme.md`.
