/**
 * The items controls of Windows Phone 8.1: ListView, GridView, ListBox, ComboBox, FlipView,
 * AutoSuggestBox and SemanticZoom. Every check goes through what a finger or a key would do (taps through
 * Kroot_inject_pointer, keys, the clock), or reads a pixel of the rendered page (Dark theme, Cobalt accent).
 */

#include "test.h"

static int g_changes, g_clicks, g_clicked, g_nav;
static Kid g_nav_root;

static void on_change(Kid sender, void* user) { (void)sender; (void)user; g_changes++; }
static void on_item_click(Kid sender, int index, void* user) { (void)sender; (void)user; g_clicks++; g_clicked = index; }

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

static void type(Kid root, const char* text) { Kroot_inject_text(root, text); }
static void key(Kid root, KKey k, int mods) { Kroot_inject_key(root, k, mods, 1); Kroot_inject_key(root, k, mods, 0); }

static void run(Kid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static Kid make_root(Kid visual, KAnimations animations) {
	Kid root = Kroot_create(480, 800);
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, animations);
	Kroot_set_visual(root, visual);
	Kroot_update_layout(root);
	return root;
}

static void fill(Kid control, int count) {
	char name[32];
	int i;
	for (i = 0; i < count; i++) {
		snprintf(name, sizeof name, "item %d", i);
		Kitems_control_add_text(control, name);
	}
}

static int pixel_is(Kid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

static int lit(Kid root, int x0, int y0, int x1, int y1) {
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


/* ---- hierarchy ---- */

static void quiet(KError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Kid lv = Klist_view_create(), gv = Kgrid_view_create(), item = Klist_view_item_create("x"), fv = Kflip_view_create();
	Kid asb = Kauto_suggest_box_create(), cb = Kcombo_box_create(), sz = Ksemantic_zoom_create();
	CHECK(Kelement_is_a(lv, K_CLASS_LIST_VIEW_BASE) && Kelement_is_a(lv, K_CLASS_SELECTOR) && Kelement_is_a(lv, K_CLASS_ITEMS_CONTROL));
	CHECK(Kelement_is_a(gv, K_CLASS_LIST_VIEW_BASE) && !Kelement_is_a(gv, K_CLASS_LIST_VIEW));
	CHECK(Kelement_get_class(lv) == K_CLASS_LIST_VIEW && Kelement_get_class(gv) == K_CLASS_GRID_VIEW);
	CHECK(Kelement_is_a(item, K_CLASS_SELECTOR_ITEM) && Kelement_is_a(item, K_CLASS_CONTENT_CONTROL));
	CHECK(Kelement_is_a(fv, K_CLASS_SELECTOR) && Kelement_is_a(cb, K_CLASS_SELECTOR));
	CHECK(Kelement_is_a(asb, K_CLASS_ITEMS_CONTROL) && !Kelement_is_a(asb, K_CLASS_SELECTOR) && !Kelement_is_a(asb, K_CLASS_TEXT_BOX));
	CHECK(Kelement_is_a(sz, K_CLASS_CONTROL));
	CHECK_STR(Kelement_get_class_name(gv), "GridView");
	CHECK_STR(Kelement_get_class_name(item), "ListViewItem");
	CHECK_STR(Kelement_get_class_name(Kitems_control_get_items_panel(lv)), "ItemsStackPanel");
	CHECK_STR(Kelement_get_class_name(Kitems_control_get_items_panel(gv)), "ItemsWrapGrid");
	Kset_error_callback(quiet, NULL);
	CHECK(Kelement_create(K_CLASS_LIST_VIEW_BASE) == K_INVALID_ID); /* abstract */
	CHECK(Kelement_create(K_CLASS_SELECTOR_ITEM) == K_INVALID_ID);
	Kset_error_callback(NULL, NULL);
	CHECK(Kelement_is_a(Kelement_create(K_CLASS_GRID_VIEW_HEADER_ITEM), K_CLASS_LIST_VIEW_BASE_HEADER_ITEM));
	Kelement_destroy(lv);
	Kelement_destroy(gv);
	Kelement_destroy(item);
	Kelement_destroy(fv);
	Kelement_destroy(asb);
	Kelement_destroy(cb);
	Kelement_destroy(sz);
}


/* ---- ListView ---- */

static void test_list_view_single_selection(void) {
	Kid lv = Klist_view_create(), root;
	int indices[4];
	fill(lv, 5);
	root = make_root(lv, K_ANIMATIONS_OFF);
	CHECK(Klist_view_get_selection_mode(lv) == K_LIST_VIEW_SELECTION_SINGLE);
	g_changes = 0;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Kitems_control_get_container(lv, 2)));
	CHECK(Kselector_get_selected_index(lv) == 2 && g_changes == 1);
	CHECK(Kselector_item_get_is_selected(Kitems_control_get_container(lv, 2)));
	tap(root, center(Kitems_control_get_container(lv, 2)));
	CHECK(Kselector_get_selected_index(lv) == 2 && g_changes == 1); /* the same item: no change */
	tap(root, center(Kitems_control_get_container(lv, 4)));
	CHECK(Kselector_get_selected_index(lv) == 4 && g_changes == 2);
	CHECK(!Kselector_item_get_is_selected(Kitems_control_get_container(lv, 2)));
	CHECK(Klist_view_get_selected_indices(lv, indices, 4) == 1 && indices[0] == 4);
	/* by the program, and SelectedItem is the container of the item */
	Kselector_set_selected_index(lv, 1);
	CHECK(Kselector_get_selected_container(lv) == Kitems_control_get_container(lv, 1) && g_changes == 3);
	Kselector_set_selected_index(lv, -1);
	CHECK(Kselector_get_selected_index(lv) == -1 && g_changes == 4);
	/* select all is for the multiple modes */
	Klist_view_select_all(lv);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 0);
	/* an index past the end selects nothing */
	Kselector_set_selected_index(lv, 99);
	CHECK(Kselector_get_selected_index(lv) == -1);
	Kroot_destroy(root);
}

static void test_list_view_none_and_item_click(void) {
	Kid lv = Klist_view_create(), root;
	fill(lv, 4);
	root = make_root(lv, K_ANIMATIONS_OFF);
	g_changes = g_clicks = 0;
	g_clicked = -1;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	Klist_view_on_item_click(lv, on_item_click, NULL);
	/* IsItemClickEnabled is off at first: a tap selects and nothing is clicked */
	CHECK(!Klist_view_get_is_item_click_enabled(lv));
	tap(root, center(Kitems_control_get_container(lv, 1)));
	CHECK(g_clicks == 0 && Kselector_get_selected_index(lv) == 1 && g_changes == 1);
	Klist_view_set_is_item_click_enabled(lv, K_TRUE);
	tap(root, center(Kitems_control_get_container(lv, 3)));
	CHECK(g_clicks == 1 && g_clicked == 3 && Kselector_get_selected_index(lv) == 3);
	/* None: the selection that was is gone, and a tap is only an ItemClick */
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_NONE);
	CHECK(Kselector_get_selected_index(lv) == -1 && g_changes == 3);
	tap(root, center(Kitems_control_get_container(lv, 0)));
	CHECK(g_clicks == 2 && g_clicked == 0 && Kselector_get_selected_index(lv) == -1 && g_changes == 3);
	Kselector_set_selected_index(lv, 2);
	CHECK(Kselector_get_selected_index(lv) == -1);
	/* a disabled item does not click */
	Kelement_set_enabled(Kitems_control_get_container(lv, 1), K_FALSE);
	tap(root, center(Kitems_control_get_container(lv, 1)));
	CHECK(g_clicks == 2);
	/* a press that slides off the item is not a tap */
	{
		KPoint p = center(Kitems_control_get_container(lv, 2));
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, 470, 790);
		Kroot_inject_pointer(root, K_POINTER_UP, 470, 790);
		CHECK(g_clicks == 2);
	}
	Klist_view_on_item_click(lv, NULL, NULL);
	tap(root, center(Kitems_control_get_container(lv, 0)));
	CHECK(g_clicks == 2);
	Kroot_destroy(root);
}

