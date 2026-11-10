/**
 * zrnui - controls/panels.c
 * Panel, StackPanel, VirtualizingStackPanel, WrapPanel, Grid, Canvas,
 * Border and Viewbox.
 */

#include "../internal.h"


/* ============================================================================
 *  Panel
 * ============================================================================ */

static int panel_hit_self(zrn_element_t* e) { return ZST_AS(zrn_panel_t, e)->has_background; }

static void panel_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_panel_t* const panel = ZST_AS(zrn_panel_t, e);
	if (panel->has_background) zrn_fill_rect(p, zrn_local_rect(e), panel->background);
}

static ZSize panel_measure(zrn_element_t* e, const ZSize available) { return zrn_measure_children_max(e, available); }

static ZSize panel_arrange(zrn_element_t* const e, const ZSize final) {
	int i;
	for (i = 0; i < e->child_count; i++) zrn_arrange(e->children[i], zrn_rect(0, 0, final.width, final.height));
	return final;
}

zrn_class_t zrn_class_panel = {
	.base = &zrn_class_element, .name = "Panel", .id = Z_CLASS_PANEL, .size = sizeof(zrn_panel_t), .abstract_class = 1,
	.measure = panel_measure, .arrange = panel_arrange, .paint = panel_paint, .hit_self = panel_hit_self
};

static zrn_element_t* get_panel(const Zid id) { return zrn_get(id, Z_CLASS_PANEL); }

Zid Zpanel_add(const Zid panel, const Zid child) { return Zpanel_insert(panel, -1, child); }

Zid Zpanel_insert(const Zid panel, const int index, const Zid child) {
	zrn_element_t* const p = get_panel(panel);
	zrn_element_t* const c = zrn_get(child, Z_CLASS_ELEMENT);
	if (!p || !c) return Z_INVALID_ID;
	zrn_add_child(p, c, index);
	return c->parent == p ? child : Z_INVALID_ID;
}

void Zpanel_remove(const Zid panel, const Zid child) {
	zrn_element_t* const p = get_panel(panel);
	zrn_element_t* c = zrn_get(child, Z_CLASS_ELEMENT);
	if (p && c && c->parent == p) zrn_remove_child(p, c);
}

void Zpanel_clear(const Zid panel) {
	zrn_element_t* const p = get_panel(panel);
	if (p) zrn_clear_children(p);
}

int Zpanel_get_count(const Zid panel) {
	const zrn_element_t* const p = get_panel(panel);
	return p ? p->child_count : 0;
}

Zid Zpanel_get_child(const Zid panel, const int index) {
	const zrn_element_t* const p = get_panel(panel);
	return p && index >= 0 && index < p->child_count ? p->children[index]->id : Z_INVALID_ID;
}

void Zpanel_set_background(const Zid panel, const ZColor color) {
	zrn_element_t* const p = get_panel(panel);
	if (!p) return;
	ZST_AS(zrn_panel_t, p)->background = color;
	ZST_AS(zrn_panel_t, p)->has_background = 1;
	zrn_changed_paint(p);
}

void Zpanel_set_zindex(const Zid child, const int zindex) {
	zrn_element_t* c = zrn_get(child, Z_CLASS_ELEMENT);
	if (!c) return;
	c->zindex = zindex;
	zrn_changed_paint(c);
}


/* ============================================================================
 *  StackPanel
 * ============================================================================ */

static ZSize stack_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_stack_panel_t* const s = ZST_AS(zrn_stack_panel_t, e);
	ZSize total = { 0, 0 };
	int i, vertical = s->orientation == Z_VERTICAL;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* c = e->children[i];
		zrn_measure(c, vertical ? zrn_size(available.width, Z_INFINITE) : zrn_size(Z_INFINITE, available.height));
		if (c->visibility == Z_COLLAPSED) continue;
		if (vertical) {
			total.height += c->desired.height;
			total.width = zrn_maxf(total.width, c->desired.width);
		} else {
			total.width += c->desired.width;
			total.height = zrn_maxf(total.height, c->desired.height);
		}
	}
	return total;
}

static ZSize stack_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_stack_panel_t* const s = ZST_AS(zrn_stack_panel_t, e);
	float pos = 0;
	int i, vertical = s->orientation == Z_VERTICAL;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* c = e->children[i];
		if (c->visibility == Z_COLLAPSED) continue;
		if (vertical) {
			zrn_arrange(c, zrn_rect(0, pos, final.width, c->desired.height));
			pos += c->desired.height;
		} else {
			zrn_arrange(c, zrn_rect(pos, 0, c->desired.width, final.height));
			pos += c->desired.width;
		}
	}
	return final;
}

static void stack_init(zrn_element_t* e) { ZST_AS(zrn_stack_panel_t, e)->orientation = Z_VERTICAL; }

zrn_class_t zrn_class_stack_panel = {
	.base = &zrn_class_panel, .name = "StackPanel", .id = Z_CLASS_STACK_PANEL, .size = sizeof(zrn_stack_panel_t),
	.init = stack_init, .measure = stack_measure, .arrange = stack_arrange
};

static void vstack_init(zrn_element_t* e) { ZST_AS(zrn_stack_panel_t, e)->virtualizing = 1; }

zrn_class_t zrn_class_virtualizing_stack_panel = {
	.base = &zrn_class_stack_panel, .name = "VirtualizingStackPanel", .id = Z_CLASS_VIRTUALIZING_STACK_PANEL, .size = sizeof(zrn_stack_panel_t),
	.init = vstack_init
};

static void items_stack_init(zrn_element_t* e) { ZST_AS(zrn_stack_panel_t, e)->virtualizing = 1; }

zrn_class_t zrn_class_items_stack_panel = {
	.base = &zrn_class_stack_panel, .name = "ItemsStackPanel", .id = Z_CLASS_ITEMS_STACK_PANEL, .size = sizeof(zrn_stack_panel_t),
	.init = items_stack_init
};

