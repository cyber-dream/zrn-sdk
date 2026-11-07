/**
 * Renders every control to PNG files in the build directory, and checks a few
 * pixels against the WP8 templates (a Dark theme page, Cobalt accent).
 */

#include "test.h"

static Kid page_with(Kid root) {
	Kid sv = Kscroll_viewer_create(), stack = Kstack_panel_create(K_VERTICAL);
	Kscroll_viewer_set_content(sv, stack);
	Kroot_set_visual(root, sv);
	return stack;
}

static Kid add_text(Kid parent, const char* text, KTextStyle style) {
	Kid t = Ktext_block_create(text);
	if (style != K_TEXT_STYLE_NONE) Ktext_block_set_style(t, style);
	return Kpanel_add(parent, t);
}

static void show(Kid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", KESTREL_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}

static void build_controls(Kid root) {
	Kid stack = page_with(root), row, pb, sl, lb, cb, tb, grid, sw;
	Kelement_set_margin(stack, Kthickness(12, 0, 12, 0));
	add_text(stack, "KESTREL", K_TEXT_STYLE_PHONE_CONTRAST);
	add_text(stack, "controls", K_TEXT_STYLE_PHONE_TITLE2);
	add_text(stack, "Subtle text style", K_TEXT_STYLE_PHONE_SUBTLE);
	row = Kpanel_add(stack, Kstack_panel_create(K_HORIZONTAL));
	Kpanel_add(row, Kbutton_create("button"));
	Kelement_set_enabled(Kpanel_add(row, Kbutton_create("disabled")), K_FALSE);
	Kpanel_add(stack, Khyperlink_button_create("hyperlink"));
	Kpanel_add(stack, Kcheck_box_create("check box"));
	Ktoggle_button_set_is_checked(Kpanel_add(stack, Kcheck_box_create("checked")), K_TRUE);
	Ktoggle_button_set_state(Kpanel_add(stack, Kcheck_box_create("indeterminate")), K_INDETERMINATE);
	Kpanel_add(stack, Kradio_button_create("radio one", "g"));
	Ktoggle_button_set_is_checked(Kpanel_add(stack, Kradio_button_create("radio two", "g")), K_TRUE);
	sl = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Krange_base_set_value(sl, 40);
	Kslider_set_header(sl, "Volume");
	Kslider_set_tick_frequency(sl, 25);
	pb = Kpanel_add(stack, Kprogress_bar_create());
	Krange_base_set_value(pb, 60);
	sw = Kpanel_add(stack, Ktoggle_switch_create());
	Ktoggle_switch_set_header(sw, "Wi-Fi");
	Ktoggle_switch_set_is_on(Kpanel_add(stack, Ktoggle_switch_create()), K_TRUE);
	Kelement_set_enabled(Kpanel_add(stack, Ktoggle_switch_create()), K_FALSE);
	Kprogress_ring_set_is_active(Kpanel_add(stack, Kprogress_ring_create()), K_TRUE);
	Ktoggle_button_set_is_checked(Kpanel_add(stack, Ktoggle_button_create("toggle")), K_TRUE);
	tb = Kpanel_add(stack, Ktext_box_create("text box"));
	Ktext_box_set_header(tb, "Name");
	Kpassword_box_set_placeholder_text(Kpanel_add(stack, Kpassword_box_create()), "password");
	Ktext_box_set_placeholder_text(Kpanel_add(stack, Ktext_box_create("")), "placeholder");
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kitems_control_add_text(cb, "first");
	Kitems_control_add_text(cb, "second");
	Kselector_set_selected_index(cb, 1);
	lb = Kpanel_add(stack, Klist_box_create());
	Kitems_control_add_text(lb, "list item one");
	Kitems_control_add_text(lb, "list item two");
	Kselector_set_selected_index(lb, 0);
	grid = Kpanel_add(stack, Kgrid_create());
	Kgrid_add_column(grid, K_GRID_STAR, 1);
	Kgrid_add_column(grid, K_GRID_STAR, 2);
	Kgrid_place(grid, Kborder_create(), 0, 0);
	(void)tb;
	(void)sw;
}

static void test_all_controls(void) {
	Kid root = Kroot_create(480, 1700);
	build_controls(root);
	show(root, "controls-dark");
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kroot_set_accent(root, K_ACCENT_ORANGE);
	show(root, "controls-light");
	Kroot_destroy(root);
}

static void test_button_pixels(void) {
	Kid root = Kroot_create(480, 200), page = Kstack_panel_create(K_VERTICAL), b;
	int r, g, bl;
	b = Kpanel_add(page, Kbutton_create("OK"));
	Kelement_set_horizontal_alignment(b, K_HALIGN_LEFT);
	Kroot_set_visual(root, page);
	Kroot_update_layout(root);
	/* the outline is 2.5 px, white in Dark, with 9.5 px of touch overhang above and none at the sides */
	test_pixel(root, 1, 30, &r, &g, &bl);
	CHECK(r == 255 && g == 255 && bl == 255);
	test_pixel(root, 6, 30, &r, &g, &bl);
	CHECK(r == 0 && g == 0 && bl == 0);
	test_pixel(root, 50, 4, &r, &g, &bl);
	CHECK(r == 0 && g == 0 && bl == 0);
	test_pixel(root, 50, 10, &r, &g, &bl);
	CHECK(r == 255 && g == 255 && bl == 255);
	/* pressed: filled with the accent (Cobalt 0050EF) */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 30, 30);
	test_pixel(root, 20, 30, &r, &g, &bl);
	CHECK(r == 0x00 && g == 0x50 && bl == 0xEF);
	Kroot_inject_pointer(root, K_POINTER_UP, 30, 30);
	test_pixel(root, 20, 30, &r, &g, &bl);
	CHECK(r == 0 && g == 0 && bl == 0);
	Kroot_destroy(root);
}

