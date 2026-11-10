/**
 * Renders every control to PNG files in the build directory, and checks a few
 * pixels against the WP8 templates (a Dark theme page, Cobalt accent).
 */

#include "test.h"

static Zid page_with(Zid root) {
	Zid sv = Zscroll_viewer_create(), stack = Zstack_panel_create(Z_VERTICAL);
	Zscroll_viewer_set_content(sv, stack);
	Zroot_set_visual(root, sv);
	return stack;
}

static Zid add_text(Zid parent, const char* text, ZTextStyle style) {
	Zid t = Ztext_block_create(text);
	if (style != Z_TEXT_STYLE_NONE) Ztext_block_set_style(t, style);
	return Zpanel_add(parent, t);
}

static void show(Zid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", ZRNUI_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}

static void build_controls(Zid root) {
	Zid stack = page_with(root), row, pb, sl, lb, cb, tb, grid, sw;
	Zelement_set_margin(stack, Zthickness(12, 0, 12, 0));
	add_text(stack, "ZRNUI", Z_TEXT_STYLE_PHONE_CONTRAST);
	add_text(stack, "controls", Z_TEXT_STYLE_PHONE_TITLE2);
	add_text(stack, "Subtle text style", Z_TEXT_STYLE_PHONE_SUBTLE);
	row = Zpanel_add(stack, Zstack_panel_create(Z_HORIZONTAL));
	Zpanel_add(row, Zbutton_create("button"));
	Zelement_set_enabled(Zpanel_add(row, Zbutton_create("disabled")), false);
	Zpanel_add(stack, Zhyperlink_button_create("hyperlink"));
	Zpanel_add(stack, Zcheck_box_create("check box"));
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zcheck_box_create("checked")), true);
	Ztoggle_button_set_state(Zpanel_add(stack, Zcheck_box_create("indeterminate")), Z_INDETERMINATE);
	Zpanel_add(stack, Zradio_button_create("radio one", "g"));
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zradio_button_create("radio two", "g")), true);
	sl = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zrange_base_set_value(sl, 40);
	Zslider_set_header(sl, "Volume");
	Zslider_set_tick_frequency(sl, 25);
	pb = Zpanel_add(stack, Zprogress_bar_create());
	Zrange_base_set_value(pb, 60);
	sw = Zpanel_add(stack, Ztoggle_switch_create());
	Ztoggle_switch_set_header(sw, "Wi-Fi");
	Ztoggle_switch_set_is_on(Zpanel_add(stack, Ztoggle_switch_create()), true);
	Zelement_set_enabled(Zpanel_add(stack, Ztoggle_switch_create()), false);
	Zprogress_ring_set_is_active(Zpanel_add(stack, Zprogress_ring_create()), true);
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Ztoggle_button_create("toggle")), true);
	tb = Zpanel_add(stack, Ztext_box_create("text box"));
	Ztext_box_set_header(tb, "Name");
	Zpassword_box_set_placeholder_text(Zpanel_add(stack, Zpassword_box_create()), "password");
	Ztext_box_set_placeholder_text(Zpanel_add(stack, Ztext_box_create("")), "placeholder");
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zitems_control_add_text(cb, "first");
	Zitems_control_add_text(cb, "second");
	Zselector_set_selected_index(cb, 1);
	lb = Zpanel_add(stack, Zlist_box_create());
	Zitems_control_add_text(lb, "list item one");
	Zitems_control_add_text(lb, "list item two");
	Zselector_set_selected_index(lb, 0);
	grid = Zpanel_add(stack, Zgrid_create());
	Zgrid_add_column(grid, Z_GRID_STAR, 1);
	Zgrid_add_column(grid, Z_GRID_STAR, 2);
	Zgrid_place(grid, Zborder_create(), 0, 0);
	(void)tb;
	(void)sw;
}

static void test_all_controls(void) {
	Zid root = zst_test_root(480, 1700);
	build_controls(root);
	show(root, "controls-dark");
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zroot_set_accent(root, Z_ACCENT_ORANGE);
	show(root, "controls-light");
	Zroot_destroy(root);
}

