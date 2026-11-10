/**
 * zrnui - controls/richtext.c
 * RichTextBlock, minimal: paragraphs of plain text under one another, each a TextBlock (the paragraph is the
 * block's child) that has the font, the color, the wrapping and the alignment of the block. A RichTextBlock
 * wraps its lines at its width unless TextWrapping says otherwise (the default of the 8.1 interface).
 * Not made: Runs and other inlines, OverflowContentTarget (RichTextBlockOverflow), selection, TextIndent,
 * MaxLines, LineStackingStrategy.
 */

#include "../internal.h"

#define RTB(e) ZST_AS(zrn_rich_text_block_t, e)

typedef struct zrn_rich_text_block {
	zrn_element_t base;
	unsigned set;
	float font_size;
	ZFontFamily family;
	ZColor foreground;
	ZTextWrapping wrapping;
	ZTextAlignment alignment;
	ZTextTrimming trimming;
	float line_height;
	int spacing;
	ZThickness padding;
} zrn_rich_text_block_t;

enum { SET_SIZE = 1, SET_FAMILY = 2, SET_FOREGROUND = 4, SET_LINE_HEIGHT = 8, SET_SPACING = 16 };

/* What the block says is the look of a paragraph. */
static void rtb_apply(const zrn_rich_text_block_t* const r, zrn_element_t* const tb) {
	const Zid id = tb->id;
	if (r->set & SET_SIZE) Ztext_block_set_font_size(id, r->font_size);
	if (r->set & SET_FAMILY) Ztext_block_set_font_family(id, r->family);
	if (r->set & SET_FOREGROUND) Ztext_block_set_foreground(id, r->foreground);
	if (r->set & SET_LINE_HEIGHT) Ztext_block_set_line_height(id, r->line_height);
	if (r->set & SET_SPACING) Ztext_block_set_character_spacing(id, r->spacing);
	Ztext_block_set_text_wrapping(id, r->wrapping);
	Ztext_block_set_text_alignment(id, r->alignment);
	Ztext_block_set_text_trimming(id, r->trimming);
}

static void rtb_apply_all(zrn_rich_text_block_t* const r) {
	int i;
	for (i = 0; i < r->base.child_count; i++) rtb_apply(r, r->base.children[i]);
	zrn_invalidate_deep(&r->base);
	zrn_changed_layout(&r->base);
}

static ZSize rtb_measure(zrn_element_t* const e, const ZSize available) {
	zrn_rich_text_block_t* const r = RTB(e);
	float pad_h = zrn_thickness_h(r->padding), w = 0, h = zrn_thickness_v(r->padding);
	int i;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* const p = e->children[i];
		zrn_measure(p, zrn_size(zrn_maxf(0, available.width - pad_h), Z_INFINITE));
		w = zrn_maxf(w, p->desired.width);
		h += p->desired.height;
	}
	return zrn_size(w + pad_h, h);
}

static ZSize rtb_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_rich_text_block_t* const r = RTB(e);
	float y = r->padding.top;
	int i;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* p = e->children[i];
		zrn_arrange(p, zrn_rect(r->padding.left, y, zrn_maxf(0, final.width - zrn_thickness_h(r->padding)), p->desired.height));
		y += p->desired.height;
	}
	return final;
}

static void rtb_init(zrn_element_t* const e) {
	zrn_rich_text_block_t* const r = RTB(e);
	r->wrapping = Z_WRAP;
	r->alignment = Z_TEXT_LEFT;
	r->trimming = Z_TRIM_NONE;
	r->family = Z_FONT_NORMAL;
	e->halign = Z_HALIGN_STRETCH;
}

zrn_class_t zrn_class_rich_text_block = {
	.base = &zrn_class_element, .name = "RichTextBlock", .id = Z_CLASS_RICH_TEXT_BLOCK, .size = sizeof(zrn_rich_text_block_t),
	.init = rtb_init, .measure = rtb_measure, .arrange = rtb_arrange
};

Zid Zrich_text_block_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_RICH_TEXT_BLOCK);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_rich_text_block_t* get_rtb(const Zid id) { return RTB(zrn_get(id, Z_CLASS_RICH_TEXT_BLOCK)); }

int Zrich_text_block_add_paragraph(const Zid id, const char* const text) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	Zid tb;
	zrn_element_t* p;
	if (!r) return -1;
	tb = Ztext_block_create(text ? text : "");
	p = zrn_lookup(tb);
	if (!p) return -1;
	rtb_apply(r, p);
	zrn_add_child(&r->base, p, -1);
	zrn_changed_layout(&r->base);
	return r->base.child_count - 1;
}

