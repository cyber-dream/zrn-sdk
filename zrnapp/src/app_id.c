/**
 * zrnapp - app_id.c
 */

#include "app_id.h"

#include <string.h>
#include <unistd.h>

static const char* base_name(const char* const path) {
	const char* const slash = strrchr(path, '/');
	return slash ? slash + 1 : path;
}

void zrnapp_choose_app_id(const char* const explicit_id, const char* const program, char* const out, const size_t capacity) {
	if (capacity == 0) return;
	const char* chosen = "zrnui";
	if (explicit_id && *explicit_id) chosen = explicit_id;
	else if (program && *base_name(program)) chosen = base_name(program);
	const size_t length = strlen(chosen);
	const size_t n = length < capacity - 1 ? length : capacity - 1;
	memcpy(out, chosen, n);
	out[n] = '\0';
}

const char* zrnapp_program_path(void) {
	static char path[1024];
	const ssize_t n = readlink("/proc/self/exe", path, sizeof path - 1);
	if (n <= 0) return nullptr;
	path[n] = '\0';
	return path;
}
