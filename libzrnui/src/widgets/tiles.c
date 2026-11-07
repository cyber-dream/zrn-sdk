// SPDX-License-Identifier: LGPL-2.1-or-later
/* Tiles that turn over.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "widgets/widgets.h"

struct tile { struct zui_node base; bool wide; char *back; int timer, period; };
static inline struct tile *tile_of(const zui_node *n) {
	assert(n && n->kind == ZK_TILE);
	return (struct tile *)n;
}

#define TILE_GAP 6

ZUI_EXPORT zui_node *zui_tiles(zui_node *parent) { return zui_node_new_kind(parent, ZK_TILES); }

ZUI_EXPORT zui_node *zui_tile(zui_node *tiles, const char *title, const char *icon, bool wide, zui_cb cb, void *data) {
	zui_node *n = zui_node_new_kind(tiles, ZK_TILE);
	zui_set_text(n, title);
	zui_set_icon(n, icon);
	tile_of(n)->wide = wide;
	n->cb = cb;
	n->data = data;
	return n;
}

static void tile_flip(void *data) {
	zui_node *n = data;
	struct tile *t = tile_of(n);
	if (!t->back) return;
	n->on = !n->on;
	zanim_go(&n->knob, n->on ? 1 : 0, 700, zui_now());
	n->app->dirty = true;
}

ZUI_EXPORT void zui_tile_set_live(zui_node *n, const char *back, int period_ms) {
	struct tile *t = n->kind == ZK_TILE ? tile_of(n) : NULL;
	if (!t) return;
	if (t->timer) zui_timer_cancel(n->app, t->timer);
	t->timer = 0;
	free(t->back);
	t->back = back && *back ? strdup(back) : NULL;
	t->period = period_ms;
	if (n->on) {
		n->on = false;
		zanim_set(&n->knob, 0);
	}
	if (t->back && period_ms > 0) {
		/* staggered, so a screenful of tiles does not turn as one */
		static int stagger;
		t->timer = zui_timer(n->app, period_ms + (stagger++ % 5) * 700, true, tile_flip, n);
	}
	n->app->dirty = true;
}

float layout_tiles(zui_node *g, float x, float y, float w) {
	float side = (w - TILE_GAP) / 2, cx = 0, cy = 0;
	for (zui_node *c = g->first; c; c = c->next) {
		if (!c->visible) continue;
		struct tile *t = c->kind == ZK_TILE ? tile_of(c) : NULL;
		bool wide = t && t->wide;
		/* a wide one has a row to itself; a square goes where there is room */
		if (cx > 0 && (wide || cx + side > w + 0.5f)) { cy += side + TILE_GAP; cx = 0; }
		c->x = x + cx;
		c->y = y + cy;
		c->w = wide ? w : side;
		c->h = side;
		cx += c->w + TILE_GAP;
		if (cx > w - 1) { cy += side + TILE_GAP; cx = 0; }
	}
	return cx > 0 ? cy + side : cy > 0 ? cy - TILE_GAP : 0;
}

void draw_tile(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	struct tile *t = tile_of(n);
	float k = zanim_value(&n->knob, now, NULL);
	struct zg_xf xf = *base;
	bool back = k > 0.5f;
	if (t->back && k > 0.001f && k < 0.999f) {
		/* turning over about its middle: the other face once it is edge on */
		zg_xf_rotate_x(&xf, back ? (k - 1) * 180 : k * 180, n->y + n->h / 2);
		app->dirty = true;
	}
	zg_set_xf(&xf);
	float x = n->x, y = n->y, w = n->w, h = n->h;
	uint32_t base_c = n->tint ? zc_rgb(n->tint & 0xffffff) : zc_rgb(app->accent);
	bool dots = dotted(app);
	uint32_t face = back ? zc_mix(base_c, 0x000000ff, 0.35f) : base_c;
	uint32_t ink = dots ? (back ? pal.text2 : pal.text) : 0xffffffff;
	if (dots) zg_border(x, y, w, h, 0, 1.2f, back ? pal.text2 : pal.text);
	else zg_rect(x, y, w, h, 0, face);
	if (back) {
		struct zt_layout *L = zt_layout_make(t->back, ZT_REGULAR, 15, w - 20, 0);
		zt_draw(L, x + 10, y + 10, ink);
		zt_layout_free(L);
		return;
	}
	if (n->icon) zui_icon(n->icon, x + (w - 44) / 2, y + (h - 44) / 2 - 8, 44, 2.2f, ink);
	text_at(&n->t[0], n->text, ZT_REGULAR, 14.5f, x + 9, y + h - 9 - zt_line_height(14.5f), ink, w - 18 - (n->badge ? 30 : 0));
	if (n->badge) {
		struct zt_layout *B = zt_slot_get(&n->t[4], n->badge, ZT_LIGHT, 22, 0, 0);
		zt_draw(B, x + w - 9 - B->w, y + h - 6 - B->line_h, ink);
	}
}

bool has_flip(zui_node *n, uint32_t now) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		if (c->kind == ZK_TILE) {
			bool running = false;
			zanim_value(&c->knob, now, &running);
			if (running) return true;
		} else if (c->kind != ZK_LIST && has_flip(c, now)) return true;
	}
	return false;
}

void tile_free(zui_node *n) {
	struct tile *t = tile_of(n);
	if (t->timer) zui_timer_cancel(n->app, t->timer);
	free(t->back);
}

static float tile_layout(zui_node *n, float x, float y, float w) { (void)x; (void)y; (void)w; return n->h; }
static bool tile_interactive(const zui_node *n) { return n->cb != NULL || n->hold_cb != NULL; }
static void tile_activate(zui_app *app, zui_node *n) {
	(void)app;
	zui_haptic("tap");
	zui_fire(n, ZUI_EVENT_CLICK);
}
static void tile_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { draw_tile(app, n, xf, now); }
const struct zui_class zui_tile_class = { .name = "tile", .size = sizeof(struct tile), .destroy = tile_free,
	.layout = tile_layout, .draw = tile_draw, .interactive = tile_interactive, .activate = tile_activate };

static float tiles_layout(zui_node *n, float x, float y, float w) { return layout_tiles(n, x, y, w); }
static void tiles_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	for (zui_node *c = n->first; c; c = c->next) if (c->visible) zui_draw_node(app, c, xf, now);
}
const struct zui_class zui_tiles_class = { .name = "tiles", .size = sizeof(struct zui_node), .layout = tiles_layout,
	.draw = tiles_draw };
