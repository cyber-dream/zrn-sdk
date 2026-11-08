// SPDX-License-Identifier: MIT
#include "text_page.h"

#include <kestrel.h>

#include <stdint.h>
#include <stdio.h>

Kid text_page(const Kid frame, void *parameter) {
    (void) frame;
    (void) parameter;

    const Kid root = (Kid) (uintptr_t) parameter;
    const Kid page = Kpage_create();

    /* Root grid of page */
    const Kid root_grid = Kgrid_create();
    Kgrid_add_row(root_grid, K_GRID_AUTO, 0); // заголовок по высоте содержимого
    Kgrid_add_row(root_grid, K_GRID_STAR, 1); // всё остальное
    Kpage_set_content(page, root_grid);
    Kgrid_set_row(root_grid, 2);
    Kgrid_set_column(root_grid, 1);

    /* Top Stack */
    {
        const Kid header_stack = Kstack_panel_create(K_VERTICAL);
        Kgrid_place(root_grid, header_stack, 0, 0);
        Kelement_set_margin(header_stack, Kthickness(19, 0, 0, 0));

        const Kid app_title_text = Ktext_block_create("TITLE");
        Ktext_block_set_style(app_title_text, K_TEXT_STYLE_TITLE);
        Kpanel_add(header_stack, app_title_text);
        Kelement_set_margin(app_title_text, Kthickness(0, 12, 0, 0));

        const Kid app_big_title = Ktext_block_create("text box");
        Ktext_block_set_style(app_big_title, K_TEXT_STYLE_HEADER);
        Kpanel_add(header_stack, app_big_title);
        Kelement_set_margin(app_big_title, Kthickness(0, 0, 0, 26.5f));
        Ktext_block_set_character_spacing(app_big_title, Kroot_get_theme_int(root, K_RES_PIVOT_HEADER_ITEM_FONT_SIZE));
    }

    /* Text boxes */
    {
        const Kid viewer = Kscroll_viewer_create();
        Kgrid_place(root_grid, viewer, 0, 1);
        const Kid stack = Kstack_panel_create(K_VERTICAL);
        Kscroll_viewer_set_content(viewer, stack);
        Kelement_set_margin(stack, Kthickness(19, 0, 19, 0));

        #define ROW(style, sample) do { \
            const Kid _t = Ktext_block_create(sample); \
            Ktext_block_set_style(_t, style); \
            Kpanel_add(stack, _t); \
            Kelement_set_margin(_t, Kthickness(0, 0, 0, 8)); \
        } while (0)
                ROW(K_TEXT_STYLE_HEADER, "Header — SemiLight 57.5");
                ROW(K_TEXT_STYLE_SUBHEADER, "Subheader — SemiLight 30");
                ROW(K_TEXT_STYLE_LIST_VIEW_ITEM, "List view item — SemiLight 29.86");
                ROW(K_TEXT_STYLE_MESSAGE_DIALOG_TITLE, "Message dialog title — SemiBold 25.5");
                ROW(K_TEXT_STYLE_GROUP_HEADER, "Group header — SemiLight 23.73");
                ROW(K_TEXT_STYLE_COMBO_BOX_PLACEHOLDER, "Combo box placeholder — Normal 20.26");
                ROW(K_TEXT_STYLE_TITLE, "TITLE — SemiBold 18.14");
                ROW(K_TEXT_STYLE_BASE, "Base — Normal 18.14");
                ROW(K_TEXT_STYLE_BODY, "Body — Normal 18.14");
                ROW(K_TEXT_STYLE_MESSAGE_DIALOG_CONTENT, "Message dialog content — Normal 18.14");
                ROW(K_TEXT_STYLE_LIST_VIEW_ITEM_CONTENT, "List view item content — Normal 16");
                ROW(K_TEXT_STYLE_CONTROL_HEADER, "Control header — Normal 16");
                ROW(K_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER, "List view item subheader — SemiLight 16");
        #undef ROW
    }

    return page;
}
