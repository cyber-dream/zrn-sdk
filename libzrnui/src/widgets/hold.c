// SPDX-License-Identifier: LGPL-2.1-or-later
/* Press and hold.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

ZUI_EXPORT void zui_set_hold(zui_node *n, zui_cb cb, void *data, zui_free_fn free_data) {
	if (n->hold_free) n->hold_free(n->hold_data);
	n->hold_cb = cb;
	n->hold_data = data;
	n->hold_free = free_data;
}

static void hold_fire(void *data) {
	zui_app *app = data;
	app->hold_timer = 0;
	zui_node *n = app->hold_node;
	if (!n || !app->touching || app->dragging || app->swiping || app->hub_drag || app->pressed != n) return;
	app->hold_fired = true;
	zui_haptic("heavy");
	zanim_go(&app->press, 0, 160, zui_now());
	app->firing++;
	n->event = ZUI_EVENT_HOLD;
	if (n->hold_cb) n->hold_cb(n, n->hold_data);
	n->event = ZUI_EVENT_NONE;
	app->dirty = true;
	zui_cb_end(app);
}

void zui_hold_start(zui_app *app, zui_node *n, uint32_t time) {
	zui_hold_cancel(app);
	app->hold_fired = false;
	if (!n || !n->hold_cb) { app->hold_node = NULL; return; }
	app->hold_node = n;
	app->hold_since = time;
	app->hold_timer = zui_timer(app, 500, false, hold_fire, app);
}

void zui_hold_cancel(zui_app *app) {
	if (app->hold_timer) zui_timer_cancel(app, app->hold_timer);
	app->hold_timer = 0;
}

/* a lift that comes after half a second on one spot is a hold, even if the timer did not wake us */
bool zui_hold_on_lift(zui_app *app, uint32_t time, uint32_t down_time) {
	if (app->hold_fired) { app->hold_fired = false; app->hold_node = NULL; return true; }
	zui_node *n = app->hold_node;
	app->hold_node = NULL;
	zui_hold_cancel(app);
	if (n && n == app->pressed && (int32_t)(time - down_time) >= 500) {
		app->hold_node = n;
		app->touching = true;	/* hold_fire checks it */
		hold_fire(app);
		app->touching = false;
		app->hold_fired = false;
		app->hold_node = NULL;
		return true;
	}
	return false;
}
