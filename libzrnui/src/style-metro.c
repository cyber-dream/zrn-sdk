// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Metro: the same widgets as ztMETRO's own apps draw them with
 * ZrnMetroCore. Black, white type in Segoe's open twin (Selawik), the app's
 * name in capitals over a large lowercase title, rectangular toggles and
 * outlined buttons, the accent for what is on. A press tilts what is under
 * the finger; pages turn on their left edge, row after row (the turnstile).
 * Numbers are ZrnMetroCore's (ztmetro/core/zrnmetrocore.c).
 */
#define _GNU_SOURCE
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zui-internal.h"

static struct zui_palette metro_palette(const zui_app *app) {
	struct zui_palette p = {
		.bg = 0x000000ff, .surface = 0x000000ff, .surface2 = 0x1a1a1aff,
		.text = 0xffffffff, .text2 = 0x999999ff, .text3 = 0x666666ff,
		.divider = 0x00000000, .track = 0x333333ff, .knob = 0xffffffff,
		.shadow = 0x00000000, .danger = 0xe51400ff, .scrim = 0x000000b3,
		.on_accent = 0xffffffff, .bubble_caption = 0xd1d1d1ff,
	};
	p.accent_text = zc_rgb(app->accent);
	return p;
}

#define M (app->style->m)
#define TILT_DEG 14.0f
#define TILT_LIFT 40.0f
#define PRESS_SINK 0.045f


static float page_top(zui_app *app, zui_node *page, float y, bool draw) {
	(void)app; (void)page; (void)draw;
	return y;
}

static void header(zui_app *app, zui_node *page, uint32_t now) {
	(void)now;
	struct zui_palette pal = app->style->palette(app);
	if (!page->bg) zg_rect(0, 0, app->width, zui_pg(page)->title_panel.h, 0, pal.bg);	/* what is under the titles, if the page has no background */
	if (zui_pg(page)->compact) {
		/* a person: their picture, their name large, how they are under it */
		zui_draw_leading(app, page, M.margin, M.top - 4, 46, 0, 0xffffffff);
		float x = M.margin + 46 + 10, room = app->width - x - M.margin;
		zt_draw(zt_slot_get(&page->title_slot[0], zui_panel_text(page, ZK_HEADER), ZT_LIGHT, 24, room, 1), x, M.top - 8, pal.text);
		if (page->subtitle && *page->subtitle)
			zt_draw(zt_slot_get(&page->t[1], page->subtitle, ZT_REGULAR, 13, room, 1), x, M.top + 24, pal.text2);
		return;
	}
	/* the TitlePanel's titles are widgets, drawn over this by the page */
}

static void card(zui_app *app, zui_node *n) { (void)app; (void)n; }
static void divider(zui_app *app, zui_node *c, zui_node *b) { (void)app; (void)c; (void)b; }

static void section_label(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	struct zt_layout *L = zt_slot_get(&n->t[0], n->text, ZT_REGULAR, M.label, n->w, 1);
	zt_draw(L, n->x, n->y + M.section_gap, pal.text2);
}

static void button(zui_app *app, zui_node *n, float press) {
	struct zui_palette pal = app->style->palette(app);
	float x = n->x, w = n->w, y = n->y, h = M.button_h;
	bool down = press > 0.5f;
	if (n->icon) {
		/* WP's action bar at this width: a 36 point ring on a 57 point slot */
		float cx = x + w / 2, cy = y + 22;
		uint32_t c = n->enabled ? pal.text : pal.text2;
		if (down) zg_circle(cx, cy, 18, c);
		else zg_ring(cx, cy, 18, 2, c);
		zui_icon(n->icon, cx - 10, cy - 10, 20, 2, down ? 0x000000ff : c);
		if (app->bar_words) {
			struct zt_layout *W = zt_slot_get(&n->t[0], n->text, ZT_REGULAR, 11.5f, w, 1);
			zt_draw(W, x + (w - W->w) / 2, y + 42, c);
		}
		return;
	}
	if (n->variant == ZUI_LINK) {
		/* a hyperlink: the accent's type, no frame; pressed it takes the text colour */
		struct zt_layout *K = zt_slot_get(&n->t[0], n->text, ZT_REGULAR, 16, w, 1);
		zt_draw(K, x + (w - K->w) / 2, y + (h - K->line_h) / 2, down ? pal.text : pal.accent_text);
		return;
	}
	uint32_t edge = n->variant == ZUI_DANGER ? pal.danger : pal.text;
	if (down) zg_rect(x, y, w, h, 0, edge);
	else if (n->variant == ZUI_PRIMARY) {
		zg_rect(x, y, w, h, 0, zc_rgb(app->accent));
		zg_border(x, y, w, h, 0, 2, edge);
	} else {
		zg_border(x, y, w, h, 0, 2, edge);
	}
	struct zt_layout *L = zt_slot_get(&n->t[0], n->text, ZT_STRONG, M.button_text, w - 16, 1);
	zt_draw(L, x + (w - L->w) / 2, y + (h - L->line_h) / 2, down ? 0x000000ff : pal.text);
}