static void test_list_view_multiple_and_extended(void) {
	Kid lv = Klist_view_create(), root;
	int indices[8];
	fill(lv, 6);
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_MULTIPLE);
	root = make_root(lv, K_ANIMATIONS_OFF);
	g_changes = 0;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Kitems_control_get_container(lv, 1)));
	tap(root, center(Kitems_control_get_container(lv, 3)));
	CHECK(Klist_view_get_selected_indices(lv, indices, 8) == 2 && indices[0] == 1 && indices[1] == 3 && g_changes == 2);
	CHECK(Kselector_get_selected_index(lv) == 1);
	tap(root, center(Kitems_control_get_container(lv, 1)));
	CHECK(Klist_view_get_selected_indices(lv, indices, 8) == 1 && indices[0] == 3 && g_changes == 3);
	Klist_view_select_all(lv);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 6);
	/* the keyboard: Ctrl+A on a view that has it */
	Kselector_set_selected_index(lv, -1);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 0);
	Kelement_focus(lv);
	key(root, K_KEY_A, K_MOD_CTRL);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 6);
	/* Extended: a tap selects that item alone */
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_EXTENDED);
	tap(root, center(Kitems_control_get_container(lv, 2)));
	CHECK(Klist_view_get_selected_indices(lv, indices, 8) == 1 && indices[0] == 2);
	/* back to Single keeps one */
	Klist_view_select_all(lv);
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_SINGLE);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 1);
	Kroot_destroy(root);
}

static void test_list_view_look(void) {
	Kid lv = Klist_view_create(), root, item;
	KRect r;
	int y;
	fill(lv, 3);
	root = make_root(lv, K_ANIMATIONS_OFF);
	item = Kitems_control_get_container(lv, 0);
	r = Kelement_get_bounds(item);
	/* the text is 12 px: a row is a line and nothing else (ListViewItem has no padding) */
	CHECK(r.height > 12 && r.height < 24 && r.x == 0);
	Kselector_set_selected_index(lv, 0);
	/* Single selection draws nothing: no check box, no accent */
	y = (int)r.y;
	CHECK(lit(root, 100, y, 480, y + (int)r.height) == 0);
	/* Multiple: the 25.5 box with the 2.5 outline, and the content moves 40.5 to the right */
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_MULTIPLE);
	Kselector_set_selected_index(lv, -1);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(Kitems_control_get_container(lv, 1));
	y = (int)r.y;
	CHECK(pixel_is(root, 1, y + 22, 255, 255, 255)); /* the left side of the outline */
	CHECK(pixel_is(root, 12, y + 22, 0, 0, 0));      /* hollow */
	CHECK(pixel_is(root, 12, y + 28, 0, 0, 0));
	CHECK(lit(root, 0, y, 40, y + 40) > 100);
	CHECK(Kelement_get_bounds(Kcontent_control_get_content(Kitems_control_get_container(lv, 1))).x >= 40.4f);
	tap(root, center(Kitems_control_get_container(lv, 1)));
	CHECK(pixel_is(root, 11, y + 28, 255, 255, 255)); /* the check mark */
	/* a disabled item is drawn at 40%: its box is dimmer */
	Kelement_set_enabled(Kitems_control_get_container(lv, 2), K_FALSE);
	r = Kelement_get_bounds(Kitems_control_get_container(lv, 2));
	{
		int pr, pg, pb;
		test_pixel(root, 1, (int)r.y + 22, &pr, &pg, &pb);
		CHECK(pr > 60 && pr < 140);
	}
	Kroot_destroy(root);
}

static void test_list_view_header_footer_and_scroll(void) {
	Kid lv = Klist_view_create(), root, sv, header;
	double extent_without;
	fill(lv, 200);
	root = make_root(lv, K_ANIMATIONS_OFF);
	sv = Kitems_control_get_scroll_viewer(lv);
	CHECK(Kelement_is_a(sv, K_CLASS_SCROLL_VIEWER));
	extent_without = Kscroll_viewer_get_extent_height(sv);
	CHECK(extent_without > 800);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(lv, 0)).y == 0);
	/* a string header and a string footer scroll with the items */
	Klist_view_set_header(lv, "Header text");
	Klist_view_set_footer(lv, "Footer text");
	Kroot_update_layout(root);
	CHECK(Kscroll_viewer_get_extent_height(sv) > extent_without + 30);
	{
		KPoint p = Kelement_to_root(Kitems_control_get_container(lv, 0), (KPoint){ 0, 0 });
		CHECK(p.y > 15);
		CHECK(lit(root, 0, 0, 200, (int)p.y) > 50); /* the header was drawn */
	}
	/* an element as header replaces the string */
	header = Kbutton_create("a button");
	Klist_view_set_header_content(lv, header);
	Kroot_update_layout(root);
	CHECK(Kelement_get_parent(header) != K_INVALID_ID && Kelement_get_root(header) == root);
	{
		KPoint p = Kelement_to_root(Kitems_control_get_container(lv, 0), (KPoint){ 0, 0 });
		CHECK(p.y >= Kelement_get_bounds(header).height);
	}
	Klist_view_set_header(lv, NULL);
	Klist_view_set_footer_content(lv, K_INVALID_ID);
	Kroot_update_layout(root);
	CHECK_NEAR(Kscroll_viewer_get_extent_height(sv), extent_without);
	/* a drag scrolls and selects nothing, the wheel scrolls, ScrollIntoView brings the item */
	g_changes = 0;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	{
		KPoint p = center(Kitems_control_get_container(lv, 10));
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 30);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 200);
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y - 200);
		CHECK_NEAR(Kscroll_viewer_get_vertical_offset(sv), 200);
		CHECK(g_changes == 0 && Kselector_get_selected_index(lv) == -1);
	}
	Kscroll_viewer_scroll_to_vertical_offset(sv, 0);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 200, 200);
	Kroot_inject_wheel(root, -1);
	CHECK(Kscroll_viewer_get_vertical_offset(sv) > 0);
	Kscroll_viewer_scroll_to_vertical_offset(sv, 0);
	Klist_view_scroll_into_view(lv, 199);
	CHECK_NEAR(Kscroll_viewer_get_vertical_offset(sv), Kscroll_viewer_get_extent_height(sv) - Kscroll_viewer_get_viewport_height(sv));
	Klist_view_scroll_into_view_with_alignment(lv, 20, K_SCROLL_INTO_VIEW_LEADING);
	Kroot_update_layout(root);
	{
		KPoint p = Kelement_to_root(Kitems_control_get_container(lv, 20), (KPoint){ 0, 0 });
		CHECK_NEAR(p.y, 0);
	}
	/* an item that is in view does not move the view */
	{
		double before = Kscroll_viewer_get_vertical_offset(sv);
		Klist_view_scroll_into_view(lv, 21);
		CHECK(Kscroll_viewer_get_vertical_offset(sv) == before);
	}
	Kroot_destroy(root);
}