Zid Zitems_stack_panel_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_ITEMS_STACK_PANEL);
	if (!e) return Z_INVALID_ID;
	ZST_AS(zrn_stack_panel_t, e)->orientation = orientation;
	return e->id;
}

Zid Zstack_panel_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_STACK_PANEL);
	if (!e) return Z_INVALID_ID;
	ZST_AS(zrn_stack_panel_t, e)->orientation = orientation;
	return e->id;
}

Zid Zvirtualizing_stack_panel_create(void) {
	zrn_element_t* const e = zrn_create(Z_CLASS_VIRTUALIZING_STACK_PANEL);
	return e ? e->id : Z_INVALID_ID;
}

void Zstack_panel_set_orientation(const Zid id, const ZOrientation orientation) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_STACK_PANEL);
	if (!e) return;
	ZST_AS(zrn_stack_panel_t, e)->orientation = orientation;
	zrn_changed_layout(e);
	zrn_items_panel_changed(e);
}

ZOrientation Zstack_panel_get_orientation(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_STACK_PANEL);
	return e ? ZST_AS(zrn_stack_panel_t, e)->orientation : Z_VERTICAL;
}


/* ============================================================================
 *  WrapPanel
 * ============================================================================ */

static ZSize wrap_item_size(const zrn_wrap_panel_t* const w, const zrn_element_t* const c) {
	return zrn_size(w->item_width != Z_AUTO ? w->item_width : c->desired.width, w->item_height != Z_AUTO ? w->item_height : c->desired.height);
}

/* Lays the children out in lines no longer than `limit`; when `place` the children are arranged. Returns the extent. */
static ZSize wrap_flow(zrn_element_t* const e, const float limit, const int place) {
	zrn_wrap_panel_t* w = ZST_AS(zrn_wrap_panel_t, e);
	int horizontal = w->orientation == Z_HORIZONTAL, i;
	float pos = 0, line_extent = 0, offset = 0, max_pos = 0;
	int line_start = 0;
	for (i = 0; i <= e->child_count; i++) {
		ZSize s = { 0, 0 };
		float along, across;
		zrn_element_t* c = i < e->child_count ? e->children[i] : nullptr;
		if (c) {
			if (c->visibility == Z_COLLAPSED) continue;
			s = wrap_item_size(w, c);
			along = horizontal ? s.width : s.height;
			across = horizontal ? s.height : s.width;
		} else {
			along = 0;
			across = 0;
		}
		if (!c || (pos > 0 && pos + along > limit + 0.01f)) {
			if (place) {
				int j;
				float p2 = 0;
				for (j = line_start; j < i; j++) {
					zrn_element_t* d = e->children[j];
					ZSize ds;
					if (d->visibility == Z_COLLAPSED) continue;
					ds = wrap_item_size(w, d);
					if (horizontal) {
						zrn_arrange(d, zrn_rect(p2, offset, ds.width, line_extent));
						p2 += ds.width;
					} else {
						zrn_arrange(d, zrn_rect(offset, p2, line_extent, ds.height));
						p2 += ds.height;
					}
				}
			}
			max_pos = zrn_maxf(max_pos, pos);
			offset += line_extent;
			pos = 0;
			line_extent = 0;
			line_start = i;
			if (!c) break;
		}
		pos += along;
		line_extent = zrn_maxf(line_extent, across);
	}
	return horizontal ? zrn_size(max_pos, offset) : zrn_size(offset, max_pos);
}

static ZSize wrap_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_wrap_panel_t* const w = ZST_AS(zrn_wrap_panel_t, e);
	int i;
	for (i = 0; i < e->child_count; i++) {
		const ZSize s = zrn_size(w->item_width != Z_AUTO ? w->item_width : available.width, w->item_height != Z_AUTO ? w->item_height : available.height);
		zrn_measure(e->children[i], s);
	}
	return wrap_flow(e, w->orientation == Z_HORIZONTAL ? available.width : available.height, 0);
}

static ZSize wrap_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_wrap_panel_t* const w = ZST_AS(zrn_wrap_panel_t, e);
	wrap_flow(e, w->orientation == Z_HORIZONTAL ? final.width : final.height, 1);
	return final;
}

static void wrap_init(zrn_element_t* const e) {
	zrn_wrap_panel_t* const w = ZST_AS(zrn_wrap_panel_t, e);
	w->orientation = Z_HORIZONTAL;
	w->item_width = Z_AUTO;
	w->item_height = Z_AUTO;
}

zrn_class_t zrn_class_wrap_panel = {
	.base = &zrn_class_panel, .name = "WrapPanel", .id = Z_CLASS_WRAP_PANEL, .size = sizeof(zrn_wrap_panel_t),
	.init = wrap_init, .measure = wrap_measure, .arrange = wrap_arrange
};

Zid Zwrap_panel_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_WRAP_PANEL);
	if (!e) return Z_INVALID_ID;
	ZST_AS(zrn_wrap_panel_t, e)->orientation = orientation;
	return e->id;
}

void Zwrap_panel_set_orientation(const Zid id, const ZOrientation orientation) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_WRAP_PANEL);
	if (!e) return;
	ZST_AS(zrn_wrap_panel_t, e)->orientation = orientation;
	zrn_changed_layout(e);
}

void Zwrap_panel_set_item_size(const Zid id, const float item_width, const float item_height) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_WRAP_PANEL);
	if (!e) return;
	ZST_AS(zrn_wrap_panel_t, e)->item_width = item_width;
	ZST_AS(zrn_wrap_panel_t, e)->item_height = item_height;
	zrn_changed_layout(e);
}


/* ============================================================================
 *  ItemsWrapGrid
 * ============================================================================ */

#define WG(e) ZST_AS(zrn_items_wrap_grid_t, e)

