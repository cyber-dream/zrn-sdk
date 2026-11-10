/**
 * zrnui - controls/combo.c
 * ComboBox and AutoSuggestBox, the two controls whose list is a Popup.
 *
 * ComboBox follows the inline template of generic.xaml (the one for a short list): a header, then a
 * box with a 2.5 px outline that holds the selected item, with PhoneTouchTargetOverhang 0,9.5 above and
 * below. A tap opens the list over the box, white with an accent border. AutoSuggestBox is a TextBox
 * and a Popup with a ListView of suggestions.
 */

#include "../internal.h"

#define IC(e) ZST_AS(zrn_items_control_t, e)
#define CB(e) ZST_AS(zrn_combo_box_t, e)
#define ASB(e) ZST_AS(zrn_auto_suggest_box_t, e)

/* ComboBox: ComboBoxBorderThemeThickness 2.5, ComboBoxItemMinHeightThemeSize 33.5, Padding 6.5 of its items, Header margin 0,0,0,-4.5 */
static constexpr float COMBO_BORDER = 2.5f;
static constexpr float COMBO_MIN_HEIGHT = 33.5f;
static constexpr float COMBO_TEXT_LEFT = 6.5f;
static constexpr float COMBO_HEADER_OVERLAP = 4.5f;
static constexpr float COMBO_OVERHANG = 9.5f;
static constexpr float HEADER_FONT = 16.0f;

static zrn_font_t header_font(void) { const zrn_font_t f = { Z_FONT_NORMAL, HEADER_FONT, 0 }; return f; }

/* The popups: a Border of the highlighted look around a ScrollViewer or a ListView. */
static zrn_element_t* make_drop_down(zrn_element_t* const owner, zrn_element_t* const content, const ZColor background, const ZColor border_brush) {
	zrn_element_t* const border = zrn_create(Z_CLASS_BORDER);
	zrn_element_t* const popup = zrn_create(Z_CLASS_POPUP);
	zrn_border_t* b;
	if (!border || !popup) {
		if (border) zrn_destroy(border);
		if (popup) zrn_destroy(popup);
		return nullptr;
	}
	b = ZST_AS(zrn_border_t, border);
	b->background = background;
	b->has_background = 1;
	b->border_brush = border_brush;
	b->has_border_brush = 1;
	b->border_thickness = Zthickness_uniform(COMBO_BORDER);
	zrn_add_child(border, content, -1);
	zrn_add_child(popup, border, -1);
	border->is_popup_child = 1;
	zrn_add_child(owner, popup, -1);
	ZST_AS(zrn_popup_t, popup)->target = owner->id;
	return popup;
}

/* The popups follow the theme of the moment they open. */
static void style_drop_down(const zrn_element_t* const popup, const ZColor background, const ZColor border_brush) {
	const auto b = ZST_AS(zrn_border_t, popup->children[0]);
	b->background = background;
	b->border_brush = border_brush;
}


/* ============================================================================
 *  ComboBox
 * ============================================================================ */

static float combo_header_height(const zrn_combo_box_t* const c) {
	return c->header && *c->header ? zrn_font_line_height(header_font()) - COMBO_HEADER_OVERLAP : 0;
}

static ZRect combo_box_rect(const zrn_element_t* const e) {
	const float top = combo_header_height(CB(e)) + COMBO_OVERHANG;
	return zrn_rect(0, top, e->rect.width, zrn_maxf(0, e->rect.height - top - COMBO_OVERHANG));
}

static const char* combo_text(const zrn_combo_box_t* const c) {
	zrn_element_t* const item = c->base.selected_index >= 0 ? zrn_ic_item(&c->base, c->base.selected_index) : nullptr;
	return item ? zrn_container_text(item) : "";
}

static ZSize combo_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_combo_box_t* const c = CB(e);
	const zrn_font_t font = { c->base.base.font_family, c->base.base.font_size, 0 };
	float w = c->placeholder ? zrn_text_width(font, c->placeholder, (int)strlen(c->placeholder)) : 0;
	int i, n = zrn_ic_count(&c->base);
	ZST_UNUSED(available);
	for (i = 0; i < n; i++) {
		const char* t = zrn_container_text(zrn_ic_item(&c->base, i));
		w = zrn_maxf(w, zrn_text_width(font, t, (int)strlen(t)));
	}
	return zrn_size(w + COMBO_TEXT_LEFT + 2 * COMBO_BORDER, combo_header_height(c) + 2 * COMBO_OVERHANG + COMBO_MIN_HEIGHT + 2 * COMBO_BORDER);
}

