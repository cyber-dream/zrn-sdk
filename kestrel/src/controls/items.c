/**
 * Kestrel - controls/items.c
 * ItemsControl, Selector, SelectorItem (the containers) and ListBox. ListView and GridView are in
 * listview.c, ComboBox and AutoSuggestBox in combo.c, FlipView and SemanticZoom in flip.c; they
 * share what is here: the items, the selection, the keys of a list and the scrolling to an item.
 *
 * The items are containers (ListBoxItem, ListViewItem...) in an items panel, `host`. A ListBox
 * shows the panel in a ScrollViewer, with a Border look of its own (generic.xaml: PhoneBaseMediumHigh
 * with black text, a 1.5 px border that is transparent).
 */

#include "../internal.h"

#define IC(e) KST_AS(kst_items_control_t, e)
#define ITEM(e) KST_AS(kst_selector_item_t, e)
#define CTL(e) KST_AS(kst_control_t, e)

/* ListBoxItem: Padding 6.5,8; ComboBoxItem: Padding 6.5,8.5,0,16; ListViewItem and GridViewItem: ControlContentThemeFontSize 12 */
#define ITEM_FONT_SIZE 12.0f
/* ListViewItemContentTranslateX: how far the content moves to the right for the check box of the multiple selection */
#define CHECK_SHIFT 40.5f
#define CHECK_BOX 25.5f
#define CHECK_BOX_TOP 9.5f
#define EARMARK 34.0f


/* ============================================================================
 *  the items of a control
 * ============================================================================ */

static int is_header_item(const kst_element_t* e) { return kst_is_a(e, K_CLASS_LIST_VIEW_BASE_HEADER_ITEM); }

kst_items_control_t* kst_item_owner(kst_element_t* item) {
	kst_element_t* a;
	for (a = item->parent; a; a = a->parent) if (kst_is_a(a, K_CLASS_ITEMS_CONTROL)) return IC(a);
	return NULL;
}

int kst_ic_count(const kst_items_control_t* o) {
	int n, i;
	if (!o->host) return 0;
	if (o->group_count == 0) return o->host->child_count;
	for (n = 0, i = 0; i < o->host->child_count; i++) if (!is_header_item(o->host->children[i])) n++;
	return n;
}

kst_element_t* kst_ic_item(const kst_items_control_t* o, int index) {
	int n = 0, i;
	if (!o->host || index < 0) return NULL;
	if (o->group_count == 0) return index < o->host->child_count ? o->host->children[index] : NULL;
	for (i = 0; i < o->host->child_count; i++) {
		if (is_header_item(o->host->children[i])) continue;
		if (n++ == index) return o->host->children[i];
	}
	return NULL;
}

int kst_ic_index_of(const kst_items_control_t* o, const kst_element_t* container) {
	int n = 0, i;
	if (!o->host || container->parent != o->host || is_header_item(container)) return -1;
	for (i = 0; i < o->host->child_count; i++) {
		if (o->host->children[i] == container) return n;
		if (o->group_count == 0 || !is_header_item(o->host->children[i])) n++;
	}
	return -1;
}

static int is_selector(const kst_items_control_t* o) { return kst_is_a(&o->base.base, K_CLASS_SELECTOR); }

static int is_container(const kst_element_t* e) { return kst_is_a(e, K_CLASS_SELECTOR_ITEM); }

void kst_item_set_selected(kst_element_t* item, int selected) {
	if (ITEM(item)->selected == selected) return;
	ITEM(item)->selected = selected;
	kst_changed_paint(item);
}

#define item_set_selected kst_item_set_selected

/** The container class an items control makes for its items. */
static KClass container_class(const kst_items_control_t* o) {
	const kst_element_t* e = &o->base.base;
	if (kst_is_a(e, K_CLASS_COMBO_BOX)) return K_CLASS_COMBO_BOX_ITEM;
	if (kst_is_a(e, K_CLASS_FLIP_VIEW)) return K_CLASS_FLIP_VIEW_ITEM;
	if (kst_is_a(e, K_CLASS_GRID_VIEW)) return K_CLASS_GRID_VIEW_ITEM;
	if (kst_is_a(e, K_CLASS_LIST_VIEW) || kst_is_a(e, K_CLASS_AUTO_SUGGEST_BOX)) return K_CLASS_LIST_VIEW_ITEM;
	return K_CLASS_LIST_BOX_ITEM;
}

static int multi_check(const kst_items_control_t* o) {
	return o && kst_is_a(&o->base.base, K_CLASS_LIST_VIEW_BASE) && (o->sel_mode == KST_SEL_MULTIPLE || o->sel_mode == KST_SEL_EXTENDED);
}


/* ============================================================================
 *  the containers
 * ============================================================================ */

static KThickness item_chrome(kst_element_t* e) {
	kst_items_control_t* o = kst_item_owner(e);
	if (kst_is_a(e, K_CLASS_LIST_VIEW_ITEM) && multi_check(o)) return Kthickness(CHECK_SHIFT, 0, 0, 0);
	return kst_thickness_zero();
}

/* The check box of a list item is 25.5 high with 9.5 above it: the row is at least that high. */
static KSize item_measure(kst_element_t* e, KSize available) {
	KSize s = kst_cc_measure(e, available);
	if (kst_is_a(e, K_CLASS_LIST_VIEW_ITEM) && multi_check(kst_item_owner(e))) s.height = kst_maxf(s.height, CHECK_BOX_TOP + CHECK_BOX);
	return s;
}

