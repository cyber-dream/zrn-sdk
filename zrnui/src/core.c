/**
 * zrnui - core.c
 * Handles, errors, classes, the visual tree, Measure / Arrange, hit testing
 * and the properties of FrameworkElement.
 */

#include "internal.h"

#include <stdarg.h>


/* ============================================================================
 *  errors
 * ============================================================================ */

static _Thread_local ZError g_error = Z_OK;
static _Thread_local char g_error_message[256];
static ZErrorCallback g_error_callback;
static void* g_error_user_data;

void zrn_error(const ZError error, const char* const fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsnprintf(g_error_message, sizeof g_error_message, fmt, args);
	va_end(args);
	g_error = error;
	if (g_error_callback) g_error_callback(error, g_error_message, g_error_user_data);
	else fprintf(stderr, "zrnui: %s\n", g_error_message);
}

ZError Zget_error(void) { return g_error; }
const char* Zget_error_message(void) { return g_error_message; }

void Zset_error_callback(const ZErrorCallback callback, void* const user_data) {
	g_error_callback = callback;
	g_error_user_data = user_data;
}

char* zrn_strdup(const char* s) {
	size_t n;
	char* d;
	if (!s) s = "";
	n = strlen(s);
	d = (char*)malloc(n + 1);
	if (d) memcpy(d, s, n + 1);
	return d;
}

void zrn_set_string(char** dest, const char* const s) {
	char* const copy = s ? zrn_strdup(s) : nullptr;
	free(*dest);
	*dest = copy;
}

ZColor Zcolor_rgba(float r, float g, float b, float a) { const ZColor c = { r, g, b, a }; return c; }

ZColor Zcolor_argb(const uint32_t argb) {
	ZColor c;
	c.a = (float)((argb >> 24) & 255) / 255.0f;
	c.r = (float)((argb >> 16) & 255) / 255.0f;
	c.g = (float)((argb >> 8) & 255) / 255.0f;
	c.b = (float)(argb & 255) / 255.0f;
	return c;
}

ZThickness Zthickness(float l, float t, float r, float b) { const ZThickness th = { l, t, r, b }; return th; }
ZThickness Zthickness_uniform(float v) { const ZThickness th = { v, v, v, v }; return th; }

const char* Zget_version(void) { return "0.1.0"; }


/* ============================================================================
 *  handles
 * ============================================================================ */

static constexpr int ZST_INDEX_BITS = 20;
#define ZST_INDEX_MASK ((1u << ZST_INDEX_BITS) - 1)

typedef struct zrn_slot {
	void* ptr;
	uint16_t generation;
	uint8_t is_root;
} zrn_slot_t;

static zrn_slot_t* g_slots;
static uint32_t g_slot_count, g_slot_cap;
static uint32_t* g_free;
static uint32_t g_free_count, g_free_cap;

Zid zrn_handle_alloc(void* const ptr, const int is_root) {
	uint32_t index;
	if (g_free_count) {
		index = g_free[--g_free_count];
	} else {
		if (g_slot_count == g_slot_cap) {
			const uint32_t cap = g_slot_cap ? g_slot_cap * 2 : 256;
			zrn_slot_t* slots = (zrn_slot_t*)realloc(g_slots, cap * sizeof *slots);
			if (!slots) return Z_INVALID_ID;
			memset(slots + g_slot_cap, 0, (cap - g_slot_cap) * sizeof *slots);
			g_slots = slots;
			g_slot_cap = cap;
		}
		index = g_slot_count++;
	}
	g_slots[index].ptr = ptr;
	g_slots[index].is_root = (uint8_t)is_root;
	if (g_slots[index].generation == 0) g_slots[index].generation = 1;
	return ((Zid)g_slots[index].generation << ZST_INDEX_BITS) | (index + 1);
}

void zrn_handle_free(const Zid id) {
	const uint32_t index = (id & ZST_INDEX_MASK) - 1;
	if ((id & ZST_INDEX_MASK) == 0 || index >= g_slot_count) return;
	g_slots[index].ptr = nullptr;
	g_slots[index].generation = (uint16_t)((g_slots[index].generation + 1) & 0xFFF);
	if (g_slots[index].generation == 0) g_slots[index].generation = 1;
	if (g_free_count == g_free_cap) {
		const uint32_t cap = g_free_cap ? g_free_cap * 2 : 64;
		uint32_t* list = (uint32_t*)realloc(g_free, cap * sizeof *list);
		if (!list) return;
		g_free = list;
		g_free_cap = cap;
	}
	g_free[g_free_count++] = index;
}

