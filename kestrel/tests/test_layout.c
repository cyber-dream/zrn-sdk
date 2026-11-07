/**
 * Measure / Arrange: StackPanel, Grid (pixel, auto, star), WrapPanel,
 * Canvas, Border, Viewbox, margins, alignment, size limits, visibility.
 */

#include "test.h"

static Kid box(float w, float h) {
	Kid b = Kborder_create();
	Kelement_set_width(b, w);
	Kelement_set_height(b, h);
	return b;
}

static Kid host(Kid visual, int w, int h) {
	Kid root = Kroot_create(w, h);
	Kroot_set_visual(root, visual);
	Kroot_update_layout(root);
	return root;
}

static void test_stack_vertical(void) {
	Kid s = Kstack_panel_create(K_VERTICAL), a = Kpanel_add(s, box(100, 40)), b = Kpanel_add(s, box(60, 30)), root;
	Kelement_set_margin(b, Kthickness(5, 10, 5, 10));
	root = host(s, 300, 400);
	/* children are stacked top to bottom, each as high as it asked, stretched across */
	CHECK_NEAR(Kelement_get_bounds(a).y, 0);
	CHECK_NEAR(Kelement_get_bounds(a).height, 40);
	CHECK_NEAR(Kelement_get_bounds(b).y, 40 + 10);
	CHECK_NEAR(Kelement_get_bounds(b).x, 5 + (300 - 10 - 60) * 0.5f); /* fixed width: centered in the slot */
	CHECK_NEAR(Kelement_get_desired_size(s).height, 40 + 30 + 20);
	CHECK_NEAR(Kelement_get_desired_size(s).width, 100);
	Kroot_destroy(root);
}

static void test_stack_horizontal(void) {
	Kid s = Kstack_panel_create(K_HORIZONTAL), a = Kpanel_add(s, box(100, 40)), b = Kpanel_add(s, box(60, 30)), root;
	Kelement_set_vertical_alignment(b, K_VALIGN_BOTTOM);
	root = host(s, 300, 400);
	CHECK_NEAR(Kelement_get_bounds(b).x, 100);
	CHECK_NEAR(Kelement_get_bounds(a).height, 40);
	/* the panel is only as high as its tallest child: stretch fills 400, so the bottom aligned one sits at the panel's bottom */
	CHECK_NEAR(Kelement_get_bounds(b).y + Kelement_get_bounds(b).height, Kelement_get_actual_height(s));
	Kroot_destroy(root);
}

static void test_collapsed_takes_no_room(void) {
	Kid s = Kstack_panel_create(K_VERTICAL), a = Kpanel_add(s, box(100, 40)), b = Kpanel_add(s, box(100, 30)), c = Kpanel_add(s, box(100, 20)), root;
	Kelement_set_visibility(b, K_COLLAPSED);
	root = host(s, 300, 400);
	CHECK_NEAR(Kelement_get_bounds(c).y, Kelement_get_bounds(a).y + 40);
	Kelement_set_visibility(b, K_VISIBLE);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(c).y, 70);
	Kroot_destroy(root);
}

static void test_alignment_and_limits(void) {
	Kid grid = Kgrid_create(), c = Kgrid_place(grid, box(50, 20), 0, 0), root;
	root = host(grid, 200, 100);
	/* a fixed size, stretched alignment: centered */
	CHECK_NEAR(Kelement_get_bounds(c).x, 75);
	CHECK_NEAR(Kelement_get_bounds(c).y, 40);
	Kelement_set_horizontal_alignment(c, K_HALIGN_RIGHT);
	Kelement_set_vertical_alignment(c, K_VALIGN_TOP);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(c).x, 150);
	CHECK_NEAR(Kelement_get_bounds(c).y, 0);
	Kelement_set_width(c, K_AUTO);
	Kelement_set_min_size(c, 120, 0);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(c).width, 120);
	Kelement_set_horizontal_alignment(c, K_HALIGN_STRETCH);
	Kelement_set_max_size(c, 90, K_INFINITE);
	Kelement_set_min_size(c, 0, 0);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(c).width, 90);
	Kroot_destroy(root);
}

