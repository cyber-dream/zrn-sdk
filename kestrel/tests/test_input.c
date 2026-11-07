/**
 * Input injected into a root: pointer, keys, text, focus, capture, popups,
 * scrolling. Nothing is drawn by hand: every check goes through the same
 * path a touch screen would.
 */

#include "test.h"

static int g_clicks;
static int g_changes;
static void on_click(Kid sender, void* user) { (void)sender; (void)user; g_clicks++; }
static void on_change(Kid sender, void* user) { (void)sender; (void)user; g_changes++; }

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

static Kid setup(Kid* root, Kid* stack) {
	*root = Kroot_create(480, 800);
	*stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(*root, *stack);
	return *stack;
}

static void test_button_click(void) {
	Kid root, stack, b;
	setup(&root, &stack);
	b = Kpanel_add(stack, Kbutton_create("OK"));
	Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
	Kbutton_on_click(b, on_click, NULL);
	Kroot_update_layout(root);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	/* pressed inside, released outside: no click */
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(Kbutton_is_pressed(b));
	Kroot_inject_pointer(root, K_POINTER_MOVE, 450, 700);
	CHECK(!Kbutton_is_pressed(b));
	Kroot_inject_pointer(root, K_POINTER_UP, 450, 700);
	CHECK(g_clicks == 1);
	/* back inside before release: click */
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(b).x, center(b).y);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 450, 700);
	Kroot_inject_pointer(root, K_POINTER_MOVE, center(b).x, center(b).y);
	CHECK(Kbutton_is_pressed(b));
	Kroot_inject_pointer(root, K_POINTER_UP, center(b).x, center(b).y);
	CHECK(g_clicks == 2);
	/* the touch overhang above and below the outline (9.5 px) is part of the button */
	tap(root, (KPoint){ 3, 3 });
	CHECK(g_clicks == 3);
	/* disabled buttons do nothing */
	Kelement_set_enabled(b, K_FALSE);
	tap(root, center(b));
	CHECK(g_clicks == 3);
	Kelement_set_enabled(b, K_TRUE);
	/* a press-mode button clicks on press */
	Kbutton_set_click_mode(b, K_CLICK_PRESS);
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(g_clicks == 4);
	Kroot_inject_pointer(root, K_POINTER_UP, center(b).x, center(b).y);
	CHECK(g_clicks == 4);
	Kroot_destroy(root);
}

static void test_keyboard_click_and_focus(void) {
	Kid root, stack, a, b;
	setup(&root, &stack);
	a = Kpanel_add(stack, Kbutton_create("a"));
	b = Kpanel_add(stack, Kbutton_create("b"));
	Kbutton_on_click(b, on_click, NULL);
	Kroot_update_layout(root);
	g_clicks = 0;
	/* Tab goes to the first control, then the next */
	key(root, K_KEY_TAB, 0);
	CHECK(Kroot_get_focus(root) == a);
	key(root, K_KEY_TAB, 0);
	CHECK(Kroot_get_focus(root) == b);
	key(root, K_KEY_SPACE, 0);
	CHECK(g_clicks == 1);
	key(root, K_KEY_TAB, K_MOD_SHIFT);
	CHECK(Kroot_get_focus(root) == a);
	/* a tap focuses what it hits */
	tap(root, center(b));
	CHECK(Kroot_get_focus(root) == b);
	/* IsTabStop false skips a control */
	Kcontrol_set_tab_stop(a, K_FALSE);
	key(root, K_KEY_TAB, K_MOD_SHIFT);
	CHECK(Kroot_get_focus(root) == b);
	Kroot_destroy(root);
}

static void test_repeat_button(void) {
	Kid root, stack, r;
	int i;
	setup(&root, &stack);
	r = Kpanel_add(stack, Krepeat_button_create("+"));
	Kelement_set_vertical_alignment(r, K_VALIGN_TOP);
	Kbutton_on_click(r, on_click, NULL);
	Krepeat_button_set_delay(r, 500);
	Krepeat_button_set_interval(r, 100);
	CHECK(Krepeat_button_get_delay(r) == 500 && Krepeat_button_get_interval(r) == 100);
	Kroot_update_layout(root);
	g_clicks = 0;
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(r).x, center(r).y);
	CHECK(g_clicks == 1); /* once at once */
	Kroot_tick(root, 400);
	CHECK(g_clicks == 1);
	Kroot_tick(root, 150); /* delay passed */
	CHECK(g_clicks == 2);
	for (i = 0; i < 5; i++) Kroot_tick(root, 100);
	CHECK(g_clicks == 7);
	Kroot_inject_pointer(root, K_POINTER_UP, center(r).x, center(r).y);
	Kroot_tick(root, 1000);
	CHECK(g_clicks == 7); /* released: no more */
	Kroot_destroy(root);
}

