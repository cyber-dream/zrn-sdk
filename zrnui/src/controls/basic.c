/**
 * zrnui - controls/basic.c
 * TextBlock, Image, Popup, Control, ContentControl, UserControl, ToolTip
 * and Separator.
 */

#include "../internal.h"


/* ============================================================================
 *  TextBlock
 * ============================================================================ */

static zrn_font_t tb_font(const zrn_text_block_t* t) {
	zrn_font_t f;
	f.size = (t->set_flags & ZST_TB_FONT_SIZE) ? t->font_size : zrn_inherited_font_size(&t->base);
	f.family = (t->set_flags & ZST_TB_FONT_FAMILY) ? t->font_family : zrn_inherited_font_family(&t->base);
	f.spacing = t->character_spacing;
	return f;
}

static ZColor tb_foreground(const zrn_text_block_t* t) {
	if (t->set_flags & ZST_TB_FOREGROUND) return t->foreground;
	switch (t->fg_role) {
		case 1: return zrn_theme_color(&t->base, ZST_TC_SUBTLE);
		case 2: return zrn_theme_color(&t->base, ZST_TC_CONTRAST_FOREGROUND);
		case 3: return zrn_theme_color(&t->base, ZST_TC_ACCENT);
		default: break;
	}
	return zrn_inherited_foreground(&t->base);
}

static void tb_drop_layout(zrn_text_block_t* t) {
	zrn_text_layout_free((zrn_text_layout_t*)t->layout);
	t->layout = NULL;
}

/* The layout for a width; cached until the text, the font or the width changes. */
static zrn_text_layout_t* tb_layout(zrn_text_block_t* t, float width) {
	zrn_text_layout_t* l = (zrn_text_layout_t*)t->layout;
	zrn_font_t font = tb_font(t);
	int wrap = t->wrapping == Z_WRAP && width > 0 && width < Z_INFINITE * 0.5f;
	if (l && (l->font.size != font.size || l->font.family != font.family || l->font.spacing != font.spacing || (wrap && t->layout_width != width) || (!wrap && t->layout_width != 0))) {
		tb_drop_layout(t);
		l = NULL;
	}
	if (!l) {
		const char* text = t->text ? t->text : "";
		l = zrn_text_layout_new(font, text, (int)strlen(text), wrap ? width : 0, wrap, t->max_lines, t->line_height);
		t->layout = l;
		t->layout_width = wrap ? width : 0;
	}
	return l;
}

static ZSize tb_measure(zrn_element_t* e, ZSize available) {
	zrn_text_block_t* t = ZST_AS(zrn_text_block_t, e);
	zrn_text_layout_t* l;
	tb_drop_layout(t);
	l = tb_layout(t, available.width);
	if (!l) return zrn_size(0, 0);
	/* an empty text still takes a line, as in Silverlight */
	return zrn_size(l->width, l->height);
}

static void tb_paint(zrn_element_t* e, zrn_painter_t* p) {
	zrn_text_block_t* t = ZST_AS(zrn_text_block_t, e);
	zrn_text_layout_t* l;
	ZColor c;
	if (!t->text || !*t->text) return;
	l = tb_layout(t, e->rect.width);
	if (!l) return;
	c = tb_foreground(t);
	if (!zrn_effectively_enabled(e)) c = zrn_theme_color(e, ZST_TC_DISABLED);
	zrn_draw_text(p, l, 0, 0, e->rect.width, t->alignment, t->trimming, c);
}

static void tb_init(zrn_element_t* e) {
	zrn_text_block_t* t = ZST_AS(zrn_text_block_t, e);
	t->wrapping = Z_NO_WRAP;
	t->alignment = Z_TEXT_LEFT;
	t->trimming = Z_TRIM_NONE;
	t->text = zrn_strdup("");
}

static void tb_destroy(zrn_element_t* e) {
	zrn_text_block_t* t = ZST_AS(zrn_text_block_t, e);
	tb_drop_layout(t);
	free(t->text);
}

zrn_class_t zrn_class_text_block = {
	.base = &zrn_class_element, .name = "TextBlock", .id = Z_CLASS_TEXT_BLOCK, .size = sizeof(zrn_text_block_t),
	.init = tb_init, .destroy = tb_destroy, .measure = tb_measure, .paint = tb_paint
};

