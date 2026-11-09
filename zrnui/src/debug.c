/**
 * zrnui - debug.c
 * The debug overlay: the bounds of every element of the tree, drawn over the
 * paint of the element itself, in its own painter state. The box uses the local
 * rect the content and the clip use, so the transforms (tilt, the pivot
 * transitions, the scales of a Viewbox) apply to it the same as to what is
 * painted there: the box is where the drawing is, in every frame of an
 * animation, and the hit testing reads the same arranged rects through
 * zrn_to_root when nothing is transformed.
 *
 * Zroot_set_debug turns it on: Z_DEBUG_BOUNDS outlines every element,
 * Z_DEBUG_LABELS writes the class name over each outline (implies BOUNDS).
 */

#include "internal.h"

#include <stdio.h>
#include <string.h>

static const char* zrn_debug_kind(const zrn_element_t* e) {
	if (e->child_count) return "container";
	if (e->cls->name) return e->cls->name;
	return "?";
}

/** The box of this element over what it and its children drew: the local rect, in the
 * painter state of the element itself (host.c calls it inside the walk). */
void zrn_debug_element(const zrn_element_t* e, zrn_painter_t* p, const zrn_root_t* r) {
	ZColor line;
	float saved_tx = p->tx, saved_ty = p->ty;
	if (!(r->debug_flags & 1) || e->visibility != Z_VISIBLE) return;
	if (e->rect.width <= 0 || e->rect.height <= 0) return;
	/* snap to the device pixel grid: a 1 px stroke that straddles a half pixel reads as a 2 px line */
	p->tx = roundf(p->tx);
	p->ty = roundf(p->ty);
	/* the containers get the accent, the leaves the base */
	line = e->child_count ? r->theme.colors[ZST_TC_ACCENT] : r->theme.colors[ZST_TC_BASE_MID];
	zrn_stroke_rect(p, zrn_rect(0, 0, roundf(e->rect.width), roundf(e->rect.height)), Zthickness_uniform(1), line);
	if ((r->debug_flags & 2) && e->rect.width > 12 && e->rect.height > 9) {
		char label[128];
		zrn_text_layout_t* l;
		zrn_font_t font = { Z_FONT_NORMAL, ZST_FONT_SMALL, 0 };
		snprintf(label, sizeof label, "%s%s%s", zrn_debug_kind(e), e->name ? " " : "", e->name ? e->name : "");
		l = zrn_text_layout_new(font, label, (int)strlen(label), 400.0f, 0, 1, 0);
		if (l) {
			/* the chip and the text in the same coordinates, distorted with the content like everything */
			zrn_fill_rect(p, zrn_rect(0, 0, roundf(l->width + 4), roundf(l->height + 2)), r->theme.colors[ZST_TC_SEMITRANSPARENT]);
			zrn_draw_text(p, l, 2, 1, l->width, Z_TEXT_LEFT,Z_TRIM_NONE, r->theme.colors[ZST_TC_FOREGROUND]);
			zrn_text_layout_free(l);
		}
	}
	p->tx = saved_tx;
	p->ty = saved_ty;
}
