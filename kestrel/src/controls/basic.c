/**
 * Kestrel - controls/basic.c
 * TextBlock, Image, Popup, Control, ContentControl, UserControl, ToolTip
 * and Separator.
 */

#include "../internal.h"


/* ============================================================================
 *  TextBlock
 * ============================================================================ */

static kst_font_t tb_font(const kst_text_block_t* t) {
	kst_font_t f;
	f.size = (t->set_flags & KST_TB_FONT_SIZE) ? t->font_size : kst_inherited_font_size(&t->base);
	f.family = (t->set_flags & KST_TB_FONT_FAMILY) ? t->font_family : kst_inherited_font_family(&t->base);
	f.spacing = t->character_spacing;
	return f;
}

static KColor tb_foreground(const kst_text_block_t* t) {
	if (t->set_flags & KST_TB_FOREGROUND) return t->foreground;
	switch (t->fg_role) {
		case 1: return kst_theme_color(&t->base, KST_TC_SUBTLE);
		case 2: return kst_theme_color(&t->base, KST_TC_CONTRAST_FOREGROUND);
		case 3: return kst_theme_color(&t->base, KST_TC_ACCENT);
		default: break;
	}
	return kst_inherited_foreground(&t->base);
}

static void tb_drop_layout(kst_text_block_t* t) {
	kst_text_layout_free((kst_text_layout_t*)t->layout);
	t->layout = NULL;
}

/* The layout for a width; cached until the text, the font or the width changes. */
static kst_text_layout_t* tb_layout(kst_text_block_t* t, float width) {
	kst_text_layout_t* l = (kst_text_layout_t*)t->layout;
	kst_font_t font = tb_font(t);
	int wrap = t->wrapping == K_WRAP && width > 0 && width < K_INFINITE * 0.5f;
	if (l && (l->font.size != font.size || l->font.family != font.family || l->font.spacing != font.spacing || (wrap && t->layout_width != width) || (!wrap && t->layout_width != 0))) {
		tb_drop_layout(t);
		l = NULL;
	}
	if (!l) {
		const char* text = t->text ? t->text : "";
		l = kst_text_layout_new(font, text, (int)strlen(text), wrap ? width : 0, wrap, t->max_lines, t->line_height);
		t->layout = l;
		t->layout_width = wrap ? width : 0;
	}
	return l;
}

static KSize tb_measure(kst_element_t* e, KSize available) {
	kst_text_block_t* t = KST_AS(kst_text_block_t, e);
	kst_text_layout_t* l;
	tb_drop_layout(t);
	l = tb_layout(t, available.width);
	if (!l) return kst_size(0, 0);
	/* an empty text still takes a line, as in Silverlight */
	return kst_size(l->width, l->height);
}

static void tb_paint(kst_element_t* e, kst_painter_t* p) {
	kst_text_block_t* t = KST_AS(kst_text_block_t, e);
	kst_text_layout_t* l;
	KColor c;
	if (!t->text || !*t->text) return;
	l = tb_layout(t, e->rect.width);
	if (!l) return;
	c = tb_foreground(t);
	if (!kst_effectively_enabled(e)) c = kst_theme_color(e, KST_TC_DISABLED);
	kst_draw_text(p, l, 0, 0, e->rect.width, t->alignment, t->trimming, c);
}

static void tb_init(kst_element_t* e) {
	kst_text_block_t* t = KST_AS(kst_text_block_t, e);
	t->wrapping = K_NO_WRAP;
	t->alignment = K_TEXT_LEFT;
	t->trimming = K_TRIM_NONE;
	t->text = kst_strdup("");
}

static void tb_destroy(kst_element_t* e) {
	kst_text_block_t* t = KST_AS(kst_text_block_t, e);
	tb_drop_layout(t);
	free(t->text);
}

kst_class_t kst_class_text_block = {
	.base = &kst_class_element, .name = "TextBlock", .id = K_CLASS_TEXT_BLOCK, .size = sizeof(kst_text_block_t),
	.init = tb_init, .destroy = tb_destroy, .measure = tb_measure, .paint = tb_paint
};

