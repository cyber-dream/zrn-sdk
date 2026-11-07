# SPDX-License-Identifier: MIT
"""Hello in Python: the same library as C, no Qt."""
import zrnui

app = zrnui.App()
page = app.page()
tp = page.title_panel()
tp.title("HELLO")
tp.header("python")
content = page.content_panel()
content.text("This page is drawn by libzrnui on the GPU; Python only says what is on it.")
card = content.section("Counter")
label = card.heading("Not tapped yet")
taps = 0


def tapped():
    global taps
    taps += 1
    label.set_text(f"Tapped {taps} time" + ("s" if taps > 1 else ""))


content.button("Tap me", tapped)
name = content.section("Your name").field("Type a name", on_submit=lambda t: app.toast(f"Hello, {t}"))
app.run(page)
