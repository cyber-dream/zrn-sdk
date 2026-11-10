/**
 * zrnui - controls/items.c
 * ItemsControl, Selector, SelectorItem (the containers) and ListBox. ListView and GridView are in
 * listview.c, ComboBox and AutoSuggestBox in combo.c, FlipView and SemanticZoom in flip.c; they
 * share what is here: the items, the selection, the keys of a list and the scrolling to an item.
 *
 * The items are containers (ListBoxItem, ListViewItem...) in an items panel, `host`. A ListBox
 * shows the panel in a ScrollViewer, with a Border look of its own (generic.xaml: PhoneBaseMediumHigh
 * with black text, a 1.5 px border that is transparent).
 */

#include "../internal.h"

#define IC(e) ZST_AS(zrn_items_control_t, e)
#define ITEM(e) ZST_AS(zrn_selector_item_t, e)
#define CTL(e) ZST_AS(zrn_control_t, e)

/* ListBoxItem: Padding 6.5,8; ComboBoxItem: Padding 6.5,8.5,0,16; ListViewItem and GridViewItem: ControlContentThemeFontSize 12 */
static constexpr float ITEM_FONT_SIZE = 12.0f;
/* ListViewItemContentTranslateX: how far the content moves to the right for the check box of the multiple selection */
static constexpr float CHECK_SHIFT = 40.5f;
static constexpr float CHECK_BOX = 25.5f;
static constexpr float CHECK_BOX_TOP = 9.5f;
static constexpr float EARMARK = 34.0f;


/* ============================================================================
 *  the items of a control
 * ============================================================================ */

static int is_header_item(const zrn_element_t* e) { return zrn_is_a(e, Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM); }

zrn_items_control_t* zrn_item_owner(zrn_element_t* const item) {
	zrn_element_t* a;
	for (a = item->parent; a; a = a->parent) if (zrn_is_a(a, Z_CLASS_ITEMS_CONTROL)) return IC(a);
	return nullptr;
}

int zrn_ic_count(const zrn_items_control_t* const o) {
	int n, i;
	if (!o->host) return 0;
	if (o->group_count == 0) return o->host->child_count;
	for (n = 0, i = 0; i < o->host->child_count; i++) if (!is_header_item(o->host->children[i])) n++;
	return n;
}

zrn_element_t* zrn_ic_item(const zrn_items_control_t* const o, const int index) {
	int n = 0, i;
	if (!o->host || index < 0) return nullptr;
	if (o->group_count == 0) return index < o->host->child_count ? o->host->children[index] : nullptr;
	for (i = 0; i < o->host->child_count; i++) {
		if (is_header_item(o->host->children[i])) continue;
		if (n++ == index) return o->host->children[i];
	}
	return nullptr;
}

int zrn_ic_index_of(const zrn_items_control_t* const o, const zrn_element_t* const container) {
	int n = 0, i;
	if (!o->host || container->parent != o->host || is_header_item(container)) return -1;
	for (i = 0; i < o->host->child_count; i++) {
		if (o->host->children[i] == container) return n;
		if (o->group_count == 0 || !is_header_item(o->host->children[i])) n++;
	}
	return -1;
}

static int is_selector(const zrn_items_control_t* o) { return zrn_is_a(&o->base.base, Z_CLASS_SELECTOR); }

static int is_container(const zrn_element_t* e) { return zrn_is_a(e, Z_CLASS_SELECTOR_ITEM); }

void zrn_item_set_selected(zrn_element_t* const item, const int selected) {
	if (ITEM(item)->selected == selected) return;
	ITEM(item)->selected = selected;
	zrn_changed_paint(item);
}

#define item_set_selected zrn_item_set_selected

/** The container class an items control makes for its items. */
static ZClass container_class(const zrn_items_control_t* const o) {
	const zrn_element_t* e = &o->base.base;
	if (zrn_is_a(e, Z_CLASS_COMBO_BOX)) return Z_CLASS_COMBO_BOX_ITEM;
	if (zrn_is_a(e, Z_CLASS_FLIP_VIEW)) return Z_CLASS_FLIP_VIEW_ITEM;
	if (zrn_is_a(e, Z_CLASS_GRID_VIEW)) return Z_CLASS_GRID_VIEW_ITEM;
	if (zrn_is_a(e, Z_CLASS_LIST_VIEW) || zrn_is_a(e, Z_CLASS_AUTO_SUGGEST_BOX)) return Z_CLASS_LIST_VIEW_ITEM;
	return Z_CLASS_LIST_BOX_ITEM;
}

static int multi_check(const zrn_items_control_t* const o) {
	return o && zrn_is_a(&o->base.base, Z_CLASS_LIST_VIEW_BASE) && (o->sel_mode == ZST_SEL_MULTIPLE || o->sel_mode == ZST_SEL_EXTENDED);
}


/* ============================================================================
 *  the containers
 * ============================================================================ */

static ZThickness item_chrome(zrn_element_t* const e) {
	const zrn_items_control_t* const o = zrn_item_owner(e);
	if (zrn_is_a(e, Z_CLASS_LIST_VIEW_ITEM) && multi_check(o)) return Zthickness(CHECK_SHIFT, 0, 0, 0);
	return zrn_thickness_zero();
}

/* The check box of a list item is 25.5 high with 9.5 above it: the row is at least that high. */
static ZSize item_measure(zrn_element_t* const e, const ZSize available) {
	ZSize s = zrn_cc_measure(e, available);
	if (zrn_is_a(e, Z_CLASS_LIST_VIEW_ITEM) && multi_check(zrn_item_owner(e))) s.height = zrn_maxf(s.height, CHECK_BOX_TOP + CHECK_BOX);
	return s;
}

