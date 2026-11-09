/**
 * RichTextBlock, minimal: paragraphs of plain text under one another with the font properties of a TextBlock.
 */

#include "test.h"

static Zid make_root(Zid visual, int theme_light) {
	Zid root = Zroot_create(480, 800);
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zelement_set_vertical_alignment(visual, Z_VALIGN_TOP);
	if (theme_light) Zroot_set_theme(root, Z_THEME_LIGHT);
	Zroot_set_visual(root, visual);
	Zroot_update_layout(root);
	return root;
}

static int lit(Zid root, ZRect r, int threshold, int light_theme) {
	ZFrame f;
	int x, y, n = 0;
	Zroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (light_theme ? px[0] < 255 - threshold : px[0] > threshold) n++;
		}
	}
	return n;
}

static void test_paragraphs_stack(void) {
	Zid rtb = Zrich_text_block_create(), root;
	ZRect a, b;
	CHECK(Zelement_is_a(rtb, Z_CLASS_ELEMENT) && !Zelement_is_a(rtb, Z_CLASS_CONTROL));
	CHECK(Zrich_text_block_add_paragraph(rtb, "first paragraph") == 0);
	CHECK(Zrich_text_block_add_paragraph(rtb, "second paragraph") == 1);
	CHECK(Zrich_text_block_get_paragraph_count(rtb) == 2);
	CHECK_STR(Zrich_text_block_get_paragraph_text(rtb, 1), "second paragraph");
	CHECK_STR(Zrich_text_block_get_paragraph_text(rtb, 5), "");
	root = make_root(rtb, 0);
	a = Zelement_get_bounds(Zrich_text_block_get_paragraph(rtb, 0));
	b = Zelement_get_bounds(Zrich_text_block_get_paragraph(rtb, 1));
	/* one under the other, no space between them, each as wide as the block */
	CHECK_NEAR(a.y, 0);
	CHECK_NEAR(b.y, a.height);
	CHECK_NEAR(a.width, 480);
	CHECK_NEAR(Zelement_get_bounds(rtb).height, a.height + b.height);
	Zrich_text_block_set_paragraph_text(rtb, 0, "changed");
	CHECK_STR(Zrich_text_block_get_paragraph_text(rtb, 0), "changed");
	Zrich_text_block_remove_paragraph_at(rtb, 0);
	CHECK(Zrich_text_block_get_paragraph_count(rtb) == 1);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(Zrich_text_block_get_paragraph(rtb, 0)).y, 0);
	Zrich_text_block_clear_paragraphs(rtb);
	CHECK(Zrich_text_block_get_paragraph_count(rtb) == 0);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(rtb).height, 0);
	Zroot_destroy(root);
}

static void test_wrapping_and_font(void) {
	Zid rtb = Zrich_text_block_create(), root;
	const char* long_text = "a long paragraph of words that has to wrap onto several lines because the block is only as wide as the screen";
	float one_line, wrapped;
	Zrich_text_block_add_paragraph(rtb, long_text);
	Zrich_text_block_set_font_size(rtb, 20);
	root = make_root(rtb, 0);
	/* wraps by default */
	wrapped = Zelement_get_bounds(rtb).height;
	Zrich_text_block_set_text_wrapping(rtb, Z_NO_WRAP);
	Zroot_update_layout(root);
	one_line = Zelement_get_bounds(rtb).height;
	CHECK(wrapped > one_line * 1.8f);
	Zrich_text_block_set_text_wrapping(rtb, Z_WRAP);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(rtb).height, wrapped);
	/* a bigger font makes more of it */
	Zrich_text_block_set_font_size(rtb, 40);
	CHECK_NEAR(Zrich_text_block_get_font_size(rtb), 40);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(rtb).height > wrapped * 1.5f);
	/* a paragraph added afterwards has the same font */
	Zrich_text_block_add_paragraph(rtb, "x");
	CHECK_NEAR(Ztext_block_set_style(Zrich_text_block_get_paragraph(rtb, 1), Z_TEXT_STYLE_PHONE_NORMAL) ? Zrich_text_block_get_font_size(rtb) : 0, 40);
	Zroot_destroy(root);
}