void* zrn_handle_get(const Zid id, const int is_root) {
	const uint32_t index = (id & ZST_INDEX_MASK) - 1;
	if ((id & ZST_INDEX_MASK) == 0 || index >= g_slot_count) return nullptr;
	if (g_slots[index].ptr == nullptr || g_slots[index].generation != (id >> ZST_INDEX_BITS)) return nullptr;
	if (g_slots[index].is_root != (uint8_t)is_root) return nullptr;
	return g_slots[index].ptr;
}

zrn_element_t* zrn_lookup(Zid id) { return (zrn_element_t*)zrn_handle_get(id, 0); }

zrn_element_t* zrn_get(const Zid id, const ZClass cls) {
	zrn_element_t* const e = zrn_lookup(id);
	if (!e) {
		zrn_error(Z_ERROR_INVALID_ID, "invalid handle 0x%x", (unsigned)id);
		return nullptr;
	}
	if (cls != Z_CLASS_ELEMENT && !zrn_is_a(e, cls)) {
		zrn_error(Z_ERROR_WRONG_CLASS, "%s is not a %s", e->cls->name, zrn_class_of(cls)->name);
		return nullptr;
	}
	return e;
}


/* ============================================================================
 *  classes
 * ============================================================================ */

const zrn_class_t* zrn_class_of(const ZClass id) {
	static const zrn_class_t* const table[] = {
		&zrn_class_element, &zrn_class_panel, &zrn_class_stack_panel, &zrn_class_virtualizing_stack_panel,
		&zrn_class_wrap_panel, &zrn_class_grid, &zrn_class_canvas, &zrn_class_border, &zrn_class_viewbox,
		&zrn_class_text_block, &zrn_class_image, &zrn_class_popup, &zrn_class_control, &zrn_class_user_control,
		&zrn_class_content_control, &zrn_class_tooltip, &zrn_class_button_base, &zrn_class_button,
		&zrn_class_hyperlink_button, &zrn_class_repeat_button, &zrn_class_toggle_button, &zrn_class_checkbox,
		&zrn_class_radio_button, &zrn_class_range_base, &zrn_class_slider, &zrn_class_progress_bar,
		&zrn_class_scrollbar, &zrn_class_thumb, &zrn_class_separator, &zrn_class_scroll_viewer,
		&zrn_class_text_box, &zrn_class_password_box, &zrn_class_items_control, &zrn_class_selector,
		&zrn_class_selector_item, &zrn_class_list_box, &zrn_class_list_box_item, &zrn_class_combo_box,
		&zrn_class_combo_box_item, &zrn_class_auto_suggest_box, &zrn_class_frame, &zrn_class_page, &zrn_class_pivot,
		&zrn_class_toggle_switch, &zrn_class_progress_ring, &zrn_class_list_view_base, &zrn_class_list_view,
		&zrn_class_grid_view, &zrn_class_list_view_item, &zrn_class_grid_view_item,
		&zrn_class_list_view_base_header_item, &zrn_class_list_view_header_item, &zrn_class_grid_view_header_item,
		&zrn_class_flip_view, &zrn_class_flip_view_item, &zrn_class_items_stack_panel, &zrn_class_items_wrap_grid,
		&zrn_class_semantic_zoom, &zrn_class_icon_element, &zrn_class_symbol_icon, &zrn_class_font_icon, &zrn_class_path_icon,
		&zrn_class_bitmap_icon, &zrn_class_app_bar, &zrn_class_command_bar, &zrn_class_app_bar_button,
		&zrn_class_app_bar_toggle_button, &zrn_class_app_bar_separator, &zrn_class_flyout_base, &zrn_class_flyout,
		&zrn_class_menu_flyout, &zrn_class_menu_flyout_item_base, &zrn_class_menu_flyout_item,
		&zrn_class_toggle_menu_flyout_item, &zrn_class_menu_flyout_separator, &zrn_class_content_dialog,
		&zrn_class_pivot_item, &zrn_class_hub, &zrn_class_hub_section, &zrn_class_looping_selector, &zrn_class_date_picker,
		&zrn_class_time_picker, &zrn_class_picker_flyout_base, &zrn_class_date_picker_flyout, &zrn_class_time_picker_flyout,
		&zrn_class_rich_text_block
	};
	const unsigned index = (unsigned)id - (unsigned)Z_CLASS_ELEMENT;
	if (index >= sizeof table / sizeof table[0]) return nullptr;
	return table[index];
}