/* ZrnMetroCore's zmc_toggle: a white frame, the accent (or dark grey) in it,
 * and a white block that sits right when on */
static void toggle(zui_app *app, zui_node *n, float x, float y, float k) {
	(void)n;
	struct zui_palette pal = app->style->palette(app);
	zg_border(x, y, 54, 24, 0, 2, pal.text);
	zg_rect(x + 4, y + 4, 46, 16, 0, zc_mix(pal.track, zc_rgb(app->accent), k));
	zg_rect(x + 1 + k * 40, y - 1, 12, 26, 0, pal.text);
}

/* Pivot: the title in front, the next ones grey after it, going round both ways;
 * the titles move by a whole title as the tab under them moves by a screen */
static void tabs(zui_app *app, zui_node *n, float pos) {
	struct zui_palette pal = app->style->palette(app);
	int count = zui_tab_count(n), cur = zui_pivot_of(n)->cur;
	float frac = pos - (float)cur;
	if (!(app->page && app->page->bg)) zg_rect(0, 24, app->width, M.title_h - 24, 0, pal.bg);
	if (count <= 0) return;	/* a pivot with no items yet: nothing to name */
	struct zt_layout *L[count];
	for (int k = 0; k < count; k++) {
		zui_node *t = zui_tab_at(n, k);
		L[k] = zt_slot_get(&t->title_slot[0], zui_tab_title(t), ZT_LIGHT, M.title, 0, 0);
	}
	/* the pivot's own small title takes the top line, the item titles go under it */
	const char *small = zui_panel_text(zui_page_of(n), ZK_TITLE);
	float top = small && *small ? M.top + 8 : M.top;	/* as a page's TitlePanel: 32 of status bar and 24 down */
	float at = M.margin - 3;
	if (frac > 0) at -= frac * (L[cur]->w + 16);
	else if (frac < 0) at += -frac * (L[(cur + count - 1) % count]->w + 16);
	/* one behind, off to the left, when going back */
	if (frac < 0) {
		int p = (cur + count - 1) % count;
		zt_draw(L[p], at - L[p]->w - 16, top, pal.text3);
	}
	for (int k = 0; k < 8 && at < app->width; k++) {
		int i = (cur + k) % count;
		float near = k == 0 ? 1 - fabsf(frac) : (k == 1 && frac > 0 ? frac : 0);
		zt_draw(L[i], at, top, zc_mix(pal.text3, pal.text, near));
		at += L[i]->w + 16;
	}
}

/* Messaging's: theirs in the accent with a tail at the top left, yours a darker
 * shade with one at the bottom right */
static void tail(zui_app *app, zui_node *n, uint32_t c) {
	/* 0.5 points into the bubble, so no seam shows between them */
	if (n->mine) zg_wedge(n->x + n->w - 12, n->y + n->h - 0.5f, 12, 8.5f, true, false, c);
	else zg_wedge(n->x, n->y, 12, 8.5f, false, true, c);
	(void)app;
}
static float lin(int v) { float c = v / 255.0f; return c <= 0.04045f ? c / 12.92f : powf((c + 0.055f) / 1.055f, 2.4f); }
/* the accent, darkened only as far as white type on it needs: 4.5 to 1 */
static uint32_t legible(uint32_t c) {
	for (int i = 0; i < 20; i++) {
		float l = 0.2126f * lin((c >> 24) & 255) + 0.7152f * lin((c >> 16) & 255) + 0.0722f * lin((c >> 8) & 255);
		if (1.05f / (l + 0.05f) >= 4.5f) break;
		c = zc_mix(c, 0x000000ff, 0.05f);
	}
	return c;
}
static void bubble(zui_app *app, zui_node *n) {
	uint32_t a = legible(zc_rgb(app->accent)), c = n->mine ? zc_mix(a, 0x000000ff, 0.5f) : a;
	tail(app, n, c);
	zg_rect(n->x, n->y + 8, n->w, n->h - 8, 0, c);
}
static void dot_bubble(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	uint32_t c = n->mine ? pal.text : zc_rgb(app->accent);
	tail(app, n, c);
	zg_border(n->x, n->y + 8, n->w, n->h - 8, 0, 1.2f, c);
}

