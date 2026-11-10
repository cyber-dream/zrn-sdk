/**
 * Input injected into a root: pointer, keys, text, focus, capture, popups,
 * scrolling. Nothing is drawn by hand: every check goes through the same
 * path a touch screen would.
 */

#include "test.h"

static int g_clicks;
static int g_changes;
static void on_click(Zid sender, void* user) { (void)sender; (void)user; g_clicks++; }
static void on_change(Zid sender, void* user) { (void)sender; (void)user; g_changes++; }

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

static Zid setup(Zid* root, Zid* stack) {
	*root = Zroot_create(480, 800);
	*stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(*root, *stack);
	return *stack;
}

static void test_button_click(void) {
	Zid root, stack, b;
	setup(&root, &stack);
	b = Zpanel_add(stack, Zbutton_create("OK"));
	Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
	Zbutton_on_click(b, on_click, NULL);
	Zroot_update_layout(root);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	/* pressed inside, released outside: no click */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(Zbutton_is_pressed(b));
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 450, 700);
	CHECK(!Zbutton_is_pressed(b));
	Zroot_inject_pointer(root, Z_POINTER_UP, 450, 700);
	CHECK(g_clicks == 1);
	/* back inside before release: click */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(b).x, center(b).y);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 450, 700);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, center(b).x, center(b).y);
	CHECK(Zbutton_is_pressed(b));
	Zroot_inject_pointer(root, Z_POINTER_UP, center(b).x, center(b).y);
	CHECK(g_clicks == 2);
	/* the touch overhang above and below the outline (9.5 px) is part of the button */
	tap(root, (ZPoint){ 3, 3 });
	CHECK(g_clicks == 3);
	/* disabled buttons do nothing */
	Zelement_set_enabled(b, false);
	tap(root, center(b));
	CHECK(g_clicks == 3);
	Zelement_set_enabled(b, true);
	/* a press-mode button clicks on press */
	Zbutton_set_click_mode(b, Z_CLICK_PRESS);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(g_clicks == 4);
	Zroot_inject_pointer(root, Z_POINTER_UP, center(b).x, center(b).y);
	CHECK(g_clicks == 4);
	Zroot_destroy(root);
}

static void test_keyboard_click_and_focus(void) {
	Zid root, stack, a, b;
	setup(&root, &stack);
	a = Zpanel_add(stack, Zbutton_create("a"));
	b = Zpanel_add(stack, Zbutton_create("b"));
	Zbutton_on_click(b, on_click, NULL);
	Zroot_update_layout(root);
	g_clicks = 0;
	/* Tab goes to the first control, then the next */
	key(root, Z_KEY_TAB, 0);
	CHECK(Zroot_get_focus(root) == a);
	key(root, Z_KEY_TAB, 0);
	CHECK(Zroot_get_focus(root) == b);
	key(root, Z_KEY_SPACE, 0);
	CHECK(g_clicks == 1);
	key(root, Z_KEY_TAB, Z_MOD_SHIFT);
	CHECK(Zroot_get_focus(root) == a);
	/* a tap focuses what it hits */
	tap(root, center(b));
	CHECK(Zroot_get_focus(root) == b);
	/* IsTabStop false skips a control */
	Zcontrol_set_tab_stop(a, false);
	key(root, Z_KEY_TAB, Z_MOD_SHIFT);
	CHECK(Zroot_get_focus(root) == b);
	Zroot_destroy(root);
}

static void test_repeat_button(void) {
	Zid root, stack, r;
	int i;
	setup(&root, &stack);
	r = Zpanel_add(stack, Zrepeat_button_create("+"));
	Zelement_set_vertical_alignment(r, Z_VALIGN_TOP);
	Zbutton_on_click(r, on_click, NULL);
	Zrepeat_button_set_delay(r, 500);
	Zrepeat_button_set_interval(r, 100);
	CHECK(Zrepeat_button_get_delay(r) == 500 && Zrepeat_button_get_interval(r) == 100);
	Zroot_update_layout(root);
	g_clicks = 0;
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(r).x, center(r).y);
	CHECK(g_clicks == 1); /* once at once */
	Zroot_tick(root, 400);
	CHECK(g_clicks == 1);
	Zroot_tick(root, 150); /* delay passed */
	CHECK(g_clicks == 2);
	for (i = 0; i < 5; i++) Zroot_tick(root, 100);
	CHECK(g_clicks == 7);
	Zroot_inject_pointer(root, Z_POINTER_UP, center(r).x, center(r).y);
	Zroot_tick(root, 1000);
	CHECK(g_clicks == 7); /* released: no more */
	Zroot_destroy(root);
}

