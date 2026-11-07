# SPDX-License-Identifier: LGPL-2.1-or-later
"""libzrnui for Python: the same C library as C, C++ and Rust apps, over ctypes.

    import zrnui

    app = zrnui.App()
    page = app.page()
    label = page.text("Not tapped yet")
    page.button("Tap me", lambda: label.set_text("Tapped"))
    app.run(page)

No Qt and nothing to compile: the widgets are drawn by libzrnui on the GPU.
Everything runs on the thread that calls App.run(); App.post() is the way in
from another thread.
"""
import ctypes
import ctypes.util
import os
import threading

__all__ = ["App", "Node", "PRIMARY", "TONAL", "PLAIN", "DANGER", "library_path"]

PRIMARY, TONAL, PLAIN, DANGER = 0, 1, 2, 3
EVENT_NONE, EVENT_CLICK, EVENT_CHANGE, EVENT_SUBMIT = 0, 1, 2, 3


def _candidates():
    env = os.environ.get("ZRNUI_LIB")
    if env:
        yield env
    here = os.path.dirname(os.path.abspath(__file__))
    # the SDK's own tree: python/zrnui -> libzrnui/build
    yield os.path.join(here, "..", "..", "libzrnui", "build", "libzrnui.so.0")
    yield "libzrnui.so.0"
    found = ctypes.util.find_library("zrnui")
    if found:
        yield found


def _load():
    errors = []
    for path in _candidates():
        try:
            return ctypes.CDLL(path), path
        except OSError as e:
            errors.append(f"{path}: {e}")
    raise ImportError("libzrnui not found; set ZRNUI_LIB to libzrnui.so.0 (tried: " + "; ".join(errors) + ")")


_lib, library_path = _load()

_p = ctypes.c_void_p
_CB = ctypes.CFUNCTYPE(None, _p, _p)       # zui_cb(node, data)
_FREE = ctypes.CFUNCTYPE(None, _p)         # zui_free_fn(data)
_FN = ctypes.CFUNCTYPE(None, _p)           # zui_fn(data)
_FD = ctypes.CFUNCTYPE(None, ctypes.c_int, _p)


def _sig(name, res, *args):
    f = getattr(_lib, name)
    f.restype = res
    f.argtypes = list(args)
    return f


_s = ctypes.c_char_p
_d = ctypes.c_double
_b = ctypes.c_bool
_i = ctypes.c_int
_u32 = ctypes.c_uint32

