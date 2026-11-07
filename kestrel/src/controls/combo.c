/**
 * Kestrel - controls/combo.c
 * ComboBox and AutoSuggestBox, the two controls whose list is a Popup.
 *
 * ComboBox follows the inline template of generic.xaml (the one for a short list): a header, then a
 * box with a 2.5 px outline that holds the selected item, with PhoneTouchTargetOverhang 0,9.5 above and
 * below. A tap opens the list over the box, white with an accent border. AutoSuggestBox is a TextBox
 * and a Popup with a ListView of suggestions.
 */

#include "../internal.h"

#define IC(e) KST_AS(kst_items_control_t, e)
#define CB(e) KST_AS(kst_combo_box_t, e)
#define ASB(e) KST_AS(kst_auto_suggest_box_t, e)

/* ComboBox: ComboBoxBorderThemeThickness 2.5, ComboBoxItemMinHeightThemeSize 33.5, Padding 6.5 of its items, Header margin 0,0,0,-4.5 */
#define COMBO_BORDER 2.5f
#define COMBO_MIN_HEIGHT 33.5f
#define COMBO_TEXT_LEFT 6.5f
#define COMBO_HEADER_OVERLAP 4.5f
#define COMBO_OVERHANG 9.5f
#define HEADER_FONT 16.0f

static kst_font_t header_font(void) { kst_font_t f = { K_FONT_NORMAL, HEADER_FONT, 0 }; return f; }

/* The popups: a Border of the highlighted look around a ScrollViewer or a ListView. */
static kst_element_t* make_drop_down(kst_element_t* owner, kst_element_t* content, KColor background, KColor border_brush) {
	kst_element_t* border = kst_create(K_CLASS_BORDER);
	kst_element_t* popup = kst_create(K_CLASS_POPUP);
	kst_border_t* b;
	if (!border || !popup) {
		if (border) kst_destroy(border);
		if (popup) kst_destroy(popup);
		return NULL;
	}
	b = KST_AS(kst_border_t, border);
	b->background = background;
	b->has_background = 1;
	b->border_brush = border_brush;
	b->has_border_brush = 1;
	b->border_thickness = Kthickness_uniform(COMBO_BORDER);
	kst_add_child(border, content, -1);
	kst_add_child(popup, border, -1);
	border->is_popup_child = 1;
	kst_add_child(owner, popup, -1);
	KST_AS(kst_popup_t, popup)->target = owner->id;
	return popup;
}

/* The popups follow the theme of the moment they open. */
static void style_drop_down(kst_element_t* popup, KColor background, KColor border_brush) {
	kst_border_t* b = KST_AS(kst_border_t, popup->children[0]);
	b->background = background;
	b->border_brush = border_brush;
}


/* ============================================================================
 *  ComboBox
 * ============================================================================ */

static float combo_header_height(const kst_combo_box_t* c) {
	return c->header && *c->header ? kst_font_line_height(header_font()) - COMBO_HEADER_OVERLAP : 0;
}

static KRect combo_box_rect(const kst_element_t* e) {
	float top = combo_header_height(CB(e)) + COMBO_OVERHANG;
	return kst_rect(0, top, e->rect.width, kst_maxf(0, e->rect.height - top - COMBO_OVERHANG));
}

static const char* combo_text(const kst_combo_box_t* c) {
	kst_element_t* item = c->base.selected_index >= 0 ? kst_ic_item(&c->base, c->base.selected_index) : NULL;
	return item ? kst_container_text(item) : "";
}

static KSize combo_measure(kst_element_t* e, KSize available) {
	kst_combo_box_t* c = CB(e);
	kst_font_t font = { c->base.base.font_family, c->base.base.font_size, 0 };
	float w = c->placeholder ? kst_text_width(font, c->placeholder, (int)strlen(c->placeholder)) : 0;
	int i, n = kst_ic_count(&c->base);
	KST_UNUSED(available);
	for (i = 0; i < n; i++) {
		const char* t = kst_container_text(kst_ic_item(&c->base, i));
		w = kst_maxf(w, kst_text_width(font, t, (int)strlen(t)));
	}
	return kst_size(w + COMBO_TEXT_LEFT + 2 * COMBO_BORDER, combo_header_height(c) + 2 * COMBO_OVERHANG + COMBO_MIN_HEIGHT + 2 * COMBO_BORDER);
}