/* Copies the slots a class left empty from its base, base first. */
static void zrn_class_resolve(const zrn_class_t* const const_cls) {
	const auto cls = (zrn_class_t*)const_cls;
	const zrn_class_t* base = cls->base;
	if (cls->resolved) return;
	cls->resolved = 1;
	if (!base) return;
	zrn_class_resolve(base);
#define INHERIT(member) if (!cls->member) cls->member = base->member
	INHERIT(measure);
	INHERIT(arrange);
	INHERIT(paint);
	INHERIT(paint_over);
	INHERIT(input);
	INHERIT(tick);
	INHERIT(child_removed);
	INHERIT(hit_self);
	INHERIT(chrome);
	INHERIT(content_foreground);
	INHERIT(content_font_size);
	INHERIT(content_font_family);
#undef INHERIT
}

int zrn_class_derives(const zrn_class_t* cls, const zrn_class_t* base) {
	for (; cls; cls = cls->base) if (cls == base) return 1;
	return 0;
}

int zrn_is_a(const zrn_element_t* const e, const ZClass id) {
	const zrn_class_t* cls = zrn_class_of(id);
	return cls && zrn_class_derives(e->cls, cls);
}


/* ============================================================================
 *  default theme for elements outside a root
 * ============================================================================ */

static zrn_theme_t g_default_theme;
static int g_default_theme_ready;

ZColor zrn_theme_color(const zrn_element_t* const e, const zrn_theme_color_id_t id) {
	if (e && e->root) return e->root->theme.colors[id];
	if (!g_default_theme_ready) {
		zrn_theme_init(&g_default_theme, Z_THEME_DARK, zrn_accent_color(Z_ACCENT_COBALT));
		g_default_theme_ready = 1;
	}
	return g_default_theme.colors[id];
}


/* ============================================================================
 *  element creation and destruction
 * ============================================================================ */

static void zrn_run_init(zrn_element_t* const e, const zrn_class_t* const cls) {
	if (cls->base) zrn_run_init(e, cls->base);
	if (cls->init) cls->init(e);
}

zrn_element_t* zrn_create(const ZClass id) {
	const zrn_class_t* cls = zrn_class_of(id);
	zrn_element_t* e;
	if (!cls) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "unknown class 0x%x", (unsigned)id);
		return nullptr;
	}
	if (cls->abstract_class) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "%s is abstract", cls->name);
		return nullptr;
	}
	zrn_class_resolve(cls);
	e = (zrn_element_t*)calloc(1, cls->size);
	if (!e) {
		zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
		return nullptr;
	}
	e->cls = cls;
	e->id = zrn_handle_alloc(e, 0);
	if (e->id == Z_INVALID_ID) {
		free(e);
		zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
		return nullptr;
	}
	zrn_run_init(e, cls);
	/* what leans when pressed, as in the Windows Phone toolkit: not a Button, which has a pressed look of its own */
	e->tilt = (zrn_is_a(e, Z_CLASS_SELECTOR_ITEM) && !zrn_is_a(e, Z_CLASS_FLIP_VIEW_ITEM)) ||
		zrn_is_a(e, Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM) || zrn_is_a(e, Z_CLASS_HYPERLINK_BUTTON) ||
		zrn_is_a(e, Z_CLASS_CHECKBOX) || zrn_is_a(e, Z_CLASS_RADIO_BUTTON) || zrn_is_a(e, Z_CLASS_MENU_FLYOUT_ITEM);
	return e;
}

static void zrn_run_destroy(zrn_element_t* const e) {
	const zrn_class_t* cls;
	for (cls = e->cls; cls; cls = cls->base) if (cls->destroy) cls->destroy(e);
}

