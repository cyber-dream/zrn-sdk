/**
 * Kestrel - controls/buttons.c
 * ButtonBase, Button, HyperlinkButton, RepeatButton, ToggleButton, CheckBox,
 * RadioButton and ToggleSwitch, after the Windows Phone 8.1 templates
 * (generic.xaml of the phone SDK): their numbers are used as they are.
 */

#include "../internal.h"


/* ============================================================================
 *  ButtonBase
 * ============================================================================ */

#define BUTTON(e) KST_AS(kst_button_base_t, e)
#define TOGGLE(e) KST_AS(kst_toggle_button_t, e)

static void toggle_on_click(kst_element_t* e);

/* A Click handler may destroy the button (a tap that opens another page): nothing touches it afterwards. */
static void button_click(kst_element_t* e) {
	Kid id = e->id;
	if (kst_is_a(e, K_CLASS_TOGGLE_BUTTON)) {
		toggle_on_click(e);
		if (!kst_lookup(id)) return;
	}
	if (!kst_raise(e, K_EVENT_CLICK)) return;
	/* Button.Flyout: the tap shows it; a command of an app bar that is a secondary one closes the bar */
	if (kst_is_a(e, K_CLASS_BUTTON) && BUTTON(e)->flyout) kst_flyout_show_at(BUTTON(e)->flyout, e);
	if (!kst_lookup(id)) return;
	if (kst_is_a(e, K_CLASS_APP_BAR_BUTTON)) kst_app_bar_item_clicked(e);
}

static int inside(const kst_element_t* e, float x, float y) { return x >= 0 && y >= 0 && x < e->rect.width && y < e->rect.height; }

/* IsPressed of the template: the button is held and the pointer is on it. */
static int button_is_pressed(const kst_button_base_t* b) { return b->pressed && b->pointer_over; }

static void button_set_pressed(kst_button_base_t* b, int pressed, int over) {
	if (b->pressed == pressed && b->pointer_over == over) return;
	b->pressed = pressed;
	b->pointer_over = over;
	kst_changed_paint(&b->base.base.base);
}

static int button_input(kst_element_t* e, kst_input_t* in) {
	kst_button_base_t* b = BUTTON(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			if (!kst_effectively_enabled(e)) return 1;
			button_set_pressed(b, 1, 1);
			if (e->root) kst_set_capture(e->root, e);
			if (b->repeat) {
				b->repeat_timer = 0;
				b->repeating = 0;
				e->wants_tick = 1;
				button_click(e);
				return 1;
			}
			if (b->click_mode == K_CLICK_PRESS) button_click(e);
			return 1;
		case KST_IN_POINTER_MOVE:
			if (b->pressed) button_set_pressed(b, 1, inside(e, in->x, in->y));
			return b->pressed;
		case KST_IN_POINTER_UP: {
			int was = b->pressed, over = inside(e, in->x, in->y);
			if (!was) return 0;
			button_set_pressed(b, 0, over);
			e->wants_tick = 0;
			if (e->root) kst_release_capture(e->root, e);
			if (over && b->click_mode == K_CLICK_RELEASE && !b->repeat) button_click(e);
			return 1;
		}
		case KST_IN_POINTER_ENTER:
			if (!b->pressed) {
				b->pointer_over = 1;
				kst_changed_paint(e);
				if (b->click_mode == K_CLICK_HOVER && kst_effectively_enabled(e)) button_click(e);
			}
			return 0;
		case KST_IN_POINTER_LEAVE:
			if (!b->pressed) {
				b->pointer_over = 0;
				kst_changed_paint(e);
			}
			return 0;
		case KST_IN_CAPTURE_LOST:
			b->pressed = 0;
			b->keyboard_pressed = 0;
			e->wants_tick = 0;
			kst_changed_paint(e);
			return 0;
		case KST_IN_KEY_DOWN:
			if (in->key == K_KEY_SPACE || in->key == K_KEY_ENTER) {
				if (!kst_effectively_enabled(e)) return 1;
				b->keyboard_pressed = 1;
				button_set_pressed(b, 1, 1);
				if (b->click_mode == K_CLICK_PRESS) button_click(e);
				return 1;
			}
			return 0;
		case KST_IN_KEY_UP:
			if ((in->key == K_KEY_SPACE || in->key == K_KEY_ENTER) && b->keyboard_pressed) {
				b->keyboard_pressed = 0;
				button_set_pressed(b, 0, e->pointer_over);
				if (b->click_mode == K_CLICK_RELEASE) button_click(e);
				return 1;
			}
			return 0;
		case KST_IN_FOCUS_OUT:
			b->keyboard_pressed = 0;
			return 0;
		default:
			return 0;
	}
}

static void button_tick(kst_element_t* e, float dt) {
	kst_button_base_t* b = BUTTON(e);
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
		Kid id = e->id;
		b->repeat_timer -= (float)b->interval_ms;
		button_click(e);
		if (!kst_lookup(id) || b->interval_ms <= 0) break;
	}
}

/* PhoneTouchTargetOverhang is 0,9.5: the Border of the template has that margin, above and below the outline only. */
static KThickness button_chrome(kst_element_t* e) {
	KST_UNUSED(e);
	return Kthickness(0, KST_81_TOUCH_OVERHANG, 0, KST_81_TOUCH_OVERHANG);
}

