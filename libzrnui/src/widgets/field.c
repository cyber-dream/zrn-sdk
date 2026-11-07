// SPDX-License-Identifier: LGPL-2.1-or-later
/* A password box and suggestions under a field.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

struct suggest { char **item; int n; };

ZUI_EXPORT void zui_field_set_password(zui_node *f, bool on) {
	if (f->kind != ZK_FIELD) return;
	zui_field_of(f)->password = on;
	f->app->dirty = true;
}

ZUI_EXPORT void zui_field_set_suggestions(zui_node *f, const char *const *items, int count) {
	if (f->kind != ZK_FIELD) return;
	struct suggest *s = zui_field_of(f)->suggest;
	if (s) {
		for (int i = 0; i < s->n; i++) free(s->item[i]);
		free(s->item);
		free(s);
	}
	zui_field_of(f)->suggest = NULL;
	if (!items || count <= 0) return;
	s = calloc(1, sizeof(*s));
	s->item = calloc((size_t)count, sizeof(char *));
	s->n = count;
	for (int i = 0; i < count; i++) s->item[i] = dupstr(items[i]);
	zui_field_of(f)->suggest = s;
}

static bool contains_nocase(const char *hay, const char *needle) {
	size_t n = strlen(needle);
	if (!n) return true;
	for (; *hay; hay++) {
		size_t i = 0;
		while (i < n && hay[i] && tolower((unsigned char)hay[i]) == tolower((unsigned char)needle[i])) i++;
		if (i == n) return true;
	}
	return false;
}

#define SUGGEST_MAX 4

#define SUGGEST_ROW 40

/* what hangs under the field with the keyboard: the matches, and where */
static int suggestions_for(zui_app *app, int out[SUGGEST_MAX], float *x, float *y, float *w) {
	zui_node *f = app->focus;
	if (!f || f->kind != ZK_FIELD || !zui_field_of(f)->suggest || !f->text || !*f->text) return 0;
	struct suggest *s = zui_field_of(f)->suggest;
	int c = 0;
	for (int i = 0; i < s->n && c < SUGGEST_MAX; i++)
		if (contains_nocase(s->item[i], f->text) && strcasecmp(s->item[i], f->text) != 0) out[c++] = i;
	if (!c) return 0;
	float top = screen_y(f) + (f->inline_hint ? 0 : 22) + M.field_h;
	float h = c * SUGGEST_ROW;
	if (top + h > app->height - 4) top = screen_y(f) + (f->inline_hint ? 0 : 22) - h;	/* over it instead */
	*x = f->x;
	*y = top;
	*w = f->w;
	return c;
}

void draw_suggestions(zui_app *app) {
	int idx[SUGGEST_MAX];
	float x, y, w;
	int c = suggestions_for(app, idx, &x, &y, &w);
	if (!c) return;
	struct suggest *s = zui_field_of(app->focus)->suggest;
	zg_set_xf(NULL);
	zg_rect(x, y, w, c * SUGGEST_ROW, 0, 0xffffffff);
	zg_border(x, y, w, c * SUGGEST_ROW, 0, 1, 0x999999ff);
	for (int i = 0; i < c; i++)
		text_now(s->item[idx[i]], ZT_REGULAR, 17, x + 10, y + i * SUGGEST_ROW + 9, 0x000000ff, w - 20);
}

bool zui_suggest_touch(zui_app *app, float x, float y, float scroll) {
	(void)scroll;
	int idx[SUGGEST_MAX];
	float fx, fy, fw;
	int c = suggestions_for(app, idx, &fx, &fy, &fw);
	if (!c || x < fx || x >= fx + fw || y < fy || y >= fy + c * SUGGEST_ROW) return false;
	zui_node *f = app->focus;
	struct suggest *s = zui_field_of(f)->suggest;
	int i = (int)((y - fy) / SUGGEST_ROW);
	zui_set_text(f, s->item[idx[i]]);
	zui_haptic("tap");
	zui_fire(f, ZUI_EVENT_SUBMIT);
	return true;
}

void suggest_free(zui_node *n) {
	struct suggest *s = zui_field_of(n)->suggest;
	if (s) {
		for (int i = 0; i < s->n; i++) free(s->item[i]);
		free(s->item);
		free(s);
	}
}