static void test_toggle_taps(void) {
	Zid root, stack, c, r1, r2;
	setup(&root, &stack);
	c = Zpanel_add(stack, Zcheck_box_create("c"));
	r1 = Zpanel_add(stack, Zradio_button_create("1", NULL));
	r2 = Zpanel_add(stack, Zradio_button_create("2", NULL));
	Zroot_update_layout(root);
	g_changes = 0;
	Zelement_on(c, Z_EVENT_CHECKED, on_change, NULL);
	tap(root, center(c));
	CHECK(Ztoggle_button_get_is_checked(c) && g_changes == 1);
	tap(root, center(c));
	CHECK(!Ztoggle_button_get_is_checked(c));
	Ztoggle_button_set_is_three_state(c, true);
	tap(root, center(c));
	tap(root, center(c));
	CHECK(Ztoggle_button_get_state(c) == Z_INDETERMINATE);
	tap(root, center(r1));
	CHECK(Ztoggle_button_get_is_checked(r1));
	tap(root, center(r2));
	CHECK(!Ztoggle_button_get_is_checked(r1) && Ztoggle_button_get_is_checked(r2));
	/* a tap on a checked radio button keeps it checked */
	tap(root, center(r2));
	CHECK(Ztoggle_button_get_is_checked(r2));
	Zroot_destroy(root);
}

static void test_slider_drag(void) {
	Zid root, stack, s;
	ZRect r;
	setup(&root, &stack);
	s = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zelement_set_vertical_alignment(s, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(s);
	/* the thumb (9.5 px) is centered on the touch, so the middle of the control gives half of 0..100 */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, r.x + r.width * 0.5f, 28);
	CHECK_NEAR(Zrange_base_get_value(s), 50);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, r.x + r.width + 100, 500); /* dragging past the end, off the control: clamps */
	CHECK(Zrange_base_get_value(s) == 100);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, -50, 28);
	CHECK(Zrange_base_get_value(s) == 0);
	Zroot_inject_pointer(root, Z_POINTER_UP, -50, 28);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 200, 28); /* no capture after release */
	CHECK(Zrange_base_get_value(s) == 0);
	/* keys */
	Zelement_focus(s);
	key(root, Z_KEY_RIGHT, 0);
	CHECK_NEAR(Zrange_base_get_value(s), 1); /* SmallChange, snapped to StepFrequency 1 */
	key(root, Z_KEY_PAGE_UP, 0);
	CHECK_NEAR(Zrange_base_get_value(s), 11); /* LargeChange */
	key(root, Z_KEY_END, 0);
	CHECK(Zrange_base_get_value(s) == 100);
	Zroot_destroy(root);
}