static KSize combo_arrange(kst_element_t* e, KSize final) {
	kst_combo_box_t* c = CB(e);
	/* the list is laid out by the root, but it is as wide as the box */
	if (c->popup && c->popup->child_count && c->popup->children[0]->width != final.width) {
		c->popup->children[0]->width = final.width;
		kst_changed_layout(c->popup->children[0]);
	}
	return final;
}

static void combo_paint(kst_element_t* e, kst_painter_t* p) {
	kst_combo_box_t* c = CB(e);
	kst_control_t* ctl = &c->base.base;
	kst_font_t font = { ctl->font_family, ctl->font_size, 0 };
	KRect box = combo_box_rect(e);
	KColor fg = kst_ctl_color(ctl, KST_C_FOREGROUND, ctl->foreground, kst_theme_color(e, KST_TC_FOREGROUND));
	KColor border = kst_ctl_border_brush(ctl, kst_theme_color(e, KST_TC_FOREGROUND));
	KColor background = kst_ctl_background(ctl, Kcolor_rgba(0, 0, 0, 0));
	int enabled = kst_effectively_enabled(e), has_selection = c->base.selected_index >= 0;
	const char* text = has_selection ? combo_text(c) : (c->placeholder ? c->placeholder : "");
	float tx = box.x + COMBO_BORDER + COMBO_TEXT_LEFT;
	if (c->header && *c->header) {
		kst_draw_string(p, header_font(), c->header, (int)strlen(c->header), 0, 0, kst_theme_color(e, enabled ? KST_TC_BASE_MID : KST_TC_BASE_LOW));
	}
	if (!enabled) {
		fg = border = kst_theme_color(e, KST_TC_BASE_LOW);
		background = Kcolor_rgba(0, 0, 0, 0);
	} else if (c->is_open) {
		/* Highlighted: ComboBoxHighlightedBackground white, the accent border, black text */
		background = kst_theme_color(e, KST_TC_BASE_WHITE);
		border = kst_theme_color(e, KST_TC_ACCENT);
		fg = kst_theme_color(e, KST_TC_BASE_BLACK);
	} else if (c->pressed) {
		background = kst_theme_color(e, KST_TC_ACCENT);
		border = kst_theme_color(e, KST_TC_FOREGROUND);
		fg = kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT);
	}
	if (!has_selection) {
		/* ComboBoxFlyoutListPlaceholderTextOpacity: 0.6, 0.45 in the Light theme */
		fg = kst_color_alpha(fg, e->root && e->root->theme.theme == K_THEME_LIGHT ? 0.45f : 0.6f);
	}
	if (background.a > 0) {
		KThickness t = Kthickness_uniform(COMBO_BORDER);
		kst_fill_rect(p, kst_rect(box.x + t.left, box.y + t.top, box.width - kst_thickness_h(t), box.height - kst_thickness_v(t)), background);
	}
	kst_stroke_rect(p, box, Kthickness_uniform(COMBO_BORDER), border);
	kst_push_clip(p, kst_rect(box.x + COMBO_BORDER, box.y + COMBO_BORDER, kst_maxf(0, box.width - 2 * COMBO_BORDER), kst_maxf(0, box.height - 2 * COMBO_BORDER)));
	kst_draw_string(p, font, text, (int)strlen(text), tx, box.y + (box.height - kst_font_line_height(font)) * 0.5f, fg);
	kst_pop_clip(p);
}

/* @return K_FALSE when a handler destroyed the box. */
static int combo_set_open(kst_combo_box_t* c, int open) {
	kst_element_t* e = &c->base.base.base;
	Kid id = e->id;
	open = open != 0;
	if (c->is_open == open || !c->popup) return 1;
	c->is_open = open;
	if (open) {
		kst_element_t* border = c->popup->children[0];
		kst_popup_t* popup = KST_AS(kst_popup_t, c->popup);
		float above = 0;
		int i;
		style_drop_down(c->popup, kst_theme_color(e, KST_TC_BASE_WHITE), kst_theme_color(e, KST_TC_ACCENT));
		border->width = e->rect.width;
		if (c->base.scroll) c->base.scroll->max_height = c->max_drop_down_height;
		/* the list opens over the box with the selected item where the box shows it */
		if (c->base.host && c->base.selected_index > 0) {
			kst_measure(c->base.host, kst_size(kst_maxf(0, e->rect.width - 2 * COMBO_BORDER), K_INFINITE));
			for (i = 0; i < c->base.selected_index; i++) above += kst_ic_item(&c->base, i)->desired.height;
		}
		popup->placement = K_PLACE_BOTTOM;
		popup->h_offset = 0;
		popup->v_offset = combo_box_rect(e).y - e->rect.height - above - 5.0f;
		Kpopup_set_open(c->popup->id, 1);
		if (!kst_raise(e, K_EVENT_DROP_DOWN_OPENED)) return 0;
	} else {
		Kpopup_set_open(c->popup->id, 0);
		if (!kst_raise(e, K_EVENT_DROP_DOWN_CLOSED)) return 0;
	}
	if (kst_lookup(id)) kst_changed_paint(e);
	return kst_lookup(id) != NULL;
}

