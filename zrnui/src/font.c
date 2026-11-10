/**
 * zrnui - font.c
 * Font loading. At start (zrn_fonts_init) every font that exists is registered: the files of the font
 * directory and, with fontconfig, all the fonts of the system (family, weight, slant, the code points
 * of each, from one FcFontList call). The five WP faces are resolved from that registry right away, and
 * the stand-in for a code point a face lacks is looked up in it too, so nothing is searched for while
 * text is drawn; only the FT_Face of a font that is really used is opened, on first use.
 *
 * The faces, in the order they are tried, the first that is in the registry wins:
 *   1. a file of the exact weight in the font directory (the Windows names of Segoe UI, the Segoe WP
 *      ones of the phone, the lower-case ones of the ztOS fonts);
 *   2. Segoe installed in the system ("Segoe WP", then "Segoe UI"), the nearest weight;
 *   3. a Segoe file of the font directory that stands in for the weight (Regular for the light
 *      faces, Bold for Semibold), so that a missing weight does not mix in another family;
 *   4. Selawik, the free font with the same metrics that is bundled.
 */

#include "font.h"

#include <dirent.h>

#ifdef ZRNUI_HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

#ifndef ZRNUI_FONT_DIR
#define ZRNUI_FONT_DIR "fonts"
#endif

static constexpr int ZST_FACE_CANDIDATES = 8;
#define ZST_NO_FONT UINT32_MAX

static char* g_font_dir;

/* The weights of fontconfig, spelled out so that the table below does not need fontconfig. */
enum { ZST_WEIGHT_LIGHT = 50, ZST_WEIGHT_SEMILIGHT = 55, ZST_WEIGHT_REGULAR = 80, ZST_WEIGHT_SEMIBOLD = 180, ZST_WEIGHT_BOLD = 200 };

typedef struct zrn_face_spec {
	const char* exact[ZST_FACE_CANDIDATES];
	const char* standin[ZST_FACE_CANDIDATES];
	int32_t weight;
} zrn_face_spec_t;

static const zrn_face_spec_t g_face_specs[ZST_FACE_COUNT] = {
	{
		.exact = { "Segoe UI.ttf", "segoeui.ttf", "Segoe WP.ttf", nullptr },
		.standin = { "selawk.ttf", nullptr },
		.weight = ZST_WEIGHT_REGULAR,
	},
	{
		.exact = { "Segoe UI Light.ttf", "segoeuil.ttf", nullptr },
		.standin = { "Segoe UI.ttf", "segoeui.ttf", "selawkl.ttf", nullptr },
		.weight = ZST_WEIGHT_LIGHT,
	},
	{
		.exact = { "Segoe UI Semilight.ttf", "segoeuisl.ttf", "Segoe WP SemiLight.ttf", nullptr },
		.standin = { "Segoe UI.ttf", "segoeui.ttf", "selawksl.ttf", "selawkl.ttf", nullptr },
		.weight = ZST_WEIGHT_SEMILIGHT,
	},
	{
		.exact = { "Segoe UI Semibold.ttf", "seguisb.ttf", "Segoe WP Semibold.ttf", nullptr },
		.standin = { "Segoe UI Bold.ttf", "segoeuib.ttf", "selawksb.ttf", nullptr },
		.weight = ZST_WEIGHT_SEMIBOLD,
	},
	{
		.exact = { "Segoe UI Bold.ttf", "segoeuib.ttf", nullptr },
		.standin = { "selawkb.ttf", nullptr },
		.weight = ZST_WEIGHT_BOLD,
	},
};

/* ---- the registry ---- */

typedef struct zrn_font_entry {
	char* path;
	/** The name inside the path: a font directory file is matched by it; the system ones are matched by family. */
	const char* file;
	/** System fonts only. */
	char* family;
	/** The FcCharSet of the font, owned by g_set; nullptr for the font directory files. */
	const void* charset;
	/** Opened on first use. */
	FT_Face face;
	int32_t index;
	int32_t weight;
	/** From the font directory, not from the system. */
	bool b_is_bundled;
	bool b_is_upright;
	bool b_is_mono;
	/** It did not open: never tried again. */
	bool b_is_failed;
} zrn_font_entry_t;

