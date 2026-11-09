/**
 * Flyout, MenuFlyout and its items, Button.Flyout, and ContentDialog: taps by Zroot_inject_pointer, what
 * opens and closes, the events and the callback, the modal dim and the input it blocks, and the pixels of the
 * templates (a Dark theme page, Cobalt accent). Popups are the host of all of them.
 */

#include "test.h"

static char g_log[512];
static void log_add(const char* s) { strcat(g_log, s); }

static void quiet_errors(ZError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

static void pixel(Zid root, int x, int y, int* r, int* g, int* b) { test_pixel(root, x, y, r, g, b); }

static int pixel_is(Zid root, int x, int y, int er, int eg, int eb) {
	int r, g, b;
	pixel(root, x, y, &r, &g, &b);
	return abs(r - er) <= 3 && abs(g - eg) <= 3 && abs(b - eb) <= 3;
}

static void show(Zid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", ZRNUI_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}

static void run(Zid root, int ms) {
	int t;
	for (t = 0; t < ms; t += 16) Zroot_tick(root, 16);
}

static void on_opening(Zid s, void* u) { (void)s; (void)u; log_add("opening "); }
static void on_opened(Zid s, void* u) { (void)s; (void)u; log_add("opened "); }
static void on_closed(Zid s, void* u) { (void)s; (void)u; log_add("closed "); }
static void on_click(Zid s, void* u) { (void)s; log_add((const char*)u); log_add(" "); }

/* A page: a button at the top left and a red band across the middle, which a dim makes darker. */
static Zid g_button, g_band, g_stack;

static Zid page_root(Zid* root) {
	Zid r = Zroot_create(480, 800);
	g_stack = Zstack_panel_create(Z_VERTICAL);
	g_button = Zpanel_add(g_stack, Zbutton_create("open"));
	Zelement_set_vertical_alignment(g_button, Z_VALIGN_TOP);
	g_band = Zpanel_add(g_stack, Zborder_create());
	Zborder_set_background(g_band, Zcolor_rgba(1, 0, 0, 1));
	Zelement_set_height(g_band, 100);
	Zelement_set_vertical_alignment(g_band, Z_VALIGN_TOP);
	Zelement_set_margin(g_band, Zthickness(0, 100, 0, 0));
	Zroot_set_visual(r, g_stack);
	Zroot_update_layout(r);
	g_log[0] = 0;
	*root = r;
	return g_stack;
}


/* ---- Flyout ---- */

static void test_flyout_show_and_hide(void) {
	Zid root, flyout, content;
	ZRect fr;
	page_root(&root);
	flyout = Zflyout_create();
	content = Ztext_block_create("flyout content");
	Zflyout_set_content(flyout, content);
	CHECK(Zelement_get_class(flyout) == Z_CLASS_FLYOUT && Zelement_is_a(flyout, Z_CLASS_FLYOUT_BASE));
	CHECK(Zflyout_get_content(flyout) == content);
	CHECK(Zflyout_base_get_placement(flyout) == Z_FLYOUT_PLACEMENT_TOP);
	Zflyout_base_on_opening(flyout, on_opening, NULL);
	Zflyout_base_on_opened(flyout, on_opened, NULL);
	Zflyout_base_on_closed(flyout, on_closed, NULL);
	/* shown at an element it is Opening, then Opened */
	Zflyout_base_set_placement(flyout, Z_FLYOUT_PLACEMENT_BOTTOM);
	Zflyout_base_show_at(flyout, g_button);
	CHECK(Zflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "opening opened ");
	Zroot_update_layout(root);
	/* as wide as the screen, under the button: the content 25.5 px below the top of the flyout, which is at least 43 px tall */
	fr = Zelement_get_bounds(content);
	{
		ZPoint o = Zelement_to_root(content, (ZPoint){ 0, 0 });
		ZRect b = Zelement_get_bounds(g_button);
		ZPoint bo = Zelement_to_root(g_button, (ZPoint){ 0, b.height });
		CHECK_NEAR(o.y, bo.y + 25.5);
		CHECK_NEAR(o.x, 0);
		CHECK(fr.height > 0);
		/* FlyoutBackgroundThemeBrush is the chrome 1F1F1F, over the whole width */
		CHECK(pixel_is(root, 400, (int)bo.y + 10, 0x1F, 0x1F, 0x1F));
		CHECK(pixel_is(root, 5, (int)bo.y + 10, 0x1F, 0x1F, 0x1F));
	}
	show(root, "flyout");
	/* a tap outside closes it, and the tap does not reach what is under it */
	{
		ZPoint away = { 400, 700 };
		g_log[0] = 0;
		tap(root, away);
		CHECK(!Zflyout_base_get_is_open(flyout));
		CHECK_STR(g_log, "closed ");
		Zroot_update_layout(root);
		CHECK(pixel_is(root, 400, 120, 0, 0, 0));
	}
	/* Hide closes it too, once */
	g_log[0] = 0;
	Zflyout_base_show_at(flyout, g_button);
	Zflyout_base_hide(flyout);
	Zflyout_base_hide(flyout);
	CHECK_STR(g_log, "opening opened closed ");
	CHECK(!Zflyout_base_get_is_open(flyout));
	/* and the program still owns it: its content goes with it */
	Zelement_destroy(flyout);
	CHECK(!Zelement_is_valid(content));
	Zroot_destroy(root);
}

static void test_flyout_placement(void) {
	Zid root, flyout = Zflyout_create(), content = Ztext_block_create("x");
	ZPoint o;
	ZRect b;
	page_root(&root);
	Zflyout_set_content(flyout, content);
	b = Zelement_get_bounds(g_button);
	/* Top, the default, wants the room above the button: there is none at the top of the screen, so it lies under it */
	Zflyout_base_show_at(flyout, g_button);
	Zroot_update_layout(root);
	o = Zelement_to_root(content, (ZPoint){ 0, 0 });
	CHECK(o.y > b.y + b.height);
	Zflyout_base_hide(flyout);
	/* the red band is below the middle: a flyout at Top is above it */
	Zflyout_base_set_placement(flyout, Z_FLYOUT_PLACEMENT_TOP);
	Zflyout_base_show_at(flyout, g_band);
	Zroot_update_layout(root);
	o = Zelement_to_root(content, (ZPoint){ 0, 0 });
	{
		ZPoint bo = Zelement_to_root(g_band, (ZPoint){ 0, 0 });
		CHECK(o.y < bo.y);
	}
	Zflyout_base_hide(flyout);
	/* Full fills the screen */
	Zflyout_base_set_placement(flyout, Z_FLYOUT_PLACEMENT_FULL);
	Zflyout_base_show_at(flyout, g_button);
	Zroot_update_layout(root);
	CHECK(pixel_is(root, 240, 790, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 240, 2, 0x1F, 0x1F, 0x1F));
	Zflyout_base_hide(flyout);
	Zset_error_callback(quiet_errors, NULL);
	Zflyout_base_set_placement(flyout, (ZFlyoutPlacementMode)7);
	CHECK(Zflyout_base_get_placement(flyout) == Z_FLYOUT_PLACEMENT_FULL);
	/* the target must be in a tree of a root */
	{
		Zid loose = Zbutton_create("loose");
		Zflyout_base_show_at(flyout, loose);
		CHECK(!Zflyout_base_get_is_open(flyout));
	}
	Zelement_destroy(flyout);
	Zroot_destroy(root);
}

static void test_flyout_target_tap_and_attach(void) {
	Zid root, flyout = Zflyout_create(), content = Ztext_block_create("attached");
	page_root(&root);
	Zflyout_set_content(flyout, content);
	Zbutton_set_flyout(g_button, flyout);
	CHECK(Zbutton_get_flyout(g_button) == flyout);
	Zflyout_base_on_closed(flyout, on_closed, NULL);
	Zbutton_on_click(g_button, on_click, "click");
	/* a tap on the button runs Click and shows the flyout */
	tap(root, center(g_button));
	CHECK(Zflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "click ");
	/* a tap on the button again closes it, and does not open it again */
	g_log[0] = 0;
	tap(root, center(g_button));
	CHECK(!Zflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "closed ");
	tap(root, center(g_button));
	CHECK(Zflyout_base_get_is_open(flyout));
	/* Back closes a flyout before anything else */
	CHECK(Zroot_back(root));
	CHECK(!Zflyout_base_get_is_open(flyout));
	/* the button owns it: destroying the button while it shows takes both away */
	tap(root, center(g_button));
	CHECK(Zflyout_base_get_is_open(flyout));
	Zelement_destroy(g_button);
	CHECK(!Zelement_is_valid(flyout) && !Zelement_is_valid(content));
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

static void test_flyout_set_flyout_errors(void) {
	Zid root, f1 = Zflyout_create(), f2 = Zflyout_create(), b2;
	page_root(&root);
	Zbutton_set_flyout(g_button, f1);
	b2 = Zpanel_add(g_stack, Zbutton_create("two"));
	Zset_error_callback(quiet_errors, NULL);
	Zbutton_set_flyout(b2, f1);
	CHECK(Zbutton_get_flyout(b2) == Z_INVALID_ID);
	/* replacing it destroys the old one; Z_INVALID_ID takes it away */
	Zbutton_set_flyout(g_button, f2);
	CHECK(!Zelement_is_valid(f1) && Zbutton_get_flyout(g_button) == f2);
	Zbutton_set_flyout(g_button, Z_INVALID_ID);
	CHECK(!Zelement_is_valid(f2) && Zbutton_get_flyout(g_button) == Z_INVALID_ID);
	Zroot_destroy(root);
}

/* The root goes while a flyout shows at it: the flyout is the program's, and is still there. */
static void test_flyout_survives_root(void) {
	Zid root, flyout = Zflyout_create();
	page_root(&root);
	Zflyout_set_content(flyout, Ztext_block_create("x"));
	Zflyout_base_show_at(flyout, g_button);
	Zroot_update_layout(root);
	Zroot_destroy(root);
	CHECK(Zelement_is_valid(flyout));
	Zelement_destroy(flyout);
	CHECK(Zinit());
	Zshutdown();
}


/* ---- MenuFlyout ---- */

static void test_menu_flyout(void) {
	Zid root, menu = Zmenu_flyout_create(), a, b, t, sep, off;
	ZPoint pa;
	ZRect ra;
	page_root(&root);
	CHECK(Zelement_get_class(menu) == Z_CLASS_MENU_FLYOUT && Zelement_is_a(menu, Z_CLASS_FLYOUT_BASE));
	a = Zmenu_flyout_item_create("first");
	b = Zmenu_flyout_item_create("second");
	t = Ztoggle_menu_flyout_item_create("toggle", Z_FALSE);
	sep = Zmenu_flyout_separator_create();
	off = Zmenu_flyout_item_create("disabled");
	CHECK(Zmenu_flyout_add_item(menu, a) == 0);
	CHECK(Zmenu_flyout_add_item(menu, b) == 1);
	CHECK(Zmenu_flyout_add_item(menu, sep) == 2);
	CHECK(Zmenu_flyout_add_item(menu, t) == 3);
	CHECK(Zmenu_flyout_add_item(menu, off) == 4);
	Zelement_set_enabled(off, Z_FALSE);
	CHECK(Zmenu_flyout_get_item_count(menu) == 5 && Zmenu_flyout_get_item(menu, 1) == b);
	CHECK(Zelement_is_a(t, Z_CLASS_MENU_FLYOUT_ITEM) && Zelement_is_a(a, Z_CLASS_MENU_FLYOUT_ITEM_BASE));
	CHECK_STR(Zmenu_flyout_item_get_text(a), "first");
	Zmenu_flyout_item_on_click(a, on_click, "a");
	Zmenu_flyout_item_on_click(b, on_click, "b");
	Zmenu_flyout_item_on_click(t, on_click, "t");
	Zmenu_flyout_item_on_click(off, on_click, "off");
	Zflyout_base_on_closed(menu, on_closed, NULL);
	Zflyout_base_set_placement(menu, Z_FLYOUT_PLACEMENT_BOTTOM);
	Zflyout_base_show_at(menu, g_button);
	Zroot_update_layout(root);
	ra = Zelement_get_bounds(a);
	pa = Zelement_to_root(a, (ZPoint){ 0, 0 });
	/* MenuFlyoutPresenter: items at 19 px from the left, 25.5 px type with 8.5 px above and below */
	{
		ZRect rs = Zelement_get_bounds(sep);
		CHECK_NEAR(pa.x, 19);
		CHECK(ra.height > 25.5f + 17 && ra.height < 25.5f * 1.6f + 17);
		CHECK_NEAR(rs.height, 17);
		CHECK_NEAR(Zelement_get_bounds(b).y, ra.y + ra.height);
	}
	/* the page background with a 1.5 px line in the foreground above (the menu is under the button) */
	{
		ZRect br = Zelement_get_bounds(g_button);
		ZPoint under = Zelement_to_root(g_button, (ZPoint){ 0, br.height });
		CHECK(pixel_is(root, 300, (int)under.y + 1, 255, 255, 255));
		CHECK(pixel_is(root, 300, (int)under.y + 10, 0, 0, 0));
	}
	show(root, "menu-flyout");
	/* a tap on an item: Click and the menu closes */
	g_log[0] = 0;
	tap(root, center(b));
	CHECK_STR(g_log, "b closed ");
	CHECK(!Zflyout_base_get_is_open(menu));
	/* a toggle item flips before its Click */
	Zflyout_base_show_at(menu, g_button);
	Zroot_update_layout(root);
	CHECK(!Ztoggle_menu_flyout_item_get_is_checked(t));
	g_log[0] = 0;
	tap(root, center(t));
	CHECK(Ztoggle_menu_flyout_item_get_is_checked(t));
	CHECK_STR(g_log, "t closed ");
	Zflyout_base_show_at(menu, g_button);
	Zroot_update_layout(root);
	show(root, "menu-flyout-checked");
	/* a disabled item does nothing and the menu stays */
	g_log[0] = 0;
	tap(root, center(off));
	CHECK_STR(g_log, "");
	CHECK(Zflyout_base_get_is_open(menu));
	/* a tap on the separator does nothing either */
	tap(root, center(sep));
	CHECK(Zflyout_base_get_is_open(menu));
	/* released outside the item: no Click */
	{
		ZPoint c = center(a);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x, c.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, c.x, c.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x, c.y + 300);
		Zroot_inject_pointer(root, Z_POINTER_UP, c.x, c.y + 300);
		CHECK_STR(g_log, "");
	}
	Zflyout_base_hide(menu);
	Ztoggle_menu_flyout_item_set_is_checked(t, Z_FALSE);
	Zmenu_flyout_item_set_text(a, "renamed");
	CHECK_STR(Zmenu_flyout_item_get_text(a), "renamed");
	Zmenu_flyout_clear_items(menu);
	CHECK(Zmenu_flyout_get_item_count(menu) == 0 && !Zelement_is_valid(a));
	Zelement_destroy(menu);
	Zroot_destroy(root);
}

static void test_menu_flyout_light(void) {
	Zid root, menu = Zmenu_flyout_create(), a = Zmenu_flyout_item_create("first");
	page_root(&root);
	Zmenu_flyout_add_item(menu, a);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zflyout_base_set_placement(menu, Z_FLYOUT_PLACEMENT_BOTTOM);
	Zflyout_base_show_at(menu, g_button);
	Zroot_update_layout(root);
	/* the page background of the Light theme, white, and the foreground line FF000000 */
	{
		ZRect br = Zelement_get_bounds(g_button);
		ZPoint under = Zelement_to_root(g_button, (ZPoint){ 0, br.height });
		CHECK(pixel_is(root, 300, (int)under.y + 10, 255, 255, 255));
		CHECK(pixel_is(root, 300, (int)under.y + 1, 0x00, 0x00, 0x00));
	}
	show(root, "menu-flyout-light");
	Zelement_destroy(menu);
	Zroot_destroy(root);
}


/* ---- ContentDialog ---- */

static ZContentDialogResult g_result;
static int g_results;
static void on_result(Zid dialog, ZContentDialogResult result, void* u) { (void)dialog; (void)u; g_result = result; g_results++; log_add("result "); }

static Zid make_dialog(void) {
	Zid d = Zcontent_dialog_create();
	Zcontent_dialog_set_title(d, "Delete?");
	Zcontent_control_set_text(d, "The file will be gone.");
	Zcontent_dialog_set_primary_button_text(d, "delete");
	Zcontent_dialog_set_secondary_button_text(d, "cancel");
	Zcontent_dialog_on_opened(d, on_opened, NULL);
	Zcontent_dialog_on_closed(d, on_closed, NULL);
	Zcontent_dialog_on_primary_button_click(d, on_click, "primary");
	Zcontent_dialog_on_secondary_button_click(d, on_click, "secondary");
	g_results = 0;
	g_result = Z_CONTENT_DIALOG_RESULT_NONE;
	return d;
}

static void test_dialog_look(void) {
	Zid root, d;
	int r, g, b;
	page_root(&root);
	d = make_dialog();
	CHECK(Zelement_get_class(d) == Z_CLASS_CONTENT_DIALOG && Zelement_is_a(d, Z_CLASS_CONTENT_CONTROL));
	CHECK_STR(Zcontent_dialog_get_title(d), "Delete?");
	CHECK_STR(Zcontent_dialog_get_primary_button_text(d), "delete");
	CHECK(Zcontent_dialog_get_is_primary_button_enabled(d) && Zcontent_dialog_get_is_secondary_button_enabled(d));
	CHECK(Zcontent_dialog_show(d, root, on_result, NULL));
	CHECK_STR(g_log, "opened ");
	Zroot_update_layout(root);
	/* ContentDialogBackgroundThemeBrush is the chrome, at the top, as wide as the screen */
	CHECK(pixel_is(root, 5, 5, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 475, 5, 0x1F, 0x1F, 0x1F));
	/* ContentDialogDimmingThemeBrush #99000000 over the page: the red band at 40% */
	pixel(root, 240, 200, &r, &g, &b);
	CHECK(r > 90 && r < 115 && g == 0 && b == 0);
	show(root, "content-dialog");
	/* the page under a Light theme is dimmed with white 60% */
	Zroot_set_theme(root, Z_THEME_LIGHT);
	pixel(root, 240, 500, &r, &g, &b);
	CHECK(r > 235 && g > 235 && b > 235);
	pixel(root, 240, 200, &r, &g, &b);
	CHECK(r > 240 && g > 150 && g < 170 && b > 150 && b < 170);
	show(root, "content-dialog-light");
	Zroot_set_theme(root, Z_THEME_DARK);
	Zcontent_dialog_hide(d);
	Zelement_destroy(d);
	Zroot_destroy(root);
}

static void test_dialog_results(void) {
	Zid root, d;
	page_root(&root);
	d = make_dialog();
	Zroot_update_layout(root);
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zroot_update_layout(root);
	/* the buttons side by side: the primary one at the left half, the secondary one at the right */
	{
		Zid p = Zroot_get_focus(root);
		ZPoint pp;
		CHECK(p != Z_INVALID_ID && !strcmp(Zcontent_control_get_text(p), "delete"));
		pp = center(p);
		CHECK(pp.x < 240);
		/* a second show of a dialog that is open is refused */
		Zset_error_callback(quiet_errors, NULL);
		CHECK(!Zcontent_dialog_show(d, root, NULL, NULL));
		g_log[0] = 0;
		tap(root, pp);
		/* the button's own Click first, Closed, then the callback that is the result */
		CHECK_STR(g_log, "primary closed result ");
		CHECK(g_result == Z_CONTENT_DIALOG_RESULT_PRIMARY && g_results == 1);
	}
	/* it goes: the page is as it was, and the dialog is not in a tree */
	Zroot_update_layout(root);
	CHECK(Zelement_get_parent(d) == Z_INVALID_ID);
	{
		int r, g, b;
		pixel(root, 240, 200, &r, &g, &b);
		CHECK(r > 250 && g == 0 && b == 0);
	}
	/* shown again, the secondary button */
	CHECK(Zcontent_dialog_show(d, root, on_result, NULL));
	Zroot_update_layout(root);
	g_log[0] = 0;
	{
		/* Tab goes to the other button of the dialog, not to the page behind it */
		Zroot_inject_key(root, Z_KEY_TAB, 0, 1);
		CHECK(!strcmp(Zcontent_control_get_text(Zroot_get_focus(root)), "cancel"));
		Zroot_inject_key(root, Z_KEY_TAB, 0, 1);
		CHECK(!strcmp(Zcontent_control_get_text(Zroot_get_focus(root)), "delete"));
		Zroot_inject_key(root, Z_KEY_TAB, 0, 1);
		Zroot_inject_key(root, Z_KEY_ENTER, 0, 1);
		Zroot_inject_key(root, Z_KEY_ENTER, 0, 0);
	}
	CHECK_STR(g_log, "secondary closed result ");
	CHECK(g_result == Z_CONTENT_DIALOG_RESULT_SECONDARY);
	/* Hide: None */
	Zcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	Zcontent_dialog_hide(d);
	CHECK_STR(g_log, "closed result ");
	CHECK(g_result == Z_CONTENT_DIALOG_RESULT_NONE);
	/* Back: None, and the page behind does not navigate */
	Zcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	CHECK(Zroot_back(root));
	CHECK_STR(g_log, "closed result ");
	CHECK(g_result == Z_CONTENT_DIALOG_RESULT_NONE);
	/* Escape does the same, through the keyboard */
	Zcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	CHECK(Zroot_inject_key(root, Z_KEY_ESCAPE, 0, 1));
	CHECK_STR(g_log, "closed result ");
	/* a disabled button does not answer */
	Zcontent_dialog_set_is_primary_button_enabled(d, Z_FALSE);
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zroot_update_layout(root);
	g_log[0] = 0;
	{
		Zid f = Zroot_get_focus(root);
		CHECK(!strcmp(Zcontent_control_get_text(f), "cancel"));
	}
	Zcontent_dialog_hide(d);
	CHECK(!Zcontent_dialog_get_is_primary_button_enabled(d));
	/* the callback may be NULL */
	Zcontent_dialog_set_is_primary_button_enabled(d, Z_TRUE);
	CHECK(Zcontent_dialog_show(d, root, NULL, NULL));
	Zcontent_dialog_hide(d);
	Zelement_destroy(d);
	Zroot_destroy(root);
}

static void test_dialog_blocks_input(void) {
	Zid root, d;
	page_root(&root);
	Zbutton_on_click(g_button, on_click, "page");
	d = make_dialog();
	/* focus on the page button before: it comes back after */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(g_button).x, center(g_button).y);
	Zroot_inject_pointer(root, Z_POINTER_UP, center(g_button).x, center(g_button).y);
	CHECK(Zroot_get_focus(root) == g_button);
	g_log[0] = 0;
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zroot_update_layout(root);
	CHECK(Zroot_get_focus(root) != g_button);
	/* a tap where the page's button is: the dialog covers the top, so the button is under the dialog's body */
	g_log[0] = 0;
	tap(root, (ZPoint){ 240, 700 });
	tap(root, center(g_band));
	/* nothing under the dialog is hit, and the dialog stays: it is modal and does not light dismiss */
	CHECK_STR(g_log, "");
	/* a button of the page is not a tab stop while the dialog shows */
	Zroot_move_focus(root, 1);
	CHECK(Zroot_get_focus(root) != g_button);
	Zcontent_dialog_hide(d);
	CHECK(Zroot_get_focus(root) == g_button);
	g_log[0] = 0;
	tap(root, center(g_button));
	CHECK_STR(g_log, "page ");
	Zelement_destroy(d);
	Zroot_destroy(root);
}

static void test_dialog_destroy_while_open(void) {
	Zid root, d;
	page_root(&root);
	d = make_dialog();
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zroot_update_layout(root);
	Zelement_destroy(d);
	Zroot_update_layout(root);
	/* the page works again */
	{
		int r, g, b;
		pixel(root, 240, 200, &r, &g, &b);
		CHECK(r > 250);
	}
	/* a dialog still open when the root goes */
	d = make_dialog();
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zroot_destroy(root);
	Zshutdown();
	CHECK(Zinit());
}

static void test_dialog_content_and_layout(void) {
	Zid root, d, c;
	ZRect r;
	page_root(&root);
	d = Zcontent_dialog_create();
	c = Zborder_create();
	Zelement_set_height(c, 100);
	Zcontent_control_set_content(d, c);
	Zcontent_dialog_set_primary_button_text(d, "ok");
	Zcontent_dialog_show(d, root, NULL, NULL);
	Zroot_update_layout(root);
	/* margins: the content 16.5 under the title row (a title row with no title is 33.5 high), 19 from the sides */
	r = Zelement_get_bounds(c);
	CHECK_NEAR(r.x, 19);
	CHECK_NEAR(r.width, 480 - 38);
	CHECK_NEAR(r.y, 33.5 + 16.5);
	/* the dialog: title row + content row + buttons row (24 + 57.5 + 9.5) */
	{
		ZRect dr = Zelement_get_bounds(d);
		CHECK_NEAR(dr.width, 480);
		CHECK_NEAR(dr.height, 33.5 + 16.5 + 100 + 91);
	}
	/* one button: the primary at the left half only */
	{
		Zid f = Zroot_get_focus(root);
		ZRect fr = Zelement_get_bounds(f);
		CHECK_NEAR(fr.x, 19);
		CHECK_NEAR(fr.width, 240 - 19 - 9.5);
	}
	Zcontent_dialog_hide(d);
	Zelement_destroy(d);
	Zroot_destroy(root);
}

/* ---- the animation clock reaches what is hosted by the root ---- */

static void test_ticks_and_tweens_in_popups(void) {
	Zid root, flyout = Zflyout_create(), content = Zborder_create(), d;
	page_root(&root);
	Zroot_set_animations(root, Z_ANIMATIONS_ON);
	Zelement_set_height(content, 50);
	Zflyout_set_content(flyout, content);
	Zflyout_base_show_at(flyout, g_button);
	Zroot_update_layout(root);
	/* a tween on an element of a flyout that is hosted by the root runs with the clock */
	Zelement_animate(content, Z_PROP_OPACITY, 0.0f, 100.0f, Z_EASE_LINEAR, 0);
	run(root, 64);
	{
		float o = Zelement_get_opacity(content);
		CHECK(o < 1.0f && o > 0.0f);
	}
	run(root, 200);
	CHECK(Zelement_get_opacity(content) < 0.01f);
	Zflyout_base_hide(flyout);
	d = make_dialog();
	Zcontent_dialog_show(d, root, on_result, NULL);
	Zelement_animate(Zcontent_control_get_content(d), Z_PROP_OPACITY, 0.5f, 50.0f, Z_EASE_LINEAR, 0);
	run(root, 200);
	CHECK_NEAR(Zelement_get_opacity(Zcontent_control_get_content(d)) * 100, 50);
	Zcontent_dialog_hide(d);
	Zelement_destroy(d);
	Zelement_destroy(flyout);
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
	Zset_error_callback(quiet_errors, NULL);
	RUN(test_flyout_show_and_hide);
	RUN(test_flyout_placement);
	RUN(test_flyout_target_tap_and_attach);
	RUN(test_flyout_set_flyout_errors);
	RUN(test_flyout_survives_root);
	RUN(test_menu_flyout);
	RUN(test_menu_flyout_light);
	RUN(test_dialog_look);
	RUN(test_dialog_results);
	RUN(test_dialog_blocks_input);
	RUN(test_dialog_destroy_while_open);
	RUN(test_dialog_content_and_layout);
	RUN(test_ticks_and_tweens_in_popups);
	Zshutdown();
	return test_finish("test_flyouts");
}
