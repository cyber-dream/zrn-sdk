/**
 * zrnui - text.c
 * UTF-8, line breaking, glyph cache and text drawing. The fonts come from font.c.
 *
 * Widths and heights are computed in logical pixels from the advances of
 * the font in em units, so the layout does not depend on the scale of the
 * root; the glyph bitmaps are rendered for the device size. There is no
 * shaping (no HarfBuzz) and no kerning: text is laid out glyph by glyph.
 */

#include "font.h"

/* ============================================================================
 *  caches
 * ============================================================================ */

typedef struct glyph_entry {
	uint32_t key_cp;
	int32_t key_face_size;
	int used;
	/** The face that draws the glyph (a fallback font when the face asked for lacks it). Valid once `resolved`. */
	int resolved;
	FT_Face ft;
	/** Advance in em. */
	float advance;
	int has_bitmap;
	int width, height, left, top;
	uint8_t* alpha;
} glyph_entry_t;

static glyph_entry_t* g_glyphs;
static size_t g_glyph_cap, g_glyph_count;

/* The cache does not grow without end: past this many entries it is emptied and the glyphs are rasterized again on demand. */
static constexpr int ZST_GLYPH_LIMIT = 8192;

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
	if (!create) return nullptr;
	if (g_glyph_count >= ZST_GLYPH_LIMIT) glyph_flush();
	if ((g_glyph_count + 1) * 2 >= g_glyph_cap) {
		const size_t old_cap = g_glyph_cap;
		glyph_entry_t* const old = g_glyphs;
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
	if (!g_glyphs) return nullptr;
	i = hash2(cp, (uint32_t)face_size) & (g_glyph_cap - 1);
	while (g_glyphs[i].used) i = (i + 1) & (g_glyph_cap - 1);
	g_glyphs[i].used = 1;
	g_glyphs[i].key_cp = cp;
	g_glyphs[i].key_face_size = face_size;
	g_glyph_count++;
	return &g_glyphs[i];
}

bool zrn_text_init(void) { return zrn_fonts_init(); }

void zrn_text_shutdown(void) {
	for (size_t i = 0; i < g_glyph_cap; i++) if (g_glyphs[i].used) free(g_glyphs[i].alpha);
	free(g_glyphs);
	g_glyphs = nullptr;
	g_glyph_cap = g_glyph_count = 0;
	zrn_fonts_shutdown();
}

/* Decides which face draws a code point, once. */
static glyph_entry_t* glyph_resolved(const uint8_t face, const uint32_t cp) {
	glyph_entry_t* const g = glyph_slot(cp, face, 1);
	if (!g) return nullptr;
	if (!g->resolved) {
		g->resolved = 1;
		g->ft = zrn_face_for_char(face, cp);
	}
	return g;
}

static FT_Face face_of(const uint8_t face, const uint32_t cp) {
	const glyph_entry_t* const g = glyph_resolved(face, cp);
	return g ? g->ft : zrn_face(face);
}

int zrn_font_has_glyph(const zrn_font_t font, const uint32_t cp) {
	const uint8_t face = zrn_face_index(font.family);
	const FT_Face ft = face_of(face, cp);
	return ft && FT_Get_Char_Index(ft, cp) != 0;
}

/* Advance of a code point in em. */
static float advance_em(const uint8_t face, const uint32_t cp) {
	glyph_entry_t* const g = glyph_resolved(face, cp);
	if (!g) return 0.0001f;
	if (g->advance == 0.0f) {
		const FT_Face ft = face_of(face, cp);
		if (ft) {
			const FT_UInt index = FT_Get_Char_Index(ft, cp);
			FT_Set_Char_Size(ft, 0, 1000 * 64, 72, 72);
			if (FT_Load_Glyph(ft, index, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP) == 0) g->advance = (float)ft->glyph->linearHoriAdvance / 65536.0f / 1000.0f;
		}
		if (g->advance == 0.0f) g->advance = 0.0001f;
	}
	return g->advance;
}

float zrn_font_line_height(const zrn_font_t font) {
	const FT_Face ft = zrn_face(zrn_face_index(font.family));
	if (!ft) return font.size * 1.3f;
	return font.size * (float)ft->height / (float)ft->units_per_EM;
}

float zrn_font_ascent(const zrn_font_t font) {
	const FT_Face ft = zrn_face(zrn_face_index(font.family));
	if (!ft) return font.size;
	return font.size * (float)ft->ascender / (float)ft->units_per_EM;
}

static float font_content_height(const zrn_font_t font) {
	const FT_Face ft = zrn_face(zrn_face_index(font.family));
	if (!ft) return font.size * 1.2f;
	return font.size * (float)(ft->ascender - ft->descender) / (float)ft->units_per_EM;
}

