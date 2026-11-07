---
name: zrnui
description: Build ztOS phone apps with libzrnui (C, C++, Rust or Python) in Metro and DotMatrix, and package them as .zexec with the zrn CLI. Use when writing or changing UI in a ztOS app project (a folder with zrnapp.toml), when adding a page, list, switch, slider or text field, when building, rendering, running, packing or verifying a ztOS app, or when asked how a ztOS app should look.
---

# libzrnui and zrn

A ztOS app is a set of **pages**. A page is a column: put things in it and
libzrnui lays them out, scrolls them, handles touch and moves between pages
on the GPU. One C library sits under every language; the names match.

## Project layout

```
zrnapp.toml        id, name, version, permissions; [build] lang + sources
icon.svg           the app's icon (square, simple)
main.c | main.cpp | main.py | src/main.rs (+ Cargo.toml)
AGENTS.md          instructions for coding agents
build/             what `zrn build` makes (do not edit)
dist/              what `zrn pack` makes: <id>-<version>-<arch>.zexec + .minisig
```

## The API in four languages

| C (`#include <zrnui.h>`) | C++ (`zrnui.hpp`) | Rust (`zrnui`) | Python (`import zrnui`) |
|---|---|---|---|
| `zui_app_new(id, title)` | `zrn::ui::App app(id, title)` | `App::new(id, title)` | `zrnui.App(id, title)` |
| `zui_page(app)` | `app.page()` | `app.page()` | `app.page()` |
| `zui_title_panel(page)`, `zui_app_title(p, "APP")`, `zui_page_title(p, "page")` | `page.title_panel().app_title("APP").page_title("page")` | same | same |
| `zui_run(app, page)` | `app.run(page)` | `app.run(page)` | `app.run(page)` |
| `zui_page_blank(app)`, `zui_page_fullscreen(app)`: an empty root (from the top of the window, edge to edge) | `app.page_blank()`, `app.page_fullscreen()` | same | same |
| `zui_content_panel(page)`: the margin and the gaps; the page itself is edge to edge | `page.content_panel()` | same | same |
| `zui_section(content, "Heading")` returns a card | `content.section(...)` | `content.section(...)` | `content.section(...)` |
| `zui_card`, `zui_column`, `zui_row`, `zui_space` | `card()`, `column()`, `row()`, `space(pt)` | same | same |
| `zui_text`, `zui_heading`, `zui_caption` | `text()`, `heading()`, `caption()` | same | same |
| `zui_button(p, "Label", cb, data)` | `p.button("Label", []{...})` | `p.button("Label").on_click(move \|\| ...)` | `p.button("Label", fn)` |
| `zui_item(p, title, subtitle, cb, data)` | `p.item(t, s, fn)` | `p.item(t, Some(s)).on_click(..)` | `p.item(t, s, on_click=fn, icon=, detail=)` |
| `zui_switch(p, title, on, cb, data)` | `p.toggle(t, on, [](bool){})` | `p.switch(t, on).on_change(\|on\| ..)` | `p.switch(t, on, on_change=fn)` |
| `zui_slider(p, title, 0..1, cb, data)` | `p.slider(t, v, [](double v, bool done){})` | `.slider(t, v).on_slide(\|v, done\| ..)` | `p.slider(t, v, on_change=, on_release=)` |
| `zui_progress(p, 0..1 or -1)` | `progress(v)` | `progress(v)` | `progress(v)` |
| `zui_field(p, placeholder, cb, data)` | `p.field(ph, [](const std::string&, bool submitted){})` | `.field(ph).on_edit(\|t, submitted\| ..)` | `p.field(ph, on_change=, on_submit=)` |
| `zui_push(page)`, `zui_back(app)` | `page.push()`, `app.back()` | `page.push()`, `app.back()` | `page.push()`, `app.back()` |
| `zui_set_text/detail/subtitle/value/on/enabled/visible` | `set_text(...)` etc. (chain) | `set_text(..)` etc. (chain) | `set_text()`, `.detail=`, `.value=`, `.on=`, `.visible=` |
| `zui_set_icon(n, "wifi")`, `zui_set_tint(n, 0x6366f1)` | `set_icon`, `set_tint` | same | `icon=`, `tint=` |
| `zui_set_variant(n, ZUI_TONAL)` | `set_variant(Variant::Tonal)` | `set_variant(Variant::Tonal)` | `variant=zrnui.TONAL` |
| `zui_toast(app, "Saved")`, `zui_busy(app, true)` | `app.toast()`, `app.busy()` | same | same |
| `zui_timer(app, ms, repeat, fn, data)` | `app.timer(ms, repeat, fn)` | `app.timer(ms, repeat, fn)` | `app.timer(ms, fn, repeat=)` |
| `zui_post(app, fn, data)` (any thread) | `app.post(fn)` | `app.poster().post(fn)` | `app.post(fn)` |
| `zui_watch_fd(app, fd, ready, data)` | (C API) | (C API) | `app.watch(fd, fn)` |

In C, one callback shape serves all: `void cb(zui_node *node, void *data)`;
`zui_event(node)` says why (`ZUI_EVENT_CLICK`, `_CHANGE`, `_SUBMIT`). A
slider sends CHANGE while it moves and SUBMIT when the finger lifts; a field
sends CHANGE per edit and SUBMIT on Enter.

Icons: wifi, bluetooth, bell, moon, sun, lock, volume, battery, info, search,
plus, check, close, more, back, chevron, music, person, gear, phone.

## Style rules

- Group related rows in a **section** (a heading over a rounded card). Lists
  of things are `item`s and `switch`es inside a card; dividers come by themselves.
- A row that opens another page has an `on_click` (it gets an arrow); a value
  on the right is `set_detail`.
- One **primary** button per page at most; others `TONAL` or `PLAIN`;
  destructive actions `DANGER`.
- Page titles (`zui_page_title`) are short nouns ("wi-fi", "settings"), written in the case they are shown in: the application title in capitals, the page title in lower case, as on Windows Phone.
- Icons in rows sit in a tinted tile; use `set_tint` to tell kinds apart,
  sparingly. Do not set the accent; the system does.
- Do not fake layout with `space()`; use sections and cards.
- The same code also renders in Metro (ztRetrofit). Check both:
  `zrn render --style metro`.

## Look at what you made (no display needed)

```
zrn build && zrn render --page 0 /tmp/p0.png          # the first page the app creates
zrn render --page 1 --scroll 300 /tmp/p1.png         # the second page, scrolled
zrn render --style dotmatrix /tmp/m.png
zrn run --headless --script "wait 400; tap 180 170; wait 600; snap /tmp/after.png; quit"
```

`--page N` counts pages in the order the app *creates* them (pages have no names). The PNG is 360x740 points at scale 2 by default (`--size`, `--scale`).
Read the PNG and check: nothing cut off, text readable, one primary action,
sections make sense. `zrn render --json` reports the path and size.

## Build, package, check

```
zrn build [--json]          # build/<arch>/...
zrn pack [--json]           # dist/<id>-<version>-<arch>.zexec + .minisig (dev key)
zrn verify dist/x.zexec     # structure + signature; exit 0 good, 1 broken, 2 untrusted
zrn install dist/x.zexec    # into this user's app list
zrn doctor                  # what is missing, with the command that fixes it
```

Errors from `zrn` name the file and the exact fix; with `--json` they are
`{"ok": false, "error": {"code", "message", "hint"}}`.

## Publishing

The ztOS store builds apps from source at a pinned commit. Push the code to a
public git repo, then submit `id`, repo URL and the full commit SHA (see the
store's CONTRIBUTING.md). Never submit binaries.
