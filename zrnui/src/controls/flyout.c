/**
 * zrnui - controls/flyout.c
 * FlyoutBase, Flyout, MenuFlyout with its items, and ContentDialog, after the Windows Phone 8.1
 * templates (generic.xaml of the phone SDK). All of them are shown in a Popup, which is what
 * puts them above the page and lets a tap outside close them.
 *
 * A flyout is not an element of the tree: it is made, given its content, and shown at an element
 * (ShowAt), or attached to a Button (Zbutton_set_flyout), which shows it when it is tapped. While it
 * is shown at an element it is hosted by the root, in a Canvas the root keeps for it.
 * On the phone a flyout is as wide as the screen and lies above or below its target.
 */

#include "../internal.h"

#define FB(e) ZST_AS(zrn_flyout_base_t, e)
#define ITEM(e) ZST_AS(zrn_menu_flyout_item_t, e)
#define DIALOG(e) ZST_AS(zrn_content_dialog_t, e)

/* FlyoutThemeMinHeight, FlyoutContentPanelPortraitThemeMargin 0,25.5,0,0 */
static constexpr float FLYOUT_MIN_HEIGHT = 43.0f;
static constexpr float FLYOUT_CONTENT_TOP = 25.5f;
/* MenuFlyoutPortraitBorderThemeThickness 0,1.5; MenuFlyoutPortraitThemePadding 19,13.5,0,26; MenuFlyoutItemThemePadding 0,8.5; TextStyleExtraLargeFontSize 25.5, SemiLight, CharacterSpacing -15 */
static constexpr float MENU_BORDER = 1.5f;
static constexpr float MENU_PAD_LEFT = 19.0f;
static constexpr float MENU_PAD_TOP = 13.5f;
static constexpr float MENU_PAD_BOTTOM = 26.0f;
static constexpr float MENU_ITEM_PAD = 8.5f;
static constexpr float MENU_FONT = 25.5f;
static constexpr int MENU_SPACING = (-15);
/* ContentDialog: title margin 19,33.5,19,0 (min row 19), content margin 19,16.5,19,0 (min row 35), button rows min 45 with the
 * hosts padded 19,24,9.5,9.5 and 9.5,24,19,9.5; the title is 25.5 SemiBold and the content 18.14 */
static constexpr float DIALOG_TITLE_MIN = 19.0f;
static constexpr float DIALOG_CONTENT_MIN = 35.0f;
static constexpr float DIALOG_BUTTONS_MIN = 45.0f;
static constexpr float DIALOG_SIDE = 19.0f;
static constexpr float DIALOG_TITLE_TOP = 33.5f;
static constexpr float DIALOG_CONTENT_TOP = 16.5f;
static constexpr float DIALOG_BUTTON_TOP = 24.0f;
static constexpr float DIALOG_BUTTON_GAP = 9.5f;
static constexpr float DIALOG_BUTTON_BOTTOM = 9.5f;
static constexpr float DIALOG_TITLE_FONT = 25.5f;
static constexpr float DIALOG_CONTENT_FONT = 18.14f;


/* ============================================================================
 *  FlyoutBase
 * ============================================================================ */

static void flyout_popup_closed(Zid popup, void* user_data);

static void set_handler(zrn_element_t* const e, const ZEvent ev, const ZRoutedCallback cb, void* const data) {
	e->handlers[ev - Z_EVENT_CLICK].callback = cb;
	e->handlers[ev - Z_EVENT_CLICK].user_data = data;
}

static zrn_element_t* presenter_of(const zrn_flyout_base_t* const f) {
	return f->popup && f->popup->child_count ? f->popup->children[0] : nullptr;
}

/* The popup the flyout shows in, and the Border around the content (the presenter) in it. */
static void flyout_base_init(zrn_element_t* const e) {
	const auto f = FB(e);
	zrn_element_t* const popup = zrn_create(Z_CLASS_POPUP);
	zrn_element_t* const presenter = zrn_create(Z_CLASS_BORDER);
	f->placement = Z_FLYOUT_PLACEMENT_TOP;
	if (!popup || !presenter) {
		if (popup) zrn_destroy(popup);
		if (presenter) zrn_destroy(presenter);
		return;
	}
	ZST_AS(zrn_popup_t, popup)->full_width = 1;
	set_handler(popup, Z_EVENT_POPUP_CLOSED, flyout_popup_closed, (void*)(uintptr_t)e->id);
	zrn_add_child(popup, presenter, -1);
	presenter->is_popup_child = 1;
	zrn_add_child(e, popup, -1);
	f->popup = popup;
}

static void flyout_base_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	if (FB(e)->popup == child) FB(e)->popup = nullptr;
}

zrn_class_t zrn_class_flyout_base = {
	.base = &zrn_class_element, .name = "FlyoutBase", .id = Z_CLASS_FLYOUT_BASE, .size = sizeof(zrn_flyout_base_t), .abstract_class = 1,
	.init = flyout_base_init, .child_removed = flyout_base_child_removed
};

