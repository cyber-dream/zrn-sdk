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

FT_Library g_ft;
char* g_font_dir;

enum zst_face { ZST_FACE_REGULAR, ZST_FACE_LIGHT, ZST_FACE_SEMILIGHT, ZST_FACE_SEMIBOLD, ZST_FACE_BOLD, ZST_FACE_COUNT };

FT_Face g_faces[ZST_FACE_COUNT];

#define ZST_MAX_FALLBACKS 24

typedef struct fallback_face {
	FT_Face face;
	char* path;
	int weight;
} fallback_face_t;

fallback_face_t g_fallbacks[ZST_MAX_FALLBACKS];
int g_fallback_count;

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

const struct zst_face_spec g_face_specs[ZST_FACE_COUNT] = {
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

int face_index(const ZFontFamily family) {
	switch (family) {
		case Z_FONT_LIGHT: return ZST_FACE_LIGHT;
		case Z_FONT_SEMILIGHT: return ZST_FACE_SEMILIGHT;
		case Z_FONT_SEMIBOLD: return ZST_FACE_SEMIBOLD;
		case Z_FONT_BOLD: return ZST_FACE_BOLD;
		default: return ZST_FACE_REGULAR;
	}
}

FT_Face open_in_dir(const char* dir, const char* name) {
	char path[1024];
	FT_Face face = NULL;
	snprintf(path, sizeof path, "%s/%s", dir, name);
	if (FT_New_Face(g_ft, path, 0, &face) != 0) return NULL;
	return face;
}

/* Segoe from the system, the nearest weight to the face; NULL when fontconfig has none. */
FT_Face open_system_segoe(const int index) {
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

FT_Face load_face(const uint8_t index) {
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