Kid Ktext_block_create(const char* text) {
	kst_element_t* e = kst_create(K_CLASS_TEXT_BLOCK);
	if (!e) return K_INVALID_ID;
	kst_set_string(&KST_AS(kst_text_block_t, e)->text, text ? text : "");
	return e->id;
}

static kst_text_block_t* get_tb(Kid id) { return KST_AS(kst_text_block_t, kst_get(id, K_CLASS_TEXT_BLOCK)); }

void Ktext_block_set_text(Kid id, const char* text) {
	kst_text_block_t* t = get_tb(id);
	if (!t) return;
	if (t->text && text && strcmp(t->text, text) == 0) return;
	kst_set_string(&t->text, text ? text : "");
	tb_drop_layout(t);
	kst_changed_layout(&t->base);
}

const char* Ktext_block_get_text(Kid id) {
	kst_text_block_t* t = get_tb(id);
	return t && t->text ? t->text : "";
}

#define TB_SET(name, type, field, flag) \
	void name(Kid id, type value) { \
		kst_text_block_t* t = get_tb(id); \
		if (!t) return; \
		t->field = value; \
		if (flag) t->set_flags |= (flag); \
		tb_drop_layout(t); \
		kst_changed_layout(&t->base); \
	}

TB_SET(Ktext_block_set_font_size, float, font_size, KST_TB_FONT_SIZE)
TB_SET(Ktext_block_set_font_family, KFontFamily, font_family, KST_TB_FONT_FAMILY)
TB_SET(Ktext_block_set_foreground, KColor, foreground, KST_TB_FOREGROUND)
TB_SET(Ktext_block_set_text_wrapping, KTextWrapping, wrapping, 0)
TB_SET(Ktext_block_set_text_alignment, KTextAlignment, alignment, 0)
TB_SET(Ktext_block_set_text_trimming, KTextTrimming, trimming, 0)
TB_SET(Ktext_block_set_max_lines, int, max_lines, 0)
TB_SET(Ktext_block_set_line_height, float, line_height, 0)
TB_SET(Ktext_block_set_character_spacing, int, character_spacing, 0)

