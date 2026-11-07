// SPDX-License-Identifier: MIT
#include <zrnui.h>

static void on_tap(zui_node *button, void *data) {
    zui_node *title = data;
    zui_set_text(title, "CLICKED");              // новый текст, регистр как напишете
}

int main(void) {
    zui_app *app = zui_app_new();
    // zui_node *page = zui_hub(app);
    // zui_hub_title(hub, "Hub Test"); // крупный заголовок хаба: свой у хаба
    //
    // zui_node *first = zui_hub_section(hub); // секция: колонка
    // zui_node *title = zui_title(first, "FIRST"); // её заголовок: мелкий title, первым
    // zui_text(first, "Omg it's a first section");
    // zui_button(first, "Press me", on_tap, title);
    //
    // zui_node *second = zui_hub_section(hub);
    // zui_title(second, "SECOND");
    // zui_text(second, "Secooond");
    //
    // zui_node *third = zui_hub_section(hub);
    // zui_title(third, "THIRD");
    // zui_text(third, "Text text text");
    // zui_check(third, "Check text", true, NULL,NULL);

    zui_node *pivot = zui_pivot(app);
    zui_pivot_title(pivot, "HELLO WORLD");       // верхний заголовок пивота: один на весь пивот

    zui_node *one = zui_pivot_item(pivot);       // айтем пивота: колонка
    zui_pivot_item_header(one, "first");                    // его заголовок (PivotItem.Header)
    zui_text(one, "Omg it's a first item");
    zui_button(one, "Press me", on_tap, NULL);

    zui_node *two = zui_pivot_item(pivot);
    zui_pivot_item_header(two, "second");
    zui_text(two, "Secooond");

    zui_node *three = zui_pivot_item(pivot);
    zui_pivot_item_header(three, "third");
    zui_text(three, "Text text text");
    zui_check(three, "Check text", true, NULL, NULL);

    return zui_run(app, pivot);
}
