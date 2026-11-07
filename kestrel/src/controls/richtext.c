/**
 * Kestrel - controls/richtext.c
 * RichTextBlock, minimal: paragraphs of plain text under one another, each a TextBlock (the paragraph is the
 * block's child) that has the font, the color, the wrapping and the alignment of the block. A RichTextBlock
 * wraps its lines at its width unless TextWrapping says otherwise (the default of the 8.1 interface).
 * Not made: Runs and other inlines, OverflowContentTarget (RichTextBlockOverflow), selection, TextIndent,
 * MaxLines, LineStackingStrategy.
 */

#include "../internal.h"

#define RTB(e) KST_AS(kst_rich_text_block_t, e)

typedef struct kst_rich_text_block {
	kst_element_t base;
	unsigned set;
	float font_size;
	KFontFamily family;
	KColor foreground;
	KTextWrapping wrapping;
	KTextAlignment alignment;
	KTextTrimming trimming;
	float line_height;
	int spacing;
	KThickness padding;
} kst_rich_text_block_t;

enum { SET_SIZE = 1, SET_FAMILY = 2, SET_FOREGROUND = 4, SET_LINE_HEIGHT = 8, SET_SPACING = 16 };

/* What the block says is the look of a paragraph. */
static void rtb_apply(const kst_rich_text_block_t* r, kst_element_t* tb) {
	Kid id = tb->id;
	if (r->set & SET_SIZE) Ktext_block_set_font_size(id, r->font_size);
	if (r->set & SET_FAMILY) Ktext_block_set_font_family(id, r->family);
	if (r->set & SET_FOREGROUND) Ktext_block_set_foreground(id, r->foreground);
	if (r->set & SET_LINE_HEIGHT) Ktext_block_set_line_height(id, r->line_height);
	if (r->set & SET_SPACING) Ktext_block_set_character_spacing(id, r->spacing);
	Ktext_block_set_text_wrapping(id, r->wrapping);
	Ktext_block_set_text_alignment(id, r->alignment);
	Ktext_block_set_text_trimming(id, r->trimming);
}

static void rtb_apply_all(kst_rich_text_block_t* r) {
	int i;
	for (i = 0; i < r->base.child_count; i++) rtb_apply(r, r->base.children[i]);
	kst_invalidate_deep(&r->base);
	kst_changed_layout(&r->base);
}

static KSize rtb_measure(kst_element_t* e, KSize available) {
	kst_rich_text_block_t* r = RTB(e);
	float pad_h = kst_thickness_h(r->padding), w = 0, h = kst_thickness_v(r->padding);
	int i;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* p = e->children[i];
		kst_measure(p, kst_size(kst_maxf(0, available.width - pad_h), K_INFINITE));
		w = kst_maxf(w, p->desired.width);
		h += p->desired.height;
	}
	return kst_size(w + pad_h, h);
}

static KSize rtb_arrange(kst_element_t* e, KSize final) {
	kst_rich_text_block_t* r = RTB(e);
	float y = r->padding.top;
	int i;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* p = e->children[i];
		kst_arrange(p, kst_rect(r->padding.left, y, kst_maxf(0, final.width - kst_thickness_h(r->padding)), p->desired.height));
		y += p->desired.height;
	}
	return final;
}

static void rtb_init(kst_element_t* e) {
	kst_rich_text_block_t* r = RTB(e);
	r->wrapping = K_WRAP;
	r->alignment = K_TEXT_LEFT;
	r->trimming = K_TRIM_NONE;
	r->family = K_FONT_NORMAL;
	e->halign = K_HALIGN_STRETCH;
}

kst_class_t kst_class_rich_text_block = {
	.base = &kst_class_element, .name = "RichTextBlock", .id = K_CLASS_RICH_TEXT_BLOCK, .size = sizeof(kst_rich_text_block_t),
	.init = rtb_init, .measure = rtb_measure, .arrange = rtb_arrange
};

Kid Krich_text_block_create(void) {
	kst_element_t* e = kst_create(K_CLASS_RICH_TEXT_BLOCK);
	return e ? e->id : K_INVALID_ID;
}

static kst_rich_text_block_t* get_rtb(Kid id) { return RTB(kst_get(id, K_CLASS_RICH_TEXT_BLOCK)); }

