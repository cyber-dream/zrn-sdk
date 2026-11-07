// SPDX-License-Identifier: LGPL-2.1-or-later
/* What goes over a page: opening, touch, motion and drawing it. menu, dialog, jump list and the date and time pickers are in their own files.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

static struct ov *OV(const zui_app *app) { return app->ov; }

void ov_free(struct ov *o) {
	if (!o) return;
	free(o->title);
	free(o->msg);
	for (int i = 0; i < o->count; i++) if (o->item) free(o->item[i]);
	free(o->item);
	free(o->live);
	free(o);
}

void zui_widgets_free(zui_app *app) {
	ov_free(app->ov);
	app->ov = NULL;
}

static void drop_focus(zui_app *app) {
	if (app->focus) {
		app->focus->focused = false;
		app->focus = NULL;
		zui_osk(app, false);
	}
}

struct ov *ov_open(zui_app *app, int kind) {
	ov_free(app->ov);
	struct ov *o = calloc(1, sizeof(*o));
	o->kind = kind;
	o->t0 = zui_now();
	o->pressed = -1;
	o->drag = -1;
	app->ov = o;
	drop_focus(app);
	zui_pickers_close_except(app, NULL);
	app->pressed = NULL;
	app->dirty = true;
	return o;
}

void ov_items(struct ov *o, const char *const *items, int count) {
	o->count = count < 0 ? 0 : count;
	o->item = calloc((size_t)(o->count ? o->count : 1), sizeof(char *));
	for (int i = 0; i < o->count; i++) o->item[i] = dupstr(items[i]);
}

ZUI_EXPORT bool zui_overlay_open(const zui_app *app) { return app->ov && !OV(app)->closing; }

ZUI_EXPORT void zui_overlay_close(zui_app *app) {
	struct ov *o = app->ov;
	if (!o || o->closing) return;
	o->closing = true;
	o->t_close = zui_now();
	app->dirty = true;
}

bool zui_widgets_back(zui_app *app) {
	if (!zui_overlay_open(app)) return false;
	zui_overlay_close(app);
	return true;
}

/* ---- touch ---- */
static void ov_choose(zui_app *app, struct ov *o, int index) {
	zui_choice_fn cb = o->cb;
	void *data = o->data;
	zui_overlay_close(app);
	zui_haptic("tap");
	if (cb) {
		app->firing++;
		cb(app, index, data);
		app->dirty = true;
		zui_cb_end(app);
	}
}

bool zui_widgets_touch(zui_app *app, int phase, float x, float y, uint32_t time) {
	struct ov *o = app->ov;
	if (!o) return false;
	uint32_t now = zui_now();
	if (o->closing) {
		if ((int32_t)(now - o->t_close) < 260) return true;	/* going: it takes the touch and ignores it */
		ov_free(o);	/* gone, though no frame has said so */
		app->ov = NULL;
		return false;
	}
	app->dirty = true;
	if (phase == 0) {
		o->down_x = x;
		o->down_y = y;
		o->moved = false;
		o->pressed = -1;
		switch (o->kind) {
		case OV_MENU: o->pressed = menu_row_at(app, o, x, y); break;
		case OV_DIALOG: o->pressed = dlg_button_at(app, o, x, y); break;
		case OV_BOARD: {
			int c = board_cell_at(app, o, x, y);
			o->pressed = c >= 0 && o->live[c] ? c : -1;
			break;
		}
		default:
			o->pressed = pick_button_at(app, x, y);
			o->drag = -1;
			if (o->pressed < 0)
				for (int i = 0; i < o->ncol; i++)
					if (x >= o->col[i].x - 6 && x < o->col[i].x + o->col[i].w + 6) o->drag = i;
			if (o->drag >= 0) {
				struct col *c = &o->col[o->drag];
				o->drag_y0 = o->last_y = y;
				o->drag_pos0 = c->pos;
				c->v = 0;
				c->target = c->pos;
				o->last_t = time;
			}
			break;
		}
		return true;
	}
	if (phase == 1) {
		if (fabsf(y - o->down_y) > 8 || fabsf(x - o->down_x) > 8) o->moved = true;
		if (o->drag >= 0 && o->moved) {
			struct col *c = &o->col[o->drag];
			float raw = o->drag_pos0 - (y - o->drag_y0) / ROW_H;
			if (!c->loop) raw = raw < -0.4f ? -0.4f : raw > c->n - 0.6f ? c->n - 0.6f : raw;
			float dt = (float)(int32_t)(time - o->last_t);
			if (dt > 0) c->v = c->v * 0.3f + (-(y - o->last_y) / ROW_H / dt) * 0.7f;
			o->last_y = y;
			o->last_t = time;
			c->pos = c->target = raw;
			pick_fit_days(o);
		}
		return true;
	}
	/* up or cancel */
	int pressed = o->pressed;
	o->pressed = -1;
	if (phase != 2) {
		if (o->drag >= 0) {
			struct col *c = &o->col[o->drag];
			c->target = roundf(c->pos);
		}
		o->drag = -1;
		return true;
	}
	switch (o->kind) {
	case OV_MENU: {
		int hit = menu_row_at(app, o, x, y);
		if (!o->moved && hit >= 0 && hit == pressed) ov_choose(app, o, hit);
		else if (!o->moved && hit < 0 && pressed < 0) zui_overlay_close(app);
		break;
	}
	case OV_DIALOG: {
		int hit = dlg_button_at(app, o, x, y);
		if (!o->moved && hit >= 0 && hit == pressed) ov_choose(app, o, hit);
		break;
	}
	case OV_BOARD: {
		int hit = board_cell_at(app, o, x, y);
		if (!o->moved && hit >= 0 && hit == pressed) ov_choose(app, o, hit);
		else if (!o->moved && hit < 0) zui_overlay_close(app);
		break;
	}
	default: {
		if (pressed >= 0 && pick_button_at(app, x, y) == pressed && !o->moved) {
			zui_haptic("tap");
			if (pressed == 0) pick_commit(app, o);
			zui_overlay_close(app);
			break;
		}
		if (o->drag >= 0) {
			struct col *c = &o->col[o->drag];
			if (!o->moved) {
				/* a tap on a row above or under the middle brings it there */
				float cy = (app->height - BAR_H) / 2 + 10;
				int k = (int)lroundf((y - cy) / ROW_H);
				c->target = roundf(c->pos) + (float)k;
				if (!c->loop) c->target = c->target < 0 ? 0 : c->target > c->n - 1 ? c->n - 1 : c->target;
			} else {
				/* it goes on a little with the speed it was let go at, then settles on a row */
				float to = roundf(c->pos + c->v * 160);
				if (!c->loop) to = to < 0 ? 0 : to > c->n - 1 ? c->n - 1 : to;
				c->target = to;
			}
		}
		break;
	}
	}
	o->drag = -1;
	(void)now;
	return true;
}

