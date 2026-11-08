/**
 * Kestrel - controls/panels.c
 * Panel, StackPanel, VirtualizingStackPanel, WrapPanel, Grid, Canvas,
 * Border and Viewbox.
 */

#include "../internal.h"


/* ============================================================================
 *  Panel
 * ============================================================================ */

static int panel_hit_self(kst_element_t* e) { return KST_AS(kst_panel_t, e)->has_background; }

static void panel_paint(kst_element_t* e, kst_painter_t* p) {
	kst_panel_t* panel = KST_AS(kst_panel_t, e);
	if (panel->has_background) kst_fill_rect(p, kst_local_rect(e), panel->background);
}

static KSize panel_measure(kst_element_t* e, KSize available) { return kst_measure_children_max(e, available); }

static KSize panel_arrange(kst_element_t* e, KSize final) {
	int i;
	for (i = 0; i < e->child_count; i++) kst_arrange(e->children[i], kst_rect(0, 0, final.width, final.height));
	return final;
}

kst_class_t kst_class_panel = {
	.base = &kst_class_element, .name = "Panel", .id = K_CLASS_PANEL, .size = sizeof(kst_panel_t), .abstract_class = 1,
	.measure = panel_measure, .arrange = panel_arrange, .paint = panel_paint, .hit_self = panel_hit_self
};

static kst_element_t* get_panel(Kid id) { return kst_get(id, K_CLASS_PANEL); }

Kid Kpanel_add(Kid panel, Kid child) { return Kpanel_insert(panel, -1, child); }

Kid Kpanel_insert(Kid panel, int index, Kid child) {
	kst_element_t* p = get_panel(panel);
	kst_element_t* c = kst_get(child, K_CLASS_ELEMENT);
	if (!p || !c) return K_INVALID_ID;
	kst_add_child(p, c, index);
	return c->parent == p ? child : K_INVALID_ID;
}

void Kpanel_remove(Kid panel, Kid child) {
	kst_element_t* p = get_panel(panel);
	kst_element_t* c = kst_get(child, K_CLASS_ELEMENT);
	if (p && c && c->parent == p) kst_remove_child(p, c);
}

void Kpanel_clear(Kid panel) {
	kst_element_t* p = get_panel(panel);
	if (p) kst_clear_children(p);
}

int Kpanel_get_count(Kid panel) {
	kst_element_t* p = get_panel(panel);
	return p ? p->child_count : 0;
}

Kid Kpanel_get_child(Kid panel, int index) {
	kst_element_t* p = get_panel(panel);
	return p && index >= 0 && index < p->child_count ? p->children[index]->id : K_INVALID_ID;
}

void Kpanel_set_background(Kid panel, KColor color) {
	kst_element_t* p = get_panel(panel);
	if (!p) return;
	KST_AS(kst_panel_t, p)->background = color;
	KST_AS(kst_panel_t, p)->has_background = 1;
	kst_changed_paint(p);
}

void Kpanel_set_zindex(Kid child, int zindex) {
	kst_element_t* c = kst_get(child, K_CLASS_ELEMENT);
	if (!c) return;
	c->zindex = zindex;
	kst_changed_paint(c);
}


/* ============================================================================
 *  StackPanel
 * ============================================================================ */

static KSize stack_measure(kst_element_t* e, KSize available) {
	kst_stack_panel_t* s = KST_AS(kst_stack_panel_t, e);
	KSize total = { 0, 0 };
	int i, vertical = s->orientation == K_VERTICAL;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		kst_measure(c, vertical ? kst_size(available.width, K_INFINITE) : kst_size(K_INFINITE, available.height));
		if (c->visibility == K_COLLAPSED) continue;
		if (vertical) {
			total.height += c->desired.height;
			total.width = kst_maxf(total.width, c->desired.width);
		} else {
			total.width += c->desired.width;
			total.height = kst_maxf(total.height, c->desired.height);
		}
	}
	return total;
}

static KSize stack_arrange(kst_element_t* e, KSize final) {
	kst_stack_panel_t* s = KST_AS(kst_stack_panel_t, e);
	float pos = 0;
	int i, vertical = s->orientation == K_VERTICAL;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		if (c->visibility == K_COLLAPSED) continue;
		if (vertical) {
			kst_arrange(c, kst_rect(0, pos, final.width, c->desired.height));
			pos += c->desired.height;
		} else {
			kst_arrange(c, kst_rect(pos, 0, c->desired.width, final.height));
			pos += c->desired.width;
		}
	}
	return final;
}

static void stack_init(kst_element_t* e) { KST_AS(kst_stack_panel_t, e)->orientation = K_VERTICAL; }

kst_class_t kst_class_stack_panel = {
	.base = &kst_class_panel, .name = "StackPanel", .id = K_CLASS_STACK_PANEL, .size = sizeof(kst_stack_panel_t),
	.init = stack_init, .measure = stack_measure, .arrange = stack_arrange
};

static void vstack_init(kst_element_t* e) { KST_AS(kst_stack_panel_t, e)->virtualizing = 1; }

kst_class_t kst_class_virtualizing_stack_panel = {
	.base = &kst_class_stack_panel, .name = "VirtualizingStackPanel", .id = K_CLASS_VIRTUALIZING_STACK_PANEL, .size = sizeof(kst_stack_panel_t),
	.init = vstack_init
};

