/**
 * LoopingSelector, DatePicker, TimePicker and their flyouts of Windows Phone 8.1. Every check goes through what a finger would do
 * (Zroot_inject_pointer on the button, on the selectors, on Done and Cancel), the keys, the clock, or a pixel of the rendered page
 * (Dark theme, Cobalt accent). The dates are always set by the test: nothing here depends on today.
 */

#include "test.h"

static int g_changes, g_date_events, g_time_events, g_picked, g_closed;
static ZDate g_old_date, g_new_date;
static ZTimeOfDay g_old_time, g_new_time;

static void on_change(Zid sender, void* u) { (void)sender; (void)u; g_changes++; }
static void on_closed(Zid sender, void* u) { (void)sender; (void)u; g_closed++; }
static void on_date(Zid sender, ZDate o, ZDate n, void* u) { (void)sender; (void)u; g_date_events++; g_old_date = o; g_new_date = n; }
static void on_time(Zid sender, ZTimeOfDay o, ZTimeOfDay n, void* u) { (void)sender; (void)u; g_time_events++; g_old_time = o; g_new_time = n; }
static void on_date_picked(Zid sender, ZDate o, ZDate n, void* u) { (void)sender; (void)u; g_picked++; g_old_date = o; g_new_date = n; }

static void run(Zid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Zroot_tick(root, 16);
}

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void tap(Zid root, ZPoint p) {
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
}

static void key(Zid root, ZKey k) { Zroot_inject_key(root, k, 0, 1); Zroot_inject_key(root, k, 0, 0); }

static int pixel_is(Zid root, int x, int y, int r, int g, int b) {
	int pr, pg, pb;
	test_pixel(root, x, y, &pr, &pg, &pb);
	return abs(pr - r) <= 2 && abs(pg - g) <= 2 && abs(pb - b) <= 2;
}

static int same_date(ZDate a, ZDate b) { return a.year == b.year && a.month == b.month && a.day == b.day; }
static int same_time(ZTimeOfDay a, ZTimeOfDay b) { return a.hour == b.hour && a.minute == b.minute; }

static Zid make_root(Zid visual, ZAnimations animations) {
	Zid root = Zroot_create(480, 800);
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, animations);
	Zroot_set_visual(root, visual);
	Zroot_update_layout(root);
	return root;
}

/* A finger on a selector goes `dy` px up or down in steps of 10 and rests there before it lets go, so that no inertia is left. */
static void drag_selector(Zid root, Zid selector, float dy) {
	ZPoint p = center(selector);
	float y;
	int i;
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	for (y = dy > 0 ? 10 : -10; fabsf(y) <= fabsf(dy); y += dy > 0 ? 10 : -10) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y + y);
	for (i = 0; i < 8; i++) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y + dy);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y + dy);
}

static Zid make_selector(int count, Zid* root, ZAnimations animations) {
	Zid sel = Zlooping_selector_create();
	int i;
	for (i = 0; i < count; i++) {
		char text[16];
		snprintf(text, sizeof text, "%d", i + 1);
		Zlooping_selector_add_item(sel, text, NULL);
	}
	Zelement_set_horizontal_alignment(sel, Z_HALIGN_LEFT);
	*root = make_root(sel, animations);
	g_changes = 0;
	Zlooping_selector_on_selection_changed(sel, on_change, NULL);
	return sel;
}

static void test_classes(void) {
	Zid sel = Zlooping_selector_create(), dp = Zdate_picker_create(), tp = Ztime_picker_create(), df = Zdate_picker_flyout_create(), tf = Ztime_picker_flyout_create();
	CHECK(Zelement_is_a(sel, Z_CLASS_CONTROL) && Zelement_is_a(dp, Z_CLASS_CONTROL) && Zelement_is_a(tp, Z_CLASS_CONTROL));
	CHECK(Zelement_is_a(df, Z_CLASS_PICKER_FLYOUT_BASE) && Zelement_is_a(df, Z_CLASS_FLYOUT_BASE) && Zelement_is_a(tf, Z_CLASS_PICKER_FLYOUT_BASE));
	CHECK(Zflyout_base_get_placement(df) == Z_FLYOUT_PLACEMENT_FULL);
	CHECK(Zlooping_selector_get_should_loop(sel) && Zlooping_selector_get_selected_index(sel) == -1 && Zlooping_selector_get_item_count(sel) == 0);
	CHECK(Zdate_picker_get_day_visible(dp) && Zdate_picker_get_month_visible(dp) && Zdate_picker_get_year_visible(dp));
	CHECK(Ztime_picker_get_clock_identifier(tp) == Z_CLOCK_12_HOUR && Ztime_picker_get_minute_increment(tp) == 1);
	CHECK(Zdate_picker_get_flyout(dp) != Z_INVALID_ID && Zdate_picker_get_flyout_button(dp) != Z_INVALID_ID);
	CHECK(Zelement_is_a(Zdate_picker_get_flyout_button(dp), Z_CLASS_BUTTON));
	CHECK(Zdate_picker_get_max_year(dp) - Zdate_picker_get_min_year(dp) == 200);
	Zelement_destroy(sel);
	Zelement_destroy(dp);
	Zelement_destroy(tp);
	Zelement_destroy(df);
	Zelement_destroy(tf);
}