/* The foreground of the content: ButtonPressedForeground while held, ButtonDisabledForeground (PhoneBaseLow) when disabled. */
static KColor button_content_foreground(kst_element_t* e) {
	kst_button_base_t* b = BUTTON(e);
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_BASE_LOW);
	if (button_is_pressed(b)) return kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT);
	return kst_ctl_foreground(&b->base.base);
}

/* The Button template: a Border (BorderThickness 2.5) with the touch overhang above and below it. */
static void button_paint(kst_element_t* e, kst_painter_t* p) {
	kst_button_base_t* b = BUTTON(e);
	kst_control_t* c = &b->base.base;
	KRect r = kst_rect(0, KST_81_TOUCH_OVERHANG, e->rect.width, e->rect.height - 2 * KST_81_TOUCH_OVERHANG);
	KColor background = kst_ctl_background(c, Kcolor_rgba(0, 0, 0, 0));
	KColor border = kst_ctl_border_brush(c, kst_ctl_foreground(c));
	if (!kst_effectively_enabled(e)) {
		background = Kcolor_rgba(0, 0, 0, 0);
		border = kst_theme_color(e, KST_TC_BASE_LOW);
	} else if (button_is_pressed(b)) {
		background = kst_theme_color(e, KST_TC_ACCENT);
	}
	if (background.a > 0) kst_fill_rect(p, r, background);
	kst_stroke_rect(p, r, c->border_thickness, border);
}

/* The style of Button, RepeatButton and ToggleButton; the classes below it change what they differ in. */
static void button_init(kst_element_t* e) {
	kst_button_base_t* b = BUTTON(e);
	kst_control_t* c = &b->base.base;
	c->border_thickness = Kthickness_uniform(KST_81_BORDER_THICKNESS);
	c->padding = Kthickness(9.5f, 0, 9.5f, 3.5f);
	c->font_family = K_FONT_SEMIBOLD;
	c->font_size = KST_81_FONT_LARGE;
	e->min_width = 109.0f;
	e->min_height = 57.5f;
	e->halign = K_HALIGN_LEFT;
	e->valign = K_VALIGN_CENTER;
	b->click_mode = K_CLICK_RELEASE;
	b->delay_ms = 250;
	b->interval_ms = 250;
}

static void button_destroy(kst_element_t* e) { free(BUTTON(e)->uri); }

static void button_child_removed(kst_element_t* e, kst_element_t* child) {
	if (BUTTON(e)->flyout == child) BUTTON(e)->flyout = NULL;
	else kst_cc_child_removed(e, child);
}

kst_class_t kst_class_button_base = {
	.base = &kst_class_content_control, .name = "ButtonBase", .id = K_CLASS_BUTTON_BASE, .size = sizeof(kst_button_base_t), .abstract_class = 1,
	.init = button_init, .destroy = button_destroy, .child_removed = button_child_removed, .input = button_input, .tick = button_tick, .chrome = button_chrome,
	.paint = button_paint, .content_foreground = button_content_foreground
};

kst_class_t kst_class_button = {
	.base = &kst_class_button_base, .name = "Button", .id = K_CLASS_BUTTON, .size = sizeof(kst_button_base_t)
};

static void repeat_init(kst_element_t* e) { BUTTON(e)->repeat = 1; }

kst_class_t kst_class_repeat_button = {
	.base = &kst_class_button_base, .name = "RepeatButton", .id = K_CLASS_REPEAT_BUTTON, .size = sizeof(kst_button_base_t),
	.init = repeat_init
};

static kst_element_t* make_button(KClass cls, const char* content) {
	kst_element_t* e = kst_create(cls);
	if (!e) return NULL;
	if (content) kst_content_set_text(KST_AS(kst_content_control_t, e), content);
	return e;
}

Kid Kbutton_create(const char* content) {
	kst_element_t* e = make_button(K_CLASS_BUTTON, content);
	return e ? e->id : K_INVALID_ID;
}

Kid Krepeat_button_create(const char* content) {
	kst_element_t* e = make_button(K_CLASS_REPEAT_BUTTON, content);
	return e ? e->id : K_INVALID_ID;
}

static kst_button_base_t* get_button(Kid id) { return BUTTON(kst_get(id, K_CLASS_BUTTON_BASE)); }

void Kbutton_on_click(Kid id, KRoutedCallback callback, void* user_data) { Kelement_on(id, K_EVENT_CLICK, callback, user_data); }

void Kbutton_set_click_mode(Kid id, KClickMode mode) {
	kst_button_base_t* b = get_button(id);
	if (b) b->click_mode = mode;
}

int Kbutton_is_pressed(Kid id) {
	kst_button_base_t* b = get_button(id);
	return b ? button_is_pressed(b) : 0;
}

int Kbutton_is_pointer_over(Kid id) {
	kst_button_base_t* b = get_button(id);
	return b ? b->base.base.base.pointer_over != 0 : 0;
}

void Kbutton_click(Kid id) {
	kst_button_base_t* b = get_button(id);
	if (b && kst_effectively_enabled(&b->base.base.base)) button_click(&b->base.base.base);
}

