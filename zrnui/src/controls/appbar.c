/**
 * zrnui - controls/appbar.c
 * IconElement (SymbolIcon, FontIcon, PathIcon, BitmapIcon), AppBar, CommandBar, AppBarButton,
 * AppBarToggleButton and AppBarSeparator, after the Windows Phone 8.1 templates (generic.xaml of
 * the phone SDK): an app bar button is a 38.5 px circle with a 2.5 px outline, its icon inside it
 * and its label under it (9.5 px type); the numbers are used as they are.
 *
 * The phone template of CommandBar is only a stub (the shell draws the real bar), so what the
 * XAML does not say is this library's own: the three dots at the right edge, the heights of
 * a Minimal bar, the rows of the secondary commands. They are named below.
 */

#include "../internal.h"

#define ICON(e) ZST_AS(zrn_icon_t, e)
#define BAR(e) ZST_AS(zrn_app_bar_t, e)
#define CBAR(e) ZST_AS(zrn_command_bar_t, e)

/* AppBarThemeMinHeight 54.5, AppBarTopBorderThemeThickness 0,0,0,1.5 / AppBarBottomBorderThemeThickness 0,1.5,0,0 and the
 * theme padding of the other side: the closed bar of a page is 1.5 + 54.5 + 1.5 = 57.5, the MaxHeight of the CommandBar style. */
static constexpr float BAR_ROW = 54.5f;
static constexpr float BAR_BORDER = 1.5f;
static constexpr float BAR_COMPACT_HEIGHT = 57.5f;
/* The height of a Minimal bar: not in the XAML, chosen. */
static constexpr float BAR_MINIMAL_HEIGHT = 35.0f;
/* The opening of the bar: chosen. */
static constexpr float BAR_OPEN_MS = 167.0f;
/* The three dots that open the bar: not in the XAML, chosen. */
static constexpr float ELLIPSIS_WIDTH = 30.0f;
static constexpr float ELLIPSIS_DOT = 5.0f;
static constexpr float ELLIPSIS_STEP = 8.5f;

/* The AppBarButton template: the circle, its margins, the label and its width. */
static constexpr float ITEM_CIRCLE = 38.5f;
static constexpr float ITEM_OUTLINE = 2.5f;
static constexpr float ITEM_TOP = 12.0f;
static constexpr float ITEM_TOGGLE_TOP = 11.0f;
static constexpr float ITEM_GAP = 4.0f;
static constexpr float ITEM_FULL_WIDTH = 80.0f;
static constexpr float ITEM_COMPACT_WIDTH = 48.0f;
static constexpr float ITEM_LABEL_WIDTH = 70.5f;
static constexpr float ITEM_LABEL_FONT = 9.5f;
static constexpr float ITEM_LABEL_BOTTOM = 10.5f;
static constexpr float ICON_SIZE = 20.0f;
/* The rows of the secondary commands take the measures of a MenuFlyoutItem: padding 0,8.5, ExtraLarge SemiLight type. */
static constexpr float ROW_PAD_V = 8.5f;
static constexpr float ROW_LEFT = 19.0f;
static constexpr float ROW_TOP = 13.5f;
static constexpr float ROW_FONT = 25.5f;
static constexpr float SEPARATOR_WIDTH = 12.0f;


/* ============================================================================
 *  IconElement
 * ============================================================================ */

static void icon_destroy(zrn_element_t* const e) {
	auto i = ICON(e);
	free(i->glyph);
	free(i->path);
	free(i->path_runs);
	if (i->image) zrn_image_free(i->image);
}

static void icon_init(zrn_element_t* const e) {
	auto i = ICON(e);
	i->font_size = ICON_SIZE;
	i->family = Z_FONT_NORMAL;
	e->hit_visible = 0;
}

static ZColor icon_color(zrn_element_t* const e) {
	auto i = ICON(e);
	return i->has_foreground ? i->foreground : zrn_inherited_foreground(e);
}

static ZSize icon_measure(zrn_element_t* const e, const ZSize available) {
	auto i = ICON(e);
	ZST_UNUSED(available);
	if (e->cls == &zrn_class_font_icon) {
		const zrn_font_t font = { i->family, i->font_size, 0 };
		const char* g = i->glyph ? i->glyph : "";
		return zrn_size(zrn_text_width(font, g, (int)strlen(g)), zrn_font_line_height(font));
	}
	return zrn_size(ICON_SIZE, ICON_SIZE);
}

zrn_class_t zrn_class_icon_element = {
	.base = &zrn_class_element, .name = "IconElement", .id = Z_CLASS_ICON_ELEMENT, .size = sizeof(zrn_icon_t), .abstract_class = 1,
	.init = icon_init, .destroy = icon_destroy, .measure = icon_measure
};


/* ---- the vector shapes of the symbols, in a box of 20 x 20 units ---- */

typedef struct shape {
	zrn_painter_t* p;
	float ox, oy, s;
	ZColor c;
} shape_t;

static float sx(const shape_t* h, const float v) { return h->ox + v * h->s; }
static float sy(const shape_t* h, const float v) { return h->oy + v * h->s; }

static void sh_line(const shape_t* const h, const float x0, const float y0, const float x1, const float y1) {
	const float t = zrn_maxf(1.0f, 2.0f * h->s);
	zrn_draw_line(h->p, sx(h, x0), sy(h, y0), sx(h, x1), sy(h, y1), t, h->c);
}

/* A run of lines with round joints. */
static void sh_poly(shape_t* const h, const float* const xy, const int n, const int closed) {
	const float t = zrn_maxf(1.0f, 2.0f * h->s);
	int i;
	for (i = 0; i + 1 < n; i++) sh_line(h, xy[2 * i], xy[2 * i + 1], xy[2 * i + 2], xy[2 * i + 3]);
	if (closed && n > 2) sh_line(h, xy[2 * n - 2], xy[2 * n - 1], xy[0], xy[1]);
	for (i = 0; i < n; i++) zrn_fill_ellipse(h->p, zrn_rect(sx(h, xy[2 * i]) - t * 0.5f, sy(h, xy[2 * i + 1]) - t * 0.5f, t, t), h->c);
}

static void sh_fill(const shape_t* h, const float* xy, int n) {
	float pts[48];
	int i;
	if (n > 24) n = 24;
	for (i = 0; i < n; i++) {
		pts[2 * i] = sx(h, xy[2 * i]);
		pts[2 * i + 1] = sy(h, xy[2 * i + 1]);
	}
	zrn_fill_polygon(h->p, pts, n, h->c);
}

static void sh_rect(const shape_t* const h, const float x, const float y, const float w, const float rh) {
	zrn_fill_rect(h->p, zrn_rect(sx(h, x), sy(h, y), w * h->s, rh * h->s), h->c);
}

static void sh_dot(const shape_t* const h, const float x, const float y, const float r) {
	zrn_fill_ellipse(h->p, zrn_rect(sx(h, x - r), sy(h, y - r), 2 * r * h->s, 2 * r * h->s), h->c);
}

static void sh_ring(const shape_t* const h, const float x, const float y, const float r, const float thickness) {
	zrn_stroke_ellipse(h->p, zrn_rect(sx(h, x - r), sy(h, y - r), 2 * r * h->s, 2 * r * h->s), zrn_maxf(1.0f, thickness * h->s), h->c);
}

static void sh_star(shape_t* const h, const int outline) {
	float xy[20];
	int i;
	for (i = 0; i < 10; i++) {
		const float a = -ZST_PI * 0.5f + (float)i * ZST_PI / 5.0f, r = (i & 1) ? 4.2f : 9.0f;
		xy[2 * i] = 10.0f + r * cosf(a);
		xy[2 * i + 1] = 10.5f + r * sinf(a);
	}
	if (outline) sh_poly(h, xy, 10, 1);
	else sh_fill(h, xy, 10);
}

