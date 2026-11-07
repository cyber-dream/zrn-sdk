# zrn-sdk

**Requires ztOS BETA-1.1 or later (version 0.2.1-BETA-1.1).** ztOS applications do not run on earlier versions.

Make apps for **ztOS** in C, C++, Rust or Python. They are drawn by
**libzrnui** on the phone's GPU in **Metro** or **DotMatrix**, whichever the system is set to, ship as one signed **`.zexec`** file, and reach people
through the ztOS store, which builds them from your source.

## Start

```
export PATH="$PWD/tools:$PATH"     # zrn and zrn-exec
zrn doctor                         # what is missing, and the command that fixes it
zrn new "Notes" --lang c           # or cpp, python, rust
cd notes
zrn build
zrn render out.png                 # the first page, as a PNG: no phone, no display needed
zrn run                            # a phone-sized window (nested Wayland compositor)
zrn pack                           # dist/<id>-0.1.0-<arch>.zexec + .minisig
zrn install dist/*.zexec           # into your app list
```

You need Python 3.11+, a C compiler, `make`, `pkg-config` and the development
files for Wayland, Mesa (EGL, GLES2, GBM), FreeType, HarfBuzz, fontconfig,
libpng and xkbcommon. Rust apps need `cargo`. `zrn build` builds libzrnui from
this tree the first time if it is not installed.

## A page

```c
#include <zrnui.h>

static void open_wifi(zui_node *row, void *data) { zui_push(data); }

int main(void) {
	zui_app *app = zui_app_new("io.github.you.settings", "Settings");
	zui_node *wifi = zui_page(app);
	zui_switch(zui_card(zui_content_panel(wifi)), "Wi-Fi", true, NULL, NULL);

	zui_node *page = zui_page(app);
	zui_node *title = zui_title_panel(page);              /* the page's TitlePanel, as on Windows Phone */
	zui_app_title(title, "SETTINGS");                     /* small, as written */
	zui_page_title(title, "settings");                    /* large, as written */
	zui_node *content = zui_content_panel(page);          /* the margin and the gaps; the page itself has none */
	zui_node *conn = zui_section(content, "Connections");     /* a heading and a card */
	zui_node *row = zui_item(conn, "Wi-Fi", NULL, open_wifi, wifi);
	zui_set_icon(row, "wifi");
	zui_set_detail(row, "Home-5G");
	zui_slider(zui_section(content, "Display"), "Brightness", 0.6, NULL, NULL);
	zui_button(content, "Check for updates", NULL, NULL);
	return zui_run(app, page);
}
```

You say what is on a page; libzrnui lays it out, scrolls it, presses it,
animates between pages and draws it on the GPU. The same in the other
languages:

```cpp
zrn::ui::App app("io.github.you.hello", "Hello");            // C++ (zrnui.hpp)
auto page = app.page();
auto label = page.text("Not tapped yet");
page.button("Tap me", [&] { label.set_text("Tapped"); });
return app.run(page);
```

```rust
let app = zrnui::App::new("io.github.you.hello", "Hello");   // Rust (crate zrnui)
let page = app.page();
let label = page.text("Not tapped yet");
page.button("Tap me").on_click(move || { label.set_text("Tapped"); });
std::process::exit(app.run(page));
```

```python
import zrnui                                                  # Python (no Qt)
app = zrnui.App("io.github.you.hello", "Hello")
page = app.page()
label = page.text("Not tapped yet")
page.button("Tap me", lambda: label.set_text("Tapped"))
app.run(page)
```

The full API, the style rules and the package format are in
[DESIGN.md](DESIGN.md); the skill coding agents get is
[templates/agents/SKILL.md](templates/agents/SKILL.md).

## What a page can hold

Items, switches, sliders, fields, buttons, long lists and pivots (`zui_tabs`),
and what Windows Phone 8.1 had besides: check boxes and radio buttons
(`zui_check`, `zui_radio`), a password box and suggestions under a field,
a progress ring, a list picker that opens in place (`zui_picker`), date and
time pickers, tiles that turn over (`zui_tiles`, `zui_tile_set_live`), the
Hub or panorama (`zui_hub`), press and hold (`zui_set_hold`), and over the
page a menu, a dialog and a jump list (`zui_menu`, `zui_dialog`,
`zui_jump_list`). Each is documented in `libzrnui/include/zrnui.h`;
`examples/settings-c` shows them on its Widgets pages. They are in C and C++;
the Rust and Python bindings do not have them yet.

`make run` (or the `run` target in CLion) opens the settings example in a
window on a Wayland desktop, with no virtual machine: `make run STYLE=dotmatrix`.

## Looking at your app without a phone

```
zrn render out.png                          # first page, 360x740 points at scale 2
zrn render --page 0 --scroll 300 out.png
zrn render --style dotmatrix out.png
zrn render --scale 4 out.png                # the S9's real 1440x2960
zrn run --headless --script "wait 500; tap 180 170; wait 600; snap after.png; quit"
```

`zrn render` draws through the same GLES2 path as the phone (Mesa's
surfaceless EGL). `zrn run --headless` starts a virtual Wayland compositor
(kwin_wayland or weston), runs the app in it and lets a script tap, type, go
back and take pictures.

## Coding agents

```
zrn init-agents            # pick: Claude Code, Codex, Gemini CLI, Cursor, Copilot, OpenCode, Antigravity, Windsurf
zrn init-agents --all      # or --agent claude,codex
```

writes each agent's instruction file and installs the `zrnui` skill in the
project. Every `zrn` command takes `--json`. See [docs/llms.txt](docs/llms.txt).

## Publishing

Push the app to a public GitHub repository, then `zrn submit` shows the entry
for your current commit and `zrn submit --send` sends it. Moderators review it
on the ztOS Mobile Discord; the store builds that exact commit, signs it and
lists it. See the store's CONTRIBUTING.md.

## The store on a phone

`zrn store refresh`, `zrn store list` and `zrn store install ID` read the signed
index, trusting only the store's key (`/usr/share/zrn/trusted-keys/store.pub`).
ztOS's App Store uses them. `make install PREFIX=/usr` installs everything needed.

## This repository

| | |
|---|---|
| `libzrnui/` | the C library (`make -C libzrnui`, `make install PREFIX=/usr`) |
| `python/zrnui/` | the Python binding |
| `rust/` | the Rust crate |
| `tools/` | `zrn`, `zrn-exec` |
| `templates/` | what `zrn new` and `zrn init-agents` write |
| `examples/` | hello in C, C++, Rust and Python; a settings app in C |
| `tests/` | `make check` |
| `docs/renders/` | pictures of every example in both styles; `docs/evidence/` packages |

`make check` runs the C tests (layout, touch, typing, an offscreen frame
checked pixel by pixel), the tool tests (signatures, packages, the CLI's
JSON contract, the Python binding) and the Rust tests.
