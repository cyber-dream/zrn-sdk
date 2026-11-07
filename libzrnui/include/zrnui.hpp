// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * libzrnui for C++: the same library as zrnui.h, with lambdas for callbacks
 * and an App that frees everything when it goes out of scope. Header only.
 *
 *	#include <zrnui.hpp>
 *	int main() {
 *		zrn::ui::App app("zrn-hello-cpp", "Hello");
 *		auto page = app.page();
 *		auto label = page.text("Not tapped yet");
 *		int taps = 0;
 *		page.button("Tap me", [=]() mutable {
 *			label.set_text("Tapped " + std::to_string(++taps) + " times");
 *		});
 *		return app.run(page);
 *	}
 *
 * A Node is a handle (copy it freely); the App owns what it points at.
 */
#ifndef ZRNUI_HPP
#define ZRNUI_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "zrnui.h"

namespace zrn::ui {

enum class Variant { Primary = ZUI_PRIMARY, Tonal = ZUI_TONAL, Plain = ZUI_PLAIN, Danger = ZUI_DANGER, Link = ZUI_LINK };

namespace detail {
using Handler = std::function<void(zui_node *)>;
inline void call(zui_node *n, void *data) { (*static_cast<Handler *>(data))(n); }
inline void drop(void *data) { delete static_cast<Handler *>(data); }
inline void call_hold(zui_node *, void *data) { (*static_cast<std::function<void()> *>(data))(); }
inline void drop_hold(void *data) { delete static_cast<std::function<void()> *>(data); }
inline void attach(zui_node *n, Handler h) {
	if (!h) return;
	zui_set_handler(n, call, new Handler(std::move(h)), drop);
}
}  // namespace detail

class Node {
public:
	Node() = default;
	explicit Node(zui_node *n) : n_(n) {}
	zui_node *get() const { return n_; }
	explicit operator bool() const { return n_ != nullptr; }

	/* what goes in it */
	Node column() { return Node(zui_column(n_)); }
	Node row() { return Node(zui_row(n_)); }
	Node card() { return Node(zui_card(n_)); }
	Node section(const std::string &title) { return Node(zui_section(n_, title.c_str())); }
	Node space(double points) { return Node(zui_space(n_, points)); }
	Node text(const std::string &s) { return Node(zui_text(n_, s.c_str())); }
	Node heading(const std::string &s) { return Node(zui_heading(n_, s.c_str())); }
	Node caption(const std::string &s) { return Node(zui_caption(n_, s.c_str())); }

	Node button(const std::string &label, std::function<void()> on_click = {}) {
		Node b(zui_button(n_, label.c_str(), nullptr, nullptr));
		if (on_click) detail::attach(b.n_, [f = std::move(on_click)](zui_node *) { f(); });
		return b;
	}
	Node item(const std::string &title, const std::string &subtitle = {}, std::function<void()> on_click = {}) {
		Node i(zui_item(n_, title.c_str(), subtitle.empty() ? nullptr : subtitle.c_str(), nullptr, nullptr));
		if (on_click) detail::attach(i.n_, [f = std::move(on_click)](zui_node *) { f(); });
		return i;
	}
	/* a switch ("switch" is taken in C++) */
	Node toggle(const std::string &title, bool on, std::function<void(bool)> on_change = {}) {
		Node s(zui_switch(n_, title.c_str(), on, nullptr, nullptr));
		if (on_change) detail::attach(s.n_, [f = std::move(on_change)](zui_node *n) { f(zui_get_on(n)); });
		return s;
	}
	/* on_change(value, done): done when the finger lets go */
	Node slider(const std::string &title, double value, std::function<void(double, bool)> on_change = {}) {
		Node s(zui_slider(n_, title.c_str(), value, nullptr, nullptr));
		if (on_change)
			detail::attach(s.n_, [f = std::move(on_change)](zui_node *n) {
				f(zui_get_value(n), zui_event(n) == ZUI_EVENT_SUBMIT);
			});
		return s;
	}
	Node progress(double value) { return Node(zui_progress(n_, value)); }
	/* on_change(text, submitted): submitted on Enter */
	Node field(const std::string &placeholder, std::function<void(const std::string &, bool)> on_change = {}) {
		Node f(zui_field(n_, placeholder.c_str(), nullptr, nullptr));
		if (on_change)
			detail::attach(f.n_, [cb = std::move(on_change)](zui_node *n) {
				cb(zui_get_text(n), zui_event(n) == ZUI_EVENT_SUBMIT);
			});
		return f;
	}

