/**
 * LoopingSelector, DatePicker, TimePicker and their flyouts of Windows Phone 8.1. Every check goes through what a finger would do
 * (Kroot_inject_pointer on the button, on the selectors, on Done and Cancel), the keys, the clock, or a pixel of the rendered page
 * (Dark theme, Cobalt accent). The dates are always set by the test: nothing here depends on today.
 */

#include "test.h"

static int g_changes, g_date_events, g_time_events, g_picked, g_closed;
static KDate g_old_date, g_new_date;
static KTimeOfDay g_old_time, g_new_time;

static void on_change(Kid sender, void* u) { (void)sender; (void)u; g_changes++; }
static void on_closed(Kid sender, void* u) { (void)sender; (void)u; g_closed++; }
static void on_date(Kid sender, KDate o, KDate n, void* u) { (void)sender; (void)u; g_date_events++; g_old_date = o; g_new_date = n; }
static void on_time(Kid sender, KTimeOfDay o, KTimeOfDay n, void* u) { (void)sender; (void)u; g_time_events++; g_old_time = o; g_new_time = n; }
static void on_date_picked(Kid sender, KDate o, KDate n, void* u) { (void)sender; (void)u; g_picked++; g_old_date = o; g_new_date = n; }

static void run(Kid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Kid root, KPoint p) {
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
}

static void key(Kid root, KKey k) { Kroot_inject_key(root, k, 0, 1); Kroot_inject_key(root, k, 0, 0); }

static int pixel_is(Kid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

static int same_date(KDate a, KDate b) { return a.year == b.year && a.month == b.month && a.day == b.day; }
static int same_time(KTimeOfDay a, KTimeOfDay b) { return a.hour == b.hour && a.minute == b.minute; }

static Kid make_root(Kid visual, KAnimations animations) {
	Kid root = Kroot_create(480, 800);
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, animations);
	Kroot_set_visual(root, visual);
	Kroot_update_layout(root);
	return root;
}

/* A finger on a selector goes `dy` px up or down in steps of 10 and rests there before it lets go, so that no inertia is left. */
static void drag_selector(Kid root, Kid selector, float dy) {
	KPoint p = center(selector);
	float y;
	int i;
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	for (y = dy > 0 ? 10 : -10; fabsf(y) <= fabsf(dy); y += dy > 0 ? 10 : -10) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y + y);
	for (i = 0; i < 8; i++) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y + dy);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y + dy);
}

static Kid make_selector(int count, Kid* root, KAnimations animations) {
	Kid sel = Klooping_selector_create();
	int i;
	for (i = 0; i < count; i++) {
		char text[16];
		snprintf(text, sizeof text, "%d", i + 1);
		Klooping_selector_add_item(sel, text, NULL);
	}
	Kelement_set_horizontal_alignment(sel, K_HALIGN_LEFT);
	*root = make_root(sel, animations);
	g_changes = 0;
	Klooping_selector_on_selection_changed(sel, on_change, NULL);
	return sel;
}

static void test_classes(void) {
	Kid sel = Klooping_selector_create(), dp = Kdate_picker_create(), tp = Ktime_picker_create(), df = Kdate_picker_flyout_create(), tf = Ktime_picker_flyout_create();
	CHECK(Kelement_is_a(sel, K_CLASS_CONTROL) && Kelement_is_a(dp, K_CLASS_CONTROL) && Kelement_is_a(tp, K_CLASS_CONTROL));
	CHECK(Kelement_is_a(df, K_CLASS_PICKER_FLYOUT_BASE) && Kelement_is_a(df, K_CLASS_FLYOUT_BASE) && Kelement_is_a(tf, K_CLASS_PICKER_FLYOUT_BASE));
	CHECK(Kflyout_base_get_placement(df) == K_FLYOUT_PLACEMENT_FULL);
	CHECK(Klooping_selector_get_should_loop(sel) && Klooping_selector_get_selected_index(sel) == -1 && Klooping_selector_get_item_count(sel) == 0);
	CHECK(Kdate_picker_get_day_visible(dp) && Kdate_picker_get_month_visible(dp) && Kdate_picker_get_year_visible(dp));
	CHECK(Ktime_picker_get_clock_identifier(tp) == K_CLOCK_12_HOUR && Ktime_picker_get_minute_increment(tp) == 1);
	CHECK(Kdate_picker_get_flyout(dp) != K_INVALID_ID && Kdate_picker_get_flyout_button(dp) != K_INVALID_ID);
	CHECK(Kelement_is_a(Kdate_picker_get_flyout_button(dp), K_CLASS_BUTTON));
	CHECK(Kdate_picker_get_max_year(dp) - Kdate_picker_get_min_year(dp) == 200);
	Kelement_destroy(sel);
	Kelement_destroy(dp);
	Kelement_destroy(tp);
	Kelement_destroy(df);
	Kelement_destroy(tf);
}

