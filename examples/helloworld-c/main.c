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

    /* Main Grid */
    {
        const Kid main_stack = Kstack_panel_create(K_VERTICAL);
        Kgrid_place(root_grid, main_stack, 0, 1);
        Kelement_set_margin(main_stack, Kthickness(19,9.5f,19,0));

        /* Auto brightness */
        {
            const Kid auto_brightness_stack = Kstack_panel_create(K_VERTICAL);
            Kpanel_add(main_stack,auto_brightness_stack );

            /* the caption is the switch's own Header (WP8.1), not a loose TextBlock */
            const Kid toggle = Ktoggle_switch_create();
            Ktoggle_switch_set_header(toggle, "Automatically adjust");
            Kpanel_add(auto_brightness_stack, toggle);
        }

        {
            const Kid level_stack = Kstack_panel_create(K_VERTICAL);
            Kpanel_add(main_stack, level_stack);

            const Kid title = Ktext_block_create("Level");
            Ktext_block_set_style(title, K_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
            Kpanel_add(level_stack, title);

            const Kid combo = Kcombo_box_create();
            Kcombo_box_set_placeholder_text(combo, "");
            Kpanel_add(level_stack, combo);
            Kitems_control_add_text(combo, "low");
            Kitems_control_add_text(combo, "medium");
            Kitems_control_add_text(combo, "high");
        }
    }

    return page;
}

int main(void) {
    if (!Kinit()) return 1;
    // 0 x 0: the size comes from the shell (KESTREL_WINDOW or the compositor configure)
    const Kid root = Kroot_create(0, 0);
    Kroot_set_debug(root, K_DEBUG_BOUNDS);  /* рамки */
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

