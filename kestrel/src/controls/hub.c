/**
 * Kestrel - Hub and HubSection, after the Windows Phone 8.1 templates (generic.xaml of the phone SDK).
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
#define HUB_HEADER_FONT 78.0f
#define HUB_HEADER_SPACING (-22)
#define HUB_HEADER_X 15.0f
#define HUB_HEADER_Y 1.0f
#define SECTION_FONT 19.0f
#define SECTION_SPACING (-10)
#define SECTION_HEADER_X (-1.0f)
#define SECTION_HEADER_TOP 5.0f
#define SECTION_HEADER_BOTTOM 31.5f
#define SECTION_MARGIN_LEFT 19.0f
/** This library's: how fast the header of the hub moves against the sections. */
#define HEADER_PARALLAX 0.5f

typedef struct kst_hub {
	kst_control_t base;
	char* header;
	KOrientation orientation;
	kst_element_t* scroll;
	/** The StackPanel in the scroller: the sections are its children. */
	kst_element_t* host;
	/** The width the hub was measured for: what a section is capped by before the first arrange. */
	float view_width;
	/** The sections in view as of the last scroll: the first, and how many. */
	int view_first, view_count;
	KHubSectionHeaderClickCallback header_click;
	void* header_click_data;
} kst_hub_t;

typedef struct kst_hub_section {
	kst_content_control_t base;
	char* header;
	int interactive, pressed;
} kst_hub_section_t;

#define HUB(e) KST_AS(kst_hub_t, e)
#define SECTION(e) KST_AS(kst_hub_section_t, e)

static kst_font_t hub_header_font(void) {
	kst_font_t f = { K_FONT_LIGHT, HUB_HEADER_FONT, HUB_HEADER_SPACING };
	return f;
}

static kst_font_t section_font(void) {
	kst_font_t f = { K_FONT_SEMIBOLD, SECTION_FONT, SECTION_SPACING };
	return f;
}

/** The row of the hub header: the header's line (none without a header) and its margin of 1. */
static float hub_header_height(const kst_hub_t* h) {
	return (h->header && *h->header ? kst_font_line_height(hub_header_font()) : 0) + HUB_HEADER_Y;
}

static kst_hub_t* hub_of(const kst_element_t* e) {
	for (e = e->parent; e; e = e->parent) if (e->cls == &kst_class_hub) return HUB(e);
	return NULL;
}

static kst_scroll_viewer_t* scroll_of(const kst_hub_t* h) { return h->scroll ? KST_AS(kst_scroll_viewer_t, h->scroll) : NULL; }

static int section_count(const kst_hub_t* h) { return h->host ? h->host->child_count : 0; }

static int is_vertical(const kst_hub_t* h) { return h->orientation == K_VERTICAL; }


/* ============================================================================
 *  HubSection
 * ============================================================================ */

/** What the section keeps above its header for the header of the hub. */
static float section_placeholder(const kst_element_t* e) {
	kst_hub_t* h = hub_of(e);
	if (!h) return 0;
	if (is_vertical(h) && kst_child_index(e->parent, e) != 0) return 0;
	return hub_header_height(h);
}

static float section_header_band(void) { return SECTION_HEADER_TOP + kst_font_line_height(section_font()) + SECTION_HEADER_BOTTOM; }

static float section_top(const kst_element_t* e) { return section_placeholder(e) + section_header_band(); }

static KSize section_measure(kst_element_t* e, KSize available) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	kst_hub_section_t* s = SECTION(e);
	kst_hub_t* h = hub_of(e);
	float top = section_top(e), w = available.width, text_w;
	KSize content = { 0, 0 };
	const char* t = s->header ? s->header : "";
	/* never wider than the screen unless Width says so */
	if (h && !is_vertical(h) && h->view_width > 0) w = kst_minf(w, kst_maxf(0, h->view_width - SECTION_MARGIN_LEFT));
	if (cc->content) {
		kst_measure(cc->content, kst_size(w, kst_maxf(0, available.height - top)));
		content = cc->content->desired;
	}
	text_w = kst_text_width(section_font(), t, (int)strlen(t)) + SECTION_HEADER_X;
	return kst_size(kst_maxf(content.width, text_w), top + content.height);
}

static KSize section_arrange(kst_element_t* e, KSize final) {
	kst_content_control_t* cc = KST_AS(kst_content_control_t, e);
	float top = section_top(e);
	if (cc->content) kst_arrange(cc->content, kst_rect(0, top, final.width, kst_maxf(0, final.height - top)));
	return final;
}

static KRect section_header_rect(kst_element_t* e) {
	kst_hub_section_t* s = SECTION(e);
	const char* t = s->header ? s->header : "";
	return kst_rect(SECTION_HEADER_X, section_placeholder(e) + SECTION_HEADER_TOP, kst_text_width(section_font(), t, (int)strlen(t)), kst_font_line_height(section_font()));
}

