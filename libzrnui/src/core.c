// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * The node tree: making and changing things, laying pages out, the finger
 * (press, tap, scroll, fling, drag a slider), moving between pages, and
 * drawing a frame through the style in use.
 */
#define _GNU_SOURCE
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <time.h>
#include <unistd.h>

#include "zui-internal.h"

#include <strings.h>
#define DOT_PALETTE_NAMES { "Mono", "Amber", "Green", "Ice", "White", "Red" }
#define SLOP 8.0f

ZUI_EXPORT const char *zui_version(void) { return "0.1.0"; }

uint32_t zui_now(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

/* ---- easing and motion ---- */
float zui_ease_out(float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	float u = 1 - t;
	return 1 - u * u * u;
}

float zui_ease_in(float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	return t * t * t;
}

/* quick away, then a long soft settle: (0.2, 0.8, 0.2, 1) as a cubic Bezier */
float zui_ease_emph(float t) {
	t = t < 0 ? 0 : t > 1 ? 1 : t;
	const float x1 = 0.2f, y1 = 0.8f, x2 = 0.2f, y2 = 1.0f;
	float s = t;
	for (int i = 0; i < 8; i++) {
		float u = 1 - s;
		float x = 3 * u * u * s * x1 + 3 * u * s * s * x2 + s * s * s - t;
		float dx = 3 * u * u * x1 + 6 * u * s * (x2 - x1) + 3 * s * s * (1 - x2);
		if (fabsf(dx) < 1e-5f) break;
		s -= x / dx;
		s = s < 0 ? 0 : s > 1 ? 1 : s;
	}
	float u = 1 - s;
	return 3 * u * u * s * y1 + 3 * u * s * s * y2 + s * s * s;
}

static uint32_t anim_until;	/* something is moving until then */

float zanim_value(const struct zanim *a, uint32_t now, bool *running) {
	if (!a->ms || (int32_t)(now - a->start) >= (int32_t)a->ms) {
		return a->to;
	}
	if (running) *running = true;
	if (a->w > 0) {	/* no overshoot, and no jump in speed from the finger */
		float e = (float)(int32_t)(now - a->start), x = a->from - a->to;
		return a->to + (x + (a->v0 + a->w * x) * e) * expf(-a->w * e);
	}
	float t = (float)(int32_t)(now - a->start) / a->ms;
	return a->from + (a->to - a->from) * zui_ease_out(t < 0 ? 0 : t);
}

void zanim_go(struct zanim *a, float to, uint32_t ms, uint32_t now) {
	a->from = zanim_value(a, now, NULL);
	a->w = 0;
	a->to = to;
	a->start = now;
	a->ms = ms;
	if ((int32_t)(now + ms - anim_until) > 0) anim_until = now + ms;
}

void zanim_set(struct zanim *a, float v) {
	a->from = a->to = v;
	a->ms = 0;
}

/* ---- where libzrnui's own files are (fonts) ---- */
const char *zui_data_dir(void) {
	static char dir[1024];
	if (dir[0]) return dir;
	const char *env = getenv("ZUI_DATA_DIR");
	if (env) {
		snprintf(dir, sizeof(dir), "%s", env);
		return dir;
	}
	/* next to the library: the build tree (libzrnui/build/libzrnui.so -> libzrnui/)
	 * or an installed one (PREFIX/lib/libzrnui.so -> PREFIX/share/zrnui) */
	Dl_info info;
	if (dladdr((void *)zui_data_dir, &info) && info.dli_fname) {
		char lib[1024];
		snprintf(lib, sizeof(lib), "%s", info.dli_fname);
		char *slash = strrchr(lib, '/');
		if (slash) {
			*slash = 0;
			const char *tries[] = { "%s/../fonts/..", "%s/../share/zrnui", "%s/../../share/zrnui" };
			for (size_t i = 0; i < sizeof(tries) / sizeof(tries[0]); i++) {
				char probe[1100], fonts[1200];
				snprintf(probe, sizeof(probe), tries[i], lib);
				snprintf(fonts, sizeof(fonts), "%s/fonts", probe);
				if (access(fonts, R_OK) == 0) {
					char *real = realpath(probe, NULL);
					snprintf(dir, sizeof(dir), "%.1000s", real ? real : probe);
					free(real);
					return dir;
				}
			}
		}
	}
#ifdef ZUI_DATADIR
	snprintf(dir, sizeof(dir), "%s", ZUI_DATADIR);
#else
	snprintf(dir, sizeof(dir), "/usr/share/zrnui");
#endif
	return dir;
}

/* ---- the system's settings (ztmetro's config file) ---- */
static bool config_value(const char *key, char *out, size_t size) {
	char path[512], line[256];
	const char *home = getenv("HOME");
	snprintf(path, sizeof(path), "%s/.config/ztmetro/config", home ? home : "");
	FILE *f = fopen(path, "r");
	bool found = false;
	while (f && fgets(line, sizeof(line), f)) {
		char *p = line;
		while (isspace((unsigned char)*p)) p++;
		size_t k = strlen(key);
		if (strncmp(p, key, k) != 0) continue;
		p += k;
		while (isspace((unsigned char)*p)) p++;
		if (*p != '=') continue;
		p++;
		while (isspace((unsigned char)*p)) p++;
		size_t n = strcspn(p, " \t\r\n#");
		snprintf(out, size, "%.*s", (int)n, p);
		found = true;	/* the last one counts, as in ztmetro */
	}
	if (f) fclose(f);
	return found;
}

static uint32_t system_accent(void) {
	static const struct { const char *name; uint32_t color; } accents[] = {
		{ "lime", 0xa4c400 }, { "green", 0x60a917 }, { "emerald", 0x008a00 },
		{ "teal", 0x00aba9 }, { "cyan", 0x1ba1e2 }, { "cobalt", 0x0050ef },
		{ "indigo", 0x6a00ff }, { "violet", 0xaa00ff }, { "pink", 0xf472d0 },
		{ "magenta", 0xd80073 }, { "crimson", 0xa20025 }, { "red", 0xe51400 },
		{ "orange", 0xfa6800 }, { "amber", 0xf0a30a }, { "yellow", 0xe3c800 },
		{ "brown", 0x825a2c }, { "olive", 0x6d8764 }, { "steel", 0x647687 },
		{ "mauve", 0x76608a }, { "taupe", 0x87794e },
	};
	char v[64];
	const char *env = getenv("ZUI_ACCENT");
	if (env) snprintf(v, sizeof(v), "%s", env);
	else if (!config_value("accent", v, sizeof(v))) return 0x0050ef;
	if (v[0] == '#') return (uint32_t)strtoul(v + 1, NULL, 16) & 0xffffff;
	for (size_t i = 0; i < sizeof(accents) / sizeof(accents[0]); i++)
		if (strcmp(accents[i].name, v) == 0) return accents[i].color;
	return 0x0050ef;
}

const struct zui_style *zui_style_find(const char *name) {
	if (!name) return NULL;
	if (strcmp(name, "dotmatrix") == 0 || strcmp(name, "DotMatrix") == 0) return &zui_style_dotmatrix;
	if (strcmp(name, "metro") == 0 || strcmp(name, "Metro") == 0) return &zui_style_metro;
	return NULL;
}

/* The style is the system's, so an app never differs from what is around it:
 * the session's (ZTRF_SHELL_STYLE), else `style` in the config, else Metro.
 * ZUI_STYLE is for renders and tests. */
static const struct zui_style *pick_style(void) {
	const struct zui_style *s = zui_style_find(getenv("ZUI_STYLE"));
	if (s) return s;
	s = zui_style_find(getenv("ZTRF_SHELL_STYLE"));
	if (s) return s;
	char v[32];
	if (config_value("style", v, sizeof(v)) && (s = zui_style_find(v))) return s;
	return &zui_style_metro;
}

/* DotMatrix's six palettes: main, accent (ztmetro/retro/dotmatrix.c) */
void zui_dot_palette(uint32_t *main_rgb, uint32_t *accent_rgb) {
	static const uint32_t P[6][2] = {
		{ 0xFFFFFF, 0xFF3B30 }, { 0xFFB02E, 0xFF4B3A }, { 0x8CFF5A, 0xFFD23F },
		{ 0x9FE7FF, 0xFF6B8A }, { 0xF4F2EC, 0xFF5A3C }, { 0xFF4636, 0xFFC857 },
	};
	static const char *const names[6] = DOT_PALETTE_NAMES;
	char v[32];
	int k = 0;
	if (config_value("dotmatrix_palette", v, sizeof(v))) {
		if (isdigit((unsigned char)v[0])) k = atoi(v);
		else for (int i = 0; i < 6; i++) if (!strcasecmp(names[i], v)) k = i;
	}
	if (k < 0 || k > 5) k = 0;
	*main_rgb = P[k][0];
	*accent_rgb = P[k][1];
}

static void apply_style(zui_app *app) {
	if (app->style == &zui_style_dotmatrix) {
		uint32_t main_rgb, acc;
		zui_dot_palette(&main_rgb, &acc);
		app->accent = acc;
		app->dot_main = main_rgb;
	}
	zt_set_dot_type(app->style == &zui_style_dotmatrix);
	zui_icons_as_dots = app->style == &zui_style_dotmatrix;
	zt_set_fonts(app->style->fonts, app->style->line_spacing);
	app->style_gen++;
	app->laid_out = false;
	app->dirty = true;
}

struct zui_post { zui_fn fn; void *data; struct zui_post *next; };

static zui_node *node_new(zui_app *app, zui_node *parent, int kind);

ZUI_EXPORT zui_app *zui_app_new(void) {
	zui_app *app = calloc(1, sizeof(*app));
	/* the id the shell knows the app by: what runs it says (zrn-exec and `zrn run`: the manifest's id),
	 * else the program's name, as GTK and Qt do; the window's title is the same */
	const char *id = getenv("ZRN_APP_ID");
	app->app_id = strdup(id && *id ? id : program_invocation_short_name);
	app->title = strdup(app->app_id);
	app->accent = system_accent();
	app->style = pick_style();
	app->root = node_new(app, NULL, ZK_FRAME);
	app->width = 360;
	app->height = 740;
	app->scale = 1;
	app->running = true;
	app->next_timer_id = 1;
	app->post_fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
	pthread_mutex_t *lock = malloc(sizeof(*lock));
	pthread_mutex_init(lock, NULL);
	app->post_lock = lock;
	apply_style(app);
	return app;
}

ZUI_EXPORT void zui_app_set_style(zui_app *app, const char *style) {
	(void)style;
	app->style = pick_style();
	apply_style(app);
}

ZUI_EXPORT const char *zui_app_style(const zui_app *app) { return app->style->name; }

ZUI_EXPORT void zui_app_set_theme(zui_app *app, const char *theme) {
	(void)app; (void)theme;
}

ZUI_EXPORT void zui_app_set_accent(zui_app *app, uint32_t rgb) {
	if (app->style == &zui_style_dotmatrix) return;	/* the palette's */
	app->accent = rgb & 0xffffff;
	app->dirty = true;
}

ZUI_EXPORT uint32_t zui_app_accent(const zui_app *app) { return app->accent; }

struct zui_palette zui_pal(const zui_app *app) { return app->style->palette(app); }

/* ---- nodes ---- */
static void changed(zui_node *n, bool layout);

static zui_node *node_new(zui_app *app, zui_node *parent, int kind) {
	const struct zui_class *cls = zui_class_of(kind);
	zui_node *n = calloc(1, cls->size);
	n->app = app;
	n->kind = kind;
	n->cls = cls;
	n->enabled = n->visible = true;
	n->parent = parent;
	if (parent) {
		n->prev = parent->last;
		if (parent->last) parent->last->next = n;
		else parent->first = n;
		parent->last = n;
	}
	app->laid_out = false;
	app->dirty = true;
	return n;
}

zui_node *zui_node_new_kind(zui_node *parent, int kind) { return node_new(parent->app, parent, kind); }

static void node_free(zui_node *n) {
	zui_node *c = n->first;
	while (c) {
		zui_node *next = c->next;
		node_free(c);
		c = next;
	}
	zui_app *app = n->app;
	if (app->pressed == n) app->pressed = NULL;
	if (app->drag_slider == n) app->drag_slider = NULL;
	if (app->focus == n) {
		app->focus = NULL;
		zui_osk(app, false);
	}
	if (n->free_data) n->free_data(n->data);
	if (n->cls->destroy) n->cls->destroy(n);
	zui_widget_free(n);
	for (int i = 0; i < 5; i++) zt_slot_free(&n->t[i]);
	for (int i = 0; i < 2; i++) zt_slot_free(&n->title_slot[i]);
	free(n->text);
	free(n->subtitle);
	free(n->detail);
	free(n->icon);
	free(n->badge);
	free(n->monogram);
	zui_pixmap_free(n->pix);
	zui_brush_free(n->bg);
	if (n->row_tex) zg_tex_free(n->row_tex);
	free(n);
}

static void unlink_node(zui_node *n);
/* Gone from the tree now; freed now, or when the handler running has returned:
 * a row's handler may refresh its own list, a button may clear its own page. */
static void bury(zui_node *n) {
	zui_app *app = n->app;
	unlink_node(n);
	if (app->firing > 0) {
		if (app->pressed) app->pressed = NULL;
		n->next = app->grave;
		app->grave = n;
	} else {
		node_free(n);
	}
}

static void unlink_node(zui_node *n) {
	zui_node *p = n->parent;
	if (!p) return;
	if (n->prev) n->prev->next = n->next;
	else p->first = n->next;
	if (n->next) n->next->prev = n->prev;
	else p->last = n->prev;
	/* a page that loses its TitlePanel or footer does not keep pointing at them */
	if (zui_is_page(p)) {
		struct zui_page *pg = zui_pg(p);
		if (pg->title_node == n) pg->title_node = NULL;
		if (pg->footer == n) pg->footer = NULL;
		if (pg->content_node == n) pg->content_node = NULL;
	}
	n->parent = n->prev = n->next = NULL;
}

static void set_str(char **slot, const char *s) {
	if (*slot && s && strcmp(*slot, s) == 0) return;
	free(*slot);
	*slot = s ? strdup(s) : NULL;
}

/* a page of this kind (ZK_PAGE, or a kind that is one: a pivot, a hub) in the frame */
zui_node *zui_page_new_kind(zui_app *app, int kind) {
	zui_node *n = node_new(app, app->root, kind);
	if (app->n_pages == app->cap_pages) {
		app->cap_pages = app->cap_pages ? app->cap_pages * 2 : 8;
		app->pages = realloc(app->pages, sizeof(*app->pages) * app->cap_pages);
	}
	app->pages[app->n_pages++] = n;
	return n;
}

/* a node that belongs to nothing in the tree: what a widget keeps for itself and draws itself (a hub's title) */
zui_node *zui_node_new_detached(zui_app *app, int kind) { return node_new(app, NULL, kind); }
void zui_node_free_detached(zui_node *n) { node_free(n); }

ZUI_EXPORT zui_node *zui_root(zui_app *app) { return app->root; }

ZUI_EXPORT zui_node *zui_page_blank(zui_app *app) { return zui_page_new_kind(app, ZK_PAGE); }
ZUI_EXPORT zui_node *zui_page_fullscreen(zui_app *app) {
	zui_node *n = zui_page_new_kind(app, ZK_PAGE);
	zui_pg(n)->fullscreen = true;
	return n;
}
ZUI_EXPORT zui_node *zui_page(zui_app *app) {
	zui_node *n = zui_page_new_kind(app, ZK_PAGE);
	zui_title_panel(n);
	zui_content_panel(n);
	return n;
}

/* the root a node belongs to: every page-level call takes the page or anything in it, so a hub
 * or a pivot made straight from the app is a root that can be run, pushed, scrolled */
zui_node *zui_page_of(zui_node *n) {
	while (n && n->parent && !zui_is_page(n)) n = n->parent;
	return n;
}

ZUI_EXPORT zui_node *zui_title_panel(zui_node *page) {
	page = zui_page_of(page);
	if (zui_pg(page)->title_node) return zui_pg(page)->title_node;
	zui_node *n = node_new(page->app, page, ZK_TITLE_PANEL);
	n->visible = false;	/* not laid out in the column, not drawn or hit as content */
	zui_pg(page)->title_node = n;
	page->app->dirty = true;
	return n;
}

ZUI_EXPORT zui_node *zui_scroll(zui_node *parent) { return node_new(parent->app, parent, ZK_SCROLL); }

ZUI_EXPORT zui_node *zui_content_panel(zui_node *parent) {
	if (zui_is_page(parent) && zui_pg(parent)->content_node) return zui_pg(parent)->content_node;
	zui_node *n = node_new(parent->app, parent, ZK_CONTENT_PANEL);
	if (zui_is_page(parent)) zui_pg(parent)->content_node = n;
	return n;
}
ZUI_EXPORT zui_node *zui_column(zui_node *parent) { return node_new(parent->app, parent, ZK_COLUMN); }
ZUI_EXPORT zui_node *zui_row(zui_node *parent) { return node_new(parent->app, parent, ZK_ROW); }
ZUI_EXPORT zui_node *zui_card(zui_node *parent) { return node_new(parent->app, parent, ZK_CARD); }

ZUI_EXPORT zui_node *zui_section(zui_node *parent, const char *title) {
	if (title && *title) {
		zui_node *label = node_new(parent->app, parent, ZK_SECTION_LABEL);
		set_str(&label->text, title);
	}
	return zui_card(parent);
}

ZUI_EXPORT zui_node *zui_footer(zui_node *page) {
	page = zui_page_of(page);
	zui_node *n = node_new(page->app, page, ZK_FOOTER);
	zui_pg(page)->footer = n;
	return n;
}

ZUI_EXPORT zui_node *zui_bubble(zui_node *parent, bool mine) {
	zui_node *n = node_new(parent->app, parent, ZK_BUBBLE);
	n->mine = mine;
	return n;
}

ZUI_EXPORT bool zui_app_active(const zui_app *app) { return app->active; }
ZUI_EXPORT void zui_field_set_multiline(zui_node *n, bool m) { if (n->kind != ZK_FIELD) return; zui_field_of(n)->multiline = m; changed(n, true); }
ZUI_EXPORT void zui_set_bar(zui_node *n, bool b) { n->bar = b; changed(n, false); }
ZUI_EXPORT void zui_page_set_compact(zui_node *page, bool c) { page = zui_page_of(page); zui_pg(page)->compact = c; page->app->dirty = true; }

ZUI_EXPORT zui_node *zui_space(zui_node *parent, double points) {
	zui_node *n = node_new(parent->app, parent, ZK_SPACE);
	n->space = points;
	return n;
}

static zui_node *text_node(zui_node *parent, int kind, const char *text) {
	zui_node *n = node_new(parent->app, parent, kind);
	set_str(&n->text, text);
	return n;
}

ZUI_EXPORT zui_node *zui_text(zui_node *p, const char *t) { return text_node(p, ZK_TEXT, t); }
ZUI_EXPORT zui_node *zui_title(zui_node *p, const char *t) { return text_node(p, ZK_TITLE, t); }
ZUI_EXPORT zui_node *zui_header(zui_node *p, const char *t) { return text_node(p, ZK_HEADER, t); }
ZUI_EXPORT zui_node *zui_heading(zui_node *p, const char *t) { return text_node(p, ZK_HEADING, t); }
ZUI_EXPORT zui_node *zui_caption(zui_node *p, const char *t) { return text_node(p, ZK_CAPTION, t); }

ZUI_EXPORT zui_node *zui_button(zui_node *parent, const char *label, zui_cb cb, void *data) {
	zui_node *n = text_node(parent, ZK_BUTTON, label);
	n->cb = cb;
	n->data = data;
	return n;
}

ZUI_EXPORT zui_node *zui_item(zui_node *parent, const char *title, const char *subtitle, zui_cb cb, void *data) {
	zui_node *n = text_node(parent, ZK_ITEM, title);
	set_str(&n->subtitle, subtitle);
	n->cb = cb;
	n->data = data;
	return n;
}

ZUI_EXPORT zui_node *zui_switch(zui_node *parent, const char *title, bool on, zui_cb cb, void *data) {
	zui_node *n = text_node(parent, ZK_SWITCH, title);
	n->on = on;
	zanim_set(&n->knob, on ? 1 : 0);
	n->cb = cb;
	n->data = data;
	return n;
}

ZUI_EXPORT zui_node *zui_slider(zui_node *parent, const char *title, double value, zui_cb cb, void *data) {
	zui_node *n = text_node(parent, ZK_SLIDER, title);
	n->value = value < 0 ? 0 : value > 1 ? 1 : value;
	n->cb = cb;
	n->data = data;
	return n;
}

ZUI_EXPORT zui_node *zui_progress(zui_node *parent, double value) {
	zui_node *n = node_new(parent->app, parent, ZK_PROGRESS);
	n->value = value > 1 ? 1 : value;
	return n;
}

ZUI_EXPORT zui_node *zui_field(zui_node *parent, const char *placeholder, zui_cb cb, void *data) {
	zui_node *n = node_new(parent->app, parent, ZK_FIELD);
	for (zui_node *p = parent; p; p = p->parent) if (p->kind == ZK_FOOTER) n->inline_hint = true;
	set_str(&n->subtitle, placeholder);
	set_str(&n->text, "");
	zui_field_of(n)->caret = -1;
	n->cb = cb;
	n->data = data;
	return n;
}

static void changed(zui_node *n, bool layout) {
	if (layout) n->app->laid_out = false;
	n->app->dirty = true;
}

ZUI_EXPORT void zui_set_text(zui_node *n, const char *t) {
	set_str(&n->text, t ? t : "");
	if (n->kind == ZK_FIELD) {	/* a new text: the caret at its end, nothing selected */
		struct zui_field *f = zui_field_of(n);
		f->caret = -1;
		f->sel_a = f->sel_b = 0;
	}
	changed(n, true);
}
ZUI_EXPORT const char *zui_get_text(const zui_node *n) { return n->text ? n->text : ""; }
ZUI_EXPORT void zui_set_detail(zui_node *n, const char *t) { set_str(&n->detail, t); changed(n, true); }
ZUI_EXPORT void zui_set_subtitle(zui_node *n, const char *t) { set_str(&n->subtitle, t); changed(n, true); }

ZUI_EXPORT void zui_set_value(zui_node *n, double v) {
	if (n->kind == ZK_SLIDER) v = v < 0 ? 0 : v > 1 ? 1 : v;
	if (n->kind == ZK_PROGRESS && v > 1) v = 1;
	n->value = v;
	changed(n, false);
}

ZUI_EXPORT double zui_get_value(const zui_node *n) { return n->kind == ZK_SWITCH ? (n->on ? 1 : 0) : n->value; }

ZUI_EXPORT void zui_set_on(zui_node *n, bool on) {
	if (n->on == on) return;
	n->on = on;
	zanim_go(&n->knob, on ? 1 : 0, 180, zui_now());
	changed(n, false);
}

ZUI_EXPORT bool zui_get_on(const zui_node *n) { return n->on; }
ZUI_EXPORT void zui_set_enabled(zui_node *n, bool e) { n->enabled = e; changed(n, false); }
ZUI_EXPORT void zui_set_visible(zui_node *n, bool v) { n->visible = v; changed(n, true); }
ZUI_EXPORT void zui_set_variant(zui_node *n, int v) { n->variant = v; changed(n, false); }
ZUI_EXPORT void zui_set_flex(zui_node *n, double f) { n->flex = f; changed(n, true); }
ZUI_EXPORT void zui_set_icon(zui_node *n, const char *icon) { set_str(&n->icon, icon); changed(n, true); }
ZUI_EXPORT void zui_set_tint(zui_node *n, uint32_t rgb) { n->tint = rgb ? (rgb & 0xffffff) | 0x1000000 : 0; changed(n, false); }

ZUI_EXPORT void zui_set_handler(zui_node *n, zui_cb cb, void *data, zui_free_fn free_data) {
	if (n->free_data) n->free_data(n->data);
	n->cb = cb;
	n->data = data;
	n->free_data = free_data;
	changed(n, false);
}

ZUI_EXPORT int zui_event(const zui_node *n) { return n->event; }
ZUI_EXPORT zui_app *zui_node_app(const zui_node *n) { return n->app; }

ZUI_EXPORT void zui_node_frame(const zui_node *n, double *x, double *y, double *w, double *h) {
	if (x) *x = n->x;
	if (y) *y = n->y;
	if (w) *w = n->w;
	if (h) *h = n->h;
}

ZUI_EXPORT void zui_clear(zui_node *n) {
	if (n->kind == ZK_FRAME) {
		/* the pages are not what the app keeps in the frame: zui_destroy takes those */
		for (zui_node *c = n->first, *next; c; c = next) {
			next = c->next;
			if (!zui_is_page(c)) bury(c);
		}
		changed(n, true);
		return;
	}
	while (n->first) {
		bury(n->first);
	}
	changed(n, true);
}

ZUI_EXPORT void zui_destroy(zui_node *n) {
	zui_app *app = n->app;
	if (zui_is_page(n)) {
		if (app->page == n || app->trans_from == n || app->trans_to == n) return;	/* in use */
		for (int i = 0; i < app->n_pages; i++) {
			if (zui_pg(app->pages[i])->back_to == n) zui_pg(app->pages[i])->back_to = NULL;
			if (app->pages[i] == n) {
				app->pages[i] = app->pages[--app->n_pages];
				i--;
			}
		}
		unlink_node(n);
		node_free(n);
		return;
	}
	bury(n);
	app->laid_out = false;
	app->dirty = true;
}

void zui_fire(zui_node *n, int event) {
	zui_app *app = n->app;
	app->firing++;
	n->event = event;
	if (n->cb) n->cb(n, n->data);
	n->event = ZUI_EVENT_NONE;
	app->dirty = true;
	zui_cb_end(app);
}

/* the end of a handler that began with app->firing++: what it destroyed goes now */
void zui_cb_end(zui_app *app) {
	if (--app->firing == 0) {
		while (app->grave) {
			zui_node *g = app->grave;
			app->grave = g->next;
			g->next = NULL;
			node_free(g);
		}
	}
}

ZUI_EXPORT void zui_busy(zui_app *app, bool busy) {
	if (busy && !app->busy) app->busy_since = zui_now();
	app->busy = busy;
	app->dirty = true;
}

ZUI_EXPORT void zui_toast(zui_app *app, const char *text) {
	set_str(&app->toast, text);
	app->toast_at = zui_now();
	app->dirty = true;
}

/* ---- pages ---- */
ZUI_EXPORT zui_node *zui_current_page(const zui_app *app) { return app->page; }

static void transition_finish(zui_app *app) {
	if (!app->trans_to) return;
	app->trans_from = app->trans_to = NULL;
	if (app->quit_after_trans) app->running = false;
	app->dirty = true;
}

ZUI_EXPORT void zui_push(zui_node *page) {
	page = zui_page_of(page);
	zui_app *app = page->app;
	if (!zui_is_page(page) || page == app->page) return;
	transition_finish(app);
	if (app->focus) {
		app->focus->focused = false;
		app->focus = NULL;
		zui_osk(app, false);
	}
	zui_pg(page)->back_to = app->page;
	zui_pg(page)->depth = app->page ? zui_pg(app->page)->depth + 1 : 0;
	app->fling = false;
	if (app->page && app->wl) {
		app->trans_from = app->page;
		app->trans_to = page;
		app->trans_dir = 1;
		app->trans_start = zui_now();
	}
	app->page = page;
	app->pressed = NULL;
	app->laid_out = false;
	app->dirty = true;
}

ZUI_EXPORT void zui_page_set_root(zui_node *page) {
	page = zui_page_of(page);
	zui_pg(page)->back_to = NULL;
	zui_pg(page)->depth = 0;
}

ZUI_EXPORT void zui_back(zui_app *app) {
	if (zui_widgets_back(app)) return;	/* a menu, a dialog or a picker first */
	transition_finish(app);
	zui_node *page = app->page;
	if (!page) return;
	if (app->focus) {
		app->focus->focused = false;
		app->focus = NULL;
		zui_osk(app, false);
	}
	if (!zui_pg(page)->back_to) {
		zui_quit(app);
		return;
	}
	if (app->wl) {
		app->trans_from = page;
		app->trans_to = zui_pg(page)->back_to;
		app->trans_dir = -1;
		app->trans_start = zui_now();
	}
	app->page = zui_pg(page)->back_to;
	app->pressed = NULL;
	app->fling = false;
	app->laid_out = false;
	app->dirty = true;
}

ZUI_EXPORT void zui_quit(zui_app *app) {
	app->running = false;
	zui_wl_wake(app);
}

ZUI_EXPORT void zui_app_free(zui_app *app) {
	if (!app) return;
	zui_hold_cancel(app);
	node_free(app->root);	/* the pages go with it */
	zui_widgets_free(app);
	free(app->pages);
	free(app->app_id);
	free(app->title);
	free(app->toast);
	struct zui_post *p = app->posts;
	while (p) {
		struct zui_post *next = p->next;
		free(p);
		p = next;
	}
	if (app->post_fd >= 0) close(app->post_fd);
	pthread_mutex_destroy(app->post_lock);
	free(app->post_lock);
	free(app);
}

/* ---- layout ---- */
static bool in_card(const zui_node *n) {
	return n->parent && n->parent->kind == ZK_CARD;
}

/* a row (item, switch) fills a card edge to edge; anything else sits in its padding */
static bool card_row_kind(int kind) {
	return kind == ZK_ITEM || kind == ZK_SWITCH || kind == ZK_CHECK || kind == ZK_RADIO;
}

static float text_size(const zui_app *app, int kind) {
	const struct zui_metrics *m = &app->style->m;
	return kind == ZK_HEADING ? m->heading : kind == ZK_CAPTION ? m->caption : kind == ZK_TITLE ? 11.5f
		: kind == ZK_HEADER ? m->title : m->text;
}

/* type in a bubble is Messaging's, smaller than a page's */
static float node_text_size(const zui_node *n) {
	if (n->parent && n->parent->kind == ZK_BUBBLE)
		return n->kind == ZK_CAPTION ? 11.5f : 15;
	return text_size(n->app, n->kind);
}

static int node_font(const zui_node *n);
static int text_font(int kind) {
	return kind == ZK_HEADING || kind == ZK_TITLE ? ZT_STRONG : kind == ZK_HEADER ? ZT_LIGHT : ZT_REGULAR;
}

static int node_font(const zui_node *n) {
	return n->quiet ? ZT_LIGHT : text_font(n->kind);
}

static float intrinsic_w(zui_node *n) {
	const struct zui_metrics *m = &n->app->style->m;
	switch (n->kind) {
	case ZK_BUTTON: {
		if (n->icon) return m->bar_slot > 0 ? m->bar_slot : 72;	/* a round button */
		float w = zt_width(n->text, ZT_MEDIUM, m->button_text) + m->button_h * 0.9f;
		return w < m->button_h * 1.6f ? m->button_h * 1.6f : w;
	}
	case ZK_TEXT: case ZK_HEADING: case ZK_CAPTION: case ZK_TITLE: case ZK_HEADER:
		return zt_width(n->text, text_font(n->kind), text_size(n->app, n->kind));
	case ZK_SPACE:
		return (float)n->space;
	default:
		return -1;	/* takes a share of the room */
	}
}

static float layout_node(zui_node *n, float x, float y, float w);

/* things in a card that read as a list: dividers go next to them */
static bool listy(const zui_node *n) {
	return n->kind == ZK_ITEM || n->kind == ZK_SWITCH || n->kind == ZK_SLIDER || n->kind == ZK_CHECK ||
		n->kind == ZK_RADIO;
}

static float layout_card(zui_node *n, float x, float y, float w, float gap) {
	float at = y;
	zui_node *prev = NULL;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		/* two things that are not list rows get room between them */
		if (prev && !listy(prev) && !listy(c)) at += gap;
		else if (prev && listy(prev) != listy(c)) at += gap * 0.5f;
		at += layout_node(c, x, at, w);
		prev = c;
	}
	return at - y;
}