int Ktext_block_set_style(Kid id, KTextStyle style) {
	/* family, size, foreground role (0 inherited, 1 subtle, 2 contrast, 3 accent), line height (0 the font's),
	 * character spacing, wrapping, margin */
	static const struct { KFontFamily family; float size; int role; float line_height; int spacing; int wrap; float margin; } styles[K_TEXT_STYLE_LAST_ - K_TEXT_STYLE_NONE] = {
		/* Windows Phone 8.1 (themeresources.xaml of the phone SDK), sizes as they are */
		[K_TEXT_STYLE_BASE - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 18.14f, 0, 21.5f, 0, 1, 0 },
		[K_TEXT_STYLE_BODY - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 18.14f, 0, 24.0f, 0, 1, 0 },
		[K_TEXT_STYLE_HEADER - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 57.5f, 0, 57.5f, 0, 0, 0 },
		[K_TEXT_STYLE_SUBHEADER - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 30.0f, 0, 33.0f, 0, 1, 0 },
		[K_TEXT_STYLE_TITLE - K_TEXT_STYLE_NONE] = { K_FONT_SEMIBOLD, 18.14f, 0, 21.5f, 0, 0, 0 },
		[K_TEXT_STYLE_GROUP_HEADER - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 23.73f, 1, 23.0f, -3, 1, 0 },
		[K_TEXT_STYLE_CONTROL_HEADER - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 16.0f, 1, 17.5f, 0, 1, 0 },
		[K_TEXT_STYLE_CONTROL_CONTEXTUAL_INFO - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 16.0f, 1, 17.5f, 0, 1, 0 },
		[K_TEXT_STYLE_FLYOUT_PICKER_TITLE - K_TEXT_STYLE_NONE] = { K_FONT_SEMIBOLD, 18.14f, 0, 21.5f, 15, 0, 0 },
		[K_TEXT_STYLE_LIST_VIEW_ITEM - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 29.86f, 0, 33.5f, 0, 0, 0 },
		[K_TEXT_STYLE_LIST_VIEW_EMPTY_STATIC - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 29.86f, 1, 33.5f, 0, 1, 0 },
		[K_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, 16.0f, 1, 20.0f, 0, 0, 0 },
		[K_TEXT_STYLE_LIST_VIEW_ITEM_CONTENT - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 16.0f, 0, 20.0f, 0, 0, 0 },
		[K_TEXT_STYLE_MESSAGE_DIALOG_TITLE - K_TEXT_STYLE_NONE] = { K_FONT_SEMIBOLD, 25.5f, 0, 31.5f, 3, 1, 0 },
		[K_TEXT_STYLE_MESSAGE_DIALOG_CONTENT - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 18.14f, 0, 21.5f, 0, 1, 0 },
		[K_TEXT_STYLE_COMBO_BOX_PLACEHOLDER - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, 20.26f, 0, 30.0f, 0, 0, 0 },
		/* Windows Phone 8.0: every PhoneText*Style has Margin = PhoneHorizontalMargin */
		[K_TEXT_STYLE_PHONE_NORMAL - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, KST_FONT_NORMAL, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_SUBTLE - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, KST_FONT_NORMAL, 1, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_TITLE1 - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_EXTRA_EXTRA_LARGE, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_TITLE2 - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_LARGE, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_TITLE3 - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_MEDIUM, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_EXTRA_LARGE - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_EXTRA_LARGE, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_GROUP_HEADER - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_LARGE, 1, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_LARGE - K_TEXT_STYLE_NONE] = { K_FONT_SEMILIGHT, KST_FONT_LARGE, 0, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_SMALL - K_TEXT_STYLE_NONE] = { K_FONT_NORMAL, KST_FONT_SMALL, 1, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_CONTRAST - K_TEXT_STYLE_NONE] = { K_FONT_SEMIBOLD, KST_FONT_NORMAL, 2, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_ACCENT - K_TEXT_STYLE_NONE] = { K_FONT_SEMIBOLD, KST_FONT_NORMAL, 3, 0, 0, 0, 12 },
		[K_TEXT_STYLE_PHONE_HUGE - K_TEXT_STYLE_NONE] = { K_FONT_LIGHT, KST_FONT_HUGE, 0, 0, 0, 0, 12 }
	};
	kst_text_block_t* t = get_tb(id);
	int i = (int)style - (int)K_TEXT_STYLE_NONE;
	if (!t || i <= 0 || i >= (int)(K_TEXT_STYLE_LAST_ - K_TEXT_STYLE_NONE)) return K_FALSE;
	t->font_family = styles[i].family;
	t->font_size = styles[i].size;
	t->set_flags |= KST_TB_FONT_FAMILY | KST_TB_FONT_SIZE;
	t->set_flags &= ~(unsigned)KST_TB_FOREGROUND;
	t->fg_role = styles[i].role;
	t->line_height = styles[i].line_height;
	t->character_spacing = styles[i].spacing;
	t->wrapping = styles[i].wrap ? K_WRAP : K_NO_WRAP;
	t->base.margin = Kthickness(styles[i].margin, 0, styles[i].margin, 0);
	tb_drop_layout(t);
	kst_changed_layout(&t->base);
	return K_TRUE;
}


/* ============================================================================
 *  Image
 * ============================================================================ */

