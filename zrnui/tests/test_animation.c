/**
 * Animation: easing, Zelement_animate, render transforms on the GPU, the turnstile of a Frame
 * and the lean of a pressed element. The clock is Zroot_tick, so time is moved by hand.
 */

#include "test.h"

static void run(const Zid root, const float ms) {
	for (int step = 0; (float)step * 16.0f < ms; step++) Zroot_tick(root, 16);
}

static Zid make_root(ZAnimations mode) {
	Zid root = zst_test_root(480, 800);
	Zroot_set_renderer(root, Z_RENDERER_SOFTWARE);
	Zroot_set_animations(root, mode);
	return root;
}

static void test_easing(void) {
	ZEasing e;
	for (e = Z_EASE_LINEAR; e <= Z_EASE_EMPHASIZED; e++) {
		float prev = 0;
		int i;
		CHECK_NEAR(Zeasing(e, 0), 0);
		CHECK_NEAR(Zeasing(e, 1), 1);
		for (i = 1; i <= 20; i++) {
			float v = Zeasing(e, (float)i / 20);
			CHECK(v >= prev - 1e-4f);
			prev = v;
		}
	}
	CHECK(Zeasing(Z_EASE_OUT, 0.5f) > 0.5f);
	CHECK(Zeasing(Z_EASE_IN, 0.5f) < 0.5f);
	CHECK(Zeasing(Z_EASE_EMPHASIZED, 0.3f) > 0.5f);
}

static void test_animate(void) {
	Zid root = make_root(Z_ANIMATIONS_OFF), stack = Zstack_panel_create(Z_VERTICAL), b;
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zborder_create());
	/* off: at once */
	Zelement_animate(b, Z_PROP_OPACITY, 0.25f, 300, Z_EASE_LINEAR, 0);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 25);
	CHECK(!Zelement_is_animating(b));
	Zroot_set_animations(root, Z_ANIMATIONS_ON);
	CHECK(Zroot_animations_enabled(root));
	Zelement_animate(b, Z_PROP_OPACITY, 1.0f, 320, Z_EASE_LINEAR, 160);
	CHECK(Zelement_is_animating(b));
	run(root, 96);
	/* still in the delay */
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 25);
	run(root, 160);
	{
		float v = Zelement_get_property(b, Z_PROP_OPACITY);
		CHECK(v > 0.3f && v < 1.0f);
	}
	run(root, 320);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	CHECK(!Zelement_is_animating(b));
	/* a new animation of the same property starts from where it is */
	Zelement_animate(b, Z_PROP_ROTATION_Y, 40, 100, Z_EASE_LINEAR, 0);
	run(root, 48);
	Zelement_animate(b, Z_PROP_ROTATION_Y, 0, 100, Z_EASE_LINEAR, 0);
	run(root, 200);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_ROTATION_Y), 0);
	Zroot_destroy(root);
}

/* The width of the red band across the middle of a frame. */
static int red_extent(ZFrame f, int* left) {
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
	Zid root = zst_test_root(480, 800), border;
	ZFrame f;
	int left, width_flat, width_turned;
	Zroot_set_renderer(root, Z_RENDERER_GPU);
	border = Zborder_create();
	Zelement_set_width(border, 200);
	Zelement_set_height(border, 200);
	Zelement_set_horizontal_alignment(border, Z_HALIGN_CENTER);
	Zelement_set_vertical_alignment(border, Z_VALIGN_CENTER);
	Zborder_set_background(border, Zcolor_rgba(1, 0, 0, 1));
	Zroot_set_visual(root, border);
	if (!Zroot_render(root, &f)) {
		printf("  no OpenGL ES 2 context here: the projection is not checked\n");
		Zroot_destroy(root);
		return;
	}
	width_flat = red_extent(f, &left);
	CHECK_NEAR(width_flat, 200);
	Zelement_set_projection(border, 0, 60, 0.5f, 0.5f);
	Zroot_render(root, &f);
	width_turned = red_extent(f, &left);
	/* cos 60 = half the width, nearer or further a little for the perspective */
	CHECK(width_turned > 80 && width_turned < 125);
	/* round the left edge the left edge stays where it was */
	Zelement_set_projection(border, 0, 60, 0.0f, 0.5f);
	Zroot_render(root, &f);
	width_turned = red_extent(f, &left);
	CHECK(width_turned > 80 && width_turned < 125);
	CHECK(left >= 138 && left <= 142);
	/* scale about the middle */
	Zelement_set_projection(border, 0, 0, 0.5f, 0.5f);
	Zelement_set_render_scale(border, 0.5f);
	Zroot_render(root, &f);
	CHECK_NEAR(red_extent(f, &left), 100);
	Zelement_set_render_scale(border, 1);
	Zelement_set_translate(border, 50, 0);
	Zroot_render(root, &f);
	red_extent(f, &left);
	CHECK(left >= 188 && left <= 192);
	Zroot_destroy(root);
}