/* The popup is closed, by a tap outside, Back, Hide or an item: the flyout is no longer shown. */
static void flyout_popup_closed(const Zid popup, void* const user_data) {
	zrn_element_t* const e = zrn_lookup((Zid)(uintptr_t)user_data);
	ZST_UNUSED(popup);
	if (!e || !FB(e)->is_open) return;
	FB(e)->is_open = 0;
	if (FB(e)->on_hide) FB(e)->on_hide(e);
	/* a flyout shown at an element, not attached to a button, leaves the root's care */
	if (e->parent && e->root && e->parent == e->root->overlay) zrn_remove_child(e->parent, e);
	zrn_raise(e, Z_EVENT_CLOSED);
}

void zrn_flyout_show_at(zrn_element_t* flyout, zrn_element_t* target) {
	auto f = FB(flyout);
	zrn_popup_t* popup;
	zrn_element_t* presenter;
	zrn_root_t* r;
	const Zid id = flyout->id, target_id = target->id;
	if (!f->popup || !(presenter = presenter_of(f))) return;
	r = target->root;
	if (!r) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "a flyout is shown at an element that is in a tree with a root");
		return;
	}
	if (f->is_open) {
		if (f->target == target_id) return;
		zrn_popup_close(f->popup);
		flyout = zrn_lookup(id);
		if (!flyout) return;
		f = FB(flyout);
	}
	if (f->on_show) f->on_show(flyout);
	if (!zrn_raise(flyout, Z_EVENT_OPENING)) return;
	target = zrn_lookup(target_id);
	if (!target || !target->root || !FB(flyout)->popup) return;
	r = target->root;
	/* hosted by the root, unless a button holds it */
	if (!flyout->parent && r->overlay) zrn_add_child(r->overlay, flyout, -1);
	if (!flyout->root) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "the flyout belongs to an element that is not in a tree with a root");
		return;
	}
	popup = ZST_AS(zrn_popup_t, f->popup);
	presenter = presenter_of(f);
	f->target = target_id;
	popup->target = target_id;
	popup->h_offset = popup->v_offset = 0;
	popup->full_width = 1;
	switch (f->placement) {
		case Z_FLYOUT_PLACEMENT_TOP: popup->placement = Z_PLACE_TOP; break;
		case Z_FLYOUT_PLACEMENT_FULL: popup->target = Z_INVALID_ID; break;
		default: popup->placement = Z_PLACE_BOTTOM; break;
	}
	/* the look of the moment: the theme can change between two showings */
	{
		const auto b = ZST_AS(zrn_border_t, presenter);
		if (flyout->cls == &zrn_class_menu_flyout) {
			b->background = zrn_theme_color(flyout, ZST_TC_BACKGROUND);
			b->border_brush = zrn_theme_color(flyout, ZST_TC_FOREGROUND);
		} else {
			b->background = zrn_theme_color(flyout, ZST_TC_CHROME);
			b->border_brush = zrn_theme_color(flyout, ZST_TC_CHROME);
		}
		b->has_background = 1;
		b->has_border_brush = 1;
		presenter->min_height = f->placement == Z_FLYOUT_PLACEMENT_FULL ? (float)r->height / r->scale : (flyout->cls == &zrn_class_menu_flyout ? 0 : FLYOUT_MIN_HEIGHT);
	}
	zrn_changed_layout(presenter);
	f->is_open = 1;
	zrn_popup_set_open(f->popup, 1);
	zrn_raise(flyout, Z_EVENT_OPENED);
}

static zrn_flyout_base_t* get_flyout(const Zid id) { return FB(zrn_get(id, Z_CLASS_FLYOUT_BASE)); }

void Zflyout_base_set_placement(const Zid flyout, const ZFlyoutPlacementMode placement) {
	zrn_flyout_base_t* const f = get_flyout(flyout);
	if (!f) return;
	if (placement < Z_FLYOUT_PLACEMENT_TOP || placement > Z_FLYOUT_PLACEMENT_FULL) {
		zrn_error(Z_ERROR_INVALID_ARGUMENT, "unknown FlyoutPlacementMode 0x%x", (unsigned)placement);
		return;
	}
	f->placement = placement;
}

ZFlyoutPlacementMode Zflyout_base_get_placement(const Zid flyout) {
	const zrn_flyout_base_t* const f = get_flyout(flyout);
	return f ? f->placement : Z_FLYOUT_PLACEMENT_TOP;
}

void Zflyout_base_show_at(const Zid flyout, const Zid placement_target) {
	zrn_flyout_base_t* const f = get_flyout(flyout);
	zrn_element_t* const t = zrn_get(placement_target, Z_CLASS_ELEMENT);
	if (f && t) zrn_flyout_show_at(&f->base, t);
}

void Zflyout_base_hide(const Zid flyout) {
	const zrn_flyout_base_t* const f = get_flyout(flyout);
	if (f && f->is_open && f->popup) zrn_popup_close(f->popup);
}

int Zflyout_base_get_is_open(const Zid flyout) {
	const zrn_flyout_base_t* const f = get_flyout(flyout);
	return f ? f->is_open : 0;
}

void Zflyout_base_on_opening(const Zid flyout, const ZRoutedCallback callback, void* const user_data) {
	if (get_flyout(flyout)) Zelement_on(flyout, Z_EVENT_OPENING, callback, user_data);
}

