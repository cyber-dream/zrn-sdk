/**
 * Hub and HubSection of Windows Phone 8.1: the big header, the sections side by side that a finger flings, the header of a
 * section that can be tapped. Every check goes through what a finger would do (Zroot_inject_pointer), the clock, or a pixel
 * of the rendered page.
 */

#include "test.h"

static int g_in_view, g_clicks;
static Zid g_clicked;

static void on_in_view(Zid sender, void* u) { (void)sender; (void)u; g_in_view++; }
static void on_header_click(Zid hub, Zid section, void* u) { (void)hub; (void)u; g_clicks++; g_clicked = section; }

static void run(const Zid root, const float ms) {
	for (int step = 0; (float)step * 16.0f < ms; step++) Zroot_tick(root, 16);
}

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

static void drag(Zid root, ZPoint p, float dx, float dy, int steps) {
	int i;
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	for (i = 1; i <= steps; i++) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x + dx * (float)i / (float)steps, p.y + dy * (float)i / (float)steps);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x + dx, p.y + dy);
}

static int pixel_is(Zid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

/* The rightmost pixel brighter than the page in a band of rows, -1 when there is none. */
static int rightmost_lit(Zid root, int y0, int y1) {
	ZFrame f;
	int x, y;
	Zroot_render(root, &f);
	for (x = f.width - 1; x >= 0; x--) {
		for (y = y0; y < y1; y++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] > 128) return x;
		}
	}
	return -1;
}

static int count_lit(Zid root, ZRect r) {
	ZFrame f;
	int x, y, n = 0;
	Zroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height); y++)
		for (x = (int)r.x; x < (int)(r.x + r.width); x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && x < f.width && px[0] > 128) n++;
		}
	return n;
}

/* A block of a color of a given width: the content of a section. */
static Zid block(float width, ZColor color) {
	Zid b = Zborder_create();
	Zelement_set_width(b, width);
	Zborder_set_background(b, color);
	return b;
}

static Zid g_hub, g_sec[3], g_block[3];