void zrn_destroy(zrn_element_t* const e) {
	if (!e) return;
	e->destroying = 1;
	if (e->parent) zrn_remove_child(e->parent, e);
	zrn_root_detach(e);
	while (e->child_count > 0) zrn_destroy(e->children[e->child_count - 1]);
	zrn_run_destroy(e);
	zrn_handle_free(e->id);
	zrn_tweens_free(e);
	zrn_bindings_free(e);
	free(e->children);
	free(e->name);
	free(e->tooltip);
	free(e);
}

/* The base of the hierarchy: UIElement + FrameworkElement defaults. */
static void element_init(zrn_element_t* const e) {
	e->width = Z_AUTO;
	e->height = Z_AUTO;
	e->min_width = 0;
	e->min_height = 0;
	e->max_width = Z_INFINITE;
	e->max_height = Z_INFINITE;
	e->halign = Z_HALIGN_STRETCH;
	e->valign = Z_VALIGN_STRETCH;
	e->visibility = Z_VISIBLE;
	e->opacity = 1.0f;
	e->enabled = 1;
	e->hit_visible = 1;
	e->grid_row_span = 1;
	e->grid_column_span = 1;
	e->render_scale = 1.0f;
	e->proj_cx = 0.5f;
	e->proj_cy = 0.5f;
	e->canvas_left = Z_AUTO;
	e->canvas_top = Z_AUTO;
	e->tab_stop = 1;
}

static ZSize element_measure(zrn_element_t* const e, const ZSize available) {
	ZST_UNUSED(e); ZST_UNUSED(available);
	return zrn_size(0, 0);
}

static ZSize element_arrange(zrn_element_t* const e, const ZSize final) {
	ZST_UNUSED(e);
	return final;
}

zrn_class_t zrn_class_element = {
	.base = nullptr, .name = "Element", .id = Z_CLASS_ELEMENT, .size = sizeof(zrn_element_t),
	.init = element_init, .measure = element_measure, .arrange = element_arrange
};


static void clear_measure_below(zrn_element_t* e);

/* ============================================================================
 *  visual tree
 * ============================================================================ */

int zrn_child_index(const zrn_element_t* const parent, const zrn_element_t* const child) {
	int i;
	for (i = 0; i < parent->child_count; i++) if (parent->children[i] == child) return i;
	return -1;
}

void zrn_add_child(zrn_element_t* parent, zrn_element_t* child, int index) {
	zrn_element_t* a;
	for (a = parent; a; a = a->parent) {
		if (a == child) {
			zrn_error(Z_ERROR_CYCLE, "an element cannot be added under itself");
			return;
		}
	}
	if (child->parent) zrn_remove_child(child->parent, child);
	if (parent->child_count == parent->child_cap) {
		const int cap = parent->child_cap ? parent->child_cap * 2 : 4;
		zrn_element_t** list = (zrn_element_t**)realloc(parent->children, (size_t)cap * sizeof *list);
		if (!list) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
		parent->children = list;
		parent->child_cap = cap;
	}
	if (index < 0 || index > parent->child_count) index = parent->child_count;
	memmove(parent->children + index + 1, parent->children + index, (size_t)(parent->child_count - index) * sizeof *parent->children);
	parent->children[index] = child;
	parent->child_count++;
	child->parent = parent;
	{
		const Zid parent_id = parent->id;
		/* the new parent can give the subtree other inherited fonts */
		clear_measure_below(child);
		if (parent->root) zrn_root_attach(parent->root, child);
		/* a Loaded handler may have destroyed the parent */
		if (zrn_lookup(parent_id)) zrn_changed_layout(parent);
	}
}

void zrn_remove_child(zrn_element_t* const parent, zrn_element_t* const child) {
	const int index = zrn_child_index(parent, child);
	if (index < 0) return;
	if (parent->cls->child_removed) parent->cls->child_removed(parent, child);
	memmove(parent->children + index, parent->children + index + 1, (size_t)(parent->child_count - index - 1) * sizeof *parent->children);
	parent->child_count--;
	child->parent = nullptr;
	zrn_root_detach(child);
	zrn_changed_layout(parent);
}

void zrn_clear_children(zrn_element_t* const parent) {
	while (parent->child_count > 0) zrn_destroy(parent->children[parent->child_count - 1]);
}

