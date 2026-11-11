/**
 * zrnapp - the window and the event loop on a real compositor: the test opens a window for a root, lets it run for a
 * moment (a few dozen frames, the root is invalidated by a running animation), ends the loop from a signal and
 * checks that Zwindow_run closed normally and used the renderer that was asked for.
 * ZRNAPP_TEST_EXPECT is the name that Zroot_get_renderer_name must have afterwards ("gles2" or "software").
 */

#define _POSIX_C_SOURCE 200809L

#include <zrnapp.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int g_checks, g_failures;

#define CHECK(condition) do { \
	g_checks++; \
	if (!(condition)) { g_failures++; printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); } \
} while (0)

static void on_alarm(int signal_number) {
	(void)signal_number;
	Zwindow_quit();
}

int main(void) {
	if (!Zinit()) return 1;
	const Zid root = Zroot_create();
	const Zid stack = Zstack_panel_create(Z_VERTICAL);
	Zroot_set_visual(root, stack);
	Zpanel_add(stack, Ztext_block_create("zrnapp window test"));
	const Zid bar = Zpanel_add(stack, Zprogress_bar_create());
	Zprogress_bar_set_is_indeterminate(bar, true);   /* keeps the root painting */

	signal(SIGALRM, on_alarm);
	alarm(2);
	const int ok = Zwindow_run(root, "zrnapp-test");
	CHECK(ok);
	if (!ok) printf("  Zwindow_run failed: %s\n", Zget_error_message());

	const char* const expected = getenv("ZRNAPP_TEST_EXPECT");
	if (expected) {
		const char* const actual = Zroot_get_renderer_name(root);
		printf("  renderer: %s (expected %s)\n", actual, expected);
		CHECK(actual && !strcmp(actual, expected));
	}
	int width = 0, height = 0;
	CHECK(Zroot_get_size(root, &width, &height));
	CHECK(width > 0 && height > 0);   /* the compositor gave the root a size */

	Zroot_destroy(root);
	Zshutdown();
	printf("test_window: %d checks, %d failed\n", g_checks, g_failures);
	return g_failures ? 1 : 0;
}