static void test_button_pixels(void) {
	Zid root = zst_test_root(480, 200), page = Zstack_panel_create(Z_VERTICAL), b;
	int r, g, bl;
	b = Zpanel_add(page, Zbutton_create("OK"));
	Zelement_set_horizontal_alignment(b, Z_HALIGN_LEFT);
	Zroot_set_visual(root, page);
	Zroot_update_layout(root);
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
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 30, 30);
	test_pixel(root, 20, 30, &r, &g, &bl);
	CHECK(r == 0x00 && g == 0x50 && bl == 0xEF);
	Zroot_inject_pointer(root, Z_POINTER_UP, 30, 30);
	test_pixel(root, 20, 30, &r, &g, &bl);
	CHECK(r == 0 && g == 0 && bl == 0);
	Zroot_destroy(root);
}

static void test_states(void) {
	static const char* const words[] = { "apple", "apricot", "banana" };
	Zid root = zst_test_root(480, 700), stack = Zstack_panel_create(Z_VERTICAL), b, tb, ac, cb, pb, sl;
	Zelement_set_margin(stack, Zthickness(12, 0, 12, 0));
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zbutton_create("pressed"));
	Zelement_set_horizontal_alignment(b, Z_HALIGN_LEFT);
	tb = Zpanel_add(stack, Ztext_box_create("focused with a selection"));
	pb = Zpanel_add(stack, Zprogress_bar_create());
	Zprogress_bar_set_is_indeterminate(pb, true);
	sl = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zrange_base_set_value(sl, 7);
	Zelement_set_enabled(Zpanel_add(stack, Zcheck_box_create("disabled check box")), false);
	ac = Zpanel_add(stack, Zauto_suggest_box_create());
	Zauto_suggest_box_set_placeholder_text(ac, "search");
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zitems_control_add_text(cb, "red"); Zitems_control_add_text(cb, "green"); Zitems_control_add_text(cb, "blue");
	Zselector_set_selected_index(cb, 0);
	Zroot_update_layout(root);
	Zroot_tick(root, 700);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, Zelement_to_root(b, (ZPoint){ 40, 30 }).x, Zelement_to_root(b, (ZPoint){ 40, 30 }).y);
	Zelement_focus(tb);
	Ztext_box_select(tb, 8, 4);
	Zelement_focus(ac);
	Zroot_inject_text(root, "ap");
	Zauto_suggest_box_set_items(ac, words, 2);
	Zelement_focus(tb);
	Zcombo_box_set_is_drop_down_open(cb, true);
	show(root, "states");
	Zroot_destroy(root);
}

/* the number of pixels in a rectangle that are not the background of the Dark theme */
static int lit_pixels(Zid root, int x0, int y0, int x1, int y1) {
	ZFrame f;
	int x, y, n = 0;
	Zroot_render(root, &f);
	for (y = y0; y < y1; y++) {
		for (x = x0; x < x1; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] || px[1] || px[2]) n++;
		}
	}
	return n;
}

static void test_cyrillic_and_password(void) {
	Zid root = zst_test_root(480, 400), stack = Zstack_panel_create(Z_VERTICAL), t, pw, ru;
	int cyrillic, boxes;
	Zroot_set_visual(root, stack);
	t = Zpanel_add(stack, Ztext_block_create("Hello, world"));
	ru = Zpanel_add(stack, Ztext_block_create("\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82, \xD0\xBC\xD0\xB8\xD1\x80 \xC3\xA9\xC3\xBC"));
	Ztext_block_set_font_size(t, 30);
	Ztext_block_set_font_size(ru, 30);
	pw = Zpanel_add(stack, Zpassword_box_create());
	Ztext_box_set_text(pw, "secret");
	Zpanel_add(stack, Zbutton_create("\xD0\x9E\xD0\x9A"));
	Zroot_update_layout(root);
	cyrillic = lit_pixels(root, 0, (int)Zelement_get_bounds(ru).y, 480, (int)(Zelement_get_bounds(ru).y + Zelement_get_bounds(ru).height));
	/* real letters light many pixels; the width tells it is not a row of missing-glyph boxes */
	CHECK(cyrillic > 400);
	CHECK(Zelement_get_actual_width(ru) > Zelement_get_actual_width(t) * 0.7f);
	/* the password shows six bullets, not the text */
	boxes = lit_pixels(root, 20, (int)Zelement_get_bounds(pw).y + 14, 300, (int)Zelement_get_bounds(pw).y + 60);
	CHECK(boxes > 200);
	show(root, "text");
	Zroot_destroy(root);
}