/* ---- LoopingSelector ---- */

static void test_selector_look(void) {
	Zid root, sel = make_selector(5, &root, Z_ANIMATIONS_OFF);
	ZRect r = Zelement_get_bounds(sel);
	float ch = Zlooping_selector_get_item_height(sel);
	int cy = (int)(r.y + r.height * 0.5f);
	CHECK(Zlooping_selector_get_selected_index(sel) == 0);
	CHECK(Zlooping_selector_get_item_count(sel) == 5);
	CHECK_STR(Zlooping_selector_get_item_text(sel, 2), "3");
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
		ZPoint p = center(sel);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		/* the border of the item below: 2.5 px at the left of its box, 5 px in */
		CHECK(pixel_is(root, (int)r.x + 6, cy + (int)ch, 0x73, 0x73, 0x73));
		CHECK(pixel_is(root, (int)r.x + 8, cy, 0x00, 0x50, 0xEF));
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y);
		/* and lets go: it is as it was */
		CHECK(pixel_is(root, (int)r.x + 6, cy + (int)ch, 0, 0, 0));
	}
	Zroot_destroy(root);
}

static void test_selector_keys_and_loop(void) {
	Zid root, sel = make_selector(5, &root, Z_ANIMATIONS_OFF);
	Zelement_focus(sel);
	key(root, Z_KEY_DOWN);
	CHECK(Zlooping_selector_get_selected_index(sel) == 1 && g_changes == 1);
	key(root, Z_KEY_UP);
	key(root, Z_KEY_UP);
	/* it goes round: from the first to the last */
	CHECK(Zlooping_selector_get_selected_index(sel) == 4 && g_changes == 3);
	key(root, Z_KEY_DOWN);
	CHECK(Zlooping_selector_get_selected_index(sel) == 0);
	/* without ShouldLoop it stops at the ends */
	Zlooping_selector_set_should_loop(sel, false);
	CHECK(!Zlooping_selector_get_should_loop(sel));
	key(root, Z_KEY_UP);
	CHECK(Zlooping_selector_get_selected_index(sel) == 0);
	Zlooping_selector_set_selected_index(sel, 4);
	CHECK(g_changes == 5);
	key(root, Z_KEY_DOWN);
	CHECK(Zlooping_selector_get_selected_index(sel) == 4);
	/* the program sets the index in range */
	Zlooping_selector_set_selected_index(sel, 40);
	CHECK(Zlooping_selector_get_selected_index(sel) == 4);
	Zroot_destroy(root);
}

static void test_selector_drag_and_tap(void) {
	Zid root, sel = make_selector(8, &root, Z_ANIMATIONS_OFF);
	float ch = Zlooping_selector_get_item_height(sel);
	ZRect r = Zelement_get_bounds(sel);
	/* a finger drawn up by three items: the chosen one is the fourth */
	drag_selector(root, sel, -3 * ch);
	CHECK(Zlooping_selector_get_selected_index(sel) == 3 && g_changes == 1);
	/* and down by five: it goes round the beginning, to the seventh */
	drag_selector(root, sel, 5 * ch);
	CHECK(Zlooping_selector_get_selected_index(sel) == 6 && g_changes == 2);
	/* less than half an item goes back */
	drag_selector(root, sel, 0.3f * ch);
	CHECK(Zlooping_selector_get_selected_index(sel) == 6 && g_changes == 2);
	/* a tap on the item above the middle chooses it */
	{
		ZPoint p = center(sel);
		tap(root, (ZPoint){ p.x, p.y - ch });
		CHECK(Zlooping_selector_get_selected_index(sel) == 5 && g_changes == 3);
		/* a tap on the item in the middle changes nothing */
		tap(root, p);
		CHECK(Zlooping_selector_get_selected_index(sel) == 5 && g_changes == 3);
		/* two below */
		tap(root, (ZPoint){ p.x, p.y + 2 * ch });
		CHECK(Zlooping_selector_get_selected_index(sel) == 7);
	}
	/* the accent box is in the middle again after every move */
	CHECK(pixel_is(root, (int)r.x + 8, (int)(r.y + r.height * 0.5f), 0x00, 0x50, 0xEF));
	/* without ShouldLoop the column stays between the first and the last item */
	Zlooping_selector_set_should_loop(sel, false);
	drag_selector(root, sel, -6 * ch);
	CHECK(Zlooping_selector_get_selected_index(sel) == 7);
	drag_selector(root, sel, 20 * ch);
	CHECK(Zlooping_selector_get_selected_index(sel) == 0);
	Zroot_destroy(root);
}