/* ---- motion ---- */
bool ov_moving(const struct ov *o, uint32_t now) {
	if (o->closing) return (int32_t)(now - o->t_close) < 260;
	uint32_t age = now - o->t0;
	if (age < 420 + (o->kind == OV_BOARD ? (uint32_t)(o->count * BOARD_STAGGER) : 0)) return true;
	for (int i = 0; i < o->ncol; i++)
		if (fabsf(o->col[i].target - o->col[i].pos) > 0.003f || o->drag == i) return true;
	return false;
}

void ov_step(zui_app *app, struct ov *o, uint32_t now) {
	float dt = o->last ? (float)(int32_t)(now - o->last) : 16;
	if (dt > 64) dt = 64;
	if (dt < 0) dt = 0;
	o->last = now;
	for (int i = 0; i < o->ncol; i++) {
		struct col *c = &o->col[i];
		if (o->drag == i) continue;
		float d = c->target - c->pos;
		if (fabsf(d) > 0.003f) {
			c->pos += d * (1 - expf(-dt / 80));
			if (fabsf(c->target - c->pos) <= 0.003f) c->pos = c->target;
			app->dirty = true;
		}
	}
	if (o->kind == OV_DATE) pick_fit_days(o);
}

/* ---- drawing the overlays ---- */
static float ov_alpha(const struct ov *o, uint32_t now) {
	if (getenv("ZUI_NO_OPEN") && !o->closing) return 1;
	if (o->closing) return 1 - clamp01((float)(int32_t)(now - o->t_close) / 200);
	return clamp01((float)(int32_t)(now - o->t0) / 120);
}

static void scrim(zui_app *app, float a, bool solid) {
	struct zui_palette pal = zui_pal(app);
	float al = solid ? 1 : dotted(app) ? 0.97f : (float)(pal.scrim & 0xff) / 255.0f;
	zg_rect(0, 0, app->width, app->height, 0, zc_alpha(pal.bg, al * a));
}

void zui_widgets_draw_over(zui_app *app, uint32_t now) {
	draw_suggestions(app);
	struct ov *o = app->ov;
	if (!o) return;
	zg_noclip();
	zg_set_xf(NULL);
	zg_set_alpha(1);
	float a = ov_alpha(o, now);
	switch (o->kind) {
	case OV_MENU:
		scrim(app, a, false);
		draw_menu(app, o, a);
		break;
	case OV_DIALOG:
		scrim(app, a, false);
		draw_dialog(app, o, a, now);
		break;
	case OV_BOARD:
		/* the page stays faintly there under the tiles, ZrnMetroCore's 0.85 */
		zg_rect(0, 0, app->width, app->height, 0, zc_alpha(zui_pal(app).bg, (dotted(app) ? 0.97f : 0.85f) * (o->closing || getenv("ZUI_NO_OPEN") ? a : clamp01((float)(int32_t)(now - o->t0) / 100))));
		draw_board(app, o, now);
		break;
	default:
		draw_pick(app, o, a, now);
		break;
	}
	zg_set_xf(NULL);
	zg_set_alpha(1);
}

/* a node that goes away is not the source of the overlay any more */
void zui_overlay_forget(zui_app *app, const zui_node *n) {
	if (app->ov && OV(app)->source == n) OV(app)->source = NULL;
}