static void draw_symbol(zrn_painter_t* const p, const ZSymbol symbol, const ZRect box, const ZColor c) {
	shape_t h;
	const float side = zrn_minf(box.width, box.height);
	h.p = p;
	h.s = side / ICON_SIZE;
	h.ox = box.x + (box.width - side) * 0.5f;
	h.oy = box.y + (box.height - side) * 0.5f;
	h.c = c;
	switch (symbol) {
		case Z_SYMBOL_ACCEPT: { static const float v[] = { 3, 10.5f, 8, 15.5f, 17, 5 }; sh_poly(&h, v, 3, 0); break; }
		case Z_SYMBOL_ADD: sh_line(&h, 10, 3, 10, 17); sh_line(&h, 3, 10, 17, 10); break;
		case Z_SYMBOL_REMOVE: sh_line(&h, 3, 10, 17, 10); break;
		case Z_SYMBOL_CANCEL: case Z_SYMBOL_CLEAR: sh_line(&h, 4, 4, 16, 16); sh_line(&h, 16, 4, 4, 16); break;
		case Z_SYMBOL_BACK: { static const float v[] = { 10, 4, 4, 10, 10, 16 }; sh_line(&h, 17, 10, 4, 10); sh_poly(&h, v, 3, 0); break; }
		case Z_SYMBOL_FORWARD: { static const float v[] = { 10, 4, 16, 10, 10, 16 }; sh_line(&h, 3, 10, 16, 10); sh_poly(&h, v, 3, 0); break; }
		case Z_SYMBOL_UP: { static const float v[] = { 4, 10, 10, 4, 16, 10 }; sh_line(&h, 10, 17, 10, 4); sh_poly(&h, v, 3, 0); break; }
		case Z_SYMBOL_DELETE: {
			static const float body[] = { 5, 7, 6, 17, 14, 17, 15, 7 };
			sh_line(&h, 3, 5, 17, 5);
			sh_line(&h, 8, 3, 12, 3);
			sh_poly(&h, body, 4, 0);
			sh_line(&h, 8.5f, 9, 8.5f, 15);
			sh_line(&h, 11.5f, 9, 11.5f, 15);
			break;
		}
		case Z_SYMBOL_EDIT: {
			static const float pen[] = { 13, 3, 17, 7, 8, 16, 3, 17, 4, 12 };
			sh_fill(&h, pen, 5);
			break;
		}
		case Z_SYMBOL_HOME: {
			static const float roof[] = { 2, 10, 10, 3, 18, 10 }, body[] = { 4.5f, 8.5f, 4.5f, 17, 15.5f, 17, 15.5f, 8.5f }, door[] = { 8, 17, 8, 12, 12, 12, 12, 17 };
			sh_poly(&h, roof, 3, 0);
			sh_poly(&h, body, 4, 0);
			sh_poly(&h, door, 4, 0);
			break;
		}
		case Z_SYMBOL_MAIL: case Z_SYMBOL_MAIL_FILLED: {
			static const float flap[] = { 3, 5, 10, 11, 17, 5 }, frame[] = { 2.5f, 4, 17.5f, 4, 17.5f, 16, 2.5f, 16 };
			sh_poly(&h, frame, 4, 1);
			sh_poly(&h, flap, 3, 0);
			break;
		}
		case Z_SYMBOL_MORE: sh_dot(&h, 4, 10, 1.8f); sh_dot(&h, 10, 10, 1.8f); sh_dot(&h, 16, 10, 1.8f); break;
		case Z_SYMBOL_REFRESH: case Z_SYMBOL_SYNC: {
			float xy[20];
			float tip[6];
			int i, n = 9;
			for (i = 0; i < n; i++) {
				const float a = (-50.0f + (float)i * 35.0f) * ZST_PI / 180.0f;
				xy[2 * i] = 10.0f + 6.5f * cosf(a);
				xy[2 * i + 1] = 10.0f + 6.5f * sinf(a);
			}
			sh_poly(&h, xy, n, 0);
			tip[0] = xy[2 * n - 2] + 3.0f; tip[1] = xy[2 * n - 1] - 4.5f;
			tip[2] = xy[2 * n - 2] - 3.5f; tip[3] = xy[2 * n - 1] - 1.0f;
			tip[4] = xy[2 * n - 2] + 1.0f; tip[5] = xy[2 * n - 1] + 3.0f;
			sh_fill(&h, tip, 3);
			break;
		}
		case Z_SYMBOL_SAVE: case Z_SYMBOL_SAVE_LOCAL: {
			static const float frame[] = { 3, 3, 17, 3, 17, 17, 3, 17 }, lower[] = { 6.5f, 17, 6.5f, 11, 13.5f, 11, 13.5f, 17 };
			sh_poly(&h, frame, 4, 1);
			sh_rect(&h, 6, 3, 8, 5);
			sh_poly(&h, lower, 4, 0);
			break;
		}
		case Z_SYMBOL_FIND: sh_ring(&h, 8, 8, 5.5f, 2); sh_line(&h, 12, 12, 17, 17); break;
		case Z_SYMBOL_SETTING: {
			int i;
			sh_ring(&h, 10, 10, 6.0f, 2);
			sh_ring(&h, 10, 10, 2.5f, 2);
			for (i = 0; i < 8; i++) {
				const float a = (float)i * ZST_PI / 4.0f;
				sh_line(&h, 10.0f + 6.5f * cosf(a), 10.0f + 6.5f * sinf(a), 10.0f + 9.0f * cosf(a), 10.0f + 9.0f * sinf(a));
			}
			break;
		}
		case Z_SYMBOL_PIN: {
			static const float head[] = { 7, 3, 13, 3, 12, 9, 15, 12, 5, 12, 8, 9 };
			sh_fill(&h, head, 6);
			sh_line(&h, 10, 12, 10, 18);
			break;
		}
		case Z_SYMBOL_PLAY: { static const float v[] = { 5, 3, 16, 10, 5, 17 }; sh_fill(&h, v, 3); break; }
		case Z_SYMBOL_PAUSE: sh_rect(&h, 4.5f, 3, 4, 14); sh_rect(&h, 11.5f, 3, 4, 14); break;
		case Z_SYMBOL_STOP: sh_rect(&h, 4, 4, 12, 12); break;
		case Z_SYMBOL_PREVIOUS: { static const float v[] = { 16, 4, 7, 10, 16, 16 }; sh_rect(&h, 4, 4, 2.5f, 12); sh_fill(&h, v, 3); break; }
		case Z_SYMBOL_NEXT: { static const float v[] = { 4, 4, 13, 10, 4, 16 }; sh_rect(&h, 13.5f, 4, 2.5f, 12); sh_fill(&h, v, 3); break; }
		case Z_SYMBOL_DOWNLOAD: { static const float v[] = { 5, 8.5f, 10, 13.5f, 15, 8.5f }; sh_line(&h, 10, 3, 10, 13); sh_poly(&h, v, 3, 0); sh_line(&h, 3, 17, 17, 17); break; }
		case Z_SYMBOL_UPLOAD: { static const float v[] = { 5, 8, 10, 3, 15, 8 }; sh_line(&h, 10, 3, 10, 13.5f); sh_poly(&h, v, 3, 0); sh_line(&h, 3, 17, 17, 17); break; }
		case Z_SYMBOL_FAVORITE: case Z_SYMBOL_SOLID_STAR: sh_star(&h, 0); break;
		case Z_SYMBOL_OUTLINE_STAR: sh_star(&h, 1); break;
		case Z_SYMBOL_SEND: { static const float v[] = { 2, 9, 18, 2, 12, 18, 9, 12 }; sh_fill(&h, v, 4); break; }
		default:
			/* a symbol that has no drawing yet: an empty box, so that it is seen and not mistaken for nothing */
			zrn_stroke_rect(p, zrn_rect(h.ox + 4 * h.s, h.oy + 4 * h.s, 12 * h.s, 12 * h.s), Zthickness_uniform(zrn_maxf(1.0f, 2.0f * h.s)), c);
			break;
	}
}