/* ---- the turnstile ---- */

static char g_log[256];
static Zid g_page_a, g_page_b;
static void note(const char* s) { strcat(g_log, s); }
static void from_a(Zid p, void* u) { (void)p; (void)u; note("fromA "); }
static void to_b(Zid p, void* u) { (void)p; (void)u; note("toB "); }
static void navigated(Zid p, void* u) { (void)p; (void)u; note("nav "); }

static Zid build_page(const char* tag, int rows) {
	Zid page = Zpage_create(), stack = Zstack_panel_create(Z_VERTICAL);
	int i;
	(void)tag;
	for (i = 0; i < rows; i++) Zpanel_add(stack, Zbutton_create("row"));
	Zpage_set_content(page, stack);
	return page;
}

static Zid page_a(Zid frame, void* param) {
	(void)frame; (void)param;
	g_page_a = build_page("a", 3);
	Zpage_on_navigated_from(g_page_a, from_a, nullptr);
	return g_page_a;
}

static Zid page_b(Zid frame, void* param) {
	(void)frame; (void)param;
	g_page_b = build_page("b", 3);
	Zpage_on_navigated_to(g_page_b, to_b, nullptr);
	return g_page_b;
}

static Zid first_row(Zid page) {
	Zid stack = Zpage_get_content(page);
	return Zpanel_get_child(stack, 0);
}

