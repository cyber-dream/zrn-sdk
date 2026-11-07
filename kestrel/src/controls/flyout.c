/**
 * Kestrel - controls/flyout.c
 * FlyoutBase, Flyout, MenuFlyout with its items, and ContentDialog, after the Windows Phone 8.1
 * templates (generic.xaml of the phone SDK). All of them are shown in a Popup, which is what
 * puts them above the page and lets a tap outside close them.
 *
 * A flyout is not an element of the tree: it is made, given its content, and shown at an element
 * (ShowAt), or attached to a Button (Kbutton_set_flyout), which shows it when it is tapped. While it
 * is shown at an element it is hosted by the root, in a Canvas the root keeps for it.
 * On the phone a flyout is as wide as the screen and lies above or below its target.
 */

#include "../internal.h"

#define FB(e) KST_AS(kst_flyout_base_t, e)
#define ITEM(e) KST_AS(kst_menu_flyout_item_t, e)
#define DIALOG(e) KST_AS(kst_content_dialog_t, e)

/* FlyoutThemeMinHeight, FlyoutContentPanelPortraitThemeMargin 0,25.5,0,0 */
#define FLYOUT_MIN_HEIGHT 43.0f
#define FLYOUT_CONTENT_TOP 25.5f
/* MenuFlyoutPortraitBorderThemeThickness 0,1.5; MenuFlyoutPortraitThemePadding 19,13.5,0,26; MenuFlyoutItemThemePadding 0,8.5; TextStyleExtraLargeFontSize 25.5, SemiLight, CharacterSpacing -15 */
#define MENU_BORDER 1.5f
#define MENU_PAD_LEFT 19.0f
#define MENU_PAD_TOP 13.5f
#define MENU_PAD_BOTTOM 26.0f
#define MENU_ITEM_PAD 8.5f
#define MENU_FONT 25.5f
#define MENU_SPACING (-15)
/* ContentDialog: title margin 19,33.5,19,0 (min row 19), content margin 19,16.5,19,0 (min row 35), button rows min 45 with the
 * hosts padded 19,24,9.5,9.5 and 9.5,24,19,9.5; the title is 25.5 SemiBold and the content 18.14 */
#define DIALOG_TITLE_MIN 19.0f
#define DIALOG_CONTENT_MIN 35.0f
#define DIALOG_BUTTONS_MIN 45.0f
#define DIALOG_SIDE 19.0f
#define DIALOG_TITLE_TOP 33.5f
#define DIALOG_CONTENT_TOP 16.5f
#define DIALOG_BUTTON_TOP 24.0f
#define DIALOG_BUTTON_GAP 9.5f
#define DIALOG_BUTTON_BOTTOM 9.5f
#define DIALOG_TITLE_FONT 25.5f
#define DIALOG_CONTENT_FONT 18.14f


/* ============================================================================
 *  FlyoutBase
 * ============================================================================ */

static void flyout_popup_closed(Kid popup, void* user_data);

static void set_handler(kst_element_t* e, KEvent ev, KRoutedCallback cb, void* data) {
	e->handlers[ev - K_EVENT_CLICK].callback = cb;
	e->handlers[ev - K_EVENT_CLICK].user_data = data;
}

static kst_element_t* presenter_of(kst_flyout_base_t* f) {
	return f->popup && f->popup->child_count ? f->popup->children[0] : NULL;
}

/* The popup the flyout shows in, and the Border around the content (the presenter) in it. */
static void flyout_base_init(kst_element_t* e) {
	kst_flyout_base_t* f = FB(e);
	kst_element_t* popup = kst_create(K_CLASS_POPUP);
	kst_element_t* presenter = kst_create(K_CLASS_BORDER);
	f->placement = K_FLYOUT_PLACEMENT_TOP;
	if (!popup || !presenter) {
		if (popup) kst_destroy(popup);
		if (presenter) kst_destroy(presenter);
		return;
	}
	KST_AS(kst_popup_t, popup)->full_width = 1;
	set_handler(popup, K_EVENT_POPUP_CLOSED, flyout_popup_closed, (void*)(uintptr_t)e->id);
	kst_add_child(popup, presenter, -1);
	presenter->is_popup_child = 1;
	kst_add_child(e, popup, -1);
	f->popup = popup;
}

static void flyout_base_child_removed(kst_element_t* e, kst_element_t* child) {
	if (FB(e)->popup == child) FB(e)->popup = NULL;
}

kst_class_t kst_class_flyout_base = {
	.base = &kst_class_element, .name = "FlyoutBase", .id = K_CLASS_FLYOUT_BASE, .size = sizeof(kst_flyout_base_t), .abstract_class = 1,
	.init = flyout_base_init, .child_removed = flyout_base_child_removed
};