static void items_stack_init(kst_element_t* e) { KST_AS(kst_stack_panel_t, e)->virtualizing = 1; }

kst_class_t kst_class_items_stack_panel = {
	.base = &kst_class_stack_panel, .name = "ItemsStackPanel", .id = K_CLASS_ITEMS_STACK_PANEL, .size = sizeof(kst_stack_panel_t),
	.init = items_stack_init
};

Kid Kitems_stack_panel_create(KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_ITEMS_STACK_PANEL);
	if (!e) return K_INVALID_ID;
	KST_AS(kst_stack_panel_t, e)->orientation = orientation;
	return e->id;
}

Kid Kstack_panel_create(const KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_STACK_PANEL);
	if (!e) return K_INVALID_ID;
	KST_AS(kst_stack_panel_t, e)->orientation = orientation;
	return e->id;
}

Kid Kvirtualizing_stack_panel_create(void) {
	kst_element_t* e = kst_create(K_CLASS_VIRTUALIZING_STACK_PANEL);
	return e ? e->id : K_INVALID_ID;
}

void Kstack_panel_set_orientation(Kid id, KOrientation orientation) {
	kst_element_t* e = kst_get(id, K_CLASS_STACK_PANEL);
	if (!e) return;
	KST_AS(kst_stack_panel_t, e)->orientation = orientation;
	kst_changed_layout(e);
	kst_items_panel_changed(e);
}

KOrientation Kstack_panel_get_orientation(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_STACK_PANEL);
	return e ? KST_AS(kst_stack_panel_t, e)->orientation : K_VERTICAL;
}


/* ============================================================================
 *  WrapPanel
 * ============================================================================ */

static KSize wrap_item_size(const kst_wrap_panel_t* w, const kst_element_t* c) {
	return kst_size(w->item_width != K_AUTO ? w->item_width : c->desired.width, w->item_height != K_AUTO ? w->item_height : c->desired.height);
}

/* Lays the children out in lines no longer than `limit`; when `place` the children are arranged. Returns the extent. */
static KSize wrap_flow(kst_element_t* e, float limit, int place) {
	kst_wrap_panel_t* w = KST_AS(kst_wrap_panel_t, e);
	int horizontal = w->orientation == K_HORIZONTAL, i;
	float pos = 0, line_extent = 0, offset = 0, max_pos = 0;
	int line_start = 0;
	for (i = 0; i <= e->child_count; i++) {
		KSize s = { 0, 0 };
		float along, across;
		kst_element_t* c = i < e->child_count ? e->children[i] : NULL;
		if (c) {
			if (c->visibility == K_COLLAPSED) continue;
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
					kst_element_t* d = e->children[j];
					KSize ds;
					if (d->visibility == K_COLLAPSED) continue;
					ds = wrap_item_size(w, d);
					if (horizontal) {
						kst_arrange(d, kst_rect(p2, offset, ds.width, line_extent));
						p2 += ds.width;
					} else {
						kst_arrange(d, kst_rect(offset, p2, line_extent, ds.height));
						p2 += ds.height;
					}
				}
			}
			max_pos = kst_maxf(max_pos, pos);
			offset += line_extent;
			pos = 0;
			line_extent = 0;
			line_start = i;
			if (!c) break;
		}
		pos += along;
		line_extent = kst_maxf(line_extent, across);
	}
	return horizontal ? kst_size(max_pos, offset) : kst_size(offset, max_pos);
}

static KSize wrap_measure(kst_element_t* e, KSize available) {
	kst_wrap_panel_t* w = KST_AS(kst_wrap_panel_t, e);
	int i;
	for (i = 0; i < e->child_count; i++) {
		KSize s = kst_size(w->item_width != K_AUTO ? w->item_width : available.width, w->item_height != K_AUTO ? w->item_height : available.height);
		kst_measure(e->children[i], s);
	}
	return wrap_flow(e, w->orientation == K_HORIZONTAL ? available.width : available.height, 0);
}

static KSize wrap_arrange(kst_element_t* e, KSize final) {
	kst_wrap_panel_t* w = KST_AS(kst_wrap_panel_t, e);
	wrap_flow(e, w->orientation == K_HORIZONTAL ? final.width : final.height, 1);
	return final;
}

static void wrap_init(kst_element_t* e) {
	kst_wrap_panel_t* w = KST_AS(kst_wrap_panel_t, e);
	w->orientation = K_HORIZONTAL;
	w->item_width = K_AUTO;
	w->item_height = K_AUTO;
}

kst_class_t kst_class_wrap_panel = {
	.base = &kst_class_panel, .name = "WrapPanel", .id = K_CLASS_WRAP_PANEL, .size = sizeof(kst_wrap_panel_t),
	.init = wrap_init, .measure = wrap_measure, .arrange = wrap_arrange
};

Kid Kwrap_panel_create(KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_WRAP_PANEL);
	if (!e) return K_INVALID_ID;
	KST_AS(kst_wrap_panel_t, e)->orientation = orientation;
	return e->id;
}

void Kwrap_panel_set_orientation(Kid id, KOrientation orientation) {
	kst_element_t* e = kst_get(id, K_CLASS_WRAP_PANEL);
	if (!e) return;
	KST_AS(kst_wrap_panel_t, e)->orientation = orientation;
	kst_changed_layout(e);
}