static void test_turnstile(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), frame = Zframe_create(), row0, row2;
	Zroot_set_visual(root, frame);
	Zelement_on(frame, Z_EVENT_NAVIGATED, navigated, nullptr);
	g_log[0] = 0;
	/* the first page has nothing to turn away from, but it swings in: the app opens as the second half of a turn */
	CHECK(Zframe_navigate(frame, page_a, nullptr));
	CHECK(Zframe_is_transitioning(frame));
	CHECK_NEAR(Zelement_get_property(first_row(g_page_a), Z_PROP_OPACITY), 0);
	run(root, 64);
	CHECK(Zelement_get_property(first_row(g_page_a), Z_PROP_OPACITY) > 0);
	CHECK(Zelement_get_property(first_row(g_page_a), Z_PROP_ROTATION_Y) > 1);
	run(root, 600);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK_NEAR(Zelement_get_property(first_row(g_page_a), Z_PROP_OPACITY) * 100, 100);
	g_log[0] = 0;
	CHECK(Zframe_navigate(frame, page_b, nullptr));
	CHECK(Zframe_is_transitioning(frame));
	CHECK(strcmp(g_log, "fromA ") == 0);
	/* the old page is still the one in the frame, turning away, and cannot be touched */
	CHECK(Zelement_is_a(g_page_a, Z_CLASS_PAGE));
	CHECK(Zframe_get_page(frame) == g_page_b);
	row0 = first_row(g_page_a);
	row2 = Zpanel_get_child(Zpage_get_content(g_page_a), 2);
	run(root, 64);
	CHECK(Zelement_get_property(row0, Z_PROP_ROTATION_Y) < -1);
	CHECK(Zelement_get_property(row0, Z_PROP_OPACITY) < 1);
	/* the rows follow one another: the third has turned less than the first */
	CHECK(Zelement_get_property(row2, Z_PROP_ROTATION_Y) > Zelement_get_property(row0, Z_PROP_ROTATION_Y));
	CHECK(strcmp(g_log, "fromA ") == 0);
	/* the new page comes in after the gap, and the old one is gone */
	run(root, 320);
	CHECK(strcmp(g_log, "fromA toB nav ") == 0);
	CHECK(!Zelement_is_a(g_page_a, Z_CLASS_PAGE) || g_page_a == g_page_b);
	CHECK(Zframe_is_transitioning(frame));
	row0 = first_row(g_page_b);
	CHECK(Zelement_get_property(row0, Z_PROP_OPACITY) < 1);
	/* and settles */
	run(root, 600);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK_NEAR(Zelement_get_property(row0, Z_PROP_ROTATION_Y), 0);
	CHECK_NEAR(Zelement_get_property(row0, Z_PROP_OPACITY) * 100, 100);
	/* a Navigate in the middle of a transition ends it at once */
	g_log[0] = 0;
	CHECK(Zframe_navigate(frame, page_a, nullptr));
	run(root, 100);
	CHECK(Zframe_navigate(frame, page_b, nullptr));
	CHECK(strstr(g_log, "nav ") != nullptr);
	CHECK(Zframe_get_back_stack_depth(frame) == 3);
	/* going back turns the other way */
	run(root, 1000);
	CHECK(Zframe_go_back(frame));
	run(root, 64);
	/* the rows go in the order of the last to the first, and the other way round */
	row2 = Zpanel_get_child(Zpage_get_content(Zcontent_control_get_content(frame)), 2);
	CHECK(Zelement_get_property(row2, Z_PROP_ROTATION_Y) > 1);
	Zroot_destroy(root);
}

static void test_no_transition_without_animations(void) {
	Zid root = make_root(Z_ANIMATIONS_OFF), frame = Zframe_create();
	Zroot_set_visual(root, frame);
	Zframe_navigate(frame, page_a, nullptr);
	Zframe_navigate(frame, page_b, nullptr);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK(Zframe_get_page(frame) == g_page_b);
	Zroot_destroy(root);
	/* a frame that is told not to */
	root = make_root(Z_ANIMATIONS_ON);
	frame = Zframe_create();
	Zroot_set_visual(root, frame);
	Zframe_set_transition(frame, Z_TRANSITION_NONE);
	Zframe_navigate(frame, page_a, nullptr);
	Zframe_navigate(frame, page_b, nullptr);
	CHECK(!Zframe_is_transitioning(frame));
	Zroot_destroy(root);
}


/* ---- tilt ---- */

