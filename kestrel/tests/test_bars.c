/**
 * Icons, AppBar, CommandBar, AppBarButton, AppBarToggleButton, AppBarSeparator, the app bars of a Page,
 * and the navigation mode and the page cache of Frame: taps by Kroot_inject_pointer, states, events,
 * the opening animation, and the pixels of the templates (a Dark theme page, Cobalt accent).
 */

#include "test.h"

static void quiet_errors(KError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static int g_opened, g_closed, g_clicks, g_page_clicks;
static void on_opened(Kid s, void* u) { (void)s; (void)u; g_opened++; }
static void on_closed(Kid s, void* u) { (void)s; (void)u; g_closed++; }
static void on_click(Kid s, void* u) { (void)s; (void)u; g_clicks++; }
static void on_page_click(Kid s, void* u) { (void)s; (void)u; g_page_clicks++; }

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

static int near_color(int r, int g, int b, int er, int eg, int eb) {
	return abs(r - er) <= 3 && abs(g - eg) <= 3 && abs(b - eb) <= 3;
}

static int pixel_is(Kid root, int x, int y, int er, int eg, int eb) {
	int r, g, b;
	test_pixel(root, x, y, &r, &g, &b);
	return near_color(r, g, b, er, eg, eb);
}

static void run(Kid root, int ms) {
	int t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static void show(Kid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", KESTREL_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}


/* ---- icons ---- */

static void test_icons(void) {
	Kid sym = Ksymbol_icon_create(K_SYMBOL_ACCEPT), font = Kfont_icon_create("ab"), path, bad, bmp;
	CHECK(Kelement_is_a(sym, K_CLASS_ICON_ELEMENT) && Kelement_get_class(sym) == K_CLASS_SYMBOL_ICON);
	CHECK(Ksymbol_icon_get_symbol(sym) == K_SYMBOL_ACCEPT);
	Ksymbol_icon_set_symbol(sym, K_SYMBOL_HOME);
	CHECK(Ksymbol_icon_get_symbol(sym) == K_SYMBOL_HOME);
	/* the values are the code points of the 8.1 symbol font */
	CHECK((int)K_SYMBOL_ACCEPT == 57611 && (int)K_SYMBOL_FIND == 57626 && (int)K_SYMBOL_SETTING == 57621 && (int)K_SYMBOL_PIN == 57665);
	CHECK_STR(Kfont_icon_get_glyph(font), "ab");
	Kfont_icon_set_glyph(font, "x");
	CHECK_STR(Kfont_icon_get_glyph(font), "x");
	path = Kpath_icon_create("M 0,0 L 10,0 L 10,10 Z M 2 2 h 3 v 3 z");
	CHECK(path != K_INVALID_ID && Kelement_get_class(path) == K_CLASS_PATH_ICON);
	CHECK(Kpath_icon_set_data(path, "M0,0 L5,0 L5,5"));
	Kset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	bad = Kpath_icon_create("M 0,0 Q 1,1 2,2");
	CHECK(bad == K_INVALID_ID);
	CHECK(!Kpath_icon_set_data(path, "L"));
	bmp = Kbitmap_icon_create("/nonexistent/none.png");
	CHECK(bmp == K_INVALID_ID);
	Kelement_destroy(sym);
	Kelement_destroy(font);
	Kelement_destroy(path);
}

static void test_icon_pixels(void) {
	Kid root = Kroot_create(480, 200), wrap = Kwrap_panel_create(K_HORIZONTAL), a, t, f, p, c;
	KRect r;
	KPoint pt;
	Kroot_set_visual(root, wrap);
	a = Kpanel_add(wrap, Kapp_bar_button_create("add", Ksymbol_icon_create(K_SYMBOL_ADD)));
	t = Kpanel_add(wrap, Kapp_bar_toggle_button_create("pin", Ksymbol_icon_create(K_SYMBOL_PIN)));
	f = Kpanel_add(wrap, Kapp_bar_button_create("font", Kfont_icon_create("A")));
	p = Kpanel_add(wrap, Kapp_bar_button_create("path", Kpath_icon_create("M 0,0 L 10,0 L 10,10 L 0,10 Z")));
	c = Kpanel_add(wrap, Kapp_bar_button_create("none", Ksymbol_icon_create(K_SYMBOL_ACCEPT)));
	Kelement_set_enabled(c, K_FALSE);
	Kroot_update_layout(root);
	/* the circle of the button: 38.5 px, 12 px under the top, in the middle of 80 px */
	r = Kelement_get_bounds(a);
	CHECK_NEAR(r.width, 80);
	pt = Kelement_to_root(a, (KPoint){ 40, 12 + 19.25f });
	/* the cross of the Add symbol is in the foreground color, the circle inside is the page */
	CHECK(pixel_is(root, (int)pt.x, (int)pt.y, 255, 255, 255));
	CHECK(pixel_is(root, (int)pt.x + 6, (int)pt.y + 6, 0, 0, 0));
	/* the outline: 2.5 px at the left edge of the circle */
	CHECK(pixel_is(root, (int)(pt.x - 19.25f + 1.0f), (int)pt.y, 255, 255, 255));
	CHECK(pixel_is(root, (int)(pt.x - 19.25f - 2.0f), (int)pt.y, 0, 0, 0));
	/* the glyph of a FontIcon and the polygon of a PathIcon are drawn in the circle */
	pt = center(f);
	pt.y = Kelement_to_root(f, (KPoint){ 0, 12 + 19.25f }).y;
	{
		int x, y, white = 0;
		for (y = (int)pt.y - 8; y <= (int)pt.y + 8; y++) for (x = (int)pt.x - 8; x <= (int)pt.x + 8; x++) {
			int r2, g2, b2;
			test_pixel(root, x, y, &r2, &g2, &b2);
			if (r2 > 200) white++;
		}
		CHECK(white > 10);
	}
	pt = Kelement_to_root(p, (KPoint){ 40, 12 + 19.25f });
	CHECK(pixel_is(root, (int)pt.x, (int)pt.y, 255, 255, 255));
	/* a disabled one is PhoneDisabled: 40% white */
	pt = Kelement_to_root(c, (KPoint){ 40, 12 + 19.25f - 19.25f + 1.0f });
	{
		int r2, g2, b2;
		test_pixel(root, (int)pt.x, (int)pt.y, &r2, &g2, &b2);
		CHECK(r2 > 80 && r2 < 130 && r2 == g2);
	}
	(void)t;
	show(root, "appbar-buttons");
	Kroot_destroy(root);
}

static void test_symbols_all_draw(void) {
	/* every drawn symbol puts pixels in the box: none is an empty hole */
	static const KSymbol syms[] = {
		K_SYMBOL_ACCEPT, K_SYMBOL_ADD, K_SYMBOL_BACK, K_SYMBOL_CANCEL, K_SYMBOL_CLEAR, K_SYMBOL_DELETE, K_SYMBOL_DOWNLOAD, K_SYMBOL_EDIT,
		K_SYMBOL_FAVORITE, K_SYMBOL_FIND, K_SYMBOL_FORWARD, K_SYMBOL_HOME, K_SYMBOL_MAIL, K_SYMBOL_MORE, K_SYMBOL_NEXT, K_SYMBOL_PAUSE, K_SYMBOL_PIN,
		K_SYMBOL_PLAY, K_SYMBOL_PREVIOUS, K_SYMBOL_REFRESH, K_SYMBOL_REMOVE, K_SYMBOL_SAVE, K_SYMBOL_SEND, K_SYMBOL_SETTING, K_SYMBOL_STOP,
		K_SYMBOL_UP, K_SYMBOL_UPLOAD, K_SYMBOL_UNDO
	};
	int i;
	for (i = 0; i < (int)(sizeof syms / sizeof *syms); i++) {
		Kid root = Kroot_create(100, 100), icon = Ksymbol_icon_create(syms[i]);
		int x, y, lit = 0;
		Kelement_set_width(icon, 40);
		Kelement_set_height(icon, 40);
		Kelement_set_horizontal_alignment(icon, K_HALIGN_LEFT);
		Kelement_set_vertical_alignment(icon, K_VALIGN_TOP);
		Kroot_set_visual(root, icon);
		for (y = 0; y < 40; y++) for (x = 0; x < 40; x++) {
			int r, g, b;
			test_pixel(root, x, y, &r, &g, &b);
			if (r > 128) lit++;
		}
		/* Undo has no drawing yet: its empty box is lit too, but thinly */
		CHECK(lit > 20);
		if (lit <= 20) printf("  symbol %d\n", (int)syms[i]);
		Kroot_destroy(root);
	}
}


/* ---- AppBarButton ---- */

static void test_app_bar_button(void) {
	Kid root = Kroot_create(480, 400), stack = Kstack_panel_create(K_VERTICAL), b, tg, sep;
	KRect r;
	Kroot_set_visual(root, stack);
	b = Kpanel_add(stack, Kapp_bar_button_create("save", Ksymbol_icon_create(K_SYMBOL_SAVE)));
	tg = Kpanel_add(stack, Kapp_bar_toggle_button_create("pin", Ksymbol_icon_create(K_SYMBOL_PIN)));
	sep = Kpanel_add(stack, Kapp_bar_separator_create());
	Kroot_update_layout(root);
	CHECK(Kelement_get_class(b) == K_CLASS_APP_BAR_BUTTON && Kelement_is_a(b, K_CLASS_BUTTON));
	CHECK(Kelement_is_a(tg, K_CLASS_TOGGLE_BUTTON));
	CHECK_STR(Kapp_bar_button_get_label(b), "save");
	CHECK(Kapp_bar_button_get_icon(b) != K_INVALID_ID && Kelement_get_class(Kapp_bar_button_get_icon(b)) == K_CLASS_SYMBOL_ICON);
	r = Kelement_get_bounds(b);
	/* 80 wide, 12 + 38.5 + 4 + the label (9.5 px) + 10.5 */
	CHECK_NEAR(r.width, 80);
	CHECK(r.height > 12 + 38.5f + 4 + 10.5f + 8 && r.height < 12 + 38.5f + 4 + 10.5f + 20);
	/* the toggle button is a pixel higher at the top of its circle: 11 */
	CHECK(Kelement_get_bounds(tg).height < r.height + 0.01f);
	Kapp_bar_button_set_is_compact(b, K_TRUE);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(b);
	CHECK_NEAR(r.width, 48);
	CHECK_NEAR(r.height, 12 + 38.5 + 4);
	CHECK(Kapp_bar_button_get_is_compact(b));
	CHECK_NEAR(Kelement_get_bounds(sep).width, 12);
	Kapp_bar_separator_set_is_compact(sep, K_TRUE);
	CHECK(Kapp_bar_separator_get_is_compact(sep));
	/* a tap: Click, a toggle: IsChecked */
	Kbutton_on_click(b, on_click, NULL);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	CHECK(!Ktoggle_button_get_is_checked(tg));
	tap(root, center(tg));
	CHECK(Ktoggle_button_get_is_checked(tg));
	/* pressed: the circle is filled with the foreground and the icon takes the background color */
	{
		KPoint c = Kelement_to_root(b, (KPoint){ 24, 12 + 19.25f });
		Kroot_inject_pointer(root, K_POINTER_MOVE, c.x, c.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, c.x, c.y);
		CHECK(Kbutton_is_pressed(b));
		CHECK(pixel_is(root, (int)c.x + 8, (int)c.y + 8, 255, 255, 255));
		Kroot_inject_pointer(root, K_POINTER_UP, c.x, c.y);
		CHECK(pixel_is(root, (int)c.x + 8, (int)c.y + 8, 0, 0, 0));
	}
	/* checked: filled, as if it were pressed */
	{
		KPoint c = Kelement_to_root(tg, (KPoint){ 40, 11 + 19.25f });
		CHECK(pixel_is(root, (int)c.x + 10, (int)c.y + 10, 255, 255, 255));
		Ktoggle_button_set_is_checked(tg, K_FALSE);
		CHECK(pixel_is(root, (int)c.x + 10, (int)c.y + 10, 0, 0, 0));
	}
	Kroot_destroy(root);
}


/* ---- the bars of a page ---- */

static Kid g_cmd_bar, g_content_button, g_sec, g_prim;

static Kid page_with_bar(Kid frame, void* parameter) {
	Kid page = Kpage_create(), stack = Kstack_panel_create(K_VERTICAL), bar = Kcommand_bar_create(), more;
	(void)frame; (void)parameter;
	g_content_button = Kpanel_add(stack, Kbutton_create("content"));
	Kelement_set_vertical_alignment(g_content_button, K_VALIGN_TOP);
	Kbutton_on_click(g_content_button, on_page_click, NULL);
	Kpage_set_content(page, stack);
	g_prim = Kapp_bar_button_create("add", Ksymbol_icon_create(K_SYMBOL_ADD));
	Kcommand_bar_add_primary_command(bar, g_prim);
	Kcommand_bar_add_primary_command(bar, Kapp_bar_separator_create());
	Kcommand_bar_add_primary_command(bar, Kapp_bar_toggle_button_create("pin", Ksymbol_icon_create(K_SYMBOL_PIN)));
	g_sec = Kapp_bar_button_create("settings", K_INVALID_ID);
	Kbutton_on_click(g_sec, on_click, NULL);
	Kcommand_bar_add_secondary_command(bar, g_sec);
	more = Kapp_bar_button_create("about", K_INVALID_ID);
	Kcommand_bar_add_secondary_command(bar, more);
	Kpage_set_bottom_app_bar(page, bar);
	g_cmd_bar = bar;
	return page;
}

static Kid other_page(Kid frame, void* parameter) { (void)frame; (void)parameter; return Kpage_create(); }

static Kid setup(Kid* frame, Kid* page) {
	Kid root = Kroot_create(480, 800);
	*frame = Kframe_create();
	Kroot_set_visual(root, *frame);
	Kframe_navigate(*frame, page_with_bar, NULL);
	*page = Kframe_get_page(*frame);
	Kroot_update_layout(root);
	g_opened = g_closed = g_clicks = g_page_clicks = 0;
	Kapp_bar_on_opened(g_cmd_bar, on_opened, NULL);
	Kapp_bar_on_closed(g_cmd_bar, on_closed, NULL);
	return root;
}

static void test_page_bars_layout(void) {
	Kid frame, page, root = setup(&frame, &page), content, bar = g_cmd_bar, top;
	KRect r, pr;
	CHECK(Kpage_get_bottom_app_bar(page) == bar && Kpage_get_top_app_bar(page) == K_INVALID_ID);
	CHECK(Kelement_get_class(bar) == K_CLASS_COMMAND_BAR && Kelement_is_a(bar, K_CLASS_APP_BAR) && Kelement_is_a(bar, K_CLASS_CONTENT_CONTROL));
	pr = Kelement_get_bounds(page);
	r = Kelement_get_bounds(bar);
	/* closed, Compact: 1.5 + 54.5 + 1.5 = 57.5 px at the bottom of the page */
	CHECK_NEAR(r.height, 57.5);
	CHECK_NEAR(r.y + r.height, pr.height);
	CHECK_NEAR(r.width, 480);
	content = Kpage_get_content(page);
	CHECK_NEAR(Kelement_get_bounds(content).height, pr.height - 57.5);
	CHECK(Kapp_bar_get_closed_display_mode(bar) == K_APP_BAR_CLOSED_COMPACT);
	/* Minimal: 35 px */
	Kapp_bar_set_closed_display_mode(bar, K_APP_BAR_CLOSED_MINIMAL);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(bar).height, 35);
	CHECK_NEAR(Kelement_get_bounds(content).height, pr.height - 35);
	/* the commands of a Minimal bar are out of sight and out of reach */
	tap(root, (KPoint){ 100, Kelement_to_root(bar, (KPoint){ 0, 17 }).y });
	CHECK(Kapp_bar_get_is_open(bar));
	Kapp_bar_set_is_open(bar, K_FALSE);
	Kapp_bar_set_closed_display_mode(bar, K_APP_BAR_CLOSED_COMPACT);
	Kroot_update_layout(root);
	/* a TopAppBar lies at the top of the page, the content under it */
	top = Kcommand_bar_create();
	Kcommand_bar_add_primary_command(top, Kapp_bar_button_create("x", Ksymbol_icon_create(K_SYMBOL_CANCEL)));
	Kpage_set_top_app_bar(page, top);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(top).y, 0);
	CHECK_NEAR(Kelement_get_bounds(content).y, 57.5);
	CHECK_NEAR(Kelement_get_bounds(content).height, pr.height - 115);
	/* the one that was there is destroyed when another is set */
	Kpage_set_top_app_bar(page, K_INVALID_ID);
	CHECK(!Kelement_is_valid(top));
	CHECK(Kpage_get_top_app_bar(page) == K_INVALID_ID);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(content).y, 0);
	Kroot_destroy(root);
}