static void combo_popup_closed(Kid popup, void* user) {
	kst_combo_box_t* c = (kst_combo_box_t*)user;
	KST_UNUSED(popup);
	if (!c->is_open) return;
	c->is_open = 0;
	if (kst_raise(&c->base.base.base, K_EVENT_DROP_DOWN_CLOSED)) kst_changed_paint(&c->base.base.base);
}

void kst_combo_item_tapped(kst_items_control_t* o, int index) {
	kst_combo_box_t* c = (kst_combo_box_t*)o;
	Kid id = o->base.base.id;
	/* a tap on an item chooses it and closes the list */
	kst_ic_select(o, index);
	if (kst_lookup(id)) combo_set_open(c, 0);
}

static int combo_input(kst_element_t* e, kst_input_t* in) {
	kst_combo_box_t* c = CB(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			if (!kst_effectively_enabled(e)) return 1;
			c->pressed = 1;
			kst_changed_paint(e);
			if (e->root) kst_set_capture(e->root, e);
			return 1;
		case KST_IN_POINTER_UP: {
			int tap = c->pressed && in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			c->pressed = 0;
			kst_changed_paint(e);
			if (e->root) kst_release_capture(e->root, e);
			if (tap) combo_set_open(c, !c->is_open);
			return 1;
		}
		case KST_IN_CAPTURE_LOST:
			c->pressed = 0;
			kst_changed_paint(e);
			return 0;
		case KST_IN_KEY_DOWN:
			if (in->key == K_KEY_ENTER || in->key == K_KEY_SPACE) { combo_set_open(c, !c->is_open); return 1; }
			if (in->key == K_KEY_ESCAPE && c->is_open) { combo_set_open(c, 0); return 1; }
			if (in->key == K_KEY_DOWN) { if (c->base.selected_index + 1 < kst_ic_count(&c->base)) kst_ic_select(&c->base, c->base.selected_index + 1); return 1; }
			if (in->key == K_KEY_UP) { if (c->base.selected_index > 0) kst_ic_select(&c->base, c->base.selected_index - 1); return 1; }
			return 0;
		case KST_IN_FOCUS_IN:
		case KST_IN_FOCUS_OUT:
			kst_changed_paint(e);
			return 0;
		default:
			return 0;
	}
}

static void combo_init(kst_element_t* e) {
	kst_combo_box_t* c = CB(e);
	kst_items_control_t* o = &c->base;
	kst_element_t* scroll = kst_create(K_CLASS_SCROLL_VIEWER);
	kst_element_t* host = kst_create(K_CLASS_VIRTUALIZING_STACK_PANEL);
	kst_element_t* popup;
	/* ComboBox: ContentControlFontSize, HorizontalContentAlignment Left, Margin 0 */
	o->base.font_size = KST_81_FONT_CONTENT;
	c->max_drop_down_height = K_INFINITE;
	e->focusable = 1;
	if (!scroll || !host) return;
	if (o->host) kst_destroy(o->host);
	kst_content_set(KST_AS(kst_content_control_t, scroll), host);
	popup = make_drop_down(e, scroll, kst_theme_color(e, KST_TC_BASE_WHITE), kst_theme_color(e, KST_TC_ACCENT));
	if (!popup) return;
	popup->handlers[K_EVENT_POPUP_CLOSED - K_EVENT_CLICK].callback = combo_popup_closed;
	popup->handlers[K_EVENT_POPUP_CLOSED - K_EVENT_CLICK].user_data = c;
	c->popup = popup;
	o->scroll = scroll;
	o->host = host;
}