static int wg_is_header(const zrn_element_t* c) { return zrn_is_a(c, Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM); }

/* The size of a cell: ItemWidth by ItemHeight, or the size of the first item for what is not set. Measures that item. */
static void wg_cell(zrn_element_t* const e, const ZSize room, float* const cw, float* const ch) {
	const zrn_items_wrap_grid_t* g = WG(e);
	int i;
	*cw = g->item_width != Z_AUTO ? g->item_width : 0;
	*ch = g->item_height != Z_AUTO ? g->item_height : 0;
	if (g->item_width != Z_AUTO && g->item_height != Z_AUTO) return;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* const c = e->children[i];
		if (c->visibility == Z_COLLAPSED || wg_is_header(c)) continue;
		zrn_measure(c, zrn_size(g->item_width != Z_AUTO ? g->item_width : room.width, g->item_height != Z_AUTO ? g->item_height : room.height));
		if (g->item_width == Z_AUTO) *cw = c->desired.width;
		if (g->item_height == Z_AUTO) *ch = c->desired.height;
		return;
	}
}

static int wg_per_line(const zrn_items_wrap_grid_t* const g, const float along_room, const float cell_along) {
	int n = 0x7fffffff;
	if (cell_along > 0 && along_room < Z_INFINITE * 0.5f) n = zrn_maxi(1, (int)floorf((along_room + 0.01f) / cell_along));
	if (g->max_rows_or_columns > 0) n = zrn_mini(n, g->max_rows_or_columns);
	return n;
}

/* Puts the cells in lines along the orientation. Returns the size taken; with `place` the items are arranged. */
static ZSize wg_flow(zrn_element_t* const e, const float along_room, const float cw, const float ch, const int per_line, const int place) {
	zrn_items_wrap_grid_t* g = WG(e);
	int horizontal = g->orientation == Z_HORIZONTAL, in_line = 0, i;
	float cell_along = horizontal ? cw : ch, cell_across = horizontal ? ch : cw;
	float across = 0, max_along = 0;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* c = e->children[i];
		if (c->visibility == Z_COLLAPSED) continue;
		if (wg_is_header(c)) {
			float header_across = horizontal ? c->desired.height : c->desired.width;
			float header_along = along_room < Z_INFINITE * 0.5f ? along_room : (horizontal ? c->desired.width : c->desired.height);
			if (in_line) {
				across += cell_across;
				max_along = zrn_maxf(max_along, (float)in_line * cell_along);
				in_line = 0;
			}
			if (place) zrn_arrange(c, horizontal ? zrn_rect(0, across, header_along, header_across) : zrn_rect(across, 0, header_across, header_along));
			across += header_across;
			continue;
		}
		if (in_line >= per_line) {
			across += cell_across;
			max_along = zrn_maxf(max_along, (float)in_line * cell_along);
			in_line = 0;
		}
		if (place) {
			const float along = (float)in_line * cell_along;
			zrn_arrange(c, horizontal ? zrn_rect(along, across, cw, ch) : zrn_rect(across, along, cw, ch));
		}
		in_line++;
	}
	if (in_line) {
		across += cell_across;
		max_along = zrn_maxf(max_along, (float)in_line * cell_along);
	}
	return horizontal ? zrn_size(max_along, across) : zrn_size(across, max_along);
}

static ZSize wg_measure(zrn_element_t* const e, const ZSize available) {
	zrn_items_wrap_grid_t* const g = WG(e);
	int horizontal = g->orientation == Z_HORIZONTAL, per_line, i;
	float cw, ch, along = horizontal ? available.width : available.height;
	wg_cell(e, available, &cw, &ch);
	per_line = wg_per_line(g, along, horizontal ? cw : ch);
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* const c = e->children[i];
		if (wg_is_header(c)) zrn_measure(c, horizontal ? zrn_size(zrn_minf(available.width, Z_INFINITE * 0.5f), Z_INFINITE) : zrn_size(Z_INFINITE, available.height));
		else zrn_measure(c, zrn_size(cw, ch));
	}
	return wg_flow(e, along, cw, ch, per_line, 0);
}

static ZSize wg_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_items_wrap_grid_t* const g = WG(e);
	const int horizontal = g->orientation == Z_HORIZONTAL;
	float cw, ch, along = horizontal ? final.width : final.height;
	wg_cell(e, final, &cw, &ch);
	wg_flow(e, along, cw, ch, wg_per_line(g, along, horizontal ? cw : ch), 1);
	return final;
}

static void wg_init(zrn_element_t* const e) {
	zrn_items_wrap_grid_t* const g = WG(e);
	g->orientation = Z_HORIZONTAL;
	g->max_rows_or_columns = -1;
	g->item_width = Z_AUTO;
	g->item_height = Z_AUTO;
}

zrn_class_t zrn_class_items_wrap_grid = {
	.base = &zrn_class_panel, .name = "ItemsWrapGrid", .id = Z_CLASS_ITEMS_WRAP_GRID, .size = sizeof(zrn_items_wrap_grid_t),
	.init = wg_init, .measure = wg_measure, .arrange = wg_arrange
};

Zid Zitems_wrap_grid_create(const ZOrientation orientation) {
	zrn_element_t* const e = zrn_create(Z_CLASS_ITEMS_WRAP_GRID);
	if (!e) return Z_INVALID_ID;
	WG(e)->orientation = orientation;
	return e->id;
}

static zrn_items_wrap_grid_t* get_wg(const Zid id) { return WG(zrn_get(id, Z_CLASS_ITEMS_WRAP_GRID)); }

void Zitems_wrap_grid_set_orientation(const Zid id, const ZOrientation orientation) {
	zrn_items_wrap_grid_t* const g = get_wg(id);
	if (!g) return;
	g->orientation = orientation;
	zrn_changed_layout(&g->base.base);
	zrn_items_panel_changed(&g->base.base);
}

