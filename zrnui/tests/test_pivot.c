/**
 * Pivot and PivotItem of Windows Phone 8.1, and the ItemClick of a ListView in one: the Settings of the phone,
 * "system | applications". Every check goes through what a finger would do (Zroot_inject_pointer), the clock, or a pixel.
 */

#include "test.h"

static int g_changes, g_clicks, g_clicked, g_button_clicks, g_loading, g_loaded, g_last_loading, g_last_loaded;

static void on_change(Zid sender, void* u) { (void)sender; (void)u; g_changes++; }
static void on_item_click(Zid list, int index, void* u) { (void)list; (void)u; g_clicks++; g_clicked = index; }
static void on_button(Zid sender, void* u) { (void)sender; (void)u; g_button_clicks++; }
static void on_loading(Zid pivot, Zid item, void* u) { (void)pivot; (void)u; g_loading++; g_last_loading = (int)item; }
static void on_loaded(Zid pivot, Zid item, void* u) { (void)pivot; (void)u; g_loaded++; g_last_loaded = (int)item; }

static Zid make_list(const char* const* names, int n) {
	Zid list = Zlist_view_create();
	int i;
	for (i = 0; i < n; i++) {
		Zid item = Zlist_view_item_create(NULL), t = Ztext_block_create(names[i]);
		Ztext_block_set_style(t, Z_TEXT_STYLE_PHONE_LARGE);
		Zelement_set_margin(t, Zthickness(0, 0, 0, 12));
		Zcontent_control_set_content(item, t);
		Zitems_control_add(list, item);
	}
	/* a row of a list of settings opens something: nothing is selected, a tap is an ItemClick */
	Zlist_view_set_selection_mode(list, Z_LIST_VIEW_SELECTION_NONE);
	Zlist_view_set_is_item_click_enabled(list, true);
	return list;
}

static const char* const SYSTEM[] = { "wi-fi", "bluetooth", "screen lock", "ringtones + sounds", "notifications + actions" };
static const char* const APPS[] = { "games", "maps", "music + videos", "photos", "store" };

static Zid g_pivot, g_system, g_apps, g_item0, g_item1;

static Zid add_page(Zid pivot, const char* header, Zid content) {
	Zid item = Zpivot_item_create(header);
	Zcontent_control_set_content(item, content);
	CHECK(Zpivot_add_item(pivot, item) >= 0);
	return item;
}

static Zid setup(Zid* root, ZAnimations anim) {
	*root = Zroot_create(480, 800);
	Zroot_set_renderer(*root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(*root, anim);
	g_pivot = Zpivot_create();
	Zpivot_set_title(g_pivot, "SETTINGS");
	g_system = make_list(SYSTEM, 5);
	g_apps = make_list(APPS, 5);
	g_item0 = add_page(g_pivot, "system", g_system);
	g_item1 = add_page(g_pivot, "applications", g_apps);
	CHECK(Zpivot_get_item_count(g_pivot) == 2);
	Zroot_set_visual(*root, g_pivot);
	Zroot_update_layout(*root);
	g_changes = g_clicks = g_button_clicks = g_loading = g_loaded = 0;
	Zpivot_on_selection_changed(g_pivot, on_change, NULL);
	Zlist_view_on_item_click(g_system, on_item_click, NULL);
	return g_pivot;
}

static void run(Zid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Zroot_tick(root, 16);
}

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static ZPoint rect_center(ZRect r) { return (ZPoint){ r.x + r.width * 0.5f, r.y + r.height * 0.5f }; }

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

/* A finger down at `p`, drawn `dx` px in steps of `step`, and lifted. */
static void drag(Zid root, ZPoint p, float dx, float step) {
	float x;
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	for (x = step; fabsf(x) <= fabsf(dx); x += step) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x + x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x + dx, p.y);
}

/* The brightest pixel (the largest red) in a rectangle of the root. */
static int brightest(Zid root, ZRect r) {
	ZFrame f;
	int x, y, best = 0;
	Zroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && px[0] > best) best = px[0];
		}
	}
	return best;
}

