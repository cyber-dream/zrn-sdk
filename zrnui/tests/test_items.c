/**
 * The items controls of Windows Phone 8.1: ListView, GridView, ListBox, ComboBox, FlipView,
 * AutoSuggestBox and SemanticZoom. Every check goes through what a finger or a key would do (taps through
 * Zroot_inject_pointer, keys, the clock), or reads a pixel of the rendered page (Dark theme, Cobalt accent).
 */

#include "test.h"

static int g_changes, g_clicks, g_clicked, g_nav;
static Zid g_nav_root;

static void on_change(Zid sender, void* user) { (void)sender; (void)user; g_changes++; }
static void on_item_click(Zid sender, int index, void* user) { (void)sender; (void)user; g_clicks++; g_clicked = index; }

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

static void type(Zid root, const char* text) { Zroot_inject_text(root, text); }
static void key(Zid root, ZKey k, int mods) { Zroot_inject_key(root, k, mods, 1); Zroot_inject_key(root, k, mods, 0); }

static void run(Zid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Zroot_tick(root, 16);
}

static Zid make_root(Zid visual, ZAnimations animations) {
	Zid root = Zroot_create(480, 800);
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, animations);
	Zroot_set_visual(root, visual);
	Zroot_update_layout(root);
	return root;
}

static void fill(Zid control, int count) {
	char name[32];
	int i;
	for (i = 0; i < count; i++) {
		snprintf(name, sizeof name, "item %d", i);
		Zitems_control_add_text(control, name);
	}
}

static int pixel_is(Zid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

static int lit(Zid root, int x0, int y0, int x1, int y1) {
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


/* ---- hierarchy ---- */

static void quiet(ZError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Zid lv = Zlist_view_create(), gv = Zgrid_view_create(), item = Zlist_view_item_create("x"), fv = Zflip_view_create();
	Zid asb = Zauto_suggest_box_create(), cb = Zcombo_box_create(), sz = Zsemantic_zoom_create();
	CHECK(Zelement_is_a(lv, Z_CLASS_LIST_VIEW_BASE) && Zelement_is_a(lv, Z_CLASS_SELECTOR) && Zelement_is_a(lv, Z_CLASS_ITEMS_CONTROL));
	CHECK(Zelement_is_a(gv, Z_CLASS_LIST_VIEW_BASE) && !Zelement_is_a(gv, Z_CLASS_LIST_VIEW));
	CHECK(Zelement_get_class(lv) == Z_CLASS_LIST_VIEW && Zelement_get_class(gv) == Z_CLASS_GRID_VIEW);
	CHECK(Zelement_is_a(item, Z_CLASS_SELECTOR_ITEM) && Zelement_is_a(item, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zelement_is_a(fv, Z_CLASS_SELECTOR) && Zelement_is_a(cb, Z_CLASS_SELECTOR));
	CHECK(Zelement_is_a(asb, Z_CLASS_ITEMS_CONTROL) && !Zelement_is_a(asb, Z_CLASS_SELECTOR) && !Zelement_is_a(asb, Z_CLASS_TEXT_BOX));
	CHECK(Zelement_is_a(sz, Z_CLASS_CONTROL));
	CHECK_STR(Zelement_get_class_name(gv), "GridView");
	CHECK_STR(Zelement_get_class_name(item), "ListViewItem");
	CHECK_STR(Zelement_get_class_name(Zitems_control_get_items_panel(lv)), "ItemsStackPanel");
	CHECK_STR(Zelement_get_class_name(Zitems_control_get_items_panel(gv)), "ItemsWrapGrid");
	Zset_error_callback(quiet, NULL);
	CHECK(Zelement_create(Z_CLASS_LIST_VIEW_BASE) == Z_INVALID_ID); /* abstract */
	CHECK(Zelement_create(Z_CLASS_SELECTOR_ITEM) == Z_INVALID_ID);
	Zset_error_callback(NULL, NULL);
	CHECK(Zelement_is_a(Zelement_create(Z_CLASS_GRID_VIEW_HEADER_ITEM), Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM));
	Zelement_destroy(lv);
	Zelement_destroy(gv);
	Zelement_destroy(item);
	Zelement_destroy(fv);
	Zelement_destroy(asb);
	Zelement_destroy(cb);
	Zelement_destroy(sz);
}


/* ---- ListView ---- */

static void test_list_view_single_selection(void) {
	Zid lv = Zlist_view_create(), root;
	int indices[4];
	fill(lv, 5);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	CHECK(Zlist_view_get_selection_mode(lv) == Z_LIST_VIEW_SELECTION_SINGLE);
	g_changes = 0;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Zitems_control_get_container(lv, 2)));
	CHECK(Zselector_get_selected_index(lv) == 2 && g_changes == 1);
	CHECK(Zselector_item_get_is_selected(Zitems_control_get_container(lv, 2)));
	tap(root, center(Zitems_control_get_container(lv, 2)));
	CHECK(Zselector_get_selected_index(lv) == 2 && g_changes == 1); /* the same item: no change */
	tap(root, center(Zitems_control_get_container(lv, 4)));
	CHECK(Zselector_get_selected_index(lv) == 4 && g_changes == 2);
	CHECK(!Zselector_item_get_is_selected(Zitems_control_get_container(lv, 2)));
	CHECK(Zlist_view_get_selected_indices(lv, indices, 4) == 1 && indices[0] == 4);
	/* by the program, and SelectedItem is the container of the item */
	Zselector_set_selected_index(lv, 1);
	CHECK(Zselector_get_selected_container(lv) == Zitems_control_get_container(lv, 1) && g_changes == 3);
	Zselector_set_selected_index(lv, -1);
	CHECK(Zselector_get_selected_index(lv) == -1 && g_changes == 4);
	/* select all is for the multiple modes */
	Zlist_view_select_all(lv);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 0);
	/* an index past the end selects nothing */
	Zselector_set_selected_index(lv, 99);
	CHECK(Zselector_get_selected_index(lv) == -1);
	Zroot_destroy(root);
}

static void test_list_view_none_and_item_click(void) {
	Zid lv = Zlist_view_create(), root;
	fill(lv, 4);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	g_changes = g_clicks = 0;
	g_clicked = -1;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	Zlist_view_on_item_click(lv, on_item_click, NULL);
	/* IsItemClickEnabled is off at first: a tap selects and nothing is clicked */
	CHECK(!Zlist_view_get_is_item_click_enabled(lv));
	tap(root, center(Zitems_control_get_container(lv, 1)));
	CHECK(g_clicks == 0 && Zselector_get_selected_index(lv) == 1 && g_changes == 1);
	Zlist_view_set_is_item_click_enabled(lv, true);
	tap(root, center(Zitems_control_get_container(lv, 3)));
	CHECK(g_clicks == 1 && g_clicked == 3 && Zselector_get_selected_index(lv) == 3);
	/* None: the selection that was is gone, and a tap is only an ItemClick */
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_NONE);
	CHECK(Zselector_get_selected_index(lv) == -1 && g_changes == 3);
	tap(root, center(Zitems_control_get_container(lv, 0)));
	CHECK(g_clicks == 2 && g_clicked == 0 && Zselector_get_selected_index(lv) == -1 && g_changes == 3);
	Zselector_set_selected_index(lv, 2);
	CHECK(Zselector_get_selected_index(lv) == -1);
	/* a disabled item does not click */
	Zelement_set_enabled(Zitems_control_get_container(lv, 1), false);
	tap(root, center(Zitems_control_get_container(lv, 1)));
	CHECK(g_clicks == 2);
	/* a press that slides off the item is not a tap */
	{
		ZPoint p = center(Zitems_control_get_container(lv, 2));
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, 470, 790);
		Zroot_inject_pointer(root, Z_POINTER_UP, 470, 790);
		CHECK(g_clicks == 2);
	}
	Zlist_view_on_item_click(lv, NULL, NULL);
	tap(root, center(Zitems_control_get_container(lv, 0)));
	CHECK(g_clicks == 2);
	Zroot_destroy(root);
}

