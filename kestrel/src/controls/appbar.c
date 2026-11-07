/**
 * Kestrel - controls/appbar.c
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

#define ICON(e) KST_AS(kst_icon_t, e)
#define BAR(e) KST_AS(kst_app_bar_t, e)
#define CBAR(e) KST_AS(kst_command_bar_t, e)

/* AppBarThemeMinHeight 54.5, AppBarTopBorderThemeThickness 0,0,0,1.5 / AppBarBottomBorderThemeThickness 0,1.5,0,0 and the
 * theme padding of the other side: the closed bar of a page is 1.5 + 54.5 + 1.5 = 57.5, the MaxHeight of the CommandBar style. */
#define BAR_ROW 54.5f
#define BAR_BORDER 1.5f
#define BAR_COMPACT_HEIGHT 57.5f
/* The height of a Minimal bar: not in the XAML, chosen. */
#define BAR_MINIMAL_HEIGHT 35.0f
/* The opening of the bar: chosen. */
#define BAR_OPEN_MS 167.0f
/* The three dots that open the bar: not in the XAML, chosen. */
#define ELLIPSIS_WIDTH 30.0f
#define ELLIPSIS_DOT 5.0f
#define ELLIPSIS_STEP 8.5f

/* The AppBarButton template: the circle, its margins, the label and its width. */
#define ITEM_CIRCLE 38.5f
#define ITEM_OUTLINE 2.5f
#define ITEM_TOP 12.0f
#define ITEM_TOGGLE_TOP 11.0f
#define ITEM_GAP 4.0f
#define ITEM_FULL_WIDTH 80.0f
#define ITEM_COMPACT_WIDTH 48.0f
#define ITEM_LABEL_WIDTH 70.5f
#define ITEM_LABEL_FONT 9.5f
#define ITEM_LABEL_BOTTOM 10.5f
#define ICON_SIZE 20.0f
/* The rows of the secondary commands take the measures of a MenuFlyoutItem: padding 0,8.5, ExtraLarge SemiLight type. */
#define ROW_PAD_V 8.5f
#define ROW_LEFT 19.0f
#define ROW_TOP 13.5f
#define ROW_FONT 25.5f
#define SEPARATOR_WIDTH 12.0f


/* ============================================================================
 *  IconElement
 * ============================================================================ */

static void icon_destroy(kst_element_t* e) {
	kst_icon_t* i = ICON(e);
	free(i->glyph);
	free(i->path);
	free(i->path_runs);
	if (i->image) kst_image_free(i->image);
}

static void icon_init(kst_element_t* e) {
	kst_icon_t* i = ICON(e);
	i->font_size = ICON_SIZE;
	i->family = K_FONT_NORMAL;
	e->hit_visible = 0;
}

static KColor icon_color(kst_element_t* e) {
	kst_icon_t* i = ICON(e);
	return i->has_foreground ? i->foreground : kst_inherited_foreground(e);
}

static KSize icon_measure(kst_element_t* e, KSize available) {
	kst_icon_t* i = ICON(e);
	KST_UNUSED(available);
	if (e->cls == &kst_class_font_icon) {
		kst_font_t font = { i->family, i->font_size, 0 };
		const char* g = i->glyph ? i->glyph : "";
		return kst_size(kst_text_width(font, g, (int)strlen(g)), kst_font_line_height(font));
	}
	return kst_size(ICON_SIZE, ICON_SIZE);
}

kst_class_t kst_class_icon_element = {
	.base = &kst_class_element, .name = "IconElement", .id = K_CLASS_ICON_ELEMENT, .size = sizeof(kst_icon_t), .abstract_class = 1,
	.init = icon_init, .destroy = icon_destroy, .measure = icon_measure
};


/* ---- the vector shapes of the symbols, in a box of 20 x 20 units ---- */

typedef struct shape {
	kst_painter_t* p;
	float ox, oy, s;
	KColor c;
} shape_t;

static float sx(const shape_t* h, float v) { return h->ox + v * h->s; }
static float sy(const shape_t* h, float v) { return h->oy + v * h->s; }

static void sh_line(shape_t* h, float x0, float y0, float x1, float y1) {
	float t = kst_maxf(1.0f, 2.0f * h->s);
	kst_draw_line(h->p, sx(h, x0), sy(h, y0), sx(h, x1), sy(h, y1), t, h->c);
}

/* A run of lines with round joints. */
static void sh_poly(shape_t* h, const float* xy, int n, int closed) {
	float t = kst_maxf(1.0f, 2.0f * h->s);
	int i;
	for (i = 0; i + 1 < n; i++) sh_line(h, xy[2 * i], xy[2 * i + 1], xy[2 * i + 2], xy[2 * i + 3]);
	if (closed && n > 2) sh_line(h, xy[2 * n - 2], xy[2 * n - 1], xy[0], xy[1]);
	for (i = 0; i < n; i++) kst_fill_ellipse(h->p, kst_rect(sx(h, xy[2 * i]) - t * 0.5f, sy(h, xy[2 * i + 1]) - t * 0.5f, t, t), h->c);
}

static void sh_fill(shape_t* h, const float* xy, int n) {
	float pts[48];
	int i;
	if (n > 24) n = 24;
	for (i = 0; i < n; i++) {
		pts[2 * i] = sx(h, xy[2 * i]);
		pts[2 * i + 1] = sy(h, xy[2 * i + 1]);
	}
	kst_fill_polygon(h->p, pts, n, h->c);
}

static void sh_rect(shape_t* h, float x, float y, float w, float rh) {
	kst_fill_rect(h->p, kst_rect(sx(h, x), sy(h, y), w * h->s, rh * h->s), h->c);
}

static void sh_dot(shape_t* h, float x, float y, float r) {
	kst_fill_ellipse(h->p, kst_rect(sx(h, x - r), sy(h, y - r), 2 * r * h->s, 2 * r * h->s), h->c);
}

static void sh_ring(shape_t* h, float x, float y, float r, float thickness) {
	kst_stroke_ellipse(h->p, kst_rect(sx(h, x - r), sy(h, y - r), 2 * r * h->s, 2 * r * h->s), kst_maxf(1.0f, thickness * h->s), h->c);
}

static void sh_star(shape_t* h, int outline) {
	float xy[20];
	int i;
	for (i = 0; i < 10; i++) {
		float a = -KST_PI * 0.5f + (float)i * KST_PI / 5.0f, r = (i & 1) ? 4.2f : 9.0f;
		xy[2 * i] = 10.0f + r * cosf(a);
		xy[2 * i + 1] = 10.5f + r * sinf(a);
	}
	if (outline) sh_poly(h, xy, 10, 1);
	else sh_fill(h, xy, 10);
}