void Kwrap_panel_set_item_size(Kid id, float item_width, float item_height) {
	kst_element_t* e = kst_get(id, K_CLASS_WRAP_PANEL);
	if (!e) return;
	KST_AS(kst_wrap_panel_t, e)->item_width = item_width;
	KST_AS(kst_wrap_panel_t, e)->item_height = item_height;
	kst_changed_layout(e);
}


/* ============================================================================
 *  ItemsWrapGrid
 * ============================================================================ */

#define WG(e) KST_AS(kst_items_wrap_grid_t, e)

static int wg_is_header(const kst_element_t* c) { return kst_is_a(c, K_CLASS_LIST_VIEW_BASE_HEADER_ITEM); }

/* The size of a cell: ItemWidth by ItemHeight, or the size of the first item for what is not set. Measures that item. */
static void wg_cell(kst_element_t* e, KSize room, float* cw, float* ch) {
	kst_items_wrap_grid_t* g = WG(e);
	int i;
	*cw = g->item_width != K_AUTO ? g->item_width : 0;
	*ch = g->item_height != K_AUTO ? g->item_height : 0;
	if (g->item_width != K_AUTO && g->item_height != K_AUTO) return;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		if (c->visibility == K_COLLAPSED || wg_is_header(c)) continue;
		kst_measure(c, kst_size(g->item_width != K_AUTO ? g->item_width : room.width, g->item_height != K_AUTO ? g->item_height : room.height));
		if (g->item_width == K_AUTO) *cw = c->desired.width;
		if (g->item_height == K_AUTO) *ch = c->desired.height;
		return;
	}
}

static int wg_per_line(const kst_items_wrap_grid_t* g, float along_room, float cell_along) {
	int n = 0x7fffffff;
	if (cell_along > 0 && along_room < K_INFINITE * 0.5f) n = kst_maxi(1, (int)floorf((along_room + 0.01f) / cell_along));
	if (g->max_rows_or_columns > 0) n = kst_mini(n, g->max_rows_or_columns);
	return n;
}

/* Puts the cells in lines along the orientation. Returns the size taken; with `place` the items are arranged. */
static KSize wg_flow(kst_element_t* e, float along_room, float cw, float ch, int per_line, int place) {
	kst_items_wrap_grid_t* g = WG(e);
	int horizontal = g->orientation == K_HORIZONTAL, in_line = 0, i;
	float cell_along = horizontal ? cw : ch, cell_across = horizontal ? ch : cw;
	float across = 0, max_along = 0;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		if (c->visibility == K_COLLAPSED) continue;
		if (wg_is_header(c)) {
			float header_across = horizontal ? c->desired.height : c->desired.width;
			float header_along = along_room < K_INFINITE * 0.5f ? along_room : (horizontal ? c->desired.width : c->desired.height);
			if (in_line) {
				across += cell_across;
				max_along = kst_maxf(max_along, (float)in_line * cell_along);
				in_line = 0;
			}
			if (place) kst_arrange(c, horizontal ? kst_rect(0, across, header_along, header_across) : kst_rect(across, 0, header_across, header_along));
			across += header_across;
			continue;
		}
		if (in_line >= per_line) {
			across += cell_across;
			max_along = kst_maxf(max_along, (float)in_line * cell_along);
			in_line = 0;
		}
		if (place) {
			float along = (float)in_line * cell_along;
			kst_arrange(c, horizontal ? kst_rect(along, across, cw, ch) : kst_rect(across, along, cw, ch));
		}
		in_line++;
	}
	if (in_line) {
		across += cell_across;
		max_along = kst_maxf(max_along, (float)in_line * cell_along);
	}
	return horizontal ? kst_size(max_along, across) : kst_size(across, max_along);
}

static KSize wg_measure(kst_element_t* e, KSize available) {
	kst_items_wrap_grid_t* g = WG(e);
	int horizontal = g->orientation == K_HORIZONTAL, per_line, i;
	float cw, ch, along = horizontal ? available.width : available.height;
	wg_cell(e, available, &cw, &ch);
	per_line = wg_per_line(g, along, horizontal ? cw : ch);
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		if (wg_is_header(c)) kst_measure(c, horizontal ? kst_size(kst_minf(available.width, K_INFINITE * 0.5f), K_INFINITE) : kst_size(K_INFINITE, available.height));
		else kst_measure(c, kst_size(cw, ch));
	}
	return wg_flow(e, along, cw, ch, per_line, 0);
}

static KSize wg_arrange(kst_element_t* e, KSize final) {
	kst_items_wrap_grid_t* g = WG(e);
	int horizontal = g->orientation == K_HORIZONTAL;
	float cw, ch, along = horizontal ? final.width : final.height;
	wg_cell(e, final, &cw, &ch);
	wg_flow(e, along, cw, ch, wg_per_line(g, along, horizontal ? cw : ch), 1);
	return final;
}

static void wg_init(kst_element_t* e) {
	kst_items_wrap_grid_t* g = WG(e);
	g->orientation = K_HORIZONTAL;
	g->max_rows_or_columns = -1;
	g->item_width = K_AUTO;
	g->item_height = K_AUTO;
}

kst_class_t kst_class_items_wrap_grid = {
	.base = &kst_class_panel, .name = "ItemsWrapGrid", .id = K_CLASS_ITEMS_WRAP_GRID, .size = sizeof(kst_items_wrap_grid_t),
	.init = wg_init, .measure = wg_measure, .arrange = wg_arrange
};

