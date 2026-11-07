/**
 * Kestrel - core.c
 * Handles, errors, classes, the visual tree, Measure / Arrange, hit testing
 * and the properties of FrameworkElement.
 */

#include "internal.h"

#include <stdarg.h>


/* ============================================================================
 *  errors
 * ============================================================================ */

static _Thread_local KError g_error = K_OK;
static _Thread_local char g_error_message[256];
static KErrorCallback g_error_callback;
static void* g_error_user_data;

void kst_error(KError error, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsnprintf(g_error_message, sizeof g_error_message, fmt, args);
	va_end(args);
	g_error = error;
	if (g_error_callback) g_error_callback(error, g_error_message, g_error_user_data);
	else fprintf(stderr, "kestrel: %s\n", g_error_message);
}

KError Kget_error(void) { return g_error; }
const char* Kget_error_message(void) { return g_error_message; }

void Kset_error_callback(KErrorCallback callback, void* user_data) {
	g_error_callback = callback;
	g_error_user_data = user_data;
}

char* kst_strdup(const char* s) {
	size_t n;
	char* d;
	if (!s) s = "";
	n = strlen(s);
	d = (char*)malloc(n + 1);
	if (d) memcpy(d, s, n + 1);
	return d;
}

void kst_set_string(char** dest, const char* s) {
	char* copy = s ? kst_strdup(s) : NULL;
	free(*dest);
	*dest = copy;
}

KColor Kcolor_rgba(float r, float g, float b, float a) { KColor c = { r, g, b, a }; return c; }

KColor Kcolor_argb(uint32_t argb) {
	KColor c;
	c.a = (float)((argb >> 24) & 255) / 255.0f;
	c.r = (float)((argb >> 16) & 255) / 255.0f;
	c.g = (float)((argb >> 8) & 255) / 255.0f;
	c.b = (float)(argb & 255) / 255.0f;
	return c;
}

KThickness Kthickness(float l, float t, float r, float b) { KThickness th = { l, t, r, b }; return th; }
KThickness Kthickness_uniform(float v) { KThickness th = { v, v, v, v }; return th; }

const char* Kget_version(void) { return "0.1.0"; }


/* ============================================================================
 *  handles
 * ============================================================================ */

#define KST_INDEX_BITS 20
#define KST_INDEX_MASK ((1u << KST_INDEX_BITS) - 1)

typedef struct kst_slot {
	void* ptr;
	uint16_t generation;
	uint8_t is_root;
} kst_slot_t;

static kst_slot_t* g_slots;
static uint32_t g_slot_count, g_slot_cap;
static uint32_t* g_free;
static uint32_t g_free_count, g_free_cap;

Kid kst_handle_alloc(void* ptr, int is_root) {
	uint32_t index;
	if (g_free_count) {
		index = g_free[--g_free_count];
	} else {
		if (g_slot_count == g_slot_cap) {
			uint32_t cap = g_slot_cap ? g_slot_cap * 2 : 256;
			kst_slot_t* slots = (kst_slot_t*)realloc(g_slots, cap * sizeof *slots);
			if (!slots) return K_INVALID_ID;
			memset(slots + g_slot_cap, 0, (cap - g_slot_cap) * sizeof *slots);
			g_slots = slots;
			g_slot_cap = cap;
		}
		index = g_slot_count++;
	}
	g_slots[index].ptr = ptr;
	g_slots[index].is_root = (uint8_t)is_root;
	if (g_slots[index].generation == 0) g_slots[index].generation = 1;
	return ((Kid)g_slots[index].generation << KST_INDEX_BITS) | (index + 1);
}

void kst_handle_free(Kid id) {
	uint32_t index = (id & KST_INDEX_MASK) - 1;
	if ((id & KST_INDEX_MASK) == 0 || index >= g_slot_count) return;
	g_slots[index].ptr = NULL;
	g_slots[index].generation = (uint16_t)((g_slots[index].generation + 1) & 0xFFF);
	if (g_slots[index].generation == 0) g_slots[index].generation = 1;
	if (g_free_count == g_free_cap) {
		uint32_t cap = g_free_cap ? g_free_cap * 2 : 64;
		uint32_t* list = (uint32_t*)realloc(g_free, cap * sizeof *list);
		if (!list) return;
		g_free = list;
		g_free_cap = cap;
	}
	g_free[g_free_count++] = index;
}