/* ---- LoopingSelector ---- */

static void test_selector_look(void) {
	Kid root, sel = make_selector(5, &root, K_ANIMATIONS_OFF);
	KRect r = Kelement_get_bounds(sel);
	float ch = Klooping_selector_get_item_height(sel);
	int cy = (int)(r.y + r.height * 0.5f);
	CHECK(Klooping_selector_get_selected_index(sel) == 0);
	CHECK(Klooping_selector_get_item_count(sel) == 5);
	CHECK_STR(Klooping_selector_get_item_text(sel, 2), "3");
	CHECK(r.width >= 64);
	/* the item of a 45 px line: 5 + 2.5 + 5 + the line less its margins + 5 + 2.5 + 5 */
	CHECK(ch > 55 && ch < 80);
	/* at rest only the chosen item shows: the accent in the middle, the page above and below */
	CHECK(pixel_is(root, (int)r.x + 8, cy, 0x00, 0x50, 0xEF));
	CHECK(pixel_is(root, (int)r.x + 8, cy - (int)ch, 0, 0, 0));
	CHECK(pixel_is(root, (int)r.x + 8, cy + (int)ch, 0, 0, 0));
	/* its box has the margin of 5 */
	CHECK(pixel_is(root, (int)r.x + 3, cy, 0, 0, 0));
	/* a finger down: the others come out, outlined in LoopingSelectorForeground (white 45%) */
	{
		KPoint p = center(sel);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		/* the border of the item below: 2.5 px at the left of its box, 5 px in */
		CHECK(pixel_is(root, (int)r.x + 6, cy + (int)ch, 0x73, 0x73, 0x73));
		CHECK(pixel_is(root, (int)r.x + 8, cy, 0x00, 0x50, 0xEF));
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y);
		/* and lets go: it is as it was */
		CHECK(pixel_is(root, (int)r.x + 6, cy + (int)ch, 0, 0, 0));
	}
	Kroot_destroy(root);
}

static void test_selector_keys_and_loop(void) {
	Kid root, sel = make_selector(5, &root, K_ANIMATIONS_OFF);
	Kelement_focus(sel);
	key(root, K_KEY_DOWN);
	CHECK(Klooping_selector_get_selected_index(sel) == 1 && g_changes == 1);
	key(root, K_KEY_UP);
	key(root, K_KEY_UP);
	/* it goes round: from the first to the last */
	CHECK(Klooping_selector_get_selected_index(sel) == 4 && g_changes == 3);
	key(root, K_KEY_DOWN);
	CHECK(Klooping_selector_get_selected_index(sel) == 0);
	/* without ShouldLoop it stops at the ends */
	Klooping_selector_set_should_loop(sel, K_FALSE);
	CHECK(!Klooping_selector_get_should_loop(sel));
	key(root, K_KEY_UP);
	CHECK(Klooping_selector_get_selected_index(sel) == 0);
	Klooping_selector_set_selected_index(sel, 4);
	CHECK(g_changes == 5);
	key(root, K_KEY_DOWN);
	CHECK(Klooping_selector_get_selected_index(sel) == 4);
	/* the program sets the index in range */
	Klooping_selector_set_selected_index(sel, 40);
	CHECK(Klooping_selector_get_selected_index(sel) == 4);
	Kroot_destroy(root);
}

static void test_selector_drag_and_tap(void) {
	Kid root, sel = make_selector(8, &root, K_ANIMATIONS_OFF);
	float ch = Klooping_selector_get_item_height(sel);
	KRect r = Kelement_get_bounds(sel);
	/* a finger drawn up by three items: the chosen one is the fourth */
	drag_selector(root, sel, -3 * ch);
	CHECK(Klooping_selector_get_selected_index(sel) == 3 && g_changes == 1);
	/* and down by five: it goes round the beginning, to the seventh */
	drag_selector(root, sel, 5 * ch);
	CHECK(Klooping_selector_get_selected_index(sel) == 6 && g_changes == 2);
	/* less than half an item goes back */
	drag_selector(root, sel, 0.3f * ch);
	CHECK(Klooping_selector_get_selected_index(sel) == 6 && g_changes == 2);
	/* a tap on the item above the middle chooses it */
	{
		KPoint p = center(sel);
		tap(root, (KPoint){ p.x, p.y - ch });
		CHECK(Klooping_selector_get_selected_index(sel) == 5 && g_changes == 3);
		/* a tap on the item in the middle changes nothing */
		tap(root, p);
		CHECK(Klooping_selector_get_selected_index(sel) == 5 && g_changes == 3);
		/* two below */
		tap(root, (KPoint){ p.x, p.y + 2 * ch });
		CHECK(Klooping_selector_get_selected_index(sel) == 7);
	}
	/* the accent box is in the middle again after every move */
	CHECK(pixel_is(root, (int)r.x + 8, (int)(r.y + r.height * 0.5f), 0x00, 0x50, 0xEF));
	/* without ShouldLoop the column stays between the first and the last item */
	Klooping_selector_set_should_loop(sel, K_FALSE);
	drag_selector(root, sel, -6 * ch);
	CHECK(Klooping_selector_get_selected_index(sel) == 7);
	drag_selector(root, sel, 20 * ch);
	CHECK(Klooping_selector_get_selected_index(sel) == 0);
	Kroot_destroy(root);
}