static void section_paint(kst_element_t* e, kst_painter_t* p) {
	kst_hub_section_t* s = SECTION(e);
	KRect r;
	if (!s->header || !*s->header) return;
	r = section_header_rect(e);
	kst_draw_string(p, section_font(), s->header, (int)strlen(s->header), r.x, r.y, kst_ctl_foreground(&s->base.base));
}

static int point_in(KRect r, float x, float y) { return x >= r.x && x < r.x + r.width && y >= r.y && y < r.y + r.height; }

/* A tap on an interactive header is SectionHeaderClick; the rest of the section does not take the pointer, the scroller does. */
static int section_input(kst_element_t* e, kst_input_t* in) {
	kst_hub_section_t* s = SECTION(e);
	switch (in->type) {
	case KST_IN_POINTER_DOWN:
		if (!s->interactive || !point_in(section_header_rect(e), in->x, in->y)) return 0;
		s->pressed = 1;
		if (e->root) kst_set_capture(e->root, e);
		return 1;
	case KST_IN_POINTER_UP: {
		Kid id = e->id;
		int inside;
		kst_hub_t* h;
		if (!s->pressed) return 0;
		s->pressed = 0;
		if (e->root) kst_release_capture(e->root, e);
		inside = point_in(section_header_rect(e), in->x, in->y);
		h = hub_of(e);
		if (inside && h && h->header_click) {
			Kid hub_id = h->base.base.id;
			KHubSectionHeaderClickCallback cb = h->header_click;
			void* data = h->header_click_data;
			cb(hub_id, id, data);
		}
		return 1;
	}
	case KST_IN_CAPTURE_LOST:
		s->pressed = 0;
		return 0;
	default:
		return 0;
	}
}

static void section_init(kst_element_t* e) {
	kst_control_t* c = KST_AS(kst_control_t, e);
	/* HubSection: Margin 19,0,0,0 (HubSectionMarginThickness); its content stretches */
	e->margin = Kthickness(SECTION_MARGIN_LEFT, 0, 0, 0);
	c->h_content = K_HALIGN_STRETCH;
	c->v_content = K_VALIGN_STRETCH;
	e->focusable = 0;
}

static void section_destroy(kst_element_t* e) {
	free(SECTION(e)->header);
	SECTION(e)->header = NULL;
}

kst_class_t kst_class_hub_section = {
	.base = &kst_class_content_control, .name = "HubSection", .id = K_CLASS_HUB_SECTION, .size = sizeof(kst_hub_section_t),
	.init = section_init, .destroy = section_destroy, .measure = section_measure, .arrange = section_arrange,
	.paint = section_paint, .input = section_input
};


/* ============================================================================
 *  Hub
 * ============================================================================ */

/** The first section in view and how many are: those that show at least half a pixel. */
static void view_range(const kst_hub_t* h, int* first, int* count) {
	kst_scroll_viewer_t* s = scroll_of(h);
	int i, n = section_count(h);
	double offset, size;
	*first = 0;
	*count = 0;
	if (!s || !h->host) return;
	offset = is_vertical(h) ? s->offset_y : s->offset_x;
	size = is_vertical(h) ? s->viewport_h : s->viewport_w;
	for (i = 0; i < n; i++) {
		kst_element_t* sec = h->host->children[i];
		float a = is_vertical(h) ? sec->rect.y - sec->margin.top : sec->rect.x - sec->margin.left;
		float b = is_vertical(h) ? sec->rect.y + sec->rect.height : sec->rect.x + sec->rect.width;
		if (sec->visibility == K_COLLAPSED) continue;
		if (b > offset + 0.5 && a < offset + size - 0.5) {
			if (*count == 0) *first = i;
			*count = i - *first + 1;
		}
	}
}

/* The scroller moved: the sections in view may be others. */
static void hub_scrolled(Kid scroll, KScrollEventType type, double value, void* user_data) {
	kst_element_t* e = kst_lookup((Kid)(uintptr_t)user_data);
	kst_hub_t* h;
	int first, count;
	KST_UNUSED(scroll); KST_UNUSED(type); KST_UNUSED(value);
	if (!e) return;
	h = HUB(e);
	view_range(h, &first, &count);
	if (first == h->view_first && count == h->view_count) {
		kst_changed_paint(e);
		return;
	}
	h->view_first = first;
	h->view_count = count;
	kst_changed_paint(e);
	kst_raise(e, K_EVENT_SECTIONS_IN_VIEW_CHANGED);
}