static void symbol_icon_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	draw_symbol(p, ICON(e)->symbol, zrn_local_rect(e), icon_color(e));
}

static void symbol_icon_init(zrn_element_t* e) { ICON(e)->symbol = Z_SYMBOL_PLACEHOLDER; }

zrn_class_t zrn_class_symbol_icon = {
	.base = &zrn_class_icon_element, .name = "SymbolIcon", .id = Z_CLASS_SYMBOL_ICON, .size = sizeof(zrn_icon_t),
	.init = symbol_icon_init, .paint = symbol_icon_paint
};

static void font_icon_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	auto i = ICON(e);
	const zrn_font_t font = { i->family, i->font_size, 0 };
	const char* g = i->glyph ? i->glyph : "";
	const float w = zrn_text_width(font, g, (int)strlen(g)), h = zrn_font_line_height(font);
	zrn_draw_string(p, font, g, (int)strlen(g), (e->rect.width - w) * 0.5f, (e->rect.height - h) * 0.5f, icon_color(e));
}

zrn_class_t zrn_class_font_icon = {
	.base = &zrn_class_icon_element, .name = "FontIcon", .id = Z_CLASS_FONT_ICON, .size = sizeof(zrn_icon_t), .paint = font_icon_paint
};

static void path_icon_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	auto i = ICON(e);
	const float bw = i->path_bounds[2] - i->path_bounds[0], bh = i->path_bounds[3] - i->path_bounds[1];
	float s, ox, oy;
	int run, at = 0, k;
	if (!i->path || bw <= 0 || bh <= 0) return;
	/* Stretch is Uniform: the geometry fits the icon and is centred in it */
	s = zrn_minf(e->rect.width / bw, e->rect.height / bh);
	ox = (e->rect.width - bw * s) * 0.5f - i->path_bounds[0] * s;
	oy = (e->rect.height - bh * s) * 0.5f - i->path_bounds[1] * s;
	for (run = 0; run < i->path_run_count; run++) {
		float pts[256];
		int n = i->path_runs[run];
		if (n > 128) n = 128;
		for (k = 0; k < n; k++) {
			pts[2 * k] = ox + i->path[2 * (at + k)] * s;
			pts[2 * k + 1] = oy + i->path[2 * (at + k) + 1] * s;
		}
		if (n >= 3) zrn_fill_polygon(p, pts, n, icon_color(e));
		at += i->path_runs[run];
	}
}

zrn_class_t zrn_class_path_icon = {
	.base = &zrn_class_icon_element, .name = "PathIcon", .id = Z_CLASS_PATH_ICON, .size = sizeof(zrn_icon_t), .paint = path_icon_paint
};

static void bitmap_icon_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_image_t* image = ICON(e)->image;
	float s, w, h;
	if (!image || image->width <= 0 || image->height <= 0) return;
	s = zrn_minf(e->rect.width / (float)image->width, e->rect.height / (float)image->height);
	w = (float)image->width * s;
	h = (float)image->height * s;
	zrn_draw_image(p, image, zrn_rect((e->rect.width - w) * 0.5f, (e->rect.height - h) * 0.5f, w, h), zrn_rect(0, 0, (float)image->width, (float)image->height));
}

zrn_class_t zrn_class_bitmap_icon = {
	.base = &zrn_class_icon_element, .name = "BitmapIcon", .id = Z_CLASS_BITMAP_ICON, .size = sizeof(zrn_icon_t), .paint = bitmap_icon_paint
};

static zrn_icon_t* get_icon(const Zid id, const ZClass cls) { return ICON(zrn_get(id, cls)); }

static Zid make_icon(const ZClass cls) {
	const zrn_element_t* e = zrn_create(cls);
	return e ? e->id : Z_INVALID_ID;
}

void Zicon_element_set_foreground(const Zid id, const ZColor color) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_ICON_ELEMENT);
	if (!i) return;
	i->foreground = color;
	i->has_foreground = 1;
	zrn_changed_paint(&i->base);
}

Zid Zsymbol_icon_create(const ZSymbol symbol) {
	const Zid id = make_icon(Z_CLASS_SYMBOL_ICON);
	if (id != Z_INVALID_ID) ICON(zrn_lookup(id))->symbol = symbol;
	return id;
}

void Zsymbol_icon_set_symbol(const Zid id, const ZSymbol symbol) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_SYMBOL_ICON);
	if (!i) return;
	i->symbol = symbol;
	zrn_changed_paint(&i->base);
}

ZSymbol Zsymbol_icon_get_symbol(const Zid id) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_SYMBOL_ICON);
	return i ? i->symbol : Z_SYMBOL_PLACEHOLDER;
}

Zid Zfont_icon_create(const char* const glyph) {
	const Zid id = make_icon(Z_CLASS_FONT_ICON);
	if (id != Z_INVALID_ID) zrn_set_string(&ICON(zrn_lookup(id))->glyph, glyph ? glyph : "");
	return id;
}

void Zfont_icon_set_glyph(const Zid id, const char* const glyph) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_FONT_ICON);
	if (!i) return;
	zrn_set_string(&i->glyph, glyph ? glyph : "");
	zrn_changed_layout(&i->base);
}

const char* Zfont_icon_get_glyph(const Zid id) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_FONT_ICON);
	return i && i->glyph ? i->glyph : "";
}

void Zfont_icon_set_font_size(const Zid id, const float font_size) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_FONT_ICON);
	if (!i || font_size <= 0) return;
	i->font_size = font_size;
	zrn_changed_layout(&i->base);
}

void Zfont_icon_set_font_family(const Zid id, const ZFontFamily family) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_FONT_ICON);
	if (!i) return;
	i->family = family;
	zrn_changed_layout(&i->base);
}

/* The mini language of Path.Data, absolute and relative: M, L, H, V and Z. Every sub-path is a polygon. */
static const char* path_number(const char* s, float* out, int* ok) {
	char* end;
	while (*s == ' ' || *s == ',' || *s == '\t' || *s == '\n' || *s == '\r') s++;
	*out = strtof(s, &end);
	*ok = end != s;
	return end;
}