static void test_styles_and_line_height(void) {
	Zid rtb = Zrich_text_block_create(), root;
	float h1, h2;
	Zrich_text_block_add_paragraph(rtb, "one line");
	root = make_root(rtb, 0);
	CHECK(Zrich_text_block_set_style(rtb, Z_TEXT_STYLE_BASE));
	Zroot_update_layout(root);
	/* 16 px with a line of 21.5 */
	h1 = Zelement_get_bounds(rtb).height;
	CHECK_NEAR(h1, 21.5f);
	CHECK(Zrich_text_block_set_style(rtb, Z_TEXT_STYLE_BODY));
	Zroot_update_layout(root);
	h2 = Zelement_get_bounds(rtb).height;
	CHECK_NEAR(h2, 24);
	CHECK(!Zrich_text_block_set_style(rtb, Z_TEXT_STYLE_HEADER));
	Zrich_text_block_set_line_height(rtb, 40);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(rtb).height, 40);
	Zroot_destroy(root);
}

static void test_padding_alignment_and_color(void) {
	Zid rtb = Zrich_text_block_create(), root;
	ZRect p;
	Zrich_text_block_add_paragraph(rtb, "right");
	Zrich_text_block_set_padding(rtb, Zthickness(10, 6, 20, 4));
	CHECK(Zrich_text_block_get_padding(rtb).left == 10 && Zrich_text_block_get_padding(rtb).bottom == 4);
	Zrich_text_block_set_text_alignment(rtb, Z_TEXT_RIGHT);
	Zrich_text_block_set_foreground(rtb, Zcolor_argb(0xFFFF0000));
	Zrich_text_block_set_font_size(rtb, 30);
	root = make_root(rtb, 0);
	p = Zelement_get_bounds(Zrich_text_block_get_paragraph(rtb, 0));
	CHECK_NEAR(p.x, 10);
	CHECK_NEAR(p.y, 6);
	CHECK_NEAR(p.width, 450);
	CHECK_NEAR(Zelement_get_bounds(rtb).height, 6 + p.height + 4);
	/* red text at the right edge of the paragraph, nothing at its left, nothing in the padding */
	{
		ZFrame f;
		int x, y, red_x = -1;
		Zroot_render(root, &f);
		for (y = (int)p.y; y < (int)(p.y + p.height); y++)
			for (x = 0; x < 480; x++) {
				const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
				if (px[0] > 200 && px[1] < 40 && px[2] < 40 && x > red_x) red_x = x;
			}
		CHECK(red_x > 400 && red_x < 461);
		CHECK(lit(root, (ZRect){ 0, 0, 10, 30 }, 20, 0) == 0);
	}
	Zroot_destroy(root);
}

static void test_light_theme_text(void) {
	Zid rtb = Zrich_text_block_create(), root;
	Zrich_text_block_add_paragraph(rtb, "dark on light");
	Zrich_text_block_set_font_size(rtb, 30);
	root = make_root(rtb, 1);
	/* the foreground of Light is black */
	CHECK(lit(root, (ZRect){ 0, 0, 300, 40 }, 200, 1) > 100);
	Zroot_destroy(root);
}

static void test_picture(void) {
	Zid rtb = Zrich_text_block_create(), root;
	char path[512];
	Zrich_text_block_set_style(rtb, Z_TEXT_STYLE_BODY);
	Zrich_text_block_set_padding(rtb, Zthickness(19, 38, 19, 0));
	Zrich_text_block_add_paragraph(rtb, "The first paragraph of a RichTextBlock wraps at the width of the block and goes on under it, line after line.");
	Zrich_text_block_add_paragraph(rtb, "The second one begins a new line.");
	root = make_root(rtb, 0);
	snprintf(path, sizeof path, "%s/rich_text_block.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
	RUN(test_paragraphs_stack);
	RUN(test_wrapping_and_font);
	RUN(test_styles_and_line_height);
	RUN(test_padding_alignment_and_color);
	RUN(test_light_theme_text);
	RUN(test_picture);
	Zshutdown();
	return test_finish("test_richtext");
}
