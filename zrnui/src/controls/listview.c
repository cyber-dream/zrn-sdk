/**
 * zrnui - controls/listview.c
 * ListViewBase, ListView, GridView and the header items of their groups.
 *
 * The 8.1 template of ListView is a Border with a ScrollViewer around an ItemsPresenter: the header,
 * the items panel and the footer, which scroll together. A ListView stacks its items in an
 * ItemsStackPanel, a GridView wraps them in an ItemsWrapGrid. What the containers look like is in items.c.
 */

#include "../internal.h"

#define IC(e) ZST_AS(zrn_items_control_t, e)
#define LVB(e) ZST_AS(zrn_list_view_base_t, e)


static int lvb_input(zrn_element_t* const e, zrn_input_t* const in) {
	switch (in->type) {
		case ZST_IN_KEY_DOWN: return zrn_ic_key(e, in);
		case ZST_IN_FOCUS_IN:
		case ZST_IN_FOCUS_OUT:
			zrn_changed_paint(e);
			return 0;
		default: return 0;
	}
}

static void lvb_init(zrn_element_t* const e) {
	const auto o = IC(e);
	zrn_element_t* const scroll = zrn_create(Z_CLASS_SCROLL_VIEWER);
	zrn_element_t* const presenter = zrn_create(Z_CLASS_STACK_PANEL);
	zrn_element_t* const host = zrn_create(Z_CLASS_ITEMS_STACK_PANEL);
	/* ListView and GridView: IsTabStop False, IsSwipeEnabled True; a touch gives them the keyboard */
	e->focusable = 1;
	e->tab_stop = 0;
	LVB(e)->swipe_enabled = 1;
	if (!scroll || !presenter || !host) return;
	if (o->host) zrn_destroy(o->host);
	zrn_add_child(e, scroll, -1);
	zrn_content_set(ZST_AS(zrn_content_control_t, scroll), presenter);
	zrn_add_child(presenter, host, -1);
	o->scroll = scroll;
	o->presenter = presenter;
	o->host = host;
}

zrn_class_t zrn_class_list_view_base = {
	.base = &zrn_class_selector, .name = "ListViewBase", .id = Z_CLASS_LIST_VIEW_BASE, .size = sizeof(zrn_list_view_base_t), .abstract_class = 1,
	.init = lvb_init, .input = lvb_input
};

static void grid_view_init(zrn_element_t* const e) {
	zrn_element_t* const grid = zrn_create(Z_CLASS_ITEMS_WRAP_GRID);
	if (grid) zrn_ic_set_host(IC(e), grid);
}

zrn_class_t zrn_class_list_view = {
	.base = &zrn_class_list_view_base, .name = "ListView", .id = Z_CLASS_LIST_VIEW, .size = sizeof(zrn_list_view_base_t)
};

zrn_class_t zrn_class_grid_view = {
	.base = &zrn_class_list_view_base, .name = "GridView", .id = Z_CLASS_GRID_VIEW, .size = sizeof(zrn_list_view_base_t), .init = grid_view_init
};

/* The tiles of a GridView and the rows of a ListView are the containers made in items.c. */
zrn_class_t zrn_class_list_view_item = {
	.base = &zrn_class_selector_item, .name = "ListViewItem", .id = Z_CLASS_LIST_VIEW_ITEM, .size = sizeof(zrn_selector_item_t)
};

zrn_class_t zrn_class_grid_view_item = {
	.base = &zrn_class_selector_item, .name = "GridViewItem", .id = Z_CLASS_GRID_VIEW_ITEM, .size = sizeof(zrn_selector_item_t)
};

Zid Zlist_view_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_LIST_VIEW);
	return e ? e->id : Z_INVALID_ID;
}

Zid Zgrid_view_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_GRID_VIEW);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_list_view_base_t* get_lvb(const Zid id) { return LVB(zrn_get(id, Z_CLASS_LIST_VIEW_BASE)); }

