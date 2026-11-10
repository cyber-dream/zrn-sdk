/**
 * zrnui - controls/buttons.c
 * ButtonBase, Button, HyperlinkButton, RepeatButton, ToggleButton, CheckBox,
 * RadioButton and ToggleSwitch, after the Windows Phone 8.1 templates
 * (generic.xaml of the phone SDK): their numbers are used as they are.
 */

#include "../internal.h"


/* ============================================================================
 *  ButtonBase
 * ============================================================================ */

#define BUTTON(e) ZST_AS(zrn_button_base_t, e)
#define TOGGLE(e) ZST_AS(zrn_toggle_button_t, e)

static void toggle_on_click(zrn_element_t* e);

/* A Click handler may destroy the button (a tap that opens another page): nothing touches it afterwards. */
static void button_click(zrn_element_t* const e) {
	const Zid id = e->id;
	if (zrn_is_a(e, Z_CLASS_TOGGLE_BUTTON)) {
		toggle_on_click(e);
		if (!zrn_lookup(id)) return;
	}
	if (!zrn_raise(e, Z_EVENT_CLICK)) return;
	/* Button.Flyout: the tap shows it; a command of an app bar that is a secondary one closes the bar */
	if (zrn_is_a(e, Z_CLASS_BUTTON) && BUTTON(e)->flyout) zrn_flyout_show_at(BUTTON(e)->flyout, e);
	if (!zrn_lookup(id)) return;
	if (zrn_is_a(e, Z_CLASS_APP_BAR_BUTTON)) zrn_app_bar_item_clicked(e);
}

static int inside(const zrn_element_t* e, float x, float y) { return x >= 0 && y >= 0 && x < e->rect.width && y < e->rect.height; }

/* IsPressed of the template: the button is held and the pointer is on it. */
static int button_is_pressed(const zrn_button_base_t* b) { return b->pressed && b->pointer_over; }

static void button_set_pressed(zrn_button_base_t* const b, const int pressed, const int over) {
	if (b->pressed == pressed && b->pointer_over == over) return;
	b->pressed = pressed;
	b->pointer_over = over;
	zrn_changed_paint(&b->base.base.base);
}

static int button_input(zrn_element_t* const e, zrn_input_t* const in) {
	zrn_button_base_t* const b = BUTTON(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			if (!zrn_effectively_enabled(e)) return 1;
			button_set_pressed(b, 1, 1);
			if (e->root) zrn_set_capture(e->root, e);
			if (b->repeat) {
				b->repeat_timer = 0;
				b->repeating = 0;
				e->wants_tick = 1;
				button_click(e);
				return 1;
			}
			if (b->click_mode == Z_CLICK_PRESS) button_click(e);
			return 1;
		case ZST_IN_POINTER_MOVE:
			if (b->pressed) button_set_pressed(b, 1, inside(e, in->x, in->y));
			return b->pressed;
		case ZST_IN_POINTER_UP: {
			const int was = b->pressed, over = inside(e, in->x, in->y);
			if (!was) return 0;
			button_set_pressed(b, 0, over);
			e->wants_tick = 0;
			if (e->root) zrn_release_capture(e->root, e);
			if (over && b->click_mode == Z_CLICK_RELEASE && !b->repeat) button_click(e);
			return 1;
		}
		case ZST_IN_POINTER_ENTER:
			if (!b->pressed) {
				b->pointer_over = 1;
				zrn_changed_paint(e);
				if (b->click_mode == Z_CLICK_HOVER && zrn_effectively_enabled(e)) button_click(e);
			}
			return 0;
		case ZST_IN_POINTER_LEAVE:
			if (!b->pressed) {
				b->pointer_over = 0;
				zrn_changed_paint(e);
			}
			return 0;
		case ZST_IN_CAPTURE_LOST:
			b->pressed = 0;
			b->keyboard_pressed = 0;
			e->wants_tick = 0;
			zrn_changed_paint(e);
			return 0;
		case ZST_IN_KEY_DOWN:
			if (in->key == Z_KEY_SPACE || in->key == Z_KEY_ENTER) {
				if (!zrn_effectively_enabled(e)) return 1;
				b->keyboard_pressed = 1;
				button_set_pressed(b, 1, 1);
				if (b->click_mode == Z_CLICK_PRESS) button_click(e);
				return 1;
			}
			return 0;
		case ZST_IN_KEY_UP:
			if ((in->key == Z_KEY_SPACE || in->key == Z_KEY_ENTER) && b->keyboard_pressed) {
				b->keyboard_pressed = 0;
				button_set_pressed(b, 0, e->pointer_over);
				if (b->click_mode == Z_CLICK_RELEASE) button_click(e);
				return 1;
			}
			return 0;
		case ZST_IN_FOCUS_OUT:
			b->keyboard_pressed = 0;
			return 0;
		default:
			return 0;
	}
}

static void button_tick(zrn_element_t* const e, const float dt) {
	zrn_button_base_t* const b = BUTTON(e);
	if (!b->repeat || !b->pressed) return;
	if (!button_is_pressed(b)) return;
	b->repeat_timer += dt;
	if (!b->repeating) {
		if (b->repeat_timer < (float)b->delay_ms) return;
		b->repeating = 1;
		b->repeat_timer = 0;
		button_click(e);
		return;
	}
	while (b->repeat_timer >= (float)b->interval_ms && b->pressed) {
		const Zid id = e->id;
		b->repeat_timer -= (float)b->interval_ms;
		button_click(e);
		if (!zrn_lookup(id) || b->interval_ms <= 0) break;
	}
}

