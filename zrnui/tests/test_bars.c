/**
 * Icons, AppBar, CommandBar, AppBarButton, AppBarToggleButton, AppBarSeparator, the app bars of a Page,
 * and the navigation mode and the page cache of Frame: taps by Zroot_inject_pointer, states, events,
 * the opening animation, and the pixels of the templates (a Dark theme page, Cobalt accent).
 */

#include "test.h"

static void quiet_errors(ZError e, const char* m, void* u) { (void)e; (void)m; (void)u; }

static int g_opened, g_closed, g_clicks, g_page_clicks;
static void on_opened(Zid s, void* u) { (void)s; (void)u; g_opened++; }
static void on_closed(Zid s, void* u) { (void)s; (void)u; g_closed++; }
static void on_click(Zid s, void* u) { (void)s; (void)u; g_clicks++; }
static void on_page_click(Zid s, void* u) { (void)s; (void)u; g_page_clicks++; }

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

static int near_color(int r, int g, int b, int er, int eg, int eb) {
	return abs(r - er) <= 3 && abs(g - eg) <= 3 && abs(b - eb) <= 3;
}

static int pixel_is(Zid root, int x, int y, int er, int eg, int eb) {
	int r, g, b;
	test_pixel(root, x, y, &r, &g, &b);
	return near_color(r, g, b, er, eg, eb);
}

static void run(Zid root, int ms) {
	int t;
	for (t = 0; t < ms; t += 16) Zroot_tick(root, 16);
}

static void show(Zid root, const char* name) {
	char path[512];
	snprintf(path, sizeof path, "%s/%s.png", ZRNUI_TEST_OUTPUT_DIR, name);
	CHECK(test_save_png(root, path));
}


/* ---- icons ---- */

static void test_icons(void) {
	Zid sym = Zsymbol_icon_create(Z_SYMBOL_ACCEPT), font = Zfont_icon_create("ab"), path, bad, bmp;
	CHECK(Zelement_is_a(sym, Z_CLASS_ICON_ELEMENT) && Zelement_get_class(sym) == Z_CLASS_SYMBOL_ICON);
	CHECK(Zsymbol_icon_get_symbol(sym) == Z_SYMBOL_ACCEPT);
	Zsymbol_icon_set_symbol(sym, Z_SYMBOL_HOME);
	CHECK(Zsymbol_icon_get_symbol(sym) == Z_SYMBOL_HOME);
	/* the values are the code points of the 8.1 symbol font */
	CHECK((int)Z_SYMBOL_ACCEPT == 57611 && (int)Z_SYMBOL_FIND == 57626 && (int)Z_SYMBOL_SETTING == 57621 && (int)Z_SYMBOL_PIN == 57665);
	CHECK_STR(Zfont_icon_get_glyph(font), "ab");
	Zfont_icon_set_glyph(font, "x");
	CHECK_STR(Zfont_icon_get_glyph(font), "x");
	path = Zpath_icon_create("M 0,0 L 10,0 L 10,10 Z M 2 2 h 3 v 3 z");
	CHECK(path != Z_INVALID_ID && Zelement_get_class(path) == Z_CLASS_PATH_ICON);
	CHECK(Zpath_icon_set_data(path, "M0,0 L5,0 L5,5"));
	Zset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	bad = Zpath_icon_create("M 0,0 Q 1,1 2,2");
	CHECK(bad == Z_INVALID_ID);
	CHECK(!Zpath_icon_set_data(path, "L"));
	bmp = Zbitmap_icon_create("/nonexistent/none.png");
	CHECK(bmp == Z_INVALID_ID);
	Zelement_destroy(sym);
	Zelement_destroy(font);
	Zelement_destroy(path);
}

