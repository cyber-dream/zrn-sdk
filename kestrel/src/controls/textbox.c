/**
 * Kestrel - controls/textbox.c
 * TextBox and PasswordBox: editing, selection, caret, clipboard, and the
 * Windows Phone 8.1 look (generic.xaml): a header over a 2.5 px bordered box,
 * 34 px high at least, light at 0.8 opacity that turns white with an accent
 * border on focus; PlaceholderText shows in it while it is empty and has no focus.
 */

#include "../internal.h"

#define TB(e) KST_AS(kst_text_box_t, e)

/* TextControlThemeMinHeight / MinWidth, TextControlHeaderMarginThemeThickness (the 4.8 under the header) */
#define TBX_MIN_HEIGHT 34.0f
#define TBX_MIN_WIDTH 51.0f
#define TBX_HEADER_GAP 4.8f
#define BLINK_MS 530.0f

static char* g_clipboard;


/* ============================================================================
 *  geometry
 * ============================================================================ */

static kst_font_t tbx_font(const kst_text_box_t* b) {
	kst_font_t f;
	f.family = b->base.font_family;
	f.size = b->base.font_size;
	f.spacing = 0;
	return f;
}

static uint32_t bullet_char(const kst_text_box_t* b);

static kst_font_t tbx_header_font(void) { kst_font_t f = { K_FONT_NORMAL, KST_81_FONT_CONTROL_HEADER, 0 }; return f; }

/* The header takes a row of its own above the box. */
static float tbx_header_height(const kst_text_box_t* b) {
	return b->header && *b->header ? kst_font_line_height(tbx_header_font()) + TBX_HEADER_GAP : 0;
}

static KThickness tbx_chrome(kst_element_t* e) {
	return Kthickness(0, tbx_header_height(TB(e)), 0, 0);
}

static KThickness tbx_insets(const kst_element_t* e) {
	const kst_control_t* c = KST_AS(const kst_control_t, e);
	KThickness ch = tbx_chrome((kst_element_t*)e), t;
	t.left = ch.left + c->border_thickness.left + c->padding.left;
	t.top = ch.top + c->border_thickness.top + c->padding.top;
	t.right = ch.right + c->border_thickness.right + c->padding.right;
	t.bottom = ch.bottom + c->border_thickness.bottom + c->padding.bottom;
	return t;
}

/* The area of the text, in local coordinates. */
static KRect tbx_content(const kst_element_t* e) {
	KThickness in = tbx_insets(e);
	return kst_rect(in.left, in.top, kst_maxf(0, e->rect.width - kst_thickness_h(in)), kst_maxf(0, e->rect.height - kst_thickness_v(in)));
}

