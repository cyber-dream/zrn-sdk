/**
 * Kestrel - controls/listview.c
 * ListViewBase, ListView, GridView and the header items of their groups.
 *
 * The 8.1 template of ListView is a Border with a ScrollViewer around an ItemsPresenter: the header,
 * the items panel and the footer, which scroll together. A ListView stacks its items in an
 * ItemsStackPanel, a GridView wraps them in an ItemsWrapGrid. What the containers look like is in items.c.
 */

#include "../internal.h"

#define IC(e) KST_AS(kst_items_control_t, e)
#define LVB(e) KST_AS(kst_list_view_base_t, e)


static int lvb_input(kst_element_t* e, kst_input_t* in) {
	switch (in->type) {
		case KST_IN_KEY_DOWN: return kst_ic_key(e, in);
		case KST_IN_FOCUS_IN:
		case KST_IN_FOCUS_OUT:
			kst_changed_paint(e);
			return 0;
		default: return 0;
	}
}

static void lvb_init(kst_element_t* e) {
	kst_items_control_t* o = IC(e);
	kst_element_t* scroll = kst_create(K_CLASS_SCROLL_VIEWER);
	kst_element_t* presenter = kst_create(K_CLASS_STACK_PANEL);
	kst_element_t* host = kst_create(K_CLASS_ITEMS_STACK_PANEL);
	/* ListView and GridView: IsTabStop False, IsSwipeEnabled True; a touch gives them the keyboard */
	e->focusable = 1;
	e->tab_stop = 0;
	LVB(e)->swipe_enabled = 1;
	if (!scroll || !presenter || !host) return;
	if (o->host) kst_destroy(o->host);
	kst_add_child(e, scroll, -1);
	kst_content_set(KST_AS(kst_content_control_t, scroll), presenter);
	kst_add_child(presenter, host, -1);
	o->scroll = scroll;
	o->presenter = presenter;
	o->host = host;
}

kst_class_t kst_class_list_view_base = {
	.base = &kst_class_selector, .name = "ListViewBase", .id = K_CLASS_LIST_VIEW_BASE, .size = sizeof(kst_list_view_base_t), .abstract_class = 1,
	.init = lvb_init, .input = lvb_input
};

static void grid_view_init(kst_element_t* e) {
	kst_element_t* grid = kst_create(K_CLASS_ITEMS_WRAP_GRID);
	if (grid) kst_ic_set_host(IC(e), grid);
}

kst_class_t kst_class_list_view = {
	.base = &kst_class_list_view_base, .name = "ListView", .id = K_CLASS_LIST_VIEW, .size = sizeof(kst_list_view_base_t)
};

kst_class_t kst_class_grid_view = {
	.base = &kst_class_list_view_base, .name = "GridView", .id = K_CLASS_GRID_VIEW, .size = sizeof(kst_list_view_base_t), .init = grid_view_init
};

/* The tiles of a GridView and the rows of a ListView are the containers made in items.c. */
kst_class_t kst_class_list_view_item = {
	.base = &kst_class_selector_item, .name = "ListViewItem", .id = K_CLASS_LIST_VIEW_ITEM, .size = sizeof(kst_selector_item_t)
};

kst_class_t kst_class_grid_view_item = {
	.base = &kst_class_selector_item, .name = "GridViewItem", .id = K_CLASS_GRID_VIEW_ITEM, .size = sizeof(kst_selector_item_t)
};

Kid Klist_view_create(void) {
	kst_element_t* e = kst_create(K_CLASS_LIST_VIEW);
	return e ? e->id : K_INVALID_ID;
}

Kid Kgrid_view_create(void) {
	kst_element_t* e = kst_create(K_CLASS_GRID_VIEW);
	return e ? e->id : K_INVALID_ID;
}

static kst_list_view_base_t* get_lvb(Kid id) { return LVB(kst_get(id, K_CLASS_LIST_VIEW_BASE)); }