static void foot(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	zg_rect(0, n->y - 8, app->width, app->height - n->y + 8, 0, pal.bg);
}

/* a row with a tile: Messaging's thread row */
/* the app list's row, as ztmetro draws it: 45 pt square, title 22 pt 13 pt after it */
static void applist_row(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	const float s = 45;
	float x = n->x, y = n->y, left = x + s + 13, right = x + n->w;
	zui_draw_leading(app, n, x, y, s, 0, 0xffffffff);
	if (n->icon && !n->pix && !n->monogram) zui_icon(n->icon, x + (s - 26) / 2, y + (s - 26) / 2, 26, 2, 0xffffffff);
	bool sub = n->subtitle && *n->subtitle;
	struct zt_layout *T = zt_slot_get(&n->t[0], n->text, ZT_LIGHT, 22, right - left, 1);
	zt_draw(T, left, sub ? y - 3 : y + (s - T->line_h) / 2, pal.text);
	if (sub) {
		struct zt_layout *S = zt_slot_get(&n->t[1], n->subtitle, ZT_REGULAR, 13, right - left, 1);
		zt_draw(S, left, y + 24, pal.text2);
	}
}

static void thread_row(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	float x = n->x, y = n->y, w = n->w, left = x + 56 + 12, right = x + w;
	zui_draw_leading(app, n, x, y, 56, 0, 0xffffffff);
	float when = 0;
	if (n->detail && *n->detail) {
		struct zt_layout *D = zt_slot_get(&n->t[3], n->detail, ZT_REGULAR, 12.5f, 0, 0);
		when = D->w;
		zt_draw(D, right - when, y + 8, pal.text2);
	}
	struct zt_layout *T = zt_slot_get(&n->t[0], n->text, n->emphasis ? ZT_STRONG : ZT_LIGHT, 24, right - left - when - 10, 1);
	zt_draw(T, left, y - 3, n->emphasis ? pal.accent_text : pal.text);
	float line_right = right;
	if (n->badge) {
		struct zt_layout *B = zt_slot_get(&n->t[4], n->badge, ZT_STRONG, 13, 0, 0);
		float bw = B->w + 10 < 22 ? 22 : B->w + 10;
		zg_rect(right - bw, y + 31, bw, 20, 0, zc_rgb(app->accent));
		zt_draw(B, right - bw + (bw - B->w) / 2, y + 31 + (20 - B->line_h) / 2, 0xffffffff);
		line_right -= bw + 8;
	}
	if (n->subtitle && *n->subtitle) {
		struct zt_layout *S = zt_slot_get(&n->t[1], n->subtitle, ZT_REGULAR, 14, line_right - left, 1);
		zt_draw(S, left, y + 30, n->emphasis ? pal.text : pal.text2);
	}
}