Zid Ztext_block_create(const char* text) {
	zrn_element_t* e = zrn_create(Z_CLASS_TEXT_BLOCK);
	if (!e) return Z_INVALID_ID;
	zrn_set_string(&ZST_AS(zrn_text_block_t, e)->text, text ? text : "");
	return e->id;
}

static zrn_text_block_t* get_tb(Zid id) { return ZST_AS(zrn_text_block_t, zrn_get(id, Z_CLASS_TEXT_BLOCK)); }

void Ztext_block_set_text(Zid id, const char* text) {
	zrn_text_block_t* t = get_tb(id);
	if (!t) return;
	if (t->text && text && strcmp(t->text, text) == 0) return;
	zrn_set_string(&t->text, text ? text : "");
	tb_drop_layout(t);
	zrn_changed_layout(&t->base);
}

const char* Ztext_block_get_text(Zid id) {
	zrn_text_block_t* t = get_tb(id);
	return t && t->text ? t->text : "";
}

#define TB_SET(name, type, field, flag, prop) \
	void name(Zid id, type value) { \
		zrn_text_block_t* t = get_tb(id); \
		if (!t) return; \
		if (prop) zrn_unbind(&t->base, prop); \
		t->field = value; \
		if (flag) t->set_flags |= (flag); \
		tb_drop_layout(t); \
		zrn_changed_layout(&t->base); \
	}

TB_SET(Ztext_block_set_font_size, float, font_size, ZST_TB_FONT_SIZE, ZST_BP_TEXT_FONT_SIZE)
TB_SET(Ztext_block_set_font_family, ZFontFamily, font_family, ZST_TB_FONT_FAMILY, 0)
TB_SET(Ztext_block_set_foreground, ZColor, foreground, ZST_TB_FOREGROUND, ZST_BP_TEXT_FOREGROUND)
TB_SET(Ztext_block_set_text_wrapping, ZTextWrapping, wrapping, 0, 0)
TB_SET(Ztext_block_set_text_alignment, ZTextAlignment, alignment, 0, 0)
TB_SET(Ztext_block_set_text_trimming, ZTextTrimming, trimming, 0, 0)
TB_SET(Ztext_block_set_max_lines, int, max_lines, 0, 0)
TB_SET(Ztext_block_set_line_height, float, line_height, 0, 0)
TB_SET(Ztext_block_set_character_spacing, int, character_spacing, 0, ZST_BP_TEXT_CHARACTER_SPACING)