void zrn_set_only_child(zrn_element_t* const parent, zrn_element_t** slot, zrn_element_t* const child) {
	zrn_element_t* const old = *slot;
	if (old == child) return;
	*slot = NULL;
	if (old) zrn_destroy(old);
	if (child) {
		zrn_add_child(parent, child, -1);
		if (child->parent == parent) *slot = child;
	}
	zrn_changed_layout(parent);
}

zrn_element_t** zrn_ordered(zrn_element_t* const e, int* const count, int* const heap) {
	int i, j, sorted = 1;
	zrn_element_t** list;
	*count = e->child_count;
	*heap = 0;
	for (i = 1; i < e->child_count; i++) if (e->children[i]->zindex < e->children[i - 1]->zindex) sorted = 0;
	if (sorted) return e->children;
	list = (zrn_element_t**)malloc((size_t)e->child_count * sizeof *list);
	if (!list) return e->children;
	*heap = 1;
	memcpy(list, e->children, (size_t)e->child_count * sizeof *list);
	for (i = 1; i < e->child_count; i++) {
		zrn_element_t* const x = list[i];
		for (j = i - 1; j >= 0 && list[j]->zindex > x->zindex; j--) list[j + 1] = list[j];
		list[j + 1] = x;
	}
	return list;
}

void zrn_ordered_free(zrn_element_t** list, const int heap) {
	if (heap) free(list);
}


/* ============================================================================
 *  layout
 * ============================================================================ */

void zrn_changed_layout(zrn_element_t* const e) {
	zrn_element_t* a;
	for (a = e; a; a = a->parent) a->measure_valid = 0;
	if (e->root) {
		e->root->layout_dirty = 1;
		e->root->paint_dirty = 1;
	}
}

static void clear_measure_below(zrn_element_t* const e) {
	int i;
	e->measure_valid = 0;
	for (i = 0; i < e->child_count; i++) clear_measure_below(e->children[i]);
}

void zrn_invalidate_deep(zrn_element_t* const e) {
	clear_measure_below(e);
	zrn_changed_layout(e);
}

void zrn_changed_arrange(zrn_element_t* const e) {
	if (e->root) {
		e->root->layout_dirty = 1;
		e->root->paint_dirty = 1;
	}
}

void zrn_changed_paint(zrn_element_t* const e) {
	if (e->root) e->root->paint_dirty = 1;
}

static float clamp_size(float v, float lo, float hi) {
	if (v > hi) v = hi;
	if (v < lo) v = lo;
	return v;
}

void zrn_measure(zrn_element_t* const e, const ZSize available) {
	ZSize inner, d;
	float mh, mv;
	if (e->visibility == Z_COLLAPSED) {
		e->desired = zrn_size(0, 0);
		e->measure_valid = 1;
		return;
	}
	if (e->measure_valid && e->last_available.width == available.width && e->last_available.height == available.height) return;
	mh = zrn_thickness_h(e->margin);
	mv = zrn_thickness_v(e->margin);
	inner.width = zrn_maxf(0, available.width - mh);
	inner.height = zrn_maxf(0, available.height - mv);
	if (e->width != Z_AUTO) inner.width = e->width;
	if (e->height != Z_AUTO) inner.height = e->height;
	inner.width = clamp_size(inner.width, e->min_width, e->max_width);
	inner.height = clamp_size(inner.height, e->min_height, e->max_height);
	d = e->cls->measure(e, inner);
	if (e->width != Z_AUTO) d.width = e->width;
	if (e->height != Z_AUTO) d.height = e->height;
	d.width = clamp_size(d.width, e->min_width, e->max_width);
	d.height = clamp_size(d.height, e->min_height, e->max_height);
	e->desired = zrn_size(d.width + mh, d.height + mv);
	e->last_available = available;
	e->measure_valid = 1;
}

