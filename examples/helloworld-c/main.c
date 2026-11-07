// SPDX-License-Identifier: MIT
#include <kestrel.h>

#include <stdint.h>
#include <stdio.h>

// Click on the button: Dark <-> Light. The root arrives as the user data
static void on_theme_click(const Kid sender, void* user_data) {
    (void)sender;
    const Kid root = (Kid)(uintptr_t)user_data;
    Kroot_set_theme(root, Kroot_get_theme(root) == K_THEME_DARK ? K_THEME_LIGHT : K_THEME_DARK);
}

static Kid main_page(const Kid frame, void* parameter) {
    (void)frame;
    const Kid root = (Kid)(uintptr_t)parameter;

    const Kid page = Kpage_create();

    /* Root grid of page */
    const Kid root_grid = Kgrid_create();
    Kpage_set_content(page, root_grid);
    Kgrid_set_row(root_grid, 2);
    Kgrid_set_column(root_grid, 1);

    const Kid header_stack = Kstack_panel_create(K_VERTICAL);
    Kgrid_place(root_grid, header_stack, 0,0);

    const Kid app_title_text = Ktext_block_create("DEMO APP");
    Ktext_block_set_style(app_title_text, K_TEXT_STYLE_TITLE);
    Kpanel_add(header_stack,app_title_text);

    const Kid app_big_title = Ktext_block_create("hi german!");
    Ktext_block_set_style(app_big_title, K_TEXT_STYLE_HEADER);
    Kpanel_add(header_stack,app_big_title);

    const Kid theme_button = Kbutton_create("Change theme");
    Kbutton_on_click(theme_button, on_theme_click, (void*)(uintptr_t)root);
    Kpanel_add(header_stack,theme_button);

    return page;
}

int main(void) {
    if (!Kinit()) return 1;
    // 0 x 0: the window gives the size
    const Kid root = Kroot_create(0, 0);

    // App::OnLaunched: a Frame is the content of the window, then it navigates to the first page
    const Kid frame = Kframe_create();
    Kroot_set_visual(root, frame);
    if (!Kframe_navigate(frame, main_page, (void*)(uintptr_t)root)) {
        fprintf(stderr, "helloworld: %s\n", Kget_error_message());
        Kroot_destroy(root);
        Kshutdown();
        return 1;
    }

    // the window runs until it is closed, or Back is pressed on the first page
    const int ok = Kwindow_run(root, "Hello World");
    if (!ok) fprintf(stderr, "helloworld: %s\n", Kget_error_message());
    Kroot_destroy(root);
    Kshutdown();
    return ok ? 0 : 1;
}

