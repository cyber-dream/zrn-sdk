/**
 * zrnui - Hub and HubSection, after the Windows Phone 8.1 templates (generic.xaml of the phone SDK).
 *
 * A Hub is a big header (78 px Light, character spacing -22, margin 15,1) over sections that stand side by
 * side in a horizontal ScrollViewer (no scroll bars, no snap points: a finger flings them freely). A section
 * is a header (19 px SemiBold, spacing -10, margin -1,5,0,31.5) over its content, with the margin 19,0,0,0;
 * each section keeps a placeholder as high as the header of the hub over its own header, so that the hub
 * header is above the first one and the sections under it begin together.
 *
 * Orientation Vertical stacks the sections under one another in a vertical scroller; only the first one has
 * the placeholder, and the header scrolls away with it.
 *
 * What the templates do not say: the width of a section is its content's desired width, never more than the
 * screen less its 19 px margin (set Width to choose); the header of the hub moves at half the speed of the
 * sections (parallax); there is no chevron on an interactive header. The parallax of the background is not made.
 */

#include "../internal.h"

/* themeresources.xaml: HubHeaderFontSize 78, HubHeaderCharacterSpacing -22, HubHeaderMarginThickness 15,1,0,0, HubSectionHeaderFontSize 19,
 * HubSectionHeaderCharacterSpacing -10, HubSectionHeaderMarginThickness -1,5,0,31.5, HubSectionMarginThickness 19,0,0,0 */
static constexpr float HUB_HEADER_X = 15.0f;
static constexpr float HUB_HEADER_Y = 1.0f;
static constexpr float SECTION_FONT = 19.0f;
static constexpr int SECTION_SPACING = (-10);
static constexpr float SECTION_HEADER_X = (-1.0f);
static constexpr float SECTION_HEADER_TOP = 5.0f;
static constexpr float SECTION_HEADER_BOTTOM = 31.5f;
static constexpr float SECTION_MARGIN_LEFT = 19.0f;
/** This library's: how fast the header of the hub moves against the sections. */
static constexpr float HEADER_PARALLAX = 0.5f;

typedef struct zrn_hub {
	zrn_control_t base;
	char* header;
	ZOrientation orientation;
	zrn_element_t* scroll;
	/** The StackPanel in the scroller: the sections are its children. */
	zrn_element_t* host;
	/** The width the hub was measured for: what a section is capped by before the first arrange. */
	float view_width;
	/** The sections in view as of the last scroll: the first, and how many. */
	int view_first, view_count;
	ZHubSectionHeaderClickCallback header_click;
	void* header_click_data;
} zrn_hub_t;

typedef struct zrn_hub_section {
	zrn_content_control_t base;
	char* header;
	int interactive, pressed;
} zrn_hub_section_t;

#define HUB(e) ZST_AS(zrn_hub_t, e)
#define SECTION(e) ZST_AS(zrn_hub_section_t, e)

/** HubHeaderFontSize and HubHeaderCharacterSpacing of the theme resources of the root (an override applies). */
static zrn_font_t hub_header_font(const zrn_hub_t* const h) {
	const zrn_root_t* r = ((const zrn_element_t*)h)->root;
	zrn_font_t f = { Z_FONT_LIGHT, 78.0f, -22 };
	zrn_res_double(r, Z_RES_HUB_HEADER_FONT_SIZE, &f.size);
	zrn_res_int(r, Z_RES_HUB_HEADER_CHARACTER_SPACING, &f.spacing);
	return f;
}

static zrn_font_t section_font(void) {
	const zrn_font_t f = { Z_FONT_SEMIBOLD, SECTION_FONT, SECTION_SPACING };
	return f;
}

/** The row of the hub header: the header's line (none without a header) and its margin of 1. */
static float hub_header_height(const zrn_hub_t* const h) {
	return (h->header && *h->header ? zrn_font_line_height(hub_header_font(h)) : 0) + HUB_HEADER_Y;
}

static zrn_hub_t* hub_of(const zrn_element_t* e) {
	for (e = e->parent; e; e = e->parent) if (e->cls == &zrn_class_hub) return HUB(e);
	return nullptr;
}

static zrn_scroll_viewer_t* scroll_of(const zrn_hub_t* h) { return h->scroll ? ZST_AS(zrn_scroll_viewer_t, h->scroll) : nullptr; }