	/* ---- what Windows Phone 8.1 had (see zrnui.h) ---- */
	Node check(const std::string &title, bool on, std::function<void(bool)> on_change = {}) {
		Node c(zui_check(n_, title.c_str(), on, nullptr, nullptr));
		if (on_change) detail::attach(c.n_, [f = std::move(on_change)](zui_node *n) { f(zui_get_on(n)); });
		return c;
	}
	Node radio(const std::string &title, bool on, std::function<void(bool)> on_change = {}) {
		Node c(zui_radio(n_, title.c_str(), on, nullptr, nullptr));
		if (on_change) detail::attach(c.n_, [f = std::move(on_change)](zui_node *n) { f(zui_get_on(n)); });
		return c;
	}
	Node ring() { return Node(zui_ring(n_)); }
	Node &set_password(bool p) { zui_field_set_password(n_, p); return *this; }
	Node &set_suggestions(const std::vector<std::string> &items) {
		std::vector<const char *> v;
		for (auto &s : items) v.push_back(s.c_str());
		zui_field_set_suggestions(n_, v.data(), static_cast<int>(v.size()));
		return *this;
	}
	Node picker(const std::string &caption, const std::vector<std::string> &options, int selected,
			std::function<void(int)> on_change = {}) {
		std::vector<const char *> v;
		for (auto &s : options) v.push_back(s.c_str());
		Node p(zui_picker(n_, caption.c_str(), v.data(), static_cast<int>(v.size()), selected, nullptr, nullptr));
		if (on_change) detail::attach(p.n_, [f = std::move(on_change)](zui_node *n) { f(zui_picker_selected(n)); });
		return p;
	}
	int selected() const { return zui_picker_selected(n_); }
	Node date_picker(const std::string &title, int year, int month, int day,
			std::function<void(int, int, int)> on_change = {}) {
		Node p(zui_date_picker(n_, title.c_str(), year, month, day, nullptr, nullptr));
		if (on_change)
			detail::attach(p.n_, [f = std::move(on_change)](zui_node *n) {
				int y, m, d;
				zui_date_picker_get(n, &y, &m, &d);
				f(y, m, d);
			});
		return p;
	}
	Node time_picker(const std::string &title, int hour, int minute, std::function<void(int, int)> on_change = {}) {
		Node p(zui_time_picker(n_, title.c_str(), hour, minute, nullptr, nullptr));
		if (on_change)
			detail::attach(p.n_, [f = std::move(on_change)](zui_node *n) {
				int h, m;
				zui_time_picker_get(n, &h, &m);
				f(h, m);
			});
		return p;
	}
	Node tiles() { return Node(zui_tiles(n_)); }
	Node tile(const std::string &title, const std::string &icon, bool wide = false, std::function<void()> on_tap = {}) {
		Node t(zui_tile(n_, title.c_str(), icon.c_str(), wide, nullptr, nullptr));
		if (on_tap) detail::attach(t.n_, [f = std::move(on_tap)](zui_node *) { f(); });
		return t;
	}
	Node &set_live(const std::string &back, int period_ms) { zui_tile_set_live(n_, back.c_str(), period_ms); return *this; }
	Node hub_section() { return Node(zui_hub_section(n_)); }
	Node &set_background(zui_pixmap *pix) { zui_hub_set_background(n_, pix); return *this; }
	Node &show_section(int i) { zui_hub_show(n_, i); return *this; }
	/* a press and hold */
	Node &on_hold(std::function<void()> f) {
		zui_set_hold(n_, detail::call_hold, new std::function<void()>(std::move(f)), detail::drop_hold);
		return *this;
	}
	Node &scroll_to(double y, bool glide = true) { zui_page_scroll_to(n_, y, glide); return *this; }

