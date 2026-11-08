/**
 * Kestrel - resources.c
 * ThemeResource: the values of the keys of themeresources.xaml (theme_resources.c) for the theme and the accent
 * of a root, the overrides an application sets, its own keys, and the bindings of properties to keys.
 */

#include "internal.h"

#define USER_KEY(key) ((int)(key) >= (int)K_RES_USER_BASE)
#define BUILTIN_KEY(key) ((int)(key) >= (int)K_RES_FIRST_ && (int)(key) < (int)K_RES_LAST_)

static kst_root_t* get_root(Kid id) { return kst_root_lookup(id); }

static const kst_res_def_t* def_of(KThemeResource key) {
	return BUILTIN_KEY(key) ? &kst_res_table[(int)key - (int)K_RES_FIRST_] : NULL;
}

static const kst_res_override_t* find_override(const kst_root_t* r, KThemeResource key) {
	int i;
	if (!r) return NULL;
	for (i = 0; i < r->override_count; i++) if (r->overrides[i].key == key) return &r->overrides[i];
	return NULL;
}

static int is_light(const kst_root_t* r) { return r && r->theme.theme == K_THEME_LIGHT; }

/* the table value of a key of a type for the theme of the root, or NULL */
static const kst_res_value_t* table_value(const kst_root_t* r, KThemeResource key, kst_res_type_t type, int* accent) {
	const kst_res_def_t* d = def_of(key);
	int light = is_light(r);
	if (!d || d->type != type) return NULL;
	if (accent) *accent = (d->accent & (light ? 2 : 1)) != 0;
	return light ? &d->light : &d->dark;
}

int kst_res_color(const kst_root_t* r, KThemeResource key, KColor* out) {
	const kst_res_override_t* o = find_override(r, key);
	const kst_res_value_t* v;
	int accent = 0;
	if (o) {
		if (o->type != KST_RT_COLOR) return K_FALSE;
		*out = o->v.color;
		return K_TRUE;
	}
	v = table_value(r, key, KST_RT_COLOR, &accent);
	if (!v) return K_FALSE;
	if (accent) {
		KColor a = r ? r->theme.accent : kst_accent_color(K_ACCENT_COBALT);
		a.a *= (float)(v->argb >> 24) / 255.0f;
		*out = a;
	} else {
		*out = Kcolor_argb(v->argb);
	}
	return K_TRUE;
}

int kst_res_double(const kst_root_t* r, KThemeResource key, float* out) {
	const kst_res_override_t* o = find_override(r, key);
	const kst_res_value_t* v;
	if (o) {
		if (o->type != KST_RT_DOUBLE) return K_FALSE;
		*out = o->v.d;
		return K_TRUE;
	}
	v = table_value(r, key, KST_RT_DOUBLE, NULL);
	if (!v) return K_FALSE;
	*out = v->f[0];
	return K_TRUE;
}

int kst_res_int(const kst_root_t* r, KThemeResource key, int* out) {
	const kst_res_override_t* o = find_override(r, key);
	const kst_res_value_t* v;
	if (o) {
		if (o->type != KST_RT_INT) return K_FALSE;
		*out = o->v.i;
		return K_TRUE;
	}
	v = table_value(r, key, KST_RT_INT, NULL);
	if (!v) return K_FALSE;
	*out = v->i;
	return K_TRUE;
}

int kst_res_thickness(const kst_root_t* r, KThemeResource key, KThickness* out) {
	const kst_res_override_t* o = find_override(r, key);
	const kst_res_value_t* v;
	if (o) {
		if (o->type != KST_RT_THICKNESS) return K_FALSE;
		*out = o->v.thickness;
		return K_TRUE;
	}
	v = table_value(r, key, KST_RT_THICKNESS, NULL);
	if (!v) return K_FALSE;
	*out = Kthickness(v->f[0], v->f[1], v->f[2], v->f[3]);
	return K_TRUE;
}

int kst_res_font_family(const kst_root_t* r, KThemeResource key, KFontFamily* out) {
	const kst_res_override_t* o = find_override(r, key);
	const kst_res_value_t* v;
	if (o) {
		if (o->type != KST_RT_FONT_FAMILY) return K_FALSE;
		*out = (KFontFamily)o->v.i;
		return K_TRUE;
	}
	v = table_value(r, key, KST_RT_FONT_FAMILY, NULL);
	if (!v) return K_FALSE;
	*out = (KFontFamily)v->i;
	return K_TRUE;
}


