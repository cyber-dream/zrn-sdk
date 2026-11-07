// SPDX-License-Identifier: MIT
/* The classes of nodes: one per kind. A kind that has nothing of its own is a plain node. */
#include "zui-internal.h"
#include "widgets/widgets.h"

static const struct zui_class plain = { .name = "node", .size = sizeof(struct zui_node) };

static void field_destroy(zui_node *n) { suggest_free(n); }
static const struct zui_class field = { .name = "field", .size = sizeof(struct zui_field), .destroy = field_destroy };
static const struct zui_class pivot = { .name = "pivot", .size = sizeof(struct zui_pivot) };

const struct zui_class *zui_class_of(int kind) {
	switch (kind) {
	case ZK_PIVOT: return &pivot;
	case ZK_FIELD: return &field;
	case ZK_HUB: return &zui_hub_class;
	case ZK_TILE: return &zui_tile_class;
	case ZK_PICKER: return &zui_picker_class;
	default: return &plain;
	}
}