/* PhoneTouchTargetOverhang is 0,9.5: the Border of the template has that margin, above and below the outline only. */
static ZThickness button_chrome(zrn_element_t* const e) {
	ZST_UNUSED(e);
	return Zthickness(0, ZST_81_TOUCH_OVERHANG, 0, ZST_81_TOUCH_OVERHANG);
}

/* The foreground of the content: ButtonPressedForeground while held, ButtonDisabledForeground (PhoneBaseLow) when disabled. */
static ZColor button_content_foreground(zrn_element_t* const e) {
	const zrn_button_base_t* const b = BUTTON(e);
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_BASE_LOW);
	if (button_is_pressed(b)) return zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT);
	return zrn_ctl_foreground(&b->base.base);
}

/* The Button template: a Border (BorderThickness 2.5) with the touch overhang above and below it. */
static void button_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_button_base_t* const b = BUTTON(e);
	const zrn_control_t* const c = &b->base.base;
	const ZRect r = zrn_rect(0, ZST_81_TOUCH_OVERHANG, e->rect.width, e->rect.height - 2 * ZST_81_TOUCH_OVERHANG);
	ZColor background = zrn_ctl_background(c, Zcolor_rgba(0, 0, 0, 0));
	ZColor border = zrn_ctl_border_brush(c, zrn_ctl_foreground(c));
	if (!zrn_effectively_enabled(e)) {
		background = Zcolor_rgba(0, 0, 0, 0);
		border = zrn_theme_color(e, ZST_TC_BASE_LOW);
	} else if (button_is_pressed(b)) {
		background = zrn_theme_color(e, ZST_TC_ACCENT);
	}
	if (background.a > 0) zrn_fill_rect(p, r, background);
	zrn_stroke_rect(p, r, c->border_thickness, border);
}

/* The style of Button, RepeatButton and ToggleButton; the classes below it change what they differ in. */
static void button_init(zrn_element_t* const e) {
	zrn_button_base_t* const b = BUTTON(e);
	zrn_control_t* const c = &b->base.base;
	c->border_thickness = Zthickness_uniform(ZST_81_BORDER_THICKNESS);
	c->padding = Zthickness(9.5f, 0, 9.5f, 3.5f);
	c->font_family = Z_FONT_SEMIBOLD;
	c->font_size = ZST_81_FONT_LARGE;
	e->min_width = 109.0f;
	e->min_height = 57.5f;
	e->halign = Z_HALIGN_LEFT;
	e->valign = Z_VALIGN_CENTER;
	b->click_mode = Z_CLICK_RELEASE;
	b->delay_ms = 250;
	b->interval_ms = 250;
}

static void button_destroy(zrn_element_t* e) { free(BUTTON(e)->uri); }

static void button_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	if (BUTTON(e)->flyout == child) BUTTON(e)->flyout = nullptr;
	else zrn_cc_child_removed(e, child);
}

zrn_class_t zrn_class_button_base = {
	.base = &zrn_class_content_control, .name = "ButtonBase", .id = Z_CLASS_BUTTON_BASE, .size = sizeof(zrn_button_base_t), .abstract_class = 1,
	.init = button_init, .destroy = button_destroy, .child_removed = button_child_removed, .input = button_input, .tick = button_tick, .chrome = button_chrome,
	.paint = button_paint, .content_foreground = button_content_foreground
};

zrn_class_t zrn_class_button = {
	.base = &zrn_class_button_base, .name = "Button", .id = Z_CLASS_BUTTON, .size = sizeof(zrn_button_base_t)
};

static void repeat_init(zrn_element_t* e) { BUTTON(e)->repeat = 1; }

zrn_class_t zrn_class_repeat_button = {
	.base = &zrn_class_button_base, .name = "RepeatButton", .id = Z_CLASS_REPEAT_BUTTON, .size = sizeof(zrn_button_base_t),
	.init = repeat_init
};

static zrn_element_t* make_button(const ZClass cls, const char* const content) {
	zrn_element_t* const e = zrn_create(cls);
	if (!e) return nullptr;
	if (content) zrn_content_set_text(ZST_AS(zrn_content_control_t, e), content);
	return e;
}

Zid Zbutton_create(const char* const content) {
	const zrn_element_t* const e = make_button(Z_CLASS_BUTTON, content);
	return e ? e->id : Z_INVALID_ID;
}

Zid Zrepeat_button_create(const char* const content) {
	const zrn_element_t* const e = make_button(Z_CLASS_REPEAT_BUTTON, content);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_button_base_t* get_button(Zid id) { return BUTTON(zrn_get(id, Z_CLASS_BUTTON_BASE)); }

void Zbutton_on_click(Zid id, ZRoutedCallback callback, void* user_data) { Zelement_on(id, Z_EVENT_CLICK, callback, user_data); }

void Zbutton_set_click_mode(const Zid id, const ZClickMode mode) {
	zrn_button_base_t* const b = get_button(id);
	if (b) b->click_mode = mode;
}

int Zbutton_is_pressed(const Zid id) {
	const zrn_button_base_t* const b = get_button(id);
	return b ? button_is_pressed(b) : 0;
}

int Zbutton_is_pointer_over(const Zid id) {
	const zrn_button_base_t* const b = get_button(id);
	return b ? b->base.base.base.pointer_over != 0 : 0;
}

void Zbutton_click(const Zid id) {
	zrn_button_base_t* const b = get_button(id);
	if (b && zrn_effectively_enabled(&b->base.base.base)) button_click(&b->base.base.base);
}

void Zrepeat_button_set_delay(const Zid id, const int delay_ms) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_REPEAT_BUTTON);
	if (e) BUTTON(e)->delay_ms = delay_ms < 0 ? 0 : delay_ms;
}