/* ============================================================================
 *  public: values
 * ============================================================================ */

KColor Kroot_get_theme_color(Kid root, KThemeResource key) {
	KColor c = Kcolor_rgba(0, 0, 0, 0);
	kst_root_t* r = get_root(root);
	if (r) kst_res_color(r, key, &c);
	return c;
}

float Kroot_get_theme_double(Kid root, KThemeResource key) {
	float v = 0;
	kst_root_t* r = get_root(root);
	if (r) kst_res_double(r, key, &v);
	return v;
}

int Kroot_get_theme_int(Kid root, KThemeResource key) {
	int v = 0;
	kst_root_t* r = get_root(root);
	if (r) kst_res_int(r, key, &v);
	return v;
}

KThickness Kroot_get_theme_thickness(Kid root, KThemeResource key) {
	KThickness v = kst_thickness_zero();
	kst_root_t* r = get_root(root);
	if (r) kst_res_thickness(r, key, &v);
	return v;
}

KFontFamily Kroot_get_theme_font_family(Kid root, KThemeResource key) {
	KFontFamily v = K_FONT_NORMAL;
	kst_root_t* r = get_root(root);
	if (r) kst_res_font_family(r, key, &v);
	return v;
}


/* ============================================================================
 *  public: overrides
 * ============================================================================ */

/* the override slot of a key for a value of a type, made when there is none; NULL when the key cannot take it */
static kst_res_override_t* override_slot(kst_root_t* r, KThemeResource key, kst_res_type_t type) {
	kst_res_override_t* o = NULL;
	int i;
	if (BUILTIN_KEY(key)) {
		if (def_of(key)->type != type) {
			kst_error(K_ERROR_INVALID_ARGUMENT, "resource %s is not of that type", def_of(key)->name);
			return NULL;
		}
	} else if (!USER_KEY(key)) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "invalid resource key 0x%X", (unsigned)key);
		return NULL;
	}
	for (i = 0; i < r->override_count; i++) if (r->overrides[i].key == key) o = &r->overrides[i];
	if (!o) {
		if (r->override_count == r->override_cap) {
			int cap = r->override_cap ? r->override_cap * 2 : 8;
			kst_res_override_t* grown = (kst_res_override_t*)realloc(r->overrides, (size_t)cap * sizeof *grown);
			if (!grown) {
				kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
				return NULL;
			}
			r->overrides = grown;
			r->override_cap = cap;
		}
		o = &r->overrides[r->override_count++];
		o->key = key;
	}
	/* a user key takes the type of the value set now */
	memset(&o->v, 0, sizeof o->v);
	o->type = type;
	return o;
}

#define RES_SET(name, ctype, rtype, assign) \
	int name(Kid root, KThemeResource key, ctype value) { \
		kst_root_t* r = get_root(root); \
		kst_res_override_t* o; \
		if (!r) return K_FALSE; \
		o = override_slot(r, key, rtype); \
		if (!o) return K_FALSE; \
		assign; \
		kst_root_resources_changed(r); \
		return K_TRUE; \
	}

RES_SET(Kroot_set_resource_color, KColor, KST_RT_COLOR, o->v.color = value)
RES_SET(Kroot_set_resource_double, float, KST_RT_DOUBLE, o->v.d = value)
RES_SET(Kroot_set_resource_int, int, KST_RT_INT, o->v.i = value)
RES_SET(Kroot_set_resource_thickness, KThickness, KST_RT_THICKNESS, o->v.thickness = value)
RES_SET(Kroot_set_resource_font_family, KFontFamily, KST_RT_FONT_FAMILY, o->v.i = (int)value)

void Kroot_clear_resource(Kid root, KThemeResource key) {
	kst_root_t* r = get_root(root);
	int i;
	if (!r) return;
	for (i = 0; i < r->override_count; i++) {
		if (r->overrides[i].key != key) continue;
		r->overrides[i] = r->overrides[--r->override_count];
		kst_root_resources_changed(r);
		return;
	}
}

void kst_root_resources_free(kst_root_t* r) {
	free(r->overrides);
	r->overrides = NULL;
	r->override_count = r->override_cap = 0;
}


/* ============================================================================
 *  bindings
 * ============================================================================ */

