// SPDX-License-Identifier: LGPL-2.1-or-later
/* Check boxes and radio buttons.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

static zui_node *check_new(zui_node *parent, int kind, const char *title, bool on, zui_cb cb, void *data) {
	zui_node *n = zui_node_new_kind(parent, kind);
	zui_set_text(n, title);
	n->on = on;
	zanim_set(&n->knob, on ? 1 : 0);
	n->cb = cb;
	n->data = data;
	return n;
}

ZUI_EXPORT zui_node *zui_check(zui_node *parent, const char *title, bool on, zui_cb cb, void *data) {
	return check_new(parent, ZK_CHECK, title, on, cb, data);
}

ZUI_EXPORT zui_node *zui_radio(zui_node *parent, const char *title, bool on, zui_cb cb, void *data) {
	return check_new(parent, ZK_RADIO, title, on, cb, data);
}

void zui_set_checked(zui_node *n, bool on) {
	if (n->on == on) return;
	n->on = on;
	zanim_go(&n->knob, on ? 1 : 0, 120, zui_now());
	n->app->dirty = true;
}

void draw_check(zui_app *app, zui_node *n, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	float k = zanim_value(&n->knob, now, NULL);
	float x = n->x, h = n->h, y = n->y + (h - 26) / 2;
	uint32_t ink = n->enabled ? pal.text : pal.text2;
	bool radio = n->kind == ZK_RADIO;
	if (radio) {
		/* WP's radio: a ring, a disc in it when chosen */
		zg_ring(x + 13, y + 13, 13, 2, ink);
		if (k > 0.01f) zg_circle(x + 13, y + 13, 6.5f * k, ink);
	} else {
		zg_border(x, y, 26, 26, 0, 2, ink);
		if (k > 0.01f) {
			zg_rect(x + 4, y + 4, 18, 18, 0, zc_alpha(ink, k));
			zui_icon("check", x + 1, y + 1, 24, 2.2f, zc_alpha(pal.bg, k));
		}
	}
	text_at(&n->t[0], n->text, ZT_LIGHT, 20, x + 26 + 14, n->y + (h - zt_line_height(20)) / 2, ink,
		n->w - 26 - 14);
}

/* radios that share a parent are one group */
void radio_chosen(zui_node *n) {
	for (zui_node *s = n->parent ? n->parent->first : NULL; s; s = s->next)
		if (s != n && s->kind == ZK_RADIO && s->on) {
			zui_set_checked(s, false);
			zui_fire(s, ZUI_EVENT_CHANGE);
		}
}

static float check_layout(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	(void)x; (void)y; (void)w;
	return M.item_h;
}
static void check_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	(void)xf;
	draw_check(app, n, now);
}
static bool check_interactive(const zui_node *n) { (void)n; return true; }
static void check_activate(zui_app *app, zui_node *n) {
	(void)app;
	zui_set_checked(n, !n->on);
	zui_haptic("click");
	zui_fire(n, ZUI_EVENT_CHANGE);
}
static void radio_activate(zui_app *app, zui_node *n) {
	(void)app;
	if (n->on) return;
	zui_set_checked(n, true);
	radio_chosen(n);
	zui_haptic("click");
	zui_fire(n, ZUI_EVENT_CHANGE);
}
const struct zui_class zui_check_class = { .name = "check", .size = sizeof(struct zui_node), .layout = check_layout,
	.draw = check_draw, .interactive = check_interactive, .activate = check_activate };
const struct zui_class zui_radio_class = { .name = "radio", .size = sizeof(struct zui_node), .layout = check_layout,
	.draw = check_draw, .interactive = check_interactive, .activate = radio_activate };