ZOrientation Zitems_wrap_grid_get_orientation(Zid id) { const zrn_items_wrap_grid_t* g = get_wg(id); return g ? g->orientation : Z_HORIZONTAL; }

void Zitems_wrap_grid_set_maximum_rows_or_columns(const Zid id, const int maximum) {
	zrn_items_wrap_grid_t* const g = get_wg(id);
	if (!g) return;
	g->max_rows_or_columns = maximum > 0 ? maximum : -1;
	zrn_changed_layout(&g->base.base);
}

int Zitems_wrap_grid_get_maximum_rows_or_columns(Zid id) { const zrn_items_wrap_grid_t* g = get_wg(id); return g ? g->max_rows_or_columns : -1; }

void Zitems_wrap_grid_set_item_width(const Zid id, const float item_width) {
	zrn_items_wrap_grid_t* const g = get_wg(id);
	if (!g) return;
	g->item_width = item_width;
	zrn_changed_layout(&g->base.base);
}

void Zitems_wrap_grid_set_item_height(const Zid id, const float item_height) {
	zrn_items_wrap_grid_t* const g = get_wg(id);
	if (!g) return;
	g->item_height = item_height;
	zrn_changed_layout(&g->base.base);
}

float Zitems_wrap_grid_get_item_width(Zid id) { const zrn_items_wrap_grid_t* g = get_wg(id); return g ? g->item_width : Z_AUTO; }
float Zitems_wrap_grid_get_item_height(Zid id) { const zrn_items_wrap_grid_t* g = get_wg(id); return g ? g->item_height : Z_AUTO; }


/* ============================================================================
 *  Grid
 * ============================================================================ */

static int grid_add_def(zrn_grid_def_t** list, int* const count, const ZGridUnitType unit, const float value) {
	zrn_grid_def_t* more;
	if (*count >= 256) return -1;
	more = (zrn_grid_def_t*)realloc(*list, (size_t)(*count + 1) * sizeof **list);
	if (!more) return -1;
	*list = more;
	more[*count].unit = unit;
	more[*count].value = unit == Z_GRID_STAR && value <= 0 ? 1.0f : value;
	more[*count].min = 0;
	more[*count].max = Z_INFINITE;
	more[*count].size = 0;
	return (*count)++;
}

typedef struct grid_axis {
	zrn_grid_def_t* defs;
	int count;
	/** The implicit single star definition when the grid has none. */
	zrn_grid_def_t implicit_def;
} grid_axis_t;

static void grid_axis_get(zrn_grid_def_t* const defs, const int count, grid_axis_t* const axis) {
	if (count > 0) {
		axis->defs = defs;
		axis->count = count;
	} else {
		axis->implicit_def.unit = Z_GRID_STAR;
		axis->implicit_def.value = 1;
		axis->implicit_def.min = 0;
		axis->implicit_def.max = Z_INFINITE;
		axis->implicit_def.size = 0;
		axis->defs = &axis->implicit_def;
		axis->count = 1;
	}
}

static void span_range(const int index, const int span, const int count, int* const first, int* const last) {
	*first = zrn_maxi(0, zrn_mini(index, count - 1));
	*last = zrn_mini(count - 1, *first + zrn_maxi(1, span) - 1);
}

typedef struct grid_cell {
	zrn_element_t* child;
	int c0, c1, r0, r1;
	int star_col, star_row, auto_col, auto_row;
} grid_cell_t;

/* How much room a measured child may take along an axis: the sizes it spans, infinite if it spans an auto or an unresolved star. */
static float cell_room(const zrn_grid_def_t* const defs, const int first, const int last, const int resolved) {
	float sum = 0;
	int i;
	for (i = first; i <= last; i++) {
		if (defs[i].unit == Z_GRID_AUTO) return Z_INFINITE;
		if (defs[i].unit == Z_GRID_STAR && !resolved) return Z_INFINITE;
		sum += defs[i].size;
	}
	return sum;
}

static void grid_learn(zrn_grid_def_t* const defs, const int first, const int last, const float need) {
	/* a child that spans one definition gives it its size; over several, the missing room goes to the last auto one */
	float have = 0;
	int i, target = -1;
	for (i = first; i <= last; i++) {
		have += defs[i].size;
		if (defs[i].unit == Z_GRID_AUTO) target = i;
	}
	if (target >= 0 && need > have) defs[target].size += need - have;
}

static void grid_resolve_star(zrn_grid_def_t* defs, int count, const float available, const float* content) {
	float fixed = 0, weights = 0, remaining;
	int i, pass;
	char frozen[256];
	if (count > 256) count = 256;
	for (i = 0; i < count; i++) {
		frozen[i] = 0;
		if (defs[i].unit == Z_GRID_STAR) weights += defs[i].value;
		else fixed += defs[i].size;
	}
	if (weights <= 0) return;
	if (available >= Z_INFINITE * 0.5f) {
		/* unlimited room: a star definition is as big as the content that spans it, in proportion to its weight */
		float ratio = 0;
		for (i = 0; i < count; i++) if (defs[i].unit == Z_GRID_STAR) ratio = zrn_maxf(ratio, content[i] / defs[i].value);
		for (i = 0; i < count; i++) if (defs[i].unit == Z_GRID_STAR) defs[i].size = zrn_clampf(ratio * defs[i].value, defs[i].min, defs[i].max);
		return;
	}
	remaining = zrn_maxf(0, available - fixed);
	/* definitions held by their min / max take their value and the rest share what is left */
	for (pass = 0; pass < count; pass++) {
		float w = 0, share_total = remaining;
		int changed = 0;
		for (i = 0; i < count; i++) {
			if (defs[i].unit != Z_GRID_STAR) continue;
			if (frozen[i]) share_total -= defs[i].size;
			else w += defs[i].value;
		}
		if (w <= 0) break;
		for (i = 0; i < count; i++) {
			float s;
			if (defs[i].unit != Z_GRID_STAR || frozen[i]) continue;
			s = share_total * defs[i].value / w;
			if (s < defs[i].min) { defs[i].size = defs[i].min; frozen[i] = 1; changed = 1; }
			else if (s > defs[i].max) { defs[i].size = defs[i].max; frozen[i] = 1; changed = 1; }
			else defs[i].size = s;
		}
		if (!changed) break;
	}
}

