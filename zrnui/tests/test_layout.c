/**
 * Measure / Arrange: StackPanel, Grid (pixel, auto, star), WrapPanel,
 * Canvas, Border, Viewbox, margins, alignment, size limits, visibility.
 */

#include "test.h"

static Zid box(float w, float h) {
	Zid b = Zborder_create();
	Zelement_set_width(b, w);
	Zelement_set_height(b, h);
	return b;
}

static Zid host(Zid visual, int w, int h) {
	Zid root = zst_test_root(w, h);
	Zroot_set_visual(root, visual);
	Zroot_update_layout(root);
	return root;
}

static void test_stack_vertical(void) {
	Zid s = Zstack_panel_create(Z_VERTICAL), a = Zpanel_add(s, box(100, 40)), b = Zpanel_add(s, box(60, 30)), root;
	Zelement_set_margin(b, Zthickness(5, 10, 5, 10));
	root = host(s, 300, 400);
	/* children are stacked top to bottom, each as high as it asked, stretched across */
	CHECK_NEAR(Zelement_get_bounds(a).y, 0);
	CHECK_NEAR(Zelement_get_bounds(a).height, 40);
	CHECK_NEAR(Zelement_get_bounds(b).y, 40 + 10);
	CHECK_NEAR(Zelement_get_bounds(b).x, 5 + (300 - 10 - 60) * 0.5f); /* fixed width: centered in the slot */
	CHECK_NEAR(Zelement_get_desired_size(s).height, 40 + 30 + 20);
	CHECK_NEAR(Zelement_get_desired_size(s).width, 100);
	Zroot_destroy(root);
}

static void test_stack_horizontal(void) {
	Zid s = Zstack_panel_create(Z_HORIZONTAL), a = Zpanel_add(s, box(100, 40)), b = Zpanel_add(s, box(60, 30)), root;
	Zelement_set_vertical_alignment(b, Z_VALIGN_BOTTOM);
	root = host(s, 300, 400);
	CHECK_NEAR(Zelement_get_bounds(b).x, 100);
	CHECK_NEAR(Zelement_get_bounds(a).height, 40);
	/* the panel is only as high as its tallest child: stretch fills 400, so the bottom aligned one sits at the panel's bottom */
	CHECK_NEAR(Zelement_get_bounds(b).y + Zelement_get_bounds(b).height, Zelement_get_actual_height(s));
	Zroot_destroy(root);
}

static void test_collapsed_takes_no_room(void) {
	Zid s = Zstack_panel_create(Z_VERTICAL), a = Zpanel_add(s, box(100, 40)), b = Zpanel_add(s, box(100, 30)), c = Zpanel_add(s, box(100, 20)), root;
	Zelement_set_visibility(b, Z_COLLAPSED);
	root = host(s, 300, 400);
	CHECK_NEAR(Zelement_get_bounds(c).y, Zelement_get_bounds(a).y + 40);
	Zelement_set_visibility(b, Z_VISIBLE);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(c).y, 70);
	Zroot_destroy(root);
}

static void test_alignment_and_limits(void) {
	Zid grid = Zgrid_create(), c = Zgrid_place(grid, box(50, 20), 0, 0), root;
	root = host(grid, 200, 100);
	/* a fixed size, stretched alignment: centered */
	CHECK_NEAR(Zelement_get_bounds(c).x, 75);
	CHECK_NEAR(Zelement_get_bounds(c).y, 40);
	Zelement_set_horizontal_alignment(c, Z_HALIGN_RIGHT);
	Zelement_set_vertical_alignment(c, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(c).x, 150);
	CHECK_NEAR(Zelement_get_bounds(c).y, 0);
	Zelement_set_width(c, Z_AUTO);
	Zelement_set_min_size(c, 120, 0);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(c).width, 120);
	Zelement_set_horizontal_alignment(c, Z_HALIGN_STRETCH);
	Zelement_set_max_size(c, 90, Z_INFINITE);
	Zelement_set_min_size(c, 0, 0);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(c).width, 90);
	Zroot_destroy(root);
}