static void test_selector_fling_and_snap(void) {
	Zid root, sel = make_selector(40, &root, Z_ANIMATIONS_ON);
	float ch = Zlooping_selector_get_item_height(sel);
	ZRect r = Zelement_get_bounds(sel);
	ZPoint p = center(sel);
	int i, before;
	/* a quick flick up: the column goes on by itself, slows down, and ends on an item */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
	for (i = 1; i <= 6; i++) Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 20.0f * (float)i);
	Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y - 120);
	CHECK(g_changes == 0);
	run(root, 100);
	before = g_changes;
	/* not chosen while it moves */
	CHECK(before == 0);
	run(root, 3000);
	CHECK(g_changes == 1);
	CHECK(Zlooping_selector_get_selected_index(sel) > 1);
	/* at rest on the item: the accent in the middle, the others hidden again (the Expanded state ends in 330 ms) */
	CHECK(pixel_is(root, (int)r.x + 8, (int)(r.y + r.height * 0.5f), 0x00, 0x50, 0xEF));
	CHECK(pixel_is(root, (int)r.x + 6, (int)(r.y + r.height * 0.5f + ch), 0, 0, 0));
	/* a tap on a neighbour goes to it in 160 ms */
	{
		int was = Zlooping_selector_get_selected_index(sel);
		tap(root, (ZPoint){ p.x, p.y + ch });
		run(root, 400);
		CHECK(Zlooping_selector_get_selected_index(sel) == was + 1);
		CHECK(g_changes == 2);
	}
	Zroot_destroy(root);
}