static int path_parse(zrn_icon_t* const i, const char* const data) {
	float* pts = nullptr;
	int* runs = nullptr;
	int count = 0, cap = 0, run_count = 0, run_start = 0, k;
	float cx = 0, cy = 0, sx0 = 0, sy0 = 0;
	char cmd = 0;
	const char* s = data;
	int fail = 0;
	float bounds[4] = { 1e30f, 1e30f, -1e30f, -1e30f };
#define ADD(px, py) do { \
	if (count == cap) { \
		int ncap = cap ? cap * 2 : 32; \
		float* np = (float*)realloc(pts, (size_t)ncap * 2 * sizeof *np); \
		if (!np) { fail = 1; break; } \
		pts = np; cap = ncap; \
	} \
	pts[2 * count] = (px); pts[2 * count + 1] = (py); count++; \
	cx = (px); cy = (py); \
} while (0)
#define END_RUN() do { \
	if (count > run_start) { \
		int* nr = (int*)realloc(runs, (size_t)(run_count + 1) * sizeof *nr); \
		if (!nr) { fail = 1; break; } \
		runs = nr; runs[run_count++] = count - run_start; run_start = count; \
	} \
} while (0)
	while (*s && !fail) {
		float a, b;
		int ok;
		while (*s == ' ' || *s == ',' || *s == '\t' || *s == '\n' || *s == '\r') s++;
		if (!*s) break;
		if ((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z')) cmd = *s++;
		else if (!cmd) { fail = 1; break; }
		if (cmd == 'Z' || cmd == 'z') {
			cx = sx0; cy = sy0;
			END_RUN();
			cmd = 0;
			continue;
		}
		if (cmd == 'H' || cmd == 'h') {
			s = path_number(s, &a, &ok);
			if (!ok) { fail = 1; break; }
			ADD(cmd == 'h' ? cx + a : a, cy);
		} else if (cmd == 'V' || cmd == 'v') {
			s = path_number(s, &a, &ok);
			if (!ok) { fail = 1; break; }
			ADD(cx, cmd == 'v' ? cy + a : a);
		} else if (cmd == 'M' || cmd == 'm' || cmd == 'L' || cmd == 'l') {
			const int relative = cmd == 'm' || cmd == 'l';
			s = path_number(s, &a, &ok);
			if (!ok) { fail = 1; break; }
			s = path_number(s, &b, &ok);
			if (!ok) { fail = 1; break; }
			if (cmd == 'M' || cmd == 'm') {
				END_RUN();
				ADD(relative ? cx + a : a, relative ? cy + b : b);
				sx0 = cx; sy0 = cy;
				cmd = relative ? 'l' : 'L';
			} else {
				ADD(relative ? cx + a : a, relative ? cy + b : b);
			}
		} else {
			fail = 1;
		}
	}
	if (!fail) END_RUN();
#undef ADD
#undef END_RUN
	if (fail || count == 0) {
		free(pts);
		free(runs);
		return false;
	}
	for (k = 0; k < count; k++) {
		bounds[0] = zrn_minf(bounds[0], pts[2 * k]);
		bounds[1] = zrn_minf(bounds[1], pts[2 * k + 1]);
		bounds[2] = zrn_maxf(bounds[2], pts[2 * k]);
		bounds[3] = zrn_maxf(bounds[3], pts[2 * k + 1]);
	}
	free(i->path);
	free(i->path_runs);
	i->path = pts;
	i->path_runs = runs;
	i->path_run_count = run_count;
	memcpy(i->path_bounds, bounds, sizeof bounds);
	return true;
}

int Zpath_icon_set_data(const Zid id, const char* const data) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_PATH_ICON);
	if (!i || !data) return false;
	if (!path_parse(i, data)) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "Path.Data is not M, L, H, V and Z commands: \"%s\"", data);
		return false;
	}
	zrn_changed_paint(&i->base);
	return true;
}

Zid Zpath_icon_create(const char* const data) {
	const Zid id = make_icon(Z_CLASS_PATH_ICON);
	if (id != Z_INVALID_ID && data && !Zpath_icon_set_data(id, data)) {
		Zelement_destroy(id);
		return Z_INVALID_ID;
	}
	return id;
}

int Zbitmap_icon_set_uri_source(const Zid id, const char* const path) {
	zrn_icon_t* i = get_icon(id, Z_CLASS_BITMAP_ICON);
	zrn_image_t* image;
	if (!i || !path) return false;
	image = zrn_image_load_png(path);
	if (!image) {
		zrn_error(Z_ERROR_IO, "cannot read the PNG \"%s\"", path);
		return false;
	}
	if (i->image) zrn_image_free(i->image);
	i->image = image;
	zrn_changed_paint(&i->base);
	return true;
}

Zid Zbitmap_icon_create(const char* const path) {
	const Zid id = make_icon(Z_CLASS_BITMAP_ICON);
	if (id != Z_INVALID_ID && path && !Zbitmap_icon_set_uri_source(id, path)) {
		Zelement_destroy(id);
		return Z_INVALID_ID;
	}
	return id;
}


/* ============================================================================
 *  AppBarButton and AppBarToggleButton
 * ============================================================================ */

static zrn_app_bar_item_t* item_of(zrn_element_t* const e) {
	if (zrn_is_a(e, Z_CLASS_APP_BAR_TOGGLE_BUTTON)) return &ZST_AS(zrn_app_bar_toggle_button_t, e)->item;
	return &ZST_AS(zrn_app_bar_button_t, e)->item;
}

static zrn_font_t label_font(zrn_element_t* const e) {
	const zrn_font_t f = { ZST_AS(zrn_control_t, e)->font_family, ITEM_LABEL_FONT, 0 };
	return f;
}

static zrn_font_t row_font(void) {
	const zrn_font_t f = { Z_FONT_SEMILIGHT, ROW_FONT, -15 };
	return f;
}

static float item_top(const zrn_element_t* e) { return zrn_is_a(e, Z_CLASS_APP_BAR_TOGGLE_BUTTON) ? ITEM_TOGGLE_TOP : ITEM_TOP; }

static float label_height(zrn_element_t* const e) {
	const zrn_app_bar_item_t* it = item_of(e);
	zrn_text_layout_t* l;
	float h;
	if (!it->label || !*it->label) return 0;
	l = zrn_text_layout_new(label_font(e), it->label, (int)strlen(it->label), ITEM_LABEL_WIDTH, 1, 0, 0);
	if (!l) return 0;
	h = l->height;
	zrn_text_layout_free(l);
	return h;
}

/* How tall the button is with its label shown or not. */
static float item_height(zrn_element_t* const e, const int compact) {
	const zrn_app_bar_item_t* it = item_of(e);
	if (it->overflow) return zrn_font_line_height(row_font()) + 2 * ROW_PAD_V;
	return item_top(e) + ITEM_CIRCLE + ITEM_GAP + (compact ? 0 : label_height(e) + ITEM_LABEL_BOTTOM);
}

static ZSize item_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_app_bar_item_t* it = item_of(e);
	if (ZST_AS(zrn_content_control_t, e)->content) zrn_measure(ZST_AS(zrn_content_control_t, e)->content, zrn_size(ITEM_CIRCLE, ITEM_CIRCLE));
	if (it->overflow) return zrn_size(available.width < Z_INFINITE * 0.5f ? available.width : 480.0f, item_height(e, 0));
	return zrn_size(it->is_compact ? ITEM_COMPACT_WIDTH : ITEM_FULL_WIDTH, item_height(e, it->is_compact));
}

static ZSize item_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	if (cc->content && !item_of(e)->overflow) {
		/* the icon is as big as it asks for, in the middle of the circle */
		const ZSize d = cc->content->desired;
		const float w = zrn_minf(d.width, ITEM_CIRCLE), h = zrn_minf(d.height, ITEM_CIRCLE);
		zrn_arrange(cc->content, zrn_rect((final.width - w) * 0.5f, item_top(e) + (ITEM_CIRCLE - h) * 0.5f, w, h));
	} else if (cc->content) {
		zrn_arrange(cc->content, zrn_rect(0, 0, 0, 0));
	}
	return final;
}

static ZThickness item_chrome(zrn_element_t* const e) {
	ZST_UNUSED(e);
	return zrn_thickness_zero();
}

static int item_pressed(zrn_element_t* const e) {
	const zrn_button_base_t* b = ZST_AS(zrn_button_base_t, e);
	return b->pressed && b->pointer_over;
}

static int item_checked(zrn_element_t* const e) {
	return zrn_is_a(e, Z_CLASS_APP_BAR_TOGGLE_BUTTON) && ZST_AS(zrn_toggle_button_t, e)->state == Z_CHECKED;
}