void Krepeat_button_set_delay(Kid id, int delay_ms) {
	kst_element_t* e = kst_get(id, K_CLASS_REPEAT_BUTTON);
	if (e) BUTTON(e)->delay_ms = delay_ms < 0 ? 0 : delay_ms;
}

int Krepeat_button_get_delay(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_REPEAT_BUTTON);
	return e ? BUTTON(e)->delay_ms : 0;
}

void Krepeat_button_set_interval(Kid id, int interval_ms) {
	kst_element_t* e = kst_get(id, K_CLASS_REPEAT_BUTTON);
	if (e) BUTTON(e)->interval_ms = interval_ms < 0 ? 0 : interval_ms;
}

int Krepeat_button_get_interval(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_REPEAT_BUTTON);
	return e ? BUTTON(e)->interval_ms : 0;
}


/* ============================================================================
 *  HyperlinkButton
 *  Text only: no outline, no touch overhang, no underline and no color of its
 *  own while pressed (the template only runs PointerDownThemeAnimation, the
 *  lean of Kelement_set_tilt); HyperlinkDisabledThemeBrush when disabled.
 * ============================================================================ */

static KThickness hyperlink_chrome(kst_element_t* e) {
	KST_UNUSED(e);
	return kst_thickness_zero();
}

static KColor hyperlink_content_foreground(kst_element_t* e) {
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_DISABLED);
	return kst_ctl_foreground(&BUTTON(e)->base.base);
}

static void hyperlink_paint(kst_element_t* e, kst_painter_t* p) {
	kst_control_t* c = &BUTTON(e)->base.base;
	/* HyperlinkButtonBackgroundThemeBrush is transparent: only a color the program set shows */
	if (c->set_flags & KST_C_BACKGROUND) kst_fill_rect(p, kst_local_rect(e), c->background);
}

static void hyperlink_init(kst_element_t* e) {
	kst_control_t* c = &BUTTON(e)->base.base;
	c->border_thickness = kst_thickness_zero();
	c->padding = Kthickness(0, 3, 0, 4);
	c->font_family = K_FONT_SEMILIGHT;
	c->font_size = KST_81_FONT_EXTRA_LARGE_PLUS;
	e->min_width = 0;
	e->min_height = 0;
	e->halign = K_HALIGN_LEFT;
	e->valign = K_VALIGN_CENTER;
}

kst_class_t kst_class_hyperlink_button = {
	.base = &kst_class_button_base, .name = "HyperlinkButton", .id = K_CLASS_HYPERLINK_BUTTON, .size = sizeof(kst_button_base_t),
	.init = hyperlink_init, .chrome = hyperlink_chrome, .paint = hyperlink_paint,
	.content_foreground = hyperlink_content_foreground
};

Kid Khyperlink_button_create(const char* content) {
	kst_element_t* e = make_button(K_CLASS_HYPERLINK_BUTTON, content);
	return e ? e->id : K_INVALID_ID;
}

void Khyperlink_button_set_uri(Kid id, const char* uri) {
	kst_element_t* e = kst_get(id, K_CLASS_HYPERLINK_BUTTON);
	if (e) kst_set_string(&BUTTON(e)->uri, uri);
}

const char* Khyperlink_button_get_uri(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_HYPERLINK_BUTTON);
	return e && BUTTON(e)->uri ? BUTTON(e)->uri : "";
}


/* ============================================================================
 *  ToggleButton
 * ============================================================================ */

