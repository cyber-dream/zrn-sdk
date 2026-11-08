/**
 * Frame and Page: Navigate, GoBack, the order of the callbacks, the Back button,
 * and a tap on a button that navigates away from the page the button is on.
 */

#include "test.h"

static char g_log[256];
static void log_add(const char* s) { strcat(g_log, s); }

static Kid g_frame, g_root;

static void to_a(Kid page, void* u) { (void)u; (void)page; log_add("toA "); }
static void from_a(Kid page, void* u) { (void)page; (void)u; log_add("fromA "); }
static void to_b(Kid page, void* u) { (void)u; log_add("toB"); log_add(Kpage_get_parameter(page) ? (const char*)Kpage_get_parameter(page) : "?"); log_add(" "); }
static void on_navigated(Kid frame, void* u) { (void)frame; (void)u; log_add("nav "); }

static void go_b_click(Kid sender, void* u);

static Kid page_a(Kid frame, void* param) {
	Kid page = Kpage_create(), stack = Kstack_panel_create(K_VERTICAL), b;
	(void)frame; (void)param;
	b = Kpanel_add(stack, Kbutton_create("to B"));
	Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
	Kelement_set_name(b, "go");
	Kbutton_on_click(b, go_b_click, NULL);
	Kpage_set_content(page, stack);
	Kpage_on_navigated_to(page, to_a, NULL);
	Kpage_on_navigated_from(page, from_a, NULL);
	return page;
}

static Kid page_b(Kid frame, void* param) {
	Kid page = Kpage_create();
	(void)frame; (void)param;
	Kpage_set_content(page, Ktext_block_create("B"));
	Kpage_on_navigated_to(page, to_b, NULL);
	return page;
}

static Kid page_c(Kid frame, void* param) { (void)frame; (void)param; return Kpage_create(); }
static Kid refuse(Kid frame, void* param) { (void)frame; (void)param; return K_INVALID_ID; }
static Kid not_a_page(Kid frame, void* param) { (void)frame; (void)param; return Kstack_panel_create(K_VERTICAL); }

static KPoint center(Kid e) {
	KRect r = Kelement_get_bounds(e);
	return Kelement_to_root(e, (KPoint){ r.width * 0.5f, r.height * 0.5f });
}

static void go_b_click(Kid sender, void* u) {
	(void)sender; (void)u;
	CHECK(Kframe_navigate(g_frame, page_b, "!"));
	log_add("(returned) ");
}

static void setup(void) {
	g_root = Kroot_create(480, 800);
	g_frame = Kframe_create();
	Kroot_set_visual(g_root, g_frame);
	g_log[0] = 0;
}

static void test_navigate_and_back(void) {
	Kid a, b, c;
	setup();
	CHECK(Kframe_get_page(g_frame) == K_INVALID_ID);
	CHECK(!Kframe_can_go_back(g_frame));
	CHECK(Kframe_navigate(g_frame, page_a, NULL));
	a = Kframe_get_page(g_frame);
	CHECK(a != K_INVALID_ID && Kelement_is_a(a, K_CLASS_PAGE));
	CHECK(Kpage_get_frame(a) == g_frame);
	CHECK(Kframe_get_back_stack_depth(g_frame) == 0);
	CHECK(Kframe_navigate(g_frame, page_b, "p1"));
	b = Kframe_get_page(g_frame);
	CHECK(b != a);
	CHECK(Kframe_can_go_back(g_frame) && Kframe_get_back_stack_depth(g_frame) == 1);
	CHECK(Kframe_navigate(g_frame, page_c, NULL));
	c = Kframe_get_page(g_frame);
	CHECK(Kframe_get_back_stack_depth(g_frame) == 2);
	/* the old page is destroyed when the frame leaves it: its Kid no longer names a Page */
	CHECK(!Kelement_is_a(b, K_CLASS_PAGE));
	(void)c;
	CHECK(Kframe_go_back(g_frame));
	/* B is built again from its entry, with its own parameter */
	CHECK(Kframe_get_page(g_frame) != c);
	CHECK(strstr(g_log, "toBp1 toBp1") != NULL);
	CHECK(Kframe_can_go_forward(g_frame));
	CHECK(Kframe_go_back(g_frame));
	CHECK(!Kframe_can_go_back(g_frame));
	CHECK(!Kframe_go_back(g_frame));
	CHECK(Kframe_go_forward(g_frame));
	/* a new Navigate drops the pages ahead */
	CHECK(Kframe_navigate(g_frame, page_c, NULL));
	CHECK(!Kframe_can_go_forward(g_frame));
	Kroot_destroy(g_root);
}

