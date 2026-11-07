/**
 * Pivot and PivotItem of Windows Phone 8.1, and the ItemClick of a ListView in one: the Settings of the phone,
 * "system | applications". Every check goes through what a finger would do (Kroot_inject_pointer), the clock, or a pixel.
 */

#include "test.h"

static int g_changes, g_clicks, g_clicked, g_button_clicks, g_loading, g_loaded, g_last_loading, g_last_loaded;

static void on_change(Kid sender, void* u) { (void)sender; (void)u; g_changes++; }
static void on_item_click(Kid list, int index, void* u) { (void)list; (void)u; g_clicks++; g_clicked = index; }
static void on_button(Kid sender, void* u) { (void)sender; (void)u; g_button_clicks++; }
static void on_loading(Kid pivot, Kid item, void* u) { (void)pivot; (void)u; g_loading++; g_last_loading = (int)item; }
static void on_loaded(Kid pivot, Kid item, void* u) { (void)pivot; (void)u; g_loaded++; g_last_loaded = (int)item; }

static Kid make_list(const char* const* names, int n) {
	Kid list = Klist_view_create();
	int i;
	for (i = 0; i < n; i++) {
		Kid item = Klist_view_item_create(NULL), t = Ktext_block_create(names[i]);
		Ktext_block_set_style(t, K_TEXT_STYLE_PHONE_LARGE);
		Kelement_set_margin(t, Kthickness(0, 0, 0, 12));
		Kcontent_control_set_content(item, t);
		Kitems_control_add(list, item);
	}
	/* a row of a list of settings opens something: nothing is selected, a tap is an ItemClick */
	Klist_view_set_selection_mode(list, K_LIST_VIEW_SELECTION_NONE);
	Klist_view_set_is_item_click_enabled(list, K_TRUE);
	return list;
}

static const char* const SYSTEM[] = { "wi-fi", "bluetooth", "screen lock", "ringtones + sounds", "notifications + actions" };
static const char* const APPS[] = { "games", "maps", "music + videos", "photos", "store" };

static Kid g_pivot, g_system, g_apps, g_item0, g_item1;

static Kid add_page(Kid pivot, const char* header, Kid content) {
	Kid item = Kpivot_item_create(header);
	Kcontent_control_set_content(item, content);
	CHECK(Kpivot_add_item(pivot, item) >= 0);
	return item;
}

static Kid setup(Kid* root, KAnimations anim) {
	*root = Kroot_create(480, 800);
	Kroot_set_renderer(*root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(*root, anim);
	g_pivot = Kpivot_create();
	Kpivot_set_title(g_pivot, "SETTINGS");
	g_system = make_list(SYSTEM, 5);
	g_apps = make_list(APPS, 5);
	g_item0 = add_page(g_pivot, "system", g_system);
	g_item1 = add_page(g_pivot, "applications", g_apps);
	CHECK(Kpivot_get_item_count(g_pivot) == 2);
	Kroot_set_visual(*root, g_pivot);
	Kroot_update_layout(*root);
	g_changes = g_clicks = g_button_clicks = g_loading = g_loaded = 0;
	Kpivot_on_selection_changed(g_pivot, on_change, NULL);
	Klist_view_on_item_click(g_system, on_item_click, NULL);
	return g_pivot;
}

static void run(Kid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static KPoint rect_center(KRect r) { return (KPoint){ r.x + r.width * 0.5f, r.y + r.height * 0.5f }; }

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

/* A finger down at `p`, drawn `dx` px in steps of `step`, and lifted. */
static void drag(Kid root, KPoint p, float dx, float step) {
	float x;
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	for (x = step; fabsf(x) <= fabsf(dx); x += step) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x + x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x + dx, p.y);
}

/* The brightest pixel (the largest red) in a rectangle of the root. */
static int brightest(Kid root, KRect r) {
	KFrame f;
	int x, y, best = 0;
	Kroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && px[0] > best) best = px[0];
		}
	}
	return best;
}

