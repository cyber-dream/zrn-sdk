// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * What a messaging app needs beyond rows and buttons: pictures, long lists of
 * rows of differing heights, tabs that swipe, a bar fixed at the bottom, and
 * speech bubbles. Their look is in the styles; only behaviour is here.
 *
 * A list keeps no texture per row. Rows are ordinary nodes, made when they come
 * near the screen and kept in a small pool; glyphs and shapes are quads anyway,
 * so a row that moves costs nothing but its vertices. Inserting at the start
 * renumbers the pool and moves the scroll: nothing is made again.
 */
#define _GNU_SOURCE
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include "../third_party/stb_image.h"
#pragma GCC diagnostic pop

#include <pthread.h>
#include <stdio.h>
#include "zui-internal.h"

/* ---- pictures ---- */
static zui_pixmap *pix_new(int w, int h, uint8_t *owned) {
	zui_pixmap *p = calloc(1, sizeof(*p));
	p->w = w;
	p->h = h;
	p->rgba = owned;
	p->refs = 1;
	return p;
}

/* box filter down to max_side: thumbnails stay small in memory and in the GPU */
static uint8_t *shrink(const uint8_t *src, int w, int h, int *ow, int *oh, int max_side) {
	int big = w > h ? w : h;
	if (max_side <= 0 || big <= max_side) return NULL;
	int f = (big + max_side - 1) / max_side;
	int nw = w / f > 0 ? w / f : 1, nh = h / f > 0 ? h / f : 1;
	uint8_t *d = malloc((size_t)nw * nh * 4);
	for (int y = 0; y < nh; y++)
		for (int x = 0; x < nw; x++) {
			unsigned s[4] = {0};
			for (int j = 0; j < f && y * f + j < h; j++) {
				const uint8_t *row = src + ((size_t)(y * f + j) * w + (size_t)x * f) * 4;
				for (int i = 0; i < f && x * f + i < w; i++)
					for (int c = 0; c < 4; c++) s[c] += row[i * 4 + c];
			}
			for (int c = 0; c < 4; c++) d[((size_t)y * nw + x) * 4 + c] = (uint8_t)(s[c] / (unsigned)(f * f));
		}
	*ow = nw;
	*oh = nh;
	return d;
}

ZUI_EXPORT zui_pixmap *zui_pixmap_decode(const void *bytes, size_t size, int max_side) {
	int w, h, n;
	uint8_t *px = stbi_load_from_memory(bytes, (int)size, &w, &h, &n, 4);
	if (!px) return NULL;
	int nw, nh;
	uint8_t *small = shrink(px, w, h, &nw, &nh, max_side);
	if (small) {
		stbi_image_free(px);
		return pix_new(nw, nh, small);
	}
	uint8_t *copy = malloc((size_t)w * h * 4);
	memcpy(copy, px, (size_t)w * h * 4);
	stbi_image_free(px);
	return pix_new(w, h, copy);
}

ZUI_EXPORT zui_pixmap *zui_pixmap_load(const char *path, int max_side) {
	FILE *f = fopen(path, "rb");
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	uint8_t *b = size > 0 ? malloc((size_t)size) : NULL;
	zui_pixmap *p = NULL;
	if (b && fread(b, 1, (size_t)size, f) == (size_t)size) p = zui_pixmap_decode(b, (size_t)size, max_side);
	free(b);
	fclose(f);
	return p;
}

ZUI_EXPORT zui_pixmap *zui_pixmap_rgba(int w, int h, const void *rgba) {
	uint8_t *copy = malloc((size_t)w * h * 4);
	memcpy(copy, rgba, (size_t)w * h * 4);
	return pix_new(w, h, copy);
}

ZUI_EXPORT void zui_pixmap_free(zui_pixmap *p) {
	if (!p || --p->refs > 0) return;
	if (p->tex && p->gen == zg_main_generation()) zg_tex_free(p->tex);
	free(p->rgba);
	free(p);
}