static int section_count(const zrn_hub_t* h) { return h->host ? h->host->child_count : 0; }

static int is_vertical(const zrn_hub_t* h) { return h->orientation == Z_VERTICAL; }


/* ============================================================================
 *  HubSection
 * ============================================================================ */

/** What the section keeps above its header for the header of the hub. */
static float section_placeholder(const zrn_element_t* const e) {
	zrn_hub_t* const h = hub_of(e);
	if (!h) return 0;
	if (is_vertical(h) && zrn_child_index(e->parent, e) != 0) return 0;
	return hub_header_height(h);
}

static float section_header_band(void) { return SECTION_HEADER_TOP + zrn_font_line_height(section_font()) + SECTION_HEADER_BOTTOM; }

static float section_top(const zrn_element_t* e) { return section_placeholder(e) + section_header_band(); }

static ZSize section_measure(zrn_element_t* const e, const ZSize available) {
	zrn_content_control_t* const cc = ZST_AS(zrn_content_control_t, e);
	zrn_hub_section_t* const s = SECTION(e);
	zrn_hub_t* const h = hub_of(e);
	float top = section_top(e), w = available.width, text_w;
	ZSize content = { 0, 0 };
	const char* t = s->header ? s->header : "";
	/* never wider than the screen unless Width says so */
	if (h && !is_vertical(h) && h->view_width > 0) w = zrn_minf(w, zrn_maxf(0, h->view_width - SECTION_MARGIN_LEFT));
	if (cc->content) {
		zrn_measure(cc->content, zrn_size(w, zrn_maxf(0, available.height - top)));
		content = cc->content->desired;
	}
	text_w = zrn_text_width(section_font(), t, (int)strlen(t)) + SECTION_HEADER_X;
	return zrn_size(zrn_maxf(content.width, text_w), top + content.height);
}

static ZSize section_arrange(zrn_element_t* const e, const ZSize final) {
	const zrn_content_control_t* const cc = ZST_AS(zrn_content_control_t, e);
	const float top = section_top(e);
	if (cc->content) zrn_arrange(cc->content, zrn_rect(0, top, final.width, zrn_maxf(0, final.height - top)));
	return final;
}

static ZRect section_header_rect(zrn_element_t* const e) {
	const zrn_hub_section_t* const s = SECTION(e);
	const char* t = s->header ? s->header : "";
	return zrn_rect(SECTION_HEADER_X, section_placeholder(e) + SECTION_HEADER_TOP, zrn_text_width(section_font(), t, (int)strlen(t)), zrn_font_line_height(section_font()));
}

static void section_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_hub_section_t* const s = SECTION(e);
	ZRect r;
	if (!s->header || !*s->header) return;
	r = section_header_rect(e);
	zrn_draw_string(p, section_font(), s->header, (int)strlen(s->header), r.x, r.y, zrn_ctl_foreground(&s->base.base));
}

static int point_in(const ZRect r, const float x, const float y) { return x >= r.x && x < r.x + r.width && y >= r.y && y < r.y + r.height; }

/* A tap on an interactive header is SectionHeaderClick; the rest of the section does not take the pointer, the scroller does. */
static int section_input(zrn_element_t* const e, zrn_input_t* const in) {
	zrn_hub_section_t* const s = SECTION(e);
	switch (in->type) {
	case ZST_IN_POINTER_DOWN:
		if (!s->interactive || !point_in(section_header_rect(e), in->x, in->y)) return 0;
		s->pressed = 1;
		if (e->root) zrn_set_capture(e->root, e);
		return 1;
	case ZST_IN_POINTER_UP: {
		const Zid id = e->id;
		int inside;
		zrn_hub_t* h;
		if (!s->pressed) return 0;
		s->pressed = 0;
		if (e->root) zrn_release_capture(e->root, e);
		inside = point_in(section_header_rect(e), in->x, in->y);
		h = hub_of(e);
		if (inside && h && h->header_click) {
			const Zid hub_id = h->base.base.id;
			const ZHubSectionHeaderClickCallback cb = h->header_click;
			void* const data = h->header_click_data;
			cb(hub_id, id, data);
		}
		return 1;
	}
	case ZST_IN_CAPTURE_LOST:
		s->pressed = 0;
		return 0;
	default:
		return 0;
	}
}