static void test_selector_secondary_text(void) {
	Zid root, sel = Zlooping_selector_create();
	ZRect r;
	Zlooping_selector_add_item(sel, "7", "Sat");
	Zlooping_selector_add_item(sel, "8", "Sun");
	Zelement_set_horizontal_alignment(sel, Z_HALIGN_LEFT);
	root = make_root(sel, Z_ANIMATIONS_OFF);
	CHECK_STR(Zlooping_selector_get_item_secondary_text(sel, 1), "Sun");
	CHECK_STR(Zlooping_selector_get_item_secondary_text(sel, 5), "");
	/* the item is taller by the second line */
	r = Zelement_get_bounds(sel);
	CHECK(Zlooping_selector_get_item_height(sel) > 75 && Zlooping_selector_get_item_height(sel) < 100);
	(void)r;
	Zlooping_selector_clear_items(sel);
	CHECK(Zlooping_selector_get_item_count(sel) == 0 && Zlooping_selector_get_selected_index(sel) == -1);
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

/* ---- DatePicker ---- */

static Zid g_dp, g_flyout, g_button;

static Zid date_setup(Zid* root, ZDate date, ZAnimations animations) {
	Zid st = Zstack_panel_create(Z_VERTICAL);
	g_dp = Zdate_picker_create();
	Zdate_picker_set_header(g_dp, "birthday");
	Zdate_picker_set_date(g_dp, date);
	Zelement_set_margin(g_dp, Zthickness(19, 40, 19, 0));
	Zpanel_add(st, g_dp);
	*root = make_root(st, animations);
	g_flyout = Zdate_picker_get_flyout(g_dp);
	g_button = Zdate_picker_get_flyout_button(g_dp);
	g_date_events = g_picked = g_closed = 0;
	Zdate_picker_on_date_changed(g_dp, on_date, NULL);
	Zdate_picker_flyout_on_date_picked(g_flyout, on_date_picked, NULL);
	Zflyout_base_on_closed(g_flyout, on_closed, NULL);
	return g_dp;
}

static ZDate D(int y, int m, int d) { ZDate x = { y, m, d }; return x; }
static ZTimeOfDay T(int h, int m) { ZTimeOfDay x = { h, m }; return x; }

static void test_date_picker_look(void) {
	Zid root;
	ZRect b, c;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	b = Zelement_get_bounds(g_button);
	c = Zelement_get_bounds(g_dp);
	/* the margin 19, the header over the button, the button as wide as the room less the margins */
	CHECK_NEAR(c.x, 19);
	CHECK_NEAR(c.width, 442);
	CHECK_NEAR(b.x, 0);
	CHECK_NEAR(b.width, 442);
	/* the button is a Button of 57.5 with its 9.5 above and below the outline, the header's line less 3 over it */
	CHECK(b.height >= 57.5f && b.height < 60);
	CHECK(b.y > 10 && b.y < 25);
	CHECK_STR(Zdate_picker_get_header(g_dp), "birthday");
	CHECK_STR(Zcontent_control_get_text(g_button), "November 7, 2026");
	/* 2.5 px outline in the foreground, the text inside it at 6.5 */
	{
		ZPoint p = Zelement_to_root(g_button, (ZPoint){ 0, 0 });
		int top = (int)p.y + 9 + 1, left = (int)p.x + 1;
		CHECK(pixel_is(root, left + 200, top, 255, 255, 255));
		CHECK(pixel_is(root, left, top + 15, 255, 255, 255));
		CHECK(pixel_is(root, left + 200, top + 10, 0, 0, 0));
	}
	/* Light: the outline is black */
	Zroot_set_theme(root, Z_THEME_LIGHT);
	Zroot_update_layout(root);
	{
		ZPoint p = Zelement_to_root(g_button, (ZPoint){ 0, 0 });
		CHECK(pixel_is(root, (int)p.x + 201, (int)p.y + 10, 0, 0, 0));
		CHECK(pixel_is(root, (int)p.x + 201, (int)p.y + 20, 255, 255, 255));
	}
	Zroot_destroy(root);
}

static void test_date_set_and_coerce(void) {
	Zid root;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 7)));
	/* a date that is the same raises nothing */
	Zdate_picker_set_date(g_dp, D(2026, 11, 7));
	CHECK(g_date_events == 0);
	/* the leap day */
	Zdate_picker_set_date(g_dp, D(2024, 2, 29));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2024, 2, 29)) && g_date_events == 1);
	CHECK(same_date(g_old_date, D(2026, 11, 7)) && same_date(g_new_date, D(2024, 2, 29)));
	CHECK_STR(Zcontent_control_get_text(g_button), "February 29, 2024");
	/* the same day of a year that has no such day */
	Zdate_picker_set_date(g_dp, D(2023, 2, 29));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2023, 2, 28)));
	Zdate_picker_set_min_year(g_dp, 1800);
	Zdate_picker_set_date(g_dp, D(1900, 2, 29));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(1900, 2, 28)));
	Zdate_picker_set_date(g_dp, D(2000, 2, 29));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2000, 2, 29)));
	Zdate_picker_set_date(g_dp, D(2026, 4, 31));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 4, 30)));
	Zdate_picker_set_date(g_dp, D(2026, 13, 0));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 12, 1)));
	/* the years of the picker */
	Zdate_picker_set_min_year(g_dp, 2020);
	Zdate_picker_set_max_year(g_dp, 2030);
	Zdate_picker_set_date(g_dp, D(2050, 5, 5));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2030, 5, 5)));
	Zdate_picker_set_date(g_dp, D(1999, 5, 5));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2020, 5, 5)));
	CHECK(Zdate_picker_get_min_year(g_dp) == 2020 && Zdate_picker_get_max_year(g_dp) == 2030);
	Zroot_destroy(root);
}

static void open_flyout(Zid root, Zid button) {
	tap(root, center(button));
	Zroot_update_layout(root);
}

static void test_date_flyout_opens_on_the_date(void) {
	Zid root, month, day, year;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	Zdate_picker_set_min_year(g_dp, 2000);
	Zdate_picker_set_max_year(g_dp, 2040);
	CHECK(!Zflyout_base_get_is_open(g_flyout));
	open_flyout(root, g_button);
	CHECK(Zflyout_base_get_is_open(g_flyout));
	month = Zpicker_flyout_base_get_selector(g_flyout, 0);
	day = Zpicker_flyout_base_get_selector(g_flyout, 1);
	year = Zpicker_flyout_base_get_selector(g_flyout, 2);
	CHECK(Zlooping_selector_get_item_count(month) == 12 && Zlooping_selector_get_selected_index(month) == 10);
	CHECK_STR(Zlooping_selector_get_item_text(month, 10), "November");
	CHECK(Zlooping_selector_get_item_count(day) == 30 && Zlooping_selector_get_selected_index(day) == 6);
	/* the 7th of November 2026 was a Saturday */
	CHECK_STR(Zlooping_selector_get_item_text(day, 6), "7");
	CHECK_STR(Zlooping_selector_get_item_secondary_text(day, 6), "Sat");
	CHECK_STR(Zlooping_selector_get_item_secondary_text(day, 0), "Sun");
	CHECK(Zlooping_selector_get_item_count(year) == 41 && Zlooping_selector_get_selected_index(year) == 26);
	CHECK_STR(Zlooping_selector_get_item_text(year, 0), "2000");
	CHECK(!Zlooping_selector_get_should_loop(year) && Zlooping_selector_get_should_loop(month) && Zlooping_selector_get_should_loop(day));
	/* the title is the header of the picker */
	CHECK_STR(Zpicker_flyout_base_get_title(g_flyout), "birthday");
	/* three selectors side by side, the page around them: chrome */
	{
		ZRect rm = Zelement_get_bounds(month), rd = Zelement_get_bounds(day), ry = Zelement_get_bounds(year);
		CHECK(rm.width > 150 && rd.width >= 64 && ry.width >= 64);
		CHECK(Zelement_to_root(month, (ZPoint){ 0, 0 }).x < Zelement_to_root(day, (ZPoint){ 0, 0 }).x);
		CHECK(Zelement_to_root(day, (ZPoint){ 0, 0 }).x < Zelement_to_root(year, (ZPoint){ 0, 0 }).x);
		CHECK(rm.height > 400);
		CHECK(pixel_is(root, 5, 400, 0x1F, 0x1F, 0x1F));
	}
	/* the buttons are at the bottom */
	{
		ZPoint d = center(Zpicker_flyout_base_get_done_button(g_flyout)), c = center(Zpicker_flyout_base_get_cancel_button(g_flyout));
		CHECK(d.y > 700 && c.y > 700 && d.x < 240 && c.x > 240);
	}
	Zroot_destroy(root);
}

