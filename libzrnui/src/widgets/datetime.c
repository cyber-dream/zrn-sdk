// SPDX-License-Identifier: LGPL-2.1-or-later
/* Date and time pickers.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets/widgets.h"

int pick_button_at(zui_app *app, float x, float y) {
	if (y < app->height - BAR_H) return -1;
	return x < app->width / 2 ? 0 : 1;	/* 0 the tick, 1 the cross */
}

static const char *const MONTHS[12] = { "January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December" };

static int days_in(int year, int month) {
	static const int d[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) return 29;
	return d[(month - 1 + 12) % 12];
}

static int col_value(const struct col *c) {
	int i = (int)lroundf(c->pos);
	if (c->loop) return ((i % c->n) + c->n) % c->n;
	return i < 0 ? 0 : i >= c->n ? c->n - 1 : i;
}

#define YEAR_LO 1900

#define YEAR_N 201

/* what the date/time overlay is editing is kept in the picker node */
struct stamp { int year, month, day, hour, minute; bool time; zui_cb cb; void *data; };

static void pick_layout_cols(zui_app *app, struct ov *o) {
	float W = app->width, avail = W - 2 * M.margin;
	if (o->kind == OV_DATE) {
		float widths[3] = { 140, 70, 100 }, total = 310 + 18, sc = avail < total ? avail / total : 1, x = (W - total * sc) / 2;
		for (int i = 0; i < 3; i++) {
			o->col[i].x = x;
			o->col[i].w = widths[i] * sc;
			x += widths[i] * sc + 9 * sc;
		}
	} else {
		float w = 110, x = (W - 2 * w - 20) / 2;
		for (int i = 0; i < 2; i++) { o->col[i].x = x; o->col[i].w = w; x += w + 20; }
	}
}

void open_picker(zui_node *n) {
	zui_app *app = n->app;
	struct stamp *s = n->ext;
	struct ov *o = ov_open(app, s->time ? OV_TIME : OV_DATE);
	o->source = n;
	o->title = dupstr(n->text);
	if (!s->time) {
		/* month, day, year: as the phone's English has them */
		o->ncol = 3;
		o->col[0] = (struct col){ .n = 12, .names = MONTHS, .loop = true, .pos = (float)(s->month - 1) };
		o->col[1] = (struct col){ .n = days_in(s->year, s->month), .lo = 1, .pad = 1, .loop = true, .pos = (float)(s->day - 1) };
		o->col[2] = (struct col){ .n = YEAR_N, .lo = YEAR_LO, .pad = 4, .loop = false, .pos = (float)(s->year - YEAR_LO) };
	} else {
		o->ncol = 2;
		o->col[0] = (struct col){ .n = 24, .lo = 0, .pad = 2, .loop = true, .pos = (float)s->hour };
		o->col[1] = (struct col){ .n = 60, .lo = 0, .pad = 2, .loop = true, .pos = (float)s->minute };
	}
	for (int i = 0; i < o->ncol; i++) o->col[i].target = o->col[i].pos;
	pick_layout_cols(app, o);
}

static void stamp_text(zui_node *n) {
	struct stamp *s = n->ext;
	char buf[48];
	if (s->time) snprintf(buf, sizeof(buf), "%02d:%02d", s->hour, s->minute);
	else snprintf(buf, sizeof(buf), "%d %s %d", s->day, MONTHS[(s->month - 1 + 12) % 12], s->year);
	zui_set_detail(n, buf);
}

void picker_row_tapped(zui_node *n, void *data) { (void)data; open_picker(n); }

static zui_node *stamp_row(zui_node *parent, const char *title, struct stamp *init, zui_cb cb, void *data) {
	zui_node *n = zui_item(parent, title, NULL, picker_row_tapped, NULL);
	struct stamp *s = malloc(sizeof(*s));
	*s = *init;
	s->cb = cb;
	s->data = data;
	n->ext = s;
	stamp_text(n);
	return n;
}