static KSize hub_measure(kst_element_t* e, KSize available) {
	kst_hub_t* h = HUB(e);
	float w = available.width < K_INFINITE * 0.5f ? available.width : 480.0f;
	float hh = available.height < K_INFINITE * 0.5f ? available.height : 800.0f;
	h->view_width = w;
	if (h->scroll) kst_measure(h->scroll, kst_size(w, hh));
	return kst_size(w, hh);
}

static KSize hub_arrange(kst_element_t* e, KSize final) {
	kst_hub_t* h = HUB(e);
	if (h->scroll) kst_arrange(h->scroll, kst_rect(0, 0, final.width, final.height));
	/* the sections are where they are now: the range is quietly the one of the layout */
	view_range(h, &h->view_first, &h->view_count);
	return final;
}

static void hub_paint(kst_element_t* e, kst_painter_t* p) {
	kst_hub_t* h = HUB(e);
	kst_scroll_viewer_t* s = scroll_of(h);
	float x = HUB_HEADER_X, y = HUB_HEADER_Y;
	if (!h->header || !*h->header || !s) return;
	/* the header goes with the sections at half the speed; vertically it scrolls away with them */
	if (is_vertical(h)) y -= (float)s->offset_y;
	else x -= (float)s->offset_x * HEADER_PARALLAX;
	kst_draw_string(p, hub_header_font(), h->header, (int)strlen(h->header), x, y, kst_ctl_foreground(&h->base));
}

static void hub_apply_orientation(kst_hub_t* h) {
	if (!h->host || !h->scroll) return;
	Kstack_panel_set_orientation(h->host->id, h->orientation);
	if (is_vertical(h)) {
		Kscroll_viewer_set_vertical_scrollbar_visibility(h->scroll->id, K_SCROLLBAR_HIDDEN);
		Kscroll_viewer_set_horizontal_scrollbar_visibility(h->scroll->id, K_SCROLLBAR_DISABLED);
	} else {
		Kscroll_viewer_set_horizontal_scrollbar_visibility(h->scroll->id, K_SCROLLBAR_HIDDEN);
		Kscroll_viewer_set_vertical_scrollbar_visibility(h->scroll->id, K_SCROLLBAR_DISABLED);
	}
}

static void hub_init(kst_element_t* e) {
	kst_hub_t* h = HUB(e);
	kst_element_t* scroll = kst_create(K_CLASS_SCROLL_VIEWER);
	kst_element_t* host = kst_create(K_CLASS_STACK_PANEL);
	e->focusable = 0;
	e->clip_to_bounds = 1;
	h->orientation = K_HORIZONTAL;
	if (!scroll || !host) {
		if (scroll) kst_destroy(scroll);
		if (host) kst_destroy(host);
		return;
	}
	kst_add_child(e, scroll, -1);
	h->scroll = scroll;
	h->host = host;
	Kscroll_viewer_set_content(scroll->id, host->id);
	Kscroll_viewer_on_scroll(scroll->id, hub_scrolled, (void*)(uintptr_t)e->id);
	hub_apply_orientation(h);
}

static void hub_destroy(kst_element_t* e) {
	free(HUB(e)->header);
	HUB(e)->header = NULL;
}

static void hub_child_removed(kst_element_t* e, kst_element_t* child) {
	kst_hub_t* h = HUB(e);
	if (h->scroll == child) {
		h->scroll = NULL;
		h->host = NULL;
	}
}

kst_class_t kst_class_hub = {
	.base = &kst_class_control, .name = "Hub", .id = K_CLASS_HUB, .size = sizeof(kst_hub_t),
	.init = hub_init, .destroy = hub_destroy, .measure = hub_measure, .arrange = hub_arrange, .paint = hub_paint,
	.child_removed = hub_child_removed
};

Kid Khub_create(void) {
	kst_element_t* e = kst_create(K_CLASS_HUB);
	return e ? e->id : K_INVALID_ID;
}

static kst_hub_t* get_hub(Kid id) { return HUB(kst_get(id, K_CLASS_HUB)); }

void Khub_set_header(Kid id, const char* header) {
	kst_hub_t* h = get_hub(id);
	if (!h) return;
	kst_set_string(&h->header, header ? header : "");
	kst_changed_layout(&h->base.base);
}

const char* Khub_get_header(Kid id) {
	kst_hub_t* h = get_hub(id);
	return h && h->header ? h->header : "";
}

void Khub_set_orientation(Kid id, KOrientation orientation) {
	kst_hub_t* h = get_hub(id);
	if (!h || (orientation != K_HORIZONTAL && orientation != K_VERTICAL)) return;
	h->orientation = orientation;
	hub_apply_orientation(h);
	if (h->scroll) {
		Kscroll_viewer_scroll_to_horizontal_offset(h->scroll->id, 0);
		Kscroll_viewer_scroll_to_vertical_offset(h->scroll->id, 0);
	}
	kst_changed_layout(&h->base.base);
}