static void test_list_view_multiple_and_extended(void) {
	Zid lv = Zlist_view_create(), root;
	int indices[8];
	fill(lv, 6);
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	g_changes = 0;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Zitems_control_get_container(lv, 1)));
	tap(root, center(Zitems_control_get_container(lv, 3)));
	CHECK(Zlist_view_get_selected_indices(lv, indices, 8) == 2 && indices[0] == 1 && indices[1] == 3 && g_changes == 2);
	CHECK(Zselector_get_selected_index(lv) == 1);
	tap(root, center(Zitems_control_get_container(lv, 1)));
	CHECK(Zlist_view_get_selected_indices(lv, indices, 8) == 1 && indices[0] == 3 && g_changes == 3);
	Zlist_view_select_all(lv);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 6);
	/* the keyboard: Ctrl+A on a view that has it */
	Zselector_set_selected_index(lv, -1);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 0);
	Zelement_focus(lv);
	key(root, Z_KEY_A, Z_MOD_CTRL);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 6);
	/* Extended: a tap selects that item alone */
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_EXTENDED);
	tap(root, center(Zitems_control_get_container(lv, 2)));
	CHECK(Zlist_view_get_selected_indices(lv, indices, 8) == 1 && indices[0] == 2);
	/* back to Single keeps one */
	Zlist_view_select_all(lv);
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_SINGLE);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 1);
	Zroot_destroy(root);
}

static void test_list_view_look(void) {
	Zid lv = Zlist_view_create(), root, item;
	ZRect r;
	int y;
	fill(lv, 3);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	item = Zitems_control_get_container(lv, 0);
	r = Zelement_get_bounds(item);
	/* the text is 12 px: a row is a line and nothing else (ListViewItem has no padding) */
	CHECK(r.height > 12 && r.height < 24 && r.x == 0);
	Zselector_set_selected_index(lv, 0);
	/* Single selection draws nothing: no check box, no accent */
	y = (int)r.y;
	CHECK(lit(root, 100, y, 480, y + (int)r.height) == 0);
	/* Multiple: the 25.5 box with the 2.5 outline, and the content moves 40.5 to the right */
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	Zselector_set_selected_index(lv, -1);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(Zitems_control_get_container(lv, 1));
	y = (int)r.y;
	CHECK(pixel_is(root, 1, y + 22, 255, 255, 255)); /* the left side of the outline */
	CHECK(pixel_is(root, 12, y + 22, 0, 0, 0));      /* hollow */
	CHECK(pixel_is(root, 12, y + 28, 0, 0, 0));
	CHECK(lit(root, 0, y, 40, y + 40) > 100);
	CHECK(Zelement_get_bounds(Zcontent_control_get_content(Zitems_control_get_container(lv, 1))).x >= 40.4f);
	tap(root, center(Zitems_control_get_container(lv, 1)));
	CHECK(pixel_is(root, 11, y + 28, 255, 255, 255)); /* the check mark */
	/* a disabled item is drawn at 40%: its box is dimmer */
	Zelement_set_enabled(Zitems_control_get_container(lv, 2), false);
	r = Zelement_get_bounds(Zitems_control_get_container(lv, 2));
	{
		int pr, pg, pb;
		test_pixel(root, 1, (int)r.y + 22, &pr, &pg, &pb);
		CHECK(pr > 60 && pr < 140);
	}
	Zroot_destroy(root);
}

static void test_list_view_header_footer_and_scroll(void) {
	Zid lv = Zlist_view_create(), root, sv, header;
	double extent_without;
	fill(lv, 200);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	sv = Zitems_control_get_scroll_viewer(lv);
	CHECK(Zelement_is_a(sv, Z_CLASS_SCROLL_VIEWER));
	extent_without = Zscroll_viewer_get_extent_height(sv);
	CHECK(extent_without > 800);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(lv, 0)).y == 0);
	/* a string header and a string footer scroll with the items */
	Zlist_view_set_header(lv, "Header text");
	Zlist_view_set_footer(lv, "Footer text");
	Zroot_update_layout(root);
	CHECK(Zscroll_viewer_get_extent_height(sv) > extent_without + 30);
	{
		ZPoint p = Zelement_to_root(Zitems_control_get_container(lv, 0), (ZPoint){ 0, 0 });
		CHECK(p.y > 15);
		CHECK(lit(root, 0, 0, 200, (int)p.y) > 50); /* the header was drawn */
	}
	/* an element as header replaces the string */
	header = Zbutton_create("a button");
	Zlist_view_set_header_content(lv, header);
	Zroot_update_layout(root);
	CHECK(Zelement_get_parent(header) != Z_INVALID_ID && Zelement_get_root(header) == root);
	{
		ZPoint p = Zelement_to_root(Zitems_control_get_container(lv, 0), (ZPoint){ 0, 0 });
		CHECK(p.y >= Zelement_get_bounds(header).height);
	}
	Zlist_view_set_header(lv, NULL);
	Zlist_view_set_footer_content(lv, Z_INVALID_ID);
	Zroot_update_layout(root);
	CHECK_NEAR(Zscroll_viewer_get_extent_height(sv), extent_without);
	/* a drag scrolls and selects nothing, the wheel scrolls, ScrollIntoView brings the item */
	g_changes = 0;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	{
		ZPoint p = center(Zitems_control_get_container(lv, 10));
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 30);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 200);
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y - 200);
		CHECK_NEAR(Zscroll_viewer_get_vertical_offset(sv), 200);
		CHECK(g_changes == 0 && Zselector_get_selected_index(lv) == -1);
	}
	Zscroll_viewer_scroll_to_vertical_offset(sv, 0);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 200, 200);
	Zroot_inject_wheel(root, -1);
	CHECK(Zscroll_viewer_get_vertical_offset(sv) > 0);
	Zscroll_viewer_scroll_to_vertical_offset(sv, 0);
	Zlist_view_scroll_into_view(lv, 199);
	CHECK_NEAR(Zscroll_viewer_get_vertical_offset(sv), Zscroll_viewer_get_extent_height(sv) - Zscroll_viewer_get_viewport_height(sv));
	Zlist_view_scroll_into_view_with_alignment(lv, 20, Z_SCROLL_INTO_VIEW_LEADING);
	Zroot_update_layout(root);
	{
		ZPoint p = Zelement_to_root(Zitems_control_get_container(lv, 20), (ZPoint){ 0, 0 });
		CHECK_NEAR(p.y, 0);
	}
	/* an item that is in view does not move the view */
	{
		double before = Zscroll_viewer_get_vertical_offset(sv);
		Zlist_view_scroll_into_view(lv, 21);
		CHECK(Zscroll_viewer_get_vertical_offset(sv) == before);
	}
	Zroot_destroy(root);
}

