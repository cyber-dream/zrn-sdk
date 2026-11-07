/**
 * Kestrel - window.c
 * A Wayland window (xdg-shell, wl_shm) for a root.
 */

#define _GNU_SOURCE
#include "internal.h"

#ifdef KESTREL_HAVE_WAYLAND

#include <poll.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include "../protocol/xdg-shell-client-protocol.h"

typedef struct buffer {
	struct wl_buffer* buffer;
	uint8_t* data;
	size_t size;
	int busy;
} buffer_t;

typedef struct window {
	struct wl_display* display;
	struct wl_registry* registry;
	struct wl_compositor* compositor;
	struct wl_shm* shm;
	struct xdg_wm_base* wm_base;
	struct wl_seat* seat;
	struct wl_pointer* pointer;
	struct wl_keyboard* keyboard;
	struct wl_touch* touch;
	struct wl_surface* surface;
	struct xdg_surface* xdg_surface;
	struct xdg_toplevel* toplevel;
	buffer_t buffers[2];
	/** The OpenGL ES 2 renderer when the window has one: no wl_shm buffers then. */
	kst_gl_t* gl;
	int gl_w, gl_h;
	int buf_w, buf_h;
	Kid root;
	int scale;
	int width, height; /* logical */
	int configured, closed, pending_w, pending_h;
	struct xkb_context* xkb;
	struct xkb_keymap* keymap;
	struct xkb_state* xkb_state;
	int repeat_key; /* evdev key held, 0 none */
	float repeat_timer;
	int repeating;
	int touch_id;
} window_t;

static window_t* g_window;

/* ---- buffers ---- */

static void buffer_release(void* data, struct wl_buffer* b) { (void)b; ((buffer_t*)data)->busy = 0; }
static const struct wl_buffer_listener buffer_listener = { .release = buffer_release };

static void buffers_free(window_t* w) {
	int i;
	for (i = 0; i < 2; i++) {
		if (w->buffers[i].buffer) wl_buffer_destroy(w->buffers[i].buffer);
		if (w->buffers[i].data) munmap(w->buffers[i].data, w->buffers[i].size);
		memset(&w->buffers[i], 0, sizeof w->buffers[i]);
	}
}

static int buffers_make(window_t* w, int pw, int ph) {
	int i;
	size_t size = (size_t)pw * (size_t)ph * 4;
	buffers_free(w);
	for (i = 0; i < 2; i++) {
		struct wl_shm_pool* pool;
		int fd = memfd_create("kestrel", MFD_CLOEXEC);
		void* data;
		if (fd < 0 || ftruncate(fd, (off_t)size) < 0) {
			if (fd >= 0) close(fd);
			return 0;
		}
		data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (data == MAP_FAILED) { close(fd); return 0; }
		pool = wl_shm_create_pool(w->shm, fd, (int32_t)size);
		w->buffers[i].buffer = wl_shm_pool_create_buffer(pool, 0, pw, ph, pw * 4, WL_SHM_FORMAT_ARGB8888);
		wl_buffer_add_listener(w->buffers[i].buffer, &buffer_listener, &w->buffers[i]);
		wl_shm_pool_destroy(pool);
		close(fd);
		w->buffers[i].data = (uint8_t*)data;
		w->buffers[i].size = size;
	}
	w->buf_w = pw;
	w->buf_h = ph;
	return 1;
}

/* One frame on the GPU: painted only when something changed, then swapped. */
static void present_gl(window_t* w) {
	kst_root_t* r = kst_root_lookup(w->root);
	kst_painter_t p;
	int pw = w->width * w->scale, ph = w->height * w->scale, painted;
	if (!r) return;
	if (pw != w->gl_w || ph != w->gl_h) {
		/* the buffer of a new size is blank: draw everything again */
		w->gl_w = pw;
		w->gl_h = ph;
		r->paint_dirty = 1;
	}
	kst_gl_begin(w->gl, &p, pw, ph, (float)w->scale);
	painted = kst_root_paint(r, &p);
	kst_gl_end(w->gl, painted);
}