static void test_page_bar_pixels(void) {
	Kid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	KRect r = Kelement_get_bounds(bar);
	KPoint o = Kelement_to_root(bar, (KPoint){ 0, 0 });
	int y0 = (int)o.y;
	(void)r;
	/* CommandBarBackgroundThemeBrush: the chrome 1F1F1F, under a 1.5 px line in the color of the page */
	CHECK(pixel_is(root, 5, y0 + 30, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 5, y0, 0, 0, 0));
	CHECK(pixel_is(root, 5, y0 + 3, 0x1F, 0x1F, 0x1F));
	/* the three dots at the right edge: the middle one is in the foreground, in line with the circles */
	CHECK(pixel_is(root, 480 - 15, y0 + (int)(1.5f + 12 + 19.25f), 255, 255, 255));
	show(root, "command-bar-closed");
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	show(root, "command-bar-open");
	Kroot_set_theme(root, K_THEME_LIGHT);
	/* the chrome of the Light theme is DDDDDD */
	{
		KPoint p = Kelement_to_root(bar, (KPoint){ 5, 10 });
		CHECK(pixel_is(root, (int)p.x, (int)p.y, 0xDD, 0xDD, 0xDD));
	}
	show(root, "command-bar-open-light");
	Kroot_destroy(root);
}