static ZSize combo_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_combo_box_t* const c = CB(e);
	/* the list is laid out by the root, but it is as wide as the box */
	if (c->popup && c->popup->child_count && c->popup->children[0]->width != final.width) {
		c->popup->children[0]->width = final.width;
		zrn_changed_layout(c->popup->children[0]);
	}
	return final;
}

static void combo_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto c = CB(e);
	const zrn_control_t* const ctl = &c->base.base;
	const zrn_font_t font = { ctl->font_family, ctl->font_size, 0 };
	const ZRect box = combo_box_rect(e);
	ZColor fg = zrn_ctl_color(ctl, ZST_C_FOREGROUND, ctl->foreground, zrn_theme_color(e, ZST_TC_FOREGROUND));
	ZColor border = zrn_ctl_border_brush(ctl, zrn_theme_color(e, ZST_TC_FOREGROUND));
	ZColor background = zrn_ctl_background(ctl, Zcolor_rgba(0, 0, 0, 0));
	const int enabled = zrn_effectively_enabled(e), has_selection = c->base.selected_index >= 0;
	const char* text = has_selection ? combo_text(c) : (c->placeholder ? c->placeholder : "");
	const float tx = box.x + COMBO_BORDER + COMBO_TEXT_LEFT;
	if (c->header && *c->header) {
		zrn_draw_string(p, header_font(), c->header, (int)strlen(c->header), 0, 0, zrn_theme_color(e, enabled ? ZST_TC_BASE_MID : ZST_TC_BASE_LOW));
	}
	if (!enabled) {
		fg = border = zrn_theme_color(e, ZST_TC_BASE_LOW);
		background = Zcolor_rgba(0, 0, 0, 0);
	} else if (c->is_open) {
		/* Highlighted: ComboBoxHighlightedBackground white, the accent border, black text */
		background = zrn_theme_color(e, ZST_TC_BASE_WHITE);
		border = zrn_theme_color(e, ZST_TC_ACCENT);
		fg = zrn_theme_color(e, ZST_TC_BASE_BLACK);
	} else if (c->pressed) {
		background = zrn_theme_color(e, ZST_TC_ACCENT);
		border = zrn_theme_color(e, ZST_TC_FOREGROUND);
		fg = zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT);
	}
	if (!has_selection) {
		/* ComboBoxFlyoutListPlaceholderTextOpacity: 0.6, 0.45 in the Light theme */
		fg = zrn_color_alpha(fg, e->root && e->root->theme.theme == Z_THEME_LIGHT ? 0.45f : 0.6f);
	}
	if (background.a > 0) {
		const ZThickness t = Zthickness_uniform(COMBO_BORDER);
		zrn_fill_rect(p, zrn_rect(box.x + t.left, box.y + t.top, box.width - zrn_thickness_h(t), box.height - zrn_thickness_v(t)), background);
	}
	zrn_stroke_rect(p, box, Zthickness_uniform(COMBO_BORDER), border);
	zrn_push_clip(p, zrn_rect(box.x + COMBO_BORDER, box.y + COMBO_BORDER, zrn_maxf(0, box.width - 2 * COMBO_BORDER), zrn_maxf(0, box.height - 2 * COMBO_BORDER)));
	zrn_draw_string(p, font, text, (int)strlen(text), tx, box.y + (box.height - zrn_font_line_height(font)) * 0.5f, fg);
	zrn_pop_clip(p);
}