static void present(window_t* w) {
	KFrame f;
	if (w->gl) { present_gl(w); return; }
	buffer_t* b = NULL;
	int i, x, y;
	int pw = w->width * w->scale, ph = w->height * w->scale;
	if (pw != w->buf_w || ph != w->buf_h) {
		if (!buffers_make(w, pw, ph)) return;
		w->buffers[0].busy = w->buffers[1].busy = 0;
	}
	for (i = 0; i < 2; i++) if (!w->buffers[i].busy) { b = &w->buffers[i]; break; }
	if (!b) return; /* both still with the compositor: next round */
	if (!Kroot_render(w->root, &f) && !w->configured) return;
	for (y = 0; y < ph && y < f.height; y++) {
		const uint8_t* src = f.pixels + (size_t)y * (size_t)f.stride;
		uint8_t* dst = b->data + (size_t)y * (size_t)pw * 4;
		for (x = 0; x < pw && x < f.width; x++) {
			dst[x * 4 + 0] = src[x * 4 + 2];
			dst[x * 4 + 1] = src[x * 4 + 1];
			dst[x * 4 + 2] = src[x * 4 + 0];
			dst[x * 4 + 3] = 255;
		}
	}
	b->busy = 1;
	wl_surface_attach(w->surface, b->buffer, 0, 0);
	wl_surface_damage_buffer(w->surface, 0, 0, pw, ph);
	wl_surface_commit(w->surface);
}

/* ---- xdg-shell ---- */

static void wm_ping(void* d, struct xdg_wm_base* b, uint32_t serial) { (void)d; xdg_wm_base_pong(b, serial); }
static const struct xdg_wm_base_listener wm_base_listener = { .ping = wm_ping };

static void surface_configure(void* d, struct xdg_surface* s, uint32_t serial) {
	window_t* w = (window_t*)d;
	xdg_surface_ack_configure(s, serial);
	if (w->pending_w > 0 && w->pending_h > 0 && (w->pending_w != w->width || w->pending_h != w->height)) {
		w->width = w->pending_w;
		w->height = w->pending_h;
		Kroot_set_size(w->root, w->width, w->height);
	}
	w->configured = 1;
	Kroot_set_scale(w->root, (float)w->scale);
	present(w);
}
static const struct xdg_surface_listener xdg_surface_listener = { .configure = surface_configure };

static void toplevel_configure(void* d, struct xdg_toplevel* t, int32_t w_, int32_t h_, struct wl_array* states) {
	window_t* w = (window_t*)d;
	(void)t; (void)states;
	w->pending_w = w_;
	w->pending_h = h_;
}
static void toplevel_close(void* d, struct xdg_toplevel* t) { (void)t; ((window_t*)d)->closed = 1; }
static const struct xdg_toplevel_listener toplevel_listener = { .configure = toplevel_configure, .close = toplevel_close };

/* ---- pointer and touch ---- */

static void ptr_enter(void* d, struct wl_pointer* p, uint32_t s, struct wl_surface* sf, wl_fixed_t x, wl_fixed_t y) {
	(void)p; (void)s; (void)sf;
	Kroot_inject_pointer(((window_t*)d)->root, K_POINTER_MOVE, (float)wl_fixed_to_double(x), (float)wl_fixed_to_double(y));
}
static void ptr_leave(void* d, struct wl_pointer* p, uint32_t s, struct wl_surface* sf) { (void)p; (void)s; (void)sf; Kroot_inject_pointer(((window_t*)d)->root, K_POINTER_LEAVE, 0, 0); }
static void ptr_motion(void* d, struct wl_pointer* p, uint32_t t, wl_fixed_t x, wl_fixed_t y) {
	(void)p; (void)t;
	Kroot_inject_pointer(((window_t*)d)->root, K_POINTER_MOVE, (float)wl_fixed_to_double(x), (float)wl_fixed_to_double(y));
}
static float g_px, g_py;
static void ptr_motion_track(void* d, struct wl_pointer* p, uint32_t t, wl_fixed_t x, wl_fixed_t y) {
	g_px = (float)wl_fixed_to_double(x);
	g_py = (float)wl_fixed_to_double(y);
	ptr_motion(d, p, t, x, y);
}
static void ptr_button(void* d, struct wl_pointer* p, uint32_t serial, uint32_t t, uint32_t button, uint32_t state) {
	(void)p; (void)serial; (void)t;
	if (button != 0x110) return; /* BTN_LEFT */
	Kroot_inject_pointer(((window_t*)d)->root, state == WL_POINTER_BUTTON_STATE_PRESSED ? K_POINTER_DOWN : K_POINTER_UP, g_px, g_py);
}
static void ptr_axis(void* d, struct wl_pointer* p, uint32_t t, uint32_t axis, wl_fixed_t v) {
	(void)p; (void)t;
	if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) Kroot_inject_wheel(((window_t*)d)->root, (float)-wl_fixed_to_double(v) / 10.0f);
}
static void ptr_frame(void* d, struct wl_pointer* p) { (void)d; (void)p; }
static void ptr_axis_source(void* d, struct wl_pointer* p, uint32_t s) { (void)d; (void)p; (void)s; }
static void ptr_axis_stop(void* d, struct wl_pointer* p, uint32_t t, uint32_t a) { (void)d; (void)p; (void)t; (void)a; }
static void ptr_axis_discrete(void* d, struct wl_pointer* p, uint32_t a, int32_t v) { (void)d; (void)p; (void)a; (void)v; }
static const struct wl_pointer_listener pointer_listener = {
	.enter = ptr_enter, .leave = ptr_leave, .motion = ptr_motion_track, .button = ptr_button, .axis = ptr_axis,
	.frame = ptr_frame, .axis_source = ptr_axis_source, .axis_stop = ptr_axis_stop, .axis_discrete = ptr_axis_discrete
};