static void test_zero_sized_canvas_is_not_culled(void) {
	Zid root = zst_test_root(200, 200), stack = Zstack_panel_create(Z_VERTICAL), canvas, red;
	int r, g, b;
	Zroot_set_visual(root, stack);
	/* a Canvas has no size of its own: its rectangle is empty, its children are not */
	canvas = Zpanel_add(stack, Zcanvas_create());
	red = Zcanvas_place(canvas, Zborder_create(), 30, 40);
	Zelement_set_width(red, 20);
	Zelement_set_height(red, 20);
	Zborder_set_background(red, Zcolor_rgba(1, 0, 0, 1));
	Zroot_update_layout(root);
	test_pixel(root, 40, 50, &r, &g, &b);
	CHECK(r == 255 && g == 0 && b == 0);
	Zroot_destroy(root);
}

static void test_image_roundtrip(void) {
	Zid root = zst_test_root(200, 200), img = Zimage_create();
	ZSize s;
	int r, g, b;
	Zroot_set_visual(root, img);
	CHECK(!Zimage_set_source_file(img, ZRNUI_TEST_OUTPUT_DIR "/does-not-exist.png"));
	/* the picture a root wrote earlier is read back: 480 x 400, scaled to fit 200 x 200 (uniform) */
	CHECK(Zimage_set_source_file(img, ZRNUI_TEST_OUTPUT_DIR "/text.png"));
	s = Zimage_get_natural_size(img);
	CHECK(s.width == 480 && s.height == 400);
	Zroot_update_layout(root);
	test_pixel(root, 100, 1, &r, &g, &b); /* above the picture (it is centered, 167 high): background */
	CHECK(r == 0 && g == 0 && b == 0);
	{
		unsigned char px[4 * 4] = { 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255 };
		CHECK(Zimage_set_source_rgba(img, 2, 2, px));
		Zimage_set_stretch(img, Z_STRETCH_FILL);
		Zroot_update_layout(root);
		test_pixel(root, 100, 100, &r, &g, &b);
		CHECK(r == 255 && g == 0);
	}
	Zroot_destroy(root);
}

/* a pixel is near a color (the software renderer rounds edges, so tests look at the middle of things) */
static int near_rgb(Zid root, int x, int y, int er, int eg, int eb, int tolerance) {
	int r, g, b;
	test_pixel(root, x, y, &r, &g, &b);
	return abs(r - er) <= tolerance && abs(g - eg) <= tolerance && abs(b - eb) <= tolerance;
}

static void test_toggle_switch_pixels(void) {
	Zid root = zst_test_root(480, 200), stack = Zstack_panel_create(Z_VERTICAL), sw;
	sw = Zpanel_add(stack, Ztoggle_switch_create());
	Zelement_set_vertical_alignment(sw, Z_VALIGN_TOP);
	Zroot_set_visual(root, stack);
	Zroot_update_layout(root);
	/* the switch is 77 x 36 at the right: off, the knob (20.5 wide) is at its left end and the track is empty */
	CHECK(near_rgb(root, 413, 18, 255, 255, 255, 0)); /* the knob: the foreground, white in Dark */
	CHECK(near_rgb(root, 440, 18, 0, 0, 0, 0));       /* nothing behind it */
	CHECK(near_rgb(root, 440, 5, 255, 255, 255, 0));  /* the 2.5 px outline, inset 4.5 */
	Ztoggle_switch_set_is_on(sw, true);
	CHECK(near_rgb(root, 470, 18, 255, 255, 255, 0)); /* the knob is at the right end now */
	CHECK(near_rgb(root, 430, 18, 0x00, 0x50, 0xEF, 0)); /* the curtain is the accent (Cobalt) */
	CHECK(near_rgb(root, 413, 18, 0x00, 0x50, 0xEF, 0));
	/* disabled: the curtain and the knob are PhoneBaseMediumMid solid (666666) */
	Zelement_set_enabled(sw, false);
	CHECK(near_rgb(root, 430, 18, 0x66, 0x66, 0x66, 0));
	CHECK(near_rgb(root, 470, 18, 0x66, 0x66, 0x66, 0));
	/* Light: the knob is the foreground, dark on the white page (PhoneForeground of the 8.1 Light dictionary: #FF000000) */
	Zelement_set_enabled(sw, true);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(near_rgb(root, 470, 18, 0x00, 0x00, 0x00, 0x08));
	CHECK(near_rgb(root, 100, 18, 255, 255, 255, 0));
	Zroot_destroy(root);
}