Kid Kitems_wrap_grid_create(KOrientation orientation) {
	kst_element_t* e = kst_create(K_CLASS_ITEMS_WRAP_GRID);
	if (!e) return K_INVALID_ID;
	WG(e)->orientation = orientation;
	return e->id;
}

static kst_items_wrap_grid_t* get_wg(Kid id) { return WG(kst_get(id, K_CLASS_ITEMS_WRAP_GRID)); }

void Kitems_wrap_grid_set_orientation(Kid id, KOrientation orientation) {
	kst_items_wrap_grid_t* g = get_wg(id);
	if (!g) return;
	g->orientation = orientation;
	kst_changed_layout(&g->base.base);
	kst_items_panel_changed(&g->base.base);
}

KOrientation Kitems_wrap_grid_get_orientation(Kid id) { kst_items_wrap_grid_t* g = get_wg(id); return g ? g->orientation : K_HORIZONTAL; }

void Kitems_wrap_grid_set_maximum_rows_or_columns(Kid id, int maximum) {
	kst_items_wrap_grid_t* g = get_wg(id);
	if (!g) return;
	g->max_rows_or_columns = maximum > 0 ? maximum : -1;
	kst_changed_layout(&g->base.base);
}

int Kitems_wrap_grid_get_maximum_rows_or_columns(Kid id) { kst_items_wrap_grid_t* g = get_wg(id); return g ? g->max_rows_or_columns : -1; }

void Kitems_wrap_grid_set_item_width(Kid id, float item_width) {
	kst_items_wrap_grid_t* g = get_wg(id);
	if (!g) return;
	g->item_width = item_width;
	kst_changed_layout(&g->base.base);
}

void Kitems_wrap_grid_set_item_height(Kid id, float item_height) {
	kst_items_wrap_grid_t* g = get_wg(id);
	if (!g) return;
	g->item_height = item_height;
	kst_changed_layout(&g->base.base);
}

float Kitems_wrap_grid_get_item_width(Kid id) { kst_items_wrap_grid_t* g = get_wg(id); return g ? g->item_width : K_AUTO; }
float Kitems_wrap_grid_get_item_height(Kid id) { kst_items_wrap_grid_t* g = get_wg(id); return g ? g->item_height : K_AUTO; }


/* ============================================================================
 *  Grid
 * ============================================================================ */

static int grid_add_def(kst_grid_def_t** list, int* count, KGridUnitType unit, float value) {
	kst_grid_def_t* more;
	if (*count >= 256) return -1;
	more = (kst_grid_def_t*)realloc(*list, (size_t)(*count + 1) * sizeof **list);
	if (!more) return -1;
	*list = more;
	more[*count].unit = unit;
	more[*count].value = unit == K_GRID_STAR && value <= 0 ? 1.0f : value;
	more[*count].min = 0;
	more[*count].max = K_INFINITE;
	more[*count].size = 0;
	return (*count)++;
}

typedef struct grid_axis {
	kst_grid_def_t* defs;
	int count;
	/** The implicit single star definition when the grid has none. */
	kst_grid_def_t implicit_def;
} grid_axis_t;

static void grid_axis_get(kst_grid_def_t* defs, int count, grid_axis_t* axis) {
	if (count > 0) {
		axis->defs = defs;
		axis->count = count;
	} else {
		axis->implicit_def.unit = K_GRID_STAR;
		axis->implicit_def.value = 1;
		axis->implicit_def.min = 0;
		axis->implicit_def.max = K_INFINITE;
		axis->implicit_def.size = 0;
		axis->defs = &axis->implicit_def;
		axis->count = 1;
	}
}

static void span_range(int index, int span, int count, int* first, int* last) {
	*first = kst_maxi(0, kst_mini(index, count - 1));
	*last = kst_mini(count - 1, *first + kst_maxi(1, span) - 1);
}

typedef struct grid_cell {
	kst_element_t* child;
	int c0, c1, r0, r1;
	int star_col, star_row, auto_col, auto_row;
} grid_cell_t;

/* How much room a measured child may take along an axis: the sizes it spans, infinite if it spans an auto or an unresolved star. */
static float cell_room(const kst_grid_def_t* defs, int first, int last, int resolved) {
	float sum = 0;
	int i;
	for (i = first; i <= last; i++) {
		if (defs[i].unit == K_GRID_AUTO) return K_INFINITE;
		if (defs[i].unit == K_GRID_STAR && !resolved) return K_INFINITE;
		sum += defs[i].size;
	}
	return sum;
}

static void grid_learn(kst_grid_def_t* defs, int first, int last, float need) {
	/* a child that spans one definition gives it its size; over several, the missing room goes to the last auto one */
	float have = 0;
	int i, target = -1;
	for (i = first; i <= last; i++) {
		have += defs[i].size;
		if (defs[i].unit == K_GRID_AUTO) target = i;
	}
	if (target >= 0 && need > have) defs[target].size += need - have;
}