static void test_grid_star_sizing(void) {
	Kid grid = Kgrid_create(), a, b, c, d, root;
	Kgrid_add_column(grid, K_GRID_PIXEL, 100);
	Kgrid_add_column(grid, K_GRID_AUTO, 0);
	Kgrid_add_column(grid, K_GRID_STAR, 1);
	Kgrid_add_column(grid, K_GRID_STAR, 2);
	Kgrid_add_row(grid, K_GRID_AUTO, 0);
	Kgrid_add_row(grid, K_GRID_STAR, 1);
	a = Kgrid_place(grid, box(10, 30), 0, 0);
	b = Kgrid_place(grid, box(50, 10), 1, 0);
	c = Kgrid_place(grid, box(10, 10), 2, 1);
	d = Kgrid_place(grid, box(10, 10), 3, 1);
	Kelement_set_horizontal_alignment(c, K_HALIGN_STRETCH);
	Kelement_set_width(c, K_AUTO);
	Kelement_set_width(d, K_AUTO);
	Kelement_set_height(c, K_AUTO);
	Kelement_set_height(d, K_AUTO);
	root = host(grid, 400, 300);
	/* 400 - 100 pixel - 50 auto = 250 shared 1:2 */
	CHECK_NEAR(Kgrid_get_column_width(grid, 0), 100);
	CHECK_NEAR(Kgrid_get_column_width(grid, 1), 50);
	CHECK_NEAR(Kgrid_get_column_width(grid, 2), 250.0f / 3);
	CHECK_NEAR(Kgrid_get_column_width(grid, 3), 500.0f / 3);
	CHECK_NEAR(Kgrid_get_row_height(grid, 0), 30);
	CHECK_NEAR(Kgrid_get_row_height(grid, 1), 270);
	CHECK_NEAR(Kelement_get_bounds(b).x, 100);
	CHECK_NEAR(Kelement_get_bounds(c).x, 150);
	CHECK_NEAR(Kelement_get_bounds(c).width, 250.0f / 3);
	CHECK_NEAR(Kelement_get_bounds(d).x, 150 + 250.0f / 3);
	CHECK_NEAR(Kelement_get_bounds(c).y, 30);
	(void)a;
	Kroot_destroy(root);
}

static void test_grid_limits_and_span(void) {
	Kid grid = Kgrid_create(), a, root;
	Kgrid_add_column(grid, K_GRID_STAR, 1);
	Kgrid_add_column(grid, K_GRID_STAR, 1);
	Kgrid_set_column_limits(grid, 0, 0, 100);
	a = Kgrid_place(grid, Kborder_create(), 0, 0);
	Kgrid_set_column_span(a, 2);
	Kelement_set_horizontal_alignment(a, K_HALIGN_STRETCH);
	root = host(grid, 400, 100);
	/* the first column is held at 100, the second takes what is left */
	CHECK_NEAR(Kgrid_get_column_width(grid, 0), 100);
	CHECK_NEAR(Kgrid_get_column_width(grid, 1), 300);
	CHECK_NEAR(Kelement_get_bounds(a).width, 400);
	Kroot_destroy(root);
}

static void test_grid_without_definitions(void) {
	Kid grid = Kgrid_create(), a = Kgrid_place(grid, box(40, 40), 0, 0), root;
	Kgrid_place(grid, box(10, 10), 5, 5);
	root = host(grid, 200, 200);
	CHECK_NEAR(Kelement_get_bounds(a).x, 80);
	Kroot_destroy(root);
}

static void test_grid_in_auto_size(void) {
	/* a grid under a horizontal stack has unlimited width: stars become their content */
	Kid s = Kstack_panel_create(K_HORIZONTAL), grid = Kpanel_add(s, Kgrid_create()), root;
	Kgrid_add_column(grid, K_GRID_STAR, 1);
	Kgrid_add_column(grid, K_GRID_AUTO, 0);
	Kgrid_place(grid, box(70, 10), 0, 0);
	Kgrid_place(grid, box(30, 10), 1, 0);
	root = host(s, 400, 100);
	CHECK_NEAR(Kelement_get_desired_size(grid).width, 100);
	Kroot_destroy(root);
}

static void test_canvas_and_zindex(void) {
	Kid canvas = Kcanvas_create(), a = Kcanvas_place(canvas, box(20, 20), 30, 40), b = Kcanvas_place(canvas, box(20, 20), 35, 45), root;
	/* a Border without a brush does not catch the pointer */
	Kborder_set_background(a, Kcolor_rgba(1, 0, 0, 1));
	Kborder_set_background(b, Kcolor_rgba(0, 1, 0, 1));
	Kpanel_set_zindex(a, 1);
	root = host(canvas, 200, 200);
	CHECK_NEAR(Kelement_get_bounds(a).x, 30);
	CHECK_NEAR(Kelement_get_bounds(a).y, 40);
	/* the one with the higher ZIndex is on top, though it was added first */
	CHECK(Kelement_hit_test(canvas, (KPoint){ 40, 50 }) == a);
	Kpanel_set_zindex(a, 0);
	CHECK(Kelement_hit_test(canvas, (KPoint){ 40, 50 }) == b);
	Kroot_destroy(root);
}