static void test_date_flyout_done(void) {
	Zid root, month, day, year;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Zpicker_flyout_base_get_selector(g_flyout, 0);
	day = Zpicker_flyout_base_get_selector(g_flyout, 1);
	year = Zpicker_flyout_base_get_selector(g_flyout, 2);
	/* a finger on the day selector: two items up, the 9th */
	drag_selector(root, day, -2 * Zlooping_selector_get_item_height(day));
	CHECK(Zlooping_selector_get_selected_index(day) == 8);
	/* the picker does not know yet */
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 9)));
	CHECK(g_date_events == 1 && same_date(g_old_date, D(2026, 11, 7)) && same_date(g_new_date, D(2026, 11, 9)));
	CHECK(g_picked == 1);
	CHECK(!Zflyout_base_get_is_open(g_flyout) && g_closed == 1);
	CHECK_STR(Zcontent_control_get_text(g_button), "November 9, 2026");
	/* Done with nothing changed: DatePicked, no DateChanged */
	open_flyout(root, g_button);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(g_picked == 2 && g_date_events == 1 && !Zflyout_base_get_is_open(g_flyout));
	/* a change by the year selector */
	open_flyout(root, g_button);
	year = Zpicker_flyout_base_get_selector(g_flyout, 2);
	(void)month;
	Zlooping_selector_set_selected_index(year, Zlooping_selector_get_selected_index(year) + 2);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2028, 11, 9)));
	Zroot_destroy(root);
}

static void test_date_flyout_cancel_and_back(void) {
	Zid root, day;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	day = Zpicker_flyout_base_get_selector(g_flyout, 1);
	Zlooping_selector_set_selected_index(day, 20);
	tap(root, center(Zpicker_flyout_base_get_cancel_button(g_flyout)));
	CHECK(!Zflyout_base_get_is_open(g_flyout) && g_closed == 1);
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0 && g_picked == 0);
	/* the selectors are back on the date */
	CHECK(Zlooping_selector_get_selected_index(day) == 6);
	/* Back */
	open_flyout(root, g_button);
	Zlooping_selector_set_selected_index(day, 12);
	CHECK(Zroot_back(root));
	CHECK(!Zflyout_base_get_is_open(g_flyout) && g_closed == 2);
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 7)) && g_date_events == 0 && g_picked == 0);
	CHECK(Zlooping_selector_get_selected_index(Zpicker_flyout_base_get_selector(g_flyout, 1)) == 6);
	/* a tap outside: the title strip is not a button, but it is the flyout; it stays */
	open_flyout(root, g_button);
	CHECK(Zflyout_base_get_is_open(g_flyout));
	Zflyout_base_hide(g_flyout);
	CHECK(!Zflyout_base_get_is_open(g_flyout) && g_closed == 3);
	Zroot_destroy(root);
}