static void item(zui_app *app, zui_node *n, float press) {
	(void)press;
	struct zui_palette pal = app->style->palette(app);
	float x = n->x, y = n->y, w = n->w, h = n->h, right = x + w;
	if (n->group && n->monogram) {  /* the app list's letter: 45 pt outline, as ztmetro draws it */
		const float s = 45, t = 1.5f;
		uint32_t c = zc_rgb(app->accent);
		zg_rect(x, y, s, t, 0, c);
		zg_rect(x, y + s - t, s, t, 0, c);
		zg_rect(x, y, t, s, 0, c);
		zg_rect(x + s - t, y, t, s, 0, c);
		struct zt_layout *G = zt_slot_get(&n->t[2], n->monogram, ZT_LIGHT, 27, 0, 0);
		zt_draw(G, x + 4, y + 7 - (G->line_h - 27) / 2, c);
		return;
	}
	if (n->applist && (n->pix || n->monogram || n->icon)) {
		applist_row(app, n);
		return;
	}
	if (n->pix || n->monogram) {
		thread_row(app, n);
		return;
	}
	if (n->icon) {
		zui_icon(n->icon, x, y + (h - 26) / 2, 26, 2, pal.text);
		x += 40;
	}
	if (n->kind == ZK_SWITCH) {
		float k = zanim_value(&n->knob, zui_now(), NULL);
		toggle(app, n, right - 54, y + 10, k);
		right -= 54 + 12;
	}
	const char *line2 = NULL;
	uint32_t c2 = pal.text2;
	float s2 = M.item_sub;
	if (n->kind == ZK_ITEM && n->detail && *n->detail) {
		line2 = n->detail;	/* a choice: its value in the accent, as zmc_choice */
		c2 = pal.accent_text;
		s2 = 15;
	} else if (n->subtitle && *n->subtitle) {
		line2 = n->subtitle;
	} else if (n->kind == ZK_SWITCH) {
		line2 = n->detail && *n->detail ? n->detail : n->on ? "On" : "Off";
	}
	struct zt_layout *T = zt_slot_get(&n->t[0], n->text, ZT_LIGHT, M.item_title, right - x, 1);
	float ty = line2 ? y + 6 : y + (h - T->line_h) / 2;
	zt_draw(T, x, ty, pal.text);
	if (line2) {
		struct zt_layout *S = zt_slot_get(&n->t[1], line2, ZT_REGULAR, s2, right - x, 1);
		zt_draw(S, x, ty + 30, c2);
	}
}

static void slider(zui_app *app, zui_node *n) {
	struct zui_palette pal = app->style->palette(app);
	float x = n->x, w = n->w, y = n->y;
	char pct[16];
	snprintf(pct, sizeof(pct), "%d%%", (int)lround(n->value * 100));
	struct zt_layout *P = zt_slot_get(&n->t[2], pct, ZT_STRONG, 14, 0, 0);
	struct zt_layout *T = zt_slot_get(&n->t[0], n->text, ZT_REGULAR, 13, w - 50, 1);
	zt_draw(T, x, y, pal.text2);
	zt_draw(P, x + w - P->w, y, pal.text);
	float ty = y + 30, fill = w * (float)n->value;
	zg_rect(x, ty, w, 4, 0, 0x383838ff);
	zg_rect(x, ty, fill, 4, 0, zc_rgb(app->accent));
	zg_rect(x + fill - 5, ty - 7, 10, 18, 0, pal.text);
}

/* five dots cross, quick at the edges and slow through the middle */
static void dots(zui_app *app, float y, float x0, float w, uint32_t since, uint32_t now) {
	uint32_t acc = zc_rgb(app->accent);
	for (int i = 0; i < 5; i++) {
		float p = (float)((now - since) % 2600) / 2000.0f - i * 0.075f;
		if (p < 0 || p > 1) continue;
		float c = p - 0.5f;
		float at = x0 + w * (0.5f + 4 * c * c * c);
		zg_rect(at - 2, y, 4, 4, 0, acc);
	}
}

static void progress(zui_app *app, zui_node *n, uint32_t now) {
	float x = n->x, w = n->w, y = n->y + (n->h - 4) / 2;
	if (n->value < 0) {
		dots(app, y, x, w, 0, now);
		return;
	}
	zg_rect(x, y, w, 4, 0, 0x383838ff);
	zg_rect(x, y, w * (float)n->value, 4, 0, zc_rgb(app->accent));
}

