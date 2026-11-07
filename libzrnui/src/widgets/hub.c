// SPDX-License-Identifier: LGPL-2.1-or-later
/* The hub: sections side by side, a panorama.
 * Nothing here knows which style is in use: it draws with the palette and the metrics. */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "widgets/widgets.h"

float zui_node_hub_shift(const zui_node *n, float screen_x) {
	for (const zui_node *c = n, *p = n->parent; p; c = p, p = p->parent)
		if (p->kind == ZK_HUB) return zui_hub_pick_shift(p, c, screen_x);
	return 0;
}

#define HUB_PEEK 44	/* how much of the next section shows */

#define HUB_GAP 12

#define HUB_HEAD 32

#define HUB_TITLE_MS 420	/* the title coming in from the other edge, as the sections go round */
#define HUB_TITLE_OUT_MS 160	/* ... after going off quickly the way the finger went */
#define HUB_TITLE_CATCH_MS 220	/* the title catching up with a finger that took it mid-way */

struct hub {
	struct zui_page page;	/* a hub is a page: the sections are its children */
	zui_pixmap *bg;
	/* the title going round: let go past the last section (or the first), it goes off one edge and comes
	 * in from the other, to where it belongs, while the sections settle */
	struct zanim title_in;		/* how far it is from where it was or where it settles: gone off an edge, or coming in */
	float title_cur;		/* where it was when the finger let go (phase 1) */
	float title_final;		/* where it settles: its shift when the sections are there (phase 2) */
	int title_phase;		/* 0: not going round; 1: going off quickly; 2: coming in from the other edge */
	zui_node *title;		/* the hub's own large title: not in its children, the hub draws it (zui_hub_title) */
	float top;			/* where its sections start, under it */
	int title_dir;			/* +1 the way forward (off the left edge), -1 back (off the right) */
};

static inline struct hub *hub_of(const zui_node *n) {
	assert(n && n->kind == ZK_HUB);
	return (struct hub *)n;
}

static float hub_stride(const zui_node *hub) {
	zui_app *app = hub->app;
	return app->width - M.margin - HUB_PEEK + HUB_GAP;
}

static int hub_count(const zui_node *hub) {
	int c = 0;
	for (const zui_node *s = hub->first; s; s = s->next) c += s->visible && s->kind == ZK_HUBSEC;
	return c;
}

static zui_node *hub_nth(const zui_node *hub, int idx) {
	for (zui_node *s = hub->first; s; s = s->next) {
		if (!s->visible || s->kind != ZK_HUBSEC) continue;
		if (idx-- == 0) return s;
	}
	return NULL;
}

/* the section in front: the one that scrolls (zui_scroller) */
zui_node *zui_hub_front(const zui_node *hub) {
	return hub_nth(hub, zui_hub_current(hub));
}

float zui_hub_offset(const zui_node *hub) {
	return zanim_value(&hub->knob, zui_now(), NULL);
}

/* The sections go round: past the last comes the first, and before the first the last. The
 * offset is not kept between 0 and the end; each section is drawn where it is nearest the screen
 * (from one stride before it to the rest of the circle after). */
static float hub_circle(const zui_node *hub) {
	return hub_count(hub) * hub_stride(hub);
}

static bool hub_loops(const zui_node *hub) {
	return hub_count(hub) > 1;
}

/* Where a section is on the screen, as what to add to a point on the screen to find it among the
 * section's own things: one place, or two when the circle is short enough to show it at both edges
 * (two sections: the other one is at the right and at the left at once, so a sliver of a section is
 * always at the side). With fewer than two sections they stand still. */
int zui_hub_copies(const zui_node *hub, const zui_node *section, float dx[3]) {
	float off = zui_hub_offset(hub);
	if (!hub_loops(hub)) {
		dx[0] = off;
		return 1;
	}
	float len = hub_circle(hub), left = section->x - off, W = hub->app->width;
	int k0 = (int)floorf((-section->w - left) / len) - 1, k1 = (int)ceilf((W - left) / len) + 1, n = 0;
	for (int k = k0; k <= k1 && n < 3; k++) {
		float x = left + len * k;
		if (x > -section->w && x < W) dx[n++] = off - len * k;
	}
	if (n == 0) dx[n++] = off;
	return n;
}

/* the copy of the section a finger at screen_x is on (the first, if none) */
float zui_hub_pick_shift(const zui_node *hub, const zui_node *section, float screen_x) {
	float dx[3];
	int n = zui_hub_copies(hub, section, dx);
	for (int i = 0; i < n; i++) {
		float x = screen_x + dx[i];
		if (x >= section->x && x < section->x + section->w) return dx[i];
	}
	return dx[0];
}