static void test_grid_star_sizing(void) {
	Zid grid = Zgrid_create(), a, b, c, d, root;
	Zgrid_add_column(grid, Z_GRID_PIXEL, 100);
	Zgrid_add_column(grid, Z_GRID_AUTO, 0);
	Zgrid_add_column(grid, Z_GRID_STAR, 1);
	Zgrid_add_column(grid, Z_GRID_STAR, 2);
	Zgrid_add_row(grid, Z_GRID_AUTO, 0);
	Zgrid_add_row(grid, Z_GRID_STAR, 1);
	a = Zgrid_place(grid, box(10, 30), 0, 0);
	b = Zgrid_place(grid, box(50, 10), 1, 0);
	c = Zgrid_place(grid, box(10, 10), 2, 1);
	d = Zgrid_place(grid, box(10, 10), 3, 1);
	Zelement_set_horizontal_alignment(c, Z_HALIGN_STRETCH);
	Zelement_set_width(c, Z_AUTO);
	Zelement_set_width(d, Z_AUTO);
	Zelement_set_height(c, Z_AUTO);
	Zelement_set_height(d, Z_AUTO);
	root = host(grid, 400, 300);
	/* 400 - 100 pixel - 50 auto = 250 shared 1:2 */
	CHECK_NEAR(Zgrid_get_column_width(grid, 0), 100);
	CHECK_NEAR(Zgrid_get_column_width(grid, 1), 50);
	CHECK_NEAR(Zgrid_get_column_width(grid, 2), 250.0f / 3);
	CHECK_NEAR(Zgrid_get_column_width(grid, 3), 500.0f / 3);
	CHECK_NEAR(Zgrid_get_row_height(grid, 0), 30);
	CHECK_NEAR(Zgrid_get_row_height(grid, 1), 270);
	CHECK_NEAR(Zelement_get_bounds(b).x, 100);
	CHECK_NEAR(Zelement_get_bounds(c).x, 150);
	CHECK_NEAR(Zelement_get_bounds(c).width, 250.0f / 3);
	CHECK_NEAR(Zelement_get_bounds(d).x, 150 + 250.0f / 3);
	CHECK_NEAR(Zelement_get_bounds(c).y, 30);
	(void)a;
	Zroot_destroy(root);
}

static void test_grid_limits_and_span(void) {
	Zid grid = Zgrid_create(), a, root;
	Zgrid_add_column(grid, Z_GRID_STAR, 1);
	Zgrid_add_column(grid, Z_GRID_STAR, 1);
	Zgrid_set_column_limits(grid, 0, 0, 100);
	a = Zgrid_place(grid, Zborder_create(), 0, 0);
	Zgrid_set_column_span(a, 2);
	Zelement_set_horizontal_alignment(a, Z_HALIGN_STRETCH);
	root = host(grid, 400, 100);
	/* the first column is held at 100, the second takes what is left */
	CHECK_NEAR(Zgrid_get_column_width(grid, 0), 100);
	CHECK_NEAR(Zgrid_get_column_width(grid, 1), 300);
	CHECK_NEAR(Zelement_get_bounds(a).width, 400);
	Zroot_destroy(root);
}

static void test_grid_without_definitions(void) {
	Zid grid = Zgrid_create(), a = Zgrid_place(grid, box(40, 40), 0, 0), root;
	Zgrid_place(grid, box(10, 10), 5, 5);
	root = host(grid, 200, 200);
	CHECK_NEAR(Zelement_get_bounds(a).x, 80);
	Zroot_destroy(root);
}

static void test_grid_in_auto_size(void) {
	/* a grid under a horizontal stack has unlimited width: stars become their content */
	Zid s = Zstack_panel_create(Z_HORIZONTAL), grid = Zpanel_add(s, Zgrid_create()), root;
	Zgrid_add_column(grid, Z_GRID_STAR, 1);
	Zgrid_add_column(grid, Z_GRID_AUTO, 0);
	Zgrid_place(grid, box(70, 10), 0, 0);
	Zgrid_place(grid, box(30, 10), 1, 0);
	root = host(s, 400, 100);
	CHECK_NEAR(Zelement_get_desired_size(grid).width, 100);
	Zroot_destroy(root);
}

static void test_canvas_and_zindex(void) {
	Zid canvas = Zcanvas_create(), a = Zcanvas_place(canvas, box(20, 20), 30, 40), b = Zcanvas_place(canvas, box(20, 20), 35, 45), root;
	/* a Border without a brush does not catch the pointer */
	Zborder_set_background(a, Zcolor_rgba(1, 0, 0, 1));
	Zborder_set_background(b, Zcolor_rgba(0, 1, 0, 1));
	Zpanel_set_zindex(a, 1);
	root = host(canvas, 200, 200);
	CHECK_NEAR(Zelement_get_bounds(a).x, 30);
	CHECK_NEAR(Zelement_get_bounds(a).y, 40);
	/* the one with the higher ZIndex is on top, though it was added first */
	CHECK(Zelement_hit_test(canvas, (ZPoint){ 40, 50 }) == a);
	Zpanel_set_zindex(a, 0);
	CHECK(Zelement_hit_test(canvas, (ZPoint){ 40, 50 }) == b);
	Zroot_destroy(root);
}

