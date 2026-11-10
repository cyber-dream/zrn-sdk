/**
 * zrnui - controls/scroll.c
 * ScrollViewer: touch scrolling with inertia, the wheel, and the thin
 * transient scroll bars of Windows Phone.
 */

#include "../internal.h"

#define SV(e) ZST_AS(zrn_scroll_viewer_t, e)

/* a drag starts after the finger moved this far */
static constexpr float DRAG_THRESHOLD = 10.0f;
/* the bars stay in view this long after the last scroll */
static constexpr float BAR_HOLD_MS = 800.0f;


static void sv_limits(const zrn_scroll_viewer_t* const s, double* const max_x, double* const max_y) {
	*max_x = s->extent_w > s->viewport_w ? s->extent_w - s->viewport_w : 0;
	*max_y = s->extent_h > s->viewport_h ? s->extent_h - s->viewport_h : 0;
}

static void sv_clamp(zrn_scroll_viewer_t* const s) {
	double mx, my;
	sv_limits(s, &mx, &my);
	if (s->offset_x < 0) s->offset_x = 0;
	if (s->offset_x > mx) s->offset_x = mx;
	if (s->offset_y < 0) s->offset_y = 0;
	if (s->offset_y > my) s->offset_y = my;
}

static void sv_changed(zrn_scroll_viewer_t* const s, const int fire) {
	zrn_element_t* const e = &s->base.base.base;
	sv_clamp(s);
	s->bar_visible = BAR_HOLD_MS;
	e->wants_tick = 1;
	zrn_changed_arrange(e);
	if (fire && s->on_scroll) s->on_scroll(e->id, Z_SCROLL_THUMB_POSITION, s->offset_y, s->on_scroll_data);
}

static ZSize sv_measure(zrn_element_t* const e, const ZSize available) {
	zrn_scroll_viewer_t* s = SV(e);
	const zrn_content_control_t* const cc = &s->base;
	ZSize room = available, content = { 0, 0 };
	if (s->h_visibility != Z_SCROLLBAR_DISABLED) room.width = Z_INFINITE;
	if (s->v_visibility != Z_SCROLLBAR_DISABLED) room.height = Z_INFINITE;
	if (cc->content) {
		zrn_measure(cc->content, room);
		content = cc->content->desired;
	}
	return zrn_size(zrn_minf(content.width, available.width), zrn_minf(content.height, available.height));
}

static ZSize sv_arrange(zrn_element_t* const e, const ZSize final) {
	zrn_scroll_viewer_t* const s = SV(e);
	const zrn_content_control_t* const cc = &s->base;
	s->viewport_w = final.width;
	s->viewport_h = final.height;
	if (cc->content) {
		const ZSize d = cc->content->desired;
		const float w = s->h_visibility == Z_SCROLLBAR_DISABLED ? final.width : zrn_maxf(d.width, final.width);
		const float h = s->v_visibility == Z_SCROLLBAR_DISABLED ? final.height : zrn_maxf(d.height, final.height);
		s->extent_w = s->h_visibility == Z_SCROLLBAR_DISABLED ? final.width : d.width;
		s->extent_h = s->v_visibility == Z_SCROLLBAR_DISABLED ? final.height : d.height;
		sv_clamp(s);
		zrn_arrange(cc->content, zrn_rect((float)-s->offset_x, (float)-s->offset_y, w, h));
	} else {
		s->extent_w = final.width;
		s->extent_h = final.height;
	}
	s->show_v = s->v_visibility != Z_SCROLLBAR_DISABLED && s->v_visibility != Z_SCROLLBAR_HIDDEN && s->extent_h > s->viewport_h + 0.5;
	s->show_h = s->h_visibility != Z_SCROLLBAR_DISABLED && s->h_visibility != Z_SCROLLBAR_HIDDEN && s->extent_w > s->viewport_w + 0.5;
	if (s->v_visibility == Z_SCROLLBAR_VISIBLE) s->show_v = 1;
	if (s->h_visibility == Z_SCROLLBAR_VISIBLE) s->show_h = 1;
	return final;
}

static void sv_paint_over(zrn_element_t* const e, zrn_painter_t* const p) {
	const zrn_scroll_viewer_t* const s = SV(e);
	ZColor color;
	double mx, my;
	float alpha;
	const int always_v = s->v_visibility == Z_SCROLLBAR_VISIBLE, always_h = s->h_visibility == Z_SCROLLBAR_VISIBLE;
	sv_limits(s, &mx, &my);
	alpha = zrn_clampf(s->bar_visible / 250.0f, 0, 1);
	color = zrn_theme_color(e, ZST_TC_DISABLED);
	if (s->show_v && (alpha > 0 || always_v)) {
		const float length = e->rect.height - (s->show_h ? 7.0f : 0);
		const float thumb = zrn_clampf((float)(s->viewport_h / s->extent_h) * length, zrn_minf(72.0f, length), length);
		const float pos = my > 0 ? (float)(s->offset_y / my) * (length - thumb) : 0;
		zrn_fill_rect(p, zrn_rect(e->rect.width - 6, pos + 1, 5, thumb - 2), zrn_color_alpha(color, always_v ? 1.0f : alpha));
	}
	if (s->show_h && (alpha > 0 || always_h)) {
		const float length = e->rect.width - (s->show_v ? 7.0f : 0);
		const float thumb = zrn_clampf((float)(s->viewport_w / s->extent_w) * length, zrn_minf(72.0f, length), length);
		const float pos = mx > 0 ? (float)(s->offset_x / mx) * (length - thumb) : 0;
		zrn_fill_rect(p, zrn_rect(pos + 1, e->rect.height - 6, thumb - 2, 5), zrn_color_alpha(color, always_h ? 1.0f : alpha));
	}
}