static int item_focused(kst_element_t* e, const kst_items_control_t* o) {
	return o && o->focus_visible && o->focus_item == e->id && e->root && e->root->focus == &o->base.base;
}

static float item_font_size(kst_element_t* e) {
	kst_control_t* c = CTL(e);
	kst_items_control_t* o = kst_item_owner(e);
	if (!(c->set_flags & KST_C_FONT_SIZE) && o && (o->base.set_flags & KST_C_FONT_SIZE)) return o->base.font_size;
	return c->font_size;
}

static KFontFamily item_font_family(kst_element_t* e) {
	kst_control_t* c = CTL(e);
	kst_items_control_t* o = kst_item_owner(e);
	if (!(c->set_flags & KST_C_FONT_FAMILY) && o && (o->base.set_flags & KST_C_FONT_FAMILY)) return o->base.font_family;
	return c->font_family;
}

static KColor item_content_foreground(kst_element_t* e) {
	kst_selector_item_t* it = ITEM(e);
	kst_control_t* c = &it->base.base;
	kst_items_control_t* o = kst_item_owner(e);
	int enabled = kst_effectively_enabled(e);
	KColor fg;
	if (kst_is_a(e, K_CLASS_LIST_BOX_ITEM)) {
		/* ListBoxItem: the Foreground of the ListBox is PhoneBaseBlack (Dark; Light has the white that reads on its dark box), selected is PhoneBaseWhite on the accent */
		if (!enabled) return kst_theme_color(e, it->selected ? KST_TC_BASE_LOW : KST_TC_BASE_MEDIUM_MID);
		if (it->selected) return kst_theme_color(e, KST_TC_BASE_WHITE);
		if (c->set_flags & KST_C_FOREGROUND) return c->foreground;
		if (o && (o->base.set_flags & KST_C_FOREGROUND)) return o->base.foreground;
		return kst_theme_color(e, KST_TC_LIST_BOX_FOREGROUND);
	}
	if (c->set_flags & KST_C_FOREGROUND) fg = c->foreground;
	else if (o && (o->base.set_flags & KST_C_FOREGROUND)) fg = o->base.foreground;
	/* the list of a ComboBox is white while it is open (ComboBoxHighlightedForeground) */
	else if (kst_is_a(e, K_CLASS_COMBO_BOX_ITEM)) fg = kst_theme_color(e, KST_TC_BASE_BLACK);
	else fg = kst_theme_color(e, KST_TC_FOREGROUND);
	if (kst_is_a(e, K_CLASS_COMBO_BOX_ITEM) && it->selected) fg = kst_theme_color(e, KST_TC_ACCENT);
	/* Disabled: the content is at ListViewItemDisabledThemeOpacity / ComboBoxItemDisabledThemeOpacity 0.4 */
	if (!enabled) fg = kst_color_alpha(fg, 0.4f);
	return fg;
}

static void check_glyph(kst_painter_t* p, float x, float y, float w, float h, KColor color) {
	/* Path "M0,123 L39,93 L124,164 L256,18 L295,49 L124,240 z", Stretch Fill */
	static const float path[12] = { 0, 123, 39, 93, 124, 164, 256, 18, 295, 49, 124, 240 };
	float pts[12];
	int i;
	for (i = 0; i < 6; i++) {
		pts[i * 2] = x + path[i * 2] / 295.0f * w;
		pts[i * 2 + 1] = y + (path[i * 2 + 1] - 18.0f) / 222.0f * h;
	}
	kst_fill_polygon(p, pts, 6, color);
}

static void item_paint(kst_element_t* e, kst_painter_t* p) {
	kst_selector_item_t* it = ITEM(e);
	kst_control_t* c = &it->base.base;
	kst_items_control_t* o = kst_item_owner(e);
	int enabled = kst_effectively_enabled(e);
	KRect r = kst_local_rect(e);
	KColor bg = kst_ctl_background(c, Kcolor_rgba(0, 0, 0, 0));
	if (kst_is_a(e, K_CLASS_LIST_BOX_ITEM)) {
		/* InnerGrid: the accent when selected, the PressedBackground rectangle while pressed */
		if (it->selected) bg = kst_theme_color(e, enabled ? KST_TC_ACCENT : KST_TC_PHONE_DISABLED);
		else if (it->pressed && enabled) bg = kst_theme_color(e, KST_TC_BASE_MEDIUM_HIGH);
		else if (!enabled) bg = Kcolor_rgba(0, 0, 0, 0);
	}
	if (bg.a > 0) kst_fill_rect(p, r, bg);
	if (kst_is_a(e, K_CLASS_LIST_VIEW_ITEM) && multi_check(o)) {
		/* the check box of ListMultiSelect: a 25.5 square at the left, 9.5 from the top */
		KRect box = kst_rect(0, CHECK_BOX_TOP, CHECK_BOX, CHECK_BOX);
		KColor line = kst_theme_color(e, enabled ? KST_TC_FOREGROUND : KST_TC_BASE_LOW);
		kst_stroke_rect(p, box, Kthickness_uniform(KST_81_BORDER_THICKNESS), line);
		if (it->selected) check_glyph(p, box.x + (CHECK_BOX - 18.5f) * 0.5f, box.y + (CHECK_BOX - 17.0f) * 0.5f, 18.5f, 17.0f, line);
	} else if (kst_is_a(e, K_CLASS_GRID_VIEW_ITEM) && multi_check(o) && it->selected && enabled) {
		/* GridMultiSelect: SelectedBorder of 2.5 in the accent and the earmark with the check in the top right corner */
		KColor accent = kst_theme_color(e, KST_TC_ACCENT);
		float tri[6] = { r.width - EARMARK, 0, r.width, 0, r.width, EARMARK };
		kst_stroke_rect(p, r, Kthickness_uniform(2.5f), accent);
		kst_fill_polygon(p, tri, 3, accent);
		check_glyph(p, r.width - 1.0f - 17.0f, 1.0f, 17.0f, 14.5f, kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT));
	}
	if (item_focused(e, o)) kst_stroke_rect(p, kst_rect(1, 1, r.width - 2, r.height - 2), Kthickness_uniform(1), kst_theme_color(e, KST_TC_BASE_MID));
}

