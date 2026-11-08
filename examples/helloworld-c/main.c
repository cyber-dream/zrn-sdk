// SPDX-License-Identifier: MIT
#include <kestrel.h>
#include "text_page.h"

#include <stdint.h>
#include <stdio.h>

// Click on the button: Dark <-> Light. The root arrives as the user data
static void on_theme_click(const Kid sender, void* user_data) {
    (void)sender;
    const Kid root = (Kid)(uintptr_t)user_data;
    Kroot_set_theme(root, Kroot_get_theme(root) == K_THEME_DARK ? K_THEME_LIGHT : K_THEME_DARK);
}

static void on_list_tap(Kid sender, int index, void* user_data) {
    (void)sender;

    const Kid root = (Kid)(uintptr_t)user_data;
    const Kid frame = Kroot_get_visual(root);

    switch (index) {
        case 0:
            Kframe_navigate(frame, text_page, (void*)(uintptr_t)root);
            break;
    default:break;
    }
}

static void on_open_text(Kid sender, void* user_data) {
    (void)sender;
    const Kid frame = (Kid)(uintptr_t)user_data;
    Kframe_navigate(frame, text_page, user_data);
}

static Kid main_page(const Kid frame, void* parameter) {
    (void)frame;
    const Kid root = (Kid)(uintptr_t)parameter;

    const Kid page = Kpage_create();

    /* Root grid of page */
    const Kid root_grid = Kgrid_create();
    Kgrid_add_row(root_grid, K_GRID_AUTO, 0);   // заголовок по высоте содержимого
    Kgrid_add_row(root_grid, K_GRID_STAR, 1);   // всё остальное
    Kpage_set_content(page, root_grid);
    Kgrid_set_row(root_grid, 2);
    Kgrid_set_column(root_grid, 1);

    /* Top Stack */
    {
        const Kid header_stack = Kstack_panel_create(K_VERTICAL);
        Kgrid_place(root_grid, header_stack, 0,0);
        Kelement_set_margin(header_stack, Kthickness(19,0,0,0));

        const Kid app_title_text = Ktext_block_create("SETTINGS");
        Ktext_block_set_style(app_title_text, K_TEXT_STYLE_TITLE);
        Kpanel_add(header_stack,app_title_text);
        Kelement_set_margin(app_title_text, Kthickness(0,12,0,0));

        const Kid app_big_title = Ktext_block_create("brightness");
        Ktext_block_set_style(app_big_title, K_TEXT_STYLE_HEADER);
        Kpanel_add(header_stack,app_big_title);
        Kelement_set_margin(app_big_title, Kthickness(0,0,0,26.5f));
        Ktext_block_set_character_spacing(app_big_title, Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
    }

    /* List View */
    {
        const Kid scroll_viewer = Kscroll_viewer_create();
        Kelement_set_margin(scroll_viewer, Kthickness(19,0,0,0));
        Kgrid_place(root_grid, scroll_viewer, 0, 1);

        {
            const Kid list = Klist_view_create();
            Klist_view_set_is_item_click_enabled(list, K_TRUE);
            Kcontent_control_set_content(scroll_viewer, list);

            /* Text */
            {
                const Kid item = Klist_view_item_create("");
                Klist_view_on_item_click(list, on_list_tap, (void*)(uintptr_t)root);
                const Kid stack = Kstack_panel_create(K_VERTICAL);
                Kcontent_control_set_content(item, stack);

                const Kid title = Ktext_block_create("text box");
                Ktext_block_set_style(title, K_TEXT_STYLE_SUBHEADER);
                Kpanel_add(stack, title);

                const Kid text = Ktext_block_create("all about text widget");
                Ktext_block_set_style(text, K_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
                Kpanel_add(stack, text);

                Kitems_control_add(list, item);
            }
        }
    }

    return page;
}

int main(void) {
    if (!Kinit()) return 1;
    // 0 x 0: the size comes from the shell (KESTREL_WINDOW or the compositor configure)
    const Kid root = Kroot_create(0, 0);
    // Kroot_set_debug(root, K_DEBUG_BOUNDS);  /* рамки */
    // Kroot_set_debug(root, K_DEBUG_LABELS);  /* рамки + имена классов */

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