static void draw_symbol(kst_painter_t* p, KSymbol symbol, KRect box, KColor c) {
	shape_t h;
	float side = kst_minf(box.width, box.height);
	h.p = p;
	h.s = side / ICON_SIZE;
	h.ox = box.x + (box.width - side) * 0.5f;
	h.oy = box.y + (box.height - side) * 0.5f;
	h.c = c;
	switch (symbol) {
		case K_SYMBOL_ACCEPT: { static const float v[] = { 3, 10.5f, 8, 15.5f, 17, 5 }; sh_poly(&h, v, 3, 0); break; }
		case K_SYMBOL_ADD: sh_line(&h, 10, 3, 10, 17); sh_line(&h, 3, 10, 17, 10); break;
		case K_SYMBOL_REMOVE: sh_line(&h, 3, 10, 17, 10); break;
		case K_SYMBOL_CANCEL: case K_SYMBOL_CLEAR: sh_line(&h, 4, 4, 16, 16); sh_line(&h, 16, 4, 4, 16); break;
		case K_SYMBOL_BACK: { static const float v[] = { 10, 4, 4, 10, 10, 16 }; sh_line(&h, 17, 10, 4, 10); sh_poly(&h, v, 3, 0); break; }
		case K_SYMBOL_FORWARD: { static const float v[] = { 10, 4, 16, 10, 10, 16 }; sh_line(&h, 3, 10, 16, 10); sh_poly(&h, v, 3, 0); break; }
		case K_SYMBOL_UP: { static const float v[] = { 4, 10, 10, 4, 16, 10 }; sh_line(&h, 10, 17, 10, 4); sh_poly(&h, v, 3, 0); break; }
		case K_SYMBOL_DELETE: {
			static const float body[] = { 5, 7, 6, 17, 14, 17, 15, 7 };
			sh_line(&h, 3, 5, 17, 5);
			sh_line(&h, 8, 3, 12, 3);
			sh_poly(&h, body, 4, 0);
			sh_line(&h, 8.5f, 9, 8.5f, 15);
			sh_line(&h, 11.5f, 9, 11.5f, 15);
			break;
		}
		case K_SYMBOL_EDIT: {
			static const float pen[] = { 13, 3, 17, 7, 8, 16, 3, 17, 4, 12 };
			sh_fill(&h, pen, 5);
			break;
		}
		case K_SYMBOL_HOME: {
			static const float roof[] = { 2, 10, 10, 3, 18, 10 }, body[] = { 4.5f, 8.5f, 4.5f, 17, 15.5f, 17, 15.5f, 8.5f }, door[] = { 8, 17, 8, 12, 12, 12, 12, 17 };
			sh_poly(&h, roof, 3, 0);
			sh_poly(&h, body, 4, 0);
			sh_poly(&h, door, 4, 0);
			break;
		}
		case K_SYMBOL_MAIL: case K_SYMBOL_MAIL_FILLED: {
			static const float flap[] = { 3, 5, 10, 11, 17, 5 }, frame[] = { 2.5f, 4, 17.5f, 4, 17.5f, 16, 2.5f, 16 };
			sh_poly(&h, frame, 4, 1);
			sh_poly(&h, flap, 3, 0);
			break;
		}
		case K_SYMBOL_MORE: sh_dot(&h, 4, 10, 1.8f); sh_dot(&h, 10, 10, 1.8f); sh_dot(&h, 16, 10, 1.8f); break;
		case K_SYMBOL_REFRESH: case K_SYMBOL_SYNC: {
			float xy[20];
			float tip[6];
			int i, n = 9;
			for (i = 0; i < n; i++) {
				float a = (-50.0f + (float)i * 35.0f) * KST_PI / 180.0f;
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
		case K_SYMBOL_SAVE: case K_SYMBOL_SAVE_LOCAL: {
			static const float frame[] = { 3, 3, 17, 3, 17, 17, 3, 17 }, lower[] = { 6.5f, 17, 6.5f, 11, 13.5f, 11, 13.5f, 17 };
			sh_poly(&h, frame, 4, 1);
			sh_rect(&h, 6, 3, 8, 5);
			sh_poly(&h, lower, 4, 0);
			break;
		}
		case K_SYMBOL_FIND: sh_ring(&h, 8, 8, 5.5f, 2); sh_line(&h, 12, 12, 17, 17); break;
		case K_SYMBOL_SETTING: {
			int i;
			sh_ring(&h, 10, 10, 6.0f, 2);
			sh_ring(&h, 10, 10, 2.5f, 2);
			for (i = 0; i < 8; i++) {
				float a = (float)i * KST_PI / 4.0f;
				sh_line(&h, 10.0f + 6.5f * cosf(a), 10.0f + 6.5f * sinf(a), 10.0f + 9.0f * cosf(a), 10.0f + 9.0f * sinf(a));
			}
			break;
		}
		case K_SYMBOL_PIN: {
			static const float head[] = { 7, 3, 13, 3, 12, 9, 15, 12, 5, 12, 8, 9 };
			sh_fill(&h, head, 6);
			sh_line(&h, 10, 12, 10, 18);
			break;
		}
		case K_SYMBOL_PLAY: { static const float v[] = { 5, 3, 16, 10, 5, 17 }; sh_fill(&h, v, 3); break; }
		case K_SYMBOL_PAUSE: sh_rect(&h, 4.5f, 3, 4, 14); sh_rect(&h, 11.5f, 3, 4, 14); break;
		case K_SYMBOL_STOP: sh_rect(&h, 4, 4, 12, 12); break;
		case K_SYMBOL_PREVIOUS: { static const float v[] = { 16, 4, 7, 10, 16, 16 }; sh_rect(&h, 4, 4, 2.5f, 12); sh_fill(&h, v, 3); break; }
		case K_SYMBOL_NEXT: { static const float v[] = { 4, 4, 13, 10, 4, 16 }; sh_rect(&h, 13.5f, 4, 2.5f, 12); sh_fill(&h, v, 3); break; }
		case K_SYMBOL_DOWNLOAD: { static const float v[] = { 5, 8.5f, 10, 13.5f, 15, 8.5f }; sh_line(&h, 10, 3, 10, 13); sh_poly(&h, v, 3, 0); sh_line(&h, 3, 17, 17, 17); break; }
		case K_SYMBOL_UPLOAD: { static const float v[] = { 5, 8, 10, 3, 15, 8 }; sh_line(&h, 10, 3, 10, 13.5f); sh_poly(&h, v, 3, 0); sh_line(&h, 3, 17, 17, 17); break; }
		case K_SYMBOL_FAVORITE: case K_SYMBOL_SOLID_STAR: sh_star(&h, 0); break;
		case K_SYMBOL_OUTLINE_STAR: sh_star(&h, 1); break;
		case K_SYMBOL_SEND: { static const float v[] = { 2, 9, 18, 2, 12, 18, 9, 12 }; sh_fill(&h, v, 4); break; }
		default:
			/* a symbol that has no drawing yet: an empty box, so that it is seen and not mistaken for nothing */
			kst_stroke_rect(p, kst_rect(h.ox + 4 * h.s, h.oy + 4 * h.s, 12 * h.s, 12 * h.s), Kthickness_uniform(kst_maxf(1.0f, 2.0f * h.s)), c);
			break;
	}
}

static void symbol_icon_paint(kst_element_t* e, kst_painter_t* p) {
	draw_symbol(p, ICON(e)->symbol, kst_local_rect(e), icon_color(e));
}

static void symbol_icon_init(kst_element_t* e) { ICON(e)->symbol = K_SYMBOL_PLACEHOLDER; }

kst_class_t kst_class_symbol_icon = {
	.base = &kst_class_icon_element, .name = "SymbolIcon", .id = K_CLASS_SYMBOL_ICON, .size = sizeof(kst_icon_t),
	.init = symbol_icon_init, .paint = symbol_icon_paint
};

static void font_icon_paint(kst_element_t* e, kst_painter_t* p) {
	kst_icon_t* i = ICON(e);
	kst_font_t font = { i->family, i->font_size, 0 };
	const char* g = i->glyph ? i->glyph : "";
	float w = kst_text_width(font, g, (int)strlen(g)), h = kst_font_line_height(font);
	kst_draw_string(p, font, g, (int)strlen(g), (e->rect.width - w) * 0.5f, (e->rect.height - h) * 0.5f, icon_color(e));
}

kst_class_t kst_class_font_icon = {
	.base = &kst_class_icon_element, .name = "FontIcon", .id = K_CLASS_FONT_ICON, .size = sizeof(kst_icon_t), .paint = font_icon_paint
};

static void path_icon_paint(kst_element_t* e, kst_painter_t* p) {
	kst_icon_t* i = ICON(e);
	float bw = i->path_bounds[2] - i->path_bounds[0], bh = i->path_bounds[3] - i->path_bounds[1];
	float s, ox, oy;
	int run, at = 0, k;
	if (!i->path || bw <= 0 || bh <= 0) return;
	/* Stretch is Uniform: the geometry fits the icon and is centred in it */
	s = kst_minf(e->rect.width / bw, e->rect.height / bh);
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
		if (n >= 3) kst_fill_polygon(p, pts, n, icon_color(e));
		at += i->path_runs[run];
	}
}

kst_class_t kst_class_path_icon = {
	.base = &kst_class_icon_element, .name = "PathIcon", .id = K_CLASS_PATH_ICON, .size = sizeof(kst_icon_t), .paint = path_icon_paint
};

static void bitmap_icon_paint(kst_element_t* e, kst_painter_t* p) {
	kst_image_t* image = ICON(e)->image;
	float s, w, h;
	if (!image || image->width <= 0 || image->height <= 0) return;
	s = kst_minf(e->rect.width / (float)image->width, e->rect.height / (float)image->height);
	w = (float)image->width * s;
	h = (float)image->height * s;
	kst_draw_image(p, image, kst_rect((e->rect.width - w) * 0.5f, (e->rect.height - h) * 0.5f, w, h), kst_rect(0, 0, (float)image->width, (float)image->height));
}

kst_class_t kst_class_bitmap_icon = {
	.base = &kst_class_icon_element, .name = "BitmapIcon", .id = K_CLASS_BITMAP_ICON, .size = sizeof(kst_icon_t), .paint = bitmap_icon_paint
};

static kst_icon_t* get_icon(Kid id, KClass cls) { return ICON(kst_get(id, cls)); }

static Kid make_icon(KClass cls) {
	kst_element_t* e = kst_create(cls);
	return e ? e->id : K_INVALID_ID;
}

void Kicon_element_set_foreground(Kid id, KColor color) {
	kst_icon_t* i = get_icon(id, K_CLASS_ICON_ELEMENT);
	if (!i) return;
	i->foreground = color;
	i->has_foreground = 1;
	kst_changed_paint(&i->base);
}

Kid Ksymbol_icon_create(KSymbol symbol) {
	Kid id = make_icon(K_CLASS_SYMBOL_ICON);
	if (id != K_INVALID_ID) ICON(kst_lookup(id))->symbol = symbol;
	return id;
}

void Ksymbol_icon_set_symbol(Kid id, KSymbol symbol) {
	kst_icon_t* i = get_icon(id, K_CLASS_SYMBOL_ICON);
	if (!i) return;
	i->symbol = symbol;
	kst_changed_paint(&i->base);
}

KSymbol Ksymbol_icon_get_symbol(Kid id) {
	kst_icon_t* i = get_icon(id, K_CLASS_SYMBOL_ICON);
	return i ? i->symbol : K_SYMBOL_PLACEHOLDER;
}

Kid Kfont_icon_create(const char* glyph) {
	Kid id = make_icon(K_CLASS_FONT_ICON);
	if (id != K_INVALID_ID) kst_set_string(&ICON(kst_lookup(id))->glyph, glyph ? glyph : "");
	return id;
}

void Kfont_icon_set_glyph(Kid id, const char* glyph) {
	kst_icon_t* i = get_icon(id, K_CLASS_FONT_ICON);
	if (!i) return;
	kst_set_string(&i->glyph, glyph ? glyph : "");
	kst_changed_layout(&i->base);
}

const char* Kfont_icon_get_glyph(Kid id) {
	kst_icon_t* i = get_icon(id, K_CLASS_FONT_ICON);
	return i && i->glyph ? i->glyph : "";
}

void Kfont_icon_set_font_size(Kid id, float font_size) {
	kst_icon_t* i = get_icon(id, K_CLASS_FONT_ICON);
	if (!i || font_size <= 0) return;
	i->font_size = font_size;
	kst_changed_layout(&i->base);
}

void Kfont_icon_set_font_family(Kid id, KFontFamily family) {
	kst_icon_t* i = get_icon(id, K_CLASS_FONT_ICON);
	if (!i) return;
	i->family = family;
	kst_changed_layout(&i->base);
}

/* The mini language of Path.Data, absolute and relative: M, L, H, V and Z. Every sub-path is a polygon. */
static const char* path_number(const char* s, float* out, int* ok) {
	char* end;
	while (*s == ' ' || *s == ',' || *s == '\t' || *s == '\n' || *s == '\r') s++;
	*out = strtof(s, &end);
	*ok = end != s;
	return end;
}

static int path_parse(kst_icon_t* i, const char* data) {
	float* pts = NULL;
	int* runs = NULL;
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
			int relative = cmd == 'm' || cmd == 'l';
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
		return K_FALSE;
	}
	for (k = 0; k < count; k++) {
		bounds[0] = kst_minf(bounds[0], pts[2 * k]);
		bounds[1] = kst_minf(bounds[1], pts[2 * k + 1]);
		bounds[2] = kst_maxf(bounds[2], pts[2 * k]);
		bounds[3] = kst_maxf(bounds[3], pts[2 * k + 1]);
	}
	free(i->path);
	free(i->path_runs);
	i->path = pts;
	i->path_runs = runs;
	i->path_run_count = run_count;
	memcpy(i->path_bounds, bounds, sizeof bounds);
	return K_TRUE;
}

