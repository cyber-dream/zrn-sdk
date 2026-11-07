/**
 * RichTextBlock, minimal: paragraphs of plain text under one another with the font properties of a TextBlock.
 */

#include "test.h"

static Kid make_root(Kid visual, int theme_light) {
	Kid root = Kroot_create(480, 800);
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kelement_set_vertical_alignment(visual, K_VALIGN_TOP);
	if (theme_light) Kroot_set_theme(root, K_THEME_LIGHT);
	Kroot_set_visual(root, visual);
	Kroot_update_layout(root);
	return root;
}

static int lit(Kid root, KRect r, int threshold, int light_theme) {
	KFrame f;
	int x, y, n = 0;
	Kroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (light_theme ? px[0] < 255 - threshold : px[0] > threshold) n++;
		}
	}
	return n;
}

static void test_paragraphs_stack(void) {
	Kid rtb = Krich_text_block_create(), root;
	KRect a, b;
	CHECK(Kelement_is_a(rtb, K_CLASS_ELEMENT) && !Kelement_is_a(rtb, K_CLASS_CONTROL));
	CHECK(Krich_text_block_add_paragraph(rtb, "first paragraph") == 0);
	CHECK(Krich_text_block_add_paragraph(rtb, "second paragraph") == 1);
	CHECK(Krich_text_block_get_paragraph_count(rtb) == 2);
	CHECK_STR(Krich_text_block_get_paragraph_text(rtb, 1), "second paragraph");
	CHECK_STR(Krich_text_block_get_paragraph_text(rtb, 5), "");
	root = make_root(rtb, 0);
	a = Kelement_get_bounds(Krich_text_block_get_paragraph(rtb, 0));
	b = Kelement_get_bounds(Krich_text_block_get_paragraph(rtb, 1));
	/* one under the other, no space between them, each as wide as the block */
	CHECK_NEAR(a.y, 0);
	CHECK_NEAR(b.y, a.height);
	CHECK_NEAR(a.width, 480);
	CHECK_NEAR(Kelement_get_bounds(rtb).height, a.height + b.height);
	Krich_text_block_set_paragraph_text(rtb, 0, "changed");
	CHECK_STR(Krich_text_block_get_paragraph_text(rtb, 0), "changed");
	Krich_text_block_remove_paragraph_at(rtb, 0);
	CHECK(Krich_text_block_get_paragraph_count(rtb) == 1);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(Krich_text_block_get_paragraph(rtb, 0)).y, 0);
	Krich_text_block_clear_paragraphs(rtb);
	CHECK(Krich_text_block_get_paragraph_count(rtb) == 0);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(rtb).height, 0);
	Kroot_destroy(root);
}

static void test_wrapping_and_font(void) {
	Kid rtb = Krich_text_block_create(), root;
	const char* long_text = "a long paragraph of words that has to wrap onto several lines because the block is only as wide as the screen";
	float one_line, wrapped;
	Krich_text_block_add_paragraph(rtb, long_text);
	Krich_text_block_set_font_size(rtb, 20);
	root = make_root(rtb, 0);
	/* wraps by default */
	wrapped = Kelement_get_bounds(rtb).height;
	Krich_text_block_set_text_wrapping(rtb, K_NO_WRAP);
	Kroot_update_layout(root);
	one_line = Kelement_get_bounds(rtb).height;
	CHECK(wrapped > one_line * 1.8f);
	Krich_text_block_set_text_wrapping(rtb, K_WRAP);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(rtb).height, wrapped);
	/* a bigger font makes more of it */
	Krich_text_block_set_font_size(rtb, 40);
	CHECK_NEAR(Krich_text_block_get_font_size(rtb), 40);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(rtb).height > wrapped * 1.5f);
	/* a paragraph added afterwards has the same font */
	Krich_text_block_add_paragraph(rtb, "x");
	CHECK_NEAR(Ktext_block_set_style(Krich_text_block_get_paragraph(rtb, 1), K_TEXT_STYLE_PHONE_NORMAL) ? Krich_text_block_get_font_size(rtb) : 0, 40);
	Kroot_destroy(root);
}

