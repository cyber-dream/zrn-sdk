// SPDX-License-Identifier: LGPL-2.1-or-later
//! libzrnui for Rust: the ztOS app toolkit. The same C library as C, C++ and
//! Python apps (widgets drawn on the GPU, in the system's style, Metro or DotMatrix), with
//! closures for callbacks and lifetimes that keep nodes from outliving their app.
//!
//! ```no_run
//! let app = zrnui::App::new();
//! let page = app.page();
//! let label = page.text("Not tapped yet");
//! let mut taps = 0;
//! page.button("Tap me").on_click(move || {
//!     taps += 1;
//!     label.set_text(&format!("Tapped {taps} times"));
//! });
//! std::process::exit(app.run(page));
//! ```
//!
//! Everything happens on the thread that calls [`App::run`]; [`Poster`] is the
//! way in from other threads. One App at a time per process: libzrnui keeps
//! its fonts and glyphs for the process.

pub mod sys;

use std::cell::RefCell;
use std::collections::HashMap;
use std::ffi::{CStr, CString};
use std::marker::PhantomData;
use std::os::raw::{c_int, c_void};
use std::sync::{Arc, Mutex};

fn cs(s: &str) -> CString {
    CString::new(s.replace('\0', "")).expect("no NUL left")
}

/// How a button looks.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Variant {
    Primary = 0,
    Tonal = 1,
    Plain = 2,
    Danger = 3,
}

/// Why a callback runs.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Event {
    None,
    Click,
    Change,
    Submit,
}

impl Event {
    fn from(e: c_int) -> Event {
        match e {
            sys::ZUI_EVENT_CLICK => Event::Click,
            sys::ZUI_EVENT_CHANGE => Event::Change,
            sys::ZUI_EVENT_SUBMIT => Event::Submit,
            _ => Event::None,
        }
    }
}

/// Returns libzrnui's version.
pub fn version() -> String {
    unsafe { CStr::from_ptr(sys::zui_version()).to_string_lossy().into_owned() }
}

// ---- callbacks: a boxed closure as the C side's data, dropped when the node goes ----
type Handler<'a> = Box<dyn FnMut(*mut sys::zui_node) + 'a>;

unsafe extern "C" fn call_handler(node: *mut sys::zui_node, data: *mut c_void) {
    let h = &mut *(data as *mut Handler);
    // a panic must not unwind into C
    let _ = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| h(node)));
}

unsafe extern "C" fn drop_handler(data: *mut c_void) {
    drop(Box::from_raw(data as *mut Handler));
}

fn attach<'a>(node: *mut sys::zui_node, f: impl FnMut(*mut sys::zui_node) + 'a) {
    let boxed: Box<Handler<'a>> = Box::new(Box::new(f));
    // The closure is called only from inside App::run (or the test calls on
    // App) and dropped when the node or the App goes: never after 'a.
    let raw = Box::into_raw(boxed) as *mut c_void;
    unsafe { sys::zui_set_handler(node, Some(call_handler), raw, Some(drop_handler)) };
}

/// The app: its pages, its window and its main loop. Dropping it frees every node.
pub struct App {
    raw: *mut sys::zui_app,
    timers: Box<RefCell<HashMap<i32, *mut TimerSlot>>>,
    alive: Arc<Mutex<usize>>,
}

struct TimerSlot {
    f: Box<dyn FnMut()>,
    repeat: bool,
    id: i32,
    map: *const RefCell<HashMap<i32, *mut TimerSlot>>,
}

unsafe extern "C" fn fire_timer(data: *mut c_void) {
    let slot = data as *mut TimerSlot;
    let _ = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| ((*slot).f)()));
    if !(*slot).repeat {
        (*(*slot).map).borrow_mut().remove(&(*slot).id);
        drop(Box::from_raw(slot));
    }
}