static void grid_prepare(zrn_grid_t* const g, grid_axis_t* const cols, grid_axis_t* const rows) {
	int i;
	grid_axis_get(g->columns, g->column_count, cols);
	grid_axis_get(g->rows, g->row_count, rows);
	for (i = 0; i < cols->count; i++) cols->defs[i].size = cols->defs[i].unit == Z_GRID_PIXEL ? zrn_clampf(cols->defs[i].value, cols->defs[i].min, cols->defs[i].max) : 0;
	for (i = 0; i < rows->count; i++) rows->defs[i].size = rows->defs[i].unit == Z_GRID_PIXEL ? zrn_clampf(rows->defs[i].value, rows->defs[i].min, rows->defs[i].max) : 0;
}

static void grid_classify(zrn_element_t* const child, const grid_axis_t* const cols, const grid_axis_t* const rows, grid_cell_t* const cell) {
	int i;
	cell->child = child;
	span_range(child->grid_column, child->grid_column_span, cols->count, &cell->c0, &cell->c1);
	span_range(child->grid_row, child->grid_row_span, rows->count, &cell->r0, &cell->r1);
	cell->star_col = cell->star_row = cell->auto_col = cell->auto_row = 0;
	for (i = cell->c0; i <= cell->c1; i++) {
		if (cols->defs[i].unit == Z_GRID_STAR) cell->star_col = 1;
		if (cols->defs[i].unit == Z_GRID_AUTO) cell->auto_col = 1;
	}
	for (i = cell->r0; i <= cell->r1; i++) {
		if (rows->defs[i].unit == Z_GRID_STAR) cell->star_row = 1;
		if (rows->defs[i].unit == Z_GRID_AUTO) cell->auto_row = 1;
	}
}

static void grid_clamp_auto(grid_axis_t* const axis) {
	int i;
	for (i = 0; i < axis->count; i++) if (axis->defs[i].unit == Z_GRID_AUTO) axis->defs[i].size = zrn_clampf(axis->defs[i].size, axis->defs[i].min, axis->defs[i].max);
}

static ZSize grid_measure(zrn_element_t* const e, const ZSize available) {
	zrn_grid_t* g = ZST_AS(zrn_grid_t, e);
	grid_axis_t cols, rows;
	grid_cell_t* cells = nullptr;
	float col_content[256] = {}, row_content[256] = {};
	int n = 0, i, group;
	ZSize result = { 0, 0 };
	const int inf_w = available.width >= Z_INFINITE * 0.5f, inf_h = available.height >= Z_INFINITE * 0.5f;
	grid_prepare(g, &cols, &rows);
	if (e->child_count > 0) cells = (grid_cell_t*)calloc((size_t)e->child_count, sizeof *cells);
	for (i = 0; i < e->child_count && cells; i++) {
		if (e->children[i]->visibility == Z_COLLAPSED) { zrn_measure(e->children[i], zrn_size(0, 0)); continue; }
		grid_classify(e->children[i], &cols, &rows, &cells[n++]);
	}
	/* four groups: no star at all; star rows only; star columns only; both.
	 * An axis with unlimited room has no stars to share: its star definitions follow their content, so
	 * for the grouping its cells count as not star. */
	for (group = 0; group < 4; group++) {
		if (group == 2 && !inf_w) grid_resolve_star(cols.defs, cols.count, available.width, col_content);
		if (group == 3 && !inf_h) grid_resolve_star(rows.defs, rows.count, available.height, row_content);
		for (i = 0; i < n; i++) {
			grid_cell_t* c = &cells[i];
			const int sc = c->star_col && !inf_w, sr = c->star_row && !inf_h;
			const int mine = sc ? (sr ? 3 : 2) : (sr ? 1 : 0);
			float room_w, room_h;
			if (mine != group) continue;
			room_w = cell_room(cols.defs, c->c0, c->c1, group >= 2);
			room_h = cell_room(rows.defs, c->r0, c->r1, group >= 3);
			if (c->star_col && inf_w) room_w = Z_INFINITE;
			if (c->star_row && inf_h) room_h = Z_INFINITE;
			zrn_measure(c->child, zrn_size(room_w, room_h));
			if (!sc) {
				if (c->star_col) { if (c->c0 == c->c1) col_content[c->c0] = zrn_maxf(col_content[c->c0], c->child->desired.width); }
				else grid_learn(cols.defs, c->c0, c->c1, c->child->desired.width);
			}
			if (!sr) {
				if (c->star_row) { if (c->r0 == c->r1) row_content[c->r0] = zrn_maxf(row_content[c->r0], c->child->desired.height); }
				else grid_learn(rows.defs, c->r0, c->r1, c->child->desired.height);
			}
			if (sc && !sr && c->auto_row) grid_learn(rows.defs, c->r0, c->r1, c->child->desired.height);
			if (sr && !sc && c->auto_col) grid_learn(cols.defs, c->c0, c->c1, c->child->desired.width);
		}
		if (group == 0 || group == 1) grid_clamp_auto(&cols);
		if (group == 0 || group == 2) grid_clamp_auto(&rows);
	}
	if (inf_w || n == 0) grid_resolve_star(cols.defs, cols.count, available.width, col_content);
	if (inf_h || n == 0) grid_resolve_star(rows.defs, rows.count, available.height, row_content);
	for (i = 0; i < cols.count; i++) result.width += cols.defs[i].size;
	for (i = 0; i < rows.count; i++) result.height += rows.defs[i].size;
	/* the sizes of the star definitions of an unlimited axis stay as measured */
	free(cells);
	return result;
}