int Zrepeat_button_get_delay(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_REPEAT_BUTTON);
	return e ? BUTTON(e)->delay_ms : 0;
}

void Zrepeat_button_set_interval(const Zid id, const int interval_ms) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_REPEAT_BUTTON);
	if (e) BUTTON(e)->interval_ms = interval_ms < 0 ? 0 : interval_ms;
}

int Zrepeat_button_get_interval(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_REPEAT_BUTTON);
	return e ? BUTTON(e)->interval_ms : 0;
}


/* ============================================================================
 *  HyperlinkButton
 *  Text only: no outline, no touch overhang, no underline and no color of its
 *  own while pressed (the template only runs PointerDownThemeAnimation, the
 *  lean of Zelement_set_tilt); HyperlinkDisabledThemeBrush when disabled.
 * ============================================================================ */

static ZThickness hyperlink_chrome(zrn_element_t* const e) {
	ZST_UNUSED(e);
	return zrn_thickness_zero();
}

static ZColor hyperlink_content_foreground(zrn_element_t* const e) {
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_DISABLED);
	return zrn_ctl_foreground(&BUTTON(e)->base.base);
}

static void hyperlink_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_control_t* const c = &BUTTON(e)->base.base;
	/* HyperlinkButtonBackgroundThemeBrush is transparent: only a color the program set shows */
	if (c->set_flags & ZST_C_BACKGROUND) zrn_fill_rect(p, zrn_local_rect(e), c->background);
}

static void hyperlink_init(zrn_element_t* const e) {
	zrn_control_t* const c = &BUTTON(e)->base.base;
	c->border_thickness = zrn_thickness_zero();
	c->padding = Zthickness(0, 3, 0, 4);
	c->font_family = Z_FONT_SEMILIGHT;
	c->font_size = ZST_81_FONT_EXTRA_LARGE_PLUS;
	e->min_width = 0;
	e->min_height = 0;
	e->halign = Z_HALIGN_LEFT;
	e->valign = Z_VALIGN_CENTER;
}

zrn_class_t zrn_class_hyperlink_button = {
	.base = &zrn_class_button_base, .name = "HyperlinkButton", .id = Z_CLASS_HYPERLINK_BUTTON, .size = sizeof(zrn_button_base_t),
	.init = hyperlink_init, .chrome = hyperlink_chrome, .paint = hyperlink_paint,
	.content_foreground = hyperlink_content_foreground
};

Zid Zhyperlink_button_create(const char* const content) {
	const zrn_element_t* const e = make_button(Z_CLASS_HYPERLINK_BUTTON, content);
	return e ? e->id : Z_INVALID_ID;
}

void Zhyperlink_button_set_uri(const Zid id, const char* const uri) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_HYPERLINK_BUTTON);
	if (e) zrn_set_string(&BUTTON(e)->uri, uri);
}

const char* Zhyperlink_button_get_uri(const Zid id) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_HYPERLINK_BUTTON);
	return e && BUTTON(e)->uri ? BUTTON(e)->uri : "";
}


/* ============================================================================
 *  ToggleButton
 * ============================================================================ */

static void toggle_find_radios(zrn_element_t* const e, zrn_element_t*** list, int* const count, int* const cap) {
	int i;
	if (e->cls == &zrn_class_radio_button) {
		if (*count == *cap) {
			const int ncap = *cap ? *cap * 2 : 8;
			zrn_element_t** more = (zrn_element_t**)realloc(*list, (size_t)ncap * sizeof *more);
			if (!more) return;
			*list = more;
			*cap = ncap;
		}
		(*list)[(*count)++] = e;
	}
	for (i = 0; i < e->child_count; i++) toggle_find_radios(e->children[i], list, count, cap);
}

/* Checking a radio button unchecks the others of its group: same GroupName, or, with none, the same parent.
 * Their states change here; the Unchecked events are the caller's, once nothing is half done. */
static int radio_exclude(zrn_element_t* const e, Zid** out_ids) {
	const zrn_toggle_button_t* const me = TOGGLE(e);
	zrn_element_t* top = e;
	zrn_element_t** list = nullptr;
	Zid* ids = nullptr;
	int count = 0, cap = 0, i, n = 0;
	while (top->parent) top = top->parent;
	toggle_find_radios(top, &list, &count, &cap);
	if (count > 0) ids = (Zid*)malloc((size_t)count * sizeof *ids);
	for (i = 0; i < count && ids; i++) {
		zrn_toggle_button_t* const other = TOGGLE(list[i]);
		int same;
		if (list[i] == e || other->state != Z_CHECKED) continue;
		if (me->group_name && *me->group_name) same = other->group_name && strcmp(other->group_name, me->group_name) == 0;
		else same = !(other->group_name && *other->group_name) && list[i]->parent == e->parent;
		if (same) {
			other->state = Z_UNCHECKED;
			zrn_changed_paint(list[i]);
			ids[n++] = list[i]->id;
		}
	}
	free(list);
	*out_ids = ids;
	return n;
}