int Kpath_icon_set_data(Kid id, const char* data) {
	kst_icon_t* i = get_icon(id, K_CLASS_PATH_ICON);
	if (!i || !data) return K_FALSE;
	if (!path_parse(i, data)) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "Path.Data is not M, L, H, V and Z commands: \"%s\"", data);
		return K_FALSE;
	}
	kst_changed_paint(&i->base);
	return K_TRUE;
}

Kid Kpath_icon_create(const char* data) {
	Kid id = make_icon(K_CLASS_PATH_ICON);
	if (id != K_INVALID_ID && data && !Kpath_icon_set_data(id, data)) {
		Kelement_destroy(id);
		return K_INVALID_ID;
	}
	return id;
}

int Kbitmap_icon_set_uri_source(Kid id, const char* path) {
	kst_icon_t* i = get_icon(id, K_CLASS_BITMAP_ICON);
	kst_image_t* image;
	if (!i || !path) return K_FALSE;
	image = kst_image_load_png(path);
	if (!image) {
		kst_error(K_ERROR_IO, "cannot read the PNG \"%s\"", path);
		return K_FALSE;
	}
	if (i->image) kst_image_free(i->image);
	i->image = image;
	kst_changed_paint(&i->base);
	return K_TRUE;
}

Kid Kbitmap_icon_create(const char* path) {
	Kid id = make_icon(K_CLASS_BITMAP_ICON);
	if (id != K_INVALID_ID && path && !Kbitmap_icon_set_uri_source(id, path)) {
		Kelement_destroy(id);
		return K_INVALID_ID;
	}
	return id;
}