void zrn_lvb_item_tapped(zrn_items_control_t* const o, const int index) {
	const auto l = (zrn_list_view_base_t*)o;
	const Zid id = o->base.base.id;
	const zrn_element_t* const item = zrn_ic_item(o, index);
	if (item) o->focus_item = item->id;
	if (o->sel_mode != ZST_SEL_NONE) {
		zrn_ic_tap(o, index);
		if (!zrn_lookup(id)) return;
	}
	/* a tap selects as the mode says, then ItemClick comes (the order is not in the SDK) */
	if (l->item_click_enabled && l->on_item_click) l->on_item_click(id, index, l->on_item_click_data);
}

void Zlist_view_set_selection_mode(const Zid list_view, const ZListViewSelectionMode mode) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	zrn_sel_mode_t m;
	if (!l) return;
	m = mode == Z_LIST_VIEW_SELECTION_NONE ? ZST_SEL_NONE : mode == Z_LIST_VIEW_SELECTION_MULTIPLE ? ZST_SEL_MULTIPLE :
		mode == Z_LIST_VIEW_SELECTION_EXTENDED ? ZST_SEL_EXTENDED : ZST_SEL_SINGLE;
	zrn_ic_set_sel_mode(&l->base, m);
}

ZListViewSelectionMode Zlist_view_get_selection_mode(const Zid list_view) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (!l) return Z_LIST_VIEW_SELECTION_SINGLE;
	switch (l->base.sel_mode) {
		case ZST_SEL_NONE: return Z_LIST_VIEW_SELECTION_NONE;
		case ZST_SEL_MULTIPLE: return Z_LIST_VIEW_SELECTION_MULTIPLE;
		case ZST_SEL_EXTENDED: return Z_LIST_VIEW_SELECTION_EXTENDED;
		default: return Z_LIST_VIEW_SELECTION_SINGLE;
	}
}

void Zlist_view_set_is_item_click_enabled(const Zid list_view, const int enabled) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) l->item_click_enabled = enabled != 0;
}

int Zlist_view_get_is_item_click_enabled(const Zid list_view) {
	const zrn_list_view_base_t* const l = get_lvb(list_view);
	return l ? l->item_click_enabled : 0;
}

void Zlist_view_on_item_click(const Zid list_view, const ZItemClickCallback callback, void* const user_data) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (!l) return;
	l->on_item_click = callback;
	l->on_item_click_data = user_data;
}

void Zlist_view_set_is_swipe_enabled(const Zid list_view, const int enabled) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) l->swipe_enabled = enabled != 0;
}

int Zlist_view_get_is_swipe_enabled(const Zid list_view) {
	const zrn_list_view_base_t* const l = get_lvb(list_view);
	return l ? l->swipe_enabled : 0;
}

int Zlist_view_get_selected_indices(const Zid list_view, int* const indices, const int max) {
	const zrn_list_view_base_t* const l = get_lvb(list_view);
	return l ? zrn_ic_selected_indices(&l->base, indices, max) : 0;
}

void Zlist_view_select_all(const Zid list_view) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) zrn_ic_select_all(&l->base);
}

void Zlist_view_scroll_into_view(const Zid list_view, const int index) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) zrn_ic_scroll_into_view(&l->base, index, Z_SCROLL_INTO_VIEW_DEFAULT);
}

void Zlist_view_scroll_into_view_with_alignment(const Zid list_view, const int index, const ZScrollIntoViewAlignment alignment) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) zrn_ic_scroll_into_view(&l->base, index, alignment);
}


/* ============================================================================
 *  Header and Footer
 * ============================================================================ */

/* The header or the footer is a ContentControl before or after the items panel in the presenter. */
static void set_part(zrn_list_view_base_t* const l, const int footer, const char* const text, zrn_element_t* const content) {
	Zid* const slot = footer ? &l->footer : &l->header;
	zrn_element_t* holder = *slot != Z_INVALID_ID ? zrn_lookup(*slot) : nullptr;
	const zrn_items_control_t* const o = &l->base;
	const int empty = (text == nullptr || !*text) && content == nullptr;
	if (empty) {
		if (holder) zrn_destroy(holder);
		*slot = Z_INVALID_ID;
		if (content) zrn_destroy(content);
		return;
	}
	if (!o->presenter) return;
	if (!holder) {
		holder = zrn_create(Z_CLASS_CONTENT_CONTROL);
		if (!holder) return;
		zrn_control_t* c = ZST_AS(zrn_control_t, holder);
		c->font_size = ZST_81_FONT_CONTENT;
		c->h_content = Z_HALIGN_STRETCH;
		c->v_content = Z_VALIGN_TOP;
		holder->focusable = 0;
		zrn_add_child(o->presenter, holder, footer ? -1 : 0);
		if (holder->parent != o->presenter) return;
		*slot = holder->id;
	}
	if (text && *text) zrn_content_set_text(ZST_AS(zrn_content_control_t, holder), text);
	else zrn_content_set(ZST_AS(zrn_content_control_t, holder), content);
}

