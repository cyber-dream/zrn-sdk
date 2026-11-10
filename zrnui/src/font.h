/**
 * zrnui - font.h
 * Font loading: the inventory of the fonts that exist (the font directory and the system),
 * the five WP faces resolved from it and the faces that stand in for the code points they lack.
 * Text layout and drawing (text.c) only asks this module for an FT_Face.
 */

#ifndef ZRNUI_FONT_H
#define ZRNUI_FONT_H

#include "internal.h"

#include <ft2build.h>
#include FT_FREETYPE_H

enum zst_face { ZST_FACE_REGULAR, ZST_FACE_LIGHT, ZST_FACE_SEMILIGHT, ZST_FACE_SEMIBOLD, ZST_FACE_BOLD, ZST_FACE_COUNT };

/**
 * Starts FreeType and fontconfig, scans every font there is (font directory, then the system) and
 * resolves the five faces from that inventory. Nothing is searched for after this call. Fails when
 * not even the regular face can be found.
 */
bool zrn_fonts_init(void);
void zrn_fonts_shutdown(void);

/** The face index of a font family. */
uint8_t zrn_face_index(ZFontFamily family);
/** The face; a weight that has no font uses the regular one. Never NULL after zrn_fonts_init. */
FT_Face zrn_face(uint8_t face);
/**
 * The face that draws `cp` for `face`: the face itself when it has the glyph, else the best font of
 * the inventory that has it, else the face itself (the missing-glyph box).
 */
FT_Face zrn_face_for_char(uint8_t face, uint32_t cp);

#endif
