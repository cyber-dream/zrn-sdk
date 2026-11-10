/**
 * zrnui - controls/textbox.c
 * TextBox and PasswordBox: editing, selection, caret, clipboard, and the
 * Windows Phone 8.1 look (generic.xaml): a header over a 2.5 px bordered box,
 * 34 px high at least, light at 0.8 opacity that turns white with an accent
 * border on focus; PlaceholderText shows in it while it is empty and has no focus.
 */

#include "../internal.h"

#define TB(e) ZST_AS(zrn_text_box_t, e)

/* TextControlThemeMinHeight / MinWidth, TextControlHeaderMarginThemeThickness (the 4.8 under the header) */
static constexpr float TBX_MIN_HEIGHT = 34.0f;
static constexpr float TBX_MIN_WIDTH = 51.0f;
static constexpr float TBX_HEADER_GAP = 4.8f;
static constexpr float BLINK_MS = 530.0f;

static char* g_clipboard;


/* ============================================================================
 *  geometry
 * ============================================================================ */

static zrn_font_t tbx_font(const zrn_text_box_t* const b) {
	zrn_font_t f;
	f.family = b->base.font_family;
	f.size = b->base.font_size;
	f.spacing = 0;
	return f;
}

static uint32_t bullet_char(const zrn_text_box_t* b);

static zrn_font_t tbx_header_font(void) { const zrn_font_t f = { Z_FONT_NORMAL, ZST_81_FONT_CONTROL_HEADER, 0 }; return f; }

/* The header takes a row of its own above the box. */
static float tbx_header_height(const zrn_text_box_t* const b) {
	return b->header && *b->header ? zrn_font_line_height(tbx_header_font()) + TBX_HEADER_GAP : 0;
}

static ZThickness tbx_chrome(zrn_element_t* const e) {
	return Zthickness(0, tbx_header_height(TB(e)), 0, 0);
}

static ZThickness tbx_insets(const zrn_element_t* const e) {
	const zrn_control_t* c = ZST_AS(const zrn_control_t, e);
	ZThickness ch = tbx_chrome((zrn_element_t*)e), t;
	t.left = ch.left + c->border_thickness.left + c->padding.left;
	t.top = ch.top + c->border_thickness.top + c->padding.top;
	t.right = ch.right + c->border_thickness.right + c->padding.right;
	t.bottom = ch.bottom + c->border_thickness.bottom + c->padding.bottom;
	return t;
}

/* The area of the text, in local coordinates. */
static ZRect tbx_content(const zrn_element_t* const e) {
	const ZThickness in = tbx_insets(e);
	return zrn_rect(in.left, in.top, zrn_maxf(0, e->rect.width - zrn_thickness_h(in)), zrn_maxf(0, e->rect.height - zrn_thickness_v(in)));
}

/* What is shown: the text, or one bullet per character for a password. The caller frees when *heap. */
static const char* tbx_display(const zrn_text_box_t* const b, int* const length, int* const heap) {
	*heap = 0;
	if (!b->password) {
		*length = b->length_bytes;
		return b->text;
	}
	{
		char bullet[4];
		int bl = zrn_utf8_encode(bullet_char(b), bullet), count = 0, i = 0;
		char* out;
		while (i < b->length_bytes) {
			i = zrn_utf8_next(b->text, b->length_bytes, i);
			count++;
		}
		out = (char*)malloc((size_t)count * (size_t)bl + 1);
		if (!out) {
			*length = 0;
			return "";
		}
		for (i = 0; i < count; i++) memcpy(out + i * bl, bullet, (size_t)bl);
		out[count * bl] = 0;
		*length = count * bl;
		*heap = 1;
		return out;
	}
}

/* Frees what tbx_display allocated: the string is const for the reader, but owned by the caller when `heap`. */
static void tbx_display_release(const char* const disp, const int heap) {
	if (heap) free((void*)(uintptr_t)disp);
}