/* The darkest pixel (the smallest red) in a rectangle of the root. */
static int darkest(Kid root, KRect r) {
	KFrame f;
	int x, y, best = 255;
	Kroot_render(root, &f);
	for (y = (int)r.y; y < (int)(r.y + r.height) && y < f.height; y++) {
		for (x = (int)r.x; x < (int)(r.x + r.width) && x < f.width; x++) {
			const unsigned char* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
			if (x >= 0 && px[0] < best) best = px[0];
		}
	}
	return best;
}

static void quiet(KError error, const char* message, void* user) { (void)error; (void)message; (void)user; }

static void test_classes(void) {
	Kid pivot = Kpivot_create(), item = Kpivot_item_create("one");
	CHECK(Kelement_is_a(pivot, K_CLASS_CONTROL));
	CHECK(Kelement_is_a(item, K_CLASS_CONTENT_CONTROL) && Kelement_is_a(item, K_CLASS_PIVOT_ITEM));
	CHECK_STR(Kpivot_item_get_header(item), "one");
	Kpivot_item_set_header(item, "two");
	CHECK_STR(Kpivot_item_get_header(item), "two");
	CHECK(Kpivot_get_selected_index(pivot) == -1);
	CHECK(Kpivot_get_selected_item(pivot) == K_INVALID_ID);
	CHECK(Kpivot_add_item(pivot, item) == 0);
	CHECK(Kpivot_get_selected_index(pivot) == 0 && Kpivot_get_selected_item(pivot) == item);
	/* an item is in one pivot only */
	{
		Kid other = Kpivot_create();
		Kset_error_callback(quiet, NULL);
		CHECK(Kpivot_add_item(other, item) == -1);
		Kset_error_callback(NULL, NULL);
		Kelement_destroy(other);
	}
	CHECK(!Kpivot_get_is_locked(pivot));
	Kpivot_set_title(pivot, "TITLE");
	CHECK_STR(Kpivot_get_title(pivot), "TITLE");
	Kelement_destroy(pivot);
}

static void test_layout(void) {
	Kid root;
	KRect r;
	setup(&root, K_ANIMATIONS_OFF);
	CHECK(Kpivot_get_selected_index(g_pivot) == 0);
	CHECK(Kpivot_get_item(g_pivot, 1) == g_item1);
	/* the page of the chosen item is in view, the other is not */
	CHECK(Kelement_get_visibility(g_item0) == K_VISIBLE);
	CHECK(Kelement_get_visibility(g_item1) == K_COLLAPSED);
	/* PivotItemMargin 19,22.5,19,0: the page is 442 wide, and starts below the title and the strip */
	r = Kelement_get_bounds(g_item0);
	CHECK_NEAR(r.x, 19);
	CHECK(r.y > 120);
	CHECK_NEAR(r.width, 442);
	/* the header of the chosen item is under the title, at 19; the next one follows it, 16 px of margin after the text */
	{
		KRect h0 = Kpivot_get_header_rect(g_pivot, 0), h1 = Kpivot_get_header_rect(g_pivot, 1);
		CHECK_NEAR(h0.x, 19);
		CHECK(h0.y > 55 && h0.y < 70);
		CHECK_NEAR(h1.x, h0.x + h0.width + 16);
		CHECK_NEAR(r.y, h0.y + h0.height + 22.5f);
	}
	Kroot_destroy(root);
}

static void test_header_colors(void) {
	KTheme themes[2] = { K_THEME_DARK, K_THEME_LIGHT };
	int t;
	for (t = 0; t < 2; t++) {
		Kid root;
		KRect h0, h1;
		setup(&root, K_ANIMATIONS_OFF);
		Kroot_set_theme(root, themes[t]);
		Kroot_update_layout(root);
		h0 = Kpivot_get_header_rect(g_pivot, 0);
		h1 = Kpivot_get_header_rect(g_pivot, 1);
		if (themes[t] == K_THEME_DARK) {
			/* PivotHeaderForegroundSelected is PhoneControlForeground, white; the others PhoneControlDisabled, white 40% */
			CHECK(brightest(root, h0) >= 250);
			CHECK_NEAR(brightest(root, h1), 0x66);
		} else {
			CHECK(darkest(root, h0) <= 5);
			CHECK_NEAR(darkest(root, h1), 255 - 0x66);
		}
		/* the title is in the foreground */
		{
			KRect title = { 19, 38, 120, 20 };
			if (themes[t] == K_THEME_DARK) CHECK(brightest(root, title) >= 250);
			else CHECK(darkest(root, title) <= 5);
		}
		Kroot_destroy(root);
	}
}