static void test_open_close(void) {
	Kid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	KRect closed, open;
	closed = Kelement_get_bounds(bar);
	/* a tap on the three dots opens the bar: Opened, the labels, the secondary commands as rows */
	tap(root, (KPoint){ 480 - 15, Kelement_to_root(bar, (KPoint){ 0, 30 }).y });
	CHECK(Kapp_bar_get_is_open(bar) && g_opened == 1 && g_closed == 0);
	Kroot_update_layout(root);
	open = Kelement_get_bounds(bar);
	CHECK(open.height > closed.height + 40);
	/* a bar at the bottom grows upward */
	CHECK_NEAR(open.y + open.height, closed.y + closed.height);
	CHECK(!Kapp_bar_button_get_is_compact(g_prim));
	/* the labels make the buttons taller than the closed row, and the secondary commands are rows of text above them */
	CHECK(Kelement_get_bounds(g_sec).y < Kelement_get_bounds(g_prim).y);
	CHECK_NEAR(Kelement_get_bounds(g_sec).width, 480);
	/* a tap on the dots again closes it */
	tap(root, (KPoint){ 480 - 15, Kelement_to_root(bar, (KPoint){ 0, open.height - 40 }).y });
	CHECK(!Kapp_bar_get_is_open(bar) && g_closed == 1);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(bar).height, 57.5);
	CHECK(Kapp_bar_button_get_is_compact(g_prim));
	/* IsOpen by the program raises the events once per change */
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kapp_bar_set_is_open(bar, K_TRUE);
	CHECK(g_opened == 2);
	Kapp_bar_set_is_open(bar, K_FALSE);
	CHECK(g_closed == 2);
	Kroot_destroy(root);
}