static void test_states(void) {
	static const char* const words[] = { "apple", "apricot", "banana" };
	Kid root = Kroot_create(480, 700), stack = Kstack_panel_create(K_VERTICAL), b, tb, ac, cb, pb, sl;
	Kelement_set_margin(stack, Kthickness(12, 0, 12, 0));
	Kroot_set_visual(root, stack);
	b = Kpanel_add(stack, Kbutton_create("pressed"));
	Kelement_set_horizontal_alignment(b, K_HALIGN_LEFT);
	tb = Kpanel_add(stack, Ktext_box_create("focused with a selection"));
	pb = Kpanel_add(stack, Kprogress_bar_create());
	Kprogress_bar_set_is_indeterminate(pb, K_TRUE);
	sl = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Krange_base_set_value(sl, 7);
	Kelement_set_enabled(Kpanel_add(stack, Kcheck_box_create("disabled check box")), K_FALSE);
	ac = Kpanel_add(stack, Kauto_suggest_box_create());
	Kauto_suggest_box_set_placeholder_text(ac, "search");
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kitems_control_add_text(cb, "red"); Kitems_control_add_text(cb, "green"); Kitems_control_add_text(cb, "blue");
	Kselector_set_selected_index(cb, 0);
	Kroot_update_layout(root);
	Kroot_tick(root, 700);
	Kroot_inject_pointer(root, K_POINTER_DOWN, Kelement_to_root(b, (KPoint){ 40, 30 }).x, Kelement_to_root(b, (KPoint){ 40, 30 }).y);
	Kelement_focus(tb);
	Ktext_box_select(tb, 8, 4);
	Kelement_focus(ac);
	Kroot_inject_text(root, "ap");
	Kauto_suggest_box_set_items(ac, words, 2);
	Kelement_focus(tb);
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	show(root, "states");
	Kroot_destroy(root);
}

/* the number of pixels in a rectangle that are not the background of the Dark theme */
static int lit_pixels(Kid root, int x0, int y0, int x1, int y1) {
	KFrame f;
	int x, y, n = 0;
	Kroot_render(root, &f);
	for (y = y0; y < y1; y++) {
		for (x = x0; x < x1; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] || px[1] || px[2]) n++;
		}
	}
	return n;
}

