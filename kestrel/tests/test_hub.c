/**
 * Hub and HubSection of Windows Phone 8.1: the big header, the sections side by side that a finger flings, the header of a
 * section that can be tapped. Every check goes through what a finger would do (Kroot_inject_pointer), the clock, or a pixel
 * of the rendered page.
 */

#include "test.h"

static int g_in_view, g_clicks;
static Kid g_clicked;

static void on_in_view(Kid sender, void* u) { (void)sender; (void)u; g_in_view++; }
static void on_header_click(Kid hub, Kid section, void* u) { (void)hub; (void)u; g_clicks++; g_clicked = section; }

static void run(Kid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

static void drag(Kid root, KPoint p, float dx, float dy, int steps) {
	int i;
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	for (i = 1; i <= steps; i++) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x + dx * (float)i / (float)steps, p.y + dy * (float)i / (float)steps);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x + dx, p.y + dy);
}

static int pixel_is(Kid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

/* The rightmost pixel brighter than the page in a band of rows, -1 when there is none. */
static int rightmost_lit(Kid root, int y0, int y1) {
	KFrame f;
	int x, y;
	Kroot_render(root, &f);
	for (x = f.width - 1; x >= 0; x--) {
		for (y = y0; y < y1; y++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] > 128) return x;
		}
	}
	return -1;
}

static int count_lit(Kid root, KRect r) {
	KFrame f;
	int x, y, n = 0;
	Kroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height); y++)
		for (x = (int)r.x; x < (int)(r.x + r.width); x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && x < f.width && px[0] > 128) n++;
		}
	return n;
}

/* A block of a color of a given width: the content of a section. */
static Kid block(float width, KColor color) {
	Kid b = Kborder_create();
	Kelement_set_width(b, width);
	Kborder_set_background(b, color);
	return b;
}

static Kid g_hub, g_sec[3], g_block[3];

static Kid setup(Kid* root, KAnimations anim) {
	KColor colors[3] = { Kcolor_argb(0xFFC00000), Kcolor_argb(0xFF00C000), Kcolor_argb(0xFF0000C0) };
	const char* names[3] = { "first", "second", "third" };
	int i;
	*root = Kroot_create(480, 800);
	Kroot_set_renderer(*root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(*root, anim);
	g_hub = Khub_create();
	Khub_set_header(g_hub, "HUB");
	for (i = 0; i < 3; i++) {
		g_sec[i] = Khub_section_create(names[i]);
		g_block[i] = block(300, colors[i]);
		Kcontent_control_set_content(g_sec[i], g_block[i]);
		CHECK(Khub_add_section(g_hub, g_sec[i]) == i);
	}
	Kroot_set_visual(*root, g_hub);
	Kroot_update_layout(*root);
	g_in_view = g_clicks = 0;
	g_clicked = K_INVALID_ID;
	Khub_on_sections_in_view_changed(g_hub, on_in_view, NULL);
	Khub_on_section_header_click(g_hub, on_header_click, NULL);
	return g_hub;
}

static void quiet(KError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Kid hub = Khub_create(), sec = Khub_section_create("s");
	CHECK(Kelement_is_a(hub, K_CLASS_CONTROL) && !Kelement_is_a(hub, K_CLASS_CONTENT_CONTROL));
	CHECK(Kelement_is_a(sec, K_CLASS_HUB_SECTION) && Kelement_is_a(sec, K_CLASS_CONTENT_CONTROL));
	CHECK(Khub_get_orientation(hub) == K_HORIZONTAL);
	CHECK_STR(Khub_get_header(hub), "");
	Khub_set_header(hub, "H");
	CHECK_STR(Khub_get_header(hub), "H");
	CHECK_STR(Khub_section_get_header(sec), "s");
	CHECK(!Khub_section_get_is_header_interactive(sec));
	Khub_section_set_is_header_interactive(sec, K_TRUE);
	CHECK(Khub_section_get_is_header_interactive(sec));
	CHECK(Khub_get_section_count(hub) == 0 && Khub_get_section(hub, 0) == K_INVALID_ID);
	CHECK(Khub_add_section(hub, sec) == 0);
	{
		Kid other = Khub_create();
		Kset_error_callback(quiet, NULL);
		CHECK(Khub_add_section(other, sec) == -1);
		Kset_error_callback(NULL, NULL);
		Kelement_destroy(other);
	}
	Kelement_destroy(hub);
}

static void test_layout_and_look(void) {
	Kid root;
	KRect b0, b1, b2;
	KPoint p0;
	setup(&root, K_ANIMATIONS_OFF);
	/* the sections stand side by side, each with the margin of 19 at its left */
	b0 = Kelement_get_bounds(g_sec[0]);
	b1 = Kelement_get_bounds(g_sec[1]);
	b2 = Kelement_get_bounds(g_sec[2]);
	CHECK_NEAR(b0.x, 19);
	CHECK_NEAR(b1.x, 19 + 300 + 19);
	CHECK_NEAR(b2.x, 19 + 2 * (300 + 19));
	CHECK_NEAR(b0.width, 300);
	/* the content is under the hub header and the header of the section (5 + the line + 31.5) */
	p0 = Kelement_to_root(g_block[0], (KPoint){ 0, 0 });
	CHECK(p0.y > 100 && p0.y < 220);
	CHECK(pixel_is(root, 150, (int)p0.y + 20, 0xC0, 0x00, 0x00));
	CHECK(pixel_is(root, 150, (int)p0.y - 3, 0, 0, 0));
	/* the hub header is white at the top, the header of the section under it, both on the page */
	CHECK(count_lit(root, (KRect){ 15, 1, 200, 90 }) > 300);
	CHECK(count_lit(root, (KRect){ 18, p0.y - 56, 150, 25 }) > 40);
	/* a section that is not wide enough to fill the screen leaves the next one in view */
	CHECK(Khub_get_sections_in_view_count(g_hub) == 2);
	CHECK(Khub_get_section_in_view(g_hub, 0) == g_sec[0] && Khub_get_section_in_view(g_hub, 1) == g_sec[1]);
	Kroot_destroy(root);
}

static void test_section_width_defaults(void) {
	Kid root = Kroot_create(480, 800), hub = Khub_create(), s = Khub_section_create("wide"), t = Ktext_block_create("a very long line of words that does not fit in the screen and would run on and on if it were not told to wrap at the width it gets");
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Ktext_block_set_text_wrapping(t, K_WRAP);
	Kcontent_control_set_content(s, t);
	Khub_add_section(hub, s);
	Kroot_set_visual(root, hub);
	Kroot_update_layout(root);
	/* the section is never wider than the screen less its margin: the text wraps there */
	CHECK(Kelement_get_bounds(s).width <= 480 - 19 + 0.5f);
	CHECK(Kelement_get_bounds(t).height > 40);
	Kroot_destroy(root);
}

static void test_scroll_and_sections_in_view(void) {
	Kid root, sv;
	setup(&root, K_ANIMATIONS_OFF);
	sv = Khub_get_scroll_viewer(g_hub);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 0);
	/* a drag of 250 px to the left moves the sections by as much: the first one is not in view any more at 400 */
	drag(root, (KPoint){ 400, 500 }, -250, 0, 25);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 250);
	CHECK(Khub_get_sections_in_view_count(g_hub) == 3);
	drag(root, (KPoint){ 400, 500 }, -250, 0, 25);
	CHECK(Kscroll_viewer_get_horizontal_offset(sv) > 400);
	CHECK(Khub_get_sections_in_view_count(g_hub) == 2 && Khub_get_section_in_view(g_hub, 0) == g_sec[1]);
	CHECK(g_in_view >= 2);
	Kroot_destroy(root);
}