static void grid_resolve_star(kst_grid_def_t* defs, int count, float available, const float* content) {
	float fixed = 0, weights = 0, remaining;
	int i, pass;
	char frozen[256];
	if (count > 256) count = 256;
	for (i = 0; i < count; i++) {
		frozen[i] = 0;
		if (defs[i].unit == K_GRID_STAR) weights += defs[i].value;
		else fixed += defs[i].size;
	}
	if (weights <= 0) return;
	if (available >= K_INFINITE * 0.5f) {
		/* unlimited room: a star definition is as big as the content that spans it, in proportion to its weight */
		float ratio = 0;
		for (i = 0; i < count; i++) if (defs[i].unit == K_GRID_STAR) ratio = kst_maxf(ratio, content[i] / defs[i].value);
		for (i = 0; i < count; i++) if (defs[i].unit == K_GRID_STAR) defs[i].size = kst_clampf(ratio * defs[i].value, defs[i].min, defs[i].max);
		return;
	}
	remaining = kst_maxf(0, available - fixed);
	/* definitions held by their min / max take their value and the rest share what is left */
	for (pass = 0; pass < count; pass++) {
		float w = 0, share_total = remaining;
		int changed = 0;
		for (i = 0; i < count; i++) {
			if (defs[i].unit != K_GRID_STAR) continue;
			if (frozen[i]) share_total -= defs[i].size;
			else w += defs[i].value;
		}
		if (w <= 0) break;
		for (i = 0; i < count; i++) {
			float s;
			if (defs[i].unit != K_GRID_STAR || frozen[i]) continue;
			s = share_total * defs[i].value / w;
			if (s < defs[i].min) { defs[i].size = defs[i].min; frozen[i] = 1; changed = 1; }
			else if (s > defs[i].max) { defs[i].size = defs[i].max; frozen[i] = 1; changed = 1; }
			else defs[i].size = s;
		}
		if (!changed) break;
	}
}

static void grid_prepare(kst_grid_t* g, grid_axis_t* cols, grid_axis_t* rows) {
	int i;
	grid_axis_get(g->columns, g->column_count, cols);
	grid_axis_get(g->rows, g->row_count, rows);
	for (i = 0; i < cols->count; i++) cols->defs[i].size = cols->defs[i].unit == K_GRID_PIXEL ? kst_clampf(cols->defs[i].value, cols->defs[i].min, cols->defs[i].max) : 0;
	for (i = 0; i < rows->count; i++) rows->defs[i].size = rows->defs[i].unit == K_GRID_PIXEL ? kst_clampf(rows->defs[i].value, rows->defs[i].min, rows->defs[i].max) : 0;
}

static void grid_classify(kst_element_t* child, const grid_axis_t* cols, const grid_axis_t* rows, grid_cell_t* cell) {
	int i;
	cell->child = child;
	span_range(child->grid_column, child->grid_column_span, cols->count, &cell->c0, &cell->c1);
	span_range(child->grid_row, child->grid_row_span, rows->count, &cell->r0, &cell->r1);
	cell->star_col = cell->star_row = cell->auto_col = cell->auto_row = 0;
	for (i = cell->c0; i <= cell->c1; i++) {
		if (cols->defs[i].unit == K_GRID_STAR) cell->star_col = 1;
		if (cols->defs[i].unit == K_GRID_AUTO) cell->auto_col = 1;
	}
	for (i = cell->r0; i <= cell->r1; i++) {
		if (rows->defs[i].unit == K_GRID_STAR) cell->star_row = 1;
		if (rows->defs[i].unit == K_GRID_AUTO) cell->auto_row = 1;
	}
}

static void grid_clamp_auto(grid_axis_t* axis) {
	int i;
	for (i = 0; i < axis->count; i++) if (axis->defs[i].unit == K_GRID_AUTO) axis->defs[i].size = kst_clampf(axis->defs[i].size, axis->defs[i].min, axis->defs[i].max);
}

