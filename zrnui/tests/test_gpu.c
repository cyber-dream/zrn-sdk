/**
 * The OpenGL ES 2 renderer against the software painter: the same page drawn by both must agree
 * to within the difference of their anti-aliasing. Without a GPU (no EGL) the test is skipped.
 */

#include "test.h"

#include <stdlib.h>

static Zid build(Zid root) {
	Zid stack = Zstack_panel_create(Z_VERTICAL), row, sl, pb, cb, lb, t;
	Zelement_set_margin(stack, Zthickness(12, 0, 12, 0));
	Zroot_set_visual(root, stack);
	t = Zpanel_add(stack, Ztext_block_create("ZRNUI gpu"));
	Ztext_block_set_style(t, Z_TEXT_STYLE_PHONE_ACCENT);
	t = Zpanel_add(stack, Ztext_block_create("Привет, мир"));
	Ztext_block_set_style(t, Z_TEXT_STYLE_PHONE_TITLE2);
	row = Zpanel_add(stack, Zstack_panel_create(Z_HORIZONTAL));
	Zpanel_add(row, Zbutton_create("button"));
	Zelement_set_enabled(Zpanel_add(row, Zbutton_create("disabled")), Z_FALSE);
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zcheck_box_create("checked")), Z_TRUE);
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zradio_button_create("radio", "g")), Z_TRUE);
	sl = Zpanel_add(stack, Zslider_create(Z_HORIZONTAL));
	Zrange_base_set_value(sl, 4);
	pb = Zpanel_add(stack, Zprogress_bar_create());
	Zrange_base_set_value(pb, 60);
	Zpanel_add(stack, Ztext_box_create("text box"));
	cb = Zpanel_add(stack, Zcombo_box_create());
	Zitems_control_add_text(cb, "first");
	Zselector_set_selected_index(cb, 0);
	lb = Zpanel_add(stack, Zlist_box_create());
	Zitems_control_add_text(lb, "list item one");
	Zitems_control_add_text(lb, "list item two");
	Zselector_set_selected_index(lb, 0);
	{
		Zid border = Zpanel_add(stack, Zborder_create());
		Zelement_set_height(border, 40);
		Zborder_set_background(border, Zcolor_rgba(0.8f, 0.2f, 0.1f, 1));
		Zborder_set_corner_radius(border, (ZCornerRadius){ 12, 12, 12, 12 });
	}
	return stack;
}

static void compare(const char* name, ZFrame a, ZFrame b, double* mean, double* bad) {
	long n = (long)a.width * a.height, i, over = 0;
	double sum = 0;
	for (i = 0; i < n; i++) {
		const uint8_t* p = a.pixels + (size_t)(i / a.width) * (size_t)a.stride + (size_t)(i % a.width) * 4;
		const uint8_t* q = b.pixels + (size_t)(i / b.width) * (size_t)b.stride + (size_t)(i % b.width) * 4;
		int d = abs(p[0] - q[0]) + abs(p[1] - q[1]) + abs(p[2] - q[2]);
		sum += d / 3.0;
		if (d > 96) over++;
	}
	*mean = sum / (double)n;
	*bad = (double)over / (double)n;
	printf("  %s: mean difference %.3f / 255, %.3f%% of the pixels differ a lot\n", name, *mean, *bad * 100.0);
}

int main(void) {
	Zid soft, gpu;
	ZFrame fs, fg;
	double mean, bad;
	char path[512];
	if (!Zinit()) return 1;
	soft = Zroot_create(480, 800);
	Zroot_set_renderer(soft, Z_RENDERER_SOFTWARE);
	build(soft);
	gpu = Zroot_create(480, 800);
	Zroot_set_renderer(gpu, Z_RENDERER_GPU);
	build(gpu);
	Zroot_render(soft, &fs);
	if (!Zroot_render(gpu, &fg)) {
		printf("test_gpu: no OpenGL ES 2 context here (%s): skipped\n", Zget_error_message());
		Zroot_destroy(soft);
		Zroot_destroy(gpu);
		Zshutdown();
		return 0;
	}
	CHECK_STR(Zroot_get_renderer_name(gpu), "gles2");
	compare("controls", fs, fg, &mean, &bad);
	CHECK(mean < 6.0);
	CHECK(bad < 0.02);
	snprintf(path, sizeof path, "%s/gpu_controls.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(gpu, path));
	snprintf(path, sizeof path, "%s/gpu_controls_software.png", ZRNUI_TEST_OUTPUT_DIR);
	CHECK(test_save_png(soft, path));
	Zroot_destroy(soft);
	Zroot_destroy(gpu);
	Zshutdown();
	return test_finish("test_gpu");
}