/* The popup is closed, by a tap outside, Back, Hide or an item: the flyout is no longer shown. */
static void flyout_popup_closed(Kid popup, void* user_data) {
	kst_element_t* e = kst_lookup((Kid)(uintptr_t)user_data);
	KST_UNUSED(popup);
	if (!e || !FB(e)->is_open) return;
	FB(e)->is_open = 0;
	if (FB(e)->on_hide) FB(e)->on_hide(e);
	/* a flyout shown at an element, not attached to a button, leaves the root's care */
	if (e->parent && e->root && e->parent == e->root->overlay) kst_remove_child(e->parent, e);
	kst_raise(e, K_EVENT_CLOSED);
}

void kst_flyout_show_at(kst_element_t* e, kst_element_t* target) {
	kst_flyout_base_t* f = FB(e);
	kst_popup_t* popup;
	kst_element_t* presenter;
	kst_root_t* r;
	Kid id = e->id, target_id = target->id;
	if (!f->popup || !(presenter = presenter_of(f))) return;
	r = target->root;
	if (!r) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "a flyout is shown at an element that is in a tree with a root");
		return;
	}
	if (f->is_open) {
		if (f->target == target_id) return;
		kst_popup_close(f->popup);
		e = kst_lookup(id);
		if (!e) return;
		f = FB(e);
	}
	if (f->on_show) f->on_show(e);
	if (!kst_raise(e, K_EVENT_OPENING)) return;
	target = kst_lookup(target_id);
	if (!target || !target->root || !FB(e)->popup) return;
	r = target->root;
	/* hosted by the root, unless a button holds it */
	if (!e->parent && r->overlay) kst_add_child(r->overlay, e, -1);
	if (!e->root) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "the flyout belongs to an element that is not in a tree with a root");
		return;
	}
	popup = KST_AS(kst_popup_t, f->popup);
	presenter = presenter_of(f);
	f->target = target_id;
	popup->target = target_id;
	popup->h_offset = popup->v_offset = 0;
	popup->full_width = 1;
	switch (f->placement) {
		case K_FLYOUT_PLACEMENT_TOP: popup->placement = K_PLACE_TOP; break;
		case K_FLYOUT_PLACEMENT_FULL: popup->target = K_INVALID_ID; break;
		default: popup->placement = K_PLACE_BOTTOM; break;
	}
	/* the look of the moment: the theme can change between two showings */
	{
		kst_border_t* b = KST_AS(kst_border_t, presenter);
		if (e->cls == &kst_class_menu_flyout) {
			b->background = kst_theme_color(e, KST_TC_BACKGROUND);
			b->border_brush = kst_theme_color(e, KST_TC_FOREGROUND);
		} else {
			b->background = kst_theme_color(e, KST_TC_CHROME);
			b->border_brush = kst_theme_color(e, KST_TC_CHROME);
		}
		b->has_background = 1;
		b->has_border_brush = 1;
		presenter->min_height = f->placement == K_FLYOUT_PLACEMENT_FULL ? (float)r->height : (e->cls == &kst_class_menu_flyout ? 0 : FLYOUT_MIN_HEIGHT);
	}
	kst_changed_layout(presenter);
	f->is_open = 1;
	kst_popup_set_open(f->popup, 1);
	kst_raise(e, K_EVENT_OPENED);
}

static kst_flyout_base_t* get_flyout(Kid id) { return FB(kst_get(id, K_CLASS_FLYOUT_BASE)); }

void Kflyout_base_set_placement(Kid id, KFlyoutPlacementMode placement) {
	kst_flyout_base_t* f = get_flyout(id);
	if (!f) return;
	if (placement < K_FLYOUT_PLACEMENT_TOP || placement > K_FLYOUT_PLACEMENT_FULL) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "unknown FlyoutPlacementMode 0x%x", (unsigned)placement);
		return;
	}
	f->placement = placement;
}

KFlyoutPlacementMode Kflyout_base_get_placement(Kid id) {
	kst_flyout_base_t* f = get_flyout(id);
	return f ? f->placement : K_FLYOUT_PLACEMENT_TOP;
}

void Kflyout_base_show_at(Kid id, Kid placement_target) {
	kst_flyout_base_t* f = get_flyout(id);
	kst_element_t* t = kst_get(placement_target, K_CLASS_ELEMENT);
	if (f && t) kst_flyout_show_at(&f->base, t);
}

void Kflyout_base_hide(Kid id) {
	kst_flyout_base_t* f = get_flyout(id);
	if (f && f->is_open && f->popup) kst_popup_close(f->popup);
}