static void test_text_box_typing(void) {
	Zid root, stack, tb;
	setup(&root, &stack);
	tb = Zpanel_add(stack, Ztext_box_create(""));
	Zelement_set_vertical_alignment(tb, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	g_changes = 0;
	Zelement_on(tb, Z_EVENT_TEXT_CHANGED, on_change, NULL);
	type(root, "ignored"); /* no focus yet */
	CHECK_STR(Ztext_box_get_text(tb), "");
	tap(root, center(tb));
	CHECK(Zroot_get_focus(root) == tb);
	type(root, "hello");
	CHECK_STR(Ztext_box_get_text(tb), "hello");
	CHECK(g_changes == 5 || g_changes == 1);
	key(root, Z_KEY_BACKSPACE, 0);
	CHECK_STR(Ztext_box_get_text(tb), "hell");
	key(root, Z_KEY_HOME, 0);
	type(root, "oh ");
	CHECK_STR(Ztext_box_get_text(tb), "oh hell");
	key(root, Z_KEY_DELETE, 0);
	CHECK_STR(Ztext_box_get_text(tb), "oh ell");
	/* selection with shift+arrows, replaced by typing */
	key(root, Z_KEY_END, 0);
	key(root, Z_KEY_LEFT, Z_MOD_SHIFT);
	key(root, Z_KEY_LEFT, Z_MOD_SHIFT);
	CHECK(Ztext_box_get_selection_length(tb) == 2);
	type(root, "X");
	CHECK_STR(Ztext_box_get_text(tb), "oh eX");
	/* select all, cut, paste */
	key(root, Z_KEY_A, Z_MOD_CTRL);
	key(root, Z_KEY_X, Z_MOD_CTRL);
	CHECK_STR(Ztext_box_get_text(tb), "");
	key(root, Z_KEY_V, Z_MOD_CTRL);
	key(root, Z_KEY_V, Z_MOD_CTRL);
	CHECK_STR(Ztext_box_get_text(tb), "oh eXoh eX");
	/* a newline is refused unless AcceptsReturn */
	type(root, "\n");
	CHECK_STR(Ztext_box_get_text(tb), "oh eXoh eX");
	Ztext_box_set_accepts_return(tb, true);
	type(root, "\nz");
	CHECK_STR(Ztext_box_get_text(tb), "oh eXoh eX\nz");
	/* MaxLength counts characters, not bytes */
	Ztext_box_set_accepts_return(tb, false);
	Ztext_box_set_text(tb, "");
	Ztext_box_set_max_length(tb, 3);
	type(root, "\xD0\xB0\xD0\xB1\xD0\xB2\xD0\xB3"); /* four Cyrillic letters */
	CHECK_STR(Ztext_box_get_text(tb), "\xD0\xB0\xD0\xB1\xD0\xB2");
	/* backspace removes a whole character */
	key(root, Z_KEY_BACKSPACE, 0);
	CHECK_STR(Ztext_box_get_text(tb), "\xD0\xB0\xD0\xB1");
	/* read only */
	Ztext_box_set_is_read_only(tb, true);
	type(root, "q");
	CHECK_STR(Ztext_box_get_text(tb), "\xD0\xB0\xD0\xB1");
	Zroot_destroy(root);
}

static void test_text_box_mouse_selection(void) {
	Zid root, stack, tb;
	ZRect r;
	setup(&root, &stack);
	tb = Zpanel_add(stack, Ztext_box_create("0123456789"));
	Zelement_set_vertical_alignment(tb, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(tb);
	/* press at the start of the text (2.5 px of border, 7 of padding), drag past its end; the box is 34 px high */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, r.x + 10, r.y + 17);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, r.x + 400, r.y + 17);
	Zroot_inject_pointer(root, Z_POINTER_UP, r.x + 400, r.y + 17);
	CHECK(Ztext_box_get_selection_length(tb) >= 9);
	Zroot_destroy(root);
}

static void test_password_box(void) {
	Zid root, stack, pw;
	setup(&root, &stack);
	pw = Zpanel_add(stack, Zpassword_box_create());
	Zroot_update_layout(root);
	Zelement_focus(pw);
	type(root, "secret");
	CHECK_STR(Ztext_box_get_text(pw), "secret");
	/* copy is refused for a password */
	key(root, Z_KEY_A, Z_MOD_CTRL);
	key(root, Z_KEY_C, Z_MOD_CTRL);
	Zroot_destroy(root);
}