static float hub_title_travel(const zui_node *hub);
static float hub_title_now(zui_node *hub, bool dragging);

/* the offset kept in one circle when a finger takes hold: the same picture, a smaller number */
float zui_hub_begin_drag(zui_node *hub) {
	float title = hub_title_now(hub, false);	/* where the title is, going round or not */
	float off = zui_hub_offset(hub);
	if (hub_loops(hub)) {
		float len = hub_circle(hub);
		off = fmodf(off, len);
		if (off < 0) off += len;
	}
	zanim_set(&hub->knob, off);
	struct hub *h = hub_of(hub);
	if (h) {
		/* a finger has the title again: it does not jump to the finger's, it is let go of where it is
		 * and catches up with it */
		float len = hub_circle(hub), tracked = len > 0 ? -off / len * hub_title_travel(hub) : 0;
		h->title_phase = 0;
		zanim_set(&h->title_in, title - tracked);
		zanim_go(&h->title_in, 0, HUB_TITLE_CATCH_MS, zui_now());
	}
	return off;
}

/* The hub's own large title (Hub.Header): not a widget the app makes, the hub has it; this sets its text. It
 * is the hub's, so it goes by more slowly than the sections and round with them. */
ZUI_EXPORT void zui_hub_title(zui_node *hub, const char *text) {
	struct hub *h = hub_of(hub);
	if (h && h->title) zui_set_text(h->title, text ? text : "");
	hub->app->laid_out = false;
}

float zui_hub_content_top(const zui_node *hub) {
	struct hub *h = hub_of(hub);
	return h ? h->top : 0;
}

ZUI_EXPORT zui_node *zui_hub(zui_app *app, const char *title) {
	zui_node *n = zui_page_new_kind(app, ZK_HUB);
	struct hub *h = hub_of(n);
	h->title = zui_node_new_detached(app, ZK_HEADER);
	zui_set_text(h->title, "");
	if (title) zui_hub_title(n, title);
	return n;
}

ZUI_EXPORT zui_node *zui_hub_section(zui_node *hub, const char *title) {
	zui_node *s = zui_node_new_kind(hub, ZK_HUBSEC);
	if (title) zui_title(s, title);	/* HubSection.Header: the small title, first in the section */
	return s;
}

ZUI_EXPORT void zui_hub_set_background(zui_node *hub, zui_pixmap *pix) {
	struct hub *h = hub_of(hub);
	if (!h) return;
	zui_pixmap_free(h->bg);
	h->bg = pix;
	if (pix) pix->refs++;
	hub->app->dirty = true;
}

static float hub_max(const zui_node *hub) {
	int c = hub_count(hub);
	return c > 1 ? (c - 1) * hub_stride(hub) : 0;
}

ZUI_EXPORT int zui_hub_current(const zui_node *hub) {
	float s = hub_stride(hub);
	int c = hub_count(hub), i = s > 0 ? (int)lroundf(zui_hub_offset(hub) / s) : 0;
	return c > 0 ? ((i % c) + c) % c : 0;
}

static void hub_title_go_round(zui_node *hub, int idx, int d, bool dragging);

ZUI_EXPORT void zui_hub_show(zui_node *hub, int section) {
	int c = hub_count(hub);
	if (section < 0) section = 0;
	if (section >= c) section = c - 1;
	/* the short way round */
	float s = hub_stride(hub), off = zui_hub_offset(hub);
	int here = s > 0 ? (int)lroundf(off / s) : 0, now_at = c > 0 ? ((here % c) + c) % c : 0, d = section - now_at;
	if (d > c / 2) d -= c;
	else if (d < -(c / 2) && c > 1) d += c;
	if (hub_loops(hub) && hub->app->wl) hub_title_go_round(hub, now_at, d, false);
	zanim_go(&hub->knob, (hub_loops(hub) ? here + d : section) * s, hub->app->wl ? 420 : 0, zui_now());
	hub->app->dirty = true;
}

void zui_hub_drag(zui_app *app, zui_node *hub, float dx) {
	float raw = app->hub_from - dx, max = hub_max(hub);
	/* the sections go round; one alone only follows the finger less and less */
	if (!hub_loops(hub)) {
		if (raw < 0) raw *= 0.4f;
		else if (raw > max) raw = max + (raw - max) * 0.4f;
	}
	zanim_set(&hub->knob, raw);
	app->dirty = true;
}