int Kflyout_base_get_is_open(Kid id) {
	kst_flyout_base_t* f = get_flyout(id);
	return f ? f->is_open : 0;
}

void Kflyout_base_on_opening(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_flyout(id)) Kelement_on(id, K_EVENT_OPENING, callback, user_data);
}

void Kflyout_base_on_opened(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_flyout(id)) Kelement_on(id, K_EVENT_OPENED, callback, user_data);
}

void Kflyout_base_on_closed(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_flyout(id)) Kelement_on(id, K_EVENT_CLOSED, callback, user_data);
}

void Kbutton_set_flyout(Kid button, Kid flyout) {
	kst_element_t* b = kst_get(button, K_CLASS_BUTTON);
	kst_element_t* f = flyout != K_INVALID_ID ? kst_get(flyout, K_CLASS_FLYOUT_BASE) : NULL;
	kst_button_base_t* bb;
	if (!b || (flyout != K_INVALID_ID && !f)) return;
	bb = KST_AS(kst_button_base_t, b);
	if (f && f->parent) {
		kst_error(K_ERROR_HAS_PARENT, "the flyout is attached to another button already");
		return;
	}
	kst_set_only_child(b, &bb->flyout, f);
}

Kid Kbutton_get_flyout(Kid button) {
	kst_element_t* b = kst_get(button, K_CLASS_BUTTON);
	return b && KST_AS(kst_button_base_t, b)->flyout ? KST_AS(kst_button_base_t, b)->flyout->id : K_INVALID_ID;
}


/* ============================================================================
 *  Flyout
 * ============================================================================ */

static void flyout_init(kst_element_t* e) {
	kst_element_t* presenter = presenter_of(FB(e));
	/* FlyoutPresenter: the content starts 25.5 px under the top edge, and it is at least 43 px tall */
	if (presenter) KST_AS(kst_border_t, presenter)->padding = Kthickness(0, FLYOUT_CONTENT_TOP, 0, 0);
}

kst_class_t kst_class_flyout = {
	.base = &kst_class_flyout_base, .name = "Flyout", .id = K_CLASS_FLYOUT, .size = sizeof(kst_flyout_base_t), .init = flyout_init
};

Kid Kflyout_create(void) {
	kst_element_t* e = kst_create(K_CLASS_FLYOUT);
	return e ? e->id : K_INVALID_ID;
}

void Kflyout_set_content(Kid id, Kid content) {
	kst_element_t* e = kst_get(id, K_CLASS_FLYOUT);
	kst_element_t* c = content != K_INVALID_ID ? kst_get(content, K_CLASS_ELEMENT) : NULL;
	kst_element_t* presenter;
	kst_element_t* slot;
	if (!e || (content != K_INVALID_ID && !c)) return;
	presenter = presenter_of(FB(e));
	if (!presenter) return;
	slot = presenter->child_count ? presenter->children[0] : NULL;
	kst_set_only_child(presenter, &slot, c);
}

Kid Kflyout_get_content(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_FLYOUT);
	kst_element_t* presenter = e ? presenter_of(FB(e)) : NULL;
	return presenter && presenter->child_count ? presenter->children[0]->id : K_INVALID_ID;
}


/* ============================================================================
 *  MenuFlyoutItem, ToggleMenuFlyoutItem and MenuFlyoutSeparator
 * ============================================================================ */

static kst_font_t menu_font(void) {
	kst_font_t f = { K_FONT_SEMILIGHT, MENU_FONT, MENU_SPACING };
	return f;
}

static void item_destroy(kst_element_t* e) { free(ITEM(e)->text); }

static void item_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	c->padding = Kthickness(0, MENU_ITEM_PAD, 0, MENU_ITEM_PAD);
	c->font_family = K_FONT_SEMILIGHT;
	c->font_size = MENU_FONT;
	e->focusable = 0;
}

static KSize item_measure(kst_element_t* e, KSize available) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	const char* t = ITEM(e)->text ? ITEM(e)->text : "";
	KST_UNUSED(available);
	return kst_size(kst_text_width(menu_font(), t, (int)strlen(t)) + kst_thickness_h(c->padding), kst_font_line_height(menu_font()) + kst_thickness_v(c->padding));
}

