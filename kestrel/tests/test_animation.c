/**
 * Animation: easing, Kelement_animate, render transforms on the GPU, the turnstile of a Frame
 * and the lean of a pressed element. The clock is Kroot_tick, so time is moved by hand.
 */

#include "test.h"

static void run(Kid root, float ms) {
	float t;
	for (t = 0; t < ms; t += 16) Kroot_tick(root, 16);
}

static Kid make_root(KAnimations mode) {
	Kid root = Kroot_create(480, 800);
	Kroot_set_renderer(root, K_RENDERER_SOFTWARE);
	Kroot_set_animations(root, mode);
	return root;
}

static void test_easing(void) {
	KEasing e;
	for (e = K_EASE_LINEAR; e <= K_EASE_EMPHASIZED; e++) {
		float prev = 0;
		int i;
		CHECK_NEAR(Keasing(e, 0), 0);
		CHECK_NEAR(Keasing(e, 1), 1);
		for (i = 1; i <= 20; i++) {
			float v = Keasing(e, (float)i / 20);
			CHECK(v >= prev - 1e-4f);
			prev = v;
		}
	}
	CHECK(Keasing(K_EASE_OUT, 0.5f) > 0.5f);
	CHECK(Keasing(K_EASE_IN, 0.5f) < 0.5f);
	CHECK(Keasing(K_EASE_EMPHASIZED, 0.3f) > 0.5f);
}

static void test_animate(void) {
	Kid root = make_root(K_ANIMATIONS_OFF), stack = Kstack_panel_create(K_VERTICAL), b;
	Kroot_set_visual(root, stack);
	b = Kpanel_add(stack, Kborder_create());
	/* off: at once */
	Kelement_animate(b, K_PROP_OPACITY, 0.25f, 300, K_EASE_LINEAR, 0);
	CHECK_NEAR(Kelement_get_property(b, K_PROP_OPACITY) * 100, 25);
	CHECK(!Kelement_is_animating(b));
	Kroot_set_animations(root, K_ANIMATIONS_ON);
	CHECK(Kroot_animations_enabled(root));
	Kelement_animate(b, K_PROP_OPACITY, 1.0f, 320, K_EASE_LINEAR, 160);
	CHECK(Kelement_is_animating(b));
	run(root, 96);
	/* still in the delay */
	CHECK_NEAR(Kelement_get_property(b, K_PROP_OPACITY) * 100, 25);
	run(root, 160);
	{
		float v = Kelement_get_property(b, K_PROP_OPACITY);
		CHECK(v > 0.3f && v < 1.0f);
	}
	run(root, 320);
	CHECK_NEAR(Kelement_get_property(b, K_PROP_OPACITY) * 100, 100);
	CHECK(!Kelement_is_animating(b));
	/* a new animation of the same property starts from where it is */
	Kelement_animate(b, K_PROP_ROTATION_Y, 40, 100, K_EASE_LINEAR, 0);
	run(root, 48);
	Kelement_animate(b, K_PROP_ROTATION_Y, 0, 100, K_EASE_LINEAR, 0);
	run(root, 200);
	CHECK_NEAR(Kelement_get_property(b, K_PROP_ROTATION_Y), 0);
	Kroot_destroy(root);
}

/* The width of the red band across the middle of a frame. */
static int red_extent(KFrame f, int* left) {
	int x, first = -1, last = -1, y = f.height / 2;
	for (x = 0; x < f.width; x++) {
		const uint8_t* px = f.pixels + (size_t)y * (size_t)f.stride + (size_t)x * 4;
		if (px[0] > 150 && px[1] < 80 && px[2] < 80) {
			if (first < 0) first = x;
			last = x;
		}
	}
	*left = first;
	return first < 0 ? 0 : last - first + 1;
}