static void combo_destroy(kst_element_t* e) {
	free(CB(e)->header);
	free(CB(e)->placeholder);
}

static void combo_child_removed(kst_element_t* e, kst_element_t* child) {
	if (CB(e)->popup == child) {
		CB(e)->popup = NULL;
		CB(e)->base.scroll = NULL;
		CB(e)->base.host = NULL;
		CB(e)->base.presenter = NULL;
	}
}

kst_class_t kst_class_combo_box = {
	.base = &kst_class_selector, .name = "ComboBox", .id = K_CLASS_COMBO_BOX, .size = sizeof(kst_combo_box_t),
	.init = combo_init, .destroy = combo_destroy, .measure = combo_measure, .arrange = combo_arrange, .paint = combo_paint,
	.input = combo_input, .child_removed = combo_child_removed
};

Kid Kcombo_box_create(void) {
	kst_element_t* e = kst_create(K_CLASS_COMBO_BOX);
	return e ? e->id : K_INVALID_ID;
}

static kst_combo_box_t* get_combo(Kid id) { return CB(kst_get(id, K_CLASS_COMBO_BOX)); }

void Kcombo_box_set_is_drop_down_open(Kid id, int is_open) {
	kst_combo_box_t* c = get_combo(id);
	if (c) combo_set_open(c, is_open);
}

int Kcombo_box_get_is_drop_down_open(Kid id) {
	kst_combo_box_t* c = get_combo(id);
	return c ? c->is_open : 0;
}

void Kcombo_box_set_max_drop_down_height(Kid id, float height) {
	kst_combo_box_t* c = get_combo(id);
	if (!c) return;
	c->max_drop_down_height = height > 0 ? height : K_INFINITE;
	if (c->base.scroll) {
		c->base.scroll->max_height = c->max_drop_down_height;
		kst_changed_layout(c->base.scroll);
	}
}

float Kcombo_box_get_max_drop_down_height(Kid id) {
	kst_combo_box_t* c = get_combo(id);
	return c ? c->max_drop_down_height : K_INFINITE;
}

void Kcombo_box_set_header(Kid id, const char* header) {
	kst_combo_box_t* c = get_combo(id);
	if (!c) return;
	kst_set_string(&c->header, header && *header ? header : NULL);
	kst_changed_layout(&c->base.base.base);
}

const char* Kcombo_box_get_header(Kid id) {
	kst_combo_box_t* c = get_combo(id);
	return c && c->header ? c->header : "";
}

void Kcombo_box_set_placeholder_text(Kid id, const char* placeholder_text) {
	kst_combo_box_t* c = get_combo(id);
	if (!c) return;
	kst_set_string(&c->placeholder, placeholder_text && *placeholder_text ? placeholder_text : NULL);
	kst_changed_layout(&c->base.base.base);
}

const char* Kcombo_box_get_placeholder_text(Kid id) {
	kst_combo_box_t* c = get_combo(id);
	return c && c->placeholder ? c->placeholder : "";
}


/* ============================================================================
 *  AutoSuggestBox
 * ============================================================================ */

/* the highlight of the suggestion the Up and Down keys are on */
#define HIGHLIGHT Kcolor_argb(0x26000000)

static int asb_text_focused(const kst_auto_suggest_box_t* a) {
	const kst_element_t* e = &a->base.base.base;
	return a->text_box && e->root && e->root->focus == a->text_box;
}

static void asb_set_highlight(kst_auto_suggest_box_t* a, int index) {
	int i, n = kst_ic_count(&a->base);
	for (i = 0; i < n; i++) Kcontrol_set_background(kst_ic_item(&a->base, i)->id, i == index ? HIGHLIGHT : Kcolor_rgba(0, 0, 0, 0));
	a->highlight = index;
}

/* @return K_FALSE when a handler destroyed the box. */
static int asb_set_open(kst_auto_suggest_box_t* a, int open) {
	kst_element_t* e = &a->base.base.base;
	Kid id = e->id;
	open = open != 0;
	if (a->is_open == open || !a->popup) return 1;
	a->is_open = open;
	if (open) {
		kst_element_t* border = a->popup->children[0];
		style_drop_down(a->popup, kst_theme_color(e, KST_TC_BASE_WHITE), kst_theme_color(e, KST_TC_ACCENT));
		border->width = e->rect.width;
		border->max_height = a->max_suggestion_list_height;
		/* the list hangs under the box of the text, the margin of the control is not part of it */
		KST_AS(kst_popup_t, a->popup)->h_offset = 0;
		KST_AS(kst_popup_t, a->popup)->v_offset = 0;
	} else {
		asb_set_highlight(a, -1);
	}
	Kpopup_set_open(a->popup->id, open);
	return kst_lookup(id) != NULL;
}