static float layout_children_column(zui_node *n, float x, float y, float w, float gap) {
	float at = y;
	bool first = true;
	bool after_label = false;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		/* a section's label sits close over its card */
		if (!first) at += after_label ? gap * 0.35f : gap;
		at += layout_node(c, x, at, w);
		first = false;
		after_label = c->kind == ZK_SECTION_LABEL;
	}
	return at - y;
}

float zui_layout_at(zui_node *n, float x, float y, float w) { return layout_node(n, x, y, w); }

void zui_move_children(zui_node *n, float dy) {
	for (zui_node *c = n->first; c; c = c->next) {
		c->y += dy;
		zui_move_children(c, dy);
	}
}

/* as wide as what is in a bubble */
static float bubble_width(zui_node *n, float most) {
	float w = 0;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		float cw = 0;
		if (c->kind == ZK_IMAGE) return most;
		if (c->text && strlen(c->text) > 160) return most;
		if (c->text && c->kind == ZK_TEXT && strchr(c->text, '\n')) {
			/* widest line */
			const char *s = c->text;
			while (*s) {
				const char *e = strchr(s, '\n');
				size_t k = e ? (size_t)(e - s) : strlen(s);
				char line[200];
				snprintf(line, sizeof(line), "%.*s", (int)k, s);
				float lw = zt_width(line, ZT_REGULAR, 15);
				if (lw > cw) cw = lw;
				s += k + (e ? 1 : 0);
			}
		} else if (c->text) {
			cw = zt_width(c->text, node_font(c), node_text_size(c));
		}
		if (cw > w) w = cw;
	}
	return w + 1 > most ? most : w + 1;
}