static FT_Library g_ft;
static zrn_font_entry_t* g_fonts;
static uint32_t g_font_count, g_font_cap;
#ifdef ZRNUI_HAVE_FONTCONFIG
static FcFontSet* g_set;
#endif
/** The registry entry of each face, ZST_NO_FONT when there is none. */
static uint32_t g_resolved[ZST_FACE_COUNT];

static zrn_font_entry_t* zrn_entry_add(void) {
	if (g_font_count < g_font_cap) {
		zrn_font_entry_t* const entry = &g_fonts[g_font_count++];
		*entry = (zrn_font_entry_t){ .index = 0 };
		return entry;
	}
	const uint32_t cap = g_font_cap ? g_font_cap * 2 : 256;
	zrn_font_entry_t* const grown = realloc(g_fonts, (size_t)cap * sizeof *grown);
	if (!grown) {
		zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
		return nullptr;
	}
	g_fonts = grown;
	g_font_cap = cap;
	return zrn_entry_add();
}

static void zrn_entry_drop_last(void) {
	const zrn_font_entry_t* const entry = &g_fonts[--g_font_count];
	free(entry->path);
	free(entry->family);
}

static bool zrn_has_suffix(const char* const name, const char* const suffix) {
	const size_t name_length = strlen(name);
	const size_t suffix_length = strlen(suffix);
	if (name_length < suffix_length) return false;
	for (size_t i = 0; i < suffix_length; i++) {
		char c = name[name_length - suffix_length + i];
		if (c >= 'A' && c <= 'Z') c = (char)(c + 32);
		if (c != suffix[i]) return false;
	}
	return true;
}

static const char* zrn_font_dir(void) {
	const char* dir = g_font_dir;
	if (!dir) dir = getenv("ZRNUI_FONT_DIR");
	if (!dir || !*dir) dir = ZRNUI_FONT_DIR;
	return dir;
}

static void zrn_scan_directory(void) {
	const char* const dir = zrn_font_dir();
	DIR* const handle = opendir(dir);
	if (!handle) return;
	const struct dirent* item;
	while ((item = readdir(handle)) != nullptr) {
		const bool b_is_font = zrn_has_suffix(item->d_name, ".ttf") || zrn_has_suffix(item->d_name, ".otf") || zrn_has_suffix(item->d_name, ".ttc");
		if (!b_is_font) continue;
		zrn_font_entry_t* const entry = zrn_entry_add();
		if (!entry) break;
		char path[1024];
		snprintf(path, sizeof path, "%s/%s", dir, item->d_name);
		entry->path = zrn_strdup(path);
		if (!entry->path) {
			g_font_count--;
			break;
		}
		entry->file = entry->path + strlen(dir) + 1;
		entry->b_is_bundled = true;
		entry->b_is_upright = true;
	}
	closedir(handle);
}

#ifdef ZRNUI_HAVE_FONTCONFIG
static void zrn_scan_system(void) {
	FcPattern* const pattern = FcPatternCreate();
	FcObjectSet* const props = FcObjectSetBuild(FC_FILE, FC_FAMILY, FC_WEIGHT, FC_SLANT, FC_SPACING, FC_INDEX, FC_CHARSET, nullptr);
	if (pattern && props) g_set = FcFontList(nullptr, pattern, props);
	if (props) FcObjectSetDestroy(props);
	if (pattern) FcPatternDestroy(pattern);
	if (!g_set) return;
	for (int32_t i = 0; i < g_set->nfont; i++) {
		const FcPattern* const font = g_set->fonts[i];
		FcChar8* file = nullptr;
		FcChar8* family = nullptr;
		FcCharSet* charset = nullptr;
		int32_t weight = ZST_WEIGHT_REGULAR;
		int32_t slant = FC_SLANT_ROMAN;
		int32_t spacing = FC_PROPORTIONAL;
		int32_t index = 0;
		if (FcPatternGetString(font, FC_FILE, 0, &file) != FcResultMatch || !file) continue;
		FcPatternGetString(font, FC_FAMILY, 0, &family);
		FcPatternGetInteger(font, FC_WEIGHT, 0, &weight);
		FcPatternGetInteger(font, FC_SLANT, 0, &slant);
		FcPatternGetInteger(font, FC_SPACING, 0, &spacing);
		FcPatternGetInteger(font, FC_INDEX, 0, &index);
		FcPatternGetCharSet(font, FC_CHARSET, 0, &charset);
		zrn_font_entry_t* const entry = zrn_entry_add();
		if (!entry) break;
		entry->path = zrn_strdup((const char*)file);
		entry->family = family ? zrn_strdup((const char*)family) : nullptr;
		if (!entry->path) {
			zrn_entry_drop_last();
			break;
		}
		entry->index = index & 0xffff;   /* the high bits of FC_INDEX select a variable instance */
		entry->weight = weight;
		entry->b_is_upright = slant == FC_SLANT_ROMAN;
		entry->b_is_mono = spacing == FC_MONO;
		entry->charset = charset;
	}
}
#endif