/* @return false when a handler destroyed the box. */
static int combo_set_open(zrn_combo_box_t* c, int open) {
	zrn_element_t* const e = &c->base.base.base;
	const Zid id = e->id;
	open = open != 0;
	if (c->is_open == open || !c->popup) return 1;
	c->is_open = open;
	if (open) {
		zrn_element_t* const border = c->popup->children[0];
		const auto popup = ZST_AS(zrn_popup_t, c->popup);
		float above = 0;
		int i;
		style_drop_down(c->popup, zrn_theme_color(e, ZST_TC_BASE_WHITE), zrn_theme_color(e, ZST_TC_ACCENT));
		border->width = e->rect.width;
		if (c->base.scroll) c->base.scroll->max_height = c->max_drop_down_height;
		/* the list opens over the box with the selected item where the box shows it */
		if (c->base.host && c->base.selected_index > 0) {
			zrn_measure(c->base.host, zrn_size(zrn_maxf(0, e->rect.width - 2 * COMBO_BORDER), Z_INFINITE));
			for (i = 0; i < c->base.selected_index; i++) above += zrn_ic_item(&c->base, i)->desired.height;
		}
		popup->placement = Z_PLACE_BOTTOM;
		popup->h_offset = 0;
		popup->v_offset = combo_box_rect(e).y - e->rect.height - above - 5.0f;
		Zpopup_set_open(c->popup->id, 1);
		if (!zrn_raise(e, Z_EVENT_DROP_DOWN_OPENED)) return 0;
	} else {
		Zpopup_set_open(c->popup->id, 0);
		if (!zrn_raise(e, Z_EVENT_DROP_DOWN_CLOSED)) return 0;
	}
	if (zrn_lookup(id)) zrn_changed_paint(e);
	return zrn_lookup(id) != nullptr;
}

static void combo_popup_closed(const Zid popup, void* const user) {
	const auto c = (zrn_combo_box_t*)user;
	ZST_UNUSED(popup);
	if (!c->is_open) return;
	c->is_open = 0;
	if (zrn_raise(&c->base.base.base, Z_EVENT_DROP_DOWN_CLOSED)) zrn_changed_paint(&c->base.base.base);
}

void zrn_combo_item_tapped(zrn_items_control_t* const o, const int index) {
	const auto c = (zrn_combo_box_t*)o;
	const Zid id = o->base.base.id;
	/* a tap on an item chooses it and closes the list */
	zrn_ic_select(o, index);
	if (zrn_lookup(id)) combo_set_open(c, 0);
}

static int combo_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto c = CB(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			if (!zrn_effectively_enabled(e)) return 1;
			c->pressed = 1;
			zrn_changed_paint(e);
			if (e->root) zrn_set_capture(e->root, e);
			return 1;
		case ZST_IN_POINTER_UP: {
			const int tap = c->pressed && in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			c->pressed = 0;
			zrn_changed_paint(e);
			if (e->root) zrn_release_capture(e->root, e);
			if (tap) combo_set_open(c, !c->is_open);
			return 1;
		}
		case ZST_IN_CAPTURE_LOST:
			c->pressed = 0;
			zrn_changed_paint(e);
			return 0;
		case ZST_IN_KEY_DOWN:
			if (in->key == Z_KEY_ENTER || in->key == Z_KEY_SPACE) { combo_set_open(c, !c->is_open); return 1; }
			if (in->key == Z_KEY_ESCAPE && c->is_open) { combo_set_open(c, 0); return 1; }
			if (in->key == Z_KEY_DOWN) { if (c->base.selected_index + 1 < zrn_ic_count(&c->base)) zrn_ic_select(&c->base, c->base.selected_index + 1); return 1; }
			if (in->key == Z_KEY_UP) { if (c->base.selected_index > 0) zrn_ic_select(&c->base, c->base.selected_index - 1); return 1; }
			return 0;
		case ZST_IN_FOCUS_IN:
		case ZST_IN_FOCUS_OUT:
			zrn_changed_paint(e);
			return 0;
		default:
			return 0;
	}
}

static void combo_init(zrn_element_t* const e) {
	const auto c = CB(e);
	zrn_items_control_t* const o = &c->base;
	zrn_element_t* const scroll = zrn_create(Z_CLASS_SCROLL_VIEWER);
	zrn_element_t* const host = zrn_create(Z_CLASS_VIRTUALIZING_STACK_PANEL);
	zrn_element_t* popup;
	/* ComboBox: ContentControlFontSize, HorizontalContentAlignment Left, Margin 0 */
	o->base.font_size = ZST_81_FONT_CONTENT;
	c->max_drop_down_height = Z_INFINITE;
	e->focusable = 1;
	if (!scroll || !host) return;
	if (o->host) zrn_destroy(o->host);
	zrn_content_set(ZST_AS(zrn_content_control_t, scroll), host);
	popup = make_drop_down(e, scroll, zrn_theme_color(e, ZST_TC_BASE_WHITE), zrn_theme_color(e, ZST_TC_ACCENT));
	if (!popup) return;
	popup->handlers[Z_EVENT_POPUP_CLOSED - Z_EVENT_CLICK].callback = combo_popup_closed;
	popup->handlers[Z_EVENT_POPUP_CLOSED - Z_EVENT_CLICK].user_data = c;
	c->popup = popup;
	o->scroll = scroll;
	o->host = host;
}