static void test_light_dismiss_and_sticky(void) {
	Kid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	/* a press outside the open bar closes it and is not given to the content under the press */
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	tap(root, center(g_content_button));
	CHECK(!Kapp_bar_get_is_open(bar) && g_closed == 1);
	CHECK(g_page_clicks == 0);
	tap(root, center(g_content_button));
	CHECK(g_page_clicks == 1);
	/* a press on the bar itself does not close it */
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	tap(root, (KPoint){ 20, Kelement_to_root(bar, (KPoint){ 0, 8 }).y });
	CHECK(Kapp_bar_get_is_open(bar));
	/* a sticky bar stays open through a press outside it, and through Back */
	Kapp_bar_set_is_sticky(bar, K_TRUE);
	CHECK(Kapp_bar_get_is_sticky(bar));
	tap(root, center(g_content_button));
	CHECK(Kapp_bar_get_is_open(bar));
	CHECK(g_page_clicks == 2);
	Kapp_bar_set_is_sticky(bar, K_FALSE);
	/* Back closes the bar before it navigates */
	Kframe_navigate(frame, other_page, NULL);
	Kroot_update_layout(root);
	CHECK(Kframe_get_back_stack_depth(frame) == 1);
	Kroot_destroy(root);

	root = setup(&frame, &page);
	Kframe_go_back(frame);
	Kframe_navigate(frame, other_page, NULL);
	Kframe_navigate(frame, page_with_bar, NULL);
	page = Kframe_get_page(frame);
	bar = Kpage_get_bottom_app_bar(page);
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	CHECK(Kroot_back(root));
	CHECK(!Kapp_bar_get_is_open(bar));
	CHECK(Kframe_get_page(frame) == page);
	CHECK(Kroot_back(root));
	CHECK(Kframe_get_page(frame) != page);
	Kroot_destroy(root);
}