void* kst_handle_get(Kid id, int is_root) {
	uint32_t index = (id & KST_INDEX_MASK) - 1;
	if ((id & KST_INDEX_MASK) == 0 || index >= g_slot_count) return NULL;
	if (g_slots[index].ptr == NULL || g_slots[index].generation != (id >> KST_INDEX_BITS)) return NULL;
	if (g_slots[index].is_root != (uint8_t)is_root) return NULL;
	return g_slots[index].ptr;
}

kst_element_t* kst_lookup(Kid id) { return (kst_element_t*)kst_handle_get(id, 0); }

kst_element_t* kst_get(Kid id, KClass cls) {
	kst_element_t* e = kst_lookup(id);
	if (!e) {
		kst_error(K_ERROR_INVALID_ID, "invalid handle 0x%x", (unsigned)id);
		return NULL;
	}
	if (cls != K_CLASS_ELEMENT && !kst_is_a(e, cls)) {
		kst_error(K_ERROR_WRONG_CLASS, "%s is not a %s", e->cls->name, kst_class_of(cls)->name);
		return NULL;
	}
	return e;
}


/* ============================================================================
 *  classes
 * ============================================================================ */

const kst_class_t* kst_class_of(KClass id) {
	static const kst_class_t* const table[] = {
		&kst_class_element, &kst_class_panel, &kst_class_stack_panel, &kst_class_virtualizing_stack_panel,
		&kst_class_wrap_panel, &kst_class_grid, &kst_class_canvas, &kst_class_border, &kst_class_viewbox,
		&kst_class_text_block, &kst_class_image, &kst_class_popup, &kst_class_control, &kst_class_user_control,
		&kst_class_content_control, &kst_class_tooltip, &kst_class_button_base, &kst_class_button,
		&kst_class_hyperlink_button, &kst_class_repeat_button, &kst_class_toggle_button, &kst_class_checkbox,
		&kst_class_radio_button, &kst_class_range_base, &kst_class_slider, &kst_class_progress_bar,
		&kst_class_scrollbar, &kst_class_thumb, &kst_class_separator, &kst_class_scroll_viewer,
		&kst_class_text_box, &kst_class_password_box, &kst_class_items_control, &kst_class_selector,
		&kst_class_selector_item, &kst_class_list_box, &kst_class_list_box_item, &kst_class_combo_box,
		&kst_class_combo_box_item, &kst_class_auto_suggest_box, &kst_class_frame, &kst_class_page, &kst_class_pivot,
		&kst_class_toggle_switch, &kst_class_progress_ring, &kst_class_list_view_base, &kst_class_list_view,
		&kst_class_grid_view, &kst_class_list_view_item, &kst_class_grid_view_item,
		&kst_class_list_view_base_header_item, &kst_class_list_view_header_item, &kst_class_grid_view_header_item,
		&kst_class_flip_view, &kst_class_flip_view_item, &kst_class_items_stack_panel, &kst_class_items_wrap_grid,
		&kst_class_semantic_zoom, &kst_class_icon_element, &kst_class_symbol_icon, &kst_class_font_icon, &kst_class_path_icon,
		&kst_class_bitmap_icon, &kst_class_app_bar, &kst_class_command_bar, &kst_class_app_bar_button,
		&kst_class_app_bar_toggle_button, &kst_class_app_bar_separator, &kst_class_flyout_base, &kst_class_flyout,
		&kst_class_menu_flyout, &kst_class_menu_flyout_item_base, &kst_class_menu_flyout_item,
		&kst_class_toggle_menu_flyout_item, &kst_class_menu_flyout_separator, &kst_class_content_dialog,
		&kst_class_pivot_item, &kst_class_hub, &kst_class_hub_section, &kst_class_looping_selector, &kst_class_date_picker,
		&kst_class_time_picker, &kst_class_picker_flyout_base, &kst_class_date_picker_flyout, &kst_class_time_picker_flyout,
		&kst_class_rich_text_block
	};
	unsigned index = (unsigned)id - (unsigned)K_CLASS_ELEMENT;
	if (index >= sizeof table / sizeof table[0]) return NULL;
	return table[index];
}