void Zflyout_base_on_opened(const Zid flyout, const ZRoutedCallback callback, void* const user_data) {
	if (get_flyout(flyout)) Zelement_on(flyout, Z_EVENT_OPENED, callback, user_data);
}

void Zflyout_base_on_closed(const Zid flyout, const ZRoutedCallback callback, void* const user_data) {
	if (get_flyout(flyout)) Zelement_on(flyout, Z_EVENT_CLOSED, callback, user_data);
}

void Zbutton_set_flyout(const Zid button, const Zid flyout) {
	zrn_element_t* const b = zrn_get(button, Z_CLASS_BUTTON);
	zrn_element_t* const f = flyout != Z_INVALID_ID ? zrn_get(flyout, Z_CLASS_FLYOUT_BASE) : nullptr;
	zrn_button_base_t* bb;
	if (!b || (flyout != Z_INVALID_ID && !f)) return;
	bb = ZST_AS(zrn_button_base_t, b);
	if (f && f->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "the flyout is attached to another button already");
		return;
	}
	zrn_set_only_child(b, &bb->flyout, f);
}

Zid Zbutton_get_flyout(const Zid button) {
	zrn_element_t* const b = zrn_get(button, Z_CLASS_BUTTON);
	return b && ZST_AS(zrn_button_base_t, b)->flyout ? ZST_AS(zrn_button_base_t, b)->flyout->id : Z_INVALID_ID;
}


/* ============================================================================
 *  Flyout
 * ============================================================================ */

static void flyout_init(zrn_element_t* const e) {
	zrn_element_t* const presenter = presenter_of(FB(e));
	/* FlyoutPresenter: the content starts 25.5 px under the top edge, and it is at least 43 px tall */
	if (presenter) ZST_AS(zrn_border_t, presenter)->padding = Zthickness(0, FLYOUT_CONTENT_TOP, 0, 0);
}

zrn_class_t zrn_class_flyout = {
	.base = &zrn_class_flyout_base, .name = "Flyout", .id = Z_CLASS_FLYOUT, .size = sizeof(zrn_flyout_base_t), .init = flyout_init
};

Zid Zflyout_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_FLYOUT);
	return e ? e->id : Z_INVALID_ID;
}

void Zflyout_set_content(const Zid flyout, const Zid content) {
	zrn_element_t* const e = zrn_get(flyout, Z_CLASS_FLYOUT);
	zrn_element_t* const c = content != Z_INVALID_ID ? zrn_get(content, Z_CLASS_ELEMENT) : nullptr;
	zrn_element_t* presenter;
	zrn_element_t* slot;
	if (!e || (content != Z_INVALID_ID && !c)) return;
	presenter = presenter_of(FB(e));
	if (!presenter) return;
	slot = presenter->child_count ? presenter->children[0] : nullptr;
	zrn_set_only_child(presenter, &slot, c);
}

Zid Zflyout_get_content(const Zid flyout) {
	zrn_element_t* const e = zrn_get(flyout, Z_CLASS_FLYOUT);
	const zrn_element_t* const presenter = e ? presenter_of(FB(e)) : nullptr;
	return presenter && presenter->child_count ? presenter->children[0]->id : Z_INVALID_ID;
}


/* ============================================================================
 *  MenuFlyoutItem, ToggleMenuFlyoutItem and MenuFlyoutSeparator
 * ============================================================================ */

static zrn_font_t menu_font(void) {
	const zrn_font_t f = { Z_FONT_SEMILIGHT, MENU_FONT, MENU_SPACING };
	return f;
}

static void item_destroy(zrn_element_t* e) { free(ITEM(e)->text); }

static void item_init(zrn_element_t* const e) {
	const auto c = ZST_AS(zrn_control_t, e);
	c->padding = Zthickness(0, MENU_ITEM_PAD, 0, MENU_ITEM_PAD);
	c->font_family = Z_FONT_SEMILIGHT;
	c->font_size = MENU_FONT;
	e->focusable = 0;
}

static ZSize item_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_control_t* const c = ZST_AS(zrn_control_t, e);
	const char* t = ITEM(e)->text ? ITEM(e)->text : "";
	ZST_UNUSED(available);
	return zrn_size(zrn_text_width(menu_font(), t, (int)strlen(t)) + zrn_thickness_h(c->padding), zrn_font_line_height(menu_font()) + zrn_thickness_v(c->padding));
}

static void item_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_control_t* const c = ZST_AS(zrn_control_t, e);
	const char* t = ITEM(e)->text ? ITEM(e)->text : "";
	/* MenuFlyoutItemDisabledForegroundThemeBrush is PhoneDisabled */
	const ZColor fg = zrn_effectively_enabled(e) ? zrn_ctl_foreground(c) : zrn_theme_color(e, ZST_TC_PHONE_DISABLED);
	zrn_draw_string(p, menu_font(), t, (int)strlen(t), c->padding.left, c->padding.top, fg);
	if (e->cls == &zrn_class_toggle_menu_flyout_item && ITEM(e)->is_checked) {
		/* the template has no mark for it: a check at the right edge, as high as a digit */
		const float x = e->rect.width - 19.0f - 16.0f, y = e->rect.height * 0.5f;
		zrn_draw_line(p, x, y, x + 5.5f, y + 5.5f, 2.5f, fg);
		zrn_draw_line(p, x + 5.5f, y + 5.5f, x + 16.0f, y - 6.0f, 2.5f, fg);
	}
}

