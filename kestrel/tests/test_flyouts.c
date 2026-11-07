/**
 * Flyout, MenuFlyout and its items, Button.Flyout, and ContentDialog: taps by Kroot_inject_pointer, what
 * opens and closes, the events and the callback, the modal dim and the input it blocks, and the pixels of the
 * templates (a Dark theme page, Cobalt accent). Popups are the host of all of them.
 */

#include "test.h"

static char g_log[512];
static void log_add(const char* s) { strcat(g_log, s); }

static void quiet_errors(KError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

static void pixel(Kid root, int x, int y, int* r, int* g, int* b) { test_pixel(root, x, y, r, g, b); }

static int pixel_is(Kid root, int x, int y, int er, int eg, int eb) {
	int r, g, b;
	pixel(root, x, y, &r, &g, &b);
	return abs(r - er) <= 3 && abs(g - eg) <= 3 && abs(b - eb) <= 3;
}

static void show(Kid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", KESTREL_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}

static void run(Kid root, int ms) {
	int t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static void on_opening(Kid s, void* u) { (void)s; (void)u; log_add("opening "); }
static void on_opened(Kid s, void* u) { (void)s; (void)u; log_add("opened "); }
static void on_closed(Kid s, void* u) { (void)s; (void)u; log_add("closed "); }
static void on_click(Kid s, void* u) { (void)s; log_add((const char*)u); log_add(" "); }

/* A page: a button at the top left and a red band across the middle, which a dim makes darker. */
static Kid g_button, g_band, g_stack;

static Kid page_root(Kid* root) {
	Kid r = Kroot_create(480, 800);
	g_stack = Kstack_panel_create(K_VERTICAL);
	g_button = Kpanel_add(g_stack, Kbutton_create("open"));
	Kelement_set_vertical_alignment(g_button, K_VALIGN_TOP);
	g_band = Kpanel_add(g_stack, Kborder_create());
	Kborder_set_background(g_band, Kcolor_rgba(1, 0, 0, 1));
	Kelement_set_height(g_band, 100);
	Kelement_set_vertical_alignment(g_band, K_VALIGN_TOP);
	Kelement_set_margin(g_band, Kthickness(0, 100, 0, 0));
	Kroot_set_visual(r, g_stack);
	Kroot_update_layout(r);
	g_log[0] = 0;
	*root = r;
	return g_stack;
}


/* ---- Flyout ---- */

static void test_flyout_show_and_hide(void) {
	Kid root, flyout, content;
	KRect fr;
	page_root(&root);
	flyout = Kflyout_create();
	content = Ktext_block_create("flyout content");
	Kflyout_set_content(flyout, content);
	CHECK(Kelement_get_class(flyout) == K_CLASS_FLYOUT && Kelement_is_a(flyout, K_CLASS_FLYOUT_BASE));
	CHECK(Kflyout_get_content(flyout) == content);
	CHECK(Kflyout_base_get_placement(flyout) == K_FLYOUT_PLACEMENT_TOP);
	Kflyout_base_on_opening(flyout, on_opening, NULL);
	Kflyout_base_on_opened(flyout, on_opened, NULL);
	Kflyout_base_on_closed(flyout, on_closed, NULL);
	/* shown at an element it is Opening, then Opened */
	Kflyout_base_set_placement(flyout, K_FLYOUT_PLACEMENT_BOTTOM);
	Kflyout_base_show_at(flyout, g_button);
	CHECK(Kflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "opening opened ");
	Kroot_update_layout(root);
	/* as wide as the screen, under the button: the content 25.5 px below the top of the flyout, which is at least 43 px tall */
	fr = Kelement_get_bounds(content);
	{
		KPoint o = Kelement_to_root(content, (KPoint){ 0, 0 });
		KRect b = Kelement_get_bounds(g_button);
		KPoint bo = Kelement_to_root(g_button, (KPoint){ 0, b.height });
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
		KPoint away = { 400, 700 };
		g_log[0] = 0;
		tap(root, away);
		CHECK(!Kflyout_base_get_is_open(flyout));
		CHECK_STR(g_log, "closed ");
		Kroot_update_layout(root);
		CHECK(pixel_is(root, 400, 120, 0, 0, 0));
	}
	/* Hide closes it too, once */
	g_log[0] = 0;
	Kflyout_base_show_at(flyout, g_button);
	Kflyout_base_hide(flyout);
	Kflyout_base_hide(flyout);
	CHECK_STR(g_log, "opening opened closed ");
	CHECK(!Kflyout_base_get_is_open(flyout));
	/* and the program still owns it: its content goes with it */
	Kelement_destroy(flyout);
	CHECK(!Kelement_is_valid(content));
	Kroot_destroy(root);
}

static void test_flyout_placement(void) {
	Kid root, flyout = Kflyout_create(), content = Ktext_block_create("x");
	KPoint o;
	KRect b;
	page_root(&root);
	Kflyout_set_content(flyout, content);
	b = Kelement_get_bounds(g_button);
	/* Top, the default, wants the room above the button: there is none at the top of the screen, so it lies under it */
	Kflyout_base_show_at(flyout, g_button);
	Kroot_update_layout(root);
	o = Kelement_to_root(content, (KPoint){ 0, 0 });
	CHECK(o.y > b.y + b.height);
	Kflyout_base_hide(flyout);
	/* the red band is below the middle: a flyout at Top is above it */
	Kflyout_base_set_placement(flyout, K_FLYOUT_PLACEMENT_TOP);
	Kflyout_base_show_at(flyout, g_band);
	Kroot_update_layout(root);
	o = Kelement_to_root(content, (KPoint){ 0, 0 });
	{
		KPoint bo = Kelement_to_root(g_band, (KPoint){ 0, 0 });
		CHECK(o.y < bo.y);
	}
	Kflyout_base_hide(flyout);
	/* Full fills the screen */
	Kflyout_base_set_placement(flyout, K_FLYOUT_PLACEMENT_FULL);
	Kflyout_base_show_at(flyout, g_button);
	Kroot_update_layout(root);
	CHECK(pixel_is(root, 240, 790, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 240, 2, 0x1F, 0x1F, 0x1F));
	Kflyout_base_hide(flyout);
	Kset_error_callback(quiet_errors, NULL);
	Kflyout_base_set_placement(flyout, (KFlyoutPlacementMode)7);
	CHECK(Kflyout_base_get_placement(flyout) == K_FLYOUT_PLACEMENT_FULL);
	/* the target must be in a tree of a root */
	{
		Kid loose = Kbutton_create("loose");
		Kflyout_base_show_at(flyout, loose);
		CHECK(!Kflyout_base_get_is_open(flyout));
	}
	Kelement_destroy(flyout);
	Kroot_destroy(root);
}

static void test_flyout_target_tap_and_attach(void) {
	Kid root, flyout = Kflyout_create(), content = Ktext_block_create("attached");
	page_root(&root);
	Kflyout_set_content(flyout, content);
	Kbutton_set_flyout(g_button, flyout);
	CHECK(Kbutton_get_flyout(g_button) == flyout);
	Kflyout_base_on_closed(flyout, on_closed, NULL);
	Kbutton_on_click(g_button, on_click, "click");
	/* a tap on the button runs Click and shows the flyout */
	tap(root, center(g_button));
	CHECK(Kflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "click ");
	/* a tap on the button again closes it, and does not open it again */
	g_log[0] = 0;
	tap(root, center(g_button));
	CHECK(!Kflyout_base_get_is_open(flyout));
	CHECK_STR(g_log, "closed ");
	tap(root, center(g_button));
	CHECK(Kflyout_base_get_is_open(flyout));
	/* Back closes a flyout before anything else */
	CHECK(Kroot_back(root));
	CHECK(!Kflyout_base_get_is_open(flyout));
	/* the button owns it: destroying the button while it shows takes both away */
	tap(root, center(g_button));
	CHECK(Kflyout_base_get_is_open(flyout));
	Kelement_destroy(g_button);
	CHECK(!Kelement_is_valid(flyout) && !Kelement_is_valid(content));
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

static void test_flyout_set_flyout_errors(void) {
	Kid root, f1 = Kflyout_create(), f2 = Kflyout_create(), b2;
	page_root(&root);
	Kbutton_set_flyout(g_button, f1);
	b2 = Kpanel_add(g_stack, Kbutton_create("two"));
	Kset_error_callback(quiet_errors, NULL);
	Kbutton_set_flyout(b2, f1);
	CHECK(Kbutton_get_flyout(b2) == K_INVALID_ID);
	/* replacing it destroys the old one; K_INVALID_ID takes it away */
	Kbutton_set_flyout(g_button, f2);
	CHECK(!Kelement_is_valid(f1) && Kbutton_get_flyout(g_button) == f2);
	Kbutton_set_flyout(g_button, K_INVALID_ID);
	CHECK(!Kelement_is_valid(f2) && Kbutton_get_flyout(g_button) == K_INVALID_ID);
	Kroot_destroy(root);
}

/* The root goes while a flyout shows at it: the flyout is the program's, and is still there. */
static void test_flyout_survives_root(void) {
	Kid root, flyout = Kflyout_create();
	page_root(&root);
	Kflyout_set_content(flyout, Ktext_block_create("x"));
	Kflyout_base_show_at(flyout, g_button);
	Kroot_update_layout(root);
	Kroot_destroy(root);
	CHECK(Kelement_is_valid(flyout));
	Kelement_destroy(flyout);
	CHECK(Kinit());
	Kshutdown();
}


/* ---- MenuFlyout ---- */

static void test_menu_flyout(void) {
	Kid root, menu = Kmenu_flyout_create(), a, b, t, sep, off;
	KPoint pa;
	KRect ra;
	page_root(&root);
	CHECK(Kelement_get_class(menu) == K_CLASS_MENU_FLYOUT && Kelement_is_a(menu, K_CLASS_FLYOUT_BASE));
	a = Kmenu_flyout_item_create("first");
	b = Kmenu_flyout_item_create("second");
	t = Ktoggle_menu_flyout_item_create("toggle", K_FALSE);
	sep = Kmenu_flyout_separator_create();
	off = Kmenu_flyout_item_create("disabled");
	CHECK(Kmenu_flyout_add_item(menu, a) == 0);
	CHECK(Kmenu_flyout_add_item(menu, b) == 1);
	CHECK(Kmenu_flyout_add_item(menu, sep) == 2);
	CHECK(Kmenu_flyout_add_item(menu, t) == 3);
	CHECK(Kmenu_flyout_add_item(menu, off) == 4);
	Kelement_set_enabled(off, K_FALSE);
	CHECK(Kmenu_flyout_get_item_count(menu) == 5 && Kmenu_flyout_get_item(menu, 1) == b);
	CHECK(Kelement_is_a(t, K_CLASS_MENU_FLYOUT_ITEM) && Kelement_is_a(a, K_CLASS_MENU_FLYOUT_ITEM_BASE));
	CHECK_STR(Kmenu_flyout_item_get_text(a), "first");
	Kmenu_flyout_item_on_click(a, on_click, "a");
	Kmenu_flyout_item_on_click(b, on_click, "b");
	Kmenu_flyout_item_on_click(t, on_click, "t");
	Kmenu_flyout_item_on_click(off, on_click, "off");
	Kflyout_base_on_closed(menu, on_closed, NULL);
	Kflyout_base_set_placement(menu, K_FLYOUT_PLACEMENT_BOTTOM);
	Kflyout_base_show_at(menu, g_button);
	Kroot_update_layout(root);
	ra = Kelement_get_bounds(a);
	pa = Kelement_to_root(a, (KPoint){ 0, 0 });
	/* MenuFlyoutPresenter: items at 19 px from the left, 25.5 px type with 8.5 px above and below */
	{
		KRect rs = Kelement_get_bounds(sep);
		CHECK_NEAR(pa.x, 19);
		CHECK(ra.height > 25.5f + 17 && ra.height < 25.5f * 1.6f + 17);
		CHECK_NEAR(rs.height, 17);
		CHECK_NEAR(Kelement_get_bounds(b).y, ra.y + ra.height);
	}
	/* the page background with a 1.5 px line in the foreground above (the menu is under the button) */
	{
		KRect br = Kelement_get_bounds(g_button);
		KPoint under = Kelement_to_root(g_button, (KPoint){ 0, br.height });
		CHECK(pixel_is(root, 300, (int)under.y + 1, 255, 255, 255));
		CHECK(pixel_is(root, 300, (int)under.y + 10, 0, 0, 0));
	}
	show(root, "menu-flyout");
	/* a tap on an item: Click and the menu closes */
	g_log[0] = 0;
	tap(root, center(b));
	CHECK_STR(g_log, "b closed ");
	CHECK(!Kflyout_base_get_is_open(menu));
	/* a toggle item flips before its Click */
	Kflyout_base_show_at(menu, g_button);
	Kroot_update_layout(root);
	CHECK(!Ktoggle_menu_flyout_item_get_is_checked(t));
	g_log[0] = 0;
	tap(root, center(t));
	CHECK(Ktoggle_menu_flyout_item_get_is_checked(t));
	CHECK_STR(g_log, "t closed ");
	Kflyout_base_show_at(menu, g_button);
	Kroot_update_layout(root);
	show(root, "menu-flyout-checked");
	/* a disabled item does nothing and the menu stays */
	g_log[0] = 0;
	tap(root, center(off));
	CHECK_STR(g_log, "");
	CHECK(Kflyout_base_get_is_open(menu));
	/* a tap on the separator does nothing either */
	tap(root, center(sep));
	CHECK(Kflyout_base_get_is_open(menu));
	/* released outside the item: no Click */
	{
		KPoint c = center(a);
		Kroot_inject_pointer(root, K_POINTER_MOVE, c.x, c.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, c.x, c.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, c.x, c.y + 300);
		Kroot_inject_pointer(root, K_POINTER_UP, c.x, c.y + 300);
		CHECK_STR(g_log, "");
	}
	Kflyout_base_hide(menu);
	Ktoggle_menu_flyout_item_set_is_checked(t, K_FALSE);
	Kmenu_flyout_item_set_text(a, "renamed");
	CHECK_STR(Kmenu_flyout_item_get_text(a), "renamed");
	Kmenu_flyout_clear_items(menu);
	CHECK(Kmenu_flyout_get_item_count(menu) == 0 && !Kelement_is_valid(a));
	Kelement_destroy(menu);
	Kroot_destroy(root);
}

static void test_menu_flyout_light(void) {
	Kid root, menu = Kmenu_flyout_create(), a = Kmenu_flyout_item_create("first");
	page_root(&root);
	Kmenu_flyout_add_item(menu, a);
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kflyout_base_set_placement(menu, K_FLYOUT_PLACEMENT_BOTTOM);
	Kflyout_base_show_at(menu, g_button);
	Kroot_update_layout(root);
	/* the page background of the Light theme, white, and the foreground line FF000000 */
	{
		KRect br = Kelement_get_bounds(g_button);
		KPoint under = Kelement_to_root(g_button, (KPoint){ 0, br.height });
		CHECK(pixel_is(root, 300, (int)under.y + 10, 255, 255, 255));
		CHECK(pixel_is(root, 300, (int)under.y + 1, 0x00, 0x00, 0x00));
	}
	show(root, "menu-flyout-light");
	Kelement_destroy(menu);
	Kroot_destroy(root);
}


/* ---- ContentDialog ---- */

static KContentDialogResult g_result;
static int g_results;
static void on_result(Kid dialog, KContentDialogResult result, void* u) { (void)dialog; (void)u; g_result = result; g_results++; log_add("result "); }

static Kid make_dialog(void) {
	Kid d = Kcontent_dialog_create();
	Kcontent_dialog_set_title(d, "Delete?");
	Kcontent_control_set_text(d, "The file will be gone.");
	Kcontent_dialog_set_primary_button_text(d, "delete");
	Kcontent_dialog_set_secondary_button_text(d, "cancel");
	Kcontent_dialog_on_opened(d, on_opened, NULL);
	Kcontent_dialog_on_closed(d, on_closed, NULL);
	Kcontent_dialog_on_primary_button_click(d, on_click, "primary");
	Kcontent_dialog_on_secondary_button_click(d, on_click, "secondary");
	g_results = 0;
	g_result = K_CONTENT_DIALOG_RESULT_NONE;
	return d;
}

static void test_dialog_look(void) {
	Kid root, d;
	int r, g, b;
	page_root(&root);
	d = make_dialog();
	CHECK(Kelement_get_class(d) == K_CLASS_CONTENT_DIALOG && Kelement_is_a(d, K_CLASS_CONTENT_CONTROL));
	CHECK_STR(Kcontent_dialog_get_title(d), "Delete?");
	CHECK_STR(Kcontent_dialog_get_primary_button_text(d), "delete");
	CHECK(Kcontent_dialog_get_is_primary_button_enabled(d) && Kcontent_dialog_get_is_secondary_button_enabled(d));
	CHECK(Kcontent_dialog_show(d, root, on_result, NULL));
	CHECK_STR(g_log, "opened ");
	Kroot_update_layout(root);
	/* ContentDialogBackgroundThemeBrush is the chrome, at the top, as wide as the screen */
	CHECK(pixel_is(root, 5, 5, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 475, 5, 0x1F, 0x1F, 0x1F));
	/* ContentDialogDimmingThemeBrush #99000000 over the page: the red band at 40% */
	pixel(root, 240, 200, &r, &g, &b);
	CHECK(r > 90 && r < 115 && g == 0 && b == 0);
	show(root, "content-dialog");
	/* the page under a Light theme is dimmed with white 60% */
	Kroot_set_theme(root, K_THEME_LIGHT);
	pixel(root, 240, 500, &r, &g, &b);
	CHECK(r > 235 && g > 235 && b > 235);
	pixel(root, 240, 200, &r, &g, &b);
	CHECK(r > 240 && g > 150 && g < 170 && b > 150 && b < 170);
	show(root, "content-dialog-light");
	Kroot_set_theme(root, K_THEME_DARK);
	Kcontent_dialog_hide(d);
	Kelement_destroy(d);
	Kroot_destroy(root);
}

static void test_dialog_results(void) {
	Kid root, d;
	page_root(&root);
	d = make_dialog();
	Kroot_update_layout(root);
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kroot_update_layout(root);
	/* the buttons side by side: the primary one at the left half, the secondary one at the right */
	{
		Kid p = Kroot_get_focus(root);
		KPoint pp;
		CHECK(p != K_INVALID_ID && !strcmp(Kcontent_control_get_text(p), "delete"));
		pp = center(p);
		CHECK(pp.x < 240);
		/* a second show of a dialog that is open is refused */
		Kset_error_callback(quiet_errors, NULL);
		CHECK(!Kcontent_dialog_show(d, root, NULL, NULL));
		g_log[0] = 0;
		tap(root, pp);
		/* the button's own Click first, Closed, then the callback that is the result */
		CHECK_STR(g_log, "primary closed result ");
		CHECK(g_result == K_CONTENT_DIALOG_RESULT_PRIMARY && g_results == 1);
	}
	/* it goes: the page is as it was, and the dialog is not in a tree */
	Kroot_update_layout(root);
	CHECK(Kelement_get_parent(d) == K_INVALID_ID);
	{
		int r, g, b;
		pixel(root, 240, 200, &r, &g, &b);
		CHECK(r > 250 && g == 0 && b == 0);
	}
	/* shown again, the secondary button */
	CHECK(Kcontent_dialog_show(d, root, on_result, NULL));
	Kroot_update_layout(root);
	g_log[0] = 0;
	{
		/* Tab goes to the other button of the dialog, not to the page behind it */
		Kroot_inject_key(root, K_KEY_TAB, 0, 1);
		CHECK(!strcmp(Kcontent_control_get_text(Kroot_get_focus(root)), "cancel"));
		Kroot_inject_key(root, K_KEY_TAB, 0, 1);
		CHECK(!strcmp(Kcontent_control_get_text(Kroot_get_focus(root)), "delete"));
		Kroot_inject_key(root, K_KEY_TAB, 0, 1);
		Kroot_inject_key(root, K_KEY_ENTER, 0, 1);
		Kroot_inject_key(root, K_KEY_ENTER, 0, 0);
	}
	CHECK_STR(g_log, "secondary closed result ");
	CHECK(g_result == K_CONTENT_DIALOG_RESULT_SECONDARY);
	/* Hide: None */
	Kcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	Kcontent_dialog_hide(d);
	CHECK_STR(g_log, "closed result ");
	CHECK(g_result == K_CONTENT_DIALOG_RESULT_NONE);
	/* Back: None, and the page behind does not navigate */
	Kcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	CHECK(Kroot_back(root));
	CHECK_STR(g_log, "closed result ");
	CHECK(g_result == K_CONTENT_DIALOG_RESULT_NONE);
	/* Escape does the same, through the keyboard */
	Kcontent_dialog_show(d, root, on_result, NULL);
	g_log[0] = 0;
	CHECK(Kroot_inject_key(root, K_KEY_ESCAPE, 0, 1));
	CHECK_STR(g_log, "closed result ");
	/* a disabled button does not answer */
	Kcontent_dialog_set_is_primary_button_enabled(d, K_FALSE);
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kroot_update_layout(root);
	g_log[0] = 0;
	{
		Kid f = Kroot_get_focus(root);
		CHECK(!strcmp(Kcontent_control_get_text(f), "cancel"));
	}
	Kcontent_dialog_hide(d);
	CHECK(!Kcontent_dialog_get_is_primary_button_enabled(d));
	/* the callback may be NULL */
	Kcontent_dialog_set_is_primary_button_enabled(d, K_TRUE);
	CHECK(Kcontent_dialog_show(d, root, NULL, NULL));
	Kcontent_dialog_hide(d);
	Kelement_destroy(d);
	Kroot_destroy(root);
}

static void test_dialog_blocks_input(void) {
	Kid root, d;
	page_root(&root);
	Kbutton_on_click(g_button, on_click, "page");
	d = make_dialog();
	/* focus on the page button before: it comes back after */
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(g_button).x, center(g_button).y);
	Kroot_inject_pointer(root, K_POINTER_UP, center(g_button).x, center(g_button).y);
	CHECK(Kroot_get_focus(root) == g_button);
	g_log[0] = 0;
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kroot_update_layout(root);
	CHECK(Kroot_get_focus(root) != g_button);
	/* a tap where the page's button is: the dialog covers the top, so the button is under the dialog's body */
	g_log[0] = 0;
	tap(root, (KPoint){ 240, 700 });
	tap(root, center(g_band));
	/* nothing under the dialog is hit, and the dialog stays: it is modal and does not light dismiss */
	CHECK_STR(g_log, "");
	/* a button of the page is not a tab stop while the dialog shows */
	Kroot_move_focus(root, 1);
	CHECK(Kroot_get_focus(root) != g_button);
	Kcontent_dialog_hide(d);
	CHECK(Kroot_get_focus(root) == g_button);
	g_log[0] = 0;
	tap(root, center(g_button));
	CHECK_STR(g_log, "page ");
	Kelement_destroy(d);
	Kroot_destroy(root);
}

static void test_dialog_destroy_while_open(void) {
	Kid root, d;
	page_root(&root);
	d = make_dialog();
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kroot_update_layout(root);
	Kelement_destroy(d);
	Kroot_update_layout(root);
	/* the page works again */
	{
		int r, g, b;
		pixel(root, 240, 200, &r, &g, &b);
		CHECK(r > 250);
	}
	/* a dialog still open when the root goes */
	d = make_dialog();
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kroot_destroy(root);
	Kshutdown();
	CHECK(Kinit());
}

static void test_dialog_content_and_layout(void) {
	Kid root, d, c;
	KRect r;
	page_root(&root);
	d = Kcontent_dialog_create();
	c = Kborder_create();
	Kelement_set_height(c, 100);
	Kcontent_control_set_content(d, c);
	Kcontent_dialog_set_primary_button_text(d, "ok");
	Kcontent_dialog_show(d, root, NULL, NULL);
	Kroot_update_layout(root);
	/* margins: the content 16.5 under the title row (a title row with no title is 33.5 high), 19 from the sides */
	r = Kelement_get_bounds(c);
	CHECK_NEAR(r.x, 19);
	CHECK_NEAR(r.width, 480 - 38);
	CHECK_NEAR(r.y, 33.5 + 16.5);
	/* the dialog: title row + content row + buttons row (24 + 57.5 + 9.5) */
	{
		KRect dr = Kelement_get_bounds(d);
		CHECK_NEAR(dr.width, 480);
		CHECK_NEAR(dr.height, 33.5 + 16.5 + 100 + 91);
	}
	/* one button: the primary at the left half only */
	{
		Kid f = Kroot_get_focus(root);
		KRect fr = Kelement_get_bounds(f);
		CHECK_NEAR(fr.x, 19);
		CHECK_NEAR(fr.width, 240 - 19 - 9.5);
	}
	Kcontent_dialog_hide(d);
	Kelement_destroy(d);
	Kroot_destroy(root);
}

/* ---- the animation clock reaches what is hosted by the root ---- */

static void test_ticks_and_tweens_in_popups(void) {
	Kid root, flyout = Kflyout_create(), content = Kborder_create(), d;
	page_root(&root);
	Kroot_set_animations(root, K_ANIMATIONS_ON);
	Kelement_set_height(content, 50);
	Kflyout_set_content(flyout, content);
	Kflyout_base_show_at(flyout, g_button);
	Kroot_update_layout(root);
	/* a tween on an element of a flyout that is hosted by the root runs with the clock */
	Kelement_animate(content, K_PROP_OPACITY, 0.0f, 100.0f, K_EASE_LINEAR, 0);
	run(root, 64);
	{
		float o = Kelement_get_opacity(content);
		CHECK(o < 1.0f && o > 0.0f);
	}
	run(root, 200);
	CHECK(Kelement_get_opacity(content) < 0.01f);
	Kflyout_base_hide(flyout);
	d = make_dialog();
	Kcontent_dialog_show(d, root, on_result, NULL);
	Kelement_animate(Kcontent_control_get_content(d), K_PROP_OPACITY, 0.5f, 50.0f, K_EASE_LINEAR, 0);
	run(root, 200);
	CHECK_NEAR(Kelement_get_opacity(Kcontent_control_get_content(d)) * 100, 50);
	Kcontent_dialog_hide(d);
	Kelement_destroy(d);
	Kelement_destroy(flyout);
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	Kset_error_callback(quiet_errors, NULL);
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
	Kshutdown();
	return test_finish("test_flyouts");
}
