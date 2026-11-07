/**
 * Kestrel - controls/scroll.c
 * ScrollViewer: touch scrolling with inertia, the wheel, and the thin
 * transient scroll bars of Windows Phone.
 */

#include "../internal.h"

#define SV(e) KST_AS(kst_scroll_viewer_t, e)

/* a drag starts after the finger moved this far */
#define DRAG_THRESHOLD 10.0f
/* the bars stay in view this long after the last scroll */
#define BAR_HOLD_MS 800.0f


static void sv_limits(const kst_scroll_viewer_t* s, double* max_x, double* max_y) {
	*max_x = s->extent_w > s->viewport_w ? s->extent_w - s->viewport_w : 0;
	*max_y = s->extent_h > s->viewport_h ? s->extent_h - s->viewport_h : 0;
}

static void sv_clamp(kst_scroll_viewer_t* s) {
	double mx, my;
	sv_limits(s, &mx, &my);
	if (s->offset_x < 0) s->offset_x = 0;
	if (s->offset_x > mx) s->offset_x = mx;
	if (s->offset_y < 0) s->offset_y = 0;
	if (s->offset_y > my) s->offset_y = my;
}

static void sv_changed(kst_scroll_viewer_t* s, int fire) {
	kst_element_t* e = &s->base.base.base;
	sv_clamp(s);
	s->bar_visible = BAR_HOLD_MS;
	e->wants_tick = 1;
	kst_changed_arrange(e);
	if (fire && s->on_scroll) s->on_scroll(e->id, K_SCROLL_THUMB_POSITION, s->offset_y, s->on_scroll_data);
}

static KSize sv_measure(kst_element_t* e, KSize available) {
	kst_scroll_viewer_t* s = SV(e);
	kst_content_control_t* cc = &s->base;
	KSize room = available, content = { 0, 0 };
	if (s->h_visibility != K_SCROLLBAR_DISABLED) room.width = K_INFINITE;
	if (s->v_visibility != K_SCROLLBAR_DISABLED) room.height = K_INFINITE;
	if (cc->content) {
		kst_measure(cc->content, room);
		content = cc->content->desired;
	}
	return kst_size(kst_minf(content.width, available.width), kst_minf(content.height, available.height));
}

static KSize sv_arrange(kst_element_t* e, KSize final) {
	kst_scroll_viewer_t* s = SV(e);
	kst_content_control_t* cc = &s->base;
	s->viewport_w = final.width;
	s->viewport_h = final.height;
	if (cc->content) {
		KSize d = cc->content->desired;
		float w = s->h_visibility == K_SCROLLBAR_DISABLED ? final.width : kst_maxf(d.width, final.width);
		float h = s->v_visibility == K_SCROLLBAR_DISABLED ? final.height : kst_maxf(d.height, final.height);
		s->extent_w = s->h_visibility == K_SCROLLBAR_DISABLED ? final.width : d.width;
		s->extent_h = s->v_visibility == K_SCROLLBAR_DISABLED ? final.height : d.height;
		sv_clamp(s);
		kst_arrange(cc->content, kst_rect((float)-s->offset_x, (float)-s->offset_y, w, h));
	} else {
		s->extent_w = final.width;
		s->extent_h = final.height;
	}
	s->show_v = s->v_visibility != K_SCROLLBAR_DISABLED && s->v_visibility != K_SCROLLBAR_HIDDEN && s->extent_h > s->viewport_h + 0.5;
	s->show_h = s->h_visibility != K_SCROLLBAR_DISABLED && s->h_visibility != K_SCROLLBAR_HIDDEN && s->extent_w > s->viewport_w + 0.5;
	if (s->v_visibility == K_SCROLLBAR_VISIBLE) s->show_v = 1;
	if (s->h_visibility == K_SCROLLBAR_VISIBLE) s->show_h = 1;
	return final;
}