static void touch_down(void* d, struct wl_touch* t, uint32_t serial, uint32_t time, struct wl_surface* s, int32_t id, wl_fixed_t x, wl_fixed_t y) {
	window_t* w = (window_t*)d;
	(void)t; (void)serial; (void)time; (void)s;
	if (w->touch_id >= 0) return;
	w->touch_id = id;
	g_px = (float)wl_fixed_to_double(x);
	g_py = (float)wl_fixed_to_double(y);
	Kroot_inject_pointer(w->root, K_POINTER_MOVE, g_px, g_py);
	Kroot_inject_pointer(w->root, K_POINTER_DOWN, g_px, g_py);
}
static void touch_up(void* d, struct wl_touch* t, uint32_t serial, uint32_t time, int32_t id) {
	window_t* w = (window_t*)d;
	(void)t; (void)serial; (void)time;
	if (id != w->touch_id) return;
	w->touch_id = -1;
	Kroot_inject_pointer(w->root, K_POINTER_UP, g_px, g_py);
	Kroot_inject_pointer(w->root, K_POINTER_LEAVE, 0, 0);
}
static void touch_motion(void* d, struct wl_touch* t, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
	window_t* w = (window_t*)d;
	(void)t; (void)time;
	if (id != w->touch_id) return;
	g_px = (float)wl_fixed_to_double(x);
	g_py = (float)wl_fixed_to_double(y);
	Kroot_inject_pointer(w->root, K_POINTER_MOVE, g_px, g_py);
}
static void touch_frame(void* d, struct wl_touch* t) { (void)d; (void)t; }
static void touch_cancel(void* d, struct wl_touch* t) {
	window_t* w = (window_t*)d;
	(void)t;
	if (w->touch_id >= 0) Kroot_inject_pointer(w->root, K_POINTER_UP, -1000, -1000);
	w->touch_id = -1;
}
static void touch_shape(void* d, struct wl_touch* t, int32_t i, wl_fixed_t a, wl_fixed_t b) { (void)d; (void)t; (void)i; (void)a; (void)b; }
static void touch_orientation(void* d, struct wl_touch* t, int32_t i, wl_fixed_t o) { (void)d; (void)t; (void)i; (void)o; }
static const struct wl_touch_listener touch_listener = {
	.down = touch_down, .up = touch_up, .motion = touch_motion, .frame = touch_frame, .cancel = touch_cancel,
	.shape = touch_shape, .orientation = touch_orientation
};

/* ---- keyboard ---- */