static void test_tilt(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), stack = Zstack_panel_create(Z_VERTICAL), link;
	ZRect r;
	ZPoint c;
	Zroot_set_visual(root, stack);
	link = Zpanel_add(stack, Zhyperlink_button_create("a link that is wide enough"));
	Zelement_set_vertical_alignment(link, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	r = Zelement_get_bounds(link);
	c = Zelement_to_root(link, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
	/* the right of the middle, a little below it */
	Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x + r.width * 0.3f, c.y + 4);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, c.x + r.width * 0.3f, c.y + 4);
	run(root, 128);
	CHECK(Zelement_get_property(link, Z_PROP_ROTATION_Y) > 1);
	CHECK(Zelement_get_property(link, Z_PROP_SCALE) < 1);
	CHECK(Zelement_get_property(link, Z_PROP_ROTATION_Y) < 15);
	Zroot_inject_pointer(root, Z_POINTER_UP, c.x + r.width * 0.3f, c.y + 4);
	run(root, 320);
	CHECK_NEAR(Zelement_get_property(link, Z_PROP_ROTATION_Y), 0);
	CHECK_NEAR(Zelement_get_property(link, Z_PROP_SCALE) * 100, 100);
	/* a Button does not lean, and with animations off nothing does */
	{
		Zid b = Zpanel_add(stack, Zbutton_create("button"));
		Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
		Zelement_set_margin(b, Zthickness(0, 60, 0, 0));
		Zroot_update_layout(root);
		r = Zelement_get_bounds(b);
		c = Zelement_to_root(b, (ZPoint){ r.width * 0.5f, r.height * 0.5f });
		Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x, c.y);
		Zroot_inject_pointer(root, Z_POINTER_DOWN, c.x, c.y);
		run(root, 128);
		CHECK_NEAR(Zelement_get_property(b, Z_PROP_SCALE) * 100, 100);
		Zroot_inject_pointer(root, Z_POINTER_UP, c.x, c.y);
	}
	Zroot_set_animations(root, Z_ANIMATIONS_OFF);
	c = Zelement_to_root(link, (ZPoint){ 10, 5 });
	Zroot_inject_pointer(root, Z_POINTER_MOVE, c.x, c.y);
	Zroot_inject_pointer(root, Z_POINTER_DOWN, c.x, c.y);
	run(root, 128);
	CHECK_NEAR(Zelement_get_property(link, Z_PROP_ROTATION_Y), 0);
	Zroot_inject_pointer(root, Z_POINTER_UP, c.x, c.y);
	Zroot_destroy(root);
}

/* The turnstile as pictures, for the eyes: out at 80 ms, the gap, in at 420 ms. */
static Zid titled_page(Zid frame, void* param) {
	Zid page = Zpage_create(), stack = Zstack_panel_create(Z_VERTICAL), t;
	int i;
	(void)frame;
	Zelement_set_margin(stack, Zthickness(19, 0, 19, 0));
	t = Zpanel_add(stack, Ztext_block_create(param ? "SECOND" : "FIRST"));
	Ztext_block_set_style(t, Z_TEXT_STYLE_PHONE_TITLE1);
	for (i = 0; i < 5; i++) Zpanel_add(stack, Zbutton_create("a row of the page"));
	Zpage_set_content(page, stack);
	return page;
}