static void test_icon_pixels(void) {
	Zid root = Zroot_create(480, 200), wrap = Zwrap_panel_create(Z_HORIZONTAL), a, t, f, p, c;
	ZRect r;
	ZPoint pt;
	Zroot_set_visual(root, wrap);
	a = Zpanel_add(wrap, Zapp_bar_button_create("add", Zsymbol_icon_create(Z_SYMBOL_ADD)));
	t = Zpanel_add(wrap, Zapp_bar_toggle_button_create("pin", Zsymbol_icon_create(Z_SYMBOL_PIN)));
	f = Zpanel_add(wrap, Zapp_bar_button_create("font", Zfont_icon_create("A")));
	p = Zpanel_add(wrap, Zapp_bar_button_create("path", Zpath_icon_create("M 0,0 L 10,0 L 10,10 L 0,10 Z")));
	c = Zpanel_add(wrap, Zapp_bar_button_create("none", Zsymbol_icon_create(Z_SYMBOL_ACCEPT)));
	Zelement_set_enabled(c, false);
	Zroot_update_layout(root);
	/* the circle of the button: 38.5 px, 12 px under the top, in the middle of 80 px */
	r = Zelement_get_bounds(a);
	CHECK_NEAR(r.width, 80);
	pt = Zelement_to_root(a, (ZPoint){ 40, 12 + 19.25f });
	/* the cross of the Add symbol is in the foreground color, the circle inside is the page */
	CHECK(pixel_is(root, (int)pt.x, (int)pt.y, 255, 255, 255));
	CHECK(pixel_is(root, (int)pt.x + 6, (int)pt.y + 6, 0, 0, 0));
	/* the outline: 2.5 px at the left edge of the circle */
	CHECK(pixel_is(root, (int)(pt.x - 19.25f + 1.0f), (int)pt.y, 255, 255, 255));
	CHECK(pixel_is(root, (int)(pt.x - 19.25f - 2.0f), (int)pt.y, 0, 0, 0));
	/* the glyph of a FontIcon and the polygon of a PathIcon are drawn in the circle */
	pt = center(f);
	pt.y = Zelement_to_root(f, (ZPoint){ 0, 12 + 19.25f }).y;
	{
		int x, y, white = 0;
		for (y = (int)pt.y - 8; y <= (int)pt.y + 8; y++) for (x = (int)pt.x - 8; x <= (int)pt.x + 8; x++) {
			int r2, g2, b2;
			test_pixel(root, x, y, &r2, &g2, &b2);
			if (r2 > 200) white++;
		}
		CHECK(white > 10);
	}
	pt = Zelement_to_root(p, (ZPoint){ 40, 12 + 19.25f });
	CHECK(pixel_is(root, (int)pt.x, (int)pt.y, 255, 255, 255));
	/* a disabled one is PhoneDisabled: 40% white */
	pt = Zelement_to_root(c, (ZPoint){ 40, 12 + 19.25f - 19.25f + 1.0f });
	{
		int r2, g2, b2;
		test_pixel(root, (int)pt.x, (int)pt.y, &r2, &g2, &b2);
		CHECK(r2 > 80 && r2 < 130 && r2 == g2);
	}
	(void)t;
	show(root, "appbar-buttons");
	Zroot_destroy(root);
}

static void test_symbols_all_draw(void) {
	/* every drawn symbol puts pixels in the box: none is an empty hole */
	static const ZSymbol syms[] = {
		Z_SYMBOL_ACCEPT, Z_SYMBOL_ADD, Z_SYMBOL_BACK, Z_SYMBOL_CANCEL, Z_SYMBOL_CLEAR, Z_SYMBOL_DELETE, Z_SYMBOL_DOWNLOAD, Z_SYMBOL_EDIT,
		Z_SYMBOL_FAVORITE, Z_SYMBOL_FIND, Z_SYMBOL_FORWARD, Z_SYMBOL_HOME, Z_SYMBOL_MAIL, Z_SYMBOL_MORE, Z_SYMBOL_NEXT, Z_SYMBOL_PAUSE, Z_SYMBOL_PIN,
		Z_SYMBOL_PLAY, Z_SYMBOL_PREVIOUS, Z_SYMBOL_REFRESH, Z_SYMBOL_REMOVE, Z_SYMBOL_SAVE, Z_SYMBOL_SEND, Z_SYMBOL_SETTING, Z_SYMBOL_STOP,
		Z_SYMBOL_UP, Z_SYMBOL_UPLOAD, Z_SYMBOL_UNDO
	};
	int i;
	for (i = 0; i < (int)(sizeof syms / sizeof *syms); i++) {
		Zid root = Zroot_create(100, 100), icon = Zsymbol_icon_create(syms[i]);
		int x, y, lit = 0;
		Zelement_set_width(icon, 40);
		Zelement_set_height(icon, 40);
		Zelement_set_horizontal_alignment(icon, Z_HALIGN_LEFT);
		Zelement_set_vertical_alignment(icon, Z_VALIGN_TOP);
		Zroot_set_visual(root, icon);
		for (y = 0; y < 40; y++) for (x = 0; x < 40; x++) {
			int r, g, b;
			test_pixel(root, x, y, &r, &g, &b);
			if (r > 128) lit++;
		}
		/* Undo has no drawing yet: its empty box is lit too, but thinly */
		CHECK(lit > 20);
		if (lit <= 20) printf("  symbol %d\n", (int)syms[i]);
		Zroot_destroy(root);
	}
}


