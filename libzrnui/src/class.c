// SPDX-License-Identifier: MIT
/* The classes of nodes: one per kind. The core's own (column, text, button, list, field, pivot...) are in
 * core.c, the widgets' in their files; here are the kinds that are only a struct, and the map from a kind to
 * its class. */
#include "zui-internal.h"
#include "widgets/widgets.h"

static const struct zui_class plain = { .name = "node", .size = sizeof(struct zui_node) };
static const struct zui_class page = { .name = "page", .size = sizeof(struct zui_page) };

const struct zui_class *zui_class_of(int kind) {
	const struct zui_class *core = zui_core_class(kind);
	if (core) return core;
	switch (kind) {
	case ZK_PAGE: return &page;
	case ZK_HUB: return &zui_hub_class;
	case ZK_HUBSEC: return &zui_hubsec_class;
	case ZK_TILES: return &zui_tiles_class;
	case ZK_TILE: return &zui_tile_class;
	case ZK_PICKER: return &zui_picker_class;
	case ZK_CHECK: return &zui_check_class;
	case ZK_RADIO: return &zui_radio_class;
	case ZK_RING: return &zui_ring_class;
	default: return &plain;
	}
}