int Ztext_block_set_style(Zid id, ZTextStyle style) {
	/* family, size, foreground role (0 inherited, 1 subtle, 2 contrast, 3 accent), line height (0 the font's),
	 * character spacing, wrapping, margin */
	static const struct { ZFontFamily family; float size; int role; float line_height; int spacing; int wrap; float margin; } styles[Z_TEXT_STYLE_LAST_ - Z_TEXT_STYLE_NONE] = {
		/* Windows Phone 8.1 (themeresources.xaml of the phone SDK), sizes as they are: the phone
		 * renders them through the scale of its shell (a Lumia 520 screen shot measures
		 * brightness 70px = 57.5 x 1.2, SETTINGS 16px = 18.14 x 1.2), the scale does the zooming */
		[Z_TEXT_STYLE_BASE - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 18.14f, 0, 21.5f, 0, 1, 0 },
		[Z_TEXT_STYLE_BODY - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 18.14f, 0, 24.0f, 0, 1, 0 },
		[Z_TEXT_STYLE_HEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 57.5f, 0, 57.5f, 0, 0, 0 },
		[Z_TEXT_STYLE_SUBHEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 30.0f, 0, 33.0f, 0, 1, 0 },
		[Z_TEXT_STYLE_TITLE - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMIBOLD, 18.14f, 0, 21.5f, 0, 0, 0 },
		[Z_TEXT_STYLE_GROUP_HEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 23.73f, 1, 23.0f, -3, 1, 0 },
		[Z_TEXT_STYLE_CONTROL_HEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 16.0f, 1, 17.5f, 0, 1, 0 },
		[Z_TEXT_STYLE_CONTROL_CONTEXTUAL_INFO - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 16.0f, 1, 17.5f, 0, 1, 0 },
		[Z_TEXT_STYLE_FLYOUT_PICKER_TITLE - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMIBOLD, 18.14f, 0, 21.5f, 15, 0, 0 },
		[Z_TEXT_STYLE_LIST_VIEW_ITEM - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 29.86f, 0, 33.5f, 0, 0, 0 },
		[Z_TEXT_STYLE_LIST_VIEW_EMPTY_STATIC - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 29.86f, 1, 33.5f, 0, 1, 0 },
		[Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, 16.0f, 1, 20.0f, 0, 0, 0 },
		[Z_TEXT_STYLE_LIST_VIEW_ITEM_CONTENT - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 16.0f, 0, 20.0f, 0, 0, 0 },
		[Z_TEXT_STYLE_MESSAGE_DIALOG_TITLE - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMIBOLD, 25.5f, 0, 31.5f, 3, 1, 0 },
		[Z_TEXT_STYLE_MESSAGE_DIALOG_CONTENT - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 18.14f, 0, 21.5f, 0, 1, 0 },
		[Z_TEXT_STYLE_COMBO_BOX_PLACEHOLDER - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, 20.26f, 0, 30.0f, 0, 0, 0 },
		/* Windows Phone 8.0: every PhoneText*Style has Margin = PhoneHorizontalMargin */
		[Z_TEXT_STYLE_PHONE_NORMAL - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, ZST_FONT_NORMAL, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_SUBTLE - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, ZST_FONT_NORMAL, 1, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_TITLE1 - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_EXTRA_EXTRA_LARGE, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_TITLE2 - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_LARGE, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_TITLE3 - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_MEDIUM, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_EXTRA_LARGE - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_EXTRA_LARGE, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_GROUP_HEADER - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_LARGE, 1, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_LARGE - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMILIGHT, ZST_FONT_LARGE, 0, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_SMALL - Z_TEXT_STYLE_NONE] = { Z_FONT_NORMAL, ZST_FONT_SMALL, 1, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_CONTRAST - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMIBOLD, ZST_FONT_NORMAL, 2, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_ACCENT - Z_TEXT_STYLE_NONE] = { Z_FONT_SEMIBOLD, ZST_FONT_NORMAL, 3, 0, 0, 0, 12 },
		[Z_TEXT_STYLE_PHONE_HUGE - Z_TEXT_STYLE_NONE] = { Z_FONT_LIGHT, ZST_FONT_HUGE, 0, 0, 0, 0, 12 }
	};
	zrn_text_block_t* t = get_tb(id);
	int i = (int)style - (int)Z_TEXT_STYLE_NONE;
	if (!t || i <= 0 || i >= (int)(Z_TEXT_STYLE_LAST_ - Z_TEXT_STYLE_NONE)) return Z_FALSE;
	zrn_unbind(&t->base, ZST_BP_TEXT_FONT_SIZE);
	zrn_unbind(&t->base, ZST_BP_TEXT_FOREGROUND);
	zrn_unbind(&t->base, ZST_BP_TEXT_CHARACTER_SPACING);
	zrn_unbind(&t->base, ZST_BP_ELEMENT_MARGIN);
	t->font_family = styles[i].family;
	t->font_size = styles[i].size;
	t->set_flags |= ZST_TB_FONT_FAMILY | ZST_TB_FONT_SIZE;
	t->set_flags &= ~(unsigned)ZST_TB_FOREGROUND;
	t->fg_role = styles[i].role;
	t->line_height = styles[i].line_height;
	t->character_spacing = styles[i].spacing;
	t->wrapping = styles[i].wrap ? Z_WRAP : Z_NO_WRAP;
	t->base.margin = Zthickness(styles[i].margin, 0, styles[i].margin, 0);
	tb_drop_layout(t);
	zrn_changed_layout(&t->base);
	return Z_TRUE;
}


/* ============================================================================
 *  Image
 * ============================================================================ */