static void test_projection_on_gpu(void) {
	Kid root = Kroot_create(480, 800), border;
	KFrame f;
	int left, width_flat, width_turned;
	Kroot_set_renderer(root, K_RENDERER_GPU);
	border = Kborder_create();
	Kelement_set_width(border, 200);
	Kelement_set_height(border, 200);
	Kelement_set_horizontal_alignment(border, K_HALIGN_CENTER);
	Kelement_set_vertical_alignment(border, K_VALIGN_CENTER);
	Kborder_set_background(border, Kcolor_rgba(1, 0, 0, 1));
	Kroot_set_visual(root, border);
	if (!Kroot_render(root, &f)) {
		printf("  no OpenGL ES 2 context here: the projection is not checked\n");
		Kroot_destroy(root);
		return;
	}
	width_flat = red_extent(f, &left);
	CHECK_NEAR(width_flat, 200);
	Kelement_set_projection(border, 0, 60, 0.5f, 0.5f);
	Kroot_render(root, &f);
	width_turned = red_extent(f, &left);
	/* cos 60 = half the width, nearer or further a little for the perspective */
	CHECK(width_turned > 80 && width_turned < 125);
	/* round the left edge the left edge stays where it was */
	Kelement_set_projection(border, 0, 60, 0.0f, 0.5f);
	Kroot_render(root, &f);
	width_turned = red_extent(f, &left);
	CHECK(width_turned > 80 && width_turned < 125);
	CHECK(left >= 138 && left <= 142);
	/* scale about the middle */
	Kelement_set_projection(border, 0, 0, 0.5f, 0.5f);
	Kelement_set_render_scale(border, 0.5f);
	Kroot_render(root, &f);
	CHECK_NEAR(red_extent(f, &left), 100);
	Kelement_set_render_scale(border, 1);
	Kelement_set_translate(border, 50, 0);
	Kroot_render(root, &f);
	red_extent(f, &left);
	CHECK(left >= 188 && left <= 192);
	Kroot_destroy(root);
}


/* ---- the turnstile ---- */

static char g_log[256];
static Kid g_page_a, g_page_b;
static void note(const char* s) { strcat(g_log, s); }
static void from_a(Kid p, void* u) { (void)p; (void)u; note("fromA "); }
static void to_b(Kid p, void* u) { (void)p; (void)u; note("toB "); }
static void navigated(Kid p, void* u) { (void)p; (void)u; note("nav "); }

static Kid build_page(const char* tag, int rows) {
	Kid page = Kpage_create(), stack = Kstack_panel_create(K_VERTICAL);
	int i;
	(void)tag;
	for (i = 0; i < rows; i++) Kpanel_add(stack, Kbutton_create("row"));
	Kpage_set_content(page, stack);
	return page;
}

static Kid page_a(Kid frame, void* param) {
	(void)frame; (void)param;
	g_page_a = build_page("a", 3);
	Kpage_on_navigated_from(g_page_a, from_a, NULL);
	return g_page_a;
}

static Kid page_b(Kid frame, void* param) {
	(void)frame; (void)param;
	g_page_b = build_page("b", 3);
	Kpage_on_navigated_to(g_page_b, to_b, NULL);
	return g_page_b;
}

static Kid first_row(Kid page) {
	Kid stack = Kpage_get_content(page);
	return Kpanel_get_child(stack, 0);
}

