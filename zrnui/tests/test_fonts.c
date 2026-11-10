/**
 * zrnui tests - fonts: the inventory is built at Zinit, faces and stand-ins come from it.
 */

#include "test.h"

#include "../src/font.h"

static void test_faces_known_at_start(void) {
	for (uint32_t face = 0; face < ZST_FACE_COUNT; face++) CHECK(zrn_face((uint8_t)face) != nullptr);
}

static void test_family_to_face(void) {
	CHECK(zrn_face_index(Z_FONT_NORMAL) == ZST_FACE_REGULAR);
	CHECK(zrn_face_index(Z_FONT_LIGHT) == ZST_FACE_LIGHT);
	CHECK(zrn_face_index(Z_FONT_SEMILIGHT) == ZST_FACE_SEMILIGHT);
	CHECK(zrn_face_index(Z_FONT_SEMIBOLD) == ZST_FACE_SEMIBOLD);
	CHECK(zrn_face_index(Z_FONT_BOLD) == ZST_FACE_BOLD);
}

static void test_latin_uses_the_face_itself(void) {
	FT_Face const regular = zrn_face(ZST_FACE_REGULAR);
	CHECK(zrn_face_for_char(ZST_FACE_REGULAR, 'A') == regular);
	CHECK(zrn_face_for_char(ZST_FACE_REGULAR, ' ') == regular);
}

static void test_stand_in_for_a_missing_glyph(void) {
	FT_Face const regular = zrn_face(ZST_FACE_REGULAR);
	/* a code point nobody has gets the face itself: its missing-glyph box */
	CHECK(zrn_face_for_char(ZST_FACE_REGULAR, 0x10FFFD) == regular);
	/* Cyrillic: when the face lacks it, the stand-in is a font that has it (none installed: the face itself) */
	FT_Face const cyrillic = zrn_face_for_char(ZST_FACE_REGULAR, 0x0416);
	CHECK(cyrillic != nullptr);
	if (FT_Get_Char_Index(regular, 0x0416) == 0 && cyrillic != regular) CHECK(FT_Get_Char_Index(cyrillic, 0x0416) != 0);
}

static void test_repeatable_without_search(void) {
	FT_Face const first = zrn_face_for_char(ZST_FACE_BOLD, 0x0416);
	FT_Face const second = zrn_face_for_char(ZST_FACE_BOLD, 0x0416);
	CHECK(first == second);
}

int main(void) {
	CHECK(Zinit());
	RUN(test_faces_known_at_start);
	RUN(test_family_to_face);
	RUN(test_latin_uses_the_face_itself);
	RUN(test_stand_in_for_a_missing_glyph);
	RUN(test_repeatable_without_search);
	Zshutdown();
	return test_finish("test_fonts");
}