static ZSize image_fit(const zrn_image_element_t* im, ZSize room) {
	float nw, nh, sx, sy;
	if (!im->image) return zrn_size(0, 0);
	nw = (float)im->image->width;
	nh = (float)im->image->height;
	switch (im->stretch) {
		case Z_STRETCH_NONE: return zrn_size(nw, nh);
		case Z_STRETCH_FILL:
			return zrn_size(room.width < Z_INFINITE * 0.5f ? room.width : nw, room.height < Z_INFINITE * 0.5f ? room.height : nh);
		default: break;
	}
	sx = room.width < Z_INFINITE * 0.5f ? room.width / nw : 1e30f;
	sy = room.height < Z_INFINITE * 0.5f ? room.height / nh : 1e30f;
	if (sx >= 1e29f && sy >= 1e29f) return zrn_size(nw, nh);
	if (im->stretch == Z_STRETCH_UNIFORM) sx = sy = zrn_minf(sx, sy);
	else sx = sy = (sx >= 1e29f ? sy : (sy >= 1e29f ? sx : zrn_maxf(sx, sy)));
	return zrn_size(nw * sx, nh * sy);
}

static ZSize image_measure(zrn_element_t* e, ZSize available) {
	zrn_image_element_t* im = ZST_AS(zrn_image_element_t, e);
	ZSize s = image_fit(im, available);
	/* UniformToFill measures to the room, the picture is cropped */
	if (im->stretch == Z_STRETCH_UNIFORM_TO_FILL) {
		if (available.width < Z_INFINITE * 0.5f) s.width = zrn_minf(s.width, available.width);
		if (available.height < Z_INFINITE * 0.5f) s.height = zrn_minf(s.height, available.height);
	}
	return s;
}

static void image_paint(zrn_element_t* e, zrn_painter_t* p) {
	zrn_image_element_t* im = ZST_AS(zrn_image_element_t, e);
	ZSize full;
	ZRect dest;
	if (!im->image) return;
	full = image_fit(im, zrn_size(e->rect.width, e->rect.height));
	dest = zrn_rect((e->rect.width - full.width) * 0.5f, (e->rect.height - full.height) * 0.5f, full.width, full.height);
	if (im->stretch == Z_STRETCH_FILL) dest = zrn_local_rect(e);
	zrn_push_clip(p, zrn_local_rect(e));
	zrn_draw_image(p, im->image, dest, zrn_rect(0, 0, (float)im->image->width, (float)im->image->height));
	zrn_pop_clip(p);
}

static void image_init(zrn_element_t* e) { ZST_AS(zrn_image_element_t, e)->stretch = Z_STRETCH_UNIFORM; }
static void image_destroy(zrn_element_t* e) { zrn_image_free(ZST_AS(zrn_image_element_t, e)->image); }

zrn_class_t zrn_class_image = {
	.base = &zrn_class_element, .name = "Image", .id = Z_CLASS_IMAGE, .size = sizeof(zrn_image_element_t),
	.init = image_init, .destroy = image_destroy, .measure = image_measure, .paint = image_paint
};

Zid Zimage_create(void) {
	zrn_element_t* e = zrn_create(Z_CLASS_IMAGE);
	return e ? e->id : Z_INVALID_ID;
}

static int image_set(zrn_element_t* e, zrn_image_t* image) {
	zrn_image_element_t* im = ZST_AS(zrn_image_element_t, e);
	if (!image) return Z_FALSE;
	zrn_image_free(im->image);
	im->image = image;
	zrn_changed_layout(e);
	return Z_TRUE;
}

int Zimage_set_source_file(Zid id, const char* path) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_IMAGE);
	if (!e || !path) return Z_FALSE;
	if (!image_set(e, zrn_image_load_png(path))) {
		zrn_error(Z_ERROR_IO, "cannot read the image %s", path);
		return Z_FALSE;
	}
	return Z_TRUE;
}

int Zimage_set_source_rgba(Zid id, int width, int height, const uint8_t* rgba) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_IMAGE);
	if (!e) return Z_FALSE;
	return image_set(e, zrn_image_from_rgba(width, height, rgba));
}

void Zimage_set_stretch(Zid id, ZStretch stretch) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_IMAGE);
	if (!e) return;
	ZST_AS(zrn_image_element_t, e)->stretch = stretch;
	zrn_changed_layout(e);
}

ZSize Zimage_get_natural_size(Zid id) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_IMAGE);
	zrn_image_t* image = e ? ZST_AS(zrn_image_element_t, e)->image : NULL;
	return image ? zrn_size((float)image->width, (float)image->height) : zrn_size(0, 0);
}


/* ============================================================================
 *  Popup
 * ============================================================================ */