/* Copies the slots a class left empty from its base, base first. */
static void kst_class_resolve(const kst_class_t* const_cls) {
	kst_class_t* cls = (kst_class_t*)const_cls;
	const kst_class_t* base = cls->base;
	if (cls->resolved) return;
	cls->resolved = 1;
	if (!base) return;
	kst_class_resolve(base);
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

int kst_class_derives(const kst_class_t* cls, const kst_class_t* base) {
	for (; cls; cls = cls->base) if (cls == base) return 1;
	return 0;
}

int kst_is_a(const kst_element_t* e, KClass id) {
	const kst_class_t* cls = kst_class_of(id);
	return cls && kst_class_derives(e->cls, cls);
}


/* ============================================================================
 *  default theme for elements outside a root
 * ============================================================================ */

static kst_theme_t g_default_theme;
static int g_default_theme_ready;

KColor kst_theme_color(const kst_element_t* e, kst_theme_color_id_t id) {
	if (e && e->root) return e->root->theme.colors[id];
	if (!g_default_theme_ready) {
		kst_theme_init(&g_default_theme, K_THEME_DARK, kst_accent_color(K_ACCENT_COBALT));
		g_default_theme_ready = 1;
	}
	return g_default_theme.colors[id];
}


/* ============================================================================
 *  element creation and destruction
 * ============================================================================ */

static void kst_run_init(kst_element_t* e, const kst_class_t* cls) {
	if (cls->base) kst_run_init(e, cls->base);
	if (cls->init) cls->init(e);
}

kst_element_t* kst_create(KClass id) {
	const kst_class_t* cls = kst_class_of(id);
	kst_element_t* e;
	if (!cls) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "unknown class 0x%x", (unsigned)id);
		return NULL;
	}
	if (cls->abstract_class) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "%s is abstract", cls->name);
		return NULL;
	}
	kst_class_resolve(cls);
	e = (kst_element_t*)calloc(1, cls->size);
	if (!e) {
		kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
		return NULL;
	}
	e->cls = cls;
	e->id = kst_handle_alloc(e, 0);
	if (e->id == K_INVALID_ID) {
		free(e);
		kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
		return NULL;
	}
	kst_run_init(e, cls);
	/* what leans when pressed, as in the Windows Phone toolkit: not a Button, which has a pressed look of its own */
	e->tilt = (kst_is_a(e, K_CLASS_SELECTOR_ITEM) && !kst_is_a(e, K_CLASS_FLIP_VIEW_ITEM)) ||
		kst_is_a(e, K_CLASS_LIST_VIEW_BASE_HEADER_ITEM) || kst_is_a(e, K_CLASS_HYPERLINK_BUTTON) ||
		kst_is_a(e, K_CLASS_CHECKBOX) || kst_is_a(e, K_CLASS_RADIO_BUTTON) || kst_is_a(e, K_CLASS_MENU_FLYOUT_ITEM);
	return e;
}

static void kst_run_destroy(kst_element_t* e) {
	const kst_class_t* cls;
	for (cls = e->cls; cls; cls = cls->base) if (cls->destroy) cls->destroy(e);
}

void kst_destroy(kst_element_t* e) {
	if (!e) return;
	e->destroying = 1;
	if (e->parent) kst_remove_child(e->parent, e);
	kst_root_detach(e);
	while (e->child_count > 0) kst_destroy(e->children[e->child_count - 1]);
	kst_run_destroy(e);
	kst_handle_free(e->id);
	kst_tweens_free(e);
	free(e->children);
	free(e->name);
	free(e->tooltip);
	free(e);
}

