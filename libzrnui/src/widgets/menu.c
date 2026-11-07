// SPDX-License-Identifier: LGPL-2.1-or-later
/* The menu over a page.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

ZUI_EXPORT void zui_menu(zui_app *app, zui_node *anchor, const char *const *items, int count,
		zui_choice_fn cb, void *data) {
	if (count <= 0) return;
	struct ov *o = ov_open(app, OV_MENU);
	ov_items(o, items, count);
	o->cb = cb;
	o->data = data;
	float h = count * 50 + 16;
	float y = anchor ? screen_y(anchor) + anchor->h : app->height - h;
	if (y + h > app->height - 8) y = app->height - 8 - h;	/* all of it on the screen */
	if (y < 0) y = 0;
	o->y = y;
}

#define MENU_ROW 50

int menu_row_at(zui_app *app, struct ov *o, float x, float y) {
	(void)x;
	float top = o->y + 8;
	if (y < top || y >= top + o->count * MENU_ROW) return -1;
	return (int)((y - top) / MENU_ROW);
}

void draw_menu(zui_app *app, struct ov *o, float a) {
	struct zui_palette pal = zui_pal(app);
	bool dots = dotted(app);
	float W = app->width, h = o->count * MENU_ROW + 16, y = o->y;
	uint32_t paper = dots ? pal.bg : pal.text, ink = dots ? pal.text : pal.bg;
	zg_rect(0, y, W, h, 0, zc_alpha(paper, a));
	if (dots) zg_border(0, y, W, h, 0, 1.2f, zc_alpha(pal.text, a));
	for (int i = 0; i < o->count; i++) {
		float top = y + 8 + i * MENU_ROW;
		bool down = o->pressed == i;
		if (down) zg_rect(0, top, W, MENU_ROW, 0, zc_alpha(zc_rgb(app->accent), a));
		text_now(o->item[i], ZT_LIGHT, 24, M.margin, top + 8, zc_alpha(down ? 0xffffffff : ink, a), W - 2 * M.margin);
	}
}