/* What is shown: the text, or one bullet per character for a password. The caller frees when *heap. */
static const char* tbx_display(const kst_text_box_t* b, int* length, int* heap) {
	*heap = 0;
	if (!b->password) {
		*length = b->length_bytes;
		return b->text;
	}
	{
		char bullet[4];
		int bl = kst_utf8_encode(bullet_char(b), bullet), count = 0, i = 0;
		char* out;
		while (i < b->length_bytes) {
			i = kst_utf8_next(b->text, b->length_bytes, i);
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

/* The bullet of the box, or U+2022 when no font has it. */
static uint32_t bullet_char(const kst_text_box_t* b) {
	uint32_t cp = b->password_char ? b->password_char : 0x25CF;
	return kst_font_has_glyph(tbx_font(b), cp) ? cp : 0x2022;
}

static int bullet_length(const kst_text_box_t* b) {
	char bullet[4];
	return kst_utf8_encode(bullet_char(b), bullet);
}

static int real_to_display(const kst_text_box_t* b, int index) {
	int count = 0, i = 0;
	if (!b->password) return index;
	while (i < index) {
		i = kst_utf8_next(b->text, b->length_bytes, i);
		count++;
	}
	return count * bullet_length(b);
}

static int display_to_real(const kst_text_box_t* b, int index) {
	int count, i = 0, k = 0;
	if (!b->password) return index;
	count = index / bullet_length(b);
	while (k < count && i < b->length_bytes) {
		i = kst_utf8_next(b->text, b->length_bytes, i);
		k++;
	}
	return i;
}

static int tbx_wrap(const kst_text_box_t* b) { return b->wrapping == K_WRAP; }

static kst_text_layout_t* tbx_layout(const kst_text_box_t* b, const char* disp, int length, float width) {
	int wrap = tbx_wrap(b) && width > 0;
	return kst_text_layout_new(tbx_font(b), disp, length, wrap ? width : 0, wrap, 0, 0);
}

/* The line and the x of a display index. */
static void caret_position(const kst_text_layout_t* l, int index, int* line, float* x) {
	int i;
	for (i = 0; i < l->line_count; i++) {
		const kst_line_t* ln = &l->lines[i];
		int next_start = i + 1 < l->line_count ? l->lines[i + 1].start : 0x7fffffff;
		if (index <= ln->end || index < next_start || i == l->line_count - 1) {
			int upto = index < ln->start ? ln->start : (index > ln->end ? ln->end : index);
			*line = i;
			*x = kst_text_width(l->font, l->text + ln->start, upto - ln->start);
			return;
		}
	}
	*line = 0;
	*x = 0;
}

static int index_at(const kst_text_layout_t* l, float x, float y) {
	int line = l->line_height > 0 ? (int)floorf(y / l->line_height) : 0;
	const kst_line_t* ln;
	if (line < 0) line = 0;
	if (line >= l->line_count) line = l->line_count - 1;
	ln = &l->lines[line];
	return ln->start + kst_text_index_at(l->font, l->text + ln->start, ln->end - ln->start, x);
}


/* ============================================================================
 *  editing
 * ============================================================================ */

static int sel_min(const kst_text_box_t* b) { return b->caret < b->anchor ? b->caret : b->anchor; }
static int sel_max(const kst_text_box_t* b) { return b->caret < b->anchor ? b->anchor : b->caret; }

static int count_chars(const char* s, int length) {
	int n = 0, i = 0;
	while (i < length) {
		i = kst_utf8_next(s, length, i);
		n++;
	}
	return n;
}

/* @return K_FALSE when a handler destroyed the box. */
static int tbx_changed(kst_text_box_t* b, int from_user) {
	kst_element_t* e = &b->base.base;
	b->caret_blink = 0;
	kst_changed_layout(e);
	if (b->quiet) return 1;
	b->from_user = from_user;
	if (!kst_raise(e, K_EVENT_TEXT_CHANGED)) return 0;
	b->from_user = 0;
	return 1;
}

/* Replaces [start, end) with `insert`. Newlines are dropped unless the box accepts them. */
static void tbx_replace(kst_text_box_t* b, int start, int end, const char* insert, int from_user) {
	int ilen = insert ? (int)strlen(insert) : 0, i, kept = 0;
	char* filtered = NULL;
	char* text;
	if (start < 0) start = 0;
	if (end > b->length_bytes) end = b->length_bytes;
	if (start > end) start = end;
	if (ilen) {
		filtered = (char*)malloc((size_t)ilen + 1);
		if (!filtered) return;
		for (i = 0; i < ilen; i++) {
			char ch = insert[i];
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
				k = kst_utf8_next(filtered, kept, k);
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

static void tbx_delete_selection(kst_text_box_t* b, int from_user) {
	if (b->caret != b->anchor) tbx_replace(b, sel_min(b), sel_max(b), "", from_user);
}

static void tbx_move(kst_text_box_t* b, int index, int extend) {
	if (index < 0) index = 0;
	if (index > b->length_bytes) index = b->length_bytes;
	b->caret = index;
	if (!extend) b->anchor = index;
	b->caret_blink = 0;
	kst_changed_paint(&b->base.base);
}

/* Keeps the caret in view by moving the scroll offsets. */
static void tbx_ensure_visible(kst_text_box_t* b) {
	kst_element_t* e = &b->base.base;
	KRect area = tbx_content(e);
	int length, heap, line;
	float x;
	const char* disp = tbx_display(b, &length, &heap);
	kst_text_layout_t* l = tbx_layout(b, disp, length, area.width);
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
			float top = (float)line * l->line_height, bottom = top + l->line_height;
			if (bottom - b->scroll_y > area.height) b->scroll_y = bottom - area.height;
			if (top - b->scroll_y < 0) b->scroll_y = top;
			if (l->height <= area.height) b->scroll_y = 0;
		}
		kst_text_layout_free(l);
	}
	if (heap) free((char*)disp);
}

static void tbx_set_text(kst_text_box_t* b, const char* text) {
	if (!text) text = "";
	if (b->text && strcmp(b->text, text) == 0) return;
	kst_set_string(&b->text, text);
	b->length_bytes = (int)strlen(b->text);
	b->caret = b->anchor = b->length_bytes;
	b->scroll_x = b->scroll_y = 0;
	tbx_changed(b, 0);
}


/* ============================================================================
 *  input
 * ============================================================================ */

static int word_left(const kst_text_box_t* b, int index) {
	while (index > 0 && b->text[index - 1] == ' ') index--;
	while (index > 0 && b->text[index - 1] != ' ' && b->text[index - 1] != '\n') index--;
	return index;
}

static int word_right(const kst_text_box_t* b, int index) {
	while (index < b->length_bytes && b->text[index] != ' ' && b->text[index] != '\n') index++;
	while (index < b->length_bytes && b->text[index] == ' ') index++;
	return index;
}

static void tbx_vertical_move(kst_text_box_t* b, int dir, int extend) {
	kst_element_t* e = &b->base.base;
	KRect area = tbx_content(e);
	int length, heap, line;
	float x;
	const char* disp = tbx_display(b, &length, &heap);
	kst_text_layout_t* l = tbx_layout(b, disp, length, area.width);
	if (l) {
		caret_position(l, real_to_display(b, b->caret), &line, &x);
		line += dir;
		if (line < 0) tbx_move(b, 0, extend);
		else if (line >= l->line_count) tbx_move(b, b->length_bytes, extend);
		else tbx_move(b, display_to_real(b, index_at(l, x, (float)line * l->line_height + 1)), extend);
		kst_text_layout_free(l);
	}
	if (heap) free((char*)disp);
}

static int tbx_key(kst_element_t* e, kst_input_t* in) {
	kst_text_box_t* b = TB(e);
	Kid id = e->id;
	int shift = (in->mods & K_MOD_SHIFT) != 0, ctrl = (in->mods & K_MOD_CTRL) != 0;
	switch (in->key) {
		case K_KEY_LEFT:
			if (b->caret != b->anchor && !shift) tbx_move(b, sel_min(b), 0);
			else tbx_move(b, ctrl ? word_left(b, b->caret) : kst_utf8_prev(b->text, b->caret), shift);
			break;
		case K_KEY_RIGHT:
			if (b->caret != b->anchor && !shift) tbx_move(b, sel_max(b), 0);
			else tbx_move(b, ctrl ? word_right(b, b->caret) : kst_utf8_next(b->text, b->length_bytes, b->caret), shift);
			break;
		case K_KEY_UP:
			if (!b->accepts_return && !tbx_wrap(b)) return 0;
			tbx_vertical_move(b, -1, shift);
			break;
		case K_KEY_DOWN:
			if (!b->accepts_return && !tbx_wrap(b)) return 0;
			tbx_vertical_move(b, 1, shift);
			break;
		case K_KEY_HOME:
			if (ctrl || !b->accepts_return) tbx_move(b, 0, shift);
			else {
				int i = b->caret;
				while (i > 0 && b->text[i - 1] != '\n') i--;
				tbx_move(b, i, shift);
			}
			break;
		case K_KEY_END:
			if (ctrl || !b->accepts_return) tbx_move(b, b->length_bytes, shift);
			else {
				int i = b->caret;
				while (i < b->length_bytes && b->text[i] != '\n') i++;
				tbx_move(b, i, shift);
			}
			break;
		case K_KEY_BACKSPACE:
			if (b->read_only) return 1;
			if (b->caret != b->anchor) tbx_delete_selection(b, 1);
			else if (b->caret > 0) tbx_replace(b, kst_utf8_prev(b->text, b->caret), b->caret, "", 1);
			break;
		case K_KEY_DELETE:
			if (b->read_only) return 1;
			if (b->caret != b->anchor) tbx_delete_selection(b, 1);
			else if (b->caret < b->length_bytes) tbx_replace(b, b->caret, kst_utf8_next(b->text, b->length_bytes, b->caret), "", 1);
			break;
		case K_KEY_ENTER:
			if (!b->accepts_return) return 0;
			if (!b->read_only) tbx_replace(b, sel_min(b), sel_max(b), "\n", 1);
			break;
		case K_KEY_A:
			if (!ctrl) return 0;
			b->anchor = 0;
			b->caret = b->length_bytes;
			break;
		case K_KEY_C:
		case K_KEY_X:
			if (!ctrl) return 0;
			if (b->caret != b->anchor && !b->password) {
				int s = sel_min(b), n = sel_max(b) - s;
				free(g_clipboard);
				g_clipboard = (char*)malloc((size_t)n + 1);
				if (g_clipboard) {
					memcpy(g_clipboard, b->text + s, (size_t)n);
					g_clipboard[n] = 0;
				}
				if (in->key == K_KEY_X && !b->read_only) tbx_delete_selection(b, 1);
			}
			break;
		case K_KEY_V:
			if (!ctrl) return 0;
			if (g_clipboard && !b->read_only) tbx_replace(b, sel_min(b), sel_max(b), g_clipboard, 1);
			break;
		default:
			return 0;
	}
	/* a TextChanged handler may have destroyed the box */
	if (!kst_lookup(id)) return 1;
	tbx_ensure_visible(b);
	kst_changed_paint(e);
	return 1;
}

static int tbx_index_from_point(kst_text_box_t* b, float x, float y) {
	kst_element_t* e = &b->base.base;
	KRect area = tbx_content(e);
	int length, heap, index = 0;
	const char* disp = tbx_display(b, &length, &heap);
	kst_text_layout_t* l = tbx_layout(b, disp, length, area.width);
	if (l) {
		index = display_to_real(b, index_at(l, x - area.x + b->scroll_x, y - area.y + b->scroll_y));
		kst_text_layout_free(l);
	}
	if (heap) free((char*)disp);
	return index;
}

static int tbx_input(kst_element_t* e, kst_input_t* in) {
	kst_text_box_t* b = TB(e);
	switch (in->type) {
		case KST_IN_POINTER_DOWN:
			if (!kst_effectively_enabled(e)) return 1;
			tbx_move(b, tbx_index_from_point(b, in->x, in->y), 0);
			b->selecting = 1;
			if (e->root) kst_set_capture(e->root, e);
			return 1;
		case KST_IN_POINTER_MOVE:
			if (!b->selecting) return 0;
			tbx_move(b, tbx_index_from_point(b, in->x, in->y), 1);
			tbx_ensure_visible(b);
			return 1;
		case KST_IN_POINTER_UP:
			if (!b->selecting) return 0;
			b->selecting = 0;
			if (e->root) kst_release_capture(e->root, e);
			return 1;
		case KST_IN_CAPTURE_LOST:
			b->selecting = 0;
			return 0;
		case KST_IN_FOCUS_IN:
			b->caret_blink = 0;
			e->wants_tick = 1;
			kst_changed_paint(e);
			return 0;
		case KST_IN_FOCUS_OUT:
			e->wants_tick = 0;
			kst_changed_paint(e);
			return 0;
		case KST_IN_KEY_DOWN:
			if (b->key_hook && b->key_hook(b, in)) return 1;
			return tbx_key(e, in);
		case KST_IN_TEXT:
			if (b->read_only || !in->text) return 1;
			{
				Kid id = e->id;
				tbx_replace(b, sel_min(b), sel_max(b), in->text, 1);
				if (kst_lookup(id)) tbx_ensure_visible(b);
			}
			return 1;
		default:
			return 0;
	}
}

static void tbx_tick(kst_element_t* e, float dt) {
	kst_text_box_t* b = TB(e);
	float before = b->caret_blink;
	b->caret_blink += dt;
	if ((int)(before / BLINK_MS) != (int)(b->caret_blink / BLINK_MS)) kst_changed_paint(e);
}


/* ============================================================================
 *  layout and paint
 * ============================================================================ */

static KSize tbx_measure(kst_element_t* e, KSize available) {
	kst_text_box_t* b = TB(e);
	KThickness in = tbx_insets(e);
	float header = tbx_header_height(b);
	float inner_w = available.width >= K_INFINITE * 0.5f ? 0 : kst_maxf(0, available.width - kst_thickness_h(in));
	int length, heap;
	const char* disp = tbx_display(b, &length, &heap);
	kst_text_layout_t* l = tbx_layout(b, disp, length, inner_w);
	float w = 0, h = kst_font_line_height(tbx_font(b));
	if (l) {
		w = l->width;
		h = kst_maxf(h, l->height);
		kst_text_layout_free(l);
	}
	if (heap) free((char*)disp);
	/* the ScrollViewer of the box has MinHeight 34 */
	return kst_size(w + kst_thickness_h(in), header + kst_maxf(TBX_MIN_HEIGHT, h + kst_thickness_v(in) - header));
}

static void tbx_paint(kst_element_t* e, kst_painter_t* p) {
	kst_text_box_t* b = TB(e);
	kst_control_t* c = &b->base;
	int focused = e->root && e->root->focus == e, enabled = kst_effectively_enabled(e);
	float header = tbx_header_height(b);
	KRect box = kst_rect(0, header, e->rect.width, e->rect.height - header);
	KColor background = kst_ctl_background(c, kst_theme_color(e, KST_TC_TEXT_BOX_BACKGROUND_81));
	KColor border = kst_ctl_border_brush(c, kst_theme_color(e, KST_TC_TEXT_BOX_BORDER_81));
	KColor fg = kst_ctl_color(c, KST_C_FOREGROUND, c->foreground, kst_theme_color(e, KST_TC_TEXT_BOX_FOREGROUND_81));
	KColor placeholder = kst_theme_color(e, KST_TC_TEXT_BOX_PLACEHOLDER_81);
	KColor header_color = kst_theme_color(e, KST_TC_BASE_MID);
	KRect area = tbx_content(e);
	int length, heap, i, sel_a = sel_min(b), sel_b = sel_max(b);
	const char* disp;
	kst_text_layout_t* l;
	if (!enabled) {
		background = Kcolor_rgba(0, 0, 0, 0);
		border = kst_theme_color(e, KST_TC_BASE_LOW);
		fg = placeholder = kst_theme_color(e, KST_TC_BASE_LOW_SOLID);
		header_color = kst_theme_color(e, KST_TC_BASE_LOW);
	} else if (focused) {
		background = kst_theme_color(e, KST_TC_TEXT_BOX_FOCUSED_BACKGROUND_81);
		border = kst_theme_color(e, KST_TC_ACCENT);
	} else {
		/* the Normal state of the template: the border element is 0.8 opaque */
		background = kst_color_alpha(background, 0.8f);
		border = kst_color_alpha(border, 0.8f);
	}
	if (header > 0) {
		kst_draw_string(p, tbx_header_font(), b->header, (int)strlen(b->header), 0, 0, header_color);
	}
	if (background.a > 0) {
		/* inside the outline only: two translucent layers would show a lighter rim */
		KThickness t = c->border_thickness;
		kst_fill_rect(p, kst_rect(box.x + t.left, box.y + t.top, box.width - kst_thickness_h(t), box.height - kst_thickness_v(t)), background);
	}
	kst_stroke_rect(p, box, c->border_thickness, border);
	kst_push_clip(p, area);
	disp = tbx_display(b, &length, &heap);
	if (length == 0 && b->placeholder && *b->placeholder && !focused) {
		kst_font_t font = tbx_font(b);
		kst_draw_string(p, font, b->placeholder, (int)strlen(b->placeholder), area.x, area.y, placeholder);
	}
	l = tbx_layout(b, disp, length, area.width);
	if (l) {
		KColor selection_fg = kst_theme_color(e, KST_TC_TEXT_OVER_ACCENT);
		KColor accent = kst_theme_color(e, KST_TC_ACCENT);
		int da = real_to_display(b, sel_a), db = real_to_display(b, sel_b);
		for (i = 0; i < l->line_count; i++) {
			const kst_line_t* ln = &l->lines[i];
			float x = area.x - b->scroll_x, y = area.y - b->scroll_y + (float)i * l->line_height;
			int s = da > ln->start ? da : ln->start, t = db < ln->end ? db : ln->end;
			if (focused && enabled && sel_a != sel_b && s < t) {
				/* before, selected, after */
				float x0 = x + kst_text_width(l->font, disp + ln->start, s - ln->start);
				float x1 = x0 + kst_text_width(l->font, disp + s, t - s);
				kst_fill_rect(p, kst_rect(x0, y, x1 - x0, l->line_height), accent);
				kst_draw_string(p, l->font, disp + ln->start, s - ln->start, x, y, fg);
				kst_draw_string(p, l->font, disp + s, t - s, x0, y, selection_fg);
				kst_draw_string(p, l->font, disp + t, ln->end - t, x1, y, fg);
			} else {
				kst_draw_string(p, l->font, disp + ln->start, ln->end - ln->start, x, y, fg);
			}
		}
		if (focused && enabled && !b->read_only && ((int)(b->caret_blink / BLINK_MS) % 2) == 0) {
			int line;
			float cx;
			caret_position(l, real_to_display(b, b->caret), &line, &cx);
			kst_fill_rect(p, kst_rect(area.x - b->scroll_x + cx, area.y - b->scroll_y + (float)line * l->line_height, 1.5f, l->line_height), fg);
		}
		kst_text_layout_free(l);
	}
	if (heap) free((char*)disp);
	kst_pop_clip(p);
}

static void tbx_init(kst_element_t* e) {
	kst_text_box_t* b = TB(e);
	kst_control_t* c = &b->base;
	/* the style of TextBox: TextControlBorderThemeThickness, TextControlThemePadding, ContentControlFontSize, TextControlMarginThemeThickness */
	c->border_thickness = Kthickness_uniform(KST_81_BORDER_THICKNESS);
	c->padding = Kthickness(7, 0.8f, 8, 0);
	c->font_size = KST_81_FONT_CONTENT;
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
	e->min_width = TBX_MIN_WIDTH;
	e->margin = Kthickness(0, KST_81_TOUCH_OVERHANG, 0, KST_81_TOUCH_OVERHANG);
	e->valign = K_VALIGN_TOP;
	b->text = kst_strdup("");
	b->wrapping = K_NO_WRAP;
}

static void tbx_destroy(kst_element_t* e) {
	free(TB(e)->text);
	free(TB(e)->placeholder);
	free(TB(e)->header);
}

kst_class_t kst_class_text_box = {
	.base = &kst_class_control, .name = "TextBox", .id = K_CLASS_TEXT_BOX, .size = sizeof(kst_text_box_t),
	.init = tbx_init, .destroy = tbx_destroy, .measure = tbx_measure, .paint = tbx_paint, .input = tbx_input,
	.tick = tbx_tick, .chrome = tbx_chrome
};

static void password_init(kst_element_t* e) {
	kst_text_box_t* b = TB(e);
	b->password = 1;
	b->password_char = 0x25CF;
}

kst_class_t kst_class_password_box = {
	.base = &kst_class_text_box, .name = "PasswordBox", .id = K_CLASS_PASSWORD_BOX, .size = sizeof(kst_text_box_t),
	.init = password_init
};

Kid Ktext_box_create(const char* text) {
	kst_element_t* e = kst_create(K_CLASS_TEXT_BOX);
	if (!e) return K_INVALID_ID;
	if (text && *text) {
		TB(e)->quiet = 1;
		tbx_set_text(TB(e), text);
		TB(e)->quiet = 0;
	}
	return e->id;
}

Kid Kpassword_box_create(void) {
	kst_element_t* e = kst_create(K_CLASS_PASSWORD_BOX);
	return e ? e->id : K_INVALID_ID;
}

static kst_text_box_t* get_tb(Kid id) { return TB(kst_get(id, K_CLASS_TEXT_BOX)); }

void Ktext_box_set_text(Kid id, const char* text) {
	kst_text_box_t* b = get_tb(id);
	if (b) tbx_set_text(b, text);
}

const char* Ktext_box_get_text(Kid id) {
	kst_text_box_t* b = get_tb(id);
	return b && b->text ? b->text : "";
}

void Ktext_box_set_accepts_return(Kid id, int accepts_return) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	b->accepts_return = accepts_return != 0;
	kst_changed_layout(&b->base.base);
}

void Ktext_box_set_text_wrapping(Kid id, KTextWrapping wrapping) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	b->wrapping = wrapping;
	kst_changed_layout(&b->base.base);
}

void Ktext_box_set_is_read_only(Kid id, int is_read_only) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	b->read_only = is_read_only != 0;
	kst_changed_paint(&b->base.base);
}

void Ktext_box_set_max_length(Kid id, int max_length) {
	kst_text_box_t* b = get_tb(id);
	if (b) b->max_length = max_length < 0 ? 0 : max_length;
}

void Ktext_box_select(Kid id, int start, int length) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	if (start < 0) start = 0;
	if (start > b->length_bytes) start = b->length_bytes;
	if (length < 0) length = 0;
	if (start + length > b->length_bytes) length = b->length_bytes - start;
	b->anchor = start;
	b->caret = start + length;
	kst_changed_paint(&b->base.base);
}