static void test_list_box_tap_and_scroll(void) {
	Zid root, stack, lb;
	int i;
	char name[32];
	setup(&root, &stack);
	lb = Zpanel_add(stack, Zlist_box_create());
	Zelement_set_height(lb, 400); /* a list in a stack panel is as tall as its items: it needs a height to scroll */
	for (i = 0; i < 40; i++) {
		snprintf(name, sizeof name, "item %d", i);
		Zitems_control_add_text(lb, name);
	}
	Zroot_update_layout(root);
	g_changes = 0;
	Zelement_on(lb, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	tap(root, center(Zitems_control_get_container(lb, 2)));
	CHECK(Zselector_get_selected_index(lb) == 2 && g_changes == 1);
	/* a vertical drag scrolls, and does not select what the finger started on */
	{
		Zid item = Zitems_control_get_container(lb, 5);
		ZPoint p = center(item);
		Zid sv = Zelement_get_child(lb, 0);
		CHECK(Zelement_is_a(sv, Z_CLASS_SCROLL_VIEWER));
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 20);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y - 100);
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y - 100);
		CHECK_NEAR(Zscroll_viewer_get_vertical_offset(sv), 100);
		CHECK(Zselector_get_selected_index(lb) == 2 && g_changes == 1);
		/* inertia carries on after the finger left */
		{
			double before = Zscroll_viewer_get_vertical_offset(sv);
			Zroot_tick(root, 16);
			Zroot_tick(root, 16);
			CHECK(Zscroll_viewer_get_vertical_offset(sv) >= before);
		}
		/* the wheel */
		Zscroll_viewer_scroll_to_vertical_offset(sv, 0);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, 200, 200);
		Zroot_inject_wheel(root, -1);
		CHECK(Zscroll_viewer_get_vertical_offset(sv) > 0);
	}
	Zlist_box_scroll_into_view(lb, 39);
	Zroot_update_layout(root);
	{
		Zid sv = Zelement_get_child(lb, 0);
		/* the last item is at the bottom of the view */
		CHECK(Zscroll_viewer_get_vertical_offset(sv) > 500);
		CHECK_NEAR(Zscroll_viewer_get_vertical_offset(sv), Zscroll_viewer_get_extent_height(sv) - Zscroll_viewer_get_viewport_height(sv));
	}
	Zroot_destroy(root);
}

static void test_button_in_scroll_viewer(void) {
	Zid root, stack, sv, inner, b;
	int i;
	setup(&root, &stack);
	sv = Zpanel_add(stack, Zscroll_viewer_create());
	Zelement_set_height(sv, 400);
	inner = Zstack_panel_create(Z_VERTICAL);
	for (i = 0; i < 20; i++) Zpanel_add(inner, Zbutton_create("b"));
	Zscroll_viewer_set_content(sv, inner);
	b = Zelement_get_child(inner, 0);
	Zbutton_on_click(b, on_click, NULL);
	Zroot_update_layout(root);
	g_clicks = 0;
	tap(root, center(b));
	CHECK(g_clicks == 1);
	/* pressing the button, then dragging: the button is released without a click and the page scrolls */
	{
		ZPoint p = center(b);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, p.x, p.y);
		CHECK(Zbutton_is_pressed(b));
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y + 30);
		Zroot_inject_pointer(root, Z_POINTER_MOVE, p.x, p.y + 5);
		Zroot_inject_pointer(root, Z_POINTER_UP, p.x, p.y + 5);
		CHECK(!Zbutton_is_pressed(b));
		CHECK(g_clicks == 1);
	}
	Zroot_destroy(root);
}

static void test_combo_box(void) {
	Zid root, stack, cb;
	setup(&root, &stack);
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(cb, Z_VALIGN_TOP);
	Zitems_control_add_text(cb, "red");
	Zitems_control_add_text(cb, "green");
	Zitems_control_add_text(cb, "blue");
	Zroot_update_layout(root);
	g_changes = 0;
	Zelement_on(cb, Z_EVENT_SELECTION_CHANGED, on_change, NULL);
	CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	tap(root, center(cb));
	CHECK(Zcombo_box_get_is_drop_down_open(cb));
	Zroot_update_layout(root);
	/* the drop-down is above everything: a tap on its second row selects and closes */
	{
		Zid item = Zitems_control_get_container(cb, 1);
		ZPoint p = center(item);
		tap(root, p);
		CHECK(Zselector_get_selected_index(cb) == 1 && g_changes == 1);
		CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	}
	/* a tap outside closes without selecting */
	tap(root, center(cb));
	CHECK(Zcombo_box_get_is_drop_down_open(cb));
	tap(root, (ZPoint){ 400, 700 });
	CHECK(!Zcombo_box_get_is_drop_down_open(cb) && Zselector_get_selected_index(cb) == 1);
	/* a tap on the box while open closes it and does not reopen */
	tap(root, center(cb));
	CHECK(Zcombo_box_get_is_drop_down_open(cb));
	tap(root, center(cb));
	CHECK(!Zcombo_box_get_is_drop_down_open(cb));
	/* the drop-down was drawn: the frame differs while open */
	Zcombo_box_set_is_drop_down_open(cb, true);
	CHECK(Zroot_render(root, NULL));
	Zroot_destroy(root);
}