static KSize image_fit(const kst_image_element_t* im, KSize room) {
	float nw, nh, sx, sy;
	if (!im->image) return kst_size(0, 0);
	nw = (float)im->image->width;
	nh = (float)im->image->height;
	switch (im->stretch) {
		case K_STRETCH_NONE: return kst_size(nw, nh);
		case K_STRETCH_FILL:
			return kst_size(room.width < K_INFINITE * 0.5f ? room.width : nw, room.height < K_INFINITE * 0.5f ? room.height : nh);
		default: break;
	}
	sx = room.width < K_INFINITE * 0.5f ? room.width / nw : 1e30f;
	sy = room.height < K_INFINITE * 0.5f ? room.height / nh : 1e30f;
	if (sx >= 1e29f && sy >= 1e29f) return kst_size(nw, nh);
	if (im->stretch == K_STRETCH_UNIFORM) sx = sy = kst_minf(sx, sy);
	else sx = sy = (sx >= 1e29f ? sy : (sy >= 1e29f ? sx : kst_maxf(sx, sy)));
	return kst_size(nw * sx, nh * sy);
}

static KSize image_measure(kst_element_t* e, KSize available) {
	kst_image_element_t* im = KST_AS(kst_image_element_t, e);
	KSize s = image_fit(im, available);
	/* UniformToFill measures to the room, the picture is cropped */
	if (im->stretch == K_STRETCH_UNIFORM_TO_FILL) {
		if (available.width < K_INFINITE * 0.5f) s.width = kst_minf(s.width, available.width);
		if (available.height < K_INFINITE * 0.5f) s.height = kst_minf(s.height, available.height);
	}
	return s;
}

static void image_paint(kst_element_t* e, kst_painter_t* p) {
	kst_image_element_t* im = KST_AS(kst_image_element_t, e);
	KSize full;
	KRect dest;
	if (!im->image) return;
	full = image_fit(im, kst_size(e->rect.width, e->rect.height));
	dest = kst_rect((e->rect.width - full.width) * 0.5f, (e->rect.height - full.height) * 0.5f, full.width, full.height);
	if (im->stretch == K_STRETCH_FILL) dest = kst_local_rect(e);
	kst_push_clip(p, kst_local_rect(e));
	kst_draw_image(p, im->image, dest, kst_rect(0, 0, (float)im->image->width, (float)im->image->height));
	kst_pop_clip(p);
}

static void image_init(kst_element_t* e) { KST_AS(kst_image_element_t, e)->stretch = K_STRETCH_UNIFORM; }
static void image_destroy(kst_element_t* e) { kst_image_free(KST_AS(kst_image_element_t, e)->image); }

kst_class_t kst_class_image = {
	.base = &kst_class_element, .name = "Image", .id = K_CLASS_IMAGE, .size = sizeof(kst_image_element_t),
	.init = image_init, .destroy = image_destroy, .measure = image_measure, .paint = image_paint
};

Kid Kimage_create(void) {
	kst_element_t* e = kst_create(K_CLASS_IMAGE);
	return e ? e->id : K_INVALID_ID;
}

static int image_set(kst_element_t* e, kst_image_t* image) {
	kst_image_element_t* im = KST_AS(kst_image_element_t, e);
	if (!image) return K_FALSE;
	kst_image_free(im->image);
	im->image = image;
	kst_changed_layout(e);
	return K_TRUE;
}

int Kimage_set_source_file(Kid id, const char* path) {
	kst_element_t* e = kst_get(id, K_CLASS_IMAGE);
	if (!e || !path) return K_FALSE;
	if (!image_set(e, kst_image_load_png(path))) {
		kst_error(K_ERROR_IO, "cannot read the image %s", path);
		return K_FALSE;
	}
	return K_TRUE;
}

int Kimage_set_source_rgba(Kid id, int width, int height, const uint8_t* rgba) {
	kst_element_t* e = kst_get(id, K_CLASS_IMAGE);
	if (!e) return K_FALSE;
	return image_set(e, kst_image_from_rgba(width, height, rgba));
}

void Kimage_set_stretch(Kid id, KStretch stretch) {
	kst_element_t* e = kst_get(id, K_CLASS_IMAGE);
	if (!e) return;
	KST_AS(kst_image_element_t, e)->stretch = stretch;
	kst_changed_layout(e);
}