static ZSize grid_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	grid_axis_t cols, rows;
	float content_cols[256] = {}, content_rows[256] = {}, col_pos[257], row_pos[257];
	int i;
	grid_prepare(g, &cols, &rows);
	/* auto sizes are the ones of the last Measure: recompute them from the children */
	for (i = 0; i < e->child_count; i++) {
		grid_cell_t cell;
		zrn_element_t* const c = e->children[i];
		if (c->visibility == Z_COLLAPSED) continue;
		grid_classify(c, &cols, &rows, &cell);
		if (!cell.star_col) grid_learn(cols.defs, cell.c0, cell.c1, c->desired.width);
		if (!cell.star_row) grid_learn(rows.defs, cell.r0, cell.r1, c->desired.height);
	}
	grid_clamp_auto(&cols);
	grid_clamp_auto(&rows);
	grid_resolve_star(cols.defs, cols.count, final.width, content_cols);
	grid_resolve_star(rows.defs, rows.count, final.height, content_rows);
	col_pos[0] = row_pos[0] = 0;
	for (i = 0; i < cols.count && i < 256; i++) col_pos[i + 1] = col_pos[i] + cols.defs[i].size;
	for (i = 0; i < rows.count && i < 256; i++) row_pos[i + 1] = row_pos[i] + rows.defs[i].size;
	for (i = 0; i < e->child_count; i++) {
		grid_cell_t cell;
		zrn_element_t* const c = e->children[i];
		if (c->visibility == Z_COLLAPSED) continue;
		grid_classify(c, &cols, &rows, &cell);
		zrn_arrange(c, zrn_rect(col_pos[cell.c0], row_pos[cell.r0], col_pos[cell.c1 + 1] - col_pos[cell.c0], row_pos[cell.r1 + 1] - row_pos[cell.r0]));
	}
	/* publish the sizes when the grid has real definitions: they are already in g->columns / g->rows */
	return final;
}

static void grid_destroy(zrn_element_t* const e) {
	const zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	free(g->columns);
	free(g->rows);
}

zrn_class_t zrn_class_grid = {
	.base = &zrn_class_panel, .name = "Grid", .id = Z_CLASS_GRID, .size = sizeof(zrn_grid_t),
	.destroy = grid_destroy, .measure = grid_measure, .arrange = grid_arrange
};

Zid Zgrid_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_GRID);
	return e ? e->id : Z_INVALID_ID;
}

int Zgrid_add_column(const Zid id, const ZGridUnitType unit, const float value) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	int index;
	if (!e) return -1;
	index = grid_add_def(&ZST_AS(zrn_grid_t, e)->columns, &ZST_AS(zrn_grid_t, e)->column_count, unit, value);
	zrn_changed_layout(e);
	return index;
}

int Zgrid_add_row(const Zid id, const ZGridUnitType unit, const float value) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	int index;
	if (!e) return -1;
	index = grid_add_def(&ZST_AS(zrn_grid_t, e)->rows, &ZST_AS(zrn_grid_t, e)->row_count, unit, value);
	zrn_changed_layout(e);
	return index;
}

void Zgrid_set_column_limits(const Zid id, const int column, const float min_width, const float max_width) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	const zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	if (!e || column < 0 || column >= g->column_count) return;
	g->columns[column].min = min_width;
	g->columns[column].max = max_width;
	zrn_changed_layout(e);
}

void Zgrid_set_row_limits(const Zid id, const int row, const float min_height, const float max_height) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	const zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	if (!e || row < 0 || row >= g->row_count) return;
	g->rows[row].min = min_height;
	g->rows[row].max = max_height;
	zrn_changed_layout(e);
}

int Zgrid_get_column_count(Zid id) { zrn_element_t* e = zrn_get(id, Z_CLASS_GRID); return e ? ZST_AS(zrn_grid_t, e)->column_count : 0; }
int Zgrid_get_row_count(Zid id) { zrn_element_t* e = zrn_get(id, Z_CLASS_GRID); return e ? ZST_AS(zrn_grid_t, e)->row_count : 0; }

float Zgrid_get_column_width(const Zid id, const int column) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	const zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	if (!e) return 0;
	if (g->column_count == 0) return column == 0 ? e->rect.width : 0;
	return column >= 0 && column < g->column_count ? g->columns[column].size : 0;
}

float Zgrid_get_row_height(const Zid id, const int row) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_GRID);
	const zrn_grid_t* const g = ZST_AS(zrn_grid_t, e);
	if (!e) return 0;
	if (g->row_count == 0) return row == 0 ? e->rect.height : 0;
	return row >= 0 && row < g->row_count ? g->rows[row].size : 0;
}

#define ATTACHED(name, field, minimum) \
	void name(Zid id, int value) { \
		zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); \
		if (!e) return; \
		e->field = value < minimum ? minimum : value; \
		if (e->parent) zrn_changed_layout(e->parent); \
	}

ATTACHED(Zgrid_set_row, grid_row, 0)
ATTACHED(Zgrid_set_column, grid_column, 0)
ATTACHED(Zgrid_set_row_span, grid_row_span, 1)
ATTACHED(Zgrid_set_column_span, grid_column_span, 1)

int Zgrid_get_row(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->grid_row : 0; }
int Zgrid_get_column(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->grid_column : 0; }

Zid Zgrid_place(const Zid grid, const Zid child, const int column, const int row) {
	Zgrid_set_column(child, column);
	Zgrid_set_row(child, row);
	return Zpanel_add(grid, child);
}


/* ============================================================================
 *  Canvas
 * ============================================================================ */