static void test_list_view_keyboard(void) {
	Zid lv = Zlist_view_create(), root;
	fill(lv, 5);
	Zlist_view_set_is_item_click_enabled(lv, true);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	g_changes = g_clicks = 0;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	Zlist_view_on_item_click(lv, on_item_click, NULL);
	/* a touch gives the list the keyboard; Down selects the first item */
	tap(root, center(Zitems_control_get_container(lv, 4)));
	CHECK(Zroot_get_focus(root) == lv && Zselector_get_selected_index(lv) == 4);
	key(root, Z_KEY_UP, 0);
	CHECK(Zselector_get_selected_index(lv) == 3);
	key(root, Z_KEY_HOME, 0);
	CHECK(Zselector_get_selected_index(lv) == 0);
	key(root, Z_KEY_DOWN, 0);
	key(root, Z_KEY_DOWN, 0);
	CHECK(Zselector_get_selected_index(lv) == 2);
	key(root, Z_KEY_END, 0);
	CHECK(Zselector_get_selected_index(lv) == 4);
	key(root, Z_KEY_DOWN, 0); /* the end of the list stays */
	CHECK(Zselector_get_selected_index(lv) == 4);
	/* Space and Enter are a tap */
	g_clicks = 0;
	key(root, Z_KEY_SPACE, 0);
	CHECK(g_clicks == 1 && g_clicked == 4);
	key(root, Z_KEY_ENTER, 0);
	CHECK(g_clicks == 2);
	/* in Multiple the arrows only move, Space toggles */
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	Zselector_set_selected_index(lv, -1);
	key(root, Z_KEY_HOME, 0);
	CHECK(Zlist_view_get_selected_indices(lv, NULL, 0) == 0);
	key(root, Z_KEY_SPACE, 0);
	key(root, Z_KEY_DOWN, 0);
	key(root, Z_KEY_DOWN, 0);
	key(root, Z_KEY_SPACE, 0);
	{
		int indices[4];
		CHECK(Zlist_view_get_selected_indices(lv, indices, 4) == 2 && indices[0] == 0 && indices[1] == 2);
	}
	Zroot_destroy(root);
}

static void test_items_panels_and_groups(void) {
	Zid lv = Zlist_view_create(), root, panel, h1, h2;
	fill(lv, 3);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	/* a horizontal panel: the items go side by side and the view scrolls sideways */
	panel = Zitems_stack_panel_create(Z_HORIZONTAL);
	Zitems_control_set_items_panel(lv, panel);
	CHECK(Zitems_control_get_items_panel(lv) == panel && Zitems_control_get_count(lv) == 3);
	Zroot_update_layout(root);
	{
		ZRect a = Zelement_get_bounds(Zitems_control_get_container(lv, 0)), b = Zelement_get_bounds(Zitems_control_get_container(lv, 1));
		CHECK(b.x > a.x + a.width - 1 && b.y == a.y);
	}
	Zroot_destroy(root);
	/* a vertical panel again, with groups */
	lv = Zlist_view_create();
	h1 = Zlist_view_add_group_header(lv, "Fruit");
	Zitems_control_add_text(lv, "apple");
	Zitems_control_add_text(lv, "banana");
	h2 = Zlist_view_add_group_header(lv, "Vegetables");
	Zitems_control_add_text(lv, "carrot");
	CHECK(Zelement_is_a(h1, Z_CLASS_LIST_VIEW_HEADER_ITEM) && Zlist_view_get_group_count(lv) == 2);
	CHECK(Zitems_control_get_count(lv) == 3);
	CHECK_STR(Zitems_control_get_text(lv, 2), "carrot");
	CHECK(Zitems_control_index_from_container(lv, Zitems_control_get_container(lv, 2)) == 2);
	CHECK(Zitems_control_index_from_container(lv, h2) == -1);
	root = make_root(lv, Z_ANIMATIONS_OFF);
	g_changes = 0;
	Zelement_on(lv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Zitems_control_get_container(lv, 2)));
	CHECK(Zselector_get_selected_index(lv) == 2);
	/* a tap on a header selects nothing */
	tap(root, center(h2));
	CHECK(Zselector_get_selected_index(lv) == 2 && g_changes == 1);
	/* the header is above its items */
	CHECK(Zelement_get_bounds(h1).y < Zelement_get_bounds(Zitems_control_get_container(lv, 0)).y);
	CHECK(Zelement_get_bounds(h2).y > Zelement_get_bounds(Zitems_control_get_container(lv, 1)).y);
	Zitems_control_remove_at(lv, 0);
	CHECK(Zitems_control_get_count(lv) == 2 && Zselector_get_selected_index(lv) == 1);
	Zitems_control_clear(lv);
	CHECK(Zitems_control_get_count(lv) == 0 && Zlist_view_get_group_count(lv) == 0);
	Zroot_destroy(root);
}


/* ---- GridView and ItemsWrapGrid ---- */

static void test_grid_view_layout(void) {
	Zid gv = Zgrid_view_create(), root, grid;
	fill(gv, 10);
	grid = Zitems_control_get_items_panel(gv);
	Zitems_wrap_grid_set_item_width(grid, 100);
	Zitems_wrap_grid_set_item_height(grid, 90);
	root = make_root(gv, Z_ANIMATIONS_OFF);
	CHECK(Zitems_wrap_grid_get_orientation(grid) == Z_HORIZONTAL && Zitems_wrap_grid_get_maximum_rows_or_columns(grid) == -1);
	{
		/* 480 / 100: four to a row, rows 90 apart */
		ZRect a = Zelement_get_bounds(Zitems_control_get_container(gv, 0)), d = Zelement_get_bounds(Zitems_control_get_container(gv, 3));
		ZRect e = Zelement_get_bounds(Zitems_control_get_container(gv, 4));
		CHECK(a.width == 100 && a.height == 90 && d.x == 300 && d.y == 0);
		CHECK(e.x == 0 && e.y == 90);
	}
	Zitems_wrap_grid_set_maximum_rows_or_columns(grid, 3);
	Zroot_update_layout(root);
	{
		ZRect d = Zelement_get_bounds(Zitems_control_get_container(gv, 3));
		CHECK(d.x == 0 && d.y == 90);
	}
	/* without an ItemWidth the cells are the size of the first item */
	Zitems_wrap_grid_set_item_width(grid, Z_AUTO);
	Zitems_wrap_grid_set_item_height(grid, Z_AUTO);
	Zitems_wrap_grid_set_maximum_rows_or_columns(grid, -1);
	Zroot_update_layout(root);
	{
		ZRect a = Zelement_get_bounds(Zitems_control_get_container(gv, 0)), b = Zelement_get_bounds(Zitems_control_get_container(gv, 1));
		CHECK(a.width == b.width && b.x == a.width && a.width > 20);
	}
	/* a Vertical grid fills a column first and scrolls sideways */
	Zitems_wrap_grid_set_item_width(grid, 100);
	Zitems_wrap_grid_set_item_height(grid, 300);
	Zitems_wrap_grid_set_orientation(grid, Z_VERTICAL);
	Zroot_update_layout(root);
	{
		ZRect b = Zelement_get_bounds(Zitems_control_get_container(gv, 1)), c = Zelement_get_bounds(Zitems_control_get_container(gv, 2));
		ZRect d = Zelement_get_bounds(Zitems_control_get_container(gv, 3));
		Zid sv = Zitems_control_get_scroll_viewer(gv);
		CHECK(b.x == 0 && b.y == 300 && c.x == 100 && c.y == 0 && d.x == 100 && d.y == 300);
		CHECK(Zscroll_viewer_get_extent_width(sv) > Zscroll_viewer_get_viewport_width(sv));
		Zlist_view_scroll_into_view(gv, 9);
		CHECK(Zscroll_viewer_get_horizontal_offset(sv) > 0);
	}
	Zroot_destroy(root);
}

static void test_grid_view_look_and_selection(void) {
	Zid gv = Zgrid_view_create(), root, grid, a, b;
	ZRect r;
	int i;
	for (i = 0; i < 6; i++) {
		Zid t = Ztext_block_create("tile");
		Zitems_control_add(gv, t);
	}
	grid = Zitems_control_get_items_panel(gv);
	Zitems_wrap_grid_set_item_width(grid, 120);
	Zitems_wrap_grid_set_item_height(grid, 120);
	root = make_root(gv, Z_ANIMATIONS_OFF);
	a = Zitems_control_get_container(gv, 0);
	b = Zitems_control_get_container(gv, 1);
	CHECK(Zelement_is_a(a, Z_CLASS_GRID_VIEW_ITEM));
	/* Single: a tap selects and shows nothing */
	tap(root, center(a));
	CHECK(Zselector_get_selected_index(gv) == 0);
	r = Zelement_get_bounds(a);
	CHECK(pixel_is(root, (int)r.x + (int)r.width - 4, 4, 0, 0, 0));
	/* Multiple: the accent border and the earmark in the corner of what is selected */
	Zlist_view_set_selection_mode(gv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	tap(root, center(b));
	r = Zelement_get_bounds(b);
	CHECK(Zselector_item_get_is_selected(b));
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0x00, 0x50, 0xEF));
	CHECK(pixel_is(root, (int)(r.x + r.width) - 6, 24, 0x00, 0x50, 0xEF)); /* the earmark */
	CHECK(pixel_is(root, (int)r.x + 60, (int)r.y + 118, 0x00, 0x50, 0xEF));
	r = Zelement_get_bounds(Zitems_control_get_container(gv, 2));
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0, 0, 0)); /* not selected: no border */
	tap(root, center(b));
	r = Zelement_get_bounds(b);
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0, 0, 0));
	Zroot_destroy(root);
}