KSize Kimage_get_natural_size(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_IMAGE);
	kst_image_t* image = e ? KST_AS(kst_image_element_t, e)->image : NULL;
	return image ? kst_size((float)image->width, (float)image->height) : kst_size(0, 0);
}


/* ============================================================================
 *  Popup
 * ============================================================================ */

static int popup_hit_self(kst_element_t* e) { KST_UNUSED(e); return 0; }
static KSize popup_measure(kst_element_t* e, KSize a) { KST_UNUSED(e); KST_UNUSED(a); return kst_size(0, 0); }
static KSize popup_arrange(kst_element_t* e, KSize f) { KST_UNUSED(e); return f; }

static void popup_init(kst_element_t* e) {
	kst_popup_t* p = KST_AS(kst_popup_t, e);
	p->stays_open = 0;
	p->placement = K_PLACE_BOTTOM;
}

static void popup_destroy(kst_element_t* e) {
	if (e->root) kst_root_unregister_popup(e->root, e);
}

kst_class_t kst_class_popup = {
	.base = &kst_class_element, .name = "Popup", .id = K_CLASS_POPUP, .size = sizeof(kst_popup_t),
	.init = popup_init, .destroy = popup_destroy, .measure = popup_measure, .arrange = popup_arrange, .hit_self = popup_hit_self
};

Kid Kpopup_create(void) {
	kst_element_t* e = kst_create(K_CLASS_POPUP);
	return e ? e->id : K_INVALID_ID;
}

void Kpopup_set_child(Kid id, Kid child) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	kst_element_t* c = child != K_INVALID_ID ? kst_get(child, K_CLASS_ELEMENT) : NULL;
	kst_element_t* slot;
	if (!e || (child != K_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : NULL;
	kst_set_only_child(e, &slot, c);
	if (c) c->is_popup_child = 1;
}

void Kpopup_set_offset(Kid id, float h, float v) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	if (!e) return;
	KST_AS(kst_popup_t, e)->h_offset = h;
	KST_AS(kst_popup_t, e)->v_offset = v;
	kst_changed_layout(e);
}

void Kpopup_set_placement_target(Kid id, Kid target, KPlacementMode mode) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	if (!e) return;
	KST_AS(kst_popup_t, e)->target = target;
	KST_AS(kst_popup_t, e)->placement = mode;
	kst_changed_layout(e);
}

void kst_popup_set_open(kst_element_t* e, int open) {
	kst_popup_t* p = KST_AS(kst_popup_t, e);
	open = open != 0;
	if (p->is_open == open) return;
	p->is_open = open;
	if (e->root) {
		if (open) kst_root_register_popup(e->root, e);
		else kst_root_unregister_popup(e->root, e);
		kst_changed_layout(e);
	}
	kst_raise(e, open ? K_EVENT_POPUP_OPENED : K_EVENT_POPUP_CLOSED);
}

void kst_popup_close(kst_element_t* e) { kst_popup_set_open(e, 0); }

void Kpopup_set_open(Kid id, int open) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	if (e) kst_popup_set_open(e, open);
}

int Kpopup_is_open(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	return e ? KST_AS(kst_popup_t, e)->is_open : 0;
}

void Kpopup_set_stays_open(Kid id, int stays_open) {
	kst_element_t* e = kst_get(id, K_CLASS_POPUP);
	if (e) KST_AS(kst_popup_t, e)->stays_open = stays_open != 0;
}


/* ============================================================================
 *  Control
 * ============================================================================ */

KColor kst_ctl_color(const kst_control_t* c, unsigned flag, KColor stored, KColor fallback) {
	KST_UNUSED(c);
	return (c->set_flags & flag) ? stored : fallback;
}

KColor kst_ctl_foreground(const kst_control_t* c) {
	return kst_ctl_color(c, KST_C_FOREGROUND, c->foreground, kst_theme_color(&c->base, KST_TC_FOREGROUND));
}