/* The colors of the states of the templates: the outline, the fill of the circle, and the foreground of the icon. */
static void item_colors(zrn_element_t* const e, ZColor* const stroke, ZColor* const fill, ZColor* const icon, ZColor* const label) {
	const ZColor fg = zrn_theme_color(e, ZST_TC_FOREGROUND), bg = zrn_theme_color(e, ZST_TC_BACKGROUND);
	const ZColor disabled = zrn_theme_color(e, ZST_TC_PHONE_DISABLED), none = Zcolor_rgba(0, 0, 0, 0);
	const int enabled = zrn_effectively_enabled(e), pressed = item_pressed(e), checked = item_checked(e);
	const ZColor own = zrn_ctl_foreground(ZST_AS(zrn_control_t, e));
	*label = fg;
	if (!enabled) {
		*label = disabled;
		if (checked) { *stroke = none; *fill = disabled; *icon = bg; }
		else { *stroke = disabled; *fill = none; *icon = disabled; }
	} else if (checked && pressed) {
		/* AppBarToggleButtonCheckedPressed: the outline and the icon in the foreground, no fill */
		*stroke = fg; *fill = none; *icon = fg;
	} else if (checked) {
		*stroke = fg; *fill = fg; *icon = bg;
	} else if (pressed) {
		*stroke = fg; *fill = fg; *icon = bg;
	} else {
		*stroke = fg; *fill = none; *icon = own;
	}
}

static ZColor item_content_foreground(zrn_element_t* const e) {
	ZColor stroke, fill, icon, label;
	item_colors(e, &stroke, &fill, &icon, &label);
	return icon;
}

static void item_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_app_bar_item_t* it = item_of(e);
	ZColor stroke, fill, icon, label;
	item_colors(e, &stroke, &fill, &icon, &label);
	if (it->overflow) {
		const zrn_font_t font = row_font();
		const char* t = it->label ? it->label : "";
		if (item_pressed(e)) zrn_fill_rect(p, zrn_local_rect(e), zrn_theme_color(e, ZST_TC_BASE_LOW));
		zrn_draw_string(p, font, t, (int)strlen(t), ROW_LEFT, ROW_PAD_V, label);
		return;
	}
	{
		const float cx = e->rect.width * 0.5f, top = item_top(e);
		const ZRect circle = zrn_rect(cx - ITEM_CIRCLE * 0.5f, top, ITEM_CIRCLE, ITEM_CIRCLE);
		if (fill.a > 0) zrn_fill_ellipse(p, circle, fill);
		if (stroke.a > 0) zrn_stroke_ellipse(p, circle, ITEM_OUTLINE, stroke);
		if (!it->is_compact && it->label && *it->label) {
			zrn_text_layout_t* l = zrn_text_layout_new(label_font(e), it->label, (int)strlen(it->label), ITEM_LABEL_WIDTH, 1, 0, 0);
			if (l) {
				zrn_draw_text(p, l, cx - ITEM_LABEL_WIDTH * 0.5f, top + ITEM_CIRCLE + ITEM_GAP, ITEM_LABEL_WIDTH, Z_TEXT_CENTER, Z_TRIM_NONE, label);
				zrn_text_layout_free(l);
			}
		}
	}
}

static void item_init(zrn_element_t* const e) {
	auto c = ZST_AS(zrn_control_t, e);
	/* the style of both: Top, Left, normal weight; no outline, no padding, no minimum size, no touch overhang */
	c->border_thickness = zrn_thickness_zero();
	c->padding = zrn_thickness_zero();
	c->font_family = Z_FONT_NORMAL;
	c->font_size = ITEM_LABEL_FONT;
	e->min_width = 0;
	e->min_height = 0;
	e->halign = Z_HALIGN_LEFT;
	e->valign = Z_VALIGN_TOP;
}

static void item_destroy(zrn_element_t* e) { free(item_of(e)->label); }

zrn_class_t zrn_class_app_bar_button = {
	.base = &zrn_class_button, .name = "AppBarButton", .id = Z_CLASS_APP_BAR_BUTTON, .size = sizeof(zrn_app_bar_button_t),
	.init = item_init, .destroy = item_destroy, .measure = item_measure, .arrange = item_arrange, .paint = item_paint,
	.chrome = item_chrome, .content_foreground = item_content_foreground
};

zrn_class_t zrn_class_app_bar_toggle_button = {
	.base = &zrn_class_toggle_button, .name = "AppBarToggleButton", .id = Z_CLASS_APP_BAR_TOGGLE_BUTTON, .size = sizeof(zrn_app_bar_toggle_button_t),
	.init = item_init, .destroy = item_destroy, .measure = item_measure, .arrange = item_arrange, .paint = item_paint,
	.chrome = item_chrome, .content_foreground = item_content_foreground
};

static Zid make_item(const ZClass cls, const char* const label, const Zid icon) {
	zrn_element_t* e = zrn_create(cls);
	if (!e) return Z_INVALID_ID;
	zrn_set_string(&item_of(e)->label, label ? label : "");
	if (icon != Z_INVALID_ID) {
		zrn_element_t* i = zrn_get(icon, Z_CLASS_ICON_ELEMENT);
		if (i) zrn_content_set(ZST_AS(zrn_content_control_t, e), i);
	}
	return e->id;
}

Zid Zapp_bar_button_create(const char* label, const Zid icon) { return make_item(Z_CLASS_APP_BAR_BUTTON, label, icon); }
Zid Zapp_bar_toggle_button_create(const char* label, const Zid icon) { return make_item(Z_CLASS_APP_BAR_TOGGLE_BUTTON, label, icon); }

static zrn_element_t* get_item(const Zid id) {
	zrn_element_t* e = zrn_lookup(id);
	if (!e || (!zrn_is_a(e, Z_CLASS_APP_BAR_BUTTON) && !zrn_is_a(e, Z_CLASS_APP_BAR_TOGGLE_BUTTON))) {
		zrn_error(e ? Z_ERROR_WRONG_CLASS : Z_ERROR_INVALID_ID, "not an app bar button");
		return nullptr;
	}
	return e;
}

void Zapp_bar_button_set_label(const Zid id, const char* const label) {
	zrn_element_t* e = get_item(id);
	if (!e) return;
	zrn_set_string(&item_of(e)->label, label ? label : "");
	zrn_changed_layout(e);
}

const char* Zapp_bar_button_get_label(const Zid id) {
	zrn_element_t* e = get_item(id);
	return e && item_of(e)->label ? item_of(e)->label : "";
}

void Zapp_bar_button_set_icon(const Zid id, const Zid icon) {
	zrn_element_t* e = get_item(id);
	zrn_element_t* i = icon != Z_INVALID_ID ? zrn_get(icon, Z_CLASS_ICON_ELEMENT) : nullptr;
	if (!e || (icon != Z_INVALID_ID && !i)) return;
	zrn_content_set(ZST_AS(zrn_content_control_t, e), i);
}

Zid Zapp_bar_button_get_icon(const Zid id) {
	zrn_element_t* e = get_item(id);
	return e && ZST_AS(zrn_content_control_t, e)->content ? ZST_AS(zrn_content_control_t, e)->content->id : Z_INVALID_ID;
}

void Zapp_bar_button_set_is_compact(const Zid id, const int is_compact) {
	zrn_element_t* e = get_item(id);
	if (!e) return;
	item_of(e)->is_compact = is_compact != 0;
	zrn_changed_layout(e);
}

int Zapp_bar_button_get_is_compact(const Zid id) {
	zrn_element_t* e = get_item(id);
	return e ? item_of(e)->is_compact : 0;
}


/* ============================================================================
 *  AppBarSeparator
 * ============================================================================ */

