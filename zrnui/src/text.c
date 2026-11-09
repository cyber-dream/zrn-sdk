/**
 * zrnui - text.c
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

#ifdef ZRNUI_HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#ifndef ZRNUI_FONT_DIR
#define ZRNUI_FONT_DIR "fonts"
#endif

static FT_Library g_ft;
static char* g_font_dir;

enum zst_face { ZST_FACE_REGULAR, ZST_FACE_LIGHT, ZST_FACE_SEMILIGHT, ZST_FACE_SEMIBOLD, ZST_FACE_BOLD, ZST_FACE_COUNT };

static FT_Face g_faces[ZST_FACE_COUNT];

#define ZST_MAX_FALLBACKS 24

typedef struct fallback_face {
	FT_Face face;
	char* path;
	int weight;
} fallback_face_t;

static fallback_face_t g_fallbacks[ZST_MAX_FALLBACKS];
static int g_fallback_count;

/*
 * The faces, in the order they are tried, the first that opens wins:
 *   1. a file of the exact weight in the font directory (the Windows names of Segoe UI, the Segoe WP
 *      ones of the phone, the lower-case ones of the ztOS fonts);
 *   2. Segoe installed in the system (fontconfig: "Segoe WP", then "Segoe UI"), the nearest weight;
 *   3. a Segoe file of the font directory that stands in for the weight (Regular for the light
 *      faces, Bold for Semibold), so that a missing weight does not mix in another family;
 *   4. Selawik, the free font with the same metrics that is bundled.
 */
#define ZST_FACE_CANDIDATES 8

struct zst_face_spec {
	const char* preferred[ZST_FACE_CANDIDATES];
	const char* fallback[ZST_FACE_CANDIDATES];
	int fc_weight;
};

static const struct zst_face_spec g_face_specs[ZST_FACE_COUNT] = {
	{
		.preferred = { "Segoe UI.ttf", "segoeui.ttf", "Segoe WP.ttf", NULL },
		.fallback = { "selawk.ttf", NULL },
		.fc_weight = FC_WEIGHT_REGULAR,
	},
	{
		.preferred = { "Segoe UI Light.ttf", "segoeuil.ttf", NULL },
		.fallback = { "Segoe UI.ttf", "segoeui.ttf", "selawkl.ttf", NULL },
		.fc_weight = FC_WEIGHT_LIGHT,
	},
	{
		.preferred = { "Segoe UI Semilight.ttf", "segoeuisl.ttf", "Segoe WP SemiLight.ttf", NULL },
		.fallback = { "Segoe UI.ttf", "segoeui.ttf", "selawkl.ttf", NULL },
		.fc_weight = FC_WEIGHT_SEMILIGHT,
	},
	{
		.preferred = { "Segoe UI Semibold.ttf", "seguisb.ttf", "Segoe WP Semibold.ttf", NULL },
		.fallback = { "Segoe UI Bold.ttf", "segoeuib.ttf", "selawksb.ttf", NULL },
		.fc_weight = FC_WEIGHT_SEMIBOLD,
	},
	{
		.preferred = { "Segoe UI Bold.ttf", "segoeuib.ttf", NULL },
		.fallback = { "selawkb.ttf", NULL },
		.fc_weight = FC_WEIGHT_BOLD,
	},

};


void Zset_font_directory(const char* path) { zrn_set_string(&g_font_dir, path); }