/* ============================================================================
 *  AppBarButton and AppBarToggleButton
 * ============================================================================ */

static kst_app_bar_item_t* item_of(kst_element_t* e) {
	if (kst_is_a(e, K_CLASS_APP_BAR_TOGGLE_BUTTON)) return &KST_AS(kst_app_bar_toggle_button_t, e)->item;
	return &KST_AS(kst_app_bar_button_t, e)->item;
}

static kst_font_t label_font(kst_element_t* e) {
	kst_font_t f = { KST_AS(kst_control_t, e)->font_family, ITEM_LABEL_FONT, 0 };
	return f;
}

static kst_font_t row_font(void) {
	kst_font_t f = { K_FONT_SEMILIGHT, ROW_FONT, -15 };
	return f;
}

static float item_top(const kst_element_t* e) { return kst_is_a(e, K_CLASS_APP_BAR_TOGGLE_BUTTON) ? ITEM_TOGGLE_TOP : ITEM_TOP; }

static float label_height(kst_element_t* e) {
	kst_app_bar_item_t* it = item_of(e);
	kst_text_layout_t* l;
	float h;
	if (!it->label || !*it->label) return 0;
	l = kst_text_layout_new(label_font(e), it->label, (int)strlen(it->label), ITEM_LABEL_WIDTH, 1, 0, 0);
	if (!l) return 0;
	h = l->height;
	kst_text_layout_free(l);
	return h;
}

/* How tall the button is with its label shown or not. */
static float item_height(kst_element_t* e, int compact) {
	kst_app_bar_item_t* it = item_of(e);
	if (it->overflow) return kst_font_line_height(row_font()) + 2 * ROW_PAD_V;
	return item_top(e) + ITEM_CIRCLE + ITEM_GAP + (compact ? 0 : label_height(e) + ITEM_LABEL_BOTTOM);
}

static KSize item_measure(kst_element_t* e, KSize available) {
	kst_app_bar_item_t* it = item_of(e);
	if (KST_AS(kst_content_control_t, e)->content) kst_measure(KST_AS(kst_content_control_t, e)->content, kst_size(ITEM_CIRCLE, ITEM_CIRCLE));
	if (it->overflow) return kst_size(available.width < K_INFINITE * 0.5f ? available.width : 480.0f, item_height(e, 0));
	return kst_size(it->is_compact ? ITEM_COMPACT_WIDTH : ITEM_FULL_WIDTH, item_height(e, it->is_compact));
}

static KSize item_arrange(kst_element_t* e, KSize final) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	if (cc->content && !item_of(e)->overflow) {
		/* the icon is as big as it asks for, in the middle of the circle */
		KSize d = cc->content->desired;
		float w = kst_minf(d.width, ITEM_CIRCLE), h = kst_minf(d.height, ITEM_CIRCLE);
		kst_arrange(cc->content, kst_rect((final.width - w) * 0.5f, item_top(e) + (ITEM_CIRCLE - h) * 0.5f, w, h));
	} else if (cc->content) {
		kst_arrange(cc->content, kst_rect(0, 0, 0, 0));
	}
	return final;
}

static KThickness item_chrome(kst_element_t* e) {
	KST_UNUSED(e);
	return kst_thickness_zero();
}

static int item_pressed(kst_element_t* e) {
	kst_button_base_t* b = KST_AS(kst_button_base_t, e);
	return b->pressed && b->pointer_over;
}

static int item_checked(kst_element_t* e) {
	return kst_is_a(e, K_CLASS_APP_BAR_TOGGLE_BUTTON) && KST_AS(kst_toggle_button_t, e)->state == K_CHECKED;
}

/* The colors of the states of the templates: the outline, the fill of the circle, and the foreground of the icon. */
static void item_colors(kst_element_t* e, KColor* stroke, KColor* fill, KColor* icon, KColor* label) {
	KColor fg = kst_theme_color(e, KST_TC_FOREGROUND), bg = kst_theme_color(e, KST_TC_BACKGROUND);
	KColor disabled = kst_theme_color(e, KST_TC_PHONE_DISABLED), none = Kcolor_rgba(0, 0, 0, 0);
	int enabled = kst_effectively_enabled(e), pressed = item_pressed(e), checked = item_checked(e);
	KColor own = kst_ctl_foreground(KST_AS(kst_control_t, e));
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

static KColor item_content_foreground(kst_element_t* e) {
	KColor stroke, fill, icon, label;
	item_colors(e, &stroke, &fill, &icon, &label);
	return icon;
}

static void item_paint(kst_element_t* e, kst_painter_t* p) {
	kst_app_bar_item_t* it = item_of(e);
	KColor stroke, fill, icon, label;
	item_colors(e, &stroke, &fill, &icon, &label);
	if (it->overflow) {
		kst_font_t font = row_font();
		const char* t = it->label ? it->label : "";
		if (item_pressed(e)) kst_fill_rect(p, kst_local_rect(e), kst_theme_color(e, KST_TC_BASE_LOW));
		kst_draw_string(p, font, t, (int)strlen(t), ROW_LEFT, ROW_PAD_V, label);
		return;
	}
	{
		float cx = e->rect.width * 0.5f, top = item_top(e);
		KRect circle = kst_rect(cx - ITEM_CIRCLE * 0.5f, top, ITEM_CIRCLE, ITEM_CIRCLE);
		if (fill.a > 0) kst_fill_ellipse(p, circle, fill);
		if (stroke.a > 0) kst_stroke_ellipse(p, circle, ITEM_OUTLINE, stroke);
		if (!it->is_compact && it->label && *it->label) {
			kst_text_layout_t* l = kst_text_layout_new(label_font(e), it->label, (int)strlen(it->label), ITEM_LABEL_WIDTH, 1, 0, 0);
			if (l) {
				kst_draw_text(p, l, cx - ITEM_LABEL_WIDTH * 0.5f, top + ITEM_CIRCLE + ITEM_GAP, ITEM_LABEL_WIDTH, K_TEXT_CENTER, K_TRIM_NONE, label);
				kst_text_layout_free(l);
			}
		}
	}
}

static void item_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	/* the style of both: Top, Left, normal weight; no outline, no padding, no minimum size, no touch overhang */
	c->border_thickness = kst_thickness_zero();
	c->padding = kst_thickness_zero();
	c->font_family = K_FONT_NORMAL;
	c->font_size = ITEM_LABEL_FONT;
	e->min_width = 0;
	e->min_height = 0;
	e->halign = K_HALIGN_LEFT;
	e->valign = K_VALIGN_TOP;
}