	Node footer() { return Node(zui_footer(n_)); }
	Node item() { return Node(zui_pivot_item(n_)); }
	Node item_header(const std::string &t) { return Node(zui_pivot_item_header(n_, t.c_str())); }
	void pivot_title(const std::string &t) { zui_pivot_title(n_, t.c_str()); }
	Node bubble(bool mine) { return Node(zui_bubble(n_, mine)); }
	Node image(zui_pixmap *pix, double w = 0, double h = 0) { return Node(zui_image(n_, pix, w, h)); }
	/* height(row) is a guess; bind(row, node) fills a row. Live as long as the list. */
	Node list(int count, std::function<double(int)> height, std::function<void(int, Node)> bind) {
		auto *d = new ListFns{std::move(height), std::move(bind)};
		zui_node *l = zui_list(n_, count, &Node::list_h, &Node::list_b, d);
		return Node(l);
	}
	Node &set_image(zui_pixmap *pix) { zui_set_image(n_, pix); return *this; }
	Node &set_image_sharp(bool s) { zui_set_image_sharp(n_, s); return *this; }
	Node &set_radius(double r) { zui_set_radius(n_, r); return *this; }
	Node &set_monogram(const std::string &s) { zui_set_monogram(n_, s.c_str()); return *this; }
	Node &set_badge(const std::string &s) { zui_set_badge(n_, s.c_str()); return *this; }
	Node &set_emphasis(bool on) { zui_set_emphasis(n_, on); return *this; }
	Node &set_align(int a) { zui_set_align(n_, a); return *this; }

	Node &set_multiline(bool m) { zui_field_set_multiline(n_, m); return *this; }
	Node &set_bar(bool b) { zui_set_bar(n_, b); return *this; }
	Node &set_compact(bool c) { zui_page_set_compact(n_, c); return *this; }
	/* a page's TitlePanel: its application title (small) and page title (large), as written */
	Node title_panel() { return Node(zui_title_panel(n_)); }
	/* a column with the margin and the gaps; the page's root itself has none */
	Node content_panel() { return Node(zui_content_panel(n_)); }
	/* a scroll viewer: what is put in it scrolls, the rest of the page stays */
	Node scroll() { return Node(zui_scroll(n_)); }
	/* the two titles: small and large, in a TitlePanel or a hub section */
	Node title(const std::string &s) { return Node(zui_title(n_, s.c_str())); }
	Node header(const std::string &s) { return Node(zui_header(n_, s.c_str())); }
	/* changing it; each returns the node, so they chain */
	Node &set_text(const std::string &s) { zui_set_text(n_, s.c_str()); return *this; }
	Node &set_detail(const std::string &s) { zui_set_detail(n_, s.c_str()); return *this; }
	Node &set_subtitle(const std::string &s) { zui_set_subtitle(n_, s.c_str()); return *this; }
	Node &set_value(double v) { zui_set_value(n_, v); return *this; }
	Node &set_on(bool on) { zui_set_on(n_, on); return *this; }
	Node &set_enabled(bool e) { zui_set_enabled(n_, e); return *this; }
	Node &set_visible(bool v) { zui_set_visible(n_, v); return *this; }
	Node &set_variant(Variant v) { zui_set_variant(n_, static_cast<int>(v)); return *this; }
	Node &set_flex(double f) { zui_set_flex(n_, f); return *this; }
	Node &set_icon(const std::string &name) { zui_set_icon(n_, name.c_str()); return *this; }
	Node &set_tint(uint32_t rgb) { zui_set_tint(n_, rgb); return *this; }
	/* any callback, with the node and why it ran (zui_event) */
	Node &on(std::function<void(Node, int)> f) {
		detail::attach(n_, [f = std::move(f)](zui_node *n) { f(Node(n), zui_event(n)); });
		return *this;
	}

	std::string text_value() const { return zui_get_text(n_); }
	double value() const { return zui_get_value(n_); }
	bool is_on() const { return zui_get_on(n_); }
	void clear() { zui_clear(n_); }
	void destroy() { zui_destroy(n_); n_ = nullptr; }
	void push() { zui_push(n_); }

private:
	struct ListFns { std::function<double(int)> h; std::function<void(int, Node)> b; };
	static double list_h(int r, void *d) { return static_cast<ListFns *>(d)->h(r); }
	static void list_b(int r, zui_node *n, void *d) { static_cast<ListFns *>(d)->b(r, Node(n)); }
	zui_node *n_ = nullptr;
};

class App {
public:
	App() : app_(zui_app_new()) {}
	~App() {
		zui_app_free(app_);
		for (auto &t : timers_) delete t.second;
	}
	App(const App &) = delete;
	App &operator=(const App &) = delete;