/* The base of the hierarchy: UIElement + FrameworkElement defaults. */
static void element_init(kst_element_t* e) {
	e->width = K_AUTO;
	e->height = K_AUTO;
	e->min_width = 0;
	e->min_height = 0;
	e->max_width = K_INFINITE;
	e->max_height = K_INFINITE;
	e->halign = K_HALIGN_STRETCH;
	e->valign = K_VALIGN_STRETCH;
	e->visibility = K_VISIBLE;
	e->opacity = 1.0f;
	e->enabled = 1;
	e->hit_visible = 1;
	e->grid_row_span = 1;
	e->grid_column_span = 1;
	e->render_scale = 1.0f;
	e->proj_cx = 0.5f;
	e->proj_cy = 0.5f;
	e->canvas_left = K_AUTO;
	e->canvas_top = K_AUTO;
	e->tab_stop = 1;
}

static KSize element_measure(kst_element_t* e, KSize available) {
	KST_UNUSED(e); KST_UNUSED(available);
	return kst_size(0, 0);
}

static KSize element_arrange(kst_element_t* e, KSize final) {
	KST_UNUSED(e);
	return final;
}

kst_class_t kst_class_element = {
	.base = NULL, .name = "Element", .id = K_CLASS_ELEMENT, .size = sizeof(kst_element_t),
	.init = element_init, .measure = element_measure, .arrange = element_arrange
};


static void clear_measure_below(kst_element_t* e);

/* ============================================================================
 *  visual tree
 * ============================================================================ */

int kst_child_index(const kst_element_t* parent, const kst_element_t* child) {
	int i;
	for (i = 0; i < parent->child_count; i++) if (parent->children[i] == child) return i;
	return -1;
}

void kst_add_child(kst_element_t* parent, kst_element_t* child, int index) {
	kst_element_t* a;
	for (a = parent; a; a = a->parent) {
		if (a == child) {
			kst_error(K_ERROR_CYCLE, "an element cannot be added under itself");
			return;
		}
	}
	if (child->parent) kst_remove_child(child->parent, child);
	if (parent->child_count == parent->child_cap) {
		int cap = parent->child_cap ? parent->child_cap * 2 : 4;
		kst_element_t** list = (kst_element_t**)realloc(parent->children, (size_t)cap * sizeof *list);
		if (!list) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
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
		Kid parent_id = parent->id;
		/* the new parent can give the subtree other inherited fonts */
		clear_measure_below(child);
		if (parent->root) kst_root_attach(parent->root, child);
		/* a Loaded handler may have destroyed the parent */
		if (kst_lookup(parent_id)) kst_changed_layout(parent);
	}
}

void kst_remove_child(kst_element_t* parent, kst_element_t* child) {
	int index = kst_child_index(parent, child);
	if (index < 0) return;
	if (parent->cls->child_removed) parent->cls->child_removed(parent, child);
	memmove(parent->children + index, parent->children + index + 1, (size_t)(parent->child_count - index - 1) * sizeof *parent->children);
	parent->child_count--;
	child->parent = NULL;
	kst_root_detach(child);
	kst_changed_layout(parent);
}

void kst_clear_children(kst_element_t* parent) {
	while (parent->child_count > 0) kst_destroy(parent->children[parent->child_count - 1]);
}

void kst_set_only_child(kst_element_t* parent, kst_element_t** slot, kst_element_t* child) {
	kst_element_t* old = *slot;
	if (old == child) return;
	*slot = NULL;
	if (old) kst_destroy(old);
	if (child) {
		kst_add_child(parent, child, -1);
		if (child->parent == parent) *slot = child;
	}
	kst_changed_layout(parent);
}