static void test_wrap_panel(void) {
	Kid w = Kwrap_panel_create(K_HORIZONTAL), a, c, root;
	a = Kpanel_add(w, box(100, 20));
	Kpanel_add(w, box(100, 30));
	c = Kpanel_add(w, box(100, 10));
	root = host(w, 250, 400);
	/* two fit in 250, the third starts a new line under the tallest of the first */
	CHECK_NEAR(Kelement_get_bounds(a).y, 5); /* 20 high in a 30 high line: centered */
	CHECK_NEAR(Kelement_get_bounds(c).x, 0);
	CHECK_NEAR(Kelement_get_bounds(c).y, 30);
	Kwrap_panel_set_item_size(w, 80, 25);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(c).x, 150); /* 100 wide centered in an 80 slot */
	Kroot_destroy(root);
}

static void test_border(void) {
	Kid b = Kborder_create(), c = box(50, 20), root;
	Kborder_set_child(b, c);
	Kborder_set_border_thickness(b, Kthickness_uniform(3));
	Kborder_set_padding(b, Kthickness(10, 5, 10, 5));
	Kelement_set_horizontal_alignment(b, K_HALIGN_LEFT);
	Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
	root = host(b, 300, 300);
	CHECK_NEAR(Kelement_get_actual_width(b), 50 + 26);
	CHECK_NEAR(Kelement_get_actual_height(b), 20 + 16);
	CHECK_NEAR(Kelement_get_bounds(c).x, 13);
	CHECK_NEAR(Kelement_get_bounds(c).y, 8);
	Kroot_destroy(root);
}

static void test_viewbox(void) {
	Kid v = Kviewbox_create(), root;
	Kviewbox_set_child(v, box(100, 50));
	root = host(v, 300, 300);
	/* uniform: 100 x 50 grown to the width of 300 */
	Kelement_set_vertical_alignment(v, K_VALIGN_TOP);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_actual_width(v), 300);
	CHECK_NEAR(Kelement_get_actual_height(v), 150);
	Kviewbox_set_stretch_direction(v, K_STRETCH_DOWN_ONLY);
	Kelement_set_horizontal_alignment(v, K_HALIGN_LEFT);
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_actual_width(v), 100);
	Kroot_destroy(root);
}

static void test_text_wrapping_measures_taller(void) {
	Kid t = Ktext_block_create("one two three four five six seven eight nine ten"), s = Kstack_panel_create(K_VERTICAL), root;
	Kpanel_add(s, t);
	root = host(s, 160, 400);
	{
		float one_line = Kelement_get_actual_height(t);
		Ktext_block_set_text_wrapping(t, K_WRAP);
		Kroot_update_layout(root);
		CHECK(Kelement_get_actual_height(t) > one_line * 2);
		CHECK(Kelement_get_actual_width(t) <= 160.5f);
	}
	Kroot_destroy(root);
}

static void test_scroll_viewer_extent(void) {
	Kid sv = Kscroll_viewer_create(), s = Kstack_panel_create(K_VERTICAL), root;
	int i;
	for (i = 0; i < 10; i++) Kpanel_add(s, box(50, 100));
	Kscroll_viewer_set_content(sv, s);
	root = host(sv, 200, 300);
	CHECK_NEAR(Kscroll_viewer_get_extent_height(sv), 1000);
	CHECK_NEAR(Kscroll_viewer_get_viewport_height(sv), 300);
	Kscroll_viewer_scroll_to_vertical_offset(sv, 5000);
	CHECK_NEAR(Kscroll_viewer_get_vertical_offset(sv), 700); /* clamped to extent - viewport */
	Kroot_update_layout(root);
	CHECK_NEAR(Kelement_get_bounds(s).y, -700);
	Kroot_destroy(root);
}

static void test_font_change_remeasures_content(void) {
	Kid b = Kbutton_create("OK"), s = Kstack_panel_create(K_VERTICAL), root;
	float before;
	Kpanel_add(s, b);
	Kelement_set_vertical_alignment(b, K_VALIGN_TOP);
	root = host(s, 300, 300);
	before = Kelement_get_desired_size(b).height;
	/* the text inside the button inherits the size: its cached measure must not survive */
	Kcontrol_set_font_size(b, 60);
	Kroot_update_layout(root);
	CHECK(Kelement_get_desired_size(b).height > before + 20);
	CHECK(Kelement_get_actual_height(b) > before + 20);
	Kroot_destroy(root);
}

int main(void) {
	CHECK(Kinit());
	RUN(test_stack_vertical);
	RUN(test_stack_horizontal);
	RUN(test_collapsed_takes_no_room);
	RUN(test_alignment_and_limits);
	RUN(test_grid_star_sizing);
	RUN(test_grid_limits_and_span);
	RUN(test_grid_without_definitions);
	RUN(test_grid_in_auto_size);
	RUN(test_canvas_and_zindex);
	RUN(test_wrap_panel);
	RUN(test_border);
	RUN(test_viewbox);
	RUN(test_text_wrapping_measures_taller);
	RUN(test_scroll_viewer_extent);
	RUN(test_font_change_remeasures_content);
	Kshutdown();
	return test_finish("test_layout");
}