static void test_cyrillic_and_password(void) {
	Kid root = Kroot_create(480, 400), stack = Kstack_panel_create(K_VERTICAL), t, pw, ru;
	int cyrillic, boxes;
	Kroot_set_visual(root, stack);
	t = Kpanel_add(stack, Ktext_block_create("Hello, world"));
	ru = Kpanel_add(stack, Ktext_block_create("\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82, \xD0\xBC\xD0\xB8\xD1\x80 \xC3\xA9\xC3\xBC"));
	Ktext_block_set_font_size(t, 30);
	Ktext_block_set_font_size(ru, 30);
	pw = Kpanel_add(stack, Kpassword_box_create());
	Ktext_box_set_text(pw, "secret");
	Kpanel_add(stack, Kbutton_create("\xD0\x9E\xD0\x9A"));
	Kroot_update_layout(root);
	cyrillic = lit_pixels(root, 0, (int)Kelement_get_bounds(ru).y, 480, (int)(Kelement_get_bounds(ru).y + Kelement_get_bounds(ru).height));
	/* real letters light many pixels; the width tells it is not a row of missing-glyph boxes */
	CHECK(cyrillic > 400);
	CHECK(Kelement_get_actual_width(ru) > Kelement_get_actual_width(t) * 0.7f);
	/* the password shows six bullets, not the text */
	boxes = lit_pixels(root, 20, (int)Kelement_get_bounds(pw).y + 14, 300, (int)Kelement_get_bounds(pw).y + 60);
	CHECK(boxes > 200);
	show(root, "text");
	Kroot_destroy(root);
}

static void test_zero_sized_canvas_is_not_culled(void) {
	Kid root = Kroot_create(200, 200), stack = Kstack_panel_create(K_VERTICAL), canvas, red;
	int r, g, b;
	Kroot_set_visual(root, stack);
	/* a Canvas has no size of its own: its rectangle is empty, its children are not */
	canvas = Kpanel_add(stack, Kcanvas_create());
	red = Kcanvas_place(canvas, Kborder_create(), 30, 40);
	Kelement_set_width(red, 20);
	Kelement_set_height(red, 20);
	Kborder_set_background(red, Kcolor_rgba(1, 0, 0, 1));
	Kroot_update_layout(root);
	test_pixel(root, 40, 50, &r, &g, &b);
	CHECK(r == 255 && g == 0 && b == 0);
	Kroot_destroy(root);
}

static void test_image_roundtrip(void) {
	Kid root = Kroot_create(200, 200), img = Kimage_create();
	KSize s;
	int r, g, b;
	Kroot_set_visual(root, img);
	CHECK(!Kimage_set_source_file(img, KESTREL_TEST_OUTPUT_DIR "/does-not-exist.png"));
	/* the picture a root wrote earlier is read back: 480 x 400, scaled to fit 200 x 200 (uniform) */
	CHECK(Kimage_set_source_file(img, KESTREL_TEST_OUTPUT_DIR "/text.png"));
	s = Kimage_get_natural_size(img);
	CHECK(s.width == 480 && s.height == 400);
	Kroot_update_layout(root);
	test_pixel(root, 100, 1, &r, &g, &b); /* above the picture (it is centered, 167 high): background */
	CHECK(r == 0 && g == 0 && b == 0);
	{
		unsigned char px[4 * 4] = { 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255 };
		CHECK(Kimage_set_source_rgba(img, 2, 2, px));
		Kimage_set_stretch(img, K_STRETCH_FILL);
		Kroot_update_layout(root);
		test_pixel(root, 100, 100, &r, &g, &b);
		CHECK(r == 255 && g == 0);
	}
	Kroot_destroy(root);
}

/* a pixel is near a color (the software renderer rounds edges, so tests look at the middle of things) */
static int near_rgb(Kid root, int x, int y, int er, int eg, int eb, int tolerance) {
	int r, g, b;
	test_pixel(root, x, y, &r, &g, &b);
	return abs(r - er) <= tolerance && abs(g - eg) <= tolerance && abs(b - eb) <= tolerance;
}