typedef struct zrn_app_bar_separator {
	zrn_control_t base;
	int is_compact;
} zrn_app_bar_separator_t;

static ZSize separator_measure(zrn_element_t* const e, const ZSize available) {
	ZST_UNUSED(e); ZST_UNUSED(available);
	return zrn_size(SEPARATOR_WIDTH, ITEM_TOP + ITEM_CIRCLE + ITEM_GAP);
}

static void separator_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	/* AppBarSeparatorForegroundThemeBrush is PhoneBaseMediumMid: a hairline as tall as the circles */
	zrn_fill_rect(p, zrn_rect(e->rect.width * 0.5f - 0.5f, ITEM_TOP, 1.0f, ITEM_CIRCLE), zrn_theme_color(e, ZST_TC_BASE_MEDIUM_MID));
}

static void separator_init(zrn_element_t* const e) {
	e->focusable = 0;
	e->halign = Z_HALIGN_LEFT;
	e->valign = Z_VALIGN_TOP;
}

zrn_class_t zrn_class_app_bar_separator = {
	.base = &zrn_class_control, .name = "AppBarSeparator", .id = Z_CLASS_APP_BAR_SEPARATOR, .size = sizeof(zrn_app_bar_separator_t),
	.init = separator_init, .measure = separator_measure, .paint = separator_paint
};

Zid Zapp_bar_separator_create(void) {
	const zrn_element_t* e = zrn_create(Z_CLASS_APP_BAR_SEPARATOR);
	return e ? e->id : Z_INVALID_ID;
}

void Zapp_bar_separator_set_is_compact(const Zid id, const int is_compact) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_APP_BAR_SEPARATOR);
	if (e) ZST_AS(zrn_app_bar_separator_t, e)->is_compact = is_compact != 0;
}

int Zapp_bar_separator_get_is_compact(const Zid id) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_APP_BAR_SEPARATOR);
	return e ? ZST_AS(zrn_app_bar_separator_t, e)->is_compact : 0;
}


/* ============================================================================
 *  AppBar
 * ============================================================================ */

float zrn_app_bar_closed_height(zrn_element_t* const bar) {
	return BAR(bar)->closed_mode == Z_APP_BAR_CLOSED_MINIMAL ? BAR_MINIMAL_HEIGHT : BAR_COMPACT_HEIGHT;
}

void zrn_app_bar_set_on_top(zrn_element_t* const bar, const int on_top) {
	if (BAR(bar)->on_top == on_top) return;
	BAR(bar)->on_top = on_top != 0;
	zrn_changed_layout(bar);
}

/* Closed and Minimal: only the three dots show. */
static int bar_hides_commands(zrn_element_t* const e) {
	const zrn_app_bar_t* b = BAR(e);
	return b->closed_mode == Z_APP_BAR_CLOSED_MINIMAL && !b->is_open && b->open_t <= 0;
}

static float bar_open_height(zrn_element_t* e, float width);

static ZColor bar_background(zrn_element_t* const e) {
	auto c = ZST_AS(zrn_control_t, e);
	/* CommandBarBackgroundThemeBrush is the chrome; AppBarBackgroundThemeBrush the background of the page */
	return zrn_ctl_background(c, zrn_theme_color(e, zrn_is_a(e, Z_CLASS_COMMAND_BAR) ? ZST_TC_CHROME : ZST_TC_BACKGROUND));
}

static float bar_width(const zrn_element_t* const e, const ZSize available) {
	ZST_UNUSED(e);
	return available.width < Z_INFINITE * 0.5f ? available.width : 480.0f;
}

/* The height now: between the closed one and the open one while it opens or closes. */
static float bar_height(zrn_element_t* const e, const float width) {
	const float closed = zrn_app_bar_closed_height(e), open = zrn_maxf(closed, bar_open_height(e, width));
	return closed + (open - closed) * zrn_ease(Z_EASE_OUT, BAR(e)->open_t);
}

float zrn_app_bar_height(zrn_element_t* bar, const float width) { return bar_height(bar, width); }

static ZSize app_bar_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	float w = bar_width(e, available), h;
	if (cc->content) zrn_measure(cc->content, zrn_size(zrn_maxf(0, w - ELLIPSIS_WIDTH), Z_INFINITE));
	h = bar_height(e, w);
	return zrn_size(w, h);
}

static float app_bar_open_height(zrn_element_t* const e, const float width) {
	const zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	ZST_UNUSED(width);
	return cc->content ? cc->content->desired.height + 2 * BAR_BORDER : 0;
}

static ZSize app_bar_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	if (cc->content) {
		if (bar_hides_commands(e)) zrn_arrange(cc->content, zrn_rect(0, final.height + 1, zrn_maxf(0, final.width - ELLIPSIS_WIDTH), cc->content->desired.height));
		else zrn_arrange(cc->content, zrn_rect(0, BAR_BORDER, zrn_maxf(0, final.width - ELLIPSIS_WIDTH), zrn_maxf(0, final.height - 2 * BAR_BORDER)));
	}
	return final;
}

/* The three dots, and where a press opens the bar: all of a Minimal bar, the right edge of a Compact one. */
static ZRect ellipsis_rect(zrn_element_t* const e) {
	if (bar_hides_commands(e)) return zrn_local_rect(e);
	return zrn_rect(e->rect.width - ELLIPSIS_WIDTH, 0, ELLIPSIS_WIDTH, e->rect.height);
}

/* Where the middle of the dots is, vertically: in line with the circles of the buttons, centred in a Minimal bar. */
static float ellipsis_y(zrn_element_t* const e, const float row_top) {
	if (bar_hides_commands(e)) return e->rect.height * 0.5f;
	return row_top + ITEM_TOP + ITEM_CIRCLE * 0.5f;
}

static float bar_row_top(zrn_element_t* e);

static void bar_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const ZRect r = zrn_local_rect(e);
	auto c = ZST_AS(zrn_control_t, e);
	const ZColor border = zrn_ctl_border_brush(c, zrn_theme_color(e, ZST_TC_BACKGROUND));
	const ZColor fg = zrn_ctl_foreground(c);
	const float cx = e->rect.width - ELLIPSIS_WIDTH * 0.5f, cy = ellipsis_y(e, bar_row_top(e));
	int i;
	zrn_fill_rect(p, r, bar_background(e));
	/* AppBarBorderThemeBrush, 1.5 px, on the edge toward the content */
	if (BAR(e)->on_top) zrn_fill_rect(p, zrn_rect(0, r.height - BAR_BORDER, r.width, BAR_BORDER), border);
	else zrn_fill_rect(p, zrn_rect(0, 0, r.width, BAR_BORDER), border);
	for (i = -1; i <= 1; i++) zrn_fill_ellipse(p, zrn_rect(cx + (float)i * ELLIPSIS_STEP - ELLIPSIS_DOT * 0.5f, cy - ELLIPSIS_DOT * 0.5f, ELLIPSIS_DOT, ELLIPSIS_DOT), fg);
}

static int bar_hit_ellipsis(zrn_element_t* const e, const float x, const float y) {
	const ZRect r = ellipsis_rect(e);
	return x >= r.x && y >= r.y && x < r.x + r.width && y < r.y + r.height;
}

static int bar_input(zrn_element_t* const e, zrn_input_t* const in) {
	int* pressed = &BAR(e)->ellipsis_pressed;
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			if (!zrn_effectively_enabled(e)) return 1;
			*pressed = bar_hit_ellipsis(e, in->x, in->y);
			if (*pressed && e->root) zrn_set_capture(e->root, e);
			/* the bar is a surface: a press on it does not reach the page under it */
			return 1;
		case ZST_IN_POINTER_UP: {
			const int was = *pressed;
			*pressed = 0;
			if (e->root) zrn_release_capture(e->root, e);
			if (was && bar_hit_ellipsis(e, in->x, in->y)) Zapp_bar_set_is_open(e->id, !BAR(e)->is_open);
			return 1;
		}
		case ZST_IN_CAPTURE_LOST:
			*pressed = 0;
			return 0;
		default:
			return 0;
	}
}

