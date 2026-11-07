// SPDX-License-Identifier: MIT
/* Hello: a page, a line of text and a button that counts. */
#include <stdio.h>
#include <zrnui.h>

static zui_node *count_label;
static int taps;

static void tapped(zui_node *button, void *data) {
	char line[64];
	snprintf(line, sizeof(line), taps++ ? "Tapped %d times" : "Tapped once", taps);
	zui_set_text(count_label, line);
}

int main(void) {
	zui_app *app = zui_app_new();
	zui_node *page = zui_page(app);
	zui_node *title = zui_title_panel(page);
	zui_title(title, "HELLO");
	zui_header(title, "hello");
	zui_node *content = zui_content_panel(page);
	zui_text(content, "This is a ztOS app written in C with libzrnui. "
		"Everything on this page is drawn by the GPU.");
	zui_node *card = zui_card(content);
	count_label = zui_heading(card, "Not tapped yet");
	zui_caption(card, "The button below changes this line.");
	zui_button(content, "Tap me", tapped, NULL);
	return zui_run(app, page);
}