static uint32_t zrn_find_in_directory(const char* const* const names) {
	for (uint32_t n = 0; n < ZST_FACE_CANDIDATES && names[n]; n++) {
		for (uint32_t i = 0; i < g_font_count; i++) {
			const zrn_font_entry_t* const entry = &g_fonts[i];
			if (entry->b_is_bundled && strcmp(entry->file, names[n]) == 0) return i;
		}
	}
	return ZST_NO_FONT;
}

/* The upright Segoe of the system nearest in weight; "Segoe WP" is tried before "Segoe UI". */
static uint32_t zrn_find_system_segoe(const int32_t weight) {
	static const char* const families[] = { "Segoe WP", "Segoe UI" };
	for (size_t f = 0; f < sizeof families / sizeof families[0]; f++) {
		uint32_t best = ZST_NO_FONT;
		for (uint32_t i = 0; i < g_font_count; i++) {
			const zrn_font_entry_t* const entry = &g_fonts[i];
			const bool b_is_candidate = !entry->b_is_bundled && entry->family && entry->b_is_upright && strcmp(entry->family, families[f]) == 0;
			if (!b_is_candidate) continue;
			if (best == ZST_NO_FONT || abs(entry->weight - weight) < abs(g_fonts[best].weight - weight)) best = i;
		}
		if (best != ZST_NO_FONT) return best;
	}
	return ZST_NO_FONT;
}

static void zrn_resolve_faces(void) {
	for (uint32_t f = 0; f < ZST_FACE_COUNT; f++) {
		uint32_t found = zrn_find_in_directory(g_face_specs[f].exact);
		if (found == ZST_NO_FONT) found = zrn_find_system_segoe(g_face_specs[f].weight);
		if (found == ZST_NO_FONT) found = zrn_find_in_directory(g_face_specs[f].standin);
		g_resolved[f] = found;
	}
}

#ifdef Z_DEBUG
static void zrn_report_inventory(void) {
	uint32_t system_count = 0;
	for (uint32_t i = 0; i < g_font_count; i++) system_count += g_fonts[i].b_is_bundled ? 0 : 1;
	Z_DEBUG_LOG("fonts: %u in %s, %u in the system\n", g_font_count - system_count, zrn_font_dir(), system_count);
	for (uint32_t f = 0; f < ZST_FACE_COUNT; f++) {
		Z_DEBUG_LOG("fonts: face %u -> %s\n", f, g_resolved[f] != ZST_NO_FONT ? g_fonts[g_resolved[f]].path : "(none)");
	}
}
#endif

/* ---- opening ---- */

static FT_Face zrn_entry_face(zrn_font_entry_t* const entry) {
	if (entry->face) return entry->face;
	if (entry->b_is_failed) return nullptr;
	if (FT_New_Face(g_ft, entry->path, entry->index, &entry->face) == 0) return entry->face;
	entry->face = nullptr;
	entry->b_is_failed = true;
	zrn_error(Z_ERROR_IO, "cannot open the font %s", entry->path);
	return nullptr;
}

uint8_t zrn_face_index(const ZFontFamily family) {
	switch (family) {
		case Z_FONT_LIGHT: return ZST_FACE_LIGHT;
		case Z_FONT_SEMILIGHT: return ZST_FACE_SEMILIGHT;
		case Z_FONT_SEMIBOLD: return ZST_FACE_SEMIBOLD;
		case Z_FONT_BOLD: return ZST_FACE_BOLD;
		default: return ZST_FACE_REGULAR;
	}
}