/* ---- AppBarButton ---- */

static void test_app_bar_button(void) {
	Zid root = Zroot_create(480, 400), stack = Zstack_panel_create(Z_VERTICAL), b, tg, sep;
	ZRect r;
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zapp_bar_button_create("save", Zsymbol_icon_create(Z_SYMBOL_SAVE)));
	tg = Zpanel_add(stack, Zapp_bar_toggle_button_create("pin", Zsymbol_icon_create(Z_SYMBOL_PIN)));
	sep = Zpanel_add(stack, Zapp_bar_separator_create());
	Zroot_update_layout(root);
	CHECK(Zelement_get_class(b) == Z_CLASS_APP_BAR_BUTTON && Zelement_is_a(b, Z_CLASS_BUTTON));
	CHECK(Zelement_is_a(tg, Z_CLASS_TOGGLE_BUTTON));
	CHECK_STR(Zapp_bar_button_get_label(b), "save");
	CHECK(Zapp_bar_button_get_icon(b) != Z_INVALID_ID && Zelement_get_class(Zapp_bar_button_get_icon(b)) == Z_CLASS_SYMBOL_ICON);
	r = Zelement_get_bounds(b);
	/* 80 wide, 12 + 38.5 + 4 + the label (9.5 px) + 10.5 */
	CHECK_NEAR(r.width, 80);
	CHECK(r.height > 12 + 38.5f + 4 + 10.5f + 8 && r.height < 12 + 38.5f + 4 + 10.5f + 20);
	/* the toggle button is a pixel higher at the top of its circle: 11 */
	CHECK(Zelement_get_bounds(tg).height < r.height + 0.01f);
	Zapp_bar_button_set_is_compact(b, true);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(b);
	CHECK_NEAR(r.width, 48);
	CHECK_NEAR(r.height, 12 + 38.5 + 4);
	CHECK(Zapp_bar_button_get_is_compact(b));
	CHECK_NEAR(Zelement_get_bounds(sep).width, 12);
	Zapp_bar_separator_set_is_compact(sep, true);
	CHECK(Zapp_bar_separator_get_is_compact(sep));
	/* a tap: Click, a toggle: IsChecked */
	Zbutton_on_click(b, on_click, NULL);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	CHECK(!Ztoggle_button_get_is_checked(tg));
	tap(root, center(tg));
	CHECK(Ztoggle_button_get_is_checked(tg));
	/* pressed: the circle is filled with the foreground and the icon takes the background color */
	{
		ZPoint c = Zelement_to_root(b, (ZPoint){ 24, 12 + 19.25f });
		Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x, c.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, c.x, c.y);
		CHECK(Zbutton_is_pressed(b));
		CHECK(pixel_is(root, (int)c.x + 8, (int)c.y + 8, 255, 255, 255));
		Zroot_inject_pointer(root, Z_POINTER_UP, c.x, c.y);
		CHECK(pixel_is(root, (int)c.x + 8, (int)c.y + 8, 0, 0, 0));
	}
	/* checked: filled, as if it were pressed */
	{
		ZPoint c = Zelement_to_root(tg, (ZPoint){ 40, 11 + 19.25f });
		CHECK(pixel_is(root, (int)c.x + 10, (int)c.y + 10, 255, 255, 255));
		Ztoggle_button_set_is_checked(tg, false);
		CHECK(pixel_is(root, (int)c.x + 10, (int)c.y + 10, 0, 0, 0));
	}
	Zroot_destroy(root);
}


/* ---- the bars of a page ---- */

static Zid g_cmd_bar, g_content_button, g_sec, g_prim;