impl App {
    /// The app has no id or title to give: see `zui_app_new`.
    pub fn new() -> App {
        let raw = unsafe { sys::zui_app_new() };
        App { raw, timers: Box::new(RefCell::new(HashMap::new())), alive: Arc::new(Mutex::new(raw as usize)) }
    }

    /// The template: a root with a TitlePanel and a ContentPanel.
    pub fn page(&self) -> Node<'_> {
        Node::wrap(unsafe { sys::zui_page(self.raw) })
    }
    /// An empty root, from the top of the window.
    pub fn page_blank(&self) -> Node<'_> {
        Node::wrap(unsafe { sys::zui_page_blank(self.raw) })
    }
    /// An empty root over the whole window above the footer; it does not scroll.
    pub fn page_fullscreen(&self) -> Node<'_> {
        Node::wrap(unsafe { sys::zui_page_fullscreen(self.raw) })
    }

    /// Shows `first` and runs until the app quits; returns the exit status.
    pub fn run(&self, first: Node<'_>) -> i32 {
        unsafe { sys::zui_run(self.raw, first.raw) }
    }

    pub fn push(&self, page: Node<'_>) {
        unsafe { sys::zui_push(page.raw) }
    }
    pub fn back(&self) {
        unsafe { sys::zui_back(self.raw) }
    }
    pub fn quit(&self) {
        unsafe { sys::zui_quit(self.raw) }
    }
    pub fn busy(&self, busy: bool) {
        unsafe { sys::zui_busy(self.raw, busy) }
    }
    pub fn toast(&self, text: &str) {
        let t = cs(text);
        unsafe { sys::zui_toast(self.raw, t.as_ptr()) }
    }
    /// The style is the system's; this does not change it.
    pub fn set_style(&self, style: &str) {
        let s = cs(style);
        unsafe { sys::zui_app_set_style(self.raw, s.as_ptr()) }
    }
    pub fn style(&self) -> String {
        unsafe { CStr::from_ptr(sys::zui_app_style(self.raw)).to_string_lossy().into_owned() }
    }
    pub fn set_theme(&self, theme: &str) {
        let s = cs(theme);
        unsafe { sys::zui_app_set_theme(self.raw, s.as_ptr()) }
    }
    pub fn set_accent(&self, rgb: u32) {
        unsafe { sys::zui_app_set_accent(self.raw, rgb) }
    }

    /// `f` after `ms`, and every `ms` after that if `repeat`. Returns an id for [`App::cancel_timer`].
    pub fn timer(&self, ms: i32, repeat: bool, f: impl FnMut() + 'static) -> i32 {
        let slot = Box::into_raw(Box::new(TimerSlot { f: Box::new(f), repeat, id: 0, map: &*self.timers }));
        let id = unsafe { sys::zui_timer(self.raw, ms, repeat, Some(fire_timer), slot as *mut c_void) };
        if id == 0 {
            unsafe { drop(Box::from_raw(slot)) };
            return 0;
        }
        unsafe { (*slot).id = id };
        self.timers.borrow_mut().insert(id, slot);
        id
    }

    pub fn cancel_timer(&self, id: i32) {
        if let Some(slot) = self.timers.borrow_mut().remove(&id) {
            unsafe {
                sys::zui_timer_cancel(self.raw, id);
                drop(Box::from_raw(slot));
            }
        }
    }

    /// A handle other threads can use to run something on the app's thread.
    pub fn poster(&self) -> Poster {
        Poster { alive: self.alive.clone() }
    }

    /// Draws `page` into a PNG with no display (`width` x `height` points at `scale`).
    pub fn render_png(&self, page: Node<'_>, path: &str, width: i32, height: i32, scale: i32) -> Result<(), i32> {
        let p = cs(path);
        match unsafe { sys::zui_render_png(self.raw, page.raw, p.as_ptr(), width, height, scale) } {
            0 => Ok(()),
            e => Err(e),
        }
    }

    /// For tests: lay `page` out with no display, then touch it.
    pub fn layout(&self, page: Node<'_>, width: i32, height: i32) {
        unsafe { sys::zui_layout(self.raw, page.raw, width, height) }
    }
    pub fn tap(&self, x: f64, y: f64) {
        unsafe {
            sys::zui_inject_touch(self.raw, 0, x, y);
            sys::zui_inject_touch(self.raw, 2, x, y);
        }
    }
}