static void sv_tick(zrn_element_t* const e, const float dt) {
	zrn_scroll_viewer_t* s = SV(e);
	int busy = 0, fire = 0;
	if (!s->dragging && (fabsf(s->velocity_x) > 20.0f || fabsf(s->velocity_y) > 20.0f)) {
		/* inertia: exponential decay, 1/3 of the speed left after 0.35 s */
		const float decay = expf(-dt / 320.0f);
		const double old_x = s->offset_x, old_y = s->offset_y;
		s->offset_x += s->velocity_x * dt / 1000.0f;
		s->offset_y += s->velocity_y * dt / 1000.0f;
		s->velocity_x *= decay;
		s->velocity_y *= decay;
		sv_clamp(s);
		if (s->offset_x == old_x) s->velocity_x = 0;
		if (s->offset_y == old_y) s->velocity_y = 0;
		zrn_changed_arrange(e);
		s->bar_visible = BAR_HOLD_MS;
		busy = 1;
		fire = s->offset_y != old_y || s->offset_x != old_x;
	} else if (!s->dragging) {
		s->velocity_x = s->velocity_y = 0;
	}
	if (s->bar_visible > 0 && !s->dragging) {
		const float before = s->bar_visible;
		s->bar_visible = zrn_maxf(0, s->bar_visible - dt);
		if (before <= 250.0f || s->bar_visible <= 250.0f) zrn_changed_paint(e);
		busy = busy || s->bar_visible > 0;
	}
	if (!busy && !s->dragging) e->wants_tick = 0;
	/* last: the handler may destroy the viewer */
	if (fire && s->on_scroll) s->on_scroll(e->id, Z_SCROLL_THUMB_POSITION, s->offset_y, s->on_scroll_data);
}

static int sv_can_scroll_y(const zrn_scroll_viewer_t* s) { return s->v_visibility != Z_SCROLLBAR_DISABLED && s->extent_h > s->viewport_h; }
static int sv_can_scroll_x(const zrn_scroll_viewer_t* s) { return s->h_visibility != Z_SCROLLBAR_DISABLED && s->extent_w > s->viewport_w; }

static int sv_input(zrn_element_t* const e, zrn_input_t* const in) {
	zrn_scroll_viewer_t* const s = SV(e);
	switch (in->type) {
		case ZST_IN_PREVIEW_DOWN:
			s->drag_start_x = in->x;
			s->drag_start_y = in->y;
			s->drag_offset_x = s->offset_x;
			s->drag_offset_y = s->offset_y;
			s->last_move_x = in->x;
			s->last_move_y = in->y;
			s->drag_moved = 0;
			s->velocity_x = s->velocity_y = 0;
			s->dragging = 0;
			s->base.base.base.wants_tick = s->bar_visible > 0;
			return 0;
		case ZST_IN_PREVIEW_MOVE:
			if (s->dragging || e->root == nullptr || e->root->capture == e) return 0;
			if (s->drag_moved == 0 && e->root->capture) {
				const float dx = in->x - s->drag_start_x, dy = in->y - s->drag_start_y;
				const int along_y = sv_can_scroll_y(s) && fabsf(dy) > DRAG_THRESHOLD && fabsf(dy) >= fabsf(dx);
				const int along_x = sv_can_scroll_x(s) && fabsf(dx) > DRAG_THRESHOLD && fabsf(dx) > fabsf(dy);
				if (along_y || along_x) {
					/* the finger moved far enough: the drag is ours, whoever had the pointer loses it */
					s->drag_moved = 1;
					s->dragging = 1;
					zrn_set_capture(e->root, e);
					e->wants_tick = 1;
				}
			}
			return 0;
		case ZST_IN_PREVIEW_UP:
			return 0;
		case ZST_IN_POINTER_DOWN:
			/* the viewer itself was hit (no child took it): a drag can start here too */
			s->drag_start_x = in->x;
			s->drag_start_y = in->y;
			s->drag_offset_x = s->offset_x;
			s->drag_offset_y = s->offset_y;
			s->last_move_x = in->x;
			s->last_move_y = in->y;
			s->velocity_x = s->velocity_y = 0;
			if (e->root && (sv_can_scroll_y(s) || sv_can_scroll_x(s))) {
				zrn_set_capture(e->root, e);
				s->dragging = 1;
				s->drag_moved = 0;
				e->wants_tick = 1;
			}
			return 1;
		case ZST_IN_POINTER_MOVE:
			if (!s->dragging) return 0;
			{
				const float dx = in->x - s->drag_start_x, dy = in->y - s->drag_start_y;
				if (!s->drag_moved && fabsf(dx) < DRAG_THRESHOLD && fabsf(dy) < DRAG_THRESHOLD) return 1;
				s->drag_moved = 1;
				if (sv_can_scroll_y(s)) s->offset_y = s->drag_offset_y - dy;
				if (sv_can_scroll_x(s)) s->offset_x = s->drag_offset_x - dx;
				/* velocity in px/s of the content, smoothed; the root ticks in between, so this uses a fixed 16 ms step */
				s->velocity_y = s->velocity_y * 0.5f + (-(in->y - s->last_move_y)) * (1000.0f / 16.0f) * 0.5f;
				s->velocity_x = s->velocity_x * 0.5f + (-(in->x - s->last_move_x)) * (1000.0f / 16.0f) * 0.5f;
				s->last_move_x = in->x;
				s->last_move_y = in->y;
				sv_changed(s, 1);
			}
			return 1;
		case ZST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) zrn_release_capture(e->root, e);
			if (!s->drag_moved) s->velocity_x = s->velocity_y = 0;
			e->wants_tick = 1;
			return 1;
		case ZST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		case ZST_IN_WHEEL:
			if (!sv_can_scroll_y(s) && !sv_can_scroll_x(s)) return 0;
			if (sv_can_scroll_y(s)) s->offset_y -= in->delta * 48.0f;
			else s->offset_x -= in->delta * 48.0f;
			s->velocity_x = s->velocity_y = 0;
			sv_changed(s, 1);
			return 1;
		default:
			return 0;
	}
}