kst_element_t** kst_ordered(kst_element_t* e, int* count, int* heap) {
	int i, j, sorted = 1;
	kst_element_t** list;
	*count = e->child_count;
	*heap = 0;
	for (i = 1; i < e->child_count; i++) if (e->children[i]->zindex < e->children[i - 1]->zindex) sorted = 0;
	if (sorted) return e->children;
	list = (kst_element_t**)malloc((size_t)e->child_count * sizeof *list);
	if (!list) return e->children;
	*heap = 1;
	memcpy(list, e->children, (size_t)e->child_count * sizeof *list);
	for (i = 1; i < e->child_count; i++) {
		kst_element_t* x = list[i];
		for (j = i - 1; j >= 0 && list[j]->zindex > x->zindex; j--) list[j + 1] = list[j];
		list[j + 1] = x;
	}
	return list;
}

void kst_ordered_free(kst_element_t** list, int heap) {
	if (heap) free(list);
}


/* ============================================================================
 *  layout
 * ============================================================================ */

void kst_changed_layout(kst_element_t* e) {
	kst_element_t* a;
	for (a = e; a; a = a->parent) a->measure_valid = 0;
	if (e->root) {
		e->root->layout_dirty = 1;
		e->root->paint_dirty = 1;
	}
}

static void clear_measure_below(kst_element_t* e) {
	int i;
	e->measure_valid = 0;
	for (i = 0; i < e->child_count; i++) clear_measure_below(e->children[i]);
}

void kst_invalidate_deep(kst_element_t* e) {
	clear_measure_below(e);
	kst_changed_layout(e);
}

void kst_changed_arrange(kst_element_t* e) {
	if (e->root) {
		e->root->layout_dirty = 1;
		e->root->paint_dirty = 1;
	}
}

void kst_changed_paint(kst_element_t* e) {
	if (e->root) e->root->paint_dirty = 1;
}

static float clamp_size(float v, float lo, float hi) {
	if (v > hi) v = hi;
	if (v < lo) v = lo;
	return v;
}

void kst_measure(kst_element_t* e, KSize available) {
	KSize inner, d;
	float mh, mv;
	if (e->visibility == K_COLLAPSED) {
		e->desired = kst_size(0, 0);
		e->measure_valid = 1;
		return;
	}
	if (e->measure_valid && e->last_available.width == available.width && e->last_available.height == available.height) return;
	mh = kst_thickness_h(e->margin);
	mv = kst_thickness_v(e->margin);
	inner.width = kst_maxf(0, available.width - mh);
	inner.height = kst_maxf(0, available.height - mv);
	if (e->width != K_AUTO) inner.width = e->width;
	if (e->height != K_AUTO) inner.height = e->height;
	inner.width = clamp_size(inner.width, e->min_width, e->max_width);
	inner.height = clamp_size(inner.height, e->min_height, e->max_height);
	d = e->cls->measure(e, inner);
	if (e->width != K_AUTO) d.width = e->width;
	if (e->height != K_AUTO) d.height = e->height;
	d.width = clamp_size(d.width, e->min_width, e->max_width);
	d.height = clamp_size(d.height, e->min_height, e->max_height);
	e->desired = kst_size(d.width + mh, d.height + mv);
	e->last_available = available;
	e->measure_valid = 1;
}

void kst_arrange(kst_element_t* e, KRect slot) {
	KSize size;
	float mh, mv, x, y, w, h;
	if (e->visibility == K_COLLAPSED) {
		e->rect = kst_rect(slot.x, slot.y, 0, 0);
		return;
	}
	mh = kst_thickness_h(e->margin);
	mv = kst_thickness_v(e->margin);
	w = kst_maxf(0, slot.width - mh);
	h = kst_maxf(0, slot.height - mv);
	size = kst_size(w, h);
	if (e->halign != K_HALIGN_STRETCH) size.width = kst_minf(size.width, kst_maxf(0, e->desired.width - mh));
	if (e->valign != K_VALIGN_STRETCH) size.height = kst_minf(size.height, kst_maxf(0, e->desired.height - mv));
	if (e->width != K_AUTO) size.width = e->width;
	if (e->height != K_AUTO) size.height = e->height;
	size.width = clamp_size(size.width, e->min_width, e->max_width);
	size.height = clamp_size(size.height, e->min_height, e->max_height);
	x = slot.x + e->margin.left;
	y = slot.y + e->margin.top;
	switch (e->halign) {
		case K_HALIGN_LEFT: break;
		case K_HALIGN_RIGHT: x += w - size.width; break;
		default: x += (w - size.width) * 0.5f; break;
	}
	switch (e->valign) {
		case K_VALIGN_TOP: break;
		case K_VALIGN_BOTTOM: y += h - size.height; break;
		default: y += (h - size.height) * 0.5f; break;
	}
	e->rect = kst_rect(x, y, size.width, size.height);
	e->cls->arrange(e, size);
}

