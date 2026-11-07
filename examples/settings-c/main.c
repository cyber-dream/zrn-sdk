// SPDX-License-Identifier: MIT
/*
 * A settings app: grouped lists, switches, sliders, a page that opens from a
 * row, a field to type into, and work that takes a while (the busy bar and a
 * toast when it is done). The kind of app most of ztOS is made of.
 */
#include <stdio.h>
#include <string.h>
#include <zrnui.h>

static zui_app *app;
static zui_node *wifi_page, *wifi_row, *join_field, *networks, *update_row;
static zui_node *controls_page, *tiles_page, *hub_page, *people_page;
static zui_node *date_row, *time_row, *lang_picker, *tile_last;

/* a page's TitlePanel, as on Windows Phone: the app's name small, the page's name large */
static zui_node *titled(const char *title) {
	zui_node *page = zui_page(app);
	zui_node *panel = zui_title_panel(page);
	zui_title(panel, "SETTINGS");
	zui_header(panel, title);
	return page;
}

static void open_wifi(zui_node *row, void *data) { zui_push(wifi_page); }

static void wifi_switched(zui_node *sw, void *data) {
	bool on = zui_get_on(sw);
	zui_set_detail(wifi_row, on ? "Home-5G" : "Off");
	zui_set_visible(networks, on);
}

static void brightness(zui_node *slider, void *data) {
	if (zui_event(slider) == ZUI_EVENT_SUBMIT)
		fprintf(stderr, "brightness set to %.0f%%\n", zui_get_value(slider) * 100);
}

static void dark_theme(zui_node *sw, void *data) {
	zui_app_set_theme(app, zui_get_on(sw) ? "dark" : "light");
}

static void checked(void *data) {
	zui_busy(app, false);
	zui_set_detail(update_row, "Up to date");
	zui_toast(app, "ztOS is up to date");
}

static void check_updates(zui_node *button, void *data) {
	zui_busy(app, true);
	zui_set_detail(update_row, "Checking...");
	zui_timer(app, 1800, false, checked, NULL);
}

static void join(zui_node *field, void *data) {
	if (zui_event(field) != ZUI_EVENT_SUBMIT && field == join_field) return;
	const char *name = zui_get_text(join_field);
	if (!*name) return;
	char line[160];
	snprintf(line, sizeof(line), "Joining %s", name);
	zui_toast(app, line);
}

static void network(zui_node *card, const char *name, const char *how, bool secured) {
	zui_node *row = zui_item(card, name, how, open_wifi, NULL);
	zui_set_icon(row, secured ? "lock" : "wifi");
	zui_set_tint(row, secured ? 0x3b82f6 : 0x10b981);
}

static void build_wifi_page(void) {
	wifi_page = titled("wi-fi");
	zui_node *content = zui_content_panel(wifi_page);
	zui_node *top = zui_card(content);
	zui_node *sw = zui_switch(top, "Wi-Fi", true, wifi_switched, NULL);
	zui_set_subtitle(sw, "Connected to Home-5G");
	zui_set_icon(sw, "wifi");
	networks = zui_column(content);
	zui_node *list = zui_section(networks, "Networks nearby");
	network(list, "Home-5G", "Connected, 5 GHz", true);
	network(list, "Zerone Lab", "Secured, 2.4 GHz", true);
	network(list, "Cafe Guest", "Open", false);
	zui_node *other = zui_section(networks, "Another network");
	join_field = zui_field(other, "Network name", join, NULL);
	zui_node *row = zui_row(other);
	zui_node *b = zui_button(row, "Join", join, NULL);
	zui_set_flex(b, 1);
}


/* ---- the controls Windows Phone 8.1 had: check boxes, pickers, dialogs ... ---- */
static void open_controls(zui_node *row, void *data) { zui_push(controls_page); }
static void open_tiles(zui_node *row, void *data) { zui_push(tiles_page); }
static void open_hub(zui_node *row, void *data) { zui_push(hub_page); }
static void open_people(zui_node *row, void *data) { zui_push(people_page); }

static void say(const char *what, zui_node *n) {
	char line[160];
	snprintf(line, sizeof(line), "%s: %s", what, zui_get_text(n));
	zui_toast(app, line);
}