/* The bullet of the box, or U+2022 when no font has it. */
static uint32_t bullet_char(const zrn_text_box_t* const b) {
	const uint32_t cp = b->password_char ? b->password_char : 0x25CF;
	return zrn_font_has_glyph(tbx_font(b), cp) ? cp : 0x2022;
}

static int bullet_length(const zrn_text_box_t* const b) {
	char bullet[4];
	return zrn_utf8_encode(bullet_char(b), bullet);
}

static int real_to_display(const zrn_text_box_t* const b, const int index) {
	int count = 0, i = 0;
	if (!b->password) return index;
	while (i < index) {
		i = zrn_utf8_next(b->text, b->length_bytes, i);
		count++;
	}
	return count * bullet_length(b);
}

static int display_to_real(const zrn_text_box_t* const b, const int index) {
	int count, i = 0, k = 0;
	if (!b->password) return index;
	count = index / bullet_length(b);
	while (k < count && i < b->length_bytes) {
		i = zrn_utf8_next(b->text, b->length_bytes, i);
		k++;
	}
	return i;
}

static int tbx_wrap(const zrn_text_box_t* b) { return b->wrapping == Z_WRAP; }

static zrn_text_layout_t* tbx_layout(const zrn_text_box_t* const b, const char* const disp, const int length, const float width) {
	const int wrap = tbx_wrap(b) && width > 0;
	return zrn_text_layout_new(tbx_font(b), disp, length, wrap ? width : 0, wrap, 0, 0);
}

/* The line and the x of a display index. */
static void caret_position(const zrn_text_layout_t* const l, const int index, int* const line, float* const x) {
	int i;
	for (i = 0; i < l->line_count; i++) {
		const zrn_line_t* ln = &l->lines[i];
		const int next_start = i + 1 < l->line_count ? l->lines[i + 1].start : 0x7fffffff;
		if (index <= ln->end || index < next_start || i == l->line_count - 1) {
			const int upto = index < ln->start ? ln->start : (index > ln->end ? ln->end : index);
			*line = i;
			*x = zrn_text_width(l->font, l->text + ln->start, upto - ln->start);
			return;
		}
	}
	*line = 0;
	*x = 0;
}

static int index_at(const zrn_text_layout_t* const l, const float x, const float y) {
	int line = l->line_height > 0 ? (int)floorf(y / l->line_height) : 0;
	const zrn_line_t* ln;
	if (line < 0) line = 0;
	if (line >= l->line_count) line = l->line_count - 1;
	ln = &l->lines[line];
	return ln->start + zrn_text_index_at(l->font, l->text + ln->start, ln->end - ln->start, x);
}


/* ============================================================================
 *  editing
 * ============================================================================ */

static int sel_min(const zrn_text_box_t* b) { return b->caret < b->anchor ? b->caret : b->anchor; }
static int sel_max(const zrn_text_box_t* b) { return b->caret < b->anchor ? b->anchor : b->caret; }

static int count_chars(const char* const s, const int length) {
	int n = 0, i = 0;
	while (i < length) {
		i = zrn_utf8_next(s, length, i);
		n++;
	}
	return n;
}

/* @return false when a handler destroyed the box. */
static int tbx_changed(zrn_text_box_t* const b, const int from_user) {
	zrn_element_t* const e = &b->base.base;
	b->caret_blink = 0;
	zrn_changed_layout(e);
	if (b->quiet) return 1;
	b->from_user = from_user;
	if (!zrn_raise(e, Z_EVENT_TEXT_CHANGED)) return 0;
	b->from_user = 0;
	return 1;
}