	zui_app *get() const { return app_; }
	/* the template (a TitlePanel and a ContentPanel), an empty root, an empty root over the whole window */
	Node page() { return Node(zui_page(app_)); }
	Node page_blank() { return Node(zui_page_blank(app_)); }
	/* a hub and a pivot are roots of their own: run or push them as they are */
	Node hub() { return Node(zui_hub(app_)); }
	Node pivot() { return Node(zui_pivot(app_)); }
	Node page_fullscreen() { return Node(zui_page_fullscreen(app_)); }
	int run(Node first) { return zui_run(app_, first.get()); }
	void push(Node page) { zui_push(page.get()); }
	void back() { zui_back(app_); }
	void quit() { zui_quit(app_); }
	void busy(bool b) { zui_busy(app_, b); }
	void lock(bool on) { zui_list_lock(app_, on); }
	void toast(const std::string &s) { zui_toast(app_, s.c_str()); }
	void set_style(const std::string &s) { zui_app_set_style(app_, s.c_str()); }
	void set_theme(const std::string &s) { zui_app_set_theme(app_, s.c_str()); }
	void set_accent(uint32_t rgb) { zui_app_set_accent(app_, rgb); }
	std::string style() const { return zui_app_style(app_); }
	int render_png(Node page, const std::string &path, int w = 360, int h = 740, int scale = 2) {
		return zui_render_png(app_, page.get(), path.c_str(), w, h, scale);
	}

	/* f() after ms (and every ms if repeat); returns an id for cancel_timer */
	int timer(int ms, bool repeat, std::function<void()> f) {
		auto *slot = new Slot{ this, 0, repeat, std::move(f) };
		int id = zui_timer(app_, ms, repeat, &App::fire, slot);
		if (!id) { delete slot; return 0; }
		slot->id = id;
		timers_[id] = slot;
		return id;
	}
	void cancel_timer(int id) {
		auto it = timers_.find(id);
		if (it == timers_.end()) return;
		zui_timer_cancel(app_, id);
		delete it->second;
		timers_.erase(it);
	}
	/* from any thread: f() on the app's thread */
	void post(std::function<void()> f) {
		zui_post(app_, [](void *d) {
			std::unique_ptr<std::function<void()>> g(static_cast<std::function<void()> *>(d));
			(*g)();
		}, new std::function<void()>(std::move(f)));
	}

	/* ---- over the page: one at a time; f gets the index chosen ---- */
	void menu(Node anchor, const std::vector<std::string> &items, std::function<void(int)> f) {
		auto v = cstrs(items);
		over_ = std::move(f);
		zui_menu(app_, anchor.get(), v.data(), static_cast<int>(v.size()), &App::chose, this);
	}
	void dialog(const std::string &title, const std::string &message, const std::vector<std::string> &buttons,
			std::function<void(int)> f) {
		auto v = cstrs(buttons);
		over_ = std::move(f);
		zui_dialog(app_, title.c_str(), message.c_str(), v.data(), static_cast<int>(v.size()), &App::chose, this);
	}
	void jump_list(const std::vector<std::string> &labels, const std::vector<bool> &live, std::function<void(int)> f) {
		auto v = cstrs(labels);
		std::unique_ptr<bool[]> l(new bool[labels.size()]);
		for (size_t i = 0; i < labels.size(); i++) l[i] = live.empty() || live[i];
		over_ = std::move(f);
		zui_jump_list(app_, v.data(), l.get(), static_cast<int>(v.size()), &App::chose, this);
	}
	bool overlay_open() { return zui_overlay_open(app_); }
	void close_overlay() { zui_overlay_close(app_); }

private:
	static std::vector<const char *> cstrs(const std::vector<std::string> &v) {
		std::vector<const char *> o;
		for (auto &s : v) o.push_back(s.c_str());
		return o;
	}
	static void chose(zui_app *, int i, void *d) {
		auto *a = static_cast<App *>(d);
		auto f = std::move(a->over_);
		if (f) f(i);
	}
	std::function<void(int)> over_;
	struct Slot { App *app; int id; bool repeat; std::function<void()> f; };
	static void fire(void *d) {
		auto *s = static_cast<Slot *>(d);
		auto f = s->f;	/* the slot may go if f cancels it */
		if (!s->repeat) {
			s->app->timers_.erase(s->id);
			delete s;
		}
		f();
	}
	zui_app *app_;
	std::map<int, Slot *> timers_;
};

}  // namespace zrn::ui

#endif