static void kb_keymap(void* d, struct wl_keyboard* k, uint32_t format, int32_t fd, uint32_t size) {
	window_t* w = (window_t*)d;
	char* map;
	(void)k;
	if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) { close(fd); return; }
	map = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (map == MAP_FAILED) return;
	if (w->xkb_state) xkb_state_unref(w->xkb_state);
	if (w->keymap) xkb_keymap_unref(w->keymap);
	w->keymap = xkb_keymap_new_from_string(w->xkb, map, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	munmap(map, size);
	w->xkb_state = w->keymap ? xkb_state_new(w->keymap) : NULL;
}
static void kb_enter(void* d, struct wl_keyboard* k, uint32_t s, struct wl_surface* sf, struct wl_array* keys) { (void)d; (void)k; (void)s; (void)sf; (void)keys; }
static void kb_leave(void* d, struct wl_keyboard* k, uint32_t s, struct wl_surface* sf) { (void)k; (void)s; (void)sf; ((window_t*)d)->repeat_key = 0; }

static KKey map_key(xkb_keysym_t sym, int ctrl) {
	switch (sym) {
		case XKB_KEY_BackSpace: return K_KEY_BACKSPACE;
		case XKB_KEY_Tab: case XKB_KEY_ISO_Left_Tab: return K_KEY_TAB;
		case XKB_KEY_Return: case XKB_KEY_KP_Enter: return K_KEY_ENTER;
		case XKB_KEY_Escape: return K_KEY_ESCAPE;
		case XKB_KEY_space: return K_KEY_SPACE;
		case XKB_KEY_Page_Up: return K_KEY_PAGE_UP;
		case XKB_KEY_Page_Down: return K_KEY_PAGE_DOWN;
		case XKB_KEY_End: return K_KEY_END;
		case XKB_KEY_Home: return K_KEY_HOME;
		case XKB_KEY_Left: return K_KEY_LEFT;
		case XKB_KEY_Up: return K_KEY_UP;
		case XKB_KEY_Right: return K_KEY_RIGHT;
		case XKB_KEY_Down: return K_KEY_DOWN;
		case XKB_KEY_Delete: return K_KEY_DELETE;
		default: break;
	}
	if (ctrl) {
		if (sym == XKB_KEY_a || sym == XKB_KEY_A) return K_KEY_A;
		if (sym == XKB_KEY_c || sym == XKB_KEY_C) return K_KEY_C;
		if (sym == XKB_KEY_v || sym == XKB_KEY_V) return K_KEY_V;
		if (sym == XKB_KEY_x || sym == XKB_KEY_X) return K_KEY_X;
	}
	return K_KEY_NONE;
}

static int modifiers(window_t* w) {
	int m = 0;
	if (xkb_state_mod_name_is_active(w->xkb_state, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_EFFECTIVE) > 0) m |= K_MOD_SHIFT;
	if (xkb_state_mod_name_is_active(w->xkb_state, XKB_MOD_NAME_CTRL, XKB_STATE_MODS_EFFECTIVE) > 0) m |= K_MOD_CTRL;
	if (xkb_state_mod_name_is_active(w->xkb_state, XKB_MOD_NAME_ALT, XKB_STATE_MODS_EFFECTIVE) > 0) m |= K_MOD_ALT;
	return m;
}

static void key_press(window_t* w, uint32_t evdev) {
	xkb_keycode_t code = evdev + 8;
	xkb_keysym_t sym = xkb_state_key_get_one_sym(w->xkb_state, code);
	int mods = modifiers(w);
	KKey key = map_key(sym, (mods & K_MOD_CTRL) != 0);
	if (key != K_KEY_NONE) {
		int handled = Kroot_inject_key(w->root, key, mods, 1);
		/* Back on the first page of a Frame leaves the app, as on Windows Phone */
		if (key == K_KEY_ESCAPE && !handled && Kelement_is_a(Kroot_get_visual(w->root), K_CLASS_FRAME)) w->closed = 1;
	}
	if (!(mods & (K_MOD_CTRL | K_MOD_ALT))) {
		char text[16];
		int n = xkb_state_key_get_utf8(w->xkb_state, code, text, sizeof text);
		if (n > 0 && (unsigned char)text[0] >= 0x20 && text[0] != 0x7f) Kroot_inject_text(w->root, text);
	}
}