static glyph_entry_t* glyph_bitmap(const uint8_t face, const float device_size, const uint32_t cp) {
	const int32_t size_key = (int32_t)(device_size * 8.0f + 0.5f);
	const int32_t key = (int32_t)((size_key << 3) | (face + 1) | 0x40000000);
	glyph_entry_t* const g = glyph_slot(cp, key, 1);
	if (!g->has_bitmap) {
		const FT_Face ft = face_of(face, cp);
		g->has_bitmap = 1;
		if (ft) {
			const FT_UInt index = FT_Get_Char_Index(ft, cp);
			FT_Set_Char_Size(ft, 0, (FT_F26Dot6)(device_size * 64.0f + 0.5f), 72, 72);
			if (FT_Load_Glyph(ft, index, FT_LOAD_NO_HINTING | FT_LOAD_RENDER) == 0 && ft->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
				FT_Bitmap* const bm = &ft->glyph->bitmap;
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

uint32_t zrn_utf8_decode(const char* const s, const int length, int* const consumed) {
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

int zrn_utf8_encode(const uint32_t cp, char* const out) {
	if (cp < 0x80) { out[0] = (char)cp; return 1; }
	if (cp < 0x800) { out[0] = (char)(0xC0 | (cp >> 6)); out[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
	if (cp < 0x10000) { out[0] = (char)(0xE0 | (cp >> 12)); out[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
	out[0] = (char)(0xF0 | (cp >> 18)); out[1] = (char)(0x80 | ((cp >> 12) & 0x3F)); out[2] = (char)(0x80 | ((cp >> 6) & 0x3F)); out[3] = (char)(0x80 | (cp & 0x3F));
	return 4;
}

int zrn_utf8_next(const char* const s, const int length, const int index) {
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

static float char_advance(const zrn_font_t font, const uint32_t cp) {
	return advance_em(zrn_face_index(font.family), cp) * font.size + (float)font.spacing * 0.001f * font.size;
}

float zrn_text_width(const zrn_font_t font, const char* const s, const int length) {
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

int zrn_text_index_at(const zrn_font_t font, const char* const s, const int length, const float x) {
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

zrn_text_layout_t* zrn_text_layout_new(const zrn_font_t font, const char* const text, const int length, const float max_width, const int wrap, const int max_lines, const float line_height) {
	zrn_text_layout_t* l = (zrn_text_layout_t*)calloc(1, sizeof *l);
	int cap = 8, i = 0;
	if (!l) return nullptr;
	l->font = font;
	l->text = text;
	l->line_height = line_height > 0 ? line_height : zrn_font_line_height(font);
	l->lines = (zrn_line_t*)malloc((size_t)cap * sizeof *l->lines);
	if (!l->lines) { free(l); return nullptr; }
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
				const zrn_line_t empty = { length, length, 0 };
				if (l->line_count < cap) l->lines[l->line_count++] = empty;
			}
			break;
		}
	}
	l->height = (float)l->line_count * l->line_height;
	return l;
}

void zrn_text_layout_free(zrn_text_layout_t* const l) {
	if (!l) return;
	free(l->lines);
	free(l);
}


/* ============================================================================
 *  drawing
 * ============================================================================ */

void zrn_blit_alpha(zrn_painter_t* p, int x, int y, int w, int h, const uint8_t* alpha, int stride, ZColor c);

void zrn_draw_string(zrn_painter_t* const p, const zrn_font_t font, const char* const s, const int length, const float x, const float y, const ZColor c) {
	const uint8_t face = zrn_face_index(font.family);
	int i = 0, used;
	const float device_size = font.size * p->scale;
	float pen = p->tx + x * p->scale;
	const float baseline = p->ty + (y + font_content_height(font) * 0 + zrn_font_ascent(font)) * p->scale;
	if (c.a * p->alpha <= 0.001f || device_size < 1.0f) return;
	while (i < length) {
		uint32_t cp = zrn_utf8_decode(s + i, length - i, &used);
		if (!used) used = 1;
		i += used;
		if (cp == '\n' || cp == '\r') continue;
		if (cp != ' ' && cp != '\t') {
			glyph_entry_t* const g = glyph_bitmap(face, device_size, cp);
			if (g->alpha) zrn_blit_alpha(p, (int)floorf(pen + 0.5f) + g->left, (int)floorf(baseline + 0.5f) - g->top, g->width, g->height, g->alpha, g->width, c);
		}
		pen += char_advance(font, cp) * p->scale;
	}
}

void zrn_draw_text(zrn_painter_t* const p, const zrn_text_layout_t* const l, const float x, const float y, const float width, const ZTextAlignment align, const ZTextTrimming trim, const ZColor c) {
	int i;
	/* the glyphs sit in the line box: centered in a custom line height */
	const float shift = (l->line_height - zrn_font_line_height(l->font)) * 0.5f;
	for (i = 0; i < l->line_count; i++) {
		const zrn_line_t* line = &l->lines[i];
		float lx = x, ly = y + (float)i * l->line_height + shift;
		const int len = line->end - line->start;
		float lw = line->width;
		const int last = (i == l->line_count - 1);
		if (trim == Z_TRIM_WORD_ELLIPSIS && width > 0 && (lw > width + 0.5f || (last && l->truncated))) {
			/* cut the line so that "..." fits */
			static const char ellipsis[] = "\xE2\x80\xA6";
			const float room = width - char_advance(l->font, 0x2026);
			const int cut = zrn_text_index_at(l->font, l->text + line->start, len, zrn_maxf(room, 0));
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
