/**
 * The API of the controls without any input: hierarchy, handles, content,
 * toggle states and events, ranges, items and selection, text boxes.
 */

#include "test.h"

static int g_count;
static int g_last;
static double g_old, g_new;

static void quiet_errors(KError e, const char* m, void* u) { (void)e; (void)m; (void)u; }
static void on_event(Kid sender, void* user) { (void)sender; g_count++; g_last = user ? *(int*)user : 0; }
static void on_value(Kid sender, double old_value, double new_value, void* user) { (void)sender; (void)user; g_count++; g_old = old_value; g_new = new_value; }

static void test_hierarchy(void) {
	Kid cb = Kcheck_box_create("x"), sl = Kslider_create(K_HORIZONTAL), lb = Klist_box_create(), cmb = Kcombo_box_create(), pw = Kpassword_box_create();
	CHECK(Kelement_get_class(cb) == K_CLASS_CHECKBOX);
	CHECK(Kelement_is_a(cb, K_CLASS_TOGGLE_BUTTON));
	CHECK(Kelement_is_a(cb, K_CLASS_BUTTON_BASE));
	CHECK(Kelement_is_a(cb, K_CLASS_CONTENT_CONTROL));
	CHECK(Kelement_is_a(cb, K_CLASS_CONTROL));
	CHECK(Kelement_is_a(cb, K_CLASS_ELEMENT));
	CHECK(!Kelement_is_a(cb, K_CLASS_BUTTON));
	CHECK(Kelement_is_a(sl, K_CLASS_RANGE_BASE));
	CHECK(!Kelement_is_a(sl, K_CLASS_CONTENT_CONTROL));
	CHECK(Kelement_is_a(lb, K_CLASS_SELECTOR) && Kelement_is_a(lb, K_CLASS_ITEMS_CONTROL));
	CHECK(Kelement_is_a(cmb, K_CLASS_SELECTOR));
	CHECK(Kelement_is_a(pw, K_CLASS_TEXT_BOX));
	CHECK_STR(Kelement_get_class_name(cb), "CheckBox");
	Kelement_destroy(cb); Kelement_destroy(sl); Kelement_destroy(lb); Kelement_destroy(cmb); Kelement_destroy(pw);
}

static void test_handles_are_checked(void) {
	Kid b = Kbutton_create("x");
	Kelement_destroy(b);
	CHECK(!Kelement_is_valid(b));
	CHECK(Kelement_get_class(b) == 0);
	CHECK(Kget_error() == K_ERROR_INVALID_ID);
	/* a slot is reused with another generation: the old handle stays dead */
	{
		Kid c = Kbutton_create("y");
		CHECK(c != b);
		CHECK(Kelement_is_valid(c));
		CHECK(!Kelement_is_valid(b));
		Kelement_destroy(c);
	}
	{
		Kid s = Kslider_create(K_HORIZONTAL);
		Kcontent_control_set_text(s, "no");  /* a Slider is not a ContentControl */
		CHECK(Kget_error() == K_ERROR_WRONG_CLASS);
		Kelement_destroy(s);
	}
	CHECK(Kelement_create(K_CLASS_PANEL) == K_INVALID_ID); /* abstract */
}

static void test_tree_ownership(void) {
	Kid p = Kstack_panel_create(K_VERTICAL), a = Kbutton_create("a"), b = Kbutton_create("b"), q = Kstack_panel_create(K_VERTICAL);
	CHECK(Kpanel_add(p, a) == a);
	CHECK(Kpanel_add(p, b) == b);
	CHECK(Kpanel_get_count(p) == 2);
	CHECK(Kelement_get_parent(a) == p);
	/* adding to another panel moves it */
	Kpanel_add(q, a);
	CHECK(Kpanel_get_count(p) == 1 && Kpanel_get_count(q) == 1);
	CHECK(Kelement_get_parent(a) == q);
	Kpanel_remove(q, a);
	CHECK(Kelement_get_parent(a) == K_INVALID_ID && Kelement_is_valid(a));
	/* a panel cannot be put under its own child */
	Kpanel_add(p, q);
	CHECK(Kpanel_add(q, p) == K_INVALID_ID);
	CHECK(Kget_error() == K_ERROR_CYCLE);
	/* destroying a parent destroys the children */
	Kelement_destroy(p);
	CHECK(!Kelement_is_valid(b) && !Kelement_is_valid(q));
	Kelement_destroy(a);
}