static void test_list_view_keyboard(void) {
	Kid lv = Klist_view_create(), root;
	fill(lv, 5);
	Klist_view_set_is_item_click_enabled(lv, K_TRUE);
	root = make_root(lv, K_ANIMATIONS_OFF);
	g_changes = g_clicks = 0;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	Klist_view_on_item_click(lv, on_item_click, NULL);
	/* a touch gives the list the keyboard; Down selects the first item */
	tap(root, center(Kitems_control_get_container(lv, 4)));
	CHECK(Kroot_get_focus(root) == lv && Kselector_get_selected_index(lv) == 4);
	key(root, K_KEY_UP, 0);
	CHECK(Kselector_get_selected_index(lv) == 3);
	key(root, K_KEY_HOME, 0);
	CHECK(Kselector_get_selected_index(lv) == 0);
	key(root, K_KEY_DOWN, 0);
	key(root, K_KEY_DOWN, 0);
	CHECK(Kselector_get_selected_index(lv) == 2);
	key(root, K_KEY_END, 0);
	CHECK(Kselector_get_selected_index(lv) == 4);
	key(root, K_KEY_DOWN, 0); /* the end of the list stays */
	CHECK(Kselector_get_selected_index(lv) == 4);
	/* Space and Enter are a tap */
	g_clicks = 0;
	key(root, K_KEY_SPACE, 0);
	CHECK(g_clicks == 1 && g_clicked == 4);
	key(root, K_KEY_ENTER, 0);
	CHECK(g_clicks == 2);
	/* in Multiple the arrows only move, Space toggles */
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_MULTIPLE);
	Kselector_set_selected_index(lv, -1);
	key(root, K_KEY_HOME, 0);
	CHECK(Klist_view_get_selected_indices(lv, NULL, 0) == 0);
	key(root, K_KEY_SPACE, 0);
	key(root, K_KEY_DOWN, 0);
	key(root, K_KEY_DOWN, 0);
	key(root, K_KEY_SPACE, 0);
	{
		int indices[4];
		CHECK(Klist_view_get_selected_indices(lv, indices, 4) == 2 && indices[0] == 0 && indices[1] == 2);
	}
	Kroot_destroy(root);
}

static void test_items_panels_and_groups(void) {
	Kid lv = Klist_view_create(), root, panel, h1, h2;
	fill(lv, 3);
	root = make_root(lv, K_ANIMATIONS_OFF);
	/* a horizontal panel: the items go side by side and the view scrolls sideways */
	panel = Kitems_stack_panel_create(K_HORIZONTAL);
	Kitems_control_set_items_panel(lv, panel);
	CHECK(Kitems_control_get_items_panel(lv) == panel && Kitems_control_get_count(lv) == 3);
	Kroot_update_layout(root);
	{
		KRect a = Kelement_get_bounds(Kitems_control_get_container(lv, 0)), b = Kelement_get_bounds(Kitems_control_get_container(lv, 1));
		CHECK(b.x > a.x + a.width - 1 && b.y == a.y);
	}
	Kroot_destroy(root);
	/* a vertical panel again, with groups */
	lv = Klist_view_create();
	h1 = Klist_view_add_group_header(lv, "Fruit");
	Kitems_control_add_text(lv, "apple");
	Kitems_control_add_text(lv, "banana");
	h2 = Klist_view_add_group_header(lv, "Vegetables");
	Kitems_control_add_text(lv, "carrot");
	CHECK(Kelement_is_a(h1, K_CLASS_LIST_VIEW_HEADER_ITEM) && Klist_view_get_group_count(lv) == 2);
	CHECK(Kitems_control_get_count(lv) == 3);
	CHECK_STR(Kitems_control_get_text(lv, 2), "carrot");
	CHECK(Kitems_control_index_from_container(lv, Kitems_control_get_container(lv, 2)) == 2);
	CHECK(Kitems_control_index_from_container(lv, h2) == -1);
	root = make_root(lv, K_ANIMATIONS_OFF);
	g_changes = 0;
	Kelement_on(lv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Kitems_control_get_container(lv, 2)));
	CHECK(Kselector_get_selected_index(lv) == 2);
	/* a tap on a header selects nothing */
	tap(root, center(h2));
	CHECK(Kselector_get_selected_index(lv) == 2 && g_changes == 1);
	/* the header is above its items */
	CHECK(Kelement_get_bounds(h1).y < Kelement_get_bounds(Kitems_control_get_container(lv, 0)).y);
	CHECK(Kelement_get_bounds(h2).y > Kelement_get_bounds(Kitems_control_get_container(lv, 1)).y);
	Kitems_control_remove_at(lv, 0);
	CHECK(Kitems_control_get_count(lv) == 2 && Kselector_get_selected_index(lv) == 1);
	Kitems_control_clear(lv);
	CHECK(Kitems_control_get_count(lv) == 0 && Klist_view_get_group_count(lv) == 0);
	Kroot_destroy(root);
}


/* ---- GridView and ItemsWrapGrid ---- */

static void test_grid_view_layout(void) {
	Kid gv = Kgrid_view_create(), root, grid;
	fill(gv, 10);
	grid = Kitems_control_get_items_panel(gv);
	Kitems_wrap_grid_set_item_width(grid, 100);
	Kitems_wrap_grid_set_item_height(grid, 90);
	root = make_root(gv, K_ANIMATIONS_OFF);
	CHECK(Kitems_wrap_grid_get_orientation(grid) == K_HORIZONTAL && Kitems_wrap_grid_get_maximum_rows_or_columns(grid) == -1);
	{
		/* 480 / 100: four to a row, rows 90 apart */
		KRect a = Kelement_get_bounds(Kitems_control_get_container(gv, 0)), d = Kelement_get_bounds(Kitems_control_get_container(gv, 3));
		KRect e = Kelement_get_bounds(Kitems_control_get_container(gv, 4));
		CHECK(a.width == 100 && a.height == 90 && d.x == 300 && d.y == 0);
		CHECK(e.x == 0 && e.y == 90);
	}
	Kitems_wrap_grid_set_maximum_rows_or_columns(grid, 3);
	Kroot_update_layout(root);
	{
		KRect d = Kelement_get_bounds(Kitems_control_get_container(gv, 3));
		CHECK(d.x == 0 && d.y == 90);
	}
	/* without an ItemWidth the cells are the size of the first item */
	Kitems_wrap_grid_set_item_width(grid, K_AUTO);
	Kitems_wrap_grid_set_item_height(grid, K_AUTO);
	Kitems_wrap_grid_set_maximum_rows_or_columns(grid, -1);
	Kroot_update_layout(root);
	{
		KRect a = Kelement_get_bounds(Kitems_control_get_container(gv, 0)), b = Kelement_get_bounds(Kitems_control_get_container(gv, 1));
		CHECK(a.width == b.width && b.x == a.width && a.width > 20);
	}
	/* a Vertical grid fills a column first and scrolls sideways */
	Kitems_wrap_grid_set_item_width(grid, 100);
	Kitems_wrap_grid_set_item_height(grid, 300);
	Kitems_wrap_grid_set_orientation(grid, K_VERTICAL);
	Kroot_update_layout(root);
	{
		KRect b = Kelement_get_bounds(Kitems_control_get_container(gv, 1)), c = Kelement_get_bounds(Kitems_control_get_container(gv, 2));
		KRect d = Kelement_get_bounds(Kitems_control_get_container(gv, 3));
		Kid sv = Kitems_control_get_scroll_viewer(gv);
		CHECK(b.x == 0 && b.y == 300 && c.x == 100 && c.y == 0 && d.x == 100 && d.y == 300);
		CHECK(Kscroll_viewer_get_extent_width(sv) > Kscroll_viewer_get_viewport_width(sv));
		Klist_view_scroll_into_view(gv, 9);
		CHECK(Kscroll_viewer_get_horizontal_offset(sv) > 0);
	}
	Kroot_destroy(root);
}