static void kb_key(void* d, struct wl_keyboard* k, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
	window_t* w = (window_t*)d;
	(void)k; (void)serial; (void)time;
	if (!w->xkb_state) return;
	if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
		key_press(w, key);
		if (xkb_keymap_key_repeats(w->keymap, key + 8)) {
			w->repeat_key = (int)key;
			w->repeat_timer = 0;
			w->repeating = 0;
		}
	} else {
		xkb_keysym_t sym = xkb_state_key_get_one_sym(w->xkb_state, key + 8);
		KKey kk = map_key(sym, 0);
		if (kk != K_KEY_NONE) Kroot_inject_key(w->root, kk, modifiers(w), 0);
		if (w->repeat_key == (int)key) w->repeat_key = 0;
	}
}
static void kb_modifiers(void* d, struct wl_keyboard* k, uint32_t serial, uint32_t depressed, uint32_t latched, uint32_t locked, uint32_t group) {
	window_t* w = (window_t*)d;
	(void)k; (void)serial;
	if (w->xkb_state) xkb_state_update_mask(w->xkb_state, depressed, latched, locked, 0, 0, group);
}
static void kb_repeat_info(void* d, struct wl_keyboard* k, int32_t rate, int32_t delay) { (void)d; (void)k; (void)rate; (void)delay; }
static const struct wl_keyboard_listener keyboard_listener = {
	.keymap = kb_keymap, .enter = kb_enter, .leave = kb_leave, .key = kb_key, .modifiers = kb_modifiers, .repeat_info = kb_repeat_info
};

/* ---- seat and registry ---- */

static void seat_caps(void* d, struct wl_seat* s, uint32_t caps) {
	window_t* w = (window_t*)d;
	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !w->pointer) { w->pointer = wl_seat_get_pointer(s); wl_pointer_add_listener(w->pointer, &pointer_listener, w); }
	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !w->keyboard) { w->keyboard = wl_seat_get_keyboard(s); wl_keyboard_add_listener(w->keyboard, &keyboard_listener, w); }
	if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !w->touch) { w->touch = wl_seat_get_touch(s); wl_touch_add_listener(w->touch, &touch_listener, w); }
}
static void seat_name(void* d, struct wl_seat* s, const char* n) { (void)d; (void)s; (void)n; }
static const struct wl_seat_listener seat_listener = { .capabilities = seat_caps, .name = seat_name };

static void reg_global(void* d, struct wl_registry* r, uint32_t name, const char* iface, uint32_t version) {
	window_t* w = (window_t*)d;
	if (!strcmp(iface, wl_compositor_interface.name)) w->compositor = (struct wl_compositor*)wl_registry_bind(r, name, &wl_compositor_interface, version < 4 ? version : 4);
	else if (!strcmp(iface, wl_shm_interface.name)) w->shm = (struct wl_shm*)wl_registry_bind(r, name, &wl_shm_interface, 1);
	else if (!strcmp(iface, xdg_wm_base_interface.name)) w->wm_base = (struct xdg_wm_base*)wl_registry_bind(r, name, &xdg_wm_base_interface, 1);
	else if (!strcmp(iface, wl_seat_interface.name)) {
		w->seat = (struct wl_seat*)wl_registry_bind(r, name, &wl_seat_interface, version < 5 ? version : 5);
		wl_seat_add_listener(w->seat, &seat_listener, w);
	}
}
static void reg_remove(void* d, struct wl_registry* r, uint32_t name) { (void)d; (void)r; (void)name; }
static const struct wl_registry_listener registry_listener = { .global = reg_global, .global_remove = reg_remove };