static int popup_hit_self(zrn_element_t* e) { ZST_UNUSED(e); return 0; }
static ZSize popup_measure(zrn_element_t* e, ZSize a) { ZST_UNUSED(e); ZST_UNUSED(a); return zrn_size(0, 0); }
static ZSize popup_arrange(zrn_element_t* e, ZSize f) { ZST_UNUSED(e); return f; }

static void popup_init(zrn_element_t* e) {
	zrn_popup_t* p = ZST_AS(zrn_popup_t, e);
	p->stays_open = 0;
	p->placement = Z_PLACE_BOTTOM;
}

static void popup_destroy(zrn_element_t* e) {
	if (e->root) zrn_root_unregister_popup(e->root, e);
}

zrn_class_t zrn_class_popup = {
	.base = &zrn_class_element, .name = "Popup", .id = Z_CLASS_POPUP, .size = sizeof(zrn_popup_t),
	.init = popup_init, .destroy = popup_destroy, .measure = popup_measure, .arrange = popup_arrange, .hit_self = popup_hit_self
};

Zid Zpopup_create(void) {
	zrn_element_t* e = zrn_create(Z_CLASS_POPUP);
	return e ? e->id : Z_INVALID_ID;
}

void Zpopup_set_child(Zid id, Zid child) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	zrn_element_t* c = child != Z_INVALID_ID ? zrn_get(child, Z_CLASS_ELEMENT) : NULL;
	zrn_element_t* slot;
	if (!e || (child != Z_INVALID_ID && !c)) return;
	slot = e->child_count ? e->children[0] : NULL;
	zrn_set_only_child(e, &slot, c);
	if (c) c->is_popup_child = 1;
}

void Zpopup_set_offset(Zid id, float h, float v) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	if (!e) return;
	ZST_AS(zrn_popup_t, e)->h_offset = h;
	ZST_AS(zrn_popup_t, e)->v_offset = v;
	zrn_changed_layout(e);
}

void Zpopup_set_placement_target(Zid id, Zid target, ZPlacementMode mode) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	if (!e) return;
	ZST_AS(zrn_popup_t, e)->target = target;
	ZST_AS(zrn_popup_t, e)->placement = mode;
	zrn_changed_layout(e);
}

void zrn_popup_set_open(zrn_element_t* e, int open) {
	zrn_popup_t* p = ZST_AS(zrn_popup_t, e);
	open = open != 0;
	if (p->is_open == open) return;
	p->is_open = open;
	if (e->root) {
		if (open) zrn_root_register_popup(e->root, e);
		else zrn_root_unregister_popup(e->root, e);
		zrn_changed_layout(e);
	}
	zrn_raise(e, open ? Z_EVENT_POPUP_OPENED : Z_EVENT_POPUP_CLOSED);
}

void zrn_popup_close(zrn_element_t* e) { zrn_popup_set_open(e, 0); }

void Zpopup_set_open(Zid id, int open) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	if (e) zrn_popup_set_open(e, open);
}

int Zpopup_is_open(Zid id) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	return e ? ZST_AS(zrn_popup_t, e)->is_open : 0;
}

void Zpopup_set_stays_open(Zid id, int stays_open) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_POPUP);
	if (e) ZST_AS(zrn_popup_t, e)->stays_open = stays_open != 0;
}


/* ============================================================================
 *  Control
 * ============================================================================ */

ZColor zrn_ctl_color(const zrn_control_t* c, unsigned flag, ZColor stored, ZColor fallback) {
	ZST_UNUSED(c);
	return (c->set_flags & flag) ? stored : fallback;
}

ZColor zrn_ctl_foreground(const zrn_control_t* c) {
	return zrn_ctl_color(c, ZST_C_FOREGROUND, c->foreground, zrn_theme_color(&c->base, ZST_TC_FOREGROUND));
}

ZColor zrn_ctl_background(const zrn_control_t* c, ZColor fallback) { return zrn_ctl_color(c, ZST_C_BACKGROUND, c->background, fallback); }
ZColor zrn_ctl_border_brush(const zrn_control_t* c, ZColor fallback) { return zrn_ctl_color(c, ZST_C_BORDER_BRUSH, c->border_brush, fallback); }

static void control_init(zrn_element_t* e) {
	zrn_control_t* c = ZST_AS(zrn_control_t, e);
	c->font_size = ZST_FONT_NORMAL;
	c->font_family = Z_FONT_NORMAL;
	c->h_content = Z_HALIGN_CENTER;
	c->v_content = Z_VALIGN_CENTER;
	e->focusable = 1;
}