_version = _sig("zui_version", _s)
_app_new = _sig("zui_app_new", _p)
_run = _sig("zui_run", _i, _p, _p)
_quit = _sig("zui_quit", None, _p)
_app_free = _sig("zui_app_free", None, _p)
_set_style = _sig("zui_app_set_style", None, _p, _s)
_style = _sig("zui_app_style", _s, _p)
_set_theme = _sig("zui_app_set_theme", None, _p, _s)
_set_accent = _sig("zui_app_set_accent", None, _p, _u32)
_busy = _sig("zui_busy", None, _p, _b)
_toast = _sig("zui_toast", None, _p, _s)
_page = _sig("zui_page", _p, _p)
_page_blank = _sig("zui_page_blank", _p, _p)
_page_fullscreen = _sig("zui_page_fullscreen", _p, _p)
_title_panel = _sig("zui_title_panel", _p, _p)
_content_panel = _sig("zui_content_panel", _p, _p)
_scroll = _sig("zui_scroll", _p, _p)
_title = _sig("zui_title", _p, _p, _s)
_header = _sig("zui_header", _p, _p, _s)
_push = _sig("zui_push", None, _p)
_back = _sig("zui_back", None, _p)
_column = _sig("zui_column", _p, _p)
_row = _sig("zui_row", _p, _p)
_card = _sig("zui_card", _p, _p)
_section = _sig("zui_section", _p, _p, _s)
_space = _sig("zui_space", _p, _p, _d)
_text = _sig("zui_text", _p, _p, _s)
_heading = _sig("zui_heading", _p, _p, _s)
_caption = _sig("zui_caption", _p, _p, _s)
_button = _sig("zui_button", _p, _p, _s, _CB, _p)
_item = _sig("zui_item", _p, _p, _s, _s, _CB, _p)
_switch = _sig("zui_switch", _p, _p, _s, _b, _CB, _p)
_slider = _sig("zui_slider", _p, _p, _s, _d, _CB, _p)
_progress = _sig("zui_progress", _p, _p, _d)
_field = _sig("zui_field", _p, _p, _s, _CB, _p)
_set_text = _sig("zui_set_text", None, _p, _s)
_get_text = _sig("zui_get_text", _s, _p)
_set_detail = _sig("zui_set_detail", None, _p, _s)
_set_subtitle = _sig("zui_set_subtitle", None, _p, _s)
_set_value = _sig("zui_set_value", None, _p, _d)
_get_value = _sig("zui_get_value", _d, _p)
_set_on = _sig("zui_set_on", None, _p, _b)
_get_on = _sig("zui_get_on", _b, _p)
_set_enabled = _sig("zui_set_enabled", None, _p, _b)
_set_visible = _sig("zui_set_visible", None, _p, _b)
_set_variant = _sig("zui_set_variant", None, _p, _i)
_set_flex = _sig("zui_set_flex", None, _p, _d)
_set_icon = _sig("zui_set_icon", None, _p, _s)
_set_tint = _sig("zui_set_tint", None, _p, _u32)
_set_handler = _sig("zui_set_handler", None, _p, _CB, _p, _FREE)
_event = _sig("zui_event", _i, _p)
_clear = _sig("zui_clear", None, _p)
_destroy = _sig("zui_destroy", None, _p)
_frame = _sig("zui_node_frame", None, _p, ctypes.POINTER(_d), ctypes.POINTER(_d), ctypes.POINTER(_d), ctypes.POINTER(_d))
_timer = _sig("zui_timer", _i, _p, _i, _b, _FN, _p)
_timer_cancel = _sig("zui_timer_cancel", None, _p, _i)
_post = _sig("zui_post", None, _p, _FN, _p)
_watch = _sig("zui_watch_fd", None, _p, _i, _FD, _p)
_render_png = _sig("zui_render_png", _i, _p, _p, _s, _i, _i, _i)
_layout = _sig("zui_layout", None, _p, _p, _i, _i)
_inject = _sig("zui_inject_touch", None, _p, _i, _d, _d)


def _enc(s):
    return None if s is None else str(s).encode("utf-8")


# Callbacks: one C trampoline each kind; the Python function is found by a
# number passed as the C side's data pointer, and forgotten when the node goes.
_handlers = {}
_next_id = [1]
_lock = threading.Lock()


def _keep(fn):
    with _lock:
        key = _next_id[0]
        _next_id[0] += 1
        _handlers[key] = fn
    return key


@_CB
def _dispatch(node, data):
    fn = _handlers.get(data or 0)
    if fn is not None:
        try:
            fn(node)
        except Exception:  # an exception must not cross into C
            import traceback
            traceback.print_exc()


@_FREE
def _forget(data):
    with _lock:
        _handlers.pop(data or 0, None)


@_FN
def _once(data):
    with _lock:
        fn = _handlers.pop(data or 0, None)
    if fn is not None:
        try:
            fn()
        except Exception:
            import traceback
            traceback.print_exc()


@_FN
def _repeat(data):
    fn = _handlers.get(data or 0)
    if fn is not None:
        try:
            fn()
        except Exception:
            import traceback
            traceback.print_exc()


@_FD
def _fd_ready(fd, data):
    fn = _handlers.get(data or 0)
    if fn is not None:
        try:
            fn(fd)
        except Exception:
            import traceback
            traceback.print_exc()


_NULL_CB = _CB()


