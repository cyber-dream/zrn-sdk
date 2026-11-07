// SPDX-License-Identifier: LGPL-2.1-or-later
/* What core.c asks of a widget: layout, drawing, touch, freeing; and what moves with no one touching.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

bool zui_widgets_animating(zui_app *app, uint32_t now) {
	struct ov *o = app->ov;
	if (o && ov_moving(o, now)) return true;
	if (app->n_pickers && app->page && pickers_moving(app->page, now)) return true;
	zui_node *hub = app->page ? zui_pg(app->page)->hub : NULL;
	if (hub) {
		bool running = false;
		zanim_value(&hub->knob, now, &running);
		if (running || app->hub_drag || zui_hub_title_moving(hub, now)) return true;
	}
	return false;
}

/* what moves with no one touching: rings and tiles, only while they are on this page */
bool zui_widgets_page_moving(zui_app *app, zui_node *page, uint32_t now) {
	(void)app;
	return has_ring(page) || has_flip(page, now);
}

void zui_widgets_step(zui_app *app, uint32_t now) {
	struct ov *o = app->ov;
	if (o) {
		ov_step(app, o, now);
		if (o->closing && (int32_t)(now - o->t_close) >= 260) {
			ov_free(o);
			app->ov = NULL;
			app->dirty = true;
		}
	}
	/* a picker opening changes the page's layout every frame until it has opened */
	if (app->n_pickers && app->page && pickers_moving(app->page, now)) {
		app->laid_out = false;
		app->dirty = true;
	} else if (app->n_pickers && app->wl) {
		app->laid_out = false;
	}
	if (app->page && zui_pg(app->page)->hub) {
		bool running = false;
		zanim_value(&zui_pg(app->page)->hub->knob, now, &running);
		if (running || zui_hub_title_moving(zui_pg(app->page)->hub, now)) app->dirty = true;
	}
}

float zui_widget_layout(zui_node *n, float x, float y, float w) {
	zui_app *app = n->app;
	(void)x; (void)y; (void)w;
	switch (n->kind) {
	case ZK_CHECK: case ZK_RADIO: return M.item_h;
	case ZK_RING: return 40;
	case ZK_PICKER: return layout_picker(n);
	case ZK_TILES: return layout_tiles(n, x, y, w);
	case ZK_TILE: return n->h;
	case ZK_HUB: return layout_hub(n);
	case ZK_HUBSEC: return n->h;
	default: return -1;
	}
}

bool zui_widget_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) {
	switch (n->kind) {
	case ZK_CHECK: case ZK_RADIO: draw_check(app, n, now); return true;
	case ZK_RING: draw_ring(app, n, now); return true;
	case ZK_PICKER: draw_picker(app, n, now); return true;
	case ZK_TILES:
		for (zui_node *c = n->first; c; c = c->next) if (c->visible) zui_draw_node(app, c, xf, now);
		return true;
	case ZK_TILE: draw_tile(app, n, xf, now); return true;
	case ZK_HUB: draw_hub(app, n, xf, now); return true;
	default: return false;
	}
}

bool zui_widget_interactive(const zui_node *n) {
	switch (n->kind) {
	case ZK_CHECK: case ZK_RADIO: case ZK_PICKER: return true;
	case ZK_TILE: return n->cb != NULL || n->hold_cb != NULL;
	default: return false;
	}
}

bool zui_widget_activate(zui_app *app, zui_node *n) {
	switch (n->kind) {
	case ZK_CHECK:
		zui_set_checked(n, !n->on);
		zui_haptic("click");
		zui_fire(n, ZUI_EVENT_CHANGE);
		return true;
	case ZK_RADIO:
		if (!n->on) {
			zui_set_checked(n, true);
			radio_chosen(n);
			zui_haptic("click");
			zui_fire(n, ZUI_EVENT_CHANGE);
		}
		return true;
	case ZK_PICKER:
		picker_tapped(app, n);
		return true;
	case ZK_TILE:
		zui_haptic("tap");
		zui_fire(n, ZUI_EVENT_CLICK);
		return true;
	default: return false;
	}
}

void zui_widget_free(zui_node *n) {
	zui_app *app = n->app;
	zui_overlay_forget(app, n);
	if (app->open_picker == n) app->open_picker = NULL;
	if (app->hold_node == n) app->hold_node = NULL;
	if (n->hold_free) n->hold_free(n->hold_data);
	n->hold_free = NULL;
	switch (n->kind) {
	case ZK_ITEM: stamp_free(n); break;
	default: break;
	}
	n->ext = NULL;
}