KColor kst_ctl_background(const kst_control_t* c, KColor fallback) { return kst_ctl_color(c, KST_C_BACKGROUND, c->background, fallback); }
KColor kst_ctl_border_brush(const kst_control_t* c, KColor fallback) { return kst_ctl_color(c, KST_C_BORDER_BRUSH, c->border_brush, fallback); }

static void control_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	c->font_size = KST_FONT_NORMAL;
	c->font_family = K_FONT_NORMAL;
	c->h_content = K_HALIGN_CENTER;
	c->v_content = K_VALIGN_CENTER;
	e->focusable = 1;
}

static KColor control_content_foreground(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	if (!kst_effectively_enabled(e)) return kst_theme_color(e, KST_TC_DISABLED);
	return kst_ctl_foreground(c);
}

static float control_content_font_size(kst_element_t* e) { return KST_AS(kst_control_t, e)->font_size; }
static KFontFamily control_content_font_family(kst_element_t* e) { return KST_AS(kst_control_t, e)->font_family; }

kst_class_t kst_class_control = {
	.base = &kst_class_element, .name = "Control", .id = K_CLASS_CONTROL, .size = sizeof(kst_control_t), .abstract_class = 1,
	.init = control_init, .content_foreground = control_content_foreground, .content_font_size = control_content_font_size,
	.content_font_family = control_content_font_family
};

static kst_control_t* get_control(Kid id) { return KST_AS(kst_control_t, kst_get(id, K_CLASS_CONTROL)); }

void Kcontrol_set_foreground(Kid id, KColor color) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->foreground = color;
	c->set_flags |= KST_C_FOREGROUND;
	kst_changed_paint(&c->base);
}

void Kcontrol_set_background(Kid id, KColor color) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->background = color;
	c->set_flags |= KST_C_BACKGROUND;
	kst_changed_paint(&c->base);
}

void Kcontrol_set_border_brush(Kid id, KColor color) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->border_brush = color;
	c->set_flags |= KST_C_BORDER_BRUSH;
	kst_changed_paint(&c->base);
}

void Kcontrol_set_border_thickness(Kid id, KThickness t) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->border_thickness = t;
	c->set_flags |= KST_C_BORDER_THICKNESS;
	kst_changed_layout(&c->base);
}

void Kcontrol_set_padding(Kid id, KThickness t) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->padding = t;
	c->set_flags |= KST_C_PADDING;
	kst_changed_layout(&c->base);
}

KThickness Kcontrol_get_padding(Kid id) {
	kst_control_t* c = get_control(id);
	return c ? c->padding : kst_thickness_zero();
}

void Kcontrol_set_font_size(Kid id, float size) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->font_size = size;
	c->set_flags |= KST_C_FONT_SIZE;
	kst_invalidate_deep(&c->base);
}

float Kcontrol_get_font_size(Kid id) {
	kst_control_t* c = get_control(id);
	return c ? c->font_size : 0;
}

void Kcontrol_set_font_family(Kid id, KFontFamily family) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->font_family = family;
	c->set_flags |= KST_C_FONT_FAMILY;
	kst_invalidate_deep(&c->base);
}

void Kcontrol_set_horizontal_content_alignment(Kid id, KHorizontalAlignment a) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->h_content = a;
	kst_changed_layout(&c->base);
}

void Kcontrol_set_vertical_content_alignment(Kid id, KVerticalAlignment a) {
	kst_control_t* c = get_control(id);
	if (!c) return;
	c->v_content = a;
	kst_changed_layout(&c->base);
}

void Kcontrol_set_tab_stop(Kid id, int is_tab_stop) {
	kst_control_t* c = get_control(id);
	if (c) c->base.tab_stop = is_tab_stop != 0;
}

void Kcontrol_set_tab_index(Kid id, int tab_index) {
	kst_control_t* c = get_control(id);
	if (c) c->base.tab_index = tab_index;
}


/* ============================================================================
 *  ContentControl
 * ============================================================================ */

