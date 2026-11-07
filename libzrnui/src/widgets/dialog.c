// SPDX-License-Identifier: LGPL-2.1-or-later
/* The dialog.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

ZUI_EXPORT void zui_dialog(zui_app *app, const char *title, const char *message, const char *const *buttons,
		int count, zui_choice_fn cb, void *data) {
	if (count <= 0) return;
	if (count > 3) count = 3;
	struct ov *o = ov_open(app, OV_DIALOG);
	ov_items(o, buttons, count);
	o->title = dupstr(title);
	o->msg = dupstr(message);
	o->cb = cb;
	o->data = data;
}

struct dlg { float y, h, ty, my, by, bw; struct zt_layout *T, *L; };

static void dlg_geometry(zui_app *app, struct ov *o, struct dlg *d) {
	float W = app->width, pad = M.margin;
	d->T = zt_layout_make(o->title, ZT_LIGHT, 26, W - 2 * pad, 0);
	d->L = zt_layout_make(o->msg, ZT_REGULAR, 16, W - 2 * pad, 0);
	d->ty = 22;
	d->my = d->ty + d->T->h + 10;
	d->by = d->my + (o->msg && *o->msg ? d->L->h : 0) + 24;
	d->h = d->by + 40 + 22;
	d->y = (app->height - d->h) / 3;
	if (d->y < 0) d->y = 0;
	d->bw = (W - 2 * pad - 12 * (o->count - 1)) / o->count;
}

static void dlg_free(struct dlg *d) {
	zt_layout_free(d->T);
	zt_layout_free(d->L);
}

int dlg_button_at(zui_app *app, struct ov *o, float x, float y) {
	struct dlg d;
	dlg_geometry(app, o, &d);
	int hit = -1;
	for (int i = 0; i < o->count; i++) {
		float bx = M.margin + i * (d.bw + 12), by = d.y + d.by;
		if (x >= bx && x < bx + d.bw && y >= by && y < by + 40) hit = i;
	}
	dlg_free(&d);
	return hit;
}

void draw_dialog(zui_app *app, struct ov *o, float a, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	struct dlg d;
	dlg_geometry(app, o, &d);
	float W = app->width, e = zui_ease_out(a);
	struct zg_xf xf;
	zg_xf_identity(&xf);
	zg_xf_translate(&xf, 0, (1 - e) * -24, 0);
	zg_set_xf(&xf);
	zg_rect(0, d.y, W, d.h, 0, zc_alpha(pal.surface2, a));
	zt_draw(d.T, M.margin, d.y + d.ty, zc_alpha(pal.text, a));
	if (o->msg && *o->msg) zt_draw(d.L, M.margin, d.y + d.my, zc_alpha(pal.text2, a));
	for (int i = 0; i < o->count; i++) {
		float bx = M.margin + i * (d.bw + 12), by = d.y + d.by;
		bool down = o->pressed == i;
		if (down) zg_rect(bx, by, d.bw, 40, 0, zc_alpha(pal.text, a));
		else zg_border(bx, by, d.bw, 40, 0, 2, zc_alpha(pal.text, a));
		struct zt_layout *L = zt_layout_make(o->item[i], ZT_STRONG, M.button_text, d.bw - 16, 1);
		zt_draw(L, bx + (d.bw - L->w) / 2, by + (40 - L->line_h) / 2, zc_alpha(down ? pal.bg : pal.text, a));
		zt_layout_free(L);
	}
	dlg_free(&d);
	zg_set_xf(NULL);
	(void)now;
}