/* Replaces [start, end) with `insert`. Newlines are dropped unless the box accepts them. */
static void tbx_replace(zrn_text_box_t* b, int start, int end, const char* insert, const int from_user) {
	int ilen = insert ? (int)strlen(insert) : 0, i, kept = 0;
	char* filtered = nullptr;
	char* text;
	if (start < 0) start = 0;
	if (end > b->length_bytes) end = b->length_bytes;
	if (start > end) start = end;
	if (ilen) {
		filtered = (char*)malloc((size_t)ilen + 1);
		if (!filtered) return;
		for (i = 0; i < ilen; i++) {
			const char ch = insert[i];
			if (ch == '\r') continue;
			if (ch == '\n' && !b->accepts_return) continue;
			filtered[kept++] = ch;
		}
		filtered[kept] = 0;
		if (b->max_length > 0) {
			int room = b->max_length - (count_chars(b->text, b->length_bytes) - count_chars(b->text + start, end - start));
			int n = 0, k = 0;
			if (room < 0) room = 0;
			while (k < kept && n < room) {
				k = zrn_utf8_next(filtered, kept, k);
				n++;
			}
			kept = k;
			filtered[kept] = 0;
		}
	}
	if (kept == 0 && start == end) {
		free(filtered);
		return;
	}
	text = (char*)malloc((size_t)(b->length_bytes - (end - start) + kept) + 1);
	if (!text) {
		free(filtered);
		return;
	}
	memcpy(text, b->text, (size_t)start);
	if (kept) memcpy(text + start, filtered, (size_t)kept);
	memcpy(text + start + kept, b->text + end, (size_t)(b->length_bytes - end));
	b->length_bytes = b->length_bytes - (end - start) + kept;
	text[b->length_bytes] = 0;
	free(b->text);
	b->text = text;
	b->caret = b->anchor = start + kept;
	free(filtered);
	tbx_changed(b, from_user);
}

static void tbx_delete_selection(zrn_text_box_t* const b, const int from_user) {
	if (b->caret != b->anchor) tbx_replace(b, sel_min(b), sel_max(b), "", from_user);
}

static void tbx_move(zrn_text_box_t* b, int index, const int extend) {
	if (index < 0) index = 0;
	if (index > b->length_bytes) index = b->length_bytes;
	b->caret = index;
	if (!extend) b->anchor = index;
	b->caret_blink = 0;
	zrn_changed_paint(&b->base.base);
}

/* Zeeps the caret in view by moving the scroll offsets. */
static void tbx_ensure_visible(zrn_text_box_t* const b) {
	const zrn_element_t* const e = &b->base.base;
	const ZRect area = tbx_content(e);
	int length, heap, line;
	float x;
	const char* disp = tbx_display(b, &length, &heap);
	zrn_text_layout_t* const l = tbx_layout(b, disp, length, area.width);
	if (l) {
		caret_position(l, real_to_display(b, b->caret), &line, &x);
		if (!tbx_wrap(b)) {
			if (x - b->scroll_x > area.width - 2) b->scroll_x = x - area.width + 2;
			if (x - b->scroll_x < 0) b->scroll_x = x;
			if (l->width <= area.width) b->scroll_x = 0;
		} else {
			b->scroll_x = 0;
		}
		{
			const float top = (float)line * l->line_height, bottom = top + l->line_height;
			if (bottom - b->scroll_y > area.height) b->scroll_y = bottom - area.height;
			if (top - b->scroll_y < 0) b->scroll_y = top;
			if (l->height <= area.height) b->scroll_y = 0;
		}
		zrn_text_layout_free(l);
	}
	tbx_display_release(disp, heap);
}

static void tbx_set_text(zrn_text_box_t* b, const char* text) {
	if (!text) text = "";
	if (b->text && strcmp(b->text, text) == 0) return;
	zrn_set_string(&b->text, text);
	b->length_bytes = (int)strlen(b->text);
	b->caret = b->anchor = b->length_bytes;
	b->scroll_x = b->scroll_y = 0;
	tbx_changed(b, 0);
}


/* ============================================================================
 *  input
 * ============================================================================ */

static int word_left(const zrn_text_box_t* b, int index) {
	while (index > 0 && b->text[index - 1] == ' ') index--;
	while (index > 0 && b->text[index - 1] != ' ' && b->text[index - 1] != '\n') index--;
	return index;
}