static void test_progress_ring_pixels(void) {
	Zid root = zst_test_root(480, 200), stack = Zstack_panel_create(Z_VERTICAL), ring;
	int before, outside;
	ring = Zpanel_add(stack, Zprogress_ring_create());
	Zelement_set_vertical_alignment(ring, Z_VALIGN_TOP);
	Zroot_set_visual(root, stack);
	Zroot_set_background(root, Zcolor_rgba(1, 0, 0, 1));
	Zroot_update_layout(root);
	/* inactive: nothing is drawn, not even the overlay */
	CHECK(near_rgb(root, 5, 5, 255, 0, 0, 0));
	Zprogress_ring_set_is_active(ring, true);
	/* active: the Background of the theme (black) at 0.8 over the control, 0.2 of the red is left */
	CHECK(near_rgb(root, 5, 5, 51, 0, 0, 2));
	CHECK(near_rgb(root, 5, 100, 255, 0, 0, 0)); /* below the 57.5 px of the control */
	/* the dots are the accent: they show while it runs, in the 57.5 px square at the middle, and not outside it */
	Zroot_tick(root, 1000);
	Zroot_tick(root, 1000);
	before = lit_pixels(root, 211, 0, 269, 58);
	CHECK(before > 0);
	outside = 0;
	{
		ZFrame f;
		int x, y;
		Zroot_render(root, &f);
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
		ZFrame f;
		unsigned char* copy;
		int changed = 0, i;
		Zroot_render(root, &f);
		copy = (unsigned char*)malloc((size_t)f.stride * (size_t)f.height);
		memcpy(copy, f.pixels, (size_t)f.stride * (size_t)f.height);
		Zroot_tick(root, 400);
		Zroot_render(root, &f);
		for (i = 0; i < f.stride * 58; i++) if (copy[i] != f.pixels[i]) { changed = 1; break; }
		free(copy);
		CHECK(changed);
	}
	/* stopped: the overlay goes with the dots */
	Zprogress_ring_set_is_active(ring, false);
	CHECK(near_rgb(root, 5, 5, 255, 0, 0, 0));
	Zroot_destroy(root);
}

static void test_slider_and_progress_bar_pixels(void) {
	Zid root = zst_test_root(480, 300), stack = Zstack_panel_create(Z_VERTICAL), sl, dis, pb, ind;
	Zroot_set_visual(root, stack);
	sl = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zrange_base_set_value(sl, 50);
	dis = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zrange_base_set_value(dis, 50);
	Zelement_set_enabled(dis, false);
	pb = Zpanel_add(stack, Zprogress_bar_create());
	Zrange_base_set_value(pb, 50);
	ind = Zpanel_add(stack, Zprogress_bar_create());
	Zprogress_bar_set_is_indeterminate(ind, true);
	Zelement_set_vertical_alignment(sl, Z_VALIGN_TOP);
	Zroot_update_layout(root);
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
	Zroot_tick(root, 1500);
	CHECK(lit_pixels(root, 0, 117, 480, 121) > 0);
	/* paused halves the indicator, an error takes it away */
	Zprogress_bar_set_show_paused(pb, true);
	CHECK(near_rgb(root, 100, 114 + 1, 0x00, 0x28 + 6, 0x77 + 19, 12));
	Zprogress_bar_set_show_error(pb, true);
	CHECK(near_rgb(root, 100, 114 + 1, 0x00, 13, 38, 2));
	Zroot_destroy(root);
}