static void test_names_and_find(void) {
	Kid p = Kstack_panel_create(K_VERTICAL), q = Kpanel_add(p, Kstack_panel_create(K_VERTICAL)), b = Kpanel_add(q, Kbutton_create("deep"));
	Kelement_set_name(b, "deep-button");
	CHECK(Kelement_find_name(p, "deep-button") == b);
	CHECK(Kelement_find_name(p, "nothing") == K_INVALID_ID);
	CHECK_STR(Kelement_get_name(b), "deep-button");
	Kelement_destroy(p);
}

static void test_content(void) {
	Kid b = Kbutton_create("OK"), t;
	CHECK_STR(Kcontent_control_get_text(b), "OK");
	CHECK(Kelement_is_a(Kcontent_control_get_content(b), K_CLASS_TEXT_BLOCK));
	Kcontent_control_set_text(b, "Cancel");
	CHECK_STR(Kcontent_control_get_text(b), "Cancel");
	/* an element as content replaces and destroys the TextBlock */
	t = Kcontent_control_get_content(b);
	Kcontent_control_set_content(b, Kimage_create());
	CHECK(!Kelement_is_valid(t));
	CHECK_STR(Kcontent_control_get_text(b), "");
	CHECK(Kelement_is_a(Kcontent_control_get_content(b), K_CLASS_IMAGE));
	/* destroying the content from outside empties the control */
	Kelement_destroy(Kcontent_control_get_content(b));
	CHECK(Kcontent_control_get_content(b) == K_INVALID_ID);
	Kelement_destroy(b);
}

static void test_toggle_states_and_events(void) {
	Kid cb = Kcheck_box_create("x");
	g_count = 0;
	Kelement_on(cb, K_EVENT_CHECKED, on_event, NULL);
	Kelement_on(cb, K_EVENT_UNCHECKED, on_event, NULL);
	Kelement_on(cb, K_EVENT_INDETERMINATE, on_event, NULL);
	CHECK(!Ktoggle_button_get_is_checked(cb));
	Ktoggle_button_set_is_checked(cb, K_TRUE);
	CHECK(Ktoggle_button_get_is_checked(cb) && g_count == 1);
	Ktoggle_button_set_is_checked(cb, K_TRUE);
	CHECK(g_count == 1); /* no change, no event */
	Ktoggle_button_set_state(cb, K_INDETERMINATE);
	CHECK(Ktoggle_button_get_state(cb) == K_INDETERMINATE && g_count == 2);
	Ktoggle_button_set_is_checked(cb, K_FALSE);
	CHECK(g_count == 3);
	Kelement_destroy(cb);
}

static void test_radio_groups(void) {
	Kid p = Kstack_panel_create(K_VERTICAL), q = Kstack_panel_create(K_VERTICAL);
	Kid a = Kpanel_add(p, Kradio_button_create("a", NULL)), b = Kpanel_add(p, Kradio_button_create("b", NULL));
	Kid c = Kpanel_add(q, Kradio_button_create("c", NULL));
	Kid g1 = Kpanel_add(p, Kradio_button_create("g1", "x")), g2 = Kpanel_add(q, Kradio_button_create("g2", "x"));
	Kid root = Kroot_create(100, 100), top = Kstack_panel_create(K_VERTICAL);
	Kpanel_add(top, p);
	Kpanel_add(top, q);
	Kroot_set_visual(root, top);
	Ktoggle_button_set_is_checked(a, K_TRUE);
	Ktoggle_button_set_is_checked(c, K_TRUE);
	/* with no GroupName the parent is the group */
	CHECK(Ktoggle_button_get_is_checked(a) && Ktoggle_button_get_is_checked(c));
	Ktoggle_button_set_is_checked(b, K_TRUE);
	CHECK(!Ktoggle_button_get_is_checked(a) && Ktoggle_button_get_is_checked(b) && Ktoggle_button_get_is_checked(c));
	/* a named group reaches across parents */
	Ktoggle_button_set_is_checked(g1, K_TRUE);
	Ktoggle_button_set_is_checked(g2, K_TRUE);
	CHECK(!Ktoggle_button_get_is_checked(g1) && Ktoggle_button_get_is_checked(g2));
	/* and does not touch the unnamed ones */
	CHECK(Ktoggle_button_get_is_checked(b));
	Kroot_destroy(root);
}