static KSize grid_measure(kst_element_t* e, KSize available) {
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	grid_axis_t cols, rows;
	grid_cell_t* cells = NULL;
	float col_content[256] = { 0 }, row_content[256] = { 0 };
	int n = 0, i, group;
	KSize result = { 0, 0 };
	int inf_w = available.width >= K_INFINITE * 0.5f, inf_h = available.height >= K_INFINITE * 0.5f;
	grid_prepare(g, &cols, &rows);
	if (e->child_count > 0) cells = (grid_cell_t*)calloc((size_t)e->child_count, sizeof *cells);
	for (i = 0; i < e->child_count && cells; i++) {
		if (e->children[i]->visibility == K_COLLAPSED) { kst_measure(e->children[i], kst_size(0, 0)); continue; }
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
			int sc = c->star_col && !inf_w, sr = c->star_row && !inf_h;
			int mine = sc ? (sr ? 3 : 2) : (sr ? 1 : 0);
			float room_w, room_h;
			if (mine != group) continue;
			room_w = cell_room(cols.defs, c->c0, c->c1, group >= 2);
			room_h = cell_room(rows.defs, c->r0, c->r1, group >= 3);
			if (c->star_col && inf_w) room_w = K_INFINITE;
			if (c->star_row && inf_h) room_h = K_INFINITE;
			kst_measure(c->child, kst_size(room_w, room_h));
			if (!sc) {
				if (c->star_col) { if (c->c0 == c->c1) col_content[c->c0] = kst_maxf(col_content[c->c0], c->child->desired.width); }
				else grid_learn(cols.defs, c->c0, c->c1, c->child->desired.width);
			}
			if (!sr) {
				if (c->star_row) { if (c->r0 == c->r1) row_content[c->r0] = kst_maxf(row_content[c->r0], c->child->desired.height); }
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

static KSize grid_arrange(kst_element_t* e, KSize final) {
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	grid_axis_t cols, rows;
	float content_cols[256] = { 0 }, content_rows[256] = { 0 }, col_pos[257], row_pos[257];
	int i;
	grid_prepare(g, &cols, &rows);
	/* auto sizes are the ones of the last Measure: recompute them from the children */
	for (i = 0; i < e->child_count; i++) {
		grid_cell_t cell;
		kst_element_t* c = e->children[i];
		if (c->visibility == K_COLLAPSED) continue;
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
		kst_element_t* c = e->children[i];
		if (c->visibility == K_COLLAPSED) continue;
		grid_classify(c, &cols, &rows, &cell);
		kst_arrange(c, kst_rect(col_pos[cell.c0], row_pos[cell.r0], col_pos[cell.c1 + 1] - col_pos[cell.c0], row_pos[cell.r1 + 1] - row_pos[cell.r0]));
	}
	/* publish the sizes when the grid has real definitions: they are already in g->columns / g->rows */
	return final;
}

static void grid_destroy(kst_element_t* e) {
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	free(g->columns);
	free(g->rows);
}

kst_class_t kst_class_grid = {
	.base = &kst_class_panel, .name = "Grid", .id = K_CLASS_GRID, .size = sizeof(kst_grid_t),
	.destroy = grid_destroy, .measure = grid_measure, .arrange = grid_arrange
};

Kid Kgrid_create(void) {
	kst_element_t* e = kst_create(K_CLASS_GRID);
	return e ? e->id : K_INVALID_ID;
}

int Kgrid_add_column(Kid id, KGridUnitType unit, float value) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	int index;
	if (!e) return -1;
	index = grid_add_def(&KST_AS(kst_grid_t, e)->columns, &KST_AS(kst_grid_t, e)->column_count, unit, value);
	kst_changed_layout(e);
	return index;
}

int Kgrid_add_row(Kid id, KGridUnitType unit, float value) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	int index;
	if (!e) return -1;
	index = grid_add_def(&KST_AS(kst_grid_t, e)->rows, &KST_AS(kst_grid_t, e)->row_count, unit, value);
	kst_changed_layout(e);
	return index;
}

void Kgrid_set_column_limits(Kid id, int column, float min_width, float max_width) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	if (!e || column < 0 || column >= g->column_count) return;
	g->columns[column].min = min_width;
	g->columns[column].max = max_width;
	kst_changed_layout(e);
}

void Kgrid_set_row_limits(Kid id, int row, float min_height, float max_height) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	if (!e || row < 0 || row >= g->row_count) return;
	g->rows[row].min = min_height;
	g->rows[row].max = max_height;
	kst_changed_layout(e);
}

int Kgrid_get_column_count(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_GRID); return e ? KST_AS(kst_grid_t, e)->column_count : 0; }
int Kgrid_get_row_count(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_GRID); return e ? KST_AS(kst_grid_t, e)->row_count : 0; }

float Kgrid_get_column_width(Kid id, int column) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	if (!e) return 0;
	if (g->column_count == 0) return column == 0 ? e->rect.width : 0;
	return column >= 0 && column < g->column_count ? g->columns[column].size : 0;
}

float Kgrid_get_row_height(Kid id, int row) {
	kst_element_t* e = kst_get(id, K_CLASS_GRID);
	kst_grid_t* g = KST_AS(kst_grid_t, e);
	if (!e) return 0;
	if (g->row_count == 0) return row == 0 ? e->rect.height : 0;
	return row >= 0 && row < g->row_count ? g->rows[row].size : 0;
}

#define ATTACHED(name, field, minimum) \
	void name(Kid id, int value) { \
		kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); \
		if (!e) return; \
		e->field = value < minimum ? minimum : value; \
		if (e->parent) kst_changed_layout(e->parent); \
	}

ATTACHED(Kgrid_set_row, grid_row, 0)
ATTACHED(Kgrid_set_column, grid_column, 0)
ATTACHED(Kgrid_set_row_span, grid_row_span, 1)
ATTACHED(Kgrid_set_column_span, grid_column_span, 1)

int Kgrid_get_row(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->grid_row : 0; }
int Kgrid_get_column(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->grid_column : 0; }

Kid Kgrid_place(Kid grid, Kid child, int column, int row) {
	Kgrid_set_column(child, column);
	Kgrid_set_row(child, row);
	return Kpanel_add(grid, child);
}


/* ============================================================================
 *  Canvas
 * ============================================================================ */

static KSize canvas_measure(kst_element_t* e, KSize available) {
	int i;
	KST_UNUSED(available);
	for (i = 0; i < e->child_count; i++) kst_measure(e->children[i], kst_size(K_INFINITE, K_INFINITE));
	return kst_size(0, 0);
}

static KSize canvas_arrange(kst_element_t* e, KSize final) {
	int i;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		float x = c->canvas_left == K_AUTO ? 0 : c->canvas_left, y = c->canvas_top == K_AUTO ? 0 : c->canvas_top;
		kst_arrange(c, kst_rect(x, y, c->desired.width, c->desired.height));
	}
	return final;
}

kst_class_t kst_class_canvas = {
	.base = &kst_class_panel, .name = "Canvas", .id = K_CLASS_CANVAS, .size = sizeof(kst_panel_t),
	.measure = canvas_measure, .arrange = canvas_arrange
};

Kid Kcanvas_create(void) {
	kst_element_t* e = kst_create(K_CLASS_CANVAS);
	return e ? e->id : K_INVALID_ID;
}