static void combo_destroy(zrn_element_t* const e) {
	free(CB(e)->header);
	free(CB(e)->placeholder);
}

static void combo_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	if (CB(e)->popup == child) {
		CB(e)->popup = nullptr;
		CB(e)->base.scroll = nullptr;
		CB(e)->base.host = nullptr;
		CB(e)->base.presenter = nullptr;
	}
}

zrn_class_t zrn_class_combo_box = {
	.base = &zrn_class_selector, .name = "ComboBox", .id = Z_CLASS_COMBO_BOX, .size = sizeof(zrn_combo_box_t),
	.init = combo_init, .destroy = combo_destroy, .measure = combo_measure, .arrange = combo_arrange, .paint = combo_paint,
	.input = combo_input, .child_removed = combo_child_removed
};

Zid Zcombo_box_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_COMBO_BOX);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_combo_box_t* get_combo(const Zid id) { return CB(zrn_get(id, Z_CLASS_COMBO_BOX)); }

void Zcombo_box_set_is_drop_down_open(const Zid id, const int is_open) {
	zrn_combo_box_t* const c = get_combo(id);
	if (c) combo_set_open(c, is_open);
}

int Zcombo_box_get_is_drop_down_open(const Zid id) {
	const zrn_combo_box_t* const c = get_combo(id);
	return c ? c->is_open : 0;
}

void Zcombo_box_set_max_drop_down_height(const Zid id, const float height) {
	zrn_combo_box_t* const c = get_combo(id);
	if (!c) return;
	c->max_drop_down_height = height > 0 ? height : Z_INFINITE;
	if (c->base.scroll) {
		c->base.scroll->max_height = c->max_drop_down_height;
		zrn_changed_layout(c->base.scroll);
	}
}

float Zcombo_box_get_max_drop_down_height(const Zid id) {
	const zrn_combo_box_t* const c = get_combo(id);
	return c ? c->max_drop_down_height : Z_INFINITE;
}

void Zcombo_box_set_header(const Zid id, const char* const header) {
	zrn_combo_box_t* const c = get_combo(id);
	if (!c) return;
	zrn_set_string(&c->header, header && *header ? header : nullptr);
	zrn_changed_layout(&c->base.base.base);
}

const char* Zcombo_box_get_header(const Zid id) {
	const zrn_combo_box_t* const c = get_combo(id);
	return c && c->header ? c->header : "";
}

void Zcombo_box_set_placeholder_text(const Zid id, const char* const placeholder_text) {
	zrn_combo_box_t* const c = get_combo(id);
	if (!c) return;
	zrn_set_string(&c->placeholder, placeholder_text && *placeholder_text ? placeholder_text : nullptr);
	zrn_changed_layout(&c->base.base.base);
}

const char* Zcombo_box_get_placeholder_text(const Zid id) {
	const zrn_combo_box_t* const c = get_combo(id);
	return c && c->placeholder ? c->placeholder : "";
}


/* ============================================================================
 *  AutoSuggestBox
 * ============================================================================ */

/* the highlight of the suggestion the Up and Down keys are on */
#define HIGHLIGHT Zcolor_argb(0x26000000)

static int asb_text_focused(const zrn_auto_suggest_box_t* const a) {
	const zrn_element_t* e = &a->base.base.base;
	return a->text_box && e->root && e->root->focus == a->text_box;
}

static void asb_set_highlight(zrn_auto_suggest_box_t* const a, const int index) {
	int i, n = zrn_ic_count(&a->base);
	for (i = 0; i < n; i++) Zcontrol_set_background(zrn_ic_item(&a->base, i)->id, i == index ? HIGHLIGHT : Zcolor_rgba(0, 0, 0, 0));
	a->highlight = index;
}