static void test_tooltip_and_hover(void) {
	Zid root, stack, b;
	ZFrame before, after;
	setup(&root, &stack);
	b = Zpanel_add(stack, Zbutton_create("hover me"));
	Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
	Zelement_set_tooltip(b, "a tooltip");
	Zroot_update_layout(root);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, center(b).x, center(b).y);
	CHECK(Zbutton_is_pointer_over(b));
	Zroot_render(root, &before);
	CHECK(Zroot_get_pointer_over(root) != Z_INVALID_ID);
	Zroot_tick(root, 600);
	CHECK(Zroot_render(root, &after)); /* the tooltip appeared */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 450, 700);
	CHECK(!Zbutton_is_pointer_over(b));
	(void)before;
	Zroot_destroy(root);
}

static void test_render_is_lazy(void) {
	Zid root, stack, b;
	setup(&root, &stack);
	b = Zpanel_add(stack, Zbutton_create("x"));
	CHECK(Zroot_render(root, NULL));
	CHECK(!Zroot_render(root, NULL)); /* nothing changed */
	Zcontent_control_set_text(b, "y");
	CHECK(Zroot_render(root, NULL));
	Zroot_destroy(root);
}

static void test_destroying_while_focused_or_captured(void) {
	Zid root, stack, b;
	setup(&root, &stack);
	b = Zpanel_add(stack, Zbutton_create("x"));
	Zroot_update_layout(root);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, center(b).x, center(b).y);
	CHECK(Zroot_get_focus(root) == b);
	Zelement_destroy(b);
	CHECK(Zroot_get_focus(root) == Z_INVALID_ID);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 10, 10); /* the capture is gone with it */
	Zroot_inject_pointer(root, Z_POINTER_UP, 10, 10);
	Zroot_destroy(root);
}

/* ---- handlers that replace the whole page, as a tap that opens another page does ---- */

static Zid g_nav_root;
static int g_nav_count;

static void navigate(Zid sender, void* user) {
	(void)sender; (void)user;
	g_nav_count++;
	Zroot_set_visual(g_nav_root, Zstack_panel_create(Z_VERTICAL)); /* destroys the sender and everything else */
}

static void navigate_reason(Zid sender, ZAutoSuggestionBoxTextChangeReason reason, void* user) { (void)reason; navigate(sender, user); }
static void navigate_value(Zid sender, double a, double b, void* user) { (void)a; (void)b; navigate(sender, user); }

/* After the handler ran, the root must still work: lay out, render, take more input. */
static void survives(Zid root) {
	Zroot_render(root, NULL);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 100, 100);
	Zroot_inject_pointer(root, Z_POINTER_UP, 100, 100);
	Zroot_inject_key(root, Z_KEY_SPACE, 0, 1);
	Zroot_inject_key(root, Z_KEY_SPACE, 0, 0);
	Zroot_inject_text(root, "x");
	Zroot_tick(root, 1000);
	Zroot_render(root, NULL);
}