static ZColor control_content_foreground(zrn_element_t* e) {
	zrn_control_t* c = ZST_AS(zrn_control_t, e);
	if (!zrn_effectively_enabled(e)) return zrn_theme_color(e, ZST_TC_DISABLED);
	return zrn_ctl_foreground(c);
}

static float control_content_font_size(zrn_element_t* e) { return ZST_AS(zrn_control_t, e)->font_size; }
static ZFontFamily control_content_font_family(zrn_element_t* e) { return ZST_AS(zrn_control_t, e)->font_family; }

zrn_class_t zrn_class_control = {
	.base = &zrn_class_element, .name = "Control", .id = Z_CLASS_CONTROL, .size = sizeof(zrn_control_t), .abstract_class = 1,
	.init = control_init, .content_foreground = control_content_foreground, .content_font_size = control_content_font_size,
	.content_font_family = control_content_font_family
};

static zrn_control_t* get_control(Zid id) { return ZST_AS(zrn_control_t, zrn_get(id, Z_CLASS_CONTROL)); }

void Zcontrol_set_foreground(Zid id, ZColor color) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_FOREGROUND);
	c->foreground = color;
	c->set_flags |= ZST_C_FOREGROUND;
	zrn_changed_paint(&c->base);
}

void Zcontrol_set_background(Zid id, ZColor color) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_BACKGROUND);
	c->background = color;
	c->set_flags |= ZST_C_BACKGROUND;
	zrn_changed_paint(&c->base);
}

void Zcontrol_set_border_brush(Zid id, ZColor color) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_BORDER_BRUSH);
	c->border_brush = color;
	c->set_flags |= ZST_C_BORDER_BRUSH;
	zrn_changed_paint(&c->base);
}

void Zcontrol_set_border_thickness(Zid id, ZThickness t) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_BORDER_THICKNESS);
	c->border_thickness = t;
	c->set_flags |= ZST_C_BORDER_THICKNESS;
	zrn_changed_layout(&c->base);
}

void Zcontrol_set_padding(Zid id, ZThickness t) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_PADDING);
	c->padding = t;
	c->set_flags |= ZST_C_PADDING;
	zrn_changed_layout(&c->base);
}

ZThickness Zcontrol_get_padding(Zid id) {
	zrn_control_t* c = get_control(id);
	return c ? c->padding : zrn_thickness_zero();
}

void Zcontrol_set_font_size(Zid id, float size) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	zrn_unbind(&c->base, ZST_BP_CONTROL_FONT_SIZE);
	c->font_size = size;
	c->set_flags |= ZST_C_FONT_SIZE;
	zrn_invalidate_deep(&c->base);
}

float Zcontrol_get_font_size(Zid id) {
	zrn_control_t* c = get_control(id);
	return c ? c->font_size : 0;
}

void Zcontrol_set_font_family(Zid id, ZFontFamily family) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	c->font_family = family;
	c->set_flags |= ZST_C_FONT_FAMILY;
	zrn_invalidate_deep(&c->base);
}

void Zcontrol_set_horizontal_content_alignment(Zid id, ZHorizontalAlignment a) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	c->h_content = a;
	zrn_changed_layout(&c->base);
}

void Zcontrol_set_vertical_content_alignment(Zid id, ZVerticalAlignment a) {
	zrn_control_t* c = get_control(id);
	if (!c) return;
	c->v_content = a;
	zrn_changed_layout(&c->base);
}

void Zcontrol_set_tab_stop(Zid id, int is_tab_stop) {
	zrn_control_t* c = get_control(id);
	if (c) c->base.tab_stop = is_tab_stop != 0;
}

void Zcontrol_set_tab_index(Zid id, int tab_index) {
	zrn_control_t* c = get_control(id);
	if (c) c->base.tab_index = tab_index;
}


/* ============================================================================
 *  ContentControl
 * ============================================================================ */

ZThickness zrn_cc_insets(zrn_element_t* e) {
	zrn_control_t* c = ZST_AS(zrn_control_t, e);
	ZThickness chrome = e->cls->chrome ? e->cls->chrome(e) : zrn_thickness_zero();
	ZThickness t;
	t.left = chrome.left + c->border_thickness.left + c->padding.left;
	t.top = chrome.top + c->border_thickness.top + c->padding.top;
	t.right = chrome.right + c->border_thickness.right + c->padding.right;
	t.bottom = chrome.bottom + c->border_thickness.bottom + c->padding.bottom;
	return t;
}

