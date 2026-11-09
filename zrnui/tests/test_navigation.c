/**
 * Frame and Page: Navigate, GoBack, the order of the callbacks, the Back button,
 * and a tap on a button that navigates away from the page the button is on.
 */

#include "test.h"

static char g_log[256];
static void log_add(const char* s) { strcat(g_log, s); }

static Zid g_frame, g_root;

static void to_a(Zid page, void* u) { (void)u; (void)page; log_add("toA "); }
static void from_a(Zid page, void* u) { (void)page; (void)u; log_add("fromA "); }
static void to_b(Zid page, void* u) { (void)u; log_add("toB"); log_add(Zpage_get_parameter(page) ? (const char*)Zpage_get_parameter(page) : "?"); log_add(" "); }
static void on_navigated(Zid frame, void* u) { (void)frame; (void)u; log_add("nav "); }

static void go_b_click(Zid sender, void* u);

static Zid page_a(Zid frame, void* param) {
	Zid page = Zpage_create(), stack = Zstack_panel_create(Z_VERTICAL), b;
	(void)frame; (void)param;
	b = Zpanel_add(stack, Zbutton_create("to B"));
	Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
	Zelement_set_name(b, "go");
	Zbutton_on_click(b, go_b_click, NULL);
	Zpage_set_content(page, stack);
	Zpage_on_navigated_to(page, to_a, NULL);
	Zpage_on_navigated_from(page, from_a, NULL);
	return page;
}

static Zid page_b(Zid frame, void* param) {
	Zid page = Zpage_create();
	(void)frame; (void)param;
	Zpage_set_content(page, Ztext_block_create("B"));
	Zpage_on_navigated_to(page, to_b, NULL);
	return page;
}

static Zid page_c(Zid frame, void* param) { (void)frame; (void)param; return Zpage_create(); }
static Zid refuse(Zid frame, void* param) { (void)frame; (void)param; return Z_INVALID_ID; }
static Zid not_a_page(Zid frame, void* param) { (void)frame; (void)param; return Zstack_panel_create(Z_VERTICAL); }