void zrn_arrange(zrn_element_t* const e, const ZRect slot) {
	ZSize size;
	float mh, mv, x, y, w, h;
	if (e->visibility == Z_COLLAPSED) {
		e->rect = zrn_rect(slot.x, slot.y, 0, 0);
		return;
	}
	mh = zrn_thickness_h(e->margin);
	mv = zrn_thickness_v(e->margin);
	w = zrn_maxf(0, slot.width - mh);
	h = zrn_maxf(0, slot.height - mv);
	size = zrn_size(w, h);
	if (e->halign != Z_HALIGN_STRETCH) size.width = zrn_minf(size.width, zrn_maxf(0, e->desired.width - mh));
	if (e->valign != Z_VALIGN_STRETCH) size.height = zrn_minf(size.height, zrn_maxf(0, e->desired.height - mv));
	if (e->width != Z_AUTO) size.width = e->width;
	if (e->height != Z_AUTO) size.height = e->height;
	size.width = clamp_size(size.width, e->min_width, e->max_width);
	size.height = clamp_size(size.height, e->min_height, e->max_height);
	x = slot.x + e->margin.left;
	y = slot.y + e->margin.top;
	switch (e->halign) {
		case Z_HALIGN_LEFT: break;
		case Z_HALIGN_RIGHT: x += w - size.width; break;
		default: x += (w - size.width) * 0.5f; break;
	}
	switch (e->valign) {
		case Z_VALIGN_TOP: break;
		case Z_VALIGN_BOTTOM: y += h - size.height; break;
		default: y += (h - size.height) * 0.5f; break;
	}
	e->rect = zrn_rect(x, y, size.width, size.height);
	e->cls->arrange(e, size);
}

ZSize zrn_measure_children_max(zrn_element_t* const e, const ZSize available) {
	ZSize m = { 0, 0 };
	int i;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* c = e->children[i];
		zrn_measure(c, available);
		m.width = zrn_maxf(m.width, c->desired.width);
		m.height = zrn_maxf(m.height, c->desired.height);
	}
	return m;
}

int zrn_effectively_enabled(const zrn_element_t* e) {
	for (; e; e = e->parent) if (!e->enabled) return 0;
	return 1;
}

int zrn_effectively_visible(const zrn_element_t* e) {
	for (; e; e = e->parent) if (e->visibility != Z_VISIBLE) return 0;
	return 1;
}

ZPoint zrn_to_root(const zrn_element_t* e, ZPoint p) {
	for (; e; e = e->parent) {
		p.x += e->rect.x;
		p.y += e->rect.y;
	}
	return p;
}

ZPoint zrn_from_root(const zrn_element_t* e, ZPoint p) {
	for (; e; e = e->parent) {
		p.x -= e->rect.x;
		p.y -= e->rect.y;
	}
	return p;
}

zrn_element_t* zrn_hit_test(zrn_element_t* const e, const ZPoint p) {
	int count, heap, i;
	zrn_element_t** list;
	if (e->visibility != Z_VISIBLE || !e->hit_visible) return nullptr;
	if (e->clip_to_bounds && (p.x < 0 || p.y < 0 || p.x >= e->rect.width || p.y >= e->rect.height)) return nullptr;
	list = zrn_ordered(e, &count, &heap);
	for (i = count - 1; i >= 0; i--) {
		zrn_element_t* const c = list[i];
		const ZPoint q = { p.x - c->rect.x, p.y - c->rect.y };
		zrn_element_t* hit;
		if (c->is_popup_child) continue;
		hit = zrn_hit_test(c, q);
		if (hit) {
			zrn_ordered_free(list, heap);
			return hit;
		}
	}
	zrn_ordered_free(list, heap);
	if (p.x >= 0 && p.y >= 0 && p.x < e->rect.width && p.y < e->rect.height) {
		if (!e->cls->hit_self || e->cls->hit_self(e)) return e;
	}
	return nullptr;
}


/* ============================================================================
 *  events
 * ============================================================================ */

int zrn_raise(zrn_element_t* const e, const ZEvent ev) {
	const zrn_handler_t h = e->handlers[ev - Z_EVENT_CLICK];
	const Zid id = e->id;
	if (h.callback) h.callback(id, h.user_data);
	return zrn_lookup(id) != nullptr;
}

int zrn_chain(const zrn_element_t* e, Zid* ids, int max) {
	int n = 0;
	for (; e && n < max; e = e->parent) ids[n++] = e->id;
	return n;
}

void Zelement_on(const Zid id, const ZEvent event, const ZRoutedCallback callback, void* const user_data) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	if (event < Z_EVENT_CLICK || event >= Z_EVENT_LAST_) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "unknown event 0x%x", (unsigned)event);
		return;
	}
	e->handlers[event - Z_EVENT_CLICK].callback = callback;
	e->handlers[event - Z_EVENT_CLICK].user_data = user_data;
}


/* ============================================================================
 *  inherited text properties
 * ============================================================================ */