static int item_focused(const zrn_element_t* const e, const zrn_items_control_t* const o) {
	return o && o->focus_visible && o->focus_item == e->id && e->root && e->root->focus == &o->base.base;
}

static float item_font_size(zrn_element_t* const e) {
	const zrn_control_t* const c = CTL(e);
	const zrn_items_control_t* const o = zrn_item_owner(e);
	if (!(c->set_flags & ZST_C_FONT_SIZE) && o && (o->base.set_flags & ZST_C_FONT_SIZE)) return o->base.font_size;
	return c->font_size;
}

static ZFontFamily item_font_family(zrn_element_t* const e) {
	const zrn_control_t* const c = CTL(e);
	const zrn_items_control_t* const o = zrn_item_owner(e);
	if (!(c->set_flags & ZST_C_FONT_FAMILY) && o && (o->base.set_flags & ZST_C_FONT_FAMILY)) return o->base.font_family;
	return c->font_family;
}

static ZColor item_content_foreground(zrn_element_t* const e) {
	const auto it = ITEM(e);
	const zrn_control_t* const c = &it->base.base;
	const zrn_items_control_t* const o = zrn_item_owner(e);
	const int enabled = zrn_effectively_enabled(e);
	ZColor fg;
	if (zrn_is_a(e, Z_CLASS_LIST_BOX_ITEM)) {
		/* ListBoxItem: the Foreground of the ListBox is PhoneBaseBlack (Dark; Light has the white that reads on its dark box), selected is PhoneBaseWhite on the accent */
		if (!enabled) return zrn_theme_color(e, it->selected ? ZST_TC_BASE_LOW : ZST_TC_BASE_MEDIUM_MID);
		if (it->selected) return zrn_theme_color(e, ZST_TC_BASE_WHITE);
		if (c->set_flags & ZST_C_FOREGROUND) return c->foreground;
		if (o && (o->base.set_flags & ZST_C_FOREGROUND)) return o->base.foreground;
		return zrn_theme_color(e, ZST_TC_LIST_BOX_FOREGROUND);
	}
	if (c->set_flags & ZST_C_FOREGROUND) fg = c->foreground;
	else if (o && (o->base.set_flags & ZST_C_FOREGROUND)) fg = o->base.foreground;
	/* the list of a ComboBox is white while it is open (ComboBoxHighlightedForeground) */
	else if (zrn_is_a(e, Z_CLASS_COMBO_BOX_ITEM)) fg = zrn_theme_color(e, ZST_TC_BASE_BLACK);
	else fg = zrn_theme_color(e, ZST_TC_FOREGROUND);
	if (zrn_is_a(e, Z_CLASS_COMBO_BOX_ITEM) && it->selected) fg = zrn_theme_color(e, ZST_TC_ACCENT);
	/* Disabled: the content is at ListViewItemDisabledThemeOpacity / ComboBoxItemDisabledThemeOpacity 0.4 */
	if (!enabled) fg = zrn_color_alpha(fg, 0.4f);
	return fg;
}

static void check_glyph(zrn_painter_t* const p, const float x, const float y, const float w, const float h, const ZColor color) {
	/* Path "M0,123 L39,93 L124,164 L256,18 L295,49 L124,240 z", Stretch Fill */
	static const float path[12] = { 0, 123, 39, 93, 124, 164, 256, 18, 295, 49, 124, 240 };
	float pts[12];
	int i;
	for (i = 0; i < 6; i++) {
		pts[i * 2] = x + path[i * 2] / 295.0f * w;
		pts[i * 2 + 1] = y + (path[i * 2 + 1] - 18.0f) / 222.0f * h;
	}
	zrn_fill_polygon(p, pts, 6, color);
}

static void item_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto it = ITEM(e);
	const zrn_control_t* const c = &it->base.base;
	const zrn_items_control_t* const o = zrn_item_owner(e);
	const int enabled = zrn_effectively_enabled(e);
	const ZRect r = zrn_local_rect(e);
	ZColor bg = zrn_ctl_background(c, Zcolor_rgba(0, 0, 0, 0));
	if (zrn_is_a(e, Z_CLASS_LIST_BOX_ITEM)) {
		/* InnerGrid: the accent when selected, the PressedBackground rectangle while pressed */
		if (it->selected) bg = zrn_theme_color(e, enabled ? ZST_TC_ACCENT : ZST_TC_PHONE_DISABLED);
		else if (it->pressed && enabled) bg = zrn_theme_color(e, ZST_TC_BASE_MEDIUM_HIGH);
		else if (!enabled) bg = Zcolor_rgba(0, 0, 0, 0);
	}
	if (bg.a > 0) zrn_fill_rect(p, r, bg);
	if (zrn_is_a(e, Z_CLASS_LIST_VIEW_ITEM) && multi_check(o)) {
		/* the check box of ListMultiSelect: a 25.5 square at the left, 9.5 from the top */
		const ZRect box = zrn_rect(0, CHECK_BOX_TOP, CHECK_BOX, CHECK_BOX);
		const ZColor line = zrn_theme_color(e, enabled ? ZST_TC_FOREGROUND : ZST_TC_BASE_LOW);
		zrn_stroke_rect(p, box, Zthickness_uniform(ZST_81_BORDER_THICKNESS), line);
		if (it->selected) check_glyph(p, box.x + (CHECK_BOX - 18.5f) * 0.5f, box.y + (CHECK_BOX - 17.0f) * 0.5f, 18.5f, 17.0f, line);
	} else if (zrn_is_a(e, Z_CLASS_GRID_VIEW_ITEM) && multi_check(o) && it->selected && enabled) {
		/* GridMultiSelect: SelectedBorder of 2.5 in the accent and the earmark with the check in the top right corner */
		const ZColor accent = zrn_theme_color(e, ZST_TC_ACCENT);
		const float tri[6] = { r.width - EARMARK, 0, r.width, 0, r.width, EARMARK };
		zrn_stroke_rect(p, r, Zthickness_uniform(2.5f), accent);
		zrn_fill_polygon(p, tri, 3, accent);
		check_glyph(p, r.width - 1.0f - 17.0f, 1.0f, 17.0f, 14.5f, zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT));
	}
	if (item_focused(e, o)) zrn_stroke_rect(p, zrn_rect(1, 1, r.width - 2, r.height - 2), Zthickness_uniform(1), zrn_theme_color(e, ZST_TC_BASE_MID));
}