static Zid setup(Zid* root, ZAnimations anim) {
	ZColor colors[3] = { Zcolor_argb(0xFFC00000), Zcolor_argb(0xFF00C000), Zcolor_argb(0xFF0000C0) };
	const char* names[3] = { "first", "second", "third" };
	int i;
	*root = zst_test_root(480, 800);
	Zroot_set_renderer(*root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(*root, anim);
	g_hub = Zhub_create();
	Zhub_set_header(g_hub, "HUB");
	for (i = 0; i < 3; i++) {
		g_sec[i] = Zhub_section_create(names[i]);
		g_block[i] = block(300, colors[i]);
		Zcontent_control_set_content(g_sec[i], g_block[i]);
		CHECK(Zhub_add_section(g_hub, g_sec[i]) == i);
	}
	Zroot_set_visual(*root, g_hub);
	Zroot_update_layout(*root);
	g_in_view = g_clicks = 0;
	g_clicked = Z_INVALID_ID;
	Zhub_on_sections_in_view_changed(g_hub, on_in_view, nullptr);
	Zhub_on_section_header_click(g_hub, on_header_click, nullptr);
	return g_hub;
}

static void quiet(ZError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Zid hub = Zhub_create(), sec = Zhub_section_create("s");
	CHECK(Zelement_is_a(hub, Z_CLASS_CONTROL) && !Zelement_is_a(hub, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zelement_is_a(sec, Z_CLASS_HUB_SECTION) && Zelement_is_a(sec, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zhub_get_orientation(hub) == Z_HORIZONTAL);
	CHECK_STR(Zhub_get_header(hub), "");
	Zhub_set_header(hub, "H");
	CHECK_STR(Zhub_get_header(hub), "H");
	CHECK_STR(Zhub_section_get_header(sec), "s");
	CHECK(!Zhub_section_get_is_header_interactive(sec));
	Zhub_section_set_is_header_interactive(sec, true);
	CHECK(Zhub_section_get_is_header_interactive(sec));
	CHECK(Zhub_get_section_count(hub) == 0 && Zhub_get_section(hub, 0) == Z_INVALID_ID);
	CHECK(Zhub_add_section(hub, sec) == 0);
	{
		Zid other = Zhub_create();
		Zset_error_callback(quiet, nullptr);
		CHECK(Zhub_add_section(other, sec) == -1);
		Zset_error_callback(nullptr, nullptr);
		Zelement_destroy(other);
	}
	Zelement_destroy(hub);
}

static void test_layout_and_look(void) {
	Zid root;
	ZRect b0, b1, b2;
	ZPoint p0;
	setup(&root, Z_ANIMATIONS_OFF);
	/* the sections stand side by side, each with the margin of 19 at its left */
	b0 = Zelement_get_bounds(g_sec[0]);
	b1 = Zelement_get_bounds(g_sec[1]);
	b2 = Zelement_get_bounds(g_sec[2]);
	CHECK_NEAR(b0.x, 19);
	CHECK_NEAR(b1.x, 19 + 300 + 19);
	CHECK_NEAR(b2.x, 19 + 2 * (300 + 19));
	CHECK_NEAR(b0.width, 300);
	/* the content is under the hub header and the header of the section (5 + the line + 31.5) */
	p0 = Zelement_to_root(g_block[0], (ZPoint){ 0, 0 });
	CHECK(p0.y > 100 && p0.y < 220);
	CHECK(pixel_is(root, 150, (int)p0.y + 20, 0xC0, 0x00, 0x00));
	CHECK(pixel_is(root, 150, (int)p0.y - 3, 0, 0, 0));
	/* the hub header is white at the top, the header of the section under it, both on the page */
	CHECK(count_lit(root, (ZRect){ 15, 1, 200, 90 }) > 300);
	CHECK(count_lit(root, (ZRect){ 18, p0.y - 56, 150, 25 }) > 40);
	/* a section that is not wide enough to fill the screen leaves the next one in view */
	CHECK(Zhub_get_sections_in_view_count(g_hub) == 2);
	CHECK(Zhub_get_section_in_view(g_hub, 0) == g_sec[0] && Zhub_get_section_in_view(g_hub, 1) == g_sec[1]);
	Zroot_destroy(root);
}

static void test_section_width_defaults(void) {
	Zid root = zst_test_root(480, 800), hub = Zhub_create(), s = Zhub_section_create("wide"), t = Ztext_block_create("a very long line of words that does not fit in the screen and would run on and on if it were not told to wrap at the width it gets");
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Ztext_block_set_text_wrapping(t, Z_WRAP);
	Zcontent_control_set_content(s, t);
	Zhub_add_section(hub, s);
	Zroot_set_visual(root, hub);
	Zroot_update_layout(root);
	/* the section is never wider than the screen less its margin: the text wraps there */
	CHECK(Zelement_get_bounds(s).width <= 480 - 19 + 0.5f);
	CHECK(Zelement_get_bounds(t).height > 40);
	Zroot_destroy(root);
}

static void test_scroll_and_sections_in_view(void) {
	Zid root, sv;
	setup(&root, Z_ANIMATIONS_OFF);
	sv = Zhub_get_scroll_viewer(g_hub);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 0);
	/* a drag of 250 px to the left moves the sections by as much: the first one is not in view any more at 400 */
	drag(root, (ZPoint){ 400, 500 }, -250, 0, 25);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 250);
	CHECK(Zhub_get_sections_in_view_count(g_hub) == 3);
	drag(root, (ZPoint){ 400, 500 }, -250, 0, 25);
	CHECK(Zscroll_viewer_get_horizontal_offset(sv) > 400);
	CHECK(Zhub_get_sections_in_view_count(g_hub) == 2 && Zhub_get_section_in_view(g_hub, 0) == g_sec[1]);
	CHECK(g_in_view >= 2);
	Zroot_destroy(root);
}

static void test_inertia_reports_sections(void) {
	Zid root, sv;
	int before;
	setup(&root, Z_ANIMATIONS_ON);
	sv = Zhub_get_scroll_viewer(g_hub);
	/* a fast flick: the finger goes 120 px in 6 moves, and the sections go on by themselves, the event comes from the ticks */
	drag(root, (ZPoint){ 400, 500 }, -120, 0, 6);
	before = g_in_view;
	CHECK(Zscroll_viewer_get_horizontal_offset(sv) > 100);
	run(root, 1500);
	CHECK(Zscroll_viewer_get_horizontal_offset(sv) > 250);
	CHECK(g_in_view > before);
	Zroot_destroy(root);
}

static void test_scroll_to_section(void) {
	Zid root, sv;
	setup(&root, Z_ANIMATIONS_OFF);
	sv = Zhub_get_scroll_viewer(g_hub);
	Zhub_scroll_to_section(g_hub, 1);
	Zroot_update_layout(root);
	/* the margin box of the section at the left edge */
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 319);
	CHECK(g_in_view == 1);
	CHECK(Zhub_get_section_in_view(g_hub, 0) == g_sec[1]);
	CHECK_NEAR(Zelement_to_root(g_sec[1], (ZPoint){ 0, 0 }).x, 19);
	/* the last section cannot go further than the extent: 957 - 480 */
	Zhub_scroll_to_section(g_hub, 2);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 957 - 480);
	Zhub_scroll_to_section(g_hub, 0);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 0);
	Zhub_scroll_to_section(g_hub, 7);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 0);
	Zroot_destroy(root);
}