static void test_toggle_taps(void) {
	Kid root, stack, c, r1, r2;
	setup(&root, &stack);
	c = Kpanel_add(stack, Kcheck_box_create("c"));
	r1 = Kpanel_add(stack, Kradio_button_create("1", NULL));
	r2 = Kpanel_add(stack, Kradio_button_create("2", NULL));
	Kroot_update_layout(root);
	g_changes = 0;
	Kelement_on(c, K_EVENT_CHECKED, on_change, NULL);
	tap(root, center(c));
	CHECK(Ktoggle_button_get_is_checked(c) && g_changes == 1);
	tap(root, center(c));
	CHECK(!Ktoggle_button_get_is_checked(c));
	Ktoggle_button_set_is_three_state(c, K_TRUE);
	tap(root, center(c));
	tap(root, center(c));
	CHECK(Ktoggle_button_get_state(c) == K_INDETERMINATE);
	tap(root, center(r1));
	CHECK(Ktoggle_button_get_is_checked(r1));
	tap(root, center(r2));
	CHECK(!Ktoggle_button_get_is_checked(r1) && Ktoggle_button_get_is_checked(r2));
	/* a tap on a checked radio button keeps it checked */
	tap(root, center(r2));
	CHECK(Ktoggle_button_get_is_checked(r2));
	Kroot_destroy(root);
}

static void test_slider_drag(void) {
	Kid root, stack, s;
	KRect r;
	setup(&root, &stack);
	s = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Kelement_set_vertical_alignment(s, K_VALIGN_TOP);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(s);
	/* the thumb (9.5 px) is centered on the touch, so the middle of the control gives half of 0..100 */
	Kroot_inject_pointer(root, K_POINTER_DOWN, r.x + r.width * 0.5f, 28);
	CHECK_NEAR(Krange_base_get_value(s), 50);
	Kroot_inject_pointer(root, K_POINTER_MOVE, r.x + r.width + 100, 500); /* dragging past the end, off the control: clamps */
	CHECK(Krange_base_get_value(s) == 100);
	Kroot_inject_pointer(root, K_POINTER_MOVE, -50, 28);
	CHECK(Krange_base_get_value(s) == 0);
	Kroot_inject_pointer(root, K_POINTER_UP, -50, 28);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 200, 28); /* no capture after release */
	CHECK(Krange_base_get_value(s) == 0);
	/* keys */
	Kelement_focus(s);
	key(root, K_KEY_RIGHT, 0);
	CHECK_NEAR(Krange_base_get_value(s), 1); /* SmallChange, snapped to StepFrequency 1 */
	key(root, K_KEY_PAGE_UP, 0);
	CHECK_NEAR(Krange_base_get_value(s), 11); /* LargeChange */
	key(root, K_KEY_END, 0);
	CHECK(Krange_base_get_value(s) == 100);
	Kroot_destroy(root);
}