/** What a tap on a container does, by the control it is in. */
static void item_tapped(zrn_element_t* const e) {
	zrn_items_control_t* const o = zrn_item_owner(e);
	int index;
	if (!o || e->parent != o->host) return;
	index = zrn_ic_index_of(o, e);
	if (index < 0) return;
	o->focus_visible = 0;
	if (zrn_is_a(&o->base.base, Z_CLASS_COMBO_BOX)) zrn_combo_item_tapped(o, index);
	else if (zrn_is_a(&o->base.base, Z_CLASS_LIST_VIEW_BASE)) zrn_lvb_item_tapped(o, index);
	else if (zrn_is_a(&o->base.base, Z_CLASS_FLIP_VIEW)) return;
	else {
		o->focus_item = e->id;
		zrn_ic_tap(o, index);
	}
}

static int item_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto it = ITEM(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			/* the container is enabled by itself, but its control is not */
			if (!zrn_effectively_enabled(e)) return 1;
			it->pressed = 1;
			it->press_x = in->x;
			it->press_y = in->y;
			zrn_changed_paint(e);
			if (e->root) zrn_set_capture(e->root, e);
			return 1;
		case ZST_IN_POINTER_UP: {
			const int tap = it->pressed && in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			it->pressed = 0;
			zrn_changed_paint(e);
			if (e->root) zrn_release_capture(e->root, e);
			if (tap) item_tapped(e);
			return 1;
		}
		case ZST_IN_CAPTURE_LOST:
			it->pressed = 0;
			zrn_changed_paint(e);
			return 0;
		default:
			return 0;
	}
}

static void item_init(zrn_element_t* const e) {
	zrn_control_t* const c = &ITEM(e)->base.base;
	c->h_content = Z_HALIGN_LEFT;
	c->v_content = Z_VALIGN_TOP;
	c->font_size = ITEM_FONT_SIZE;
	e->focusable = 0;
}

zrn_class_t zrn_class_selector_item = {
	.base = &zrn_class_content_control, .name = "SelectorItem", .id = Z_CLASS_SELECTOR_ITEM, .size = sizeof(zrn_selector_item_t), .abstract_class = 1,
	.init = item_init, .measure = item_measure, .paint = item_paint, .input = item_input, .chrome = item_chrome, .content_foreground = item_content_foreground,
	.content_font_size = item_font_size, .content_font_family = item_font_family
};

static void list_box_item_init(zrn_element_t* e) { CTL(e)->padding = Zthickness(6.5f, 8, 6.5f, 8); }

static void combo_box_item_init(zrn_element_t* const e) {
	const auto c = CTL(e);
	c->padding = Zthickness(6.5f, 8.5f, 0, 16);
	c->h_content = Z_HALIGN_STRETCH;
	c->font_size = ZST_81_FONT_CONTENT;
	e->valign = Z_VALIGN_TOP;
}

static void flip_view_item_init(zrn_element_t* const e) {
	const auto c = CTL(e);
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
	c->font_size = ZST_81_FONT_CONTENT;
}

zrn_class_t zrn_class_list_box_item = {
	.base = &zrn_class_selector_item, .name = "ListBoxItem", .id = Z_CLASS_LIST_BOX_ITEM, .size = sizeof(zrn_selector_item_t), .init = list_box_item_init
};

zrn_class_t zrn_class_combo_box_item = {
	.base = &zrn_class_selector_item, .name = "ComboBoxItem", .id = Z_CLASS_COMBO_BOX_ITEM, .size = sizeof(zrn_selector_item_t), .init = combo_box_item_init
};

zrn_class_t zrn_class_flip_view_item = {
	.base = &zrn_class_selector_item, .name = "FlipViewItem", .id = Z_CLASS_FLIP_VIEW_ITEM, .size = sizeof(zrn_selector_item_t), .init = flip_view_item_init
};

static zrn_element_t* make_item(const ZClass cls, const char* const text) {
	zrn_element_t* const e = zrn_create(cls);
	if (!e) return nullptr;
	if (text) zrn_content_set_text(&ITEM(e)->base, text);
	return e;
}

static Zid create_item(const ZClass cls, const char* const content) {
	const zrn_element_t* e = make_item(cls, content);
	return e ? e->id : Z_INVALID_ID;
}

Zid Zlist_box_item_create(const char* content) { return create_item(Z_CLASS_LIST_BOX_ITEM, content); }
Zid Zcombo_box_item_create(const char* content) { return create_item(Z_CLASS_COMBO_BOX_ITEM, content); }
Zid Zflip_view_item_create(const char* content) { return create_item(Z_CLASS_FLIP_VIEW_ITEM, content); }
Zid Zlist_view_item_create(const char* content) { return create_item(Z_CLASS_LIST_VIEW_ITEM, content); }
Zid Zgrid_view_item_create(const char* content) { return create_item(Z_CLASS_GRID_VIEW_ITEM, content); }