void zui_hub_release(zui_app *app, zui_node *hub, float v) {
	float s = hub_stride(hub), off = zui_hub_offset(hub);
	int from = (int)lroundf(app->hub_from / s), c = hub_count(hub), to = from;
	float moved = off - app->hub_from;
	/* as pivots: 50 points, or 0.3 points a ms, go on to the next */
	if (v > 0.3f || (moved > 50 && v >= -0.3f)) to = from + 1;
	else if (v < -0.3f || (moved < -50 && v <= 0.3f)) to = from - 1;
	if (!hub_loops(hub)) {
		if (to < 0) to = 0;
		if (to >= c) to = c - 1;
	} else {
		hub_title_go_round(hub, from, to - from, true);
	}
	zanim_go(&hub->knob, to * s, 420, zui_now());
	hub->knob.v0 = v;
	hub->knob.w = 0.018f;
	app->dirty = true;
}

/* How far the title can go over a whole turn of the circle: half of its own width, so that however
 * many sections there are, the rest of it is still on the screen. (A fixed share of the sections'
 * speed took it off the screen from the third section on.) */
static float hub_title_travel(const zui_node *hub) {
	zui_app *app = hub->app;
	struct hub *h = hub_of(hub);
	const char *text = h && h->title ? h->title->text : NULL;
	return text && *text ? zt_width(text, ZT_LIGHT, M.title) * 0.5f : 0;
}

/* Where the title is now. It goes by slower than the sections, by the same share at every section (the
 * whole of its travel is spread over the circle). While a finger is on the hub it follows the finger, straight
 * on past either end. Let go past the end, it goes off quickly the way the finger went and comes in from the
 * other edge to where it belongs, as on Windows Phone (zui_hub_release and zui_hub_show start that). Whatever
 * takes it over, a finger or a new turn round, starts from where it is: title_in is how far it still is from
 * what it is going to, and runs down to 0 from there. */
static float hub_title_now(zui_node *hub, bool dragging) {
	zui_app *app = hub->app;
	struct hub *h = hub_of(hub);
	uint32_t now = zui_now();
	bool running = false;
	float apart = h ? zanim_value(&h->title_in, now, &running) : 0;
	if (h && h->title_phase && !dragging) {
		if (h->title_phase == 1 && !running) {
			/* it is gone off one edge: now it comes in from the other */
			h->title_phase = 2;
			zanim_set(&h->title_in, h->title_dir * app->width);
			zanim_go(&h->title_in, 0, HUB_TITLE_MS, now);
			apart = zanim_value(&h->title_in, now, &running);
		}
		if (h->title_phase == 2 && !running) h->title_phase = 0;
		else return (h->title_phase == 1 ? h->title_cur : h->title_final) + apart;
	}
	float off = zui_hub_offset(hub), len = hub_circle(hub);
	if (hub_loops(hub) && !dragging) {
		off = fmodf(off, len);
		if (off < 0) off += len;
	}
	return (len > 0 ? -off / len * hub_title_travel(hub) : 0) + apart;
}


/* the title sets off for the other edge: first quickly off the way the finger went, from where it is,
 * then in from the other edge (hub_title_now does the second half). `idx` is where among the sections
 * it started (0 .. count-1) and `d` how many it goes by; outside the circle it goes round. `dragging`:
 * a finger just let go, so the title is where it followed it. */
static void hub_title_go_round(zui_node *hub, int idx, int d, bool dragging) {
	struct hub *h = hub_of(hub);
	int c = hub_count(hub), to = idx + d;
	if (!h || c < 2 || (to >= 0 && to < c)) return;
	float len = hub_circle(hub), travel = hub_title_travel(hub);
	h->title_cur = hub_title_now(hub, dragging);	/* from where it is, not from where it would be */
	h->title_dir = to >= c ? 1 : -1;
	h->title_final = -(float)(((to % c) + c) % c) * hub_stride(hub) / len * travel;
	h->title_phase = 1;
	zanim_set(&h->title_in, 0);
	zanim_go(&h->title_in, -h->title_dir * hub->app->width, HUB_TITLE_OUT_MS, zui_now());
}

/* the title is on its way: frames until it is there */
bool zui_hub_title_moving(zui_node *hub, uint32_t now) {
	struct hub *h = hub_of(hub);
	bool running = false;
	if (h) zanim_value(&h->title_in, now, &running);
	return h && (h->title_phase != 0 || running);
}