static void test_tap_header(void) {
	Kid root;
	setup(&root, K_ANIMATIONS_OFF);
	/* the header "applications" is to the right of "system", at the top */
	tap(root, rect_center(Kpivot_get_header_rect(g_pivot, 1)));
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	CHECK(Kpivot_get_selected_item(g_pivot) == g_item1);
	CHECK(g_changes == 1);
	Kroot_update_layout(root);
	CHECK(Kelement_get_visibility(g_item1) == K_VISIBLE);
	CHECK(Kelement_get_visibility(g_item0) == K_COLLAPSED);
	CHECK_NEAR(Kelement_get_bounds(g_item1).x, 19);
	/* the chosen header is at 19 now, and the strip has gone round: "system" is after it */
	{
		KRect h1 = Kpivot_get_header_rect(g_pivot, 1);
		CHECK_NEAR(h1.x, 19);
	}
	/* and back: a tap on the header of the first page, that has come round after the second */
	{
		KRect h0 = Kpivot_get_header_rect(g_pivot, 0);
		CHECK(h0.width > 0 && h0.x > 19);
		tap(root, rect_center(h0));
	}
	CHECK(Kpivot_get_selected_index(g_pivot) == 0);
	CHECK(g_changes == 2);
	/* a tap on the chosen header changes nothing */
	tap(root, rect_center(Kpivot_get_header_rect(g_pivot, 0)));
	CHECK(Kpivot_get_selected_index(g_pivot) == 0 && g_changes == 2);
	Kroot_destroy(root);
}

static void test_drag(void) {
	Kid root;
	KPoint p;
	setup(&root, K_ANIMATIONS_OFF);
	p = center(Kitems_control_get_container(g_system, 0));
	/* a finger on a row, drawn left 240 px: a row of the list does not take it, the pivot does */
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	{
		int x;
		for (x = 0; x <= 240; x += 20) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x - (float)x, p.y);
	}
	Kroot_update_layout(root);
	{
		/* the pages are following the finger: half a page to the left, and the header strip with them */
		KRect r = Kelement_get_bounds(g_item0);
		CHECK(r.x < -150);
		CHECK(Kelement_get_visibility(g_item1) == K_VISIBLE);
		CHECK(Kpivot_get_header_rect(g_pivot, 0).x < 19 - 50);
	}
	Kroot_inject_pointer(root, K_POINTER_UP, p.x - 240, p.y);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	CHECK(g_clicks == 0);
	CHECK(g_changes == 1);
	/* a short drag goes back to where it was */
	Kroot_update_layout(root);
	p = center(Kitems_control_get_container(g_apps, 0));
	drag(root, p, 40, 2);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(g_item1).x, 19);
	/* a swipe past the last page goes round to the first */
	drag(root, p, -300, -20);
	CHECK(Kpivot_get_selected_index(g_pivot) == 0);
	CHECK(g_changes == 2);
	/* and a swipe to the right from the first goes round to the last */
	Kroot_update_layout(root);
	p = center(Kitems_control_get_container(g_system, 0));
	drag(root, p, 300, 20);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	CHECK(g_changes == 3);
	Kroot_destroy(root);
}

static void test_flick(void) {
	Kid root;
	KPoint p;
	setup(&root, K_ANIMATIONS_OFF);
	/* a short and fast flick: 60 px in two moves goes to the next page */
	p = center(Kitems_control_get_container(g_system, 0));
	drag(root, p, -60, -30);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	Kroot_destroy(root);
}

