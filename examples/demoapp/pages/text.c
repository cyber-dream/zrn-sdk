// SPDX-License-Identifier: MIT
#include "text.h"

#include <zrnui.h>

#include <stdint.h>
#include <stdio.h>

Zid text_page(const Zid frame, void *parameter) {
    (void) frame;
    (void) parameter;

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

        const Zid app_title_text = Ztext_block_create("TITLE");
        Ztext_block_set_style(app_title_text, Z_TEXT_STYLE_TITLE);
        Zpanel_add(header_stack, app_title_text);
        Zelement_set_margin(app_title_text, Zthickness(0, 12, 0, 0));

        const Zid app_big_title = Ztext_block_create("text box");
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

        #define ROW(style, sample) do { \
            const Zid _t = Ztext_block_create(sample); \
            Ztext_block_set_style(_t, style); \
            Zpanel_add(stack, _t); \
            Zelement_set_margin(_t, Zthickness(0, 0, 0, 8)); \
        } while (0)
                ROW(Z_TEXT_STYLE_HEADER, "Header — SemiLight 57.5");
                ROW(Z_TEXT_STYLE_SUBHEADER, "Subheader — SemiLight 30");
                ROW(Z_TEXT_STYLE_LIST_VIEW_ITEM, "List view item — SemiLight 29.86");
                ROW(Z_TEXT_STYLE_MESSAGE_DIALOG_TITLE, "Message dialog title — SemiBold 25.5");
                ROW(Z_TEXT_STYLE_GROUP_HEADER, "Group header — SemiLight 23.73");
                ROW(Z_TEXT_STYLE_COMBO_BOX_PLACEHOLDER, "Combo box placeholder — Normal 20.26");
                ROW(Z_TEXT_STYLE_TITLE, "TITLE — SemiBold 18.14");
                ROW(Z_TEXT_STYLE_BASE, "Base — Normal 18.14");
                ROW(Z_TEXT_STYLE_BODY, "Body — Normal 18.14");
                ROW(Z_TEXT_STYLE_MESSAGE_DIALOG_CONTENT, "Message dialog content — Normal 18.14");
                ROW(Z_TEXT_STYLE_LIST_VIEW_ITEM_CONTENT, "List view item content — Normal 16");
                ROW(Z_TEXT_STYLE_CONTROL_HEADER, "Control header — Normal 16");
                ROW(Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER, "List view item subheader — SemiLight 16");
        #undef ROW
    }

    return page;
}