void zrn_app_bar_item_clicked(zrn_element_t* const item) {
	/* a secondary command was chosen: the menu goes */
	if (zrn_is_a(item, Z_CLASS_APP_BAR_BUTTON) && item_of(item)->overflow) {
		zrn_element_t* p;
		for (p = item->parent; p; p = p->parent) {
			if (zrn_is_a(p, Z_CLASS_APP_BAR)) {
				Zapp_bar_set_is_open(p->id, false);
				return;
			}
		}
	}
}

static void bar_sync_compact(zrn_element_t* e);

static void bar_tick(zrn_element_t* const e, const float dt) {
	auto b = BAR(e);
	const float goal = b->is_open ? 1.0f : 0.0f;
	if (b->open_t == goal) { e->wants_tick = 0; return; }
	if (goal > b->open_t) b->open_t = zrn_minf(goal, b->open_t + dt / BAR_OPEN_MS);
	else b->open_t = zrn_maxf(goal, b->open_t - dt / BAR_OPEN_MS);
	bar_sync_compact(e);
	zrn_changed_layout(e);
	if (b->open_t == goal) e->wants_tick = 0;
}

static void app_bar_init(zrn_element_t* const e) {
	auto c = ZST_AS(zrn_control_t, e);
	BAR(e)->closed_mode = Z_APP_BAR_CLOSED_COMPACT;
	e->focusable = 0;
	e->clip_to_bounds = 1;
	e->halign = Z_HALIGN_STRETCH;
	e->valign = Z_VALIGN_TOP;
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
}

zrn_class_t zrn_class_app_bar = {
	.base = &zrn_class_content_control, .name = "AppBar", .id = Z_CLASS_APP_BAR, .size = sizeof(zrn_app_bar_t),
	.init = app_bar_init, .measure = app_bar_measure, .arrange = app_bar_arrange, .paint = bar_paint, .input = bar_input, .tick = bar_tick
};

Zid Zapp_bar_create(void) {
	const zrn_element_t* e = zrn_create(Z_CLASS_APP_BAR);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_app_bar_t* get_bar(const Zid id) { return BAR(zrn_get(id, Z_CLASS_APP_BAR)); }

void Zapp_bar_set_is_open(Zid id, int is_open) {
	zrn_app_bar_t* b = get_bar(id);
	zrn_element_t* e;
	float goal;
	if (!b) return;
	e = &b->base.base.base;
	is_open = is_open != 0;
	if (b->is_open == is_open) return;
	b->is_open = is_open;
	goal = is_open ? 1.0f : 0.0f;
	if (e->root) {
		if (is_open) e->root->open_bar = e;
		else if (e->root->open_bar == e) e->root->open_bar = nullptr;
	}
	if (e->root && zrn_animations_enabled(e->root)) {
		e->wants_tick = 1;
	} else {
		b->open_t = goal;
		e->wants_tick = 0;
	}
	bar_sync_compact(e);
	zrn_changed_layout(e);
	zrn_raise(e, is_open ? Z_EVENT_OPENED : Z_EVENT_CLOSED);
}

int Zapp_bar_get_is_open(const Zid id) {
	const zrn_app_bar_t* b = get_bar(id);
	return b ? b->is_open : 0;
}

void Zapp_bar_set_is_sticky(const Zid id, const int is_sticky) {
	zrn_app_bar_t* b = get_bar(id);
	if (b) b->is_sticky = is_sticky != 0;
}

int Zapp_bar_get_is_sticky(const Zid id) {
	zrn_app_bar_t* b = get_bar(id);
	return b ? b->is_sticky : 0;
}

void Zapp_bar_set_closed_display_mode(const Zid id, const ZAppBarClosedDisplayMode mode) {
	zrn_app_bar_t* b = get_bar(id);
	if (!b) return;
	if (mode != Z_APP_BAR_CLOSED_COMPACT && mode != Z_APP_BAR_CLOSED_MINIMAL) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "unknown ClosedDisplayMode 0x%x", (unsigned)mode);
		return;
	}
	b->closed_mode = mode;
	bar_sync_compact(&b->base.base.base);
	zrn_changed_layout(&b->base.base.base);
}

ZAppBarClosedDisplayMode Zapp_bar_get_closed_display_mode(const Zid id) {
	zrn_app_bar_t* b = get_bar(id);
	return b ? b->closed_mode : Z_APP_BAR_CLOSED_COMPACT;
}

void Zapp_bar_on_opened(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(id, Z_CLASS_APP_BAR)) Zelement_on(id, Z_EVENT_OPENED, callback, user_data);
}

void Zapp_bar_on_closed(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(id, Z_CLASS_APP_BAR)) Zelement_on(id, Z_EVENT_CLOSED, callback, user_data);
}


/* ============================================================================
 *  CommandBar
 *  The primary commands in a row at the right, the content at the left; open, the buttons show
 *  their labels and the secondary commands are a list of text rows over (or, on top, under) the row.
 * ============================================================================ */

static void list_remove(zrn_element_t*** list, int* const count, const zrn_element_t* const child) {
	int i;
	for (i = 0; i < *count; i++) {
		if ((*list)[i] == child) {
			memmove(*list + i, *list + i + 1, (size_t)(*count - i - 1) * sizeof **list);
			(*count)--;
			return;
		}
	}
}

static void command_bar_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	auto c = CBAR(e);
	list_remove(&c->primary, &c->primary_count, child);
	list_remove(&c->secondary, &c->secondary_count, child);
	zrn_cc_child_removed(e, child);
}

static void command_bar_destroy(zrn_element_t* const e) {
	free(CBAR(e)->primary);
	free(CBAR(e)->secondary);
	CBAR(e)->primary = nullptr;
	CBAR(e)->secondary = nullptr;
}

/* The labels show as soon as the bar starts to open and go when it has closed. */
static void bar_sync_compact(zrn_element_t* const e) {
	zrn_command_bar_t* c;
	int compact, i;
	if (!zrn_is_a(e, Z_CLASS_COMMAND_BAR)) return;
	c = CBAR(e);
	compact = !(c->base.is_open || c->base.open_t > 0);
	for (i = 0; i < c->primary_count; i++) {
		zrn_element_t* b = c->primary[i];
		if (zrn_is_a(b, Z_CLASS_APP_BAR_BUTTON) || zrn_is_a(b, Z_CLASS_APP_BAR_TOGGLE_BUTTON)) {
			if (item_of(b)->is_compact != compact) {
				item_of(b)->is_compact = compact;
				zrn_changed_layout(b);
			}
		} else if (zrn_is_a(b, Z_CLASS_APP_BAR_SEPARATOR)) {
			ZST_AS(zrn_app_bar_separator_t, b)->is_compact = compact;
		}
	}
}

/* The row of the primary commands, as tall as the tallest: closed (no labels) or open. */
static float command_row_height(const zrn_command_bar_t* const c, const int open) {
	float h = BAR_ROW;
	int i;
	for (i = 0; i < c->primary_count; i++) {
		zrn_element_t* b = c->primary[i];
		if (zrn_is_a(b, Z_CLASS_APP_BAR_BUTTON) || zrn_is_a(b, Z_CLASS_APP_BAR_TOGGLE_BUTTON)) h = zrn_maxf(h, item_height(b, !open));
	}
	return h;
}

