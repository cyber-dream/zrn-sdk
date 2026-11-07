# Changes that break compatibility

## Pages have no text name; a page's titles are a TitlePanel

As on Windows Phone, a page is a TitlePanel and a ContentPanel under it. The TitlePanel
holds the application's title (small) and the page's title (large), written by the app
in the case it wants them shown in.

**What broke**

- `zui_page(app, "Title")` is now `zui_page(app)`. The same in C++ (`app.page()`), Rust
  (`app.page()`) and Python (`app.page()`). The ABI of `zui_page` changed: an app built
  against the old library must be rebuilt.
- Nothing is drawn at the top of a page until the app adds a TitlePanel:

  ```c
  zui_node *page  = zui_page(app);
  zui_node *title = zui_title_panel(page);
  zui_app_title(title, "SETTINGS");     /* small, as written */
  zui_page_title(title, "settings");    /* large, as written */
  ```

  C++: `page.title_panel().app_title("SETTINGS").page_title("settings")`; the same
  names in Rust and Python.
- The style no longer turns the application's title into capitals and the page's title
  into lower case: write them the way they are shown.
- A page without a TitlePanel starts at the top of the window (y = 0): an app's window is
  the whole screen, and keeping clear of the status bar is the app's business. The
  template's TitlePanel does it: its first line starts 32 + 24 down.
- `zrn render --page` takes only a number (0 is the first page the app makes). A page
  title is not a name to look it up by any more.
- A compact page (a person: `zui_page_set_compact`) shows its title panel's page title
  as the name.

**Not changed yet:** the date and time pickers still write the application's title in
capitals and their own title in lower case over the page.

## The root of a page has no margin; the margin belongs to the ContentPanel

As on Windows Phone, where `LayoutRoot` has no margin and `ContentPanel` has. What is put
straight into a page now goes edge to edge, one under another, with no gap between.
`zui_content_panel(page)` (C++/Rust/Python: `page.content_panel()`) is a column with the
24 point margin on both sides and the usual gaps; put the usual things in it.

**What broke**

- Everything an app put straight into a page used to get the margin and the gaps. It must
  go into a ContentPanel now, or it touches the screen's edges. The examples and the
  templates do that.
- A tab (`zui_tab`) is a ContentPanel itself: the margin is its own, nothing to change.
  A Hub keeps placing its sections by the margin.
- The room under the last thing on a page (48 points) is the ContentPanel's, so a page
  without one ends where its last thing ends.