static void test_grid_view_groups(void) {
	Zid gv = Zgrid_view_create(), root, grid, header;
	header = Zlist_view_add_group_header(gv, "Group A");
	Zitems_control_add_text(gv, "a1");
	Zitems_control_add_text(gv, "a2");
	Zlist_view_add_group_header(gv, "Group B");
	Zitems_control_add_text(gv, "b1");
	CHECK(Zelement_is_a(header, Z_CLASS_GRID_VIEW_HEADER_ITEM));
	grid = Zitems_control_get_items_panel(gv);
	Zitems_wrap_grid_set_item_width(grid, 100);
	Zitems_wrap_grid_set_item_height(grid, 100);
	root = make_root(gv, Z_ANIMATIONS_OFF);
	{
		ZRect h = Zelement_get_bounds(header), a1 = Zelement_get_bounds(Zitems_control_get_container(gv, 0));
		ZRect a2 = Zelement_get_bounds(Zitems_control_get_container(gv, 1)), b1 = Zelement_get_bounds(Zitems_control_get_container(gv, 2));
		/* a header has a line of its own, as wide as the grid */
		CHECK(h.x == 0 && h.width == 480 && a1.y >= h.height && a1.x == 0);
		CHECK(a2.x == 100 && a2.y == a1.y);
		CHECK(b1.x == 0 && b1.y > a1.y + 100);
	}
	Zroot_destroy(root);
}


/* ---- ListBox ---- */

static void test_list_box_modes_and_look(void) {
	Zid lb = Zlist_box_create(), root;
	int indices[4];
	fill(lb, 4);
	root = make_root(lb, Z_ANIMATIONS_OFF);
	g_changes = 0;
	Zelement_on(lb, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(Zlist_box_get_selection_mode(lb) == Z_SELECTION_SINGLE);
	/* the box is PhoneBaseMediumHigh, 80% white: grey on the black page; a selected row is the accent */
	{
		ZRect r = Zelement_get_bounds(Zitems_control_get_container(lb, 3));
		CHECK(pixel_is(root, 470, (int)(Zelement_to_root(Zitems_control_get_container(lb, 3), (ZPoint){ 0, 0 }).y + r.height - 2), 0xCC, 0xCC, 0xCC));
	}
	tap(root, center(Zitems_control_get_container(lb, 1)));
	CHECK(Zselector_get_selected_index(lb) == 1 && g_changes == 1);
	{
		ZPoint p = Zelement_to_root(Zitems_control_get_container(lb, 1), (ZPoint){ 0, 0 });
		CHECK(pixel_is(root, 470, (int)p.y + 3, 0x00, 0x50, 0xEF));
	}
	/* pressed: a PhoneBaseMediumHigh rectangle over the box, brighter (the tilt shows only on the GPU) */
	{
		ZPoint p = center(Zitems_control_get_container(lb, 2));
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		CHECK(pixel_is(root, 470, (int)Zelement_to_root(Zitems_control_get_container(lb, 2), (ZPoint){ 0, 0 }).y + 3, 0xF5, 0xF5, 0xF5));
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
		CHECK(Zselector_get_selected_index(lb) == 2);
	}
	/* Multiple and Extended */
	Zlist_box_set_selection_mode(lb, Z_SELECTION_MULTIPLE);
	tap(root, center(Zitems_control_get_container(lb, 0)));
	tap(root, center(Zitems_control_get_container(lb, 3)));
	CHECK(Zlist_box_get_selected_indices(lb, indices, 4) == 3 && indices[0] == 0 && indices[1] == 2 && indices[2] == 3);
	Zlist_box_set_selection_mode(lb, Z_SELECTION_EXTENDED);
	tap(root, center(Zitems_control_get_container(lb, 1)));
	CHECK(Zlist_box_get_selected_indices(lb, indices, 4) == 1 && indices[0] == 1);
	/* the keyboard moves the selection in Single and Extended */
	key(root, Z_KEY_DOWN, 0);
	CHECK(Zselector_get_selected_index(lb) == 2);
	/* disabled: no accent, a tap does nothing */
	Zelement_set_enabled(lb, false);
	tap(root, center(Zitems_control_get_container(lb, 0)));
	CHECK(Zselector_get_selected_index(lb) == 2);
	Zroot_destroy(root);
}


/* The biggest difference in brightness between a pixel of the rectangle and its top left pixel: what a reader sees of the text in it. */
static int contrast_in(Zid root, int x0, int y0, int x1, int y1) {
	ZFrame f;
	int x, y, best = 0, base;
	const unsigned char* b;
	Zroot_render(root, &f);
	b = f.pixels + (size_t)y0 * (size_t)f.stride + (size_t)x0 * 4;
	base = (b[0] + b[1] + b[2]) / 3;
	for (y = y0; y < y1; y++) {
		for (x = x0; x < x1; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			int d = abs((px[0] + px[1] + px[2]) / 3 - base);
			if (d > best) best = d;
		}
	}
	return best;
}

/* The row of a container as a rectangle in the root: left of the text to a hundred px on. */
static void row_rect(Zid container, int* x0, int* y0, int* x1, int* y1) {
	ZRect r = Zelement_get_bounds(container);
	ZPoint p = Zelement_to_root(container, (ZPoint){ 0, 0 });
	*x0 = (int)p.x + 2;
	*y0 = (int)p.y + 1;
	*x1 = (int)p.x + 100;
	*y1 = (int)(p.y + r.height) - 1;
}

/* Light: the 8.1 dictionary has PhoneBaseBlack text on a PhoneBaseMediumHigh box that is black 75%; the library reads white on it. */
static void test_list_box_light_is_readable(void) {
	ZTheme themes[2] = { Z_THEME_LIGHT, Z_THEME_DARK };
	int t;
	for (t = 0; t < 2; t++) {
		Zid lb = Zlist_box_create(), root;
		int x0, y0, x1, y1;
		fill(lb, 4);
		root = make_root(lb, Z_ANIMATIONS_OFF);
		Zroot_set_theme(root, themes[t]);
		Zroot_update_layout(root);
		/* a row of the box */
		row_rect(Zitems_control_get_container(lb, 0), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 120);
		if (themes[t] == Z_THEME_LIGHT) {
			int r, g, b;
			test_pixel(root, 470, y0 + 2, &r, &g, &b);
			CHECK(r == 0x40 && g == 0x40 && b == 0x40);	/* black 75% over the white page */
		}
		/* selected: the text is on the accent */
		tap(root, center(Zitems_control_get_container(lb, 1)));
		row_rect(Zitems_control_get_container(lb, 1), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 120);
		/* pressed */
		{
			ZPoint p = center(Zitems_control_get_container(lb, 2));
			Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
			row_rect(Zitems_control_get_container(lb, 2), &x0, &y0, &x1, &y1);
			CHECK(contrast_in(root, x0, y0, x1, y1) > 100);
			Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
		}
		/* disabled: no box, the text is PhoneBaseMediumMid on the page */
		Zelement_set_enabled(lb, false);
		row_rect(Zitems_control_get_container(lb, 0), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 60);
		Zroot_destroy(root);
	}
}


/* ---- ComboBox ---- */

static void test_combo_box_8_1(void) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), cb, root;
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(cb, Z_VALIGN_TOP);
	Zcombo_box_set_header(cb, "Color");
	Zcombo_box_set_placeholder_text(cb, "pick one");
	fill(cb, 3);
	root = make_root(stack, Z_ANIMATIONS_OFF);
	CHECK_STR(Zcombo_box_get_header(cb), "Color");
	CHECK_STR(Zcombo_box_get_placeholder_text(cb), "pick one");
	CHECK(Zelement_is_a(Zitems_control_get_container(cb, 0), Z_CLASS_COMBO_BOX_ITEM));
	/* a header, the 9.5 of the touch target, a 38.5 outline, 9.5 */
	CHECK(Zelement_get_bounds(cb).height > 57.5 + 10);
	/* with a header off, the control is 57.5 high, like a Button */
	Zcombo_box_set_header(cb, NULL);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(cb).height, 57.5);
	{
		/* the outline, 2.5 in the foreground; the placeholder is drawn dim; the inside is empty */
		CHECK(pixel_is(root, 100, 9 + 1, 255, 255, 255));
		CHECK(pixel_is(root, 479 - 100, 9 + 38 - 1, 255, 255, 255));
		CHECK(pixel_is(root, 300, 9 + 19, 0, 0, 0));
		CHECK(lit(root, 8, 14, 100, 42) > 40);
	}
	/* a selected item shows in the box, brighter than the placeholder */
	{
		int before = lit(root, 8, 14, 100, 42);
		Zselector_set_selected_index(cb, 1);
		CHECK(lit(root, 8, 14, 100, 42) != before);
	}
	/* open: white with an accent border, the selected item in the accent */
	tap(root, center(cb));
	CHECK(Zcombo_box_get_is_drop_down_open(cb));
	Zroot_update_layout(root);
	{
		ZRect r = Zelement_get_bounds(Zitems_control_get_container(cb, 0));
		ZPoint p = Zelement_to_root(Zitems_control_get_container(cb, 1), (ZPoint){ 0, 0 });
		CHECK(r.width > 400);
		CHECK(pixel_is(root, 400, (int)p.y + 3, 255, 255, 255));
		CHECK(pixel_is(root, 1, (int)p.y + 10, 0x00, 0x50, 0xEF)); /* the border of the list */
	}
	/* Escape closes, the keys change the selection */
	key(root, Z_KEY_ESCAPE, 0);
	CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	CHECK(Zselector_get_selected_index(cb) == 1);
	Zelement_focus(cb);
	key(root, Z_KEY_DOWN, 0);
	CHECK(Zselector_get_selected_index(cb) == 2);
	key(root, Z_KEY_UP, 0);
	key(root, Z_KEY_UP, 0);
	CHECK(Zselector_get_selected_index(cb) == 0);
	key(root, Z_KEY_SPACE, 0);
	CHECK(Zcombo_box_get_is_drop_down_open(cb));
	key(root, Z_KEY_ENTER, 0);
	CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	/* MaxDropDownHeight limits the list, which then scrolls */
	Zcombo_box_set_max_drop_down_height(cb, 80);
	CHECK(Zcombo_box_get_max_drop_down_height(cb) == 80);
	Zcombo_box_set_is_drop_down_open(cb, true);
	Zroot_update_layout(root);
	CHECK(Zscroll_viewer_get_viewport_height(Zitems_control_get_scroll_viewer(cb)) <= 80.5);
	Zcombo_box_set_is_drop_down_open(cb, false);
	/* disabled: a tap does not open it */
	Zelement_set_enabled(cb, false);
	tap(root, center(cb));
	CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	Zroot_destroy(root);
}