static int word_right(const zrn_text_box_t* b, int index) {
	while (index < b->length_bytes && b->text[index] != ' ' && b->text[index] != '\n') index++;
	while (index < b->length_bytes && b->text[index] == ' ') index++;
	return index;
}

static void tbx_vertical_move(zrn_text_box_t* const b, const int dir, const int extend) {
	const zrn_element_t* e = &b->base.base;
	const ZRect area = tbx_content(e);
	int length, heap, line;
	float x;
	const char* disp = tbx_display(b, &length, &heap);
	zrn_text_layout_t* const l = tbx_layout(b, disp, length, area.width);
	if (l) {
		caret_position(l, real_to_display(b, b->caret), &line, &x);
		line += dir;
		if (line < 0) tbx_move(b, 0, extend);
		else if (line >= l->line_count) tbx_move(b, b->length_bytes, extend);
		else tbx_move(b, display_to_real(b, index_at(l, x, (float)line * l->line_height + 1)), extend);
		zrn_text_layout_free(l);
	}
	tbx_display_release(disp, heap);
}

static int tbx_key(zrn_element_t* const e, const zrn_input_t* const in) {
	const auto b = TB(e);
	const Zid id = e->id;
	const int shift = (in->mods & Z_MOD_SHIFT) != 0, ctrl = (in->mods & Z_MOD_CTRL) != 0;
	switch (in->key) {
		case Z_KEY_LEFT:
			if (b->caret != b->anchor && !shift) tbx_move(b, sel_min(b), 0);
			else tbx_move(b, ctrl ? word_left(b, b->caret) : zrn_utf8_prev(b->text, b->caret), shift);
			break;
		case Z_KEY_RIGHT:
			if (b->caret != b->anchor && !shift) tbx_move(b, sel_max(b), 0);
			else tbx_move(b, ctrl ? word_right(b, b->caret) : zrn_utf8_next(b->text, b->length_bytes, b->caret), shift);
			break;
		case Z_KEY_UP:
			if (!b->accepts_return && !tbx_wrap(b)) return 0;
			tbx_vertical_move(b, -1, shift);
			break;
		case Z_KEY_DOWN:
			if (!b->accepts_return && !tbx_wrap(b)) return 0;
			tbx_vertical_move(b, 1, shift);
			break;
		case Z_KEY_HOME:
			if (ctrl || !b->accepts_return) tbx_move(b, 0, shift);
			else {
				int i = b->caret;
				while (i > 0 && b->text[i - 1] != '\n') i--;
				tbx_move(b, i, shift);
			}
			break;
		case Z_KEY_END:
			if (ctrl || !b->accepts_return) tbx_move(b, b->length_bytes, shift);
			else {
				int i = b->caret;
				while (i < b->length_bytes && b->text[i] != '\n') i++;
				tbx_move(b, i, shift);
			}
			break;
		case Z_KEY_BACKSPACE:
			if (b->read_only) return 1;
			if (b->caret != b->anchor) tbx_delete_selection(b, 1);
			else if (b->caret > 0) tbx_replace(b, zrn_utf8_prev(b->text, b->caret), b->caret, "", 1);
			break;
		case Z_KEY_DELETE:
			if (b->read_only) return 1;
			if (b->caret != b->anchor) tbx_delete_selection(b, 1);
			else if (b->caret < b->length_bytes) tbx_replace(b, b->caret, zrn_utf8_next(b->text, b->length_bytes, b->caret), "", 1);
			break;
		case Z_KEY_ENTER:
			if (!b->accepts_return) return 0;
			if (!b->read_only) tbx_replace(b, sel_min(b), sel_max(b), "\n", 1);
			break;
		case Z_KEY_A:
			if (!ctrl) return 0;
			b->anchor = 0;
			b->caret = b->length_bytes;
			break;
		case Z_KEY_C:
		case Z_KEY_X:
			if (!ctrl) return 0;
			if (b->caret != b->anchor && !b->password) {
				const int s = sel_min(b), n = sel_max(b) - s;
				free(g_clipboard);
				g_clipboard = (char*)malloc((size_t)n + 1);
				if (g_clipboard) {
					memcpy(g_clipboard, b->text + s, (size_t)n);
					g_clipboard[n] = 0;
				}
				if (in->key == Z_KEY_X && !b->read_only) tbx_delete_selection(b, 1);
			}
			break;
		case Z_KEY_V:
			if (!ctrl) return 0;
			if (g_clipboard && !b->read_only) tbx_replace(b, sel_min(b), sel_max(b), g_clipboard, 1);
			break;
		default:
			return 0;
	}
	/* a TextChanged handler may have destroyed the box */
	if (!zrn_lookup(id)) return 1;
	tbx_ensure_visible(b);
	zrn_changed_paint(e);
	return 1;
}