static void test_selector_fling_and_snap(void) {
	Kid root, sel = make_selector(40, &root, K_ANIMATIONS_ON);
	float ch = Klooping_selector_get_item_height(sel);
	KRect r = Kelement_get_bounds(sel);
	KPoint p = center(sel);
	int i, before;
	/* a quick flick up: the column goes on by itself, slows down, and ends on an item */
	Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
	for (i = 1; i <= 6; i++) Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 20.0f * (float)i);
	Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y - 120);
	CHECK(g_changes == 0);
	run(root, 100);
	before = g_changes;
	/* not chosen while it moves */
	CHECK(before == 0);
	run(root, 3000);
	CHECK(g_changes == 1);
	CHECK(Klooping_selector_get_selected_index(sel) > 1);
	/* at rest on the item: the accent in the middle, the others hidden again (the Expanded state ends in 330 ms) */
	CHECK(pixel_is(root, (int)r.x + 8, (int)(r.y + r.height * 0.5f), 0x00, 0x50, 0xEF));
	CHECK(pixel_is(root, (int)r.x + 6, (int)(r.y + r.height * 0.5f + ch), 0, 0, 0));
	/* a tap on a neighbour goes to it in 160 ms */
	{
		int was = Klooping_selector_get_selected_index(sel);
		tap(root, (KPoint){ p.x, p.y + ch });
		run(root, 400);
		CHECK(Klooping_selector_get_selected_index(sel) == was + 1);
		CHECK(g_changes == 2);
	}
	Kroot_destroy(root);
}

