// SPDX-License-Identifier: LGPL-2.1-or-later
/* libzrnui internals: the node tree, the GPU batcher, text and the styles. */
#ifndef ZUI_INTERNAL_H
#define ZUI_INTERNAL_H

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "zrnui.h"

#define ZUI_EXPORT __attribute__((visibility("default")))

/* ------------------------------------------------------------------ gfx --- */
/* A transform applied to every corner before the GPU's perspective divide:
 * points in, points out, z toward the viewer. Row-major 4x4 on (x, y, z, 1). */
struct zg_xf { float m[16]; };
void zg_xf_identity(struct zg_xf *xf);
bool zg_xf_is_identity(const struct zg_xf *xf);
/* each of these applies after what xf already does */
void zg_xf_translate(struct zg_xf *xf, float x, float y, float z);
void zg_xf_scale_about(struct zg_xf *xf, float s, float cx, float cy);
void zg_xf_rotate_y(struct zg_xf *xf, float deg, float px);	/* about the line x = px */
void zg_xf_rotate_x(struct zg_xf *xf, float deg, float py);	/* about the line y = py */
void zg_xf_apply(const struct zg_xf *xf, float x, float y, float out[3]);

bool zg_init(void);		/* in a current GLES2 context */
void zg_fini(void);
unsigned zg_generation(void);	/* bumps on every zg_init: glyphs must be uploaded again */
void zg_begin(int fb_w, int fb_h, float scale, uint32_t clear_rgba);
void zg_end(void);
float zui_gfx_scale(void);	/* pixels per point in the frame being drawn */
void zg_set_xf(const struct zg_xf *xf);	/* NULL: none */
const struct zg_xf *zg_get_xf(void);
void zg_set_alpha(float a);			/* multiplies everything drawn */
float zg_get_alpha(void);
unsigned zg_tex_new(int w, int h, const uint8_t *rgba, bool nearest);
void zg_tex_free(unsigned tex);
/* a texture over a rounded box; crop 0..1 of the picture's width and height, centred */
unsigned zg_shader_compile(const char *fragment_source, char *error, size_t error_size);	/* 0: did not compile */
void zg_shader_free(unsigned program);
void zg_shader_rect(unsigned program, float x, float y, float w, float h, float r, float time,
	int nuniforms, const char *const *names, const float (*values)[4]);
void zg_image(unsigned tex, float x, float y, float w, float h, float r, float crop_x, float crop_y);
/* DotMatrix: everything filled comes out as dots `pitch` points apart on one
 * lattice (0: off); a picture as dots of that colour at its brightness */
void zg_set_dots(float pitch, uint32_t main_rgb);
void zg_set_dot_origin(float y);
void zg_dot(float cx, float cy, float radius, uint32_t rgba);
void zg_use_row_batcher(void);
unsigned zg_main_generation(void);
bool zg_ready_here(void);
void zg_row_begin(unsigned tex, int w, int h, float scale);
void zg_row_end(void);
unsigned zg_row_tex(int w, int h);
void zg_row(unsigned tex, float x, float y, float w, float h);
void zg_clip(float x, float y, float w, float h);	/* points; only without a transform */
void zg_noclip(void);
void zg_rect(float x, float y, float w, float h, float r, uint32_t rgba);
void zg_border(float x, float y, float w, float h, float r, float width, uint32_t rgba);
void zg_shadow(float x, float y, float w, float h, float r, float blur, uint32_t rgba);
void zg_line(float x0, float y0, float x1, float y1, float width, uint32_t rgba);
void zg_wedge(float x, float y, float w, float h, bool right, bool bottom, uint32_t rgba);
void zg_circle(float cx, float cy, float radius, uint32_t rgba);
void zg_ring(float cx, float cy, float radius, float width, uint32_t rgba);
/* an arc of a circle, as short strokes: angles in degrees, 0 = right, clockwise */
void zg_arc(float cx, float cy, float radius, float a0, float a1, float width, uint32_t rgba);
/* the glyph atlas: an 8-bit coverage texture shared by all text */
#define ZG_ATLAS 2048
bool zg_atlas_alloc(int w, int h, int *x, int *y);
void zg_atlas_upload(int x, int y, int w, int h, const uint8_t *pixels, int pitch);
void zg_atlas_reset(void);
void zg_glyph(float x, float y, float w, float h, float u0, float v0, float u1, float v1, uint32_t rgba);