/* A tap runs Click, and the flyout the item is in closes. */
static int item_input(zrn_element_t* e, zrn_input_t* in) {
	const auto m = ITEM(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			m->pressed = 1;
			if (e->root) zrn_set_capture(e->root, e);
			return 1;
		case ZST_IN_POINTER_UP: {
			const int inside = in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			const Zid id = e->id;
			zrn_element_t* p;
			if (!m->pressed) return 0;
			m->pressed = 0;
			if (e->root) zrn_release_capture(e->root, e);
			if (!inside) return 1;
			if (e->cls == &zrn_class_toggle_menu_flyout_item) {
				m->is_checked = !m->is_checked;
				zrn_changed_paint(e);
			}
			if (!zrn_raise(e, Z_EVENT_CLICK)) return 1;
			e = zrn_lookup(id);
			for (p = e->parent; p; p = p->parent) {
				if (p->cls == &zrn_class_popup) {
					zrn_popup_close(p);
					break;
				}
			}
			return 1;
		}
		case ZST_IN_CAPTURE_LOST:
			m->pressed = 0;
			return 0;
		default:
			return 0;
	}
}

zrn_class_t zrn_class_menu_flyout_item_base = {
	.base = &zrn_class_control, .name = "MenuFlyoutItemBase", .id = Z_CLASS_MENU_FLYOUT_ITEM_BASE, .size = sizeof(zrn_menu_flyout_item_t), .abstract_class = 1,
	.init = item_init, .destroy = item_destroy
};

zrn_class_t zrn_class_menu_flyout_item = {
	.base = &zrn_class_menu_flyout_item_base, .name = "MenuFlyoutItem", .id = Z_CLASS_MENU_FLYOUT_ITEM, .size = sizeof(zrn_menu_flyout_item_t),
	.measure = item_measure, .paint = item_paint, .input = item_input
};

zrn_class_t zrn_class_toggle_menu_flyout_item = {
	.base = &zrn_class_menu_flyout_item, .name = "ToggleMenuFlyoutItem", .id = Z_CLASS_TOGGLE_MENU_FLYOUT_ITEM, .size = sizeof(zrn_menu_flyout_item_t)
};

/* The template of the separator is not in the phone XAML: a hairline in a row as high as the other rows' padding. */
static ZSize separator_measure(zrn_element_t* const e, const ZSize available) {
	ZST_UNUSED(e); ZST_UNUSED(available);
	return zrn_size(0, 2 * MENU_ITEM_PAD);
}

static void separator_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_fill_rect(p, zrn_rect(0, e->rect.height * 0.5f - 0.5f, e->rect.width, 1.0f), zrn_theme_color(e, ZST_TC_BASE_MEDIUM_MID));
}

zrn_class_t zrn_class_menu_flyout_separator = {
	.base = &zrn_class_menu_flyout_item_base, .name = "MenuFlyoutSeparator", .id = Z_CLASS_MENU_FLYOUT_SEPARATOR, .size = sizeof(zrn_menu_flyout_item_t),
	.measure = separator_measure, .paint = separator_paint
};

static Zid make_menu_item(const ZClass cls, const char* const text) {
	zrn_element_t* const e = zrn_create(cls);
	if (!e) return Z_INVALID_ID;
	zrn_set_string(&ITEM(e)->text, text ? text : "");
	return e->id;
}

Zid Zmenu_flyout_item_create(const char* text) { return make_menu_item(Z_CLASS_MENU_FLYOUT_ITEM, text); }

Zid Ztoggle_menu_flyout_item_create(const char* const text, const int is_checked) {
	const Zid id = make_menu_item(Z_CLASS_TOGGLE_MENU_FLYOUT_ITEM, text);
	if (id != Z_INVALID_ID) ITEM(zrn_lookup(id))->is_checked = is_checked != 0;
	return id;
}

Zid Zmenu_flyout_separator_create(void) { return make_menu_item(Z_CLASS_MENU_FLYOUT_SEPARATOR, nullptr); }

void Zmenu_flyout_item_set_text(const Zid menu_flyout_item, const char* const text) {
	zrn_element_t* const e = zrn_get(menu_flyout_item, Z_CLASS_MENU_FLYOUT_ITEM);
	if (!e) return;
	zrn_set_string(&ITEM(e)->text, text ? text : "");
	zrn_changed_layout(e);
}

const char* Zmenu_flyout_item_get_text(const Zid menu_flyout_item) {
	zrn_element_t* const e = zrn_get(menu_flyout_item, Z_CLASS_MENU_FLYOUT_ITEM);
	return e && ITEM(e)->text ? ITEM(e)->text : "";
}