static void test_ring_of_three(void) {
	Kid root = Kroot_create(480, 800), pivot = Kpivot_create(), items[3];
	const char* names[3] = { "one", "two", "three" };
	int i;
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	for (i = 0; i < 3; i++) {
		items[i] = Kpivot_item_create(names[i]);
		Kcontent_control_set_text(items[i], names[i]);
		Kpivot_add_item(pivot, items[i]);
	}
	Kroot_set_visual(root, pivot);
	Kroot_update_layout(root);
	Kpivot_set_selected_index(pivot, 1);
	Kroot_update_layout(root);
	{
		/* "two" is at the left, "three" after it and "one" last, as the ring of the PivotHeaderPanel goes */
		KRect a = Kpivot_get_header_rect(pivot, 0), b = Kpivot_get_header_rect(pivot, 1), c = Kpivot_get_header_rect(pivot, 2);
		CHECK_NEAR(b.x, 19);
		CHECK(c.x > b.x && a.x > c.x);
	}
	/* from the first to the last the short way is back */
	Kpivot_set_selected_index(pivot, 0);
	Kpivot_set_selected_index(pivot, 2);
	Kroot_update_layout(root);
	CHECK(Kpivot_get_selected_index(pivot) == 2);
	CHECK_NEAR(Kpivot_get_header_rect(pivot, 2).x, 19);
	Kroot_destroy(root);
}

static void test_slide(void) {
	Kid root;
	setup(&root, K_ANIMATIONS_ON);
	Kpivot_on_pivot_item_loading(g_pivot, on_loading, NULL);
	Kpivot_on_pivot_item_loaded(g_pivot, on_loaded, NULL);
	Kpivot_set_selected_index(g_pivot, 1);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1);
	/* Loading comes with the choice, Loaded when the page has arrived */
	CHECK(g_loading == 1 && g_last_loading == (int)g_item1);
	CHECK(g_loaded == 0);
	run(root, 64);
	Kroot_update_layout(root);
	{
		/* on its way: the first page has left a little, the second is coming, the strip is on the move */
		KRect a = Kelement_get_bounds(g_item0), b = Kelement_get_bounds(g_item1);
		CHECK(a.x < 19 && a.x > -442);
		CHECK(b.x > 19 && b.x < 480);
		CHECK(Kpivot_get_header_rect(g_pivot, 0).x < 19 && Kpivot_get_header_rect(g_pivot, 0).x > -200);
	}
	CHECK(g_loaded == 0);
	run(root, 400);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(g_item1).x, 19);
	CHECK(g_loaded == 1 && g_last_loaded == (int)g_item1);
	/* without animations all at once */
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	Kpivot_set_selected_index(g_pivot, 0);
	CHECK(g_loading == 2 && g_loaded == 2 && g_last_loaded == (int)g_item0);
	Kroot_destroy(root);
}

static void test_locked(void) {
	Kid root;
	KPoint p;
	setup(&root, K_ANIMATIONS_OFF);
	Kpivot_set_is_locked(g_pivot, K_TRUE);
	CHECK(Kpivot_get_is_locked(g_pivot));
	Kroot_update_layout(root);
	/* the headers of the other items are not shown */
	CHECK(Kpivot_get_header_rect(g_pivot, 1).width == 0);
	CHECK(Kpivot_get_header_rect(g_pivot, 0).width > 0);
	p = center(Kitems_control_get_container(g_system, 0));
	drag(root, p, -300, -20);
	CHECK(Kpivot_get_selected_index(g_pivot) == 0 && g_changes == 0);
	tap(root, rect_center(Kpivot_get_header_rect(g_pivot, 0)));
	CHECK(g_changes == 0);
	/* the program may still go on */
	Kpivot_set_selected_index(g_pivot, 1);
	CHECK(Kpivot_get_selected_index(g_pivot) == 1 && g_changes == 1);
	Kpivot_set_is_locked(g_pivot, K_FALSE);
	Kroot_update_layout(root);
	CHECK(Kpivot_get_header_rect(g_pivot, 0).width > 0);
	Kroot_destroy(root);
}

static void test_item_click(void) {
	Kid root, item;
	setup(&root, K_ANIMATIONS_OFF);
	item = Kitems_control_get_container(g_system, 2);
	tap(root, center(item));
	CHECK(g_clicks == 1);
	CHECK(g_clicked == 2);
	/* a row opens something and does not stay chosen */
	CHECK(Kselector_get_selected_index(g_system) == -1);
	tap(root, center(Kitems_control_get_container(g_system, 0)));
	CHECK(g_clicks == 2 && g_clicked == 0);
	Kroot_destroy(root);
}