ZUI_EXPORT zui_node *zui_date_picker(zui_node *parent, const char *title, int year, int month, int day,
		zui_cb cb, void *data) {
	struct stamp s = { .year = year, .month = month < 1 ? 1 : month > 12 ? 12 : month, .day = day < 1 ? 1 : day, .time = false };
	if (s.day > days_in(year, s.month)) s.day = days_in(year, s.month);
	if (s.year < YEAR_LO) s.year = YEAR_LO;
	if (s.year >= YEAR_LO + YEAR_N) s.year = YEAR_LO + YEAR_N - 1;
	return stamp_row(parent, title, &s, cb, data);
}

ZUI_EXPORT zui_node *zui_time_picker(zui_node *parent, const char *title, int hour, int minute, zui_cb cb, void *data) {
	struct stamp s = { .hour = ((hour % 24) + 24) % 24, .minute = ((minute % 60) + 60) % 60, .time = true };
	return stamp_row(parent, title, &s, cb, data);
}

ZUI_EXPORT void zui_date_picker_get(const zui_node *n, int *y, int *m, int *d) {
	struct stamp *s = n->ext;
	if (!s || s->time) return;
	if (y) *y = s->year;
	if (m) *m = s->month;
	if (d) *d = s->day;
}

ZUI_EXPORT void zui_date_picker_set(zui_node *n, int y, int m, int d) {
	struct stamp *s = n->ext;
	if (!s || s->time) return;
	s->year = y < YEAR_LO ? YEAR_LO : y >= YEAR_LO + YEAR_N ? YEAR_LO + YEAR_N - 1 : y;
	s->month = m < 1 ? 1 : m > 12 ? 12 : m;
	s->day = d < 1 ? 1 : d > days_in(s->year, s->month) ? days_in(s->year, s->month) : d;
	stamp_text(n);
}

ZUI_EXPORT void zui_time_picker_get(const zui_node *n, int *h, int *m) {
	struct stamp *s = n->ext;
	if (!s || !s->time) return;
	if (h) *h = s->hour;
	if (m) *m = s->minute;
}

ZUI_EXPORT void zui_time_picker_set(zui_node *n, int h, int m) {
	struct stamp *s = n->ext;
	if (!s || !s->time) return;
	s->hour = ((h % 24) + 24) % 24;
	s->minute = ((m % 60) + 60) % 60;
	stamp_text(n);
}

void pick_commit(zui_app *app, struct ov *o) {
	zui_node *n = o->source;
	if (!n) return;
	struct stamp *s = n->ext;
	if (o->kind == OV_DATE) {
		s->month = col_value(&o->col[0]) + 1;
		s->year = YEAR_LO + col_value(&o->col[2]);
		int d = col_value(&o->col[1]) + 1, dim = days_in(s->year, s->month);
		s->day = d > dim ? dim : d;
	} else {
		s->hour = col_value(&o->col[0]);
		s->minute = col_value(&o->col[1]);
	}
	stamp_text(n);
	if (s->cb) {
		app->firing++;
		n->event = ZUI_EVENT_CHANGE;
		s->cb(n, s->data);
		n->event = ZUI_EVENT_NONE;
		zui_cb_end(app);
	}
}

/* the day column is as long as its month */
void pick_fit_days(struct ov *o) {
	if (o->kind != OV_DATE) return;
	int month = col_value(&o->col[0]) + 1, year = YEAR_LO + col_value(&o->col[2]);
	struct col *d = &o->col[1];
	int dim = days_in(year, month);
	if (dim == d->n) return;
	int v = col_value(d);
	d->n = dim;
	int nv = v >= dim ? dim - 1 : v;
	d->pos = d->target = (float)nv;
}