int Krich_text_block_add_paragraph(Kid id, const char* text) {
	kst_rich_text_block_t* r = get_rtb(id);
	Kid tb;
	kst_element_t* p;
	if (!r) return -1;
	tb = Ktext_block_create(text ? text : "");
	p = kst_lookup(tb);
	if (!p) return -1;
	rtb_apply(r, p);
	kst_add_child(&r->base, p, -1);
	kst_changed_layout(&r->base);
	return r->base.child_count - 1;
}

void Krich_text_block_set_paragraph_text(Kid id, int index, const char* text) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r || index < 0 || index >= r->base.child_count) return;
	Ktext_block_set_text(r->base.children[index]->id, text ? text : "");
}

const char* Krich_text_block_get_paragraph_text(Kid id, int index) {
	kst_rich_text_block_t* r = get_rtb(id);
	return r && index >= 0 && index < r->base.child_count ? Ktext_block_get_text(r->base.children[index]->id) : "";
}

int Krich_text_block_get_paragraph_count(Kid id) {
	kst_rich_text_block_t* r = get_rtb(id);
	return r ? r->base.child_count : 0;
}

Kid Krich_text_block_get_paragraph(Kid id, int index) {
	kst_rich_text_block_t* r = get_rtb(id);
	return r && index >= 0 && index < r->base.child_count ? r->base.children[index]->id : K_INVALID_ID;
}

void Krich_text_block_remove_paragraph_at(Kid id, int index) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r || index < 0 || index >= r->base.child_count) return;
	kst_destroy(r->base.children[index]);
	kst_changed_layout(&r->base);
}

void Krich_text_block_clear_paragraphs(Kid id) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	while (r->base.child_count > 0) kst_destroy(r->base.children[r->base.child_count - 1]);
	kst_changed_layout(&r->base);
}

void Krich_text_block_set_font_size(Kid id, float size) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->font_size = size;
	r->set |= SET_SIZE;
	rtb_apply_all(r);
}

float Krich_text_block_get_font_size(Kid id) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return 0;
	return r->set & SET_SIZE ? r->font_size : (r->base.child_count ? kst_inherited_font_size(r->base.children[0]) : KST_FONT_NORMAL);
}

void Krich_text_block_set_font_family(Kid id, KFontFamily family) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->family = family;
	r->set |= SET_FAMILY;
	rtb_apply_all(r);
}

void Krich_text_block_set_foreground(Kid id, KColor color) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->foreground = color;
	r->set |= SET_FOREGROUND;
	rtb_apply_all(r);
}

void Krich_text_block_set_text_wrapping(Kid id, KTextWrapping wrapping) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->wrapping = wrapping;
	rtb_apply_all(r);
}

void Krich_text_block_set_text_alignment(Kid id, KTextAlignment alignment) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->alignment = alignment;
	rtb_apply_all(r);
}

void Krich_text_block_set_text_trimming(Kid id, KTextTrimming trimming) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->trimming = trimming;
	rtb_apply_all(r);
}

void Krich_text_block_set_line_height(Kid id, float line_height) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->line_height = line_height;
	r->set |= SET_LINE_HEIGHT;
	rtb_apply_all(r);
}

void Krich_text_block_set_character_spacing(Kid id, int spacing) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->spacing = spacing;
	r->set |= SET_SPACING;
	rtb_apply_all(r);
}

void Krich_text_block_set_padding(Kid id, KThickness padding) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return;
	r->padding = padding;
	kst_changed_layout(&r->base);
}

KThickness Krich_text_block_get_padding(Kid id) {
	kst_rich_text_block_t* r = get_rtb(id);
	return r ? r->padding : kst_thickness_zero();
}

int Krich_text_block_set_style(Kid id, KTextStyle style) {
	kst_rich_text_block_t* r = get_rtb(id);
	if (!r) return K_FALSE;
	/* BaseRichTextBlockStyle: PhoneFontFamilyNormal, TextStyleMediumFontSize 16, LineHeight 21.5, WrapWholeWords, Clip;
	 * BodyRichTextBlockStyle on it: 18 and a line of 24 */
	if (style == K_TEXT_STYLE_BASE) {
		r->font_size = KST_81_FONT_CONTROL_HEADER;
		r->line_height = 21.5f;
	} else if (style == K_TEXT_STYLE_BODY) {
		r->font_size = 18.0f;
		r->line_height = 24.0f;
	} else {
		return K_FALSE;
	}
	r->family = K_FONT_NORMAL;
	r->wrapping = K_WRAP;
	r->trimming = K_TRIM_NONE;
	r->set |= SET_SIZE | SET_FAMILY | SET_LINE_HEIGHT;
	rtb_apply_all(r);
	return K_TRUE;
}