/* ZrnMetroCore's zmc_textbox: a caption, then a box, white while typed into */
static void field(zui_app *app, zui_node *n, uint32_t now) {
	(void)now;
	struct zui_palette pal = app->style->palette(app);
	float x = n->x, w = n->w, y = n->y;
	bool dotted = app->style == &zui_style_dotmatrix;
	float by = n->inline_hint ? y : y + 22, h = M.field_h;
	if (!n->inline_hint) {
		struct zt_layout *C = zt_slot_get(&n->t[1], n->subtitle ? n->subtitle : "", ZT_REGULAR, 13, w, 1);
		zt_draw(C, x, y, pal.text2);
	}
	/* Metro's box is pale with dark text; a pale fill of dots would drown the text */
	uint32_t ink = dotted ? pal.text : 0x000000ff;
	if (dotted) zg_border(x, by, w, h, 0, 1.2f, pal.text);
	else zg_rect(x, by, w, h, 0, n->focused ? 0xffffffff : 0xcfcfcfff);
	if (n->inline_hint && (!n->text || !*n->text) && !n->focused) {
		struct zt_layout *P = zt_slot_get(&n->t[1], n->subtitle ? n->subtitle : "", ZT_REGULAR, 16, w - 20, 1);
		zt_draw(P, x + 10, by + (h - P->line_h) / 2, dotted ? pal.text2 : 0x595959ff);
	}
	if (zui_field_of(n)->multiline) {
		h = n->h;
		zg_rect(x, by, w, h, 0, n->focused ? 0xffffffff : 0xcfcfcfff);
		if (dotted) { zg_rect(x, by, w, h, 0, 0x000000ff); zg_border(x, by, w, h, 0, 1.2f, pal.text); }
		/* the tail, as WhatsApp's box has at its bottom right */
		for (int i = 0; i < 12; i += 2)
			zg_rect(x + w - 12 + i, by + h, 2, (12 - i) * 0.67f, 0, dotted ? pal.text : n->focused ? 0xffffffff : 0xcfcfcfff);
		char *t = NULL;
		const char *txt = n->text ? n->text : "";
		if (n->focused) {
			size_t tl = strlen(txt), at = zui_field_of(n)->caret < 0 || (size_t)zui_field_of(n)->caret > tl ? tl : (size_t)zui_field_of(n)->caret;
			if (asprintf(&t, "%.*s|%s", (int)at, txt, txt + at) > 0) txt = t;
		}
		if (*txt) {
			struct zt_layout *T = zt_slot_get(&n->t[0], txt, ZT_REGULAR, 16, w - 20, 0);
			float over = T->h + 18 - h;
			if (over > 0) zg_clip(x, by + 2, w, h - 4);
			if (zui_field_of(n)->sel_b > zui_field_of(n)->sel_a && n->text) {
				/* what is selected, under the text */
				float sx, sy, ex, ey, lh;
				zt_caret_at(n->text, zui_field_of(n)->sel_a, w - 20, &sx, &sy, &lh);
				zt_caret_at(n->text, zui_field_of(n)->sel_b, w - 20, &ex, &ey, &lh);
				float off = over > 0 ? over : 0;
				if (sy == ey) zg_rect(x + 10 + sx, by + 9 + sy - off, ex - sx, lh, 0, zc_alpha(zc_rgb(app->accent), 0.45f));
				else {
					zg_rect(x + 10 + sx, by + 9 + sy - off, w - 20 - sx, lh, 0, zc_alpha(zc_rgb(app->accent), 0.45f));
					for (float l = sy + lh; l < ey; l += lh) zg_rect(x + 10, by + 9 + l - off, w - 20, lh, 0, zc_alpha(zc_rgb(app->accent), 0.45f));
					zg_rect(x + 10, by + 9 + ey - off, ex, lh, 0, zc_alpha(zc_rgb(app->accent), 0.45f));
				}
			}
			zt_draw(T, x + 10, by + 9 - (over > 0 ? over : 0), ink);
			if (over > 0) zg_noclip();
		} else {
			struct zt_layout *P = zt_slot_get(&n->t[1], n->subtitle ? n->subtitle : "", ZT_REGULAR, 16, w - 20, 1);
			zt_draw(P, x + 10, by + (h - P->line_h) / 2, dotted ? pal.text2 : 0x595959ff);
		}
		free(t);
		return;
	}
	float avail = w - 16, end = 0;
	char *dots_text = NULL;
	const char *shown = n->text;
	if (zui_field_of(n)->password && n->text && *n->text) {
		/* one bullet for each character, not each byte */
		size_t chars = 0;
		for (const char *c = n->text; *c; c++) chars += ((unsigned char)*c & 0xc0) != 0x80;
		const char *bullet = dotted ? "*" : "\xe2\x80\xa2";
		size_t bl = strlen(bullet);
		dots_text = malloc(chars * bl + 1);
		for (size_t i = 0; i < chars; i++) memcpy(dots_text + i * bl, bullet, bl);
		dots_text[chars * bl] = 0;
		shown = dots_text;
	}
	if (shown && *shown) {
		struct zt_layout *T = zt_slot_get(&n->t[0], shown, ZT_REGULAR, 17, n->focused ? 0 : avail, n->focused ? 0 : 1);
		float shift = n->focused && T->w > avail - 4 ? T->w - (avail - 4) : 0;
		zt_draw_clipped(T, x + 8 - shift, by + (h - T->line_h) / 2, ink, shift, shift + avail);
		end = T->w - shift;
	}
	if (n->focused) zg_rect(x + 8 + end + 1, by + 8, 1.5f, h - 16, 0, ink);
	free(dots_text);
}