void Zmenu_flyout_item_on_click(const Zid menu_flyout_item, const ZRoutedCallback callback, void* const user_data) {
	if (zrn_get(menu_flyout_item, Z_CLASS_MENU_FLYOUT_ITEM)) Zelement_on(menu_flyout_item, Z_EVENT_CLICK, callback, user_data);
}

void Ztoggle_menu_flyout_item_set_is_checked(const Zid toggle_menu_flyout_item, const int is_checked) {
	zrn_element_t* const e = zrn_get(toggle_menu_flyout_item, Z_CLASS_TOGGLE_MENU_FLYOUT_ITEM);
	if (!e) return;
	ITEM(e)->is_checked = is_checked != 0;
	zrn_changed_paint(e);
}

int Ztoggle_menu_flyout_item_get_is_checked(const Zid toggle_menu_flyout_item) {
	zrn_element_t* const e = zrn_get(toggle_menu_flyout_item, Z_CLASS_TOGGLE_MENU_FLYOUT_ITEM);
	return e ? ITEM(e)->is_checked : 0;
}


/* ============================================================================
 *  MenuFlyout
 * ============================================================================ */

static zrn_element_t* menu_stack(zrn_flyout_base_t* const f) {
	const zrn_element_t* const presenter = presenter_of(f);
	return presenter && presenter->child_count ? presenter->children[0] : nullptr;
}

static void menu_flyout_init(zrn_element_t* const e) {
	zrn_element_t* const presenter = presenter_of(FB(e));
	zrn_element_t* const stack = zrn_create(Z_CLASS_STACK_PANEL);
	if (!presenter || !stack) {
		if (stack) zrn_destroy(stack);
		return;
	}
	/* MenuFlyoutPresenter: a 1.5 px line above and below, the items padded 19,13.5,0,26 */
	ZST_AS(zrn_border_t, presenter)->border_thickness = Zthickness(0, MENU_BORDER, 0, MENU_BORDER);
	ZST_AS(zrn_border_t, presenter)->padding = Zthickness(MENU_PAD_LEFT, MENU_PAD_TOP, 0, MENU_PAD_BOTTOM);
	zrn_add_child(presenter, stack, -1);
}

zrn_class_t zrn_class_menu_flyout = {
	.base = &zrn_class_flyout_base, .name = "MenuFlyout", .id = Z_CLASS_MENU_FLYOUT, .size = sizeof(zrn_flyout_base_t), .init = menu_flyout_init
};

Zid Zmenu_flyout_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_MENU_FLYOUT);
	return e ? e->id : Z_INVALID_ID;
}

int Zmenu_flyout_add_item(const Zid menu_flyout, const Zid item) {
	zrn_element_t* const e = zrn_get(menu_flyout, Z_CLASS_MENU_FLYOUT);
	zrn_element_t* const i = item != Z_INVALID_ID ? zrn_get(item, Z_CLASS_MENU_FLYOUT_ITEM_BASE) : nullptr;
	zrn_element_t* stack;
	if (!e || !i) return -1;
	stack = menu_stack(FB(e));
	if (!stack || i->parent) {
		if (i->parent) zrn_error(Z_ERROR_HAS_PARENT, "the item already has a parent");
		return -1;
	}
	zrn_add_child(stack, i, -1);
	return i->parent == stack ? stack->child_count - 1 : -1;
}

int Zmenu_flyout_get_item_count(const Zid menu_flyout) {
	zrn_element_t* const e = zrn_get(menu_flyout, Z_CLASS_MENU_FLYOUT);
	const zrn_element_t* const stack = e ? menu_stack(FB(e)) : nullptr;
	return stack ? stack->child_count : 0;
}

Zid Zmenu_flyout_get_item(const Zid menu_flyout, const int index) {
	zrn_element_t* const e = zrn_get(menu_flyout, Z_CLASS_MENU_FLYOUT);
	const zrn_element_t* const stack = e ? menu_stack(FB(e)) : nullptr;
	return stack && index >= 0 && index < stack->child_count ? stack->children[index]->id : Z_INVALID_ID;
}

void Zmenu_flyout_clear_items(const Zid menu_flyout) {
	zrn_element_t* const e = zrn_get(menu_flyout, Z_CLASS_MENU_FLYOUT);
	zrn_element_t* const stack = e ? menu_stack(FB(e)) : nullptr;
	if (stack) zrn_clear_children(stack);
}


/* ============================================================================
 *  ContentDialog
 *  A modal Popup over the whole screen: what is under it is dimmed and does not get the input,
 *  and Back closes it as Hide does. The dialog lies at the top, as wide as the screen.
 * ============================================================================ */

static void dialog_button_click(Zid sender, void* user_data);
static void dialog_popup_closed(Zid popup, void* user_data);

static zrn_element_t* make_dialog_button(zrn_element_t* const dialog, const int primary) {
	zrn_element_t* const b = zrn_create(Z_CLASS_BUTTON);
	if (!b) return nullptr;
	b->halign = Z_HALIGN_STRETCH;
	b->valign = Z_VALIGN_TOP;
	set_handler(b, Z_EVENT_CLICK, dialog_button_click, (void*)(uintptr_t)dialog->id);
	b->tag = (void*)(uintptr_t)(primary ? 1 : 2);
	zrn_add_child(dialog, b, -1);
	return b;
}