static void test_turnstile_pictures(void) {
	Zid root = zst_test_root(480, 800), frame = Zframe_create();
	ZFrame f;
	char path[512];
	Zroot_set_renderer(root, Z_RENDERER_GPU);
	Zroot_set_animations(root, Z_ANIMATIONS_ON);
	Zroot_set_visual(root, frame);
	Zframe_navigate(frame, titled_page, nullptr);
	if (!Zroot_render(root, &f)) {
		printf("  no OpenGL ES 2 context here: no pictures\n");
		Zroot_destroy(root);
		return;
	}
	Zframe_navigate(frame, titled_page, "second");
	run(root, 80);
	snprintf(path, sizeof path, "%s/turnstile_out_80ms.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	run(root, 352);
	snprintf(path, sizeof path, "%s/turnstile_in_432ms.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(root, path));
	run(root, 600);
	CHECK(!Zframe_is_transitioning(frame));
	Zroot_destroy(root);
}


/* ---- the transitions of Windows Phone, by kind ---- */

static int g_completed;
static void completed(Zid e, void* u) { (void)e; (void)u; g_completed++; }

static int is_feather(ZTransition t) { return t >= Z_TRANSITION_FEATHER_FORWARD_IN && t <= Z_TRANSITION_FEATHER_BACKWARD_OUT; }
static int is_out(ZTransition t) {
	switch (t) {
	case Z_TRANSITION_FADE_OUT: case Z_TRANSITION_SLIDE_UP_FADE_OUT: case Z_TRANSITION_SLIDE_DOWN_FADE_OUT:
	case Z_TRANSITION_SLIDE_LEFT_FADE_OUT: case Z_TRANSITION_SLIDE_RIGHT_FADE_OUT: case Z_TRANSITION_SWIVEL_FULL_SCREEN_OUT:
	case Z_TRANSITION_SWIVEL_FORWARD_OUT: case Z_TRANSITION_SWIVEL_BACKWARD_OUT: case Z_TRANSITION_TURNSTILE_FORWARD_OUT:
	case Z_TRANSITION_TURNSTILE_BACKWARD_OUT: case Z_TRANSITION_ROTATE_OUT_90_CLOCKWISE: case Z_TRANSITION_ROTATE_OUT_90_COUNTERCLOCKWISE:
	case Z_TRANSITION_ROTATE_OUT_180_CLOCKWISE: case Z_TRANSITION_ROTATE_OUT_180_COUNTERCLOCKWISE: case Z_TRANSITION_ROLL_OUT:
		return 1;
	default: return 0;
	}
}

static int at_rest(Zid e) {
	return fabsf(Zelement_get_property(e, Z_PROP_ROTATION_X)) < 1e-3f && fabsf(Zelement_get_property(e, Z_PROP_ROTATION_Y)) < 1e-3f &&
		fabsf(Zelement_get_property(e, Z_PROP_ROTATION_Z)) < 1e-3f && fabsf(Zelement_get_property(e, Z_PROP_TRANSLATE_X)) < 1e-3f &&
		fabsf(Zelement_get_property(e, Z_PROP_TRANSLATE_Y)) < 1e-3f && fabsf(Zelement_get_property(e, Z_PROP_SCALE) - 1) < 1e-3f;
}

static void test_transition_kinds(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), stack = Zstack_panel_create(Z_VERTICAL), b;
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zborder_create());
	Zelement_set_opacity(b, 0.5f);
	Zelement_on(b, Z_EVENT_TRANSITION_COMPLETED, completed, nullptr);
	for (ZTransition t = Z_TRANSITION_FADE_IN; t < Z_TRANSITION_LAST_; t++) {
		if (is_feather(t)) continue;
		g_completed = 0;
		Zelement_play_transition(b, t, 100);
		/* it shows the start at once, and keeps it while it waits: an "In" is not there yet, an "Out" is untouched */
		if (is_out(t)) CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 50);
		else CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
		CHECK(Zelement_is_animating(b));
		run(root, 64);
		if (is_out(t)) CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 50);
		else CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
		CHECK(g_completed == 0);
		run(root, 700);
		CHECK(!Zelement_is_animating(b));
		CHECK(g_completed == 1);
		if (is_out(t)) {
			CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
			Zelement_clear_transition(b);
		} else {
			/* an "In" ends at rest, at the opacity the element had */
			CHECK(at_rest(b));
			CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 50);
		}
		CHECK(at_rest(b));
		CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 50);
	}
	/* the directions: a slide left comes from the right, a clockwise rotation in turns clockwise */
	Zelement_play_transition(b, Z_TRANSITION_SLIDE_LEFT_FADE_IN, 0);
	CHECK(Zelement_get_property(b, Z_PROP_TRANSLATE_X) > 0);
	run(root, 700);
	Zelement_play_transition(b, Z_TRANSITION_ROTATE_IN_90_CLOCKWISE, 0);
	CHECK(Zelement_get_property(b, Z_PROP_ROTATION_Z) < 0);
	run(root, 40);
	CHECK(Zelement_get_property(b, Z_PROP_ROTATION_Z) > -90 && Zelement_get_property(b, Z_PROP_ROTATION_Z) < 0);
	run(root, 700);
	/* a kind that is not an element's is refused */
	Zelement_play_transition(b, Z_TRANSITION_TURNSTILE, 0);
	CHECK(at_rest(b));
	Zroot_destroy(root);
}

static void test_transition_without_animations(void) {
	Zid root = make_root(Z_ANIMATIONS_OFF), stack = Zstack_panel_create(Z_VERTICAL), b;
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zborder_create());
	Zelement_on(b, Z_EVENT_TRANSITION_COMPLETED, completed, nullptr);
	g_completed = 0;
	Zelement_play_transition(b, Z_TRANSITION_SLIDE_UP_FADE_IN, 100);
	CHECK(at_rest(b));
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	CHECK(g_completed == 1);
	/* an "Out" ends at once */
	Zelement_play_transition(b, Z_TRANSITION_FADE_OUT, 0);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
	Zelement_clear_transition(b);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	Zroot_destroy(root);
}