static void item_destroy(kst_element_t* e) { free(item_of(e)->label); }

kst_class_t kst_class_app_bar_button = {
	.base = &kst_class_button, .name = "AppBarButton", .id = K_CLASS_APP_BAR_BUTTON, .size = sizeof(kst_app_bar_button_t),
	.init = item_init, .destroy = item_destroy, .measure = item_measure, .arrange = item_arrange, .paint = item_paint,
	.chrome = item_chrome, .content_foreground = item_content_foreground
};

kst_class_t kst_class_app_bar_toggle_button = {
	.base = &kst_class_toggle_button, .name = "AppBarToggleButton", .id = K_CLASS_APP_BAR_TOGGLE_BUTTON, .size = sizeof(kst_app_bar_toggle_button_t),
	.init = item_init, .destroy = item_destroy, .measure = item_measure, .arrange = item_arrange, .paint = item_paint,
	.chrome = item_chrome, .content_foreground = item_content_foreground
};

static Kid make_item(KClass cls, const char* label, Kid icon) {
	kst_element_t* e = kst_create(cls);
	if (!e) return K_INVALID_ID;
	kst_set_string(&item_of(e)->label, label ? label : "");
	if (icon != K_INVALID_ID) {
		kst_element_t* i = kst_get(icon, K_CLASS_ICON_ELEMENT);
		if (i) kst_content_set(KST_AS(kst_content_control_t, e), i);
	}
	return e->id;
}

Kid Kapp_bar_button_create(const char* label, Kid icon) { return make_item(K_CLASS_APP_BAR_BUTTON, label, icon); }
Kid Kapp_bar_toggle_button_create(const char* label, Kid icon) { return make_item(K_CLASS_APP_BAR_TOGGLE_BUTTON, label, icon); }

static kst_element_t* get_item(Kid id) {
	kst_element_t* e = kst_lookup(id);
	if (!e || (!kst_is_a(e, K_CLASS_APP_BAR_BUTTON) && !kst_is_a(e, K_CLASS_APP_BAR_TOGGLE_BUTTON))) {
		kst_error(e ? K_ERROR_WRONG_CLASS : K_ERROR_INVALID_ID, "not an app bar button");
		return NULL;
	}
	return e;
}

void Kapp_bar_button_set_label(Kid id, const char* label) {
	kst_element_t* e = get_item(id);
	if (!e) return;
	kst_set_string(&item_of(e)->label, label ? label : "");
	kst_changed_layout(e);
}

const char* Kapp_bar_button_get_label(Kid id) {
	kst_element_t* e = get_item(id);
	return e && item_of(e)->label ? item_of(e)->label : "";
}

void Kapp_bar_button_set_icon(Kid id, Kid icon) {
	kst_element_t* e = get_item(id);
	kst_element_t* i = icon != K_INVALID_ID ? kst_get(icon, K_CLASS_ICON_ELEMENT) : NULL;
	if (!e || (icon != K_INVALID_ID && !i)) return;
	kst_content_set(KST_AS(kst_content_control_t, e), i);
}

Kid Kapp_bar_button_get_icon(Kid id) {
	kst_element_t* e = get_item(id);
	return e && KST_AS(kst_content_control_t, e)->content ? KST_AS(kst_content_control_t, e)->content->id : K_INVALID_ID;
}

void Kapp_bar_button_set_is_compact(Kid id, int is_compact) {
	kst_element_t* e = get_item(id);
	if (!e) return;
	item_of(e)->is_compact = is_compact != 0;
	kst_changed_layout(e);
}

int Kapp_bar_button_get_is_compact(Kid id) {
	kst_element_t* e = get_item(id);
	return e ? item_of(e)->is_compact : 0;
}


/* ============================================================================
 *  AppBarSeparator
 * ============================================================================ */

typedef struct kst_app_bar_separator {
	kst_control_t base;
	int is_compact;
} kst_app_bar_separator_t;

static KSize separator_measure(kst_element_t* e, KSize available) {
	KST_UNUSED(e); KST_UNUSED(available);
	return kst_size(SEPARATOR_WIDTH, ITEM_TOP + ITEM_CIRCLE + ITEM_GAP);
}

static void separator_paint(kst_element_t* e, kst_painter_t* p) {
	/* AppBarSeparatorForegroundThemeBrush is PhoneBaseMediumMid: a hairline as tall as the circles */
	kst_fill_rect(p, kst_rect(e->rect.width * 0.5f - 0.5f, ITEM_TOP, 1.0f, ITEM_CIRCLE), kst_theme_color(e, KST_TC_BASE_MEDIUM_MID));
}

static void separator_init(kst_element_t* e) {
	e->focusable = 0;
	e->halign = K_HALIGN_LEFT;
	e->valign = K_VALIGN_TOP;
}

kst_class_t kst_class_app_bar_separator = {
	.base = &kst_class_control, .name = "AppBarSeparator", .id = K_CLASS_APP_BAR_SEPARATOR, .size = sizeof(kst_app_bar_separator_t),
	.init = separator_init, .measure = separator_measure, .paint = separator_paint
};

Kid Kapp_bar_separator_create(void) {
	kst_element_t* e = kst_create(K_CLASS_APP_BAR_SEPARATOR);
	return e ? e->id : K_INVALID_ID;
}

void Kapp_bar_separator_set_is_compact(Kid id, int is_compact) {
	kst_element_t* e = kst_get(id, K_CLASS_APP_BAR_SEPARATOR);
	if (e) KST_AS(kst_app_bar_separator_t, e)->is_compact = is_compact != 0;
}

int Kapp_bar_separator_get_is_compact(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_APP_BAR_SEPARATOR);
	return e ? KST_AS(kst_app_bar_separator_t, e)->is_compact : 0;
}


/* ============================================================================
 *  AppBar
 * ============================================================================ */

