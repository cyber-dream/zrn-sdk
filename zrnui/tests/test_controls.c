/**
 * The API of the controls without any input: hierarchy, handles, content,
 * toggle states and events, ranges, items and selection, text boxes.
 */

#include "test.h"

static int g_count;
static int g_last;
static double g_old, g_new;

static void quiet_errors(ZError e, const char* m, void* u) { (void)e; (void)m; (void)u; }
static void on_event(Zid sender, void* user) { (void)sender; g_count++; g_last = user ? *(int*)user : 0; }
static void on_value(Zid sender, double old_value, double new_value, void* user) { (void)sender; (void)user; g_count++; g_old = old_value; g_new = new_value; }

static void test_hierarchy(void) {
	Zid cb = Zcheck_box_create("x"), sl = Zslider_create(Z_HORIZONTAL), lb = Zlist_box_create(), cmb = Zcombo_box_create(), pw = Zpassword_box_create();
	CHECK(Zelement_get_class(cb) == Z_CLASS_CHECKBOX);
	CHECK(Zelement_is_a(cb, Z_CLASS_TOGGLE_BUTTON));
	CHECK(Zelement_is_a(cb, Z_CLASS_BUTTON_BASE));
	CHECK(Zelement_is_a(cb, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zelement_is_a(cb, Z_CLASS_CONTROL));
	CHECK(Zelement_is_a(cb, Z_CLASS_ELEMENT));
	CHECK(!Zelement_is_a(cb, Z_CLASS_BUTTON));
	CHECK(Zelement_is_a(sl, Z_CLASS_RANGE_BASE));
	CHECK(!Zelement_is_a(sl, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zelement_is_a(lb, Z_CLASS_SELECTOR) && Zelement_is_a(lb, Z_CLASS_ITEMS_CONTROL));
	CHECK(Zelement_is_a(cmb, Z_CLASS_SELECTOR));
	CHECK(Zelement_is_a(pw, Z_CLASS_TEXT_BOX));
	CHECK_STR(Zelement_get_class_name(cb), "CheckBox");
	Zelement_destroy(cb); Zelement_destroy(sl); Zelement_destroy(lb); Zelement_destroy(cmb); Zelement_destroy(pw);
}

static void test_handles_are_checked(void) {
	Zid b = Zbutton_create("x");
	Zelement_destroy(b);
	CHECK(!Zelement_is_valid(b));
	CHECK(Zelement_get_class(b) == 0);
	CHECK(Zget_error() == Z_ERROR_INVALID_ID);
	/* a slot is reused with another generation: the old handle stays dead */
	{
		Zid c = Zbutton_create("y");
		CHECK(c != b);
		CHECK(Zelement_is_valid(c));
		CHECK(!Zelement_is_valid(b));
		Zelement_destroy(c);
	}
	{
		Zid s = Zslider_create(Z_HORIZONTAL);
		Zcontent_control_set_text(s, "no");  /* a Slider is not a ContentControl */
		CHECK(Zget_error() == Z_ERROR_WRONG_CLASS);
		Zelement_destroy(s);
	}
	CHECK(Zelement_create(Z_CLASS_PANEL) == Z_INVALID_ID); /* abstract */
}

static void test_tree_ownership(void) {
	Zid p = Zstack_panel_create(Z_VERTICAL), a = Zbutton_create("a"), b = Zbutton_create("b"), q = Zstack_panel_create(Z_VERTICAL);
	CHECK(Zpanel_add(p, a) == a);
	CHECK(Zpanel_add(p, b) == b);
	CHECK(Zpanel_get_count(p) == 2);
	CHECK(Zelement_get_parent(a) == p);
	/* adding to another panel moves it */
	Zpanel_add(q, a);
	CHECK(Zpanel_get_count(p) == 1 && Zpanel_get_count(q) == 1);
	CHECK(Zelement_get_parent(a) == q);
	Zpanel_remove(q, a);
	CHECK(Zelement_get_parent(a) == Z_INVALID_ID && Zelement_is_valid(a));
	/* a panel cannot be put under its own child */
	Zpanel_add(p, q);
	CHECK(Zpanel_add(q, p) == Z_INVALID_ID);
	CHECK(Zget_error() == Z_ERROR_CYCLE);
	/* destroying a parent destroys the children */
	Zelement_destroy(p);
	CHECK(!Zelement_is_valid(b) && !Zelement_is_valid(q));
	Zelement_destroy(a);
}

static void test_names_and_find(void) {
	Zid p = Zstack_panel_create(Z_VERTICAL), q = Zpanel_add(p, Zstack_panel_create(Z_VERTICAL)), b = Zpanel_add(q, Zbutton_create("deep"));
	Zelement_set_name(b, "deep-button");
	CHECK(Zelement_find_name(p, "deep-button") == b);
	CHECK(Zelement_find_name(p, "nothing") == Z_INVALID_ID);
	CHECK_STR(Zelement_get_name(b), "deep-button");
	Zelement_destroy(p);
}

static void test_content(void) {
	Zid b = Zbutton_create("OK"), t;
	CHECK_STR(Zcontent_control_get_text(b), "OK");
	CHECK(Zelement_is_a(Zcontent_control_get_content(b), Z_CLASS_TEXT_BLOCK));
	Zcontent_control_set_text(b, "Cancel");
	CHECK_STR(Zcontent_control_get_text(b), "Cancel");
	/* an element as content replaces and destroys the TextBlock */
	t = Zcontent_control_get_content(b);
	Zcontent_control_set_content(b, Zimage_create());
	CHECK(!Zelement_is_valid(t));
	CHECK_STR(Zcontent_control_get_text(b), "");
	CHECK(Zelement_is_a(Zcontent_control_get_content(b), Z_CLASS_IMAGE));
	/* destroying the content from outside empties the control */
	Zelement_destroy(Zcontent_control_get_content(b));
	CHECK(Zcontent_control_get_content(b) == Z_INVALID_ID);
	Zelement_destroy(b);
}

static void test_toggle_states_and_events(void) {
	Zid cb = Zcheck_box_create("x");
	g_count = 0;
	Zelement_on(cb, Z_EVENT_CHECKED, on_event, NULL);
	Zelement_on(cb, Z_EVENT_UNCHECKED, on_event, NULL);
	Zelement_on(cb, Z_EVENT_INDETERMINATE, on_event, NULL);
	CHECK(!Ztoggle_button_get_is_checked(cb));
	Ztoggle_button_set_is_checked(cb, Z_TRUE);
	CHECK(Ztoggle_button_get_is_checked(cb) && g_count == 1);
	Ztoggle_button_set_is_checked(cb, Z_TRUE);
	CHECK(g_count == 1); /* no change, no event */
	Ztoggle_button_set_state(cb, Z_INDETERMINATE);
	CHECK(Ztoggle_button_get_state(cb) == Z_INDETERMINATE && g_count == 2);
	Ztoggle_button_set_is_checked(cb, Z_FALSE);
	CHECK(g_count == 3);
	Zelement_destroy(cb);
}

static void test_radio_groups(void) {
	Zid p = Zstack_panel_create(Z_VERTICAL), q = Zstack_panel_create(Z_VERTICAL);
	Zid a = Zpanel_add(p, Zradio_button_create("a", NULL)), b = Zpanel_add(p, Zradio_button_create("b", NULL));
	Zid c = Zpanel_add(q, Zradio_button_create("c", NULL));
	Zid g1 = Zpanel_add(p, Zradio_button_create("g1", "x")), g2 = Zpanel_add(q, Zradio_button_create("g2", "x"));
	Zid root = Zroot_create(100, 100), top = Zstack_panel_create(Z_VERTICAL);
	Zpanel_add(top, p);
	Zpanel_add(top, q);
	Zroot_set_visual(root, top);
	Ztoggle_button_set_is_checked(a, Z_TRUE);
	Ztoggle_button_set_is_checked(c, Z_TRUE);
	/* with no GroupName the parent is the group */
	CHECK(Ztoggle_button_get_is_checked(a) && Ztoggle_button_get_is_checked(c));
	Ztoggle_button_set_is_checked(b, Z_TRUE);
	CHECK(!Ztoggle_button_get_is_checked(a) && Ztoggle_button_get_is_checked(b) && Ztoggle_button_get_is_checked(c));
	/* a named group reaches across parents */
	Ztoggle_button_set_is_checked(g1, Z_TRUE);
	Ztoggle_button_set_is_checked(g2, Z_TRUE);
	CHECK(!Ztoggle_button_get_is_checked(g1) && Ztoggle_button_get_is_checked(g2));
	/* and does not touch the unnamed ones */
	CHECK(Ztoggle_button_get_is_checked(b));
	Zroot_destroy(root);
}

static void test_range(void) {
	Zid sl = Zslider_create(Z_HORIZONTAL), pb = Zprogress_bar_create();
	g_count = 0;
	CHECK(Zrange_base_get_maximum(sl) == 100 && Zrange_base_get_minimum(sl) == 0); /* 8.1 slider defaults */
	CHECK(Zrange_base_get_maximum(pb) == 100);
	Zrange_base_on_value_changed(sl, on_value, NULL);
	Zrange_base_set_value(sl, 4);
	CHECK(g_count == 1 && g_old == 0 && g_new == 4);
	Zrange_base_set_value(sl, 4);
	CHECK(g_count == 1);
	Zrange_base_set_value(sl, 199); /* coerced */
	CHECK(Zrange_base_get_value(sl) == 100 && g_new == 100);
	Zrange_base_set_range(sl, 0, 5); /* the value is coerced again */
	CHECK(Zrange_base_get_value(sl) == 5);
	Zrange_base_set_range(sl, 2, 1); /* maximum below minimum becomes the minimum */
	CHECK(Zrange_base_get_maximum(sl) == 2 && Zrange_base_get_value(sl) == 2);
	Zelement_destroy(sl);
	Zelement_destroy(pb);
}

static void test_items_and_selection(void) {
	Zid lb = Zlist_box_create(), c1, c2;
	g_count = 0;
	Zelement_on(lb, Z_EVENT_SELECTION_CHANGED, on_event, NULL);
	c1 = Zitems_control_add_text(lb, "one");
	c2 = Zitems_control_add_text(lb, "two");
	Zitems_control_add_text(lb, "three");
	CHECK(Zitems_control_get_count(lb) == 3);
	CHECK(Zelement_is_a(c1, Z_CLASS_LIST_BOX_ITEM));
	CHECK_STR(Zitems_control_get_text(lb, 1), "two");
	CHECK(Zselector_get_selected_index(lb) == -1);
	Zselector_set_selected_index(lb, 1);
	CHECK(Zselector_get_selected_index(lb) == 1 && g_count == 1);
	CHECK(Zselector_get_selected_container(lb) == c2);
	Zselector_set_selected_index(lb, 1);
	CHECK(g_count == 1);
	/* removing an item before the selected one shifts the index, removing the selected one clears it */
	Zitems_control_remove_at(lb, 0);
	CHECK(Zselector_get_selected_index(lb) == 0 && !Zelement_is_valid(c1));
	Zitems_control_remove_at(lb, 0);
	CHECK(Zselector_get_selected_index(lb) == -1 && g_count == 2);
	Zitems_control_clear(lb);
	CHECK(Zitems_control_get_count(lb) == 0);
	/* an element is wrapped in a container */
	{
		Zid t = Ztext_block_create("custom"), c = Zitems_control_add(lb, t);
		CHECK(Zelement_is_a(c, Z_CLASS_LIST_BOX_ITEM) && Zcontent_control_get_content(c) == t);
		CHECK_STR(Zitems_control_get_text(lb, 0), "");
	}
	Zelement_destroy(lb);
}

static void test_multiple_selection(void) {
	Zid lb = Zlist_box_create(), a, c;
	int indices[4];
	Zlist_box_set_selection_mode(lb, Z_SELECTION_MULTIPLE);
	CHECK(Zlist_box_get_selection_mode(lb) == Z_SELECTION_MULTIPLE);
	a = Zitems_control_add_text(lb, "a");
	Zitems_control_add_text(lb, "b");
	c = Zitems_control_add_text(lb, "c");
	Zselector_item_set_is_selected(a, Z_TRUE);
	Zselector_item_set_is_selected(c, Z_TRUE);
	CHECK(Zselector_item_get_is_selected(a) && !Zselector_item_get_is_selected(Zitems_control_get_container(lb, 1)) && Zselector_item_get_is_selected(c));
	CHECK(Zlist_box_get_selected_indices(lb, indices, 4) == 2 && indices[0] == 0 && indices[1] == 2);
	CHECK(Zselector_get_selected_index(lb) == 0);
	Zselector_item_set_is_selected(a, Z_FALSE);
	CHECK(!Zselector_item_get_is_selected(a) && Zselector_item_get_is_selected(c));
	CHECK(Zselector_get_selected_index(lb) == 2);
	Zlist_box_select_all(lb);
	CHECK(Zlist_box_get_selected_indices(lb, NULL, 0) == 3);
	Zelement_destroy(lb);
}

static void test_text_box(void) {
	Zid tb = Ztext_box_create("hello");
	g_count = 0;
	Zelement_on(tb, Z_EVENT_TEXT_CHANGED, on_event, NULL);
	CHECK_STR(Ztext_box_get_text(tb), "hello");
	Ztext_box_set_text(tb, "hello");
	CHECK(g_count == 0);
	Ztext_box_set_text(tb, "world");
	CHECK(g_count == 1);
	Ztext_box_select(tb, 1, 3);
	CHECK(Ztext_box_get_selection_start(tb) == 1 && Ztext_box_get_selection_length(tb) == 3);
	Ztext_box_select(tb, 3, 100);
	CHECK(Ztext_box_get_selection_length(tb) == 2);
	Ztext_box_select_all(tb);
	CHECK(Ztext_box_get_selection_length(tb) == 5);
	Zelement_destroy(tb);
}

static void test_theme_colors(void) {
	Zid root = Zroot_create(100, 100);
	ZColor c = Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_COLOR);
	CHECK(c.r == 1 && c.g == 1 && c.b == 1 && c.a == 1);
	Zroot_set_theme(root, Z_THEME_LIGHT);
	c = Zroot_get_theme_color(root, Z_RES_PHONE_BACKGROUND_COLOR);
	CHECK(c.r == 1 && c.a == 1);
	c = Zroot_get_theme_color(root, Z_RES_PHONE_FOREGROUND_COLOR);
	CHECK(c.r == 0 && c.a == 1); /* #FF000000 of the 8.1 Light dictionary (the WP8 table had #DE000000) */
	Zroot_set_accent(root, Z_ACCENT_LIME);
	c = Zroot_get_accent_color(root);
	CHECK_NEAR(c.r * 255, 0xA4);
	CHECK_NEAR(c.g * 255, 0xC4);
	CHECK(Zroot_get_theme(root) == Z_THEME_LIGHT);
	Zroot_destroy(root);
}

static void test_text_block_styles(void) {
	Zid t = Ztext_block_create("x");
	CHECK(Ztext_block_set_style(t, Z_TEXT_STYLE_PHONE_TITLE1));
	CHECK(!Ztext_block_set_style(t, Z_TEXT_STYLE_NONE));
	CHECK(!Ztext_block_set_style(t, Z_TEXT_STYLE_LAST_));
	CHECK_NEAR(Zelement_get_margin(t).left, 12);
	/* the 8.1 styles have no margin; a property set after a style wins */
	CHECK(Ztext_block_set_style(t, Z_TEXT_STYLE_HEADER));
	CHECK_NEAR(Zelement_get_margin(t).left, 0);
	Ztext_block_set_font_size(t, 40);
	Zelement_destroy(t);
}

static void test_new_classes(void) {
	Zid sw = Ztoggle_switch_create(), ring = Zprogress_ring_create();
	CHECK(Zelement_get_class(sw) == Z_CLASS_TOGGLE_SWITCH && Zelement_get_class(ring) == Z_CLASS_PROGRESS_RING);
	CHECK(Zelement_is_a(sw, Z_CLASS_CONTROL) && !Zelement_is_a(sw, Z_CLASS_CONTENT_CONTROL));
	CHECK(Zelement_is_a(ring, Z_CLASS_CONTROL) && !Zelement_is_a(ring, Z_CLASS_RANGE_BASE));
	CHECK_STR(Zelement_get_class_name(sw), "ToggleSwitch");
	CHECK_STR(Zelement_get_class_name(ring), "ProgressRing");
	Ztoggle_switch_set_is_on(ring, Z_TRUE); /* not a ToggleSwitch */
	CHECK(Zget_error() == Z_ERROR_WRONG_CLASS);
	Zelement_destroy(sw);
	Zelement_destroy(ring);
}

static void test_toggle_switch_api(void) {
	Zid sw = Ztoggle_switch_create();
	g_count = 0;
	/* off, with the strings of the template */
	CHECK(!Ztoggle_switch_get_is_on(sw));
	CHECK_STR(Ztoggle_switch_get_header(sw), "");
	CHECK_STR(Ztoggle_switch_get_on_content(sw), "On");
	CHECK_STR(Ztoggle_switch_get_off_content(sw), "Off");
	Ztoggle_switch_on_toggled(sw, on_event, NULL);
	Ztoggle_switch_set_is_on(sw, Z_TRUE);
	CHECK(Ztoggle_switch_get_is_on(sw) && g_count == 1);
	Ztoggle_switch_set_is_on(sw, Z_TRUE);
	CHECK(g_count == 1); /* no change, no event */
	Ztoggle_switch_set_is_on(sw, Z_FALSE);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_count == 2);
	Ztoggle_switch_set_header(sw, "Wi-Fi");
	Ztoggle_switch_set_on_content(sw, "Connected");
	Ztoggle_switch_set_off_content(sw, "Disconnected");
	CHECK_STR(Ztoggle_switch_get_header(sw), "Wi-Fi");
	CHECK_STR(Ztoggle_switch_get_on_content(sw), "Connected");
	CHECK_STR(Ztoggle_switch_get_off_content(sw), "Disconnected");
	Ztoggle_switch_set_header(sw, NULL);
	CHECK_STR(Ztoggle_switch_get_header(sw), "");
	Zelement_destroy(sw);
}