/* The darkest pixel (the smallest red) in a rectangle of the root. */
static int darkest(Zid root, ZRect r) {
	ZFrame f;
	int x, y, best = 255;
	Zroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && px[0] < best) best = px[0];
		}
	}
	return best;
}

static void quiet(ZError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Zid pivot = Zpivot_create(), item = Zpivot_item_create("one");
	CHECK(Zelement_is_a(pivot, Z_CLASS_CONTROL));
	CHECK(Zelement_is_a(item, Z_CLASS_CONTENT_CONTROL) && Zelement_is_a(item, Z_CLASS_PIVOT_ITEM));
	CHECK_STR(Zpivot_item_get_header(item), "one");
	Zpivot_item_set_header(item, "two");
	CHECK_STR(Zpivot_item_get_header(item), "two");
	CHECK(Zpivot_get_selected_index(pivot) == -1);
	CHECK(Zpivot_get_selected_item(pivot) == Z_INVALID_ID);
	CHECK(Zpivot_add_item(pivot, item) == 0);
	CHECK(Zpivot_get_selected_index(pivot) == 0 && Zpivot_get_selected_item(pivot) == item);
	/* an item is in one pivot only */
	{
		Zid other = Zpivot_create();
		Zset_error_callback(quiet, NULL);
		CHECK(Zpivot_add_item(other, item) == -1);
		Zset_error_callback(NULL, NULL);
		Zelement_destroy(other);
	}
	CHECK(!Zpivot_get_is_locked(pivot));
	Zpivot_set_title(pivot, "TITLE");
	CHECK_STR(Zpivot_get_title(pivot), "TITLE");
	Zelement_destroy(pivot);
}

static void test_layout(void) {
	Zid root;
	ZRect r;
	setup(&root, Z_ANIMATIONS_OFF);
	CHECK(Zpivot_get_selected_index(g_pivot) == 0);
	CHECK(Zpivot_get_item(g_pivot, 1) == g_item1);
	/* the page of the chosen item is in view, the other is not */
	CHECK(Zelement_get_visibility(g_item0) == Z_VISIBLE);
	CHECK(Zelement_get_visibility(g_item1) == Z_COLLAPSED);
	/* PivotItemMargin 19,22.5,19,0: the page is 442 wide, and starts below the title and the strip */
	r = Zelement_get_bounds(g_item0);
	CHECK_NEAR(r.x, 19);
	CHECK(r.y > 120);
	CHECK_NEAR(r.width, 442);
	/* the header of the chosen item is under the title, at 19; the next one follows it, 16 px of margin after the text */
	{
		ZRect h0 = Zpivot_get_header_rect(g_pivot, 0), h1 = Zpivot_get_header_rect(g_pivot, 1);
		CHECK_NEAR(h0.x, 19);
		CHECK(h0.y > 55 && h0.y < 70);
		CHECK_NEAR(h1.x, h0.x + h0.width + 16);
		CHECK_NEAR(r.y, h0.y + h0.height + 22.5f);
	}
	Zroot_destroy(root);
}

static void test_header_colors(void) {
	ZTheme themes[2] = { Z_THEME_DARK, Z_THEME_LIGHT };
	int t;
	for (t = 0; t < 2; t++) {
		Zid root;
		ZRect h0, h1;
		setup(&root, Z_ANIMATIONS_OFF);
		Zroot_set_theme(root, themes[t]);
		Zroot_update_layout(root);
		h0 = Zpivot_get_header_rect(g_pivot, 0);
		h1 = Zpivot_get_header_rect(g_pivot, 1);
		if (themes[t] == Z_THEME_DARK) {
			/* PivotHeaderForegroundSelected is PhoneControlForeground, white; the others PhoneControlDisabled, white 40% */
			CHECK(brightest(root, h0) >= 250);
			CHECK_NEAR(brightest(root, h1), 0x66);
		} else {
			CHECK(darkest(root, h0) <= 5);
			CHECK_NEAR(darkest(root, h1), 255 - 0x66);
		}
		/* the title is in the foreground */
		{
			ZRect title = { 19, 38, 120, 20 };
			if (themes[t] == Z_THEME_DARK) CHECK(brightest(root, title) >= 250);
			else CHECK(darkest(root, title) <= 5);
		}
		Zroot_destroy(root);
	}
}

