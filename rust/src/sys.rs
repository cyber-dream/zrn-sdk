//! The C ABI of libzrnui (zrnui.h), by hand: no bindgen, no dependencies.
#![allow(non_camel_case_types)]
use std::os::raw::{c_char, c_double, c_int, c_void};

#[repr(C)]
pub struct zui_app {
    _private: [u8; 0],
}
#[repr(C)]
pub struct zui_node {
    _private: [u8; 0],
}

pub type zui_cb = Option<unsafe extern "C" fn(node: *mut zui_node, data: *mut c_void)>;
pub type zui_free_fn = Option<unsafe extern "C" fn(data: *mut c_void)>;
pub type zui_fn = Option<unsafe extern "C" fn(data: *mut c_void)>;

pub const ZUI_EVENT_NONE: c_int = 0;
pub const ZUI_EVENT_CLICK: c_int = 1;
pub const ZUI_EVENT_CHANGE: c_int = 2;
pub const ZUI_EVENT_SUBMIT: c_int = 3;

extern "C" {
    pub fn zui_version() -> *const c_char;
    pub fn zui_app_new() -> *mut zui_app;
    pub fn zui_run(app: *mut zui_app, first: *mut zui_node) -> c_int;
    pub fn zui_quit(app: *mut zui_app);
    pub fn zui_app_free(app: *mut zui_app);
    pub fn zui_app_set_style(app: *mut zui_app, style: *const c_char);
    pub fn zui_app_style(app: *mut zui_app) -> *const c_char;
    pub fn zui_app_set_theme(app: *mut zui_app, theme: *const c_char);
    pub fn zui_app_set_accent(app: *mut zui_app, rgb: u32);
    pub fn zui_busy(app: *mut zui_app, busy: bool);
    pub fn zui_toast(app: *mut zui_app, text: *const c_char);
    pub fn zui_page(app: *mut zui_app) -> *mut zui_node;
    pub fn zui_page_blank(app: *mut zui_app) -> *mut zui_node;
    pub fn zui_page_fullscreen(app: *mut zui_app) -> *mut zui_node;
    pub fn zui_push(page: *mut zui_node);
    pub fn zui_back(app: *mut zui_app);
    pub fn zui_column(parent: *mut zui_node) -> *mut zui_node;
    pub fn zui_row(parent: *mut zui_node) -> *mut zui_node;
    pub fn zui_card(parent: *mut zui_node) -> *mut zui_node;
    pub fn zui_section(parent: *mut zui_node, title: *const c_char) -> *mut zui_node;
    pub fn zui_space(parent: *mut zui_node, points: c_double) -> *mut zui_node;
    pub fn zui_text(parent: *mut zui_node, text: *const c_char) -> *mut zui_node;
    pub fn zui_heading(parent: *mut zui_node, text: *const c_char) -> *mut zui_node;
    pub fn zui_caption(parent: *mut zui_node, text: *const c_char) -> *mut zui_node;
    pub fn zui_button(parent: *mut zui_node, label: *const c_char, cb: zui_cb, data: *mut c_void) -> *mut zui_node;
    pub fn zui_item(parent: *mut zui_node, title: *const c_char, subtitle: *const c_char, cb: zui_cb,
        data: *mut c_void) -> *mut zui_node;
    pub fn zui_switch(parent: *mut zui_node, title: *const c_char, on: bool, cb: zui_cb, data: *mut c_void) -> *mut zui_node;
    pub fn zui_slider(parent: *mut zui_node, title: *const c_char, value: c_double, cb: zui_cb,
        data: *mut c_void) -> *mut zui_node;
    pub fn zui_progress(parent: *mut zui_node, value: c_double) -> *mut zui_node;
    pub fn zui_field(parent: *mut zui_node, placeholder: *const c_char, cb: zui_cb, data: *mut c_void) -> *mut zui_node;
    pub fn zui_set_text(node: *mut zui_node, text: *const c_char);
    pub fn zui_title_panel(page: *mut zui_node) -> *mut zui_node;
    pub fn zui_content_panel(parent: *mut zui_node) -> *mut zui_node;
    pub fn zui_scroll(parent: *mut zui_node) -> *mut zui_node;
    pub fn zui_title(parent: *mut zui_node, text: *const c_char) -> *mut zui_node;
    pub fn zui_header(parent: *mut zui_node, text: *const c_char) -> *mut zui_node;
    pub fn zui_get_text(node: *mut zui_node) -> *const c_char;
    pub fn zui_set_detail(node: *mut zui_node, text: *const c_char);
    pub fn zui_set_subtitle(node: *mut zui_node, text: *const c_char);
    pub fn zui_set_value(node: *mut zui_node, value: c_double);
    pub fn zui_get_value(node: *mut zui_node) -> c_double;
    pub fn zui_set_on(node: *mut zui_node, on: bool);
    pub fn zui_get_on(node: *mut zui_node) -> bool;
    pub fn zui_set_enabled(node: *mut zui_node, enabled: bool);
    pub fn zui_set_visible(node: *mut zui_node, visible: bool);
    pub fn zui_set_variant(node: *mut zui_node, variant: c_int);
    pub fn zui_set_flex(node: *mut zui_node, flex: c_double);
    pub fn zui_set_icon(node: *mut zui_node, icon: *const c_char);
    pub fn zui_set_tint(node: *mut zui_node, rgb: u32);
    pub fn zui_set_handler(node: *mut zui_node, cb: zui_cb, data: *mut c_void, free_data: zui_free_fn);
    pub fn zui_event(node: *mut zui_node) -> c_int;
    pub fn zui_clear(node: *mut zui_node);
    pub fn zui_destroy(node: *mut zui_node);
    pub fn zui_timer(app: *mut zui_app, ms: c_int, repeat: bool, f: zui_fn, data: *mut c_void) -> c_int;
    pub fn zui_timer_cancel(app: *mut zui_app, id: c_int);
    pub fn zui_post(app: *mut zui_app, f: zui_fn, data: *mut c_void);
    pub fn zui_render_png(app: *mut zui_app, page: *mut zui_node, path: *const c_char, w: c_int, h: c_int,
        scale: c_int) -> c_int;
    pub fn zui_layout(app: *mut zui_app, page: *mut zui_node, w: c_int, h: c_int);
    pub fn zui_inject_touch(app: *mut zui_app, phase: c_int, x: c_double, y: c_double);
    pub fn zui_node_frame(node: *mut zui_node, x: *mut c_double, y: *mut c_double, w: *mut c_double, h: *mut c_double);
}