static void test_selector_secondary_text(void) {
	Kid root, sel = Klooping_selector_create();
	KRect r;
	Klooping_selector_add_item(sel, "7", "Sat");
	Klooping_selector_add_item(sel, "8", "Sun");
	Kelement_set_horizontal_alignment(sel, K_HALIGN_LEFT);
	root = make_root(sel, K_ANIMATIONS_OFF);
	CHECK_STR(Klooping_selector_get_item_secondary_text(sel, 1), "Sun");
	CHECK_STR(Klooping_selector_get_item_secondary_text(sel, 5), "");
	/* the item is taller by the second line */
	r = Kelement_get_bounds(sel);
	CHECK(Klooping_selector_get_item_height(sel) > 75 && Klooping_selector_get_item_height(sel) < 100);
	(void)r;
	Klooping_selector_clear_items(sel);
	CHECK(Klooping_selector_get_item_count(sel) == 0 && Klooping_selector_get_selected_index(sel) == -1);
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

/* ---- DatePicker ---- */

static Kid g_dp, g_flyout, g_button;

static Kid date_setup(Kid* root, KDate date, KAnimations animations) {
	Kid st = Kstack_panel_create(K_VERTICAL);
	g_dp = Kdate_picker_create();
	Kdate_picker_set_header(g_dp, "birthday");
	Kdate_picker_set_date(g_dp, date);
	Kelement_set_margin(g_dp, Kthickness(19, 40, 19, 0));
	Kpanel_add(st, g_dp);
	*root = make_root(st, animations);
	g_flyout = Kdate_picker_get_flyout(g_dp);
	g_button = Kdate_picker_get_flyout_button(g_dp);
	g_date_events = g_picked = g_closed = 0;
	Kdate_picker_on_date_changed(g_dp, on_date, NULL);
	Kdate_picker_flyout_on_date_picked(g_flyout, on_date_picked, NULL);
	Kflyout_base_on_closed(g_flyout, on_closed, NULL);
	return g_dp;
}

static KDate D(int y, int m, int d) { KDate x = { y, m, d }; return x; }
static KTimeOfDay T(int h, int m) { KTimeOfDay x = { h, m }; return x; }

static void test_date_picker_look(void) {
	Kid root;
	KRect b, c;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	b = Kelement_get_bounds(g_button);
	c = Kelement_get_bounds(g_dp);
	/* the margin 19, the header over the button, the button as wide as the room less the margins */
	CHECK_NEAR(c.x, 19);
	CHECK_NEAR(c.width, 442);
	CHECK_NEAR(b.x, 0);
	CHECK_NEAR(b.width, 442);
	/* the button is a Button of 57.5 with its 9.5 above and below the outline, the header's line less 3 over it */
	CHECK(b.height >= 57.5f && b.height < 60);
	CHECK(b.y > 10 && b.y < 25);
	CHECK_STR(Kdate_picker_get_header(g_dp), "birthday");
	CHECK_STR(Kcontent_control_get_text(g_button), "November 7, 2026");
	/* 2.5 px outline in the foreground, the text inside it at 6.5 */
	{
		KPoint p = Kelement_to_root(g_button, (KPoint){ 0, 0 });
		int top = (int)p.y + 9 + 1, left = (int)p.x + 1;
		CHECK(pixel_is(root, left + 200, top, 255, 255, 255));
		CHECK(pixel_is(root, left, top + 15, 255, 255, 255));
		CHECK(pixel_is(root, left + 200, top + 10, 0, 0, 0));
	}
	/* Light: the outline is black */
	Kroot_set_theme(root, K_THEME_LIGHT);
	Kroot_update_layout(root);
	{
		KPoint p = Kelement_to_root(g_button, (KPoint){ 0, 0 });
		CHECK(pixel_is(root, (int)p.x + 201, (int)p.y + 10, 0, 0, 0));
		CHECK(pixel_is(root, (int)p.x + 201, (int)p.y + 20, 255, 255, 255));
	}
	Kroot_destroy(root);
}

static void test_date_set_and_coerce(void) {
	Kid root;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 7)));
	/* a date that is the same raises nothing */
	Kdate_picker_set_date(g_dp, D(2026, 11, 7));
	CHECK(g_date_events == 0);
	/* the leap day */
	Kdate_picker_set_date(g_dp, D(2024, 2, 29));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2024, 2, 29)) && g_date_events == 1);
	CHECK(same_date(g_old_date, D(2026, 11, 7)) && same_date(g_new_date, D(2024, 2, 29)));
	CHECK_STR(Kcontent_control_get_text(g_button), "February 29, 2024");
	/* the same day of a year that has no such day */
	Kdate_picker_set_date(g_dp, D(2023, 2, 29));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2023, 2, 28)));
	Kdate_picker_set_min_year(g_dp, 1800);
	Kdate_picker_set_date(g_dp, D(1900, 2, 29));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(1900, 2, 28)));
	Kdate_picker_set_date(g_dp, D(2000, 2, 29));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2000, 2, 29)));
	Kdate_picker_set_date(g_dp, D(2026, 4, 31));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 4, 30)));
	Kdate_picker_set_date(g_dp, D(2026, 13, 0));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 12, 1)));
	/* the years of the picker */
	Kdate_picker_set_min_year(g_dp, 2020);
	Kdate_picker_set_max_year(g_dp, 2030);
	Kdate_picker_set_date(g_dp, D(2050, 5, 5));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2030, 5, 5)));
	Kdate_picker_set_date(g_dp, D(1999, 5, 5));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2020, 5, 5)));
	CHECK(Kdate_picker_get_min_year(g_dp) == 2020 && Kdate_picker_get_max_year(g_dp) == 2030);
	Kroot_destroy(root);
}

static void open_flyout(Kid root, Kid button) {
	tap(root, center(button));
	Kroot_update_layout(root);
}

static void test_date_flyout_opens_on_the_date(void) {
	Kid root, month, day, year;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	Kdate_picker_set_min_year(g_dp, 2000);
	Kdate_picker_set_max_year(g_dp, 2040);
	CHECK(!Kflyout_base_get_is_open(g_flyout));
	open_flyout(root, g_button);
	CHECK(Kflyout_base_get_is_open(g_flyout));
	month = Kpicker_flyout_base_get_selector(g_flyout, 0);
	day = Kpicker_flyout_base_get_selector(g_flyout, 1);
	year = Kpicker_flyout_base_get_selector(g_flyout, 2);
	CHECK(Klooping_selector_get_item_count(month) == 12 && Klooping_selector_get_selected_index(month) == 10);
	CHECK_STR(Klooping_selector_get_item_text(month, 10), "November");
	CHECK(Klooping_selector_get_item_count(day) == 30 && Klooping_selector_get_selected_index(day) == 6);
	/* the 7th of November 2026 was a Saturday */
	CHECK_STR(Klooping_selector_get_item_text(day, 6), "7");
	CHECK_STR(Klooping_selector_get_item_secondary_text(day, 6), "Sat");
	CHECK_STR(Klooping_selector_get_item_secondary_text(day, 0), "Sun");
	CHECK(Klooping_selector_get_item_count(year) == 41 && Klooping_selector_get_selected_index(year) == 26);
	CHECK_STR(Klooping_selector_get_item_text(year, 0), "2000");
	CHECK(!Klooping_selector_get_should_loop(year) && Klooping_selector_get_should_loop(month) && Klooping_selector_get_should_loop(day));
	/* the title is the header of the picker */
	CHECK_STR(Kpicker_flyout_base_get_title(g_flyout), "birthday");
	/* three selectors side by side, the page around them: chrome */
	{
		KRect rm = Kelement_get_bounds(month), rd = Kelement_get_bounds(day), ry = Kelement_get_bounds(year);
		CHECK(rm.width > 150 && rd.width >= 64 && ry.width >= 64);
		CHECK(Kelement_to_root(month, (KPoint){ 0, 0 }).x < Kelement_to_root(day, (KPoint){ 0, 0 }).x);
		CHECK(Kelement_to_root(day, (KPoint){ 0, 0 }).x < Kelement_to_root(year, (KPoint){ 0, 0 }).x);
		CHECK(rm.height > 400);
		CHECK(pixel_is(root, 5, 400, 0x1F, 0x1F, 0x1F));
	}
	/* the buttons are at the bottom */
	{
		KPoint d = center(Kpicker_flyout_base_get_done_button(g_flyout)), c = center(Kpicker_flyout_base_get_cancel_button(g_flyout));
		CHECK(d.y > 700 && c.y > 700 && d.x < 240 && c.x > 240);
	}
	Kroot_destroy(root);
}

