/**
 * zrnapp - the app id of a window: the .desktop name when the program gives it, else the name of the executable; never the title.
 */

#include "app_id.h"

#include <stdio.h>
#include <string.h>

static int g_checks, g_failures;

#define CHECK_STR(actual, expected) do { \
	g_checks++; \
	if (strcmp((actual), (expected)) != 0) { g_failures++; printf("  FAIL %s:%d: \"%s\", expected \"%s\"\n", __FILE__, __LINE__, (actual), (expected)); } \
} while (0)

static void test_explicit_id_wins(void) {
	char id[64];
	zrnapp_choose_app_id("org.example.Notes", "/usr/bin/notes", id, sizeof id);
	CHECK_STR(id, "org.example.Notes");
}

static void test_program_name_is_the_file_name(void) {
	char id[64];
	zrnapp_choose_app_id(nullptr, "/usr/bin/notes", id, sizeof id);
	CHECK_STR(id, "notes");
	zrnapp_choose_app_id("", "/opt/zrn/bin/zrn-demoapp", id, sizeof id);   /* an empty id is no id */
	CHECK_STR(id, "zrn-demoapp");
	zrnapp_choose_app_id(nullptr, "plain", id, sizeof id);
	CHECK_STR(id, "plain");
}

static void test_fallback(void) {
	char id[64];
	zrnapp_choose_app_id(nullptr, nullptr, id, sizeof id);
	CHECK_STR(id, "zrnui");
	zrnapp_choose_app_id(nullptr, "/usr/bin/", id, sizeof id);   /* a path with no file name */
	CHECK_STR(id, "zrnui");
}

static void test_cut_to_fit(void) {
	char id[8];
	zrnapp_choose_app_id("org.example.Notes", nullptr, id, sizeof id);
	CHECK_STR(id, "org.exa");   /* 8 bytes: 7 characters and the NUL */
	char one[1] = { 'x' };
	zrnapp_choose_app_id("abc", nullptr, one, sizeof one);
	CHECK_STR(one, "");
	zrnapp_choose_app_id("abc", nullptr, one, 0);   /* nothing to write into: untouched */
	CHECK_STR(one, "");
}

static void test_running_program_is_known(void) {
	const char* const path = zrnapp_program_path();
	g_checks++;
	if (!path || !*path) { g_failures++; printf("  FAIL: the path of the executable is not known\n"); return; }
	char id[256];
	zrnapp_choose_app_id(nullptr, path, id, sizeof id);
	CHECK_STR(id, "test_app_id");
}

int main(void) {
	test_explicit_id_wins();
	test_program_name_is_the_file_name();
	test_fallback();
	test_cut_to_fit();
	test_running_program_is_known();
	printf("test_app_id: %d checks, %d failed\n", g_checks, g_failures);
	return g_failures ? 1 : 0;
}