impl Drop for App {
    fn drop(&mut self) {
        let mut alive = self.alive.lock().unwrap_or_else(|e| e.into_inner());
        *alive = 0;
        unsafe { sys::zui_app_free(self.raw) };
        for (_, slot) in self.timers.borrow_mut().drain() {
            unsafe { drop(Box::from_raw(slot)) };
        }
    }
}

/// Runs closures on the app's thread from any thread. Posting after the App
/// is gone does nothing and returns false.
#[derive(Clone)]
pub struct Poster {
    alive: Arc<Mutex<usize>>,
}

unsafe extern "C" fn run_posted(data: *mut c_void) {
    let f = Box::from_raw(data as *mut Box<dyn FnOnce() + Send>);
    let _ = std::panic::catch_unwind(std::panic::AssertUnwindSafe(move || f()));
}

impl Poster {
    pub fn post(&self, f: impl FnOnce() + Send + 'static) -> bool {
        let alive = self.alive.lock().unwrap_or_else(|e| e.into_inner());
        if *alive == 0 {
            return false;
        }
        let boxed: Box<Box<dyn FnOnce() + Send>> = Box::new(Box::new(f));
        unsafe { sys::zui_post(*alive as *mut sys::zui_app, Some(run_posted), Box::into_raw(boxed) as *mut c_void) };
        true
    }
}

/// Something on a page. A handle: copy it freely; the App owns what it points at.
#[derive(Clone, Copy)]
pub struct Node<'a> {
    raw: *mut sys::zui_node,
    _app: PhantomData<&'a App>,
}