float kst_app_bar_closed_height(kst_element_t* bar) {
	return BAR(bar)->closed_mode == K_APP_BAR_CLOSED_MINIMAL ? BAR_MINIMAL_HEIGHT : BAR_COMPACT_HEIGHT;
}

void kst_app_bar_set_on_top(kst_element_t* bar, int on_top) {
	if (BAR(bar)->on_top == on_top) return;
	BAR(bar)->on_top = on_top != 0;
	kst_changed_layout(bar);
}

/* Closed and Minimal: only the three dots show. */
static int bar_hides_commands(kst_element_t* e) {
	kst_app_bar_t* b = BAR(e);
	return b->closed_mode == K_APP_BAR_CLOSED_MINIMAL && !b->is_open && b->open_t <= 0;
}

static float bar_open_height(kst_element_t* e, float width);

static KColor bar_background(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	/* CommandBarBackgroundThemeBrush is the chrome; AppBarBackgroundThemeBrush the background of the page */
	return kst_ctl_background(c, kst_theme_color(e, kst_is_a(e, K_CLASS_COMMAND_BAR) ? KST_TC_CHROME : KST_TC_BACKGROUND));
}

static float bar_width(kst_element_t* e, KSize available) {
	KST_UNUSED(e);
	return available.width < K_INFINITE * 0.5f ? available.width : 480.0f;
}

/* The height now: between the closed one and the open one while it opens or closes. */
static float bar_height(kst_element_t* e, float width) {
	float closed = kst_app_bar_closed_height(e), open = kst_maxf(closed, bar_open_height(e, width));
	return closed + (open - closed) * kst_ease(K_EASE_OUT, BAR(e)->open_t);
}

float kst_app_bar_height(kst_element_t* bar, float width) { return bar_height(bar, width); }

static KSize app_bar_measure(kst_element_t* e, KSize available) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	float w = bar_width(e, available), h;
	if (cc->content) kst_measure(cc->content, kst_size(kst_maxf(0, w - ELLIPSIS_WIDTH), K_INFINITE));
	h = bar_height(e, w);
	return kst_size(w, h);
}

static float app_bar_open_height(kst_element_t* e, float width) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	KST_UNUSED(width);
	return cc->content ? cc->content->desired.height + 2 * BAR_BORDER : 0;
}

static KSize app_bar_arrange(kst_element_t* e, KSize final) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	if (cc->content) {
		if (bar_hides_commands(e)) kst_arrange(cc->content, kst_rect(0, final.height + 1, kst_maxf(0, final.width - ELLIPSIS_WIDTH), cc->content->desired.height));
		else kst_arrange(cc->content, kst_rect(0, BAR_BORDER, kst_maxf(0, final.width - ELLIPSIS_WIDTH), kst_maxf(0, final.height - 2 * BAR_BORDER)));
	}
	return final;
}

/* The three dots, and where a press opens the bar: all of a Minimal bar, the right edge of a Compact one. */
static KRect ellipsis_rect(kst_element_t* e) {
	if (bar_hides_commands(e)) return kst_local_rect(e);
	return kst_rect(e->rect.width - ELLIPSIS_WIDTH, 0, ELLIPSIS_WIDTH, e->rect.height);
}

/* Where the middle of the dots is, vertically: in line with the circles of the buttons, centred in a Minimal bar. */
static float ellipsis_y(kst_element_t* e, float row_top) {
	if (bar_hides_commands(e)) return e->rect.height * 0.5f;
	return row_top + ITEM_TOP + ITEM_CIRCLE * 0.5f;
}

static float bar_row_top(kst_element_t* e);

static void bar_paint(kst_element_t* e, kst_painter_t* p) {
	KRect r = kst_local_rect(e);
	kst_control_t* c = KST_AS(kst_control_t, e);
	KColor border = kst_ctl_border_brush(c, kst_theme_color(e, KST_TC_BACKGROUND));
	KColor fg = kst_ctl_foreground(c);
	float cx = e->rect.width - ELLIPSIS_WIDTH * 0.5f, cy = ellipsis_y(e, bar_row_top(e));
	int i;
	kst_fill_rect(p, r, bar_background(e));
	/* AppBarBorderThemeBrush, 1.5 px, on the edge toward the content */
	if (BAR(e)->on_top) kst_fill_rect(p, kst_rect(0, r.height - BAR_BORDER, r.width, BAR_BORDER), border);
	else kst_fill_rect(p, kst_rect(0, 0, r.width, BAR_BORDER), border);
	for (i = -1; i <= 1; i++) kst_fill_ellipse(p, kst_rect(cx + (float)i * ELLIPSIS_STEP - ELLIPSIS_DOT * 0.5f, cy - ELLIPSIS_DOT * 0.5f, ELLIPSIS_DOT, ELLIPSIS_DOT), fg);
}

static int bar_hit_ellipsis(kst_element_t* e, float x, float y) {
	KRect r = ellipsis_rect(e);
	return x >= r.x && y >= r.y && x < r.x + r.width && y < r.y + r.height;
}

static int bar_input(kst_element_t* e, kst_input_t* in) {
	int* pressed = &BAR(e)->ellipsis_pressed;
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			if (!kst_effectively_enabled(e)) return 1;
			*pressed = bar_hit_ellipsis(e, in->x, in->y);
			if (*pressed && e->root) kst_set_capture(e->root, e);
			/* the bar is a surface: a press on it does not reach the page under it */
			return 1;
		case KST_IN_POINTER_UP: {
			int was = *pressed;
			*pressed = 0;
			if (e->root) kst_release_capture(e->root, e);
			if (was && bar_hit_ellipsis(e, in->x, in->y)) Kapp_bar_set_is_open(e->id, !BAR(e)->is_open);
			return 1;
		}
		case KST_IN_CAPTURE_LOST:
			*pressed = 0;
			return 0;
		default:
			return 0;
	}
}

void kst_app_bar_item_clicked(kst_element_t* item) {
	/* a secondary command was chosen: the menu goes */
	if (kst_is_a(item, K_CLASS_APP_BAR_BUTTON) && item_of(item)->overflow) {
		kst_element_t* p;
		for (p = item->parent; p; p = p->parent) {
			if (kst_is_a(p, K_CLASS_APP_BAR)) {
				Kapp_bar_set_is_open(p->id, K_FALSE);
				return;
			}
		}
	}
}

static void bar_sync_compact(kst_element_t* e);

static void bar_tick(kst_element_t* e, float dt) {
	kst_app_bar_t* b = BAR(e);
	float goal = b->is_open ? 1.0f : 0.0f;
	if (b->open_t == goal) { e->wants_tick = 0; return; }
	if (goal > b->open_t) b->open_t = kst_minf(goal, b->open_t + dt / BAR_OPEN_MS);
	else b->open_t = kst_maxf(goal, b->open_t - dt / BAR_OPEN_MS);
	bar_sync_compact(e);
	kst_changed_layout(e);
	if (b->open_t == goal) e->wants_tick = 0;
}