static void dialog_init(zrn_element_t* const e) {
	const auto d = DIALOG(e);
	zrn_control_t* const c = &d->base.base;
	zrn_text_block_t* t;
	e->focusable = 0;
	c->font_size = DIALOG_CONTENT_FONT;
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
	e->halign = Z_HALIGN_STRETCH;
	e->valign = Z_VALIGN_TOP;
	d->primary_enabled = d->secondary_enabled = 1;
	d->title = zrn_create(Z_CLASS_TEXT_BLOCK);
	if (d->title) {
		t = ZST_AS(zrn_text_block_t, d->title);
		t->font_size = DIALOG_TITLE_FONT;
		t->font_family = Z_FONT_SEMIBOLD;
		t->set_flags |= ZST_TB_FONT_SIZE | ZST_TB_FONT_FAMILY;
		t->wrapping = Z_WRAP;
		zrn_add_child(e, d->title, -1);
	}
	d->button1 = make_dialog_button(e, 1);
	d->button2 = make_dialog_button(e, 0);
	if (d->button1) d->button1->visibility = Z_COLLAPSED;
	if (d->button2) d->button2->visibility = Z_COLLAPSED;
}

static void dialog_destroy(zrn_element_t* const e) {
	const auto d = DIALOG(e);
	zrn_element_t* const popup = d->popup != Z_INVALID_ID ? zrn_lookup(d->popup) : nullptr;
	/* a dialog that is destroyed while it shows takes its popup with it, unless the popup is what is destroying it */
	if (popup && !popup->destroying) zrn_destroy(popup);
	d->popup = Z_INVALID_ID;
	free(d->primary_text);
	free(d->secondary_text);
}

static void dialog_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	const auto d = DIALOG(e);
	if (d->title == child) d->title = nullptr;
	else if (d->button1 == child) d->button1 = nullptr;
	else if (d->button2 == child) d->button2 = nullptr;
	else zrn_cc_child_removed(e, child);
}

static int visible(const zrn_element_t* e) { return e && e->visibility == Z_VISIBLE; }

static float title_height(const zrn_content_dialog_t* const d) {
	return d->title && visible(d->title) ? d->title->desired.height : 0;
}

static ZSize dialog_measure(zrn_element_t* const e, const ZSize available) {
	const auto d = DIALOG(e);
	const float w = available.width < Z_INFINITE * 0.5f ? available.width : 480.0f;
	const float inner = zrn_maxf(0, w - 2 * DIALOG_SIDE);
	const float col = w * 0.5f;
	float title_row, content_row, buttons_row = DIALOG_BUTTONS_MIN, ch = 0;
	if (d->title) {
		const char* s = Ztext_block_get_text(d->title->id);
		d->title->visibility = s && *s ? Z_VISIBLE : Z_COLLAPSED;
		zrn_measure(d->title, zrn_size(inner, Z_INFINITE));
	}
	title_row = zrn_maxf(DIALOG_TITLE_MIN, DIALOG_TITLE_TOP + title_height(d));
	if (d->base.content) {
		zrn_measure(d->base.content, zrn_size(inner, Z_INFINITE));
		ch = d->base.content->desired.height;
	}
	content_row = zrn_maxf(DIALOG_CONTENT_MIN, DIALOG_CONTENT_TOP + ch);
	if (d->button1) zrn_measure(d->button1, zrn_size(zrn_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), Z_INFINITE));
	if (d->button2) zrn_measure(d->button2, zrn_size(zrn_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), Z_INFINITE));
	if (visible(d->button1) || visible(d->button2)) {
		const float bh = zrn_maxf(visible(d->button1) ? d->button1->desired.height : 0, visible(d->button2) ? d->button2->desired.height : 0);
		buttons_row = zrn_maxf(DIALOG_BUTTONS_MIN, DIALOG_BUTTON_TOP + bh + DIALOG_BUTTON_BOTTOM);
	}
	return zrn_size(w, title_row + content_row + buttons_row);
}

static ZSize dialog_arrange(zrn_element_t* const e, const ZSize final) {
	const auto d = DIALOG(e);
	const float inner = zrn_maxf(0, final.width - 2 * DIALOG_SIDE), col = final.width * 0.5f;
	const float title_row = zrn_maxf(DIALOG_TITLE_MIN, DIALOG_TITLE_TOP + title_height(d));
	const float ch = d->base.content ? d->base.content->desired.height : 0;
	const float content_row = zrn_maxf(DIALOG_CONTENT_MIN, DIALOG_CONTENT_TOP + ch);
	const float y = title_row + content_row;
	if (d->title) zrn_arrange(d->title, zrn_rect(DIALOG_SIDE, DIALOG_TITLE_TOP, inner, title_height(d)));
	if (d->base.content) zrn_arrange(d->base.content, zrn_rect(DIALOG_SIDE, title_row + DIALOG_CONTENT_TOP, inner, ch));
	if (d->button1) zrn_arrange(d->button1, zrn_rect(DIALOG_SIDE, y + DIALOG_BUTTON_TOP, zrn_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), d->button1->desired.height));
	if (d->button2) zrn_arrange(d->button2, zrn_rect(col + DIALOG_BUTTON_GAP, y + DIALOG_BUTTON_TOP, zrn_maxf(0, col - DIALOG_SIDE - DIALOG_BUTTON_GAP), d->button2->desired.height));
	return final;
}