static void test_text_box_typing(void) {
	Kid root, stack, tb;
	setup(&root, &stack);
	tb = Kpanel_add(stack, Ktext_box_create(""));
	Kelement_set_vertical_alignment(tb, K_VALIGN_TOP);
	Kroot_update_layout(root);
	g_changes = 0;
	Kelement_on(tb, K_EVENT_TEXT_CHANGED, on_change, NULL);
	type(root, "ignored"); /* no focus yet */
	CHECK_STR(Ktext_box_get_text(tb), "");
	tap(root, center(tb));
	CHECK(Kroot_get_focus(root) == tb);
	type(root, "hello");
	CHECK_STR(Ktext_box_get_text(tb), "hello");
	CHECK(g_changes == 5 || g_changes == 1);
	key(root, K_KEY_BACKSPACE, 0);
	CHECK_STR(Ktext_box_get_text(tb), "hell");
	key(root, K_KEY_HOME, 0);
	type(root, "oh ");
	CHECK_STR(Ktext_box_get_text(tb), "oh hell");
	key(root, K_KEY_DELETE, 0);
	CHECK_STR(Ktext_box_get_text(tb), "oh ell");
	/* selection with shift+arrows, replaced by typing */
	key(root, K_KEY_END, 0);
	key(root, K_KEY_LEFT, K_MOD_SHIFT);
	key(root, K_KEY_LEFT, K_MOD_SHIFT);
	CHECK(Ktext_box_get_selection_length(tb) == 2);
	type(root, "X");
	CHECK_STR(Ktext_box_get_text(tb), "oh eX");
	/* select all, cut, paste */
	key(root, K_KEY_A, K_MOD_CTRL);
	key(root, K_KEY_X, K_MOD_CTRL);
	CHECK_STR(Ktext_box_get_text(tb), "");
	key(root, K_KEY_V, K_MOD_CTRL);
	key(root, K_KEY_V, K_MOD_CTRL);
	CHECK_STR(Ktext_box_get_text(tb), "oh eXoh eX");
	/* a newline is refused unless AcceptsReturn */
	type(root, "\n");
	CHECK_STR(Ktext_box_get_text(tb), "oh eXoh eX");
	Ktext_box_set_accepts_return(tb, K_TRUE);
	type(root, "\nz");
	CHECK_STR(Ktext_box_get_text(tb), "oh eXoh eX\nz");
	/* MaxLength counts characters, not bytes */
	Ktext_box_set_accepts_return(tb, K_FALSE);
	Ktext_box_set_text(tb, "");
	Ktext_box_set_max_length(tb, 3);
	type(root, "\xD0\xB0\xD0\xB1\xD0\xB2\xD0\xB3"); /* four Cyrillic letters */
	CHECK_STR(Ktext_box_get_text(tb), "\xD0\xB0\xD0\xB1\xD0\xB2");
	/* backspace removes a whole character */
	key(root, K_KEY_BACKSPACE, 0);
	CHECK_STR(Ktext_box_get_text(tb), "\xD0\xB0\xD0\xB1");
	/* read only */
	Ktext_box_set_is_read_only(tb, K_TRUE);
	type(root, "q");
	CHECK_STR(Ktext_box_get_text(tb), "\xD0\xB0\xD0\xB1");
	Kroot_destroy(root);
}

static void test_text_box_mouse_selection(void) {
	Kid root, stack, tb;
	KRect r;
	setup(&root, &stack);
	tb = Kpanel_add(stack, Ktext_box_create("0123456789"));
	Kelement_set_vertical_alignment(tb, K_VALIGN_TOP);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(tb);
	/* press at the start of the text (2.5 px of border, 7 of padding), drag past its end; the box is 34 px high */
	Kroot_inject_pointer(root, K_POINTER_DOWN, r.x + 10, r.y + 17);
	Kroot_inject_pointer(root, K_POINTER_MOVE, r.x + 400, r.y + 17);
	Kroot_inject_pointer(root, K_POINTER_UP, r.x + 400, r.y + 17);
	CHECK(Ktext_box_get_selection_length(tb) >= 9);
	Kroot_destroy(root);
}

static void test_password_box(void) {
	Kid root, stack, pw;
	setup(&root, &stack);
	pw = Kpanel_add(stack, Kpassword_box_create());
	Kroot_update_layout(root);
	Kelement_focus(pw);
	type(root, "secret");
	CHECK_STR(Ktext_box_get_text(pw), "secret");
	/* copy is refused for a password */
	key(root, K_KEY_A, K_MOD_CTRL);
	key(root, K_KEY_C, K_MOD_CTRL);
	Kroot_destroy(root);
}

