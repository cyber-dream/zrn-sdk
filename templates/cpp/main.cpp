#include <zrnui.hpp>

int main() {
	zrn::ui::App app;
	auto page = app.page();
	{ auto tp = page.title_panel(); tp.title("{{NAME}}"); tp.header("home"); };
	auto content = page.content_panel();
	content.text("A new ztOS app. Edit main.cpp, then run `zrn build && zrn render out.png`.");
	auto status = content.section("Status").heading("Not tapped yet");
	content.button("Tap me", [&]() {
		status.set_text("Tapped");
		app.toast("Hello from {{NAME}}");
	});
	return app.run(page);
}