static int tbx_index_from_point(zrn_text_box_t* const b, const float x, const float y) {
	const zrn_element_t* e = &b->base.base;
	const ZRect area = tbx_content(e);
	int length, heap, index = 0;
	const char* disp = tbx_display(b, &length, &heap);
	zrn_text_layout_t* const l = tbx_layout(b, disp, length, area.width);
	if (l) {
		index = display_to_real(b, index_at(l, x - area.x + b->scroll_x, y - area.y + b->scroll_y));
		zrn_text_layout_free(l);
	}
	tbx_display_release(disp, heap);
	return index;
}

static int tbx_input(zrn_element_t* const e, zrn_input_t* const in) {
	const auto b = TB(e);
	switch (in->type) {
		case ZST_IN_POINTER_DOWN:
			if (!zrn_effectively_enabled(e)) return 1;
			tbx_move(b, tbx_index_from_point(b, in->x, in->y), 0);
			b->selecting = 1;
			if (e->root) zrn_set_capture(e->root, e);
			return 1;
		case ZST_IN_POINTER_MOVE:
			if (!b->selecting) return 0;
			tbx_move(b, tbx_index_from_point(b, in->x, in->y), 1);
			tbx_ensure_visible(b);
			return 1;
		case ZST_IN_POINTER_UP:
			if (!b->selecting) return 0;
			b->selecting = 0;
			if (e->root) zrn_release_capture(e->root, e);
			return 1;
		case ZST_IN_CAPTURE_LOST:
			b->selecting = 0;
			return 0;
		case ZST_IN_FOCUS_IN:
			b->caret_blink = 0;
			e->wants_tick = 1;
			zrn_changed_paint(e);
			return 0;
		case ZST_IN_FOCUS_OUT:
			e->wants_tick = 0;
			zrn_changed_paint(e);
			return 0;
		case ZST_IN_KEY_DOWN:
			if (b->key_hook && b->key_hook(b, in)) return 1;
			return tbx_key(e, in);
		case ZST_IN_TEXT:
			if (b->read_only || !in->text) return 1;
			{
				const Zid id = e->id;
				tbx_replace(b, sel_min(b), sel_max(b), in->text, 1);
				if (zrn_lookup(id)) tbx_ensure_visible(b);
			}
			return 1;
		default:
			return 0;
	}
}

static void tbx_tick(zrn_element_t* const e, const float dt) {
	const auto b = TB(e);
	const float before = b->caret_blink;
	b->caret_blink += dt;
	if ((int)(before / BLINK_MS) != (int)(b->caret_blink / BLINK_MS)) zrn_changed_paint(e);
}


/* ============================================================================
 *  layout and paint
 * ============================================================================ */

static ZSize tbx_measure(zrn_element_t* const e, const ZSize available) {
	const zrn_text_box_t* const b = TB(e);
	const ZThickness in = tbx_insets(e);
	const float header = tbx_header_height(b);
	const float inner_w = available.width >= Z_INFINITE * 0.5f ? 0 : zrn_maxf(0, available.width - zrn_thickness_h(in));
	int length, heap;
	const char* disp = tbx_display(b, &length, &heap);
	zrn_text_layout_t* l = tbx_layout(b, disp, length, inner_w);
	float w = 0, h = zrn_font_line_height(tbx_font(b));
	if (l) {
		w = l->width;
		h = zrn_maxf(h, l->height);
		zrn_text_layout_free(l);
	}
	tbx_display_release(disp, heap);
	/* the ScrollViewer of the box has MinHeight 34 */
	return zrn_size(w + zrn_thickness_h(in), header + zrn_maxf(TBX_MIN_HEIGHT, h + zrn_thickness_v(in) - header));
}