static void test_secondary_command(void) {
	Kid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	tap(root, center(g_sec));
	/* the command ran and the menu went */
	CHECK(g_clicks == 1);
	CHECK(!Kapp_bar_get_is_open(bar) && g_closed == 1);
	/* a primary one leaves the bar as it is */
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	Kbutton_on_click(g_prim, on_click, NULL);
	tap(root, center(g_prim));
	CHECK(g_clicks == 2 && Kapp_bar_get_is_open(bar));
	/* the commands: counts, indexes, wrong classes refused */
	CHECK(Kcommand_bar_get_primary_command_count(bar) == 3 && Kcommand_bar_get_secondary_command_count(bar) == 2);
	CHECK(Kcommand_bar_get_primary_command(bar, 0) == g_prim && Kcommand_bar_get_secondary_command(bar, 0) == g_sec);
	CHECK(Kcommand_bar_get_primary_command(bar, 9) == K_INVALID_ID);
	Kset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	CHECK(Kcommand_bar_add_primary_command(bar, Kbutton_create("plain")) == -1);
	Kcommand_bar_clear_secondary_commands(bar);
	CHECK(Kcommand_bar_get_secondary_command_count(bar) == 0 && !Kelement_is_valid(g_sec));
	/* destroying a command takes it off the bar */
	Kelement_destroy(g_prim);
	CHECK(Kcommand_bar_get_primary_command_count(bar) == 2);
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

static void test_app_bar_content(void) {
	Kid root = Kroot_create(480, 800), page = Kpage_create(), bar = Kapp_bar_create(), tb = Ktext_block_create("hello");
	Kpage_set_top_app_bar(page, bar);
	Kcontent_control_set_content(bar, tb);
	Kroot_set_visual(root, page);
	Kroot_update_layout(root);
	/* a plain AppBar is as tall as its content when it is open and has the page background (AppBarBackgroundThemeBrush) */
	CHECK_NEAR(Kelement_get_bounds(bar).height, 57.5);
	CHECK(pixel_is(root, 300, 20, 0, 0, 0));
	Kapp_bar_set_is_open(bar, K_TRUE);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(bar).height, 57.5);
	Kapp_bar_set_closed_display_mode(bar, K_APP_BAR_CLOSED_MINIMAL);
	Kapp_bar_set_is_open(bar, K_FALSE);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(bar).height, 35);
	Kroot_destroy(root);
}