static int face_index(const ZFontFamily family) {
	switch (family) {
		case Z_FONT_LIGHT: return ZST_FACE_LIGHT;
		case Z_FONT_SEMILIGHT: return ZST_FACE_SEMILIGHT;
		case Z_FONT_SEMIBOLD: return ZST_FACE_SEMIBOLD;
		case Z_FONT_BOLD: return ZST_FACE_BOLD;
		default: return ZST_FACE_REGULAR;
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

/* The cache does not grow without end: past this many entries it is emptied and the glyphs are rasterized again on demand. */
#define ZST_GLYPH_LIMIT 8192

/* Empties the glyph cache: every bitmap is freed and the table is reset, so no entry is half-reset. */
static void glyph_flush(void) {
	if (!g_glyphs) return;
	for (size_t i = 0; i < g_glyph_cap; i++) if (g_glyphs[i].used) free(g_glyphs[i].alpha);
	memset(g_glyphs, 0, g_glyph_cap * sizeof *g_glyphs);
	g_glyph_count = 0;
}

static uint32_t hash2(const uint32_t a, const uint32_t b) {
	uint32_t h = a * 2654435761u ^ (b + 0x9e3779b9u + (a << 6) + (a >> 2));
	h ^= h >> 15;
	h *= 2246822519u;
	h ^= h >> 13;
	return h;
}

static glyph_entry_t* glyph_slot(const uint32_t cp, const int32_t face_size, const int create) {
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
	if (g_glyph_count >= ZST_GLYPH_LIMIT) glyph_flush();
	if ((g_glyph_count + 1) * 2 >= g_glyph_cap) {
		const size_t old_cap = g_glyph_cap;
		glyph_entry_t* old = g_glyphs;
		const size_t new_cap = old_cap ? old_cap * 2 : 1024;
		glyph_entry_t* fresh = (glyph_entry_t*)calloc(new_cap, sizeof *fresh);
		if (fresh) {
			g_glyph_cap = new_cap;
			g_glyphs = fresh;
			g_glyph_count = 0;
			for (size_t j = 0; j < old_cap; j++) {
				if (old[j].used) {
					size_t k = hash2(old[j].key_cp, (uint32_t)old[j].key_face_size) & (g_glyph_cap - 1);
					while (g_glyphs[k].used) k = (k + 1) & (g_glyph_cap - 1);
					g_glyphs[k] = old[j];
					g_glyph_count++;
				}
			}
			free(old);
		} else {
			/* out of memory: keep the old table, only fuller; a completely full table is emptied instead, so the probe below always finds a free slot */
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "cannot grow the glyph cache");
			if (g_glyph_count + 1 >= g_glyph_cap) glyph_flush();
		}
	}
	if (!g_glyphs) return NULL;
	i = hash2(cp, (uint32_t)face_size) & (g_glyph_cap - 1);
	while (g_glyphs[i].used) i = (i + 1) & (g_glyph_cap - 1);
	g_glyphs[i].used = 1;
	g_glyphs[i].key_cp = cp;
	g_glyphs[i].key_face_size = face_size;
	g_glyph_count++;
	return &g_glyphs[i];
}

static FT_Face open_in_dir(const char* dir, const char* name) {
	char path[1024];
	FT_Face face = NULL;
	snprintf(path, sizeof path, "%s/%s", dir, name);
	if (FT_New_Face(g_ft, path, 0, &face) != 0) return NULL;
	return face;
}

/* Segoe from the system, the nearest weight to the face; NULL when fontconfig has none. */
static FT_Face open_system_segoe(const int index) {
#ifdef ZRNUI_HAVE_FONTCONFIG
	static const char* const families[] = { "Segoe WP", "Segoe UI" };
	int weight = g_face_specs[index].fc_weight;
	size_t f;
	for (f = 0; f < sizeof families / sizeof families[0]; f++) {
		FcPattern* pattern = FcPatternCreate();
		FcPattern* match;
		FcResult result;
		FcChar8* file = NULL;
		FcChar8* family = NULL;
		FT_Face face = NULL;
		if (!pattern) return NULL;
		FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)families[f]);
		FcPatternAddInteger(pattern, FC_WEIGHT, weight);
		FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ROMAN);
		FcConfigSubstitute(NULL, pattern, FcMatchPattern);
		FcDefaultSubstitute(pattern);
		match = FcFontMatch(NULL, pattern, &result);
		/* fontconfig always answers: accept only a face that is Segoe */
		if (match && FcPatternGetString(match, FC_FAMILY, 0, &family) == FcResultMatch && family &&
			strncmp((const char*)family, "Segoe", 5) == 0 &&
			FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch && file) {
			if (FT_New_Face(g_ft, (const char*)file, 0, &face) != 0) face = NULL;
		}
		if (match) FcPatternDestroy(match);
		FcPatternDestroy(pattern);
		if (face) return face;
	}