static void sv_paint_over(kst_element_t* e, kst_painter_t* p) {
	kst_scroll_viewer_t* s = SV(e);
	KColor color;
	double mx, my;
	float alpha;
	int always_v = s->v_visibility == K_SCROLLBAR_VISIBLE, always_h = s->h_visibility == K_SCROLLBAR_VISIBLE;
	sv_limits(s, &mx, &my);
	alpha = kst_clampf(s->bar_visible / 250.0f, 0, 1);
	color = kst_theme_color(e, KST_TC_DISABLED);
	if (s->show_v && (alpha > 0 || always_v)) {
		float length = e->rect.height - (s->show_h ? 7.0f : 0);
		float thumb = kst_clampf((float)(s->viewport_h / s->extent_h) * length, kst_minf(72.0f, length), length);
		float pos = my > 0 ? (float)(s->offset_y / my) * (length - thumb) : 0;
		kst_fill_rect(p, kst_rect(e->rect.width - 6, pos + 1, 5, thumb - 2), kst_color_alpha(color, always_v ? 1.0f : alpha));
	}
	if (s->show_h && (alpha > 0 || always_h)) {
		float length = e->rect.width - (s->show_v ? 7.0f : 0);
		float thumb = kst_clampf((float)(s->viewport_w / s->extent_w) * length, kst_minf(72.0f, length), length);
		float pos = mx > 0 ? (float)(s->offset_x / mx) * (length - thumb) : 0;
		kst_fill_rect(p, kst_rect(pos + 1, e->rect.height - 6, thumb - 2, 5), kst_color_alpha(color, always_h ? 1.0f : alpha));
	}
}

static void sv_tick(kst_element_t* e, float dt) {
	kst_scroll_viewer_t* s = SV(e);
	int busy = 0, fire = 0;
	if (!s->dragging && (fabsf(s->velocity_x) > 20.0f || fabsf(s->velocity_y) > 20.0f)) {
		/* inertia: exponential decay, 1/3 of the speed left after 0.35 s */
		float decay = expf(-dt / 320.0f);
		double old_x = s->offset_x, old_y = s->offset_y;
		s->offset_x += s->velocity_x * dt / 1000.0f;
		s->offset_y += s->velocity_y * dt / 1000.0f;
		s->velocity_x *= decay;
		s->velocity_y *= decay;
		sv_clamp(s);
		if (s->offset_x == old_x) s->velocity_x = 0;
		if (s->offset_y == old_y) s->velocity_y = 0;
		kst_changed_arrange(e);
		s->bar_visible = BAR_HOLD_MS;
		busy = 1;
		fire = s->offset_y != old_y || s->offset_x != old_x;
	} else if (!s->dragging) {
		s->velocity_x = s->velocity_y = 0;
	}
	if (s->bar_visible > 0 && !s->dragging) {
		float before = s->bar_visible;
		s->bar_visible = kst_maxf(0, s->bar_visible - dt);
		if (before <= 250.0f || s->bar_visible <= 250.0f) kst_changed_paint(e);
		busy = busy || s->bar_visible > 0;
	}
	if (!busy && !s->dragging) e->wants_tick = 0;
	/* last: the handler may destroy the viewer */
	if (fire && s->on_scroll) s->on_scroll(e->id, K_SCROLL_THUMB_POSITION, s->offset_y, s->on_scroll_data);
}

static int sv_can_scroll_y(const kst_scroll_viewer_t* s) { return s->v_visibility != K_SCROLLBAR_DISABLED && s->extent_h > s->viewport_h; }
static int sv_can_scroll_x(const kst_scroll_viewer_t* s) { return s->h_visibility != K_SCROLLBAR_DISABLED && s->extent_w > s->viewport_w; }