/* ---- layout of the core's own kinds: each is a class operation, so that this does not switch on kind ---- */
static float node_pad(const zui_node *n) {
	return in_card(n) && !card_row_kind(n->kind) ? n->app->style->m.card_pad : 0;
}

static float lay_column(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	float pad = node_pad(n);
	return layout_children_column(n, x + pad, y, w - 2 * pad, n->has_gap ? n->row_gap : m->gap);
}

/* edge to edge, one under another; it reaches to the bottom of the window, and what does not fit under it
 * is what a finger moves */
static float lay_scroll(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	float inner = layout_children_column(n, x, y, w, 0);
	zui_sv(n)->content_h = y + inner;
	return app->height - y > 0 ? app->height - y : 0;
}

/* the margin on both sides, and room under the last thing */
static float lay_content_panel(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	float h = layout_children_column(n, x + m->margin, y, w - 2 * m->margin, n->has_gap ? n->row_gap : m->gap);
	return h > 0 ? h + m->margin * 2 : h;
}

static float lay_card(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	float h = layout_card(n, x, y, w, m->gap * 0.75f);
	if (n->first && m->card_r > 0) {
		/* a card's own breathing room above its first and under its last
		 * thing, unless those are rows that bring their own */
		zui_node *f = n->first, *l = n->last;
		while (f && !f->visible) f = f->next;
		while (l && !l->visible) l = l->prev;
		float top = f && !card_row_kind(f->kind) ? m->card_pad * 0.75f : 0;
		float bottom = l && !card_row_kind(l->kind) ? m->card_pad * 0.75f : 0;
		if (top > 0) {
			for (zui_node *c = n->first; c; c = c->next)
				if (c->visible) layout_node(c, c->x, c->y + top, c->w);
		}
		h += top + bottom;
	}
	return h;
}

