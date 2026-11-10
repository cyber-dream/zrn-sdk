#include <stdint.h>
#include <stdio.h>
#include "zrnui.h"

/* The first page: a title and a line of text. */
static Zid main_page(const Zid frame, void* const parameter) {
    (void)frame;
    (void)parameter;
    const Zid page = Zpage_create();
    const Zid stack = Zstack_panel_create(Z_VERTICAL);
    Zpage_set_content(page, stack);
    Zelement_set_margin(stack, Zthickness(19, 32, 19, 0));

    const Zid title = Ztext_block_create("HELLO WORLD");
    Ztext_block_set_style(title, Z_TEXT_STYLE_TITLE);
    Zpanel_add(stack, title);

    const Zid text = Ztext_block_create("zrnui in a few lines of C");
    Ztext_block_set_style(text, Z_TEXT_STYLE_PHONE_TITLE2);
    Zpanel_add(stack, text);
    return page;
}

int main(void) {
    if (!Zinit()) return 1;
    // the size comes from the shell (the compositor configure)
    const Zid root = Zroot_create();

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