float layout_hub(zui_node *hub) {
	zui_app *app = hub->app;
	struct hub *hb = hub_of(hub);
	float stride = hub_stride(hub), sw = stride - HUB_GAP, h = 0;
	hub->x = 0;
	hub->w = app->width;
	/* the hub is the whole screen: its title first, from the status bar and 24 down, and the sections under it */
	bool titled = hb && hb->title && hb->title->text && *hb->title->text;
	hb->top = titled ? M.title_h : 0;
	if (hb && hb->title) zui_layout_at(hb->title, M.margin - 3, hub->y + M.top, app->width - 2 * M.margin);
	float y0 = hub->y + hb->top;
	int i = 0;
	for (zui_node *s = hub->first; s; s = s->next) {
		if (!s->visible || s->kind != ZK_HUBSEC) continue;
		float sx = M.margin + i++ * stride, at = y0 + HUB_HEAD;
		for (zui_node *c = s->first; c; c = c->next) {
			if (!c->visible) continue;
			if (c->kind == ZK_TITLE) {
				/* the section's title, the same widget as the application's: above its column */
				zui_layout_at(c, sx, y0 + 2, sw);
				continue;
			}
			at += zui_layout_at(c, sx, at, sw) + M.gap;
		}
		s->x = sx;
		s->y = y0;
		s->w = sw;
		s->h = at - y0;
		zui_sv(s)->content_h = y0 + (s->h > app->height - y0 ? s->h : app->height - y0);
		if (s->h > h) h = s->h;
	}
	/* it fills the screen under the title, so the picture behind has somewhere to be */
	float fill = app->height - y0;
	return hb->top + (h > fill ? h : fill);
}

void draw_hub(zui_app *app, zui_node *hub, const struct zg_xf *base, uint32_t now) {
	struct zui_palette pal = zui_pal(app);
	struct hub *hb = hub_of(hub);
	float off = zanim_value(&hub->knob, now, NULL), W = app->width;
	if (hb && hb->bg) {
		/* the picture moves a quarter as fast as the sections, and is dimmed to read over */
		float span = W + hub_max(hub) * 0.25f, x = -off * 0.25f;
		struct zg_xf id = *base;
		zg_set_xf(&id);
		if (hub_loops(hub)) {
			/* going round, the picture goes round too: one more of it where the first ends */
			float period = hub_circle(hub) * 0.25f;
			x = -fmodf(off, hub_circle(hub)) * 0.25f;
			if (x > 0) x -= period;
			zui_draw_pixmap(app, hb->bg, x, hub->y, span, hub->h, 0, false, pal.surface2);
			zui_draw_pixmap(app, hb->bg, x + period, hub->y, span, hub->h, 0, false, pal.surface2);
		} else {
			zui_draw_pixmap(app, hb->bg, x, hub->y, span, hub->h, 0, false, pal.surface2);
		}
		zg_rect(0, hub->y, W, hub->h, 0, zc_alpha(pal.bg, 0.6f));
	}
	/* the hub's own title, over the picture, going by slower than the sections */
	float top = hub->y + (hb ? hb->top : 0);
	if (hb && hb->title && hb->title->text && *hb->title->text) {
		struct zg_xf tx = *base;
		zg_xf_translate(&tx, hub_title_now(hub, app->hub_drag), 0, 0);
		zg_set_xf(&tx);
		zui_draw_node(app, hb->title, &tx, now);
	}
	/* what scrolls in a section goes no higher than under the title */
	zg_clip(0, top, W, app->height - top);
	for (zui_node *s = hub->first; s; s = s->next) {
		if (!s->visible || s->kind != ZK_HUBSEC) continue;
		/* each section where it shows: they go round, and one may show at both edges */
		float dx[3];
		int copies = zui_hub_copies(hub, s, dx);
		for (int k = 0; k < copies; k++) {
			float shift = -dx[k], sx = s->x + shift;
			if (sx > W || sx + s->w < 0) continue;
			struct zg_xf xf = *base;
			zg_xf_translate(&xf, shift, -zui_sv(s)->scroll, 0);
			zg_set_xf(&xf);
			for (zui_node *c = s->first; c; c = c->next)
				if (c->visible) zui_draw_node(app, c, &xf, now);
		}
	}
	zg_clip(0, zui_pg(hub)->title_panel.h, W, app->height - zui_pg(hub)->title_panel.h);
}

void hub_free(zui_node *n) {
	zui_page_destroy(n);
	struct hub *h = hub_of(n);
	if (h) {
		if (h->title) zui_node_free_detached(h->title);
		zui_pixmap_free(h->bg);
	}
}

static float hub_layout(zui_node *n, float x, float y, float w) { (void)x; (void)y; (void)w; return layout_hub(n); }
static void hub_draw(zui_app *app, zui_node *n, const struct zg_xf *xf, uint32_t now) { draw_hub(app, n, xf, now); }
const struct zui_class zui_hub_class = { .name = "hub", .size = sizeof(struct hub), .is_page = true, .destroy = hub_free,
	.layout = hub_layout, .draw = hub_draw };

/* a section is a scroll container (an heir of it) whose place the hub gives it */
static float hubsec_layout(zui_node *n, float x, float y, float w) { (void)x; (void)y; (void)w; return n->h; }
const struct zui_class zui_hubsec_class = { .name = "hub section", .size = sizeof(struct zui_scroll), .scrolls = true,
	.layout = hubsec_layout };
