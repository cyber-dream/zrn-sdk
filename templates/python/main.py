import zrnui

app = zrnui.App()
page = app.page()
tp = page.title_panel()
tp.title("{{NAME}}")
tp.header("home")
content = page.content_panel()
content.text("A new ztOS app. Edit main.py, then run `zrn render out.png`.")
status = content.section("Status").heading("Not tapped yet")


def tapped():
    status.set_text("Tapped")
    app.toast("Hello from {{NAME}}")


content.button("Tap me", tapped)
app.run(page)