static void test_header_parallax(void) {
	Zid root;
	int x0, x1;
	setup(&root, Z_ANIMATIONS_OFF);
	Zhub_set_header(g_hub, "HUB HEADER");
	Zroot_update_layout(root);
	x0 = rightmost_lit(root, 5, 90);
	Zhub_scroll_to_section(g_hub, 1);
	Zroot_update_layout(root);
	x1 = rightmost_lit(root, 5, 90);
	/* 319 px of sections, the header has gone half as far */
	CHECK(x0 > 200 && x0 < 479);
	CHECK(x1 < x0 - 150 && x1 > x0 - 170);
	Zroot_destroy(root);
}

static void test_header_click(void) {
	Zid root;
	ZPoint p;
	setup(&root, Z_ANIMATIONS_OFF);
	/* where the header of the first section is: over its content, in the band that the 31.5 px leave */
	p = Zelement_to_root(g_block[0], (ZPoint){ 0, 0 });
	p.x += 10;
	p.y -= 43;
	tap(root, p);
	CHECK(g_clicks == 0);
	Zhub_section_set_is_header_interactive(g_sec[0], true);
	tap(root, p);
	CHECK(g_clicks == 1 && g_clicked == g_sec[0]);
	/* beside the text of the header and below it is not the header */
	tap(root, (ZPoint){ p.x + 200, p.y });
	CHECK(g_clicks == 1);
	tap(root, (ZPoint){ p.x, p.y + 30 });
	CHECK(g_clicks == 1);
	/* a press on the header that turns into a drag scrolls and does not click */
	{
		Zid sv = Zhub_get_scroll_viewer(g_hub);
		drag(root, p, -100, 0, 10);
		CHECK(g_clicks == 1);
		CHECK(Zscroll_viewer_get_horizontal_offset(sv) > 50);
	}
	/* released away from the header: no click */
	Zscroll_viewer_scroll_to_horizontal_offset(Zhub_get_scroll_viewer(g_hub), 0);
	Zroot_update_layout(root);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x + 3, p.y + 80);
	CHECK(g_clicks == 1);
	Zroot_destroy(root);
}