static void test_toggle_and_check_pixels(void) {
	Zid root = zst_test_root(480, 300), stack = Zstack_panel_create(Z_VERTICAL), cb, tg;
	Zroot_set_visual(root, stack);
	cb = Zpanel_add(stack, Zcheck_box_create("check"));
	tg = Zpanel_add(stack, Ztoggle_button_create("toggle"));
	Zroot_update_layout(root);
	/* the 25.5 px box is 16 px under the top; its 2.5 px border is the foreground */
	CHECK(near_rgb(root, 1, 16 + 12, 255, 255, 255, 0));
	CHECK(near_rgb(root, 12, 16 + 12, 0, 0, 0, 0));
	CHECK(near_rgb(root, 12, 2, 0, 0, 0, 0)); /* nothing in the overhang */
	/* the check mark is drawn once checked; pressed, the box is the accent */
	CHECK(near_rgb(root, 11, 34, 0, 0, 0, 0));
	Ztoggle_button_set_is_checked(cb, true);
	CHECK(near_rgb(root, 11, 34, 255, 255, 255, 0));
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 60, 28);
	CHECK(near_rgb(root, 8, 16 + 4, 0x00, 0x50, 0xEF, 0));
	Zroot_inject_pointer(root, Z_POINTER_UP, 400, 250);
	/* a ToggleButton is the accent while checked, and the text over it is white */
	{
		ZRect r = Zelement_get_bounds(tg);
		int x = (int)r.x + 50, y = (int)(r.y + r.height * 0.5f) - 14;
		CHECK(near_rgb(root, x, y, 0, 0, 0, 0));
		Ztoggle_button_set_is_checked(tg, true);
		CHECK(near_rgb(root, x, y, 0x00, 0x50, 0xEF, 0));
	}
	Zroot_destroy(root);
}

static void test_text_box_pixels(void) {
	Zid root = zst_test_root(480, 300), stack = Zstack_panel_create(Z_VERTICAL), tb, other;
	int lit;
	Zroot_set_visual(root, stack);
	tb = Zpanel_add(stack, Ztext_box_create(""));
	Ztext_box_set_placeholder_text(tb, "placeholder");
	other = Zpanel_add(stack, Ztext_box_create("abc"));
	Zroot_update_layout(root);
	/* the box is 34 px high under 9.5 px of margin, white at 0.8 opacity (204) with a border of the same */
	CHECK(near_rgb(root, 400, 9 + 17, 204, 204, 204, 2));
	CHECK(near_rgb(root, 1, 9 + 17, 204, 204, 204, 2));
	CHECK(near_rgb(root, 400, 4, 0, 0, 0, 0));
	/* the placeholder is drawn while it is empty and has no focus; the focus takes it away and the box turns white with an accent border */
	lit = 0;
	{
		ZFrame f;
		int x, y;
		Zroot_render(root, &f);
		for (y = 12; y < 38; y++) for (x = 9; x < 150; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (abs(px[0] - 204) > 10) lit++;
		}
	}
	CHECK(lit > 50);
	Zelement_focus(tb);
	CHECK(near_rgb(root, 400, 9 + 17, 255, 255, 255, 0));
	CHECK(near_rgb(root, 1, 9 + 17, 0x00, 0x50, 0xEF, 0));
	lit = 0;
	{
		ZFrame f;
		int x, y;
		Zroot_render(root, &f);
		for (y = 14; y < 34; y++) for (x = 12; x < 150; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (px[0] != 255) lit++;
		}
	}
	CHECK(lit < 20); /* only the caret */
	/* disabled: no fill, the border is PhoneBaseLow (73FFFFFF over black: 115) */
	Zelement_set_enabled(other, false);
	Zelement_focus(tb);
	{
		ZRect r = Zelement_get_bounds(other);
		CHECK(near_rgb(root, 400, (int)r.y + 17, 0, 0, 0, 0));
		CHECK(near_rgb(root, 1, (int)r.y + 17, 115, 115, 115, 2));
	}
	/* Light: BFBFBF at 0.8 */
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zelement_set_enabled(other, true);
	{
		ZRect r = Zelement_get_bounds(other);
		/* 0.8 * BF over white */
		CHECK(near_rgb(root, 400, (int)r.y + 17, 255 - (int)(0.8f * (255 - 0xBF)), 255 - (int)(0.8f * (255 - 0xBF)), 255 - (int)(0.8f * (255 - 0xBF)), 2));
	}
	/* a header pushes the box down */
	Ztext_box_set_header(tb, "Header");
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(tb).height > 34 + 20);
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
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
	Zshutdown();
	return test_finish("test_render");
}