static void toggle_set_state(zrn_element_t* const e, const ZToggleState state) {
	zrn_toggle_button_t* const t = TOGGLE(e);
	Zid* others = nullptr;
	int n = 0, i;
	if (t->state == state) return;
	t->state = state;
	if (state == Z_CHECKED && e->cls == &zrn_class_radio_button) n = radio_exclude(e, &others);
	zrn_changed_paint(e);
	zrn_raise(e, state == Z_CHECKED ? Z_EVENT_CHECKED : (state == Z_UNCHECKED ? Z_EVENT_UNCHECKED : Z_EVENT_INDETERMINATE));
	for (i = 0; i < n; i++) {
		zrn_element_t* const other = zrn_lookup(others[i]);
		if (other) zrn_raise(other, Z_EVENT_UNCHECKED);
	}
	free(others);
}

static void toggle_on_click(zrn_element_t* const e) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	if (e->cls == &zrn_class_radio_button) {
		toggle_set_state(e, Z_CHECKED);
		return;
	}
	switch (t->state) {
		case Z_CHECKED: toggle_set_state(e, t->three_state ? Z_INDETERMINATE : Z_UNCHECKED); break;
		case Z_UNCHECKED: toggle_set_state(e, Z_CHECKED); break;
		default: toggle_set_state(e, Z_UNCHECKED); break;
	}
}

/*
 * A plain ToggleButton is a Button with no pressed look, that fills with the accent while checked
 * (text over accent). Disabled it is the outline and text of PhoneBaseLow; checked and disabled it
 * is the checked look at 0.4 opacity.
 */
static ZColor toggle_content_foreground(zrn_element_t* const e) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	const int enabled = zrn_effectively_enabled(e);
	if (t->state == Z_CHECKED) return zrn_color_alpha(zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT), enabled ? 1.0f : 0.4f);
	if (!enabled) return zrn_theme_color(e, ZST_TC_BASE_LOW);
	return zrn_ctl_foreground(&t->base.base.base);
}

static void toggle_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_toggle_button_t* const t = TOGGLE(e);
	const zrn_control_t* const c = &t->base.base.base;
	const ZRect r = zrn_rect(0, ZST_81_TOUCH_OVERHANG, e->rect.width, e->rect.height - 2 * ZST_81_TOUCH_OVERHANG);
	ZColor background = zrn_ctl_background(c, Zcolor_rgba(0, 0, 0, 0));
	ZColor border = zrn_ctl_border_brush(c, zrn_ctl_foreground(c));
	const int enabled = zrn_effectively_enabled(e);
	if (t->state == Z_CHECKED) {
		background = zrn_theme_color(e, ZST_TC_ACCENT);
		if (!enabled) {
			background = zrn_color_alpha(background, 0.4f);
			border = zrn_color_alpha(border, 0.4f);
		}
	} else if (!enabled) {
		background = Zcolor_rgba(0, 0, 0, 0);
		border = zrn_theme_color(e, ZST_TC_BASE_LOW);
	}
	if (background.a > 0) zrn_fill_rect(p, r, background);
	zrn_stroke_rect(p, r, c->border_thickness, border);
}

static void toggle_init(zrn_element_t* e) { TOGGLE(e)->state = Z_UNCHECKED; }
static void toggle_destroy(zrn_element_t* e) { free(TOGGLE(e)->group_name); }

zrn_class_t zrn_class_toggle_button = {
	.base = &zrn_class_button_base, .name = "ToggleButton", .id = Z_CLASS_TOGGLE_BUTTON, .size = sizeof(zrn_toggle_button_t),
	.init = toggle_init, .destroy = toggle_destroy, .paint = toggle_paint, .content_foreground = toggle_content_foreground
};

Zid Ztoggle_button_create(const char* const content) {
	const zrn_element_t* const e = make_button(Z_CLASS_TOGGLE_BUTTON, content);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_toggle_button_t* get_toggle(Zid id) { return TOGGLE(zrn_get(id, Z_CLASS_TOGGLE_BUTTON)); }

void Ztoggle_button_set_state(const Zid id, const ZToggleState state) {
	zrn_toggle_button_t* const t = get_toggle(id);
	if (t) toggle_set_state(&t->base.base.base.base, state);
}

void Ztoggle_button_set_is_checked(Zid id, int is_checked) { Ztoggle_button_set_state(id, is_checked ? Z_CHECKED : Z_UNCHECKED); }
ZToggleState Ztoggle_button_get_state(Zid id) { const zrn_toggle_button_t* t = get_toggle(id); return t ? t->state : Z_UNCHECKED; }
int Ztoggle_button_get_is_checked(Zid id) { return Ztoggle_button_get_state(id) == Z_CHECKED; }

void Ztoggle_button_set_is_three_state(const Zid id, const int three_state) {
	zrn_toggle_button_t* const t = get_toggle(id);
	if (t) t->three_state = three_state != 0;
}


/* ============================================================================
 *  CheckBox and RadioButton: a 25.5 px box with the content after it, and
 *  PhoneTouchTargetLargeOverhang (0,16) above and below
 * ============================================================================ */

static constexpr float BOX = 25.5f;
static constexpr float CHECK_MARK_W = 18.5f;
static constexpr float CHECK_MARK_H = 17.0f;
static constexpr float RADIO_MARK = 12.5f;

/* The box is in the chrome; for a CheckBox the border thickness (it is the border of the box) is taken back out of the insets. */
static ZThickness box_chrome(zrn_element_t* const e) {
	const zrn_control_t* const c = ZST_AS(zrn_control_t, e);
	const ZThickness b = c->border_thickness;
	return Zthickness(BOX - b.left, ZST_81_TOUCH_LARGE_OVERHANG - b.top, -b.right, ZST_81_TOUCH_LARGE_OVERHANG - b.bottom);
}

static ZSize box_measure(zrn_element_t* const e, const ZSize available) {
	ZSize s = zrn_cc_measure(e, available);
	/* the row is as high as the box with its overhang */
	s.height = zrn_maxf(s.height, BOX + 2 * ZST_81_TOUCH_LARGE_OVERHANG);
	s.width = zrn_maxf(s.width, BOX);
	return s;
}

/* The common style of CheckBox and RadioButton: Padding 8,0,0,0, MinWidth 168, top left. */
static void box_style_init(zrn_element_t* const e) {
	zrn_control_t* const c = &TOGGLE(e)->base.base.base;
	c->padding = Zthickness(8, 0, 0, 0);
	c->font_family = Z_FONT_NORMAL;
	c->font_size = ZST_81_FONT_LARGE;
	c->h_content = Z_HALIGN_LEFT;
	c->v_content = Z_VALIGN_TOP;
	e->min_width = 168.0f;
	e->min_height = 0;
	e->halign = Z_HALIGN_LEFT;
	e->valign = Z_VALIGN_TOP;
}

static void checkbox_init(zrn_element_t* const e) {
	box_style_init(e);
	TOGGLE(e)->base.base.base.border_thickness = Zthickness_uniform(ZST_81_BORDER_THICKNESS);
}

static void radio_init(zrn_element_t* const e) {
	box_style_init(e);
	/* the circle has the PhoneStrokeThickness of its own, not the BorderThickness of the control */
	TOGGLE(e)->base.base.base.border_thickness = zrn_thickness_zero();
}

static ZRect box_rect(void) { return zrn_rect(0, ZST_81_TOUCH_LARGE_OVERHANG, BOX, BOX); }

static ZColor box_border(zrn_element_t* const e) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_BASE_LOW);
	return zrn_ctl_border_brush(&t->base.base.base, zrn_theme_color(e, ZST_TC_FOREGROUND));
}