/* colours: 0xRRGGBBAA */
static inline uint32_t zc_rgb(uint32_t rgb) { return (rgb << 8) | 0xff; }
static inline uint32_t zc_alpha(uint32_t c, float a) {
	uint32_t al = (uint32_t)((c & 0xff) * (a < 0 ? 0 : a > 1 ? 1 : a) + 0.5f);
	return (c & 0xffffff00u) | al;
}
uint32_t zc_mix(uint32_t a, uint32_t b, float t);	/* a to b, alpha too */
float zc_luma(uint32_t c);

/* ----------------------------------------------------------------- text --- */
enum zt_font { ZT_REGULAR, ZT_MEDIUM, ZT_STRONG, ZT_LIGHT, ZT_FONTS };
struct zt_glyph { uint16_t face; uint32_t gid; float x, y; };	/* y: the baseline */
struct zt_layout {
	struct zt_glyph *g;
	int n, lines;
	float w, h, line_h, ascent, size;
};
/* what each role is in the style in use: font files or fontconfig names */
void zt_use_row_cache(void);
void zui_lock(void);
void zui_unlock(void);
void zt_set_fonts(const char *const files[ZT_FONTS], float line_spacing);
/* DotMatrix's 3x5 dot type in place of all fonts (and of shaping) */
void zt_set_dot_type(bool on);
/* max_w <= 0: one line, no wrapping; max_lines 1 with max_w > 0: ellipsis */
struct zt_layout *zt_layout_make(const char *utf8, int font, float size, float max_w, int max_lines);
void zt_layout_free(struct zt_layout *layout);
bool zt_icon(uint32_t code, float x, float y, float size, uint32_t rgba);
void zt_draw(const struct zt_layout *layout, float x, float y, uint32_t rgba);
void zt_draw_clipped(const struct zt_layout *layout, float x, float y, uint32_t rgba, float x0, float x1);
float zt_width(const char *utf8, int font, float size);
float zt_line_height(float size);
void zt_fini(void);

/* a node's cached text: laid out again only when something it depends on changes */
struct zt_slot {
	struct zt_layout *layout;
	char *text;
	int font, max_lines;
	float size, max_w;
	unsigned style_gen;
};
struct zt_layout *zt_slot_get(struct zt_slot *slot, const char *text, int font, float size,
		float max_w, int max_lines);
void zt_slot_free(struct zt_slot *slot);

/* ------------------------------------------------------------- the tree --- */
enum zui_kind {
	ZK_PAGE, ZK_COLUMN, ZK_ROW, ZK_CARD, ZK_SECTION_LABEL, ZK_SPACE,
	ZK_TEXT, ZK_HEADING, ZK_CAPTION, ZK_BUTTON, ZK_ITEM, ZK_SWITCH,
	ZK_SLIDER, ZK_PROGRESS, ZK_FIELD, ZK_IMAGE, ZK_LIST, ZK_PIVOT, ZK_FOOTER, ZK_BUBBLE,
	ZK_CHECK, ZK_RADIO, ZK_PICKER, ZK_RING, ZK_TILES, ZK_TILE, ZK_HUB, ZK_HUBSEC,
	ZK_SCROLL,	/* a scroll viewer: its things go on under it, as far as it reaches, and a finger moves them */
	ZK_TITLE,	/* the small title: the application's, a pivot's, a hub section's */
	ZK_HEADER,	/* the large title: a page's, a hub's */
	ZK_FRAME,	/* the app's one root: the pages are its children, whatever else is in it is over them all */
	ZK_CONTENT_PANEL,	/* a page's ContentPanel: a column with the margin and the gaps; the root has none */
	ZK_TITLE_PANEL,	/* a page's TitlePanel: only holds the two titles, the style draws them */
	ZK_PIVOT_ITEM,	/* a pivot's item: a column that scrolls (an heir of the scroll container) */
};

struct zui_pixmap { int w, h, refs; uint8_t *rgba; unsigned tex, gen; };

#define ZUI_LIST_SLOTS 48

/* something moving from one value to another over time */
struct zanim { float from, to; uint32_t start, ms; float v0, w; };	/* w: a spring, per ms; 0: eased */
float zanim_value(const struct zanim *a, uint32_t now, bool *running);
void zanim_go(struct zanim *a, float to, uint32_t ms, uint32_t now);
void zanim_set(struct zanim *a, float value);