static void test_range(void) {
	Kid sl = Kslider_create(K_HORIZONTAL), pb = Kprogress_bar_create();
	g_count = 0;
	CHECK(Krange_base_get_maximum(sl) == 100 && Krange_base_get_minimum(sl) == 0); /* 8.1 slider defaults */
	CHECK(Krange_base_get_maximum(pb) == 100);
	Krange_base_on_value_changed(sl, on_value, NULL);
	Krange_base_set_value(sl, 4);
	CHECK(g_count == 1 && g_old == 0 && g_new == 4);
	Krange_base_set_value(sl, 4);
	CHECK(g_count == 1);
	Krange_base_set_value(sl, 199); /* coerced */
	CHECK(Krange_base_get_value(sl) == 100 && g_new == 100);
	Krange_base_set_range(sl, 0, 5); /* the value is coerced again */
	CHECK(Krange_base_get_value(sl) == 5);
	Krange_base_set_range(sl, 2, 1); /* maximum below minimum becomes the minimum */
	CHECK(Krange_base_get_maximum(sl) == 2 && Krange_base_get_value(sl) == 2);
	Kelement_destroy(sl);
	Kelement_destroy(pb);
}

static void test_items_and_selection(void) {
	Kid lb = Klist_box_create(), c1, c2;
	g_count = 0;
	Kelement_on(lb, K_EVENT_SELECTION_CHANGED, on_event, NULL);
	c1 = Kitems_control_add_text(lb, "one");
	c2 = Kitems_control_add_text(lb, "two");
	Kitems_control_add_text(lb, "three");
	CHECK(Kitems_control_get_count(lb) == 3);
	CHECK(Kelement_is_a(c1, K_CLASS_LIST_BOX_ITEM));
	CHECK_STR(Kitems_control_get_text(lb, 1), "two");
	CHECK(Kselector_get_selected_index(lb) == -1);
	Kselector_set_selected_index(lb, 1);
	CHECK(Kselector_get_selected_index(lb) == 1 && g_count == 1);
	CHECK(Kselector_get_selected_container(lb) == c2);
	Kselector_set_selected_index(lb, 1);
	CHECK(g_count == 1);
	/* removing an item before the selected one shifts the index, removing the selected one clears it */
	Kitems_control_remove_at(lb, 0);
	CHECK(Kselector_get_selected_index(lb) == 0 && !Kelement_is_valid(c1));
	Kitems_control_remove_at(lb, 0);
	CHECK(Kselector_get_selected_index(lb) == -1 && g_count == 2);
	Kitems_control_clear(lb);
	CHECK(Kitems_control_get_count(lb) == 0);
	/* an element is wrapped in a container */
	{
		Kid t = Ktext_block_create("custom"), c = Kitems_control_add(lb, t);
		CHECK(Kelement_is_a(c, K_CLASS_LIST_BOX_ITEM) && Kcontent_control_get_content(c) == t);
		CHECK_STR(Kitems_control_get_text(lb, 0), "");
	}
	Kelement_destroy(lb);
}