/* The check, the dash and the dot: the foreground of the theme, over the accent while pressed. */
static ZColor box_mark(zrn_element_t* const e) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_BASE_LOW);
	if (button_is_pressed(&t->base)) return zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT);
	return zrn_theme_color(e, ZST_TC_FOREGROUND);
}

static ZColor box_fill(zrn_element_t* const e) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	if (zrn_effectively_enabled(e) && button_is_pressed(&t->base)) return zrn_theme_color(e, ZST_TC_ACCENT);
	return zrn_ctl_background(&t->base.base.base, Zcolor_rgba(0, 0, 0, 0));
}

static void checkbox_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	zrn_toggle_button_t* const t = TOGGLE(e);
	const zrn_control_t* const c = &t->base.base.base;
	const ZRect box = box_rect();
	const ZColor fill = box_fill(e), mark = box_mark(e);
	if (fill.a > 0) zrn_fill_rect(p, box, fill);
	zrn_stroke_rect(p, box, c->border_thickness, box_border(e));
	if (t->state == Z_CHECKED) {
		/* Path "M0,123 L39,93 L124,164 L256,18 L295,49 L124,240 z", Stretch Fill into 18.5 x 17 at the center of the box */
		static const float path[12] = { 0, 123, 39, 93, 124, 164, 256, 18, 295, 49, 124, 240 };
		float pts[12];
		int i;
		for (i = 0; i < 6; i++) {
			pts[i * 2] = box.x + (BOX - CHECK_MARK_W) * 0.5f + path[i * 2] / 295.0f * CHECK_MARK_W;
			pts[i * 2 + 1] = box.y + (BOX - CHECK_MARK_H) * 0.5f + (path[i * 2 + 1] - 18.0f) / 222.0f * CHECK_MARK_H;
		}
		zrn_fill_polygon(p, pts, 6, mark);
	} else if (t->state == Z_INDETERMINATE) {
		/* NormalRectangle, 13 x 13 */
		zrn_fill_rect(p, zrn_rect(box.x + (BOX - 13.0f) * 0.5f, box.y + (BOX - 13.0f) * 0.5f, 13, 13), mark);
	}
}

static void radio_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_toggle_button_t* const t = TOGGLE(e);
	const ZRect box = box_rect();
	const ZColor fill = box_fill(e);
	/* the stroke of an Ellipse is inside its bounds */
	if (fill.a > 0) zrn_fill_ellipse(p, zrn_rect(box.x + ZST_81_BORDER_THICKNESS * 0.5f, box.y + ZST_81_BORDER_THICKNESS * 0.5f, BOX - ZST_81_BORDER_THICKNESS, BOX - ZST_81_BORDER_THICKNESS), fill);
	zrn_stroke_ellipse(p, box, ZST_81_BORDER_THICKNESS, box_border(e));
	if (t->state == Z_CHECKED) zrn_fill_ellipse(p, zrn_rect(box.x + (BOX - RADIO_MARK) * 0.5f, box.y + (BOX - RADIO_MARK) * 0.5f, RADIO_MARK, RADIO_MARK), box_mark(e));
}

static ZColor box_content_foreground(zrn_element_t* const e) {
	const zrn_toggle_button_t* t = TOGGLE(e);
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_BASE_LOW);
	return zrn_ctl_foreground(&t->base.base.base);
}

zrn_class_t zrn_class_checkbox = {
	.base = &zrn_class_toggle_button, .name = "CheckBox", .id = Z_CLASS_CHECKBOX, .size = sizeof(zrn_toggle_button_t),
	.init = checkbox_init, .measure = box_measure, .chrome = box_chrome, .paint = checkbox_paint,
	.content_foreground = box_content_foreground
};