/** What a tap on a container does, by the control it is in. */
static void item_tapped(kst_element_t* e) {
	kst_items_control_t* o = kst_item_owner(e);
	int index;
	if (!o || e->parent != o->host) return;
	index = kst_ic_index_of(o, e);
	if (index < 0) return;
	o->focus_visible = 0;
	if (kst_is_a(&o->base.base, K_CLASS_COMBO_BOX)) kst_combo_item_tapped(o, index);
	else if (kst_is_a(&o->base.base, K_CLASS_LIST_VIEW_BASE)) kst_lvb_item_tapped(o, index);
	else if (kst_is_a(&o->base.base, K_CLASS_FLIP_VIEW)) return;
	else {
		o->focus_item = e->id;
		kst_ic_tap(o, index);
	}
}

static int item_input(kst_element_t* e, kst_input_t* in) {
	kst_selector_item_t* it = ITEM(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			/* the container is enabled by itself, but its control is not */
			if (!kst_effectively_enabled(e)) return 1;
			it->pressed = 1;
			it->press_x = in->x;
			it->press_y = in->y;
			kst_changed_paint(e);
			if (e->root) kst_set_capture(e->root, e);
			return 1;
		case KST_IN_POINTER_UP: {
			int tap = it->pressed && in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			it->pressed = 0;
			kst_changed_paint(e);
			if (e->root) kst_release_capture(e->root, e);
			if (tap) item_tapped(e);
			return 1;
		}
		case KST_IN_CAPTURE_LOST:
			it->pressed = 0;
			kst_changed_paint(e);
			return 0;
		default:
			return 0;
	}
}

static void item_init(kst_element_t* e) {
	kst_control_t* c = &ITEM(e)->base.base;
	c->h_content = K_HALIGN_LEFT;
	c->v_content = K_VALIGN_TOP;
	c->font_size = ITEM_FONT_SIZE;
	e->focusable = 0;
}

kst_class_t kst_class_selector_item = {
	.base = &kst_class_content_control, .name = "SelectorItem", .id = K_CLASS_SELECTOR_ITEM, .size = sizeof(kst_selector_item_t), .abstract_class = 1,
	.init = item_init, .measure = item_measure, .paint = item_paint, .input = item_input, .chrome = item_chrome, .content_foreground = item_content_foreground,
	.content_font_size = item_font_size, .content_font_family = item_font_family
};

static void list_box_item_init(kst_element_t* e) { CTL(e)->padding = Kthickness(6.5f, 8, 6.5f, 8); }

static void combo_box_item_init(kst_element_t* e) {
	kst_control_t* c = CTL(e);
	c->padding = Kthickness(6.5f, 8.5f, 0, 16);
	c->h_content = K_HALIGN_STRETCH;
	c->font_size = KST_81_FONT_CONTENT;
	e->valign = K_VALIGN_TOP;
}

static void flip_view_item_init(kst_element_t* e) {
	kst_control_t* c = CTL(e);
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
	c->font_size = KST_81_FONT_CONTENT;
}

kst_class_t kst_class_list_box_item = {
	.base = &kst_class_selector_item, .name = "ListBoxItem", .id = K_CLASS_LIST_BOX_ITEM, .size = sizeof(kst_selector_item_t), .init = list_box_item_init
};

kst_class_t kst_class_combo_box_item = {
	.base = &kst_class_selector_item, .name = "ComboBoxItem", .id = K_CLASS_COMBO_BOX_ITEM, .size = sizeof(kst_selector_item_t), .init = combo_box_item_init
};

kst_class_t kst_class_flip_view_item = {
	.base = &kst_class_selector_item, .name = "FlipViewItem", .id = K_CLASS_FLIP_VIEW_ITEM, .size = sizeof(kst_selector_item_t), .init = flip_view_item_init
};

static kst_element_t* make_item(KClass cls, const char* text) {
	kst_element_t* e = kst_create(cls);
	if (!e) return NULL;
	if (text) kst_content_set_text(&ITEM(e)->base, text);
	return e;
}

static Kid create_item(KClass cls, const char* content) {
	kst_element_t* e = make_item(cls, content);
	return e ? e->id : K_INVALID_ID;
}