static void test_grid_view_look_and_selection(void) {
	Kid gv = Kgrid_view_create(), root, grid, a, b;
	KRect r;
	int i;
	for (i = 0; i < 6; i++) {
		Kid t = Ktext_block_create("tile");
		Kitems_control_add(gv, t);
	}
	grid = Kitems_control_get_items_panel(gv);
	Kitems_wrap_grid_set_item_width(grid, 120);
	Kitems_wrap_grid_set_item_height(grid, 120);
	root = make_root(gv, K_ANIMATIONS_OFF);
	a = Kitems_control_get_container(gv, 0);
	b = Kitems_control_get_container(gv, 1);
	CHECK(Kelement_is_a(a, K_CLASS_GRID_VIEW_ITEM));
	/* Single: a tap selects and shows nothing */
	tap(root, center(a));
	CHECK(Kselector_get_selected_index(gv) == 0);
	r = Kelement_get_bounds(a);
	CHECK(pixel_is(root, (int)r.x + (int)r.width - 4, 4, 0, 0, 0));
	/* Multiple: the accent border and the earmark in the corner of what is selected */
	Klist_view_set_selection_mode(gv, K_LIST_VIEW_SELECTION_MULTIPLE);
	tap(root, center(b));
	r = Kelement_get_bounds(b);
	CHECK(Kselector_item_get_is_selected(b));
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0x00, 0x50, 0xEF));
	CHECK(pixel_is(root, (int)(r.x + r.width) - 6, 24, 0x00, 0x50, 0xEF)); /* the earmark */
	CHECK(pixel_is(root, (int)r.x + 60, (int)r.y + 118, 0x00, 0x50, 0xEF));
	r = Kelement_get_bounds(Kitems_control_get_container(gv, 2));
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0, 0, 0)); /* not selected: no border */
	tap(root, center(b));
	r = Kelement_get_bounds(b);
	CHECK(pixel_is(root, (int)r.x + 1, (int)r.y + 60, 0, 0, 0));
	Kroot_destroy(root);
}

static void test_grid_view_groups(void) {
	Kid gv = Kgrid_view_create(), root, grid, header;
	header = Klist_view_add_group_header(gv, "Group A");
	Kitems_control_add_text(gv, "a1");
	Kitems_control_add_text(gv, "a2");
	Klist_view_add_group_header(gv, "Group B");
	Kitems_control_add_text(gv, "b1");
	CHECK(Kelement_is_a(header, K_CLASS_GRID_VIEW_HEADER_ITEM));
	grid = Kitems_control_get_items_panel(gv);
	Kitems_wrap_grid_set_item_width(grid, 100);
	Kitems_wrap_grid_set_item_height(grid, 100);
	root = make_root(gv, K_ANIMATIONS_OFF);
	{
		KRect h = Kelement_get_bounds(header), a1 = Kelement_get_bounds(Kitems_control_get_container(gv, 0));
		KRect a2 = Kelement_get_bounds(Kitems_control_get_container(gv, 1)), b1 = Kelement_get_bounds(Kitems_control_get_container(gv, 2));
		/* a header has a line of its own, as wide as the grid */
		CHECK(h.x == 0 && h.width == 480 && a1.y >= h.height && a1.x == 0);
		CHECK(a2.x == 100 && a2.y == a1.y);
		CHECK(b1.x == 0 && b1.y > a1.y + 100);
	}
	Kroot_destroy(root);
}


/* ---- ListBox ---- */

static void test_list_box_modes_and_look(void) {
	Kid lb = Klist_box_create(), root;
	int indices[4];
	fill(lb, 4);
	root = make_root(lb, K_ANIMATIONS_OFF);
	g_changes = 0;
	Kelement_on(lb, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(Klist_box_get_selection_mode(lb) == K_SELECTION_SINGLE);
	/* the box is PhoneBaseMediumHigh, 80% white: grey on the black page; a selected row is the accent */
	{
		KRect r = Kelement_get_bounds(Kitems_control_get_container(lb, 3));
		CHECK(pixel_is(root, 470, (int)(Kelement_to_root(Kitems_control_get_container(lb, 3), (KPoint){ 0, 0 }).y + r.height - 2), 0xCC, 0xCC, 0xCC));
	}
	tap(root, center(Kitems_control_get_container(lb, 1)));
	CHECK(Kselector_get_selected_index(lb) == 1 && g_changes == 1);
	{
		KPoint p = Kelement_to_root(Kitems_control_get_container(lb, 1), (KPoint){ 0, 0 });
		CHECK(pixel_is(root, 470, (int)p.y + 3, 0x00, 0x50, 0xEF));
	}
	/* pressed: a PhoneBaseMediumHigh rectangle over the box, brighter (the tilt shows only on the GPU) */
	{
		KPoint p = center(Kitems_control_get_container(lb, 2));
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		CHECK(pixel_is(root, 470, (int)Kelement_to_root(Kitems_control_get_container(lb, 2), (KPoint){ 0, 0 }).y + 3, 0xF5, 0xF5, 0xF5));
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
		CHECK(Kselector_get_selected_index(lb) == 2);
	}
	/* Multiple and Extended */
	Klist_box_set_selection_mode(lb, K_SELECTION_MULTIPLE);
	tap(root, center(Kitems_control_get_container(lb, 0)));
	tap(root, center(Kitems_control_get_container(lb, 3)));
	CHECK(Klist_box_get_selected_indices(lb, indices, 4) == 3 && indices[0] == 0 && indices[1] == 2 && indices[2] == 3);
	Klist_box_set_selection_mode(lb, K_SELECTION_EXTENDED);
	tap(root, center(Kitems_control_get_container(lb, 1)));
	CHECK(Klist_box_get_selected_indices(lb, indices, 4) == 1 && indices[0] == 1);
	/* the keyboard moves the selection in Single and Extended */
	key(root, K_KEY_DOWN, 0);
	CHECK(Kselector_get_selected_index(lb) == 2);
	/* disabled: no accent, a tap does nothing */
	Kelement_set_enabled(lb, K_FALSE);
	tap(root, center(Kitems_control_get_container(lb, 0)));
	CHECK(Kselector_get_selected_index(lb) == 2);
	Kroot_destroy(root);
}


/* The biggest difference in brightness between a pixel of the rectangle and its top left pixel: what a reader sees of the text in it. */
static int contrast_in(Kid root, int x0, int y0, int x1, int y1) {
	KFrame f;
	int x, y, best = 0, base;
	const unsigned char* b;
	Kroot_render(root, &f);
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
static void row_rect(Kid container, int* x0, int* y0, int* x1, int* y1) {
	KRect r = Kelement_get_bounds(container);
	KPoint p = Kelement_to_root(container, (KPoint){ 0, 0 });
	*x0 = (int)p.x + 2;
	*y0 = (int)p.y + 1;
	*x1 = (int)p.x + 100;
	*y1 = (int)(p.y + r.height) - 1;
}

/* Light: the 8.1 dictionary has PhoneBaseBlack text on a PhoneBaseMediumHigh box that is black 75%; the library reads white on it. */
static void test_list_box_light_is_readable(void) {
	KTheme themes[2] = { K_THEME_LIGHT, K_THEME_DARK };
	int t;
	for (t = 0; t < 2; t++) {
		Kid lb = Klist_box_create(), root;
		int x0, y0, x1, y1;
		fill(lb, 4);
		root = make_root(lb, K_ANIMATIONS_OFF);
		Kroot_set_theme(root, themes[t]);
		Kroot_update_layout(root);
		/* a row of the box */
		row_rect(Kitems_control_get_container(lb, 0), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 120);
		if (themes[t] == K_THEME_LIGHT) {
			int r, g, b;
			test_pixel(root, 470, y0 + 2, &r, &g, &b);
			CHECK(r == 0x40 && g == 0x40 && b == 0x40);	/* black 75% over the white page */
		}
		/* selected: the text is on the accent */
		tap(root, center(Kitems_control_get_container(lb, 1)));
		row_rect(Kitems_control_get_container(lb, 1), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 120);
		/* pressed */
		{
			KPoint p = center(Kitems_control_get_container(lb, 2));
			Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
			row_rect(Kitems_control_get_container(lb, 2), &x0, &y0, &x1, &y1);
			CHECK(contrast_in(root, x0, y0, x1, y1) > 100);
			Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
		}
		/* disabled: no box, the text is PhoneBaseMediumMid on the page */
		Kelement_set_enabled(lb, K_FALSE);
		row_rect(Kitems_control_get_container(lb, 0), &x0, &y0, &x1, &y1);
		CHECK(contrast_in(root, x0, y0, x1, y1) > 60);
		Kroot_destroy(root);
	}
}


/* ---- ComboBox ---- */

static void test_combo_box_8_1(void) {
	Kid stack = Kstack_panel_create(K_VERTICAL), cb, root;
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(cb, K_VALIGN_TOP);
	Kcombo_box_set_header(cb, "Color");
	Kcombo_box_set_placeholder_text(cb, "pick one");
	fill(cb, 3);
	root = make_root(stack, K_ANIMATIONS_OFF);
	CHECK_STR(Kcombo_box_get_header(cb), "Color");
	CHECK_STR(Kcombo_box_get_placeholder_text(cb), "pick one");
	CHECK(Kelement_is_a(Kitems_control_get_container(cb, 0), K_CLASS_COMBO_BOX_ITEM));
	/* a header, the 9.5 of the touch target, a 38.5 outline, 9.5 */
	CHECK(Kelement_get_bounds(cb).height > 57.5 + 10);
	/* with a header off, the control is 57.5 high, like a Button */
	Kcombo_box_set_header(cb, NULL);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(cb).height, 57.5);
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
		Kselector_set_selected_index(cb, 1);
		CHECK(lit(root, 8, 14, 100, 42) != before);
	}
	/* open: white with an accent border, the selected item in the accent */
	tap(root, center(cb));
	CHECK(Kcombo_box_get_is_drop_down_open(cb));
	Kroot_update_layout(root);
	{
		KRect r = Kelement_get_bounds(Kitems_control_get_container(cb, 0));
		KPoint p = Kelement_to_root(Kitems_control_get_container(cb, 1), (KPoint){ 0, 0 });
		CHECK(r.width > 400);
		CHECK(pixel_is(root, 400, (int)p.y + 3, 255, 255, 255));
		CHECK(pixel_is(root, 1, (int)p.y + 10, 0x00, 0x50, 0xEF)); /* the border of the list */
	}
	/* Escape closes, the keys change the selection */
	key(root, K_KEY_ESCAPE, 0);
	CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	CHECK(Kselector_get_selected_index(cb) == 1);
	Kelement_focus(cb);
	key(root, K_KEY_DOWN, 0);
	CHECK(Kselector_get_selected_index(cb) == 2);
	key(root, K_KEY_UP, 0);
	key(root, K_KEY_UP, 0);
	CHECK(Kselector_get_selected_index(cb) == 0);
	key(root, K_KEY_SPACE, 0);
	CHECK(Kcombo_box_get_is_drop_down_open(cb));
	key(root, K_KEY_ENTER, 0);
	CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	/* MaxDropDownHeight limits the list, which then scrolls */
	Kcombo_box_set_max_drop_down_height(cb, 80);
	CHECK(Kcombo_box_get_max_drop_down_height(cb) == 80);
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	Kroot_update_layout(root);
	CHECK(Kscroll_viewer_get_viewport_height(Kitems_control_get_scroll_viewer(cb)) <= 80.5);
	Kcombo_box_set_is_drop_down_open(cb, K_FALSE);
	/* disabled: a tap does not open it */
	Kelement_set_enabled(cb, K_FALSE);
	tap(root, center(cb));
	CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	Kroot_destroy(root);
}