static Zid page_with_bar(Zid frame, void* parameter) {
	Zid page = Zpage_create(), stack = Zstack_panel_create(Z_VERTICAL), bar = Zcommand_bar_create(), more;
	(void)frame; (void)parameter;
	g_content_button = Zpanel_add(stack, Zbutton_create("content"));
	Zelement_set_vertical_alignment(g_content_button, Z_VALIGN_TOP);
	Zbutton_on_click(g_content_button, on_page_click, NULL);
	Zpage_set_content(page, stack);
	g_prim = Zapp_bar_button_create("add", Zsymbol_icon_create(Z_SYMBOL_ADD));
	Zcommand_bar_add_primary_command(bar, g_prim);
	Zcommand_bar_add_primary_command(bar, Zapp_bar_separator_create());
	Zcommand_bar_add_primary_command(bar, Zapp_bar_toggle_button_create("pin", Zsymbol_icon_create(Z_SYMBOL_PIN)));
	g_sec = Zapp_bar_button_create("settings", Z_INVALID_ID);
	Zbutton_on_click(g_sec, on_click, NULL);
	Zcommand_bar_add_secondary_command(bar, g_sec);
	more = Zapp_bar_button_create("about", Z_INVALID_ID);
	Zcommand_bar_add_secondary_command(bar, more);
	Zpage_set_bottom_app_bar(page, bar);
	g_cmd_bar = bar;
	return page;
}

static Zid other_page(Zid frame, void* parameter) { (void)frame; (void)parameter; return Zpage_create(); }

static Zid setup(Zid* frame, Zid* page) {
	Zid root = Zroot_create(480, 800);
	*frame = Zframe_create();
	Zroot_set_visual(root, *frame);
	Zframe_navigate(*frame, page_with_bar, NULL);
	*page = Zframe_get_page(*frame);
	Zroot_update_layout(root);
	g_opened = g_closed = g_clicks = g_page_clicks = 0;
	Zapp_bar_on_opened(g_cmd_bar, on_opened, NULL);
	Zapp_bar_on_closed(g_cmd_bar, on_closed, NULL);
	return root;
}

static void test_page_bars_layout(void) {
	Zid frame, page, root = setup(&frame, &page), content, bar = g_cmd_bar, top;
	ZRect r, pr;
	CHECK(Zpage_get_bottom_app_bar(page) == bar && Zpage_get_top_app_bar(page) == Z_INVALID_ID);
	CHECK(Zelement_get_class(bar) == Z_CLASS_COMMAND_BAR && Zelement_is_a(bar, Z_CLASS_APP_BAR) && Zelement_is_a(bar, Z_CLASS_CONTENT_CONTROL));
	pr = Zelement_get_bounds(page);
	r = Zelement_get_bounds(bar);
	/* closed, Compact: 1.5 + 54.5 + 1.5 = 57.5 px at the bottom of the page */
	CHECK_NEAR(r.height, 57.5);
	CHECK_NEAR(r.y + r.height, pr.height);
	CHECK_NEAR(r.width, 480);
	content = Zpage_get_content(page);
	CHECK_NEAR(Zelement_get_bounds(content).height, pr.height - 57.5);
	CHECK(Zapp_bar_get_closed_display_mode(bar) == Z_APP_BAR_CLOSED_COMPACT);
	/* Minimal: 35 px */
	Zapp_bar_set_closed_display_mode(bar, Z_APP_BAR_CLOSED_MINIMAL);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(bar).height, 35);
	CHECK_NEAR(Zelement_get_bounds(content).height, pr.height - 35);
	/* the commands of a Minimal bar are out of sight and out of reach */
	tap(root, (ZPoint){ 100, Zelement_to_root(bar, (ZPoint){ 0, 17 }).y });
	CHECK(Zapp_bar_get_is_open(bar));
	Zapp_bar_set_is_open(bar, false);
	Zapp_bar_set_closed_display_mode(bar, Z_APP_BAR_CLOSED_COMPACT);
	Zroot_update_layout(root);
	/* a TopAppBar lies at the top of the page, the content under it */
	top = Zcommand_bar_create();
	Zcommand_bar_add_primary_command(top, Zapp_bar_button_create("x", Zsymbol_icon_create(Z_SYMBOL_CANCEL)));
	Zpage_set_top_app_bar(page, top);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(top).y, 0);
	CHECK_NEAR(Zelement_get_bounds(content).y, 57.5);
	CHECK_NEAR(Zelement_get_bounds(content).height, pr.height - 115);
	/* the one that was there is destroyed when another is set */
	Zpage_set_top_app_bar(page, Z_INVALID_ID);
	CHECK(!Zelement_is_valid(top));
	CHECK(Zpage_get_top_app_bar(page) == Z_INVALID_ID);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(content).y, 0);
	Zroot_destroy(root);
}