/* ============================================================================
 *  ItemsControl
 * ============================================================================ */

static ZThickness ic_insets(const zrn_element_t* const e) {
	const zrn_control_t* c = CTL(e);
	return Zthickness(c->border_thickness.left + c->padding.left, c->border_thickness.top + c->padding.top,
		c->border_thickness.right + c->padding.right, c->border_thickness.bottom + c->padding.bottom);
}

static ZSize ic_measure(zrn_element_t* const e, const ZSize available) {
	const ZThickness in = ic_insets(e);
	ZSize s = { 0, 0 };
	if (e->child_count) {
		zrn_measure(e->children[0], zrn_size(zrn_maxf(0, available.width - zrn_thickness_h(in)), zrn_maxf(0, available.height - zrn_thickness_v(in))));
		s = e->children[0]->desired;
	}
	return zrn_size(s.width + zrn_thickness_h(in), s.height + zrn_thickness_v(in));
}

static ZSize ic_arrange(zrn_element_t* const e, const ZSize final) {
	const ZThickness in = ic_insets(e);
	if (e->child_count) zrn_arrange(e->children[0], zrn_rect(in.left, in.top, zrn_maxf(0, final.width - zrn_thickness_h(in)), zrn_maxf(0, final.height - zrn_thickness_v(in))));
	return final;
}

static void ic_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_control_t* const c = CTL(e);
	const ZRect r = zrn_local_rect(e);
	const int box = zrn_is_a(e, Z_CLASS_LIST_BOX), enabled = zrn_effectively_enabled(e);
	ZColor bg = box ? zrn_theme_color(e, ZST_TC_BASE_MEDIUM_HIGH) : Zcolor_rgba(0, 0, 0, 0);
	ZColor border = box ? zrn_theme_color(e, ZST_TC_BASE_BLACK) : Zcolor_rgba(0, 0, 0, 0);
	bg = zrn_ctl_background(c, bg);
	border = zrn_ctl_border_brush(c, Zcolor_rgba(0, 0, 0, 0));
	if (box && !enabled) {
		/* Disabled: a transparent background and the border in ListBoxDisabledForegroundThemeBrush */
		bg = Zcolor_rgba(0, 0, 0, 0);
		border = zrn_theme_color(e, ZST_TC_PHONE_DISABLED);
	}
	if (bg.a > 0) zrn_fill_rect(p, r, bg);
	if (border.a > 0) zrn_stroke_rect(p, r, c->border_thickness, border);
}

static void ic_init(zrn_element_t* const e) {
	const auto o = IC(e);
	zrn_element_t* const host = zrn_create(Z_CLASS_STACK_PANEL);
	o->selected_index = -1;
	o->sel_mode = ZST_SEL_SINGLE;
	e->focusable = 0;
	if (host) {
		zrn_add_child(e, host, -1);
		o->host = host;
	}
}

static void ic_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	const auto o = IC(e);
	if (o->host == child) o->host = nullptr;
	if (o->scroll == child) {
		o->scroll = nullptr;
		o->presenter = nullptr;
		o->host = nullptr;
	}
}

zrn_class_t zrn_class_items_control = {
	.base = &zrn_class_control, .name = "ItemsControl", .id = Z_CLASS_ITEMS_CONTROL, .size = sizeof(zrn_items_control_t),
	.init = ic_init, .measure = ic_measure, .arrange = ic_arrange, .paint = ic_paint, .child_removed = ic_child_removed
};

zrn_class_t zrn_class_selector = {
	.base = &zrn_class_items_control, .name = "Selector", .id = Z_CLASS_SELECTOR, .size = sizeof(zrn_items_control_t), .abstract_class = 1
};

static zrn_items_control_t* get_ic(const Zid id) { return IC(zrn_get(id, Z_CLASS_ITEMS_CONTROL)); }
static zrn_items_control_t* get_selector(const Zid id) { return IC(zrn_get(id, Z_CLASS_SELECTOR)); }

Zid Zitems_control_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_ITEMS_CONTROL);
	return e ? e->id : Z_INVALID_ID;
}

zrn_element_t* zrn_ic_add(zrn_items_control_t* const o, zrn_element_t* const item, const char* const text) {
	zrn_element_t* container;
	if (!o->host) return nullptr;
	if (!is_selector(o) && !zrn_is_a(&o->base.base, Z_CLASS_AUTO_SUGGEST_BOX)) {
		/* a plain ItemsControl shows the element itself */
		container = item;
		if (!container) {
			container = zrn_create(Z_CLASS_TEXT_BLOCK);
			if (container) zrn_set_string(&ZST_AS(zrn_text_block_t, container)->text, text);
		}
	} else if (item && is_container(item)) {
		container = item;
	} else {
		container = make_item(container_class(o), text);
		if (container && item) zrn_content_set(&ITEM(container)->base, item);
	}
	if (!container) return nullptr;
	if (zrn_is_a(&o->base.base, Z_CLASS_AUTO_SUGGEST_BOX) && is_container(container) && !(CTL(container)->set_flags & ZST_C_FOREGROUND)) {
		/* the ItemContainerStyle of AutoSuggestBox: Margin 6.5,9.5,0,0, ContentControlFontSize and the foreground of a TextBox */
		container->margin = Zthickness(6.5f, 9.5f, 0, 0);
		Zcontrol_set_font_size(container->id, ZST_81_FONT_CONTENT);
		Zcontrol_set_foreground(container->id, zrn_theme_color(container, ZST_TC_BASE_BLACK));
	}
	zrn_add_child(o->host, container, -1);
	if (container->parent != o->host) return nullptr;
	if (zrn_is_a(&o->base.base, Z_CLASS_FLIP_VIEW)) {
		zrn_changed_layout(&o->base.base);
		zrn_flip_items_changed(o);
	}
	return container;
}