void kst_lvb_item_tapped(kst_items_control_t* o, int index) {
	kst_list_view_base_t* l = (kst_list_view_base_t*)o;
	Kid id = o->base.base.id;
	kst_element_t* item = kst_ic_item(o, index);
	if (item) o->focus_item = item->id;
	if (o->sel_mode != KST_SEL_NONE) {
		kst_ic_tap(o, index);
		if (!kst_lookup(id)) return;
	}
	/* a tap selects as the mode says, then ItemClick comes (the order is not in the SDK) */
	if (l->item_click_enabled && l->on_item_click) l->on_item_click(id, index, l->on_item_click_data);
}

void Klist_view_set_selection_mode(Kid id, KListViewSelectionMode mode) {
	kst_list_view_base_t* l = get_lvb(id);
	kst_sel_mode_t m;
	if (!l) return;
	m = mode == K_LIST_VIEW_SELECTION_NONE ? KST_SEL_NONE : mode == K_LIST_VIEW_SELECTION_MULTIPLE ? KST_SEL_MULTIPLE :
		mode == K_LIST_VIEW_SELECTION_EXTENDED ? KST_SEL_EXTENDED : KST_SEL_SINGLE;
	kst_ic_set_sel_mode(&l->base, m);
}

KListViewSelectionMode Klist_view_get_selection_mode(Kid id) {
	kst_list_view_base_t* l = get_lvb(id);
	if (!l) return K_LIST_VIEW_SELECTION_SINGLE;
	switch (l->base.sel_mode) {
		case KST_SEL_NONE: return K_LIST_VIEW_SELECTION_NONE;
		case KST_SEL_MULTIPLE: return K_LIST_VIEW_SELECTION_MULTIPLE;
		case KST_SEL_EXTENDED: return K_LIST_VIEW_SELECTION_EXTENDED;
		default: return K_LIST_VIEW_SELECTION_SINGLE;
	}
}

void Klist_view_set_is_item_click_enabled(Kid id, int enabled) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) l->item_click_enabled = enabled != 0;
}

int Klist_view_get_is_item_click_enabled(Kid id) {
	kst_list_view_base_t* l = get_lvb(id);
	return l ? l->item_click_enabled : 0;
}

void Klist_view_on_item_click(Kid id, KItemClickCallback callback, void* user_data) {
	kst_list_view_base_t* l = get_lvb(id);
	if (!l) return;
	l->on_item_click = callback;
	l->on_item_click_data = user_data;
}

void Klist_view_set_is_swipe_enabled(Kid id, int enabled) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) l->swipe_enabled = enabled != 0;
}

int Klist_view_get_is_swipe_enabled(Kid id) {
	kst_list_view_base_t* l = get_lvb(id);
	return l ? l->swipe_enabled : 0;
}

int Klist_view_get_selected_indices(Kid id, int* indices, int max) {
	kst_list_view_base_t* l = get_lvb(id);
	return l ? kst_ic_selected_indices(&l->base, indices, max) : 0;
}

void Klist_view_select_all(Kid id) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) kst_ic_select_all(&l->base);
}

void Klist_view_scroll_into_view(Kid id, int index) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) kst_ic_scroll_into_view(&l->base, index, K_SCROLL_INTO_VIEW_DEFAULT);
}

void Klist_view_scroll_into_view_with_alignment(Kid id, int index, KScrollIntoViewAlignment alignment) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) kst_ic_scroll_into_view(&l->base, index, alignment);
}


/* ============================================================================
 *  Header and Footer
 * ============================================================================ */

/* The header or the footer is a ContentControl before or after the items panel in the presenter. */
static void set_part(kst_list_view_base_t* l, int footer, const char* text, kst_element_t* content) {
	Kid* slot = footer ? &l->footer : &l->header;
	kst_element_t* holder = *slot != K_INVALID_ID ? kst_lookup(*slot) : NULL;
	kst_items_control_t* o = &l->base;
	int empty = (text == NULL || !*text) && content == NULL;
	if (empty) {
		if (holder) kst_destroy(holder);
		*slot = K_INVALID_ID;
		if (content) kst_destroy(content);
		return;
	}
	if (!o->presenter) return;
	if (!holder) {
		kst_control_t* c;
		holder = kst_create(K_CLASS_CONTENT_CONTROL);
		if (!holder) return;
		c = KST_AS(kst_control_t, holder);
		c->font_size = KST_81_FONT_CONTENT;
		c->h_content = K_HALIGN_STRETCH;
		c->v_content = K_VALIGN_TOP;
		holder->focusable = 0;
		kst_add_child(o->presenter, holder, footer ? -1 : 0);
		if (holder->parent != o->presenter) return;
		*slot = holder->id;
	}
	if (text && *text) kst_content_set_text(KST_AS(kst_content_control_t, holder), text);
	else kst_content_set(KST_AS(kst_content_control_t, holder), content);
}