static void test_date_flyout_done(void) {
	Kid root, month, day, year;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Kpicker_flyout_base_get_selector(g_flyout, 0);
	day = Kpicker_flyout_base_get_selector(g_flyout, 1);
	year = Kpicker_flyout_base_get_selector(g_flyout, 2);
	/* a finger on the day selector: two items up, the 9th */
	drag_selector(root, day, -2 * Klooping_selector_get_item_height(day));
	CHECK(Klooping_selector_get_selected_index(day) == 8);
	/* the picker does not know yet */
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 9)));
	CHECK(g_date_events == 1 && same_date(g_old_date, D(2026, 11, 7)) && same_date(g_new_date, D(2026, 11, 9)));
	CHECK(g_picked == 1);
	CHECK(!Kflyout_base_get_is_open(g_flyout) && g_closed == 1);
	CHECK_STR(Kcontent_control_get_text(g_button), "November 9, 2026");
	/* Done with nothing changed: DatePicked, no DateChanged */
	open_flyout(root, g_button);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(g_picked == 2 && g_date_events == 1 && !Kflyout_base_get_is_open(g_flyout));
	/* a change by the year selector */
	open_flyout(root, g_button);
	year = Kpicker_flyout_base_get_selector(g_flyout, 2);
	(void)month;
	Klooping_selector_set_selected_index(year, Klooping_selector_get_selected_index(year) + 2);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2028, 11, 9)));
	Kroot_destroy(root);
}

static void test_date_flyout_cancel_and_back(void) {
	Kid root, day;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	day = Kpicker_flyout_base_get_selector(g_flyout, 1);
	Klooping_selector_set_selected_index(day, 20);
	tap(root, center(Kpicker_flyout_base_get_cancel_button(g_flyout)));
	CHECK(!Kflyout_base_get_is_open(g_flyout) && g_closed == 1);
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0 && g_picked == 0);
	/* the selectors are back on the date */
	CHECK(Klooping_selector_get_selected_index(day) == 6);
	/* Back */
	open_flyout(root, g_button);
	Klooping_selector_set_selected_index(day, 12);
	CHECK(Kroot_back(root));
	CHECK(!Kflyout_base_get_is_open(g_flyout) && g_closed == 2);
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0 && g_picked == 0);
	CHECK(Klooping_selector_get_selected_index(Kpicker_flyout_base_get_selector(g_flyout, 1)) == 6);
	/* a tap outside: the title strip is not a button, but it is the flyout; it stays */
	open_flyout(root, g_button);
	CHECK(Kflyout_base_get_is_open(g_flyout));
	Kflyout_base_hide(g_flyout);
	CHECK(!Kflyout_base_get_is_open(g_flyout) && g_closed == 3);
	Kroot_destroy(root);
}