KThickness kst_cc_insets(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	KThickness chrome = e->cls->chrome ? e->cls->chrome(e) : kst_thickness_zero();
	KThickness t;
	t.left = chrome.left + c->border_thickness.left + c->padding.left;
	t.top = chrome.top + c->border_thickness.top + c->padding.top;
	t.right = chrome.right + c->border_thickness.right + c->padding.right;
	t.bottom = chrome.bottom + c->border_thickness.bottom + c->padding.bottom;
	return t;
}

KSize kst_cc_measure(kst_element_t* e, KSize available) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	KThickness in = kst_cc_insets(e);
	KSize content = { 0, 0 };
	if (cc->content) {
		kst_measure(cc->content, kst_size(kst_maxf(0, available.width - kst_thickness_h(in)), kst_maxf(0, available.height - kst_thickness_v(in))));
		content = cc->content->desired;
	}
	return kst_size(content.width + kst_thickness_h(in), content.height + kst_thickness_v(in));
}

KSize kst_cc_arrange(kst_element_t* e, KSize final) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	kst_control_t* c = &cc->base;
	KThickness in = kst_cc_insets(e);
	KRect inner = kst_rect(in.left, in.top, kst_maxf(0, final.width - kst_thickness_h(in)), kst_maxf(0, final.height - kst_thickness_v(in)));
	if (cc->content) {
		KSize d = cc->content->desired;
		KRect r = inner;
		/* ContentPresenter: the alignment of the content is the control's HorizontalContentAlignment */
		if (c->h_content != K_HALIGN_STRETCH) {
			r.width = kst_minf(inner.width, d.width);
			if (c->h_content == K_HALIGN_CENTER) r.x += (inner.width - r.width) * 0.5f;
			else if (c->h_content == K_HALIGN_RIGHT) r.x += inner.width - r.width;
		}
		if (c->v_content != K_VALIGN_STRETCH) {
			r.height = kst_minf(inner.height, d.height);
			if (c->v_content == K_VALIGN_CENTER) r.y += (inner.height - r.height) * 0.5f;
			else if (c->v_content == K_VALIGN_BOTTOM) r.y += inner.height - r.height;
		}
		kst_arrange(cc->content, r);
	}
	return final;
}

void kst_cc_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	if (cc->content == child) {
		cc->content = NULL;
		cc->content_is_text = 0;
	}
}

kst_class_t kst_class_content_control = {
	.base = &kst_class_control, .name = "ContentControl", .id = K_CLASS_CONTENT_CONTROL, .size = sizeof(kst_content_control_t),
	.measure = kst_cc_measure, .arrange = kst_cc_arrange, .child_removed = kst_cc_child_removed
};

void kst_content_set(kst_content_control_t* c, kst_element_t* content) {
	c->content_is_text = 0;
	kst_set_only_child(&c->base.base, &c->content, content);
}

void kst_content_set_text(kst_content_control_t* c, const char* text) {
	if (c->content && c->content_is_text) {
		Ktext_block_set_text(c->content->id, text);
		return;
	}
	{
		kst_element_t* tb = kst_create(K_CLASS_TEXT_BLOCK);
		if (!tb) return;
		kst_set_string(&KST_AS(kst_text_block_t, tb)->text, text ? text : "");
		kst_content_set(c, tb);
		c->content_is_text = c->content == tb;
	}
}

static kst_content_control_t* get_cc(Kid id) { return KST_AS(kst_content_control_t, kst_get(id, K_CLASS_CONTENT_CONTROL)); }

Kid Kcontent_control_create(void) {
	kst_element_t* e = kst_create(K_CLASS_CONTENT_CONTROL);
	return e ? e->id : K_INVALID_ID;
}

void Kcontent_control_set_content(Kid id, Kid content) {
	kst_content_control_t* c = get_cc(id);
	kst_element_t* e = content != K_INVALID_ID ? kst_get(content, K_CLASS_ELEMENT) : NULL;
	if (!c || (content != K_INVALID_ID && !e)) return;
	kst_content_set(c, e);
}

void Kcontent_control_set_text(Kid id, const char* text) {
	kst_content_control_t* c = get_cc(id);
	if (c) kst_content_set_text(c, text);
}

