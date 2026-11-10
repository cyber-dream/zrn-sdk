/**
 * zrnui fuzzer - theme.ini: arbitrary bytes as the theme file, read again for every input.
 */

#include "fuzz_common.h"

#include <unistd.h>

/* internal: forgets the tables read from theme.ini, so that the next root reads the file again */
extern void zrn_theme_reset(void);

static char g_path[] = "/tmp/zrnui-fuzz-theme-XXXXXX";
static int g_fd = -1;

int LLVMFuzzerInitialize(int*, char***) {
	zst_fuzz_start();
	g_fd = mkstemp(g_path);
	if (g_fd < 0) return 1;
	setenv("ZRNUI_THEME", g_path, 1);
	return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* const data, const size_t size) {
	if (ftruncate(g_fd, 0) != 0 || pwrite(g_fd, data, size, 0) != (ssize_t)size) return 0;
	zrn_theme_reset();
	const Zid root = Zroot_create();
	Zroot_destroy(root);
	return 0;
}
