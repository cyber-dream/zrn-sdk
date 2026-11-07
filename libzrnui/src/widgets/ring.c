// SPDX-License-Identifier: LGPL-2.1-or-later
/* The progress ring.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

ZUI_EXPORT zui_node *zui_ring(zui_node *parent) { return zui_node_new_kind(parent, ZK_RING); }

void draw_ring(zui_app *app, zui_node *n, uint32_t now) {
	uint32_t acc = zc_rgb(app->accent);
	float cx = n->x + n->w / 2, cy = n->y + n->h / 2, r = 13;
	for (int i = 0; i < 5; i++) {
		/* each dot starts a little after the one before, quick through the
		 * bottom and slow over the top as WP's ring is */
		float u = (float)(now % 2400) / 2400.0f - i * 0.055f;
		u -= floorf(u);
		float a = (float)M_PI * 2 * (u < 0.5f ? 2 * u * u : 1 - 2 * (1 - u) * (1 - u));
		float fade = u > 0.02f && u < 0.98f ? 1 : 0;
		if (fade) zg_circle(cx + r * sinf(a), cy - r * cosf(a), 2.4f, acc);
	}
}

bool has_ring(zui_node *n) {
	for (zui_node *c = n->first; c; c = c->next) {
		if (!c->visible) continue;
		if (c->kind == ZK_RING) return true;
		if (c->kind != ZK_LIST && has_ring(c)) return true;
	}
	return false;
}

static float ring_layout(zui_node *n, float x, float y, float w) { (void)n; (void)x; (void)y; (void)w; return 40; }
static void ring_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	(void)xf;
	draw_ring(app, n, now);
}
const struct zui_class zui_ring_class = { .name = "ring", .size = sizeof(struct zui_node), .layout = ring_layout,
	.draw = ring_draw };