struct zui_rect { float x, y, w, h; };
struct zui_node;
struct zui_node *zui_page_new_kind(zui_app *app, int kind);
float layout_hub(zui_node *hub);	/* widgets/hub.c */
struct zui_node *zui_page_of(struct zui_node *n);	/* the page a node is in (or is): what a hub or a pivot made from the app stands for */

/* What a kind of node is: the size of its struct (zui_node is the first member of every
 * subtype, as the base in front of what is its own) and what it does that the base cannot. One
 * per kind, in class.c: a new kind of node is a struct, a class and nothing in the core. */
struct zui_class {
	const char *name;
	size_t size;				/* of the node's struct: at least sizeof(struct zui_node) */
	void (*destroy)(zui_node *n);		/* frees what the subtype holds (not the node itself) */
	bool is_page;				/* a page, or a kind that is one (a pivot, a hub): its struct starts with zui_page */
	bool scrolls;				/* a scroll container or one of its heirs: its struct is a zui_scroll */
	/* what the core asks of a kind, when it is not one of the core's own (all optional) */
	float (*layout)(zui_node *n, float x, float y, float w);	/* its height; it sets what is its own */
	void (*draw)(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now);
	bool (*interactive)(const zui_node *n);	/* a finger can press it */
	void (*activate)(zui_app *app, zui_node *n);	/* the lift of a tap on it */
};
const struct zui_class *zui_class_of(int kind);
const struct zui_class *zui_core_class(int kind);	/* core.c: the kinds it lays out and draws itself */
void zui_list_destroy(zui_node *n);
void suggest_free(zui_node *n);

struct zui_node {
	zui_app *app;
	int kind;
	const struct zui_class *cls;
	zui_node *parent, *first, *last, *next, *prev;
	char *text, *subtitle, *detail, *icon;
	double value, flex, space;
	bool on, enabled, visible, focused;
	int variant;
	uint32_t tint;
	zui_cb cb;
	void *data;
	zui_free_fn free_data;
	int event;
	/* the last layout, in page points */
	float x, y, w, h;
	struct zt_slot t[5];
	struct zui_brush *bg;	/* what fills the node's area, behind it (zui_set_background) */
	struct zanim knob;	/* a switch's knob, 0 off .. 1 on */
	struct zanim focus_anim;
	struct zt_slot title_slot[2];
	/* pictures, lists, the rest */
	zui_pixmap *pix;
	bool group, applist, has_gap, fit, no_dots, quiet, sharp, emphasis, mine, inline_hint;
	float radius;
	int align;
	bool bar;
	struct zanim arrive;	/* a new row rising in */
	char *badge, *monogram;
	float row_gap;
	/* a row of a list: painted once, then only moved */
	unsigned row_tex;
	int row_tw, row_th;
	bool row_painted, row_measured;
	struct zanim swivel;
	bool swivel_out;
	uint32_t swivel_at;
	/* widgets/: pickers, tiles, hubs, suggestions; hold */
	void *ext;
	zui_cb hold_cb;
	void *hold_data;
	zui_free_fn hold_free;
};

/* A scroll container, as Windows Phone's ScrollViewer: the one thing that scrolls. A hub's section and
 * a pivot's item are heirs of it (a class with `scrolls`), and nothing else has a scroll: a page does not.
 * The finger moves `scroll`; `vel` carries on when it lets go. */
struct zui_scroll {
	struct zui_node base;
	float scroll, content_h, vel;
	float goal;			/* where a scroll to a place is going, if goal_on */
	bool goal_on;
	bool stick_end, glide;		/* kept at its end as things come; going there smoothly */
	uint32_t glide_at;
};
/* the scroll state of a node that has it, NULL for the rest */
static inline struct zui_scroll *zui_sv(const zui_node *n) {
	return n && n->cls->scrolls ? (struct zui_scroll *)n : NULL;
}

/* A page: the root of what is on the screen. It is two panels, as on Windows Phone: the TitlePanel
 * (the top margin, the app's name, the page's title) stays, the ContentPanel under it holds the content.
 * A hub or a pivot made from the app is one in the page it makes for itself. */
struct zui_page {
	struct zui_node base;
	struct zui_rect title_panel, content_panel;
	int depth;		/* 0 for the first page */
	zui_node *back_to;	/* the page under this one */
	zui_node *footer, *title_node, *content_node;
	float content_h;	/* how far down it goes: its layout, a page itself never scrolls */
	bool fullscreen, compact;
};
static inline bool zui_is_page(const zui_node *n) { return n && n->cls->is_page; }
static inline struct zui_page *zui_pg(const zui_node *n) {
	assert(zui_is_page(n));
	return (struct zui_page *)n;
}