/* @return false when a handler destroyed the box. */
static int asb_set_open(zrn_auto_suggest_box_t* a, int open) {
	const zrn_element_t* const e = &a->base.base.base;
	const Zid id = e->id;
	open = open != 0;
	if (a->is_open == open || !a->popup) return 1;
	a->is_open = open;
	if (open) {
		zrn_element_t* const border = a->popup->children[0];
		style_drop_down(a->popup, zrn_theme_color(e, ZST_TC_BASE_WHITE), zrn_theme_color(e, ZST_TC_ACCENT));
		border->width = e->rect.width;
		border->max_height = a->max_suggestion_list_height;
		/* the list hangs under the box of the text, the margin of the control is not part of it */
		ZST_AS(zrn_popup_t, a->popup)->h_offset = 0;
		ZST_AS(zrn_popup_t, a->popup)->v_offset = 0;
	} else {
		asb_set_highlight(a, -1);
	}
	Zpopup_set_open(a->popup->id, open);
	return zrn_lookup(id) != nullptr;
}

static void asb_popup_closed(const Zid popup, void* const user) {
	const auto a = (zrn_auto_suggest_box_t*)user;
	ZST_UNUSED(popup);
	if (!a->is_open) return;
	a->is_open = 0;
	asb_set_highlight(a, -1);
}

/* A suggestion was chosen: SuggestionChosen, the text, TextChanged with the reason, QuerySubmitted. */
static void asb_choose(zrn_auto_suggest_box_t* const a, const int index) {
	const zrn_element_t* const e = &a->base.base.base;
	const Zid id = e->id;
	zrn_element_t* const item = zrn_ic_item(&a->base, index);
	char* text;
	if (!item) return;
	text = zrn_strdup(zrn_container_text(item));
	if (!text) return;
	if (!asb_set_open(a, 0) || !zrn_lookup(id)) goto done;
	if (a->on_suggestion_chosen) {
		a->on_suggestion_chosen(id, index, a->on_suggestion_chosen_data);
		if (!zrn_lookup(id)) goto done;
	}
	if (a->update_text_on_select) {
		a->reason = Z_SUGGEST_SUGGESTION_CHOSEN;
		a->reason_set = 1;
		Ztext_box_set_text(a->text_box->id, text);
		if (!zrn_lookup(id)) goto done;
		a->reason_set = 0;
	}
	if (a->on_query_submitted) a->on_query_submitted(id, Ztext_box_get_text(a->text_box->id), index, a->on_query_submitted_data);
done:
	free(text);
}

static void asb_suggestion_tapped(const Zid list, const int index, void* const user) {
	ZST_UNUSED(list);
	asb_choose((zrn_auto_suggest_box_t*)user, index);
}

/* TextChanged of the text box inside. */
static void asb_text_changed(const Zid sender, void* const user) {
	const Zid id = (Zid)(uintptr_t)user;
	zrn_element_t* const e = zrn_lookup(id);
	zrn_text_box_t* tb;
	zrn_auto_suggest_box_t* a;
	ZAutoSuggestionBoxTextChangeReason reason;
	if (!e) return;
	a = ASB(e);
	tb = ZST_AS(zrn_text_box_t, zrn_lookup(sender));
	if (!tb) return;
	reason = tb->from_user ? Z_SUGGEST_USER_INPUT : (a->reason_set ? a->reason : Z_SUGGEST_PROGRAMMATIC_CHANGE);
	if (reason == Z_SUGGEST_USER_INPUT) asb_set_highlight(a, -1);
	if (a->on_text_changed) a->on_text_changed(id, reason, a->on_text_changed_data);
}

static void asb_lost_focus(const Zid sender, void* const user) {
	zrn_element_t* const e = zrn_lookup((Zid)(uintptr_t)user), *f;
	ZST_UNUSED(sender);
	if (!e) return;
	/* the keyboard goes to where the finger is only for a moment, and to the box itself: the list stays */
	for (f = e->root ? e->root->focus : nullptr; f; f = f->parent) if (f == e) return;
	asb_set_open(ASB(e), 0);
}

/* The keys the text box leaves to the suggestions. */
static int asb_key(zrn_text_box_t* const b, zrn_input_t* const in) {
	zrn_element_t* const e = b->base.base.parent;
	zrn_auto_suggest_box_t* a;
	Zid id;
	int n;
	if (!e || !zrn_is_a(e, Z_CLASS_AUTO_SUGGEST_BOX)) return 0;
	a = ASB(e);
	id = e->id;
	n = zrn_ic_count(&a->base);
	switch (in->key) {
		case Z_KEY_DOWN:
		case Z_KEY_UP:
			if (!a->is_open || n == 0) return 0;
			{
				int next = a->highlight + (in->key == Z_KEY_DOWN ? 1 : -1);
				if (next < 0) next = n - 1;
				if (next >= n) next = 0;
				asb_set_highlight(a, next);
			}
			return 1;
		case Z_KEY_ENTER:
			if (a->is_open && a->highlight >= 0) {
				asb_choose(a, a->highlight);
				return 1;
			}
			if (a->is_open && !asb_set_open(a, 0)) return 1;
			if (zrn_lookup(id) && a->on_query_submitted) a->on_query_submitted(id, Ztext_box_get_text(a->text_box->id), -1, a->on_query_submitted_data);
			return 1;
		case Z_KEY_ESCAPE:
			if (!a->is_open) return 0;
			asb_set_open(a, 0);
			return 1;
		default:
			return 0;
	}
}