static void test_combo_box_placement(void) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), cb, root;
	Zid spacer = Zpanel_add(stack, Zborder_create());
	Zelement_set_height(spacer, 300);
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(cb, Z_VALIGN_TOP);
	fill(cb, 4);
	Zselector_set_selected_index(cb, 2);
	root = make_root(stack, Z_ANIMATIONS_OFF);
	tap(root, center(cb));
	Zroot_update_layout(root);
	{
		/* the list opens over the box with the selected item at the place of the box: its top is 2.5 above the outline */
		ZPoint box = Zelement_to_root(cb, (ZPoint){ 0, 9.5f });
		ZPoint item = Zelement_to_root(Zitems_control_get_container(cb, 2), (ZPoint){ 0, 0 });
		CHECK_NEAR(item.y, box.y - 2.5);
		CHECK_NEAR(item.x, 2.5);
		CHECK(Zelement_get_bounds(Zitems_control_get_container(cb, 0)).width > 400);
	}
	/* a list that does not fit above the top of the screen is moved down, whole */
	Zcombo_box_set_is_drop_down_open(cb, false);
	Zelement_set_height(spacer, 0);
	Zroot_update_layout(root);
	Zcombo_box_set_is_drop_down_open(cb, true);
	Zroot_update_layout(root);
	CHECK(Zelement_to_root(Zitems_control_get_container(cb, 0), (ZPoint){ 0, 0 }).y >= 0);
	Zroot_destroy(root);
}

static int g_opened, g_closed;
static void on_opened(Zid sender, void* user) { (void)sender; (void)user; g_opened++; }
static void on_closed(Zid sender, void* user) { (void)sender; (void)user; g_closed++; }

static void test_combo_box_events(void) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), cb, root;
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(cb, Z_VALIGN_TOP);
	fill(cb, 3);
	root = make_root(stack, Z_ANIMATIONS_OFF);
	g_opened = g_closed = g_changes = 0;
	Zelement_on(cb, Z_EVENT_DROP_DOWN_OPENED, on_opened, NULL);
	Zelement_on(cb, Z_EVENT_DROP_DOWN_CLOSED, on_closed, NULL);
	Zelement_on(cb, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(cb));
	CHECK(g_opened == 1 && g_closed == 0);
	Zroot_update_layout(root);
	tap(root, center(Zitems_control_get_container(cb, 2)));
	CHECK(g_closed == 1 && g_changes == 1 && Zselector_get_selected_index(cb) == 2);
	/* the open list covers the box: a tap on the selected item chooses it again and closes */
	tap(root, center(cb));
	Zroot_update_layout(root);
	CHECK(g_opened == 2);
	tap(root, (ZPoint){ 450, 780 });
	CHECK(g_closed == 2 && g_changes == 1);
	Zcombo_box_set_is_drop_down_open(cb, true);
	Zcombo_box_set_is_drop_down_open(cb, true);
	CHECK(g_opened == 3);
	Zroot_destroy(root);
}


/* ---- FlipView ---- */

/* A finger from a point by dx in steps: what a step is in the time of the clock decides whether it is a flick, and the test has no clock. */
static void drag_steps(Zid root, ZPoint from, float dx, float step) {
	float x;
	int sign = dx < 0 ? -1 : 1;
	Zroot_inject_pointer(root, Z_POINTER_MOVE, from.x, from.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, from.x, from.y);
	for (x = 0; fabsf(x) <= fabsf(dx); x += (float)sign * step) Zroot_inject_pointer(root, Z_POINTER_MOVE, from.x + x, from.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, from.x + dx, from.y);
}

static void drag(Zid root, ZPoint from, float dx) { drag_steps(root, from, dx, 20); }