ZSize zrn_cc_measure(zrn_element_t* e, ZSize available) {
	zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	ZThickness in = zrn_cc_insets(e);
	ZSize content = { 0, 0 };
	if (cc->content) {
		zrn_measure(cc->content, zrn_size(zrn_maxf(0, available.width - zrn_thickness_h(in)), zrn_maxf(0, available.height - zrn_thickness_v(in))));
		content = cc->content->desired;
	}
	return zrn_size(content.width + zrn_thickness_h(in), content.height + zrn_thickness_v(in));
}

ZSize zrn_cc_arrange(zrn_element_t* e, ZSize final) {
	zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	zrn_control_t* c = &cc->base;
	ZThickness in = zrn_cc_insets(e);
	ZRect inner = zrn_rect(in.left, in.top, zrn_maxf(0, final.width - zrn_thickness_h(in)), zrn_maxf(0, final.height - zrn_thickness_v(in)));
	if (cc->content) {
		ZSize d = cc->content->desired;
		ZRect r = inner;
		/* ContentPresenter: the alignment of the content is the control's HorizontalContentAlignment */
		if (c->h_content != Z_HALIGN_STRETCH) {
			r.width = zrn_minf(inner.width, d.width);
			if (c->h_content == Z_HALIGN_CENTER) r.x += (inner.width - r.width) * 0.5f;
			else if (c->h_content == Z_HALIGN_RIGHT) r.x += inner.width - r.width;
		}
		if (c->v_content != Z_VALIGN_STRETCH) {
			r.height = zrn_minf(inner.height, d.height);
			if (c->v_content == Z_VALIGN_CENTER) r.y += (inner.height - r.height) * 0.5f;
			else if (c->v_content == Z_VALIGN_BOTTOM) r.y += inner.height - r.height;
		}
		zrn_arrange(cc->content, r);
	}
	return final;
}

void zrn_cc_child_removed(zrn_element_t* e, zrn_element_t* child) {
	zrn_content_control_t* cc = ZST_AS(zrn_content_control_t, e);
	if (cc->content == child) {
		cc->content = NULL;
		cc->content_is_text = 0;
	}
}

zrn_class_t zrn_class_content_control = {
	.base = &zrn_class_control, .name = "ContentControl", .id = Z_CLASS_CONTENT_CONTROL, .size = sizeof(zrn_content_control_t),
	.measure = zrn_cc_measure, .arrange = zrn_cc_arrange, .child_removed = zrn_cc_child_removed
};

void zrn_content_set(zrn_content_control_t* c, zrn_element_t* content) {
	c->content_is_text = 0;
	zrn_set_only_child(&c->base.base, &c->content, content);
}

void zrn_content_set_text(zrn_content_control_t* c, const char* text) {
	if (c->content && c->content_is_text) {
		Ztext_block_set_text(c->content->id, text);
		return;
	}
	{
		zrn_element_t* tb = zrn_create(Z_CLASS_TEXT_BLOCK);
		if (!tb) return;
		zrn_set_string(&ZST_AS(zrn_text_block_t, tb)->text, text ? text : "");
		zrn_content_set(c, tb);
		c->content_is_text = c->content == tb;
	}
}

static zrn_content_control_t* get_cc(Zid id) { return ZST_AS(zrn_content_control_t, zrn_get(id, Z_CLASS_CONTENT_CONTROL)); }

Zid Zcontent_control_create(void) {
	zrn_element_t* e = zrn_create(Z_CLASS_CONTENT_CONTROL);
	return e ? e->id : Z_INVALID_ID;
}

void Zcontent_control_set_content(Zid id, Zid content) {
	zrn_content_control_t* c = get_cc(id);
	zrn_element_t* e = content != Z_INVALID_ID ? zrn_get(content, Z_CLASS_ELEMENT) : NULL;
	if (!c || (content != Z_INVALID_ID && !e)) return;
	zrn_content_set(c, e);
}

void Zcontent_control_set_text(Zid id, const char* text) {
	zrn_content_control_t* c = get_cc(id);
	if (c) zrn_content_set_text(c, text);
}

