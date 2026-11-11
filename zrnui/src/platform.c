/**
 * zrnui - platform.c
 * The seam to the platform layer (include/zrnui/platform.h): zrnapp or a shell that embeds zrnui.
 */

#include "internal.h"

int Zroot_get_size(const Zid root, int* const width, int* const height) {
	const zrn_root_t* const r = zrn_root_lookup(root);
	if (!r) return false;
	if (width) *width = r->width;
	if (height) *height = r->height;
	return true;
}

void Zroot_invalidate(const Zid root) {
	zrn_root_t* const r = zrn_root_lookup(root);
	if (r) r->paint_dirty = 1;
}

void Zroot_start_intro(const Zid root) {
	zrn_root_t* const r = zrn_root_lookup(root);
	if (r) zrn_root_start_intro(r);
}

void Zroot_report_presenter(const Zid root, const char* const renderer_name, const int is_gpu) {
	zrn_root_t* const r = zrn_root_lookup(root);
	zrn_note_renderer(renderer_name);
	if (r) r->gpu_window = is_gpu != 0;
}

void Zreport_error(const ZError error, const char* const message) {
	zrn_error(error, "%s", message ? message : "");
}

ZGlTarget* Zgl_target_create(void (*const make_current)(void* user), void* const user) {
	return (ZGlTarget*)zrn_gl_create_external(make_current, user);
}

int Zgl_target_paint(const Zid root, ZGlTarget* const target, const int width, const int height) {
	zrn_root_t* const r = zrn_root_lookup(root);
	if (!r || !target) return false;
	zrn_gl_t* const gl = (zrn_gl_t*)target;
	zrn_painter_t p;
	/* the buffer of a new size is blank: everything is drawn again */
	if (zrn_gl_width(gl) != width || zrn_gl_height(gl) != height) r->paint_dirty = 1;
	zrn_gl_begin(gl, &p, width, height, r->scale);
	const int painted = zrn_root_paint(r, &p);
	zrn_gl_end(gl, 0);   /* flushed, not swapped: the buffers are the platform's */
	return painted;
}

void Zgl_target_destroy(ZGlTarget* const target) {
	zrn_gl_destroy((zrn_gl_t*)target);
}