static void test_date_flyout_days_of_the_month(void) {
	Kid root, month, day, year;
	date_setup(&root, D(2024, 3, 31), K_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Kpicker_flyout_base_get_selector(g_flyout, 0);
	day = Kpicker_flyout_base_get_selector(g_flyout, 1);
	year = Kpicker_flyout_base_get_selector(g_flyout, 2);
	CHECK(Klooping_selector_get_item_count(day) == 31 && Klooping_selector_get_selected_index(day) == 30);
	/* February of a leap year: 29 days, and the 31st becomes the 29th */
	Klooping_selector_set_selected_index(month, 1);
	CHECK(Klooping_selector_get_item_count(day) == 29 && Klooping_selector_get_selected_index(day) == 28);
	/* the year that is not a leap year: 28 */
	Klooping_selector_set_selected_index(year, Klooping_selector_get_selected_index(year) - 1);
	CHECK(Klooping_selector_get_item_count(day) == 28 && Klooping_selector_get_selected_index(day) == 27);
	/* a century year */
	Klooping_selector_set_selected_index(year, 1900 - Kdate_picker_get_min_year(g_dp));
	CHECK(Kdate_picker_get_min_year(g_dp) > 1900 || Klooping_selector_get_item_count(day) == 28);
	/* April has 30 */
	Klooping_selector_set_selected_index(month, 3);
	CHECK(Klooping_selector_get_item_count(day) == 30);
	/* the days have their weekdays: 1 April 2023 was a Saturday */
	Klooping_selector_set_selected_index(year, 2023 - Kdate_picker_get_min_year(g_dp));
	CHECK_STR(Klooping_selector_get_item_secondary_text(day, 0), "Sat");
	tap(root, center(Kpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2023, 4, 28)));
	Kroot_destroy(root);
}

static void test_date_flyout_columns(void) {
	Kid root, day;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	Kdate_picker_set_day_visible(g_dp, K_FALSE);
	CHECK(!Kdate_picker_get_day_visible(g_dp));
	open_flyout(root, g_button);
	day = Kpicker_flyout_base_get_selector(g_flyout, 1);
	CHECK(Kelement_get_visibility(day) == K_COLLAPSED);
	CHECK(Kelement_get_visibility(Kpicker_flyout_base_get_selector(g_flyout, 0)) == K_VISIBLE);
	/* the day is kept as it was */
	tap(root, center(Kpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Kdate_picker_get_date(g_dp), D(2026, 11, 7)));
	Kdate_picker_set_day_visible(g_dp, K_TRUE);
	Kdate_picker_set_year_visible(g_dp, K_FALSE);
	CHECK(!Kdate_picker_get_year_visible(g_dp) && Kdate_picker_get_month_visible(g_dp));
	Kroot_destroy(root);
}

static void test_date_picker_programmatic_while_open(void) {
	Kid root, month;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Kpicker_flyout_base_get_selector(g_flyout, 0);
	Kdate_picker_set_date(g_dp, D(2026, 3, 5));
	CHECK(g_date_events == 1);
	CHECK(Klooping_selector_get_selected_index(month) == 2);
	Kroot_destroy(root);
}

static void test_date_picker_flyout_alone(void) {
	Kid root, flyout = Kdate_picker_flyout_create(), button = Kbutton_create("open"), month;
	Kdate_picker_flyout_set_min_year(flyout, 1990);
	Kdate_picker_flyout_set_max_year(flyout, 2010);
	Kdate_picker_flyout_set_date(flyout, D(2001, 9, 11));
	Kpicker_flyout_base_set_title(flyout, "pick");
	root = make_root(button, K_ANIMATIONS_OFF);
	g_picked = 0;
	Kdate_picker_flyout_on_date_picked(flyout, on_date_picked, NULL);
	CHECK(same_date(Kdate_picker_flyout_get_date(flyout), D(2001, 9, 11)));
	Kflyout_base_show_at(flyout, button);
	Kroot_update_layout(root);
	CHECK(Kflyout_base_get_is_open(flyout));
	CHECK_STR(Kpicker_flyout_base_get_title(flyout), "pick");
	month = Kpicker_flyout_base_get_selector(flyout, 0);
	CHECK(Klooping_selector_get_selected_index(month) == 8);
	Klooping_selector_set_selected_index(month, 0);
	tap(root, center(Kpicker_flyout_base_get_done_button(flyout)));
	CHECK(g_picked == 1 && same_date(g_old_date, D(2001, 9, 11)) && same_date(g_new_date, D(2001, 1, 11)));
	CHECK(same_date(Kdate_picker_flyout_get_date(flyout), D(2001, 1, 11)));
	CHECK(!Kflyout_base_get_is_open(flyout));
	Kelement_destroy(flyout);
	Kroot_destroy(root);
}

/* ---- TimePicker ---- */

static Kid g_tp, g_tflyout, g_tbutton;

static Kid time_setup(Kid* root, KTimeOfDay time, KAnimations animations) {
	Kid st = Kstack_panel_create(K_VERTICAL);
	g_tp = Ktime_picker_create();
	Ktime_picker_set_header(g_tp, "alarm");
	Ktime_picker_set_time(g_tp, time);
	Kelement_set_margin(g_tp, Kthickness(19, 40, 19, 0));
	Kpanel_add(st, g_tp);
	*root = make_root(st, animations);
	g_tflyout = Ktime_picker_get_flyout(g_tp);
	g_tbutton = Ktime_picker_get_flyout_button(g_tp);
	g_time_events = 0;
	Ktime_picker_on_time_changed(g_tp, on_time, NULL);
	return g_tp;
}