static void test_progress_ring_api(void) {
	Zid ring = Zprogress_ring_create();
	CHECK(!Zprogress_ring_get_is_active(ring));
	Zprogress_ring_set_is_active(ring, Z_TRUE);
	CHECK(Zprogress_ring_get_is_active(ring));
	Zprogress_ring_set_is_active(ring, Z_FALSE);
	CHECK(!Zprogress_ring_get_is_active(ring));
	Zelement_destroy(ring);
}

static void test_81_properties(void) {
	Zid sl = Zslider_create(Z_HORIZONTAL), pb = Zprogress_bar_create(), tb = Ztext_box_create(""), pw = Zpassword_box_create(), rb = Zrepeat_button_create("+");
	/* slider */
	CHECK(Zslider_get_step_frequency(sl) == 1 && Zslider_get_tick_frequency(sl) == 0);
	Zslider_set_step_frequency(sl, 0.5);
	Zslider_set_tick_frequency(sl, 10);
	Zslider_set_tick_placement(sl, Z_TICK_INLINE);
	Zslider_set_snaps_to(sl, Z_SNAPS_TO_TICKS);
	CHECK(Zslider_get_step_frequency(sl) == 0.5 && Zslider_get_tick_frequency(sl) == 10);
	Zslider_set_header(sl, "Volume");
	CHECK_STR(Zslider_get_header(sl), "Volume");
	/* progress bar */
	CHECK(!Zprogress_bar_get_is_indeterminate(pb));
	Zprogress_bar_set_is_indeterminate(pb, Z_TRUE);
	CHECK(Zprogress_bar_get_is_indeterminate(pb));
	Zprogress_bar_set_show_error(pb, Z_TRUE);
	Zprogress_bar_set_show_paused(pb, Z_TRUE);
	/* text box: the style of the 8.1 template */
	CHECK_NEAR(Zelement_get_margin(tb).top, 9.5);
	CHECK_NEAR(Zelement_get_margin(tb).bottom, 9.5);
	CHECK_NEAR(Zelement_get_margin(tb).left, 0);
	CHECK_STR(Ztext_box_get_placeholder_text(tb), "");
	Ztext_box_set_placeholder_text(tb, "Search");
	Ztext_box_set_header(tb, "Query");
	CHECK_STR(Ztext_box_get_placeholder_text(tb), "Search");
	CHECK_STR(Ztext_box_get_header(tb), "Query");
	Ztext_box_set_placeholder_text(tb, NULL);
	CHECK_STR(Ztext_box_get_placeholder_text(tb), "");
	Ztext_box_set_is_read_only(tb, Z_TRUE);
	Ztext_box_set_max_length(tb, 7);
	CHECK(Ztext_box_get_is_read_only(tb) && Ztext_box_get_max_length(tb) == 7);
	/* password box */
	Zpassword_box_set_password(pw, "hunter2");
	CHECK_STR(Zpassword_box_get_password(pw), "hunter2");
	CHECK_STR(Ztext_box_get_text(pw), "hunter2");
	Zpassword_box_set_header(pw, "Password");
	Zpassword_box_set_placeholder_text(pw, "secret");
	CHECK_STR(Ztext_box_get_header(pw), "Password");
	Zpassword_box_set_header(tb, "x"); /* not a PasswordBox */
	CHECK(Zget_error() == Z_ERROR_WRONG_CLASS);
	/* repeat button: Delay and Interval of 8.1 */
	CHECK(Zrepeat_button_get_delay(rb) == 250 && Zrepeat_button_get_interval(rb) == 250);
	Zelement_destroy(sl); Zelement_destroy(pb); Zelement_destroy(tb); Zelement_destroy(pw); Zelement_destroy(rb);
}