static void test_transition_appears(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), stack = Zstack_panel_create(Z_VERTICAL), b, c;
	Zroot_set_visual(root, stack);
	/* set before it is in a tree: it plays when it gets there */
	b = Zborder_create();
	Zelement_set_transition(b, Z_TRANSITION_FADE_IN, 50);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	Zpanel_add(stack, b);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
	run(root, 800);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	/* and each time it becomes Visible */
	Zelement_set_visibility(b, Z_COLLAPSED);
	Zelement_set_visibility(b, Z_VISIBLE);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 0);
	run(root, 800);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_OPACITY) * 100, 100);
	/* set on an element that is shown: it plays now; NONE stops it */
	c = Zpanel_add(stack, Zborder_create());
	Zelement_set_transition(c, Z_TRANSITION_SLIDE_RIGHT_FADE_IN, 0);
	CHECK(Zelement_get_property(c, Z_PROP_TRANSLATE_X) < 0);
	Zelement_set_transition(c, Z_TRANSITION_NONE, 0);
	Zelement_play_transition(c, Z_TRANSITION_NONE, 0);
	CHECK(!Zelement_is_animating(c));
	Zroot_destroy(root);
}

static void test_transition_feather(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), stack = Zstack_panel_create(Z_VERTICAL), r0, r2;
	int i;
	Zroot_set_visual(root, stack);
	for (i = 0; i < 3; i++) Zpanel_add(stack, Zbutton_create("row"));
	Zroot_update_layout(root);
	r0 = Zpanel_get_child(stack, 0);
	r2 = Zpanel_get_child(stack, 2);
	Zelement_play_transition(stack, Z_TRANSITION_FEATHER_FORWARD_OUT, 0);
	run(root, 64);
	CHECK(Zelement_get_property(r0, Z_PROP_ROTATION_Y) < -1);
	/* the rows follow one another */
	CHECK(Zelement_get_property(r2, Z_PROP_ROTATION_Y) > Zelement_get_property(r0, Z_PROP_ROTATION_Y));
	run(root, 400);
	CHECK_NEAR(Zelement_get_property(r0, Z_PROP_OPACITY), 0);
	CHECK_NEAR(Zelement_get_property(r2, Z_PROP_OPACITY), 0);
	Zelement_clear_transition(stack);
	CHECK(at_rest(r0) && at_rest(r2));
	CHECK_NEAR(Zelement_get_property(r0, Z_PROP_OPACITY), 1);
	/* going in: from behind the screen */
	Zelement_play_transition(stack, Z_TRANSITION_FEATHER_BACKWARD_IN, 0);
	CHECK(Zelement_get_property(r0, Z_PROP_ROTATION_Y) > 1);
	run(root, 600);
	CHECK(at_rest(r0) && at_rest(r2));
	CHECK_NEAR(Zelement_get_property(r2, Z_PROP_OPACITY), 1);
	Zroot_destroy(root);
}