static void test_time_picker_text_and_value(void) {
	Kid root;
	time_setup(&root, T(15, 7), K_ANIMATIONS_OFF);
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "3:07 PM");
	Ktime_picker_set_time(g_tp, T(0, 5));
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "12:05 AM");
	CHECK(g_time_events == 1 && same_time(g_old_time, T(15, 7)) && same_time(g_new_time, T(0, 5)));
	Ktime_picker_set_time(g_tp, T(12, 0));
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "12:00 PM");
	Ktime_picker_set_time(g_tp, T(12, 0));
	CHECK(g_time_events == 2);
	Ktime_picker_set_clock_identifier(g_tp, K_CLOCK_24_HOUR);
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "12:00");
	Ktime_picker_set_time(g_tp, T(23, 59));
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "23:59");
	Ktime_picker_set_time(g_tp, T(30, 80));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(23, 59)));
	CHECK_STR(Ktime_picker_get_header(g_tp), "alarm");
	{
		KRect b = Kelement_get_bounds(g_tbutton);
		CHECK_NEAR(b.width, 442);
	}
	Kroot_destroy(root);
}

static void test_time_flyout_12_hour(void) {
	Kid root, hour, minute, period;
	time_setup(&root, T(15, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_tbutton);
	CHECK(Kflyout_base_get_is_open(g_tflyout));
	hour = Kpicker_flyout_base_get_selector(g_tflyout, 0);
	minute = Kpicker_flyout_base_get_selector(g_tflyout, 1);
	period = Kpicker_flyout_base_get_selector(g_tflyout, 2);
	CHECK(Klooping_selector_get_item_count(hour) == 12 && Klooping_selector_get_selected_index(hour) == 2);
	CHECK_STR(Klooping_selector_get_item_text(hour, 2), "3");
	CHECK_STR(Klooping_selector_get_item_text(hour, 11), "12");
	CHECK(Klooping_selector_get_item_count(minute) == 60 && Klooping_selector_get_selected_index(minute) == 7);
	CHECK_STR(Klooping_selector_get_item_text(minute, 7), "07");
	CHECK(Klooping_selector_get_item_count(period) == 2 && Klooping_selector_get_selected_index(period) == 1);
	CHECK_STR(Klooping_selector_get_item_text(period, 1), "PM");
	CHECK(Kelement_get_visibility(period) == K_VISIBLE);
	CHECK(!Klooping_selector_get_should_loop(period) && Klooping_selector_get_should_loop(hour));
	/* a finger on the hours: three down, 6 o'clock; the minutes: 4 up */
	drag_selector(root, hour, 3 * Klooping_selector_get_item_height(hour));
	CHECK(Klooping_selector_get_selected_index(hour) == 11 - 0 || Klooping_selector_get_selected_index(hour) == (2 - 3 + 12) % 12);
	Klooping_selector_set_selected_index(hour, 5);
	Klooping_selector_set_selected_index(minute, 30);
	Klooping_selector_set_selected_index(period, 0);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(6, 30)));
	CHECK(g_time_events == 1 && same_time(g_old_time, T(15, 7)) && same_time(g_new_time, T(6, 30)));
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "6:30 AM");
	/* 12 is 0 in the morning and 12 at noon */
	open_flyout(root, g_tbutton);
	Klooping_selector_set_selected_index(hour, 11);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(0, 30)));
	open_flyout(root, g_tbutton);
	Klooping_selector_set_selected_index(period, 1);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(12, 30)));
	/* cancel leaves it */
	open_flyout(root, g_tbutton);
	Klooping_selector_set_selected_index(hour, 3);
	tap(root, center(Kpicker_flyout_base_get_cancel_button(g_tflyout)));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(12, 30)) && g_time_events == 3);
	CHECK(Klooping_selector_get_selected_index(hour) == 11);
	Kroot_destroy(root);
}