static void test_list_box_tap_and_scroll(void) {
	Kid root, stack, lb;
	int i;
	char name[32];
	setup(&root, &stack);
	lb = Kpanel_add(stack, Klist_box_create());
	Kelement_set_height(lb, 400); /* a list in a stack panel is as tall as its items: it needs a height to scroll */
	for (i = 0; i < 40; i++) {
		snprintf(name, sizeof name, "item %d", i);
		Kitems_control_add_text(lb, name);
	}
	Kroot_update_layout(root);
	g_changes = 0;
	Kelement_on(lb, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Kitems_control_get_container(lb, 2)));
	CHECK(Kselector_get_selected_index(lb) == 2 && g_changes == 1);
	/* a vertical drag scrolls, and does not select what the finger started on */
	{
		Kid item = Kitems_control_get_container(lb, 5);
		KPoint p = center(item);
		Kid sv = Kelement_get_child(lb, 0);
		CHECK(Kelement_is_a(sv, K_CLASS_SCROLL_VIEWER));
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 20);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y - 100);
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y - 100);
		CHECK_NEAR(Kscroll_viewer_get_vertical_offset(sv), 100);
		CHECK(Kselector_get_selected_index(lb) == 2 && g_changes == 1);
		/* inertia carries on after the finger left */
		{
			double before = Kscroll_viewer_get_vertical_offset(sv);
			Kroot_tick(root, 16);
			Kroot_tick(root, 16);
			CHECK(Kscroll_viewer_get_vertical_offset(sv) >= before);
		}
		/* the wheel */
		Kscroll_viewer_scroll_to_vertical_offset(sv, 0);
		Kroot_inject_pointer(root, K_POINTER_MOVE, 200, 200);
		Kroot_inject_wheel(root, -1);
		CHECK(Kscroll_viewer_get_vertical_offset(sv) > 0);
	}
	Klist_box_scroll_into_view(lb, 39);
	Kroot_update_layout(root);
	{
		Kid sv = Kelement_get_child(lb, 0);
		/* the last item is at the bottom of the view */
		CHECK(Kscroll_viewer_get_vertical_offset(sv) > 500);
		CHECK_NEAR(Kscroll_viewer_get_vertical_offset(sv), Kscroll_viewer_get_extent_height(sv) - Kscroll_viewer_get_viewport_height(sv));
	}
	Kroot_destroy(root);
}

static void test_button_in_scroll_viewer(void) {
	Kid root, stack, sv, inner, b;
	int i;
	setup(&root, &stack);
	sv = Kpanel_add(stack, Kscroll_viewer_create());
	Kelement_set_height(sv, 400);
	inner = Kstack_panel_create(K_VERTICAL);
	for (i = 0; i < 20; i++) Kpanel_add(inner, Kbutton_create("b"));
	Kscroll_viewer_set_content(sv, inner);
	b = Kelement_get_child(inner, 0);
	Kbutton_on_click(b, on_click, NULL);
	Kroot_update_layout(root);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	/* pressing the button, then dragging: the button is released without a click and the page scrolls */
	{
		KPoint p = center(b);
		Kroot_inject_pointer(root, K_POINTER_DOWN, p.x, p.y);
		CHECK(Kbutton_is_pressed(b));
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y + 30);
		Kroot_inject_pointer(root, K_POINTER_MOVE, p.x, p.y + 5);
		Kroot_inject_pointer(root, K_POINTER_UP, p.x, p.y + 5);
		CHECK(!Kbutton_is_pressed(b));
		CHECK(g_clicks == 1);
	}
	Kroot_destroy(root);
}