static float secondary_height(const zrn_command_bar_t* const c) {
	float h = 0;
	int i;
	if (c->secondary_count == 0) return 0;
	for (i = 0; i < c->secondary_count; i++) h += c->secondary[i]->desired.height;
	return ROW_TOP + h;
}

static float command_open_height(zrn_element_t* const e, const float width) {
	auto c = CBAR(e);
	ZST_UNUSED(width);
	return 2 * BAR_BORDER + secondary_height(c) + command_row_height(c, 1);
}

static float bar_open_height(zrn_element_t* const e, const float width) {
	if (zrn_is_a(e, Z_CLASS_COMMAND_BAR)) return command_open_height(e, width);
	return app_bar_open_height(e, width);
}

/* Where the row of the buttons starts: under the border, and under the secondary list that has come into view. */
static float bar_row_top(zrn_element_t* const e) {
	auto b = BAR(e);
	float t = zrn_ease(Z_EASE_OUT, b->open_t);
	if (!zrn_is_a(e, Z_CLASS_COMMAND_BAR) || b->on_top) return BAR_BORDER;
	return BAR_BORDER + secondary_height(CBAR(e)) * t;
}

static ZSize command_bar_measure(zrn_element_t* const e, const ZSize available) {
	auto c = CBAR(e);
	zrn_content_control_t* cc = &c->base.base;
	float w = bar_width(e, available), primary_w = 0;
	int i;
	bar_sync_compact(e);
	for (i = 0; i < c->primary_count; i++) {
		zrn_measure(c->primary[i], zrn_size(Z_INFINITE, Z_INFINITE));
		primary_w += c->primary[i]->desired.width;
	}
	for (i = 0; i < c->secondary_count; i++) zrn_measure(c->secondary[i], zrn_size(w, Z_INFINITE));
	if (cc->content) zrn_measure(cc->content, zrn_size(zrn_maxf(0, w - primary_w - ELLIPSIS_WIDTH), Z_INFINITE));
	return zrn_size(w, bar_height(e, w));
}

static ZSize command_bar_arrange(zrn_element_t* const e, const ZSize final) {
	auto c = CBAR(e);
	zrn_content_control_t* cc = &c->base.base;
	float t = zrn_ease(Z_EASE_OUT, c->base.open_t);
	float row_top = bar_row_top(e), x = final.width - ELLIPSIS_WIDTH, primary_w = 0, sec_h = secondary_height(c), y;
	int hidden = bar_hides_commands(e), i;
	/* what a Minimal bar hides is put below its edge, where the clip of the bar cuts it */
	float away = final.height + 1;
	for (i = c->primary_count - 1; i >= 0; i--) {
		zrn_element_t* b = c->primary[i];
		x -= b->desired.width;
		primary_w += b->desired.width;
		zrn_arrange(b, zrn_rect(x, hidden ? away : row_top, b->desired.width, b->desired.height));
	}
	if (cc->content) {
		float room = zrn_maxf(0, final.width - primary_w - ELLIPSIS_WIDTH);
		zrn_arrange(cc->content, zrn_rect(0, hidden ? away : row_top, room, zrn_minf(cc->content->desired.height, BAR_ROW)));
	}
	/* the secondary rows: on a bar at the bottom they slide down into view above the buttons, on one at the top they lie under them */
	if (c->base.on_top) y = BAR_BORDER + command_row_height(c, 1) + ROW_TOP;
	else y = BAR_BORDER + ROW_TOP - sec_h * (1 - t);
	if (c->secondary_count == 0) y = away;
	for (i = 0; i < c->secondary_count; i++) {
		zrn_element_t* b = c->secondary[i];
		zrn_arrange(b, zrn_rect(0, hidden ? away : y, final.width, b->desired.height));
		y += b->desired.height;
	}
	return final;
}

zrn_class_t zrn_class_command_bar = {
	.base = &zrn_class_app_bar, .name = "CommandBar", .id = Z_CLASS_COMMAND_BAR, .size = sizeof(zrn_command_bar_t),
	.destroy = command_bar_destroy, .measure = command_bar_measure, .arrange = command_bar_arrange, .child_removed = command_bar_child_removed
};

Zid Zcommand_bar_create(void) {
	const zrn_element_t* e = zrn_create(Z_CLASS_COMMAND_BAR);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_command_bar_t* get_command_bar(const Zid id) { return CBAR(zrn_get(id, Z_CLASS_COMMAND_BAR)); }

static int is_command(const zrn_element_t* const e) {
	return zrn_is_a(e, Z_CLASS_APP_BAR_BUTTON) || zrn_is_a(e, Z_CLASS_APP_BAR_TOGGLE_BUTTON) || zrn_is_a(e, Z_CLASS_APP_BAR_SEPARATOR);
}

static int add_command(const Zid bar, const Zid command, const int secondary) {
	zrn_command_bar_t* c = get_command_bar(bar);
	zrn_element_t* cmd = command != Z_INVALID_ID ? zrn_get(command, Z_CLASS_ELEMENT) : nullptr;
	zrn_element_t*** list;
	int* count;
	int* cap;
	if (!c || !cmd) return -1;
	if (!is_command(cmd)) {
		zrn_error(Z_ERROR_WRONG_CLASS, "a command is an AppBarButton, an AppBarToggleButton or an AppBarSeparator, not a %s", cmd->cls->name);
		return -1;
	}
	if (cmd->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "the command already has a parent");
		return -1;
	}
	list = secondary ? &c->secondary : &c->primary;
	count = secondary ? &c->secondary_count : &c->primary_count;
	cap = secondary ? &c->secondary_cap : &c->primary_cap;
	if (*count == *cap) {
		const int ncap = *cap ? *cap * 2 : 4;
		zrn_element_t** more = (zrn_element_t**)realloc(*list, (size_t)ncap * sizeof *more);
		if (!more) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		*list = more;
		*cap = ncap;
	}
	if (zrn_is_a(cmd, Z_CLASS_APP_BAR_BUTTON) || zrn_is_a(cmd, Z_CLASS_APP_BAR_TOGGLE_BUTTON)) item_of(cmd)->overflow = secondary;
	(*list)[*count] = cmd;
	(*count)++;
	zrn_add_child(&c->base.base.base.base, cmd, -1);
	if (cmd->parent != &c->base.base.base.base) return -1;
	bar_sync_compact(&c->base.base.base.base);
	return *count - 1;
}

int Zcommand_bar_add_primary_command(Zid bar, const Zid command) { return add_command(bar, command, 0); }
int Zcommand_bar_add_secondary_command(Zid bar, const Zid command) { return add_command(bar, command, 1); }

int Zcommand_bar_get_primary_command_count(const Zid bar) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	return c ? c->primary_count : 0;
}

int Zcommand_bar_get_secondary_command_count(const Zid bar) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	return c ? c->secondary_count : 0;
}

Zid Zcommand_bar_get_primary_command(const Zid bar, const int index) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	return c && index >= 0 && index < c->primary_count ? c->primary[index]->id : Z_INVALID_ID;
}

Zid Zcommand_bar_get_secondary_command(const Zid bar, const int index) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	return c && index >= 0 && index < c->secondary_count ? c->secondary[index]->id : Z_INVALID_ID;
}

void Zcommand_bar_clear_primary_commands(const Zid bar) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	while (c && c->primary_count > 0) zrn_destroy(c->primary[c->primary_count - 1]);
}

void Zcommand_bar_clear_secondary_commands(const Zid bar) {
	const zrn_command_bar_t* c = get_command_bar(bar);
	while (c && c->secondary_count > 0) zrn_destroy(c->secondary[c->secondary_count - 1]);
}
