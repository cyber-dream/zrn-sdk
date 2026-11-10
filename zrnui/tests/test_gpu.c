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
	Zelement_set_enabled(Zpanel_add(row, Zbutton_create("disabled")), false);
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zcheck_box_create("checked")), true);
	Ztoggle_button_set_is_checked(Zpanel_add(stack, Zradio_button_create("radio", "g")), true);
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

/* The edge of a filled triangle is not covered by the anti-aliasing of the fragment shader (rects and rounded rects are), so the
 * pixels between the shape and the background exist only when the framebuffer itself is multisampled. */
static void test_msaa_edges(void) {
	const Zid root = zst_test_root(100, 100);
	Zroot_set_renderer(root, Z_RENDERER_GPU);
	const Zid stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	const Zid icon = Zpanel_add(stack, Zpath_icon_create("M 0,0 L 90,0 L 31,71 Z"));
	Zelement_set_width(icon, 90);
	Zelement_set_height(icon, 80);
	ZFrame frame;
	if (!Zroot_render(root, &frame)) {
		printf("  no GPU renderer: the MSAA check is skipped\n");
		Zroot_destroy(root);
		return;
	}
	/* the inside of the triangle and the background, read from the picture; the edge is what lies between the two */
	const uint8_t* const inside = frame.pixels + (size_t)10 * (size_t)frame.stride + (size_t)10 * 4;
	const uint8_t* const outside = frame.pixels + (size_t)95 * (size_t)frame.stride + (size_t)95 * 4;
	const int inside_luma = (inside[0] + inside[1] + inside[2]) / 3;
	const int outside_luma = (outside[0] + outside[1] + outside[2]) / 3;
	const int low = inside_luma < outside_luma ? inside_luma : outside_luma;
	const int high = inside_luma < outside_luma ? outside_luma : inside_luma;
	bool b_seen[256] = {};
	int mixed = 0;
	for (int y = 0; y < frame.height; y++) {
		for (int x = 0; x < frame.width; x++) {
			const uint8_t* const px = frame.pixels + (size_t)y * (size_t)frame.stride + (size_t)x * 4;
			const int luma = (px[0] + px[1] + px[2]) / 3;
			if (luma > low + 6 && luma < high - 6) {
				mixed++;
				b_seen[luma] = true;
			}
		}
	}
	printf("  inside luma %d, outside luma %d\n", inside_luma, outside_luma);
	CHECK(high - low > 40);
	int levels = 0;
	for (int i = 0; i < 256; i++) levels += b_seen[i] ? 1 : 0;
	printf("  triangle edge: %d mixed pixels, %d distinct levels\n", mixed, levels);
	CHECK(mixed > 40);
	CHECK(levels >= 3);
	Zroot_destroy(root);
}

int main(void) {
	Zid soft, gpu;
	ZFrame fs, fg;
	double mean, bad;
	char path[512];
	if (!Zinit()) return 1;
	soft = zst_test_root(480, 800);
	Zroot_set_renderer(soft, Z_RENDERER_SOFTWARE);
	build(soft);
	gpu = zst_test_root(480, 800);
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
	RUN(test_msaa_edges);
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
