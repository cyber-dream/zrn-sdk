#include "button.h"

void on_repeat_button_pressed(Zid sender, void* user_data) {
    (void)sender; (void)user_data;


}

Zid button_page(const Zid frame, void *parameter) {
    (void) frame; (void) parameter;

    const Zid root = (Zid) (uintptr_t) parameter;
    const Zid page = Zpage_create();

    /* Root grid of page */
    const Zid root_grid = Zgrid_create();
    Zgrid_add_row(root_grid, Z_GRID_AUTO, 0); // заголовок по высоте содержимого
    Zgrid_add_row(root_grid, Z_GRID_STAR, 1); // всё остальное
    Zpage_set_content(page, root_grid);
    Zgrid_set_row(root_grid, 2);
    Zgrid_set_column(root_grid, 1);

    /* Top Stack */
    {
        const Zid header_stack = Zstack_panel_create(Z_VERTICAL);
        Zgrid_place(root_grid, header_stack, 0, 0);
        Zelement_set_margin(header_stack, Zthickness(19, 0, 0, 0));

        const Zid app_title_text = Ztext_block_create("DEMO APP");
        Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_TITLE);
        Zpanel_add(header_stack, app_title_text);
        Zelement_set_margin(app_title_text, Zthickness(0, 12, 0, 0));

        const Zid app_big_title = Ztext_block_create("push button");
        Ztext_block_set_style(app_big_title, Z_TEXT_STYLE_HEADER);
        Zpanel_add(header_stack, app_big_title);
        Zelement_set_margin(app_big_title, Zthickness(0, 0, 0, 26.5f));
        Ztext_block_set_character_spacing(app_big_title, Zroot_get_theme_int(root, Z_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
    }

    /* Text boxes */
    {
        const Zid viewer = Zscroll_viewer_create();
        Zgrid_place(root_grid, viewer, 0, 1);

        const Zid stack = Zstack_panel_create(Z_VERTICAL);
        Zscroll_viewer_set_content(viewer, stack);
        Zelement_set_margin(stack, Zthickness(19, 0, 19, 0));

        /* Normal push button */
        {
            const Zid stack_button = Zstack_panel_create(Z_VERTICAL);
            Zpanel_add(stack, stack_button);

            const Zid app_title_text = Ztext_block_create("normal button");
            Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
            Zpanel_add(stack_button,app_title_text);

            const Zid button = Zbutton_create("button");
            Zpanel_add(stack_button, button);
        }

        /* Disable button TODO */
        {
            const Zid stack_button = Zstack_panel_create(Z_VERTICAL);
            Zpanel_add(stack, stack_button);

            const Zid app_title_text = Ztext_block_create("disabled button");
            Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
            Zpanel_add(stack_button,app_title_text);

            const Zid button = Zbutton_create("button");
            Zpanel_add(stack_button, button);
        }

        /* Repeat button */
        {
            const Zid stack_button = Zstack_panel_create(Z_VERTICAL);
            Zpanel_add(stack, stack_button);

            const Zid app_title_text = Ztext_block_create("repeated button");
            Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
            Zpanel_add(stack_button,app_title_text);

            const Zid button = Zrepeat_button_create("hold me");
            Zbutton_on_click(button, on_repeat_button_pressed, NULL);
            Zpanel_add(stack_button, button);
        }
    }

    return page;
}