static void test_turnstile(void) {
	Kid root = make_root(K_ANIMATIONS_ON), frame = Kframe_create(), row0, row2;
	Kroot_set_visual(root, frame);
	Kelement_on(frame, K_EVENT_NAVIGATED, navigated, NULL);
	g_log[0] = 0;
	/* the first page has nothing to turn away from, but it swings in: the app opens as the second half of a turn */
	CHECK(Kframe_navigate(frame, page_a, NULL));
	CHECK(Kframe_is_transitioning(frame));
	CHECK_NEAR(Kelement_get_property(first_row(g_page_a), K_PROP_OPACITY), 0);
	run(root, 64);
	CHECK(Kelement_get_property(first_row(g_page_a), K_PROP_OPACITY) > 0);
	CHECK(Kelement_get_property(first_row(g_page_a), K_PROP_ROTATION_Y) > 1);
	run(root, 600);
	CHECK(!Kframe_is_transitioning(frame));
	CHECK_NEAR(Kelement_get_property(first_row(g_page_a), K_PROP_OPACITY) * 100, 100);
	g_log[0] = 0;
	CHECK(Kframe_navigate(frame, page_b, NULL));
	CHECK(Kframe_is_transitioning(frame));
	CHECK(strcmp(g_log, "fromA ") == 0);
	/* the old page is still the one in the frame, turning away, and cannot be touched */
	CHECK(Kelement_is_a(g_page_a, K_CLASS_PAGE));
	CHECK(Kframe_get_page(frame) == g_page_b);
	row0 = first_row(g_page_a);
	row2 = Kpanel_get_child(Kpage_get_content(g_page_a), 2);
	run(root, 64);
	CHECK(Kelement_get_property(row0, K_PROP_ROTATION_Y) < -1);
	CHECK(Kelement_get_property(row0, K_PROP_OPACITY) < 1);
	/* the rows follow one another: the third has turned less than the first */
	CHECK(Kelement_get_property(row2, K_PROP_ROTATION_Y) > Kelement_get_property(row0, K_PROP_ROTATION_Y));
	CHECK(strcmp(g_log, "fromA ") == 0);
	/* the new page comes in after the gap, and the old one is gone */
	run(root, 320);
	CHECK(strcmp(g_log, "fromA toB nav ") == 0);
	CHECK(!Kelement_is_a(g_page_a, K_CLASS_PAGE) || g_page_a == g_page_b);
	CHECK(Kframe_is_transitioning(frame));
	row0 = first_row(g_page_b);
	CHECK(Kelement_get_property(row0, K_PROP_OPACITY) < 1);
	/* and settles */
	run(root, 600);
	CHECK(!Kframe_is_transitioning(frame));
	CHECK_NEAR(Kelement_get_property(row0, K_PROP_ROTATION_Y), 0);
	CHECK_NEAR(Kelement_get_property(row0, K_PROP_OPACITY) * 100, 100);
	/* a Navigate in the middle of a transition ends it at once */
	g_log[0] = 0;
	CHECK(Kframe_navigate(frame, page_a, NULL));
	run(root, 100);
	CHECK(Kframe_navigate(frame, page_b, NULL));
	CHECK(strstr(g_log, "nav ") != NULL);
	CHECK(Kframe_get_back_stack_depth(frame) == 3);
	/* going back turns the other way */
	run(root, 1000);
	CHECK(Kframe_go_back(frame));
	run(root, 64);
	/* the rows go in the order of the last to the first, and the other way round */
	row2 = Kpanel_get_child(Kpage_get_content(Kcontent_control_get_content(frame)), 2);
	CHECK(Kelement_get_property(row2, K_PROP_ROTATION_Y) > 1);
	Kroot_destroy(root);
}

static void test_no_transition_without_animations(void) {
	Kid root = make_root(K_ANIMATIONS_OFF), frame = Kframe_create();
	Kroot_set_visual(root, frame);
	Kframe_navigate(frame, page_a, NULL);
	Kframe_navigate(frame, page_b, NULL);
	CHECK(!Kframe_is_transitioning(frame));
	CHECK(Kframe_get_page(frame) == g_page_b);
	Kroot_destroy(root);
	/* a frame that is told not to */
	root = make_root(K_ANIMATIONS_ON);
	frame = Kframe_create();
	Kroot_set_visual(root, frame);
	Kframe_set_transition(frame, K_TRANSITION_NONE);
	Kframe_navigate(frame, page_a, NULL);
	Kframe_navigate(frame, page_b, NULL);
	CHECK(!Kframe_is_transitioning(frame));
	Kroot_destroy(root);
}


/* ---- tilt ---- */