zrn_class_t zrn_class_radio_button = {
	.base = &zrn_class_toggle_button, .name = "RadioButton", .id = Z_CLASS_RADIO_BUTTON, .size = sizeof(zrn_toggle_button_t),
	.init = radio_init, .measure = box_measure, .chrome = box_chrome, .paint = radio_paint,
	.content_foreground = box_content_foreground
};

Zid Zcheck_box_create(const char* const content) {
	const zrn_element_t* const e = make_button(Z_CLASS_CHECKBOX, content);
	return e ? e->id : Z_INVALID_ID;
}

Zid Zradio_button_create(const char* const content, const char* const group_name) {
	zrn_element_t* const e = make_button(Z_CLASS_RADIO_BUTTON, content);
	if (!e) return Z_INVALID_ID;
	if (group_name && *group_name) zrn_set_string(&TOGGLE(e)->group_name, group_name);
	return e->id;
}

void Zradio_button_set_group_name(const Zid id, const char* const group_name) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_RADIO_BUTTON);
	if (e) zrn_set_string(&TOGGLE(e)->group_name, group_name && *group_name ? group_name : nullptr);
}


/* ============================================================================
 *  ToggleSwitch
 *  The 8.1 template: a header, the content of the state (OnContent or
 *  OffContent, 25.5 px) and, bottom right, a 77 x 36 switch: a 2.5 px outline
 *  of the foreground (inset 3,4.5), the accent curtain behind the knob that
 *  slides in from the left, and a 20.5 px knob that travels 56.5 px. The
 *  control has Padding 0,0,25.5,0 around the texts, and 13.5 px between them
 *  and the switch. A tap anywhere or Space flips it; a drag that starts on
 *  the knob (with the touch margin of the template) carries it, and it lets
 *  go to the nearer end. The ToggleSwitchTemplateSettings offsets are the
 *  positions the knob and the curtain are drawn at; they are not exposed.
 * ============================================================================ */

#define TSW(e) ZST_AS(zrn_toggle_switch_t, e)
static constexpr float TS_SWITCH_W = 77.0f;
static constexpr float TS_SWITCH_H = 36.0f;
static constexpr float TS_GAP = 13.5f;
static constexpr float TS_BOTTOM_ROW = 9.5f;
static constexpr float TS_KNOB_W = 20.5f;
static constexpr float TS_KNOB_TRAVEL = 56.5f;
static constexpr float TS_OUTLINE = 2.5f;
/** The animation of the knob, in ms. */

static zrn_font_t ts_header_font(void) { const zrn_font_t f = { Z_FONT_NORMAL, ZST_81_FONT_CONTROL_HEADER, 0 }; return f; }
static zrn_font_t ts_content_font(void) { const zrn_font_t f = { Z_FONT_NORMAL, ZST_81_FONT_EXTRA_LARGE, 0 }; return f; }

static int ts_has(const char* s) { return s && *s; }

typedef struct ts_geometry {
	float header_row, content_row, region;
	/** The 77 x 36 box of the switch, in local coordinates. */
	ZRect box;
} ts_geometry_t;

static void ts_geometry(const zrn_element_t* const e, ts_geometry_t* const g) {
	const zrn_toggle_switch_t* t = TSW((zrn_element_t*)e);
	const ZThickness pad = t->base.padding;
	g->header_row = ts_has(t->header) ? zrn_font_line_height(ts_header_font()) + zrn_thickness_v(pad) : 0;
	g->content_row = ts_has(t->on_content) || ts_has(t->off_content) ? zrn_font_line_height(ts_content_font()) + zrn_thickness_v(pad) : 0;
	g->region = zrn_maxf(g->header_row + g->content_row, TS_SWITCH_H);
	g->box = zrn_rect(e->rect.width - TS_SWITCH_W, g->region - TS_SWITCH_H, TS_SWITCH_W, TS_SWITCH_H);
}

static ZSize ts_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_toggle_switch_t* t = TSW(e);
	const ZThickness pad = t->base.padding;
	ts_geometry_t g;
	float w = 0;
	ts_geometry(e, &g);
	if (ts_has(t->header)) w = zrn_maxf(w, zrn_text_width(ts_header_font(), t->header, (int)strlen(t->header)));
	if (ts_has(t->on_content)) w = zrn_maxf(w, zrn_text_width(ts_content_font(), t->on_content, (int)strlen(t->on_content)));
	if (ts_has(t->off_content)) w = zrn_maxf(w, zrn_text_width(ts_content_font(), t->off_content, (int)strlen(t->off_content)));
	w += zrn_thickness_h(pad) + TS_GAP + TS_SWITCH_W;
	if (available.width < Z_INFINITE * 0.5f) w = zrn_minf(w, available.width);
	return zrn_size(w, g.region + TS_BOTTOM_ROW);
}