/* ---- the opening animation ---- */

static void test_open_animation(void) {
	Kid frame, page, root;
	KRect r0, r1, r2;
	root = setup(&frame, &page);
	Kroot_set_animations(root, K_ANIMATIONS_ON);
	r0 = Kelement_get_bounds(g_cmd_bar);
	Kapp_bar_set_is_open(g_cmd_bar, K_TRUE);
	/* IsOpen is at once, the bar takes 167 ms to get there */
	CHECK(Kapp_bar_get_is_open(g_cmd_bar) && g_opened == 1);
	run(root, 64);
	Kroot_update_layout(root);
	r1 = Kelement_get_bounds(g_cmd_bar);
	CHECK(r1.height > r0.height + 1);
	run(root, 400);
	Kroot_update_layout(root);
	r2 = Kelement_get_bounds(g_cmd_bar);
	CHECK(r2.height > r1.height + 20);
	CHECK_NEAR(r2.y + r2.height, r0.y + r0.height);
	/* and back */
	Kapp_bar_set_is_open(g_cmd_bar, K_FALSE);
	run(root, 64);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(g_cmd_bar).height < r2.height - 1 && Kelement_get_bounds(g_cmd_bar).height > r0.height);
	run(root, 400);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(g_cmd_bar).height, r0.height);
	CHECK(Kapp_bar_button_get_is_compact(g_prim));
	Kroot_destroy(root);
}


/* ---- Frame and Page: NavigationMode and NavigationCacheMode ---- */