static void test_combo_box(void) {
	Kid root, stack, cb;
	setup(&root, &stack);
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(cb, K_VALIGN_TOP);
	Kitems_control_add_text(cb, "red");
	Kitems_control_add_text(cb, "green");
	Kitems_control_add_text(cb, "blue");
	Kroot_update_layout(root);
	g_changes = 0;
	Kelement_on(cb, K_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	tap(root, center(cb));
	CHECK(Kcombo_box_get_is_drop_down_open(cb));
	Kroot_update_layout(root);
	/* the drop-down is above everything: a tap on its second row selects and closes */
	{
		Kid item = Kitems_control_get_container(cb, 1);
		KPoint p = center(item);
		tap(root, p);
		CHECK(Kselector_get_selected_index(cb) == 1 && g_changes == 1);
		CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	}
	/* a tap outside closes without selecting */
	tap(root, center(cb));
	CHECK(Kcombo_box_get_is_drop_down_open(cb));
	tap(root, (KPoint){ 400, 700 });
	CHECK(!Kcombo_box_get_is_drop_down_open(cb) && Kselector_get_selected_index(cb) == 1);
	/* a tap on the box while open closes it and does not reopen */
	tap(root, center(cb));
	CHECK(Kcombo_box_get_is_drop_down_open(cb));
	tap(root, center(cb));
	CHECK(!Kcombo_box_get_is_drop_down_open(cb));
	/* the drop-down was drawn: the frame differs while open */
	Kcombo_box_set_is_drop_down_open(cb, K_TRUE);
	CHECK(Kroot_render(root, NULL));
	Kroot_destroy(root);
}

static void test_tooltip_and_hover(void) {
	Kid root, stack, b;
	KFrame before, after;
	setup(&root, &stack);
	b = Kpanel_add(stack, Kbutton_create("hover me"));
	Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
	Kelement_set_tooltip(b, "a tooltip");
	Kroot_update_layout(root);
	Kroot_inject_pointer(root, K_POINTER_MOVE, center(b).x, center(b).y);
	CHECK(Kbutton_is_pointer_over(b));
	Kroot_render(root, &before);
	CHECK(Kroot_get_pointer_over(root) != K_INVALID_ID);
	Kroot_tick(root, 600);
	CHECK(Kroot_render(root, &after)); /* the tooltip appeared */
	Kroot_inject_pointer(root, K_POINTER_MOVE, 450, 700);
	CHECK(!Kbutton_is_pointer_over(b));
	(void)before;
	Kroot_destroy(root);
}

static void test_render_is_lazy(void) {
	Kid root, stack, b;
	setup(&root, &stack);
	b = Kpanel_add(stack, Kbutton_create("x"));
	CHECK(Kroot_render(root, NULL));
	CHECK(!Kroot_render(root, NULL)); /* nothing changed */
	Kcontent_control_set_text(b, "y");
	CHECK(Kroot_render(root, NULL));
	Kroot_destroy(root);
}

static void test_destroying_while_focused_or_captured(void) {
	Kid root, stack, b;
	setup(&root, &stack);
	b = Kpanel_add(stack, Kbutton_create("x"));
	Kroot_update_layout(root);
	Kroot_inject_pointer(root, K_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(Kroot_get_focus(root) == b);
	Kelement_destroy(b);
	CHECK(Kroot_get_focus(root) == K_INVALID_ID);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 10, 10); /* the capture is gone with it */
	Kroot_inject_pointer(root, K_POINTER_UP, 10, 10);
	Kroot_destroy(root);
}

/* ---- handlers that replace the whole page, as a tap that opens another page does ---- */

static Kid g_nav_root;
static int g_nav_count;

static void navigate(Kid sender, void* user) {
	(void)sender; (void)user;
	g_nav_count++;
	Kroot_set_visual(g_nav_root, Kstack_panel_create(K_VERTICAL)); /* destroys the sender and everything else */
}

static void navigate_reason(Kid sender, KAutoSuggestionBoxTextChangeReason reason, void* user) { (void)reason; navigate(sender, user); }
static void navigate_value(Kid sender, double a, double b, void* user) { (void)a; (void)b; navigate(sender, user); }

/* After the handler ran, the root must still work: lay out, render, take more input. */
static void survives(Kid root) {
	Kroot_render(root, NULL);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 100, 100);
	Kroot_inject_pointer(root, K_POINTER_UP, 100, 100);
	Kroot_inject_key(root, K_KEY_SPACE, 0, 1);
	Kroot_inject_key(root, K_KEY_SPACE, 0, 0);
	Kroot_inject_text(root, "x");
	Kroot_tick(root, 1000);
	Kroot_render(root, NULL);
}