static float lay_row(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	float pad = node_pad(n), h = 0;
	float gap = n->has_gap ? n->row_gap : m->gap * 0.75f, fixed = 0, flex = 0;
	int count = 0;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		count++;
		float iw = c->flex > 0 ? -1 : intrinsic_w(c);
		if (iw < 0) flex += c->flex > 0 ? (float)c->flex : 1;
		else fixed += iw;
	}
	float room = w - 2 * pad - fixed - gap * (count > 1 ? count - 1 : 0);
	if (room < 0) room = 0;
	float at = x + pad;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		float iw = c->flex > 0 ? -1 : intrinsic_w(c);
		float cw = iw < 0 ? room * (c->flex > 0 ? (float)c->flex : 1) / (flex > 0 ? flex : 1) : iw;
		float ch = layout_node(c, at, y, cw);
		if (ch > h) h = ch;
		at += cw + gap;
	}
	/* centred across the row's height */
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible || c->h >= h) continue;
		layout_node(c, c->x, y + (h - c->h) / 2, c->w);
	}
	return h;
}

static float lay_image(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	float pad = node_pad(n);
	if (n->fit) {
		n->w = w;
		return app->fit_h > 0 ? app->fit_h : app->height;
	}
	float iw = n->space > 0 ? (float)n->space : w - 2 * pad;
	float h = n->value > 0 ? (float)n->value : n->pix ? iw * n->pix->h / n->pix->w : iw;
	if (n->space > 0) n->x = x + (w - iw) / 2;
	n->w = iw;
	return h;
}

static float lay_list(zui_node *n, float x, float y, float w) { return zui_list_layout(n, x, y, w); }

static float lay_pivot(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	float h = 0;
	int i = 0;
	for (zui_node *c = n->first; c; c = c->next) {
		if (c->kind != ZK_PIVOT_ITEM) continue;	/* (the page's TitlePanel is a child too) */
		/* a tab is a ContentPanel: the margin is its own */
		if (i++ == zui_pivot_of(n)->cur) { h = layout_node(c, x + m->margin, y, w - 2 * m->margin); zui_sv(c)->content_h = c->y + h; }
		else {
			int k = zui_tab_count(n);
			int at = i - 1;	/* (i has gone on) */
			if (at == zui_pivot_of(n)->prev || at == (zui_pivot_of(n)->cur + 1) % k || at == (zui_pivot_of(n)->cur + k - 1) % k) {
				float hh = layout_node(c, x + m->margin, y, w - 2 * m->margin);
				zui_sv(c)->content_h = c->y + hh;
			}
		}
	}
	return h;
}

static float lay_footer(zui_node *n, float x, float y, float w) {
	float at = y;
	for (zui_node *c = n->first; c; c = c->next) if (c->visible) at += layout_node(c, x, at, w);
	return at - y;
}

static float lay_bubble(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	float bp = app->style->bubble_pad, most = w * 0.8f - 2 * bp;
	float bw = bubble_width(n, most);
	n->w = bw + 2 * bp;
	n->x = n->mine ? x + w - n->w : x;
	float at = y + 8 + bp;	/* (8: the tail, above a bubble) */
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		at += layout_node(c, n->x + bp, at, bw) + 2;
	}
	return at - y + bp - 2;
}

static float lay_section_label(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	(void)x; (void)y; (void)w;
	return zt_line_height(m->label) + m->section_gap;
}

static float lay_space(zui_node *n, float x, float y, float w) { (void)x; (void)y; (void)w; return (float)n->space; }

static float lay_text(zui_node *n, float x, float y, float w) {
	(void)x; (void)y;
	struct zt_layout *L = zt_slot_get(&n->t[0], n->text, node_font(n), node_text_size(n), w - 2 * node_pad(n), 0);
	return L->h;
}

static float lay_button(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	const struct zui_metrics *m = &app->style->m;
	(void)x; (void)y; (void)w;
	return n->icon ? (m->bar_button > 0 ? m->bar_button + (app->bar_words ? 16 : 0) : 66) : m->button_h;
}

static float lay_item(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	(void)x; (void)y; (void)w;
	float h = (n->subtitle && *n->subtitle) || n->detail ? m->item_h2 : m->item_h;
	if (n->pix || n->monogram || n->applist) h = n->group || n->applist ? 55 : 76;
	return h;
}

static float lay_switch(zui_node *n, float x, float y, float w) { (void)x; (void)y; (void)w; return n->app->style->m.item_h2; }
static float lay_slider(zui_node *n, float x, float y, float w) { (void)n; (void)x; (void)y; (void)w; return 56; }
static float lay_progress(zui_node *n, float x, float y, float w) { (void)n; (void)x; (void)y; (void)w; return 12; }

static float lay_field(zui_node *n, float x, float y, float w) {
	const struct zui_metrics *m = &n->app->style->m;
	(void)x; (void)y;
	float h = m->field_h + (n->inline_hint ? 0 : 22);
	if (zui_field_of(n)->multiline && n->text && *n->text) {
		char *t = NULL;
		if (asprintf(&t, "%s|", n->text) > 0) {
			struct zt_layout *L = zt_layout_make(t, ZT_REGULAR, 16, w - 20, 0);
			int lines = L->lines > 4 ? 4 : L->lines;
			float want = lines * L->line_h + 18;
			if (want > h) h = want;
			zt_layout_free(L);
			free(t);
		}
	}
	return h;
}

static float layout_node(zui_node *n, float x, float y, float w) {
	n->x = x;
	n->y = y;
	n->w = w;
	/* a kind with no layout of its own takes no room (the page and the frame lay out their own) */
	float h = n->cls->layout ? n->cls->layout(n, x, y, w) : 0;
	n->h = h;
	return h;
}

const char *zui_panel_text(const zui_node *page, int kind) {
	zui_node *tp = page ? zui_pg(page)->title_node : NULL;
	for (zui_node *c = tp ? tp->first : NULL; c; c = c->next)
		if (c->kind == kind) return c->text ? c->text : "";
	return "";
}

/* the TitlePanel's titles: from the status bar and 24 down, a small one's line hardly more than 8 */
static void layout_title_panel(zui_app *app, zui_node *page) {
	zui_node *tp = zui_pg(page)->title_node;
	if (!tp) return;
	const struct zui_metrics *m = &app->style->m;
	tp->x = 0;
	tp->y = 0;
	tp->w = app->width;
	tp->h = zui_pg(page)->title_panel.h;
	float at = m->top;
	for (zui_node *c = tp->first; c; c = c->next) {
		if (!c->visible) continue;
		float h = layout_node(c, c->kind == ZK_HEADER ? m->margin - 3 : m->margin, at, app->width - 2 * m->margin);
		at += c->kind == ZK_TITLE ? 8 : h;
	}
}

/* what the app keeps in the frame: edge to edge from the top of the window, one under another,
 * over the pages (the pages themselves are laid out one by one) */
static void layout_frame(zui_app *app) {
	float at = 0;
	for (zui_node *c = app->root->first; c; c = c->next) {
		if (zui_is_page(c) || !c->visible) continue;
		at += layout_node(c, 0, at, app->width);
	}
}

/* what is under a page's titles: its things one under another, edge to edge, or (a pivot, a hub) its own
 * body, which is the page's children laid out by the page's class */
static float layout_body(zui_node *page, float y, float w) {
	if (page->kind == ZK_PIVOT) return lay_pivot(page, 0, y, w);
	if (page->kind == ZK_HUB) return layout_hub(page);
	return layout_children_column(page, 0, y, w, 0);
}

void zui_do_layout(zui_app *app, zui_node *page) {
	zui_lock();
	layout_frame(app);
	const struct zui_metrics *m = &app->style->m;
	page->x = 0;
	page->y = 0;
	page->w = app->width;
	if (zui_pg(page)->fullscreen) {
		zui_pg(page)->title_panel = (struct zui_rect){ 0, 0, app->width, 0 };
		zui_pg(page)->content_panel = (struct zui_rect){ 0, 0, app->width, app->height };
		float foot = 0;
		if (zui_pg(page)->footer && zui_pg(page)->footer->visible) {
			foot = layout_node(zui_pg(page)->footer, m->margin, 0, app->width - 2 * m->margin) + 16;
			zui_pg(page)->footer->visible = false;
		}
		app->fit_h = app->height - (foot > 0 ? foot + 8 : 0);
		layout_body(page, 0, app->width);
		if (foot > 0) {
			zui_pg(page)->footer->visible = true;
			layout_node(zui_pg(page)->footer, m->margin, app->height - foot + 8, app->width - 2 * m->margin);
		}
		zui_pg(page)->content_h = app->height;
		page->h = zui_pg(page)->content_h;
		zui_unlock();
		return;
	}
	float title_h = zui_pg(page)->compact ? m->title_compact_h : zui_page_pivot(page) ? m->title_h + app->style->tabs_h
		: zui_pg(page)->title_node ? m->title_h : 0;	/* no TitlePanel: the root starts at the top of the window */
	zui_pg(page)->title_panel = (struct zui_rect){ 0, 0, app->width, title_h };
	zui_pg(page)->content_panel = (struct zui_rect){ 0, title_h, app->width, app->height - title_h };
	layout_title_panel(app, page);
	float y = zui_pg(page)->content_panel.y;
	y = zui_page_pivot(page) ? y + 10 : zui_pg(page)->footer ? y + 8 : app->style->page_top(app, page, y, false);
	float foot = 0;
	if (zui_pg(page)->footer && zui_pg(page)->footer->visible) {
		zui_pg(page)->footer->visible = false;	/* laid out by itself, in window points */
		y += layout_body(page, y, app->width);
		zui_pg(page)->footer->visible = true;
		foot = layout_node(zui_pg(page)->footer, m->margin, 0, app->width - 2 * m->margin) + 16;
		layout_node(zui_pg(page)->footer, m->margin, app->height - foot + 8, app->width - 2 * m->margin);
	} else {
		y += layout_body(page, y, app->width);	/* the root: edge to edge */
	}
	zui_pg(page)->content_h = y + (foot > 0 ? foot + 8 : 0);
	page->h = zui_pg(page)->content_h;
	zui_unlock();
}

ZUI_EXPORT void zui_layout(zui_app *app, zui_node *page, int width, int height) {
	page = zui_page_of(page);
	app->width = (float)width;
	app->height = (float)height;
	if (!app->page) app->page = page;
	zui_do_layout(app, page);
}

/* the first scroll viewer on a page (not in tabs or a hub, which scroll by themselves) */
static zui_node *find_scroll(zui_node *n) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible || c->kind == ZK_FOOTER || c->kind == ZK_PIVOT || c->kind == ZK_HUB) continue;
		if (c->kind == ZK_SCROLL) return c;
		zui_node *deeper = find_scroll(c);
		if (deeper) return deeper;
	}
	return NULL;
}

/* What scrolls on a page is a scroll container (or an heir of it: the section in front of a hub, the item in
 * front of a pivot), never the page; NULL when the page has none. Its scroll, vel and content_h are what a
 * finger moves. */
struct zui_scroll *zui_scroller(zui_node *page) {
	if (zui_page_hub(page)) {
		zui_node *front = zui_hub_front(zui_page_hub(page));
		if (front) return zui_sv(front);
	}
	if (zui_page_pivot(page)) {
		zui_node *tab = zui_tab_at(zui_page_pivot(page), zui_pivot_of(zui_page_pivot(page))->cur);
		if (tab) return zui_sv(tab);
	}
	return zui_sv(find_scroll(page));
}