static ZPoint center(Zid e) {
	ZRect r = Zelement_get_bounds(e);
	return Zelement_to_root(e, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void go_b_click(Zid sender, void* u) {
	(void)sender; (void)u;
	CHECK(Zframe_navigate(g_frame, page_b, "!"));
	log_add("(returned) ");
}

static void setup(void) {
	g_root = Zroot_create(480, 800);
	g_frame = Zframe_create();
	Zroot_set_visual(g_root, g_frame);
	g_log[0] = 0;
}

static void test_navigate_and_back(void) {
	Zid a, b, c;
	setup();
	CHECK(Zframe_get_page(g_frame) == Z_INVALID_ID);
	CHECK(!Zframe_can_go_back(g_frame));
	CHECK(Zframe_navigate(g_frame, page_a, NULL));
	a = Zframe_get_page(g_frame);
	CHECK(a != Z_INVALID_ID && Zelement_is_a(a, Z_CLASS_PAGE));
	CHECK(Zpage_get_frame(a) == g_frame);
	CHECK(Zframe_get_back_stack_depth(g_frame) == 0);
	CHECK(Zframe_navigate(g_frame, page_b, "p1"));
	b = Zframe_get_page(g_frame);
	CHECK(b != a);
	CHECK(Zframe_can_go_back(g_frame) && Zframe_get_back_stack_depth(g_frame) == 1);
	CHECK(Zframe_navigate(g_frame, page_c, NULL));
	c = Zframe_get_page(g_frame);
	CHECK(Zframe_get_back_stack_depth(g_frame) == 2);
	/* the old page is destroyed when the frame leaves it: its Zid no longer names a Page */
	CHECK(!Zelement_is_a(b, Z_CLASS_PAGE));
	(void)c;
	CHECK(Zframe_go_back(g_frame));
	/* B is built again from its entry, with its own parameter */
	CHECK(Zframe_get_page(g_frame) != c);
	CHECK(strstr(g_log, "toBp1 toBp1") != NULL);
	CHECK(Zframe_can_go_forward(g_frame));
	CHECK(Zframe_go_back(g_frame));
	CHECK(!Zframe_can_go_back(g_frame));
	CHECK(!Zframe_go_back(g_frame));
	CHECK(Zframe_go_forward(g_frame));
	/* a new Navigate drops the pages ahead */
	CHECK(Zframe_navigate(g_frame, page_c, NULL));
	CHECK(!Zframe_can_go_forward(g_frame));
	Zroot_destroy(g_root);
}

static void test_safe_area(void) {
	Zid page;
	ZRect r;
	setup();
	Zframe_navigate(g_frame, page_a, NULL);
	Zroot_update_layout(g_root);
	page = Zframe_get_page(g_frame);
	r = Zelement_get_bounds(page);
	/* the page starts under the status bar of 32 (ZST_SAFE_AREA_TOP), the frame itself fills the screen */
	CHECK_NEAR(r.y, 32);
	CHECK_NEAR(r.height, 768);
	r = Zelement_get_bounds(g_frame);
	CHECK_NEAR(r.y, 0);
	CHECK_NEAR(r.height, 800);
	Zroot_destroy(g_root);
}

static void test_refusal(void) {
	Zid shown;
	setup();
	CHECK(Zframe_navigate(g_frame, page_a, NULL));
	shown = Zframe_get_page(g_frame);
	CHECK(!Zframe_navigate(g_frame, refuse, NULL));
	CHECK(!Zframe_navigate(g_frame, not_a_page, NULL));
	CHECK(!Zframe_navigate(g_frame, NULL, NULL));
	CHECK(Zframe_get_page(g_frame) == shown);
	CHECK(Zframe_get_back_stack_depth(g_frame) == 0);
	Zroot_destroy(g_root);
}

static void test_callback_order(void) {
	setup();
	Zelement_on(g_frame, Z_EVENT_NAVIGATED, on_navigated, NULL);
	Zframe_navigate(g_frame, page_a, NULL);
	CHECK(strcmp(g_log, "toA nav ") == 0);
	g_log[0] = 0;
	Zframe_navigate(g_frame, page_b, "x");
	CHECK(strcmp(g_log, "fromA toBx nav ") == 0);
	Zroot_destroy(g_root);
}

static void test_back_button(void) {
	setup();
	CHECK(!Zroot_back(g_root));
	Zframe_navigate(g_frame, page_a, NULL);
	CHECK(!Zroot_back(g_root));
	Zframe_navigate(g_frame, page_c, NULL);
	CHECK(Zroot_back(g_root));
	CHECK(!Zroot_back(g_root));
	/* Escape is Back, and it is handled only when there was a page to go back to */
	Zframe_navigate(g_frame, page_c, NULL);
	CHECK(Zroot_inject_key(g_root, Z_KEY_ESCAPE, 0, 1));
	CHECK(!Zroot_inject_key(g_root, Z_KEY_ESCAPE, 0, 1));
	Zroot_destroy(g_root);
	{
		/* with no Frame there is nothing to go back to */
		Zid root = Zroot_create(480, 800);
		Zroot_set_visual(root, Zstack_panel_create(Z_VERTICAL));
		CHECK(!Zroot_back(root));
		Zroot_destroy(root);
	}
}

/* the tap runs a Click handler of a button on page A that replaces page A */
static void test_tap_navigates_away(void) {
	Zid button;
	ZPoint p;
	setup();
	Zframe_navigate(g_frame, page_a, NULL);
	Zroot_update_layout(g_root);
	button = Zelement_find_name(Zframe_get_page(g_frame), "go");
	CHECK(button != Z_INVALID_ID);
	p = center(button);
	Zroot_inject_pointer(g_root, Z_POINTER_MOVE, p.x, p.y);
	Zroot_inject_pointer(g_root, Z_POINTER_DOWN, p.x, p.y);
	Zroot_inject_pointer(g_root, Z_POINTER_UP, p.x, p.y);
	CHECK(strstr(g_log, "fromA toB!") != NULL);
	CHECK(strstr(g_log, "(returned)") != NULL);
	CHECK(Zframe_get_back_stack_depth(g_frame) == 1);
	/* the new page draws and takes input */
	Zroot_update_layout(g_root);
	Zroot_inject_pointer(g_root, Z_POINTER_MOVE, 10, 10);
	Zroot_inject_pointer(g_root, Z_POINTER_DOWN, 10, 10);
	Zroot_inject_pointer(g_root, Z_POINTER_UP, 10, 10);
	{ ZFrame f; CHECK(Zroot_render(g_root, &f)); }
	CHECK(Zroot_back(g_root));
	Zroot_update_layout(g_root);
	CHECK(Zelement_find_name(Zframe_get_page(g_frame), "go") != Z_INVALID_ID);
	Zroot_destroy(g_root);
}

int main(void) {
	RUN(test_navigate_and_back);
	RUN(test_safe_area);
	RUN(test_refusal);
	RUN(test_callback_order);
	RUN(test_back_button);
	RUN(test_tap_navigates_away);
	return test_finish("test_navigation");
}