static void test_flip_view(void) {
	Zid fv = Zflip_view_create(), root;
	CHECK(Zselector_get_selected_index(fv) == -1);
	fill(fv, 4);
	/* the first item is chosen as soon as there is one */
	CHECK(Zselector_get_selected_index(fv) == 0);
	root = make_root(fv, Z_ANIMATIONS_OFF);
	g_changes = 0;
	Zelement_on(fv, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(Zelement_is_a(Zitems_control_get_container(fv, 0), Z_CLASS_FLIP_VIEW_ITEM));
	{
		/* the pages are the size of the view less the 2.5 margin, one page apart */
		ZRect a = Zelement_get_bounds(Zitems_control_get_container(fv, 0)), b = Zelement_get_bounds(Zitems_control_get_container(fv, 1));
		CHECK(a.x == 0 && a.width == 475 && a.height == 795 && b.x == 475);
		CHECK_NEAR(Zelement_to_root(Zitems_control_get_container(fv, 0), (ZPoint){ 0, 0 }).x, 2.5);
	}
	/* a drag of more than a fifth of a page turns to the next one */
	drag(root, (ZPoint){ 300, 400 }, -200);
	CHECK(Zselector_get_selected_index(fv) == 1 && g_changes == 1);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 1)).x == 0);
	/* a short one goes back */
	drag_steps(root, (ZPoint){ 300, 400 }, 40, 4);
	CHECK(Zselector_get_selected_index(fv) == 1 && g_changes == 1);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 1)).x == 0);
	/* to the right: the previous page */
	drag(root, (ZPoint){ 100, 400 }, 200);
	CHECK(Zselector_get_selected_index(fv) == 0 && g_changes == 2);
	/* the first page has nothing before it */
	drag(root, (ZPoint){ 100, 400 }, 300);
	CHECK(Zselector_get_selected_index(fv) == 0 && g_changes == 2);
	/* a mostly vertical drag is not the view's */
	drag(root, (ZPoint){ 300, 100 }, -5);
	CHECK(Zselector_get_selected_index(fv) == 0);
	/* the keys */
	Zelement_focus(fv);
	key(root, Z_KEY_RIGHT, 0);
	key(root, Z_KEY_RIGHT, 0);
	CHECK(Zselector_get_selected_index(fv) == 2);
	key(root, Z_KEY_LEFT, 0);
	CHECK(Zselector_get_selected_index(fv) == 1);
	key(root, Z_KEY_END, 0);
	CHECK(Zselector_get_selected_index(fv) == 3);
	key(root, Z_KEY_RIGHT, 0);
	CHECK(Zselector_get_selected_index(fv) == 3);
	key(root, Z_KEY_HOME, 0);
	CHECK(Zselector_get_selected_index(fv) == 0);
	/* by the program; -1 is refused while there are items */
	Zselector_set_selected_index(fv, 2);
	CHECK(Zselector_get_selected_index(fv) == 2);
	Zselector_set_selected_index(fv, -1);
	CHECK(Zselector_get_selected_index(fv) == 2);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 2)).x == 0);
	/* removing the item that is shown shows the one that takes its place; clearing leaves none */
	Zitems_control_remove_at(fv, 2);
	CHECK(Zselector_get_selected_index(fv) == 2 && Zitems_control_get_count(fv) == 3);
	Zitems_control_remove_at(fv, 2);
	CHECK(Zselector_get_selected_index(fv) == 1);
	Zitems_control_clear(fv);
	CHECK(Zselector_get_selected_index(fv) == -1);
	/* the picture of a page */
	Zitems_control_add_text(fv, "a page");
	Zroot_update_layout(root);
	Zcontrol_set_font_size(Zitems_control_get_container(fv, 0), 40);
	CHECK(lit(root, 0, 0, 480, 200) > 100);
	Zroot_destroy(root);
}

static void test_flip_view_slide(void) {
	Zid fv = Zflip_view_create(), root;
	fill(fv, 3);
	root = make_root(fv, Z_ANIMATIONS_ON);
	Zselector_set_selected_index(fv, 1);
	run(root, 64);
	Zroot_update_layout(root);
	{
		/* on its way: the first page has left a little, the second is coming */
		ZRect a = Zelement_get_bounds(Zitems_control_get_container(fv, 0)), b = Zelement_get_bounds(Zitems_control_get_container(fv, 1));
		CHECK(a.x < 0 && a.x > -475);
		CHECK(b.x > 0 && b.x < 475);
	}
	run(root, 400);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 1)).x == 0);
	/* UseTouchAnimationsForAllNavigation off: the program jumps */
	CHECK(Zflip_view_get_use_touch_animations_for_all_navigation(fv));
	Zflip_view_set_use_touch_animations_for_all_navigation(fv, false);
	Zselector_set_selected_index(fv, 2);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 2)).x == 0);
	/* a finger still slides it */
	drag(root, (ZPoint){ 100, 400 }, 200);
	CHECK(Zselector_get_selected_index(fv) == 1);
	run(root, 16);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 1)).x != 0);
	run(root, 400);
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(Zitems_control_get_container(fv, 1)).x == 0);
	Zroot_destroy(root);
}


/* ---- AutoSuggestBox ---- */

static const char* const FRUIT[] = { "apple", "apricot", "banana", "blueberry", "cherry" };
static ZAutoSuggestionBoxTextChangeReason g_reasons[16];
static int g_reason_count, g_chosen, g_query_count, g_query_chosen;
static char g_query[64];
static int g_order[16], g_order_count;

static void filter_into(Zid box) {
	const char* text = Zauto_suggest_box_get_text(box);
	const char* hits[5];
	int i, n = 0;
	for (i = 0; i < 5; i++) if (*text && strncmp(FRUIT[i], text, strlen(text)) == 0) hits[n++] = FRUIT[i];
	Zauto_suggest_box_set_items(box, hits, n);
}

static void asb_changed(Zid box, ZAutoSuggestionBoxTextChangeReason reason, void* user) {
	(void)user;
	if (g_reason_count < 16) g_reasons[g_reason_count++] = reason;
	if (g_order_count < 16) g_order[g_order_count++] = 1;
	if (reason == Z_SUGGEST_USER_INPUT) filter_into(box);
}

static void asb_chosen(Zid box, int index, void* user) {
	(void)box; (void)user;
	g_chosen = index;
	if (g_order_count < 16) g_order[g_order_count++] = 2;
}

static void asb_query(Zid box, const char* text, int chosen, void* user) {
	(void)box; (void)user;
	g_query_count++;
	g_query_chosen = chosen;
	snprintf(g_query, sizeof g_query, "%s", text);
	if (g_order_count < 16) g_order[g_order_count++] = 3;
}

static Zid asb_page(Zid* root) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), box = Zpanel_add(stack, Zauto_suggest_box_create());
	Zelement_set_vertical_alignment(box, Z_VALIGN_TOP);
	Zpanel_add(stack, Zbutton_create("next"));
	Zauto_suggest_box_on_text_changed(box, asb_changed, NULL);
	Zauto_suggest_box_on_suggestion_chosen(box, asb_chosen, NULL);
	Zauto_suggest_box_on_query_submitted(box, asb_query, NULL);
	*root = make_root(stack, Z_ANIMATIONS_OFF);
	g_reason_count = g_chosen = g_query_count = g_order_count = 0;
	g_query_chosen = -2;
	g_query[0] = 0;
	return box;
}