ZColor zrn_inherited_foreground(const zrn_element_t* const e) {
	const zrn_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_foreground) return p->cls->content_foreground((zrn_element_t*)p);
	return zrn_theme_color(e, ZST_TC_FOREGROUND);
}

float zrn_inherited_font_size(const zrn_element_t* const e) {
	const zrn_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_font_size) return p->cls->content_font_size((zrn_element_t*)p);
	return ZST_FONT_NORMAL;
}

ZFontFamily zrn_inherited_font_family(const zrn_element_t* const e) {
	const zrn_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_font_family) return p->cls->content_font_family((zrn_element_t*)p);
	return Z_FONT_NORMAL;
}


/* ============================================================================
 *  lifecycle
 * ============================================================================ */

static int g_init_count;

int Zinit(void) {
	if (g_init_count++ > 0) return true;
	if (!zrn_text_init()) {
		g_init_count--;
		return false;
	}
	return true;
}

void Zshutdown(void) {
	uint32_t i;
	if (g_init_count == 0) return;
	if (--g_init_count > 0) return;
	for (i = 0; i < g_slot_count; i++) {
		if (!g_slots[i].ptr) continue;
		if (g_slots[i].is_root) {
			Zroot_destroy(((zrn_root_t*)g_slots[i].ptr)->id);
		}
	}
	for (i = 0; i < g_slot_count; i++) {
		const auto e = (zrn_element_t*)g_slots[i].ptr;
		if (!e || g_slots[i].is_root) continue;
		if (!e->parent) zrn_destroy(e);
	}
	zrn_text_shutdown();
	free(g_slots);
	free(g_free);
	g_slots = nullptr;
	g_free = nullptr;
	g_slot_count = g_slot_cap = g_free_count = g_free_cap = 0;
}


/* ============================================================================
 *  public API: creation and tree
 * ============================================================================ */

Zid Zelement_create(const ZClass element_class) {
	const zrn_element_t* const e = zrn_create(element_class);
	return e ? e->id : Z_INVALID_ID;
}

void Zelement_destroy(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) zrn_destroy(e);
}

int Zelement_is_valid(Zid id) { return zrn_lookup(id) != nullptr; }

ZClass Zelement_get_class(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? e->cls->id : (ZClass)0;
}

int Zelement_is_a(const Zid id, const ZClass cls) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? zrn_is_a(e, cls) : false;
}

const char* Zelement_get_class_name(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? e->cls->name : "";
}

void Zelement_set_name(const Zid id, const char* const name) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) zrn_set_string(&e->name, name);
}

const char* Zelement_get_name(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e && e->name ? e->name : "";
}

static zrn_element_t* find_name(zrn_element_t* const e, const char* const name) {
	int i;
	if (e->name && strcmp(e->name, name) == 0) return e;
	for (i = 0; i < e->child_count; i++) {
		zrn_element_t* const f = find_name(e->children[i], name);
		if (f) return f;
	}
	return nullptr;
}

Zid Zelement_find_name(const Zid scope, const char* const name) {
	zrn_element_t* const e = zrn_get(scope, Z_CLASS_ELEMENT), *f;
	if (!e || !name) return Z_INVALID_ID;
	f = find_name(e, name);
	return f ? f->id : Z_INVALID_ID;
}

void Zelement_set_tag(const Zid id, void* const tag) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) e->tag = tag;
}

void* Zelement_get_tag(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? e->tag : nullptr;
}

Zid Zelement_get_parent(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e && e->parent ? e->parent->id : Z_INVALID_ID;
}

int Zelement_get_child_count(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? e->child_count : 0;
}

Zid Zelement_get_child(const Zid id, const int index) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e || index < 0 || index >= e->child_count) return Z_INVALID_ID;
	return e->children[index]->id;
}

Zid Zelement_get_root(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e && e->root ? e->root->id : Z_INVALID_ID;
}

ZPoint Zelement_to_root(const Zid id, const ZPoint p) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? zrn_to_root(e, p) : p;
}

ZPoint Zelement_from_root(const Zid id, const ZPoint p) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e ? zrn_from_root(e, p) : p;
}

Zid Zelement_hit_test(const Zid scope, const ZPoint p) {
	zrn_element_t* const e = zrn_get(scope, Z_CLASS_ELEMENT), *hit;
	if (!e) return Z_INVALID_ID;
	hit = zrn_hit_test(e, p);
	return hit ? hit->id : Z_INVALID_ID;
}