ZUI_EXPORT void zui_pixmap_size(const zui_pixmap *p, int *w, int *h) {
	if (w) *w = p->w;
	if (h) *h = p->h;
}

/* A frame uploads at most this many pixels: pictures arrive over a few frames
 * instead of one frame arriving late. */
#define UPLOAD_BUDGET (1024 * 1024)
static long frame_uploaded;
static __thread bool on_row_thread;
void zui_frame_begin(void) { frame_uploaded = 0; }

bool zui_pixmap_ready(zui_app *app, zui_pixmap *p, bool nearest) {
	if (p->tex && p->gen == zg_main_generation()) return true;
	long cost = (long)p->w * p->h;
	if (!on_row_thread && frame_uploaded > 0 && frame_uploaded + cost > UPLOAD_BUDGET) {
		app->dirty = true;	/* next frame */
		return false;
	}
	frame_uploaded += cost;
	p->tex = zg_tex_new(p->w, p->h, p->rgba, nearest);
	p->gen = zg_main_generation();
	return true;
}

ZUI_EXPORT zui_node *zui_image(zui_node *parent, zui_pixmap *pix, double w, double h) {
	zui_node *n = zui_node_new_kind(parent, ZK_IMAGE);
	n->space = w;
	n->value = h;
	zui_set_image(n, pix);
	return n;
}

ZUI_EXPORT void zui_set_image(zui_node *n, zui_pixmap *pix) {
	if (pix) pix->refs++;
	zui_pixmap_free(n->pix);
	n->pix = pix;
	n->app->laid_out = false;
	n->app->dirty = true;
}