static ZSize canvas_measure(zrn_element_t* const e, const ZSize available) {
	int i;
	ZST_UNUSED(available);
	for (i = 0; i < e->child_count; i++) zrn_measure(e->children[i], zrn_size(Z_INFINITE, Z_INFINITE));
	return zrn_size(0, 0);
}

static ZSize canvas_arrange(zrn_element_t* const e, const ZSize final) {
	int i;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* const c = e->children[i];
		const float x = c->canvas_left == Z_AUTO ? 0 : c->canvas_left, y = c->canvas_top == Z_AUTO ? 0 : c->canvas_top;
		zrn_arrange(c, zrn_rect(x, y, c->desired.width, c->desired.height));
	}
	return final;
}

zrn_class_t zrn_class_canvas = {
	.base = &zrn_class_panel, .name = "Canvas", .id = Z_CLASS_CANVAS, .size = sizeof(zrn_panel_t),
	.measure = canvas_measure, .arrange = canvas_arrange
};

Zid Zcanvas_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_CANVAS);
	return e ? e->id : Z_INVALID_ID;
}

void Zcanvas_set_left(const Zid id, const float left) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->canvas_left = left;
	if (e->parent) zrn_changed_layout(e->parent);
}

void Zcanvas_set_top(const Zid id, const float top) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->canvas_top = top;
	if (e->parent) zrn_changed_layout(e->parent);
}

Zid Zcanvas_place(const Zid canvas, const Zid child, const float left, const float top) {
	Zcanvas_set_left(child, left);
	Zcanvas_set_top(child, top);
	return Zpanel_add(canvas, child);
}


/* ============================================================================
 *  Border
 * ============================================================================ */

static ZThickness border_inset(const zrn_border_t* const b) {
	ZThickness t;
	t.left = b->border_thickness.left + b->padding.left;
	t.top = b->border_thickness.top + b->padding.top;
	t.right = b->border_thickness.right + b->padding.right;
	t.bottom = b->border_thickness.bottom + b->padding.bottom;
	return t;
}

static ZSize border_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_border_t* const b = ZST_AS(zrn_border_t, e);
	const ZThickness inset = border_inset(b);
	ZSize content = { 0, 0 };
	if (e->child_count > 0) {
		zrn_measure(e->children[0], zrn_size(zrn_maxf(0, available.width - zrn_thickness_h(inset)), zrn_maxf(0, available.height - zrn_thickness_v(inset))));
		content = e->children[0]->desired;
	}
	return zrn_size(content.width + zrn_thickness_h(inset), content.height + zrn_thickness_v(inset));
}

static ZSize border_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_border_t* const b = ZST_AS(zrn_border_t, e);
	const ZThickness inset = border_inset(b);
	if (e->child_count > 0) zrn_arrange(e->children[0], zrn_rect(inset.left, inset.top, zrn_maxf(0, final.width - zrn_thickness_h(inset)), zrn_maxf(0, final.height - zrn_thickness_v(inset))));
	return final;
}

static int border_has_radius(const zrn_border_t* const b) {
	return b->corner_radius.top_left > 0 || b->corner_radius.top_right > 0 || b->corner_radius.bottom_right > 0 || b->corner_radius.bottom_left > 0;
}

static void border_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_border_t* const b = ZST_AS(zrn_border_t, e);
	const ZRect r = zrn_local_rect(e);
	if (border_has_radius(b)) {
		if (b->has_border_brush) zrn_fill_rrect(p, r, b->corner_radius, b->border_brush);
		if (b->has_background) {
			const ZRect inner = zrn_rect(r.x + b->border_thickness.left, r.y + b->border_thickness.top, r.width - zrn_thickness_h(b->border_thickness), r.height - zrn_thickness_v(b->border_thickness));
			const ZCornerRadius ir = { zrn_maxf(0, b->corner_radius.top_left - b->border_thickness.left), zrn_maxf(0, b->corner_radius.top_right - b->border_thickness.right), zrn_maxf(0, b->corner_radius.bottom_right - b->border_thickness.right), zrn_maxf(0, b->corner_radius.bottom_left - b->border_thickness.left) };
			zrn_fill_rrect(p, inner, ir, b->background);
		}
		return;
	}
	if (b->has_background) zrn_fill_rect(p, r, b->background);
	if (b->has_border_brush) zrn_stroke_rect(p, r, b->border_thickness, b->border_brush);
}

static int border_hit_self(zrn_element_t* const e) {
	const zrn_border_t* const b = ZST_AS(zrn_border_t, e);
	return b->has_background || b->has_border_brush;
}

static void border_child_removed(zrn_element_t* e, zrn_element_t* child) { ZST_UNUSED(e); ZST_UNUSED(child); }

zrn_class_t zrn_class_border = {
	.base = &zrn_class_element, .name = "Border", .id = Z_CLASS_BORDER, .size = sizeof(zrn_border_t),
	.measure = border_measure, .arrange = border_arrange, .paint = border_paint, .hit_self = border_hit_self,
	.child_removed = border_child_removed
};

Zid Zborder_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_BORDER);
	return e ? e->id : Z_INVALID_ID;
}

void Zborder_set_child(const Zid id, const Zid child) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	zrn_element_t* const c = child != Z_INVALID_ID ? zrn_get(child, Z_CLASS_ELEMENT) : nullptr;
	zrn_element_t* slot;
	if (!e || (child != Z_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : nullptr;
	zrn_set_only_child(e, &slot, c);
}

Zid Zborder_get_child(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	return e && e->child_count ? e->children[0]->id : Z_INVALID_ID;
}

void Zborder_set_background(const Zid id, const ZColor color) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	if (!e) return;
	zrn_unbind(e, ZST_BP_BORDER_BACKGROUND);
	ZST_AS(zrn_border_t, e)->background = color;
	ZST_AS(zrn_border_t, e)->has_background = 1;
	zrn_changed_paint(e);
}