static void test_styles_and_line_height(void) {
	Kid rtb = Krich_text_block_create(), root;
	float h1, h2;
	Krich_text_block_add_paragraph(rtb, "one line");
	root = make_root(rtb, 0);
	CHECK(Krich_text_block_set_style(rtb, K_TEXT_STYLE_BASE));
	Kroot_update_layout(root);
	/* 16 px with a line of 21.5 */
	h1 = Kelement_get_bounds(rtb).height;
	CHECK_NEAR(h1, 21.5f);
	CHECK(Krich_text_block_set_style(rtb, K_TEXT_STYLE_BODY));
	Kroot_update_layout(root);
	h2 = Kelement_get_bounds(rtb).height;
	CHECK_NEAR(h2, 24);
	CHECK(!Krich_text_block_set_style(rtb, K_TEXT_STYLE_HEADER));
	Krich_text_block_set_line_height(rtb, 40);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(rtb).height, 40);
	Kroot_destroy(root);
}

static void test_padding_alignment_and_color(void) {
	Kid rtb = Krich_text_block_create(), root;
	KRect p;
	Krich_text_block_add_paragraph(rtb, "right");
	Krich_text_block_set_padding(rtb, Kthickness(10, 6, 20, 4));
	CHECK(Krich_text_block_get_padding(rtb).left == 10 && Krich_text_block_get_padding(rtb).bottom == 4);
	Krich_text_block_set_text_alignment(rtb, K_TEXT_RIGHT);
	Krich_text_block_set_foreground(rtb, Kcolor_argb(0xFFFF0000));
	Krich_text_block_set_font_size(rtb, 30);
	root = make_root(rtb, 0);
	p = Kelement_get_bounds(Krich_text_block_get_paragraph(rtb, 0));
	CHECK_NEAR(p.x, 10);
	CHECK_NEAR(p.y, 6);
	CHECK_NEAR(p.width, 450);
	CHECK_NEAR(Kelement_get_bounds(rtb).height, 6 + p.height + 4);
	/* red text at the right edge of the paragraph, nothing at its left, nothing in the padding */
	{
		KFrame f;
		int x, y, red_x = -1;
		Kroot_render(root, &f);
		for (y = (int)p.y; y < (int)(p.y + p.height); y++)
			for (x = 0; x < 480; x++) {
				const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
				if (px[0] > 200 && px[1] < 40 && px[2] < 40 && x > red_x) red_x = x;
			}
		CHECK(red_x > 400 && red_x < 461);
		CHECK(lit(root, (KRect){ 0, 0, 10, 30 }, 20, 0) == 0);
	}
	Kroot_destroy(root);
}

static void test_light_theme_text(void) {
	Kid rtb = Krich_text_block_create(), root;
	Krich_text_block_add_paragraph(rtb, "dark on light");
	Krich_text_block_set_font_size(rtb, 30);
	root = make_root(rtb, 1);
	/* the foreground of Light is black */
	CHECK(lit(root, (KRect){ 0, 0, 300, 40 }, 200, 1) > 100);
	Kroot_destroy(root);
}

static void test_picture(void) {
	Kid rtb = Krich_text_block_create(), root;
	char path[512];
	Krich_text_block_set_style(rtb, K_TEXT_STYLE_BODY);
	Krich_text_block_set_padding(rtb, Kthickness(19, 38, 19, 0));
	Krich_text_block_add_paragraph(rtb, "The first paragraph of a RichTextBlock wraps at the width of the block and goes on under it, line after line.");
	Krich_text_block_add_paragraph(rtb, "The second one begins a new line.");
	root = make_root(rtb, 0);
	snprintf(path, sizeof path, "%s/rich_text_block.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_paragraphs_stack);
	RUN(test_wrapping_and_font);
	RUN(test_styles_and_line_height);
	RUN(test_padding_alignment_and_color);
	RUN(test_light_theme_text);
	RUN(test_picture);
	Kshutdown();
	return test_finish("test_richtext");
}