/* A button on a page: a tap is its Click; a drag that starts on it, from the 13th px of the move on, is the pivot's, and the button is not clicked. */
static void test_button_on_page(void) {
	Kid root = Kroot_create(480, 800), pivot = Kpivot_create(), button = Kbutton_create("go"), other = Kbutton_create("x");
	KPoint p;
	int i;
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	Kbutton_on_click(button, on_button, NULL);
	add_page(pivot, "a", button);
	add_page(pivot, "b", other);
	Kpivot_on_selection_changed(pivot, on_change, NULL);
	Kroot_set_visual(root, pivot);
	Kroot_update_layout(root);
	g_button_clicks = g_changes = 0;
	p = center(button);
	tap(root, p);
	CHECK(g_button_clicks == 1);
	/* a move of exactly 12 px is not a drag yet, the ones after it are */
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x - 12, p.y);
	CHECK(Kroot_get_pointer_over(root) != K_INVALID_ID);
	for (i = 2; i <= 12; i++) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x - (float)(i * 20), p.y);
	CHECK(!Kbutton_is_pressed(button));
	Kroot_inject_pointer(root, K_POINTER_UP, p.x - 240, p.y);
	CHECK(g_button_clicks == 1);
	CHECK(Kpivot_get_selected_index(pivot) == 1 && g_changes == 1);
	/* the up of a drag does not leave the capture with the button */
	CHECK(!Kbutton_is_pressed(button));
	Kroot_destroy(root);
}

/* A press on the page where nothing takes it (the pivot has the pointer from the start) follows the finger too. */
static void test_press_on_empty_page(void) {
	Kid root = Kroot_create(480, 800), pivot = Kpivot_create();
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	Kpivot_add_item(pivot, Kpivot_item_create("a"));
	Kpivot_add_item(pivot, Kpivot_item_create("b"));
	Kpivot_add_item(pivot, Kpivot_item_create("c"));
	Kroot_set_visual(root, pivot);
	Kroot_update_layout(root);
	drag(root, (KPoint){ 300, 500 }, -200, -20);
	CHECK(Kpivot_get_selected_index(pivot) == 1);
	drag(root, (KPoint){ 100, 500 }, 200, 20);
	CHECK(Kpivot_get_selected_index(pivot) == 0);
	Kroot_destroy(root);
}

static void test_items_added_and_removed(void) {
	Kid root = Kroot_create(480, 800), pivot = Kpivot_create(), a = Kpivot_item_create("a"), b = Kpivot_item_create("b"), c = Kpivot_item_create("c"), z = Kpivot_item_create("z");
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	Kpivot_add_item(pivot, a);
	Kpivot_add_item(pivot, b);
	Kpivot_add_item(pivot, c);
	Kroot_set_visual(root, pivot);
	Kpivot_set_selected_index(pivot, 1);
	/* an item before the chosen one: the chosen one is still b, now the third */
	CHECK(Kpivot_insert_item(pivot, 0, z) == 0);
	CHECK(Kpivot_get_selected_index(pivot) == 2 && Kpivot_get_selected_item(pivot) == b);
	Kpivot_remove_item_at(pivot, 0);
	CHECK(Kpivot_get_item_count(pivot) == 3 && Kpivot_get_selected_item(pivot) == b);
	/* the chosen one goes: the one that takes its place is chosen */
	Kpivot_remove_item_at(pivot, 1);
	CHECK(Kpivot_get_item_count(pivot) == 2 && Kpivot_get_selected_item(pivot) == c);
	Kpivot_clear_items(pivot);
	CHECK(Kpivot_get_item_count(pivot) == 0 && Kpivot_get_selected_index(pivot) == -1);
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

static void test_picture(void) {
	Kid root;
	char path[512];
	setup(&root, K_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/pivot_system.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kpivot_set_selected_index(g_pivot, 1);
	snprintf(path, sizeof path, "%s/pivot_applications.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kroot_destroy(root);
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