Zid Zcontent_control_get_content(Zid id) {
	zrn_content_control_t* c = get_cc(id);
	return c && c->content ? c->content->id : Z_INVALID_ID;
}

const char* Zcontent_control_get_text(Zid id) {
	zrn_content_control_t* c = get_cc(id);
	if (!c || !c->content || !c->content_is_text) return "";
	return Ztext_block_get_text(c->content->id);
}


/* ============================================================================
 *  UserControl
 * ============================================================================ */

zrn_class_t zrn_class_user_control = {
	.base = &zrn_class_control, .name = "UserControl", .id = Z_CLASS_USER_CONTROL, .size = sizeof(zrn_content_control_t),
	.measure = zrn_cc_measure, .arrange = zrn_cc_arrange, .child_removed = zrn_cc_child_removed
};

Zid Zuser_control_create(void) {
	zrn_element_t* e = zrn_create(Z_CLASS_USER_CONTROL);
	if (!e) return Z_INVALID_ID;
	e->focusable = 0;
	ZST_AS(zrn_control_t, e)->h_content = Z_HALIGN_STRETCH;
	ZST_AS(zrn_control_t, e)->v_content = Z_VALIGN_STRETCH;
	return e->id;
}

void Zuser_control_set_content(Zid id, Zid child) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_USER_CONTROL);
	zrn_element_t* c = child != Z_INVALID_ID ? zrn_get(child, Z_CLASS_ELEMENT) : NULL;
	if (!e || (child != Z_INVALID_ID && !c)) return;
	zrn_content_set(ZST_AS(zrn_content_control_t, e), c);
}


Zid Zuser_control_get_content(Zid id) {
	zrn_element_t* e = zrn_get(id, Z_CLASS_USER_CONTROL);
	return e && ZST_AS(zrn_content_control_t, e)->content ? ZST_AS(zrn_content_control_t, e)->content->id : Z_INVALID_ID;
}


/* ============================================================================
 *  ToolTip
 * ============================================================================ */

static void tooltip_paint(zrn_element_t* e, zrn_painter_t* p) {
	ZRect r = zrn_local_rect(e);
	zrn_fill_rect(p, r, zrn_theme_color(e, ZST_TC_CHROME));
	zrn_stroke_rect(p, r, Zthickness_uniform(1), zrn_theme_color(e, ZST_TC_BORDER));
}

static void tooltip_init(zrn_element_t* e) {
	zrn_control_t* c = ZST_AS(zrn_control_t, e);
	e->focusable = 0;
	c->padding = Zthickness(8, 4, 8, 4);
	c->border_thickness = Zthickness_uniform(1);
	c->font_size = ZST_FONT_SMALL;
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
}

zrn_class_t zrn_class_tooltip = {
	.base = &zrn_class_content_control, .name = "ToolTip", .id = Z_CLASS_TOOLTIP, .size = sizeof(zrn_content_control_t),
	.init = tooltip_init, .paint = tooltip_paint
};

Zid Ztool_tip_create(const char* text) {
	zrn_element_t* e = zrn_create(Z_CLASS_TOOLTIP);
	if (!e) return Z_INVALID_ID;
	zrn_content_set_text(ZST_AS(zrn_content_control_t, e), text);
	return e->id;
}


/* ============================================================================
 *  Separator
 * ============================================================================ */

static ZSize separator_measure(zrn_element_t* e, ZSize available) {
	ZST_UNUSED(e); ZST_UNUSED(available);
	return zrn_size(1, 1);
}

static void separator_paint(zrn_element_t* e, zrn_painter_t* p) {
	zrn_control_t* c = ZST_AS(zrn_control_t, e);
	zrn_fill_rect(p, zrn_local_rect(e), zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_INACTIVE)));
}

static void separator_init(zrn_element_t* e) { e->focusable = 0; }

zrn_class_t zrn_class_separator = {
	.base = &zrn_class_control, .name = "Separator", .id = Z_CLASS_SEPARATOR, .size = sizeof(zrn_control_t),
	.init = separator_init, .measure = separator_measure, .paint = separator_paint
};

Zid Zseparator_create(void) {
	zrn_element_t* e = zrn_create(Z_CLASS_SEPARATOR);
	return e ? e->id : Z_INVALID_ID;
}