static kst_res_type_t prop_type(kst_bind_prop_t prop) {
	switch (prop) {
		case KST_BP_ELEMENT_MARGIN:
		case KST_BP_CONTROL_BORDER_THICKNESS:
		case KST_BP_CONTROL_PADDING:
		case KST_BP_BORDER_BORDER_THICKNESS:
		case KST_BP_BORDER_PADDING:
			return KST_RT_THICKNESS;
		case KST_BP_TEXT_FONT_SIZE:
		case KST_BP_CONTROL_FONT_SIZE:
			return KST_RT_DOUBLE;
		case KST_BP_TEXT_CHARACTER_SPACING:
			return KST_RT_INT;
		default:
			return KST_RT_COLOR;
	}
}

/* Resolves a binding and writes the property. A key that has no value (a user key not set) leaves the property as it is. */
static void apply_binding(kst_element_t* e, const kst_binding_t* b) {
	const kst_root_t* r = e->root;
	KColor c = Kcolor_rgba(0, 0, 0, 0);
	KThickness t = kst_thickness_zero();
	float d = 0;
	int i = 0;
	switch (prop_type(b->prop)) {
		case KST_RT_COLOR: if (!kst_res_color(r, b->key, &c)) return; break;
		case KST_RT_THICKNESS: if (!kst_res_thickness(r, b->key, &t)) return; break;
		case KST_RT_DOUBLE: if (!kst_res_double(r, b->key, &d)) return; break;
		default: if (!kst_res_int(r, b->key, &i)) return; break;
	}
	switch (b->prop) {
		case KST_BP_ELEMENT_MARGIN:
			e->margin = t;
			kst_changed_layout(e);
			break;
		case KST_BP_TEXT_FOREGROUND: {
			kst_text_block_t* tb = KST_AS(kst_text_block_t, e);
			tb->foreground = c;
			tb->set_flags |= KST_TB_FOREGROUND;
			kst_changed_paint(e);
			break;
		}
		case KST_BP_TEXT_FONT_SIZE: {
			kst_text_block_t* tb = KST_AS(kst_text_block_t, e);
			tb->font_size = d;
			tb->set_flags |= KST_TB_FONT_SIZE;
			kst_changed_layout(e);
			break;
		}
		case KST_BP_TEXT_CHARACTER_SPACING:
			KST_AS(kst_text_block_t, e)->character_spacing = i;
			kst_changed_layout(e);
			break;
		case KST_BP_CONTROL_FOREGROUND:
			KST_AS(kst_control_t, e)->foreground = c;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_FOREGROUND;
			kst_changed_paint(e);
			break;
		case KST_BP_CONTROL_BACKGROUND:
			KST_AS(kst_control_t, e)->background = c;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_BACKGROUND;
			kst_changed_paint(e);
			break;
		case KST_BP_CONTROL_BORDER_BRUSH:
			KST_AS(kst_control_t, e)->border_brush = c;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_BORDER_BRUSH;
			kst_changed_paint(e);
			break;
		case KST_BP_CONTROL_BORDER_THICKNESS:
			KST_AS(kst_control_t, e)->border_thickness = t;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_BORDER_THICKNESS;
			kst_changed_layout(e);
			break;
		case KST_BP_CONTROL_PADDING:
			KST_AS(kst_control_t, e)->padding = t;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_PADDING;
			kst_changed_layout(e);
			break;
		case KST_BP_CONTROL_FONT_SIZE:
			KST_AS(kst_control_t, e)->font_size = d;
			KST_AS(kst_control_t, e)->set_flags |= KST_C_FONT_SIZE;
			kst_invalidate_deep(e);
			break;
		case KST_BP_BORDER_BACKGROUND:
			KST_AS(kst_border_t, e)->background = c;
			KST_AS(kst_border_t, e)->has_background = 1;
			kst_changed_paint(e);
			break;
		case KST_BP_BORDER_BORDER_BRUSH:
			KST_AS(kst_border_t, e)->border_brush = c;
			KST_AS(kst_border_t, e)->has_border_brush = 1;
			kst_changed_paint(e);
			break;
		case KST_BP_BORDER_BORDER_THICKNESS:
			KST_AS(kst_border_t, e)->border_thickness = t;
			kst_changed_layout(e);
			break;
		case KST_BP_BORDER_PADDING:
			KST_AS(kst_border_t, e)->padding = t;
			kst_changed_layout(e);
			break;
	}
}