void Klist_view_set_header(Kid id, const char* header) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) set_part(l, 0, header, NULL);
}

void Klist_view_set_header_content(Kid id, Kid header) {
	kst_list_view_base_t* l = get_lvb(id);
	kst_element_t* c = header != K_INVALID_ID ? kst_get(header, K_CLASS_ELEMENT) : NULL;
	if (!l || (header != K_INVALID_ID && !c)) return;
	set_part(l, 0, NULL, c);
}

void Klist_view_set_footer(Kid id, const char* footer) {
	kst_list_view_base_t* l = get_lvb(id);
	if (l) set_part(l, 1, footer, NULL);
}

void Klist_view_set_footer_content(Kid id, Kid footer) {
	kst_list_view_base_t* l = get_lvb(id);
	kst_element_t* c = footer != K_INVALID_ID ? kst_get(footer, K_CLASS_ELEMENT) : NULL;
	if (!l || (footer != K_INVALID_ID && !c)) return;
	set_part(l, 1, NULL, c);
}


/* ============================================================================
 *  groups: ListViewHeaderItem and GridViewHeaderItem
 * ============================================================================ */

static void header_item_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	c->h_content = K_HALIGN_LEFT;
	c->v_content = K_VALIGN_TOP;
	c->font_size = 12.0f;
	e->focusable = 0;
}

kst_class_t kst_class_list_view_base_header_item = {
	.base = &kst_class_content_control, .name = "ListViewBaseHeaderItem", .id = K_CLASS_LIST_VIEW_BASE_HEADER_ITEM,
	.size = sizeof(kst_content_control_t), .abstract_class = 1, .init = header_item_init
};

kst_class_t kst_class_list_view_header_item = {
	.base = &kst_class_list_view_base_header_item, .name = "ListViewHeaderItem", .id = K_CLASS_LIST_VIEW_HEADER_ITEM, .size = sizeof(kst_content_control_t)
};

kst_class_t kst_class_grid_view_header_item = {
	.base = &kst_class_list_view_base_header_item, .name = "GridViewHeaderItem", .id = K_CLASS_GRID_VIEW_HEADER_ITEM, .size = sizeof(kst_content_control_t)
};

Kid Klist_view_add_group_header(Kid id, const char* title) {
	kst_list_view_base_t* l = get_lvb(id);
	kst_element_t* header, *text;
	if (!l || !l->base.host) return K_INVALID_ID;
	header = kst_create(kst_is_a(&l->base.base.base, K_CLASS_GRID_VIEW) ? K_CLASS_GRID_VIEW_HEADER_ITEM : K_CLASS_LIST_VIEW_HEADER_ITEM);
	if (!header) return K_INVALID_ID;
	/* what an app puts in a GroupStyle.HeaderTemplate: a text in the GroupHeader style */
	text = kst_create(K_CLASS_TEXT_BLOCK);
	if (text) {
		kst_set_string(&KST_AS(kst_text_block_t, text)->text, title ? title : "");
		Ktext_block_set_style(text->id, K_TEXT_STYLE_GROUP_HEADER);
		kst_content_set(KST_AS(kst_content_control_t, header), text);
	}
	kst_add_child(l->base.host, header, -1);
	if (header->parent != l->base.host) return K_INVALID_ID;
	l->base.group_count++;
	return header->id;
}

int Klist_view_get_group_count(Kid id) {
	kst_list_view_base_t* l = get_lvb(id);
	int i, n = 0;
	if (!l || !l->base.host) return 0;
	for (i = 0; i < l->base.host->child_count; i++) if (kst_is_a(l->base.host->children[i], K_CLASS_LIST_VIEW_BASE_HEADER_ITEM)) n++;
	return n;
}
