/**
 * Kestrel - text.c
 * Fonts (FreeType), UTF-8, line breaking and text drawing.
 *
 * Widths and heights are computed in logical pixels from the advances of
 * the font in em units, so the layout does not depend on the scale of the
 * root; the glyph bitmaps are rendered for the device size. There is no
 * shaping (no HarfBuzz) and no kerning: text is laid out glyph by glyph.
 */

#include "internal.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef KESTREL_HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#ifndef KESTREL_FONT_DIR
#define KESTREL_FONT_DIR "fonts"
#endif

static FT_Library g_ft;
static char* g_font_dir;

enum { KST_FACE_REGULAR, KST_FACE_SEMILIGHT, KST_FACE_SEMIBOLD, KST_FACE_COUNT };

static FT_Face g_faces[KST_FACE_COUNT];

#define KST_MAX_FALLBACKS 24

typedef struct fallback_face {
	FT_Face face;
	char* path;
	int weight;
} fallback_face_t;

static fallback_face_t g_fallbacks[KST_MAX_FALLBACKS];
static int g_fallback_count;

/*
 * The files of each face, the first that opens wins: Segoe UI when the font directory has it (its
 * Windows names, or the lower-case ones of the ztOS fonts), else Selawik, the free font with the
 * same metrics that is bundled. Segoe UI Bold is not a stand-in for Semibold: it is a heavier
 * weight, so a missing Semibold stays Selawik's.
 */
#define KST_FACE_CANDIDATES 4
static const char* const g_face_files[KST_FACE_COUNT][KST_FACE_CANDIDATES] = {
	{ "Segoe UI.ttf", "segoeui.ttf", "Selawik-Regular.ttf", NULL },
	{ "Segoe UI Semilight.ttf", "segoeuisl.ttf", "Selawik-Semilight.ttf", NULL },
	{ "Segoe UI Semibold.ttf", "seguisb.ttf", "Selawik-Semibold.ttf", NULL }
};

void Kset_font_directory(const char* path) { kst_set_string(&g_font_dir, path); }

static int face_index(KFontFamily family) {
	switch (family) {
		case K_FONT_LIGHT:
		case K_FONT_SEMILIGHT: return KST_FACE_SEMILIGHT;
		case K_FONT_SEMIBOLD:
		case K_FONT_BOLD: return KST_FACE_SEMIBOLD;
		default: return KST_FACE_REGULAR;
	}
}


/* ============================================================================
 *  caches
 * ============================================================================ */

typedef struct glyph_entry {
	uint32_t key_cp;
	int32_t key_face_size;
	int used;
	/** Which face draws the glyph: 0 the Selawik face asked for, n the fallback n - 1. Valid once `resolved`. */
	int resolved;
	int fallback;
	/** Advance in em. */
	float advance;
	int has_bitmap;
	int width, height, left, top;
	uint8_t* alpha;
} glyph_entry_t;

static glyph_entry_t* g_glyphs;
static size_t g_glyph_cap, g_glyph_count;

static uint32_t hash2(uint32_t a, uint32_t b) {
	uint32_t h = a * 2654435761u ^ (b + 0x9e3779b9u + (a << 6) + (a >> 2));
	h ^= h >> 15;
	h *= 2246822519u;
	h ^= h >> 13;
	return h;
}

static glyph_entry_t* glyph_slot(uint32_t cp, int32_t face_size, int create) {
	size_t i;
	/* growing moves the entries, so it happens only when a new one is needed: pointers to existing entries stay valid across lookups */
	if (g_glyph_cap) {
		i = hash2(cp, (uint32_t)face_size) & (g_glyph_cap - 1);
		while (g_glyphs[i].used) {
			if (g_glyphs[i].key_cp == cp && g_glyphs[i].key_face_size == face_size) return &g_glyphs[i];
			i = (i + 1) & (g_glyph_cap - 1);
		}
	}
	if (!create) return NULL;
	if ((g_glyph_count + 1) * 2 >= g_glyph_cap) {
		size_t old_cap = g_glyph_cap, j;
		glyph_entry_t* old = g_glyphs;
		g_glyph_cap = old_cap ? old_cap * 2 : 1024;
		g_glyphs = (glyph_entry_t*)calloc(g_glyph_cap, sizeof *g_glyphs);
		g_glyph_count = 0;
		for (j = 0; j < old_cap; j++) {
			if (old[j].used) {
				size_t k = hash2(old[j].key_cp, (uint32_t)old[j].key_face_size) & (g_glyph_cap - 1);
				while (g_glyphs[k].used) k = (k + 1) & (g_glyph_cap - 1);
				g_glyphs[k] = old[j];
				g_glyph_count++;
			}
		}
		free(old);
	}
	i = hash2(cp, (uint32_t)face_size) & (g_glyph_cap - 1);
	while (g_glyphs[i].used) i = (i + 1) & (g_glyph_cap - 1);
	g_glyphs[i].used = 1;
	g_glyphs[i].key_cp = cp;
	g_glyphs[i].key_face_size = face_size;
	g_glyph_count++;
	return &g_glyphs[i];
}

