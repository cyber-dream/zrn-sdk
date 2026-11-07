fn main() {
    let app = zrnui::App::new();
    let page = app.page();
    let tp = page.title_panel();
    tp.title("{{NAME}}");
    tp.header("home");;
    let content = page.content_panel();
    content.text("A new ztOS app. Edit src/main.rs, then run `zrn build && zrn render out.png`.");
    let status = content.section("Status").heading("Not tapped yet");
    let app_ref = &app;
    content.button("Tap me").on_click(move || {
        status.set_text("Tapped");
        app_ref.toast("Hello from {{NAME}}");
    });
    std::process::exit(app.run(page));
}