static void box_ticked(zui_node *c, void *data) {
	char line[120];
	snprintf(line, sizeof(line), "%s %s", zui_get_text(c), zui_get_on(c) ? "ticked" : "cleared");
	zui_toast(app, line);
}

static void radio_changed(zui_node *r, void *data) {
	if (zui_get_on(r)) say("Chosen", r);
}

static void lang_changed(zui_node *p, void *data) {
	char line[80];
	snprintf(line, sizeof(line), "Language %d chosen", zui_picker_selected(p));
	zui_toast(app, line);
}

static void date_changed(zui_node *p, void *data) {
	int y, m, d;
	zui_date_picker_get(p, &y, &m, &d);
	char line[80];
	snprintf(line, sizeof(line), "Date %04d-%02d-%02d", y, m, d);
	zui_toast(app, line);
}

static void time_changed(zui_node *p, void *data) {
	int h, m;
	zui_time_picker_get(p, &h, &m);
	char line[80];
	snprintf(line, sizeof(line), "Time %02d:%02d", h, m);
	zui_toast(app, line);
}

static void dialog_chose(zui_app *a, int index, void *data) {
	static const char *const what[] = { "Deleted", "Kept" };
	zui_toast(a, what[index == 0 ? 0 : 1]);
}

static void ask_dialog(zui_node *b, void *data) {
	static const char *const buttons[] = { "delete", "keep" };
	zui_dialog(app, "Delete this album?",
		"The 42 pictures in it go too, and cannot be got back.", buttons, 2, dialog_chose, NULL);
}

static void menu_chose(zui_app *a, int index, void *data) {
	static const char *const what[] = { "Copied", "Pinned to Start", "Deleted" };
	zui_toast(a, what[index < 3 ? index : 0]);
}

static void menu_for(zui_node *row, void *data) {
	static const char *const items[] = { "copy", "pin to start", "delete" };
	zui_menu(app, row, items, 3, menu_chose, NULL);
}

void people_jump(int index);
static void jumped(zui_app *a, int index, void *data) { people_jump(index); }