static FT_Face load_face(int index) {
	char path[1024];
	int c;
	const char* dir = g_font_dir;
	if (g_faces[index]) return g_faces[index];
	if (!dir) dir = getenv("KESTREL_FONT_DIR");
	if (!dir || !*dir) dir = KESTREL_FONT_DIR;
	for (c = 0; c < KST_FACE_CANDIDATES && g_face_files[index][c]; c++) {
		snprintf(path, sizeof path, "%s/%s", dir, g_face_files[index][c]);
		if (FT_New_Face(g_ft, path, 0, &g_faces[index]) == 0) return g_faces[index];
		g_faces[index] = NULL;
	}
	kst_error(K_ERROR_IO, "cannot open a font for the face %d in %s", index, dir);
	return NULL;
}

int kst_text_init(void) {
#ifdef KESTREL_HAVE_FONTCONFIG
	FcInit();
#endif
	if (FT_Init_FreeType(&g_ft) != 0) {
		kst_error(K_ERROR_IO, "FreeType did not start");
		return 0;
	}
	if (!load_face(KST_FACE_REGULAR)) {
		FT_Done_FreeType(g_ft);
		g_ft = NULL;
		return 0;
	}
	return 1;
}

void kst_text_shutdown(void) {
	size_t i;
	int f;
	for (i = 0; i < g_glyph_cap; i++) if (g_glyphs[i].used) free(g_glyphs[i].alpha);
	free(g_glyphs);
	g_glyphs = NULL;
	g_glyph_cap = g_glyph_count = 0;
	for (f = 0; f < KST_FACE_COUNT; f++) {
		if (g_faces[f]) FT_Done_Face(g_faces[f]);
		g_faces[f] = NULL;
	}
	for (f = 0; f < g_fallback_count; f++) {
		FT_Done_Face(g_fallbacks[f].face);
		free(g_fallbacks[f].path);
	}
	g_fallback_count = 0;
	if (g_ft) FT_Done_FreeType(g_ft);
	g_ft = NULL;
#ifdef KESTREL_HAVE_FONTCONFIG
	FcFini();
#endif
	free(g_font_dir);
	g_font_dir = NULL;
}


/* ---- faces for the code points Selawik does not have (it is a Latin subset): found with fontconfig ---- */

static FT_Face usable_face(int face);

/* The index + 1 of a fallback face that has `cp` at the weight of `face`, 0 when there is none. */
static int find_fallback(int face, uint32_t cp) {
#ifdef KESTREL_HAVE_FONTCONFIG
	FcPattern* pattern;
	FcCharSet* set;
	FcResult result;
	FcPattern* match;
	FcChar8* file = NULL;
	int weight = face == KST_FACE_SEMIBOLD ? FC_WEIGHT_DEMIBOLD : (face == KST_FACE_SEMILIGHT ? FC_WEIGHT_LIGHT : FC_WEIGHT_REGULAR);
	int found = 0, i;
	/* a fallback already loaded for this weight that has the character */
	for (i = 0; i < g_fallback_count; i++) {
		if (g_fallbacks[i].weight == weight && FT_Get_Char_Index(g_fallbacks[i].face, cp) != 0) return i + 1;
	}
	if (g_fallback_count >= KST_MAX_FALLBACKS) return 0;
	pattern = FcPatternCreate();
	set = FcCharSetCreate();
	if (!pattern || !set) {
		if (pattern) FcPatternDestroy(pattern);
		if (set) FcCharSetDestroy(set);
		return 0;
	}
	FcCharSetAddChar(set, cp);
	FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)"sans-serif");
	FcPatternAddInteger(pattern, FC_WEIGHT, weight);
	FcPatternAddCharSet(pattern, FC_CHARSET, set);
	FcConfigSubstitute(NULL, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	match = FcFontMatch(NULL, pattern, &result);
	if (match && FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch && file) {
		FT_Face ft;
		/* the same file may serve another weight: reuse it */
		for (i = 0; i < g_fallback_count; i++) {
			if (strcmp(g_fallbacks[i].path, (const char*)file) == 0 && FT_Get_Char_Index(g_fallbacks[i].face, cp) != 0) {
				found = i + 1;
				break;
			}
		}
		if (!found && FT_New_Face(g_ft, (const char*)file, 0, &ft) == 0) {
			if (FT_Get_Char_Index(ft, cp) != 0) {
				g_fallbacks[g_fallback_count].face = ft;
				g_fallbacks[g_fallback_count].path = kst_strdup((const char*)file);
				g_fallbacks[g_fallback_count].weight = weight;
				found = ++g_fallback_count;
			} else {
				FT_Done_Face(ft);
			}
		}
	}
	if (match) FcPatternDestroy(match);
	FcPatternDestroy(pattern);
	FcCharSetDestroy(set);
	return found;
#else
	KST_UNUSED(face); KST_UNUSED(cp);
	return 0;
#endif
}