static void test_auto_suggest_box(void) {
	Zid root, box = asb_page(&root);
	Zauto_suggest_box_set_placeholder_text(box, "search");
	Zauto_suggest_box_set_header(box, "Fruit");
	CHECK_STR(Zauto_suggest_box_get_placeholder_text(box), "search");
	CHECK_STR(Zauto_suggest_box_get_header(box), "Fruit");
	/* a touch on the box gives its text the keyboard; every typed letter is a TextChanged, with the reason */
	tap(root, center(box));
	CHECK(Zroot_get_focus(root) != box && Zelement_get_class(Zroot_get_focus(root)) == Z_CLASS_TEXT_BOX);
	type(root, "a");
	CHECK(g_reason_count == 1 && g_reasons[0] == Z_SUGGEST_USER_INPUT);
	CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box) && Zitems_control_get_count(box) == 2);
	CHECK_STR(Zitems_control_get_text(box, 1), "apricot");
	Zroot_update_layout(root);
	/* the list hangs under the text box, white with an accent border */
	{
		ZRect tb = Zelement_get_bounds(Zelement_get_child(box, 0));
		ZPoint p = Zelement_to_root(Zitems_control_get_container(box, 0), (ZPoint){ 0, 0 });
		ZPoint q = Zelement_to_root(Zelement_get_child(box, 0), (ZPoint){ 0, tb.height });
		CHECK(p.y >= q.y && p.y < q.y + 20);
		CHECK(pixel_is(root, 400, (int)p.y + 3, 255, 255, 255));
		CHECK(pixel_is(root, 1, (int)p.y + 10, 0x00, 0x50, 0xEF));
	}
	type(root, "p");
	CHECK(g_reason_count == 2 && Zitems_control_get_count(box) == 2);
	type(root, "x");
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box) && Zitems_control_get_count(box) == 0);
	key(root, Z_KEY_BACKSPACE, 0);
	CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box));
	/* Down and Enter choose the first suggestion: SuggestionChosen, then TextChanged, then QuerySubmitted */
	g_reason_count = g_order_count = 0;
	key(root, Z_KEY_DOWN, 0);
	CHECK(g_chosen == 0 && g_query_count == 0 && Zauto_suggest_box_get_is_suggestion_list_open(box));
	key(root, Z_KEY_ENTER, 0);
	CHECK(g_chosen == 0 && g_query_count == 1 && g_query_chosen == 0);
	CHECK_STR(Zauto_suggest_box_get_text(box), "apple");
	CHECK_STR(g_query, "apple");
	CHECK(g_reason_count == 1 && g_reasons[0] == Z_SUGGEST_SUGGESTION_CHOSEN);
	CHECK(g_order_count == 3 && g_order[0] == 2 && g_order[1] == 1 && g_order[2] == 3);
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	/* Enter on a text that was not chosen: the query is the text */
	g_query_count = 0;
	key(root, Z_KEY_ENTER, 0);
	CHECK(g_query_count == 1 && g_query_chosen == -1);
	/* a tap on a suggestion */
	Zauto_suggest_box_set_text(box, "");
	type(root, "b");
	CHECK(Zitems_control_get_count(box) == 2);
	Zroot_update_layout(root);
	g_reason_count = 0;
	/* a finger on a suggestion does not close the list or take the keyboard from the text */
	{
		ZPoint p = center(Zitems_control_get_container(box, 1));
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box));
		CHECK(Zelement_get_class(Zroot_get_focus(root)) == Z_CLASS_TEXT_BOX);
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
	}
	CHECK_STR(Zauto_suggest_box_get_text(box), "blueberry");
	CHECK(g_chosen == 1 && g_query_chosen == 1 && g_reasons[g_reason_count - 1] == Z_SUGGEST_SUGGESTION_CHOSEN);
	CHECK(Zelement_get_class(Zroot_get_focus(root)) == Z_CLASS_TEXT_BOX); /* the text kept the keyboard */
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	/* Up and Down go round, Escape closes */
	Zauto_suggest_box_set_text(box, "");
	type(root, "a");
	key(root, Z_KEY_UP, 0);
	key(root, Z_KEY_ESCAPE, 0);
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	/* a tap outside closes the list */
	Zauto_suggest_box_set_text(box, "");
	type(root, "c");
	CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box));
	tap(root, (ZPoint){ 400, 700 });
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	Zroot_destroy(root);
}

static void test_auto_suggest_box_programmatic(void) {
	Zid root, box = asb_page(&root);
	/* a text that the program sets is a ProgrammaticChange, and opens nothing */
	Zauto_suggest_box_set_text(box, "ban");
	CHECK(g_reason_count == 1 && g_reasons[0] == Z_SUGGEST_PROGRAMMATIC_CHANGE);
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	CHECK_STR(Zauto_suggest_box_get_text(box), "ban");
	Zauto_suggest_box_set_text(box, "ban");
	CHECK(g_reason_count == 1); /* the same text: no change */
	/* suggestions given to a box that has no keyboard do not open it */
	Zauto_suggest_box_set_items(box, FRUIT, 5);
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box) && Zitems_control_get_count(box) == 5);
	Zauto_suggest_box_set_is_suggestion_list_open(box, true);
	CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box));
	Zauto_suggest_box_set_is_suggestion_list_open(box, false);
	/* UpdateTextOnSelect off: the text stays, the events come */
	Zauto_suggest_box_set_update_text_on_select(box, false);
	CHECK(Zelement_focus(box));
	CHECK(Zelement_get_class(Zroot_get_focus(root)) == Z_CLASS_TEXT_BOX);
	Zauto_suggest_box_set_is_suggestion_list_open(box, true);
	key(root, Z_KEY_DOWN, 0);
	key(root, Z_KEY_DOWN, 0);
	key(root, Z_KEY_ENTER, 0);
	CHECK(g_chosen == 1 && g_query_chosen == 1);
	CHECK_STR(Zauto_suggest_box_get_text(box), "ban");
	/* MaxSuggestionListHeight: the list is as tall as that and scrolls */
	Zauto_suggest_box_set_max_suggestion_list_height(box, 60);
	Zauto_suggest_box_set_is_suggestion_list_open(box, true);
	Zroot_update_layout(root);
	CHECK(Zscroll_viewer_get_viewport_height(Zitems_control_get_scroll_viewer(box)) <= 60);
	CHECK(Zscroll_viewer_get_extent_height(Zitems_control_get_scroll_viewer(box)) > 60);
	/* a drag on the list scrolls it and does not close it or choose anything */
	{
		Zid sv = Zitems_control_get_scroll_viewer(box);
		ZPoint p = center(Zitems_control_get_container(box, 0));
		int chosen_before = g_query_count;
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 20);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 45);
		CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box));
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y - 45);
		CHECK(Zscroll_viewer_get_vertical_offset(sv) > 20);
		CHECK(Zauto_suggest_box_get_is_suggestion_list_open(box) && g_query_count == chosen_before);
		CHECK(Zelement_get_class(Zroot_get_focus(root)) == Z_CLASS_TEXT_BOX);
	}
	/* Tab leaves the text and closes the list */
	Zroot_move_focus(root, true);
	CHECK(!Zauto_suggest_box_get_is_suggestion_list_open(box));
	Zroot_destroy(root);
}


/* ---- SemanticZoom ---- */

static int g_zoom_log[8], g_zoom_count;
static void zoom_started(Zid sender, int source_in, void* user) { (void)sender; (void)user; g_zoom_log[g_zoom_count++] = 10 + source_in; }
static void zoom_completed(Zid sender, int source_in, void* user) { (void)sender; (void)user; g_zoom_log[g_zoom_count++] = 20 + source_in; }

