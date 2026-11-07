// SPDX-License-Identifier: LGPL-2.1-or-later
/* The list picker, in place (zrnmetrocore's zmc_picker).
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

struct picker { char **opt; int count, sel; };

ZUI_EXPORT zui_node *zui_picker(zui_node *parent, const char *caption, const char *const *options, int count,
		int selected, zui_cb cb, void *data) {
	zui_node *n = zui_node_new_kind(parent, ZK_PICKER);
	zui_set_text(n, caption);
	struct picker *p = calloc(1, sizeof(*p));
	p->count = count < 0 ? 0 : count;
	p->opt = calloc((size_t)(p->count ? p->count : 1), sizeof(char *));
	for (int i = 0; i < p->count; i++) p->opt[i] = dupstr(options[i]);
	p->sel = selected < 0 || selected >= p->count ? 0 : selected;
	n->ext = p;
	n->cb = cb;
	n->data = data;
	n->app->n_pickers++;
	return n;
}

ZUI_EXPORT int zui_picker_selected(const zui_node *n) {
	return n->kind == ZK_PICKER && n->ext ? ((struct picker *)n->ext)->sel : -1;
}

ZUI_EXPORT void zui_picker_select(zui_node *n, int index) {
	struct picker *p = n->kind == ZK_PICKER ? n->ext : NULL;
	if (!p || index < 0 || index >= p->count) return;
	p->sel = index;
	n->app->dirty = true;
}

static float picker_open(const zui_node *n, uint32_t now) { return clamp01(zanim_value(&n->knob, now, NULL)); }

float layout_picker(zui_node *n) {
	struct picker *p = n->ext;
	/* (a file render has no motion: the end state) */
	float o = n->app->wl ? picker_open(n, zui_now()) : (n->on ? 1 : 0);
	return 24 + PICK_ROW + ((p->count - 1) * PICK_ROW + 8) * o + 18;
}

static void picker_set_open(zui_node *n, bool open) {
	if (n->on == open) return;
	n->on = open;
	zanim_go(&n->knob, open ? 1 : 0, open ? PICK_OPEN_MS : PICK_CLOSE_MS, zui_now());
	n->app->laid_out = false;
	n->app->dirty = true;
	if (open) n->app->open_picker = n;
	else if (n->app->open_picker == n) n->app->open_picker = NULL;
}

void draw_picker(zui_app *app, zui_node *n, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	struct picker *p = n->ext;
	float o = app->wl ? picker_open(n, now) : (n->on ? 1 : 0);
	float x = n->x, w = n->w, y = n->y + 24, h = PICK_ROW + ((p->count - 1) * PICK_ROW + 8) * o;
	bool dots = dotted(app);
	text_at(&n->t[0], n->text, ZT_REGULAR, 15, n->x, n->y, pal.text2, w);
	/* the box: an outline closed, the edit background open */
	uint32_t paper = dots ? pal.bg : 0xffffffff;
	if (!dots) zg_rect(x, y, w, h, 0, zc_alpha(paper, o));
	zg_border(x, y, w, h, 0, 2, pal.text);
	/* (no clip: an option never leaves the box, from closed to open) */
	for (int i = 0; i < p->count; i++) {
		/* each option slides from where the chosen one sits to its own place */
		float top = y + 4 * o + i * PICK_ROW * o;
		uint32_t ink;
		if (i == p->sel) ink = zc_mix(pal.text, pal.accent_text, o);
		else {
			if (o <= 0.001f) continue;
			ink = zc_alpha(dots ? pal.text : 0x000000ff, o);
		}
		if (i == p->sel && !dots) ink = zc_mix(0xffffffff, zc_rgb(app->accent), o);
		text_now(p->opt[i], ZT_REGULAR, 17, x + 10, top + 9, ink, w - 20);
	}
}

void picker_tapped(zui_app *app, zui_node *n) {
	struct picker *p = n->ext;
	if (!n->on) {
		picker_set_open(n, true);
		return;
	}
	float py = app->down_y + (in_footer(n) || !app->page ? 0 : app->page->scroll);
	int i = (int)floorf((py - (n->y + 24 + 4)) / PICK_ROW);
	if (i >= 0 && i < p->count) {
		bool changed = i != p->sel;
		p->sel = i;
		picker_set_open(n, false);
		zui_haptic("tick");
		if (changed) zui_fire(n, ZUI_EVENT_CHANGE);
	} else {
		picker_set_open(n, false);
	}
}

/* a tap anywhere but on the open picker shuts it */
void zui_pickers_close_except(zui_app *app, zui_node *keep) {
	zui_node *o = app->open_picker;
	if (o && o != keep) picker_set_open(o, false);
}

bool pickers_moving(zui_node *n, uint32_t now) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (n->kind == ZK_LIST) break;
		if (c->kind == ZK_PICKER) {
			bool running = false;
			zanim_value(&c->knob, now, &running);
			if (running) return true;
		}
		if (pickers_moving(c, now)) return true;
	}
	return false;
}

void picker_free(zui_node *n) {
	zui_app *app = n->app;
	struct picker *p = n->ext;
	if (p) {
		for (int i = 0; i < p->count; i++) free(p->opt[i]);
		free(p->opt);
		free(p);
	}
	if (app->n_pickers > 0) app->n_pickers--;
}