static void test_tap_header(void) {
	Zid root;
	setup(&root, Z_ANIMATIONS_OFF);
	/* the header "applications" is to the right of "system", at the top */
	tap(root, rect_center(Zpivot_get_header_rect(g_pivot, 1)));
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	CHECK(Zpivot_get_selected_item(g_pivot) == g_item1);
	CHECK(g_changes == 1);
	Zroot_update_layout(root);
	CHECK(Zelement_get_visibility(g_item1) == Z_VISIBLE);
	CHECK(Zelement_get_visibility(g_item0) == Z_COLLAPSED);
	CHECK_NEAR(Zelement_get_bounds(g_item1).x, 19);
	/* the chosen header is at 19 now, and the strip has gone round: "system" is after it */
	{
		ZRect h1 = Zpivot_get_header_rect(g_pivot, 1);
		CHECK_NEAR(h1.x, 19);
	}
	/* and back: a tap on the header of the first page, that has come round after the second */
	{
		ZRect h0 = Zpivot_get_header_rect(g_pivot, 0);
		CHECK(h0.width > 0 && h0.x > 19);
		tap(root, rect_center(h0));
	}
	CHECK(Zpivot_get_selected_index(g_pivot) == 0);
	CHECK(g_changes == 2);
	/* a tap on the chosen header changes nothing */
	tap(root, rect_center(Zpivot_get_header_rect(g_pivot, 0)));
	CHECK(Zpivot_get_selected_index(g_pivot) == 0 && g_changes == 2);
	Zroot_destroy(root);
}

static void test_drag(void) {
	Zid root;
	ZPoint p;
	setup(&root, Z_ANIMATIONS_OFF);
	p = center(Zitems_control_get_container(g_system, 0));
	/* a finger on a row, drawn left 240 px: a row of the list does not take it, the pivot does */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	{
		int x;
		for (x = 0; x <= 240; x += 20) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x - (float)x, p.y);
	}
	Zroot_update_layout(root);
	{
		/* the pages are following the finger: half a page to the left, and the header strip with them */
		ZRect r = Zelement_get_bounds(g_item0);
		CHECK(r.x < -150);
		CHECK(Zelement_get_visibility(g_item1) == Z_VISIBLE);
		CHECK(Zpivot_get_header_rect(g_pivot, 0).x < 19 - 50);
	}
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x - 240, p.y);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	CHECK(g_clicks == 0);
	CHECK(g_changes == 1);
	/* a short drag goes back to where it was */
	Zroot_update_layout(root);
	p = center(Zitems_control_get_container(g_apps, 0));
	drag(root, p, 40, 2);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(g_item1).x, 19);
	/* a swipe past the last page goes round to the first */
	drag(root, p, -300, -20);
	CHECK(Zpivot_get_selected_index(g_pivot) == 0);
	CHECK(g_changes == 2);
	/* and a swipe to the right from the first goes round to the last */
	Zroot_update_layout(root);
	p = center(Zitems_control_get_container(g_system, 0));
	drag(root, p, 300, 20);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	CHECK(g_changes == 3);
	Zroot_destroy(root);
}

static void test_flick(void) {
	Zid root;
	ZPoint p;
	setup(&root, Z_ANIMATIONS_OFF);
	/* a short and fast flick: 60 px in two moves goes to the next page */
	p = center(Zitems_control_get_container(g_system, 0));
	drag(root, p, -60, -30);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	Zroot_destroy(root);
}

static void test_ring_of_three(void) {
	Zid root = Zroot_create(480, 800), pivot = Zpivot_create(), items[3];
	const char* names[3] = { "one", "two", "three" };
	int i;
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	for (i = 0; i < 3; i++) {
		items[i] = Zpivot_item_create(names[i]);
		Zcontent_control_set_text(items[i], names[i]);
		Zpivot_add_item(pivot, items[i]);
	}
	Zroot_set_visual(root, pivot);
	Zroot_update_layout(root);
	Zpivot_set_selected_index(pivot, 1);
	Zroot_update_layout(root);
	{
		/* "two" is at the left, "three" after it and "one" last, as the ring of the PivotHeaderPanel goes */
		ZRect a = Zpivot_get_header_rect(pivot, 0), b = Zpivot_get_header_rect(pivot, 1), c = Zpivot_get_header_rect(pivot, 2);
		CHECK_NEAR(b.x, 19);
		CHECK(c.x > b.x && a.x > c.x);
	}
	/* from the first to the last the short way is back */
	Zpivot_set_selected_index(pivot, 0);
	Zpivot_set_selected_index(pivot, 2);
	Zroot_update_layout(root);
	CHECK(Zpivot_get_selected_index(pivot) == 2);
	CHECK_NEAR(Zpivot_get_header_rect(pivot, 2).x, 19);
	Zroot_destroy(root);
}