KOrientation Khub_get_orientation(Kid id) {
	kst_hub_t* h = get_hub(id);
	return h ? h->orientation : K_HORIZONTAL;
}

Kid Khub_get_scroll_viewer(Kid id) {
	kst_hub_t* h = get_hub(id);
	return h && h->scroll ? h->scroll->id : K_INVALID_ID;
}

int Khub_insert_section(Kid id, int index, Kid section) {
	kst_hub_t* h = get_hub(id);
	kst_element_t* s = section != K_INVALID_ID ? kst_get(section, K_CLASS_HUB_SECTION) : NULL;
	if (!h || !s || !h->host) return -1;
	if (s->parent) {
		kst_error(K_ERROR_HAS_PARENT, "the HubSection is in a hub already");
		return -1;
	}
	if (index < 0 || index > h->host->child_count) index = h->host->child_count;
	kst_add_child(h->host, s, index);
	kst_changed_layout(&h->base.base);
	return index;
}

int Khub_add_section(Kid id, Kid section) {
	kst_hub_t* h = get_hub(id);
	return h ? Khub_insert_section(id, section_count(h), section) : -1;
}

void Khub_remove_section_at(Kid id, int index) {
	kst_hub_t* h = get_hub(id);
	if (!h || index < 0 || index >= section_count(h)) return;
	kst_destroy(h->host->children[index]);
	kst_changed_layout(&h->base.base);
}

void Khub_clear_sections(Kid id) {
	kst_hub_t* h = get_hub(id);
	while (h && section_count(h) > 0) kst_destroy(h->host->children[section_count(h) - 1]);
	if (h) kst_changed_layout(&h->base.base);
}

int Khub_get_section_count(Kid id) {
	kst_hub_t* h = get_hub(id);
	return h ? section_count(h) : 0;
}

Kid Khub_get_section(Kid id, int index) {
	kst_hub_t* h = get_hub(id);
	return h && index >= 0 && index < section_count(h) ? h->host->children[index]->id : K_INVALID_ID;
}

void Khub_scroll_to_section(Kid id, int index) {
	kst_hub_t* h = get_hub(id);
	kst_element_t* sec;
	if (!h || index < 0 || index >= section_count(h) || !h->scroll) return;
	/* what the last layout knows: the margin box of the section at the left edge of the view */
	sec = h->host->children[index];
	if (is_vertical(h)) Kscroll_viewer_scroll_to_vertical_offset(h->scroll->id, sec->rect.y - sec->margin.top);
	else Kscroll_viewer_scroll_to_horizontal_offset(h->scroll->id, sec->rect.x - sec->margin.left);
}

int Khub_get_sections_in_view_count(Kid id) {
	kst_hub_t* h = get_hub(id);
	int first, count;
	if (!h) return 0;
	view_range(h, &first, &count);
	return count;
}

Kid Khub_get_section_in_view(Kid id, int index) {
	kst_hub_t* h = get_hub(id);
	int first, count;
	if (!h) return K_INVALID_ID;
	view_range(h, &first, &count);
	return index >= 0 && index < count ? h->host->children[first + index]->id : K_INVALID_ID;
}

void Khub_on_sections_in_view_changed(Kid id, KRoutedCallback callback, void* user_data) {
	if (get_hub(id)) Kelement_on(id, K_EVENT_SECTIONS_IN_VIEW_CHANGED, callback, user_data);
}

void Khub_on_section_header_click(Kid id, KHubSectionHeaderClickCallback callback, void* user_data) {
	kst_hub_t* h = get_hub(id);
	if (!h) return;
	h->header_click = callback;
	h->header_click_data = user_data;
}

Kid Khub_section_create(const char* header) {
	kst_element_t* e = kst_create(K_CLASS_HUB_SECTION);
	if (!e) return K_INVALID_ID;
	kst_set_string(&SECTION(e)->header, header ? header : "");
	return e->id;
}

void Khub_section_set_header(Kid id, const char* header) {
	kst_element_t* e = kst_get(id, K_CLASS_HUB_SECTION);
	if (!e) return;
	kst_set_string(&SECTION(e)->header, header ? header : "");
	kst_changed_layout(e);
}

const char* Khub_section_get_header(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_HUB_SECTION);
	return e && SECTION(e)->header ? SECTION(e)->header : "";
}

void Khub_section_set_is_header_interactive(Kid id, int interactive) {
	kst_element_t* e = kst_get(id, K_CLASS_HUB_SECTION);
	if (e) SECTION(e)->interactive = interactive != 0;
}

int Khub_section_get_is_header_interactive(Kid id) {
	kst_element_t* e = kst_get(id, K_CLASS_HUB_SECTION);
	return e ? SECTION(e)->interactive : 0;
}
