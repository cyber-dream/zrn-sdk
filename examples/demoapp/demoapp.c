// SPDX-License-Identifier: MIT
#include <zrnui.h>
#include <stdint.h>
#include <stdio.h>
#include "pages/main.h"


int main(void) {
    if (!Zinit()) return 1;
    const Zid root = Zroot_create(0, 0);

    const Zid frame = Zframe_create();
    Zroot_set_visual(root, frame);
    if (!Zframe_navigate(frame, main_page, (void*)(uintptr_t)root)) {
        fprintf(stderr, "helloworld: %s\n", Zget_error_message());
        Zroot_destroy(root);
        Zshutdown();
        return 1;
    }

    const int ok = Zwindow_run(root, "Hello World");
    if (!ok) fprintf(stderr, "helloworld: %s\n", Zget_error_message());
    Zroot_destroy(root);
    Zshutdown();
    return ok ? 0 : 1;
}