static const char *const LETTERS[] = { "#", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
	"o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "•" };

static zui_node *letter_sections[28];
static bool letter_live[28];

void people_jump(int index) {
	if (index < 28 && letter_sections[index]) {
		double x, y, w, h;
		zui_node_frame(letter_sections[index], &x, &y, &w, &h);
		zui_page_scroll_to(people_page, y - 8, true);
	}
}

static void open_jump(zui_node *b, void *data) {
	zui_jump_list(app, LETTERS, letter_live, 27, jumped, NULL);
}

static void build_people_page(void) {
	static const char *const names[] = { "Ada Lovelace", "Alan Turing", "Barbara Liskov", "Bjarne Stroustrup",
		"Dennis Ritchie", "Donald Knuth", "Edsger Dijkstra", "Grace Hopper", "Guido van Rossum",
		"Ken Thompson", "Linus Torvalds", "Margaret Hamilton", "Tim Berners-Lee" };
	people_page = titled("people");
	zui_node *content = zui_content_panel(people_page);
	zui_node *b = zui_button(content, "Jump to a letter", open_jump, NULL);
	zui_set_variant(b, ZUI_LINK);
	char last = 0;
	zui_node *group = NULL;
	for (unsigned i = 0; i < sizeof(names) / sizeof(*names); i++) {
		char c = (char)(names[i][0] | 0x20);
		if (c != last) {
			char head[2] = { (char)(c - 32), 0 };
			group = zui_section(content, head);
			letter_sections[c - 'a' + 1] = group;
			letter_live[c - 'a' + 1] = true;
			last = c;
		}
		zui_node *it = zui_item(group, names[i], NULL, NULL, NULL);
		zui_set_monogram(it, (char[]){ names[i][0], 0 });
		zui_set_hold(it, menu_for, NULL, NULL);
	}
	zui_space(content, 80);
}

static void tile_tapped(zui_node *t, void *data) { say("Opened", t); }

static void build_tiles_page(void) {
	tiles_page = titled("tiles");
	zui_node *content = zui_content_panel(tiles_page);
	zui_node *g = zui_tiles(content);
	zui_node *t = zui_tile(g, "Phone", "info", false, tile_tapped, NULL);
	zui_set_badge(t, "3");
	zui_set_hold(t, menu_for, NULL, NULL);
	t = zui_tile(g, "Messaging", "send", false, tile_tapped, NULL);
	zui_set_badge(t, "12");
	zui_tile_set_live(t, "Mum: dinner at 7?", 4000);
	zui_set_hold(t, menu_for, NULL, NULL);
	t = zui_tile(g, "Weather", "sun", true, tile_tapped, NULL);
	zui_set_tint(t, 0x1ba1e2);
	zui_tile_set_live(t, "Cloudy, 14°.  Rain after 6.", 5000);
	zui_set_hold(t, menu_for, NULL, NULL);
	t = zui_tile(g, "Music", "music", false, tile_tapped, NULL);
	zui_set_tint(t, 0xa200ff);
	zui_set_hold(t, menu_for, NULL, NULL);
	t = zui_tile(g, "People", "person", false, open_people, NULL);
	zui_set_tint(t, 0x60a917);
	t = zui_tile(g, "Wi-Fi", "wifi", false, open_wifi, NULL);
	zui_set_tint(t, 0xf0a30a);
	t = zui_tile(g, "Settings", "settings", false, tile_tapped, NULL);
	zui_set_tint(t, 0xe51400);
	tile_last = t;
	zui_caption(content, "Hold a tile for its menu. The live ones turn over now and then.");
}

static zui_pixmap *panorama(void) {
	/* a soft accent-coloured picture to move behind the sections */
	enum { W = 256, H = 160 };
	static uint8_t px[W * H * 4];
	for (int y = 0; y < H; y++)
		for (int x = 0; x < W; x++) {
			float u = (float)x / W, v = (float)y / H;
			uint8_t *p = px + (y * W + x) * 4;
			p[0] = (uint8_t)(40 + 160 * u);
			p[1] = (uint8_t)(90 + 100 * v);
			p[2] = (uint8_t)(220 - 120 * u * v);
			p[3] = 255;
		}
	return zui_pixmap_rgba(W, H, px);
}

static void build_hub_page(void) {
	zui_node *hub = zui_hub(app);
	hub_page = hub;
	zui_hub_title(hub, "hub");
	zui_pixmap *bg = panorama();
	zui_hub_set_background(hub, bg);
	zui_pixmap_free(bg);

	zui_node *s = zui_hub_section(hub);
	zui_title(s, "featured");
	zui_node *g = zui_tiles(s);
	zui_node *t = zui_tile(g, "Photos", "info", false, tile_tapped, NULL);
	zui_set_tint(t, 0xe3a21a);
	zui_tile_set_live(t, "42 new pictures", 3500);
	t = zui_tile(g, "Maps", "search", false, tile_tapped, NULL);
	zui_set_tint(t, 0x00aba9);
	t = zui_tile(g, "Store", "plus", true, tile_tapped, NULL);
	zui_set_tint(t, 0xd80073);

	s = zui_hub_section(hub);
	zui_title(s, "recent");
	zui_node *card = zui_card(s);
	zui_item(card, "Holiday album", "12 pictures", NULL, NULL);
	zui_item(card, "Receipts", "3 documents", NULL, NULL);
	zui_item(card, "Voice note", "yesterday", NULL, NULL);
	zui_item(card, "Backup", "completed", NULL, NULL);

	s = zui_hub_section(hub);
	zui_title(s, "about");
	zui_text(s, "A hub is one wide page made of sections. Swipe sideways: the next section is always showing at the edge.");
	zui_space(s, 8);
	zui_caption(s, "The title and the picture behind move more slowly than the sections.");
	zui_node *b = zui_button(s, "See all", open_people, NULL);
	zui_set_variant(b, ZUI_LINK);
}

static void build_controls_page(void) {
	controls_page = titled("controls");
	zui_node *content = zui_content_panel(controls_page);

	zui_node *c = zui_section(content, "Check and choose");
	zui_check(c, "Remember me", true, box_ticked, NULL);
	zui_check(c, "Send crash reports", false, box_ticked, NULL);
	zui_radio(c, "Small", false, radio_changed, NULL);
	zui_radio(c, "Medium", true, radio_changed, NULL);
	zui_radio(c, "Large", false, radio_changed, NULL);

	c = zui_section(content, "Text");
	zui_node *pw = zui_field(c, "Password", NULL, NULL);
	zui_field_set_password(pw, true);
	zui_set_text(pw, "hunter22");
	static const char *const cities[] = { "Moscow", "Minsk", "Madrid", "Milan", "Munich", "Montreal", "Berlin", "Bern" };
	zui_node *sg = zui_field(c, "City (suggestions)", NULL, NULL);
	zui_field_set_suggestions(sg, cities, 8);
	zui_node *l = zui_button(c, "Forgot your password?", NULL, NULL);
	zui_set_variant(l, ZUI_LINK);

	c = zui_section(content, "Pickers");
	static const char *const langs[] = { "English", "Deutsch", "Français", "Русский" };
	lang_picker = zui_picker(c, "Language", langs, 4, 0, lang_changed, NULL);
	date_row = zui_date_picker(c, "Birthday", 1990, 11, 5, date_changed, NULL);
	time_row = zui_time_picker(c, "Alarm", 7, 30, time_changed, NULL);

	c = zui_section(content, "Over the page");
	zui_node *row = zui_row(c);
	zui_node *b = zui_button(row, "Dialog", ask_dialog, NULL);
	zui_set_flex(b, 1);
	b = zui_button(row, "Menu", menu_for, NULL);
	zui_set_flex(b, 1);
	b = zui_button(row, "Jump", open_jump, NULL);
	zui_set_flex(b, 1);
	zui_node *hold = zui_item(c, "Hold me", "press and hold for a menu", NULL, NULL);
	zui_set_hold(hold, menu_for, NULL, NULL);

	c = zui_section(content, "Working");
	zui_node *ring = zui_ring(c);
	(void)ring;
	zui_space(content, 40);
}

int main(void) {
	app = zui_app_new();
	build_wifi_page();
	build_controls_page();
	build_tiles_page();
	build_hub_page();
	build_people_page();
	zui_node *page = titled("settings");
	zui_node *content = zui_content_panel(page);

	zui_node *conn = zui_section(content, "Connections");
	wifi_row = zui_item(conn, "Wi-Fi", NULL, open_wifi, NULL);
	zui_set_icon(wifi_row, "wifi");
	zui_set_detail(wifi_row, "Home-5G");
	zui_node *bt = zui_switch(conn, "Bluetooth", false, NULL, NULL);
	zui_set_icon(bt, "bluetooth");
	zui_set_tint(bt, 0x6366f1);
	zui_node *dnd = zui_switch(conn, "Do not disturb", true, NULL, NULL);
	zui_set_subtitle(dnd, "Until 7:00");
	zui_set_icon(dnd, "moon");
	zui_set_tint(dnd, 0x8b5cf6);

	zui_node *more = zui_section(content, "Widgets");
	zui_item(more, "Controls", "check boxes, pickers, dialogs", open_controls, NULL);
	zui_item(more, "Tiles", "live tiles you can hold", open_tiles, NULL);
	zui_item(more, "Hub", "sections side by side", open_hub, NULL);
	zui_item(more, "People", "a list with a jump list", open_people, NULL);

	zui_node *disp = zui_section(content, "Display and sound");
	zui_slider(disp, "Brightness", 0.62, brightness, NULL);
	zui_slider(disp, "Volume", 0.35, NULL, NULL);
	zui_node *dark = zui_switch(disp, "Dark theme", true, dark_theme, NULL);
	zui_set_icon(dark, "sun");
	zui_set_tint(dark, 0xf59e0b);

	zui_node *about = zui_section(content, "This phone");
	update_row = zui_item(about, "Software", "ztOS BETA-1", NULL, NULL);
	zui_set_icon(update_row, "info");
	zui_set_tint(update_row, 0x64748b);
	zui_set_detail(update_row, "Up to date");
	zui_node *bat = zui_item(about, "Battery", "Not charging", NULL, NULL);
	zui_set_icon(bat, "battery");
	zui_set_tint(bat, 0x22c55e);
	zui_set_detail(bat, "84%");
	zui_node *storage = zui_column(about);
	zui_caption(storage, "Storage: 41.2 GB of 64 GB used");
	zui_progress(storage, 0.64);

	zui_node *b = zui_button(content, "Check for updates", check_updates, NULL);
	zui_set_variant(b, ZUI_TONAL);
	return zui_run(app, page);
}
