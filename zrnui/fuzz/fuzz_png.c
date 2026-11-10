/**
 * zrnui fuzzer - image files: arbitrary bytes as a PNG file through Zimage_set_source_file and a render.
 */

#include "fuzz_common.h"

#include <unistd.h>

static char g_path[] = "/tmp/zrnui-fuzz-png-XXXXXX";
static int g_fd = -1;

int LLVMFuzzerInitialize(int*, char***) {
	zst_fuzz_start();
	g_fd = mkstemp(g_path);
	return g_fd < 0 ? 1 : 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* const data, const size_t size) {
	if (ftruncate(g_fd, 0) != 0 || pwrite(g_fd, data, size, 0) != (ssize_t)size) return 0;
	const Zid root = Zroot_create();
	const Zid image = Zimage_create();
	Zroot_set_size(root, 200, 200);
	Zroot_set_visual(root, image);
	(void)Zimage_set_source_file(image, g_path);
	ZFrame frame;
	(void)Zroot_render(root, &frame);
	Zroot_destroy(root);
	return 0;
}