static void tbx_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const auto b = TB(e);
	zrn_control_t* const c = &b->base;
	const int focused = e->root && e->root->focus == e, enabled = zrn_effectively_enabled(e);
	const float header = tbx_header_height(b);
	const ZRect box = zrn_rect(0, header, e->rect.width, e->rect.height - header);
	ZColor background = zrn_ctl_background(c, zrn_theme_color(e, ZST_TC_TEXT_BOX_BACKGROUND_81));
	ZColor border = zrn_ctl_border_brush(c, zrn_theme_color(e, ZST_TC_TEXT_BOX_BORDER_81));
	ZColor fg = zrn_ctl_color(c, ZST_C_FOREGROUND, c->foreground, zrn_theme_color(e, ZST_TC_TEXT_BOX_FOREGROUND_81));
	ZColor placeholder = zrn_theme_color(e, ZST_TC_TEXT_BOX_PLACEHOLDER_81);
	ZColor header_color = zrn_theme_color(e, ZST_TC_BASE_MID);
	const ZRect area = tbx_content(e);
	int length, heap, i, sel_a = sel_min(b), sel_b = sel_max(b);
	const char* disp;
	zrn_text_layout_t* l;
	if (!enabled) {
		background = Zcolor_rgba(0, 0, 0, 0);
		border = zrn_theme_color(e, ZST_TC_BASE_LOW);
		fg = placeholder = zrn_theme_color(e, ZST_TC_BASE_LOW_SOLID);
		header_color = zrn_theme_color(e, ZST_TC_BASE_LOW);
	} else if (focused) {
		background = zrn_theme_color(e, ZST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81);
		border = zrn_theme_color(e, ZST_TC_ACCENT);
	} else {
		/* the Normal state of the template: the border element is 0.8 opaque */
		background = zrn_color_alpha(background, 0.8f);
		border = zrn_color_alpha(border, 0.8f);
	}
	if (header > 0) {
		zrn_draw_string(p, tbx_header_font(), b->header, (int)strlen(b->header), 0, 0, header_color);
	}
	if (background.a > 0) {
		/* inside the outline only: two translucent layers would show a lighter rim */
		const ZThickness t = c->border_thickness;
		zrn_fill_rect(p, zrn_rect(box.x + t.left, box.y + t.top, box.width - zrn_thickness_h(t), box.height - zrn_thickness_v(t)), background);
	}
	zrn_stroke_rect(p, box, c->border_thickness, border);
	zrn_push_clip(p, area);
	disp = tbx_display(b, &length, &heap);
	if (length == 0 && b->placeholder && *b->placeholder && !focused) {
		const zrn_font_t font = tbx_font(b);
		zrn_draw_string(p, font, b->placeholder, (int)strlen(b->placeholder), area.x, area.y, placeholder);
	}
	l = tbx_layout(b, disp, length, area.width);
	if (l) {
		const ZColor selection_fg = zrn_theme_color(e, ZST_TC_TEXT_OVER_ACCENT);
		const ZColor accent = zrn_theme_color(e, ZST_TC_ACCENT);
		const int da = real_to_display(b, sel_a), db = real_to_display(b, sel_b);
		for (i = 0; i < l->line_count; i++) {
			const zrn_line_t* ln = &l->lines[i];
			const float x = area.x - b->scroll_x, y = area.y - b->scroll_y + (float)i * l->line_height;
			const int s = da > ln->start ? da : ln->start, t = db < ln->end ? db : ln->end;
			if (focused && enabled && sel_a != sel_b && s < t) {
				/* before, selected, after */
				const float x0 = x + zrn_text_width(l->font, disp + ln->start, s - ln->start);
				const float x1 = x0 + zrn_text_width(l->font, disp + s, t - s);
				zrn_fill_rect(p, zrn_rect(x0, y, x1 - x0, l->line_height), accent);
				zrn_draw_string(p, l->font, disp + ln->start, s - ln->start, x, y, fg);
				zrn_draw_string(p, l->font, disp + s, t - s, x0, y, selection_fg);
				zrn_draw_string(p, l->font, disp + t, ln->end - t, x1, y, fg);
			} else {
				zrn_draw_string(p, l->font, disp + ln->start, ln->end - ln->start, x, y, fg);
			}
		}
		if (focused && enabled && !b->read_only && ((int)(b->caret_blink / BLINK_MS) % 2) == 0) {
			int line;
			float cx;
			caret_position(l, real_to_display(b, b->caret), &line, &cx);
			zrn_fill_rect(p, zrn_rect(area.x - b->scroll_x + cx, area.y - b->scroll_y + (float)line * l->line_height, 1.5f, l->line_height), fg);
		}
		zrn_text_layout_free(l);
	}
	tbx_display_release(disp, heap);
	zrn_pop_clip(p);
}