static void test_combo_box_placement(void) {
	Kid stack = Kstack_panel_create(K_VERTICAL), cb, root;
	Kid spacer = Kpanel_add(stack, Kborder_create());
	Kelement_set_height(spacer, 300);
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(cb, K_VALIGN_TOP);
	fill(cb, 4);
	Kselector_set_selected_index(cb, 2);
	root = make_root(stack, K_ANIMATIONS_OFF);
	tap(root, center(cb));
	Kroot_update_layout(root);
	{
		/* the list opens over the box with the selected item at the place of the box: its top is 2.5 above the outline */
		KPoint box = Kelement_to_root(cb, (KPoint){ 0, 9.5f });
		KPoint item = Kelement_to_root(Kitems_control_get_container(cb, 2), (KPoint){ 0, 0 });
		CHECK_NEAR(item.y, box.y - 2.5);
		CHECK_NEAR(item.x, 2.5);
		CHECK(Kelement_get_bounds(Kitems_control_get_container(cb, 0)).width > 400);
	}
	/* a list that does not fit above the top of the screen is moved down, whole */
	Kcombo_box_set_is_drop_down_open(cb, K_FALSE);
	Kelement_set_height(spacer, 0);
	Kroot_update_layout(root);
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	Kroot_update_layout(root);
	CHECK(Kelement_to_root(Kitems_control_get_container(cb, 0), (KPoint){ 0, 0 }).y >= 0);
	Kroot_destroy(root);
}

static int g_opened, g_closed;
static void on_opened(Kid sender, void* user) { (void)sender; (void)user; g_opened++; }
static void on_closed(Kid sender, void* user) { (void)sender; (void)user; g_closed++; }

static void test_combo_box_events(void) {
	Kid stack = Kstack_panel_create(K_VERTICAL), cb, root;
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(cb, K_VALIGN_TOP);
	fill(cb, 3);
	root = make_root(stack, K_ANIMATIONS_OFF);
	g_opened = g_closed = g_changes = 0;
	Kelement_on(cb, K_EVENT_DROP_DOWN_OPENED, on_opened, NULL);
	Kelement_on(cb, K_EVENT_DROP_DOWN_CLOSED, on_closed, NULL);
	Kelement_on(cb, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(cb));
	CHECK(g_opened == 1 && g_closed == 0);
	Kroot_update_layout(root);
	tap(root, center(Kitems_control_get_container(cb, 2)));
	CHECK(g_closed == 1 && g_changes == 1 && Kselector_get_selected_index(cb) == 2);
	/* the open list covers the box: a tap on the selected item chooses it again and closes */
	tap(root, center(cb));
	Kroot_update_layout(root);
	CHECK(g_opened == 2);
	tap(root, (KPoint){ 450, 780 });
	CHECK(g_closed == 2 && g_changes == 1);
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	CHECK(g_opened == 3);
	Kroot_destroy(root);
}


/* ---- FlipView ---- */

/* A finger from a point by dx in steps: what a step is in the time of the clock decides whether it is a flick, and the test has no clock. */
static void drag_steps(Kid root, KPoint from, float dx, float step) {
	float x;
	int sign = dx < 0 ? -1 : 1;
	Kroot_inject_pointer(root, K_POINTER_MOVE, from.x, from.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, from.x, from.y);
	for (x = 0; fabsf(x) <= fabsf(dx); x += (float)sign * step) Kroot_inject_pointer(root, K_POINTER_MOVE, from.x + x, from.y);
	Kroot_inject_pointer(root, K_POINTER_UP, from.x + dx, from.y);
}

static void drag(Kid root, KPoint from, float dx) { drag_steps(root, from, dx, 20); }