static void test_date_flyout_days_of_the_month(void) {
	Zid root, month, day, year;
	date_setup(&root, D(2024, 3, 31), Z_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Zpicker_flyout_base_get_selector(g_flyout, 0);
	day = Zpicker_flyout_base_get_selector(g_flyout, 1);
	year = Zpicker_flyout_base_get_selector(g_flyout, 2);
	CHECK(Zlooping_selector_get_item_count(day) == 31 && Zlooping_selector_get_selected_index(day) == 30);
	/* February of a leap year: 29 days, and the 31st becomes the 29th */
	Zlooping_selector_set_selected_index(month, 1);
	CHECK(Zlooping_selector_get_item_count(day) == 29 && Zlooping_selector_get_selected_index(day) == 28);
	/* the year that is not a leap year: 28 */
	Zlooping_selector_set_selected_index(year, Zlooping_selector_get_selected_index(year) - 1);
	CHECK(Zlooping_selector_get_item_count(day) == 28 && Zlooping_selector_get_selected_index(day) == 27);
	/* a century year */
	Zlooping_selector_set_selected_index(year, 1900 - Zdate_picker_get_min_year(g_dp));
	CHECK(Zdate_picker_get_min_year(g_dp) > 1900 || Zlooping_selector_get_item_count(day) == 28);
	/* April has 30 */
	Zlooping_selector_set_selected_index(month, 3);
	CHECK(Zlooping_selector_get_item_count(day) == 30);
	/* the days have their weekdays: 1 April 2023 was a Saturday */
	Zlooping_selector_set_selected_index(year, 2023 - Zdate_picker_get_min_year(g_dp));
	CHECK_STR(Zlooping_selector_get_item_secondary_text(day, 0), "Sat");
	tap(root, center(Zpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2023, 4, 28)));
	Zroot_destroy(root);
}

static void test_date_flyout_columns(void) {
	Zid root, day;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	Zdate_picker_set_day_visible(g_dp, false);
	CHECK(!Zdate_picker_get_day_visible(g_dp));
	open_flyout(root, g_button);
	day = Zpicker_flyout_base_get_selector(g_flyout, 1);
	CHECK(Zelement_get_visibility(day) == Z_COLLAPSED);
	CHECK(Zelement_get_visibility(Zpicker_flyout_base_get_selector(g_flyout, 0)) == Z_VISIBLE);
	/* the day is kept as it was */
	tap(root, center(Zpicker_flyout_base_get_done_button(g_flyout)));
	CHECK(same_date(Zdate_picker_get_date(g_dp), D(2026, 11, 7)));
	Zdate_picker_set_day_visible(g_dp, true);
	Zdate_picker_set_year_visible(g_dp, false);
	CHECK(!Zdate_picker_get_year_visible(g_dp) && Zdate_picker_get_month_visible(g_dp));
	Zroot_destroy(root);
}

static void test_date_picker_programmatic_while_open(void) {
	Zid root, month;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	month = Zpicker_flyout_base_get_selector(g_flyout, 0);
	Zdate_picker_set_date(g_dp, D(2026, 3, 5));
	CHECK(g_date_events == 1);
	CHECK(Zlooping_selector_get_selected_index(month) == 2);
	Zroot_destroy(root);
}

static void test_date_picker_flyout_alone(void) {
	Zid root, flyout = Zdate_picker_flyout_create(), button = Zbutton_create("open"), month;
	Zdate_picker_flyout_set_min_year(flyout, 1990);
	Zdate_picker_flyout_set_max_year(flyout, 2010);
	Zdate_picker_flyout_set_date(flyout, D(2001, 9, 11));
	Zpicker_flyout_base_set_title(flyout, "pick");
	root = make_root(button, Z_ANIMATIONS_OFF);
	g_picked = 0;
	Zdate_picker_flyout_on_date_picked(flyout, on_date_picked, NULL);
	CHECK(same_date(Zdate_picker_flyout_get_date(flyout), D(2001, 9, 11)));
	Zflyout_base_show_at(flyout, button);
	Zroot_update_layout(root);
	CHECK(Zflyout_base_get_is_open(flyout));
	CHECK_STR(Zpicker_flyout_base_get_title(flyout), "pick");
	month = Zpicker_flyout_base_get_selector(flyout, 0);
	CHECK(Zlooping_selector_get_selected_index(month) == 8);
	Zlooping_selector_set_selected_index(month, 0);
	tap(root, center(Zpicker_flyout_base_get_done_button(flyout)));
	CHECK(g_picked == 1 && same_date(g_old_date, D(2001, 9, 11)) && same_date(g_new_date, D(2001, 1, 11)));
	CHECK(same_date(Zdate_picker_flyout_get_date(flyout), D(2001, 1, 11)));
	CHECK(!Zflyout_base_get_is_open(flyout));
	Zelement_destroy(flyout);
	Zroot_destroy(root);
}

/* ---- TimePicker ---- */

static Zid g_tp, g_tflyout, g_tbutton;