void Kcanvas_set_left(Kid id, float left) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	e->canvas_left = left;
	if (e->parent) kst_changed_layout(e->parent);
}

void Kcanvas_set_top(Kid id, float top) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	e->canvas_top = top;
	if (e->parent) kst_changed_layout(e->parent);
}

Kid Kcanvas_place(Kid canvas, Kid child, float left, float top) {
	Kcanvas_set_left(child, left);
	Kcanvas_set_top(child, top);
	return Kpanel_add(canvas, child);
}


/* ============================================================================
 *  Border
 * ============================================================================ */

static KThickness border_inset(const kst_border_t* b) {
	KThickness t;
	t.left = b->border_thickness.left + b->padding.left;
	t.top = b->border_thickness.top + b->padding.top;
	t.right = b->border_thickness.right + b->padding.right;
	t.bottom = b->border_thickness.bottom + b->padding.bottom;
	return t;
}

static KSize border_measure(kst_element_t* e, KSize available) {
	kst_border_t* b = KST_AS(kst_border_t, e);
	KThickness inset = border_inset(b);
	KSize content = { 0, 0 };
	if (e->child_count > 0) {
		kst_measure(e->children[0], kst_size(kst_maxf(0, available.width - kst_thickness_h(inset)), kst_maxf(0, available.height - kst_thickness_v(inset))));
		content = e->children[0]->desired;
	}
	return kst_size(content.width + kst_thickness_h(inset), content.height + kst_thickness_v(inset));
}

static KSize border_arrange(kst_element_t* e, KSize final) {
	kst_border_t* b = KST_AS(kst_border_t, e);
	KThickness inset = border_inset(b);
	if (e->child_count > 0) kst_arrange(e->children[0], kst_rect(inset.left, inset.top, kst_maxf(0, final.width - kst_thickness_h(inset)), kst_maxf(0, final.height - kst_thickness_v(inset))));
	return final;
}

static int border_has_radius(const kst_border_t* b) {
	return b->corner_radius.top_left > 0 || b->corner_radius.top_right > 0 || b->corner_radius.bottom_right > 0 || b->corner_radius.bottom_left > 0;
}

static void border_paint(kst_element_t* e, kst_painter_t* p) {
	kst_border_t* b = KST_AS(kst_border_t, e);
	KRect r = kst_local_rect(e);
	if (border_has_radius(b)) {
		if (b->has_border_brush) kst_fill_rrect(p, r, b->corner_radius, b->border_brush);
		if (b->has_background) {
			KRect inner = kst_rect(r.x + b->border_thickness.left, r.y + b->border_thickness.top, r.width - kst_thickness_h(b->border_thickness), r.height - kst_thickness_v(b->border_thickness));
			KCornerRadius ir = { kst_maxf(0, b->corner_radius.top_left - b->border_thickness.left), kst_maxf(0, b->corner_radius.top_right - b->border_thickness.right), kst_maxf(0, b->corner_radius.bottom_right - b->border_thickness.right), kst_maxf(0, b->corner_radius.bottom_left - b->border_thickness.left) };
			kst_fill_rrect(p, inner, ir, b->background);
		}
		return;
	}
	if (b->has_background) kst_fill_rect(p, r, b->background);
	if (b->has_border_brush) kst_stroke_rect(p, r, b->border_thickness, b->border_brush);
}

static int border_hit_self(kst_element_t* e) {
	kst_border_t* b = KST_AS(kst_border_t, e);
	return b->has_background || b->has_border_brush;
}

static void border_child_removed(kst_element_t* e, kst_element_t* child) { KST_UNUSED(e); KST_UNUSED(child); }

kst_class_t kst_class_border = {
	.base = &kst_class_element, .name = "Border", .id = K_CLASS_BORDER, .size = sizeof(kst_border_t),
	.measure = border_measure, .arrange = border_arrange, .paint = border_paint, .hit_self = border_hit_self,
	.child_removed = border_child_removed
};

Kid Kborder_create(void) {
	kst_element_t* e = kst_create(K_CLASS_BORDER);
	return e ? e->id : K_INVALID_ID;
}

void Kborder_set_child(Kid id, Kid child) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	kst_element_t* c = child != K_INVALID_ID ? kst_get(child, K_CLASS_ELEMENT) : NULL;
	kst_element_t* slot;
	if (!e || (child != K_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : NULL;
	kst_set_only_child(e, &slot, c);
}

Kid Kborder_get_child(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	return e && e->child_count ? e->children[0]->id : K_INVALID_ID;
}

void Kborder_set_background(Kid id, KColor color) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	if (!e) return;
	kst_unbind(e, KST_BP_BORDER_BACKGROUND);
	KST_AS(kst_border_t, e)->background = color;
	KST_AS(kst_border_t, e)->has_background = 1;
	kst_changed_paint(e);
}

void Kborder_set_border_brush(Kid id, KColor color) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	if (!e) return;
	kst_unbind(e, KST_BP_BORDER_BORDER_BRUSH);
	KST_AS(kst_border_t, e)->border_brush = color;
	KST_AS(kst_border_t, e)->has_border_brush = 1;
	kst_changed_paint(e);
}

void Kborder_set_border_thickness(Kid id, KThickness thickness) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	if (!e) return;
	kst_unbind(e, KST_BP_BORDER_BORDER_THICKNESS);
	KST_AS(kst_border_t, e)->border_thickness = thickness;
	kst_changed_layout(e);
}