static void test_flip_view(void) {
	Kid fv = Kflip_view_create(), root;
	CHECK(Kselector_get_selected_index(fv) == -1);
	fill(fv, 4);
	/* the first item is chosen as soon as there is one */
	CHECK(Kselector_get_selected_index(fv) == 0);
	root = make_root(fv, K_ANIMATIONS_OFF);
	g_changes = 0;
	Kelement_on(fv, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(Kelement_is_a(Kitems_control_get_container(fv, 0), K_CLASS_FLIP_VIEW_ITEM));
	{
		/* the pages are the size of the view less the 2.5 margin, one page apart */
		KRect a = Kelement_get_bounds(Kitems_control_get_container(fv, 0)), b = Kelement_get_bounds(Kitems_control_get_container(fv, 1));
		CHECK(a.x == 0 && a.width == 475 && a.height == 795 && b.x == 475);
		CHECK_NEAR(Kelement_to_root(Kitems_control_get_container(fv, 0), (KPoint){ 0, 0 }).x, 2.5);
	}
	/* a drag of more than a fifth of a page turns to the next one */
	drag(root, (KPoint){ 300, 400 }, -200);
	CHECK(Kselector_get_selected_index(fv) == 1 && g_changes == 1);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 1)).x == 0);
	/* a short one goes back */
	drag_steps(root, (KPoint){ 300, 400 }, 40, 4);
	CHECK(Kselector_get_selected_index(fv) == 1 && g_changes == 1);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 1)).x == 0);
	/* to the right: the previous page */
	drag(root, (KPoint){ 100, 400 }, 200);
	CHECK(Kselector_get_selected_index(fv) == 0 && g_changes == 2);
	/* the first page has nothing before it */
	drag(root, (KPoint){ 100, 400 }, 300);
	CHECK(Kselector_get_selected_index(fv) == 0 && g_changes == 2);
	/* a mostly vertical drag is not the view's */
	drag(root, (KPoint){ 300, 100 }, -5);
	CHECK(Kselector_get_selected_index(fv) == 0);
	/* the keys */
	Kelement_focus(fv);
	key(root, K_KEY_RIGHT, 0);
	key(root, K_KEY_RIGHT, 0);
	CHECK(Kselector_get_selected_index(fv) == 2);
	key(root, K_KEY_LEFT, 0);
	CHECK(Kselector_get_selected_index(fv) == 1);
	key(root, K_KEY_END, 0);
	CHECK(Kselector_get_selected_index(fv) == 3);
	key(root, K_KEY_RIGHT, 0);
	CHECK(Kselector_get_selected_index(fv) == 3);
	key(root, K_KEY_HOME, 0);
	CHECK(Kselector_get_selected_index(fv) == 0);
	/* by the program; -1 is refused while there are items */
	Kselector_set_selected_index(fv, 2);
	CHECK(Kselector_get_selected_index(fv) == 2);
	Kselector_set_selected_index(fv, -1);
	CHECK(Kselector_get_selected_index(fv) == 2);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 2)).x == 0);
	/* removing the item that is shown shows the one that takes its place; clearing leaves none */
	Kitems_control_remove_at(fv, 2);
	CHECK(Kselector_get_selected_index(fv) == 2 && Kitems_control_get_count(fv) == 3);
	Kitems_control_remove_at(fv, 2);
	CHECK(Kselector_get_selected_index(fv) == 1);
	Kitems_control_clear(fv);
	CHECK(Kselector_get_selected_index(fv) == -1);
	/* the picture of a page */
	Kitems_control_add_text(fv, "a page");
	Kroot_update_layout(root);
	Kcontrol_set_font_size(Kitems_control_get_container(fv, 0), 40);
	CHECK(lit(root, 0, 0, 480, 200) > 100);
	Kroot_destroy(root);
}

static void test_flip_view_slide(void) {
	Kid fv = Kflip_view_create(), root;
	fill(fv, 3);
	root = make_root(fv, K_ANIMATIONS_ON);
	Kselector_set_selected_index(fv, 1);
	run(root, 64);
	Kroot_update_layout(root);
	{
		/* on its way: the first page has left a little, the second is coming */
		KRect a = Kelement_get_bounds(Kitems_control_get_container(fv, 0)), b = Kelement_get_bounds(Kitems_control_get_container(fv, 1));
		CHECK(a.x < 0 && a.x > -475);
		CHECK(b.x > 0 && b.x < 475);
	}
	run(root, 400);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 1)).x == 0);
	/* UseTouchAnimationsForAllNavigation off: the program jumps */
	CHECK(Kflip_view_get_use_touch_animations_for_all_navigation(fv));
	Kflip_view_set_use_touch_animations_for_all_navigation(fv, K_FALSE);
	Kselector_set_selected_index(fv, 2);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 2)).x == 0);
	/* a finger still slides it */
	drag(root, (KPoint){ 100, 400 }, 200);
	CHECK(Kselector_get_selected_index(fv) == 1);
	run(root, 16);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 1)).x != 0);
	run(root, 400);
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(Kitems_control_get_container(fv, 1)).x == 0);
	Kroot_destroy(root);
}


/* ---- AutoSuggestBox ---- */

static const char* const FRUIT[] = { "apple", "apricot", "banana", "blueberry", "cherry" };
static KAutoSuggestionBoxTextChangeReason g_reasons[16];
static int g_reason_count, g_chosen, g_query_count, g_query_chosen;
static char g_query[64];
static int g_order[16], g_order_count;

static void filter_into(Kid box) {
	const char* text = Kauto_suggest_box_get_text(box);
	const char* hits[5];
	int i, n = 0;
	for (i = 0; i < 5; i++) if (*text && strncmp(FRUIT[i], text, strlen(text)) == 0) hits[n++] = FRUIT[i];
	Kauto_suggest_box_set_items(box, hits, n);
}

static void asb_changed(Kid box, KAutoSuggestionBoxTextChangeReason reason, void* user) {
	(void)user;
	if (g_reason_count < 16) g_reasons[g_reason_count++] = reason;
	if (g_order_count < 16) g_order[g_order_count++] = 1;
	if (reason == K_SUGGEST_USER_INPUT) filter_into(box);
}

static void asb_chosen(Kid box, int index, void* user) {
	(void)box; (void)user;
	g_chosen = index;
	if (g_order_count < 16) g_order[g_order_count++] = 2;
}

static void asb_query(Kid box, const char* text, int chosen, void* user) {
	(void)box; (void)user;
	g_query_count++;
	g_query_chosen = chosen;
	snprintf(g_query, sizeof g_query, "%s", text);
	if (g_order_count < 16) g_order[g_order_count++] = 3;
}

static Kid asb_page(Kid* root) {
	Kid stack = Kstack_panel_create(K_VERTICAL), box = Kpanel_add(stack, Kauto_suggest_box_create());
	Kelement_set_vertical_alignment(box, K_VALIGN_TOP);
	Kpanel_add(stack, Kbutton_create("next"));
	Kauto_suggest_box_on_text_changed(box, asb_changed, NULL);
	Kauto_suggest_box_on_suggestion_chosen(box, asb_chosen, NULL);
	Kauto_suggest_box_on_query_submitted(box, asb_query, NULL);
	*root = make_root(stack, K_ANIMATIONS_OFF);
	g_reason_count = g_chosen = g_query_count = g_order_count = 0;
	g_query_chosen = -2;
	g_query[0] = 0;
	return box;
}