static void item_paint(kst_element_t* e, kst_painter_t* p) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	const char* t = ITEM(e)->text ? ITEM(e)->text : "";
	/* MenuFlyoutItemDisabledForegroundThemeBrush is PhoneDisabled */
	KColor fg = kst_effectively_enabled(e) ? kst_ctl_foreground(c) : kst_theme_color(e, KST_TC_PHONE_DISABLED);
	kst_draw_string(p, menu_font(), t, (int)strlen(t), c->padding.left, c->padding.top, fg);
	if (e->cls == &kst_class_toggle_menu_flyout_item && ITEM(e)->is_checked) {
		/* the template has no mark for it: a check at the right edge, as high as a digit */
		float x = e->rect.width - 19.0f - 16.0f, y = e->rect.height * 0.5f;
		kst_draw_line(p, x, y, x + 5.5f, y + 5.5f, 2.5f, fg);
		kst_draw_line(p, x + 5.5f, y + 5.5f, x + 16.0f, y - 6.0f, 2.5f, fg);
	}
}

/* A tap runs Click, and the flyout the item is in closes. */
static int item_input(kst_element_t* e, kst_input_t* in) {
	kst_menu_flyout_item_t* m = ITEM(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			m->pressed = 1;
			if (e->root) kst_set_capture(e->root, e);
			return 1;
		case KST_IN_POINTER_UP: {
			int inside = in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			Kid id = e->id;
			kst_element_t* p;
			if (!m->pressed) return 0;
			m->pressed = 0;
			if (e->root) kst_release_capture(e->root, e);
			if (!inside) return 1;
			if (e->cls == &kst_class_toggle_menu_flyout_item) {
				m->is_checked = !m->is_checked;
				kst_changed_paint(e);
			}
			if (!kst_raise(e, K_EVENT_CLICK)) return 1;
			e = kst_lookup(id);
			for (p = e->parent; p; p = p->parent) {
				if (p->cls == &kst_class_popup) {
					kst_popup_close(p);
					break;
				}
			}
			return 1;
		}
		case KST_IN_CAPTURE_LOST:
			m->pressed = 0;
			return 0;
		default:
			return 0;
	}
}

kst_class_t kst_class_menu_flyout_item_base = {
	.base = &kst_class_control, .name = "MenuFlyoutItemBase", .id = K_CLASS_MENU_FLYOUT_ITEM_BASE, .size = sizeof(kst_menu_flyout_item_t), .abstract_class = 1,
	.init = item_init, .destroy = item_destroy
};

kst_class_t kst_class_menu_flyout_item = {
	.base = &kst_class_menu_flyout_item_base, .name = "MenuFlyoutItem", .id = K_CLASS_MENU_FLYOUT_ITEM, .size = sizeof(kst_menu_flyout_item_t),
	.measure = item_measure, .paint = item_paint, .input = item_input
};

kst_class_t kst_class_toggle_menu_flyout_item = {
	.base = &kst_class_menu_flyout_item, .name = "ToggleMenuFlyoutItem", .id = K_CLASS_TOGGLE_MENU_FLYOUT_ITEM, .size = sizeof(kst_menu_flyout_item_t)
};

/* The template of the separator is not in the phone XAML: a hairline in a row as high as the other rows' padding. */
static KSize separator_measure(kst_element_t* e, KSize available) {
	KST_UNUSED(e); KST_UNUSED(available);
	return kst_size(0, 2 * MENU_ITEM_PAD);
}

static void separator_paint(kst_element_t* e, kst_painter_t* p) {
	kst_fill_rect(p, kst_rect(0, e->rect.height * 0.5f - 0.5f, e->rect.width, 1.0f), kst_theme_color(e, KST_TC_BASE_MEDIUM_MID));
}

kst_class_t kst_class_menu_flyout_separator = {
	.base = &kst_class_menu_flyout_item_base, .name = "MenuFlyoutSeparator", .id = K_CLASS_MENU_FLYOUT_SEPARATOR, .size = sizeof(kst_menu_flyout_item_t),
	.measure = separator_measure, .paint = separator_paint
};

static Kid make_menu_item(KClass cls, const char* text) {
	kst_element_t* e = kst_create(cls);
	if (!e) return K_INVALID_ID;
	kst_set_string(&ITEM(e)->text, text ? text : "");
	return e->id;
}

Kid Kmenu_flyout_item_create(const char* text) { return make_menu_item(K_CLASS_MENU_FLYOUT_ITEM, text); }

Kid Ktoggle_menu_flyout_item_create(const char* text, int is_checked) {
	Kid id = make_menu_item(K_CLASS_TOGGLE_MENU_FLYOUT_ITEM, text);
	if (id != K_INVALID_ID) ITEM(kst_lookup(id))->is_checked = is_checked != 0;
	return id;
}

Kid Kmenu_flyout_separator_create(void) { return make_menu_item(K_CLASS_MENU_FLYOUT_SEPARATOR, NULL); }

void Kmenu_flyout_item_set_text(Kid id, const char* text) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT_ITEM);
	if (!e) return;
	kst_set_string(&ITEM(e)->text, text ? text : "");
	kst_changed_layout(e);
}

