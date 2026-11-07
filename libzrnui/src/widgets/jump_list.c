// SPDX-License-Identifier: LGPL-2.1-or-later
/* The jump list (zrnmetrocore's letter board).
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

ZUI_EXPORT void zui_jump_list(zui_app *app, const char *const *labels, const bool *live, int count,
		zui_choice_fn cb, void *data) {
	if (count <= 0) return;
	struct ov *o = ov_open(app, OV_BOARD);
	ov_items(o, labels, count);
	o->live = calloc((size_t)count, sizeof(bool));
	for (int i = 0; i < count; i++) o->live[i] = live ? live[i] : true;
	o->cb = cb;
	o->data = data;
}

static void board_geometry(zui_app *app, struct ov *o, float *x0, float *top, float *side) {
	float W = app->width;
	float header = app->page ? zui_pg(app->page)->title_panel.h : 0;
	float s = (W - 2 * M.margin - (BOARD_COLS - 1) * BOARD_GAP) / BOARD_COLS;
	int rows = (o->count + BOARD_COLS - 1) / BOARD_COLS;
	float avail = app->height - header - 2 * BOARD_GAP;
	if (rows * (s + BOARD_GAP) > avail) s = avail / rows - BOARD_GAP;
	*x0 = M.margin;
	*top = header + BOARD_GAP;
	*side = s;
}

int board_cell_at(zui_app *app, struct ov *o, float x, float y) {
	float x0, top, side;
	board_geometry(app, o, &x0, &top, &side);
	for (int i = 0; i < o->count; i++) {
		float cx = x0 + (i % BOARD_COLS) * (side + BOARD_GAP), cy = top + (i / BOARD_COLS) * (side + BOARD_GAP);
		if (x >= cx && x < cx + side && y >= cy && y < cy + side) return i;
	}
	return -1;
}

static float swivel_ease(float t, bool in) {
	t = clamp01(t);
	float k = (in ? 200.0f : 150.0f) / 30.0f;
	return in ? 1 - (expf(k * (1 - t)) - 1) / (expf(k) - 1) : (expf(k * t) - 1) / (expf(k) - 1);
}

void draw_board(zui_app *app, struct ov *o, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	float x0, top, side;
	board_geometry(app, o, &x0, &top, &side);
	bool nomove = getenv("ZUI_NO_OPEN") != NULL;
	float age = (float)(int32_t)(now - (o->closing ? o->t_close : o->t0));
	bool dots = dotted(app);
	for (int i = 0; i < o->count; i++) {
		float own = age - i * BOARD_STAGGER;
		float angle, alpha = 1;
		if (o->closing) {
			if (own >= SWIVEL_OUT_MS) continue;
			float t = own / SWIVEL_OUT_MS;
			angle = own <= 0 ? 0 : 60 * swivel_ease(t, false);
			alpha = own <= 0 ? 1 : 1 - clamp01(t);
		} else {
			if (nomove) own = 1e6f;
			if (own < 0) continue;	/* its turn has not come */
			float t = own / SWIVEL_IN_MS, e = swivel_ease(t, true);
			angle = -45 * (1 - e);
			alpha = t >= 1 ? 1 : clamp01(e * 1.6f);
		}
		float cx = x0 + (i % BOARD_COLS) * (side + BOARD_GAP), cy = top + (i / BOARD_COLS) * (side + BOARD_GAP);
		struct zg_xf xf;
		zg_xf_identity(&xf);
		bool pressed = o->pressed == i;
		if (angle != 0) zg_xf_rotate_x(&xf, angle, cy + side / 2);
		if (pressed) zg_xf_scale_about(&xf, 0.94f, cx + side / 2, cy + side / 2);
		zg_set_xf(&xf);
		bool live = o->live[i];
		uint32_t fill = live ? zc_rgb(app->accent) : 0x262626ff;
		uint32_t ink = live ? 0xffffffff : 0xd9d9d9ff;
		if (dots) {
			fill = live ? zc_rgb(app->accent) : pal.text3;
			zg_border(cx, cy, side, side, 0, 1.2f, zc_alpha(fill, alpha));
			ink = live ? pal.text : pal.text3;
		} else {
			zg_rect(cx, cy, side, side, 0, zc_alpha(fill, alpha));
		}
		struct zt_layout *L = zt_layout_make(o->item[i], ZT_LIGHT, BOARD_FONT, side - BOARD_INSET, 1);
		zt_draw(L, cx + BOARD_INSET, cy + side - BOARD_INSET - L->line_h, zc_alpha(ink, alpha));
		zt_layout_free(L);
	}
	zg_set_xf(NULL);
}