int kst_bind(kst_element_t* e, kst_bind_prop_t prop, KThemeResource key) {
	kst_binding_t* b = NULL;
	int i;
	if (!e) return K_FALSE;
	if (BUILTIN_KEY(key)) {
		if (def_of(key)->type != prop_type(prop)) {
			kst_error(K_ERROR_INVALID_ARGUMENT, "resource %s is not of the type of the property", def_of(key)->name);
			return K_FALSE;
		}
	} else if (!USER_KEY(key)) {
		kst_error(K_ERROR_INVALID_ARGUMENT, "invalid resource key 0x%X", (unsigned)key);
		return K_FALSE;
	}
	for (i = 0; i < e->binding_count; i++) if (e->bindings[i].prop == prop) b = &e->bindings[i];
	if (!b) {
		/* grown one by one: an element binds a few properties at most */
		kst_binding_t* grown = (kst_binding_t*)realloc(e->bindings, (size_t)(e->binding_count + 1) * sizeof *grown);
		if (!grown) {
			kst_error(K_ERROR_OUT_OF_MEMORY, "out of memory");
			return K_FALSE;
		}
		e->bindings = grown;
		b = &e->bindings[e->binding_count++];
		b->prop = prop;
	}
	b->key = key;
	apply_binding(e, b);
	return K_TRUE;
}

void kst_unbind(kst_element_t* e, kst_bind_prop_t prop) {
	int i;
	if (!e || !e->binding_count) return;
	for (i = 0; i < e->binding_count; i++) {
		if (e->bindings[i].prop != prop) continue;
		e->bindings[i] = e->bindings[--e->binding_count];
		if (!e->binding_count) {
			free(e->bindings);
			e->bindings = NULL;
		}
		return;
	}
}

void kst_bindings_apply(kst_element_t* e) {
	int i;
	/* applying changes no list: a copy is not needed, but the count is read each time */
	for (i = 0; i < e->binding_count; i++) apply_binding(e, &e->bindings[i]);
}

void kst_bindings_free(kst_element_t* e) {
	free(e->bindings);
	e->bindings = NULL;
	e->binding_count = 0;
}

static void apply_tree(kst_element_t* e) {
	int i;
	if (!e) return;
	kst_bindings_apply(e);
	for (i = 0; i < e->child_count; i++) apply_tree(e->children[i]);
}

void kst_root_resources_changed(kst_root_t* r) {
	int i;
	/* the popups that are open may be in no tree of the root's own: they are listed apart */
	apply_tree(r->visual);
	apply_tree(r->overlay);
	for (i = 0; i < r->popup_count; i++) apply_tree(r->popups[i]);
	r->paint_dirty = 1;
}

#define BIND_FN(name, cls, prop) \
	int name(Kid id, KThemeResource key) { \
		kst_element_t* e = kst_get(id, cls); \
		return e ? kst_bind(e, prop, key) : K_FALSE; \
	}

BIND_FN(Kelement_bind_margin, K_CLASS_ELEMENT, KST_BP_ELEMENT_MARGIN)
BIND_FN(Ktext_block_bind_foreground, K_CLASS_TEXT_BLOCK, KST_BP_TEXT_FOREGROUND)
BIND_FN(Ktext_block_bind_font_size, K_CLASS_TEXT_BLOCK, KST_BP_TEXT_FONT_SIZE)
BIND_FN(Ktext_block_bind_character_spacing, K_CLASS_TEXT_BLOCK, KST_BP_TEXT_CHARACTER_SPACING)
BIND_FN(Kcontrol_bind_foreground, K_CLASS_CONTROL, KST_BP_CONTROL_FOREGROUND)
BIND_FN(Kcontrol_bind_background, K_CLASS_CONTROL, KST_BP_CONTROL_BACKGROUND)
BIND_FN(Kcontrol_bind_border_brush, K_CLASS_CONTROL, KST_BP_CONTROL_BORDER_BRUSH)
BIND_FN(Kcontrol_bind_border_thickness, K_CLASS_CONTROL, KST_BP_CONTROL_BORDER_THICKNESS)
BIND_FN(Kcontrol_bind_padding, K_CLASS_CONTROL, KST_BP_CONTROL_PADDING)
BIND_FN(Kcontrol_bind_font_size, K_CLASS_CONTROL, KST_BP_CONTROL_FONT_SIZE)
BIND_FN(Kborder_bind_background, K_CLASS_BORDER, KST_BP_BORDER_BACKGROUND)
BIND_FN(Kborder_bind_border_brush, K_CLASS_BORDER, KST_BP_BORDER_BORDER_BRUSH)
BIND_FN(Kborder_bind_border_thickness, K_CLASS_BORDER, KST_BP_BORDER_BORDER_THICKNESS)
BIND_FN(Kborder_bind_padding, K_CLASS_BORDER, KST_BP_BORDER_PADDING)