static void tbx_init(zrn_element_t* const e) {
	const auto b = TB(e);
	zrn_control_t* const c = &b->base;
	/* the style of TextBox: TextControlBorderThemeThickness, TextControlThemePadding, ContentControlFontSize, TextControlMarginThemeThickness */
	c->border_thickness = Zthickness_uniform(ZST_81_BORDER_THICKNESS);
	c->padding = Zthickness(7, 0.8f, 8, 0);
	c->font_size = ZST_81_FONT_CONTENT;
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
	e->min_width = TBX_MIN_WIDTH;
	e->margin = Zthickness(0, ZST_81_TOUCH_OVERHANG, 0, ZST_81_TOUCH_OVERHANG);
	e->valign = Z_VALIGN_TOP;
	b->text = zrn_strdup("");
	b->wrapping = Z_NO_WRAP;
}

static void tbx_destroy(zrn_element_t* const e) {
	free(TB(e)->text);
	free(TB(e)->placeholder);
	free(TB(e)->header);
}

zrn_class_t zrn_class_text_box = {
	.base = &zrn_class_control, .name = "TextBox", .id = Z_CLASS_TEXT_BOX, .size = sizeof(zrn_text_box_t),
	.init = tbx_init, .destroy = tbx_destroy, .measure = tbx_measure, .paint = tbx_paint, .input = tbx_input,
	.tick = tbx_tick, .chrome = tbx_chrome
};

static void password_init(zrn_element_t* const e) {
	const auto b = TB(e);
	b->password = 1;
	b->password_char = 0x25CF;
}

zrn_class_t zrn_class_password_box = {
	.base = &zrn_class_text_box, .name = "PasswordBox", .id = Z_CLASS_PASSWORD_BOX, .size = sizeof(zrn_text_box_t),
	.init = password_init
};

Zid Ztext_box_create(const char* const text) {
	zrn_element_t* const e = zrn_create(Z_CLASS_TEXT_BOX);
	if (!e) return Z_INVALID_ID;
	if (text && *text) {
		TB(e)->quiet = 1;
		tbx_set_text(TB(e), text);
		TB(e)->quiet = 0;
	}
	return e->id;
}

Zid Zpassword_box_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_PASSWORD_BOX);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_text_box_t* get_tb(const Zid id) { return TB(zrn_get(id, Z_CLASS_TEXT_BOX)); }

void Ztext_box_set_text(const Zid text_box, const char* const text) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (b) tbx_set_text(b, text);
}

const char* Ztext_box_get_text(const Zid text_box) {
	const zrn_text_box_t* const b = get_tb(text_box);
	return b && b->text ? b->text : "";
}