void Zborder_set_border_brush(const Zid id, const ZColor color) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	if (!e) return;
	zrn_unbind(e, ZST_BP_BORDER_BORDER_BRUSH);
	ZST_AS(zrn_border_t, e)->border_brush = color;
	ZST_AS(zrn_border_t, e)->has_border_brush = 1;
	zrn_changed_paint(e);
}

void Zborder_set_border_thickness(const Zid id, const ZThickness thickness) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	if (!e) return;
	zrn_unbind(e, ZST_BP_BORDER_BORDER_THICKNESS);
	ZST_AS(zrn_border_t, e)->border_thickness = thickness;
	zrn_changed_layout(e);
}

void Zborder_set_corner_radius(const Zid id, const ZCornerRadius radius) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	if (!e) return;
	ZST_AS(zrn_border_t, e)->corner_radius = radius;
	zrn_changed_paint(e);
}

void Zborder_set_padding(const Zid id, const ZThickness padding) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_BORDER);
	if (!e) return;
	zrn_unbind(e, ZST_BP_BORDER_PADDING);
	ZST_AS(zrn_border_t, e)->padding = padding;
	zrn_changed_layout(e);
}


/* ============================================================================
 *  Viewbox
 *  The child is laid out at its natural size and painted scaled. It is
 *  not hit tested: a Viewbox shows, it does not interact.
 * ============================================================================ */

static void viewbox_scale(zrn_viewbox_t* const v, const ZSize content, const ZSize room, float* const sx, float* const sy) {
	*sx = *sy = 1.0f;
	if (v->stretch == Z_STRETCH_NONE || content.width <= 0 || content.height <= 0) return;
	if (room.width < Z_INFINITE * 0.5f) *sx = room.width / content.width;
	if (room.height < Z_INFINITE * 0.5f) *sy = room.height / content.height;
	if (room.width >= Z_INFINITE * 0.5f) *sx = *sy;
	if (room.height >= Z_INFINITE * 0.5f) *sy = *sx;
	if (v->stretch == Z_STRETCH_UNIFORM) *sx = *sy = zrn_minf(*sx, *sy);
	else if (v->stretch == Z_STRETCH_UNIFORM_TO_FILL) *sx = *sy = zrn_maxf(*sx, *sy);
	if (v->direction == Z_STRETCH_UP_ONLY) { *sx = zrn_maxf(*sx, 1); *sy = zrn_maxf(*sy, 1); }
	else if (v->direction == Z_STRETCH_DOWN_ONLY) { *sx = zrn_minf(*sx, 1); *sy = zrn_minf(*sy, 1); }
}

static ZSize viewbox_measure(zrn_element_t* const e, const ZSize available) {
	zrn_viewbox_t* const v = ZST_AS(zrn_viewbox_t, e);
	float sx, sy;
	zrn_element_t* c;
	if (!e->child_count) return zrn_size(0, 0);
	c = e->children[0];
	zrn_measure(c, zrn_size(Z_INFINITE, Z_INFINITE));
	viewbox_scale(v, c->desired, available, &sx, &sy);
	return zrn_size(c->desired.width * sx, c->desired.height * sy);
}

static ZSize viewbox_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_viewbox_t* const v = ZST_AS(zrn_viewbox_t, e);
	zrn_element_t* c;
	if (!e->child_count) return final;
	c = e->children[0];
	c->is_popup_child = 1;
	zrn_arrange(c, zrn_rect(0, 0, c->desired.width, c->desired.height));
	viewbox_scale(v, c->desired, final, &v->scale_x, &v->scale_y);
	return final;
}

static void viewbox_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_viewbox_t* const v = ZST_AS(zrn_viewbox_t, e);
	zrn_element_t* c;
	float scale = p->scale, tx = p->tx, ty = p->ty, w, h;
	if (!e->child_count) return;
	c = e->children[0];
	w = c->desired.width * v->scale_x;
	h = c->desired.height * v->scale_y;
	p->tx += (e->rect.width - w) * 0.5f * scale;
	p->ty += (e->rect.height - h) * 0.5f * scale;
	p->scale = scale * zrn_minf(v->scale_x, v->scale_y);
	zrn_paint_element(c, p, c->root);
	p->scale = scale;
	p->tx = tx;
	p->ty = ty;
}

static void viewbox_init(zrn_element_t* const e) {
	zrn_viewbox_t* const v = ZST_AS(zrn_viewbox_t, e);
	v->stretch = Z_STRETCH_UNIFORM;
	v->direction = Z_STRETCH_BOTH;
	v->scale_x = v->scale_y = 1.0f;
}

zrn_class_t zrn_class_viewbox = {
	.base = &zrn_class_element, .name = "Viewbox", .id = Z_CLASS_VIEWBOX, .size = sizeof(zrn_viewbox_t),
	.init = viewbox_init, .measure = viewbox_measure, .arrange = viewbox_arrange, .paint = viewbox_paint
};

Zid Zviewbox_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_VIEWBOX);
	return e ? e->id : Z_INVALID_ID;
}

void Zviewbox_set_child(const Zid id, const Zid child) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_VIEWBOX);
	zrn_element_t* const c = child != Z_INVALID_ID ? zrn_get(child, Z_CLASS_ELEMENT) : nullptr;
	zrn_element_t* slot;
	if (!e || (child != Z_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : nullptr;
	zrn_set_only_child(e, &slot, c);
}

void Zviewbox_set_stretch(const Zid id, const ZStretch stretch) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_VIEWBOX);
	if (!e) return;
	ZST_AS(zrn_viewbox_t, e)->stretch = stretch;
	zrn_changed_layout(e);
}

void Zviewbox_set_stretch_direction(const Zid id, const ZStretchDirection direction) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_VIEWBOX);
	if (!e) return;
	ZST_AS(zrn_viewbox_t, e)->direction = direction;
	zrn_changed_layout(e);
}