/* a page does not scroll by itself, as on Windows Phone: only a scroll container does */
static float scroll_max(zui_app *app, const struct zui_scroll *sv) {
	if (!sv) return 0;
	float max = sv->content_h - app->height;
	return max > 0 ? max : 0;
}

/* ---- the finger ---- */
static bool interactive(const zui_node *n) {
	return n->enabled && n->cls->interactive && n->cls->interactive(n);
}

static zui_node *hit_in(zui_node *n, float x, float y) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible || c->kind == ZK_FOOTER) continue;
		if (n->kind == ZK_PIVOT && c != zui_tab_at(n, zui_pivot_of(n)->cur)) continue;
		/* a hub's sections have slid, each by its own (they go round) */
		if (n->kind == ZK_HUB) {
			/* a section may show in two places: each is tried */
			float dx[3];
			int copies = zui_hub_copies(n, c, dx);
			for (int k = 0; k < copies; k++) {
				float cx = x + dx[k], cy = y + zui_sv(c)->scroll;
				if (y < n->y + zui_hub_content_top(n)) continue;	/* the hub's title is over the sections' top */
				if (cx < c->x || cx >= c->x + c->w || cy < c->y || cy >= c->y + c->h) continue;
				zui_node *deeper = hit_in(c, cx, cy);
				if (deeper) return deeper;
			}
			continue;
		}
		/* a tab scrolls by itself: what is on the screen is `scroll` further down in it */
		float cy = n->kind == ZK_PIVOT ? y + zui_sv(c)->scroll : n->kind == ZK_SCROLL ? y + zui_sv(n)->scroll : y;
		if (x < c->x || x >= c->x + c->w || cy < c->y || cy >= c->y + c->h) continue;
		zui_node *deeper = hit_in(c, x, cy);
		if (deeper) return deeper;
		if (interactive(c)) return c;
	}
	return NULL;
}

static void focus(zui_app *app, zui_node *field) {
	if (app->focus == field) return;
	uint32_t now = zui_now();
	if (app->focus) {
		app->focus->focused = false;
		zanim_go(&app->focus->focus_anim, 0, 150, now);
	}
	app->focus = field;
	if (field) {
		field->focused = true;
		zanim_go(&field->focus_anim, 1, 150, now);
	}
	zui_osk(app, field != NULL);
	app->dirty = true;
}

static void slider_to(zui_app *app, zui_node *s, float x) {
	float pad = in_card(s) ? app->style->m.card_pad : 0;
	x += zui_node_hub_shift(s, x);
	float x0 = s->x + pad, w = s->w - 2 * pad;
	double v = w > 0 ? (x - x0) / w : 0;
	v = v < 0 ? 0 : v > 1 ? 1 : v;
	if (fabs(v - s->value) < 1e-4) return;
	s->value = v;
	zui_fire(s, ZUI_EVENT_CHANGE);
}

/* where the finger put the caret: the character boundary nearest it; twice: the word */
static void field_tapped(zui_app *app, zui_node *f, bool twice) {
	const char *t = f->text ? f->text : "";
	size_t len = strlen(t);
	zui_field_of(f)->sel_a = zui_field_of(f)->sel_b = 0;
	if (!zui_field_of(f)->multiline || !len || len > 1500) { zui_field_of(f)->caret = (int)len; return; }
	float tx = app->last_x + zui_node_hub_shift(f, app->last_x) - f->x - 10, ty = app->last_y - f->y - 9;
	if (f->parent) {}
	int best = (int)len;
	float bd = 1e9f;
	for (size_t i = 0; i <= len; i++) {
		if (i < len && ((unsigned char)t[i] & 0xc0) == 0x80) continue;
		float x, y, lh;
		zt_caret_at(t, (int)i, f->w - 20, &x, &y, &lh);
		float dy = ty - (y + lh / 2), dx = tx - x, d = dx * dx + dy * dy * 6;
		if (d < bd) { bd = d; best = (int)i; }
	}
	zui_field_of(f)->caret = best;
	if (twice) {
		int a = best, b = best;
		while (a > 0 && t[a - 1] != ' ' && t[a - 1] != '\n') a--;
		while (t[b] && t[b] != ' ' && t[b] != '\n') b++;
		zui_field_of(f)->sel_a = a;
		zui_field_of(f)->sel_b = b;
		zui_field_of(f)->caret = b;
	}
	app->dirty = true;
}

/* what a tap does: the class's own, else a click */
static void activate(zui_app *app, zui_node *n) {
	if (n->cls->activate) {
		n->cls->activate(app, n);
		return;
	}
	zui_haptic("tap");
	zui_fire(n, ZUI_EVENT_CLICK);
}

static bool int_always(const zui_node *n) { (void)n; return true; }
static bool int_with_handler(const zui_node *n) { return n->cb != NULL || n->hold_cb != NULL; }

static void act_switch(zui_app *app, zui_node *n) {
	(void)app;
	zui_set_on(n, !n->on);
	zui_haptic("click");
	zui_fire(n, ZUI_EVENT_CHANGE);
}

static void act_field(zui_app *app, zui_node *n) {
	uint32_t t = zui_now();
	bool twice = app->focus == n && t - zui_field_of(n)->last_tap < 350;
	zui_field_of(n)->last_tap = t;
	focus(app, n);
	field_tapped(app, n, twice);
}

/* what of the frame's own is under the finger (window points: it does not scroll) */
static zui_node *frame_hit(zui_app *app, float x, float y) {
	for (zui_node *c = app->root->first; c; c = c->next) {
		if (zui_is_page(c) || !c->visible) continue;
		if (x < c->x || x >= c->x + c->w || y < c->y || y >= c->y + c->h) continue;
		zui_node *deeper = hit_in(c, x, y);
		if (deeper) return deeper;
		if (interactive(c)) return c;
	}
	return NULL;
}

/* phase 0 down, 1 move, 2 up, 3 cancel; x, y in window points */
void zui_touch(zui_app *app, int phase, float x, float y, uint32_t time) {
	zui_node *page = app->page;
	if (!page) return;
	struct zui_scroll *sv = zui_scroller(page);
	uint32_t now = zui_now();
	if (!app->laid_out) {
		zui_do_layout(app, page);
		app->laid_out = true;
	}
	if (app->ov) {
		/* a menu, a dialog or a picker over the page takes every touch */
		if (zui_widgets_touch(app, phase, x, y, time)) {
			if (phase >= 2) {
				app->touching = app->dragging = false;
				app->hold_fired = false;
				app->hold_node = NULL;
				zui_hold_cancel(app);
			}
			return;
		}
	}
	if (phase == 0) {
		/* the suggestions under a field are over the page and get the touch first */
		if (zui_suggest_touch(app, x, y, 0)) return;
		zui_hold_start(app, NULL, time);
		app->hub_drag = false;
		if (app->trans_to) transition_finish(app);
		app->touching = true;
		app->dragging = false;
		app->down_x = app->last_x = x;
		app->down_y = app->last_y = y;
		app->last_t = time;
		app->down_scroll = sv ? sv->scroll : 0;
		app->fling = false;
		if (sv) sv->vel = 0;
		app->pressed = NULL;
		app->drag_slider = NULL;
		zui_lock();
		zui_node *fh = frame_hit(app, x, y);
		if (fh) {
			/* what the app keeps in the frame is over the page and gets the touch first */
			app->in_footer = true;
			app->press_fx = app->press_fy = 0;
			app->pressed = fh;
			zanim_go(&app->press, 1, 90, now);
			zui_hold_start(app, fh, time);
			app->dirty = true;
			zui_unlock();
			return;
		}
		zui_unlock();
		if (y < zui_pg(page)->title_panel.h) {
			return;	/* the header: a tap there is handled on the lift */
		}
		if (sv) sv->stick_end = false;
		zui_lock();
		zui_node *hit = NULL;
		if (zui_pg(page)->footer && zui_pg(page)->footer->visible && y >= zui_pg(page)->footer->y - 8) {
			hit = hit_in(zui_pg(page)->footer, x, y);
			app->in_footer = true;
			for (zui_node *b = zui_pg(page)->footer->first; b; b = b->next)
				if (b->bar && !b->no_dots && app->style->m.bar_button > 0 && x > app->width - 44 && y >= b->y - 4) {
					app->bar_words = !app->bar_words;
					app->laid_out = false;
					app->dirty = true;
					return;
				}
			app->press_fx = app->press_fy = 0;
			if (hit) {
				app->pressed = hit;
				zanim_go(&app->press, 1, 90, now);
				zui_hold_start(app, hit, time);
			}
			app->dirty = true;
			zui_unlock();
			return;
		}
		app->in_footer = false;
		hit = hit_in(page, x, y);
		zui_unlock();
		if (hit && hit->kind == ZK_SLIDER) {
			app->drag_slider = hit;
			slider_to(app, hit, x);
			zui_haptic("tick");
			return;
		}
		if (hit) {
			float sx = x + zui_node_hub_shift(hit, x);
			app->pressed = hit;
			app->press_fx = hit->w > 0 ? (sx - hit->x) / hit->w * 2 - 1 : 0;
			app->press_fy = hit->h > 0 ? (y + (sv ? sv->scroll : 0) - hit->y) / hit->h * 2 - 1 : 0;
			zanim_go(&app->press, 1, 90, now);
			zui_hold_start(app, hit, time);
		}
		app->dirty = true;
		return;
	}
	if (!app->touching) return;
	if (phase == 1) {
		if (app->drag_slider) {
			slider_to(app, app->drag_slider, x);
			return;
		}
		float dy = y - app->down_y, dx = x - app->down_x;
		if (app->in_footer) return;
		if (zui_page_hub(page) && !app->dragging && !app->hub_drag && !app->swiping && fabsf(dx) > SLOP && fabsf(dx) > fabsf(dy) * 1.2f) {
			/* the hub's sections follow the finger sideways */
			app->hub_drag = true;
			app->hub_from = zui_hub_begin_drag(zui_page_hub(page));
			app->swipe_v = 0;
			zui_hold_cancel(app);
			if (app->pressed) zanim_go(&app->press, 0, 160, now);
		}
		if (app->hub_drag) {
			float dt = (float)(int32_t)(time - app->last_t);
			if (dt > 0) app->swipe_v = app->swipe_v * 0.3f + (-(x - app->last_x) / dt) * 0.7f;
			app->last_t = time;
			app->last_x = x;
			app->last_y = y;
			zui_hub_drag(app, zui_page_hub(page), dx);
			return;
		}
		if (!app->dragging && !app->swiping && zui_page_pivot(page) && fabsf(dx) > SLOP && fabsf(dx) > fabsf(dy) * 1.2f) {
			app->swiping = true;
			zui_hold_cancel(app);
			if (app->pressed) zanim_go(&app->press, 0, 160, now);
			app->laid_out = false;
		}
		if (app->swiping) {
			zui_node *T = zui_page_pivot(page);
			float pos = -dx / app->width;
			int k = zui_tab_count(T);
			float dt = (float)(int32_t)(time - app->last_t);
			if (dt > 0) app->swipe_v = app->swipe_v * 0.3f + (-(x - app->last_x) / app->width / dt) * 0.7f;
			app->last_t = time;
			zanim_set(&zui_pivot_of(T)->pos, pos);
			zui_pivot_of(T)->prev = ((zui_pivot_of(T)->cur + (pos > 0 ? 1 : -1)) % k + k) % k;
			app->last_x = x;
			app->dirty = true;
			return;
		}
		if (!app->dragging && fabsf(dy) > SLOP) {
			app->dragging = true;
			zui_hold_cancel(app);
			if (app->pressed) zanim_go(&app->press, 0, 160, now);
		}
		if (app->dragging && !sv) {
			app->dirty = true;	/* nothing here scrolls: the finger only takes the press off what it was on */
		} else if (app->dragging) {
			float raw = app->down_scroll - dy, max = scroll_max(app, sv);
			/* past either end the page follows the finger less and less */
			if (raw < 0) raw *= 0.4f;
			else if (raw > max) raw = max + (raw - max) * 0.4f;
			float dt = (float)(time - app->last_t);
			if (dt > 0) {
				float v = -(y - app->last_y) / dt;	/* points per ms */
				sv->vel = sv->vel * 0.3f + v * 0.7f;
			}
			sv->scroll = raw;
			app->dirty = true;
		}
		app->last_x = x;
		app->last_y = y;
		app->last_t = time;
		return;
	}
	/* up or cancel */
	app->touching = false;
	zui_hold_cancel(app);
	if (app->hub_drag) {
		app->hub_drag = false;
		zui_hub_release(app, zui_page_hub(page), app->swipe_v);
		app->swipe_v = 0;
		app->hold_fired = false;
		return;
	}
	if (app->swiping) {
		app->swiping = false;
		zui_node *T = zui_page_pivot(page);
		float pos = zui_pivot_of(T)->pos.to, v = app->swipe_v, sw = 50.0f / app->width, fl = 0.3f / app->width;
		/* as ZrnMetroCore's pivots: 50 points, or 0.3 points a ms, turn to the next */
		zui_tabs_step(T, v > fl || (pos > sw && v >= -fl) ? 1 : v < -fl || (pos < -sw && v <= fl) ? -1 : 0);
		zui_pivot_of(T)->pos.v0 = v;
		zui_pivot_of(T)->pos.w = 0.018f;
		zui_pivot_of(T)->pos.ms = 420;
		app->swipe_v = 0;
		return;
	}
	if (app->drag_slider) {
		zui_node *s = app->drag_slider;
		app->drag_slider = NULL;
		if (phase == 2) zui_fire(s, ZUI_EVENT_SUBMIT);
		return;
	}
	if (app->dragging) {
		app->dragging = false;
		if (phase == 2 && (int32_t)(time - app->last_t) < 80) {
			app->fling = true;
			app->fling_t = now;
		} else if (sv) {
			sv->vel = 0;
		}
		app->dirty = true;
		return;
	}
	if (phase != 2) {
		if (app->pressed) zanim_go(&app->press, 0, 160, now);
		return;
	}
	/* a hold already did what it does; the lift is not a tap */
	if (zui_hold_on_lift(app, time, app->hold_since)) {
		app->dirty = true;
		return;
	}
	/* a tap */
	if (!app->in_footer && app->down_y < zui_pg(page)->title_panel.h && y < zui_pg(page)->title_panel.h) {
		if (zui_page_pivot(page)) {
			/* the next title: slide to it, and they go round */
			zui_haptic("tick");
			zui_tabs_step(zui_page_pivot(page), 1);
			return;
		}
		if (zui_pg(page)->depth > 0) {
			zui_haptic("tap");
			zui_back(app);
		}
		return;
	}
	zui_node *hit = app->pressed;
	zui_pickers_close_except(app, hit);	/* a tap anywhere else shuts a list picker */
	if (hit) {
		zanim_go(&app->press, 0, 200, now);
		/* the press shows for at least a moment even on a quick tap */
		if (app->press.from < 0.6f) {
			app->press.from = 1;
		}
		activate(app, hit);
	} else if (app->focus) {
		focus(app, NULL);
	}
	app->dirty = true;
}

