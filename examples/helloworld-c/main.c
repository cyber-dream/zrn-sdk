// SPDX-License-Identifier: MIT
#include <zrnui.h>

int main(void) {
    zui_app *app = zui_app_new();
    zui_node* pivot = zui_pivot(app);

    return zui_run(app, pivot);
}