static void test_toggle_switch_pixels(void) {
	Kid root = Kroot_create(480, 200), stack = Kstack_panel_create(K_VERTICAL), sw;
	sw = Kpanel_add(stack, Ktoggle_switch_create());
	Kelement_set_vertical_alignment(sw, K_VALIGN_TOP);
	Kroot_set_visual(root, stack);
	Kroot_update_layout(root);
	/* the switch is 77 x 36 at the right: off, the knob (20.5 wide) is at its left end and the track is empty */
	CHECK(near_rgb(root, 413, 18, 255, 255, 255, 0)); /* the knob: the foreground, white in Dark */
	CHECK(near_rgb(root, 440, 18, 0, 0, 0, 0));       /* nothing behind it */
	CHECK(near_rgb(root, 440, 5, 255, 255, 255, 0));  /* the 2.5 px outline, inset 4.5 */
	Ktoggle_switch_set_is_on(sw, K_TRUE);
	CHECK(near_rgb(root, 470, 18, 255, 255, 255, 0)); /* the knob is at the right end now */
	CHECK(near_rgb(root, 430, 18, 0x00, 0x50, 0xEF, 0)); /* the curtain is the accent (Cobalt) */
	CHECK(near_rgb(root, 413, 18, 0x00, 0x50, 0xEF, 0));
	/* disabled: the curtain and the knob are PhoneBaseMediumMid solid (666666) */
	Kelement_set_enabled(sw, K_FALSE);
	CHECK(near_rgb(root, 430, 18, 0x66, 0x66, 0x66, 0));
	CHECK(near_rgb(root, 470, 18, 0x66, 0x66, 0x66, 0));
	/* Light: the knob is the foreground, dark on the white page (PhoneForeground of the 8.1 Light dictionary: #FF000000) */
	Kelement_set_enabled(sw, K_TRUE);
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(near_rgb(root, 470, 18, 0x00, 0x00, 0x00, 0x08));
	CHECK(near_rgb(root, 100, 18, 255, 255, 255, 0));
	Kroot_destroy(root);
}

static void test_progress_ring_pixels(void) {
	Kid root = Kroot_create(480, 200), stack = Kstack_panel_create(K_VERTICAL), ring;
	int before, outside;
	ring = Kpanel_add(stack, Kprogress_ring_create());
	Kelement_set_vertical_alignment(ring, K_VALIGN_TOP);
	Kroot_set_visual(root, stack);
	Kroot_set_background(root, Kcolor_rgba(1, 0, 0, 1));
	Kroot_update_layout(root);
	/* inactive: nothing is drawn, not even the overlay */
	CHECK(near_rgb(root, 5, 5, 255, 0, 0, 0));
	Kprogress_ring_set_is_active(ring, K_TRUE);
	/* active: the Background of the theme (black) at 0.8 over the control, 0.2 of the red is left */
	CHECK(near_rgb(root, 5, 5, 51, 0, 0, 2));
	CHECK(near_rgb(root, 5, 100, 255, 0, 0, 0)); /* below the 57.5 px of the control */
	/* the dots are the accent: they show while it runs, in the 57.5 px square at the middle, and not outside it */
	Kroot_tick(root, 1000);
	Kroot_tick(root, 1000);
	before = lit_pixels(root, 211, 0, 269, 58);
	CHECK(before > 0);
	outside = 0;
	{
		KFrame f;
		int x, y;
		Kroot_render(root, &f);
		for (y = 0; y < 58; y++) {
			for (x = 0; x < 480; x++) {
				const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
				if ((x < 211 || x >= 269) && px[2] > 100) outside++;
			}
		}
	}
	CHECK(outside == 0);
	/* the dots move: a little later the picture is another one */
	{
		KFrame f;
		unsigned char* copy;
		int changed = 0, i;
		Kroot_render(root, &f);
		copy = (unsigned char*)malloc((size_t)f.stride * (size_t)f.height);
		memcpy(copy, f.pixels, (size_t)f.stride * (size_t)f.height);
		Kroot_tick(root, 400);
		Kroot_render(root, &f);
		for (i = 0; i < f.stride * 58; i++) if (copy[i] != f.pixels[i]) { changed = 1; break; }
		free(copy);
		CHECK(changed);
	}
	/* stopped: the overlay goes with the dots */
	Kprogress_ring_set_is_active(ring, K_FALSE);
	CHECK(near_rgb(root, 5, 5, 255, 0, 0, 0));
	Kroot_destroy(root);
}