ZUI_EXPORT void zui_set_image_sharp(zui_node *n, bool sharp) { n->sharp = sharp; n->app->dirty = true; }
ZUI_EXPORT void zui_set_radius(zui_node *n, double r) { n->radius = (float)r; n->app->dirty = true; }
ZUI_EXPORT void zui_set_align(zui_node *n, int a) { n->align = a; n->app->dirty = true; }
ZUI_EXPORT void zui_set_badge(zui_node *n, const char *t) {
	free(n->badge);
	n->badge = t && *t ? strdup(t) : NULL;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_monogram(zui_node *n, const char *t) {
	free(n->monogram);
	n->monogram = t && *t ? strdup(t) : NULL;
	n->app->laid_out = false;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_image_fit(zui_node *n, bool fit) {
	n->fit = fit;
	n->app->laid_out = false;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_bar_dots(zui_node *n, bool dots) {
	n->no_dots = !dots;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_quiet(zui_node *n, bool quiet) {
	n->quiet = quiet;
	n->app->laid_out = false;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_gap(zui_node *n, double gap) {
	n->has_gap = true;
	n->row_gap = (float)gap;
	n->app->laid_out = false;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_applist_row(zui_node *n, bool on) {
	n->applist = on;
	n->app->laid_out = false;
	n->app->dirty = true;
}
ZUI_EXPORT void zui_set_group_letter(zui_node *n, const char *t) {
	n->group = t && *t;
	zui_set_monogram(n, t);
}
ZUI_EXPORT void zui_set_emphasis(zui_node *n, bool on) { n->emphasis = on; n->app->dirty = true; }

/* a picture filling (x,y,w,h), cropped to keep its shape */
void zui_draw_pixmap(zui_app *app, zui_pixmap *p, float x, float y, float w, float h, float r, bool sharp, uint32_t back) {
	if (!p || !zui_pixmap_ready(app, p, sharp)) {
		zg_rect(x, y, w, h, r, back);
		return;
	}
	float sa = w / h, ia = (float)p->w / p->h, cx = 1, cy = 1;
	if (ia > sa) cx = sa / ia;
	else cy = ia / sa;
	zg_image(p->tex, x, y, w, h, r, cx, cy);
}

/* an avatar: the person's picture, or their initial on the accent */
void zui_draw_leading(zui_app *app, zui_node *n, float x, float y, float size, float r, uint32_t fg) {
	if (n->pix) {
		zui_draw_pixmap(app, n->pix, x, y, size, size, r, false, zc_rgb(app->accent));
		return;
	}
	zg_rect(x, y, size, size, r, n->tint ? zc_rgb(n->tint & 0xffffff) : zc_rgb(app->accent));
	if (n->monogram) {
		struct zt_layout *L = zt_slot_get(&n->t[2], n->monogram, ZT_MEDIUM, size * 0.42f, 0, 0);
		zt_draw(L, x + (size - L->w) / 2, y + (size - L->line_h) / 2, fg);
	}
}

/* ---- long lists ---- */
static void rebuild_prefix(struct zui_list *L) {
	free(L->y);
	L->y = malloc(sizeof(float) * (size_t)(L->count + 1));
	float at = 0;
	for (int i = 0; i < L->count; i++) {
		L->y[i] = at;
		at += L->height ? (float)L->height(i, L->data) : L->row_h;
	}
	L->y[L->count] = at;
}

ZUI_EXPORT zui_node *zui_list(zui_node *parent, int count, zui_list_height height, zui_list_bind bind, void *data) {
	zui_node *n = zui_node_new_kind(parent, ZK_LIST);
	struct zui_list *L = zui_list_get(n);
	L->count = count < 0 ? 0 : count;
	L->height = height;
	L->bind = bind;
	L->data = data;
	L->row_h = 64;
	rebuild_prefix(L);
	return n;
}

ZUI_EXPORT void zui_list_set_row_height(zui_node *n, double h) { zui_list_get(n)->row_h = (float)h; rebuild_prefix(zui_list_get(n)); n->app->laid_out = false; }

static void slot_drop(zui_node *list, int i) {
	zui_lock();
	struct zui_list *L = zui_list_get(list);
	if (L->slot[i]) zui_destroy(L->slot[i]);
	L->slot[i] = NULL;
	L->slot_row[i] = -1;
	zui_unlock();
}

static void list_destroy(zui_node *n) {
	struct zui_list *L = zui_list_get(n);
	free(L->y);
	L->y = NULL;
}
const struct zui_class zui_list_class = { .name = "list", .size = sizeof(struct zui_list), .destroy = list_destroy };

/* what scrolls the list: the tab it is in, the hub section it is in, or its page (see zui_scroller) */
zui_node *zui_list_owner(zui_node *n) {
	for (zui_node *p = n->parent; p; p = p->parent) {
		if (p->kind == ZK_SCROLL) return p;
		if (p->parent && (p->parent->kind == ZK_PIVOT || p->parent->kind == ZK_HUB)) return p;
	}
	return zui_page_of(n);
}

static void changed_height(zui_node *n, int row, float delta) {
	struct zui_list *L = zui_list_get(n);
	if (fabsf(delta) < 0.01f) return;
	for (int i = row + 1; i <= L->count; i++) L->y[i] += delta;
	struct zui_scroll *owner = zui_sv(zui_list_owner(n));
	zui_node *page = zui_page_of(n);
	float scroll = owner ? owner->scroll : 0;
	n->h += delta;
	for (int i = 0; i < ZUI_LIST_SLOTS; i++)
		if (L->slot[i] && L->slot_row[i] > row) L->slot[i]->y += delta, zui_move_children(L->slot[i], delta);
	if (owner) owner->content_h += delta;
	/* what is on the screen stays where it is, when a row above it turned out
	 * another height than guessed */
	if (owner && n->y + L->y[row] < scroll + zui_pg(page)->title_panel.h && !owner->stick_end) {
		owner->scroll += delta;
		n->app->down_scroll += delta;	/* or the next move of the finger undoes it */
	}
}

static void bind_row(zui_node *n, int row) {
	struct zui_list *L = zui_list_get(n);
	int at = -1;
	float best = -1;
	for (int i = 0; i < ZUI_LIST_SLOTS; i++) {
		if (!L->slot[i]) { at = i; break; }
		if (L->slot_row[i] >= L->want0 && L->slot_row[i] <= L->want1) continue;
		float far = (float)(L->slot_row[i] < L->want0 ? L->want0 - L->slot_row[i] : L->slot_row[i] - L->want1);
		if (far > best) { best = far; at = i; }
	}
	if (at < 0) return;
	if (L->slot[at]) slot_drop(n, at);
	zui_app *app = n->app;
	bool was = app->laid_out, dirty = app->dirty;
	zui_node *s = zui_column(n);
	L->slot[at] = s;
	L->slot_row[at] = row;
	if (L->bind) L->bind(row, s, L->data);
	if (row >= L->arrive_from && L->arrive_from > 0) zanim_go(&s->arrive, 1, 260, zui_now()), s->arrive.from = 0;
	(void)zui_layout_at(s, n->x, n->y + L->y[row], n->w);
	s->h = s->h;
	if (on_row_thread) { app->laid_out = was; app->dirty = dirty; } else app->laid_out = was;
}

/* ---- rows painted on a thread, then only moved ----
 * As ZrnMetroCore does: the row about to arrive is made and painted away from the
 * frame, and a frame only moves textures. The thread has its own GL context sharing
 * textures, its own batcher, program, atlas and glyph cache; the tree and the fonts
 * are under one lock, which a frame takes only if a row it needs is not ready. */
#define PAINT_MAX_PT 1400	/* taller than this is drawn live */
static pthread_mutex_t big_lock;
static pthread_once_t big_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t q_wake = PTHREAD_COND_INITIALIZER;
static bool rows_on;
static struct { zui_node *list; int r0, r1, dir; bool set; } want;
static zui_app *rows_app;
static void big_init(void) {
	pthread_mutexattr_t a;
	pthread_mutexattr_init(&a);
	pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&big_lock, &a);
	pthread_mutexattr_destroy(&a);
}
void zui_lock(void) { pthread_once(&big_once, big_init); pthread_mutex_lock(&big_lock); }
void zui_unlock(void) { pthread_mutex_unlock(&big_lock); }
ZUI_EXPORT void zui_list_lock(zui_app *app, bool lock) { (void)app; if (lock) zui_lock(); else zui_unlock(); }
bool zui_rows_running(void) { return rows_on; }

static zui_node *slot_of(struct zui_list *L, int row) {
	for (int i = 0; i < ZUI_LIST_SLOTS; i++)
		if (L->slot[i] && L->slot_row[i] == row) return L->slot[i];
	return NULL;
}

static void paint_slot(zui_node *n, zui_node *s) {
	zui_app *app = n->app;
	float scale = app->paint_scale > 0 ? app->paint_scale : 1;
	int tw = (int)lroundf(app->width * scale), th = (int)ceilf((s->h + 2) * scale);
	if (s->h <= 0 || s->h > PAINT_MAX_PT || !zg_ready_here()) return;
	if (s->row_tex && (s->row_tw != tw || s->row_th != th)) { zg_tex_free(s->row_tex); s->row_tex = 0; }
	if (!s->row_tex) { s->row_tex = zg_row_tex(tw, th); s->row_tw = tw; s->row_th = th; }
	float ox = s->x, oy = s->y;
	zg_row_begin(s->row_tex, tw, th, scale);
	if (app->style == &zui_style_dotmatrix) { zg_set_dots(2.3f, app->dot_main); zg_set_dot_origin(-oy); }
	struct zg_xf xf;
	zg_xf_identity(&xf);
	zg_xf_translate(&xf, 0, -oy, 0);
	(void)ox;
	zg_set_alpha(1);
	for (zui_node *c = s->first; c; c = c->next) if (c->visible) zui_draw_node(app, c, &xf, zui_now());
	zg_set_xf(NULL);
	zg_row_end();
	s->row_painted = true;
}

static void *rows_main(void *arg) {
	void **a = arg;
	void *(*current)(void *) = (void *(*)(void *))a[0];
	on_row_thread = true;
	zg_use_row_batcher();
	zt_use_row_cache();
	if (!current(a[1]) || !zg_init()) return NULL;
	rows_on = true;
	for (;;) {
		pthread_mutex_lock(&q_lock);
		while (!want.set) pthread_cond_wait(&q_wake, &q_lock);
		zui_node *n = want.list;
		int r0 = want.r0, r1 = want.r1, dir = want.dir;
		want.set = false;
		pthread_mutex_unlock(&q_lock);
		/* in sight first, then ahead the way it is going */
		for (int k = 0; k < (r1 - r0 + 1) + 10; k++) {
			int row = k <= r1 - r0 ? r0 + k : dir >= 0 ? r1 + (k - (r1 - r0)) : r0 - (k - (r1 - r0));
			zui_lock();
			struct zui_list *L = zui_list_get(n);
			if (!L || want.set || row < 0 || row >= L->count) {
				zui_unlock();
				if (want.set) break;
				continue;
			}
			zui_node *s = slot_of(L, row);
			if (!s) {
				extern void zui_bind_row(zui_node *, int);
				zui_bind_row(n, row);
				s = slot_of(L, row);
			}
			if (s && !s->row_painted) paint_slot(n, s);
			zui_unlock();
		}
	}
	return NULL;
}

void zui_rows_start(zui_app *app, void *(*make_current)(void *), void *arg) {
	static void *a[2];
	static pthread_t t;
	a[0] = (void *)make_current;
	a[1] = arg;
	rows_app = app;
	pthread_create(&t, NULL, rows_main, a);
	pthread_detach(t);
}

void zui_bind_row(zui_node *n, int row) { bind_row(n, row); }

/* the frame's side: heights the thread found, rows it must not wait for, what next */
void zui_list_ensure(zui_node *n, uint32_t now) {
	struct zui_list *L = zui_list_get(n);
	if (!L->count) return;
	struct zui_scroll *owner = zui_sv(zui_list_owner(n));
	float scroll = owner ? owner->scroll : 0, H = n->app->height;
	float top = scroll - n->y, bot = top + H;
	int lo = 0, hi = L->count;
	while (lo < hi) {
		int m = (lo + hi) / 2;
		if (L->y[m + 1] <= top) lo = m + 1; else hi = m;
	}
	int first = lo, last = first;
	while (last + 1 < L->count && L->y[last + 1] < bot) last++;
	L->want0 = first;
	L->want1 = last;
	int dir = first > L->last_first ? 1 : first < L->last_first ? -1 : L->last_dir;
	L->last_dir = dir;
	L->last_first = first;
	bool behind = n->app->trans_to && (int32_t)(now - n->app->trans_start) < n->app->style->transition_ms / 3;
	zui_lock();
	for (int r = first; r <= last; r++) {
		zui_node *s = slot_of(L, r);
		if (!s && !(rows_on && behind)) {
			bind_row(n, r);
			s = slot_of(L, r);
		}
		if (s && !s->row_measured) {
			s->row_measured = true;
			float was = L->y[r + 1] - L->y[r];
			changed_height(n, r, s->h - was);
		}
	}
	/* rows the thread made ahead of those in sight */
	for (int i = 0; i < ZUI_LIST_SLOTS; i++) {
		zui_node *s = L->slot[i];
		if (!s || s->row_measured) continue;
		int r = L->slot_row[i];
		s->row_measured = true;
		changed_height(n, r, s->h - (L->y[r + 1] - L->y[r]));
	}
	zui_unlock();
	if (rows_on) {
		pthread_mutex_lock(&q_lock);
		want.list = n;
		want.r0 = first;
		want.r1 = last;
		want.dir = dir;
		want.set = true;
		pthread_cond_signal(&q_wake);
		pthread_mutex_unlock(&q_lock);
	}
	if (first < 45 && !L->asked && n->cb) {
		L->asked = true;
		zui_fire(n, ZUI_EVENT_CHANGE);
	}
}

/* Moved textures; a row not painted yet, or under the finger, drawn as it is */
void zui_list_draw(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now, int role, float t, int dir, float scroll) {
	struct zui_list *L = zui_list_get(n);
	float a0 = zg_get_alpha(), s = zui_gfx_scale();
	float rounded = roundf(scroll * s) / s;
	/* only the tab in front swivels: turned rows off to the side are pulled back
	 * into view by the perspective */
	zui_node *tabs = NULL;
	for (zui_node *p = n; p && p->parent; p = p->parent)
		if (p->parent->kind == ZK_PIVOT && zui_tab_at(p->parent, zui_pivot_of(p->parent)->cur) == p) tabs = p->parent;
	int k = 0;
	for (int row = L->want0; row <= L->want1 && row < L->count; row++) {
		zui_node *c = slot_of(L, row);
		if (!c || c->y - scroll > app->height || c->y + c->h - scroll < 0) continue;
		struct zg_xf rx = *base;
		float al = 1;
		if (role) {
			zg_xf_identity(&rx);
			zg_xf_translate(&rx, 0, -rounded, 0);
			app->style->page_xf(app, t, dir, role == 1, 1 + k, 12, &rx, &al);
			if (al <= 0.002f) { k++; continue; }
		}
		k++;
		/* a tab arriving: its rows swivel in, one after another */
		bool running = false;
		float arrive = c->arrive.ms ? zanim_value(&c->arrive, now, &running) : 1;
		if (arrive < 1) {
			zg_xf_translate(&rx, 0, (1 - arrive) * 36, 0);
			al *= arrive;
		}
		zg_set_alpha(a0 * al);
		if (c->row_painted && c->row_tex && !zui_has_pressed(app, c)) {
			zg_set_xf(&rx);
			zg_row(c->row_tex, 0, c->y, (float)c->row_tw / s, (float)c->row_th / s);
		} else {
			zui_lock();
			zui_draw_node(app, c, &rx, now);
			zui_unlock();
		}
	}
	zg_set_alpha(a0);
}

ZUI_EXPORT void zui_swivel(zui_node *n, bool in, int delay_ms) {
	n->swivel_out = !in;
	n->swivel_at = zui_now() + (uint32_t)(delay_ms < 0 ? 0 : delay_ms);
	zanim_go(&n->swivel, 0, 0, zui_now());
	zanim_go(&n->focus_anim, n->focused ? 1 : 0, (uint32_t)delay_ms + (in ? 300 : 200), zui_now());
	n->app->dirty = true;
}

float zui_list_layout(zui_node *n, float x, float y, float w) {
	struct zui_list *L = zui_list_get(n);
	for (int i = 0; i < ZUI_LIST_SLOTS; i++) {
		if (!L->slot[i]) continue;
		int r = L->slot_row[i];
		float h = zui_layout_at(L->slot[i], x, y + L->y[r], w);
		float was = L->y[r + 1] - L->y[r];
		if (fabsf(h - was) > 0.01f) {
			for (int k = r + 1; k <= L->count; k++) L->y[k] += h - was;
		}
	}
	/* (rows moved by that) */
	for (int i = 0; i < ZUI_LIST_SLOTS; i++)
		if (L->slot[i]) zui_layout_at(L->slot[i], x, y + L->y[L->slot_row[i]], w);
	return L->y[L->count];
}

ZUI_EXPORT void zui_list_set_count(zui_node *n, int count) {
	zui_lock();
	struct zui_list *L = zui_list_get(n);
	for (int i = 0; i < ZUI_LIST_SLOTS; i++) if (L->slot[i]) slot_drop(n, i);
	L->arrive_from = 0;
	L->count = count < 0 ? 0 : count;
	L->asked = false;
	rebuild_prefix(L);
	n->app->laid_out = false;
	n->app->dirty = true;
	zui_unlock();
}

/* k more rows at the end */
ZUI_EXPORT void zui_list_append(zui_node *n, int k) {
	zui_lock();
	struct zui_list *L = zui_list_get(n);
	int old = L->count;
	L->arrive_from = old;
	L->y = realloc(L->y, sizeof(float) * (size_t)(old + k + 1));
	L->count = old + k;
	for (int i = old; i < old + k; i++)
		L->y[i + 1] = L->y[i] + (L->height ? (float)L->height(i, L->data) : L->row_h);
	n->app->laid_out = false;
	n->app->dirty = true;
	zui_unlock();
}

/* k more rows at the start; nothing on the screen moves or is made again */
ZUI_EXPORT void zui_list_prepend(zui_node *n, int k) {
	zui_lock();
	struct zui_list *L = zui_list_get(n);
	int old = L->count;
	float *y = malloc(sizeof(float) * (size_t)(old + k + 1));
	float at = 0;
	for (int i = 0; i < k; i++) {
		y[i] = at;
		at += L->height ? (float)L->height(i, L->data) : L->row_h;
	}
	for (int i = 0; i <= old; i++) y[k + i] = at + L->y[i];
	free(L->y);
	L->y = y;
	L->count = old + k;
	L->asked = false;
	L->arrive_from = 0;
	for (int i = 0; i < ZUI_LIST_SLOTS; i++)
		if (L->slot[i]) L->slot_row[i] += k;
	if (zui_sv(zui_list_owner(n))) zui_sv(zui_list_owner(n))->scroll += at;
	n->app->down_scroll += at;
	n->app->laid_out = false;
	n->app->dirty = true;
	zui_unlock();
}

/* this row's content changed */
ZUI_EXPORT void zui_list_refresh(zui_node *n, int row) {
	struct zui_list *L = zui_list_get(n);
	for (int i = 0; i < ZUI_LIST_SLOTS; i++)
		if (L->slot[i] && L->slot_row[i] == row) slot_drop(n, i);
	n->app->dirty = true;
}

ZUI_EXPORT void zui_list_allow_more(zui_node *n) { zui_list_get(n)->asked = false; }

/* ---- the end of a page ---- */
ZUI_EXPORT void zui_page_scroll_end(zui_node *page, bool glide) {
	struct zui_scroll *sv = zui_scroller(zui_page_of(page));
	if (!sv) return;	/* a page does not scroll */
	sv->stick_end = true;
	sv->glide = glide;
	if (glide) sv->glide_at = zui_now();
	sv->base.app->dirty = true;
}
ZUI_EXPORT bool zui_page_at_end(zui_node *page, double within) {
	struct zui_scroll *sv = zui_scroller(zui_page_of(page));
	if (!sv) return true;	/* it does not scroll: it is all at its end */
	return sv->stick_end || sv->content_h - sv->base.app->height - sv->scroll <= within;
}

/* ---- tabs ---- */
ZUI_EXPORT zui_node *zui_pivot(zui_app *app) {
	zui_node *page = zui_page_blank(app);
	zui_node *n = zui_node_new_kind(page, ZK_PIVOT);
	zui_pg(page)->tabs = n;
	return n;
}
ZUI_EXPORT void zui_pivot_title(zui_node *pivot, const char *text) {
	zui_node *panel = zui_title_panel(pivot);
	for (zui_node *c = panel->first; c; c = c->next)
		if (c->kind == ZK_TITLE) { zui_set_text(c, text ? text : ""); return; }
	zui_title(panel, text ? text : "");
}
ZUI_EXPORT zui_node *zui_pivot_item(zui_node *pivot) { return zui_node_new_kind(pivot, ZK_PIVOT_ITEM); }
/* the item's title (PivotItem.Header): a header the pivot draws in its title row, the column skips it;
 * called again, it changes the text */
ZUI_EXPORT zui_node *zui_pivot_item_header(zui_node *item, const char *text) {
	for (zui_node *c = item->first; c; c = c->next)
		if (c->kind == ZK_HEADER) { zui_set_text(c, text ? text : ""); return c; }
	zui_node *n = zui_header(item, text ? text : "");
	n->visible = false;
	return n;
}
/* an item's title: its first zui_header (hidden from the column, the pivot draws it) */
const char *zui_tab_title(const zui_node *item) {
	for (zui_node *c = item->first; c; c = c->next)
		if (c->kind == ZK_HEADER) return c->text ? c->text : "";
	return "";
}
int zui_child_index(const zui_node *c) {
	int i = 0;
	for (const zui_node *p = c->prev; p; p = p->prev) i++;
	return i;
}
zui_node *zui_tab_at(zui_node *tabs, int i) {
	zui_node *c = tabs->first;
	while (c && i-- > 0) c = c->next;
	return c;
}
int zui_tab_count(const zui_node *tabs) {
	int k = 0;
	for (zui_node *c = tabs->first; c; c = c->next) k++;
	return k;
}
ZUI_EXPORT int zui_pivot_current(const zui_node *tabs) { return zui_pivot_of(tabs)->cur; }
/* one tab along, going round; the slide carries on from where it is */
void zui_tabs_step(zui_node *tabs, int dir) {
	int k = zui_tab_count(tabs), cur = zui_pivot_of(tabs)->cur;
	zui_app *app = tabs->app;
	uint32_t now = zui_now();
	float off = zanim_value(&zui_pivot_of(tabs)->pos, now, NULL);
	if (dir) {
		int i = ((cur + dir) % k + k) % k;
		zui_node *old = zui_tab_at(tabs, cur);
		zui_pivot_of(tabs)->cur = i;
		zui_pivot_of(tabs)->prev = cur;
		zui_sv(old)->vel = 0;
		app->fling = false;
		app->laid_out = false;
		off -= (float)dir;
		zui_fire(tabs, ZUI_EVENT_CHANGE);
	}
	zui_pivot_of(tabs)->pos.from = off;
	zui_pivot_of(tabs)->pos.to = 0;
	zui_pivot_of(tabs)->pos.start = now;
	zui_pivot_of(tabs)->pos.ms = 260;
	zanim_go(&tabs->focus_anim, 0, 260, now);
	app->dirty = true;
}

ZUI_EXPORT void zui_pivot_select(zui_node *tabs, int i) {
	int count = zui_tab_count(tabs);
	if (i < 0 || i >= count) i = zui_pivot_of(tabs)->cur;
	zui_app *app = tabs->app;
	uint32_t now = zui_now();
	/* the slide carries on from where the finger left it */
	float from = (float)zui_pivot_of(tabs)->cur + zanim_value(&zui_pivot_of(tabs)->pos, now, NULL);
	if (i != zui_pivot_of(tabs)->cur) {
		zui_node *old = zui_tab_at(tabs, zui_pivot_of(tabs)->cur);
		zui_pivot_of(tabs)->cur = i;
		zui_sv(old)->vel = 0;
		app->fling = false;
		zui_pivot_of(tabs)->prev = zui_child_index(old);
		app->laid_out = false;
		zui_fire(tabs, ZUI_EVENT_CHANGE);
	}
	zui_pivot_of(tabs)->pos.from = from - (float)i;
	zui_pivot_of(tabs)->pos.to = 0;
	zui_pivot_of(tabs)->pos.start = now;
	zui_pivot_of(tabs)->pos.ms = 240;
	zanim_go(&tabs->focus_anim, 0, 240, now);	/* (keeps frames coming) */
	app->dirty = true;
}