static void test_semantic_zoom(void) {
	Zid sz = Zsemantic_zoom_create(), in = Zlist_view_create(), out = Zgrid_view_create(), root;
	fill(in, 20);
	fill(out, 4);
	Zsemantic_zoom_set_zoomed_in_view(sz, in);
	Zsemantic_zoom_set_zoomed_out_view(sz, out);
	root = make_root(sz, Z_ANIMATIONS_OFF);
	CHECK(Zsemantic_zoom_get_is_zoomed_in_view_active(sz));
	CHECK(Zelement_get_visibility(in) == Z_VISIBLE && Zelement_get_visibility(out) == Z_COLLAPSED);
	CHECK(Zelement_get_bounds(in).width == 480 && Zelement_get_bounds(in).height == 800);
	Zsemantic_zoom_on_view_change_started(sz, zoom_started, NULL);
	Zsemantic_zoom_on_view_change_completed(sz, zoom_completed, NULL);
	g_zoom_count = 0;
	Zsemantic_zoom_toggle_active_view(sz);
	CHECK(!Zsemantic_zoom_get_is_zoomed_in_view_active(sz));
	CHECK(g_zoom_count == 2 && g_zoom_log[0] == 11 && g_zoom_log[1] == 21);
	Zroot_update_layout(root);
	CHECK(Zelement_get_visibility(in) == Z_COLLAPSED && Zelement_get_visibility(out) == Z_VISIBLE && Zelement_get_bounds(out).width == 480);
	/* the zoomed out view takes taps: a handler of its ItemClick goes back */
	tap(root, center(Zitems_control_get_container(out, 1)));
	CHECK(Zselector_get_selected_index(out) == 1);
	Zsemantic_zoom_set_is_zoomed_in_view_active(sz, true);
	CHECK(Zsemantic_zoom_get_is_zoomed_in_view_active(sz) && g_zoom_count == 4 && g_zoom_log[2] == 10 && g_zoom_log[3] == 20);
	/* the same view again: no change, no events */
	Zsemantic_zoom_set_is_zoomed_in_view_active(sz, true);
	CHECK(g_zoom_count == 4);
	/* CanChangeViews off keeps the view */
	Zsemantic_zoom_set_can_change_views(sz, false);
	Zsemantic_zoom_toggle_active_view(sz);
	CHECK(Zsemantic_zoom_get_is_zoomed_in_view_active(sz) && g_zoom_count == 4);
	Zsemantic_zoom_set_is_zoom_out_button_enabled(sz, false);
	Zroot_destroy(root);
}


/* ---- handlers that destroy the control ---- */

static void destroy_self(Zid sender, void* user) { (void)user; g_nav++; Zroot_set_visual(g_nav_root, Zstack_panel_create(Z_VERTICAL)); (void)sender; }
static void destroy_on_click(Zid sender, int index, void* user) { (void)index; destroy_self(sender, user); }
static void destroy_on_chosen(Zid sender, int index, void* user) { (void)index; destroy_self(sender, user); }
static void destroy_on_reason(Zid sender, ZAutoSuggestionBoxTextChangeReason reason, void* user) { (void)reason; destroy_self(sender, user); }
static void destroy_on_zoom(Zid sender, int source, void* user) { (void)source; destroy_self(sender, user); }

static void survives(Zid root) {
	Zroot_render(root, NULL);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 100, 100);
	Zroot_inject_pointer(root, Z_POINTER_UP, 100, 100);
	Zroot_inject_key(root, Z_KEY_DOWN, 0, 1);
	Zroot_inject_key(root, Z_KEY_DOWN, 0, 0);
	Zroot_inject_text(root, "x");
	Zroot_tick(root, 1000);
	Zroot_render(root, NULL);
}

static void test_handlers_that_destroy(void) {
	Zid root = Zroot_create(480, 800), c;
	g_nav_root = root;
	/* ItemClick of a ListView */
	c = Zlist_view_create();
	fill(c, 3);
	Zlist_view_set_is_item_click_enabled(c, true);
	Zlist_view_on_item_click(c, destroy_on_click, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Zitems_control_get_container(c, 1)));
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	/* SelectionChanged of a ListView and of a GridView */
	c = Zgrid_view_create();
	fill(c, 3);
	Zelement_on(c, Z_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Zitems_control_get_container(c, 1)));
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	/* SelectionChanged of a FlipView, from a drag */
	c = Zflip_view_create();
	fill(c, 3);
	Zelement_on(c, Z_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	g_nav = 0;
	drag(root, (ZPoint){ 300, 400 }, -200);
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	/* SuggestionChosen, and TextChanged of an AutoSuggestBox */
	c = Zauto_suggest_box_create();
	Zauto_suggest_box_on_suggestion_chosen(c, destroy_on_chosen, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	tap(root, center(c));
	Zauto_suggest_box_set_items(c, FRUIT, 3);
	g_nav = 0;
	Zroot_update_layout(root);
	tap(root, center(Zitems_control_get_container(c, 0)));
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	c = Zauto_suggest_box_create();
	Zauto_suggest_box_on_text_changed(c, destroy_on_reason, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	tap(root, center(c));
	g_nav = 0;
	type(root, "ab");
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	/* ViewChangeStarted of a SemanticZoom */
	c = Zsemantic_zoom_create();
	Zsemantic_zoom_set_zoomed_in_view(c, Zlist_view_create());
	Zsemantic_zoom_on_view_change_started(c, destroy_on_zoom, NULL);
	Zroot_set_visual(root, c);
	g_nav = 0;
	Zsemantic_zoom_toggle_active_view(c);
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	/* the drop-down of a ComboBox closed by an item whose SelectionChanged destroys the page */
	c = Zcombo_box_create();
	fill(c, 3);
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zelement_on(c, Z_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Zroot_set_visual(root, c);
	Zroot_update_layout(root);
	tap(root, center(c));
	Zroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Zitems_control_get_container(c, 2)));
	CHECK(g_nav == 1 && !Zelement_is_valid(c));
	survives(root);
	Zroot_destroy(root);
}


/* ---- pictures ---- */

static void test_picture(void) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), lv, gv, cb, box, grid;
	Zid root = Zroot_create(480, 800);
	char path[512];
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zelement_set_margin(stack, Zthickness(12, 0, 12, 0));
	Zroot_set_visual(root, stack);
	lv = Zpanel_add(stack, Zlist_view_create());
	Zlist_view_set_header(lv, "ListView, Multiple");
	Zlist_view_set_selection_mode(lv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	fill(lv, 3);
	Zelement_set_height(lv, 150);
	Zselector_item_set_is_selected(Zitems_control_get_container(lv, 1), true);
	gv = Zpanel_add(stack, Zgrid_view_create());
	Zlist_view_set_selection_mode(gv, Z_LIST_VIEW_SELECTION_MULTIPLE);
	fill(gv, 5);
	grid = Zitems_control_get_items_panel(gv);
	Zitems_wrap_grid_set_item_width(grid, 80);
	Zitems_wrap_grid_set_item_height(grid, 60);
	Zelement_set_height(gv, 140);
	Zselector_item_set_is_selected(Zitems_control_get_container(gv, 2), true);
	Zpanel_add(stack, Zlist_box_create());
	fill(Zelement_get_child(stack, 2), 2);
	Zselector_set_selected_index(Zelement_get_child(stack, 2), 0);
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zcombo_box_set_header(cb, "ComboBox");
	Zcombo_box_set_placeholder_text(cb, "pick a color");
	fill(cb, 3);
	box = Zpanel_add(stack, Zauto_suggest_box_create());
	Zauto_suggest_box_set_header(box, "AutoSuggestBox");
	Zauto_suggest_box_set_placeholder_text(box, "search fruit");
	Zroot_update_layout(root);
	snprintf(path, sizeof path, "%s/items.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zelement_focus(box);
	Zroot_inject_text(root, "a");
	Zauto_suggest_box_set_items(box, FRUIT, 2);
	snprintf(path, sizeof path, "%s/items-suggest.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
	RUN(test_classes);
	RUN(test_list_view_single_selection);
	RUN(test_list_view_none_and_item_click);
	RUN(test_list_view_multiple_and_extended);
	RUN(test_list_view_look);
	RUN(test_list_view_header_footer_and_scroll);
	RUN(test_list_view_keyboard);
	RUN(test_items_panels_and_groups);
	RUN(test_grid_view_layout);
	RUN(test_grid_view_look_and_selection);
	RUN(test_grid_view_groups);
	RUN(test_list_box_modes_and_look);
	RUN(test_list_box_light_is_readable);
	RUN(test_combo_box_8_1);
	RUN(test_combo_box_placement);
	RUN(test_combo_box_events);
	RUN(test_flip_view);
	RUN(test_flip_view_slide);
	RUN(test_auto_suggest_box);
	RUN(test_auto_suggest_box_programmatic);
	RUN(test_semantic_zoom);
	RUN(test_handlers_that_destroy);
	RUN(test_picture);
	Zshutdown();
	return test_finish("test_items");
}