/* OBSOLETE DEPRECATED TODO delete this method, add text_block like other widgets*/
Zid Zitems_control_add_text(const Zid id, const char* const text) {
	zrn_items_control_t* const o = get_ic(id);
	zrn_element_t* c;
	if (!o) return Z_INVALID_ID;
	c = zrn_ic_add(o, nullptr, text ? text : "");
	return c ? c->id : Z_INVALID_ID;
}

Zid Zitems_control_add(const Zid id, const Zid item) {
	zrn_items_control_t* const o = get_ic(id);
	zrn_element_t* const e = zrn_get(item, Z_CLASS_ELEMENT), *c;
	if (!o || !e) return Z_INVALID_ID;
	c = zrn_ic_add(o, e, nullptr);
	return c ? c->id : Z_INVALID_ID;
}

static void ic_refresh_selected_index(zrn_items_control_t* const o) {
	int i, n = zrn_ic_count(o);
	o->selected_index = -1;
	for (i = 0; i < n; i++) if (ITEM(zrn_ic_item(o, i))->selected) { o->selected_index = i; return; }
}

void Zitems_control_remove_at(const Zid id, const int index) {
	zrn_items_control_t* const o = get_ic(id);
	zrn_element_t* item;
	int was_selected, old_selected;
	if (!o || !(item = zrn_ic_item(o, index))) return;
	was_selected = is_selector(o) && is_container(item) && ITEM(item)->selected;
	old_selected = o->selected_index;
	zrn_destroy(item);
	if (is_selector(o)) {
		ic_refresh_selected_index(o);
		if (zrn_is_a(&o->base.base, Z_CLASS_FLIP_VIEW)) {
			/* the item that takes the place of the one that was shown is shown */
			o->selected_index = was_selected ? index : (old_selected > index ? old_selected - 1 : old_selected);
			zrn_flip_items_changed(o);
		}
		if (was_selected && !zrn_raise(&o->base.base, Z_EVENT_SELECTION_CHANGED)) return;
	}
	if (zrn_lookup(id)) zrn_changed_layout(&o->base.base);
}

void Zitems_control_clear(const Zid id) {
	zrn_items_control_t* o = get_ic(id);
	int had = 0, i, n;
	if (!o || !o->host) return;
	n = zrn_ic_count(o);
	for (i = 0; i < n && !had; i++) {
		zrn_element_t* const item = zrn_ic_item(o, i);
		had = is_container(item) && ITEM(item)->selected;
	}
	zrn_clear_children(o->host);
	o->selected_index = -1;
	if (zrn_is_a(&o->base.base, Z_CLASS_FLIP_VIEW)) zrn_flip_items_changed(o);
	o->group_count = 0;
	o->focus_item = Z_INVALID_ID;
	if (had && is_selector(o) && !zrn_raise(&o->base.base, Z_EVENT_SELECTION_CHANGED)) return;
	if (zrn_lookup(id)) zrn_changed_layout(&o->base.base);
}

int Zitems_control_get_count(const Zid id) {
	const zrn_items_control_t* const o = get_ic(id);
	return o ? zrn_ic_count(o) : 0;
}

Zid Zitems_control_get_container(const Zid id, const int index) {
	const zrn_items_control_t* const o = get_ic(id);
	const zrn_element_t* const item = o ? zrn_ic_item(o, index) : nullptr;
	return item ? item->id : Z_INVALID_ID;
}

int Zitems_control_index_from_container(const Zid id, const Zid container) {
	const zrn_items_control_t* const o = get_ic(id);
	const zrn_element_t* const c = zrn_get(container, Z_CLASS_ELEMENT);
	return o && c ? zrn_ic_index_of(o, c) : -1;
}

const char* zrn_container_text(zrn_element_t* const container) {
	if (zrn_is_a(container, Z_CLASS_TEXT_BLOCK)) return Ztext_block_get_text(container->id);
	if (zrn_is_a(container, Z_CLASS_CONTENT_CONTROL)) return Zcontent_control_get_text(container->id);
	return "";
}

const char* Zitems_control_get_text(const Zid id, const int index) {
	const zrn_items_control_t* const o = get_ic(id);
	zrn_element_t* const item = o ? zrn_ic_item(o, index) : nullptr;
	return item ? zrn_container_text(item) : "";
}

Zid Zitems_control_get_scroll_viewer(const Zid id) {
	const zrn_items_control_t* const o = get_ic(id);
	return o && o->scroll ? o->scroll->id : Z_INVALID_ID;
}


/* ============================================================================
 *  the items panel and the scrolling
 * ============================================================================ */

/* The view scrolls along its panel: sideways for a horizontal stack and for a vertical wrap grid. */
static int panel_scrolls_sideways(const zrn_element_t* const panel) {
	if (!panel) return 0;
	if (zrn_is_a(panel, Z_CLASS_ITEMS_WRAP_GRID)) return ZST_AS(const zrn_items_wrap_grid_t, panel)->orientation == Z_VERTICAL;
	if (zrn_is_a(panel, Z_CLASS_STACK_PANEL)) return ZST_AS(const zrn_stack_panel_t, panel)->orientation == Z_HORIZONTAL;
	if (zrn_is_a(panel, Z_CLASS_WRAP_PANEL)) return ZST_AS(const zrn_wrap_panel_t, panel)->orientation == Z_VERTICAL;
	return 0;
}