static void test_vertical(void) {
	Zid root, sv;
	ZRect b0, b1;
	setup(&root, Z_ANIMATIONS_OFF);
	Zhub_set_orientation(g_hub, Z_VERTICAL);
	CHECK(Zhub_get_orientation(g_hub) == Z_VERTICAL);
	{
		int i;
		for (i = 0; i < 3; i++) {
			Zelement_set_width(g_block[i], Z_AUTO);
			Zelement_set_height(g_block[i], 500);
		}
	}
	Zroot_update_layout(root);
	b0 = Zelement_get_bounds(g_sec[0]);
	b1 = Zelement_get_bounds(g_sec[1]);
	/* the sections are under one another, as wide as the screen less the margin, and only the first has the room for the header */
	CHECK_NEAR(b0.x, 19);
	CHECK_NEAR(b0.width, 480 - 19);
	CHECK_NEAR(b1.y, b0.y + b0.height);
	{
		ZRect c0 = Zelement_get_bounds(g_block[0]), c1 = Zelement_get_bounds(g_block[1]);
		CHECK(c0.y > 100);
		/* the second section has no placeholder: its content is 5 + a line + 31.5 under it */
		CHECK(c1.y > 55 && c1.y < 65);
		CHECK(c0.y > c1.y + 40);
	}
	sv = Zhub_get_scroll_viewer(g_hub);
	drag(root, (ZPoint){ 240, 600 }, 0, -300, 20);
	CHECK(Zscroll_viewer_get_vertical_offset(sv) > 250);
	CHECK_NEAR(Zscroll_viewer_get_horizontal_offset(sv), 0);
	Zroot_destroy(root);
}

static void test_sections_added_and_removed(void) {
	Zid root, extra;
	setup(&root, Z_ANIMATIONS_OFF);
	extra = Zhub_section_create("zero");
	Zcontent_control_set_content(extra, block(100, Zcolor_argb(0xFFFFFFFF)));
	CHECK(Zhub_insert_section(g_hub, 0, extra) == 0);
	CHECK(Zhub_get_section_count(g_hub) == 4 && Zhub_get_section(g_hub, 1) == g_sec[0]);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(g_sec[0]).x, 19 + 100 + 19);
	Zhub_remove_section_at(g_hub, 0);
	CHECK(Zhub_get_section_count(g_hub) == 3);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(g_sec[0]).x, 19);
	Zhub_clear_sections(g_hub);
	CHECK(Zhub_get_section_count(g_hub) == 0 && Zhub_get_sections_in_view_count(g_hub) == 0);
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

static void test_light_theme(void) {
	Zid root;
	setup(&root, Z_ANIMATIONS_OFF);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zroot_update_layout(root);
	/* the header is the foreground of the theme: black on white */
	{
		ZFrame f;
		int x, y, darkest = 255;
		Zroot_render(root, &f);
		for (y = 1; y < 90; y++)
			for (x = 15; x < 200; x++) {
				const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
				if (px[0] < darkest) darkest = px[0];
			}
		CHECK(darkest <= 4);
		CHECK(pixel_is(root, 330, 700, 255, 255, 255));
	}
	Zroot_destroy(root);
}

static void test_picture(void) {
	Zid root;
	char path[512];
	setup(&root, Z_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/hub.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zhub_scroll_to_section(g_hub, 1);
	snprintf(path, sizeof path, "%s/hub_scrolled.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zroot_destroy(root);
}

int main(void) {
	RUN(test_classes);
	RUN(test_layout_and_look);
	RUN(test_section_width_defaults);
	RUN(test_scroll_and_sections_in_view);
	RUN(test_inertia_reports_sections);
	RUN(test_scroll_to_section);
	RUN(test_header_parallax);
	RUN(test_header_click);
	RUN(test_vertical);
	RUN(test_sections_added_and_removed);
	RUN(test_light_theme);
	RUN(test_picture);
	return test_finish("test_hub");
}