void Kborder_set_corner_radius(Kid id, KCornerRadius radius) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	if (!e) return;
	KST_AS(kst_border_t, e)->corner_radius = radius;
	kst_changed_paint(e);
}

void Kborder_set_padding(Kid id, KThickness padding) {
	kst_element_t* e = kst_get(id, K_CLASS_BORDER);
	if (!e) return;
	kst_unbind(e, KST_BP_BORDER_PADDING);
	KST_AS(kst_border_t, e)->padding = padding;
	kst_changed_layout(e);
}


/* ============================================================================
 *  Viewbox
 *  The child is laid out at its natural size and painted scaled. It is
 *  not hit tested: a Viewbox shows, it does not interact.
 * ============================================================================ */

static void viewbox_scale(kst_viewbox_t* v, KSize content, KSize room, float* sx, float* sy) {
	*sx = *sy = 1.0f;
	if (v->stretch == K_STRETCH_NONE || content.width <= 0 || content.height <= 0) return;
	if (room.width < K_INFINITE * 0.5f) *sx = room.width / content.width;
	if (room.height < K_INFINITE * 0.5f) *sy = room.height / content.height;
	if (room.width >= K_INFINITE * 0.5f) *sx = *sy;
	if (room.height >= K_INFINITE * 0.5f) *sy = *sx;
	if (v->stretch == K_STRETCH_UNIFORM) *sx = *sy = kst_minf(*sx, *sy);
	else if (v->stretch == K_STRETCH_UNIFORM_TO_FILL) *sx = *sy = kst_maxf(*sx, *sy);
	if (v->direction == K_STRETCH_UP_ONLY) { *sx = kst_maxf(*sx, 1); *sy = kst_maxf(*sy, 1); }
	else if (v->direction == K_STRETCH_DOWN_ONLY) { *sx = kst_minf(*sx, 1); *sy = kst_minf(*sy, 1); }
}

static KSize viewbox_measure(kst_element_t* e, KSize available) {
	kst_viewbox_t* v = KST_AS(kst_viewbox_t, e);
	float sx, sy;
	kst_element_t* c;
	if (!e->child_count) return kst_size(0, 0);
	c = e->children[0];
	kst_measure(c, kst_size(K_INFINITE, K_INFINITE));
	viewbox_scale(v, c->desired, available, &sx, &sy);
	return kst_size(c->desired.width * sx, c->desired.height * sy);
}

static KSize viewbox_arrange(kst_element_t* e, KSize final) {
	kst_viewbox_t* v = KST_AS(kst_viewbox_t, e);
	kst_element_t* c;
	if (!e->child_count) return final;
	c = e->children[0];
	c->is_popup_child = 1;
	kst_arrange(c, kst_rect(0, 0, c->desired.width, c->desired.height));
	viewbox_scale(v, c->desired, final, &v->scale_x, &v->scale_y);
	return final;
}

static void viewbox_paint(kst_element_t* e, kst_painter_t* p) {
	kst_viewbox_t* v = KST_AS(kst_viewbox_t, e);
	kst_element_t* c;
	float scale = p->scale, tx = p->tx, ty = p->ty, w, h;
	if (!e->child_count) return;
	c = e->children[0];
	w = c->desired.width * v->scale_x;
	h = c->desired.height * v->scale_y;
	p->tx += (e->rect.width - w) * 0.5f * scale;
	p->ty += (e->rect.height - h) * 0.5f * scale;
	p->scale = scale * kst_minf(v->scale_x, v->scale_y);
	kst_paint_element(c, p, c->root);
	p->scale = scale;
	p->tx = tx;
	p->ty = ty;
}

static void viewbox_init(kst_element_t* e) {
	kst_viewbox_t* v = KST_AS(kst_viewbox_t, e);
	v->stretch = K_STRETCH_UNIFORM;
	v->direction = K_STRETCH_BOTH;
	v->scale_x = v->scale_y = 1.0f;
}

kst_class_t kst_class_viewbox = {
	.base = &kst_class_element, .name = "Viewbox", .id = K_CLASS_VIEWBOX, .size = sizeof(kst_viewbox_t),
	.init = viewbox_init, .measure = viewbox_measure, .arrange = viewbox_arrange, .paint = viewbox_paint
};

Kid Kviewbox_create(void) {
	kst_element_t* e = kst_create(K_CLASS_VIEWBOX);
	return e ? e->id : K_INVALID_ID;
}

void Kviewbox_set_child(Kid id, Kid child) {
	kst_element_t* e = kst_get(id, K_CLASS_VIEWBOX);
	kst_element_t* c = child != K_INVALID_ID ? kst_get(child, K_CLASS_ELEMENT) : NULL;
	kst_element_t* slot;
	if (!e || (child != K_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : NULL;
	kst_set_only_child(e, &slot, c);
}

void Kviewbox_set_stretch(Kid id, KStretch stretch) {
	kst_element_t* e = kst_get(id, K_CLASS_VIEWBOX);
	if (!e) return;
	KST_AS(kst_viewbox_t, e)->stretch = stretch;
	kst_changed_layout(e);
}

void Kviewbox_set_stretch_direction(Kid id, KStretchDirection direction) {
	kst_element_t* e = kst_get(id, K_CLASS_VIEWBOX);
	if (!e) return;
	KST_AS(kst_viewbox_t, e)->direction = direction;
	kst_changed_layout(e);
}