KSize kst_measure_children_max(kst_element_t* e, KSize available) {
	KSize m = { 0, 0 };
	int i;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* c = e->children[i];
		kst_measure(c, available);
		m.width = kst_maxf(m.width, c->desired.width);
		m.height = kst_maxf(m.height, c->desired.height);
	}
	return m;
}

int kst_effectively_enabled(const kst_element_t* e) {
	for (; e; e = e->parent) if (!e->enabled) return 0;
	return 1;
}

int kst_effectively_visible(const kst_element_t* e) {
	for (; e; e = e->parent) if (e->visibility != K_VISIBLE) return 0;
	return 1;
}

KPoint kst_to_root(const kst_element_t* e, KPoint p) {
	for (; e; e = e->parent) {
		p.x += e->rect.x;
		p.y += e->rect.y;
	}
	return p;
}

KPoint kst_from_root(const kst_element_t* e, KPoint p) {
	for (; e; e = e->parent) {
		p.x -= e->rect.x;
		p.y -= e->rect.y;
	}
	return p;
}

kst_element_t* kst_hit_test(kst_element_t* e, KPoint p) {
	int count, heap, i;
	kst_element_t** list;
	if (e->visibility != K_VISIBLE || !e->hit_visible) return NULL;
	if (e->clip_to_bounds && (p.x < 0 || p.y < 0 || p.x >= e->rect.width || p.y >= e->rect.height)) return NULL;
	list = kst_ordered(e, &count, &heap);
	for (i = count - 1; i >= 0; i--) {
		kst_element_t* c = list[i];
		KPoint q = { p.x - c->rect.x, p.y - c->rect.y };
		kst_element_t* hit;
		if (c->is_popup_child) continue;
		hit = kst_hit_test(c, q);
		if (hit) {
			kst_ordered_free(list, heap);
			return hit;
		}
	}
	kst_ordered_free(list, heap);
	if (p.x >= 0 && p.y >= 0 && p.x < e->rect.width && p.y < e->rect.height) {
		if (!e->cls->hit_self || e->cls->hit_self(e)) return e;
	}
	return NULL;
}


/* ============================================================================
 *  events
 * ============================================================================ */

int kst_raise(kst_element_t* e, KEvent ev) {
	kst_handler_t h = e->handlers[ev - K_EVENT_CLICK];
	Kid id = e->id;
	if (h.callback) h.callback(id, h.user_data);
	return kst_lookup(id) != NULL;
}

int kst_chain(const kst_element_t* e, Kid* ids, int max) {
	int n = 0;
	for (; e && n < max; e = e->parent) ids[n++] = e->id;
	return n;
}

void Kelement_on(Kid id, KEvent event, KRoutedCallback callback, void* user_data) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	if (event < K_EVENT_CLICK || event >= K_EVENT_LAST_) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "unknown event 0x%x", (unsigned)event);
		return;
	}
	e->handlers[event - K_EVENT_CLICK].callback = callback;
	e->handlers[event - K_EVENT_CLICK].user_data = user_data;
}


/* ============================================================================
 *  inherited text properties
 * ============================================================================ */

KColor kst_inherited_foreground(const kst_element_t* e) {
	const kst_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_foreground) return p->cls->content_foreground((kst_element_t*)p);
	return kst_theme_color(e, KST_TC_FOREGROUND);
}