Kid Kcontent_control_get_content(Kid id) {
	kst_content_control_t* c = get_cc(id);
	return c && c->content ? c->content->id : K_INVALID_ID;
}

const char* Kcontent_control_get_text(Kid id) {
	kst_content_control_t* c = get_cc(id);
	if (!c || !c->content || !c->content_is_text) return "";
	return Ktext_block_get_text(c->content->id);
}


/* ============================================================================
 *  UserControl
 * ============================================================================ */

kst_class_t kst_class_user_control = {
	.base = &kst_class_control, .name = "UserControl", .id = K_CLASS_USER_CONTROL, .size = sizeof(kst_content_control_t),
	.measure = kst_cc_measure, .arrange = kst_cc_arrange, .child_removed = kst_cc_child_removed
};

Kid Kuser_control_create(void) {
	kst_element_t* e = kst_create(K_CLASS_USER_CONTROL);
	if (!e) return K_INVALID_ID;
	e->focusable = 0;
	KST_AS(kst_control_t, e)->h_content = K_HALIGN_STRETCH;
	KST_AS(kst_control_t, e)->v_content = K_VALIGN_STRETCH;
	return e->id;
}

void Kuser_control_set_content(Kid id, Kid child) {
	kst_element_t* e = kst_get(id, K_CLASS_USER_CONTROL);
	kst_element_t* c = child != K_INVALID_ID ? kst_get(child, K_CLASS_ELEMENT) : NULL;
	if (!e || (child != K_INVALID_ID && !c)) return;
	kst_content_set(KST_AS(kst_content_control_t, e), c);
}


Kid Kuser_control_get_content(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_USER_CONTROL);
	return e && KST_AS(kst_content_control_t, e)->content ? KST_AS(kst_content_control_t, e)->content->id : K_INVALID_ID;
}


/* ============================================================================
 *  ToolTip
 * ============================================================================ */

static void tooltip_paint(kst_element_t* e, kst_painter_t* p) {
	KRect r = kst_local_rect(e);
	kst_fill_rect(p, r, kst_theme_color(e, KST_TC_CHROME));
	kst_stroke_rect(p, r, Kthickness_uniform(1), kst_theme_color(e, KST_TC_BORDER));
}

static void tooltip_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	e->focusable = 0;
	c->padding = Kthickness(8, 4, 8, 4);
	c->border_thickness = Kthickness_uniform(1);
	c->font_size = KST_FONT_SMALL;
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
}

kst_class_t kst_class_tooltip = {
	.base = &kst_class_content_control, .name = "ToolTip", .id = K_CLASS_TOOLTIP, .size = sizeof(kst_content_control_t),
	.init = tooltip_init, .paint = tooltip_paint
};

Kid Ktool_tip_create(const char* text) {
	kst_element_t* e = kst_create(K_CLASS_TOOLTIP);
	if (!e) return K_INVALID_ID;
	kst_content_set_text(KST_AS(kst_content_control_t, e), text);
	return e->id;
}


/* ============================================================================
 *  Separator
 * ============================================================================ */

static KSize separator_measure(kst_element_t* e, KSize available) {
	KST_UNUSED(e); KST_UNUSED(available);
	return kst_size(1, 1);
}

static void separator_paint(kst_element_t* e, kst_painter_t* p) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	kst_fill_rect(p, kst_local_rect(e), kst_ctl_background(c, kst_theme_color(e, KST_TC_INACTIVE)));
}

static void separator_init(kst_element_t* e) { e->focusable = 0; }

kst_class_t kst_class_separator = {
	.base = &kst_class_control, .name = "Separator", .id = K_CLASS_SEPARATOR, .size = sizeof(kst_control_t),
	.init = separator_init, .measure = separator_measure, .paint = separator_paint
};

Kid Kseparator_create(void) {
	kst_element_t* e = kst_create(K_CLASS_SEPARATOR);
	return e ? e->id : K_INVALID_ID;
}