static void ts_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_toggle_switch_t* const t = TSW(e);
	const ZThickness pad = t->base.padding;
	ts_geometry_t g;
	const int enabled = zrn_effectively_enabled(e);
	ZColor foreground = zrn_theme_color(e, ZST_TC_FOREGROUND), text_color, outline, curtain, knob_fill;
	ZRect outer, inner, curtain_rect;
	float knob_x;
	const char* content = t->is_on ? t->on_content : t->off_content;
	ts_geometry(e, &g);
	if (t->base.set_flags & ZST_C_FOREGROUND) foreground = t->base.foreground;
	text_color = enabled ? foreground : zrn_theme_color(e, ZST_TC_BASE_LOW);
	if (t->base.set_flags & ZST_C_BACKGROUND) zrn_fill_rect(p, zrn_local_rect(e), t->base.background);
	if (ts_has(t->header)) {
		zrn_draw_string(p, ts_header_font(), t->header, (int)strlen(t->header), pad.left, pad.top,
			enabled ? zrn_theme_color(e, ZST_TC_BASE_MID) : zrn_theme_color(e, ZST_TC_DISABLED));
	}
	if (ts_has(content)) zrn_draw_string(p, ts_content_font(), content, (int)strlen(content), pad.left, g.header_row + pad.top, text_color);

	/* the switch */
	outline = enabled ? zrn_theme_color(e, ZST_TC_FOREGROUND) : zrn_theme_color(e, ZST_TC_BASE_LOW);
	curtain = enabled ? zrn_theme_color(e, ZST_TC_ACCENT) : zrn_theme_color(e, ZST_TC_BASE_MEDIUM_MID_SOLID);
	knob_fill = enabled ? zrn_theme_color(e, ZST_TC_FOREGROUND) : zrn_theme_color(e, ZST_TC_BASE_MEDIUM_MID_SOLID);
	outer = zrn_rect(g.box.x + 3.0f, g.box.y + 4.5f, TS_SWITCH_W - 6.0f, TS_SWITCH_H - 9.0f);
	/* the inner border of the template has a transparent brush, so the curtain lies 2.5 px inside the outline's inner edge */
	inner = zrn_rect(outer.x + 2 * TS_OUTLINE, outer.y + 2 * TS_OUTLINE, outer.width - 4 * TS_OUTLINE, outer.height - 4 * TS_OUTLINE);
	zrn_stroke_rect(p, outer, Zthickness_uniform(TS_OUTLINE), outline);
	/* the curtain is the accent fill to the left of the knob: it grows with the knob
	 * and stays full while the knob squashes against the wall on the overshoot */
	float curtain_w = inner.width * t->knob;
	if (curtain_w < 0) curtain_w = 0;
	if (curtain_w > inner.width) curtain_w = inner.width;
	curtain_rect = zrn_rect(inner.x, inner.y, curtain_w, inner.height);
	if (curtain_rect.width > 0) zrn_fill_rect(p, curtain_rect, curtain);
	knob_x = g.box.x + TS_KNOB_TRAVEL * t->knob;
	/* SwitchKnob: Fill is the foreground, the 2.5 px Stroke (inside its bounds) the background of the page */
	zrn_fill_rect(p, zrn_rect(knob_x, g.box.y, TS_KNOB_W, TS_SWITCH_H), zrn_theme_color(e, ZST_TC_BACKGROUND));
	zrn_fill_rect(p, zrn_rect(knob_x + TS_OUTLINE, g.box.y + TS_OUTLINE, TS_KNOB_W - 2 * TS_OUTLINE, TS_SWITCH_H - 2 * TS_OUTLINE), knob_fill);
}

static void ts_set_goal(zrn_toggle_switch_t* const t, const float goal) {
	zrn_element_t* const e = &t->base.base;
	t->knob_goal = goal;
	if (!zrn_animations_enabled(e->root)) {
		t->knob = goal;
		t->knob_animating = 0;
		e->wants_tick = 0;
	} else if (t->knob != goal) {
		t->knob_from = t->knob;
		t->knob_elapsed = 0;
		t->knob_animating = 1;
		e->wants_tick = 1;
	}
	zrn_changed_paint(e);
}

/* @return false when a Toggled handler destroyed the switch. */
static int ts_set_on(zrn_toggle_switch_t* t, int on) {
	zrn_element_t* const e = &t->base.base;
	on = on != 0;
	if (t->is_on == on) return 1;
	t->is_on = on;
	ts_set_goal(t, on ? 1.0f : 0.0f);
	zrn_changed_paint(e);
	return zrn_raise(e, Z_EVENT_TOGGLED);
}

static void ts_tick(zrn_element_t* const e, const float dt) {
	zrn_toggle_switch_t* const t = TSW(e);
	if (t->dragging) { t->knob_animating = 0; return; }
	if (!t->knob_animating) return;
	t->knob_elapsed += dt;
	{
		const float tt = t->knob_elapsed / 180.0f;
		if (tt >= 1.0f) { t->knob = t->knob_goal; t->knob_animating = 0; e->wants_tick = 0; }
		else {
			const float u = 1 - tt, ease = 1 - u * u * u;
			t->knob = t->knob_from + (t->knob_goal - t->knob_from) * ease;
		}
	}
	zrn_changed_paint(e);
}

/* The Thumb of the template covers the knob with margins -13.5,-15.5,-13.5,-6.5. */
static int ts_on_knob(const zrn_element_t* const e, const float x, const float y) {
	const zrn_toggle_switch_t* t = TSW((zrn_element_t*)e);
	ts_geometry_t g;
	float kx;
	ts_geometry(e, &g);
	kx = g.box.x + TS_KNOB_TRAVEL * t->knob;
	return x >= kx - 13.5f && x < kx + TS_KNOB_W + 13.5f && y >= g.box.y - 15.5f && y < g.box.y + TS_SWITCH_H + 6.5f;
}

