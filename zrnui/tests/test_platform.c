/**
 * The seam to the platform layer (zrnui/platform.h), as a shell that embeds zrnui uses it: the test makes its own OpenGL ES 2
 * context, zrnui draws a root in it through a ZGlTarget, and the pixels agree with the software painter.
 * Without EGL / a GPU the test is skipped.
 */

#include "test.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

static constexpr int SIZE = 160;

static EGLDisplay g_display;
static EGLContext g_context;
static EGLSurface g_surface;

static void make_current(void* const user) {
	(void)user;
	eglMakeCurrent(g_display, g_surface, g_surface, g_context);
}

static bool make_context(void) {
	const PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	g_display = get_platform ? get_platform(EGL_PLATFORM_SURFACELESS_MESA, nullptr, nullptr) : EGL_NO_DISPLAY;
	if (g_display == EGL_NO_DISPLAY) g_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	EGLint major, minor;
	if (g_display == EGL_NO_DISPLAY || !eglInitialize(g_display, &major, &minor)) return false;
	if (!eglBindAPI(EGL_OPENGL_ES_API)) return false;
	const EGLint attribs[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
	EGLConfig config;
	EGLint count = 0;
	if (!eglChooseConfig(g_display, attribs, &config, 1, &count) || count < 1) return false;
	const EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	g_context = eglCreateContext(g_display, config, EGL_NO_CONTEXT, context_attribs);
	const EGLint pbuffer[] = { EGL_WIDTH, SIZE, EGL_HEIGHT, SIZE, EGL_NONE };
	g_surface = eglCreatePbufferSurface(g_display, config, pbuffer);
	if (g_context == EGL_NO_CONTEXT || g_surface == EGL_NO_SURFACE) return false;
	return eglMakeCurrent(g_display, g_surface, g_surface, g_context);
}

static Zid build_page(const Zid root) {
	const Zid stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	const Zid label = Zpanel_add(stack, Ztext_block_create("platform"));
	Ztext_block_set_style(label, Z_TEXT_STYLE_PHONE_TITLE2);
	const Zid border = Zpanel_add(stack, Zborder_create());
	Zelement_set_height(border, 60);
	Zborder_set_background(border, Zcolor_rgba(0.8f, 0.2f, 0.1f, 1));
	Zborder_set_corner_radius(border, (ZCornerRadius){ 8, 8, 8, 8 });
	return stack;
}

static void test_size_and_invalidate(void) {
	const Zid root = zst_test_root(200, 300);
	int width = 0, height = 0;
	CHECK(Zroot_get_size(root, &width, &height));
	CHECK(width == 200 && height == 300);
	Zroot_invalidate(root);
	Zroot_set_density(root, 2.0f);
	CHECK(!Zroot_get_size(Z_INVALID_ID, &width, &height));
	Zroot_destroy(root);
}

static void test_error_report(void) {
	Zreport_error(Z_ERROR_IO, "a platform error");
	CHECK(Zget_error() == Z_ERROR_IO);
	CHECK_STR(Zget_error_message(), "a platform error");
}

static void test_target_draws_like_software(void) {
	ZGlTarget* const target = Zgl_target_create(make_current, nullptr);
	CHECK(target != nullptr);
	if (!target) return;
	const Zid gpu = zst_test_root(SIZE, SIZE);
	const Zid soft = zst_test_root(SIZE, SIZE);
	Zroot_set_renderer(soft, Z_RENDERER_SOFTWARE);
	build_page(gpu);
	build_page(soft);
	CHECK(Zgl_target_paint(gpu, target, SIZE, SIZE));   /* the first frame paints */
	static uint8_t gl_pixels[SIZE * SIZE * 4];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, SIZE, SIZE, GL_RGBA, GL_UNSIGNED_BYTE, gl_pixels);
	ZFrame frame;
	CHECK(Zroot_render(soft, &frame));
	double sum = 0;
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			const uint8_t* const g = gl_pixels + ((size_t)(SIZE - 1 - y) * SIZE + (size_t)x) * 4;   /* GL rows run bottom to top */
			const uint8_t* const s = frame.pixels + (size_t)y * (size_t)frame.stride + (size_t)x * 4;
			sum += (abs(g[0] - s[0]) + abs(g[1] - s[1]) + abs(g[2] - s[2])) / 3.0;
		}
	}
	const double mean = sum / (SIZE * SIZE);
	printf("  target against software: mean difference %.3f / 255\n", mean);
	CHECK(mean < 6.0);
	/* nothing changed: the next frame is not painted again; a new size paints everything */
	CHECK(!Zgl_target_paint(gpu, target, SIZE, SIZE));
	Zroot_invalidate(gpu);
	CHECK(Zgl_target_paint(gpu, target, SIZE, SIZE));
	Zroot_destroy(gpu);
	Zroot_destroy(soft);
	make_current(nullptr);
	Zgl_target_destroy(target);
}

int main(void) {
	if (!Zinit()) return 1;
	RUN(test_size_and_invalidate);
	RUN(test_error_report);
	if (!make_context()) {
		printf("test_platform: no OpenGL ES 2 context here: the GPU target is skipped\n");
		Zshutdown();
		return test_finish("test_platform");
	}
	RUN(test_target_draws_like_software);
	eglMakeCurrent(g_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(g_display, g_surface);
	eglDestroyContext(g_display, g_context);
	eglTerminate(g_display);
	Zshutdown();
	return test_finish("test_platform");
}