static void dialog_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	/* ContentDialogBackgroundThemeBrush is the chrome */
	zrn_fill_rect(p, zrn_local_rect(e), zrn_ctl_background(&DIALOG(e)->base.base, zrn_theme_color(e, ZST_TC_CHROME)));
}

zrn_class_t zrn_class_content_dialog = {
	.base = &zrn_class_content_control, .name = "ContentDialog", .id = Z_CLASS_CONTENT_DIALOG, .size = sizeof(zrn_content_dialog_t),
	.init = dialog_init, .destroy = dialog_destroy, .child_removed = dialog_child_removed, .measure = dialog_measure,
	.arrange = dialog_arrange, .paint = dialog_paint
};

Zid Zcontent_dialog_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_CONTENT_DIALOG);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_content_dialog_t* get_dialog(const Zid id) { return DIALOG(zrn_get(id, Z_CLASS_CONTENT_DIALOG)); }

void Zcontent_dialog_set_title(const Zid content_dialog, const char* const title) {
	zrn_content_dialog_t* const d = get_dialog(content_dialog);
	if (d && d->title) {
		Ztext_block_set_text(d->title->id, title ? title : "");
		zrn_changed_layout(&d->base.base.base);
	}
}

const char* Zcontent_dialog_get_title(const Zid content_dialog) {
	const zrn_content_dialog_t* const d = get_dialog(content_dialog);
	return d && d->title ? Ztext_block_get_text(d->title->id) : "";
}

static void dialog_set_button_text(const Zid id, const int primary, const char* const text) {
	zrn_content_dialog_t* const d = get_dialog(id);
	zrn_element_t* b;
	if (!d) return;
	b = primary ? d->button1 : d->button2;
	zrn_set_string(primary ? &d->primary_text : &d->secondary_text, text && *text ? text : nullptr);
	if (!b) return;
	/* a button with no text is not there */
	b->visibility = text && *text ? Z_VISIBLE : Z_COLLAPSED;
	Zcontent_control_set_text(b->id, text ? text : "");
	zrn_changed_layout(&d->base.base.base);
}

void Zcontent_dialog_set_primary_button_text(Zid id, const char* text) { dialog_set_button_text(id, 1, text); }
void Zcontent_dialog_set_secondary_button_text(Zid content_dialog, const char* text) { dialog_set_button_text(content_dialog, 0, text); }

const char* Zcontent_dialog_get_primary_button_text(const Zid content_dialog) {
	const zrn_content_dialog_t* const d = get_dialog(content_dialog);
	return d && d->primary_text ? d->primary_text : "";
}

const char* Zcontent_dialog_get_secondary_button_text(const Zid content_dialog) {
	const zrn_content_dialog_t* const d = get_dialog(content_dialog);
	return d && d->secondary_text ? d->secondary_text : "";
}

void Zcontent_dialog_set_is_primary_button_enabled(const Zid content_dialog, const int is_enabled) {
	zrn_content_dialog_t* const d = get_dialog(content_dialog);
	if (!d) return;
	d->primary_enabled = is_enabled != 0;
	if (d->button1) Zelement_set_enabled(d->button1->id, is_enabled);
}

void Zcontent_dialog_set_is_secondary_button_enabled(const Zid content_dialog, const int is_enabled) {
	zrn_content_dialog_t* const d = get_dialog(content_dialog);
	if (!d) return;
	d->secondary_enabled = is_enabled != 0;
	if (d->button2) Zelement_set_enabled(d->button2->id, is_enabled);
}

int Zcontent_dialog_get_is_primary_button_enabled(const Zid content_dialog) {
	const zrn_content_dialog_t* const d = get_dialog(content_dialog);
	return d ? d->primary_enabled : 0;
}

int Zcontent_dialog_get_is_secondary_button_enabled(const Zid content_dialog) {
	const zrn_content_dialog_t* const d = get_dialog(content_dialog);
	return d ? d->secondary_enabled : 0;
}

void Zcontent_dialog_on_primary_button_click(const Zid content_dialog, const ZRoutedCallback callback, void* const user_data) {
	if (get_dialog(content_dialog)) Zelement_on(content_dialog, Z_EVENT_PRIMARY_BUTTON_CLICK, callback, user_data);
}

void Zcontent_dialog_on_secondary_button_click(const Zid content_dialog, const ZRoutedCallback callback, void* const user_data) {
	if (get_dialog(content_dialog)) Zelement_on(content_dialog, Z_EVENT_SECONDARY_BUTTON_CLICK, callback, user_data);
}

void Zcontent_dialog_on_opened(const Zid content_dialog, const ZRoutedCallback callback, void* const user_data) {
	if (get_dialog(content_dialog)) Zelement_on(content_dialog, Z_EVENT_OPENED, callback, user_data);
}