static void ic_sync_scroll(const zrn_items_control_t* const o) {
	int sideways = panel_scrolls_sideways(o->host);
	if (o->presenter) Zstack_panel_set_orientation(o->presenter->id, sideways ? Z_HORIZONTAL : Z_VERTICAL);
	if (o->scroll && !zrn_is_a(&o->base.base, Z_CLASS_COMBO_BOX)) {
		Zscroll_viewer_set_vertical_scrollbar_visibility(o->scroll->id, sideways ? Z_SCROLLBAR_DISABLED : Z_SCROLLBAR_AUTO);
		Zscroll_viewer_set_horizontal_scrollbar_visibility(o->scroll->id, sideways ? Z_SCROLLBAR_AUTO : Z_SCROLLBAR_DISABLED);
	}
}

void zrn_items_panel_changed(zrn_element_t* const panel) {
	zrn_element_t* a;
	for (a = panel->parent; a; a = a->parent) {
		if (zrn_is_a(a, Z_CLASS_ITEMS_CONTROL) && IC(a)->host == panel) {
			ic_sync_scroll(IC(a));
			return;
		}
	}
}

void zrn_ic_set_host(zrn_items_control_t* const o, zrn_element_t* const panel) {
	zrn_element_t* const old = o->host;
	zrn_element_t** moved = nullptr;
	int count = 0, i;
	if (old && old->child_count) {
		moved = (zrn_element_t**)malloc((size_t)old->child_count * sizeof *moved);
		if (!moved) {
			zrn_error(Z_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
		while (old->child_count > 0) {
			zrn_element_t* c = old->children[0];
			moved[count++] = c;
			zrn_remove_child(old, c);
		}
	}
	if (o->presenter && old && old->parent == o->presenter) {
		int at = zrn_child_index(o->presenter, old);
		zrn_destroy(old);
		zrn_add_child(o->presenter, panel, at);
	} else if (o->scroll && old && old->parent == o->scroll) {
		zrn_content_set(ZST_AS(zrn_content_control_t, o->scroll), panel);
	} else {
		if (old) zrn_destroy(old);
		zrn_add_child(&o->base.base, panel, 0);
	}
	o->host = panel;
	for (i = 0; i < count; i++) zrn_add_child(panel, moved[i], -1);
	free(moved);
	ic_sync_scroll(o);
	zrn_changed_layout(&o->base.base);
}

void Zitems_control_set_items_panel(const Zid id, const Zid panel) {
	zrn_items_control_t* const o = get_ic(id);
	zrn_element_t* const p = zrn_get(panel, Z_CLASS_PANEL);
	if (!o || !p) return;
	if (p->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "the items panel already has a parent");
		return;
	}
	zrn_ic_set_host(o, p);
}

Zid Zitems_control_get_items_panel(const Zid id) {
	const zrn_items_control_t* o = get_ic(id);
	return o && o->host ? o->host->id : Z_INVALID_ID;
}

void zrn_ic_scroll_into_view(zrn_items_control_t* const o, const int index, const ZScrollIntoViewAlignment alignment) {
	zrn_element_t* const item = zrn_ic_item(o, index);
	zrn_element_t* content;
	ZPoint at, origin;
	int sideways;
	double start, end, offset, view;
	if (!item || !o->scroll || !o->base.base.root) return;
	Zroot_update_layout(o->base.base.root->id);
	content = ZST_AS(zrn_content_control_t, o->scroll)->content;
	if (!content) return;
	at = zrn_to_root(item, (ZPoint){ 0, 0 });
	origin = zrn_to_root(content, (ZPoint){ 0, 0 });
	sideways = panel_scrolls_sideways(o->host);
	if (sideways) {
		start = at.x - origin.x;
		end = start + item->rect.width;
		offset = Zscroll_viewer_get_horizontal_offset(o->scroll->id);
		view = Zscroll_viewer_get_viewport_width(o->scroll->id);
	} else {
		start = at.y - origin.y;
		end = start + item->rect.height;
		offset = Zscroll_viewer_get_vertical_offset(o->scroll->id);
		view = Zscroll_viewer_get_viewport_height(o->scroll->id);
	}
	if (alignment == Z_SCROLL_INTO_VIEW_LEADING) offset = start;
	else if (start < offset) offset = start;
	else if (end > offset + view) offset = end - view;
	else return;
	if (sideways) Zscroll_viewer_scroll_to_horizontal_offset(o->scroll->id, offset);
	else Zscroll_viewer_scroll_to_vertical_offset(o->scroll->id, offset);
}


/* ============================================================================
 *  selection
 * ============================================================================ */

int zrn_ic_selected_indices(const zrn_items_control_t* const o, int* const out, const int max) {
	int i, n = zrn_ic_count(o), found = 0;
	for (i = 0; i < n; i++) {
		if (!ITEM(zrn_ic_item(o, i))->selected) continue;
		if (out && found < max) out[found] = i;
		found++;
	}
	return found;
}

static int selected_count(const zrn_items_control_t* o) { return zrn_ic_selected_indices(o, nullptr, 0); }

/* Selects exactly the items of `want` (index or -1; or all when `all`): true when the selection changed. */
static int apply_selection(zrn_items_control_t* const o, const int want, const int all) {
	int i, n = zrn_ic_count(o), changed = 0;
	for (i = 0; i < n; i++) {
		zrn_element_t* const item = zrn_ic_item(o, i);
		const int selected = all || i == want;
		if (ITEM(item)->selected != selected) {
			item_set_selected(item, selected);
			changed = 1;
		}
	}
	ic_refresh_selected_index(o);
	return changed;
}

void zrn_ic_select(zrn_items_control_t* o, int index) {
	zrn_element_t* const e = &o->base.base;
	const int flip = zrn_is_a(e, Z_CLASS_FLIP_VIEW);
	if (o->sel_mode == ZST_SEL_NONE) return;
	if (index >= zrn_ic_count(o) || index < -1) index = -1;
	/* a FlipView always shows an item */
	if (flip && index < 0 && zrn_ic_count(o) > 0) return;
	if (apply_selection(o, index, 0)) {
		zrn_changed_paint(e);
		if (flip) zrn_flip_selected(o);
		zrn_raise(e, Z_EVENT_SELECTION_CHANGED);
	}
}

void zrn_ic_tap(zrn_items_control_t* const o, const int index) {
	zrn_element_t* const e = &o->base.base;
	zrn_element_t* const item = zrn_ic_item(o, index);
	if (!item || o->sel_mode == ZST_SEL_NONE) return;
	if (o->sel_mode == ZST_SEL_MULTIPLE) {
		item_set_selected(item, !ITEM(item)->selected);
		ic_refresh_selected_index(o);
		zrn_changed_paint(e);
		zrn_raise(e, Z_EVENT_SELECTION_CHANGED);
		return;
	}
	zrn_ic_select(o, index);
}

void zrn_ic_select_all(zrn_items_control_t* const o) {
	zrn_element_t* const e = &o->base.base;
	if (o->sel_mode != ZST_SEL_MULTIPLE && o->sel_mode != ZST_SEL_EXTENDED) return;
	if (apply_selection(o, -1, 1)) {
		zrn_changed_paint(e);
		zrn_raise(e, Z_EVENT_SELECTION_CHANGED);
	}
}

void zrn_ic_set_sel_mode(zrn_items_control_t* const o, const zrn_sel_mode_t mode) {
	zrn_element_t* const e = &o->base.base;
	int keep = o->selected_index;
	if (o->sel_mode == mode) return;
	o->sel_mode = mode;
	if (mode == ZST_SEL_NONE) keep = -1;
	if (mode == ZST_SEL_NONE || mode == ZST_SEL_SINGLE) {
		if (selected_count(o) > (keep >= 0 ? 1 : 0) || (mode == ZST_SEL_NONE && selected_count(o) > 0)) {
			apply_selection(o, keep, 0);
			zrn_changed_paint(e);
			if (!zrn_raise(e, Z_EVENT_SELECTION_CHANGED)) return;
		}
	}
	/* the check boxes come or go, and the rows are measured again */
	zrn_invalidate_deep(e);
}

void Zselector_set_selected_index(const Zid id, const int index) {
	zrn_items_control_t* o = get_selector(id);
	if (o) zrn_ic_select(o, index);
}

int Zselector_get_selected_index(const Zid id) {
	zrn_items_control_t* o = get_selector(id);
	return o ? o->selected_index : -1;
}

Zid Zselector_get_selected_container(const Zid id) {
	zrn_items_control_t* o = get_selector(id);
	zrn_element_t* const item = o && o->selected_index >= 0 ? zrn_ic_item(o, o->selected_index) : nullptr;
	return item ? item->id : Z_INVALID_ID;
}

void Zselector_item_set_is_selected(Zid id, int is_selected) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_SELECTOR_ITEM);
	zrn_items_control_t* o;
	if (!e) return;
	is_selected = is_selected != 0;
	o = zrn_item_owner(e);
	if (!o || e->parent != o->host || !is_selector(o)) {
		item_set_selected(e, is_selected);
		return;
	}
	if (o->sel_mode == ZST_SEL_NONE || ITEM(e)->selected == is_selected) return;
	if (o->sel_mode == ZST_SEL_SINGLE) {
		const int index = zrn_ic_index_of(o, e);
		if (is_selected) zrn_ic_select(o, index);
		else if (o->selected_index == index) zrn_ic_select(o, -1);
		return;
	}
	item_set_selected(e, is_selected);
	ic_refresh_selected_index(o);
	zrn_raise(&o->base.base, Z_EVENT_SELECTION_CHANGED);
}