static void test_frame_transitions(void) {
	Zid root = make_root(Z_ANIMATIONS_ON), frame = Zframe_create();
	Zroot_set_visual(root, frame);
	/* the Toolkit's pair per direction: the old page slides out to the left, the new one in from the right; back is the other way */
	Zframe_set_transitions(frame, Z_TRANSITION_SLIDE_LEFT_FADE_OUT, Z_TRANSITION_SLIDE_LEFT_FADE_IN,
		Z_TRANSITION_SLIDE_RIGHT_FADE_OUT, Z_TRANSITION_SLIDE_RIGHT_FADE_IN);
	Zelement_on(frame, Z_EVENT_NAVIGATED, navigated, nullptr);
	CHECK(Zframe_navigate(frame, page_a, nullptr));
	run(root, 800);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK(at_rest(g_page_a));
	g_log[0] = 0;
	CHECK(Zframe_navigate(frame, page_b, nullptr));
	CHECK(Zframe_is_transitioning(frame));
	CHECK(strcmp(g_log, "fromA ") == 0);
	run(root, 64);
	CHECK(Zelement_get_property(g_page_a, Z_PROP_TRANSLATE_X) < 0);
	CHECK(Zelement_get_property(g_page_a, Z_PROP_OPACITY) < 1);
	CHECK(strcmp(g_log, "fromA ") == 0);
	/* the new page comes in when the old one has gone, from the right */
	run(root, 220);
	CHECK(strcmp(g_log, "fromA toB nav ") == 0);
	CHECK(Zframe_is_transitioning(frame));
	CHECK(Zelement_get_property(g_page_b, Z_PROP_TRANSLATE_X) > 0);
	run(root, 700);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK(at_rest(g_page_b));
	CHECK_NEAR(Zelement_get_property(g_page_b, Z_PROP_OPACITY) * 100, 100);
	/* going back: out to the right */
	CHECK(Zframe_go_back(frame));
	run(root, 64);
	CHECK(Zelement_get_property(g_page_b, Z_PROP_TRANSLATE_X) > 0);
	/* a Navigate in the middle ends it at once, with nothing left turned */
	CHECK(Zframe_navigate(frame, page_a, nullptr));
	run(root, 1200);
	CHECK(!Zframe_is_transitioning(frame));
	CHECK(at_rest(Zframe_get_page(frame)));
	CHECK_NEAR(Zelement_get_property(Zframe_get_page(frame), Z_PROP_OPACITY) * 100, 100);
	/* NONE for every half: the page is replaced at once */
	Zframe_set_transitions(frame, Z_TRANSITION_NONE, Z_TRANSITION_NONE, Z_TRANSITION_NONE, Z_TRANSITION_NONE);
	CHECK(Zframe_navigate(frame, page_b, nullptr));
	run(root, 32);
	CHECK(!Zframe_is_transitioning(frame));
	/* and back to the feathered turnstile */
	Zframe_set_transition(frame, Z_TRANSITION_TURNSTILE);
	CHECK(Zframe_navigate(frame, page_a, nullptr));
	run(root, 64);
	CHECK(Zelement_get_property(first_row(g_page_b), Z_PROP_ROTATION_Y) < -1);
	Zroot_destroy(root);
}

/* ZRNUI_ANIMATION_SPEED moves the tweens too: at 2 a 100 ms animation takes 50 ms of the clock */
static void test_animation_speed(void) {
	ZConfig config;
	Zid root, stack, b;
	Zconfig_default(&config);
	config.animation_speed = 2.0f;
	config.theme_file = "";
	CHECK(Zinit_ex(&config));
	root = make_root(Z_ANIMATIONS_ON);
	stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	b = Zpanel_add(stack, Zborder_create());
	Zelement_animate(b, Z_PROP_TRANSLATE_X, 100, 100, Z_EASE_LINEAR, 0);
	run(root, 32);
	{
		const float v = Zelement_get_property(b, Z_PROP_TRANSLATE_X);
		CHECK(v > 55 && v < 75);
	}
	run(root, 48);
	CHECK_NEAR(Zelement_get_property(b, Z_PROP_TRANSLATE_X), 100);
	Zroot_destroy(root);
	Zshutdown();
}

int main(void) {
	RUN(test_animation_speed);
	RUN(test_easing);
	RUN(test_animate);
	RUN(test_projection_on_gpu);
	RUN(test_turnstile);
	RUN(test_no_transition_without_animations);
	RUN(test_tilt);
	RUN(test_turnstile_pictures);
	RUN(test_transition_kinds);
	RUN(test_transition_without_animations);
	RUN(test_transition_appears);
	RUN(test_transition_feather);
	RUN(test_frame_transitions);
	return test_finish("test_animation");
}