impl<'a> Node<'a> {
    fn wrap(raw: *mut sys::zui_node) -> Node<'a> {
        assert!(!raw.is_null(), "libzrnui returned no node");
        Node { raw, _app: PhantomData }
    }

    pub fn as_ptr(&self) -> *mut sys::zui_node {
        self.raw
    }

    // ---- what goes in it ----
    pub fn column(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_column(self.raw) })
    }
    pub fn row(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_row(self.raw) })
    }
    pub fn card(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_card(self.raw) })
    }
    /// A heading with a card under it; returns the card.
    pub fn section(&self, title: &str) -> Node<'a> {
        let t = cs(title);
        Node::wrap(unsafe { sys::zui_section(self.raw, t.as_ptr()) })
    }
    pub fn space(&self, points: f64) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_space(self.raw, points) })
    }
    pub fn text(&self, s: &str) -> Node<'a> {
        let t = cs(s);
        Node::wrap(unsafe { sys::zui_text(self.raw, t.as_ptr()) })
    }
    pub fn heading(&self, s: &str) -> Node<'a> {
        let t = cs(s);
        Node::wrap(unsafe { sys::zui_heading(self.raw, t.as_ptr()) })
    }
    pub fn caption(&self, s: &str) -> Node<'a> {
        let t = cs(s);
        Node::wrap(unsafe { sys::zui_caption(self.raw, t.as_ptr()) })
    }
    pub fn button(&self, label: &str) -> Node<'a> {
        let t = cs(label);
        Node::wrap(unsafe { sys::zui_button(self.raw, t.as_ptr(), None, std::ptr::null_mut()) })
    }
    /// A list row; give it [`Node::on_click`] and it gets an arrow.
    pub fn item(&self, title: &str, subtitle: Option<&str>) -> Node<'a> {
        let t = cs(title);
        let s = subtitle.map(cs);
        Node::wrap(unsafe {
            sys::zui_item(self.raw, t.as_ptr(), s.as_ref().map_or(std::ptr::null(), |s| s.as_ptr()), None,
                std::ptr::null_mut())
        })
    }
    pub fn switch(&self, title: &str, on: bool) -> Node<'a> {
        let t = cs(title);
        Node::wrap(unsafe { sys::zui_switch(self.raw, t.as_ptr(), on, None, std::ptr::null_mut()) })
    }
    pub fn slider(&self, title: &str, value: f64) -> Node<'a> {
        let t = cs(title);
        Node::wrap(unsafe { sys::zui_slider(self.raw, t.as_ptr(), value, None, std::ptr::null_mut()) })
    }
    /// 0..1, or below 0 for busy with no end in sight.
    pub fn progress(&self, value: f64) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_progress(self.raw, value) })
    }
    pub fn field(&self, placeholder: &str) -> Node<'a> {
        let t = cs(placeholder);
        Node::wrap(unsafe { sys::zui_field(self.raw, t.as_ptr(), None, std::ptr::null_mut()) })
    }

    // ---- what happens ----
    /// A button or item was tapped.
    pub fn on_click(self, mut f: impl FnMut() + 'a) -> Self {
        attach(self.raw, move |_| f());
        self
    }
    /// A switch turned: `f(on)`.
    pub fn on_change(self, mut f: impl FnMut(bool) + 'a) -> Self {
        attach(self.raw, move |n| f(unsafe { sys::zui_get_on(n) }));
        self
    }
    /// A slider moved: `f(value, done)`, done when the finger lets go.
    pub fn on_slide(self, mut f: impl FnMut(f64, bool) + 'a) -> Self {
        attach(self.raw, move |n| unsafe { f(sys::zui_get_value(n), sys::zui_event(n) == sys::ZUI_EVENT_SUBMIT) });
        self
    }
    /// A field changed: `f(text, submitted)`, submitted on Enter.
    pub fn on_edit(self, mut f: impl FnMut(&str, bool) + 'a) -> Self {
        attach(self.raw, move |n| unsafe {
            let t = CStr::from_ptr(sys::zui_get_text(n)).to_string_lossy();
            f(&t, sys::zui_event(n) == sys::ZUI_EVENT_SUBMIT)
        });
        self
    }
    /// Anything the node reports, with why.
    pub fn on_event(self, mut f: impl FnMut(Node<'a>, Event) + 'a) -> Self {
        attach(self.raw, move |n| f(Node::wrap(n), Event::from(unsafe { sys::zui_event(n) })));
        self
    }

    // ---- a page's TitlePanel: the application's title (small), the page's title (large) ----
    pub fn title_panel(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_title_panel(self.raw) })
    }
    /// A scroll viewer: what is put in it scrolls, the rest of the page stays.
    pub fn scroll(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_scroll(self.raw) })
    }
    pub fn content_panel(&self) -> Node<'a> {
        Node::wrap(unsafe { sys::zui_content_panel(self.raw) })
    }
    /// The small title (the application's, a hub section's), in a TitlePanel or a section.
    pub fn title(&self, s: &str) -> Node<'a> {
        let t = cs(s);
        Node::wrap(unsafe { sys::zui_title(self.raw, t.as_ptr()) })
    }
    /// The large title (a page's, a hub's), in a TitlePanel.
    pub fn header(&self, s: &str) -> Node<'a> {
        let t = cs(s);
        Node::wrap(unsafe { sys::zui_header(self.raw, t.as_ptr()) })
    }

    // ---- changing it (each returns the node, so they chain) ----
    pub fn set_text(self, s: &str) -> Self {
        let t = cs(s);
        unsafe { sys::zui_set_text(self.raw, t.as_ptr()) };
        self
    }
    pub fn set_detail(self, s: &str) -> Self {
        let t = cs(s);
        unsafe { sys::zui_set_detail(self.raw, t.as_ptr()) };
        self
    }
    pub fn set_subtitle(self, s: &str) -> Self {
        let t = cs(s);
        unsafe { sys::zui_set_subtitle(self.raw, t.as_ptr()) };
        self
    }
    pub fn set_value(self, v: f64) -> Self {
        unsafe { sys::zui_set_value(self.raw, v) };
        self
    }
    pub fn set_on(self, on: bool) -> Self {
        unsafe { sys::zui_set_on(self.raw, on) };
        self
    }
    pub fn set_enabled(self, e: bool) -> Self {
        unsafe { sys::zui_set_enabled(self.raw, e) };
        self
    }
    pub fn set_visible(self, v: bool) -> Self {
        unsafe { sys::zui_set_visible(self.raw, v) };
        self
    }
    pub fn set_variant(self, v: Variant) -> Self {
        unsafe { sys::zui_set_variant(self.raw, v as c_int) };
        self
    }
    pub fn set_flex(self, f: f64) -> Self {
        unsafe { sys::zui_set_flex(self.raw, f) };
        self
    }
    pub fn set_icon(self, name: &str) -> Self {
        let t = cs(name);
        unsafe { sys::zui_set_icon(self.raw, t.as_ptr()) };
        self
    }
    pub fn set_tint(self, rgb: u32) -> Self {
        unsafe { sys::zui_set_tint(self.raw, rgb) };
        self
    }

    pub fn get_text(&self) -> String {
        unsafe { CStr::from_ptr(sys::zui_get_text(self.raw)).to_string_lossy().into_owned() }
    }
    pub fn value(&self) -> f64 {
        unsafe { sys::zui_get_value(self.raw) }
    }
    pub fn is_on(&self) -> bool {
        unsafe { sys::zui_get_on(self.raw) }
    }
    /// Where the last layout put it: (x, y, w, h) in page points.
    pub fn frame(&self) -> (f64, f64, f64, f64) {
        let (mut x, mut y, mut w, mut h) = (0.0, 0.0, 0.0, 0.0);
        unsafe { sys::zui_node_frame(self.raw, &mut x, &mut y, &mut w, &mut h) };
        (x, y, w, h)
    }
    /// Removes everything in it. Handles to those children must not be used after.
    pub fn clear(&self) {
        unsafe { sys::zui_clear(self.raw) }
    }
    /// Go to this page.
    pub fn push(&self) {
        unsafe { sys::zui_push(self.raw) }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::cell::Cell;
    use std::rc::Rc;
    use std::sync::Mutex;

    /* libzrnui is one app on one thread: the tests take turns */
    static ONE_AT_A_TIME: Mutex<()> = Mutex::new(());

    #[test]
    fn tap_runs_the_closure() {
        let _turn = ONE_AT_A_TIME.lock().unwrap_or_else(|e| e.into_inner());
        let app = App::new();
        let page = app.page();
        let hits = Rc::new(Cell::new(0));
        let h = hits.clone();
        let b = page.button("Tap").on_click(move || h.set(h.get() + 1));
        app.layout(page, 360, 740);
        let (x, y, w, hgt) = b.frame();
        assert!(w > 0.0 && hgt > 0.0);
        app.tap(x + w / 2.0, y + hgt / 2.0);
        assert_eq!(hits.get(), 1);
    }

    #[test]
    fn switch_reports_its_state() {
        let _turn = ONE_AT_A_TIME.lock().unwrap_or_else(|e| e.into_inner());
        let app = App::new();
        let page = app.page();
        let seen = Rc::new(Cell::new(false));
        let s2 = seen.clone();
        let sw = page.card().switch("Wi-Fi", false).on_change(move |on| s2.set(on));
        app.layout(page, 360, 740);
        let (x, y, w, h) = sw.frame();
        app.tap(x + w / 2.0, y + h / 2.0);
        assert!(seen.get());
        assert!(sw.is_on());
    }
}