int Zselector_item_get_is_selected(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_SELECTOR_ITEM);
	return e ? ITEM(e)->selected : 0;
}


/* ============================================================================
 *  keys
 * ============================================================================ */

/* How many items are in the first line of a wrap grid, for the Up and Down keys. */
static int grid_columns(const zrn_items_control_t* const o) {
	int n = zrn_ic_count(o), i, columns = 1;
	zrn_element_t* const first = zrn_ic_item(o, 0);
	if (!first || !zrn_is_a(o->host, Z_CLASS_ITEMS_WRAP_GRID)) return 1;
	for (i = 1; i < n; i++) {
		zrn_element_t* item = zrn_ic_item(o, i);
		if (ZST_AS(zrn_items_wrap_grid_t, o->host)->orientation == Z_HORIZONTAL ? item->rect.y == first->rect.y : item->rect.x == first->rect.x) columns++;
		else break;
	}
	return columns;
}

int zrn_ic_key(zrn_element_t* const e, zrn_input_t* const in) {
	auto o = IC(e);
	int n = zrn_ic_count(o), at, next, step = 0, grid = o->host && zrn_is_a(o->host, Z_CLASS_ITEMS_WRAP_GRID);
	zrn_element_t* focus = zrn_lookup(o->focus_item);
	int sideways = panel_scrolls_sideways(o->host);
	const int is_list_view = zrn_is_a(e, Z_CLASS_LIST_VIEW_BASE);
	if (n == 0) return 0;
	at = focus ? zrn_ic_index_of(o, focus) : -1;
	if (at < 0) at = o->selected_index;
	switch (in->key) {
		case Z_KEY_DOWN: step = sideways && !grid ? 0 : (grid && !sideways ? grid_columns(o) : 1); break;
		case Z_KEY_UP: step = sideways && !grid ? 0 : -(grid && !sideways ? grid_columns(o) : 1); break;
		case Z_KEY_RIGHT: step = grid || sideways ? 1 : 0; break;
		case Z_KEY_LEFT: step = grid || sideways ? -1 : 0; break;
		case Z_KEY_HOME: next = 0; goto go;
		case Z_KEY_END: next = n - 1; goto go;
		case Z_KEY_SPACE:
		case Z_KEY_ENTER:
			if (at < 0) return 0;
			if (is_list_view) zrn_lvb_item_tapped(o, at);
			else zrn_ic_tap(o, at);
			return 1;
		case Z_KEY_A:
			if (!(in->mods & Z_MOD_CTRL)) return 0;
			zrn_ic_select_all(o);
			return 1;
		default:
			return 0;
	}
	if (step == 0) return 0;
	next = at < 0 ? (step > 0 ? 0 : n - 1) : at + step;
	if (next < 0 || next >= n) next = at < 0 ? 0 : at;
go:
	{
		zrn_element_t* const item = zrn_ic_item(o, next);
		const Zid id = e->id;
		if (!item) return 0;
		o->focus_item = item->id;
		o->focus_visible = 1;
		zrn_changed_paint(e);
		/* the keyboard selects where a tap would select without toggling */
		if (o->sel_mode == ZST_SEL_SINGLE || o->sel_mode == ZST_SEL_EXTENDED) zrn_ic_select(o, next);
		if (!zrn_lookup(id)) return 1;
		zrn_ic_scroll_into_view(o, next, Z_SCROLL_INTO_VIEW_DEFAULT);
	}
	return 1;
}