static double now_ms(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

void Kwindow_quit(void) { if (g_window) g_window->closed = 1; }

int Kwindow_run(Kid root, const char* title) {
	window_t w;
	const char* scale_env = getenv("KESTREL_SCALE");
	double last;
	int ok;
	kst_root_t* r = kst_root_lookup(root);
	if (!r) { kst_error(K_ERROR_INVALID_ID, "invalid root handle"); return K_FALSE; }
	memset(&w, 0, sizeof w);
	w.root = root;
	/* a root of 0 x 0 takes its size from the window: the first configure of the compositor, 480 x 800 until then */
	w.width = r->width > 0 ? r->width : 480;
	w.height = r->height > 0 ? r->height : 800;
	if (r->width <= 0) Kroot_set_size(root, w.width, w.height);
	w.scale = scale_env && atoi(scale_env) > 0 ? atoi(scale_env) : 1;
	w.touch_id = -1;
	w.display = wl_display_connect(NULL);
	if (!w.display) { kst_error(K_ERROR_IO, "no Wayland display (is WAYLAND_DISPLAY set?)"); return K_FALSE; }
	w.xkb = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	w.registry = wl_display_get_registry(w.display);
	wl_registry_add_listener(w.registry, &registry_listener, &w);
	wl_display_roundtrip(w.display);
	if (!w.compositor || !w.shm || !w.wm_base) {
		kst_error(K_ERROR_IO, "the compositor lacks wl_compositor, wl_shm or xdg_wm_base");
		wl_display_disconnect(w.display);
		return K_FALSE;
	}
	g_window = &w;
	xdg_wm_base_add_listener(w.wm_base, &wm_base_listener, &w);
	w.surface = wl_compositor_create_surface(w.compositor);
	if (w.scale > 1) wl_surface_set_buffer_scale(w.surface, w.scale);
	w.xdg_surface = xdg_wm_base_get_xdg_surface(w.wm_base, w.surface);
	xdg_surface_add_listener(w.xdg_surface, &xdg_surface_listener, &w);
	w.toplevel = xdg_surface_get_toplevel(w.xdg_surface);
	xdg_toplevel_add_listener(w.toplevel, &toplevel_listener, &w);
	xdg_toplevel_set_title(w.toplevel, title ? title : "Kestrel");
	xdg_toplevel_set_app_id(w.toplevel, title ? title : "kestrel");
	wl_surface_commit(w.surface);
	Kroot_set_scale(root, (float)w.scale);
	/* the GPU when asked or when it can: the software painter and wl_shm stay as the safe mode */
	ok = 1;
	if (r->renderer != K_RENDERER_SOFTWARE) {
		w.gl = kst_gl_create_window(w.display, w.surface, w.width * w.scale, w.height * w.scale);
		if (!w.gl && r->renderer == K_RENDERER_GPU) {
			kst_error(K_ERROR_IO, "no OpenGL ES 2 window (K_RENDERER_GPU): %s", Kget_error_message());
			ok = 0;
			w.closed = 1;
		}
	}
	kst_note_renderer(w.gl ? "gles2" : "software");
	r->gpu_window = w.gl != NULL;
	/* a first page that went into the Frame before this window existed swings in now */
	kst_root_start_intro(r);
	last = now_ms();
	while (!w.closed) {
		struct pollfd pfd;
		double t, dt;
		while (wl_display_prepare_read(w.display) != 0) wl_display_dispatch_pending(w.display);
		if (wl_display_flush(w.display) < 0) { wl_display_cancel_read(w.display); break; }
		pfd.fd = wl_display_get_fd(w.display);
		pfd.events = POLLIN;
		if (poll(&pfd, 1, 16) > 0 && (pfd.revents & POLLIN)) {
			if (wl_display_read_events(w.display) < 0) break;
		} else {
			wl_display_cancel_read(w.display);
		}
		wl_display_dispatch_pending(w.display);
		if (!kst_root_lookup(root)) break;
		t = now_ms();
		dt = t - last;
		last = t;
		if (dt > 0) Kroot_tick(root, (float)dt);
		if (w.repeat_key) {
			w.repeat_timer += (float)dt;
			if (!w.repeating && w.repeat_timer >= 500) { w.repeating = 1; w.repeat_timer = 0; key_press(&w, (uint32_t)w.repeat_key); }
			else if (w.repeating && w.repeat_timer >= 33) { w.repeat_timer -= 33; key_press(&w, (uint32_t)w.repeat_key); }
		}
		if (w.configured) present(&w);
	}
	g_window = NULL;
	buffers_free(&w);
	if (w.gl) kst_gl_destroy(w.gl);
	if (kst_root_lookup(root)) kst_root_lookup(root)->gpu_window = 0;
	if (w.xkb_state) xkb_state_unref(w.xkb_state);
	if (w.keymap) xkb_keymap_unref(w.keymap);
	xkb_context_unref(w.xkb);
	xdg_toplevel_destroy(w.toplevel);
	xdg_surface_destroy(w.xdg_surface);
	wl_surface_destroy(w.surface);
	wl_display_disconnect(w.display);
	return ok;
}

#else

int Kwindow_run(Kid root, const char* title) {
	(void)root; (void)title;
	kst_error(K_ERROR_IO, "Kestrel was built without Wayland support (wayland-client and xkbcommon were not found)");
	return K_FALSE;
}

void Kwindow_quit(void) {}

#endif