static void toggle_find_radios(kst_element_t* e, kst_element_t*** list, int* count, int* cap) {
	int i;
	if (e->cls == &kst_class_radio_button) {
		if (*count == *cap) {
			int ncap = *cap ? *cap * 2 : 8;
			kst_element_t** more = (kst_element_t**)realloc(*list, (size_t)ncap * sizeof *more);
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
static int radio_exclude(kst_element_t* e, Kid** out_ids) {
	kst_toggle_button_t* me = TOGGLE(e);
	kst_element_t* top = e;
	kst_element_t** list = NULL;
	Kid* ids = NULL;
	int count = 0, cap = 0, i, n = 0;
	while (top->parent) top = top->parent;
	toggle_find_radios(top, &list, &count, &cap);
	if (count > 0) ids = (Kid*)malloc((size_t)count * sizeof *ids);
	for (i = 0; i < count && ids; i++) {
		kst_toggle_button_t* other = TOGGLE(list[i]);
		int same;
		if (list[i] == e || other->state != K_CHECKED) continue;
		if (me->group_name && *me->group_name) same = other->group_name && strcmp(other->group_name, me->group_name) == 0;
		else same = !(other->group_name && *other->group_name) && list[i]->parent == e->parent;
		if (same) {
			other->state = K_UNCHECKED;
			kst_changed_paint(list[i]);
			ids[n++] = list[i]->id;
		}
	}
	free(list);
	*out_ids = ids;
	return n;
}

static void toggle_set_state(kst_element_t* e, KToggleState state) {
	kst_toggle_button_t* t = TOGGLE(e);
	Kid* others = NULL;
	int n = 0, i;
	if (t->state == state) return;
	t->state = state;
	if (state == K_CHECKED && e->cls == &kst_class_radio_button) n = radio_exclude(e, &others);
	kst_changed_paint(e);
	kst_raise(e, state == K_CHECKED ? K_EVENT_CHECKED : (state == K_UNCHECKED ? K_EVENT_UNCHECKED : K_EVENT_INDETERMINATE));
	for (i = 0; i < n; i++) {
		kst_element_t* other = kst_lookup(others[i]);
		if (other) kst_raise(other, K_EVENT_UNCHECKED);
	}
	free(others);
}

static void toggle_on_click(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	if (e->cls == &kst_class_radio_button) {
		toggle_set_state(e, K_CHECKED);
		return;
	}
	switch (t->state) {
		case K_CHECKED: toggle_set_state(e, t->three_state ? K_INDETERMINATE : K_UNCHECKED); break;
		case K_UNCHECKED: toggle_set_state(e, K_CHECKED); break;
		default: toggle_set_state(e, K_UNCHECKED); break;
	}
}

/*
 * A plain ToggleButton is a Button with no pressed look, that fills with the accent while checked
 * (text over accent). Disabled it is the outline and text of PhoneBaseLow; checked and disabled it
 * is the checked look at 0.4 opacity.
 */
static KColor toggle_content_foreground(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	int enabled = kst_effectively_enabled(e);
	if (t->state == K_CHECKED) return kst_color_alpha(kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT), enabled ? 1.0f : 0.4f);
	if (!enabled) return kst_theme_color(e, KST_TC_BASE_LOW);
	return kst_ctl_foreground(&t->base.base.base);
}

static void toggle_paint(kst_element_t* e, kst_painter_t* p) {
	kst_toggle_button_t* t = TOGGLE(e);
	kst_control_t* c = &t->base.base.base;
	KRect r = kst_rect(0, KST_81_TOUCH_OVERHANG, e->rect.width, e->rect.height - 2 * KST_81_TOUCH_OVERHANG);
	KColor background = kst_ctl_background(c, Kcolor_rgba(0, 0, 0, 0));
	KColor border = kst_ctl_border_brush(c, kst_ctl_foreground(c));
	int enabled = kst_effectively_enabled(e);
	if (t->state == K_CHECKED) {
		background = kst_theme_color(e, KST_TC_ACCENT);
		if (!enabled) {
			background = kst_color_alpha(background, 0.4f);
			border = kst_color_alpha(border, 0.4f);
		}
	} else if (!enabled) {
		background = Kcolor_rgba(0, 0, 0, 0);
		border = kst_theme_color(e, KST_TC_BASE_LOW);
	}
	if (background.a > 0) kst_fill_rect(p, r, background);
	kst_stroke_rect(p, r, c->border_thickness, border);
}

static void toggle_init(kst_element_t* e) { TOGGLE(e)->state = K_UNCHECKED; }
static void toggle_destroy(kst_element_t* e) { free(TOGGLE(e)->group_name); }

kst_class_t kst_class_toggle_button = {
	.base = &kst_class_button_base, .name = "ToggleButton", .id = K_CLASS_TOGGLE_BUTTON, .size = sizeof(kst_toggle_button_t),
	.init = toggle_init, .destroy = toggle_destroy, .paint = toggle_paint, .content_foreground = toggle_content_foreground
};

Kid Ktoggle_button_create(const char* content) {
	kst_element_t* e = make_button(K_CLASS_TOGGLE_BUTTON, content);
	return e ? e->id : K_INVALID_ID;
}

static kst_toggle_button_t* get_toggle(Kid id) { return TOGGLE(kst_get(id, K_CLASS_TOGGLE_BUTTON)); }

void Ktoggle_button_set_state(Kid id, KToggleState state) {
	kst_toggle_button_t* t = get_toggle(id);
	if (t) toggle_set_state(&t->base.base.base.base, state);
}

void Ktoggle_button_set_is_checked(Kid id, int is_checked) { Ktoggle_button_set_state(id, is_checked ? K_CHECKED : K_UNCHECKED); }
KToggleState Ktoggle_button_get_state(Kid id) { kst_toggle_button_t* t = get_toggle(id); return t ? t->state : K_UNCHECKED; }
int Ktoggle_button_get_is_checked(Kid id) { return Ktoggle_button_get_state(id) == K_CHECKED; }

void Ktoggle_button_set_is_three_state(Kid id, int three_state) {
	kst_toggle_button_t* t = get_toggle(id);
	if (t) t->three_state = three_state != 0;
}


/* ============================================================================
 *  CheckBox and RadioButton: a 25.5 px box with the content after it, and
 *  PhoneTouchTargetLargeOverhang (0,16) above and below
 * ============================================================================ */

#define BOX 25.5f
#define CHECK_MARK_W 18.5f
#define CHECK_MARK_H 17.0f
#define RADIO_MARK 12.5f

/* The box is in the chrome; for a CheckBox the border thickness (it is the border of the box) is taken back out of the insets. */
static KThickness box_chrome(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	KThickness b = c->border_thickness;
	return Kthickness(BOX - b.left, KST_81_TOUCH_LARGE_OVERHANG - b.top, -b.right, KST_81_TOUCH_LARGE_OVERHANG - b.bottom);
}

static KSize box_measure(kst_element_t* e, KSize available) {
	KSize s = kst_cc_measure(e, available);
	/* the row is as high as the box with its overhang */
	s.height = kst_maxf(s.height, BOX + 2 * KST_81_TOUCH_LARGE_OVERHANG);
	s.width = kst_maxf(s.width, BOX);
	return s;
}

/* The common style of CheckBox and RadioButton: Padding 8,0,0,0, MinWidth 168, top left. */
static void box_style_init(kst_element_t* e) {
	kst_control_t* c = &TOGGLE(e)->base.base.base;
	c->padding = Kthickness(8, 0, 0, 0);
	c->font_family = K_FONT_NORMAL;
	c->font_size = KST_81_FONT_LARGE;
	c->h_content = K_HALIGN_LEFT;
	c->v_content = K_VALIGN_TOP;
	e->min_width = 168.0f;
	e->min_height = 0;
	e->halign = K_HALIGN_LEFT;
	e->valign = K_VALIGN_TOP;
}

static void checkbox_init(kst_element_t* e) {
	box_style_init(e);
	TOGGLE(e)->base.base.base.border_thickness = Kthickness_uniform(KST_81_BORDER_THICKNESS);
}

static void radio_init(kst_element_t* e) {
	box_style_init(e);
	/* the circle has the PhoneStrokeThickness of its own, not the BorderThickness of the control */
	TOGGLE(e)->base.base.base.border_thickness = kst_thickness_zero();
}

static KRect box_rect(void) { return kst_rect(0, KST_81_TOUCH_LARGE_OVERHANG, BOX, BOX); }

static KColor box_border(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_BASE_LOW);
	return kst_ctl_border_brush(&t->base.base.base, kst_theme_color(e, KST_TC_FOREGROUND));
}

/* The check, the dash and the dot: the foreground of the theme, over the accent while pressed. */
static KColor box_mark(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_BASE_LOW);
	if (button_is_pressed(&t->base)) return kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT);
	return kst_theme_color(e, KST_TC_FOREGROUND);
}

