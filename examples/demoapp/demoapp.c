// SPDX-License-Identifier: MIT
#include <zrnui.h>
#include <stdint.h>
#include <stdio.h>
#include "pages/main.h"


int main() {
    /* TODO describe an error */
    if (!Zinit())
        return 1;

    const Zid root = Zroot_create();

    const Zid frame = Zframe_create();
    Zroot_set_visual(root, frame);
    if (!Zframe_navigate(frame, main_page, (void*)(uintptr_t)root)) {
        fprintf(stderr, "error: %s\n", Zget_error_message());
        Zroot_destroy(root);
        Zshutdown();
        return 1;
    }

    const int ok = Zwindow_run(root, "Hello World");
    if (!ok) fprintf(stderr, "error: %s\n", Zget_error_message());
    Zroot_destroy(root);
    Zshutdown();
    return ok ? 0 : 1;
}