static void test_inertia_reports_sections(void) {
	Kid root, sv;
	int before;
	setup(&root, K_ANIMATIONS_ON);
	sv = Khub_get_scroll_viewer(g_hub);
	/* a fast flick: the finger goes 120 px in 6 moves, and the sections go on by themselves, the event comes from the ticks */
	drag(root, (KPoint){ 400, 500 }, -120, 0, 6);
	before = g_in_view;
	CHECK(Kscroll_viewer_get_horizontal_offset(sv) > 100);
	run(root, 1500);
	CHECK(Kscroll_viewer_get_horizontal_offset(sv) > 250);
	CHECK(g_in_view > before);
	Kroot_destroy(root);
}

static void test_scroll_to_section(void) {
	Kid root, sv;
	setup(&root, K_ANIMATIONS_OFF);
	sv = Khub_get_scroll_viewer(g_hub);
	Khub_scroll_to_section(g_hub, 1);
	Kroot_update_layout(root);
	/* the margin box of the section at the left edge */
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 319);
	CHECK(g_in_view == 1);
	CHECK(Khub_get_section_in_view(g_hub, 0) == g_sec[1]);
	CHECK_NEAR(Kelement_to_root(g_sec[1], (KPoint){ 0, 0 }).x, 19);
	/* the last section cannot go further than the extent: 957 - 480 */
	Khub_scroll_to_section(g_hub, 2);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 957 - 480);
	Khub_scroll_to_section(g_hub, 0);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 0);
	Khub_scroll_to_section(g_hub, 7);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 0);
	Kroot_destroy(root);
}

static void test_header_parallax(void) {
	Kid root;
	int x0, x1;
	setup(&root, K_ANIMATIONS_OFF);
	Khub_set_header(g_hub, "HUB HEADER");
	Kroot_update_layout(root);
	x0 = rightmost_lit(root, 5, 90);
	Khub_scroll_to_section(g_hub, 1);
	Kroot_update_layout(root);
	x1 = rightmost_lit(root, 5, 90);
	/* 319 px of sections, the header has gone half as far */
	CHECK(x0 > 200 && x0 < 479);
	CHECK(x1 < x0 - 150 && x1 > x0 - 170);
	Kroot_destroy(root);
}