static void test_handlers_that_navigate(void) {
	Zid root, stack, c, other;
	setup(&root, &stack);
	g_nav_root = root;

	/* Click */
	c = Zpanel_add(stack, Zbutton_create("go"));
	Zbutton_on_click(c, navigate, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(c));
	CHECK(g_nav_count == 1 && !Zelement_is_valid(c));
	survives(root);

	/* Checked: the toggle raises Click after Checked, on an element that is gone */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zcheck_box_create("c"));
	Zelement_on(c, Z_EVENT_CHECKED, navigate, NULL);
	Zbutton_on_click(c, on_click, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(c));
	CHECK(g_nav_count == 1);
	survives(root);

	/* Unchecked of another radio button: its handler runs while the checked one is being set */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zradio_button_create("1", NULL));
	other = Zpanel_add(stack, Zradio_button_create("2", NULL));
	Ztoggle_button_set_is_checked(c, true);
	Zelement_on(c, Z_EVENT_UNCHECKED, navigate, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(other));
	CHECK(g_nav_count == 1);
	survives(root);

	/* SelectionChanged of a ListBox */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zlist_box_create());
	Zitems_control_add_text(c, "a"); Zitems_control_add_text(c, "b");
	Zelement_on(c, Z_EVENT_SELECTION_CHANGED, navigate, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(Zitems_control_get_container(c, 1)));
	CHECK(g_nav_count == 1);
	survives(root);

	/* SelectionChanged of a ComboBox, from an item of its drop-down */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zitems_control_add_text(c, "a"); Zitems_control_add_text(c, "b");
	Zelement_on(c, Z_EVENT_SELECTION_CHANGED, navigate, NULL);
	Zroot_update_layout(root);
	tap(root, center(c));
	Zroot_update_layout(root);
	g_nav_count = 0;
	tap(root, center(Zitems_control_get_container(c, 1)));
	CHECK(g_nav_count == 1);
	survives(root);

	/* DropDownClosed of a ComboBox, closed by a tap outside */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zcombo_box_create());
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zitems_control_add_text(c, "a");
	Zelement_on(c, Z_EVENT_DROP_DOWN_CLOSED, navigate, NULL);
	Zroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	tap(root, (ZPoint){ 400, 700 });
	CHECK(g_nav_count == 1);
	survives(root);

	/* TextChanged of an AutoSuggestBox while typing: the handler replaces the page */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zauto_suggest_box_create());
	Zauto_suggest_box_on_text_changed(c, navigate_reason, NULL);
	Zroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	type(root, "abc");
	CHECK(g_nav_count == 1);
	survives(root);
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Ztext_box_create(""));
	Zelement_on(c, Z_EVENT_TEXT_CHANGED, navigate, NULL);
	Zroot_update_layout(root);
	tap(root, center(c));
	g_nav_count = 0;
	key(root, Z_KEY_V, Z_MOD_CTRL); /* paste: the clipboard of an earlier test holds text */
	type(root, "q");
	CHECK(g_nav_count >= 1);
	survives(root);

	/* ValueChanged of a Slider, mid-drag */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zrange_base_on_value_changed(c, navigate_value, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 240, 28);
	CHECK(g_nav_count == 1);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 300, 28);
	Zroot_inject_pointer(root, Z_POINTER_UP, 300, 28);
	survives(root);

	/* Click-on-hover destroying the button while the pointer enters it */
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	c = Zpanel_add(stack, Zbutton_create("hover"));
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zbutton_set_click_mode(c, Z_CLICK_HOVER);
	Zbutton_on_click(c, navigate, NULL);
	Zroot_update_layout(root);
	g_nav_count = 0;
	Zroot_inject_pointer(root, Z_POINTER_MOVE, center(c).x, center(c).y);
	CHECK(g_nav_count == 1);
	survives(root);

	Zroot_destroy(root);
}