Kid Klist_box_item_create(const char* content) { return create_item(K_CLASS_LIST_BOX_ITEM, content); }
Kid Kcombo_box_item_create(const char* content) { return create_item(K_CLASS_COMBO_BOX_ITEM, content); }
Kid Kflip_view_item_create(const char* content) { return create_item(K_CLASS_FLIP_VIEW_ITEM, content); }
Kid Klist_view_item_create(const char* content) { return create_item(K_CLASS_LIST_VIEW_ITEM, content); }
Kid Kgrid_view_item_create(const char* content) { return create_item(K_CLASS_GRID_VIEW_ITEM, content); }


/* ============================================================================
 *  ItemsControl
 * ============================================================================ */

static KThickness ic_insets(const kst_element_t* e) {
	const kst_control_t* c = CTL(e);
	return Kthickness(c->border_thickness.left + c->padding.left, c->border_thickness.top + c->padding.top,
		c->border_thickness.right + c->padding.right, c->border_thickness.bottom + c->padding.bottom);
}

static KSize ic_measure(kst_element_t* e, KSize available) {
	KThickness in = ic_insets(e);
	KSize s = { 0, 0 };
	if (e->child_count) {
		kst_measure(e->children[0], kst_size(kst_maxf(0, available.width - kst_thickness_h(in)), kst_maxf(0, available.height - kst_thickness_v(in))));
		s = e->children[0]->desired;
	}
	return kst_size(s.width + kst_thickness_h(in), s.height + kst_thickness_v(in));
}

static KSize ic_arrange(kst_element_t* e, KSize final) {
	KThickness in = ic_insets(e);
	if (e->child_count) kst_arrange(e->children[0], kst_rect(in.left, in.top, kst_maxf(0, final.width - kst_thickness_h(in)), kst_maxf(0, final.height - kst_thickness_v(in))));
	return final;
}

static void ic_paint(kst_element_t* e, kst_painter_t* p) {
	kst_control_t* c = CTL(e);
	KRect r = kst_local_rect(e);
	int box = kst_is_a(e, K_CLASS_LIST_BOX), enabled = kst_effectively_enabled(e);
	KColor bg = box ? kst_theme_color(e, KST_TC_BASE_MEDIUM_HIGH) : Kcolor_rgba(0, 0, 0, 0);
	KColor border = box ? kst_theme_color(e, KST_TC_BASE_BLACK) : Kcolor_rgba(0, 0, 0, 0);
	bg = kst_ctl_background(c, bg);
	border = kst_ctl_border_brush(c, Kcolor_rgba(0, 0, 0, 0));
	if (box && !enabled) {
		/* Disabled: a transparent background and the border in ListBoxDisabledForegroundThemeBrush */
		bg = Kcolor_rgba(0, 0, 0, 0);
		border = kst_theme_color(e, KST_TC_PHONE_DISABLED);
	}
	if (bg.a > 0) kst_fill_rect(p, r, bg);
	if (border.a > 0) kst_stroke_rect(p, r, c->border_thickness, border);
}

static void ic_init(kst_element_t* e) {
	kst_items_control_t* o = IC(e);
	kst_element_t* host = kst_create(K_CLASS_STACK_PANEL);
	o->selected_index = -1;
	o->sel_mode = KST_SEL_SINGLE;
	e->focusable = 0;
	if (host) {
		kst_add_child(e, host, -1);
		o->host = host;
	}
}

static void ic_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_items_control_t* o = IC(e);
	if (o->host == child) o->host = NULL;
	if (o->scroll == child) {
		o->scroll = NULL;
		o->presenter = NULL;
		o->host = NULL;
	}
}

kst_class_t kst_class_items_control = {
	.base = &kst_class_control, .name = "ItemsControl", .id = K_CLASS_ITEMS_CONTROL, .size = sizeof(kst_items_control_t),
	.init = ic_init, .measure = ic_measure, .arrange = ic_arrange, .paint = ic_paint, .child_removed = ic_child_removed
};

kst_class_t kst_class_selector = {
	.base = &kst_class_items_control, .name = "Selector", .id = K_CLASS_SELECTOR, .size = sizeof(kst_items_control_t), .abstract_class = 1
};

static kst_items_control_t* get_ic(Kid id) { return IC(kst_get(id, K_CLASS_ITEMS_CONTROL)); }
static kst_items_control_t* get_selector(Kid id) { return IC(kst_get(id, K_CLASS_SELECTOR)); }

Kid Kitems_control_create(void) {
	kst_element_t* e = kst_create(K_CLASS_ITEMS_CONTROL);
	return e ? e->id : K_INVALID_ID;
}

kst_element_t* kst_ic_add(kst_items_control_t* o, kst_element_t* item, const char* text) {
	kst_element_t* container;
	if (!o->host) return NULL;
	if (!is_selector(o) && !kst_is_a(&o->base.base, K_CLASS_AUTO_SUGGEST_BOX)) {
		/* a plain ItemsControl shows the element itself */
		container = item;
		if (!container) {
			container = kst_create(K_CLASS_TEXT_BLOCK);
			if (container) kst_set_string(&KST_AS(kst_text_block_t, container)->text, text);
		}
	} else if (item && is_container(item)) {
		container = item;
	} else {
		container = make_item(container_class(o), text);
		if (container && item) kst_content_set(&ITEM(container)->base, item);
	}
	if (!container) return NULL;
	if (kst_is_a(&o->base.base, K_CLASS_AUTO_SUGGEST_BOX) && is_container(container) && !(CTL(container)->set_flags & KST_C_FOREGROUND)) {
		/* the ItemContainerStyle of AutoSuggestBox: Margin 6.5,9.5,0,0, ContentControlFontSize and the foreground of a TextBox */
		container->margin = Kthickness(6.5f, 9.5f, 0, 0);
		Kcontrol_set_font_size(container->id, KST_81_FONT_CONTENT);
		Kcontrol_set_foreground(container->id, kst_theme_color(container, KST_TC_BASE_BLACK));
	}
	kst_add_child(o->host, container, -1);
	if (container->parent != o->host) return NULL;
	if (kst_is_a(&o->base.base, K_CLASS_FLIP_VIEW)) {
		kst_changed_layout(&o->base.base);
		kst_flip_items_changed(o);
	}
	return container;
}

