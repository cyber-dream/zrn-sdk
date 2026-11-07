// SPDX-License-Identifier: MIT
// Hello in C++: the same library as C, with lambdas.
#include <string>
#include <zrnui.hpp>

int main() {
	zrn::ui::App app;
	auto page = app.page();
	{ auto tp = page.title_panel(); tp.title("HELLO"); tp.header("c++"); };
	auto content = page.content_panel();
	content.text("Callbacks are lambdas; the App frees everything when it goes.");
	auto card = content.card();
	auto label = card.heading("Not tapped yet");
	auto toggle = card.toggle("Count by two", false);
	toggle.set_icon("plus");
	int taps = 0;
	content.button("Tap me", [&]() {
		taps += toggle.is_on() ? 2 : 1;
		label.set_text("Tapped " + std::to_string(taps) + (taps == 1 ? " time" : " times"));
	});
	content.button("Reset", [&]() {
		taps = 0;
		label.set_text("Not tapped yet");
		app.toast("Counter reset");
	}).set_variant(zrn::ui::Variant::Plain);
	return app.run(page);
}