static void test_header_click(void) {
	Kid root;
	KPoint p;
	setup(&root, K_ANIMATIONS_OFF);
	/* where the header of the first section is: over its content, in the band that the 31.5 px leave */
	p = Kelement_to_root(g_block[0], (KPoint){ 0, 0 });
	p.x += 10;
	p.y -= 43;
	tap(root, p);
	CHECK(g_clicks == 0);
	Khub_section_set_is_header_interactive(g_sec[0], K_TRUE);
	tap(root, p);
	CHECK(g_clicks == 1 && g_clicked == g_sec[0]);
	/* beside the text of the header and below it is not the header */
	tap(root, (KPoint){ p.x + 200, p.y });
	CHECK(g_clicks == 1);
	tap(root, (KPoint){ p.x, p.y + 30 });
	CHECK(g_clicks == 1);
	/* a press on the header that turns into a drag scrolls and does not click */
	{
		Kid sv = Khub_get_scroll_viewer(g_hub);
		drag(root, p, -100, 0, 10);
		CHECK(g_clicks == 1);
		CHECK(Kscroll_viewer_get_horizontal_offset(sv) > 50);
	}
	/* released away from the header: no click */
	Kscroll_viewer_scroll_to_horizontal_offset(Khub_get_scroll_viewer(g_hub), 0);
	Kroot_update_layout(root);
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x + 3, p.y + 80);
	CHECK(g_clicks == 1);
	Kroot_destroy(root);
}

static void test_vertical(void) {
	Kid root, sv;
	KRect b0, b1;
	setup(&root, K_ANIMATIONS_OFF);
	Khub_set_orientation(g_hub, K_VERTICAL);
	CHECK(Khub_get_orientation(g_hub) == K_VERTICAL);
	{
		int i;
		for (i = 0; i < 3; i++) {
			Kelement_set_width(g_block[i], K_AUTO);
			Kelement_set_height(g_block[i], 500);
		}
	}
	Kroot_update_layout(root);
	b0 = Kelement_get_bounds(g_sec[0]);
	b1 = Kelement_get_bounds(g_sec[1]);
	/* the sections are under one another, as wide as the screen less the margin, and only the first has the room for the header */
	CHECK_NEAR(b0.x, 19);
	CHECK_NEAR(b0.width, 480 - 19);
	CHECK_NEAR(b1.y, b0.y + b0.height);
	{
		KRect c0 = Kelement_get_bounds(g_block[0]), c1 = Kelement_get_bounds(g_block[1]);
		CHECK(c0.y > 100);
		/* the second section has no placeholder: its content is 5 + a line + 31.5 under it */
		CHECK(c1.y > 55 && c1.y < 65);
		CHECK(c0.y > c1.y + 40);
	}
	sv = Khub_get_scroll_viewer(g_hub);
	drag(root, (KPoint){ 240, 600 }, 0, -300, 20);
	CHECK(Kscroll_viewer_get_vertical_offset(sv) > 250);
	CHECK_NEAR(Kscroll_viewer_get_horizontal_offset(sv), 0);
	Kroot_destroy(root);
}

static void test_sections_added_and_removed(void) {
	Kid root, extra;
	setup(&root, K_ANIMATIONS_OFF);
	extra = Khub_section_create("zero");
	Kcontent_control_set_content(extra, block(100, Kcolor_argb(0xFFFFFFFF)));
	CHECK(Khub_insert_section(g_hub, 0, extra) == 0);
	CHECK(Khub_get_section_count(g_hub) == 4 && Khub_get_section(g_hub, 1) == g_sec[0]);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(g_sec[0]).x, 19 + 100 + 19);
	Khub_remove_section_at(g_hub, 0);
	CHECK(Khub_get_section_count(g_hub) == 3);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(g_sec[0]).x, 19);
	Khub_clear_sections(g_hub);
	CHECK(Khub_get_section_count(g_hub) == 0 && Khub_get_sections_in_view_count(g_hub) == 0);
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

static void test_light_theme(void) {
	Kid root;
	setup(&root, K_ANIMATIONS_OFF);
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kroot_update_layout(root);
	/* the header is the foreground of the theme: black on white */
	{
		KFrame f;
		int x, y, darkest = 255;
		Kroot_render(root, &f);
		for (y = 1; y < 90; y++)
			for (x = 15; x < 200; x++) {
				const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
				if (px[0] < darkest) darkest = px[0];
			}
		CHECK(darkest <= 4);
		CHECK(pixel_is(root, 330, 700, 255, 255, 255));
	}
	Kroot_destroy(root);
}

static void test_picture(void) {
	Kid root;
	char path[512];
	setup(&root, K_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/hub.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Khub_scroll_to_section(g_hub, 1);
	snprintf(path, sizeof path, "%s/hub_scrolled.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kroot_destroy(root);
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