static void test_tilt(void) {
	Kid root = make_root(K_ANIMATIONS_ON), stack = Kstack_panel_create(K_VERTICAL), link;
	KRect r;
	KPoint c;
	Kroot_set_visual(root, stack);
	link = Kpanel_add(stack, Khyperlink_button_create("a link that is wide enough"));
	Kelement_set_vertical_alignment(link, K_VALIGN_TOP);
	Kroot_update_layout(root);
	r = Kelement_get_bounds(link);
	c = Kelement_to_root(link, (KPoint){ r.width * 0.5f, r.height * 0.5f });
	/* the right of the middle, a little below it */
	Kroot_inject_pointer(root, K_POINTER_MOVE, c.x + r.width * 0.3f, c.y + 4);
	Kroot_inject_pointer(root, K_POINTER_DOWN, c.x + r.width * 0.3f, c.y + 4);
	run(root, 128);
	CHECK(Kelement_get_property(link, K_PROP_ROTATION_Y) > 1);
	CHECK(Kelement_get_property(link, K_PROP_SCALE) < 1);
	CHECK(Kelement_get_property(link, K_PROP_ROTATION_Y) < 15);
	Kroot_inject_pointer(root, K_POINTER_UP, c.x + r.width * 0.3f, c.y + 4);
	run(root, 320);
	CHECK_NEAR(Kelement_get_property(link, K_PROP_ROTATION_Y), 0);
	CHECK_NEAR(Kelement_get_property(link, K_PROP_SCALE) * 100, 100);
	/* a Button does not lean, and with animations off nothing does */
	{
		Kid b = Kpanel_add(stack, Kbutton_create("button"));
		Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
		Kelement_set_margin(b, Kthickness(0, 60, 0, 0));
		Kroot_update_layout(root);
		r = Kelement_get_bounds(b);
		c = Kelement_to_root(b, (KPoint){ r.width * 0.5f, r.height * 0.5f });
		Kroot_inject_pointer(root, K_POINTER_MOVE, c.x, c.y);
		Kroot_inject_pointer(root, K_POINTER_DOWN, c.x, c.y);
		run(root, 128);
		CHECK_NEAR(Kelement_get_property(b, K_PROP_SCALE) * 100, 100);
		Kroot_inject_pointer(root, K_POINTER_UP, c.x, c.y);
	}
	Kroot_set_animations(root, K_ANIMATIONS_OFF);
	c = Kelement_to_root(link, (KPoint){ 10, 5 });
	Kroot_inject_pointer(root, K_POINTER_MOVE, c.x, c.y);
	Kroot_inject_pointer(root, K_POINTER_DOWN, c.x, c.y);
	run(root, 128);
	CHECK_NEAR(Kelement_get_property(link, K_PROP_ROTATION_Y), 0);
	Kroot_inject_pointer(root, K_POINTER_UP, c.x, c.y);
	Kroot_destroy(root);
}

/* The turnstile as pictures, for the eyes: out at 80 ms, the gap, in at 420 ms. */
static Kid titled_page(Kid frame, void* param) {
	Kid page = Kpage_create(), stack = Kstack_panel_create(K_VERTICAL), t;
	int i;
	(void)frame;
	Kelement_set_margin(stack, Kthickness(19, 0, 19, 0));
	t = Kpanel_add(stack, Ktext_block_create(param ? "SECOND" : "FIRST"));
	Ktext_block_set_style(t, K_TEXT_STYLE_PHONE_TITLE1);
	for (i = 0; i < 5; i++) Kpanel_add(stack, Kbutton_create("a row of the page"));
	Kpage_set_content(page, stack);
	return page;
}

static void test_turnstile_pictures(void) {
	Kid root = Kroot_create(480, 800), frame = Kframe_create();
	KFrame f;
	char path[512];
	Kroot_set_renderer(root, K_RENDERER_GPU);
	Kroot_set_animations(root, K_ANIMATIONS_ON);
	Kroot_set_visual(root, frame);
	Kframe_navigate(frame, titled_page, NULL);
	if (!Kroot_render(root, &f)) {
		printf("  no OpenGL ES 2 context here: no pictures\n");
		Kroot_destroy(root);
		return;
	}
	Kframe_navigate(frame, titled_page, "second");
	run(root, 80);
	snprintf(path, sizeof path, "%s/turnstile_out_80ms.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	run(root, 352);
	snprintf(path, sizeof path, "%s/turnstile_in_432ms.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	run(root, 600);
	CHECK(!Kframe_is_transitioning(frame));
	Kroot_destroy(root);
}

int main(void) {
	RUN(test_easing);
	RUN(test_animate);
	RUN(test_projection_on_gpu);
	RUN(test_turnstile);
	RUN(test_no_transition_without_animations);
	RUN(test_tilt);
	RUN(test_turnstile_pictures);
	return test_finish("test_animation");
}