static Zid time_setup(Zid* root, ZTimeOfDay time, ZAnimations animations) {
	Zid st = Zstack_panel_create(Z_VERTICAL);
	g_tp = Ztime_picker_create();
	Ztime_picker_set_header(g_tp, "alarm");
	Ztime_picker_set_time(g_tp, time);
	Zelement_set_margin(g_tp, Zthickness(19, 40, 19, 0));
	Zpanel_add(st, g_tp);
	*root = make_root(st, animations);
	g_tflyout = Ztime_picker_get_flyout(g_tp);
	g_tbutton = Ztime_picker_get_flyout_button(g_tp);
	g_time_events = 0;
	Ztime_picker_on_time_changed(g_tp, on_time, NULL);
	return g_tp;
}

static void test_time_picker_text_and_value(void) {
	Zid root;
	time_setup(&root, T(15, 7), Z_ANIMATIONS_OFF);
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "3:07 PM");
	Ztime_picker_set_time(g_tp, T(0, 5));
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "12:05 AM");
	CHECK(g_time_events == 1 && same_time(g_old_time, T(15, 7)) && same_time(g_new_time, T(0, 5)));
	Ztime_picker_set_time(g_tp, T(12, 0));
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "12:00 PM");
	Ztime_picker_set_time(g_tp, T(12, 0));
	CHECK(g_time_events == 2);
	Ztime_picker_set_clock_identifier(g_tp, Z_CLOCK_24_HOUR);
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "12:00");
	Ztime_picker_set_time(g_tp, T(23, 59));
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "23:59");
	Ztime_picker_set_time(g_tp, T(30, 80));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(23, 59)));
	CHECK_STR(Ztime_picker_get_header(g_tp), "alarm");
	{
		ZRect b = Zelement_get_bounds(g_tbutton);
		CHECK_NEAR(b.width, 442);
	}
	Zroot_destroy(root);
}

static void test_time_flyout_12_hour(void) {
	Zid root, hour, minute, period;
	time_setup(&root, T(15, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_tbutton);
	CHECK(Zflyout_base_get_is_open(g_tflyout));
	hour = Zpicker_flyout_base_get_selector(g_tflyout, 0);
	minute = Zpicker_flyout_base_get_selector(g_tflyout, 1);
	period = Zpicker_flyout_base_get_selector(g_tflyout, 2);
	CHECK(Zlooping_selector_get_item_count(hour) == 12 && Zlooping_selector_get_selected_index(hour) == 2);
	CHECK_STR(Zlooping_selector_get_item_text(hour, 2), "3");
	CHECK_STR(Zlooping_selector_get_item_text(hour, 11), "12");
	CHECK(Zlooping_selector_get_item_count(minute) == 60 && Zlooping_selector_get_selected_index(minute) == 7);
	CHECK_STR(Zlooping_selector_get_item_text(minute, 7), "07");
	CHECK(Zlooping_selector_get_item_count(period) == 2 && Zlooping_selector_get_selected_index(period) == 1);
	CHECK_STR(Zlooping_selector_get_item_text(period, 1), "PM");
	CHECK(Zelement_get_visibility(period) == Z_VISIBLE);
	CHECK(!Zlooping_selector_get_should_loop(period) && Zlooping_selector_get_should_loop(hour));
	/* a finger on the hours: three down, 6 o'clock; the minutes: 4 up */
	drag_selector(root, hour, 3 * Zlooping_selector_get_item_height(hour));
	CHECK(Zlooping_selector_get_selected_index(hour) == 11 - 0 || Zlooping_selector_get_selected_index(hour) == (2 - 3 + 12) % 12);
	Zlooping_selector_set_selected_index(hour, 5);
	Zlooping_selector_set_selected_index(minute, 30);
	Zlooping_selector_set_selected_index(period, 0);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(6, 30)));
	CHECK(g_time_events == 1 && same_time(g_old_time, T(15, 7)) && same_time(g_new_time, T(6, 30)));
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "6:30 AM");
	/* 12 is 0 in the morning and 12 at noon */
	open_flyout(root, g_tbutton);
	Zlooping_selector_set_selected_index(hour, 11);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(0, 30)));
	open_flyout(root, g_tbutton);
	Zlooping_selector_set_selected_index(period, 1);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(12, 30)));
	/* cancel leaves it */
	open_flyout(root, g_tbutton);
	Zlooping_selector_set_selected_index(hour, 3);
	tap(root, center(Zpicker_flyout_base_get_cancel_button(g_tflyout)));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(12, 30)) && g_time_events == 3);
	CHECK(Zlooping_selector_get_selected_index(hour) == 11);
	Zroot_destroy(root);
}