static void test_multiple_selection(void) {
	Kid lb = Klist_box_create(), a, c;
	int indices[4];
	Klist_box_set_selection_mode(lb, K_SELECTION_MULTIPLE);
	CHECK(Klist_box_get_selection_mode(lb) == K_SELECTION_MULTIPLE);
	a = Kitems_control_add_text(lb, "a");
	Kitems_control_add_text(lb, "b");
	c = Kitems_control_add_text(lb, "c");
	Kselector_item_set_is_selected(a, K_TRUE);
	Kselector_item_set_is_selected(c, K_TRUE);
	CHECK(Kselector_item_get_is_selected(a) && !Kselector_item_get_is_selected(Kitems_control_get_container(lb, 1)) && Kselector_item_get_is_selected(c));
	CHECK(Klist_box_get_selected_indices(lb, indices, 4) == 2 && indices[0] == 0 && indices[1] == 2);
	CHECK(Kselector_get_selected_index(lb) == 0);
	Kselector_item_set_is_selected(a, K_FALSE);
	CHECK(!Kselector_item_get_is_selected(a) && Kselector_item_get_is_selected(c));
	CHECK(Kselector_get_selected_index(lb) == 2);
	Klist_box_select_all(lb);
	CHECK(Klist_box_get_selected_indices(lb, NULL, 0) == 3);
	Kelement_destroy(lb);
}

static void test_text_box(void) {
	Kid tb = Ktext_box_create("hello");
	g_count = 0;
	Kelement_on(tb, K_EVENT_TEXT_CHANGED, on_event, NULL);
	CHECK_STR(Ktext_box_get_text(tb), "hello");
	Ktext_box_set_text(tb, "hello");
	CHECK(g_count == 0);
	Ktext_box_set_text(tb, "world");
	CHECK(g_count == 1);
	Ktext_box_select(tb, 1, 3);
	CHECK(Ktext_box_get_selection_start(tb) == 1 && Ktext_box_get_selection_length(tb) == 3);
	Ktext_box_select(tb, 3, 100);
	CHECK(Ktext_box_get_selection_length(tb) == 2);
	Ktext_box_select_all(tb);
	CHECK(Ktext_box_get_selection_length(tb) == 5);
	Kelement_destroy(tb);
}

static void test_theme_colors(void) {
	Kid root = Kroot_create(100, 100);
	KColor c = Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_COLOR);
	CHECK(c.r == 1 && c.g == 1 && c.b == 1 && c.a == 1);
	Kroot_set_theme(root, K_THEME_LIGHT);
	c = Kroot_get_theme_color(root, K_RES_PHONE_BACKGROUND_COLOR);
	CHECK(c.r == 1 && c.a == 1);
	c = Kroot_get_theme_color(root, K_RES_PHONE_FOREGROUND_COLOR);
	CHECK(c.r == 0 && c.a == 1); /* #FF000000 of the 8.1 Light dictionary (the WP8 table had #DE000000) */
	Kroot_set_accent(root, K_ACCENT_LIME);
	c = Kroot_get_accent_color(root);
	CHECK_NEAR(c.r * 255, 0xA4);
	CHECK_NEAR(c.g * 255, 0xC4);
	CHECK(Kroot_get_theme(root) == K_THEME_LIGHT);
	Kroot_destroy(root);
}

static void test_text_block_styles(void) {
	Kid t = Ktext_block_create("x");
	CHECK(Ktext_block_set_style(t, K_TEXT_STYLE_PHONE_TITLE1));
	CHECK(!Ktext_block_set_style(t, K_TEXT_STYLE_NONE));
	CHECK(!Ktext_block_set_style(t, K_TEXT_STYLE_LAST_));
	CHECK_NEAR(Kelement_get_margin(t).left, 12);
	/* the 8.1 styles have no margin; a property set after a style wins */
	CHECK(Ktext_block_set_style(t, K_TEXT_STYLE_HEADER));
	CHECK_NEAR(Kelement_get_margin(t).left, 0);
	Ktext_block_set_font_size(t, 40);
	Kelement_destroy(t);
}

static void test_new_classes(void) {
	Kid sw = Ktoggle_switch_create(), ring = Kprogress_ring_create();
	CHECK(Kelement_get_class(sw) == K_CLASS_TOGGLE_SWITCH && Kelement_get_class(ring) == K_CLASS_PROGRESS_RING);
	CHECK(Kelement_is_a(sw, K_CLASS_CONTROL) && !Kelement_is_a(sw, K_CLASS_CONTENT_CONTROL));
	CHECK(Kelement_is_a(ring, K_CLASS_CONTROL) && !Kelement_is_a(ring, K_CLASS_RANGE_BASE));
	CHECK_STR(Kelement_get_class_name(sw), "ToggleSwitch");
	CHECK_STR(Kelement_get_class_name(ring), "ProgressRing");
	Ktoggle_switch_set_is_on(ring, K_TRUE); /* not a ToggleSwitch */
	CHECK(Kget_error() == K_ERROR_WRONG_CLASS);
	Kelement_destroy(sw);
	Kelement_destroy(ring);
}