/* OBSOLETE DEPRECATED TODO delete this method, add text_block like other widgets*/
Kid Kitems_control_add_text(Kid id, const char* text) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* c;
	if (!o) return K_INVALID_ID;
	c = kst_ic_add(o, NULL, text ? text : "");
	return c ? c->id : K_INVALID_ID;
}

Kid Kitems_control_add(Kid id, Kid item) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* e = kst_get(item, K_CLASS_ELEMENT), *c;
	if (!o || !e) return K_INVALID_ID;
	c = kst_ic_add(o, e, NULL);
	return c ? c->id : K_INVALID_ID;
}

static void ic_refresh_selected_index(kst_items_control_t* o) {
	int i, n = kst_ic_count(o);
	o->selected_index = -1;
	for (i = 0; i < n; i++) if (ITEM(kst_ic_item(o, i))->selected) { o->selected_index = i; return; }
}

void Kitems_control_remove_at(Kid id, int index) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* item;
	int was_selected, old_selected;
	if (!o || !(item = kst_ic_item(o, index))) return;
	was_selected = is_selector(o) && is_container(item) && ITEM(item)->selected;
	old_selected = o->selected_index;
	kst_destroy(item);
	if (is_selector(o)) {
		ic_refresh_selected_index(o);
		if (kst_is_a(&o->base.base, K_CLASS_FLIP_VIEW)) {
			/* the item that takes the place of the one that was shown is shown */
			o->selected_index = was_selected ? index : (old_selected > index ? old_selected - 1 : old_selected);
			kst_flip_items_changed(o);
		}
		if (was_selected && !kst_raise(&o->base.base, K_EVENT_SELECTION_CHANGED)) return;
	}
	if (kst_lookup(id)) kst_changed_layout(&o->base.base);
}

void Kitems_control_clear(Kid id) {
	kst_items_control_t* o = get_ic(id);
	int had = 0, i, n;
	if (!o || !o->host) return;
	n = kst_ic_count(o);
	for (i = 0; i < n && !had; i++) {
		kst_element_t* item = kst_ic_item(o, i);
		had = is_container(item) && ITEM(item)->selected;
	}
	kst_clear_children(o->host);
	o->selected_index = -1;
	if (kst_is_a(&o->base.base, K_CLASS_FLIP_VIEW)) kst_flip_items_changed(o);
	o->group_count = 0;
	o->focus_item = K_INVALID_ID;
	if (had && is_selector(o) && !kst_raise(&o->base.base, K_EVENT_SELECTION_CHANGED)) return;
	if (kst_lookup(id)) kst_changed_layout(&o->base.base);
}

int Kitems_control_get_count(Kid id) {
	kst_items_control_t* o = get_ic(id);
	return o ? kst_ic_count(o) : 0;
}

Kid Kitems_control_get_container(Kid id, int index) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* item = o ? kst_ic_item(o, index) : NULL;
	return item ? item->id : K_INVALID_ID;
}

int Kitems_control_index_from_container(Kid id, Kid container) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* c = kst_get(container, K_CLASS_ELEMENT);
	return o && c ? kst_ic_index_of(o, c) : -1;
}

const char* kst_container_text(kst_element_t* container) {
	if (kst_is_a(container, K_CLASS_TEXT_BLOCK)) return Ktext_block_get_text(container->id);
	if (kst_is_a(container, K_CLASS_CONTENT_CONTROL)) return Kcontent_control_get_text(container->id);
	return "";
}

const char* Kitems_control_get_text(Kid id, int index) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* item = o ? kst_ic_item(o, index) : NULL;
	return item ? kst_container_text(item) : "";
}

Kid Kitems_control_get_scroll_viewer(Kid id) {
	kst_items_control_t* o = get_ic(id);
	return o && o->scroll ? o->scroll->id : K_INVALID_ID;
}


/* ============================================================================
 *  the items panel and the scrolling
 * ============================================================================ */

/* The view scrolls along its panel: sideways for a horizontal stack and for a vertical wrap grid. */
static int panel_scrolls_sideways(const kst_element_t* panel) {
	if (!panel) return 0;
	if (kst_is_a(panel, K_CLASS_ITEMS_WRAP_GRID)) return KST_AS(const kst_items_wrap_grid_t, panel)->orientation == K_VERTICAL;
	if (kst_is_a(panel, K_CLASS_STACK_PANEL)) return KST_AS(const kst_stack_panel_t, panel)->orientation == K_HORIZONTAL;
	if (kst_is_a(panel, K_CLASS_WRAP_PANEL)) return KST_AS(const kst_wrap_panel_t, panel)->orientation == K_VERTICAL;
	return 0;
}