static void busy(zui_app *app, uint32_t now) {
	dots(app, 4, 0, app->width, app->busy_since, now);
}

static void toast(zui_app *app, const char *text, float shown) {
	struct zg_xf xf;
	zg_xf_identity(&xf);
	zg_xf_translate(&xf, 0, -(1 - shown) * 60, 0);
	zg_set_xf(&xf);
	zg_rect(0, 0, app->width, 60, 0, zc_rgb(app->accent));
	struct zt_layout *L = zt_layout_make(text, ZT_STRONG, 15, app->width - 2 * M.margin, 1);
	zt_draw(L, M.margin, 30 - L->line_h / 2 + 6, 0xffffffff);
	zt_layout_free(L);
	zg_set_xf(NULL);
}

/* Windows Phone's tilt: what is under the finger leans toward it */
static void press_xf(zui_app *app, zui_node *n, float amount, float fx, float fy, struct zg_xf *xf) {
	(void)app;
	if (n->kind == ZK_BUTTON || n->kind == ZK_SLIDER || n->kind == ZK_FIELD || n->kind == ZK_PICKER) return;
	float cx = n->x + n->w / 2, cy = n->y + n->h / 2;
	fx = fx < -1 ? -1 : fx > 1 ? 1 : fx;
	fy = fy < -1 ? -1 : fy > 1 ? 1 : fy;
	/* its far end never rises more than TILT_LIFT points */
	float cap_y = n->w > 0 ? asinf(fminf(1, TILT_LIFT / (n->w / 2))) * 180 / (float)M_PI : TILT_DEG;
	float cap_x = n->h > 0 ? asinf(fminf(1, TILT_LIFT / (n->h / 2))) * 180 / (float)M_PI : TILT_DEG;
	float dy = fx * fminf(TILT_DEG, cap_y) * amount, dx = fy * fminf(TILT_DEG, cap_x) * amount;
	zg_xf_rotate_y(xf, dy, cx);
	zg_xf_rotate_x(xf, dx, cy);
	zg_xf_scale_about(xf, 1 - PRESS_SINK * amount, cx, cy);
}

static float clamp01(float v) { return v < 0 ? 0 : v > 1 ? 1 : v; }

/* The turnstile, as ZrnMetroCore times it from the Lumia: the old page's
 * rows turn away round the left edge one after another (128 ms each, 33 ms
 * apart; toward the viewer going forward, away going back), a moment of
 * black, then the new rows swing in from behind the screen (180 ms each).
 * Going back both go from the bottom up. */
#define OUT_MS 128.0f
#define IN_MS 180.0f
#define STAGGER 28.0f
#define GAP_AT 330.0f
static void page_xf(zui_app *app, float t, int dir, bool incoming, int row, int rows,
		struct zg_xf *xf, float *alpha) {
	float ms = t * app->style->transition_ms;
	int r = row < 0 ? 0 : row + 1;	/* the header goes first */
	if (dir < 0) r = row < 0 ? rows + 1 : rows - row;
	if (r > 7) r = 7;
	if (!incoming) {
		float u = clamp01((ms - r * STAGGER) / OUT_MS);
		zg_xf_rotate_y(xf, (dir > 0 ? -50.0f : 50.0f) * zui_ease_in(u), 0);
		*alpha = u >= 1 ? 0 : 1 - u * u;
	} else {
		float u = clamp01((ms - GAP_AT - r * STAGGER) / IN_MS);
		zg_xf_rotate_y(xf, 80.0f * (1 - zui_ease_out(u)), 0);
		*alpha = u <= 0 ? 0 : clamp01(u * 3);
	}
}