#else
	ZST_UNUSED(index);
#endif
	return NULL;
}

static FT_Face load_face(const uint8_t index) {
	int c;
	FT_Face face;
	const char* dir = g_font_dir;
	if (g_faces[index]) return g_faces[index];
	if (!dir) dir = getenv("ZRNUI_FONT_DIR");
	if (!dir || !*dir) dir = ZRNUI_FONT_DIR;
	for (c = 0; c < ZST_FACE_CANDIDATES && g_face_specs[index].preferred[c]; c++) {
		if ((face = open_in_dir(dir, g_face_specs[index].preferred[c])) != NULL) return g_faces[index] = face;
	}
	if ((face = open_system_segoe(index)) != NULL) return g_faces[index] = face;
	for (c = 0; c < ZST_FACE_CANDIDATES && g_face_specs[index].fallback[c]; c++) {
		if ((face = open_in_dir(dir, g_face_specs[index].fallback[c])) != NULL) return g_faces[index] = face;
	}
	zrn_error(Z_ERROR_IO, "cannot open a font for the face %d in %s", index, dir);
	return NULL;
}

int zrn_text_init(void) {
#ifdef ZRNUI_HAVE_FONTCONFIG
	FcInit();
#endif
	if (FT_Init_FreeType(&g_ft) != 0) {
		zrn_error(Z_ERROR_IO, "FreeType did not start");
		return 0;
	}
	if (!load_face(ZST_FACE_REGULAR)) {
		FT_Done_FreeType(g_ft);
		g_ft = NULL;
		return 0;
	}
	return 1;
}