/* A pivot: a page whose items are its children, one in front. */
struct zui_pivot {
	struct zui_page page;
	int cur, prev;		/* the item in front, and the one that was */
	struct zanim pos;	/* the highlight, from cur: 0 at rest, the way left to a neighbour while moving */
};
/* A text field: the caret and the selection are in bytes (caret -1: the end). */
struct zui_field {
	struct zui_node base;
	int caret, sel_a, sel_b;
	uint32_t last_tap;	/* for the second tap: a word */
	bool password, multiline;
	void *suggest;		/* the matches under it (widgets/field.c) */
};
static inline struct zui_field *zui_field_of(const zui_node *n) {
	assert(n && n->kind == ZK_FIELD);
	return (struct zui_field *)n;
}
/* A list: the rows are made as they come into sight (children of it, ZUI_LIST_SLOTS at most). */
struct zui_list {
	struct zui_node base;
	int count, want0, want1, last_first, last_dir, arrive_from;
	bool asked;
	float row_h, *y;	/* y[row], y[count] the end */
	zui_list_height height;
	zui_list_bind bind;
	void *data;
	zui_node *slot[ZUI_LIST_SLOTS];
	int slot_row[ZUI_LIST_SLOTS];
};
static inline struct zui_list *zui_list_get(const zui_node *n) {	/* NULL for what is not a list */
	return n && n->kind == ZK_LIST ? (struct zui_list *)n : NULL;
}
/* the page's pivot or hub, which is the page itself (NULL if it is neither) */
static inline zui_node *zui_page_pivot(const zui_node *n) { return n && n->kind == ZK_PIVOT ? (zui_node *)n : NULL; }
static inline zui_node *zui_page_hub(const zui_node *n) { return n && n->kind == ZK_HUB ? (zui_node *)n : NULL; }
static inline struct zui_pivot *zui_pivot_of(const zui_node *n) {
	assert(n && n->kind == ZK_PIVOT);
	return (struct zui_pivot *)n;
}

#define ZUI_MAX_FDS 32
#define ZUI_MAX_TIMERS 64

struct zui_timer_entry { int id; uint64_t due, every; zui_fn fn; void *data; };

struct zui_app {
	float fit_h;	/* a fullscreen page: the height an image set to fit has */
	char *app_id, *title;
	const struct zui_style *style;
	uint32_t dot_main;
	uint32_t accent;	/* 0xRRGGBB */
	unsigned style_gen;
	zui_node *page;		/* in front */
	zui_node **pages;	/* every page made, freed with the app */
	int n_pages, cap_pages;
	float width, height;	/* points */
	int scale;
	bool running, dirty, laid_out;
	int status;
	/* moving between pages */
	zui_node *trans_from, *trans_to;
	int trans_dir;		/* 1 forward, -1 back */
	uint32_t trans_start;
	bool quit_after_trans;
	/* the finger */
	bool touching, dragging, scrolling_page, swiping;
	bool bar_words;	/* the action bar is open, words under its buttons */
	float swipe_v;	/* tabs under the finger, of a screen per ms */
	uint32_t open_at;	/* the first frame: the app turns in */
	bool active;
	float paint_scale;
	int firing;		/* inside a handler: what it destroys is freed after it */
	zui_node *grave;
	zui_node *root;	/* the frame: the pages in it, and what the app keeps over all of them */
	bool in_footer;
	float down_x, down_y, last_x, last_y, down_scroll;
	uint32_t last_t;
	zui_node *pressed, *drag_slider;
	float press_fx, press_fy;
	struct zanim press;
	bool fling;
	uint32_t fling_t;
	zui_node *focus;	/* the field with the keyboard */
	/* busy dots, toast */
	bool busy;
	uint32_t busy_since;
	char *toast;
	uint32_t toast_at;
	/* the main loop */
	struct { int fd; void (*ready)(int, void *); void *data; } fds[ZUI_MAX_FDS];
	struct zui_timer_entry timers[ZUI_MAX_TIMERS];
	int next_timer_id;
	int post_fd;
	struct zui_post *posts;
	void *post_lock;
	struct zui_wl *wl;	/* the window, when there is one */
	/* widgets/ */
	void *ov;		/* the menu, dialog or picker over the page */
	zui_node *open_picker;	/* the list picker that is open */
	int hold_timer;
	zui_node *hold_node;
	bool hold_fired, hub_drag;
	float hub_from;
	uint32_t hold_since;
	int n_pickers;
};