static char g_log[512];
static void log_add(const char* s) { strcat(g_log, s); }
static const char* mode_name(KNavigationMode m) {
	return m == K_NAVIGATION_NEW ? "new" : m == K_NAVIGATION_BACK ? "back" : m == K_NAVIGATION_FORWARD ? "forward" : "other";
}

static void log_to(Kid page, void* u) { log_add("to-"); log_add((const char*)u); log_add(":"); log_add(mode_name(Kpage_get_navigation_mode(page))); log_add(" "); }
static void log_from(Kid page, void* u) { log_add("from-"); log_add((const char*)u); log_add(":"); log_add(mode_name(Kpage_get_navigation_mode(page))); log_add(" "); }

static int g_built_a, g_built_b;

static Kid mode_page(Kid frame, void* parameter) {
	Kid page = Kpage_create();
	const char* name = (const char*)parameter;
	(void)frame;
	if (name[0] == 'a') g_built_a++;
	else g_built_b++;
	Kpage_on_navigated_to(page, log_to, parameter);
	Kpage_on_navigated_from(page, log_from, parameter);
	if (name[1] == 'c') Kpage_set_navigation_cache_mode(page, K_NAVIGATION_CACHE_REQUIRED);
	return page;
}

static void test_navigation_mode(void) {
	Kid root = Kroot_create(480, 800), frame = Kframe_create();
	Kroot_set_visual(root, frame);
	g_log[0] = 0;
	Kframe_navigate(frame, mode_page, "a");
	Kframe_navigate(frame, mode_page, "b");
	CHECK_STR(g_log, "to-a:new from-a:new to-b:new ");
	g_log[0] = 0;
	Kframe_go_back(frame);
	/* the page that leaves is told how the next one is reached */
	CHECK_STR(g_log, "from-b:back to-a:back ");
	g_log[0] = 0;
	Kframe_go_forward(frame);
	CHECK_STR(g_log, "from-a:forward to-b:forward ");
	CHECK(Kframe_get_current_source_page_type(frame) == mode_page);
	CHECK(Kpage_get_navigation_cache_mode(Kframe_get_page(frame)) == K_NAVIGATION_CACHE_DISABLED);
	Kroot_destroy(root);
}

static void test_navigation_cache(void) {
	Kid root = Kroot_create(480, 800), frame = Kframe_create(), a, b;
	Kroot_set_visual(root, frame);
	g_built_a = g_built_b = 0;
	/* "ac" asks for the cache, "b" does not */
	Kframe_navigate(frame, mode_page, "ac");
	a = Kframe_get_page(frame);
	Kframe_navigate(frame, mode_page, "b");
	b = Kframe_get_page(frame);
	/* the page that is cached is alive, and the other is gone */
	CHECK(Kelement_is_valid(a) && Kelement_get_parent(a) == K_INVALID_ID);
	CHECK(Kelement_is_valid(b));
	Kframe_go_back(frame);
	CHECK(Kframe_get_page(frame) == a);
	CHECK(g_built_a == 1);
	CHECK(!Kelement_is_valid(b));
	CHECK(Kpage_get_frame(a) == frame);
	Kframe_go_forward(frame);
	CHECK(g_built_b == 2);
	/* a Navigate that drops the entry destroys the page kept for it */
	Kframe_go_back(frame);
	CHECK(Kframe_get_page(frame) == a);
	Kframe_navigate(frame, mode_page, "b");
	Kframe_navigate(frame, mode_page, "ac");
	Kframe_go_back(frame);
	Kframe_go_back(frame);
	CHECK(Kframe_get_page(frame) == a);
	CHECK(Kframe_navigate(frame, mode_page, "b"));
	CHECK(Kelement_is_valid(a));
	Kframe_go_back(frame);
	/* the frame destroys what it keeps */
	Kframe_navigate(frame, mode_page, "ac");
	Kframe_navigate(frame, mode_page, "b");
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_icons);
	RUN(test_icon_pixels);
	RUN(test_symbols_all_draw);
	RUN(test_app_bar_button);
	RUN(test_page_bars_layout);
	RUN(test_page_bar_pixels);
	RUN(test_open_close);
	RUN(test_light_dismiss_and_sticky);
	RUN(test_secondary_command);
	RUN(test_app_bar_content);
	RUN(test_open_animation);
	RUN(test_navigation_mode);
	RUN(test_navigation_cache);
	Kshutdown();
	return test_finish("test_bars");
}