static void test_time_flyout_24_hour_and_increment(void) {
	Kid root, hour, minute, period;
	time_setup(&root, T(15, 7), K_ANIMATIONS_OFF);
	Ktime_picker_set_clock_identifier(g_tp, K_CLOCK_24_HOUR);
	Ktime_picker_set_minute_increment(g_tp, 15);
	CHECK(Ktime_picker_get_minute_increment(g_tp) == 15);
	open_flyout(root, g_tbutton);
	hour = Kpicker_flyout_base_get_selector(g_tflyout, 0);
	minute = Kpicker_flyout_base_get_selector(g_tflyout, 1);
	period = Kpicker_flyout_base_get_selector(g_tflyout, 2);
	CHECK(Klooping_selector_get_item_count(hour) == 24 && Klooping_selector_get_selected_index(hour) == 15);
	CHECK_STR(Klooping_selector_get_item_text(hour, 0), "0");
	CHECK(Kelement_get_visibility(period) == K_COLLAPSED);
	/* 0, 15, 30, 45; 7 is nearer to 0 than to 15 - it is shown on 0 */
	CHECK(Klooping_selector_get_item_count(minute) == 4);
	CHECK_STR(Klooping_selector_get_item_text(minute, 3), "45");
	CHECK(Klooping_selector_get_selected_index(minute) == 0);
	Klooping_selector_set_selected_index(minute, 2);
	Klooping_selector_set_selected_index(hour, 23);
	tap(root, center(Kpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ktime_picker_get_time(g_tp), T(23, 30)));
	CHECK_STR(Kcontent_control_get_text(g_tbutton), "23:30");
	/* 8 is nearer to 15: the nearest */
	Ktime_picker_set_time(g_tp, T(1, 8));
	open_flyout(root, g_tbutton);
	CHECK(Klooping_selector_get_selected_index(minute) == 1);
	Kflyout_base_hide(g_tflyout);
	Kroot_destroy(root);
}

static void test_time_picker_flyout_alone(void) {
	Kid root, flyout = Ktime_picker_flyout_create(), button = Kbutton_create("open");
	root = make_root(button, K_ANIMATIONS_OFF);
	Ktime_picker_flyout_set_time(flyout, T(9, 45));
	CHECK(same_time(Ktime_picker_flyout_get_time(flyout), T(9, 45)));
	CHECK(Ktime_picker_flyout_get_clock_identifier(flyout) == K_CLOCK_12_HOUR);
	Kflyout_base_show_at(flyout, button);
	Kroot_update_layout(root);
	CHECK(Klooping_selector_get_selected_index(Kpicker_flyout_base_get_selector(flyout, 0)) == 8);
	CHECK(Klooping_selector_get_selected_index(Kpicker_flyout_base_get_selector(flyout, 1)) == 45);
	Kelement_destroy(flyout);
	Kroot_destroy(root);
}

static void test_destroy_with_open_flyout(void) {
	Kid root;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	CHECK(Kflyout_base_get_is_open(g_flyout));
	Kelement_destroy(g_dp);
	Kroot_update_layout(root);
	Kroot_destroy(root);
}

static void test_light_theme(void) {
	Kid root, month;
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	Kroot_set_theme(root, K_THEME_LIGHT);
	open_flyout(root, g_button);
	month = Kpicker_flyout_base_get_selector(g_flyout, 0);
	/* the chrome of Light is DDDDDD, the chosen item the accent */
	CHECK(pixel_is(root, 5, 400, 0xDD, 0xDD, 0xDD));
	{
		KRect r = Kelement_get_bounds(month);
		KPoint p = Kelement_to_root(month, (KPoint){ 8, r.height * 0.5f });
		CHECK(pixel_is(root, (int)p.x, (int)p.y, 0x00, 0x50, 0xEF));
	}
	Kroot_destroy(root);
}

static void test_picture(void) {
	Kid root;
	char path[512];
	date_setup(&root, D(2026, 11, 7), K_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/date_picker.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	open_flyout(root, g_button);
	snprintf(path, sizeof path, "%s/date_picker_flyout.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	{
		Kid day = Kpicker_flyout_base_get_selector(g_flyout, 1);
		KPoint p = center(day);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 40);
		snprintf(path, sizeof path, "%s/date_picker_flyout_touched.png", KESTREL_TEST_OUTPUT_DIR);
		CHECK(test_save_png(root, path));
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y - 40);
	}
	Kroot_destroy(root);
	time_setup(&root, T(15, 7), K_ANIMATIONS_OFF);
	open_flyout(root, g_tbutton);
	snprintf(path, sizeof path, "%s/time_picker_flyout.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_classes);
	RUN(test_selector_look);
	RUN(test_selector_keys_and_loop);
	RUN(test_selector_drag_and_tap);
	RUN(test_selector_fling_and_snap);
	RUN(test_selector_secondary_text);
	RUN(test_date_picker_look);
	RUN(test_date_set_and_coerce);
	RUN(test_date_flyout_opens_on_the_date);
	RUN(test_date_flyout_done);
	RUN(test_date_flyout_cancel_and_back);
	RUN(test_date_flyout_days_of_the_month);
	RUN(test_date_flyout_columns);
	RUN(test_date_picker_programmatic_while_open);
	RUN(test_date_picker_flyout_alone);
	RUN(test_time_picker_text_and_value);
	RUN(test_time_flyout_12_hour);
	RUN(test_time_flyout_24_hour_and_increment);
	RUN(test_time_picker_flyout_alone);
	RUN(test_destroy_with_open_flyout);
	RUN(test_light_theme);
	RUN(test_picture);
	Kshutdown();
	return test_finish("test_pickers");
}