uint32_t zui_now(void);
void zui_redraw(zui_app *app);
void zui_fire(zui_node *node, int event);
bool zui_animating(zui_app *app);
/* draw the app as it is now (pages, transitions, overlays) */
void zui_draw(zui_app *app, uint32_t now);
void zui_do_layout(zui_app *app, zui_node *page);
void zui_touch(zui_app *app, int phase, float x, float y, uint32_t time);
void zui_scroll_by(zui_app *app, float dy);
void zui_key_text(zui_app *app, const char *utf8);	/* "\b" backspace, "\n" enter */
void zui_step(zui_app *app, uint32_t now);	/* flings and springs */
const char *zui_data_dir(void);
void zui_osk(zui_app *app, bool show);

/* widgets/: what core.c asks of it */
void zui_widget_free(zui_node *n);
void zui_widgets_free(zui_app *app);
void zui_widgets_draw_over(zui_app *app, uint32_t now);	/* overlays and suggestions, after the page */
bool zui_widgets_touch(zui_app *app, int phase, float x, float y, uint32_t time);	/* true: taken */
bool zui_widgets_back(zui_app *app);	/* true: it closed something */
bool zui_widgets_animating(zui_app *app, uint32_t now);
void zui_widgets_step(zui_app *app, uint32_t now);
float zui_hub_offset(const zui_node *hub);
void zui_brush_paint(zui_app *app, struct zui_brush *b, float x, float y, float w, float h, float r);
void zui_brush_free(struct zui_brush *b);
struct zui_scroll *zui_scroller(zui_node *page);	/* what scrolls on a page: the tab or hub section in front, else the page */
zui_node *zui_list_owner(zui_node *list);	/* what scrolls a list */
zui_node *zui_hub_front(const zui_node *hub);	/* the section nearest the screen's left edge */
int zui_hub_copies(const zui_node *hub, const zui_node *section, float dx[3]);
float zui_hub_pick_shift(const zui_node *hub, const zui_node *section, float screen_x);
float zui_hub_begin_drag(zui_node *hub);
bool zui_hub_title_moving(zui_node *hub, uint32_t now);
void zui_hub_drag(zui_app *app, zui_node *hub, float dx);	/* the finger moved dx from where it went down */
void zui_hub_release(zui_app *app, zui_node *hub, float velocity);	/* points per ms, + to the right */
const char *zui_panel_text(const zui_node *page, int kind);	/* the text of a TitlePanel's first ZK_TITLE or ZK_HEADER */
float zui_hub_content_top(const zui_node *hub);	/* where a hub's sections start: under its title */
zui_node *zui_node_new_detached(zui_app *app, int kind);
void zui_node_free_detached(zui_node *n);
void zui_hold_start(zui_app *app, zui_node *n, uint32_t time);
void zui_hold_cancel(zui_app *app);
bool zui_hold_on_lift(zui_app *app, uint32_t time, uint32_t down_time);
void zui_pickers_close_except(zui_app *app, zui_node *keep);
float zui_node_hub_shift(const zui_node *n, float screen_x);
bool zui_widgets_page_moving(zui_app *app, zui_node *page, uint32_t now);
void zui_cb_end(zui_app *app);
void zui_set_checked(zui_node *n, bool on);
bool zui_suggest_touch(zui_app *app, float x, float y, float scroll);

/* extras.c */
zui_node *zui_node_new_kind(zui_node *parent, int kind);
float zui_layout_at(zui_node *n, float x, float y, float w);
void zui_move_children(zui_node *n, float dy);
void zui_list_ensure(zui_node *n, uint32_t now);
void zui_rows_start(zui_app *app, void *(*make_current)(void *), void *arg);
bool zui_rows_running(void);
void zui_list_draw(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now, int role, float t, int dir, float scroll);
void zui_draw_node(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now);
bool zui_has_pressed(const zui_app *app, const zui_node *n);
void zt_caret_at(const char *text, int upto, float w, float *x, float *y, float *line_h);
void zui_tabs_step(zui_node *tabs, int dir);
float zui_list_layout(zui_node *n, float x, float y, float w);
int zui_child_index(const zui_node *c);
const char *zui_tab_title(const zui_node *item);
zui_node *zui_tab_at(zui_node *tabs, int i);
int zui_tab_index(const zui_node *tabs, const zui_node *item);
int zui_tab_count(const zui_node *tabs);
void zui_frame_begin(void);
bool zui_pixmap_ready(zui_app *app, zui_pixmap *p, bool nearest);
void zui_draw_pixmap(zui_app *app, zui_pixmap *p, float x, float y, float w, float h, float r, bool sharp, uint32_t back);
void zui_draw_leading(zui_app *app, zui_node *n, float x, float y, float size, float r, uint32_t fg);