/* Decides which face draws a code point, once. */
static glyph_entry_t* glyph_resolved(int face, uint32_t cp) {
	glyph_entry_t* g = glyph_slot(cp, face, 1);
	if (!g->resolved) {
		FT_Face ft = usable_face(face);
		g->resolved = 1;
		g->fallback = 0;
		if (ft && FT_Get_Char_Index(ft, cp) == 0 && cp > 0x20) g->fallback = find_fallback(face, cp);
	}
	return g;
}

static FT_Face face_of(int face, uint32_t cp) {
	glyph_entry_t* g = glyph_resolved(face, cp);
	if (g->fallback > 0) return g_fallbacks[g->fallback - 1].face;
	return usable_face(face);
}

int kst_font_has_glyph(kst_font_t font, uint32_t cp) {
	int face = face_index(font.family);
	FT_Face ft = face_of(face, cp);
	return ft && FT_Get_Char_Index(ft, cp) != 0;
}

/* Advance of a code point in em. */
static float advance_em(int face, uint32_t cp) {
	glyph_entry_t* g = glyph_resolved(face, cp);
	if (g->advance == 0.0f) {
		FT_Face ft = face_of(face, cp);
		if (ft) {
			FT_UInt index = FT_Get_Char_Index(ft, cp);
			FT_Set_Char_Size(ft, 0, 1000 * 64, 72, 72);
			if (FT_Load_Glyph(ft, index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP) == 0) g->advance = (float)ft->glyph->linearHoriAdvance / 65536.0f / 1000.0f;
		}
		if (g->advance == 0.0f) g->advance = 0.0001f;
	}
	return g->advance;
}

static FT_Face usable_face(int face) {
	FT_Face ft = g_faces[face] ? g_faces[face] : load_face(face);
	return ft ? ft : g_faces[KST_FACE_REGULAR];
}

float kst_font_line_height(kst_font_t font) {
	FT_Face ft = usable_face(face_index(font.family));
	if (!ft) return font.size * 1.3f;
	return font.size * (float)ft->height / (float)ft->units_per_EM;
}

float kst_font_ascent(kst_font_t font) {
	FT_Face ft = usable_face(face_index(font.family));
	if (!ft) return font.size;
	return font.size * (float)ft->ascender / (float)ft->units_per_EM;
}

static float font_content_height(kst_font_t font) {
	FT_Face ft = usable_face(face_index(font.family));
	if (!ft) return font.size * 1.2f;
	return font.size * (float)(ft->ascender - ft->descender) / (float)ft->units_per_EM;
}

static glyph_entry_t* glyph_bitmap(int face, float device_size, uint32_t cp) {
	int32_t size_key = (int32_t)(device_size * 8.0f + 0.5f);
	int32_t key = (int32_t)((size_key << 3) | (face + 1) | 0x40000000);
	glyph_entry_t* g = glyph_slot(cp, key, 1);
	if (!g->has_bitmap) {
		FT_Face ft = face_of(face, cp);
		g->has_bitmap = 1;
		if (ft) {
			FT_UInt index = FT_Get_Char_Index(ft, cp);
			FT_Set_Char_Size(ft, 0, (FT_F26Dot6)(device_size * 64.0f + 0.5f), 72, 72);
			if (FT_Load_Glyph(ft, index, FT_LOAD_NO_HINTING | FT_LOAD_RENDER) == 0 && ft->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
				FT_Bitmap* bm = &ft->glyph->bitmap;
				unsigned row;
				g->width = (int)bm->width;
				g->height = (int)bm->rows;
				g->left = ft->glyph->bitmap_left;
				g->top = ft->glyph->bitmap_top;
				if (g->width > 0 && g->height > 0) {
					g->alpha = (uint8_t*)malloc((size_t)g->width * (size_t)g->height);
					if (g->alpha) {
						for (row = 0; row < bm->rows; row++) memcpy(g->alpha + (size_t)row * (size_t)g->width, bm->buffer + (size_t)row * (size_t)bm->pitch, (size_t)g->width);
					} else {
						g->width = g->height = 0;
					}
				}
			}
		}
	}
	return g;
}