void Ztext_box_set_accepts_return(const Zid text_box, const int accepts_return) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	b->accepts_return = accepts_return != 0;
	zrn_changed_layout(&b->base.base);
}

void Ztext_box_set_text_wrapping(const Zid text_box, const ZTextWrapping wrapping) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	b->wrapping = wrapping;
	zrn_changed_layout(&b->base.base);
}

void Ztext_box_set_is_read_only(const Zid text_box, const int is_read_only) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	b->read_only = is_read_only != 0;
	zrn_changed_paint(&b->base.base);
}

void Ztext_box_set_max_length(const Zid text_box, const int max_length) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (b) b->max_length = max_length < 0 ? 0 : max_length;
}

void Ztext_box_select(Zid text_box, int start, int length) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	if (start < 0) start = 0;
	if (start > b->length_bytes) start = b->length_bytes;
	if (length < 0) length = 0;
	if (start + length > b->length_bytes) length = b->length_bytes - start;
	b->anchor = start;
	b->caret = start + length;
	zrn_changed_paint(&b->base.base);
}

void Ztext_box_select_all(const Zid text_box) {
	const zrn_text_box_t* const b = get_tb(text_box);
	if (b) Ztext_box_select(text_box, 0, b->length_bytes);
}

int Ztext_box_get_selection_start(Zid text_box) { const zrn_text_box_t* b = get_tb(text_box); return b ? sel_min(b) : 0; }
int Ztext_box_get_selection_length(Zid text_box) { const zrn_text_box_t* b = get_tb(text_box); return b ? sel_max(b) - sel_min(b) : 0; }

void Ztext_box_set_placeholder_text(const Zid text_box, const char* const placeholder_text) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	zrn_set_string(&b->placeholder, placeholder_text && *placeholder_text ? placeholder_text : nullptr);
	zrn_changed_paint(&b->base.base);
}

const char* Ztext_box_get_placeholder_text(const Zid text_box) {
	const zrn_text_box_t* const b = get_tb(text_box);
	return b && b->placeholder ? b->placeholder : "";
}

void Ztext_box_set_header(const Zid text_box, const char* const header) {
	zrn_text_box_t* const b = get_tb(text_box);
	if (!b) return;
	zrn_set_string(&b->header, header && *header ? header : nullptr);
	zrn_changed_layout(&b->base.base);
}

const char* Ztext_box_get_header(const Zid text_box) {
	const zrn_text_box_t* const b = get_tb(text_box);
	return b && b->header ? b->header : "";
}

int Ztext_box_get_is_read_only(Zid id) { const zrn_text_box_t* b = get_tb(id); return b ? b->read_only : 0; }
int Ztext_box_get_max_length(Zid text_box) { const zrn_text_box_t* b = get_tb(text_box); return b ? b->max_length : 0; }

void Zpassword_box_set_password(const Zid text_box, const char* const password) {
	zrn_element_t* const e = zrn_get(text_box, Z_CLASS_PASSWORD_BOX);
	if (e) tbx_set_text(TB(e), password);
}

const char* Zpassword_box_get_password(const Zid password_box) {
	zrn_element_t* const e = zrn_get(password_box, Z_CLASS_PASSWORD_BOX);
	return e && TB(e)->text ? TB(e)->text : "";
}

void Zpassword_box_set_header(const Zid password_box, const char* const header) {
	if (zrn_get(password_box, Z_CLASS_PASSWORD_BOX)) Ztext_box_set_header(password_box, header);
}

void Zpassword_box_set_placeholder_text(const Zid password_box, const char* const placeholder_text) {
	if (zrn_get(password_box, Z_CLASS_PASSWORD_BOX)) Ztext_box_set_placeholder_text(password_box, placeholder_text);
}

void Zpassword_box_set_password_char(const Zid password_box, const uint32_t codepoint) {
	zrn_element_t* const e = zrn_get(password_box, Z_CLASS_PASSWORD_BOX);
	if (!e) return;
	TB(e)->password_char = codepoint;
	zrn_changed_layout(e);
}