class Node:
    """A thing on a page. Methods that add something return the new Node."""

    __slots__ = ("app", "_n")

    def __init__(self, app, ptr):
        if not ptr:
            raise RuntimeError("libzrnui returned no node")
        self.app = app
        self._n = ptr

    def _wrap(self, ptr):
        return Node(self.app, ptr)

    def _on(self, fn):
        key = _keep(fn)
        _set_handler(self._n, _dispatch, key, _forget)
        return self

    # ---- what goes in it ----
    def column(self):
        return self._wrap(_column(self._n))

    def row(self):
        return self._wrap(_row(self._n))

    def card(self):
        return self._wrap(_card(self._n))

    def section(self, title):
        """A heading with a card under it; returns the card."""
        return self._wrap(_section(self._n, _enc(title)))

    def space(self, points):
        return self._wrap(_space(self._n, points))

    def text(self, s):
        return self._wrap(_text(self._n, _enc(s)))

    def heading(self, s):
        return self._wrap(_heading(self._n, _enc(s)))

    def caption(self, s):
        return self._wrap(_caption(self._n, _enc(s)))

    def button(self, label, on_click=None, variant=None):
        b = self._wrap(_button(self._n, _enc(label), _NULL_CB, None))
        if on_click:
            b._on(lambda n: on_click())
        if variant is not None:
            b.variant = variant
        return b

    def item(self, title, subtitle=None, on_click=None, icon=None, detail=None, tint=None):
        it = self._wrap(_item(self._n, _enc(title), _enc(subtitle), _NULL_CB, None))
        if on_click:
            it._on(lambda n: on_click())
        if icon:
            it.icon = icon
        if detail is not None:
            it.detail = detail
        if tint:
            it.tint = tint
        return it

    def switch(self, title, on=False, on_change=None, icon=None, subtitle=None, tint=None):
        """on_change(on: bool)"""
        sw = self._wrap(_switch(self._n, _enc(title), bool(on), _NULL_CB, None))
        if on_change:
            sw._on(lambda n: on_change(bool(_get_on(n))))
        if icon:
            sw.icon = icon
        if subtitle:
            sw.subtitle = subtitle
        if tint:
            sw.tint = tint
        return sw

    def slider(self, title, value=0.0, on_change=None, on_release=None):
        """on_change(value) while it moves; on_release(value) when the finger lets go"""
        sl = self._wrap(_slider(self._n, _enc(title), float(value), _NULL_CB, None))

        def handle(n):
            v = _get_value(n)
            if _event(n) == EVENT_SUBMIT:
                if on_release:
                    on_release(v)
            elif on_change:
                on_change(v)
        if on_change or on_release:
            sl._on(handle)
        return sl

    def progress(self, value=-1.0):
        """0..1, or below 0 for busy with no end in sight"""
        return self._wrap(_progress(self._n, float(value)))

    def field(self, placeholder="", on_change=None, on_submit=None):
        """on_change(text) on every edit, on_submit(text) on Enter"""
        f = self._wrap(_field(self._n, _enc(placeholder), _NULL_CB, None))

        def handle(n):
            t = (_get_text(n) or b"").decode("utf-8", "replace")
            if _event(n) == EVENT_SUBMIT:
                if on_submit:
                    on_submit(t)
            elif on_change:
                on_change(t)
        if on_change or on_submit:
            f._on(handle)
        return f

    # ---- a page's TitlePanel: the application's title (small), the page's title (large) ----
    def title_panel(self):
        return Node(self.app, _title_panel(self._n))

    def scroll(self):
        """A scroll viewer: what is put in it scrolls, the rest of the page stays."""
        return Node(self.app, _scroll(self._n))

    def content_panel(self):
        return Node(self.app, _content_panel(self._n))

    def title(self, s):
        """The small title (the application's, a hub section's), in a TitlePanel or a section."""
        return self._wrap(_title(self._n, _enc(s)))

    def header(self, s):
        """The large title (a page's, a hub's), in a TitlePanel."""
        return self._wrap(_header(self._n, _enc(s)))

    # ---- what it is ----
    def set_text(self, s):
        _set_text(self._n, _enc(s))
        return self

    text_value = property(lambda self: (_get_text(self._n) or b"").decode("utf-8", "replace"),
                          lambda self, s: _set_text(self._n, _enc(s)))
    detail = property(None, lambda self, s: _set_detail(self._n, _enc(s)))
    subtitle = property(None, lambda self, s: _set_subtitle(self._n, _enc(s)))
    value = property(lambda self: _get_value(self._n), lambda self, v: _set_value(self._n, float(v)))
    on = property(lambda self: bool(_get_on(self._n)), lambda self, v: _set_on(self._n, bool(v)))
    enabled = property(None, lambda self, v: _set_enabled(self._n, bool(v)))
    visible = property(None, lambda self, v: _set_visible(self._n, bool(v)))
    variant = property(None, lambda self, v: _set_variant(self._n, int(v)))
    flex = property(None, lambda self, v: _set_flex(self._n, float(v)))
    icon = property(None, lambda self, v: _set_icon(self._n, _enc(v)))
    tint = property(None, lambda self, v: _set_tint(self._n, int(v)))

    def on_event(self, fn):
        """fn(node, event) for anything the node reports (EVENT_CLICK, _CHANGE, _SUBMIT)"""
        return self._on(lambda n: fn(self._wrap(n), _event(n)))

    def frame(self):
        v = [_d(), _d(), _d(), _d()]
        _frame(self._n, *[ctypes.byref(x) for x in v])
        return tuple(x.value for x in v)

    def clear(self):
        _clear(self._n)

    def destroy(self):
        _destroy(self._n)
        self._n = None

    def push(self):
        """Go to this page."""
        _push(self._n)