ZUI_EXPORT void zui_page_scroll_to(zui_node *page, double y, bool glide) {
	page = zui_page_of(page);
	struct zui_scroll *sv;
	zui_app *app = page->app;
	if (!app->laid_out) zui_do_layout(app, page);
	sv = zui_scroller(page);
	if (!sv) return;	/* nothing here scrolls */
	float max = scroll_max(app, sv);
	float to = (float)y - (sv->base.kind == ZK_SCROLL ? sv->base.y : zui_pg(page)->title_panel.h);
	to = to < 0 ? 0 : to > max ? max : to;
	sv->stick_end = false;
	sv->vel = 0;
	app->fling = false;
	if (glide && app->wl) {
		sv->goal = to;
		sv->goal_on = true;
	} else {
		sv->scroll = to;
		sv->goal_on = false;
	}
	app->dirty = true;
}

ZUI_EXPORT void zui_inject_touch(zui_app *app, int phase, double x, double y) {
	static uint32_t t;
	t += 16;
	zui_touch(app, phase, (float)x, (float)y, t);
}

ZUI_EXPORT void zui_inject_text(zui_app *app, const char *utf8) {
	zui_key_text(app, utf8);
}

void zui_scroll_by(zui_app *app, float dy) {
	zui_node *page = app->page;
	if (!page) return;
	struct zui_scroll *sv = zui_scroller(page);
	if (!sv) return;
	float s = sv->scroll + dy, max = scroll_max(app, sv);
	sv->scroll = s < 0 ? 0 : s > max ? max : s;
	app->dirty = true;
}

void zui_key_text(zui_app *app, const char *utf8) {
	zui_node *f = app->focus;
	if (!f) return;
	if (strcmp(utf8, "\n") == 0 && !zui_field_of(f)->multiline) {
		zui_fire(f, ZUI_EVENT_SUBMIT);
		return;
	}
	if (zui_field_of(f)->multiline) app->laid_out = false;
	size_t len = f->text ? strlen(f->text) : 0;
	size_t at = zui_field_of(f)->caret < 0 || (size_t)zui_field_of(f)->caret > len ? len : (size_t)zui_field_of(f)->caret, cut_a = at, cut_b = at;
	bool back = strcmp(utf8, "\b") == 0;
	if (zui_field_of(f)->sel_b > zui_field_of(f)->sel_a && (size_t)zui_field_of(f)->sel_b <= len) {
		cut_a = (size_t)zui_field_of(f)->sel_a;
		cut_b = (size_t)zui_field_of(f)->sel_b;
	} else if (back) {
		if (!at) return;
		cut_a = at - 1;
		while (cut_a > 0 && ((unsigned char)f->text[cut_a] & 0xc0) == 0x80) cut_a--;
	}
	const char *ins = back ? "" : utf8;
	size_t il = strlen(ins);
	char *t = malloc(len - (cut_b - cut_a) + il + 1);
	memcpy(t, f->text ? f->text : "", cut_a);
	memcpy(t + cut_a, ins, il);
	strcpy(t + cut_a + il, f->text ? f->text + cut_b : "");
	free(f->text);
	f->text = t;
	zui_field_of(f)->caret = (int)(cut_a + il);
	zui_field_of(f)->sel_a = zui_field_of(f)->sel_b = 0;
	app->dirty = true;
	zui_fire(f, ZUI_EVENT_CHANGE);
}

/* the keyboard is the shell's: asked for down its control pipe, with where
 * the field ends so the shell can lift the app over the keyboard */
void zui_osk(zui_app *app, bool show) {
	const char *runtime = getenv("XDG_RUNTIME_DIR");
	if (!runtime || !app->wl) return;
	char path[300], line[64];
	snprintf(path, sizeof(path), "%s/ztmetro.ctl", runtime);
	if (show && app->focus && app->page) {
		bool foot = false;
		for (zui_node *p = app->focus; p; p = p->parent) foot = foot || p->kind == ZK_FOOTER;
		int bottom = foot ? (int)lroundf(app->height) : (int)lroundf(app->focus->y + app->focus->h - (zui_scroller(app->page) ? zui_scroller(app->page)->scroll : 0));
		snprintf(line, sizeof(line), "osk show %d\n", bottom);
	} else {
		snprintf(line, sizeof(line), show ? "osk show\n" : "osk hide\n");
	}
	int fd = open(path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
	if (fd >= 0) {
		if (write(fd, line, strlen(line)) < 0) {}
		close(fd);
	}
}

ZUI_EXPORT void zui_haptic(const char *kind) {
	static int fd = -1;
	static uint32_t tried;
	if (fd < 0) {
		uint32_t now = zui_now();
		const char *runtime = getenv("XDG_RUNTIME_DIR");
		if (!runtime || (tried && now - tried < 2000)) return;
		tried = now ? now : 1;
		char path[300];
		snprintf(path, sizeof(path), "%s/ztmetro.ctl", runtime);
		fd = open(path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd < 0) return;
	}
	char line[48];
	int n = snprintf(line, sizeof(line), "haptic %s\n", kind);
	if (write(fd, line, (size_t)n) < 0 && errno != EAGAIN) {
		close(fd);
		fd = -1;
	}
}

/* ---- motion between frames ---- */
static uint32_t last_step;

void zui_step(zui_app *app, uint32_t now) {
	zui_node *page = app->page;
	float dt = last_step ? (float)(int32_t)(now - last_step) : 16;
	if (dt > 64) dt = 64;
	if (dt < 0) dt = 0;
	last_step = now;
	if (app->trans_to && (int32_t)(now - app->trans_start) >= app->style->transition_ms)
		transition_finish(app);
	zui_widgets_step(app, now);
	if (!page) return;
	struct zui_scroll *sv = zui_scroller(page);
	if (sv && sv->goal_on && !app->touching) {
		sv->scroll += (sv->goal - sv->scroll) * (1 - expf(-dt / 90));
		if (fabsf(sv->goal - sv->scroll) < 0.5f) { sv->scroll = sv->goal; sv->goal_on = false; }
		app->dirty = true;
	}
	if (zui_page_pivot(page) && !app->swiping) {
		bool running = false;
		zanim_value(&zui_pivot_of(zui_page_pivot(page))->pos, now, &running);
		if (running) app->dirty = true;
	}
	if (app->touching) return;
	if (!sv) {
		app->fling = false;	/* nothing here scrolls */
		return;
	}
	float max = scroll_max(app, sv);
	if (sv->stick_end) {
		if (sv->glide && (int32_t)(now - sv->glide_at) < 600) {
			sv->scroll += (max - sv->scroll) * (1 - expf(-dt / 45));
			if (fabsf(max - sv->scroll) < 0.5f) { sv->scroll = max; sv->glide = false; }
			app->dirty = true;
		} else {
			sv->scroll = max;
		}
	}
	if (app->fling) {
		sv->scroll += sv->vel * dt;
		float bound = sv->scroll < 0 ? 0 : sv->scroll > max ? max : sv->scroll;
		if (bound != sv->scroll) {
			sv->vel *= expf(-dt / 35);
			sv->scroll += (bound - sv->scroll) * (1 - expf(-dt / 110));
			if (fabsf(sv->vel) < 0.05f && fabsf(bound - sv->scroll) < 0.5f) {
				sv->scroll = bound;
				app->fling = false;
			}
		} else {
			sv->vel *= expf(-dt / 350);
			if (fabsf(sv->vel) < 0.02f) {
				app->fling = false;
				sv->vel = 0;
			}
		}
		app->dirty = true;
	}
	if (!app->fling && (sv->scroll < 0 || sv->scroll > max)) {
		float target = sv->scroll < 0 ? 0 : max;
		sv->scroll += (target - sv->scroll) * (1 - expf(-dt / 110));
		if (fabsf(target - sv->scroll) < 0.3f) sv->scroll = target;
		app->dirty = true;
	}
}

static bool has_indeterminate(zui_node *n) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		if (c->kind == ZK_PROGRESS && c->value < 0) return true;
		if (has_indeterminate(c)) return true;
	}
	return false;
}

bool zui_animating(zui_app *app) {
	uint32_t now = zui_now();
	zui_node *page = app->page;
	struct zui_scroll *sv = page ? zui_scroller(page) : NULL;
	if (app->trans_to || app->fling || app->busy) return true;
	if (sv && sv->glide && sv->stick_end) return true;
	if (page && zui_page_pivot(page) && zui_pivot_of(zui_page_pivot(page))->pos.ms && (int32_t)(now - zui_pivot_of(zui_page_pivot(page))->pos.start) < (int32_t)zui_pivot_of(zui_page_pivot(page))->pos.ms) return true;
	if ((int32_t)(anim_until - now) > 0) return true;
	if (app->toast && (int32_t)(now - app->toast_at) < 2600) return true;
	if (sv && !app->touching && (sv->scroll < 0 || sv->scroll > scroll_max(app, sv))) return true;
	if (page && has_indeterminate(page)) return true;
	if (sv && sv->goal_on) return true;
	if (zui_widgets_animating(app, now)) return true;
	if (page && zui_widgets_page_moving(app, page, now)) return true;
	return false;
}