static void test_slider_and_progress_bar_pixels(void) {
	Kid root = Kroot_create(480, 300), stack = Kstack_panel_create(K_VERTICAL), sl, dis, pb, ind;
	Kroot_set_visual(root, stack);
	sl = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Krange_base_set_value(sl, 50);
	dis = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Krange_base_set_value(dis, 50);
	Kelement_set_enabled(dis, K_FALSE);
	pb = Kpanel_add(stack, Kprogress_bar_create());
	Krange_base_set_value(pb, 50);
	ind = Kpanel_add(stack, Kprogress_bar_create());
	Kprogress_bar_set_is_indeterminate(ind, K_TRUE);
	Kelement_set_vertical_alignment(sl, K_VALIGN_TOP);
	Kroot_update_layout(root);
	/* the 9 px track is 17.5 px under the top of the 57 px band; the thumb (9.5 x 19) is at 13 */
	CHECK(near_rgb(root, 400, 22, 0x1F, 0x1F, 0x1F, 0));          /* SliderTrackBackground */
	CHECK(near_rgb(root, 100, 22, 0x00, 0x50, 0xEF, 0));          /* the accent up to the thumb */
	CHECK(near_rgb(root, 240, 22, 255, 255, 255, 0));             /* the thumb at the middle: 0.5 * (480 - 9.5) + 4.75 */
	CHECK(near_rgb(root, 240, 16, 255, 255, 255, 0));             /* the thumb is taller than the track */
	CHECK(near_rgb(root, 100, 10, 0, 0, 0, 0));                   /* nothing above the track */
	/* disabled: decrease 666666, track and thumb 999999 / 1F1F1F */
	CHECK(near_rgb(root, 100, 57 + 22, 0x66, 0x66, 0x66, 0));
	CHECK(near_rgb(root, 240, 57 + 22, 0x99, 0x99, 0x99, 0));
	/* the ProgressBar is 3.5 px high: the indicator is the accent, the track the accent at 0.16 */
	CHECK(near_rgb(root, 100, 114 + 1, 0x00, 0x50, 0xEF, 0));
	CHECK(near_rgb(root, 400, 114 + 1, 0x00, 13, 38, 2));
	/* indeterminate: no track, and squares that appear once it runs */
	CHECK(near_rgb(root, 400, 117 + 1, 0, 0, 0, 0));
	Kroot_tick(root, 1500);
	CHECK(lit_pixels(root, 0, 117, 480, 121) > 0);
	/* paused halves the indicator, an error takes it away */
	Kprogress_bar_set_show_paused(pb, K_TRUE);
	CHECK(near_rgb(root, 100, 114 + 1, 0x00, 0x28 + 6, 0x77 + 19, 12));
	Kprogress_bar_set_show_error(pb, K_TRUE);
	CHECK(near_rgb(root, 100, 114 + 1, 0x00, 13, 38, 2));
	Kroot_destroy(root);
}

static void test_toggle_and_check_pixels(void) {
	Kid root = Kroot_create(480, 300), stack = Kstack_panel_create(K_VERTICAL), cb, tg;
	Kroot_set_visual(root, stack);
	cb = Kpanel_add(stack, Kcheck_box_create("check"));
	tg = Kpanel_add(stack, Ktoggle_button_create("toggle"));
	Kroot_update_layout(root);
	/* the 25.5 px box is 16 px under the top; its 2.5 px border is the foreground */
	CHECK(near_rgb(root, 1, 16 + 12, 255, 255, 255, 0));
	CHECK(near_rgb(root, 12, 16 + 12, 0, 0, 0, 0));
	CHECK(near_rgb(root, 12, 2, 0, 0, 0, 0)); /* nothing in the overhang */
	/* the check mark is drawn once checked; pressed, the box is the accent */
	CHECK(near_rgb(root, 11, 34, 0, 0, 0, 0));
	Ktoggle_button_set_is_checked(cb, K_TRUE);
	CHECK(near_rgb(root, 11, 34, 255, 255, 255, 0));
	Kroot_inject_pointer(root, K_POINTER_DOWN, 60, 28);
	CHECK(near_rgb(root, 8, 16 + 4, 0x00, 0x50, 0xEF, 0));
	Kroot_inject_pointer(root, K_POINTER_UP, 400, 250);
	/* a ToggleButton is the accent while checked, and the text over it is white */
	{
		KRect r = Kelement_get_bounds(tg);
		int x = (int)r.x + 50, y = (int)(r.y + r.height * 0.5f) - 14;
		CHECK(near_rgb(root, x, y, 0, 0, 0, 0));
		Ktoggle_button_set_is_checked(tg, K_TRUE);
		CHECK(near_rgb(root, x, y, 0x00, 0x50, 0xEF, 0));
	}
	Kroot_destroy(root);
}