class App:
    def __init__(self, style=None, theme=None, accent=None):
        self._a = _app_new()
        if style:
            _set_style(self._a, _enc(style))
        if theme:
            _set_theme(self._a, _enc(theme))
        if accent is not None:
            _set_accent(self._a, accent)

    @property
    def style(self):
        return _style(self._a).decode()

    def page(self):
        """The template: a root with a TitlePanel and a ContentPanel."""
        return Node(self, _page(self._a))

    def page_blank(self):
        """An empty root, from the top of the window."""
        return Node(self, _page_blank(self._a))

    def page_fullscreen(self):
        """An empty root over the whole window above the footer; it does not scroll."""
        return Node(self, _page_fullscreen(self._a))

    def run(self, first_page):
        return _run(self._a, first_page._n)

    def push(self, page):
        _push(page._n)

    def back(self):
        _back(self._a)

    def quit(self):
        _quit(self._a)

    def busy(self, on=True):
        _busy(self._a, bool(on))

    def toast(self, text):
        _toast(self._a, _enc(text))

    def set_theme(self, theme):
        _set_theme(self._a, _enc(theme))

    def timer(self, ms, fn, repeat=False):
        """fn() after ms (every ms if repeat); returns an id for cancel()"""
        key = _keep(fn)
        tid = _timer(self._a, int(ms), bool(repeat), _repeat if repeat else _once, key)
        if repeat:
            self.__dict__.setdefault("_timers", {})[tid] = key
        return tid

    def cancel(self, timer_id):
        _timer_cancel(self._a, timer_id)
        key = self.__dict__.get("_timers", {}).pop(timer_id, None)
        if key:
            _forget(key)

    def post(self, fn):
        """From any thread: fn() on the app's thread."""
        _post(self._a, _once, _keep(fn))

    def watch(self, fd, fn):
        """fn(fd) when fd is readable; fn None stops."""
        fd = fd if isinstance(fd, int) else fd.fileno()
        if fn is None:
            _watch(self._a, fd, _FD(), None)
            return
        _watch(self._a, fd, _fd_ready, _keep(fn))

    def render_png(self, page, path, width=360, height=740, scale=2):
        return _render_png(self._a, page._n, _enc(path), width, height, scale)

    # for tests: lay out without a display and touch it
    def layout(self, page, width=360, height=740):
        _layout(self._a, page._n, width, height)

    def touch(self, phase, x, y):
        _inject(self._a, phase, x, y)

    def tap(self, x, y):
        _inject(self._a, 0, x, y)
        _inject(self._a, 2, x, y)


def version():
    return _version().decode()