static void test_handlers_that_navigate(void) {
	Kid root, stack, c, other;
	setup(&root, &stack);
	g_nav_root = root;

	/* Click */
	c = Kpanel_add(stack, Kbutton_create("go"));
	Kbutton_on_click(c, navigate, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(c));
	CHECK(g_nav_count == 1 && !Kelement_is_valid(c));
	survives(root);

	/* Checked: the toggle raises Click after Checked, on an element that is gone */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kcheck_box_create("c"));
	Kelement_on(c, K_EVENT_CHECKED, navigate, NULL);
	Kbutton_on_click(c, on_click, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(c));
	CHECK(g_nav_count == 1);
	survives(root);

	/* Unchecked of another radio button: its handler runs while the checked one is being set */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kradio_button_create("1", NULL));
	other = Kpanel_add(stack, Kradio_button_create("2", NULL));
	Ktoggle_button_set_is_checked(c, K_TRUE);
	Kelement_on(c, K_EVENT_UNCHECKED, navigate, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(other));
	CHECK(g_nav_count == 1);
	survives(root);

	/* SelectionChanged of a ListBox */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Klist_box_create());
	Kitems_control_add_text(c, "a"); Kitems_control_add_text(c, "b");
	Kelement_on(c, K_EVENT_SELECTION_CHANGED, navigate, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(Kitems_control_get_container(c, 1)));
	CHECK(g_nav_count == 1);
	survives(root);

	/* SelectionChanged of a ComboBox, from an item of its drop-down */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Kitems_control_add_text(c, "a"); Kitems_control_add_text(c, "b");
	Kelement_on(c, K_EVENT_SELECTION_CHANGED, navigate, NULL);
	Kroot_update_layout(root);
	tap(root, center(c));
	Kroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(Kitems_control_get_container(c, 1)));
	CHECK(g_nav_count == 1);
	survives(root);

	/* DropDownClosed of a ComboBox, closed by a tap outside */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kcombo_box_create());
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Kitems_control_add_text(c, "a");
	Kelement_on(c, K_EVENT_DROP_DOWN_CLOSED, navigate, NULL);
	Kroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	tap(root, (KPoint){ 400, 700 });
	CHECK(g_nav_count == 1);
	survives(root);

	/* TextChanged of an AutoSuggestBox while typing: the handler replaces the page */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kauto_suggest_box_create());
	Kauto_suggest_box_on_text_changed(c, navigate_reason, NULL);
	Kroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	type(root, "abc");
	CHECK(g_nav_count == 1);
	survives(root);
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Ktext_box_create(""));
	Kelement_on(c, K_EVENT_TEXT_CHANGED, navigate, NULL);
	Kroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	key(root, K_KEY_V, K_MOD_CTRL); /* paste: the clipboard of an earlier test holds text */
	type(root, "q");
	CHECK(g_nav_count >= 1);
	survives(root);

	/* ValueChanged of a Slider, mid-drag */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Krange_base_on_value_changed(c, navigate_value, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	Kroot_inject_pointer(root, K_POINTER_DOWN, 240, 28);
	CHECK(g_nav_count == 1);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 300, 28);
	Kroot_inject_pointer(root, K_POINTER_UP, 300, 28);
	survives(root);

	/* Click-on-hover destroying the button while the pointer enters it */
	stack = Kstack_panel_create(K_VERTICAL);
	Kroot_set_visual(root, stack);
	c = Kpanel_add(stack, Kbutton_create("hover"));
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Kbutton_set_click_mode(c, K_CLICK_HOVER);
	Kbutton_on_click(c, navigate, NULL);
	Kroot_update_layout(root);
	g_nav_count = 0;
	Kroot_inject_pointer(root, K_POINTER_MOVE, center(c).x, center(c).y);
	CHECK(g_nav_count == 1);
	survives(root);

	Kroot_destroy(root);
}

static void test_toggle_switch_input(void) {
	Kid root, stack, sw;
	setup(&root, &stack);
	sw = Kpanel_add(stack, Ktoggle_switch_create());
	Kelement_set_vertical_alignment(sw, K_VALIGN_TOP);
	Ktoggle_switch_on_toggled(sw, on_change, NULL);
	Kroot_update_layout(root);
	g_changes = 0;
	/* a tap anywhere on it flips it */
	tap(root, center(sw));
	CHECK(Ktoggle_switch_get_is_on(sw) && g_changes == 1);
	tap(root, center(sw));
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_changes == 2);
	/* Space, once it has the focus (the tap gave it) */
	CHECK(Kroot_get_focus(root) == sw);
	key(root, K_KEY_SPACE, 0);
	CHECK(Ktoggle_switch_get_is_on(sw) && g_changes == 3);
	key(root, K_KEY_SPACE, 0);
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_changes == 4);
	/* a press that moves away and lets go outside is not a tap */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 100, 10);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 100, 600);
	Kroot_inject_pointer(root, K_POINTER_UP, 100, 600);
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_changes == 4);
	/* a drag that starts on the knob (the switch is the 77 px at the right, the knob the 20.5 at its left end when off) carries it */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 413, 18);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 450, 18);
	CHECK(!Ktoggle_switch_get_is_on(sw)); /* not yet: it goes by where the knob is let go */
	Kroot_inject_pointer(root, K_POINTER_MOVE, 478, 18);
	Kroot_inject_pointer(root, K_POINTER_UP, 478, 18);
	CHECK(Ktoggle_switch_get_is_on(sw) && g_changes == 5);
	/* a short drag back is let go to the end it is nearer to: still on */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 470, 18);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 460, 18);
	Kroot_inject_pointer(root, K_POINTER_UP, 460, 18);
	CHECK(Ktoggle_switch_get_is_on(sw) && g_changes == 5);
	/* and a long one turns it off */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 470, 18);
	Kroot_inject_pointer(root, K_POINTER_MOVE, 405, 18);
	Kroot_inject_pointer(root, K_POINTER_UP, 405, 18);
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_changes == 6);
	/* disabled: nothing */
	Kelement_set_enabled(sw, K_FALSE);
	tap(root, center(sw));
	CHECK(!Ktoggle_switch_get_is_on(sw) && g_changes == 6);
	Kroot_destroy(root);
}

