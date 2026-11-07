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
	zui_node *hub = app->page ? zui_page_hub(app->page) : NULL;
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
	if (app->page && zui_page_hub(app->page)) {
		bool running = false;
		zanim_value(&zui_page_hub(app->page)->knob, now, &running);
		if (running || zui_hub_title_moving(zui_page_hub(app->page), now)) app->dirty = true;
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