static void test_slide(void) {
	Zid root;
	setup(&root, Z_ANIMATIONS_ON);
	Zpivot_on_pivot_item_loading(g_pivot, on_loading, NULL);
	Zpivot_on_pivot_item_loaded(g_pivot, on_loaded, NULL);
	Zpivot_set_selected_index(g_pivot, 1);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1);
	/* Loading comes with the choice, Loaded when the page has arrived */
	CHECK(g_loading == 1 && g_last_loading == (int)g_item1);
	CHECK(g_loaded == 0);
	run(root, 64);
	Zroot_update_layout(root);
	{
		/* on its way: the first page has left a little, the second is coming, the strip is on the move */
		ZRect a = Zelement_get_bounds(g_item0), b = Zelement_get_bounds(g_item1);
		CHECK(a.x < 19 && a.x > -442);
		CHECK(b.x > 19 && b.x < 480);
		CHECK(Zpivot_get_header_rect(g_pivot, 0).x < 19 && Zpivot_get_header_rect(g_pivot, 0).x > -200);
	}
	CHECK(g_loaded == 0);
	run(root, 400);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(g_item1).x, 19);
	CHECK(g_loaded == 1 && g_last_loaded == (int)g_item1);
	/* without animations all at once */
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	Zpivot_set_selected_index(g_pivot, 0);
	CHECK(g_loading == 2 && g_loaded == 2 && g_last_loaded == (int)g_item0);
	Zroot_destroy(root);
}

static void test_locked(void) {
	Zid root;
	ZPoint p;
	setup(&root, Z_ANIMATIONS_OFF);
	Zpivot_set_is_locked(g_pivot, true);
	CHECK(Zpivot_get_is_locked(g_pivot));
	Zroot_update_layout(root);
	/* the headers of the other items are not shown */
	CHECK(Zpivot_get_header_rect(g_pivot, 1).width == 0);
	CHECK(Zpivot_get_header_rect(g_pivot, 0).width > 0);
	p = center(Zitems_control_get_container(g_system, 0));
	drag(root, p, -300, -20);
	CHECK(Zpivot_get_selected_index(g_pivot) == 0 && g_changes == 0);
	tap(root, rect_center(Zpivot_get_header_rect(g_pivot, 0)));
	CHECK(g_changes == 0);
	/* the program may still go on */
	Zpivot_set_selected_index(g_pivot, 1);
	CHECK(Zpivot_get_selected_index(g_pivot) == 1 && g_changes == 1);
	Zpivot_set_is_locked(g_pivot, false);
	Zroot_update_layout(root);
	CHECK(Zpivot_get_header_rect(g_pivot, 0).width > 0);
	Zroot_destroy(root);
}

static void test_item_click(void) {
	Zid root, item;
	setup(&root, Z_ANIMATIONS_OFF);
	item = Zitems_control_get_container(g_system, 2);
	tap(root, center(item));
	CHECK(g_clicks == 1);
	CHECK(g_clicked == 2);
	/* a row opens something and does not stay chosen */
	CHECK(Zselector_get_selected_index(g_system) == -1);
	tap(root, center(Zitems_control_get_container(g_system, 0)));
	CHECK(g_clicks == 2 && g_clicked == 0);
	Zroot_destroy(root);
}

