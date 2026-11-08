/**
 * ThemeResource: the values of the keys of themeresources.xaml for the theme and the accent, overrides,
 * keys of the application, bindings of properties to keys, and the agreement of the two color tables.
 */

#include "test.h"
#include "../src/internal.h"

static void quiet_errors(KError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static int same_color(KColor c, uint32_t argb) {
	return fabs(c.a * 255 - (argb >> 24)) < 0.51 && fabs(c.r * 255 - ((argb >> 16) & 255)) < 0.51 &&
		fabs(c.g * 255 - ((argb >> 8) & 255)) < 0.51 && fabs(c.b * 255 - (argb & 255)) < 0.51;
}

static int near_color(KColor a, KColor b) {
	return fabs(a.r - b.r) < 0.003 && fabs(a.g - b.g) < 0.003 && fabs(a.b - b.b) < 0.003 && fabs(a.a - b.a) < 0.003;
}

/* how many pixels of the frame are this gray (give or take the rounding of a color to 8 bits) */
static int count_gray(Kid root, int gray) {
	KFrame f;
	int x, y, n = 0;
	Kroot_render(root, &f);
	for (y = 0; y < f.height; y++) {
		for (x = 0; x < f.width; x++) {
			const uint8_t* p = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (abs(p[0] - gray) <= 1 && abs(p[1] - gray) <= 1 && abs(p[2] - gray) <= 1) n++;
		}
	}
	return n;
}

static void test_known_values(void) {
	Kid root = Kroot_create(480, 800);
	KThickness t;
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	CHECK(Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -25);
	CHECK(Kroot_get_theme_font_family(root, K_RES_PIVOT_HEADER_ITEM_FONT_FAMILY) == K_FONT_SEMILIGHT);
	CHECK(Kroot_get_theme_font_family(root, K_RES_PHONE_FONT_FAMILY_SEMI_BOLD) == K_FONT_SEMIBOLD);
	CHECK(Kroot_get_theme_int(root, K_RES_PHONE_BUTTON_FONT_WEIGHT) == 600);
	t = Kroot_get_theme_thickness(root, K_RES_PIVOT_HEADER_ITEM_MARGIN);
	CHECK(t.left == 16 && t.top == -6.5f && t.right == 0 && t.bottom == 0);
	t = Kroot_get_theme_thickness(root, K_RES_PHONE_TOUCH_TARGET_OVERHANG);
	CHECK(t.left == 0 && t.top == 9.5f && t.right == 0 && t.bottom == 9.5f);
	t = Kroot_get_theme_thickness(root, K_RES_PHONE_BORDER_THICKNESS);
	CHECK(t.left == 2.5f && t.top == 2.5f && t.right == 2.5f && t.bottom == 2.5f);
	/* a Dark / Light pair */
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_BRUSH), 0xFFFFFFFF));
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_CHROME_BRUSH), 0xFF1F1F1F));
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_COMBO_BOX_FLYOUT_LIST_PLACEHOLDER_TEXT_OPACITY) * 100, 60);
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_BRUSH), 0xFF000000));
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_BACKGROUND_BRUSH), 0xFFFFFFFF));
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_CHROME_BRUSH), 0xFFDDDDDD));
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_COMBO_BOX_FLYOUT_LIST_PLACEHOLDER_TEXT_OPACITY) * 100, 45);
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_RADIO_CHECK_BOX_BRUSH), 0x00FFFFFF)); /* Transparent, of the top level */
	Kroot_destroy(root);
}

static void test_accent(void) {
	Kid root = Kroot_create(100, 100);
	KColor a = Kroot_get_accent_color(root), c;
	c = Kroot_get_theme_color(root, K_RES_PHONE_ACCENT_BRUSH);
	CHECK(near_color(c, a));
	CHECK(near_color(Kroot_get_theme_color(root, K_RES_SYSTEM_COLOR_CONTROL_ACCENT_COLOR), a));
	/* a brush made of the accent: ProgressBarBackgroundThemeBrush is the accent at 0.16 */
	c = Kroot_get_theme_color(root, K_RES_PROGRESS_BAR_BACKGROUND_THEME_BRUSH);
	CHECK(fabs(c.r - a.r) < 0.003 && fabs(c.a - 0.16) < 0.004);
	Kroot_set_accent(root, K_ACCENT_LIME);
	a = Kroot_get_accent_color(root);
	CHECK_NEAR(a.r * 255, 0xA4);
	CHECK(near_color(Kroot_get_theme_color(root, K_RES_PHONE_ACCENT_BRUSH), a));
	CHECK(near_color(Kroot_get_theme_color(root, K_RES_BUTTON_PRESSED_BACKGROUND_THEME_BRUSH), a));
	c = Kroot_get_theme_color(root, K_RES_PROGRESS_BAR_BACKGROUND_THEME_BRUSH);
	CHECK(fabs(c.r - a.r) < 0.003 && fabs(c.a - 0.16) < 0.004);
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(near_color(Kroot_get_theme_color(root, K_RES_PHONE_ACCENT_BRUSH), a));
	Kroot_set_accent_color(root, Kcolor_rgba(0.25f, 0.5f, 0.75f, 1));
	c = Kroot_get_theme_color(root, K_RES_PHONE_ACCENT_BRUSH);
	CHECK(near_color(c, Kcolor_rgba(0.25f, 0.5f, 0.75f, 1)));
	Kroot_destroy(root);
}