static void section_init(zrn_element_t* const e) {
	zrn_control_t* const c = ZST_AS(zrn_control_t, e);
	/* HubSection: Margin 19,0,0,0 (HubSectionMarginThickness); its content stretches */
	e->margin = Zthickness(SECTION_MARGIN_LEFT, 0, 0, 0);
	c->h_content = Z_HALIGN_STRETCH;
	c->v_content = Z_VALIGN_STRETCH;
	e->focusable = 0;
}

static void section_destroy(zrn_element_t* const e) {
	free(SECTION(e)->header);
	SECTION(e)->header = nullptr;
}

zrn_class_t zrn_class_hub_section = {
	.base = &zrn_class_content_control, .name = "HubSection", .id = Z_CLASS_HUB_SECTION, .size = sizeof(zrn_hub_section_t),
	.init = section_init, .destroy = section_destroy, .measure = section_measure, .arrange = section_arrange,
	.paint = section_paint, .input = section_input
};


/* ============================================================================
 *  Hub
 * ============================================================================ */

/** The first section in view and how many are: those that show at least half a pixel. */
static void view_range(const zrn_hub_t* const h, int* const first, int* const count) {
	zrn_scroll_viewer_t* const s = scroll_of(h);
	int i, n = section_count(h);
	double offset, size;
	*first = 0;
	*count = 0;
	if (!s || !h->host) return;
	offset = is_vertical(h) ? s->offset_y : s->offset_x;
	size = is_vertical(h) ? s->viewport_h : s->viewport_w;
	for (i = 0; i < n; i++) {
		const zrn_element_t* const sec = h->host->children[i];
		const float a = is_vertical(h) ? sec->rect.y - sec->margin.top : sec->rect.x - sec->margin.left;
		const float b = is_vertical(h) ? sec->rect.y + sec->rect.height : sec->rect.x + sec->rect.width;
		if (sec->visibility == Z_COLLAPSED) continue;
		if (b > offset + 0.5 && a < offset + size - 0.5) {
			if (*count == 0) *first = i;
			*count = i - *first + 1;
		}
	}
}

/* The scroller moved: the sections in view may be others. */
static void hub_scrolled(const Zid scroll, const ZScrollEventType type, const double value, void* const user_data) {
	zrn_element_t* const e = zrn_lookup((Zid)(uintptr_t)user_data);
	zrn_hub_t* h;
	int first, count;
	ZST_UNUSED(scroll); ZST_UNUSED(type); ZST_UNUSED(value);
	if (!e) return;
	h = HUB(e);
	view_range(h, &first, &count);
	if (first == h->view_first && count == h->view_count) {
		zrn_changed_paint(e);
		return;
	}
	h->view_first = first;
	h->view_count = count;
	zrn_changed_paint(e);
	zrn_raise(e, Z_EVENT_SECTIONS_IN_VIEW_CHANGED);
}

static ZSize hub_measure(zrn_element_t* const e, const ZSize available) {
	zrn_hub_t* const h = HUB(e);
	const float w = available.width < Z_INFINITE * 0.5f ? available.width : 480.0f;
	const float hh = available.height < Z_INFINITE * 0.5f ? available.height : 800.0f;
	h->view_width = w;
	if (h->scroll) zrn_measure(h->scroll, zrn_size(w, hh));
	return zrn_size(w, hh);
}

static ZSize hub_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_hub_t* const h = HUB(e);
	if (h->scroll) zrn_arrange(h->scroll, zrn_rect(0, 0, final.width, final.height));
	/* the sections are where they are now: the range is quietly the one of the layout */
	view_range(h, &h->view_first, &h->view_count);
	return final;
}

static void hub_paint(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_hub_t* const h = HUB(e);
	zrn_scroll_viewer_t* const s = scroll_of(h);
	float x = HUB_HEADER_X, y = HUB_HEADER_Y;
	if (!h->header || !*h->header || !s) return;
	/* the header goes with the sections at half the speed; vertically it scrolls away with them */
	if (is_vertical(h)) y -= (float)s->offset_y;
	else x -= (float)s->offset_x * HEADER_PARALLAX;
	zrn_draw_string(p, hub_header_font(h), h->header, (int)strlen(h->header), x, y, zrn_ctl_foreground(&h->base));
}

