/**
 * The OpenGL ES 2 renderer against the software painter: the same page drawn by both must agree
 * to within the difference of their anti-aliasing. Without a GPU (no EGL) the test is skipped.
 */

#include "test.h"

#include <stdlib.h>

static Kid build(Kid root) {
	Kid stack = Kstack_panel_create(K_VERTICAL), row, sl, pb, cb, lb, t;
	Kelement_set_margin(stack, Kthickness(12, 0, 12, 0));
	Kroot_set_visual(root, stack);
	t = Kpanel_add(stack, Ktext_block_create("KESTREL gpu"));
	Ktext_block_set_style(t, K_TEXT_STYLE_PHONE_ACCENT);
	t = Kpanel_add(stack, Ktext_block_create("Привет, мир"));
	Ktext_block_set_style(t, K_TEXT_STYLE_PHONE_TITLE2);
	row = Kpanel_add(stack, Kstack_panel_create(K_HORIZONTAL));
	Kpanel_add(row, Kbutton_create("button"));
	Kelement_set_enabled(Kpanel_add(row, Kbutton_create("disabled")), K_FALSE);
	Ktoggle_button_set_is_checked(Kpanel_add(stack, Kcheck_box_create("checked")), K_TRUE);
	Ktoggle_button_set_is_checked(Kpanel_add(stack, Kradio_button_create("radio", "g")), K_TRUE);
	sl = Kpanel_add(stack, Kslider_create(K_HORIZONTAL));
	Krange_base_set_value(sl, 4);
	pb = Kpanel_add(stack, Kprogress_bar_create());
	Krange_base_set_value(pb, 60);
	Kpanel_add(stack, Ktext_box_create("text box"));
	cb = Kpanel_add(stack, Kcombo_box_create());
	Kitems_control_add_text(cb, "first");
	Kselector_set_selected_index(cb, 0);
	lb = Kpanel_add(stack, Klist_box_create());
	Kitems_control_add_text(lb, "list item one");
	Kitems_control_add_text(lb, "list item two");
	Kselector_set_selected_index(lb, 0);
	{
		Kid border = Kpanel_add(stack, Kborder_create());
		Kelement_set_height(border, 40);
		Kborder_set_background(border, Kcolor_rgba(0.8f, 0.2f, 0.1f, 1));
		Kborder_set_corner_radius(border, (KCornerRadius){ 12, 12, 12, 12 });
	}
	return stack;
}

static void compare(const char* name, KFrame a, KFrame b, double* mean, double* bad) {
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
	Kid soft, gpu;
	KFrame fs, fg;
	double mean, bad;
	char path[512];
	if (!Kinit()) return 1;
	soft = Kroot_create(480, 800);
	Kroot_set_renderer(soft, K_RENDERER_SOFTWARE);
	build(soft);
	gpu = Kroot_create(480, 800);
	Kroot_set_renderer(gpu, K_RENDERER_GPU);
	build(gpu);
	Kroot_render(soft, &fs);
	if (!Kroot_render(gpu, &fg)) {
		printf("test_gpu: no OpenGL ES 2 context here (%s): skipped\n", Kget_error_message());
		Kroot_destroy(soft);
		Kroot_destroy(gpu);
		Kshutdown();
		return 0;
	}
	CHECK_STR(Kroot_get_renderer_name(gpu), "gles2");
	compare("controls", fs, fg, &mean, &bad);
	CHECK(mean < 6.0);
	CHECK(bad < 0.02);
	snprintf(path, sizeof path, "%s/gpu_controls.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(gpu, path));
	snprintf(path, sizeof path, "%s/gpu_controls_software.png", KESTREL_TEST_OUTPUT_DIR);
	CHECK(test_save_png(soft, path));
	Kroot_destroy(soft);
	Kroot_destroy(gpu);
	Kshutdown();
	return test_finish("test_gpu");
}