/* ---- drawing ---- */
static void draw_node(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now);
static int turn_role, turn_dir;
static float turn_t, turn_scroll;

/* Rows are made before anything is drawn: a row that turns out another height
 * than guessed moves the scroll, and a frame must not show half of each. */
static void ensure_lists(zui_node *n, uint32_t now) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		if (c->kind == ZK_LIST) { zui_list_ensure(c, now); continue; }
		if (n->kind == ZK_PIVOT && c->kind == ZK_PIVOT_ITEM) {
			int i = zui_tab_index(n, c), k = zui_tab_count(n);
			float off = zanim_value(&zui_pivot_of(n)->pos, now, NULL);
			int other = ((zui_pivot_of(n)->cur + (off > 0 ? 1 : -1)) % k + k) % k;
			if (i != zui_pivot_of(n)->cur && !(fabsf(off) > 0.001f && i == other)) continue;
		}
		ensure_lists(c, now);
	}
}

static void draw_children(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now) {
	const struct zui_style *st = app->style;
	zui_node *prev = NULL;
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		if (n->kind == ZK_CARD && prev && (listy(prev) || listy(c))) {
			zg_set_xf(base);
			st->divider(app, n, c);
		}
		draw_node(app, c, base, now);
		prev = c;
	}
}

/* ---- drawing of the core's own kinds: class operations like the layout's ---- */
static float draw_press;	/* how far the node being drawn is pressed (what its class's draw may look at) */

static void drw_scroll(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	/* its things are drawn where its scroll has moved them, inside its own box */
	zui_node *pg = zui_page_of(n);
	zg_clip(0, n->y, app->width, n->h);
	struct zg_xf in = *xf;
	zg_xf_translate(&in, 0, -roundf(zui_sv(n)->scroll * zui_gfx_scale()) / zui_gfx_scale(), 0);
	draw_children(app, n, &in, now);
	zg_clip(0, zui_pg(pg)->title_panel.h, app->width, app->height - zui_pg(pg)->title_panel.h);
}

static void drw_column(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	struct zui_palette pal = app->style->palette(app);
	if (n->bar) zg_rect(0, n->y - 4, app->width, app->height - n->y + 4, 0, pal.surface2);
	if (n->bar && !n->no_dots && app->style->m.bar_button > 0)	/* WP's three dots: open it for the words */
		for (int d = 0; d < 3; d++) zg_circle(app->width - 26 + d * 7, n->y + 6, 1.5f, pal.text);
	draw_children(app, n, xf, now);
}

static void drw_footer(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	if (app->style->footer) app->style->footer(app, n);
	draw_children(app, n, xf, now);
}

static void drw_list(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	struct zui_scroll *owner = zui_sv(zui_list_owner(n));
	float sc = owner ? owner->scroll : 0;
	zui_list_draw(app, n, xf, now, turn_role, turn_t, turn_dir, sc);
}

static void drw_pivot(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	bool running = false;
	float off = zanim_value(&zui_pivot_of(n)->pos, now, &running);
	int cur = zui_pivot_of(n)->cur;
	zui_node *front = zui_tab_at(n, cur);
	float W = app->width;
	struct zg_xf a = *xf;
	zg_xf_translate(&a, -off * W, front ? -zui_sv(front)->scroll : 0, 0);
	if (fabsf(off) > 0.001f) {
		int k = zui_tab_count(n);
		int other = ((cur + (off > 0 ? 1 : -1)) % k + k) % k;
		zui_node *o = zui_tab_at(n, other);
		if (o) {
			struct zg_xf b = *xf;
			zg_xf_translate(&b, -off * W + (off > 0 ? W : -W), -zui_sv(o)->scroll, 0);
			draw_node(app, o, &b, now);
		}
	}
	if (front) draw_node(app, front, &a, now);
}

static void drw_bubble(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	app->style->bubble(app, n);
	draw_children(app, n, xf, now);
}

static void drw_image(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	struct zui_palette pal = app->style->palette(app);
	(void)xf; (void)now;
	if (n->fit) {  /* whole, centred, the larger side touching the edges */
		if (n->pix) {
			float k = fminf(n->w / n->pix->w, n->h / n->pix->h), w = n->pix->w * k, h = n->pix->h * k;
			zui_draw_pixmap(app, n->pix, n->x + (n->w - w) / 2, n->y + (n->h - h) / 2, w, h, 0, n->sharp, 0);
		}
		return;
	}
	zui_draw_pixmap(app, n->pix, n->x, n->y, n->w, n->h, n->radius, n->sharp, pal.surface2);
}

static void drw_card(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	app->style->card(app, n);
	draw_children(app, n, xf, now);
}

static void drw_section_label(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	(void)xf; (void)now;
	app->style->section_label(app, n);
}

static void drw_text(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	struct zui_palette pal = app->style->palette(app);
	float pad = node_pad(n);
	(void)xf; (void)now;
	struct zt_layout *L = zt_slot_get(&n->t[0], n->text, node_font(n), node_text_size(n), n->w - 2 * pad, 0);
	float tx = n->x + pad;
	if (n->align == 1) tx = n->x + n->w - pad - L->w;
	else if (n->align == 2) tx = n->x + (n->w - L->w) / 2;
	uint32_t ink = n->kind == ZK_CAPTION || n->quiet ? pal.text2 : pal.text;
	if (n->parent && n->parent->kind == ZK_BUBBLE && n->kind == ZK_CAPTION) ink = pal.bubble_caption;
	zt_draw(L, tx, n->y, ink);
}