const char* Kmenu_flyout_item_get_text(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT_ITEM);
	return e && ITEM(e)->text ? ITEM(e)->text : "";
}

void Kmenu_flyout_item_on_click(Kid id, KRoutedCallback callback, void* user_data) {
	if (kst_get(id, K_CLASS_MENU_FLYOUT_ITEM)) Kelement_on(id, K_EVENT_CLICK, callback, user_data);
}

void Ktoggle_menu_flyout_item_set_is_checked(Kid id, int is_checked) {
	kst_element_t* e = kst_get(id, K_CLASS_TOGGLE_MENU_FLYOUT_ITEM);
	if (!e) return;
	ITEM(e)->is_checked = is_checked != 0;
	kst_changed_paint(e);
}

int Ktoggle_menu_flyout_item_get_is_checked(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_TOGGLE_MENU_FLYOUT_ITEM);
	return e ? ITEM(e)->is_checked : 0;
}


/* ============================================================================
 *  MenuFlyout
 * ============================================================================ */

static kst_element_t* menu_stack(kst_flyout_base_t* f) {
	kst_element_t* presenter = presenter_of(f);
	return presenter && presenter->child_count ? presenter->children[0] : NULL;
}

static void menu_flyout_init(kst_element_t* e) {
	kst_element_t* presenter = presenter_of(FB(e));
	kst_element_t* stack = kst_create(K_CLASS_STACK_PANEL);
	if (!presenter || !stack) {
		if (stack) kst_destroy(stack);
		return;
	}
	/* MenuFlyoutPresenter: a 1.5 px line above and below, the items padded 19,13.5,0,26 */
	KST_AS(kst_border_t, presenter)->border_thickness = Kthickness(0, MENU_BORDER, 0, MENU_BORDER);
	KST_AS(kst_border_t, presenter)->padding = Kthickness(MENU_PAD_LEFT, MENU_PAD_TOP, 0, MENU_PAD_BOTTOM);
	kst_add_child(presenter, stack, -1);
}

kst_class_t kst_class_menu_flyout = {
	.base = &kst_class_flyout_base, .name = "MenuFlyout", .id = K_CLASS_MENU_FLYOUT, .size = sizeof(kst_flyout_base_t), .init = menu_flyout_init
};

Kid Kmenu_flyout_create(void) {
	kst_element_t* e = kst_create(K_CLASS_MENU_FLYOUT);
	return e ? e->id : K_INVALID_ID;
}

int Kmenu_flyout_add_item(Kid id, Kid item) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT);
	kst_element_t* i = item != K_INVALID_ID ? kst_get(item, K_CLASS_MENU_FLYOUT_ITEM_BASE) : NULL;
	kst_element_t* stack;
	if (!e || !i) return -1;
	stack = menu_stack(FB(e));
	if (!stack || i->parent) {
		if (i->parent) kst_error(K_ERROR_HAS_PARENT, "the item already has a parent");
		return -1;
	}
	kst_add_child(stack, i, -1);
	return i->parent == stack ? stack->child_count - 1 : -1;
}

int Kmenu_flyout_get_item_count(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT);
	kst_element_t* stack = e ? menu_stack(FB(e)) : NULL;
	return stack ? stack->child_count : 0;
}

Kid Kmenu_flyout_get_item(Kid id, int index) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT);
	kst_element_t* stack = e ? menu_stack(FB(e)) : NULL;
	return stack && index >= 0 && index < stack->child_count ? stack->children[index]->id : K_INVALID_ID;
}

void Kmenu_flyout_clear_items(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_MENU_FLYOUT);
	kst_element_t* stack = e ? menu_stack(FB(e)) : NULL;
	if (stack) kst_clear_children(stack);
}


/* ============================================================================
 *  ContentDialog
 *  A modal Popup over the whole screen: what is under it is dimmed and does not get the input,
 *  and Back closes it as Hide does. The dialog lies at the top, as wide as the screen.
 * ============================================================================ */

static void dialog_button_click(Kid sender, void* user_data);
static void dialog_popup_closed(Kid popup, void* user_data);

static kst_element_t* make_dialog_button(kst_element_t* dialog, int primary) {
	kst_element_t* b = kst_create(K_CLASS_BUTTON);
	if (!b) return NULL;
	b->halign = K_HALIGN_STRETCH;
	b->valign = K_VALIGN_TOP;
	set_handler(b, K_EVENT_CLICK, dialog_button_click, (void*)(uintptr_t)dialog->id);
	b->tag = (void*)(uintptr_t)(primary ? 1 : 2);
	kst_add_child(dialog, b, -1);
	return b;
}