static void test_toggle_switch_api(void) {
	Kid sw = Ktoggle_switch_create();
	g_count = 0;
	/* off, with the strings of the template */
	CHECK(!Ktoggle_switch_get_is_on(sw));
	CHECK_STR(Ktoggle_switch_get_header(sw), "");
	CHECK_STR(Ktoggle_switch_get_on_content(sw), "On");
	CHECK_STR(Ktoggle_switch_get_off_content(sw), "Off");
	Ktoggle_switch_on_toggled(sw, on_event, NULL);
	Ktoggle_switch_set_is_on(sw, K_TRUE);
	CHECK(Ktoggle_switch_get_is_on(sw) && g_count == 1);
	Ktoggle_switch_set_is_on(sw, K_TRUE);
	CHECK(g_count == 1); /* no change, no event */
	Ktoggle_switch_set_is_on(sw, K_FALSE);
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_count == 2);
	Ktoggle_switch_set_header(sw, "Wi-Fi");
	Ktoggle_switch_set_on_content(sw, "Connected");
	Ktoggle_switch_set_off_content(sw, "Disconnected");
	CHECK_STR(Ktoggle_switch_get_header(sw), "Wi-Fi");
	CHECK_STR(Ktoggle_switch_get_on_content(sw), "Connected");
	CHECK_STR(Ktoggle_switch_get_off_content(sw), "Disconnected");
	Ktoggle_switch_set_header(sw, NULL);
	CHECK_STR(Ktoggle_switch_get_header(sw), "");
	Kelement_destroy(sw);
}

static void test_progress_ring_api(void) {
	Kid ring = Kprogress_ring_create();
	CHECK(!Kprogress_ring_get_is_active(ring));
	Kprogress_ring_set_is_active(ring, K_TRUE);
	CHECK(Kprogress_ring_get_is_active(ring));
	Kprogress_ring_set_is_active(ring, K_FALSE);
	CHECK(!Kprogress_ring_get_is_active(ring));
	Kelement_destroy(ring);
}

static void test_81_properties(void) {
	Kid sl = Kslider_create(K_HORIZONTAL), pb = Kprogress_bar_create(), tb = Ktext_box_create(""), pw = Kpassword_box_create(), rb = Krepeat_button_create("+");
	/* slider */
	CHECK(Kslider_get_step_frequency(sl) == 1 && Kslider_get_tick_frequency(sl) == 0);
	Kslider_set_step_frequency(sl, 0.5);
	Kslider_set_tick_frequency(sl, 10);
	Kslider_set_tick_placement(sl, K_TICK_INLINE);
	Kslider_set_snaps_to(sl, K_SNAPS_TO_TICKS);
	CHECK(Kslider_get_step_frequency(sl) == 0.5 && Kslider_get_tick_frequency(sl) == 10);
	Kslider_set_header(sl, "Volume");
	CHECK_STR(Kslider_get_header(sl), "Volume");
	/* progress bar */
	CHECK(!Kprogress_bar_get_is_indeterminate(pb));
	Kprogress_bar_set_is_indeterminate(pb, K_TRUE);
	CHECK(Kprogress_bar_get_is_indeterminate(pb));
	Kprogress_bar_set_show_error(pb, K_TRUE);
	Kprogress_bar_set_show_paused(pb, K_TRUE);
	/* text box: the style of the 8.1 template */
	CHECK_NEAR(Kelement_get_margin(tb).top, 9.5);
	CHECK_NEAR(Kelement_get_margin(tb).bottom, 9.5);
	CHECK_NEAR(Kelement_get_margin(tb).left, 0);
	CHECK_STR(Ktext_box_get_placeholder_text(tb), "");
	Ktext_box_set_placeholder_text(tb, "Search");
	Ktext_box_set_header(tb, "Query");
	CHECK_STR(Ktext_box_get_placeholder_text(tb), "Search");
	CHECK_STR(Ktext_box_get_header(tb), "Query");
	Ktext_box_set_placeholder_text(tb, NULL);
	CHECK_STR(Ktext_box_get_placeholder_text(tb), "");
	Ktext_box_set_is_read_only(tb, K_TRUE);
	Ktext_box_set_max_length(tb, 7);
	CHECK(Ktext_box_get_is_read_only(tb) && Ktext_box_get_max_length(tb) == 7);
	/* password box */
	Kpassword_box_set_password(pw, "hunter2");
	CHECK_STR(Kpassword_box_get_password(pw), "hunter2");
	CHECK_STR(Ktext_box_get_text(pw), "hunter2");
	Kpassword_box_set_header(pw, "Password");
	Kpassword_box_set_placeholder_text(pw, "secret");
	CHECK_STR(Ktext_box_get_header(pw), "Password");
	Kpassword_box_set_header(tb, "x"); /* not a PasswordBox */
	CHECK(Kget_error() == K_ERROR_WRONG_CLASS);
	/* repeat button: Delay and Interval of 8.1 */
	CHECK(Krepeat_button_get_delay(rb) == 250 && Krepeat_button_get_interval(rb) == 250);
	Kelement_destroy(sl); Kelement_destroy(pb); Kelement_destroy(tb); Kelement_destroy(pw); Kelement_destroy(rb);
}