static void test_time_flyout_24_hour_and_increment(void) {
	Zid root, hour, minute, period;
	time_setup(&root, T(15, 7), Z_ANIMATIONS_OFF);
	Ztime_picker_set_clock_identifier(g_tp, Z_CLOCK_24_HOUR);
	Ztime_picker_set_minute_increment(g_tp, 15);
	CHECK(Ztime_picker_get_minute_increment(g_tp) == 15);
	open_flyout(root, g_tbutton);
	hour = Zpicker_flyout_base_get_selector(g_tflyout, 0);
	minute = Zpicker_flyout_base_get_selector(g_tflyout, 1);
	period = Zpicker_flyout_base_get_selector(g_tflyout, 2);
	CHECK(Zlooping_selector_get_item_count(hour) == 24 && Zlooping_selector_get_selected_index(hour) == 15);
	CHECK_STR(Zlooping_selector_get_item_text(hour, 0), "0");
	CHECK(Zelement_get_visibility(period) == Z_COLLAPSED);
	/* 0, 15, 30, 45; 7 is nearer to 0 than to 15 - it is shown on 0 */
	CHECK(Zlooping_selector_get_item_count(minute) == 4);
	CHECK_STR(Zlooping_selector_get_item_text(minute, 3), "45");
	CHECK(Zlooping_selector_get_selected_index(minute) == 0);
	Zlooping_selector_set_selected_index(minute, 2);
	Zlooping_selector_set_selected_index(hour, 23);
	tap(root, center(Zpicker_flyout_base_get_done_button(g_tflyout)));
	CHECK(same_time(Ztime_picker_get_time(g_tp), T(23, 30)));
	CHECK_STR(Zcontent_control_get_text(g_tbutton), "23:30");
	/* 8 is nearer to 15: the nearest */
	Ztime_picker_set_time(g_tp, T(1, 8));
	open_flyout(root, g_tbutton);
	CHECK(Zlooping_selector_get_selected_index(minute) == 1);
	Zflyout_base_hide(g_tflyout);
	Zroot_destroy(root);
}

static void test_time_picker_flyout_alone(void) {
	Zid root, flyout = Ztime_picker_flyout_create(), button = Zbutton_create("open");
	root = make_root(button, Z_ANIMATIONS_OFF);
	Ztime_picker_flyout_set_time(flyout, T(9, 45));
	CHECK(same_time(Ztime_picker_flyout_get_time(flyout), T(9, 45)));
	CHECK(Ztime_picker_flyout_get_clock_identifier(flyout) == Z_CLOCK_12_HOUR);
	Zflyout_base_show_at(flyout, button);
	Zroot_update_layout(root);
	CHECK(Zlooping_selector_get_selected_index(Zpicker_flyout_base_get_selector(flyout, 0)) == 8);
	CHECK(Zlooping_selector_get_selected_index(Zpicker_flyout_base_get_selector(flyout, 1)) == 45);
	Zelement_destroy(flyout);
	Zroot_destroy(root);
}

static void test_destroy_with_open_flyout(void) {
	Zid root;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_button);
	CHECK(Zflyout_base_get_is_open(g_flyout));
	Zelement_destroy(g_dp);
	Zroot_update_layout(root);
	Zroot_destroy(root);
}

static void test_light_theme(void) {
	Zid root, month;
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	open_flyout(root, g_button);
	month = Zpicker_flyout_base_get_selector(g_flyout, 0);
	/* the chrome of Light is DDDDDD, the chosen item the accent */
	CHECK(pixel_is(root, 5, 400, 0xDD, 0xDD, 0xDD));
	{
		ZRect r = Zelement_get_bounds(month);
		ZPoint p = Zelement_to_root(month, (ZPoint){ 8, r.height * 0.5f });
		CHECK(pixel_is(root, (int)p.x, (int)p.y, 0x00, 0x50, 0xEF));
	}
	Zroot_destroy(root);
}

static void test_picture(void) {
	Zid root;
	char path[512];
	date_setup(&root, D(2026, 11, 7), Z_ANIMATIONS_OFF);
	snprintf(path, sizeof path, "%s/date_picker.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	open_flyout(root, g_button);
	snprintf(path, sizeof path, "%s/date_picker_flyout.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	{
		Zid day = Zpicker_flyout_base_get_selector(g_flyout, 1);
		ZPoint p = center(day);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 40);
		snprintf(path, sizeof path, "%s/date_picker_flyout_touched.png", ZRNUI_TEST_OUTPUT_DIR);
		CHECK(test_save_png(root, path));
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y - 40);
	}
	Zroot_destroy(root);
	time_setup(&root, T(15, 7), Z_ANIMATIONS_OFF);
	open_flyout(root, g_tbutton);
	snprintf(path, sizeof path, "%s/time_picker_flyout.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
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
	Zshutdown();
	return test_finish("test_pickers");
}