static void dialog_init(kst_element_t* e) {
	kst_content_dialog_t* d = DIALOG(e);
	kst_control_t* c = &d->base.base;
	kst_text_block_t* t;
	e->focusable = 0;
	c->font_size = DIALOG_CONTENT_FONT;
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
	e->halign = K_HALIGN_STRETCH;
	e->valign = K_VALIGN_TOP;
	d->primary_enabled = d->secondary_enabled = 1;
	d->title = kst_create(K_CLASS_TEXT_BLOCK);
	if (d->title) {
		t = KST_AS(kst_text_block_t, d->title);
		t->font_size = DIALOG_TITLE_FONT;
		t->font_family = K_FONT_SEMIBOLD;
		t->set_flags |= KST_TB_FONT_SIZE | KST_TB_FONT_FAMILY;
		t->wrapping = K_WRAP;
		kst_add_child(e, d->title, -1);
	}
	d->button1 = make_dialog_button(e, 1);
	d->button2 = make_dialog_button(e, 0);
	if (d->button1) d->button1->visibility = K_COLLAPSED;
	if (d->button2) d->button2->visibility = K_COLLAPSED;
}

static void dialog_destroy(kst_element_t* e) {
	kst_content_dialog_t* d = DIALOG(e);
	kst_element_t* popup = d->popup != K_INVALID_ID ? kst_lookup(d->popup) : NULL;
	/* a dialog that is destroyed while it shows takes its popup with it, unless the popup is what is destroying it */
	if (popup && !popup->destroying) kst_destroy(popup);
	d->popup = K_INVALID_ID;
	free(d->primary_text);
	free(d->secondary_text);
}

static void dialog_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_content_dialog_t* d = DIALOG(e);
	if (d->title == child) d->title = NULL;
	else if (d->button1 == child) d->button1 = NULL;
	else if (d->button2 == child) d->button2 = NULL;
	else kst_cc_child_removed(e, child);
}

static int visible(const kst_element_t* e) { return e && e->visibility == K_VISIBLE; }

static float title_height(kst_content_dialog_t* d) {
	return d->title && visible(d->title) ? d->title->desired.height : 0;
}

static KSize dialog_measure(kst_element_t* e, KSize available) {
	kst_content_dialog_t* d = DIALOG(e);
	float w = available.width < K_INFINITE * 0.5f ? available.width : 480.0f;
	float inner = kst_maxf(0, w - 2 * DIALOG_SIDE);
	float col = w * 0.5f;
	float title_row, content_row, buttons_row = DIALOG_BUTTONS_MIN, ch = 0;
	if (d->title) {
		const char* s = Ktext_block_get_text(d->title->id);
		d->title->visibility = s && *s ? K_VISIBLE : K_COLLAPSED;
		kst_measure(d->title, kst_size(inner, K_INFINITE));
	}
	title_row = kst_maxf(DIALOG_TITLE_MIN, DIALOG_TITLE_TOP + title_height(d));
	if (d->base.content) {
		kst_measure(d->base.content, kst_size(inner, K_INFINITE));
		ch = d->base.content->desired.height;
	}
	content_row = kst_maxf(DIALOG_CONTENT_MIN, DIALOG_CONTENT_TOP + ch);
	if (d->button1) kst_measure(d->button1, kst_size(kst_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), K_INFINITE));
	if (d->button2) kst_measure(d->button2, kst_size(kst_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), K_INFINITE));
	if (visible(d->button1) || visible(d->button2)) {
		float bh = kst_maxf(visible(d->button1) ? d->button1->desired.height : 0, visible(d->button2) ? d->button2->desired.height : 0);
		buttons_row = kst_maxf(DIALOG_BUTTONS_MIN, DIALOG_BUTTON_TOP + bh + DIALOG_BUTTON_BOTTOM);
	}
	return kst_size(w, title_row + content_row + buttons_row);
}

static KSize dialog_arrange(kst_element_t* e, KSize final) {
	kst_content_dialog_t* d = DIALOG(e);
	float inner = kst_maxf(0, final.width - 2 * DIALOG_SIDE), col = final.width * 0.5f;
	float title_row = kst_maxf(DIALOG_TITLE_MIN, DIALOG_TITLE_TOP + title_height(d));
	float ch = d->base.content ? d->base.content->desired.height : 0;
	float content_row = kst_maxf(DIALOG_CONTENT_MIN, DIALOG_CONTENT_TOP + ch);
	float y = title_row + content_row;
	if (d->title) kst_arrange(d->title, kst_rect(DIALOG_SIDE, DIALOG_TITLE_TOP, inner, title_height(d)));
	if (d->base.content) kst_arrange(d->base.content, kst_rect(DIALOG_SIDE, title_row + DIALOG_CONTENT_TOP, inner, ch));
	if (d->button1) kst_arrange(d->button1, kst_rect(DIALOG_SIDE, y + DIALOG_BUTTON_TOP, kst_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), d->button1->desired.height));
	if (d->button2) kst_arrange(d->button2, kst_rect(col + DIALOG_BUTTON_GAP, y + DIALOG_BUTTON_TOP, kst_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), d->button2->desired.height));
	return final;
}