/* the sizes the 8.1 styles give: MinWidth/MinHeight, touch overhangs, the rows of the templates */
static void test_81_default_sizes(void) {
	Zid root = Zroot_create(480, 800), stack = Zstack_panel_create(Z_VERTICAL);
	Zid b = Zpanel_add(stack, Zbutton_create("OK")), cb = Zpanel_add(stack, Zcheck_box_create("c")), rb = Zpanel_add(stack, Zradio_button_create("r", NULL));
	Zid link = Zpanel_add(stack, Zhyperlink_button_create("link")), tb = Zpanel_add(stack, Ztext_box_create("t")), sl = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zid pb = Zpanel_add(stack, Zprogress_bar_create()), sw = Zpanel_add(stack, Ztoggle_switch_create()), ring = Zpanel_add(stack, Zprogress_ring_create());
	Zid tbh = Zpanel_add(stack, Ztext_box_create("h"));
	Ztext_box_set_header(tbh, "Header");
	Zroot_set_visual(root, stack);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(b).width, 109);
	CHECK_NEAR(Zelement_get_bounds(b).height, 57.5);
	CHECK_NEAR(Zelement_get_bounds(cb).width, 168);
	CHECK_NEAR(Zelement_get_bounds(cb).height, 57.5); /* 16 + the 25.5 box + 16 */
	CHECK_NEAR(Zelement_get_bounds(rb).width, 168);
	CHECK(Zelement_get_bounds(link).height > 30 && Zelement_get_bounds(link).height < 60); /* 29.86 px text, padding 3 and 4 */
	CHECK_NEAR(Zelement_get_bounds(tb).height, 34);
	CHECK(Zelement_get_bounds(tbh).height > 34 + 20); /* the header and the 4.8 under it */
	CHECK_NEAR(Zelement_get_bounds(sl).height, 57);
	CHECK_NEAR(Zelement_get_bounds(sl).width, 480);
	CHECK_NEAR(Zelement_get_bounds(pb).height, 3.5);
	CHECK_NEAR(Zelement_get_bounds(sw).height, 45.5); /* the 36 px switch and the 9.5 px row under it */
	CHECK_NEAR(Zelement_get_bounds(sw).width, 480);
	CHECK_NEAR(Zelement_get_bounds(ring).height, 57.5);
	CHECK_NEAR(Zelement_get_bounds(ring).width, 480);
	Ztoggle_switch_set_header(sw, "Header");
	Zroot_update_layout(root);
	CHECK(Zelement_get_bounds(sw).height > 45.5 + 15);
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
	Zset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
	RUN(test_hierarchy);
	RUN(test_handles_are_checked);
	RUN(test_tree_ownership);
	RUN(test_names_and_find);
	RUN(test_content);
	RUN(test_toggle_states_and_events);
	RUN(test_radio_groups);
	RUN(test_range);
	RUN(test_items_and_selection);
	RUN(test_multiple_selection);
	RUN(test_text_box);
	RUN(test_theme_colors);
	RUN(test_text_block_styles);
	RUN(test_new_classes);
	RUN(test_toggle_switch_api);
	RUN(test_progress_ring_api);
	RUN(test_81_properties);
	RUN(test_81_default_sizes);
	Zshutdown();
	return test_finish("test_controls");
}