static void hub_apply_orientation(zrn_hub_t* const h) {
	if (!h->host || !h->scroll) return;
	Zstack_panel_set_orientation(h->host->id, h->orientation);
	if (is_vertical(h)) {
		Zscroll_viewer_set_vertical_scrollbar_visibility(h->scroll->id, Z_SCROLLBAR_HIDDEN);
		Zscroll_viewer_set_horizontal_scrollbar_visibility(h->scroll->id, Z_SCROLLBAR_DISABLED);
	} else {
		Zscroll_viewer_set_horizontal_scrollbar_visibility(h->scroll->id, Z_SCROLLBAR_HIDDEN);
		Zscroll_viewer_set_vertical_scrollbar_visibility(h->scroll->id, Z_SCROLLBAR_DISABLED);
	}
}

static void hub_init(zrn_element_t* const e) {
	zrn_hub_t* const h = HUB(e);
	zrn_element_t* const scroll = zrn_create(Z_CLASS_SCROLL_VIEWER);
	zrn_element_t* const host = zrn_create(Z_CLASS_STACK_PANEL);
	e->focusable = 0;
	e->clip_to_bounds = 1;
	h->orientation = Z_HORIZONTAL;
	if (!scroll || !host) {
		if (scroll) zrn_destroy(scroll);
		if (host) zrn_destroy(host);
		return;
	}
	zrn_add_child(e, scroll, -1);
	h->scroll = scroll;
	h->host = host;
	Zscroll_viewer_set_content(scroll->id, host->id);
	Zscroll_viewer_on_scroll(scroll->id, hub_scrolled, (void*)(uintptr_t)e->id);
	hub_apply_orientation(h);
}

static void hub_destroy(zrn_element_t* const e) {
	free(HUB(e)->header);
	HUB(e)->header = nullptr;
}

static void hub_child_removed(zrn_element_t* const e, zrn_element_t* const child) {
	zrn_hub_t* const h = HUB(e);
	if (h->scroll == child) {
		h->scroll = nullptr;
		h->host = nullptr;
	}
}

zrn_class_t zrn_class_hub = {
	.base = &zrn_class_control, .name = "Hub", .id = Z_CLASS_HUB, .size = sizeof(zrn_hub_t),
	.init = hub_init, .destroy = hub_destroy, .measure = hub_measure, .arrange = hub_arrange, .paint = hub_paint,
	.child_removed = hub_child_removed
};

Zid Zhub_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_HUB);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_hub_t* get_hub(const Zid id) { return HUB(zrn_get(id, Z_CLASS_HUB)); }

void Zhub_set_header(const Zid id, const char* const header) {
	zrn_hub_t* const h = get_hub(id);
	if (!h) return;
	zrn_set_string(&h->header, header ? header : "");
	zrn_changed_layout(&h->base.base);
}

const char* Zhub_get_header(const Zid id) {
	const zrn_hub_t* const h = get_hub(id);
	return h && h->header ? h->header : "";
}

void Zhub_set_orientation(const Zid id, const ZOrientation orientation) {
	zrn_hub_t* const h = get_hub(id);
	if (!h || (orientation != Z_HORIZONTAL && orientation != Z_VERTICAL)) return;
	h->orientation = orientation;
	hub_apply_orientation(h);
	if (h->scroll) {
		Zscroll_viewer_scroll_to_horizontal_offset(h->scroll->id, 0);
		Zscroll_viewer_scroll_to_vertical_offset(h->scroll->id, 0);
	}
	zrn_changed_layout(&h->base.base);
}

ZOrientation Zhub_get_orientation(const Zid id) {
	const zrn_hub_t* const h = get_hub(id);
	return h ? h->orientation : Z_HORIZONTAL;
}

Zid Zhub_get_scroll_viewer(const Zid id) {
	const zrn_hub_t* const h = get_hub(id);
	return h && h->scroll ? h->scroll->id : Z_INVALID_ID;
}