static void app_bar_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	BAR(e)->closed_mode = K_APP_BAR_CLOSED_COMPACT;
	e->focusable = 0;
	e->clip_to_bounds = 1;
	e->halign = K_HALIGN_STRETCH;
	e->valign = K_VALIGN_TOP;
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
}

kst_class_t kst_class_app_bar = {
	.base = &kst_class_content_control, .name = "AppBar", .id = K_CLASS_APP_BAR, .size = sizeof(kst_app_bar_t),
	.init = app_bar_init, .measure = app_bar_measure, .arrange = app_bar_arrange, .paint = bar_paint, .input = bar_input, .tick = bar_tick
};

Kid Kapp_bar_create(void) {
	kst_element_t* e = kst_create(K_CLASS_APP_BAR);
	return e ? e->id : K_INVALID_ID;
}

static kst_app_bar_t* get_bar(Kid id) { return BAR(kst_get(id, K_CLASS_APP_BAR)); }

void Kapp_bar_set_is_open(Kid id, int is_open) {
	kst_app_bar_t* b = get_bar(id);
	kst_element_t* e;
	float goal;
	if (!b) return;
	e = &b->base.base.base;
	is_open = is_open != 0;
	if (b->is_open == is_open) return;
	b->is_open = is_open;
	goal = is_open ? 1.0f : 0.0f;
	if (e->root) {
		if (is_open) e->root->open_bar = e;
		else if (e->root->open_bar == e) e->root->open_bar = NULL;
	}
	if (e->root && kst_animations_enabled(e->root)) {
		e->wants_tick = 1;
	} else {
		b->open_t = goal;
		e->wants_tick = 0;
	}
	bar_sync_compact(e);
	kst_changed_layout(e);
	kst_raise(e, is_open ? K_EVENT_OPENED : K_EVENT_CLOSED);
}

int Kapp_bar_get_is_open(Kid id) {
	kst_app_bar_t* b = get_bar(id);
	return b ? b->is_open : 0;
}

void Kapp_bar_set_is_sticky(Kid id, int is_sticky) {
	kst_app_bar_t* b = get_bar(id);
	if (b) b->is_sticky = is_sticky != 0;
}

int Kapp_bar_get_is_sticky(Kid id) {
	kst_app_bar_t* b = get_bar(id);
	return b ? b->is_sticky : 0;
}

void Kapp_bar_set_closed_display_mode(Kid id, KAppBarClosedDisplayMode mode) {
	kst_app_bar_t* b = get_bar(id);
	if (!b) return;
	if (mode != K_APP_BAR_CLOSED_COMPACT && mode != K_APP_BAR_CLOSED_MINIMAL) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "unknown ClosedDisplayMode 0x%x", (unsigned)mode);
		return;
	}
	b->closed_mode = mode;
	bar_sync_compact(&b->base.base.base);
	kst_changed_layout(&b->base.base.base);
}

KAppBarClosedDisplayMode Kapp_bar_get_closed_display_mode(Kid id) {
	kst_app_bar_t* b = get_bar(id);
	return b ? b->closed_mode : K_APP_BAR_CLOSED_COMPACT;
}

void Kapp_bar_on_opened(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_APP_BAR)) Kelement_on(id, K_EVENT_OPENED, callback, user_data);
}

void Kapp_bar_on_closed(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_APP_BAR)) Kelement_on(id, K_EVENT_CLOSED, callback, user_data);
}


/* ============================================================================
 *  CommandBar
 *  The primary commands in a row at the right, the content at the left; open, the buttons show
 *  their labels and the secondary commands are a list of text rows over (or, on top, under) the row.
 * ============================================================================ */

static void list_remove(kst_element_t*** list, int* count, kst_element_t* child) {
	int i;
	for (i = 0; i < *count; i++) {
		if ((*list)[i] == child) {
			memmove(*list + i, *list + i + 1, (size_t)(*count - i - 1) * sizeof **list);
			(*count)--;
			return;
		}
	}
}

static void command_bar_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_command_bar_t* c = CBAR(e);
	list_remove(&c->primary, &c->primary_count, child);
	list_remove(&c->secondary, &c->secondary_count, child);
	kst_cc_child_removed(e, child);
}

static void command_bar_destroy(kst_element_t* e) {
	free(CBAR(e)->primary);
	free(CBAR(e)->secondary);
	CBAR(e)->primary = NULL;
	CBAR(e)->secondary = NULL;
}

/* The labels show as soon as the bar starts to open and go when it has closed. */
static void bar_sync_compact(kst_element_t* e) {
	kst_command_bar_t* c;
	int compact, i;
	if (!kst_is_a(e, K_CLASS_COMMAND_BAR)) return;
	c = CBAR(e);
	compact = !(c->base.is_open || c->base.open_t > 0);
	for (i = 0; i < c->primary_count; i++) {
		kst_element_t* b = c->primary[i];
		if (kst_is_a(b, K_CLASS_APP_BAR_BUTTON) || kst_is_a(b, K_CLASS_APP_BAR_TOGGLE_BUTTON)) {
			if (item_of(b)->is_compact != compact) {
				item_of(b)->is_compact = compact;
				kst_changed_layout(b);
			}
		} else if (kst_is_a(b, K_CLASS_APP_BAR_SEPARATOR)) {
			KST_AS(kst_app_bar_separator_t, b)->is_compact = compact;
		}
	}
}

/* The row of the primary commands, as tall as the tallest: closed (no labels) or open. */
static float command_row_height(kst_command_bar_t* c, int open) {
	float h = BAR_ROW;
	int i;
	for (i = 0; i < c->primary_count; i++) {
		kst_element_t* b = c->primary[i];
		if (kst_is_a(b, K_CLASS_APP_BAR_BUTTON) || kst_is_a(b, K_CLASS_APP_BAR_TOGGLE_BUTTON)) h = kst_maxf(h, item_height(b, !open));
	}
	return h;
}

static float secondary_height(kst_command_bar_t* c) {
	float h = 0;
	int i;
	if (c->secondary_count == 0) return 0;
	for (i = 0; i < c->secondary_count; i++) h += c->secondary[i]->desired.height;
	return ROW_TOP + h;
}

static float command_open_height(kst_element_t* e, float width) {
	kst_command_bar_t* c = CBAR(e);
	KST_UNUSED(width);
	return 2 * BAR_BORDER + secondary_height(c) + command_row_height(c, 1);
}

static float bar_open_height(kst_element_t* e, float width) {
	if (kst_is_a(e, K_CLASS_COMMAND_BAR)) return command_open_height(e, width);
	return app_bar_open_height(e, width);
}

/* Where the row of the buttons starts: under the border, and under the secondary list that has come into view. */
static float bar_row_top(kst_element_t* e) {
	kst_app_bar_t* b = BAR(e);
	float t = kst_ease(K_EASE_OUT, b->open_t);
	if (!kst_is_a(e, K_CLASS_COMMAND_BAR) || b->on_top) return BAR_BORDER;
	return BAR_BORDER + secondary_height(CBAR(e)) * t;
}