**Deleting from the root, as in XAML:** `zui_clear(page)` empties a page, `zui_destroy(node)`
removes a TitlePanel or a ContentPanel. The page does not keep pointing at what is gone;
a TitlePanel can be made again with `zui_title_panel`. There is no separate root node over
the pages (Windows Phone's frame): the page itself is the root.

## Pages are made explicitly: zui_page, zui_page_blank, zui_page_fullscreen

- `zui_page(app)` is the Windows Phone template: a root with a TitlePanel and a ContentPanel
  in it. `zui_title_panel(page)` and `zui_content_panel(page)` return those two.
- `zui_page_blank(app)` is an empty root from the top of the window, edge to edge. A pivot
  (`zui_tabs`) or a hub (`zui_hub`) goes in one of these, with a `zui_title_panel` for the
  application's title.
- `zui_page_fullscreen(app)` is an empty root over the whole window above the footer, not
  scrolling (a photo viewer).
- `zui_page_set_fullscreen` is gone: make the page with `zui_page_fullscreen`. A page does
  not change its kind after it is made. The same in C++, Rust and Python:
  `app.page_blank()`, `app.page_fullscreen()`.
- An empty ContentPanel takes no room (a pivot or hub page made with `zui_page` has no gap).

## The app has no id or title to give

- `zui_app_new(id, title)` is now `zui_app_new()`. The same in C++ (`zrn::ui::App app;`), Rust
  (`App::new()`) and Python (`zrnui.App(style=...)`). The ABI of `zui_app_new` changed.
- The Wayland app_id is `ZRN_APP_ID` if the environment has it (`zrn-exec` and `zrn run` set it
  from the manifest), else the program's name, as GTK and Qt do. The window's title is the same.
  The manifest's `id` is the only place an app says what it is.
- The application title the date and time pickers write over the page is the page's TitlePanel's
  application title, not the app's.

## A hub and a pivot carry their own title

As on Windows Phone, where `Hub.Header` and `Pivot.Title` are properties of the control and the
page's root holds nothing else.

- `zui_hub_set_header(hub, text)` is the hub's large title, which goes by more slowly than the
  sections. `zui_tabs_set_title(tabs, text)` is the pivot's small title over the tab titles. Both
  are drawn as written.
- A page with a hub or tabs needs no TitlePanel: make it with `zui_page_blank`, put the hub or
  the tabs in it. The title room above them is theirs.
- **What broke:** on a page with a hub or tabs the TitlePanel's two titles are not drawn any
  more; the hub's or the pivot's own are. A hub page made with `zui_page` and a TitlePanel shows
  no title until `zui_hub_set_header` is called.

## A hub and a pivot are roots, made from the app

```c
zui_app *app = zui_app_new();
zui_node *hub = zui_hub(app);                 /* a root: no page to make first */
zui_hub_set_header(hub, "HEADER");
zui_text(zui_hub_section(hub, "one"), "Hello");
return zui_run(app, hub);
```

- `zui_hub(app)` and `zui_tabs(app)` take the app, not a page. They make the root (an empty page
  underneath) and return the hub or the tabs. Page-level calls take the page or anything in it:
  `zui_run`, `zui_push`, `zui_page_set_root`, `zui_footer`, `zui_title_panel`, `zui_layout`,
  `zui_render_png`, `zui_page_scroll_to/end`, `zui_page_at_end`, `zui_page_set_compact`.
- **What broke:** `zui_hub(page)` and `zui_tabs(page)` do not exist any more; a hub or a pivot is
  never put in a page that has other things. In C++ `app.hub()` and `app.tabs()` replace
  `page.hub()` and `page.tabs()`.

## The app has one root: the frame

As on Windows Phone, where one `Frame` holds the pages and a page's own root is inside it.

- `zui_root(app)` is the frame. The pages are its children, one in front at a time (`zui_push`,
  `zui_back` as before). What the app puts in it directly (`zui_button(zui_root(app), ...)`) is
  over every page: edge to edge from the top of the window, one under another, not scrolling, and it
  gets the touch before the page. A bar that stays on every page.
- `zui_clear(zui_root(app))` removes what the app kept there, not the pages. A page is removed with
  `zui_destroy(page)` as before; the frame itself is not for `zui_destroy`.
- The frame's things take no room from the pages: they lie over them. Keep clear of them with
  the page's own margins or a space.
- No change for apps that do not use it.

## A hub section's header is set apart

`zui_hub_section(hub, "title")` is now `zui_hub_section(hub)`; the header over the section is
`zui_hub_section_set_header(section, "title")`, as `zui_hub_set_header` is the hub's. A section
without one has no header. C++: `hub.hub_section()` and `set_section_header(...)`.

## The titles are widgets: zui_title and zui_header (replaces the title setters above)

As on Windows Phone, where the application's name, a page's name, a hub section's header and a
pivot's title are all a `TextBlock` with a style. Here it is one widget in two styles, made apart
from whatever holds it:

```c
zui_node *title = zui_title_panel(page);   /* a hub or a pivot is a page too: zui_title_panel(hub) */
zui_title(title, "SETTINGS");              /* the small one: the application's, a pivot's */
zui_header(title, "settings");             /* the large one: a page's, a hub's */

zui_node *section = zui_hub_section(hub);
zui_title(section, "first");               /* a section's header: the same small title, first in it */
```

- **Gone:** `zui_app_title`, `zui_page_title`, `zui_hub_set_header`, `zui_tabs_set_title`,
  `zui_hub_section_set_header` and the argument of `zui_hub_section` (see above). C++: `title(text)`
  and `header(text)` on a node, Rust and Python the same.
- The TitlePanel lays its titles out (from the status bar and 24 down) and the page draws them over
  its header; a hub's go by slowly. A section's title is laid out above its column.
- A hub or a pivot with no TitlePanel has no titles and no room for them.
- The text is as written, in the case it is written in; the case is the app's.

## Scrolling is a widget: zui_scroll

As on Windows Phone, where a page does not scroll and a `ScrollViewer` (built into lists and hubs)
does. What scrolls on a page is a node of its own, whatever the page is:

- **`zui_scroll(parent)`** is a scroll viewer: a column, edge to edge, that reaches from where it is to
  the bottom of the window; what is put in it goes on beyond, a finger moves it with inertia and a bounce
  at the ends. The titles and everything outside it stay. C++/Rust/Python: `scroll()`.
- **Tabs and a hub's sections** are scroll viewers of their own: each keeps its own place, only the one in
  front moves, and the page no longer swaps its scroll with theirs.
- **A page does not scroll by itself** any more, with or without these: what is to scroll goes in a
  `zui_scroll`. **What broke:** an app that put a long list of things straight into a page (or in its
  ContentPanel) now has them cut off at the bottom of the window until it puts them in a `zui_scroll`.
  `zui_page_scroll_to` does nothing on such a page and `zui_page_at_end` is always true.
- The scroll of a list is that of the viewer, tab or section it is in. `zui_page_scroll_to`,
  `zui_page_scroll_end` and `zui_page_at_end` act on what scrolls on the page.
- `zui_page_set_fullscreen`-style pages (`zui_page_fullscreen`) do not scroll, as before.

## A hub is the whole screen and has its own title: zui_hub_title

As on Windows Phone, where `Hub.Header` belongs to the hub and the hub is the whole page.

- `zui_hub_title(hub, "text")` sets the hub's large title. It is not a widget the app makes
  (`zui_header(zui_title_panel(hub), ...)` for a hub is gone): the hub has one and draws it, going by
  more slowly than the sections and round with them.
- The hub covers the whole screen from the top; its sections start under its title. A background set
  on it (`zui_set_background(hub, brush)`) fills all of it, the title's room too.
- What scrolls in a section goes no higher than under the title, and a touch on the title is not a touch
  on the sections.
- A pivot's small title is still a `zui_title` in `zui_title_panel(tabs)` for now.

## Brushes (new): solid, gradients, pictures, shaders

`zui_set_background(node, brush)` fills any node's area behind it: a page (the whole window, behind its
titles), a ContentPanel, a card, a hub (the whole screen), the app's root (behind every page).
`zui_brush_solid`, `zui_brush_linear`, `zui_brush_radial` (+ `zui_brush_stop`), `zui_brush_image` (with
`zui_brush_set_stretch`, `zui_brush_set_align`), `zui_brush_set_opacity`, `zui_brush_ref`, `zui_brush_free`.
`zui_brush_shader(source)` paints with the app's own OpenGL ES 1.00 fragment shader:
`vec4 zui_shade(vec2 uv)`, reading `u_time`, `u_res` and its own uniforms (`zui_brush_set_uniform`);
`zui_brush_error` says why one did not compile. A page or a section with a background no longer has the
header paint black under its titles.

## Pivot as a root with items (like the hub)
- `zui_tabs(app)` → `zui_pivot(app)`, `zui_tab(tabs, "title")` → `zui_pivot_item(pivot)`,
  `zui_tabs_select/current` → `zui_pivot_select/current`. C++: `app.pivot()`, `pivot.item()`.
- An item's title is no longer an argument: call `zui_pivot_item_header(item, "title")`; the pivot draws it
  in the title row and the column skips it (calling it again, or `zui_set_text` on its node, renames the tab).
- `zui_pivot_title(pivot, "…")`: the pivot's own small title (Pivot.Title), one per pivot; a second call changes the text.