void zrn_text_shutdown(void) {
	int f;
	for (size_t i = 0; i < g_glyph_cap; i++) if (g_glyphs[i].used) free(g_glyphs[i].alpha);
	free(g_glyphs);
	g_glyphs = NULL;
	g_glyph_cap = g_glyph_count = 0;
	for (f = 0; f < ZST_FACE_COUNT; f++) {
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
#ifdef ZRNUI_HAVE_FONTCONFIG
	FcFini();
#endif
	free(g_font_dir);
	g_font_dir = NULL;
}


/* ---- faces for the code points Selawik does not have (it is a Latin subset): found with fontconfig ---- */

static FT_Face usable_face(uint8_t face);

/* The index + 1 of a fallback face that has `cp` at the weight of `face`, 0 when there is none. */
static int find_fallback(const uint8_t face, const uint32_t cp) {
#ifdef ZRNUI_HAVE_FONTCONFIG
	FcPattern* pattern;
	FcCharSet* set;
	FcResult result;
	FcPattern* match;
	FcChar8* file = NULL;
	int weight = g_face_specs[face].fc_weight;
	int found = 0, i;
	/* a fallback already loaded for this weight that has the character */
	for (i = 0; i < g_fallback_count; i++) {
		if (g_fallbacks[i].weight == weight && FT_Get_Char_Index(g_fallbacks[i].face, cp) != 0) return i + 1;
	}
	if (g_fallback_count >= ZST_MAX_FALLBACKS) return 0;
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
				g_fallbacks[g_fallback_count].path = zrn_strdup((const char*)file);
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
	ZST_UNUSED(face); ZST_UNUSED(cp);
	return 0;
#endif
}

/* Decides which face draws a code point, once. */
static glyph_entry_t* glyph_resolved(const uint8_t face, const uint32_t cp) {
	glyph_entry_t* g = glyph_slot(cp, face, 1);
	if (!g) return NULL;
	if (!g->resolved) {
		const FT_Face ft = usable_face(face);
		g->resolved = 1;
		g->fallback = 0;
		if (ft && FT_Get_Char_Index(ft, cp) == 0 && cp > 0x20) g->fallback = find_fallback(face, cp);
	}
	return g;
}

static FT_Face face_of(int face, uint32_t cp) {
	glyph_entry_t* g = glyph_resolved(face, cp);
	if (g && g->fallback > 0) return g_fallbacks[g->fallback - 1].face;
	return usable_face(face);
}

int zrn_font_has_glyph(zrn_font_t font, uint32_t cp) {
	int face = face_index(font.family);
	FT_Face ft = face_of(face, cp);
	return ft && FT_Get_Char_Index(ft, cp) != 0;
}

/* Advance of a code point in em. */
static float advance_em(int face, uint32_t cp) {
	glyph_entry_t* g = glyph_resolved(face, cp);
	if (!g) return 0.0001f;
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

static FT_Face usable_face(const uint8_t face) {
	const FT_Face ft = g_faces[face] ? g_faces[face] : load_face(face);
	return ft ? ft : g_faces[ZST_FACE_REGULAR];
}

float zrn_font_line_height(zrn_font_t font) {
	FT_Face ft = usable_face(face_index(font.family));
	if (!ft) return font.size * 1.3f;
	return font.size * (float)ft->height / (float)ft->units_per_EM;
}

float zrn_font_ascent(zrn_font_t font) {
	FT_Face ft = usable_face(face_index(font.family));
	if (!ft) return font.size;
	return font.size * (float)ft->ascender / (float)ft->units_per_EM;
}

static float font_content_height(zrn_font_t font) {
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

uint32_t zrn_utf8_decode(const char* s, int length, int* consumed) {
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

int zrn_utf8_encode(uint32_t cp, char* out) {
	if (cp < 0x80) { out[0] = (char)cp; return 1; }
	if (cp < 0x800) { out[0] = (char)(0xC0 | (cp >> 6)); out[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
	if (cp < 0x10000) { out[0] = (char)(0xE0 | (cp >> 12)); out[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
	out[0] = (char)(0xF0 | (cp >> 18)); out[1] = (char)(0x80 | ((cp >> 12) & 0x3F)); out[2] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[3] = (char)(0x80 | (cp & 0x3F));
	return 4;
}

int zrn_utf8_next(const char* s, int length, int index) {
	int used;
	if (index >= length) return length;
	zrn_utf8_decode(s + index, length - index, &used);
	return index + (used ? used : 1);
}

int zrn_utf8_prev(const char* s, int index) {
	if (index <= 0) return 0;
	index--;
	while (index > 0 && ((unsigned char)s[index] & 0xC0) == 0x80) index--;
	return index;
}


/* ============================================================================
 *  measuring
 * ============================================================================ */

static float char_advance(zrn_font_t font, uint32_t cp) {
	return advance_em(face_index(font.family), cp) * font.size + (float)font.spacing * 0.001f * font.size;
}

float zrn_text_width(zrn_font_t font, const char* s, int length) {
	float w = 0;
	int i = 0, used;
	while (i < length) {
		uint32_t cp = zrn_utf8_decode(s + i, length - i, &used);
		if (cp == '\n' || cp == '\r') break;
		w += char_advance(font, cp);
		i += used ? used : 1;
	}
	return w;
}

int zrn_text_index_at(zrn_font_t font, const char* s, int length, float x) {
	float pen = 0;
	int i = 0, used;
	while (i < length) {
		uint32_t cp = zrn_utf8_decode(s + i, length - i, &used);
		float a = char_advance(font, cp);
		if (x < pen + a * 0.5f) return i;
		pen += a;
		i += used ? used : 1;
	}
	return length;
}

zrn_text_layout_t* zrn_text_layout_new(zrn_font_t font, const char* text, int length, float max_width, int wrap, int max_lines, float line_height) {
	zrn_text_layout_t* l = (zrn_text_layout_t*)calloc(1, sizeof *l);
	int cap = 8, i = 0;
	if (!l) return NULL;
	l->font = font;
	l->text = text;
	l->line_height = line_height > 0 ? line_height : zrn_font_line_height(font);
	l->lines = (zrn_line_t*)malloc((size_t)cap * sizeof *l->lines);
	if (!l->lines) { free(l); return NULL; }
	for (;;) {
		/* one paragraph, up to a newline */
		int start = i, end = i, last_space = -1, used;
		float pen = 0, width_at_space = 0;
		int line_end = -1, next = -1;
		zrn_line_t line;
		for (;;) {
			uint32_t cp;
			float a;
			if (end >= length) { line_end = end; next = length; break; }
			cp = zrn_utf8_decode(text + end, length - end, &used);
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
		if (next == length && line_end == length) pen = zrn_text_width(font, text + start, line_end - start);
		else pen = zrn_text_width(font, text + start, line_end - start);
		line.start = start;
		line.end = line_end;
		line.width = pen;
		if (l->line_count == cap) {
			zrn_line_t* more;
			cap *= 2;
			more = (zrn_line_t*)realloc(l->lines, (size_t)cap * sizeof *more);
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
				zrn_line_t empty = { length, length, 0 };
				if (l->line_count < cap) l->lines[l->line_count++] = empty;
			}
			break;
		}
	}
	l->height = (float)l->line_count * l->line_height;
	return l;
}

void zrn_text_layout_free(zrn_text_layout_t* l) {
	if (!l) return;
	free(l->lines);
	free(l);
}


/* ============================================================================
 *  drawing
 * ============================================================================ */

void zrn_blit_alpha(zrn_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, ZColor c);

void zrn_draw_string(zrn_painter_t* p, const zrn_font_t font, const char* s, int length, float x, float y, ZColor c) {
	int face = face_index(font.family), i = 0, used;
	float device_size = font.size * p->scale;
	float pen = p->tx + x * p->scale;
	float baseline = p->ty + (y + font_content_height(font) * 0 + zrn_font_ascent(font)) * p->scale;
	if (c.a * p->alpha <= 0.001f || device_size < 1.0f) return;
	while (i < length) {
		uint32_t cp = zrn_utf8_decode(s + i, length - i, &used);
		if (!used) used = 1;
		i += used;
		if (cp == '\n' || cp == '\r') continue;
		if (cp != ' ' && cp != '\t') {
			glyph_entry_t* g = glyph_bitmap(face, device_size, cp);
			if (g->alpha) zrn_blit_alpha(p, (int)floorf(pen + 0.5f) + g->left, (int)floorf(baseline + 0.5f) - g->top, g->width, g->height, g->alpha, g->width, c);
		}
		pen += char_advance(font, cp) * p->scale;
	}
}

void zrn_draw_text(zrn_painter_t* p, const zrn_text_layout_t* l, float x, float y, float width, ZTextAlignment align, ZTextTrimming trim, ZColor c) {
	int i;
	/* the glyphs sit in the line box: centered in a custom line height */
	float shift = (l->line_height - zrn_font_line_height(l->font)) * 0.5f;
	for (i = 0; i < l->line_count; i++) {
		const zrn_line_t* line = &l->lines[i];
		float lx = x, ly = y + (float)i * l->line_height + shift;
		int len = line->end - line->start;
		float lw = line->width;
		int last = (i == l->line_count - 1);
		if (trim == Z_TRIM_WORD_ELLIPSIS && width > 0 && (lw > width + 0.5f || (last && l->truncated))) {
			/* cut the line so that "..." fits */
			static const char ellipsis[] = "\xE2\x80\xA6";
			float room = width - char_advance(l->font, 0x2026);
			int cut = zrn_text_index_at(l->font, l->text + line->start, len, zrn_maxf(room, 0));
			char buffer[1024];
			if (cut > 0 && cut < (int)sizeof buffer - 4) {
				memcpy(buffer, l->text + line->start, (size_t)cut);
				memcpy(buffer + cut, ellipsis, 4);
				zrn_draw_string(p, l->font, buffer, cut + 3, lx, ly, c);
				continue;
			}
		}
		if (align == Z_TEXT_CENTER) lx += (width - lw) * 0.5f;
		else if (align == Z_TEXT_RIGHT) lx += width - lw;
		zrn_draw_string(p, l->font, l->text + line->start, len, lx, ly, c);
	}
}
