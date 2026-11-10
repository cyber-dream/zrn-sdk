#include <stdint.h>

#include "pivot.h"
#include "text.h"
#include "button.h"
#include "hub.h"

// Click on the button: Dark <-> Light. The root arrives as the user data
static void on_theme_click(const Zid sender, void* user_data) {
    (void)sender;
    const Zid root = (Zid)(uintptr_t)user_data;
    Zroot_set_theme(root, Zroot_get_theme(root) == Z_THEME_DARK ? Z_THEME_LIGHT : Z_THEME_DARK);
}

static void on_list_tap(const Zid sender, const int list_item_idx, void* user_data) {
    (void)sender;

    const Zid root = (Zid)(uintptr_t)user_data;
    const Zid frame = Zroot_get_visual(root);

    switch (list_item_idx) {
        case 0:
            Zframe_navigate(frame, text_page, (void*)(uintptr_t)root);
            break;
        case 1:
            Zframe_navigate(frame, button_page, (void*)(uintptr_t)root);
            break;
        case 2:
            Zframe_navigate(frame, page_hub, (void*)(uintptr_t)root);
            break;
        case 3:
            Zframe_navigate(frame, page_pivot, (void*)(uintptr_t)root);
            break;
        default:
            break;
    }
}

static void on_open_text(const Zid sender, void* user_data) {
    (void)sender;
    const Zid frame = (Zid)(uintptr_t)user_data;
    Zframe_navigate(frame, text_page, user_data);
}

static void on_open_pivot(const Zid sender, void* user_data) {
    (void)sender;
    const Zid frame = (Zid)(uintptr_t)user_data;
    Zframe_navigate(frame, page_pivot, user_data);
}

Zid main_page(const Zid frame, void* parameter) {
    (void)frame;
    const Zid root = (Zid)(uintptr_t)parameter;

    const Zid page = Zpage_create();

    /* Root grid of page */
    const Zid root_grid = Zgrid_create();
    Zgrid_add_row(root_grid, Z_GRID_AUTO, 0);   // заголовок по высоте содержимого
    Zgrid_add_row(root_grid, Z_GRID_STAR, 1);   // всё остальное
    Zpage_set_content(page, root_grid);
    Zgrid_set_row(root_grid, 2);
    Zgrid_set_column(root_grid, 1);

    /* Top Stack */
    {
        const Zid header_stack = Zstack_panel_create(Z_VERTICAL);
        Zgrid_place(root_grid, header_stack, 0,0);
        Zelement_set_margin(header_stack, Zthickness(19,0,0,0));

        const Zid app_title_text = Ztext_block_create("DEMO APP");
        Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_TITLE);
        Zpanel_add(header_stack,app_title_text);
        Zelement_set_margin(app_title_text, Zthickness(0,12,0,0));

        const Zid app_big_title = Ztext_block_create("widgets");
        Ztext_block_set_style(app_big_title, Z_TEXT_STYLE_HEADER);
        Zpanel_add(header_stack,app_big_title);
        Zelement_set_margin(app_big_title, Zthickness(0,0,0,26.5f));
        Ztext_block_set_character_spacing(app_big_title, Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
    }

    /* List View */
    {
        const Zid scroll_viewer = Zscroll_viewer_create();
        Zelement_set_margin(scroll_viewer, Zthickness(19,0,0,0));
        Zgrid_place(root_grid, scroll_viewer, 0, 1);

        {
            const Zid list = Zlist_view_create();
            Zlist_view_set_is_item_click_enabled(list, true);
            Zcontent_control_set_content(scroll_viewer, list);

            /* Text */
            {
                const Zid item = Zlist_view_item_create("");
                Zlist_view_on_item_click(list, on_list_tap, (void*)(uintptr_t)root);
                const Zid stack = Zstack_panel_create(Z_VERTICAL);
                Zcontent_control_set_content(item, stack);

                const Zid title = Ztext_block_create("text box");
                Ztext_block_set_style(title, Z_TEXT_STYLE_SUBHEADER);
                Zpanel_add(stack, title);

                const Zid text = Ztext_block_create("all about text widget");
                Ztext_block_set_style(text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
                Zpanel_add(stack, text);

                Zitems_control_add(list, item);
            }

            /* Buttons */
            {
                const Zid item = Zlist_view_item_create("");
                Zlist_view_on_item_click(list, on_list_tap, (void*)(uintptr_t)root);
                const Zid stack = Zstack_panel_create(Z_VERTICAL);
                Zcontent_control_set_content(item, stack);

                const Zid title = Ztext_block_create("buttons");
                Ztext_block_set_style(title, Z_TEXT_STYLE_SUBHEADER);
                Zpanel_add(stack, title);

                const Zid text = Ztext_block_create("all about buttons");
                Ztext_block_set_style(text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
                Zpanel_add(stack, text);

                Zitems_control_add(list, item);
            }

            /* Hub */
            {
                const Zid item = Zlist_view_item_create("");
                Zlist_view_on_item_click(list, on_list_tap, (void*)(uintptr_t)root);
                const Zid stack = Zstack_panel_create(Z_VERTICAL);
                Zcontent_control_set_content(item, stack);

                const Zid title = Ztext_block_create("hub");
                Ztext_block_set_style(title, Z_TEXT_STYLE_SUBHEADER);
                Zpanel_add(stack, title);

                const Zid text = Ztext_block_create("space for exploring");
                Ztext_block_set_style(text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
                Zpanel_add(stack, text);

                Zitems_control_add(list, item);
            }

            /* Pivot */
            {
                const Zid item = Zlist_view_item_create("");
                Zlist_view_on_item_click(list, on_list_tap, (void*)(uintptr_t)root);
                const Zid stack = Zstack_panel_create(Z_VERTICAL);
                Zcontent_control_set_content(item, stack);

                const Zid title = Ztext_block_create("pivot");
                Ztext_block_set_style(title, Z_TEXT_STYLE_SUBHEADER);
                Zpanel_add(stack, title);

                const Zid text = Ztext_block_create("tabbed pages");
                Ztext_block_set_style(text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
                Zpanel_add(stack, text);

                Zitems_control_add(list, item);
            }
        }

    return page;
}
}