static KColor box_fill(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	if (kst_effectively_enabled(e) && button_is_pressed(&t->base)) return kst_theme_color(e, KST_TC_ACCENT);
	return kst_ctl_background(&t->base.base.base, Kcolor_rgba(0, 0, 0, 0));
}

static void checkbox_paint(kst_element_t* e, kst_painter_t* p) {
	kst_toggle_button_t* t = TOGGLE(e);
	kst_control_t* c = &t->base.base.base;
	KRect box = box_rect();
	KColor fill = box_fill(e), mark = box_mark(e);
	if (fill.a > 0) kst_fill_rect(p, box, fill);
	kst_stroke_rect(p, box, c->border_thickness, box_border(e));
	if (t->state == K_CHECKED) {
		/* Path "M0,123 L39,93 L124,164 L256,18 L295,49 L124,240 z", Stretch Fill into 18.5 x 17 at the center of the box */
		static const float path[12] = { 0, 123, 39, 93, 124, 164, 256, 18, 295, 49, 124, 240 };
		float pts[12];
		int i;
		for (i = 0; i < 6; i++) {
			pts[i * 2] = box.x + (BOX - CHECK_MARK_W) * 0.5f + path[i * 2] / 295.0f * CHECK_MARK_W;
			pts[i * 2 + 1] = box.y + (BOX - CHECK_MARK_H) * 0.5f + (path[i * 2 + 1] - 18.0f) / 222.0f * CHECK_MARK_H;
		}
		kst_fill_polygon(p, pts, 6, mark);
	} else if (t->state == K_INDETERMINATE) {
		/* NormalRectangle, 13 x 13 */
		kst_fill_rect(p, kst_rect(box.x + (BOX - 13.0f) * 0.5f, box.y + (BOX - 13.0f) * 0.5f, 13, 13), mark);
	}
}

static void radio_paint(kst_element_t* e, kst_painter_t* p) {
	kst_toggle_button_t* t = TOGGLE(e);
	KRect box = box_rect();
	KColor fill = box_fill(e);
	/* the stroke of an Ellipse is inside its bounds */
	if (fill.a > 0) kst_fill_ellipse(p, kst_rect(box.x + KST_81_BORDER_THICKNESS * 0.5f, box.y + KST_81_BORDER_THICKNESS * 0.5f, BOX - KST_81_BORDER_THICKNESS, BOX - KST_81_BORDER_THICKNESS), fill);
	kst_stroke_ellipse(p, box, KST_81_BORDER_THICKNESS, box_border(e));
	if (t->state == K_CHECKED) kst_fill_ellipse(p, kst_rect(box.x + (BOX - RADIO_MARK) * 0.5f, box.y + (BOX - RADIO_MARK) * 0.5f, RADIO_MARK, RADIO_MARK), box_mark(e));
}

static KColor box_content_foreground(kst_element_t* e) {
	kst_toggle_button_t* t = TOGGLE(e);
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_BASE_LOW);
	return kst_ctl_foreground(&t->base.base.base);
}

kst_class_t kst_class_checkbox = {
	.base = &kst_class_toggle_button, .name = "CheckBox", .id = K_CLASS_CHECKBOX, .size = sizeof(kst_toggle_button_t),
	.init = checkbox_init, .measure = box_measure, .chrome = box_chrome, .paint = checkbox_paint,
	.content_foreground = box_content_foreground
};