/* ============================================================================
 *  ListBox
 * ============================================================================ */

static void list_box_init(zrn_element_t* const e) {
	const auto o = IC(e);
	const auto c = CTL(e);
	zrn_element_t* const scroll = zrn_create(Z_CLASS_SCROLL_VIEWER);
	zrn_element_t* const host = zrn_create(Z_CLASS_VIRTUALIZING_STACK_PANEL);
	/* ListBox: BorderThickness 1.5, FontSize 12, IsTabStop False but it takes the keyboard when it is touched */
	c->border_thickness = Zthickness_uniform(1.5f);
	c->font_size = ITEM_FONT_SIZE;
	e->focusable = 1;
	e->tab_stop = 0;
	if (!scroll || !host) return;
	/* the plain panel made by ItemsControl is replaced by the virtualizing one inside a scroll viewer */
	if (o->host) zrn_destroy(o->host);
	zrn_add_child(e, scroll, -1);
	zrn_content_set(ZST_AS(zrn_content_control_t, scroll), host);
	o->scroll = scroll;
	o->host = host;
}

static int list_box_input(zrn_element_t* const e, zrn_input_t* const in) {
	if (in->type == ZST_IN_KEY_DOWN) return zrn_ic_key(e, in);
	return 0;
}

zrn_class_t zrn_class_list_box = {
	.base = &zrn_class_selector, .name = "ListBox", .id = Z_CLASS_LIST_BOX, .size = sizeof(zrn_items_control_t),
	.init = list_box_init, .input = list_box_input
};

Zid Zlist_box_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_LIST_BOX);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_items_control_t* get_list_box(const Zid id) { return IC(zrn_get(id, Z_CLASS_LIST_BOX)); }

void Zlist_box_set_selection_mode(const Zid id, const ZSelectionMode mode) {
	zrn_items_control_t* const o = get_list_box(id);
	if (!o) return;
	zrn_ic_set_sel_mode(o, mode == Z_SELECTION_MULTIPLE ? ZST_SEL_MULTIPLE : mode == Z_SELECTION_EXTENDED ? ZST_SEL_EXTENDED : ZST_SEL_SINGLE);
}

ZSelectionMode Zlist_box_get_selection_mode(const Zid id) {
	zrn_items_control_t* const o = get_list_box(id);
	if (!o) return Z_SELECTION_SINGLE;
	return o->sel_mode == ZST_SEL_MULTIPLE ? Z_SELECTION_MULTIPLE : o->sel_mode == ZST_SEL_EXTENDED ? Z_SELECTION_EXTENDED : Z_SELECTION_SINGLE;
}

int Zlist_box_get_selected_indices(const Zid id, int* const indices, const int max) {
	const zrn_items_control_t* const o = get_list_box(id);
	return o ? zrn_ic_selected_indices(o, indices, max) : 0;
}

void Zlist_box_select_all(const Zid id) {
	zrn_items_control_t* const o = get_list_box(id);
	if (o) zrn_ic_select_all(o);
}

void Zlist_box_scroll_into_view(const Zid id, const int index) {
	zrn_items_control_t* const o = get_list_box(id);
	if (o) zrn_ic_scroll_into_view(o, index, Z_SCROLL_INTO_VIEW_DEFAULT);
}