static void test_toggle_switch_input(void) {
	Zid root, stack, sw;
	ZPoint sw_c;
	setup(&root, &stack);
	sw = Zpanel_add(stack, Ztoggle_switch_create());
	Zelement_set_vertical_alignment(sw, Z_VALIGN_TOP);
	Ztoggle_switch_on_toggled(sw, on_change, NULL);
	Zroot_update_layout(root);
	g_changes = 0;
	/* the switch is the 77 x 36 box at the right of the control: only a tap on it flips it */
	{ ZPoint c = { 480 - 38.5f, 18 }; sw_c = c; }
	tap(root, (ZPoint){ 100, 18 });          /* empty space of the row */
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 0);
	tap(root, (ZPoint){ 480 - 80, 18 });     /* just left of the switch */
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 0);
	tap(root, sw_c);
	CHECK(Ztoggle_switch_get_is_on(sw) && g_changes == 1);
	tap(root, sw_c);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 2);
	/* Space, once it has the focus (the tap on the switch gave it) */
	CHECK(Zroot_get_focus(root) == sw);
	key(root, Z_KEY_SPACE, 0);
	CHECK(Ztoggle_switch_get_is_on(sw) && g_changes == 3);
	key(root, Z_KEY_SPACE, 0);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 4);
	/* a press that moves away and lets go outside is not a tap */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 100, 10);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 100, 600);
	Zroot_inject_pointer(root, Z_POINTER_UP, 100, 600);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 4);
	/* a drag that starts on the knob (the switch is the 77 px at the right, the knob the 20.5 at its left end when off) carries it */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 413, 18);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 450, 18);
	CHECK(!Ztoggle_switch_get_is_on(sw)); /* not yet: it goes by where the knob is let go */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 478, 18);
	Zroot_inject_pointer(root, Z_POINTER_UP, 478, 18);
	CHECK(Ztoggle_switch_get_is_on(sw) && g_changes == 5);
	/* a short drag back is let go to the end it is nearer to: still on */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 470, 18);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 460, 18);
	Zroot_inject_pointer(root, Z_POINTER_UP, 460, 18);
	CHECK(Ztoggle_switch_get_is_on(sw) && g_changes == 5);
	/* and a long one turns it off */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 470, 18);
	Zroot_inject_pointer(root, Z_POINTER_MOVE, 405, 18);
	Zroot_inject_pointer(root, Z_POINTER_UP, 405, 18);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 6);
	/* disabled: nothing */
	Zelement_set_enabled(sw, false);
	tap(root, sw_c);
	CHECK(!Ztoggle_switch_get_is_on(sw) && g_changes == 6);
	Zroot_destroy(root);
}

static void test_slider_steps_and_ticks(void) {
	Zid root, stack, s;
	ZRect r;
	setup(&root, &stack);
	s = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zelement_set_vertical_alignment(s, Z_VALIGN_TOP);
	Zslider_set_header(s, "Level");
	Zrange_base_set_range(s, 0, 10);
	Zrange_base_set_changes(s, 1, 2);
	Zslider_set_step_frequency(s, 2.5);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(s);
	/* the band is under the header (the 16 px text), the track in it */
	CHECK(r.height > 57 + 15);
	/* a touch at 30 % of the thumb travel is 3, which the step of 2.5 snaps to 2.5 */
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 4.75f + 0.3f * (480 - 9.5f), r.y + 50);
	CHECK_NEAR(Zrange_base_get_value(s), 2.5);
	Zroot_inject_pointer(root, Z_POINTER_UP, 0, 0);
	/* with the ticks every 4 and SnapsTo Ticks it lands on 0, 4 or 8 */
	Zslider_set_tick_frequency(s, 4);
	Zslider_set_snaps_to(s, Z_SNAPS_TO_TICKS);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 4.75f + 0.55f * (480 - 9.5f), r.y + 50);
	CHECK_NEAR(Zrange_base_get_value(s), 4);
	Zroot_inject_pointer(root, Z_POINTER_UP, 0, 0);
	/* the value the program sets is not snapped */
	Zrange_base_set_value(s, 3.3);
	CHECK(Zrange_base_get_value(s) == 3.3);
	/* a disabled slider takes no touch */
	Zelement_set_enabled(s, false);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, 400, r.y + 50);
	CHECK(Zrange_base_get_value(s) == 3.3);
	Zroot_destroy(root);
}

static void test_progress_ring_and_bar_run_on_tick(void) {
	Zid root, stack, ring, pb;
	setup(&root, &stack);
	ring = Zpanel_add(stack, Zprogress_ring_create());
	pb = Zpanel_add(stack, Zprogress_bar_create());
	Zprogress_bar_set_is_indeterminate(pb, true);
	Zroot_update_layout(root);
	/* neither takes the pointer or the focus */
	tap(root, center(ring));
	CHECK(Zroot_get_focus(root) != ring);
	Zroot_tick(root, 16);
	Zprogress_ring_set_is_active(ring, true);
	Zroot_tick(root, 1000);
	Zroot_tick(root, 5000);
	CHECK(Zprogress_ring_get_is_active(ring));
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
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
	Zshutdown();
	return test_finish("test_input");
}