static KSize command_bar_measure(kst_element_t* e, KSize available) {
	kst_command_bar_t* c = CBAR(e);
	kst_content_control_t* cc = &c->base.base;
	float w = bar_width(e, available), primary_w = 0;
	int i;
	bar_sync_compact(e);
	for (i = 0; i < c->primary_count; i++) {
		kst_measure(c->primary[i], kst_size(K_INFINITE, K_INFINITE));
		primary_w += c->primary[i]->desired.width;
	}
	for (i = 0; i < c->secondary_count; i++) kst_measure(c->secondary[i], kst_size(w, K_INFINITE));
	if (cc->content) kst_measure(cc->content, kst_size(kst_maxf(0, w - primary_w - ELLIPSIS_WIDTH), K_INFINITE));
	return kst_size(w, bar_height(e, w));
}

static KSize command_bar_arrange(kst_element_t* e, KSize final) {
	kst_command_bar_t* c = CBAR(e);
	kst_content_control_t* cc = &c->base.base;
	float t = kst_ease(K_EASE_OUT, c->base.open_t);
	float row_top = bar_row_top(e), x = final.width - ELLIPSIS_WIDTH, primary_w = 0, sec_h = secondary_height(c), y;
	int hidden = bar_hides_commands(e), i;
	/* what a Minimal bar hides is put below its edge, where the clip of the bar cuts it */
	float away = final.height + 1;
	for (i = c->primary_count - 1; i >= 0; i--) {
		kst_element_t* b = c->primary[i];
		x -= b->desired.width;
		primary_w += b->desired.width;
		kst_arrange(b, kst_rect(x, hidden ? away : row_top, b->desired.width, b->desired.height));
	}
	if (cc->content) {
		float room = kst_maxf(0, final.width - primary_w - ELLIPSIS_WIDTH);
		kst_arrange(cc->content, kst_rect(0, hidden ? away : row_top, room, kst_minf(cc->content->desired.height, BAR_ROW)));
	}
	/* the secondary rows: on a bar at the bottom they slide down into view above the buttons, on one at the top they lie under them */
	if (c->base.on_top) y = BAR_BORDER + command_row_height(c, 1) + ROW_TOP;
	else y = BAR_BORDER + ROW_TOP - sec_h * (1 - t);
	if (c->secondary_count == 0) y = away;
	for (i = 0; i < c->secondary_count; i++) {
		kst_element_t* b = c->secondary[i];
		kst_arrange(b, kst_rect(0, hidden ? away : y, final.width, b->desired.height));
		y += b->desired.height;
	}
	return final;
}

kst_class_t kst_class_command_bar = {
	.base = &kst_class_app_bar, .name = "CommandBar", .id = K_CLASS_COMMAND_BAR, .size = sizeof(kst_command_bar_t),
	.destroy = command_bar_destroy, .measure = command_bar_measure, .arrange = command_bar_arrange, .child_removed = command_bar_child_removed
};

Kid Kcommand_bar_create(void) {
	kst_element_t* e = kst_create(K_CLASS_COMMAND_BAR);
	return e ? e->id : K_INVALID_ID;
}

static kst_command_bar_t* get_command_bar(Kid id) { return CBAR(kst_get(id, K_CLASS_COMMAND_BAR)); }

static int is_command(const kst_element_t* e) {
	return kst_is_a(e, K_CLASS_APP_BAR_BUTTON) || kst_is_a(e, K_CLASS_APP_BAR_TOGGLE_BUTTON) || kst_is_a(e, K_CLASS_APP_BAR_SEPARATOR);
}

static int add_command(Kid bar, Kid command, int secondary) {
	kst_command_bar_t* c = get_command_bar(bar);
	kst_element_t* cmd = command != K_INVALID_ID ? kst_get(command, K_CLASS_ELEMENT) : NULL;
	kst_element_t*** list;
	int* count;
	int* cap;
	if (!c || !cmd) return -1;
	if (!is_command(cmd)) {
		kst_error(K_ERROR_WRONG_CLASS, "a command is an AppBarButton, an AppBarToggleButton or an AppBarSeparator, not a %s", cmd->cls->name);
		return -1;
	}
	if (cmd->parent) {
		kst_error(K_ERROR_HAS_PARENT, "the command already has a parent");
		return -1;
	}
	list = secondary ? &c->secondary : &c->primary;
	count = secondary ? &c->secondary_count : &c->primary_count;
	cap = secondary ? &c->secondary_cap : &c->primary_cap;
	if (*count == *cap) {
		int ncap = *cap ? *cap * 2 : 4;
		kst_element_t** more = (kst_element_t**)realloc(*list, (size_t)ncap * sizeof *more);
		if (!more) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			return -1;
		}
		*list = more;
		*cap = ncap;
	}
	if (kst_is_a(cmd, K_CLASS_APP_BAR_BUTTON) || kst_is_a(cmd, K_CLASS_APP_BAR_TOGGLE_BUTTON)) item_of(cmd)->overflow = secondary;
	(*list)[*count] = cmd;
	(*count)++;
	kst_add_child(&c->base.base.base.base, cmd, -1);
	if (cmd->parent != &c->base.base.base.base) return -1;
	bar_sync_compact(&c->base.base.base.base);
	return *count - 1;
}

int Kcommand_bar_add_primary_command(Kid bar, Kid command) { return add_command(bar, command, 0); }
int Kcommand_bar_add_secondary_command(Kid bar, Kid command) { return add_command(bar, command, 1); }

int Kcommand_bar_get_primary_command_count(Kid bar) {
	kst_command_bar_t* c = get_command_bar(bar);
	return c ? c->primary_count : 0;
}

int Kcommand_bar_get_secondary_command_count(Kid bar) {
	kst_command_bar_t* c = get_command_bar(bar);
	return c ? c->secondary_count : 0;
}

Kid Kcommand_bar_get_primary_command(Kid bar, int index) {
	kst_command_bar_t* c = get_command_bar(bar);
	return c && index >= 0 && index < c->primary_count ? c->primary[index]->id : K_INVALID_ID;
}

Kid Kcommand_bar_get_secondary_command(Kid bar, int index) {
	kst_command_bar_t* c = get_command_bar(bar);
	return c && index >= 0 && index < c->secondary_count ? c->secondary[index]->id : K_INVALID_ID;
}

void Kcommand_bar_clear_primary_commands(Kid bar) {
	kst_command_bar_t* c = get_command_bar(bar);
	while (c && c->primary_count > 0) kst_destroy(c->primary[c->primary_count - 1]);
}

void Kcommand_bar_clear_secondary_commands(Kid bar) {
	kst_command_bar_t* c = get_command_bar(bar);
	while (c && c->secondary_count > 0) kst_destroy(c->secondary[c->secondary_count - 1]);
}