const struct zui_style zui_style_metro = {
	.name = "metro",
	.fonts = {
		[ZT_REGULAR] = "Selawik-Regular.ttf;Segoe UI",
		[ZT_MEDIUM] = "Selawik-Semibold.ttf;Segoe UI Semibold",
		[ZT_STRONG] = "Selawik-Semibold.ttf;Segoe UI Semibold",
		[ZT_LIGHT] = "Selawik-Semilight.ttf;Segoe UI Semilight",
	},
	.line_spacing = 1.25f,
	.transition_ms = 720,
	.turn_in_ms = 330,
	.palette = metro_palette,
	.m = {
		.margin = 24, .top = 56, .gap = 12, .card_r = 0, .card_pad = 0, .section_gap = 10,
		.text = 16, .heading = 25.5f, .caption = 13, .label = 13, .title = 48, .bar_title = 0,
		.item_h = 52, .item_h2 = 66, .item_title = 21, .item_sub = 13, .icon_tile = 0,
		.button_h = 40, .button_r = 0, .button_text = 14.5f,
		.field_h = 44, .field_r = 0,
		.bar_slot = 57, .bar_button = 44,
		.title_h = 152, .title_compact_h = 106, .large_title_h = 0,
	},
	.header = header,
	.page_top = page_top,
	.card = card,
	.divider = divider,
	.section_label = section_label,
	.button = button,
	.item = item,
	.toggle = toggle,
	.slider = slider,
	.progress = progress,
	.field = field,
	.busy = busy,
	.toast = toast,
	.tabs = tabs,
	.tabs_h = 0,
	.bubble = bubble,
	.bubble_pad = 10,
	.footer = foot,
	.press_xf = press_xf,
	.page_xf = page_xf,
	.rows_highlight = false,
	.flat_cards = true,
};

/* DotMatrix: the same widgets, the same places. The renderer draws every fill as
 * dots on one lattice (zg_set_dots) and text in the 3x5 dot type (zt_set_dot_type). */
static struct zui_palette dot_palette(const zui_app *app) {
	uint32_t main_c = zc_rgb(app->dot_main), acc = zc_rgb(app->accent);
	struct zui_palette p = {
		.bg = 0x000000ff, .surface = 0x000000ff, .surface2 = 0x000000ff,
		.text = main_c, .text2 = zc_mix(main_c, 0x000000ff, 0.4f), .text3 = zc_mix(main_c, 0x000000ff, 0.62f),
		.divider = 0x00000000, .track = zc_mix(main_c, 0x000000ff, 0.7f), .knob = main_c,
		.shadow = 0x00000000, .danger = acc, .scrim = 0x000000b3,
		.on_accent = main_c, .accent_text = acc, .bubble_caption = zc_mix(main_c, 0x000000ff, 0.4f),
	};
	return p;
}

const struct zui_style zui_style_dotmatrix = {
	.name = "dotmatrix",
	.fonts = {
		[ZT_REGULAR] = "Selawik-Regular.ttf;Segoe UI",
		[ZT_MEDIUM] = "Selawik-Semibold.ttf;Segoe UI Semibold",
		[ZT_STRONG] = "Selawik-Semibold.ttf;Segoe UI Semibold",
		[ZT_LIGHT] = "Selawik-Semilight.ttf;Segoe UI Semilight",
	},
	.line_spacing = 1.25f,
	.transition_ms = 720,
	.palette = dot_palette,
	.m = {
		.margin = 24, .top = 56, .gap = 12, .card_r = 0, .card_pad = 0, .section_gap = 10,
		.text = 16, .heading = 25.5f, .caption = 13, .label = 13, .title = 48, .bar_title = 0,
		.item_h = 52, .item_h2 = 66, .item_title = 21, .item_sub = 13, .icon_tile = 0,
		.button_h = 40, .button_r = 0, .button_text = 14.5f,
		.field_h = 44, .field_r = 0,
		.title_h = 152, .title_compact_h = 106, .large_title_h = 0,
	},
	.header = header,
	.page_top = page_top,
	.card = card,
	.divider = divider,
	.section_label = section_label,
	.button = button,
	.item = item,
	.toggle = toggle,
	.slider = slider,
	.progress = progress,
	.field = field,
	.busy = busy,
	.toast = toast,
	.tabs = tabs,
	.tabs_h = 0,
	.bubble = dot_bubble,
	.bubble_pad = 10,
	.footer = foot,
	.press_xf = press_xf,
	.page_xf = page_xf,
	.rows_highlight = false,
	.flat_cards = true,
};