float kst_inherited_font_size(const kst_element_t* e) {
	const kst_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_font_size) return p->cls->content_font_size((kst_element_t*)p);
	return KST_FONT_NORMAL;
}

KFontFamily kst_inherited_font_family(const kst_element_t* e) {
	const kst_element_t* p;
	for (p = e->parent; p; p = p->parent) if (p->cls->content_font_family) return p->cls->content_font_family((kst_element_t*)p);
	return K_FONT_NORMAL;
}


/* ============================================================================
 *  lifecycle
 * ============================================================================ */

static int g_init_count;

int Kinit(void) {
	if (g_init_count++ > 0) return K_TRUE;
	if (!kst_text_init()) {
		g_init_count--;
		return K_FALSE;
	}
	return K_TRUE;
}

void Kshutdown(void) {
	uint32_t i;
	if (g_init_count == 0) return;
	if (--g_init_count > 0) return;
	for (i = 0; i < g_slot_count; i++) {
		if (!g_slots[i].ptr) continue;
		if (g_slots[i].is_root) {
			Kroot_destroy(((kst_root_t*)g_slots[i].ptr)->id);
		}
	}
	for (i = 0; i < g_slot_count; i++) {
		kst_element_t* e = (kst_element_t*)g_slots[i].ptr;
		if (!e || g_slots[i].is_root) continue;
		if (!e->parent) kst_destroy(e);
	}
	kst_text_shutdown();
	free(g_slots);
	free(g_free);
	g_slots = NULL;
	g_free = NULL;
	g_slot_count = g_slot_cap = g_free_count = g_free_cap = 0;
}


/* ============================================================================
 *  public API: creation and tree
 * ============================================================================ */

Kid Kelement_create(KClass element_class) {
	kst_element_t* e = kst_create(element_class);
	return e ? e->id : K_INVALID_ID;
}

void Kelement_destroy(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) kst_destroy(e);
}

int Kelement_is_valid(Kid id) { return kst_lookup(id) != NULL; }

KClass Kelement_get_class(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? e->cls->id : (KClass)0;
}

int Kelement_is_a(Kid id, KClass cls) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? kst_is_a(e, cls) : K_FALSE;
}

const char* Kelement_get_class_name(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? e->cls->name : "";
}

void Kelement_set_name(Kid id, const char* name) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) kst_set_string(&e->name, name);
}

const char* Kelement_get_name(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e && e->name ? e->name : "";
}

static kst_element_t* find_name(kst_element_t* e, const char* name) {
	int i;
	if (e->name && strcmp(e->name, name) == 0) return e;
	for (i = 0; i < e->child_count; i++) {
		kst_element_t* f = find_name(e->children[i], name);
		if (f) return f;
	}
	return NULL;
}

Kid Kelement_find_name(Kid scope, const char* name) {
	kst_element_t* e = kst_get(scope, K_CLASS_ELEMENT), *f;
	if (!e || !name) return K_INVALID_ID;
	f = find_name(e, name);
	return f ? f->id : K_INVALID_ID;
}

void Kelement_set_tag(Kid id, void* tag) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) e->tag = tag;
}

void* Kelement_get_tag(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? e->tag : NULL;
}

Kid Kelement_get_parent(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e && e->parent ? e->parent->id : K_INVALID_ID;
}

int Kelement_get_child_count(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? e->child_count : 0;
}

Kid Kelement_get_child(Kid id, int index) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e || index < 0 || index >= e->child_count) return K_INVALID_ID;
	return e->children[index]->id;
}

Kid Kelement_get_root(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e && e->root ? e->root->id : K_INVALID_ID;
}

KPoint Kelement_to_root(Kid id, KPoint p) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? kst_to_root(e, p) : p;
}

KPoint Kelement_from_root(Kid id, KPoint p) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e ? kst_from_root(e, p) : p;
}