static void test_auto_suggest_box(void) {
	Kid root, box = asb_page(&root);
	Kauto_suggest_box_set_placeholder_text(box, "search");
	Kauto_suggest_box_set_header(box, "Fruit");
	CHECK_STR(Kauto_suggest_box_get_placeholder_text(box), "search");
	CHECK_STR(Kauto_suggest_box_get_header(box), "Fruit");
	/* a touch on the box gives its text the keyboard; every typed letter is a TextChanged, with the reason */
	tap(root, center(box));
	CHECK(Kroot_get_focus(root) != box && Kelement_get_class(Kroot_get_focus(root)) == K_CLASS_TEXT_BOX);
	type(root, "a");
	CHECK(g_reason_count == 1 && g_reasons[0] == K_SUGGEST_USER_INPUT);
	CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box) && Kitems_control_get_count(box) == 2);
	CHECK_STR(Kitems_control_get_text(box, 1), "apricot");
	Kroot_update_layout(root);
	/* the list hangs under the text box, white with an accent border */
	{
		KRect tb = Kelement_get_bounds(Kelement_get_child(box, 0));
		KPoint p = Kelement_to_root(Kitems_control_get_container(box, 0), (KPoint){ 0, 0 });
		KPoint q = Kelement_to_root(Kelement_get_child(box, 0), (KPoint){ 0, tb.height });
		CHECK(p.y >= q.y && p.y < q.y + 20);
		CHECK(pixel_is(root, 400, (int)p.y + 3, 255, 255, 255));
		CHECK(pixel_is(root, 1, (int)p.y + 10, 0x00, 0x50, 0xEF));
	}
	type(root, "p");
	CHECK(g_reason_count == 2 && Kitems_control_get_count(box) == 2);
	type(root, "x");
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box) && Kitems_control_get_count(box) == 0);
	key(root, K_KEY_BACKSPACE, 0);
	CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box));
	/* Down and Enter choose the first suggestion: SuggestionChosen, then TextChanged, then QuerySubmitted */
	g_reason_count = g_order_count = 0;
	key(root, K_KEY_DOWN, 0);
	CHECK(g_chosen == 0 && g_query_count == 0 && Kauto_suggest_box_get_is_suggestion_list_open(box));
	key(root, K_KEY_ENTER, 0);
	CHECK(g_chosen == 0 && g_query_count == 1 && g_query_chosen == 0);
	CHECK_STR(Kauto_suggest_box_get_text(box), "apple");
	CHECK_STR(g_query, "apple");
	CHECK(g_reason_count == 1 && g_reasons[0] == K_SUGGEST_SUGGESTION_CHOSEN);
	CHECK(g_order_count == 3 && g_order[0] == 2 && g_order[1] == 1 && g_order[2] == 3);
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	/* Enter on a text that was not chosen: the query is the text */
	g_query_count = 0;
	key(root, K_KEY_ENTER, 0);
	CHECK(g_query_count == 1 && g_query_chosen == -1);
	/* a tap on a suggestion */
	Kauto_suggest_box_set_text(box, "");
	type(root, "b");
	CHECK(Kitems_control_get_count(box) == 2);
	Kroot_update_layout(root);
	g_reason_count = 0;
	/* a finger on a suggestion does not close the list or take the keyboard from the text */
	{
		KPoint p = center(Kitems_control_get_container(box, 1));
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box));
		CHECK(Kelement_get_class(Kroot_get_focus(root)) == K_CLASS_TEXT_BOX);
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
	}
	CHECK_STR(Kauto_suggest_box_get_text(box), "blueberry");
	CHECK(g_chosen == 1 && g_query_chosen == 1 && g_reasons[g_reason_count - 1] == K_SUGGEST_SUGGESTION_CHOSEN);
	CHECK(Kelement_get_class(Kroot_get_focus(root)) == K_CLASS_TEXT_BOX); /* the text kept the keyboard */
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	/* Up and Down go round, Escape closes */
	Kauto_suggest_box_set_text(box, "");
	type(root, "a");
	key(root, K_KEY_UP, 0);
	key(root, K_KEY_ESCAPE, 0);
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	/* a tap outside closes the list */
	Kauto_suggest_box_set_text(box, "");
	type(root, "c");
	CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box));
	tap(root, (KPoint){ 400, 700 });
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	Kroot_destroy(root);
}

static void test_auto_suggest_box_programmatic(void) {
	Kid root, box = asb_page(&root);
	/* a text that the program sets is a ProgrammaticChange, and opens nothing */
	Kauto_suggest_box_set_text(box, "ban");
	CHECK(g_reason_count == 1 && g_reasons[0] == K_SUGGEST_PROGRAMMATIC_CHANGE);
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	CHECK_STR(Kauto_suggest_box_get_text(box), "ban");
	Kauto_suggest_box_set_text(box, "ban");
	CHECK(g_reason_count == 1); /* the same text: no change */
	/* suggestions given to a box that has no keyboard do not open it */
	Kauto_suggest_box_set_items(box, FRUIT, 5);
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box) && Kitems_control_get_count(box) == 5);
	Kauto_suggest_box_set_is_suggestion_list_open(box, K_TRUE);
	CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box));
	Kauto_suggest_box_set_is_suggestion_list_open(box, K_FALSE);
	/* UpdateTextOnSelect off: the text stays, the events come */
	Kauto_suggest_box_set_update_text_on_select(box, K_FALSE);
	CHECK(Kelement_focus(box));
	CHECK(Kelement_get_class(Kroot_get_focus(root)) == K_CLASS_TEXT_BOX);
	Kauto_suggest_box_set_is_suggestion_list_open(box, K_TRUE);
	key(root, K_KEY_DOWN, 0);
	key(root, K_KEY_DOWN, 0);
	key(root, K_KEY_ENTER, 0);
	CHECK(g_chosen == 1 && g_query_chosen == 1);
	CHECK_STR(Kauto_suggest_box_get_text(box), "ban");
	/* MaxSuggestionListHeight: the list is as tall as that and scrolls */
	Kauto_suggest_box_set_max_suggestion_list_height(box, 60);
	Kauto_suggest_box_set_is_suggestion_list_open(box, K_TRUE);
	Kroot_update_layout(root);
	CHECK(Kscroll_viewer_get_viewport_height(Kitems_control_get_scroll_viewer(box)) <= 60);
	CHECK(Kscroll_viewer_get_extent_height(Kitems_control_get_scroll_viewer(box)) > 60);
	/* a drag on the list scrolls it and does not close it or choose anything */
	{
		Kid sv = Kitems_control_get_scroll_viewer(box);
		KPoint p = center(Kitems_control_get_container(box, 0));
		int chosen_before = g_query_count;
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 20);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 45);
		CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box));
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y - 45);
		CHECK(Kscroll_viewer_get_vertical_offset(sv) > 20);
		CHECK(Kauto_suggest_box_get_is_suggestion_list_open(box) && g_query_count == chosen_before);
		CHECK(Kelement_get_class(Kroot_get_focus(root)) == K_CLASS_TEXT_BOX);
	}
	/* Tab leaves the text and closes the list */
	Kroot_move_focus(root, K_TRUE);
	CHECK(!Kauto_suggest_box_get_is_suggestion_list_open(box));
	Kroot_destroy(root);
}


/* ---- SemanticZoom ---- */

static int g_zoom_log[8], g_zoom_count;
static void zoom_started(Kid sender, int source_in, void* user) { (void)sender; (void)user; g_zoom_log[g_zoom_count++] = 10 + source_in; }
static void zoom_completed(Kid sender, int source_in, void* user) { (void)sender; (void)user; g_zoom_log[g_zoom_count++] = 20 + source_in; }