static ZSize asb_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_auto_suggest_box_t* const a = ASB(e);
	if (!a->text_box) return zrn_size(0, 0);
	zrn_measure(a->text_box, available);
	return a->text_box->desired;
}

static ZSize asb_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_auto_suggest_box_t* const a = ASB(e);
	if (a->text_box) zrn_arrange(a->text_box, zrn_rect(0, 0, final.width, final.height));
	if (a->popup && a->popup->child_count && a->popup->children[0]->width != final.width) {
		a->popup->children[0]->width = final.width;
		zrn_changed_layout(a->popup->children[0]);
	}
	return final;
}

static int asb_input(zrn_element_t* const e, zrn_input_t* const in) {
	const zrn_auto_suggest_box_t* const a = ASB(e);
	/* the box itself takes the keyboard for its text box */
	if (in->type == ZST_IN_FOCUS_IN && a->text_box && e->root) zrn_set_focus(e->root, a->text_box);
	return 0;
}

static void asb_init(zrn_element_t* const e) {
	zrn_auto_suggest_box_t* const a = ASB(e);
	zrn_items_control_t* const o = &a->base;
	zrn_element_t* const tb = zrn_create(Z_CLASS_TEXT_BOX);
	zrn_element_t* const list = zrn_create(Z_CLASS_LIST_VIEW);
	zrn_element_t* popup;
	/* the style: Margin TextControlMarginThemeThickness 0,9.5; the TextBox inside has none; MaxSuggestionListHeight is not given */
	e->margin = Zthickness(0, ZST_81_TOUCH_OVERHANG, 0, ZST_81_TOUCH_OVERHANG);
	e->valign = Z_VALIGN_TOP;
	e->focusable = 1;
	e->tab_stop = 0;
	a->update_text_on_select = 1;
	a->max_suggestion_list_height = 300.0f;
	a->highlight = -1;
	if (o->host) zrn_destroy(o->host);
	o->host = nullptr;
	if (!tb || !list) return;
	zrn_add_child(e, tb, -1);
	a->text_box = tb;
	tb->margin = zrn_thickness_zero();
	ZST_AS(zrn_text_box_t, tb)->key_hook = asb_key;
	tb->handlers[Z_EVENT_TEXT_CHANGED - Z_EVENT_CLICK].callback = asb_text_changed;
	tb->handlers[Z_EVENT_TEXT_CHANGED - Z_EVENT_CLICK].user_data = (void*)(uintptr_t)e->id;
	tb->handlers[Z_EVENT_LOST_FOCUS - Z_EVENT_CLICK].callback = asb_lost_focus;
	tb->handlers[Z_EVENT_LOST_FOCUS - Z_EVENT_CLICK].user_data = (void*)(uintptr_t)e->id;
	/* the list of suggestions: a tap is an ItemClick, it never takes the keyboard from the text */
	list->focusable = 0;
	ZST_AS(zrn_list_view_base_t, list)->item_click_enabled = 1;
	ZST_AS(zrn_list_view_base_t, list)->on_item_click = asb_suggestion_tapped;
	ZST_AS(zrn_list_view_base_t, list)->on_item_click_data = a;
	zrn_ic_set_sel_mode(IC(list), ZST_SEL_NONE);
	popup = make_drop_down(e, list, zrn_theme_color(e, ZST_TC_BASE_WHITE), zrn_theme_color(e, ZST_TC_ACCENT));
	if (!popup) return;
	popup->handlers[Z_EVENT_POPUP_CLOSED - Z_EVENT_CLICK].callback = asb_popup_closed;
	popup->handlers[Z_EVENT_POPUP_CLOSED - Z_EVENT_CLICK].user_data = a;
	a->popup = popup;
	a->list = list;
	o->scroll = IC(list)->scroll;
	o->presenter = IC(list)->presenter;
	o->host = IC(list)->host;
}

