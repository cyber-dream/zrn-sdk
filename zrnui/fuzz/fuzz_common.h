/**
 * zrnui fuzzers - shared setup: software renderer, no window, errors silenced.
 */

#pragma once

#define _POSIX_C_SOURCE 200809L

#include <zrnui.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void zst_fuzz_quiet(ZError, const char*, void*) {}

/** Once per process: the renderer is the software one so that no GPU is touched, and the errors are not printed. */
static inline void zst_fuzz_start(void) {
	setenv("ZRNUI_RENDERER", "software", 1);
	setenv("ZRNUI_ANIMATIONS", "off", 1);
	(void)Zinit();
	Zset_error_callback(zst_fuzz_quiet, nullptr);
}

/** A NUL-terminated copy of the input, so that the library sees a C string. */
static inline char* zst_fuzz_string(const uint8_t* const data, const size_t size) {
	char* const text = (char*)malloc(size + 1);
	if (!text) return nullptr;
	memcpy(text, data, size);
	text[size] = '\0';
	return text;
}