/* ============================================================================
 *  UTF-8
 * ============================================================================ */

uint32_t kst_utf8_decode(const char* s, int length, int* consumed) {
	const unsigned char* u = (const unsigned char*)s;
	uint32_t cp;
	int n, i;
	if (length <= 0) { *consumed = 0; return 0; }
	if (u[0] < 0x80) { *consumed = 1; return u[0]; }
	if ((u[0] & 0xE0) == 0xC0) { n = 2; cp = u[0] & 0x1F; }
	else if ((u[0] & 0xF0) == 0xE0) { n = 3; cp = u[0] & 0x0F; }
	else if ((u[0] & 0xF8) == 0xF0) { n = 4; cp = u[0] & 0x07; }
	else { *consumed = 1; return 0xFFFD; }
	if (n > length) { *consumed = 1; return 0xFFFD; }
	for (i = 1; i < n; i++) {
		if ((u[i] & 0xC0) != 0x80) { *consumed = 1; return 0xFFFD; }
		cp = (cp << 6) | (u[i] & 0x3F);
	}
	*consumed = n;
	return cp;
}

int kst_utf8_encode(uint32_t cp, char* out) {
	if (cp < 0x80) { out[0] = (char)cp; return 1; }
	if (cp < 0x800) { out[0] = (char)(0xC0 | (cp >> 6)); out[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
	if (cp < 0x10000) { out[0] = (char)(0xE0 | (cp >> 12)); out[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
	out[0] = (char)(0xF0 | (cp >> 18)); out[1] = (char)(0x80 | ((cp >> 12) & 0x3F)); out[2] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[3] = (char)(0x80 | (cp & 0x3F));
	return 4;
}

int kst_utf8_next(const char* s, int length, int index) {
	int used;
	if (index >= length) return length;
	kst_utf8_decode(s + index, length - index, &used);
	return index + (used ? used : 1);
}

int kst_utf8_prev(const char* s, int index) {
	if (index <= 0) return 0;
	index--;
	while (index > 0 && ((unsigned char)s[index] & 0xC0) == 0x80) index--;
	return index;
}


/* ============================================================================
 *  measuring
 * ============================================================================ */

static float char_advance(kst_font_t font, uint32_t cp) {
	return advance_em(face_index(font.family), cp) * font.size + (float)font.spacing * 0.001f * font.size;
}

float kst_text_width(kst_font_t font, const char* s, int length) {
	float w = 0;
	int i = 0, used;
	while (i < length) {
		uint32_t cp = kst_utf8_decode(s + i, length - i, &used);
		if (cp == '\n' || cp == '\r') break;
		w += char_advance(font, cp);
		i += used ? used : 1;
	}
	return w;
}

int kst_text_index_at(kst_font_t font, const char* s, int length, float x) {
	float pen = 0;
	int i = 0, used;
	while (i < length) {
		uint32_t cp = kst_utf8_decode(s + i, length - i, &used);
		float a = char_advance(font, cp);
		if (x < pen + a * 0.5f) return i;
		pen += a;
		i += used ? used : 1;
	}
	return length;
}

kst_text_layout_t* kst_text_layout_new(kst_font_t font, const char* text, int length, float max_width, int wrap, int max_lines, float line_height) {
	kst_text_layout_t* l = (kst_text_layout_t*)calloc(1, sizeof *l);
	int cap = 8, i = 0;
	if (!l) return NULL;
	l->font = font;
	l->text = text;
	l->line_height = line_height > 0 ? line_height : kst_font_line_height(font);
	l->lines = (kst_line_t*)malloc((size_t)cap * sizeof *l->lines);
	if (!l->lines) { free(l); return NULL; }
	for (;;) {
		/* one paragraph, up to a newline */
		int start = i, end = i, last_space = -1, used;
		float pen = 0, width_at_space = 0;
		int line_end = -1, next = -1;
		kst_line_t line;
		for (;;) {
			uint32_t cp;
			float a;
			if (end >= length) { line_end = end; next = length; break; }
			cp = kst_utf8_decode(text + end, length - end, &used);
			if (!used) used = 1;
			if (cp == '\n') { line_end = end; next = end + 1; break; }
			if (cp == '\r') { end += used; continue; }
			a = char_advance(font, cp);
			if (wrap && max_width > 0 && pen + a > max_width + 0.01f && end > start) {
				if (last_space > start) {
					line_end = last_space;
					pen = width_at_space;
					next = last_space + 1;
				} else {
					line_end = end;
					next = end;
				}
				break;
			}
			if (cp == ' ') { last_space = end; width_at_space = pen; }
			pen += a;
			end += used;
		}
		if (next == length && line_end == length) pen = kst_text_width(font, text + start, line_end - start);
		else pen = kst_text_width(font, text + start, line_end - start);
		line.start = start;
		line.end = line_end;
		line.width = pen;
		if (l->line_count == cap) {
			kst_line_t* more;
			cap *= 2;
			more = (kst_line_t*)realloc(l->lines, (size_t)cap * sizeof *more);
			if (!more) break;
			l->lines = more;
		}
		l->lines[l->line_count++] = line;
		if (line.width > l->width) l->width = line.width;
		i = next;
		if (max_lines > 0 && l->line_count >= max_lines) {
			if (i < length) l->truncated = 1;
			break;
		}
		if (i >= length) {
			/* a text that ends with a newline has an empty last line */
			if (next == line_end + 1 && next == length && line_end < length && text[line_end] == '\n') {
				kst_line_t empty = { length, length, 0 };
				if (l->line_count < cap) l->lines[l->line_count++] = empty;
			}
			break;
		}
	}
	l->height = (float)l->line_count * l->line_height;
	return l;
}

void kst_text_layout_free(kst_text_layout_t* l) {
	if (!l) return;
	free(l->lines);
	free(l);
}


/* ============================================================================
 *  drawing
 * ============================================================================ */

void kst_blit_alpha(kst_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, KColor c);

void kst_draw_string(kst_painter_t* p, kst_font_t font, const char* s, int length, float x, float y, KColor c) {
	int face = face_index(font.family), i = 0, used;
	float device_size = font.size * p->scale;
	float pen = p->tx + x * p->scale;
	float baseline = p->ty + (y + font_content_height(font) * 0 + kst_font_ascent(font)) * p->scale;
	if (c.a * p->alpha <= 0.001f || device_size < 1.0f) return;
	while (i < length) {
		uint32_t cp = kst_utf8_decode(s + i, length - i, &used);
		if (!used) used = 1;
		i += used;
		if (cp == '\n' || cp == '\r') continue;
		if (cp != ' ' && cp != '\t') {
			glyph_entry_t* g = glyph_bitmap(face, device_size, cp);
			if (g->alpha) kst_blit_alpha(p, (int)floorf(pen + 0.5f) + g->left, (int)floorf(baseline + 0.5f) - g->top, g->width, g->height, g->alpha, g->width, c);
		}
		pen += char_advance(font, cp) * p->scale;
	}
}

void kst_draw_text(kst_painter_t* p, const kst_text_layout_t* l, float x, float y, float width, KTextAlignment align, KTextTrimming trim, KColor c) {
	int i;
	/* the glyphs sit in the line box: centered in a custom line height */
	float shift = (l->line_height - kst_font_line_height(l->font)) * 0.5f;
	for (i = 0; i < l->line_count; i++) {
		const kst_line_t* line = &l->lines[i];
		float lx = x, ly = y + (float)i * l->line_height + shift;
		int len = line->end - line->start;
		float lw = line->width;
		int last = (i == l->line_count - 1);
		if (trim == K_TRIM_WORD_ELLIPSIS && width > 0 && (lw > width + 0.5f || (last && l->truncated))) {
			/* cut the line so that "..." fits */
			static const char ellipsis[] = "\xE2\x80\xA6";
			float room = width - char_advance(l->font, 0x2026);
			int cut = kst_text_index_at(l->font, l->text + line->start, len, kst_maxf(room, 0));
			char buffer[1024];
			if (cut > 0 && cut < (int)sizeof buffer - 4) {
				memcpy(buffer, l->text + line->start, (size_t)cut);
				memcpy(buffer + cut, ellipsis, 4);
				kst_draw_string(p, l->font, buffer, cut + 3, lx, ly, c);
				continue;
			}
		}
		if (align == K_TEXT_CENTER) lx += (width - lw) * 0.5f;
		else if (align == K_TEXT_RIGHT) lx += width - lw;
		kst_draw_string(p, l->font, l->text + line->start, len, lx, ly, c);
	}
}