static void test_safe_area(void) {
	Kid page;
	KRect r;
	setup();
	Kframe_navigate(g_frame, page_a, NULL);
	Kroot_update_layout(g_root);
	page = Kframe_get_page(g_frame);
	r = Kelement_get_bounds(page);
	/* the page starts under the status bar of 32 (KST_SAFE_AREA_TOP), the frame itself fills the screen */
	CHECK_NEAR(r.y, 32);
	CHECK_NEAR(r.height, 768);
	r = Kelement_get_bounds(g_frame);
	CHECK_NEAR(r.y, 0);
	CHECK_NEAR(r.height, 800);
	Kroot_destroy(g_root);
}

static void test_refusal(void) {
	Kid shown;
	setup();
	CHECK(Kframe_navigate(g_frame, page_a, NULL));
	shown = Kframe_get_page(g_frame);
	CHECK(!Kframe_navigate(g_frame, refuse, NULL));
	CHECK(!Kframe_navigate(g_frame, not_a_page, NULL));
	CHECK(!Kframe_navigate(g_frame, NULL, NULL));
	CHECK(Kframe_get_page(g_frame) == shown);
	CHECK(Kframe_get_back_stack_depth(g_frame) == 0);
	Kroot_destroy(g_root);
}

static void test_callback_order(void) {
	setup();
	Kelement_on(g_frame, K_EVENT_NAVIGATED, on_navigated, NULL);
	Kframe_navigate(g_frame, page_a, NULL);
	CHECK(strcmp(g_log, "toA nav ") == 0);
	g_log[0] = 0;
	Kframe_navigate(g_frame, page_b, "x");
	CHECK(strcmp(g_log, "fromA toBx nav ") == 0);
	Kroot_destroy(g_root);
}

static void test_back_button(void) {
	setup();
	CHECK(!Kroot_back(g_root));
	Kframe_navigate(g_frame, page_a, NULL);
	CHECK(!Kroot_back(g_root));
	Kframe_navigate(g_frame, page_c, NULL);
	CHECK(Kroot_back(g_root));
	CHECK(!Kroot_back(g_root));
	/* Escape is Back, and it is handled only when there was a page to go back to */
	Kframe_navigate(g_frame, page_c, NULL);
	CHECK(Kroot_inject_key(g_root, K_KEY_ESCAPE, 0, 1));
	CHECK(!Kroot_inject_key(g_root, K_KEY_ESCAPE, 0, 1));
	Kroot_destroy(g_root);
	{
		/* with no Frame there is nothing to go back to */
		Kid root = Kroot_create(480, 800);
		Kroot_set_visual(root, Kstack_panel_create(K_VERTICAL));
		CHECK(!Kroot_back(root));
		Kroot_destroy(root);
	}
}

/* the tap runs a Click handler of a button on page A that replaces page A */
static void test_tap_navigates_away(void) {
	Kid button;
	KPoint p;
	setup();
	Kframe_navigate(g_frame, page_a, NULL);
	Kroot_update_layout(g_root);
	button = Kelement_find_name(Kframe_get_page(g_frame), "go");
	CHECK(button != K_INVALID_ID);
	p = center(button);
	Kroot_inject_pointer(g_root, K_POINTER_MOVE, p.x, p.y);
	Kroot_inject_pointer(g_root, K_POINTER_DOWN, p.x, p.y);
	Kroot_inject_pointer(g_root, K_POINTER_UP, p.x, p.y);
	CHECK(strstr(g_log, "fromA toB!") != NULL);
	CHECK(strstr(g_log, "(returned)") != NULL);
	CHECK(Kframe_get_back_stack_depth(g_frame) == 1);
	/* the new page draws and takes input */
	Kroot_update_layout(g_root);
	Kroot_inject_pointer(g_root, K_POINTER_MOVE, 10, 10);
	Kroot_inject_pointer(g_root, K_POINTER_DOWN, 10, 10);
	Kroot_inject_pointer(g_root, K_POINTER_UP, 10, 10);
	{ KFrame f; CHECK(Kroot_render(g_root, &f)); }
	CHECK(Kroot_back(g_root));
	Kroot_update_layout(g_root);
	CHECK(Kelement_find_name(Kframe_get_page(g_frame), "go") != K_INVALID_ID);
	Kroot_destroy(g_root);
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