int Zhub_insert_section(Zid id, int index, Zid section) {
	zrn_hub_t* const h = get_hub(id);
	zrn_element_t* const s = section != Z_INVALID_ID ? zrn_get(section, Z_CLASS_HUB_SECTION) : nullptr;
	if (!h || !s || !h->host) return -1;
	if (s->parent) {
		zrn_error(Z_ERROR_HAS_PARENT, "the HubSection is in a hub already");
		return -1;
	}
	if (index < 0 || index > h->host->child_count) index = h->host->child_count;
	zrn_add_child(h->host, s, index);
	zrn_changed_layout(&h->base.base);
	return index;
}

int Zhub_add_section(const Zid id, const Zid section) {
	const zrn_hub_t* const h = get_hub(id);
	return h ? Zhub_insert_section(id, section_count(h), section) : -1;
}

void Zhub_remove_section_at(const Zid id, const int index) {
	zrn_hub_t* const h = get_hub(id);
	if (!h || index < 0 || index >= section_count(h)) return;
	zrn_destroy(h->host->children[index]);
	zrn_changed_layout(&h->base.base);
}

void Zhub_clear_sections(const Zid id) {
	zrn_hub_t* const h = get_hub(id);
	while (h && section_count(h) > 0) zrn_destroy(h->host->children[section_count(h) - 1]);
	if (h) zrn_changed_layout(&h->base.base);
}

int Zhub_get_section_count(const Zid id) {
	const zrn_hub_t* const h = get_hub(id);
	return h ? section_count(h) : 0;
}

Zid Zhub_get_section(const Zid id, const int index) {
	const zrn_hub_t* const h = get_hub(id);
	return h && index >= 0 && index < section_count(h) ? h->host->children[index]->id : Z_INVALID_ID;
}

void Zhub_scroll_to_section(const Zid id, const int index) {
	zrn_hub_t* const h = get_hub(id);
	zrn_element_t* sec;
	if (!h || index < 0 || index >= section_count(h) || !h->scroll) return;
	/* what the last layout knows: the margin box of the section at the left edge of the view */
	sec = h->host->children[index];
	if (is_vertical(h)) Zscroll_viewer_scroll_to_vertical_offset(h->scroll->id, sec->rect.y - sec->margin.top);
	else Zscroll_viewer_scroll_to_horizontal_offset(h->scroll->id, sec->rect.x - sec->margin.left);
}

int Zhub_get_sections_in_view_count(const Zid id) {
	zrn_hub_t* const h = get_hub(id);
	int first, count;
	if (!h) return 0;
	view_range(h, &first, &count);
	return count;
}

Zid Zhub_get_section_in_view(const Zid id, const int index) {
	zrn_hub_t* const h = get_hub(id);
	int first, count;
	if (!h) return Z_INVALID_ID;
	view_range(h, &first, &count);
	return index >= 0 && index < count ? h->host->children[first + index]->id : Z_INVALID_ID;
}

void Zhub_on_sections_in_view_changed(const Zid id, const ZRoutedCallback callback, void* const user_data) {
	if (get_hub(id)) Zelement_on(id, Z_EVENT_SECTIONS_IN_VIEW_CHANGED, callback, user_data);
}

void Zhub_on_section_header_click(const Zid id, const ZHubSectionHeaderClickCallback callback, void* const user_data) {
	zrn_hub_t* const h = get_hub(id);
	if (!h) return;
	h->header_click = callback;
	h->header_click_data = user_data;
}

Zid Zhub_section_create(const char* const header) {
	zrn_element_t* const e = zrn_create(Z_CLASS_HUB_SECTION);
	if (!e) return Z_INVALID_ID;
	zrn_set_string(&SECTION(e)->header, header ? header : "");
	return e->id;
}

void Zhub_section_set_header(const Zid id, const char* const header) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_HUB_SECTION);
	if (!e) return;
	zrn_set_string(&SECTION(e)->header, header ? header : "");
	zrn_changed_layout(e);
}

const char* Zhub_section_get_header(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_HUB_SECTION);
	return e && SECTION(e)->header ? SECTION(e)->header : "";
}

void Zhub_section_set_is_header_interactive(const Zid id, const int interactive) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_HUB_SECTION);
	if (e) SECTION(e)->interactive = interactive != 0;
}

int Zhub_section_get_is_header_interactive(const Zid id) {
	zrn_element_t* const e = zrn_get(id, Z_CLASS_HUB_SECTION);
	return e ? SECTION(e)->interactive : 0;
}