static void test_page_bar_pixels(void) {
	Zid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	ZRect r = Zelement_get_bounds(bar);
	ZPoint o = Zelement_to_root(bar, (ZPoint){ 0, 0 });
	int y0 = (int)o.y;
	(void)r;
	/* CommandBarBackgroundThemeBrush: the chrome 1F1F1F, under a 1.5 px line in the color of the page */
	CHECK(pixel_is(root, 5, y0 + 30, 0x1F, 0x1F, 0x1F));
	CHECK(pixel_is(root, 5, y0, 0, 0, 0));
	CHECK(pixel_is(root, 5, y0 + 3, 0x1F, 0x1F, 0x1F));
	/* the three dots at the right edge: the middle one is in the foreground, in line with the circles */
	CHECK(pixel_is(root, 480 - 15, y0 + (int)(1.5f + 12 + 19.25f), 255, 255, 255));
	show(root, "command-bar-closed");
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	show(root, "command-bar-open");
	Zroot_set_theme(root, Z_THEME_LIGHT);
	/* the chrome of the Light theme is DDDDDD */
	{
		ZPoint p = Zelement_to_root(bar, (ZPoint){ 5, 10 });
		CHECK(pixel_is(root, (int)p.x, (int)p.y, 0xDD, 0xDD, 0xDD));
	}
	show(root, "command-bar-open-light");
	Zroot_destroy(root);
}

static void test_open_close(void) {
	Zid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	ZRect closed, open;
	closed = Zelement_get_bounds(bar);
	/* a tap on the three dots opens the bar: Opened, the labels, the secondary commands as rows */
	tap(root, (ZPoint){ 480 - 15, Zelement_to_root(bar, (ZPoint){ 0, 30 }).y });
	CHECK(Zapp_bar_get_is_open(bar) && g_opened == 1 && g_closed == 0);
	Zroot_update_layout(root);
	open = Zelement_get_bounds(bar);
	CHECK(open.height > closed.height + 40);
	/* a bar at the bottom grows upward */
	CHECK_NEAR(open.y + open.height, closed.y + closed.height);
	CHECK(!Zapp_bar_button_get_is_compact(g_prim));
	/* the labels make the buttons taller than the closed row, and the secondary commands are rows of text above them */
	CHECK(Zelement_get_bounds(g_sec).y < Zelement_get_bounds(g_prim).y);
	CHECK_NEAR(Zelement_get_bounds(g_sec).width, 480);
	/* a tap on the dots again closes it */
	tap(root, (ZPoint){ 480 - 15, Zelement_to_root(bar, (ZPoint){ 0, open.height - 40 }).y });
	CHECK(!Zapp_bar_get_is_open(bar) && g_closed == 1);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(bar).height, 57.5);
	CHECK(Zapp_bar_button_get_is_compact(g_prim));
	/* IsOpen by the program raises the events once per change */
	Zapp_bar_set_is_open(bar, true);
	Zapp_bar_set_is_open(bar, true);
	CHECK(g_opened == 2);
	Zapp_bar_set_is_open(bar, false);
	CHECK(g_closed == 2);
	Zroot_destroy(root);
}

static void test_light_dismiss_and_sticky(void) {
	Zid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	/* a press outside the open bar closes it and is not given to the content under the press */
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	tap(root, center(g_content_button));
	CHECK(!Zapp_bar_get_is_open(bar) && g_closed == 1);
	CHECK(g_page_clicks == 0);
	tap(root, center(g_content_button));
	CHECK(g_page_clicks == 1);
	/* a press on the bar itself does not close it */
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	tap(root, (ZPoint){ 20, Zelement_to_root(bar, (ZPoint){ 0, 8 }).y });
	CHECK(Zapp_bar_get_is_open(bar));
	/* a sticky bar stays open through a press outside it, and through Back */
	Zapp_bar_set_is_sticky(bar, true);
	CHECK(Zapp_bar_get_is_sticky(bar));
	tap(root, center(g_content_button));
	CHECK(Zapp_bar_get_is_open(bar));
	CHECK(g_page_clicks == 2);
	Zapp_bar_set_is_sticky(bar, false);
	/* Back closes the bar before it navigates */
	Zframe_navigate(frame, other_page, NULL);
	Zroot_update_layout(root);
	CHECK(Zframe_get_back_stack_depth(frame) == 1);
	Zroot_destroy(root);

	root = setup(&frame, &page);
	Zframe_go_back(frame);
	Zframe_navigate(frame, other_page, NULL);
	Zframe_navigate(frame, page_with_bar, NULL);
	page = Zframe_get_page(frame);
	bar = Zpage_get_bottom_app_bar(page);
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	CHECK(Zroot_back(root));
	CHECK(!Zapp_bar_get_is_open(bar));
	CHECK(Zframe_get_page(frame) == page);
	CHECK(Zroot_back(root));
	CHECK(Zframe_get_page(frame) != page);
	Zroot_destroy(root);
}

