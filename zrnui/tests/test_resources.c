/**
 * ThemeResource: the values of the keys of themeresources.xaml for the theme and the accent, overrides,
 * keys of the application, bindings of properties to keys, and the agreement of the two color tables.
 */

#include "test.h"
#include "../src/internal.h"

static void quiet_errors(ZError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static int same_color(ZColor c, uint32_t argb) {
	return fabs(c.a * 255 - (argb >> 24)) < 0.51 && fabs(c.r * 255 - ((argb >> 16) & 255)) < 0.51 &&
		fabs(c.g * 255 - ((argb >> 8) & 255)) < 0.51 && fabs(c.b * 255 - (argb & 255)) < 0.51;
}

static int near_color(ZColor a, ZColor b) {
	return fabs(a.r - b.r) < 0.003 && fabs(a.g - b.g) < 0.003 && fabs(a.b - b.b) < 0.003 && fabs(a.a - b.a) < 0.003;
}

/* how many pixels of the frame are this gray (give or take the rounding of a color to 8 bits) */
static int count_gray(Zid root, int gray) {
	ZFrame f;
	int x, y, n = 0;
	Zroot_render(root, &f);
	for (y = 0; y < f.height; y++) {
		for (x = 0; x < f.width; x++) {
			const uint8_t* p = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (abs(p[0] - gray) <= 1 && abs(p[1] - gray) <= 1 && abs(p[2] - gray) <= 1) n++;
		}
	}
	return n;
}

static void test_known_values(void) {
	Zid root = Zroot_create(480, 800);
	ZThickness t;
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	CHECK(Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -25);
	CHECK(Zroot_get_theme_font_family(root, Z_RES_PIVOT_HEADER_ITEM_FONT_FAMILY) == Z_FONT_SEMILIGHT);
	CHECK(Zroot_get_theme_font_family(root, Z_RES_PHONE_FONT_FAMILY_SEMI_BOLD) == Z_FONT_SEMIBOLD);
	CHECK(Zroot_get_theme_int(root, Z_RES_PHONE_BUTTON_FONT_WEIGHT) == 600);
	t = Zroot_get_theme_thickness(root, Z_RES_PIVOT_HEADER_ITEM_MARGIN);
	CHECK(t.left == 16 && t.top == -6.5f && t.right == 0 && t.bottom == 0);
	t = Zroot_get_theme_thickness(root, Z_RES_PHONE_TOUCH_TARGET_OVERHANG);
	CHECK(t.left == 0 && t.top == 9.5f && t.right == 0 && t.bottom == 9.5f);
	t = Zroot_get_theme_thickness(root, Z_RES_PHONE_BORDER_THICKNESS);
	CHECK(t.left == 2.5f && t.top == 2.5f && t.right == 2.5f && t.bottom == 2.5f);
	/* a Dark / Light pair */
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0xFFFFFFFF));
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_CHROME_BRUSH), 0xFF1F1F1F));
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_COMBO_BOX_FLYOUT_LIST_PLACEHOLDER_TEXT_OPACITY) * 100, 60);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0xFF000000));
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_BACKGROUND_BRUSH), 0xFFFFFFFF));
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_CHROME_BRUSH), 0xFFDDDDDD));
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_COMBO_BOX_FLYOUT_LIST_PLACEHOLDER_TEXT_OPACITY) * 100, 45);
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_RADIO_CHECK_BOX_BRUSH), 0x00FFFFFF)); /* Transparent, of the top level */
	Zroot_destroy(root);
}

static void test_accent(void) {
	Zid root = Zroot_create(100, 100);
	ZColor a = Zroot_get_accent_color(root), c;
	c = Zroot_get_theme_color(root, Z_RES_PHONE_ACCENT_BRUSH);
	CHECK(near_color(c, a));
	CHECK(near_color(Zroot_get_theme_color(root, Z_RES_SYSTEM_COLOR_CONTROL_ACCENT_COLOR), a));
	/* a brush made of the accent: ProgressBarBackgroundThemeBrush is the accent at 0.16 */
	c = Zroot_get_theme_color(root, Z_RES_PROGRESS_BAR_BACKGROUND_THEME_BRUSH);
	CHECK(fabs(c.r - a.r) < 0.003 && fabs(c.a - 0.16) < 0.004);
	Zroot_set_accent(root, Z_ACCENT_LIME);
	a = Zroot_get_accent_color(root);
	CHECK_NEAR(a.r * 255, 0xA4);
	CHECK(near_color(Zroot_get_theme_color(root, Z_RES_PHONE_ACCENT_BRUSH), a));
	CHECK(near_color(Zroot_get_theme_color(root, Z_RES_BUTTON_PRESSED_BACKGROUND_THEME_BRUSH), a));
	c = Zroot_get_theme_color(root, Z_RES_PROGRESS_BAR_BACKGROUND_THEME_BRUSH);
	CHECK(fabs(c.r - a.r) < 0.003 && fabs(c.a - 0.16) < 0.004);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(near_color(Zroot_get_theme_color(root, Z_RES_PHONE_ACCENT_BRUSH), a));
	Zroot_set_accent_color(root, Zcolor_rgba(0.25f, 0.5f, 0.75f, 1));
	c = Zroot_get_theme_color(root, Z_RES_PHONE_ACCENT_BRUSH);
	CHECK(near_color(c, Zcolor_rgba(0.25f, 0.5f, 0.75f, 1)));
	Zroot_destroy(root);
}