static void dialog_paint(kst_element_t* e, kst_painter_t* p) {
	/* ContentDialogBackgroundThemeBrush is the chrome */
	kst_fill_rect(p, kst_local_rect(e), kst_ctl_background(&DIALOG(e)->base.base, kst_theme_color(e, KST_TC_CHROME)));
}

kst_class_t kst_class_content_dialog = {
	.base = &kst_class_content_control, .name = "ContentDialog", .id = K_CLASS_CONTENT_DIALOG, .size = sizeof(kst_content_dialog_t),
	.init = dialog_init, .destroy = dialog_destroy, .child_removed = dialog_child_removed, .measure = dialog_measure,
	.arrange = dialog_arrange, .paint = dialog_paint
};

Kid Kcontent_dialog_create(void) {
	kst_element_t* e = kst_create(K_CLASS_CONTENT_DIALOG);
	return e ? e->id : K_INVALID_ID;
}

static kst_content_dialog_t* get_dialog(Kid id) { return DIALOG(kst_get(id, K_CLASS_CONTENT_DIALOG)); }

void Kcontent_dialog_set_title(Kid id, const char* title) {
	kst_content_dialog_t* d = get_dialog(id);
	if (d && d->title) {
		Ktext_block_set_text(d->title->id, title ? title : "");
		kst_changed_layout(&d->base.base.base);
	}
}

const char* Kcontent_dialog_get_title(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	return d && d->title ? Ktext_block_get_text(d->title->id) : "";
}

static void dialog_set_button_text(Kid id, int primary, const char* text) {
	kst_content_dialog_t* d = get_dialog(id);
	kst_element_t* b;
	if (!d) return;
	b = primary ? d->button1 : d->button2;
	kst_set_string(primary ? &d->primary_text : &d->secondary_text, text && *text ? text : NULL);
	if (!b) return;
	/* a button with no text is not there */
	b->visibility = text && *text ? K_VISIBLE : K_COLLAPSED;
	Kcontent_control_set_text(b->id, text ? text : "");
	kst_changed_layout(&d->base.base.base);
}

void Kcontent_dialog_set_primary_button_text(Kid id, const char* text) { dialog_set_button_text(id, 1, text); }
void Kcontent_dialog_set_secondary_button_text(Kid id, const char* text) { dialog_set_button_text(id, 0, text); }

const char* Kcontent_dialog_get_primary_button_text(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	return d && d->primary_text ? d->primary_text : "";
}

const char* Kcontent_dialog_get_secondary_button_text(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	return d && d->secondary_text ? d->secondary_text : "";
}

void Kcontent_dialog_set_is_primary_button_enabled(Kid id, int enabled) {
	kst_content_dialog_t* d = get_dialog(id);
	if (!d) return;
	d->primary_enabled = enabled != 0;
	if (d->button1) Kelement_set_enabled(d->button1->id, enabled);
}

void Kcontent_dialog_set_is_secondary_button_enabled(Kid id, int enabled) {
	kst_content_dialog_t* d = get_dialog(id);
	if (!d) return;
	d->secondary_enabled = enabled != 0;
	if (d->button2) Kelement_set_enabled(d->button2->id, enabled);
}

int Kcontent_dialog_get_is_primary_button_enabled(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	return d ? d->primary_enabled : 0;
}

int Kcontent_dialog_get_is_secondary_button_enabled(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	return d ? d->secondary_enabled : 0;
}

void Kcontent_dialog_on_primary_button_click(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_dialog(id)) Kelement_on(id, K_EVENT_PRIMARY_BUTTON_CLICK, callback, user_data);
}

void Kcontent_dialog_on_secondary_button_click(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_dialog(id)) Kelement_on(id, K_EVENT_SECONDARY_BUTTON_CLICK, callback, user_data);
}

void Kcontent_dialog_on_opened(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_dialog(id)) Kelement_on(id, K_EVENT_OPENED, callback, user_data);
}

void Kcontent_dialog_on_closed(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_dialog(id)) Kelement_on(id, K_EVENT_CLOSED, callback, user_data);
}