Kid Kelement_hit_test(Kid scope, KPoint p) {
	kst_element_t* e = kst_get(scope, K_CLASS_ELEMENT), *hit;
	if (!e) return K_INVALID_ID;
	hit = kst_hit_test(e, p);
	return hit ? hit->id : K_INVALID_ID;
}


/* ============================================================================
 *  public API: FrameworkElement properties
 * ============================================================================ */

#define SETTER(name, type, field, layout) \
	void name(Kid id, type value) { \
		kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); \
		if (!e) return; \
		e->field = value; \
		if (layout) kst_changed_layout(e); else kst_changed_paint(e); \
	}

SETTER(Kelement_set_width, float, width, 1)
SETTER(Kelement_set_height, float, height, 1)
SETTER(Kelement_set_margin, KThickness, margin, 1)
SETTER(Kelement_set_horizontal_alignment, KHorizontalAlignment, halign, 1)
SETTER(Kelement_set_vertical_alignment, KVerticalAlignment, valign, 1)
SETTER(Kelement_set_visibility, KVisibility, visibility, 1)
SETTER(Kelement_set_opacity, float, opacity, 0)
SETTER(Kelement_set_clip_to_bounds, int, clip_to_bounds, 0)

float Kelement_get_width(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->width : 0; }
float Kelement_get_height(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->height : 0; }
KThickness Kelement_get_margin(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->margin : kst_thickness_zero(); }
KHorizontalAlignment Kelement_get_horizontal_alignment(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->halign : K_HALIGN_STRETCH; }
KVerticalAlignment Kelement_get_vertical_alignment(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->valign : K_VALIGN_STRETCH; }
KVisibility Kelement_get_visibility(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->visibility : K_COLLAPSED; }
float Kelement_get_opacity(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->opacity : 0; }

void Kelement_set_min_size(Kid id, float w, float h) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	e->min_width = w;
	e->min_height = h;
	kst_changed_layout(e);
}

void Kelement_set_max_size(Kid id, float w, float h) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	e->max_width = w;
	e->max_height = h;
	kst_changed_layout(e);
}

float Kelement_get_actual_width(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->rect.width : 0; }
float Kelement_get_actual_height(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->rect.height : 0; }
KSize Kelement_get_desired_size(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->desired : kst_size(0, 0); }
KRect Kelement_get_bounds(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->rect : kst_rect(0, 0, 0, 0); }

void Kelement_set_enabled(Kid id, int enabled) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e) return;
	e->enabled = enabled != 0;
	if (!enabled && e->root && e->root->focus && kst_effectively_enabled(e->root->focus) == 0) kst_set_focus(e->root, NULL);
	kst_changed_paint(e);
}

int Kelement_get_enabled(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? e->enabled : 0; }
int Kelement_is_effectively_enabled(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); return e ? kst_effectively_enabled(e) : 0; }

void Kelement_set_hit_test_visible(Kid id, int visible) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) e->hit_visible = visible != 0;
}

void Kelement_set_tooltip(Kid id, const char* text) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) kst_set_string(&e->tooltip, text && *text ? text : NULL);
}

void Kelement_invalidate_measure(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); if (e) kst_changed_layout(e); }
void Kelement_invalidate_arrange(Kid id) { kst_element_t* e = kst_get(id, K_CLASS_ELEMENT); if (e) kst_changed_layout(e); }

void Kelement_measure(Kid id, KSize available) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) kst_measure(e, available);
}

void Kelement_arrange(Kid id, KRect r) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (e) kst_arrange(e, r);
}

int Kelement_focus(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	if (!e || !e->root || !e->focusable || !kst_effectively_enabled(e) || !kst_effectively_visible(e)) return K_FALSE;
	kst_set_focus(e->root, e);
	/* an element that hands the keyboard on to a part of its own (AutoSuggestBox) has taken it */
	{
		const kst_element_t* f;
		for (f = e->root->focus; f; f = f->parent) if (f == e) return K_TRUE;
	}
	return K_FALSE;
}

int Kelement_has_focus(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_ELEMENT);
	return e && e->root && e->root->focus == e;
}