static int sv_input(kst_element_t* e, kst_input_t* in) {
	kst_scroll_viewer_t* s = SV(e);
	switch (in->type) {
		case KST_IN_PREVIEW_DOWN:
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
		case KST_IN_PREVIEW_MOVE:
			if (s->dragging || e->root == NULL || e->root->capture == e) return 0;
			if (s->drag_moved == 0 && e->root->capture) {
				float dx = in->x - s->drag_start_x, dy = in->y - s->drag_start_y;
				int along_y = sv_can_scroll_y(s) && fabsf(dy) > DRAG_THRESHOLD && fabsf(dy) >= fabsf(dx);
				int along_x = sv_can_scroll_x(s) && fabsf(dx) > DRAG_THRESHOLD && fabsf(dx) > fabsf(dy);
				if (along_y || along_x) {
					/* the finger moved far enough: the drag is ours, whoever had the pointer loses it */
					s->drag_moved = 1;
					s->dragging = 1;
					kst_set_capture(e->root, e);
					e->wants_tick = 1;
				}
			}
			return 0;
		case KST_IN_PREVIEW_UP:
			return 0;
		case KST_IN_POINTER_DOWN:
			/* the viewer itself was hit (no child took it): a drag can start here too */
			s->drag_start_x = in->x;
			s->drag_start_y = in->y;
			s->drag_offset_x = s->offset_x;
			s->drag_offset_y = s->offset_y;
			s->last_move_x = in->x;
			s->last_move_y = in->y;
			s->velocity_x = s->velocity_y = 0;
			if (e->root && (sv_can_scroll_y(s) || sv_can_scroll_x(s))) {
				kst_set_capture(e->root, e);
				s->dragging = 1;
				s->drag_moved = 0;
				e->wants_tick = 1;
			}
			return 1;
		case KST_IN_POINTER_MOVE:
			if (!s->dragging) return 0;
			{
				float dx = in->x - s->drag_start_x, dy = in->y - s->drag_start_y;
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
		case KST_IN_POINTER_UP:
			if (!s->dragging) return 0;
			s->dragging = 0;
			if (e->root) kst_release_capture(e->root, e);
			if (!s->drag_moved) s->velocity_x = s->velocity_y = 0;
			e->wants_tick = 1;
			return 1;
		case KST_IN_CAPTURE_LOST:
			s->dragging = 0;
			return 0;
		case KST_IN_WHEEL:
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

static void sv_init(kst_element_t* e) {
	kst_scroll_viewer_t* s = SV(e);
	s->v_visibility = K_SCROLLBAR_AUTO;
	s->h_visibility = K_SCROLLBAR_DISABLED;
	e->clip_to_bounds = 1;
	e->focusable = 0;
	e->observes_pointer = 1;
	s->base.base.h_content = K_HALIGN_STRETCH;
	s->base.base.v_content = K_VALIGN_STRETCH;
}

kst_class_t kst_class_scroll_viewer = {
	.base = &kst_class_content_control, .name = "ScrollViewer", .id = K_CLASS_SCROLL_VIEWER, .size = sizeof(kst_scroll_viewer_t),
	.init = sv_init, .measure = sv_measure, .arrange = sv_arrange, .paint_over = sv_paint_over, .input = sv_input, .tick = sv_tick
};

Kid Kscroll_viewer_create(void) {
	kst_element_t* e = kst_create(K_CLASS_SCROLL_VIEWER);
	return e ? e->id : K_INVALID_ID;
}

static kst_scroll_viewer_t* get_sv(Kid id) { return SV(kst_get(id, K_CLASS_SCROLL_VIEWER)); }

void Kscroll_viewer_set_content(Kid id, Kid content) {
	kst_scroll_viewer_t* s = get_sv(id);
	kst_element_t* c = content != K_INVALID_ID ? kst_get(content, K_CLASS_ELEMENT) : NULL;
	if (!s || (content != K_INVALID_ID && !c)) return;
	kst_content_set(&s->base, c);
}

void Kscroll_viewer_set_vertical_scrollbar_visibility(Kid id, KScrollBarVisibility v) {
	kst_scroll_viewer_t* s = get_sv(id);
	if (!s) return;
	s->v_visibility = v;
	kst_changed_layout(&s->base.base.base);
}

void Kscroll_viewer_set_horizontal_scrollbar_visibility(Kid id, KScrollBarVisibility v) {
	kst_scroll_viewer_t* s = get_sv(id);
	if (!s) return;
	s->h_visibility = v;
	kst_changed_layout(&s->base.base.base);
}

void Kscroll_viewer_scroll_to_vertical_offset(Kid id, double offset) {
	kst_scroll_viewer_t* s = get_sv(id);
	if (!s) return;
	s->offset_y = offset;
	s->velocity_y = 0;
	sv_changed(s, 1);
}

void Kscroll_viewer_scroll_to_horizontal_offset(Kid id, double offset) {
	kst_scroll_viewer_t* s = get_sv(id);
	if (!s) return;
	s->offset_x = offset;
	s->velocity_x = 0;
	sv_changed(s, 1);
}

double Kscroll_viewer_get_vertical_offset(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->offset_y : 0; }
double Kscroll_viewer_get_horizontal_offset(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->offset_x : 0; }
double Kscroll_viewer_get_extent_height(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->extent_h : 0; }
double Kscroll_viewer_get_extent_width(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->extent_w : 0; }
double Kscroll_viewer_get_viewport_height(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->viewport_h : 0; }
double Kscroll_viewer_get_viewport_width(Kid id) { kst_scroll_viewer_t* s = get_sv(id); return s ? s->viewport_w : 0; }

void Kscroll_viewer_on_scroll(Kid id, KScrollCallback callback, void* user_data) {
	kst_scroll_viewer_t* s = get_sv(id);
	if (!s) return;
	s->on_scroll = callback;
	s->on_scroll_data = user_data;
}