static void ic_sync_scroll(kst_items_control_t* o) {
	int sideways = panel_scrolls_sideways(o->host);
	if (o->presenter) Kstack_panel_set_orientation(o->presenter->id, sideways ? K_HORIZONTAL : K_VERTICAL);
	if (o->scroll && !kst_is_a(&o->base.base, K_CLASS_COMBO_BOX)) {
		Kscroll_viewer_set_vertical_scrollbar_visibility(o->scroll->id, sideways ? K_SCROLLBAR_DISABLED : K_SCROLLBAR_AUTO);
		Kscroll_viewer_set_horizontal_scrollbar_visibility(o->scroll->id, sideways ? K_SCROLLBAR_AUTO : K_SCROLLBAR_DISABLED);
	}
}

void kst_items_panel_changed(kst_element_t* panel) {
	kst_element_t* a;
	for (a = panel->parent; a; a = a->parent) {
		if (kst_is_a(a, K_CLASS_ITEMS_CONTROL) && IC(a)->host == panel) {
			ic_sync_scroll(IC(a));
			return;
		}
	}
}

void kst_ic_set_host(kst_items_control_t* o, kst_element_t* panel) {
	kst_element_t* old = o->host;
	kst_element_t** moved = NULL;
	int count = 0, i;
	if (old && old->child_count) {
		moved = (kst_element_t**)malloc((size_t)old->child_count * sizeof *moved);
		if (!moved) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			return;
		}
		while (old->child_count > 0) {
			kst_element_t* c = old->children[0];
			moved[count++] = c;
			kst_remove_child(old, c);
		}
	}
	if (o->presenter && old && old->parent == o->presenter) {
		int at = kst_child_index(o->presenter, old);
		kst_destroy(old);
		kst_add_child(o->presenter, panel, at);
	} else if (o->scroll && old && old->parent == o->scroll) {
		kst_content_set(KST_AS(kst_content_control_t, o->scroll), panel);
	} else {
		if (old) kst_destroy(old);
		kst_add_child(&o->base.base, panel, 0);
	}
	o->host = panel;
	for (i = 0; i < count; i++) kst_add_child(panel, moved[i], -1);
	free(moved);
	ic_sync_scroll(o);
	kst_changed_layout(&o->base.base);
}

void Kitems_control_set_items_panel(Kid id, Kid panel) {
	kst_items_control_t* o = get_ic(id);
	kst_element_t* p = kst_get(panel, K_CLASS_PANEL);
	if (!o || !p) return;
	if (p->parent) {
		kst_error(K_ERROR_HAS_PARENT, "the items panel already has a parent");
		return;
	}
	kst_ic_set_host(o, p);
}

Kid Kitems_control_get_items_panel(Kid id) {
	kst_items_control_t* o = get_ic(id);
	return o && o->host ? o->host->id : K_INVALID_ID;
}

void kst_ic_scroll_into_view(kst_items_control_t* o, int index, KScrollIntoViewAlignment alignment) {
	kst_element_t* item = kst_ic_item(o, index);
	kst_element_t* content;
	KPoint at, origin;
	int sideways;
	double start, end, offset, view;
	if (!item || !o->scroll || !o->base.base.root) return;
	Kroot_update_layout(o->base.base.root->id);
	content = KST_AS(kst_content_control_t, o->scroll)->content;
	if (!content) return;
	at = kst_to_root(item, (KPoint){ 0, 0 });
	origin = kst_to_root(content, (KPoint){ 0, 0 });
	sideways = panel_scrolls_sideways(o->host);
	if (sideways) {
		start = at.x - origin.x;
		end = start + item->rect.width;
		offset = Kscroll_viewer_get_horizontal_offset(o->scroll->id);
		view = Kscroll_viewer_get_viewport_width(o->scroll->id);
	} else {
		start = at.y - origin.y;
		end = start + item->rect.height;
		offset = Kscroll_viewer_get_vertical_offset(o->scroll->id);
		view = Kscroll_viewer_get_viewport_height(o->scroll->id);
	}
	if (alignment == K_SCROLL_INTO_VIEW_LEADING) offset = start;
	else if (start < offset) offset = start;
	else if (end > offset + view) offset = end - view;
	else return;
	if (sideways) Kscroll_viewer_scroll_to_horizontal_offset(o->scroll->id, offset);
	else Kscroll_viewer_scroll_to_vertical_offset(o->scroll->id, offset);
}


/* ============================================================================
 *  selection
 * ============================================================================ */

int kst_ic_selected_indices(const kst_items_control_t* o, int* out, int max) {
	int i, n = kst_ic_count(o), found = 0;
	for (i = 0; i < n; i++) {
		if (!ITEM(kst_ic_item(o, i))->selected) continue;
		if (out && found < max) out[found] = i;
		found++;
	}
	return found;
}

static int selected_count(const kst_items_control_t* o) { return kst_ic_selected_indices(o, NULL, 0); }

/* Selects exactly the items of `want` (index or -1; or all when `all`): K_TRUE when the selection changed. */
static int apply_selection(kst_items_control_t* o, int want, int all) {
	int i, n = kst_ic_count(o), changed = 0;
	for (i = 0; i < n; i++) {
		kst_element_t* item = kst_ic_item(o, i);
		int selected = all || i == want;
		if (ITEM(item)->selected != selected) {
			item_set_selected(item, selected);
			changed = 1;
		}
	}
	ic_refresh_selected_index(o);
	return changed;
}

