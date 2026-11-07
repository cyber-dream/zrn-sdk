// SPDX-License-Identifier: LGPL-2.1-or-later
/* What every widget uses: the palette helpers, text at a place, where a node is on the screen.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

float clamp01(float v) { return v < 0 ? 0 : v > 1 ? 1 : v; }

char *dupstr(const char *s) { return strdup(s ? s : ""); }

void text_at(struct zt_slot *slot, const char *text, int font, float size, float x, float y,
		uint32_t color, float max_w) {
	struct zt_layout *L = zt_slot_get(slot, text, font, size, max_w, max_w > 0 ? 1 : 0);
	zt_draw(L, x, y, color);
}

/* one line of text drawn now, not cached (overlays: few and short) */
float text_now(const char *text, int font, float size, float x, float y, uint32_t color, float max_w) {
	struct zt_layout *L = zt_layout_make(text, font, size, max_w, max_w > 0 ? 1 : 0);
	zt_draw(L, x, y, color);
	float w = L->w;
	zt_layout_free(L);
	return w;
}

bool in_footer(const zui_node *n) {
	for (; n; n = n->parent) {
		if (n->kind == ZK_FOOTER) return true;
		/* what the app keeps in the frame is over the pages and does not scroll */
		if (n->parent && n->parent->kind == ZK_FRAME && !zui_is_page(n)) return true;
	}
	return false;
}

/* a node's top in window points, whatever scrolls */
float screen_y(const zui_node *n) {
	zui_app *app = n->app;
	(void)app;	/* a page does not scroll: what is under a scroll container is moved by it */
	return n->y;
}
