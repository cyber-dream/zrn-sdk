#include <stdio.h>
#include <zrnui.h>

static zui_node *status;

static void tapped(zui_node *button, void *data) {
	zui_set_text(status, "Tapped");
	zui_toast(zui_node_app(button), "Hello from {{NAME}}");
}

int main(void) {
	zui_app *app = zui_app_new();
	zui_node *page = zui_page(app);
	zui_node *title = zui_title_panel(page);
	zui_title(title, "{{NAME}}");
	zui_header(title, "home");
	zui_node *content = zui_content_panel(page);
	zui_text(content, "A new ztOS app. Edit main.c, then run `zrn build && zrn render out.png`.");
	zui_node *card = zui_section(content, "Status");
	status = zui_heading(card, "Not tapped yet");
	zui_button(content, "Tap me", tapped, NULL);
	return zui_run(app, page);
}