static void asb_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	zrn_auto_suggest_box_t* const a = ASB(e);
	if (a->text_box == child) a->text_box = nullptr;
	if (a->popup == child) {
		a->popup = nullptr;
		a->list = nullptr;
		a->base.scroll = nullptr;
		a->base.presenter = nullptr;
		a->base.host = nullptr;
	}
}

zrn_class_t zrn_class_auto_suggest_box = {
	.base = &zrn_class_items_control, .name = "AutoSuggestBox", .id = Z_CLASS_AUTO_SUGGEST_BOX, .size = sizeof(zrn_auto_suggest_box_t),
	.init = asb_init, .measure = asb_measure, .arrange = asb_arrange, .input = asb_input, .child_removed = asb_child_removed
};

Zid Zauto_suggest_box_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_AUTO_SUGGEST_BOX);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_auto_suggest_box_t* get_asb(const Zid id) { return ASB(zrn_get(id, Z_CLASS_AUTO_SUGGEST_BOX)); }

void Zauto_suggest_box_set_items(const Zid id, const char* const* items, const int count) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	int i;
	if (!a || !a->base.host) return;
	zrn_clear_children(a->base.host);
	a->highlight = -1;
	for (i = 0; items && i < count; i++) zrn_ic_add(&a->base, nullptr, items[i] ? items[i] : "");
	/* the suggestions of what the user typed show under the box he types in */
	asb_set_open(a, zrn_ic_count(&a->base) > 0 && asb_text_focused(a));
}

void Zauto_suggest_box_set_text(const Zid id, const char* const text) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	if (a && a->text_box) Ztext_box_set_text(a->text_box->id, text);
}

const char* Zauto_suggest_box_get_text(const Zid id) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	return a && a->text_box ? Ztext_box_get_text(a->text_box->id) : "";
}

void Zauto_suggest_box_set_placeholder_text(const Zid id, const char* const placeholder_text) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	if (a && a->text_box) Ztext_box_set_placeholder_text(a->text_box->id, placeholder_text);
}

const char* Zauto_suggest_box_get_placeholder_text(const Zid id) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	return a && a->text_box ? Ztext_box_get_placeholder_text(a->text_box->id) : "";
}

void Zauto_suggest_box_set_header(const Zid id, const char* const header) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	if (a && a->text_box) Ztext_box_set_header(a->text_box->id, header);
}

const char* Zauto_suggest_box_get_header(const Zid id) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	return a && a->text_box ? Ztext_box_get_header(a->text_box->id) : "";
}

void Zauto_suggest_box_set_is_suggestion_list_open(const Zid id, const int is_open) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (a) asb_set_open(a, is_open && zrn_ic_count(&a->base) > 0);
}

int Zauto_suggest_box_get_is_suggestion_list_open(const Zid id) {
	const zrn_auto_suggest_box_t* const a = get_asb(id);
	return a ? a->is_open : 0;
}

void Zauto_suggest_box_set_max_suggestion_list_height(const Zid id, const float height) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (!a) return;
	a->max_suggestion_list_height = height > 0 ? height : Z_INFINITE;
	if (a->popup && a->popup->child_count) {
		a->popup->children[0]->max_height = a->max_suggestion_list_height;
		zrn_changed_layout(a->popup->children[0]);
	}
}

void Zauto_suggest_box_set_update_text_on_select(const Zid id, const int update) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (a) a->update_text_on_select = update != 0;
}

void Zauto_suggest_box_on_text_changed(const Zid id, const ZAutoSuggestTextChangedCallback callback, void* const user_data) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (!a) return;
	a->on_text_changed = callback;
	a->on_text_changed_data = user_data;
}

void Zauto_suggest_box_on_suggestion_chosen(const Zid id, const ZAutoSuggestSuggestionChosenCallback callback, void* const user_data) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (!a) return;
	a->on_suggestion_chosen = callback;
	a->on_suggestion_chosen_data = user_data;
}

void Zauto_suggest_box_on_query_submitted(const Zid id, const ZAutoSuggestQuerySubmittedCallback callback, void* const user_data) {
	zrn_auto_suggest_box_t* const a = get_asb(id);
	if (!a) return;
	a->on_query_submitted = callback;
	a->on_query_submitted_data = user_data;
}