void Zlist_view_set_header(const Zid list_view, const char* const header) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) set_part(l, 0, header, nullptr);
}

void Zlist_view_set_header_content(const Zid list_view, const Zid header) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	zrn_element_t* const c = header != Z_INVALID_ID ? zrn_get(header, Z_CLASS_ELEMENT) : nullptr;
	if (!l || (header != Z_INVALID_ID && !c)) return;
	set_part(l, 0, nullptr, c);
}

void Zlist_view_set_footer(const Zid list_view, const char* const footer) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	if (l) set_part(l, 1, footer, nullptr);
}

void Zlist_view_set_footer_content(const Zid list_view, const Zid footer) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	zrn_element_t* const c = footer != Z_INVALID_ID ? zrn_get(footer, Z_CLASS_ELEMENT) : nullptr;
	if (!l || (footer != Z_INVALID_ID && !c)) return;
	set_part(l, 1, nullptr, c);
}


/* ============================================================================
 *  groups: ListViewHeaderItem and GridViewHeaderItem
 * ============================================================================ */

static void header_item_init(zrn_element_t* const e) {
	const auto c = ZST_AS(zrn_control_t, e);
	c->h_content = Z_HALIGN_LEFT;
	c->v_content = Z_VALIGN_TOP;
	c->font_size = 12.0f;
	e->focusable = 0;
}

zrn_class_t zrn_class_list_view_base_header_item = {
	.base = &zrn_class_content_control, .name = "ListViewBaseHeaderItem", .id = Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM,
	.size = sizeof(zrn_content_control_t), .abstract_class = 1, .init = header_item_init
};

zrn_class_t zrn_class_list_view_header_item = {
	.base = &zrn_class_list_view_base_header_item, .name = "ListViewHeaderItem", .id = Z_CLASS_LIST_VIEW_HEADER_ITEM, .size = sizeof(zrn_content_control_t)
};

zrn_class_t zrn_class_grid_view_header_item = {
	.base = &zrn_class_list_view_base_header_item, .name = "GridViewHeaderItem", .id = Z_CLASS_GRID_VIEW_HEADER_ITEM, .size = sizeof(zrn_content_control_t)
};

Zid Zlist_view_add_group_header(const Zid list_view, const char* const title) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	zrn_element_t* header, *text;
	if (!l || !l->base.host) return Z_INVALID_ID;
	header = zrn_create(zrn_is_a(&l->base.base.base, Z_CLASS_GRID_VIEW) ? Z_CLASS_GRID_VIEW_HEADER_ITEM : Z_CLASS_LIST_VIEW_HEADER_ITEM);
	if (!header) return Z_INVALID_ID;
	/* what an app puts in a GroupStyle.HeaderTemplate: a text in the GroupHeader style */
	text = zrn_create(Z_CLASS_TEXT_BLOCK);
	if (text) {
		zrn_set_string(&ZST_AS(zrn_text_block_t, text)->text, title ? title : "");
		Ztext_block_set_style(text->id, Z_TEXT_STYLE_GROUP_HEADER);
		zrn_content_set(ZST_AS(zrn_content_control_t, header), text);
	}
	zrn_add_child(l->base.host, header, -1);
	if (header->parent != l->base.host) return Z_INVALID_ID;
	l->base.group_count++;
	return header->id;
}

int Zlist_view_get_group_count(const Zid list_view) {
	zrn_list_view_base_t* const l = get_lvb(list_view);
	int i, n = 0;
	if (!l || !l->base.host) return 0;
	for (i = 0; i < l->base.host->child_count; i++) if (zrn_is_a(l->base.host->children[i], Z_CLASS_LIST_VIEW_BASE_HEADER_ITEM)) n++;
	return n;
}