static void test_semantic_zoom(void) {
	Kid sz = Ksemantic_zoom_create(), in = Klist_view_create(), out = Kgrid_view_create(), root;
	fill(in, 20);
	fill(out, 4);
	Ksemantic_zoom_set_zoomed_in_view(sz, in);
	Ksemantic_zoom_set_zoomed_out_view(sz, out);
	root = make_root(sz, K_ANIMATIONS_OFF);
	CHECK(Ksemantic_zoom_get_is_zoomed_in_view_active(sz));
	CHECK(Kelement_get_visibility(in) == K_VISIBLE && Kelement_get_visibility(out) == K_COLLAPSED);
	CHECK(Kelement_get_bounds(in).width == 480 && Kelement_get_bounds(in).height == 800);
	Ksemantic_zoom_on_view_change_started(sz, zoom_started, NULL);
	Ksemantic_zoom_on_view_change_completed(sz, zoom_completed, NULL);
	g_zoom_count = 0;
	Ksemantic_zoom_toggle_active_view(sz);
	CHECK(!Ksemantic_zoom_get_is_zoomed_in_view_active(sz));
	CHECK(g_zoom_count == 2 && g_zoom_log[0] == 11 && g_zoom_log[1] == 21);
	Kroot_update_layout(root);
	CHECK(Kelement_get_visibility(in) == K_COLLAPSED && Kelement_get_visibility(out) == K_VISIBLE && Kelement_get_bounds(out).width == 480);
	/* the zoomed out view takes taps: a handler of its ItemClick goes back */
	tap(root, center(Kitems_control_get_container(out, 1)));
	CHECK(Kselector_get_selected_index(out) == 1);
	Ksemantic_zoom_set_is_zoomed_in_view_active(sz, K_TRUE);
	CHECK(Ksemantic_zoom_get_is_zoomed_in_view_active(sz) && g_zoom_count == 4 && g_zoom_log[2] == 10 && g_zoom_log[3] == 20);
	/* the same view again: no change, no events */
	Ksemantic_zoom_set_is_zoomed_in_view_active(sz, K_TRUE);
	CHECK(g_zoom_count == 4);
	/* CanChangeViews off keeps the view */
	Ksemantic_zoom_set_can_change_views(sz, K_FALSE);
	Ksemantic_zoom_toggle_active_view(sz);
	CHECK(Ksemantic_zoom_get_is_zoomed_in_view_active(sz) && g_zoom_count == 4);
	Ksemantic_zoom_set_is_zoom_out_button_enabled(sz, K_FALSE);
	Kroot_destroy(root);
}


/* ---- handlers that destroy the control ---- */

static void destroy_self(Kid sender, void* user) { (void)user; g_nav++; Kroot_set_visual(g_nav_root, Kstack_panel_create(K_VERTICAL)); (void)sender; }
static void destroy_on_click(Kid sender, int index, void* user) { (void)index; destroy_self(sender, user); }
static void destroy_on_chosen(Kid sender, int index, void* user) { (void)index; destroy_self(sender, user); }
static void destroy_on_reason(Kid sender, KAutoSuggestionBoxTextChangeReason reason, void* user) { (void)reason; destroy_self(sender, user); }
static void destroy_on_zoom(Kid sender, int source, void* user) { (void)source; destroy_self(sender, user); }

static void survives(Kid root) {
	Kroot_render(root, NULL);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 100, 100);
	Kroot_inject_pointer(root, K_POINTER_UP, 100, 100);
	Kroot_inject_key(root, K_KEY_DOWN, 0, 1);
	Kroot_inject_key(root, K_KEY_DOWN, 0, 0);
	Kroot_inject_text(root, "x");
	Kroot_tick(root, 1000);
	Kroot_render(root, NULL);
}

static void test_handlers_that_destroy(void) {
	Kid root = Kroot_create(480, 800), c;
	g_nav_root = root;
	/* ItemClick of a ListView */
	c = Klist_view_create();
	fill(c, 3);
	Klist_view_set_is_item_click_enabled(c, K_TRUE);
	Klist_view_on_item_click(c, destroy_on_click, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Kitems_control_get_container(c, 1)));
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	/* SelectionChanged of a ListView and of a GridView */
	c = Kgrid_view_create();
	fill(c, 3);
	Kelement_on(c, K_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Kitems_control_get_container(c, 1)));
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	/* SelectionChanged of a FlipView, from a drag */
	c = Kflip_view_create();
	fill(c, 3);
	Kelement_on(c, K_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	g_nav = 0;
	drag(root, (KPoint){ 300, 400 }, -200);
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	/* SuggestionChosen, and TextChanged of an AutoSuggestBox */
	c = Kauto_suggest_box_create();
	Kauto_suggest_box_on_suggestion_chosen(c, destroy_on_chosen, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	tap(root, center(c));
	Kauto_suggest_box_set_items(c, FRUIT, 3);
	g_nav = 0;
	Kroot_update_layout(root);
	tap(root, center(Kitems_control_get_container(c, 0)));
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	c = Kauto_suggest_box_create();
	Kauto_suggest_box_on_text_changed(c, destroy_on_reason, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	tap(root, center(c));
	g_nav = 0;
	type(root, "ab");
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	/* ViewChangeStarted of a SemanticZoom */
	c = Ksemantic_zoom_create();
	Ksemantic_zoom_set_zoomed_in_view(c, Klist_view_create());
	Ksemantic_zoom_on_view_change_started(c, destroy_on_zoom, NULL);
	Kroot_set_visual(root, c);
	g_nav = 0;
	Ksemantic_zoom_toggle_active_view(c);
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	/* the drop-down of a ComboBox closed by an item whose SelectionChanged destroys the page */
	c = Kcombo_box_create();
	fill(c, 3);
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Kelement_on(c, K_EVENT_SELECTION_CHANGED, destroy_self, NULL);
	Kroot_set_visual(root, c);
	Kroot_update_layout(root);
	tap(root, center(c));
	Kroot_update_layout(root);
	g_nav = 0;
	tap(root, center(Kitems_control_get_container(c, 2)));
	CHECK(g_nav == 1 && !Kelement_is_valid(c));
	survives(root);
	Kroot_destroy(root);
}


/* ---- pictures ---- */

static void test_picture(void) {
	Kid stack = Kstack_panel_create(K_VERTICAL), lv, gv, cb, box, grid;
	Kid root = Kroot_create(480, 800);
	char path[512];
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kelement_set_margin(stack, Kthickness(12, 0, 12, 0));
	Kroot_set_visual(root, stack);
	lv = Kpanel_add(stack, Klist_view_create());
	Klist_view_set_header(lv, "ListView, Multiple");
	Klist_view_set_selection_mode(lv, K_LIST_VIEW_SELECTION_MULTIPLE);
	fill(lv, 3);
	Kelement_set_height(lv, 150);
	Kselector_item_set_is_selected(Kitems_control_get_container(lv, 1), K_TRUE);
	gv = Kpanel_add(stack, Kgrid_view_create());
	Klist_view_set_selection_mode(gv, K_LIST_VIEW_SELECTION_MULTIPLE);
	fill(gv, 5);
	grid = Kitems_control_get_items_panel(gv);
	Kitems_wrap_grid_set_item_width(grid, 80);
	Kitems_wrap_grid_set_item_height(grid, 60);
	Kelement_set_height(gv, 140);
	Kselector_item_set_is_selected(Kitems_control_get_container(gv, 2), K_TRUE);
	Kpanel_add(stack, Klist_box_create());
	fill(Kelement_get_child(stack, 2), 2);
	Kselector_set_selected_index(Kelement_get_child(stack, 2), 0);
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kcombo_box_set_header(cb, "ComboBox");
	Kcombo_box_set_placeholder_text(cb, "pick a color");
	fill(cb, 3);
	box = Kpanel_add(stack, Kauto_suggest_box_create());
	Kauto_suggest_box_set_header(box, "AutoSuggestBox");
	Kauto_suggest_box_set_placeholder_text(box, "search fruit");
	Kroot_update_layout(root);
	snprintf(path, sizeof path, "%s/items.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kelement_focus(box);
	Kroot_inject_text(root, "a");
	Kauto_suggest_box_set_items(box, FRUIT, 2);
	snprintf(path, sizeof path, "%s/items-suggest.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
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
	Kshutdown();
	return test_finish("test_items");
}
