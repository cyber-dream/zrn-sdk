/**
 * zrnui fuzzer - text: arbitrary bytes (broken UTF-8 included) through TextBlock layout, drawing and a TextBox
 * that is typed into. The first bytes choose the width, the font, the wrapping and the keys.
 */

#include "fuzz_common.h"

int LLVMFuzzerInitialize(int*, char***) {
	zst_fuzz_start();
	return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* const data, size_t size) {
	if (size < 5 || size > 4096) return 0;
	const int width = 40 + data[0] * 2;
	const ZFontFamily family = (ZFontFamily)(data[1] % 5);
	const ZTextWrapping wrapping = data[2] & 1 ? Z_WRAP : Z_NO_WRAP;
	const uint8_t key_count = data[3] & 7;
	char* const text = zst_fuzz_string(data + 4, size - 4);
	if (!text) return 0;

	const Zid root = Zroot_create();
	const Zid stack = Zstack_panel_create(Z_VERTICAL);
	const Zid block = Ztext_block_create(text);
	const Zid box = Ztext_box_create(text);
	Ztext_block_set_font_family(block, family);
	Ztext_block_set_text_wrapping(block, wrapping);
	Ztext_box_set_text_wrapping(box, wrapping);
	Zpanel_add(stack, block);
	Zpanel_add(stack, box);
	Zroot_set_size(root, width, 400);
	Zroot_set_visual(root, stack);
	ZFrame frame;
	(void)Zroot_render(root, &frame);
	(void)Zroot_inject_pointer(root, Z_POINTER_DOWN, 10, 10);
	(void)Zroot_inject_pointer(root, Z_POINTER_UP, 10, 10);
	for (uint8_t i = 0; i < key_count; i++) {
		(void)Zroot_inject_text(root, text);
		(void)Zroot_inject_key(root, (ZKey)(data[4 + i % (size - 4)] % 128), 0, 1);
	}
	(void)Zroot_render(root, &frame);
	Zroot_destroy(root);
	free(text);
	return 0;
}