static void asb_popup_closed(Kid popup, void* user) {
	kst_auto_suggest_box_t* a = (kst_auto_suggest_box_t*)user;
	KST_UNUSED(popup);
	if (!a->is_open) return;
	a->is_open = 0;
	asb_set_highlight(a, -1);
}

/* A suggestion was chosen: SuggestionChosen, the text, TextChanged with the reason, QuerySubmitted. */
static void asb_choose(kst_auto_suggest_box_t* a, int index) {
	kst_element_t* e = &a->base.base.base;
	Kid id = e->id;
	kst_element_t* item = kst_ic_item(&a->base, index);
	char* text;
	if (!item) return;
	text = kst_strdup(kst_container_text(item));
	if (!text) return;
	if (!asb_set_open(a, 0) || !kst_lookup(id)) goto done;
	if (a->on_suggestion_chosen) {
		a->on_suggestion_chosen(id, index, a->on_suggestion_chosen_data);
		if (!kst_lookup(id)) goto done;
	}
	if (a->update_text_on_select) {
		a->reason = K_SUGGEST_SUGGESTION_CHOSEN;
		a->reason_set = 1;
		Ktext_box_set_text(a->text_box->id, text);
		if (!kst_lookup(id)) goto done;
		a->reason_set = 0;
	}
	if (a->on_query_submitted) a->on_query_submitted(id, Ktext_box_get_text(a->text_box->id), index, a->on_query_submitted_data);
done:
	free(text);
}

static void asb_suggestion_tapped(Kid list, int index, void* user) {
	KST_UNUSED(list);
	asb_choose((kst_auto_suggest_box_t*)user, index);
}

/* TextChanged of the text box inside. */
static void asb_text_changed(Kid sender, void* user) {
	Kid id = (Kid)(uintptr_t)user;
	kst_element_t* e = kst_lookup(id);
	kst_text_box_t* tb;
	kst_auto_suggest_box_t* a;
	KAutoSuggestionBoxTextChangeReason reason;
	if (!e) return;
	a = ASB(e);
	tb = KST_AS(kst_text_box_t, kst_lookup(sender));
	if (!tb) return;
	reason = tb->from_user ? K_SUGGEST_USER_INPUT : (a->reason_set ? a->reason : K_SUGGEST_PROGRAMMATIC_CHANGE);
	if (reason == K_SUGGEST_USER_INPUT) asb_set_highlight(a, -1);
	if (a->on_text_changed) a->on_text_changed(id, reason, a->on_text_changed_data);
}

static void asb_lost_focus(Kid sender, void* user) {
	kst_element_t* e = kst_lookup((Kid)(uintptr_t)user), *f;
	KST_UNUSED(sender);
	if (!e) return;
	/* the keyboard goes to where the finger is only for a moment, and to the box itself: the list stays */
	for (f = e->root ? e->root->focus : NULL; f; f = f->parent) if (f == e) return;
	asb_set_open(ASB(e), 0);
}

/* The keys the text box leaves to the suggestions. */
static int asb_key(kst_text_box_t* b, kst_input_t* in) {
	kst_element_t* e = b->base.base.parent;
	kst_auto_suggest_box_t* a;
	Kid id;
	int n;
	if (!e || !kst_is_a(e, K_CLASS_AUTO_SUGGEST_BOX)) return 0;
	a = ASB(e);
	id = e->id;
	n = kst_ic_count(&a->base);
	switch (in->key) {
		case K_KEY_DOWN:
		case K_KEY_UP:
			if (!a->is_open || n == 0) return 0;
			{
				int next = a->highlight + (in->key == K_KEY_DOWN ? 1 : -1);
				if (next < 0) next = n - 1;
				if (next >= n) next = 0;
				asb_set_highlight(a, next);
			}
			return 1;
		case K_KEY_ENTER:
			if (a->is_open && a->highlight >= 0) {
				asb_choose(a, a->highlight);
				return 1;
			}
			if (a->is_open && !asb_set_open(a, 0)) return 1;
			if (kst_lookup(id) && a->on_query_submitted) a->on_query_submitted(id, Ktext_box_get_text(a->text_box->id), -1, a->on_query_submitted_data);
			return 1;
		case K_KEY_ESCAPE:
			if (!a->is_open) return 0;
			asb_set_open(a, 0);
			return 1;
		default:
			return 0;
	}
}