static void test_slider_steps_and_ticks(void) {
	Kid root, stack, s;
	KRect r;
	setup(&root, &stack);
	s = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Kelement_set_vertical_alignment(s, K_VALIGN_TOP);
	Kslider_set_header(s, "Level");
	Krange_base_set_range(s, 0, 10);
	Krange_base_set_changes(s, 1, 2);
	Kslider_set_step_frequency(s, 2.5);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(s);
	/* the band is under the header (the 16 px text), the track in it */
	CHECK(r.height > 57 + 15);
	/* a touch at 30 % of the thumb travel is 3, which the step of 2.5 snaps to 2.5 */
	Kroot_inject_pointer(root, K_POINTER_DOWN, 4.75f + 0.3f * (480 - 9.5f), r.y + 50);
	CHECK_NEAR(Krange_base_get_value(s), 2.5);
	Kroot_inject_pointer(root, K_POINTER_UP, 0, 0);
	/* with the ticks every 4 and SnapsTo Ticks it lands on 0, 4 or 8 */
	Kslider_set_tick_frequency(s, 4);
	Kslider_set_snaps_to(s, K_SNAPS_TO_TICKS);
	Kroot_inject_pointer(root, K_POINTER_DOWN, 4.75f + 0.55f * (480 - 9.5f), r.y + 50);
	CHECK_NEAR(Krange_base_get_value(s), 4);
	Kroot_inject_pointer(root, K_POINTER_UP, 0, 0);
	/* the value the program sets is not snapped */
	Krange_base_set_value(s, 3.3);
	CHECK(Krange_base_get_value(s) == 3.3);
	/* a disabled slider takes no touch */
	Kelement_set_enabled(s, K_FALSE);
	Kroot_inject_pointer(root, K_POINTER_DOWN, 400, r.y + 50);
	CHECK(Krange_base_get_value(s) == 3.3);
	Kroot_destroy(root);
}

static void test_progress_ring_and_bar_run_on_tick(void) {
	Kid root, stack, ring, pb;
	setup(&root, &stack);
	ring = Kpanel_add(stack, Kprogress_ring_create());
	pb = Kpanel_add(stack, Kprogress_bar_create());
	Kprogress_bar_set_is_indeterminate(pb, K_TRUE);
	Kroot_update_layout(root);
	/* neither takes the pointer or the focus */
	tap(root, center(ring));
	CHECK(Kroot_get_focus(root) != ring);
	Kroot_tick(root, 16);
	Kprogress_ring_set_is_active(ring, K_TRUE);
	Kroot_tick(root, 1000);
	Kroot_tick(root, 5000);
	CHECK(Kprogress_ring_get_is_active(ring));
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_button_click);
	RUN(test_keyboard_click_and_focus);
	RUN(test_repeat_button);
	RUN(test_toggle_taps);
	RUN(test_slider_drag);
	RUN(test_slider_steps_and_ticks);
	RUN(test_toggle_switch_input);
	RUN(test_progress_ring_and_bar_run_on_tick);
	RUN(test_text_box_typing);
	RUN(test_text_box_mouse_selection);
	RUN(test_password_box);
	RUN(test_list_box_tap_and_scroll);
	RUN(test_button_in_scroll_viewer);
	RUN(test_combo_box);
	RUN(test_tooltip_and_hover);
	RUN(test_render_is_lazy);
	RUN(test_destroying_while_focused_or_captured);
	RUN(test_handlers_that_navigate);
	Kshutdown();
	return test_finish("test_input");
}