/* A button on a page: a tap is its Click; a drag that starts on it, from the 13th px of the move on, is the pivot's, and the button is not clicked. */
static void test_button_on_page(void) {
	Zid root = Zroot_create(480, 800), pivot = Zpivot_create(), button = Zbutton_create("go"), other = Zbutton_create("x");
	ZPoint p;
	int i;
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	Zbutton_on_click(button, on_button, NULL);
	add_page(pivot, "a", button);
	add_page(pivot, "b", other);
	Zpivot_on_selection_changed(pivot, on_change, NULL);
	Zroot_set_visual(root, pivot);
	Zroot_update_layout(root);
	g_button_clicks = g_changes = 0;
	p = center(button);
	tap(root, p);
	CHECK(g_button_clicks == 1);
	/* a move of exactly 12 px is not a drag yet, the ones after it are */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x - 12, p.y);
	CHECK(Zroot_get_pointer_over(root) != Z_INVALID_ID);
	for (i = 2; i <= 12; i++) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x - (float)(i * 20), p.y);
	CHECK(!Zbutton_is_pressed(button));
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x - 240, p.y);
	CHECK(g_button_clicks == 1);
	CHECK(Zpivot_get_selected_index(pivot) == 1 && g_changes == 1);
	/* the up of a drag does not leave the capture with the button */
	CHECK(!Zbutton_is_pressed(button));
	Zroot_destroy(root);
}

/* A press on the page where nothing takes it (the pivot has the pointer from the start) follows the finger too. */
static void test_press_on_empty_page(void) {
	Zid root = Zroot_create(480, 800), pivot = Zpivot_create();
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	Zpivot_add_item(pivot, Zpivot_item_create("a"));
	Zpivot_add_item(pivot, Zpivot_item_create("b"));
	Zpivot_add_item(pivot, Zpivot_item_create("c"));
	Zroot_set_visual(root, pivot);
	Zroot_update_layout(root);
	drag(root, (ZPoint){ 300, 500 }, -200, -20);
	CHECK(Zpivot_get_selected_index(pivot) == 1);
	drag(root, (ZPoint){ 100, 500 }, 200, 20);
	CHECK(Zpivot_get_selected_index(pivot) == 0);
	Zroot_destroy(root);
}

static void test_items_added_and_removed(void) {
	Zid root = Zroot_create(480, 800), pivot = Zpivot_create(), a = Zpivot_item_create("a"), b = Zpivot_item_create("b"), c = Zpivot_item_create("c"), z = Zpivot_item_create("z");
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	Zpivot_add_item(pivot, a);
	Zpivot_add_item(pivot, b);
	Zpivot_add_item(pivot, c);
	Zroot_set_visual(root, pivot);
	Zpivot_set_selected_index(pivot, 1);
	/* an item before the chosen one: the chosen one is still b, now the third */
	CHECK(Zpivot_insert_item(pivot, 0, z) == 0);
	CHECK(Zpivot_get_selected_index(pivot) == 2 && Zpivot_get_selected_item(pivot) == b);
	Zpivot_remove_item_at(pivot, 0);
	CHECK(Zpivot_get_item_count(pivot) == 3 && Zpivot_get_selected_item(pivot) == b);
	/* the chosen one goes: the one that takes its place is chosen */
	Zpivot_remove_item_at(pivot, 1);
	CHECK(Zpivot_get_item_count(pivot) == 2 && Zpivot_get_selected_item(pivot) == c);
	Zpivot_clear_items(pivot);
	CHECK(Zpivot_get_item_count(pivot) == 0 && Zpivot_get_selected_index(pivot) == -1);
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

static void test_picture(void) {
	Zid root;
	char path[512];
	setup(&root, Z_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/pivot_system.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zpivot_set_selected_index(g_pivot, 1);
	snprintf(path, sizeof path, "%s/pivot_applications.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zroot_destroy(root);
}

int main(void) {
	RUN(test_classes);
	RUN(test_layout);
	RUN(test_header_colors);
	RUN(test_tap_header);
	RUN(test_drag);
	RUN(test_flick);
	RUN(test_ring_of_three);
	RUN(test_slide);
	RUN(test_locked);
	RUN(test_item_click);
	RUN(test_button_on_page);
	RUN(test_press_on_empty_page);
	RUN(test_items_added_and_removed);
	RUN(test_picture);
	return test_finish("test_pivot");
}