/* the sizes the 8.1 styles give: MinWidth/MinHeight, touch overhangs, the rows of the templates */
static void test_81_default_sizes(void) {
	Kid root = Kroot_create(480, 800), stack = Kstack_panel_create(K_VERTICAL);
	Kid b = Kpanel_add(stack, Kbutton_create("OK")), cb = Kpanel_add(stack, Kcheck_box_create("c")), rb = Kpanel_add(stack, Kradio_button_create("r", NULL));
	Kid link = Kpanel_add(stack, Khyperlink_button_create("link")), tb = Kpanel_add(stack, Ktext_box_create("t")), sl = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Kid pb = Kpanel_add(stack, Kprogress_bar_create()), sw = Kpanel_add(stack, Ktoggle_switch_create()), ring = Kpanel_add(stack, Kprogress_ring_create());
	Kid tbh = Kpanel_add(stack, Ktext_box_create("h"));
	Ktext_box_set_header(tbh, "Header");
	Kroot_set_visual(root, stack);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(b).width, 109);
	CHECK_NEAR(Kelement_get_bounds(b).height, 57.5);
	CHECK_NEAR(Kelement_get_bounds(cb).width, 168);
	CHECK_NEAR(Kelement_get_bounds(cb).height, 57.5); /* 16 + the 25.5 box + 16 */
	CHECK_NEAR(Kelement_get_bounds(rb).width, 168);
	CHECK(Kelement_get_bounds(link).height > 30 && Kelement_get_bounds(link).height < 60); /* 29.86 px text, padding 3 and 4 */
	CHECK_NEAR(Kelement_get_bounds(tb).height, 34);
	CHECK(Kelement_get_bounds(tbh).height > 34 + 20); /* the header and the 4.8 under it */
	CHECK_NEAR(Kelement_get_bounds(sl).height, 57);
	CHECK_NEAR(Kelement_get_bounds(sl).width, 480);
	CHECK_NEAR(Kelement_get_bounds(pb).height, 3.5);
	CHECK_NEAR(Kelement_get_bounds(sw).height, 45.5); /* the 36 px switch and the 9.5 px row under it */
	CHECK_NEAR(Kelement_get_bounds(sw).width, 480);
	CHECK_NEAR(Kelement_get_bounds(ring).height, 57.5);
	CHECK_NEAR(Kelement_get_bounds(ring).width, 480);
	Ktoggle_switch_set_header(sw, "Header");
	Kroot_update_layout(root);
	CHECK(Kelement_get_bounds(sw).height > 45.5 + 15);
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	Kset_error_callback(quiet_errors, NULL); /* the tests provoke errors on purpose */
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
	Kshutdown();
	return test_finish("test_controls");
}