void kst_ic_select(kst_items_control_t* o, int index) {
	kst_element_t* e = &o->base.base;
	int flip = kst_is_a(e, K_CLASS_FLIP_VIEW);
	if (o->sel_mode == KST_SEL_NONE) return;
	if (index >= kst_ic_count(o) || index < -1) index = -1;
	/* a FlipView always shows an item */
	if (flip && index < 0 && kst_ic_count(o) > 0) return;
	if (apply_selection(o, index, 0)) {
		kst_changed_paint(e);
		if (flip) kst_flip_selected(o);
		kst_raise(e, K_EVENT_SELECTION_CHANGED);
	}
}

void kst_ic_tap(kst_items_control_t* o, int index) {
	kst_element_t* e = &o->base.base;
	kst_element_t* item = kst_ic_item(o, index);
	if (!item || o->sel_mode == KST_SEL_NONE) return;
	if (o->sel_mode == KST_SEL_MULTIPLE) {
		item_set_selected(item, !ITEM(item)->selected);
		ic_refresh_selected_index(o);
		kst_changed_paint(e);
		kst_raise(e, K_EVENT_SELECTION_CHANGED);
		return;
	}
	kst_ic_select(o, index);
}

void kst_ic_select_all(kst_items_control_t* o) {
	kst_element_t* e = &o->base.base;
	if (o->sel_mode != KST_SEL_MULTIPLE && o->sel_mode != KST_SEL_EXTENDED) return;
	if (apply_selection(o, -1, 1)) {
		kst_changed_paint(e);
		kst_raise(e, K_EVENT_SELECTION_CHANGED);
	}
}

void kst_ic_set_sel_mode(kst_items_control_t* o, kst_sel_mode_t mode) {
	kst_element_t* e = &o->base.base;
	int keep = o->selected_index;
	if (o->sel_mode == mode) return;
	o->sel_mode = mode;
	if (mode == KST_SEL_NONE) keep = -1;
	if (mode == KST_SEL_NONE || mode == KST_SEL_SINGLE) {
		if (selected_count(o) > (keep >= 0 ? 1 : 0) || (mode == KST_SEL_NONE && selected_count(o) > 0)) {
			apply_selection(o, keep, 0);
			kst_changed_paint(e);
			if (!kst_raise(e, K_EVENT_SELECTION_CHANGED)) return;
		}
	}
	/* the check boxes come or go, and the rows are measured again */
	kst_invalidate_deep(e);
}

void Kselector_set_selected_index(Kid id, int index) {
	kst_items_control_t* o = get_selector(id);
	if (o) kst_ic_select(o, index);
}

int Kselector_get_selected_index(Kid id) {
	kst_items_control_t* o = get_selector(id);
	return o ? o->selected_index : -1;
}

Kid Kselector_get_selected_container(Kid id) {
	kst_items_control_t* o = get_selector(id);
	kst_element_t* item = o && o->selected_index >= 0 ? kst_ic_item(o, o->selected_index) : NULL;
	return item ? item->id : K_INVALID_ID;
}

void Kselector_item_set_is_selected(Kid id, int is_selected) {
	kst_element_t* e = kst_get(id, K_CLASS_SELECTOR_ITEM);
	kst_items_control_t* o;
	if (!e) return;
	is_selected = is_selected != 0;
	o = kst_item_owner(e);
	if (!o || e->parent != o->host || !is_selector(o)) {
		item_set_selected(e, is_selected);
		return;
	}
	if (o->sel_mode == KST_SEL_NONE || ITEM(e)->selected == is_selected) return;
	if (o->sel_mode == KST_SEL_SINGLE) {
		int index = kst_ic_index_of(o, e);
		if (is_selected) kst_ic_select(o, index);
		else if (o->selected_index == index) kst_ic_select(o, -1);
		return;
	}
	item_set_selected(e, is_selected);
	ic_refresh_selected_index(o);
	kst_raise(&o->base.base, K_EVENT_SELECTION_CHANGED);
}

int Kselector_item_get_is_selected(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_SELECTOR_ITEM);
	return e ? ITEM(e)->selected : 0;
}


/* ============================================================================
 *  keys
 * ============================================================================ */

/* How many items are in the first line of a wrap grid, for the Up and Down keys. */
static int grid_columns(const kst_items_control_t* o) {
	int n = kst_ic_count(o), i, columns = 1;
	kst_element_t* first = kst_ic_item(o, 0);
	if (!first || !kst_is_a(o->host, K_CLASS_ITEMS_WRAP_GRID)) return 1;
	for (i = 1; i < n; i++) {
		kst_element_t* item = kst_ic_item(o, i);
		if (KST_AS(kst_items_wrap_grid_t, o->host)->orientation == K_HORIZONTAL ? item->rect.y == first->rect.y : item->rect.x == first->rect.x) columns++;
		else break;
	}
	return columns;
}

