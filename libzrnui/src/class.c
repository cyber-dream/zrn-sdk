// SPDX-License-Identifier: MIT
/* The classes of nodes: one per kind. A kind that has nothing of its own is a plain node. */
#include "zui-internal.h"

static const struct zui_class plain = { .name = "node", .size = sizeof(struct zui_node) };

static const struct zui_class pivot = { .name = "pivot", .size = sizeof(struct zui_pivot) };

const struct zui_class *zui_class_of(int kind) {
	switch (kind) {
	case ZK_PIVOT: return &pivot;
	default: return &plain;
	}
}