FT_Face zrn_face(const uint8_t face) {
	if (g_resolved[face] != ZST_NO_FONT) {
		FT_Face const ft = zrn_entry_face(&g_fonts[g_resolved[face]]);
		if (ft) return ft;
	}
	if (g_resolved[ZST_FACE_REGULAR] == ZST_NO_FONT) return nullptr;
	return zrn_entry_face(&g_fonts[g_resolved[ZST_FACE_REGULAR]]);
}

/* The best font of the system that has `cp` at the weight of `face`: upright, proportional, nearest in weight. */
static FT_Face zrn_find_fallback(const uint8_t face, const uint32_t cp) {
#ifdef ZRNUI_HAVE_FONTCONFIG
	const int32_t weight = g_face_specs[face].weight;
	for (;;) {
		zrn_font_entry_t* best = nullptr;
		int32_t best_score = 0;
		for (uint32_t i = 0; i < g_font_count; i++) {
			zrn_font_entry_t* const entry = &g_fonts[i];
			const bool b_is_candidate = !entry->b_is_bundled && !entry->b_is_failed && entry->charset && FcCharSetHasChar(entry->charset, cp);
			if (!b_is_candidate) continue;
			/* an already opened font wins a tie, so that the text does not open a new font for every glyph */
			const int32_t score = abs(entry->weight - weight) + (entry->b_is_upright ? 0 : 1000) + (entry->b_is_mono ? 500 : 0) - (entry->face ? 1 : 0);
			if (best && score >= best_score) continue;
			best = entry;
			best_score = score;
		}
		if (!best) return nullptr;
		/* a font that does not open is marked failed, and the next best is taken */
		if (zrn_entry_face(best)) return best->face;
	}
#else
	ZST_UNUSED(face);
	ZST_UNUSED(cp);
	return nullptr;
#endif
}

FT_Face zrn_face_for_char(const uint8_t face, const uint32_t cp) {
	FT_Face const base = zrn_face(face);
	if (!base || cp <= 0x20 || FT_Get_Char_Index(base, cp) != 0) return base;
	FT_Face const other = zrn_find_fallback(face, cp);
	return other ? other : base;
}

/* ---- life ---- */

void Zset_font_directory(const char* const path) { zrn_set_string(&g_font_dir, path); }

bool zrn_fonts_init(void) {
#ifdef ZRNUI_HAVE_FONTCONFIG
	FcInit();
#endif
	for (uint32_t f = 0; f < ZST_FACE_COUNT; f++) g_resolved[f] = ZST_NO_FONT;
	if (FT_Init_FreeType(&g_ft) != 0) {
		zrn_error(Z_ERROR_IO, "FreeType did not start");
		return false;
	}
	zrn_scan_directory();
#ifdef ZRNUI_HAVE_FONTCONFIG
	zrn_scan_system();
#endif
	zrn_resolve_faces();
#ifdef Z_DEBUG
	zrn_report_inventory();
#endif
	if (zrn_face(ZST_FACE_REGULAR)) return true;
	zrn_error(Z_ERROR_IO, "no font for the regular face: none of %u fonts in %s or in the system fits", g_font_count, zrn_font_dir());
	zrn_fonts_shutdown();
	return false;
}

void zrn_fonts_shutdown(void) {
	for (uint32_t i = 0; i < g_font_count; i++) {
		if (g_fonts[i].face) FT_Done_Face(g_fonts[i].face);
		free(g_fonts[i].path);
		free(g_fonts[i].family);
	}
	free(g_fonts);
	g_fonts = nullptr;
	g_font_count = g_font_cap = 0;
	for (uint32_t f = 0; f < ZST_FACE_COUNT; f++) g_resolved[f] = ZST_NO_FONT;
	if (g_ft) FT_Done_FreeType(g_ft);
	g_ft = nullptr;
#ifdef ZRNUI_HAVE_FONTCONFIG
	if (g_set) FcFontSetDestroy(g_set);
	g_set = nullptr;
	FcFini();
#endif
	free(g_font_dir);
	g_font_dir = nullptr;
}