static KSize asb_measure(kst_element_t* e, KSize available) {
	kst_auto_suggest_box_t* a = ASB(e);
	if (!a->text_box) return kst_size(0, 0);
	kst_measure(a->text_box, available);
	return a->text_box->desired;
}

static KSize asb_arrange(kst_element_t* e, KSize final) {
	kst_auto_suggest_box_t* a = ASB(e);
	if (a->text_box) kst_arrange(a->text_box, kst_rect(0, 0, final.width, final.height));
	if (a->popup && a->popup->child_count && a->popup->children[0]->width != final.width) {
		a->popup->children[0]->width = final.width;
		kst_changed_layout(a->popup->children[0]);
	}
	return final;
}

static int asb_input(kst_element_t* e, kst_input_t* in) {
	kst_auto_suggest_box_t* a = ASB(e);
	/* the box itself takes the keyboard for its text box */
	if (in->type == KST_IN_FOCUS_IN && a->text_box && e->root) kst_set_focus(e->root, a->text_box);
	return 0;
}

static void asb_init(kst_element_t* e) {
	kst_auto_suggest_box_t* a = ASB(e);
	kst_items_control_t* o = &a->base;
	kst_element_t* tb = kst_create(K_CLASS_TEXT_BOX);
	kst_element_t* list = kst_create(K_CLASS_LIST_VIEW);
	kst_element_t* popup;
	/* the style: Margin TextControlMarginThemeThickness 0,9.5; the TextBox inside has none; MaxSuggestionListHeight is not given */
	e->margin = Kthickness(0, KST_81_TOUCH_OVERHANG, 0, KST_81_TOUCH_OVERHANG);
	e->valign = K_VALIGN_TOP;
	e->focusable = 1;
	e->tab_stop = 0;
	a->update_text_on_select = 1;
	a->max_suggestion_list_height = 300.0f;
	a->highlight = -1;
	if (o->host) kst_destroy(o->host);
	o->host = NULL;
	if (!tb || !list) return;
	kst_add_child(e, tb, -1);
	a->text_box = tb;
	tb->margin = kst_thickness_zero();
	KST_AS(kst_text_box_t, tb)->key_hook = asb_key;
	tb->handlers[K_EVENT_TEXT_CHANGED - K_EVENT_CLICK].callback = asb_text_changed;
	tb->handlers[K_EVENT_TEXT_CHANGED - K_EVENT_CLICK].user_data = (void*)(uintptr_t)e->id;
	tb->handlers[K_EVENT_LOST_FOCUS - K_EVENT_CLICK].callback = asb_lost_focus;
	tb->handlers[K_EVENT_LOST_FOCUS - K_EVENT_CLICK].user_data = (void*)(uintptr_t)e->id;
	/* the list of suggestions: a tap is an ItemClick, it never takes the keyboard from the text */
	list->focusable = 0;
	KST_AS(kst_list_view_base_t, list)->item_click_enabled = 1;
	KST_AS(kst_list_view_base_t, list)->on_item_click = asb_suggestion_tapped;
	KST_AS(kst_list_view_base_t, list)->on_item_click_data = a;
	kst_ic_set_sel_mode(IC(list), KST_SEL_NONE);
	popup = make_drop_down(e, list, kst_theme_color(e, KST_TC_BASE_WHITE), kst_theme_color(e, KST_TC_ACCENT));
	if (!popup) return;
	popup->handlers[K_EVENT_POPUP_CLOSED - K_EVENT_CLICK].callback = asb_popup_closed;
	popup->handlers[K_EVENT_POPUP_CLOSED - K_EVENT_CLICK].user_data = a;
	a->popup = popup;
	a->list = list;
	o->scroll = IC(list)->scroll;
	o->presenter = IC(list)->presenter;
	o->host = IC(list)->host;
}