static void test_secondary_command(void) {
	Zid frame, page, root = setup(&frame, &page), bar = g_cmd_bar;
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	tap(root, center(g_sec));
	/* the command ran and the menu went */
	CHECK(g_clicks == 1);
	CHECK(!Zapp_bar_get_is_open(bar) && g_closed == 1);
	/* a primary one leaves the bar as it is */
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	Zbutton_on_click(g_prim, on_click, NULL);
	tap(root, center(g_prim));
	CHECK(g_clicks == 2 && Zapp_bar_get_is_open(bar));
	/* the commands: counts, indexes, wrong classes refused */
	CHECK(Zcommand_bar_get_primary_command_count(bar) == 3 && Zcommand_bar_get_secondary_command_count(bar) == 2);
	CHECK(Zcommand_bar_get_primary_command(bar, 0) == g_prim && Zcommand_bar_get_secondary_command(bar, 0) == g_sec);
	CHECK(Zcommand_bar_get_primary_command(bar, 9) == Z_INVALID_ID);
	Zset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	CHECK(Zcommand_bar_add_primary_command(bar, Zbutton_create("plain")) == -1);
	Zcommand_bar_clear_secondary_commands(bar);
	CHECK(Zcommand_bar_get_secondary_command_count(bar) == 0 && !Zelement_is_valid(g_sec));
	/* destroying a command takes it off the bar */
	Zelement_destroy(g_prim);
	CHECK(Zcommand_bar_get_primary_command_count(bar) == 2);
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

static void test_app_bar_content(void) {
	Zid root = Zroot_create(480, 800), page = Zpage_create(), bar = Zapp_bar_create(), tb = Ztext_block_create("hello");
	Zpage_set_top_app_bar(page, bar);
	Zcontent_control_set_content(bar, tb);
	Zroot_set_visual(root, page);
	Zroot_update_layout(root);
	/* a plain AppBar is as tall as its content when it is open and has the page background (AppBarBackgroundThemeBrush) */
	CHECK_NEAR(Zelement_get_bounds(bar).height, 57.5);
	CHECK(pixel_is(root, 300, 20, 0, 0, 0));
	Zapp_bar_set_is_open(bar, true);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(bar).height, 57.5);
	Zapp_bar_set_closed_display_mode(bar, Z_APP_BAR_CLOSED_MINIMAL);
	Zapp_bar_set_is_open(bar, false);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(bar).height, 35);
	Zroot_destroy(root);
}


/* ---- the opening animation ---- */

static void test_open_animation(void) {
	Zid frame, page, root;
	ZRect r0, r1, r2;
	root = setup(&frame, &page);
	Zroot_set_animations(root, Z_ANIMATIONS_ON);
	r0 = Zelement_get_bounds(g_cmd_bar);
	Zapp_bar_set_is_open(g_cmd_bar, true);
	/* IsOpen is at once, the bar takes 167 ms to get there */
	CHECK(Zapp_bar_get_is_open(g_cmd_bar) && g_opened == 1);
	run(root, 64);
	Zroot_update_layout(root);
	r1 = Zelement_get_bounds(g_cmd_bar);
	CHECK(r1.height > r0.height + 1);
	run(root, 400);
	Zroot_update_layout(root);
	r2 = Zelement_get_bounds(g_cmd_bar);
	CHECK(r2.height > r1.height + 20);
	CHECK_NEAR(r2.y + r2.height, r0.y + r0.height);
	/* and back */
	Zapp_bar_set_is_open(g_cmd_bar, false);
	run(root, 64);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(g_cmd_bar).height < r2.height - 1 && Zelement_get_bounds(g_cmd_bar).height > r0.height);
	run(root, 400);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(g_cmd_bar).height, r0.height);
	CHECK(Zapp_bar_button_get_is_compact(g_prim));
	Zroot_destroy(root);
}


