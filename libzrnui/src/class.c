// SPDX-License-Identifier: MIT
/* The classes of nodes: one per kind. A kind that has nothing of its own is a plain node. */
#include "zui-internal.h"
#include "widgets/widgets.h"
extern const struct zui_class zui_list_class;

static const struct zui_class plain = { .name = "node", .size = sizeof(struct zui_node) };

static void field_destroy(zui_node *n) { suggest_free(n); }
static const struct zui_class field = { .name = "field", .size = sizeof(struct zui_field), .destroy = field_destroy };
const struct zui_class zui_scroll_class = { .name = "scroll", .size = sizeof(struct zui_scroll), .scrolls = true };
static const struct zui_class pivot_item = { .name = "pivot item", .size = sizeof(struct zui_scroll), .scrolls = true };
static const struct zui_class page = { .name = "page", .size = sizeof(struct zui_page) };
static const struct zui_class pivot = { .name = "pivot", .size = sizeof(struct zui_pivot) };

const struct zui_class *zui_class_of(int kind) {
	switch (kind) {
	case ZK_PAGE: return &page;
	case ZK_SCROLL: return &zui_scroll_class;
	case ZK_HUBSEC: return &zui_hubsec_class;
	case ZK_CHECK: return &zui_check_class;
	case ZK_RADIO: return &zui_radio_class;
	case ZK_RING: return &zui_ring_class;
	case ZK_TILES: return &zui_tiles_class;
	case ZK_PIVOT_ITEM: return &pivot_item;
	case ZK_PIVOT: return &pivot;
	case ZK_FIELD: return &field;
	case ZK_LIST: return &zui_list_class;
	case ZK_HUB: return &zui_hub_class;
	case ZK_TILE: return &zui_tile_class;
	case ZK_PICKER: return &zui_picker_class;
	default: return &plain;
	}
}