void Ktext_box_select_all(Kid id) {
	kst_text_box_t* b = get_tb(id);
	if (b) Ktext_box_select(id, 0, b->length_bytes);
}

int Ktext_box_get_selection_start(Kid id) { kst_text_box_t* b = get_tb(id); return b ? sel_min(b) : 0; }
int Ktext_box_get_selection_length(Kid id) { kst_text_box_t* b = get_tb(id); return b ? sel_max(b) - sel_min(b) : 0; }

void Ktext_box_set_placeholder_text(Kid id, const char* placeholder_text) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	kst_set_string(&b->placeholder, placeholder_text && *placeholder_text ? placeholder_text : NULL);
	kst_changed_paint(&b->base.base);
}

const char* Ktext_box_get_placeholder_text(Kid id) {
	kst_text_box_t* b = get_tb(id);
	return b && b->placeholder ? b->placeholder : "";
}

void Ktext_box_set_header(Kid id, const char* header) {
	kst_text_box_t* b = get_tb(id);
	if (!b) return;
	kst_set_string(&b->header, header && *header ? header : NULL);
	kst_changed_layout(&b->base.base);
}

const char* Ktext_box_get_header(Kid id) {
	kst_text_box_t* b = get_tb(id);
	return b && b->header ? b->header : "";
}