kst_class_t kst_class_radio_button = {
	.base = &kst_class_toggle_button, .name = "RadioButton", .id = K_CLASS_RADIO_BUTTON, .size = sizeof(kst_toggle_button_t),
	.init = radio_init, .measure = box_measure, .chrome = box_chrome, .paint = radio_paint,
	.content_foreground = box_content_foreground
};

Kid Kcheck_box_create(const char* content) {
	kst_element_t* e = make_button(K_CLASS_CHECKBOX, content);
	return e ? e->id : K_INVALID_ID;
}

Kid Kradio_button_create(const char* content, const char* group_name) {
	kst_element_t* e = make_button(K_CLASS_RADIO_BUTTON, content);
	if (!e) return K_INVALID_ID;
	if (group_name && *group_name) kst_set_string(&TOGGLE(e)->group_name, group_name);
	return e->id;
}

void Kradio_button_set_group_name(Kid id, const char* group_name) {
	kst_element_t* e = kst_get(id, K_CLASS_RADIO_BUTTON);
	if (e) kst_set_string(&TOGGLE(e)->group_name, group_name && *group_name ? group_name : NULL);
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

#define TSW(e) KST_AS(kst_toggle_switch_t, e)
#define TS_SWITCH_W 77.0f
#define TS_SWITCH_H 36.0f
#define TS_GAP 13.5f
#define TS_BOTTOM_ROW 9.5f
#define TS_KNOB_W 20.5f
#define TS_KNOB_TRAVEL 56.5f
#define TS_CURTAIN_W 64.0f
#define TS_OUTLINE 2.5f
/** The animation of the knob, in ms. */
#define TS_ANIMATION_MS 200.0f

static kst_font_t ts_header_font(void) { kst_font_t f = { K_FONT_NORMAL, KST_81_FONT_CONTROL_HEADER, 0 }; return f; }
static kst_font_t ts_content_font(void) { kst_font_t f = { K_FONT_NORMAL, KST_81_FONT_EXTRA_LARGE, 0 }; return f; }

static int ts_has(const char* s) { return s && *s; }

typedef struct ts_geometry {
	float header_row, content_row, region;
	/** The 77 x 36 box of the switch, in local coordinates. */
	KRect box;
} ts_geometry_t;

static void ts_geometry(const kst_element_t* e, ts_geometry_t* g) {
	const kst_toggle_switch_t* t = TSW((kst_element_t*)e);
	KThickness pad = t->base.padding;
	g->header_row = ts_has(t->header) ? kst_font_line_height(ts_header_font()) + kst_thickness_v(pad) : 0;
	g->content_row = ts_has(t->on_content) || ts_has(t->off_content) ? kst_font_line_height(ts_content_font()) + kst_thickness_v(pad) : 0;
	g->region = kst_maxf(g->header_row + g->content_row, TS_SWITCH_H);
	g->box = kst_rect(e->rect.width - TS_SWITCH_W, g->region - TS_SWITCH_H, TS_SWITCH_W, TS_SWITCH_H);
}

static KSize ts_measure(kst_element_t* e, KSize available) {
	kst_toggle_switch_t* t = TSW(e);
	KThickness pad = t->base.padding;
	ts_geometry_t g;
	float w = 0;
	ts_geometry(e, &g);
	if (ts_has(t->header)) w = kst_maxf(w, kst_text_width(ts_header_font(), t->header, (int)strlen(t->header)));
	if (ts_has(t->on_content)) w = kst_maxf(w, kst_text_width(ts_content_font(), t->on_content, (int)strlen(t->on_content)));
	if (ts_has(t->off_content)) w = kst_maxf(w, kst_text_width(ts_content_font(), t->off_content, (int)strlen(t->off_content)));
	w += kst_thickness_h(pad) + TS_GAP + TS_SWITCH_W;
	if (available.width < K_INFINITE * 0.5f) w = kst_minf(w, available.width);
	return kst_size(w, g.region + TS_BOTTOM_ROW);
}

static void ts_paint(kst_element_t* e, kst_painter_t* p) {
	kst_toggle_switch_t* t = TSW(e);
	KThickness pad = t->base.padding;
	ts_geometry_t g;
	int enabled = kst_effectively_enabled(e);
	KColor foreground = kst_theme_color(e, KST_TC_FOREGROUND), text_color, outline, curtain, knob_fill;
	KRect outer, inner, curtain_rect;
	float knob_x, curtain_x;
	const char* content = t->is_on ? t->on_content : t->off_content;
	ts_geometry(e, &g);
	if (t->base.set_flags & KST_C_FOREGROUND) foreground = t->base.foreground;
	text_color = enabled ? foreground : kst_theme_color(e, KST_TC_BASE_LOW);
	if (t->base.set_flags & KST_C_BACKGROUND) kst_fill_rect(p, kst_local_rect(e), t->base.background);
	if (ts_has(t->header)) {
		kst_draw_string(p, ts_header_font(), t->header, (int)strlen(t->header), pad.left, pad.top,
			enabled ? kst_theme_color(e, KST_TC_BASE_MID) : kst_theme_color(e, KST_TC_DISABLED));
	}
	if (ts_has(content)) kst_draw_string(p, ts_content_font(), content, (int)strlen(content), pad.left, g.header_row + pad.top, text_color);

	/* the switch */
	outline = enabled ? kst_theme_color(e, KST_TC_FOREGROUND) : kst_theme_color(e, KST_TC_BASE_LOW);
	curtain = enabled ? kst_theme_color(e, KST_TC_ACCENT) : kst_theme_color(e, KST_TC_BASE_MEDIUM_MID_SOLID);
	knob_fill = enabled ? kst_theme_color(e, KST_TC_FOREGROUND) : kst_theme_color(e, KST_TC_BASE_MEDIUM_MID_SOLID);
	outer = kst_rect(g.box.x + 3.0f, g.box.y + 4.5f, TS_SWITCH_W - 6.0f, TS_SWITCH_H - 9.0f);
	/* the inner border of the template has a transparent brush, so the curtain lies 2.5 px inside the outline's inner edge */
	inner = kst_rect(outer.x + 2 * TS_OUTLINE, outer.y + 2 * TS_OUTLINE, outer.width - 4 * TS_OUTLINE, outer.height - 4 * TS_OUTLINE);
	kst_stroke_rect(p, outer, Kthickness_uniform(TS_OUTLINE), outline);
	curtain_x = -TS_CURTAIN_W + TS_CURTAIN_W * t->knob;
	curtain_rect = kst_rect(inner.x + curtain_x, inner.y, TS_CURTAIN_W, inner.height);
	if (curtain_rect.x < inner.x) {
		curtain_rect.width -= inner.x - curtain_rect.x;
		curtain_rect.x = inner.x;
	}
	if (curtain_rect.x + curtain_rect.width > inner.x + inner.width) curtain_rect.width = inner.x + inner.width - curtain_rect.x;
	if (curtain_rect.width > 0) kst_fill_rect(p, curtain_rect, curtain);
	knob_x = g.box.x + TS_KNOB_TRAVEL * t->knob;
	/* SwitchKnob: Fill is the foreground, the 2.5 px Stroke (inside its bounds) the background of the page */
	kst_fill_rect(p, kst_rect(knob_x, g.box.y, TS_KNOB_W, TS_SWITCH_H), kst_theme_color(e, KST_TC_BACKGROUND));
	kst_fill_rect(p, kst_rect(knob_x + TS_OUTLINE, g.box.y + TS_OUTLINE, TS_KNOB_W - 2 * TS_OUTLINE, TS_SWITCH_H - 2 * TS_OUTLINE), knob_fill);
}

static void ts_set_goal(kst_toggle_switch_t* t, float goal) {
	kst_element_t* e = &t->base.base;
	t->knob_goal = goal;
	if (!kst_animations_enabled(e->root)) {
		t->knob = goal;
		e->wants_tick = 0;
	} else if (t->knob != goal) {
		e->wants_tick = 1;
	}
	kst_changed_paint(e);
}

/* @return K_FALSE when a Toggled handler destroyed the switch. */
static int ts_set_on(kst_toggle_switch_t* t, int on) {
	kst_element_t* e = &t->base.base;
	on = on != 0;
	if (t->is_on == on) return 1;
	t->is_on = on;
	ts_set_goal(t, on ? 1.0f : 0.0f);
	kst_changed_paint(e);
	return kst_raise(e, K_EVENT_TOGGLED);
}

static void ts_tick(kst_element_t* e, float dt) {
	kst_toggle_switch_t* t = TSW(e);
	float step = dt / TS_ANIMATION_MS;
	if (t->dragging) return;
	if (t->knob < t->knob_goal) t->knob = kst_minf(t->knob + step, t->knob_goal);
	else if (t->knob > t->knob_goal) t->knob = kst_maxf(t->knob - step, t->knob_goal);
	if (t->knob == t->knob_goal) e->wants_tick = 0;
	kst_changed_paint(e);
}

/* The Thumb of the template covers the knob with margins -13.5,-15.5,-13.5,-6.5. */
static int ts_on_knob(const kst_element_t* e, float x, float y) {
	const kst_toggle_switch_t* t = TSW((kst_element_t*)e);
	ts_geometry_t g;
	float kx;
	ts_geometry(e, &g);
	kx = g.box.x + TS_KNOB_TRAVEL * t->knob;
	return x >= kx - 13.5f && x < kx + TS_KNOB_W + 13.5f && y >= g.box.y - 15.5f && y < g.box.y + TS_SWITCH_H + 6.5f;
}

static int ts_input(kst_element_t* e, kst_input_t* in) {
	kst_toggle_switch_t* t = TSW(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN: {
			ts_geometry_t box;
			/* only the switch itself takes a tap: the header, the On/Off text and the empty space pass it on */
			ts_geometry(e, &box);
			if (in->x < box.box.x || in->x >= box.box.x + box.box.width || in->y < box.box.y || in->y >= box.box.y + box.box.height) return 0;
			if (!kst_effectively_enabled(e)) return 1;
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
			if (e->root) kst_set_capture(e->root, e);
			return 1;
		}
		case KST_IN_POINTER_MOVE:
			if (!t->pressed) return 0;
			if (t->moved == -1 && fabsf(in->x - t->press_x) > 3.0f) t->dragging = 1;
			if (t->dragging) {
				ts_geometry_t g;
				ts_geometry(e, &g);
				t->knob = kst_clampf((in->x - t->grab - g.box.x) / TS_KNOB_TRAVEL, 0.0f, 1.0f);
				t->knob_goal = t->knob;
				kst_changed_paint(e);
			} else if (fabsf(in->x - t->press_x) > 3.0f) {
				t->moved = 1;
			}
			return 1;
		case KST_IN_POINTER_UP: {
			int was_dragging = t->dragging, inside_now = in->x >= 0 && in->y >= 0 && in->x < e->rect.width && in->y < e->rect.height;
			Kid id = e->id;
			if (!t->pressed) return 0;
			t->pressed = 0;
			t->dragging = 0;
			if (e->root) kst_release_capture(e->root, e);
			if (!kst_lookup(id)) return 1;
			if (was_dragging) {
				int on = t->knob >= 0.5f;
				if (on == t->is_on) ts_set_goal(t, on ? 1.0f : 0.0f);
				else ts_set_on(t, on);
			} else if (t->moved != 1 && inside_now) {
				ts_set_on(t, !t->is_on);
			} else {
				ts_set_goal(t, t->is_on ? 1.0f : 0.0f);
			}
			return 1;
		}
		case KST_IN_CAPTURE_LOST:
			t->pressed = 0;
			t->dragging = 0;
			ts_set_goal(t, t->is_on ? 1.0f : 0.0f);
			return 0;
		case KST_IN_KEY_DOWN:
			if (in->key != K_KEY_SPACE) return 0;
			if (kst_effectively_enabled(e)) ts_set_on(t, !t->is_on);
			return 1;
		default:
			return 0;
	}
}

static void ts_init(kst_element_t* e) {
	kst_toggle_switch_t* t = TSW(e);
	kst_control_t* c = &t->base;
	c->padding = Kthickness(0, 0, 25.5f, 0);
	c->font_family = K_FONT_SEMIBOLD;
	c->font_size = 12.0f;
	c->h_content = K_HALIGN_LEFT;
	c->v_content = K_VALIGN_TOP;
	e->halign = K_HALIGN_STRETCH;
	e->valign = K_VALIGN_CENTER;
	t->on_content = kst_strdup("On");
	t->off_content = kst_strdup("Off");
}

static void ts_destroy(kst_element_t* e) {
	kst_toggle_switch_t* t = TSW(e);
	free(t->header);
	free(t->on_content);
	free(t->off_content);
}

kst_class_t kst_class_toggle_switch = {
	.base = &kst_class_control, .name = "ToggleSwitch", .id = K_CLASS_TOGGLE_SWITCH, .size = sizeof(kst_toggle_switch_t),
	.init = ts_init, .destroy = ts_destroy, .measure = ts_measure, .paint = ts_paint, .input = ts_input, .tick = ts_tick
};

Kid Ktoggle_switch_create(void) {
	kst_element_t* e = kst_create(K_CLASS_TOGGLE_SWITCH);
	return e ? e->id : K_INVALID_ID;
}

static kst_toggle_switch_t* get_ts(Kid id) { return TSW(kst_get(id, K_CLASS_TOGGLE_SWITCH)); }

void Ktoggle_switch_set_is_on(Kid id, int is_on) {
	kst_toggle_switch_t* t = get_ts(id);
	if (t) ts_set_on(t, is_on);
}

int Ktoggle_switch_get_is_on(Kid id) {
	kst_toggle_switch_t* t = get_ts(id);
	return t ? t->is_on : 0;
}

static void ts_set_text(kst_toggle_switch_t* t, char** slot, const char* text) {
	kst_set_string(slot, text && *text ? text : NULL);
	kst_changed_layout(&t->base.base);
}

void Ktoggle_switch_set_header(Kid id, const char* header) {
	kst_toggle_switch_t* t = get_ts(id);
	if (t) ts_set_text(t, &t->header, header);
}

const char* Ktoggle_switch_get_header(Kid id) {
	kst_toggle_switch_t* t = get_ts(id);
	return t && t->header ? t->header : "";
}

void Ktoggle_switch_set_on_content(Kid id, const char* content) {
	kst_toggle_switch_t* t = get_ts(id);
	if (t) ts_set_text(t, &t->on_content, content);
}

const char* Ktoggle_switch_get_on_content(Kid id) {
	kst_toggle_switch_t* t = get_ts(id);
	return t && t->on_content ? t->on_content : "";
}

void Ktoggle_switch_set_off_content(Kid id, const char* content) {
	kst_toggle_switch_t* t = get_ts(id);
	if (t) ts_set_text(t, &t->off_content, content);
}

const char* Ktoggle_switch_get_off_content(Kid id) {
	kst_toggle_switch_t* t = get_ts(id);
	return t && t->off_content ? t->off_content : "";
}

void Ktoggle_switch_on_toggled(Kid id, KRoutedCallback callback, void* user_data) { Kelement_on(id, K_EVENT_TOGGLED, callback, user_data); }