static int ts_input(zrn_element_t* const e, zrn_input_t* const in) {
	zrn_toggle_switch_t* const t = TSW(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN: {
			ts_geometry_t box;
			/* only the switch itself takes a tap: the header, the On/Off text and the empty space pass it on */
			ts_geometry(e, &box);
			if (in->x < box.box.x || in->x >= box.box.x + box.box.width || in->y < box.box.y || in->y >= box.box.y + box.box.height) return 0;
			if (!zrn_effectively_enabled(e)) return 1;
			t->pressed = 1;
			t->dragging = 0;
			t->moved = 0;
			t->press_x = in->x;
			if (ts_on_knob(e, in->x, in->y)) {
				ts_geometry_t g;
				ts_geometry(e, &g);
				t->grab = in->x - (g.box.x + TS_KNOB_TRAVEL * t->knob);
				t->moved = -1;  /* a drag may start */
			}
			if (e->root) zrn_set_capture(e->root, e);
			return 1;
		}
		case ZST_IN_POINTER_MOVE:
			if (!t->pressed) return 0;
			if (t->moved == -1 && fabsf(in->x - t->press_x) > 3.0f) t->dragging = 1;
			if (t->dragging) {
				ts_geometry_t g;
				ts_geometry(e, &g);
				t->knob = zrn_clampf((in->x - t->grab - g.box.x) / TS_KNOB_TRAVEL, 0.0f, 1.0f);
				t->knob_goal = t->knob;
				zrn_changed_paint(e);
			} else if (fabsf(in->x - t->press_x) > 3.0f) {
				t->moved = 1;
			}
			return 1;
		case ZST_IN_POINTER_UP: {
			const int was_dragging = t->dragging, inside_now = in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			const Zid id = e->id;
			if (!t->pressed) return 0;
			t->pressed = 0;
			t->dragging = 0;
			if (e->root) zrn_release_capture(e->root, e);
			if (!zrn_lookup(id)) return 1;
			if (was_dragging) {
				const int on = t->knob >= 0.5f;
				if (on == t->is_on) ts_set_goal(t, on ? 1.0f : 0.0f);
				else ts_set_on(t, on);
			} else if (t->moved != 1 && inside_now) {
				ts_set_on(t, !t->is_on);
			} else {
				ts_set_goal(t, t->is_on ? 1.0f : 0.0f);
			}
			return 1;
		}
		case ZST_IN_CAPTURE_LOST:
			t->pressed = 0;
			t->dragging = 0;
			ts_set_goal(t, t->is_on ? 1.0f : 0.0f);
			return 0;
		case ZST_IN_KEY_DOWN:
			if (in->key != Z_KEY_SPACE) return 0;
			if (zrn_effectively_enabled(e)) ts_set_on(t, !t->is_on);
			return 1;
		default:
			return 0;
	}
}

static void ts_init(zrn_element_t* const e) {
	zrn_toggle_switch_t* const t = TSW(e);
	zrn_control_t* const c = &t->base;
	c->padding = Zthickness(0, 0, 25.5f, 0);
	c->font_family = Z_FONT_SEMIBOLD;
	c->font_size = 12.0f;
	c->h_content = Z_HALIGN_LEFT;
	c->v_content = Z_VALIGN_TOP;
	e->halign = Z_HALIGN_STRETCH;
	e->valign = Z_VALIGN_CENTER;
	t->on_content = zrn_strdup("On");
	t->off_content = zrn_strdup("Off");
}

static void ts_destroy(zrn_element_t* const e) {
	const zrn_toggle_switch_t* const t = TSW(e);
	free(t->header);
	free(t->on_content);
	free(t->off_content);
}

zrn_class_t zrn_class_toggle_switch = {
	.base = &zrn_class_control, .name = "ToggleSwitch", .id = Z_CLASS_TOGGLE_SWITCH, .size = sizeof(zrn_toggle_switch_t),
	.init = ts_init, .destroy = ts_destroy, .measure = ts_measure, .paint = ts_paint, .input = ts_input, .tick = ts_tick
};

Zid Ztoggle_switch_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_TOGGLE_SWITCH);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_toggle_switch_t* get_ts(Zid id) { return TSW(zrn_get(id, Z_CLASS_TOGGLE_SWITCH)); }

void Ztoggle_switch_set_is_on(const Zid id, const int is_on) {
	zrn_toggle_switch_t* const t = get_ts(id);
	if (t) ts_set_on(t, is_on);
}

int Ztoggle_switch_get_is_on(const Zid id) {
	const zrn_toggle_switch_t* const t = get_ts(id);
	return t ? t->is_on : 0;
}

static void ts_set_text(zrn_toggle_switch_t* const t, char** slot, const char* const text) {
	zrn_set_string(slot, text && *text ? text : nullptr);
	zrn_changed_layout(&t->base.base);
}

void Ztoggle_switch_set_header(const Zid id, const char* const header) {
	zrn_toggle_switch_t* const t = get_ts(id);
	if (t) ts_set_text(t, &t->header, header);
}

const char* Ztoggle_switch_get_header(const Zid id) {
	const zrn_toggle_switch_t* const t = get_ts(id);
	return t && t->header ? t->header : "";
}

void Ztoggle_switch_set_on_content(const Zid id, const char* const content) {
	zrn_toggle_switch_t* const t = get_ts(id);
	if (t) ts_set_text(t, &t->on_content, content);
}

const char* Ztoggle_switch_get_on_content(const Zid id) {
	const zrn_toggle_switch_t* const t = get_ts(id);
	return t && t->on_content ? t->on_content : "";
}

void Ztoggle_switch_set_off_content(const Zid id, const char* const content) {
	zrn_toggle_switch_t* const t = get_ts(id);
	if (t) ts_set_text(t, &t->off_content, content);
}

const char* Ztoggle_switch_get_off_content(const Zid id) {
	const zrn_toggle_switch_t* const t = get_ts(id);
	return t && t->off_content ? t->off_content : "";
}

void Ztoggle_switch_on_toggled(Zid id, ZRoutedCallback callback, void* user_data) { Zelement_on(id, Z_EVENT_TOGGLED, callback, user_data); }