static void sv_init(zrn_element_t* const e) {
	zrn_scroll_viewer_t* const s = SV(e);
	s->v_visibility = Z_SCROLLBAR_AUTO;
	s->h_visibility = Z_SCROLLBAR_DISABLED;
	e->clip_to_bounds = 1;
	e->focusable = 0;
	e->observes_pointer = 1;
	s->base.base.h_content = Z_HALIGN_STRETCH;
	s->base.base.v_content = Z_VALIGN_STRETCH;
}

zrn_class_t zrn_class_scroll_viewer = {
	.base = &zrn_class_content_control, .name = "ScrollViewer", .id = Z_CLASS_SCROLL_VIEWER, .size = sizeof(zrn_scroll_viewer_t),
	.init = sv_init, .measure = sv_measure, .arrange = sv_arrange, .paint_over = sv_paint_over, .input = sv_input, .tick = sv_tick
};

Zid Zscroll_viewer_create(void) {
	const zrn_element_t* const e = zrn_create(Z_CLASS_SCROLL_VIEWER);
	return e ? e->id : Z_INVALID_ID;
}

static zrn_scroll_viewer_t* get_sv(const Zid id) { return SV(zrn_get(id, Z_CLASS_SCROLL_VIEWER)); }

void Zscroll_viewer_set_content(const Zid id, const Zid content) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	zrn_element_t* const c = content != Z_INVALID_ID ? zrn_get(content, Z_CLASS_ELEMENT) : nullptr;
	if (!s || (content != Z_INVALID_ID && !c)) return;
	zrn_content_set(&s->base, c);
}

void Zscroll_viewer_set_vertical_scrollbar_visibility(const Zid id, const ZScrollBarVisibility v) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	if (!s) return;
	s->v_visibility = v;
	zrn_changed_layout(&s->base.base.base);
}

void Zscroll_viewer_set_horizontal_scrollbar_visibility(const Zid id, const ZScrollBarVisibility v) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	if (!s) return;
	s->h_visibility = v;
	zrn_changed_layout(&s->base.base.base);
}

void Zscroll_viewer_scroll_to_vertical_offset(const Zid id, const double offset) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	if (!s) return;
	s->offset_y = offset;
	s->velocity_y = 0;
	sv_changed(s, 1);
}

void Zscroll_viewer_scroll_to_horizontal_offset(const Zid id, const double offset) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	if (!s) return;
	s->offset_x = offset;
	s->velocity_x = 0;
	sv_changed(s, 1);
}

double Zscroll_viewer_get_vertical_offset(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->offset_y : 0; }
double Zscroll_viewer_get_horizontal_offset(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->offset_x : 0; }
double Zscroll_viewer_get_extent_height(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->extent_h : 0; }
double Zscroll_viewer_get_extent_width(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->extent_w : 0; }
double Zscroll_viewer_get_viewport_height(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->viewport_h : 0; }
double Zscroll_viewer_get_viewport_width(Zid id) { const zrn_scroll_viewer_t* s = get_sv(id); return s ? s->viewport_w : 0; }

void Zscroll_viewer_on_scroll(const Zid id, const ZScrollCallback callback, void* const user_data) {
	zrn_scroll_viewer_t* const s = get_sv(id);
	if (!s) return;
	s->on_scroll = callback;
	s->on_scroll_data = user_data;
}