int kst_ic_key(kst_element_t* e, kst_input_t* in) {
	kst_items_control_t* o = IC(e);
	int n = kst_ic_count(o), at, next, step = 0, grid = o->host && kst_is_a(o->host, K_CLASS_ITEMS_WRAP_GRID);
	kst_element_t* focus = kst_lookup(o->focus_item);
	int sideways = panel_scrolls_sideways(o->host);
	int is_list_view = kst_is_a(e, K_CLASS_LIST_VIEW_BASE);
	if (n == 0) return 0;
	at = focus ? kst_ic_index_of(o, focus) : -1;
	if (at < 0) at = o->selected_index;
	switch (in->key) {
		case K_KEY_DOWN: step = sideways && !grid ? 0 : (grid && !sideways ? grid_columns(o) : 1); break;
		case K_KEY_UP: step = sideways && !grid ? 0 : -(grid && !sideways ? grid_columns(o) : 1); break;
		case K_KEY_RIGHT: step = grid || sideways ? 1 : 0; break;
		case K_KEY_LEFT: step = grid || sideways ? -1 : 0; break;
		case K_KEY_HOME: next = 0; goto go;
		case K_KEY_END: next = n - 1; goto go;
		case K_KEY_SPACE:
		case K_KEY_ENTER:
			if (at < 0) return 0;
			if (is_list_view) kst_lvb_item_tapped(o, at);
			else kst_ic_tap(o, at);
			return 1;
		case K_KEY_A:
			if (!(in->mods & K_MOD_CTRL)) return 0;
			kst_ic_select_all(o);
			return 1;
		default:
			return 0;
	}
	if (step == 0) return 0;
	next = at < 0 ? (step > 0 ? 0 : n - 1) : at + step;
	if (next < 0 || next >= n) next = at < 0 ? 0 : at;
go:
	{
		kst_element_t* item = kst_ic_item(o, next);
		Kid id = e->id;
		if (!item) return 0;
		o->focus_item = item->id;
		o->focus_visible = 1;
		kst_changed_paint(e);
		/* the keyboard selects where a tap would select without toggling */
		if (o->sel_mode == KST_SEL_SINGLE || o->sel_mode == KST_SEL_EXTENDED) kst_ic_select(o, next);
		if (!kst_lookup(id)) return 1;
		kst_ic_scroll_into_view(o, next, K_SCROLL_INTO_VIEW_DEFAULT);
	}
	return 1;
}


/* ============================================================================
 *  ListBox
 * ============================================================================ */

static void list_box_init(kst_element_t* e) {
	kst_items_control_t* o = IC(e);
	kst_control_t* c = CTL(e);
	kst_element_t* scroll = kst_create(K_CLASS_SCROLL_VIEWER);
	kst_element_t* host = kst_create(K_CLASS_VIRTUALIZING_STACK_PANEL);
	/* ListBox: BorderThickness 1.5, FontSize 12, IsTabStop False but it takes the keyboard when it is touched */
	c->border_thickness = Kthickness_uniform(1.5f);
	c->font_size = ITEM_FONT_SIZE;
	e->focusable = 1;
	e->tab_stop = 0;
	if (!scroll || !host) return;
	/* the plain panel made by ItemsControl is replaced by the virtualizing one inside a scroll viewer */
	if (o->host) kst_destroy(o->host);
	kst_add_child(e, scroll, -1);
	kst_content_set(KST_AS(kst_content_control_t, scroll), host);
	o->scroll = scroll;
	o->host = host;
}

static int list_box_input(kst_element_t* e, kst_input_t* in) {
	if (in->type == KST_IN_KEY_DOWN) return kst_ic_key(e, in);
	return 0;
}

kst_class_t kst_class_list_box = {
	.base = &kst_class_selector, .name = "ListBox", .id = K_CLASS_LIST_BOX, .size = sizeof(kst_items_control_t),
	.init = list_box_init, .input = list_box_input
};

Kid Klist_box_create(void) {
	kst_element_t* e = kst_create(K_CLASS_LIST_BOX);
	return e ? e->id : K_INVALID_ID;
}

static kst_items_control_t* get_list_box(Kid id) { return IC(kst_get(id, K_CLASS_LIST_BOX)); }

void Klist_box_set_selection_mode(Kid id, KSelectionMode mode) {
	kst_items_control_t* o = get_list_box(id);
	if (!o) return;
	kst_ic_set_sel_mode(o, mode == K_SELECTION_MULTIPLE ? KST_SEL_MULTIPLE : mode == K_SELECTION_EXTENDED ? KST_SEL_EXTENDED : KST_SEL_SINGLE);
}

KSelectionMode Klist_box_get_selection_mode(Kid id) {
	kst_items_control_t* o = get_list_box(id);
	if (!o) return K_SELECTION_SINGLE;
	return o->sel_mode == KST_SEL_MULTIPLE ? K_SELECTION_MULTIPLE : o->sel_mode == KST_SEL_EXTENDED ? K_SELECTION_EXTENDED : K_SELECTION_SINGLE;
}

int Klist_box_get_selected_indices(Kid id, int* indices, int max) {
	kst_items_control_t* o = get_list_box(id);
	return o ? kst_ic_selected_indices(o, indices, max) : 0;
}

void Klist_box_select_all(Kid id) {
	kst_items_control_t* o = get_list_box(id);
	if (o) kst_ic_select_all(o);
}

void Klist_box_scroll_into_view(Kid id, int index) {
	kst_items_control_t* o = get_list_box(id);
	if (o) kst_ic_scroll_into_view(o, index, K_SCROLL_INTO_VIEW_DEFAULT);
}