/* ============================================================================
 *  public API: FrameworkElement properties
 * ============================================================================ */

#define SETTER(name, type, field, layout) \
	void name(Zid id, type value) { \
		zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); \
		if (!e) return; \
		e->field = value; \
		if (layout) zrn_changed_layout(e); else zrn_changed_paint(e); \
	}

SETTER(Zelement_set_width, float, width, 1)
SETTER(Zelement_set_height, float, height, 1)
void Zelement_set_margin(const Zid id, const ZThickness value) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	zrn_unbind(e, ZST_BP_ELEMENT_MARGIN);
	e->margin = value;
	zrn_changed_layout(e);
}
SETTER(Zelement_set_horizontal_alignment, ZHorizontalAlignment, halign, 1)
SETTER(Zelement_set_vertical_alignment, ZVerticalAlignment, valign, 1)
SETTER(Zelement_set_visibility, ZVisibility, visibility, 1)
SETTER(Zelement_set_opacity, float, opacity, 0)
SETTER(Zelement_set_clip_to_bounds, int, clip_to_bounds, 0)

float Zelement_get_width(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->width : 0; }
float Zelement_get_height(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->height : 0; }
ZThickness Zelement_get_margin(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->margin : zrn_thickness_zero(); }
ZHorizontalAlignment Zelement_get_horizontal_alignment(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->halign : Z_HALIGN_STRETCH; }
ZVerticalAlignment Zelement_get_vertical_alignment(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->valign : Z_VALIGN_STRETCH; }
ZVisibility Zelement_get_visibility(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->visibility : Z_COLLAPSED; }
float Zelement_get_opacity(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->opacity : 0; }

void Zelement_set_min_size(const Zid id, const float w, const float h) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->min_width = w;
	e->min_height = h;
	zrn_changed_layout(e);
}

void Zelement_set_max_size(const Zid id, const float w, const float h) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->max_width = w;
	e->max_height = h;
	zrn_changed_layout(e);
}

float Zelement_get_actual_width(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->rect.width : 0; }
float Zelement_get_actual_height(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->rect.height : 0; }
ZSize Zelement_get_desired_size(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->desired : zrn_size(0, 0); }
ZRect Zelement_get_bounds(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->rect : zrn_rect(0, 0, 0, 0); }

void Zelement_set_enabled(const Zid id, const int enabled) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e) return;
	e->enabled = enabled != 0;
	if (!enabled && e->root && e->root->focus && zrn_effectively_enabled(e->root->focus) == 0) zrn_set_focus(e->root, nullptr);
	zrn_changed_paint(e);
}

int Zelement_get_enabled(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? e->enabled : 0; }
int Zelement_is_effectively_enabled(Zid id) { const zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); return e ? zrn_effectively_enabled(e) : 0; }

void Zelement_set_hit_test_visible(const Zid id, const int visible) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) e->hit_visible = visible != 0;
}

void Zelement_set_tooltip(const Zid id, const char* const text) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) zrn_set_string(&e->tooltip, text && *text ? text : nullptr);
}

void Zelement_invalidate_measure(Zid id) { zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); if (e) zrn_changed_layout(e); }
void Zelement_invalidate_arrange(Zid id) { zrn_element_t* e = zrn_get(id, Z_CLASS_ELEMENT); if (e) zrn_changed_layout(e); }

void Zelement_measure(const Zid id, const ZSize available) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) zrn_measure(e, available);
}

void Zelement_arrange(const Zid id, const ZRect r) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (e) zrn_arrange(e, r);
}

int Zelement_focus(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	if (!e || !e->root || !e->focusable || !zrn_effectively_enabled(e) || !zrn_effectively_visible(e)) return false;
	zrn_set_focus(e->root, e);
	/* an element that hands the keyboard on to a part of its own (AutoSuggestBox) has taken it */
	{
		const zrn_element_t* f;
		for (f = e->root->focus; f; f = f->parent) if (f == e) return true;
	}
	return false;
}

int Zelement_has_focus(const Zid id) {
	const zrn_element_t* const e = zrn_get(id, Z_CLASS_ELEMENT);
	return e && e->root && e->root->focus == e;
}