static void test_override_and_clear(void) {
	Kid root = Kroot_create(100, 100);
	KThickness t = Kthickness(1, 2, 3, 4);
	CHECK(Kroot_set_resource_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 30));
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 30);
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 30); /* in both themes */
	CHECK(Kroot_set_resource_int(root, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING, -5));
	CHECK(Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -5);
	CHECK(Kroot_set_resource_color(root, K_RES_PHONE_FOREGROUND_BRUSH, Kcolor_rgba(1, 0, 0, 1)));
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_BRUSH), 0xFFFF0000));
	Kroot_set_theme(root, K_THEME_DARK);
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_BRUSH), 0xFFFF0000));
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_COLOR), 0xFFFFFFFF)); /* another key */
	CHECK(Kroot_set_resource_thickness(root, K_RES_PIVOT_HEADER_ITEM_MARGIN, t));
	CHECK(Kroot_get_theme_thickness(root, K_RES_PIVOT_HEADER_ITEM_MARGIN).bottom == 4);
	CHECK(Kroot_set_resource_font_family(root, K_RES_PIVOT_HEADER_ITEM_FONT_FAMILY, K_FONT_BOLD));
	CHECK(Kroot_get_theme_font_family(root, K_RES_PIVOT_HEADER_ITEM_FONT_FAMILY) == K_FONT_BOLD);
	/* a second set replaces the first, a clear brings the theme back */
	CHECK(Kroot_set_resource_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 31));
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 31);
	Kroot_clear_resource(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE);
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
	Kroot_clear_resource(root, K_RES_PHONE_FOREGROUND_BRUSH);
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_BRUSH), 0xFFFFFFFF));
	Kroot_clear_resource(root, K_RES_PHONE_FOREGROUND_BRUSH); /* nothing to clear: harmless */
	CHECK(Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -5);
	/* the root of another is not touched */
	{
		Kid other = Kroot_create(100, 100);
		CHECK_NEAR(Kroot_get_theme_double(other, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57);
		CHECK(Kroot_get_theme_int(other, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == -25);
		Kroot_destroy(other);
	}
	Kroot_destroy(root);
}

static void test_user_keys(void) {
	Kid root = Kroot_create(100, 100);
	KThemeResource k1 = (KThemeResource)(K_RES_USER_BASE + 1), k2 = (KThemeResource)(K_RES_USER_BASE + 2), far = (KThemeResource)(K_RES_USER_BASE + 100000);
	int i;
	CHECK(Kroot_get_theme_int(root, k1) == 0);
	CHECK(Kroot_set_resource_color(root, k1, Kcolor_rgba(0, 1, 0, 1)));
	CHECK(same_color(Kroot_get_theme_color(root, k1), 0xFF00FF00));
	CHECK_NEAR(Kroot_get_theme_double(root, k1), 0); /* a color, not a double */
	CHECK(Kroot_set_resource_double(root, k2, 12.5f));
	CHECK_NEAR(Kroot_get_theme_double(root, k2), 12.5);
	CHECK(same_color(Kroot_get_theme_color(root, k2), 0x00000000));
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(same_color(Kroot_get_theme_color(root, k1), 0xFF00FF00)); /* in both themes */
	CHECK(Kroot_set_resource_int(root, k1, 7)); /* the type of the last set */
	CHECK(Kroot_get_theme_int(root, k1) == 7);
	CHECK(same_color(Kroot_get_theme_color(root, k1), 0x00000000));
	CHECK(Kroot_set_resource_thickness(root, far, Kthickness_uniform(3)));
	CHECK(Kroot_get_theme_thickness(root, far).top == 3);
	Kroot_clear_resource(root, k1);
	CHECK(Kroot_get_theme_int(root, k1) == 0);
	CHECK_NEAR(Kroot_get_theme_double(root, k2), 12.5);
	for (i = 0; i < 40; i++) CHECK(Kroot_set_resource_int(root, (KThemeResource)(K_RES_USER_BASE + 1000 + i), i)); /* the table grows */
	for (i = 0; i < 40; i++) CHECK(Kroot_get_theme_int(root, (KThemeResource)(K_RES_USER_BASE + 1000 + i)) == i);
	Kroot_destroy(root);
}

static void test_invalid(void) {
	Kid root = Kroot_create(100, 100);
	KColor c;
	CHECK(!Kroot_set_resource_double(root, K_RES_NONE, 1));
	CHECK(!Kroot_set_resource_double(root, K_RES_LAST_, 1));
	CHECK(!Kroot_set_resource_double(root, (KThemeResource)(K_RES_USER_BASE - 1), 1));
	CHECK(!Kroot_set_resource_double(root, (KThemeResource)-5, 1));
	CHECK(!Kroot_set_resource_color(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE, Kcolor_rgba(1, 1, 1, 1))); /* a Double key */
	CHECK(!Kroot_set_resource_int(root, K_RES_PHONE_FOREGROUND_BRUSH, 1));
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE), 57); /* the refused did nothing */
	CHECK(!Kroot_set_resource_double(K_INVALID_ID, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE, 1));
	/* every getter on a key of another type, on no key, on no root */
	CHECK_NEAR(Kroot_get_theme_double(root, K_RES_PHONE_FOREGROUND_BRUSH), 0);
	CHECK(Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE) == 0);
	CHECK(same_color(Kroot_get_theme_color(root, K_RES_PIVOT_HEADER_ITEM_MARGIN), 0x00000000));
	CHECK(Kroot_get_theme_thickness(root, K_RES_PHONE_FOREGROUND_BRUSH).left == 0);
	CHECK(Kroot_get_theme_font_family(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE) == K_FONT_NORMAL);
	c = Kroot_get_theme_color(root, K_RES_NONE);
	CHECK(c.a == 0 && c.r == 0);
	c = Kroot_get_theme_color(root, K_RES_LAST_);
	CHECK(c.a == 0);
	CHECK(Kroot_get_theme_int(root, (KThemeResource)0x7000) == 0);
	CHECK(Kroot_get_theme_int(K_INVALID_ID, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING) == 0);
	CHECK(Kroot_get_theme_color(K_INVALID_ID, K_RES_PHONE_FOREGROUND_BRUSH).a == 0);
	Kroot_clear_resource(K_INVALID_ID, K_RES_PHONE_FOREGROUND_BRUSH);
	Kroot_clear_resource(root, K_RES_NONE);
	/* a binding of the wrong type, of no key, of no element, of another class */
	{
		Kid tb = Ktext_block_create("x"), border = Kborder_create(), button = Kbutton_create("b");
		CHECK(!Ktext_block_bind_foreground(tb, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
		CHECK(!Ktext_block_bind_font_size(tb, K_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Ktext_block_bind_character_spacing(tb, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE)); /* a Double, not an Int */
		CHECK(!Ktext_block_bind_foreground(tb, K_RES_NONE));
		CHECK(!Ktext_block_bind_foreground(tb, K_RES_LAST_));
		CHECK(!Ktext_block_bind_foreground(border, K_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Kborder_bind_background(tb, K_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Kcontrol_bind_foreground(border, K_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Kcontrol_bind_padding(button, K_RES_PHONE_FOREGROUND_BRUSH));
		CHECK(!Kelement_bind_margin(K_INVALID_ID, K_RES_PIVOT_ITEM_MARGIN));
		CHECK(!Kelement_bind_margin(button, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
		CHECK(Kelement_bind_margin(button, K_RES_PIVOT_ITEM_MARGIN));
		CHECK(Kelement_bind_margin(tb, K_RES_PIVOT_ITEM_MARGIN));
		Kelement_destroy(tb);
		Kelement_destroy(border);
		Kelement_destroy(button);
	}
	Kroot_destroy(root);
}

static void test_bindings(void) {
	Kid root = Kroot_create(200, 200);
	Kid button = Kbutton_create("b"), tb = Ktext_block_create("M");
	KThemeResource user = (KThemeResource)(K_RES_USER_BASE + 7);
	KThickness t;
	/* bound before the root has it: the default theme, then the root's values on attach */
	CHECK(Kcontrol_bind_font_size(button, K_RES_TEXT_STYLE_LARGE_FONT_SIZE));
	CHECK_NEAR(Kcontrol_get_font_size(button), 18.14);
	CHECK(Kcontrol_bind_padding(button, K_RES_PHONE_BUTTON_CONTENT_PADDING));
	t = Kcontrol_get_padding(button);
	CHECK(t.left == 9.5f && t.bottom == 3.5f);
	Kroot_set_resource_double(root, K_RES_TEXT_STYLE_LARGE_FONT_SIZE, 26);
	CHECK_NEAR(Kcontrol_get_font_size(button), 18.14); /* not in the root yet */
	Kroot_set_visual(root, button);
	CHECK_NEAR(Kcontrol_get_font_size(button), 26);
	/* an override, a clear */
	Kroot_set_resource_double(root, K_RES_TEXT_STYLE_LARGE_FONT_SIZE, 30);
	CHECK_NEAR(Kcontrol_get_font_size(button), 30);
	Kroot_clear_resource(root, K_RES_TEXT_STYLE_LARGE_FONT_SIZE);
	CHECK_NEAR(Kcontrol_get_font_size(button), 18.14);
	Kroot_set_resource_thickness(root, K_RES_PHONE_BUTTON_CONTENT_PADDING, Kthickness_uniform(5));
	CHECK(Kcontrol_get_padding(button).top == 5);
	/* a direct setter wins and removes the binding */
	Kcontrol_set_font_size(button, 40);
	Kroot_set_resource_double(root, K_RES_TEXT_STYLE_LARGE_FONT_SIZE, 22);
	CHECK_NEAR(Kcontrol_get_font_size(button), 40);
	Kcontrol_set_padding(button, Kthickness_uniform(1));
	Kroot_clear_resource(root, K_RES_PHONE_BUTTON_CONTENT_PADDING);
	CHECK(Kcontrol_get_padding(button).top == 1);
	/* margin of any element */
	CHECK(Kelement_bind_margin(button, K_RES_PIVOT_ITEM_MARGIN));
	CHECK(Kelement_get_margin(button).left == 19 && Kelement_get_margin(button).top == 22.5f);
	Kroot_set_resource_thickness(root, K_RES_PIVOT_ITEM_MARGIN, Kthickness(1, 2, 3, 4));
	CHECK(Kelement_get_margin(button).right == 3);
	Kelement_set_margin(button, Kthickness_uniform(8));
	Kroot_clear_resource(root, K_RES_PIVOT_ITEM_MARGIN);
	CHECK(Kelement_get_margin(button).left == 8);
	/* a key of the application, set after the binding */
	Kroot_set_visual(root, tb);
	Kelement_set_horizontal_alignment(tb, K_HALIGN_LEFT);
	Kelement_set_vertical_alignment(tb, K_VALIGN_TOP);
	CHECK(Ktext_block_bind_font_size(tb, user));
	CHECK(Ktext_block_bind_foreground(tb, K_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR));
	Kroot_set_resource_double(root, user, 120);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(tb).height > 100);
	Kroot_set_resource_double(root, user, 30);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(tb).height < 60);
	Kroot_set_resource_double(root, user, 120);
	Kroot_clear_resource(root, user); /* gone: the property keeps what it had */
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(tb).height > 100);
	/* the text block: a theme change recolors it (0x66 gray in Dark, 0xCC in Light) */
	CHECK(count_gray(root, 0x66) > 50);
	CHECK(count_gray(root, 0xCC) == 0);
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(count_gray(root, 0xCC) > 50);
	CHECK(count_gray(root, 0x66) == 0);
	Kroot_set_theme(root, K_THEME_DARK);
	CHECK(count_gray(root, 0x66) > 50);
	/* the override of the color */
	Kroot_set_resource_color(root, K_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR, Kcolor_argb(0xFF808080));
	CHECK(count_gray(root, 0x80) > 50 && count_gray(root, 0x80) > 3 * count_gray(root, 0x66)); /* the 0x66 left is the edge of the glyphs */
	Kroot_clear_resource(root, K_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR);
	CHECK(count_gray(root, 0x66) > 50);
	/* the direct setter: the color stays */
	Ktext_block_set_foreground(tb, Kcolor_argb(0xFF909090));
	Kroot_set_theme(root, K_THEME_LIGHT);
	CHECK(count_gray(root, 0x90) > 50 && count_gray(root, 0x66) == 0); /* on white, no edge is darker than the text */
	/* character spacing, an Int key */
	Kroot_set_theme(root, K_THEME_DARK);
	CHECK(Ktext_block_bind_character_spacing(tb, K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING));
	Kroot_destroy(root);
}

static void test_bound_accent_and_border(void) {
	Kid root = Kroot_create(100, 100);
	Kid border = Kborder_create();
	int r, g, b;
	Kelement_set_width(border, 100);
	Kelement_set_height(border, 100);
	CHECK(Kborder_bind_background(border, K_RES_PHONE_ACCENT_BRUSH));
	CHECK(Kborder_bind_border_thickness(border, K_RES_PHONE_BORDER_THICKNESS));
	CHECK(Kborder_bind_border_brush(border, K_RES_PHONE_FOREGROUND_BRUSH));
	CHECK(Kborder_bind_padding(border, K_RES_PHONE_BUTTON_CONTENT_PADDING));
	Kroot_set_visual(root, border);
	Kroot_set_accent(root, K_ACCENT_LIME);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0xA4 && g == 0xC4 && b == 0x00);
	Kroot_set_accent(root, K_ACCENT_RED);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0xE5 && g == 0x14 && b == 0x00);
	test_pixel(root, 1, 50, &r, &g, &b); /* the border brush, white in Dark */
	CHECK(r == 255 && g == 255 && b == 255);
	Kroot_set_theme(root, K_THEME_LIGHT);
	test_pixel(root, 1, 50, &r, &g, &b);
	CHECK(r == 0 && g == 0 && b == 0);
	Kborder_set_background(border, Kcolor_argb(0xFF0000FF));
	Kroot_set_accent(root, K_ACCENT_LIME);
	test_pixel(root, 50, 50, &r, &g, &b);
	CHECK(r == 0 && g == 0 && b == 255);
	Kroot_destroy(root);
}

static void test_destroy_paths(void) {
	/* bound elements destroyed before and with the root, a theme change in between */
	Kid root = Kroot_create(100, 100);
	Kid panel = Kstack_panel_create(K_VERTICAL);
	Kid a = Ktext_block_create("a"), b = Ktext_block_create("b"), c = Ktext_block_create("c");
	Kpanel_add(panel, a);
	Kpanel_add(panel, b);
	Kpanel_add(panel, c);
	Ktext_block_bind_foreground(a, K_RES_PHONE_FOREGROUND_BRUSH);
	Ktext_block_bind_font_size(a, K_RES_USER_BASE);
	Ktext_block_bind_foreground(b, K_RES_PHONE_ACCENT_BRUSH);
	Ktext_block_bind_foreground(c, K_RES_PHONE_BACKGROUND_BRUSH);
	Kroot_set_visual(root, panel);
	Kelement_destroy(b);
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kroot_set_resource_double(root, K_RES_USER_BASE, 20);
	Kpanel_remove(panel, c); /* out of the tree, alive: no longer followed, applied again when it is back */
	Kroot_set_theme(root, K_THEME_DARK);
	Kpanel_add(panel, c);
	Kroot_render(root, NULL);
	Kelement_destroy(c);
	Kroot_destroy(root); /* with a and the panel */
}

/* what the generated table and the internal one both define, in both themes */
static void test_tables_agree(void) {
	static const struct { KThemeResource key; kst_theme_color_id_t id; } pairs[] = {
		{ K_RES_PHONE_FOREGROUND_COLOR, KST_TC_FOREGROUND }, { K_RES_PHONE_FOREGROUND_BRUSH, KST_TC_FOREGROUND },
		{ K_RES_PHONE_BACKGROUND_COLOR, KST_TC_BACKGROUND }, { K_RES_PHONE_BACKGROUND_BRUSH, KST_TC_BACKGROUND },
		/* KST_TC_DISABLED is the WP8 PhoneDisabledColor (0x4D in Light), not the 8.1 one that KST_TC_PHONE_DISABLED is */
		{ K_RES_PHONE_DISABLED_COLOR, KST_TC_PHONE_DISABLED },
		{ K_RES_PHONE_BUTTON_BASE_PRESSED_FOREGROUND_BRUSH, KST_TC_BUTTON_PRESSED_FOREGROUND },
		{ K_RES_PHONE_CHROME_COLOR, KST_TC_CHROME }, { K_RES_PHONE_CHROME_BRUSH, KST_TC_CHROME },
		{ K_RES_PHONE_ACCENT_BRUSH, KST_TC_ACCENT }, { K_RES_SYSTEM_COLOR_CONTROL_ACCENT_COLOR, KST_TC_ACCENT },
		{ K_RES_PHONE_BASE_MEDIUM_HIGH_COLOR, KST_TC_BASE_MEDIUM_HIGH }, { K_RES_PHONE_BASE_MID_COLOR, KST_TC_BASE_MID },
		{ K_RES_PHONE_BASE_MEDIUM_MID_COLOR, KST_TC_BASE_MEDIUM_MID }, { K_RES_PHONE_BASE_MEDIUM_MID_SOLID_COLOR, KST_TC_BASE_MEDIUM_MID_SOLID },
		{ K_RES_PHONE_BASE_LOW_COLOR, KST_TC_BASE_LOW }, { K_RES_PHONE_BASE_LOW_SOLID_COLOR, KST_TC_BASE_LOW_SOLID },
		{ K_RES_PHONE_TEXT_OVER_ACCENT_COLOR, KST_TC_TEXT_OVER_ACCENT },
		{ K_RES_TEXT_BOX_BACKGROUND_COLOR, KST_TC_TEXT_BOX_BACKGROUND_81 }, { K_RES_TEXT_BOX_BORDER_COLOR, KST_TC_TEXT_BOX_BORDER_81 },
		{ K_RES_TEXT_BOX_FOREGROUND_COLOR, KST_TC_TEXT_BOX_FOREGROUND_81 }, { K_RES_TEXT_BOX_PLACEHOLDER_COLOR, KST_TC_TEXT_BOX_PLACEHOLDER_81 },
		{ K_RES_TEXT_BOX_FOCUSED_BACKGROUND_THEME_BRUSH, KST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81 },
		{ K_RES_SLIDER_THUMB_BACKGROUND_THEME_BRUSH, KST_TC_SLIDER_THUMB },
		{ K_RES_SLIDER_THUMB_DISABLED_BACKGROUND_THEME_BRUSH, KST_TC_SLIDER_THUMB_DISABLED },
		{ K_RES_SLIDER_TRACK_BACKGROUND_THEME_BRUSH, KST_TC_SLIDER_TRACK },
		{ K_RES_SLIDER_TRACK_DECREASE_DISABLED_BACKGROUND_THEME_BRUSH, KST_TC_SLIDER_DECREASE_DISABLED },
		{ K_RES_PHONE_BASE_BLACK_COLOR, KST_TC_BASE_BLACK }, { K_RES_PHONE_BASE_WHITE_COLOR, KST_TC_BASE_WHITE },
		{ K_RES_PHONE_TEXT_MID_COLOR, KST_TC_TEXT_MID }, { K_RES_PHONE_TEXT_LOW_COLOR, KST_TC_TEXT_LOW },
		{ K_RES_CONTENT_DIALOG_DIMMING_COLOR, KST_TC_CONTENT_DIALOG_DIMMING }
	};
	Kid root = Kroot_create(100, 100);
	int theme, accent, i;
	for (theme = 0; theme < 2; theme++) {
		for (accent = 0; accent < 2; accent++) {
			kst_theme_t t;
			Kroot_set_theme(root, theme ? K_THEME_LIGHT : K_THEME_DARK);
			Kroot_set_accent(root, accent ? K_ACCENT_ORANGE : K_ACCENT_COBALT);
			kst_theme_init(&t, theme ? K_THEME_LIGHT : K_THEME_DARK, Kroot_get_accent_color(root));
			for (i = 0; i < (int)(sizeof pairs / sizeof pairs[0]); i++) {
				KColor got = Kroot_get_theme_color(root, pairs[i].key), want = t.colors[pairs[i].id];
				if (!near_color(got, want)) printf("  key 0x%X (id %d), theme %d: got %g %g %g %g, internal %g %g %g %g\n", pairs[i].key, pairs[i].id, theme,
					got.r, got.g, got.b, got.a, want.r, want.g, want.b, want.a);
				CHECK(near_color(got, want));
			}
		}
	}
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	Kset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	RUN(test_known_values);
	RUN(test_accent);
	RUN(test_override_and_clear);
	RUN(test_user_keys);
	RUN(test_invalid);
	RUN(test_bindings);
	RUN(test_bound_accent_and_border);
	RUN(test_destroy_paths);
	RUN(test_tables_agree);
	Kshutdown();
	return test_finish("test_resources");
}