static void test_text_box_pixels(void) {
	Kid root = Kroot_create(480, 300), stack = Kstack_panel_create(K_VERTICAL), tb, other;
	int lit;
	Kroot_set_visual(root, stack);
	tb = Kpanel_add(stack, Ktext_box_create(""));
	Ktext_box_set_placeholder_text(tb, "placeholder");
	other = Kpanel_add(stack, Ktext_box_create("abc"));
	Kroot_update_layout(root);
	/* the box is 34 px high under 9.5 px of margin, white at 0.8 opacity (204) with a border of the same */
	CHECK(near_rgb(root, 400, 9 + 17, 204, 204, 204, 2));
	CHECK(near_rgb(root, 1, 9 + 17, 204, 204, 204, 2));
	CHECK(near_rgb(root, 400, 4, 0, 0, 0, 0));
	/* the placeholder is drawn while it is empty and has no focus; the focus takes it away and the box turns white with an accent border */
	lit = 0;
	{
		KFrame f;
		int x, y;
		Kroot_render(root, &f);
		for (y = 12; y < 38; y++) for (x = 9; x < 150; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (abs(px[0] - 204) > 10) lit++;
		}
	}
	CHECK(lit > 50);
	Kelement_focus(tb);
	CHECK(near_rgb(root, 400, 9 + 17, 255, 255, 255, 0));
	CHECK(near_rgb(root, 1, 9 + 17, 0x00, 0x50, 0xEF, 0));
	lit = 0;
	{
		KFrame f;
		int x, y;
		Kroot_render(root, &f);
		for (y = 14; y < 34; y++) for (x = 12; x < 150; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] != 255) lit++;
		}
	}
	CHECK(lit < 20); /* only the caret */
	/* disabled: no fill, the border is PhoneBaseLow (73FFFFFF over black: 115) */
	Kelement_set_enabled(other, K_FALSE);
	Kelement_focus(tb);
	{
		KRect r = Kelement_get_bounds(other);
		CHECK(near_rgb(root, 400, (int)r.y + 17, 0, 0, 0, 0));
		CHECK(near_rgb(root, 1, (int)r.y + 17, 115, 115, 115, 2));
	}
	/* Light: BFBFBF at 0.8 */
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kelement_set_enabled(other, K_TRUE);
	{
		KRect r = Kelement_get_bounds(other);
		/* 0.8 * BF over white */
		CHECK(near_rgb(root, 400, (int)r.y + 17, 255 - (int)(0.8f * (255 - 0xBF)), 255 - (int)(0.8f * (255 - 0xBF)), 255 - (int)(0.8f * (255 - 0xBF)), 2));
	}
	/* a header pushes the box down */
	Ktext_box_set_header(tb, "Header");
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(tb).height > 34 + 20);
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_button_pixels);
	RUN(test_all_controls);
	RUN(test_states);
	RUN(test_cyrillic_and_password);
	RUN(test_toggle_switch_pixels);
	RUN(test_progress_ring_pixels);
	RUN(test_slider_and_progress_bar_pixels);
	RUN(test_toggle_and_check_pixels);
	RUN(test_text_box_pixels);
	RUN(test_zero_sized_canvas_is_not_culled);
	RUN(test_image_roundtrip);
	Kshutdown();
	return test_finish("test_render");
}