static void drw_button(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { (void)xf; (void)now; app->style->button(app, n, draw_press); }
static void drw_item(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { (void)xf; (void)now; app->style->item(app, n, draw_press); }
static void drw_slider(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { (void)xf; (void)now; app->style->slider(app, n); }
static void drw_progress(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { (void)xf; app->style->progress(app, n, now); }
static void drw_field(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { (void)xf; app->style->field(app, n, now); }

static void draw_node(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now) {
	const struct zui_style *st = app->style;
	float press = 0;
	struct zg_xf xf = *base;
	if (n == app->pressed) {
		bool running = false;
		press = zanim_value(&app->press, now, &running);
		if (press > 0.001f && st->press_xf) {
			struct zg_xf p;
			zg_xf_identity(&p);
			st->press_xf(app, n, press, app->press_fx, app->press_fy, &p);
			/* the press happens on the page, before the page's own movement */
			struct zg_xf combined;
			/* combined = base * p: apply p first, then base */
			float r[16];
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) {
					float s = 0;
					for (int k = 0; k < 4; k++) s += base->m[i * 4 + k] * p.m[k * 4 + j];
					r[i * 4 + j] = s;
				}
			memcpy(combined.m, r, sizeof(r));
			xf = combined;
		}
		if (!running && press <= 0.001f) app->pressed = NULL;
	}
	float alpha = zg_get_alpha();
	if (n->swivel_at) {
		float dur = n->swivel_out ? 200 : 300, u = (float)(int32_t)(now - n->swivel_at) / dur;
		u = u < 0 ? 0 : u > 1 ? 1 : u;
		float e = zui_ease_emph(u);
		zg_xf_rotate_x(&xf, n->swivel_out ? 60 * e : -45 * (1 - e), n->y + n->h / 2);
		zg_set_alpha(alpha * (n->swivel_out ? 1 - u : (u <= 0 ? 0 : e)));
		if (u >= 1 && !n->swivel_out) n->swivel_at = 0;
		else app->dirty = true;
	}
	zg_set_xf(&xf);
	if (!n->enabled) zg_set_alpha(zg_get_alpha() * 0.4f);
	if (n->bg) zui_brush_paint(app, n->bg, n->x, n->y, n->w, n->h, 0);	/* behind it */
	draw_press = press;
	if (n->cls->draw) n->cls->draw(app, n, &xf, now);
	zg_set_alpha(alpha);
}

void zui_draw_node(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now) { draw_node(app, n, base, now); }

bool zui_has_pressed(const zui_app *app, const zui_node *n) {
	for (const zui_node *p = app->pressed; p; p = p->parent) if (p == n) return true;
	return false;
}

/* one page, moved by `trans` (NULL at rest); role: 0 at rest, 1 incoming, 2 outgoing */
static void draw_page(zui_app *app, zui_node *page, uint32_t now, int role, float t, int dir) {
	const struct zui_style *st = app->style;
	/* whole pixels, as the shell moves a window: glyphs stay on their grid and
	 * do not shimmer as a list goes by */
	float scroll = 0;	/* a page does not scroll: what does is a scroll container in it */
	turn_role = role;
	turn_t = t;
	turn_dir = dir;
	struct zui_scroll *sv = zui_scroller(page);
	turn_scroll = sv ? sv->scroll : 0;
	/* DotMatrix's lattice moves with what is on it, or dots would crawl */
	if (page->bg) {
		/* the page's own, over the whole window: behind the titles and what scrolls, and itself still */
		struct zg_xf still;
		zg_xf_identity(&still);
		zg_set_xf(&still);
		zg_set_alpha(1);
		zg_set_dot_origin(0);
		zui_brush_paint(app, page->bg, 0, 0, app->width, app->height, 0);
	}
	zg_set_dot_origin(role || !sv ? 0 : -roundf(sv->scroll * zui_gfx_scale()) / zui_gfx_scale());
	/* a pivot or a hub is a page whose body is one thing, its class draws it: one row of the turn */
	bool body = page->kind == ZK_PIVOT || page->kind == ZK_HUB;
	int rows = body ? 1 : 0;
	bool flat = st->flat_cards;
	for (zui_node *c = body ? NULL : page->first; c; c = c->next) {
		if (!c->visible) continue;
		if (flat && c->kind == ZK_CARD && c->first) {
			for (zui_node *k = c->first; k; k = k->next) rows += k->visible;
		} else {
			rows++;
		}
	}
	zg_clip(0, zui_pg(page)->title_panel.h, app->width, app->height - zui_pg(page)->title_panel.h);
	struct zg_xf base;
	if (sv && sv->stick_end && !role) {
		float mx = scroll_max(app, sv);
		if (!sv->glide) sv->scroll = mx;
	}
	float alpha = 1;
	/* the large title, if the style has one, scrolls as the first row */
	zg_xf_identity(&base);
	zg_xf_translate(&base, 0, -scroll, 0);
	if (role) st->page_xf(app, t, dir, role == 1, 0, rows + 1, &base, &alpha);
	zg_set_xf(&base);
	zg_set_alpha(alpha);
	if (!zui_page_pivot(page) && !zui_pg(page)->footer) st->page_top(app, page, zui_pg(page)->title_panel.h, true);
	int row = 1;
	if (body) {
		zg_xf_identity(&base);
		zg_xf_translate(&base, 0, -scroll, 0);
		alpha = 1;
		if (role) st->page_xf(app, t, dir, role == 1, 1, rows + 1, &base, &alpha);
		if (alpha > 0.002f) {
			zg_set_alpha(alpha);
			zg_set_xf(&base);
			draw_press = 0;
			page->cls->draw(app, page, &base, now);
		}
	}
	for (zui_node *c = body ? NULL : page->first; c; c = c->next) {
		if (!c->visible || c->kind == ZK_FOOTER) continue;
		bool split = flat && c->kind == ZK_CARD && c->first;
		zui_node *k = split ? c->first : c;
		/* one row, or each row of a card in turn */
		for (; k; k = split ? k->next : NULL) {
			if (!k->visible) continue;
			int r = row++;
			if (k->kind != ZK_PIVOT && (k->y - scroll > app->height || k->y + k->h - scroll < 0)) continue;
			zg_xf_identity(&base);
			zg_xf_translate(&base, 0, -scroll, 0);
			alpha = 1;
			if (role) st->page_xf(app, t, dir, role == 1, r, rows + 1, &base, &alpha);
			if (alpha <= 0.002f) continue;
			zg_set_alpha(alpha);
			if (split && k == c->first) {
				zg_set_xf(&base);
				st->card(app, c);
			}
			draw_node(app, k, &base, now);
		}
	}
	zg_noclip();
	zg_set_dot_origin(0);
	zg_xf_identity(&base);
	alpha = 1;
	if (role) st->page_xf(app, t, dir, role == 1, -1, rows + 1, &base, &alpha);
	zg_set_xf(&base);
	zg_set_alpha(alpha);
	if (zui_pg(page)->footer && zui_pg(page)->footer->visible) draw_node(app, zui_pg(page)->footer, &base, now);
	zg_set_xf(&base);
	if (!zui_pg(page)->fullscreen) st->header(app, page, now);
	if (zui_page_pivot(page) && !zui_pg(page)->fullscreen) {
		float off = zanim_value(&zui_pivot_of(zui_page_pivot(page))->pos, now, NULL);
		st->tabs(app, zui_page_pivot(page), (float)zui_pivot_of(zui_page_pivot(page))->cur + off);
	}
	/* the TitlePanel's titles, as widgets like the rest, over the header: a hub's go by slowly */
	if (zui_pg(page)->title_node && !zui_pg(page)->fullscreen && !zui_pg(page)->compact) {
		struct zg_xf tx = base;
		for (zui_node *c = zui_pg(page)->title_node->first; c; c = c->next)
			if (c->visible) draw_node(app, c, &tx, now);
	}
	zg_set_xf(NULL);
	zg_set_alpha(1);
}

void zui_draw(zui_app *app, uint32_t now) {
	zui_frame_begin();
	zg_set_dots(app->style == &zui_style_dotmatrix ? 2.3f : 0, app->dot_main);
	const struct zui_style *st = app->style;
	zui_node *page = app->page;
	if (!page) return;
	if (!app->laid_out) {
		for (int i = 0; i < app->n_pages; i++) {
			zui_node *p = app->pages[i];
			if (p == page || p == app->trans_from || p == app->trans_to) zui_do_layout(app, p);
		}
		app->laid_out = true;
	}
	ensure_lists(page, now);
	if (app->trans_from) ensure_lists(app->trans_from, now);
	if (!app->laid_out) {
		zui_do_layout(app, page);
		app->laid_out = true;
	}
	if (app->root->bg) {
		/* the app's, behind every page */
		struct zg_xf still;
		zg_xf_identity(&still);
		zg_set_xf(&still);
		zg_set_alpha(1);
		zg_set_dot_origin(0);
		zui_brush_paint(app, app->root->bg, 0, 0, app->width, app->height, 0);
		zg_set_xf(NULL);
	}
	if (!app->open_at) app->open_at = getenv("ZUI_NO_OPEN") ? now - 100000 : (now ? now : 1);
	float opened = (float)(int32_t)(now - app->open_at);
	if (!app->trans_to && st->turn_in_ms > 0 && opened < st->transition_ms - st->turn_in_ms) {
		/* the app opens as the second half of a page turn, forward */
		draw_page(app, page, now, 1, (opened + st->turn_in_ms) / st->transition_ms, 1);
		app->dirty = true;
	} else if (app->trans_to) {
		float t = (float)(int32_t)(now - app->trans_start) / st->transition_ms;
		t = t < 0 ? 0 : t > 1 ? 1 : t;
		/* the page on top is drawn last: going forward the new one, going back the old one */
		if (app->trans_dir > 0) {
			draw_page(app, app->trans_from, now, 2, t, 1);
			draw_page(app, app->trans_to, now, 1, t, 1);
		} else {
			draw_page(app, app->trans_to, now, 1, t, -1);
			draw_page(app, app->trans_from, now, 2, t, -1);
		}
	} else {
		draw_page(app, page, now, 0, 0, 0);
	}
	/* what the app keeps in the frame, over the pages, under the menus and dialogs */
	{
		struct zg_xf id;
		zg_xf_identity(&id);
		zg_set_xf(&id);
		zg_set_alpha(1);
		for (zui_node *c = app->root->first; c; c = c->next)
			if (!zui_is_page(c) && c->visible) draw_node(app, c, &id, now);
		zg_set_xf(NULL);
	}
	zui_widgets_draw_over(app, now);
	if (app->busy) st->busy(app, now);
	if (app->toast) {
		float age = (float)(int32_t)(now - app->toast_at);
		if (age < 2600) {
			float shown = age < 200 ? age / 200 : age > 2300 ? (2600 - age) / 300 : 1;
			st->toast(app, app->toast, zui_ease_out(shown));
		}
	}
}

ZUI_EXPORT void zui_post(zui_app *app, zui_fn fn, void *data) {
	struct zui_post *p = malloc(sizeof(*p));
	p->fn = fn;
	p->data = data;
	p->next = NULL;
	pthread_mutex_lock(app->post_lock);
	struct zui_post **tail = &app->posts;
	while (*tail) tail = &(*tail)->next;
	*tail = p;
	pthread_mutex_unlock(app->post_lock);
	uint64_t one = 1;
	if (write(app->post_fd, &one, sizeof(one)) < 0) {}
}

/* run what other threads posted (from the main loop) */
void zui_run_posts(zui_app *app);
void zui_run_posts(zui_app *app) {
	uint64_t count;
	if (read(app->post_fd, &count, sizeof(count)) < 0) {}
	pthread_mutex_lock(app->post_lock);
	struct zui_post *p = app->posts;
	app->posts = NULL;
	pthread_mutex_unlock(app->post_lock);
	while (p) {
		struct zui_post *next = p->next;
		p->fn(p->data);
		free(p);
		p = next;
	}
	app->dirty = true;
}

ZUI_EXPORT int zui_render_png(zui_app *app, zui_node *page, const char *path, int width, int height, int scale) {
	page = zui_page_of(page);
	return zui_offscreen_png(app, page, path, width, height, scale);
}

ZUI_EXPORT int zui_run(zui_app *app, zui_node *first) {
	first = zui_page_of(first);
	if (first && !app->page) {
		zui_pg(first)->depth = 0;
		zui_pg(first)->back_to = NULL;
		app->page = first;
	}
	const char *png = getenv("ZUI_RENDER_PNG");
	if (png && *png) {
		/* draw the first page into a PNG and stop: for evidence and tests */
		const char *size = getenv("ZUI_RENDER_SIZE");
		int w = 360, h = 740, s = 2;
		if (size) sscanf(size, "%dx%d@%d", &w, &h, &s);
		/* ZUI_RENDER_PAGE: another page than the first, by its number in the order the
		 * app made its pages (0 is the first made); pages have no names */
		zui_node *page = app->page;
		const char *which = getenv("ZUI_RENDER_PAGE");
		if (which && *which) {
			char *end;
			long k = strtol(which, &end, 10);
			zui_node *found = NULL;
			if (!*end && k >= 0 && k < app->n_pages) found = app->pages[k];
			if (!found) {
				fprintf(stderr, "zrnui: no page \"%s\" (the app has %d pages, numbered from 0)\n", which, app->n_pages);
				return 2;
			}
			if (found != page) {
				zui_pg(found)->back_to = page;
				zui_pg(found)->depth = 1;	/* drawn as if opened from the first page */
				page = found;
			}
		}
		return zui_offscreen_png(app, page, png, w, h, s) == 0 ? 0 : 1;
	}
	return zui_wl_run(app);
}

/* ---- the classes of the core's own kinds: what each is in layout and drawing ---- */
#define CORE_CLASS(var, nm, st, ...) static const struct zui_class var = { .name = nm, .size = sizeof(st), __VA_ARGS__ }
CORE_CLASS(k_column, "column", struct zui_node, .layout = lay_column, .draw = drw_column);
CORE_CLASS(k_row, "row", struct zui_node, .layout = lay_row, .draw = drw_column);
CORE_CLASS(k_content, "content panel", struct zui_node, .layout = lay_content_panel, .draw = drw_column);
CORE_CLASS(k_card, "card", struct zui_node, .layout = lay_card, .draw = drw_card);
CORE_CLASS(k_footer, "footer", struct zui_node, .layout = lay_footer, .draw = drw_footer);
CORE_CLASS(k_bubble, "bubble", struct zui_node, .layout = lay_bubble, .draw = drw_bubble, .interactive = int_with_handler);
CORE_CLASS(k_label, "section label", struct zui_node, .layout = lay_section_label, .draw = drw_section_label);
CORE_CLASS(k_space, "space", struct zui_node, .layout = lay_space);
CORE_CLASS(k_text, "text", struct zui_node, .layout = lay_text, .draw = drw_text);
CORE_CLASS(k_button, "button", struct zui_node, .layout = lay_button, .draw = drw_button, .interactive = int_always);
CORE_CLASS(k_item, "item", struct zui_node, .layout = lay_item, .draw = drw_item, .interactive = int_with_handler);
CORE_CLASS(k_switch, "switch", struct zui_node, .layout = lay_switch, .draw = drw_item, .interactive = int_always,
	.activate = act_switch);
CORE_CLASS(k_slider, "slider", struct zui_node, .layout = lay_slider, .draw = drw_slider, .interactive = int_always);
CORE_CLASS(k_progress, "progress", struct zui_node, .layout = lay_progress, .draw = drw_progress);
CORE_CLASS(k_image, "image", struct zui_node, .layout = lay_image, .draw = drw_image, .interactive = int_with_handler);
CORE_CLASS(k_scroll, "scroll", struct zui_scroll, .scrolls = true, .layout = lay_scroll, .draw = drw_scroll);
CORE_CLASS(k_pivot_item, "pivot item", struct zui_scroll, .scrolls = true, .layout = lay_column, .draw = drw_column);
CORE_CLASS(k_pivot, "pivot", struct zui_pivot, .is_page = true, .layout = lay_pivot, .draw = drw_pivot);
CORE_CLASS(k_list_ops, "list", struct zui_list, .layout = lay_list, .draw = drw_list, .destroy = zui_list_destroy);
CORE_CLASS(k_field_ops, "field", struct zui_field, .layout = lay_field, .draw = drw_field, .destroy = suggest_free,
	.interactive = int_always, .activate = act_field);

/* the class of a kind the core lays out and draws itself; NULL for a kind with another home */
const struct zui_class *zui_core_class(int kind) {
	switch (kind) {
	case ZK_COLUMN: return &k_column;
	case ZK_ROW: return &k_row;
	case ZK_CONTENT_PANEL: return &k_content;
	case ZK_CARD: return &k_card;
	case ZK_FOOTER: return &k_footer;
	case ZK_BUBBLE: return &k_bubble;
	case ZK_SECTION_LABEL: return &k_label;
	case ZK_SPACE: return &k_space;
	case ZK_TEXT: case ZK_HEADING: case ZK_CAPTION: case ZK_TITLE: case ZK_HEADER: return &k_text;
	case ZK_BUTTON: return &k_button;
	case ZK_ITEM: return &k_item;
	case ZK_SWITCH: return &k_switch;
	case ZK_SLIDER: return &k_slider;
	case ZK_PROGRESS: return &k_progress;
	case ZK_IMAGE: return &k_image;
	case ZK_SCROLL: return &k_scroll;
	case ZK_PIVOT_ITEM: return &k_pivot_item;
	case ZK_PIVOT: return &k_pivot;
	case ZK_LIST: return &k_list_ops;
	case ZK_FIELD: return &k_field_ops;
	default: return NULL;
	}
}