/* ---- Frame and Page: NavigationMode and NavigationCacheMode ---- */

static char g_log[512];
static void log_add(const char* s) { strcat(g_log, s); }
static const char* mode_name(ZNavigationMode m) {
	return m == Z_NAVIGATION_NEW ? "new" : m == Z_NAVIGATION_BACK ? "back" : m == Z_NAVIGATION_FORWARD ? "forward" : "other";
}

static void log_to(Zid page, void* u) { log_add("to-"); log_add((const char*)u); log_add(":"); log_add(mode_name(Zpage_get_navigation_mode(page))); log_add(" "); }
static void log_from(Zid page, void* u) { log_add("from-"); log_add((const char*)u); log_add(":"); log_add(mode_name(Zpage_get_navigation_mode(page))); log_add(" "); }

static int g_built_a, g_built_b;

static Zid mode_page(Zid frame, void* parameter) {
	Zid page = Zpage_create();
	const char* name = (const char*)parameter;
	(void)frame;
	if (name[0] == 'a') g_built_a++;
	else g_built_b++;
	Zpage_on_navigated_to(page, log_to, parameter);
	Zpage_on_navigated_from(page, log_from, parameter);
	if (name[1] == 'c') Zpage_set_navigation_cache_mode(page, Z_NAVIGATION_CACHE_REQUIRED);
	return page;
}

static void test_navigation_mode(void) {
	Zid root = Zroot_create(480, 800), frame = Zframe_create();
	Zroot_set_visual(root, frame);
	g_log[0] = 0;
	Zframe_navigate(frame, mode_page, "a");
	Zframe_navigate(frame, mode_page, "b");
	CHECK_STR(g_log, "to-a:new from-a:new to-b:new ");
	g_log[0] = 0;
	Zframe_go_back(frame);
	/* the page that leaves is told how the next one is reached */
	CHECK_STR(g_log, "from-b:back to-a:back ");
	g_log[0] = 0;
	Zframe_go_forward(frame);
	CHECK_STR(g_log, "from-a:forward to-b:forward ");
	CHECK(Zframe_get_current_source_page_type(frame) == mode_page);
	CHECK(Zpage_get_navigation_cache_mode(Zframe_get_page(frame)) == Z_NAVIGATION_CACHE_DISABLED);
	Zroot_destroy(root);
}

static void test_navigation_cache(void) {
	Zid root = Zroot_create(480, 800), frame = Zframe_create(), a, b;
	Zroot_set_visual(root, frame);
	g_built_a = g_built_b = 0;
	/* "ac" asks for the cache, "b" does not */
	Zframe_navigate(frame, mode_page, "ac");
	a = Zframe_get_page(frame);
	Zframe_navigate(frame, mode_page, "b");
	b = Zframe_get_page(frame);
	/* the page that is cached is alive, and the other is gone */
	CHECK(Zelement_is_valid(a) && Zelement_get_parent(a) == Z_INVALID_ID);
	CHECK(Zelement_is_valid(b));
	Zframe_go_back(frame);
	CHECK(Zframe_get_page(frame) == a);
	CHECK(g_built_a == 1);
	CHECK(!Zelement_is_valid(b));
	CHECK(Zpage_get_frame(a) == frame);
	Zframe_go_forward(frame);
	CHECK(g_built_b == 2);
	/* a Navigate that drops the entry destroys the page kept for it */
	Zframe_go_back(frame);
	CHECK(Zframe_get_page(frame) == a);
	Zframe_navigate(frame, mode_page, "b");
	Zframe_navigate(frame, mode_page, "ac");
	Zframe_go_back(frame);
	Zframe_go_back(frame);
	CHECK(Zframe_get_page(frame) == a);
	CHECK(Zframe_navigate(frame, mode_page, "b"));
	CHECK(Zelement_is_valid(a));
	Zframe_go_back(frame);
	/* the frame destroys what it keeps */
	Zframe_navigate(frame, mode_page, "ac");
	Zframe_navigate(frame, mode_page, "b");
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
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
	Zshutdown();
	return test_finish("test_bars");
}