int Ktext_box_get_is_read_only(Kid id) { kst_text_box_t* b = get_tb(id); return b ? b->read_only : 0; }
int Ktext_box_get_max_length(Kid id) { kst_text_box_t* b = get_tb(id); return b ? b->max_length : 0; }

void Kpassword_box_set_password(Kid id, const char* password) {
	kst_element_t* e = kst_get(id, K_CLASS_PASSWORD_BOX);
	if (e) tbx_set_text(TB(e), password);
}

const char* Kpassword_box_get_password(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_PASSWORD_BOX);
	return e && TB(e)->text ? TB(e)->text : "";
}

void Kpassword_box_set_header(Kid id, const char* header) {
	if (kst_get(id, K_CLASS_PASSWORD_BOX)) Ktext_box_set_header(id, header);
}

void Kpassword_box_set_placeholder_text(Kid id, const char* placeholder_text) {
	if (kst_get(id, K_CLASS_PASSWORD_BOX)) Ktext_box_set_placeholder_text(id, placeholder_text);
}

void Kpassword_box_set_password_char(Kid id, uint32_t codepoint) {
	kst_element_t* e = kst_get(id, K_CLASS_PASSWORD_BOX);
	if (!e) return;
	TB(e)->password_char = codepoint;
	kst_changed_layout(e);
}

/* used by AutoSuggestBox */
void kst_text_box_set_text_quiet(kst_text_box_t* b, const char* text) {
	b->quiet = 1;
	tbx_set_text(b, text);
	b->quiet = 0;
}