/* The dialog is over: the popup goes, the focus returns where it was, Closed and then the callback tell. */
static void dialog_finish(kst_element_t* e, KContentDialogResult result) {
	kst_content_dialog_t* d = DIALOG(e);
	kst_root_t* r = e->root;
	kst_element_t* popup = d->popup != K_INVALID_ID ? kst_lookup(d->popup) : NULL;
	kst_element_t* previous = d->previous_focus != K_INVALID_ID ? kst_lookup(d->previous_focus) : NULL;
	KContentDialogResultCallback callback = d->callback;
	void* data = d->callback_data;
	Kid id = e->id;
	if (!d->is_open) return;
	d->is_open = 0;
	d->result = result;
	d->callback = NULL;
	d->callback_data = NULL;
	d->previous_focus = K_INVALID_ID;
	d->popup = K_INVALID_ID;
	if (popup) {
		kst_popup_set_open(popup, 0);
		/* the dialog goes back to the program, the popup is only the means of showing it */
		kst_remove_child(popup, e);
		kst_destroy(popup);
	}
	if (r && r->focus == NULL && previous && previous->root == r) kst_set_focus(r, previous);
	if (!kst_raise(kst_lookup(id), K_EVENT_CLOSED)) return;
	if (callback) callback(id, result, data);
}

static void dialog_button_click(Kid sender, void* user_data) {
	kst_element_t* e = kst_lookup((Kid)(uintptr_t)user_data);
	kst_element_t* b = kst_lookup(sender);
	int primary;
	Kid id;
	if (!e || !b || !DIALOG(e)->is_open) return;
	id = e->id;
	primary = (uintptr_t)b->tag == 1;
	if (!kst_raise(e, primary ? K_EVENT_PRIMARY_BUTTON_CLICK : K_EVENT_SECONDARY_BUTTON_CLICK)) return;
	dialog_finish(kst_lookup(id), primary ? K_CONTENT_DIALOG_RESULT_PRIMARY : K_CONTENT_DIALOG_RESULT_SECONDARY);
}

/* Back, or anything that closes the popup but a button: the result is None. */
static void dialog_popup_closed(Kid popup, void* user_data) {
	kst_element_t* e = kst_lookup((Kid)(uintptr_t)user_data);
	KST_UNUSED(popup);
	if (e && DIALOG(e)->is_open) dialog_finish(e, K_CONTENT_DIALOG_RESULT_NONE);
}

int Kcontent_dialog_show(Kid id, Kid root, KContentDialogResultCallback callback, void* user_data) {
	kst_content_dialog_t* d = get_dialog(id);
	kst_root_t* r = kst_root_lookup(root);
	kst_element_t *e, *popup, *focus;
	if (!d) return K_FALSE;
	e = &d->base.base.base;
	if (!r) {
		kst_error(K_ERROR_INVALID_ID, "invalid root handle 0x%x", (unsigned)root);
		return K_FALSE;
	}
	if (d->is_open) return K_FALSE;
	if (e->parent) {
		kst_error(K_ERROR_HAS_PARENT, "a ContentDialog is shown with Kcontent_dialog_show, it is not put in a tree");
		return K_FALSE;
	}
	popup = kst_create(K_CLASS_POPUP);
	if (!popup || !r->overlay) {
		if (popup) kst_destroy(popup);
		return K_FALSE;
	}
	KST_AS(kst_popup_t, popup)->modal = 1;
	KST_AS(kst_popup_t, popup)->stays_open = 1;
	KST_AS(kst_popup_t, popup)->full_width = 1;
	set_handler(popup, K_EVENT_POPUP_CLOSED, dialog_popup_closed, (void*)(uintptr_t)e->id);
	kst_add_child(popup, e, -1);
	e->is_popup_child = 1;
	kst_add_child(r->overlay, popup, -1);
	d->popup = popup->id;
	d->callback = callback;
	d->callback_data = user_data;
	d->is_open = 1;
	d->previous_focus = r->focus ? r->focus->id : K_INVALID_ID;
	kst_popup_set_open(popup, 1);
	/* the keyboard goes into the dialog: the button that is the answer, or the first one there is */
	focus = visible(d->button1) && d->primary_enabled ? d->button1 : (visible(d->button2) && d->secondary_enabled ? d->button2 : NULL);
	kst_set_focus(r, focus);
	if (!kst_raise(e, K_EVENT_OPENED)) return K_TRUE;
	return K_TRUE;
}

void Kcontent_dialog_hide(Kid id) {
	kst_content_dialog_t* d = get_dialog(id);
	if (d && d->is_open) dialog_finish(&d->base.base.base, K_CONTENT_DIALOG_RESULT_NONE);
}