static void test_wrap_panel(void) {
	Zid w = Zwrap_panel_create(Z_HORIZONTAL), a, c, root;
	a = Zpanel_add(w, box(100, 20));
	Zpanel_add(w, box(100, 30));
	c = Zpanel_add(w, box(100, 10));
	root = host(w, 250, 400);
	/* two fit in 250, the third starts a new line under the tallest of the first */
	CHECK_NEAR(Zelement_get_bounds(a).y, 5); /* 20 high in a 30 high line: centered */
	CHECK_NEAR(Zelement_get_bounds(c).x, 0);
	CHECK_NEAR(Zelement_get_bounds(c).y, 30);
	Zwrap_panel_set_item_size(w, 80, 25);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(c).x, 150); /* 100 wide centered in an 80 slot */
	Zroot_destroy(root);
}

static void test_border(void) {
	Zid b = Zborder_create(), c = box(50, 20), root;
	Zborder_set_child(b, c);
	Zborder_set_border_thickness(b, Zthickness_uniform(3));
	Zborder_set_padding(b, Zthickness(10, 5, 10, 5));
	Zelement_set_horizontal_alignment(b, Z_HALIGN_LEFT);
	Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
	root = host(b, 300, 300);
	CHECK_NEAR(Zelement_get_actual_width(b), 50 + 26);
	CHECK_NEAR(Zelement_get_actual_height(b), 20 + 16);
	CHECK_NEAR(Zelement_get_bounds(c).x, 13);
	CHECK_NEAR(Zelement_get_bounds(c).y, 8);
	Zroot_destroy(root);
}

static void test_viewbox(void) {
	Zid v = Zviewbox_create(), root;
	Zviewbox_set_child(v, box(100, 50));
	root = host(v, 300, 300);
	/* uniform: 100 x 50 grown to the width of 300 */
	Zelement_set_vertical_alignment(v, Z_VALIGN_TOP);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_actual_width(v), 300);
	CHECK_NEAR(Zelement_get_actual_height(v), 150);
	Zviewbox_set_stretch_direction(v, Z_STRETCH_DOWN_ONLY);
	Zelement_set_horizontal_alignment(v, Z_HALIGN_LEFT);
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_actual_width(v), 100);
	Zroot_destroy(root);
}

static void test_text_wrapping_measures_taller(void) {
	Zid t = Ztext_block_create("one two three four five six seven eight nine ten"), s = Zstack_panel_create(Z_VERTICAL), root;
	Zpanel_add(s, t);
	root = host(s, 160, 400);
	{
		float one_line = Zelement_get_actual_height(t);
		Ztext_block_set_text_wrapping(t, Z_WRAP);
		Zroot_update_layout(root);
		CHECK(Zelement_get_actual_height(t) > one_line * 2);
		CHECK(Zelement_get_actual_width(t) <= 160.5f);
	}
	Zroot_destroy(root);
}

static void test_scroll_viewer_extent(void) {
	Zid sv = Zscroll_viewer_create(), s = Zstack_panel_create(Z_VERTICAL), root;
	int i;
	for (i = 0; i < 10; i++) Zpanel_add(s, box(50, 100));
	Zscroll_viewer_set_content(sv, s);
	root = host(sv, 200, 300);
	CHECK_NEAR(Zscroll_viewer_get_extent_height(sv), 1000);
	CHECK_NEAR(Zscroll_viewer_get_viewport_height(sv), 300);
	Zscroll_viewer_scroll_to_vertical_offset(sv, 5000);
	CHECK_NEAR(Zscroll_viewer_get_vertical_offset(sv), 700); /* clamped to extent - viewport */
	Zroot_update_layout(root);
	CHECK_NEAR(Zelement_get_bounds(s).y, -700);
	Zroot_destroy(root);
}

static void test_font_change_remeasures_content(void) {
	Zid b = Zbutton_create("OK"), s = Zstack_panel_create(Z_VERTICAL), root;
	float before;
	Zpanel_add(s, b);
	Zelement_set_vertical_alignment(b, Z_VALIGN_TOP);
	root = host(s, 300, 300);
	before = Zelement_get_desired_size(b).height;
	/* the text inside the button inherits the size: its cached measure must not survive */
	Zcontrol_set_font_size(b, 60);
	Zroot_update_layout(root);
	CHECK(Zelement_get_desired_size(b).height > before + 20);
	CHECK(Zelement_get_actual_height(b) > before + 20);
	Zroot_destroy(root);
}

int main(void) {
	CHECK(Zinit());
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
	Zshutdown();
	return test_finish("test_layout");
}