static void draw_pick_button(zui_app *app, float cx, float y, const char *icon, const char *word, bool down, float a) {
	struct zui_palette pal = zui_pal(app);
	uint32_t c = zc_alpha(pal.text, a);
	if (down) zg_circle(cx, y + 22, 18, c);
	else zg_ring(cx, y + 22, 18, 2, c);
	zui_icon(icon, cx - 10, y + 12, 20, 2, down ? zc_alpha(pal.bg, a) : c);
	struct zt_layout *L = zt_layout_make(word, ZT_REGULAR, 11.5f, 0, 0);
	zt_draw(L, cx - L->w / 2, y + 42, c);
	zt_layout_free(L);
}

static void col_label(const struct col *c, int i, char *out, size_t size) {
	int v = ((i % c->n) + c->n) % c->n;
	if (c->names) snprintf(out, size, "%s", c->names[v]);
	else snprintf(out, size, "%0*d", c->pad, c->lo + v);
}

void draw_pick(zui_app *app, struct ov *o, float a, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	float W = app->width, H = app->height;
	/* the page is gone behind black: the screen belongs to the picker */
	zg_rect(0, 0, W, H, 0, zc_alpha(pal.bg, a));
	char *caps = dupstr(zui_panel_text(app->page, ZK_TITLE));
	for (char *p = caps; *p; p++) if ((unsigned char)*p < 0x80) *p = (char)toupper((unsigned char)*p);
	text_now(caps, ZT_STRONG, 11.5f, M.margin, M.top, zc_alpha(pal.text, a), W - 2 * M.margin);
	free(caps);
	char *lower = dupstr(o->title);
	for (char *p = lower; *p; p++) if ((unsigned char)*p < 0x80) *p = (char)tolower((unsigned char)*p);
	text_now(lower, ZT_LIGHT, 38, M.margin - 2, M.top + 16, zc_alpha(pal.text, a), W - 2 * M.margin);
	free(lower);
	float cy = (H - BAR_H) / 2 + 10;
	for (int ci = 0; ci < o->ncol; ci++) {
		struct col *c = &o->col[ci];
		int mid = (int)floorf(c->pos);
		zg_clip(c->x - 4, cy - 2.6f * ROW_H, c->w + 8, 5.2f * ROW_H);
		for (int i = mid - 3; i <= mid + 4; i++) {
			if (!c->loop && (i < 0 || i >= c->n)) continue;
			float d = fabsf((float)i - c->pos), y = cy + ((float)i - c->pos) * ROW_H;
			if (d > 2.6f) continue;
			char buf[24];
			col_label(c, i, buf, sizeof(buf));
			/* the one in the middle in the accent; the rest grey and fading */
			uint32_t colour = zc_mix(pal.text3, pal.accent_text, clamp01(1.2f - d));
			float fade = d < 0.5f ? 1 : clamp01(0.75f - (d - 0.5f) * 0.32f);
			struct zt_layout *L = zt_layout_make(buf, ZT_LIGHT, c->names ? 28 : 34, c->w, 1);
			zt_draw(L, c->x + (c->w - L->w) / 2, y - L->line_h / 2, zc_alpha(colour, a * fade));
			zt_layout_free(L);
		}
		zg_noclip();
	}
	if (o->kind == OV_TIME)
		text_now(":", ZT_LIGHT, 34, o->col[0].x + o->col[0].w + 4, cy - 24, zc_alpha(pal.text3, a), 0);
	/* the bar: a tick and a cross */
	zg_rect(0, H - BAR_H, W, BAR_H, 0, zc_alpha(pal.surface2, a));
	draw_pick_button(app, W * 0.25f, H - BAR_H + 8, "check", "done", o->pressed == 0, a);
	draw_pick_button(app, W * 0.75f, H - BAR_H + 8, "close", "cancel", o->pressed == 1, a);
	(void)now;
}

/* a date or time row owns its stamp */
void stamp_free(zui_node *n) {
	if (n->ext && n->cb == picker_row_tapped) free(n->ext);
}