void Zrich_text_block_set_paragraph_text(const Zid id, const int index, const char* const text) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r || index < 0 || index >= r->base.child_count) return;
	Ztext_block_set_text(r->base.children[index]->id, text ? text : "");
}

const char* Zrich_text_block_get_paragraph_text(const Zid id, const int index) {
	const zrn_rich_text_block_t* const r = get_rtb(id);
	return r && index >= 0 && index < r->base.child_count ? Ztext_block_get_text(r->base.children[index]->id) : "";
}

int Zrich_text_block_get_paragraph_count(const Zid id) {
	const zrn_rich_text_block_t* const r = get_rtb(id);
	return r ? r->base.child_count : 0;
}

Zid Zrich_text_block_get_paragraph(const Zid id, const int index) {
	const zrn_rich_text_block_t* const r = get_rtb(id);
	return r && index >= 0 && index < r->base.child_count ? r->base.children[index]->id : Z_INVALID_ID;
}

void Zrich_text_block_remove_paragraph_at(const Zid id, const int index) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r || index < 0 || index >= r->base.child_count) return;
	zrn_destroy(r->base.children[index]);
	zrn_changed_layout(&r->base);
}

void Zrich_text_block_clear_paragraphs(const Zid id) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	while (r->base.child_count > 0) zrn_destroy(r->base.children[r->base.child_count - 1]);
	zrn_changed_layout(&r->base);
}

void Zrich_text_block_set_font_size(const Zid id, const float size) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->font_size = size;
	r->set |= SET_SIZE;
	rtb_apply_all(r);
}

float Zrich_text_block_get_font_size(const Zid id) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return 0;
	return r->set & SET_SIZE ? r->font_size : (r->base.child_count ? zrn_inherited_font_size(r->base.children[0]) : ZST_FONT_NORMAL);
}

void Zrich_text_block_set_font_family(const Zid id, const ZFontFamily family) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->family = family;
	r->set |= SET_FAMILY;
	rtb_apply_all(r);
}

void Zrich_text_block_set_foreground(const Zid id, const ZColor color) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->foreground = color;
	r->set |= SET_FOREGROUND;
	rtb_apply_all(r);
}

void Zrich_text_block_set_text_wrapping(const Zid id, const ZTextWrapping wrapping) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->wrapping = wrapping;
	rtb_apply_all(r);
}

void Zrich_text_block_set_text_alignment(const Zid id, const ZTextAlignment alignment) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->alignment = alignment;
	rtb_apply_all(r);
}

void Zrich_text_block_set_text_trimming(const Zid id, const ZTextTrimming trimming) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->trimming = trimming;
	rtb_apply_all(r);
}

void Zrich_text_block_set_line_height(const Zid id, const float line_height) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->line_height = line_height;
	r->set |= SET_LINE_HEIGHT;
	rtb_apply_all(r);
}

void Zrich_text_block_set_character_spacing(const Zid id, const int spacing) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->spacing = spacing;
	r->set |= SET_SPACING;
	rtb_apply_all(r);
}

void Zrich_text_block_set_padding(const Zid id, const ZThickness padding) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return;
	r->padding = padding;
	zrn_changed_layout(&r->base);
}

ZThickness Zrich_text_block_get_padding(const Zid id) {
	const zrn_rich_text_block_t* const r = get_rtb(id);
	return r ? r->padding : zrn_thickness_zero();
}

int Zrich_text_block_set_style(const Zid id, const ZTextStyle style) {
	zrn_rich_text_block_t* const r = get_rtb(id);
	if (!r) return false;
	/* BaseRichTextBlockStyle: PhoneFontFamilyNormal, TextStyleMediumFontSize 16, LineHeight 21.5, WrapWholeWords, Clip;
	 * BodyRichTextBlockStyle on it: 18 and a line of 24 */
	if (style == Z_TEXT_STYLE_BASE) {
		r->font_size = ZST_81_FONT_CONTROL_HEADER;
		r->line_height = 21.5f;
	} else if (style == Z_TEXT_STYLE_BODY) {
		r->font_size = 18.0f;
		r->line_height = 24.0f;
	} else {
		return false;
	}
	r->family = Z_FONT_NORMAL;
	r->wrapping = Z_WRAP;
	r->trimming = Z_TRIM_NONE;
	r->set |= SET_SIZE | SET_FAMILY | SET_LINE_HEIGHT;
	rtb_apply_all(r);
	return true;
}