static void test_override_and_clear(void) {
	Zid root = Zroot_create(100, 100);
	ZThickness t = Zthickness(1, 2, 3, 4);
	CHECK(Zroot_set_resource_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 30));
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 30);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 30); /* in both themes */
	CHECK(Zroot_set_resource_int(root, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING, -5));
	CHECK(Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -5);
	CHECK(Zroot_set_resource_color(root, Z_RES_PHONE_FOREGROUND_BRUSH, Zcolor_rgba(1, 0, 0, 1)));
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0xFFFF0000));
	Zroot_set_theme(root, Z_THEME_DARK);
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0xFFFF0000));
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_COLOR), 0xFFFFFFFF)); /* another key */
	CHECK(Zroot_set_resource_thickness(root, Z_RES_PIVOT_HEADER_ITEM_MARGIN, t));
	CHECK(Zroot_get_theme_thickness(root, Z_RES_PIVOT_HEADER_ITEM_MARGIN).bottom == 4);
	CHECK(Zroot_set_resource_font_family(root, Z_RES_PIVOT_HEADER_ITEM_FONT_FAMILY, Z_FONT_BOLD));
	CHECK(Zroot_get_theme_font_family(root, Z_RES_PIVOT_HEADER_ITEM_FONT_FAMILY) == Z_FONT_BOLD);
	/* a second set replaces the first, a clear brings the theme back */
	CHECK(Zroot_set_resource_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 31));
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 31);
	Zroot_clear_resource(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE);
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	Zroot_clear_resource(root, Z_RES_PHONE_FOREGROUND_BRUSH);
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0xFFFFFFFF));
	Zroot_clear_resource(root, Z_RES_PHONE_FOREGROUND_BRUSH); /* nothing to clear: harmless */
	CHECK(Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -5);
	/* the root of another is not touched */
	{
		Zid other = Zroot_create(100, 100);
		CHECK_NEAR(Zroot_get_theme_double(other, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
		CHECK(Zroot_get_theme_int(other, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -25);
		Zroot_destroy(other);
	}
	Zroot_destroy(root);
}

static void test_user_keys(void) {
	Zid root = Zroot_create(100, 100);
	ZThemeResource k1 = (ZThemeResource)(Z_RES_USER_BASE + 1), k2 = (ZThemeResource)(Z_RES_USER_BASE + 2), far = (ZThemeResource)(Z_RES_USER_BASE + 100000);
	int i;
	CHECK(Zroot_get_theme_int(root, k1) == 0);
	CHECK(Zroot_set_resource_color(root, k1, Zcolor_rgba(0, 1, 0, 1)));
	CHECK(same_color(Zroot_get_theme_color(root, k1), 0xFF00FF00));
	CHECK_NEAR(Zroot_get_theme_double(root, k1), 0); /* a color, not a double */
	CHECK(Zroot_set_resource_double(root, k2, 12.5f));
	CHECK_NEAR(Zroot_get_theme_double(root, k2), 12.5);
	CHECK(same_color(Zroot_get_theme_color(root, k2), 0x00000000));
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(same_color(Zroot_get_theme_color(root, k1), 0xFF00FF00)); /* in both themes */
	CHECK(Zroot_set_resource_int(root, k1, 7)); /* the type of the last set */
	CHECK(Zroot_get_theme_int(root, k1) == 7);
	CHECK(same_color(Zroot_get_theme_color(root, k1), 0x00000000));
	CHECK(Zroot_set_resource_thickness(root, far, Zthickness_uniform(3)));
	CHECK(Zroot_get_theme_thickness(root, far).top == 3);
	Zroot_clear_resource(root, k1);
	CHECK(Zroot_get_theme_int(root, k1) == 0);
	CHECK_NEAR(Zroot_get_theme_double(root, k2), 12.5);
	for (i = 0; i < 40; i++) CHECK(Zroot_set_resource_int(root, (ZThemeResource)(Z_RES_USER_BASE + 1000 + i), i)); /* the table grows */
	for (i = 0; i < 40; i++) CHECK(Zroot_get_theme_int(root, (ZThemeResource)(Z_RES_USER_BASE + 1000 + i)) == i);
	Zroot_destroy(root);
}

static void test_invalid(void) {
	Zid root = Zroot_create(100, 100);
	ZColor c;
	CHECK(!Zroot_set_resource_double(root, Z_RES_NONE, 1));
	CHECK(!Zroot_set_resource_double(root, Z_RES_LAST_, 1));
	CHECK(!Zroot_set_resource_double(root, (ZThemeResource)(Z_RES_USER_BASE - 1), 1));
	CHECK(!Zroot_set_resource_double(root, (ZThemeResource)-5, 1));
	CHECK(!Zroot_set_resource_color(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE, Zcolor_rgba(1, 1, 1, 1))); /* a Double key */
	CHECK(!Zroot_set_resource_int(root, Z_RES_PHONE_FOREGROUND_BRUSH, 1));
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57); /* the refused did nothing */
	CHECK(!Zroot_set_resource_double(Z_INVALID_ID, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 1));
	/* every getter on a key of another type, on no key, on no root */
	CHECK_NEAR(Zroot_get_theme_double(root, Z_RES_PHONE_FOREGROUND_BRUSH), 0);
	CHECK(Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE) == 0);
	CHECK(same_color(Zroot_get_theme_color(root, Z_RES_PIVOT_HEADER_ITEM_MARGIN), 0x00000000));
	CHECK(Zroot_get_theme_thickness(root, Z_RES_PHONE_FOREGROUND_BRUSH).left == 0);
	CHECK(Zroot_get_theme_font_family(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE) == Z_FONT_NORMAL);
	c = Zroot_get_theme_color(root, Z_RES_NONE);
	CHECK(c.a == 0 && c.r == 0);
	c = Zroot_get_theme_color(root, Z_RES_LAST_);
	CHECK(c.a == 0);
	CHECK(Zroot_get_theme_int(root, (ZThemeResource)0x7000) == 0);
	CHECK(Zroot_get_theme_int(Z_INVALID_ID, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == 0);
	CHECK(Zroot_get_theme_color(Z_INVALID_ID, Z_RES_PHONE_FOREGROUND_BRUSH).a == 0);
	Zroot_clear_resource(Z_INVALID_ID, Z_RES_PHONE_FOREGROUND_BRUSH);
	Zroot_clear_resource(root, Z_RES_NONE);
	/* a binding of the wrong type, of no key, of no element, of another class */
	{
		Zid tb = Ztext_block_create("x"), border = Zborder_create(), button = Zbutton_create("b");
		CHECK(!Ztext_block_bind_foreground(tb, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
		CHECK(!Ztext_block_bind_font_size(tb, Z_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Ztext_block_bind_character_spacing(tb, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE)); /* a Double, not an Int */
		CHECK(!Ztext_block_bind_foreground(tb, Z_RES_NONE));
		CHECK(!Ztext_block_bind_foreground(tb, Z_RES_LAST_));
		CHECK(!Ztext_block_bind_foreground(border, Z_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Zborder_bind_background(tb, Z_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Zcontrol_bind_foreground(border, Z_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Zcontrol_bind_padding(button, Z_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Zelement_bind_margin(Z_INVALID_ID, Z_RES_PIVOT_ITEM_MARGIN));
		CHECK(!Zelement_bind_margin(button, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
		CHECK(Zelement_bind_margin(button, Z_RES_PIVOT_ITEM_MARGIN));
		CHECK(Zelement_bind_margin(tb, Z_RES_PIVOT_ITEM_MARGIN));
		Zelement_destroy(tb);
		Zelement_destroy(border);
		Zelement_destroy(button);
	}
	Zroot_destroy(root);
}

static void test_bindings(void) {
	Zid root = Zroot_create(200, 200);
	Zid button = Zbutton_create("b"), tb = Ztext_block_create("M");
	ZThemeResource user = (ZThemeResource)(Z_RES_USER_BASE + 7);
	ZThickness t;
	/* bound before the root has it: the default theme, then the root's values on attach */
	CHECK(Zcontrol_bind_font_size(button, Z_RES_TEXT_STYLE_LARGE_FONT_SIZE));
	CHECK_NEAR(Zcontrol_get_font_size(button), 18.14);
	CHECK(Zcontrol_bind_padding(button, Z_RES_PHONE_BUTTON_CONTENT_PADDING));
	t = Zcontrol_get_padding(button);
	CHECK(t.left == 9.5f && t.bottom == 3.5f);
	Zroot_set_resource_double(root, Z_RES_TEXT_STYLE_LARGE_FONT_SIZE, 26);
	CHECK_NEAR(Zcontrol_get_font_size(button), 18.14); /* not in the root yet */
	Zroot_set_visual(root, button);
	CHECK_NEAR(Zcontrol_get_font_size(button), 26);
	/* an override, a clear */
	Zroot_set_resource_double(root, Z_RES_TEXT_STYLE_LARGE_FONT_SIZE, 30);
	CHECK_NEAR(Zcontrol_get_font_size(button), 30);
	Zroot_clear_resource(root, Z_RES_TEXT_STYLE_LARGE_FONT_SIZE);
	CHECK_NEAR(Zcontrol_get_font_size(button), 18.14);
	Zroot_set_resource_thickness(root, Z_RES_PHONE_BUTTON_CONTENT_PADDING, Zthickness_uniform(5));
	CHECK(Zcontrol_get_padding(button).top == 5);
	/* a direct setter wins and removes the binding */
	Zcontrol_set_font_size(button, 40);
	Zroot_set_resource_double(root, Z_RES_TEXT_STYLE_LARGE_FONT_SIZE, 22);
	CHECK_NEAR(Zcontrol_get_font_size(button), 40);
	Zcontrol_set_padding(button, Zthickness_uniform(1));
	Zroot_clear_resource(root, Z_RES_PHONE_BUTTON_CONTENT_PADDING);
	CHECK(Zcontrol_get_padding(button).top == 1);
	/* margin of any element */
	CHECK(Zelement_bind_margin(button, Z_RES_PIVOT_ITEM_MARGIN));
	CHECK(Zelement_get_margin(button).left == 19 && Zelement_get_margin(button).top == 22.5f);
	Zroot_set_resource_thickness(root, Z_RES_PIVOT_ITEM_MARGIN, Zthickness(1, 2, 3, 4));
	CHECK(Zelement_get_margin(button).right == 3);
	Zelement_set_margin(button, Zthickness_uniform(8));
	Zroot_clear_resource(root, Z_RES_PIVOT_ITEM_MARGIN);
	CHECK(Zelement_get_margin(button).left == 8);
	/* a key of the application, set after the binding */
	Zroot_set_visual(root, tb);
	Zelement_set_horizontal_alignment(tb, Z_HALIGN_LEFT);
	Zelement_set_vertical_alignment(tb, Z_VALIGN_TOP);
	CHECK(Ztext_block_bind_font_size(tb, user));
	CHECK(Ztext_block_bind_foreground(tb, Z_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR));
	Zroot_set_resource_double(root, user, 120);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(tb).height > 100);
	Zroot_set_resource_double(root, user, 30);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(tb).height < 60);
	Zroot_set_resource_double(root, user, 120);
	Zroot_clear_resource(root, user); /* gone: the property keeps what it had */
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(tb).height > 100);
	/* the text block: a theme change recolors it (0x66 gray in Dark, 0xCC in Light) */
	CHECK(count_gray(root, 0x66) > 50);
	CHECK(count_gray(root, 0xCC) == 0);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(count_gray(root, 0xCC) > 50);
	CHECK(count_gray(root, 0x66) == 0);
	Zroot_set_theme(root, Z_THEME_DARK);
	CHECK(count_gray(root, 0x66) > 50);
	/* the override of the color */
	Zroot_set_resource_color(root, Z_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR, Zcolor_argb(0xFF808080));
	CHECK(count_gray(root, 0x80) > 50 && count_gray(root, 0x80) > 3 * count_gray(root, 0x66)); /* the 0x66 left is the edge of the glyphs */
	Zroot_clear_resource(root, Z_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR);
	CHECK(count_gray(root, 0x66) > 50);
	/* the direct setter: the color stays */
	Ztext_block_set_foreground(tb, Zcolor_argb(0xFF909090));
	Zroot_set_theme(root, Z_THEME_LIGHT);
	CHECK(count_gray(root, 0x90) > 50 && count_gray(root, 0x66) == 0); /* on white, no edge is darker than the text */
	/* character spacing, an Int key */
	Zroot_set_theme(root, Z_THEME_DARK);
	CHECK(Ztext_block_bind_character_spacing(tb, Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING));
	Zroot_destroy(root);
}

static void test_bound_accent_and_border(void) {
	Zid root = Zroot_create(100, 100);
	Zid border = Zborder_create();
	int r, g, b;
	Zelement_set_width(border, 100);
	Zelement_set_height(border, 100);
	CHECK(Zborder_bind_background(border, Z_RES_PHONE_ACCENT_BRUSH));
	CHECK(Zborder_bind_border_thickness(border, Z_RES_PHONE_BORDER_THICKNESS));
	CHECK(Zborder_bind_border_brush(border, Z_RES_PHONE_FOREGROUND_BRUSH));
	CHECK(Zborder_bind_padding(border, Z_RES_PHONE_BUTTON_CONTENT_PADDING));
	Zroot_set_visual(root, border);
	Zroot_set_accent(root, Z_ACCENT_LIME);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0xA4 && g == 0xC4 && b == 0x00);
	Zroot_set_accent(root, Z_ACCENT_RED);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0xE5 && g == 0x14 && b == 0x00);
	test_pixel(root, 1, 50, &r, &g, &b); /* the border brush, white in Dark */
	CHECK(r == 255 && g == 255 && b == 255);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	test_pixel(root, 1, 50, &r, &g, &b);
	CHECK(r == 0 && g == 0 && b == 0);
	Zborder_set_background(border, Zcolor_argb(0xFF0000FF));
	Zroot_set_accent(root, Z_ACCENT_LIME);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0 && g == 0 && b == 255);
	Zroot_destroy(root);
}

static void test_destroy_paths(void) {
	/* bound elements destroyed before and with the root, a theme change in between */
	Zid root = Zroot_create(100, 100);
	Zid panel = Zstack_panel_create(Z_VERTICAL);
	Zid a = Ztext_block_create("a"), b = Ztext_block_create("b"), c = Ztext_block_create("c");
	Zpanel_add(panel, a);
	Zpanel_add(panel, b);
	Zpanel_add(panel, c);
	Ztext_block_bind_foreground(a, Z_RES_PHONE_FOREGROUND_BRUSH);
	Ztext_block_bind_font_size(a, Z_RES_USER_BASE);
	Ztext_block_bind_foreground(b, Z_RES_PHONE_ACCENT_BRUSH);
	Ztext_block_bind_foreground(c, Z_RES_PHONE_BACKGROUND_BRUSH);
	Zroot_set_visual(root, panel);
	Zelement_destroy(b);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zroot_set_resource_double(root, Z_RES_USER_BASE, 20);
	Zpanel_remove(panel, c); /* out of the tree, alive: no longer followed, applied again when it is back */
	Zroot_set_theme(root, Z_THEME_DARK);
	Zpanel_add(panel, c);
	Zroot_render(root, NULL);
	Zelement_destroy(c);
	Zroot_destroy(root); /* with a and the panel */
}

/* what the generated table and the internal one both define, in both themes */
static void test_tables_agree(void) {
	static const struct { ZThemeResource key; zrn_theme_color_id_t id; } pairs[] = {
		{ Z_RES_PHONE_FOREGROUND_COLOR, ZST_TC_FOREGROUND }, { Z_RES_PHONE_FOREGROUND_BRUSH, ZST_TC_FOREGROUND },
		{ Z_RES_PHONE_BACKGROUND_COLOR, ZST_TC_BACKGROUND }, { Z_RES_PHONE_BACKGROUND_BRUSH, ZST_TC_BACKGROUND },
		/* ZST_TC_DISABLED is the WP8 PhoneDisabledColor (0x4D in Light), not the 8.1 one that ZST_TC_PHONE_DISABLED is */
		{ Z_RES_PHONE_DISABLED_COLOR, ZST_TC_PHONE_DISABLED },
		{ Z_RES_PHONE_BUTTON_BASE_PRESSED_FOREGROUND_BRUSH, ZST_TC_BUTTON_PRESSED_FOREGROUND },
		{ Z_RES_PHONE_CHROME_COLOR, ZST_TC_CHROME }, { Z_RES_PHONE_CHROME_BRUSH, ZST_TC_CHROME },
		{ Z_RES_PHONE_ACCENT_BRUSH, ZST_TC_ACCENT }, { Z_RES_SYSTEM_COLOR_CONTROL_ACCENT_COLOR, ZST_TC_ACCENT },
		{ Z_RES_PHONE_BASE_MEDIUM_HIGH_COLOR, ZST_TC_BASE_MEDIUM_HIGH }, { Z_RES_PHONE_BASE_MID_COLOR, ZST_TC_BASE_MID },
		{ Z_RES_PHONE_BASE_MEDIUM_MID_COLOR, ZST_TC_BASE_MEDIUM_MID }, { Z_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR, ZST_TC_BASE_MEDIUM_MID_SOLID },
		{ Z_RES_PHONE_BASE_LOW_COLOR, ZST_TC_BASE_LOW }, { Z_RES_PHONE_BASE_LOW_SOLID_COLOR, ZST_TC_BASE_LOW_SOLID },
		{ Z_RES_PHONE_TEXT_OVER_ACCENT_COLOR, ZST_TC_TEXT_OVER_ACCENT },
		{ Z_RES_TEXT_BOX_BACKGROUND_COLOR, ZST_TC_TEXT_BOX_BACKGROUND_81 }, { Z_RES_TEXT_BOX_BORDER_COLOR, ZST_TC_TEXT_BOX_BORDER_81 },
		{ Z_RES_TEXT_BOX_FOREGROUND_COLOR, ZST_TC_TEXT_BOX_FOREGROUND_81 }, { Z_RES_TEXT_BOX_PLACEHOLDER_COLOR, ZST_TC_TEXT_BOX_PLACEHOLDER_81 },
		{ Z_RES_TEXT_BOX_FOCUSED_BACKGROUND_THEME_BRUSH, ZST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81 },
		{ Z_RES_SLIDER_THUMB_BACKGROUND_THEME_BRUSH, ZST_TC_SLIDER_THUMB },
		{ Z_RES_SLIDER_THUMB_DISABLED_BACKGROUND_THEME_BRUSH, ZST_TC_SLIDER_THUMB_DISABLED },
		{ Z_RES_SLIDER_TRACK_BACKGROUND_THEME_BRUSH, ZST_TC_SLIDER_TRACK },
		{ Z_RES_SLIDER_TRACK_DECREASE_DISABLED_BACKGROUND_THEME_BRUSH, ZST_TC_SLIDER_DECREASE_DISABLED },
		{ Z_RES_PHONE_BASE_BLACK_COLOR, ZST_TC_BASE_BLACK }, { Z_RES_PHONE_BASE_WHITE_COLOR, ZST_TC_BASE_WHITE },
		{ Z_RES_PHONE_TEXT_MID_COLOR, ZST_TC_TEXT_MID }, { Z_RES_PHONE_TEXT_LOW_COLOR, ZST_TC_TEXT_LOW },
		{ Z_RES_CONTENT_DIALOG_DIMMING_COLOR, ZST_TC_CONTENT_DIALOG_DIMMING }
	};
	Zid root = Zroot_create(100, 100);
	int theme, accent, i;
	for (theme = 0; theme < 2; theme++) {
		for (accent = 0; accent < 2; accent++) {
			zrn_theme_t t;
			Zroot_set_theme(root, theme ? Z_THEME_LIGHT : Z_THEME_DARK);
			Zroot_set_accent(root, accent ? Z_ACCENT_ORANGE : Z_ACCENT_COBALT);
			zrn_theme_init(&t, theme ? Z_THEME_LIGHT : Z_THEME_DARK, Zroot_get_accent_color(root));
			for (i = 0; i < (int)(sizeof pairs / sizeof pairs[0]); i++) {
				ZColor got = Zroot_get_theme_color(root, pairs[i].key), want = t.colors[pairs[i].id];
				if (!near_color(got, want)) printf("  key 0x%X (id %d), theme %d: got %g %g %g %g, internal %g %g %g %g\n", pairs[i].key, pairs[i].id, theme,
					got.r, got.g, got.b, got.a, want.r, want.g, want.b, want.a);
				CHECK(near_color(got, want));
			}
		}
	}
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
	Zset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	RUN(test_known_values);
	RUN(test_accent);
	RUN(test_override_and_clear);
	RUN(test_user_keys);
	RUN(test_invalid);
	RUN(test_bindings);
	RUN(test_bound_accent_and_border);
	RUN(test_destroy_paths);
	RUN(test_tables_agree);
	Zshutdown();
	return test_finish("test_resources");
}