void Zcontent_dialog_on_closed(const Zid content_dialog, const ZRoutedCallback callback, void* const user_data) {
	if (get_dialog(content_dialog)) Zelement_on(content_dialog, Z_EVENT_CLOSED, callback, user_data);
}

/* The dialog is over: the popup goes, the focus returns where it was, Closed and then the callback tell. */
static void dialog_finish(zrn_element_t* const e, const ZContentDialogResult result) {
	const auto d = DIALOG(e);
	zrn_root_t* const r = e->root;
	zrn_element_t* const popup = d->popup != Z_INVALID_ID ? zrn_lookup(d->popup) : nullptr;
	zrn_element_t* const previous = d->previous_focus != Z_INVALID_ID ? zrn_lookup(d->previous_focus) : nullptr;
	const ZContentDialogResultCallback callback = d->callback;
	void* const data = d->callback_data;
	const Zid id = e->id;
	if (!d->is_open) return;
	d->is_open = 0;
	d->result = result;
	d->callback = nullptr;
	d->callback_data = nullptr;
	d->previous_focus = Z_INVALID_ID;
	d->popup = Z_INVALID_ID;
	if (popup) {
		zrn_popup_set_open(popup, 0);
		/* the dialog goes back to the program, the popup is only the means of showing it */
		zrn_remove_child(popup, e);
		zrn_destroy(popup);
	}
	if (r && r->focus == nullptr && previous && previous->root == r) zrn_set_focus(r, previous);
	if (!zrn_raise(zrn_lookup(id), Z_EVENT_CLOSED)) return;
	if (callback) callback(id, result, data);
}

static void dialog_button_click(const Zid sender, void* const user_data) {
	zrn_element_t* const e = zrn_lookup((Zid)(uintptr_t)user_data);
	zrn_element_t* const b = zrn_lookup(sender);
	int primary;
	Zid id;
	if (!e || !b || !DIALOG(e)->is_open) return;
	id = e->id;
	primary = (uintptr_t)b->tag == 1;
	if (!zrn_raise(e, primary ? Z_EVENT_PRIMARY_BUTTON_CLICK : Z_EVENT_SECONDARY_BUTTON_CLICK)) return;
	dialog_finish(zrn_lookup(id), primary ? Z_CONTENT_DIALOG_RESULT_PRIMARY : Z_CONTENT_DIALOG_RESULT_SECONDARY);
}

/* Back, or anything that closes the popup but a button: the result is None. */
static void dialog_popup_closed(const Zid popup, void* const user_data) {
	zrn_element_t* const e = zrn_lookup((Zid)(uintptr_t)user_data);
	ZST_UNUSED(popup);
	if (e && DIALOG(e)->is_open) dialog_finish(e, Z_CONTENT_DIALOG_RESULT_NONE);
}

int Zcontent_dialog_show(const Zid content_dialog, const Zid root, const ZContentDialogResultCallback callback, void* const user_data) {
	zrn_content_dialog_t* const d = get_dialog(content_dialog);
	zrn_root_t* const r = zrn_root_lookup(root);
	zrn_element_t *e, *popup, *focus;
	if (!d) return false;
	e = &d->base.base.base;
	if (!r) {
		zrn_error(Z_ERROR_INVALID_ID, "invalid root handle 0x%x", (unsigned)root);
		return false;
	}
	if (d->is_open) return false;
	if (e->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "a ContentDialog is shown with Zcontent_dialog_show, it is not put in a tree");
		return false;
	}
	popup = zrn_create(Z_CLASS_POPUP);
	if (!popup || !r->overlay) {
		if (popup) zrn_destroy(popup);
		return false;
	}
	ZST_AS(zrn_popup_t, popup)->modal = 1;
	ZST_AS(zrn_popup_t, popup)->stays_open = 1;
	ZST_AS(zrn_popup_t, popup)->full_width = 1;
	set_handler(popup, Z_EVENT_POPUP_CLOSED, dialog_popup_closed, (void*)(uintptr_t)e->id);
	zrn_add_child(popup, e, -1);
	e->is_popup_child = 1;
	zrn_add_child(r->overlay, popup, -1);
	d->popup = popup->id;
	d->callback = callback;
	d->callback_data = user_data;
	d->is_open = 1;
	d->previous_focus = r->focus ? r->focus->id : Z_INVALID_ID;
	zrn_popup_set_open(popup, 1);
	/* the keyboard goes into the dialog: the button that is the answer, or the first one there is */
	focus = visible(d->button1) && d->primary_enabled ? d->button1 : (visible(d->button2) && d->secondary_enabled ? d->button2 : nullptr);
	zrn_set_focus(r, focus);
	if (!zrn_raise(e, Z_EVENT_OPENED)) return true;
	return true;
}

void Zcontent_dialog_hide(const Zid content_dialog) {
	zrn_content_dialog_t* const d = get_dialog(content_dialog);
	if (d && d->is_open) dialog_finish(&d->base.base.base, Z_CONTENT_DIALOG_RESULT_NONE);
}
