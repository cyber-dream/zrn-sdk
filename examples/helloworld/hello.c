#include <stdio.h>
#include "zrnui.h"
#include ""
int main(void) {
    if (!Zinit()) return 1;
    // 0 x 0: the size comes from the shell (ZRNUI_WINDOW or the compositor configure)
    const Zid root = Zroot_create(0, 0);
    // Zroot_set_debug(root, K_DEBUG_BOUNDS);  /* рамки */
    // Zroot_set_debug(root, K_DEBUG_LABELS);  /* рамки + имена классов */

    // App::OnLaunched: a Frame is the content of the window, then it navigates to the first page
    const Zid frame = Zframe_create();
    Zroot_set_visual(root, frame);
    if (!Zframe_navigate(frame, main_page, (void*)(uintptr_t)root)) {
        fprintf(stderr, "helloworld: %s\n", Zget_error_message());
        Zroot_destroy(root);
        Zshutdown();
        return 1;
    }

    // the window runs until it is closed, or Back is pressed on the first page
    const int ok = Zwindow_run(root, "Hello World");
    if (!ok) fprintf(stderr, "helloworld: %s\n", Zget_error_message());
    Zroot_destroy(root);
    Zshutdown();
    return ok ? 0 : 1;
}