/* run the main loop with a window (wl.c) */
int zui_wl_run(zui_app *app);
void zui_wl_wake(zui_app *app);
/* offscreen.c */
int zui_offscreen_png(zui_app *app, zui_node *page, const char *path, int w, int h, int scale);
int zui_png_write(const char *path, const uint8_t *rgba, int w, int h, bool flip);

/* -------------------------------------------------------------- styles --- */
struct zui_palette {
	uint32_t bg, surface, surface2, text, text2, text3, divider, track, knob, shadow,
		on_accent, accent_text, danger, scrim, bubble_caption;
};
struct zui_metrics {
	float margin, gap, card_r, card_pad, section_gap;
	float top;	/* where the TitlePanel's first line starts: 32 for the status bar and 24 under it */
	float text, heading, caption, label, title, bar_title;
	float item_h, item_h2, item_title, item_sub, icon_tile;
	float button_h, button_r, button_text;
	float field_h, field_r;
	float bar_slot, bar_button;	/* an action bar's round button: its width and height; 0: with its word always */
	float title_h, title_compact_h;	/* the TitlePanel's height: from the top of the screen, with the status bar's room */
	float large_title_h;
};
struct zui_style {
	float turn_in_ms;	/* when a page turn's incoming half begins; 0: an app just appears */
	const char *name;
	const char *fonts[ZT_FONTS];
	float line_spacing;
	int transition_ms;
	struct zui_palette (*palette)(const zui_app *app);
	struct zui_metrics m;
	/* the fixed part of a page over its list, drawn last */
	void (*header)(zui_app *app, zui_node *page, uint32_t now);
	/* what a page has at its top that scrolls with it (zrnCoolStyle's large title) */
	float (*page_top)(zui_app *app, zui_node *page, float y, bool draw);
	void (*card)(zui_app *app, zui_node *card);
	void (*divider)(zui_app *app, zui_node *card, zui_node *below);
	void (*section_label)(zui_app *app, zui_node *n);
	void (*button)(zui_app *app, zui_node *n, float press);
	void (*item)(zui_app *app, zui_node *n, float press);
	void (*toggle)(zui_app *app, zui_node *n, float x, float y, float knob);
	void (*slider)(zui_app *app, zui_node *n);
	void (*progress)(zui_app *app, zui_node *n, uint32_t now);
	void (*field)(zui_app *app, zui_node *n, uint32_t now);
	void (*busy)(zui_app *app, uint32_t now);
	void (*toast)(zui_app *app, const char *text, float shown);
	/* tabs under the title; pos: where the highlight is, a tab's index or between */
	void (*tabs)(zui_app *app, zui_node *tabs, float pos);
	float tabs_h;
	void (*bubble)(zui_app *app, zui_node *n);
	float bubble_pad;
	void (*footer)(zui_app *app, zui_node *n);
	/* the press: a transform for the node under the finger (amount 0..1) */
	void (*press_xf)(zui_app *app, zui_node *n, float amount, float fx, float fy, struct zg_xf *xf);
	/* a page coming or going: t 0..1 through the change, dir 1 forward, row
	 * the index of a thing on the page (-1 the header) */
	void (*page_xf)(zui_app *app, float t, int dir, bool incoming, int row, int rows,
			struct zg_xf *xf, float *alpha);
	/* does the press fill the whole row (lists) or tilt it */
	bool rows_highlight;
	bool flat_cards;	/* a card's rows move one by one in page changes */
};
extern const struct zui_style zui_style_metro, zui_style_dotmatrix;
void zui_dot_palette(uint32_t *main_rgb, uint32_t *accent_rgb);
const struct zui_style *zui_style_find(const char *name);
struct zui_palette zui_pal(const zui_app *app);
/* a small picture by name, in a box `size` points across */
extern bool zui_icons_as_dots;
void zui_icon(const char *name, float x, float y, float size, float stroke, uint32_t rgba);
float zui_ease_out(float t);		/* cubic */
float zui_ease_emph(float t);		/* zrnCoolStyle's settle: quick start, long soft end */
float zui_ease_in(float t);

#endif