static void asb_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_auto_suggest_box_t* a = ASB(e);
	if (a->text_box == child) a->text_box = NULL;
	if (a->popup == child) {
		a->popup = NULL;
		a->list = NULL;
		a->base.scroll = NULL;
		a->base.presenter = NULL;
		a->base.host = NULL;
	}
}

kst_class_t kst_class_auto_suggest_box = {
	.base = &kst_class_items_control, .name = "AutoSuggestBox", .id = K_CLASS_AUTO_SUGGEST_BOX, .size = sizeof(kst_auto_suggest_box_t),
	.init = asb_init, .measure = asb_measure, .arrange = asb_arrange, .input = asb_input, .child_removed = asb_child_removed
};

Kid Kauto_suggest_box_create(void) {
	kst_element_t* e = kst_create(K_CLASS_AUTO_SUGGEST_BOX);
	return e ? e->id : K_INVALID_ID;
}

static kst_auto_suggest_box_t* get_asb(Kid id) { return ASB(kst_get(id, K_CLASS_AUTO_SUGGEST_BOX)); }

void Kauto_suggest_box_set_items(Kid id, const char* const* items, int count) {
	kst_auto_suggest_box_t* a = get_asb(id);
	int i;
	if (!a || !a->base.host) return;
	kst_clear_children(a->base.host);
	a->highlight = -1;
	for (i = 0; items && i < count; i++) kst_ic_add(&a->base, NULL, items[i] ? items[i] : "");
	/* the suggestions of what the user typed show under the box he types in */
	asb_set_open(a, kst_ic_count(&a->base) > 0 && asb_text_focused(a));
}

void Kauto_suggest_box_set_text(Kid id, const char* text) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (a && a->text_box) Ktext_box_set_text(a->text_box->id, text);
}

const char* Kauto_suggest_box_get_text(Kid id) {
	kst_auto_suggest_box_t* a = get_asb(id);
	return a && a->text_box ? Ktext_box_get_text(a->text_box->id) : "";
}

void Kauto_suggest_box_set_placeholder_text(Kid id, const char* placeholder_text) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (a && a->text_box) Ktext_box_set_placeholder_text(a->text_box->id, placeholder_text);
}

const char* Kauto_suggest_box_get_placeholder_text(Kid id) {
	kst_auto_suggest_box_t* a = get_asb(id);
	return a && a->text_box ? Ktext_box_get_placeholder_text(a->text_box->id) : "";
}

void Kauto_suggest_box_set_header(Kid id, const char* header) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (a && a->text_box) Ktext_box_set_header(a->text_box->id, header);
}

const char* Kauto_suggest_box_get_header(Kid id) {
	kst_auto_suggest_box_t* a = get_asb(id);
	return a && a->text_box ? Ktext_box_get_header(a->text_box->id) : "";
}

void Kauto_suggest_box_set_is_suggestion_list_open(Kid id, int is_open) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (a) asb_set_open(a, is_open && kst_ic_count(&a->base) > 0);
}

int Kauto_suggest_box_get_is_suggestion_list_open(Kid id) {
	kst_auto_suggest_box_t* a = get_asb(id);
	return a ? a->is_open : 0;
}

void Kauto_suggest_box_set_max_suggestion_list_height(Kid id, float height) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (!a) return;
	a->max_suggestion_list_height = height > 0 ? height : K_INFINITE;
	if (a->popup && a->popup->child_count) {
		a->popup->children[0]->max_height = a->max_suggestion_list_height;
		kst_changed_layout(a->popup->children[0]);
	}
}

void Kauto_suggest_box_set_update_text_on_select(Kid id, int update) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (a) a->update_text_on_select = update != 0;
}

void Kauto_suggest_box_on_text_changed(Kid id, KAutoSuggestTextChangedCallback callback, void* user_data) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (!a) return;
	a->on_text_changed = callback;
	a->on_text_changed_data = user_data;
}

void Kauto_suggest_box_on_suggestion_chosen(Kid id, KAutoSuggestSuggestionChosenCallback callback, void* user_data) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (!a) return;
	a->on_suggestion_chosen = callback;
	a->on_suggestion_chosen_data = user_data;
}

void Kauto_suggest_box_on_query_submitted(Kid id, KAutoSuggestQuerySubmittedCallback callback, void* user_data) {
	kst_auto_suggest_box_t* a = get_asb(id);
	if (!a) return;
	a->on_query_submitted = callback;
	a->on_query_submitted_data = user_data;
}
