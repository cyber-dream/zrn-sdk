// SPDX-License-Identifier: MIT
#include "hub.h"

#include "button.h"

#include <zrnui.h>

Zid page_hub(const Zid frame, void *parameter) {
    (void) frame;
    (void) parameter;

    const Zid root = (Zid) (uintptr_t) parameter;
    const Zid page = Zpage_create();

    const Zid hub = Zhub_create();
    Zhub_set_header(hub, "my app");
    Zpage_set_content(page, hub);

    /* Section 1 */
    {
        const Zid section = Zhub_section_create("SECTION 1");
        Zhub_section_set_is_header_interactive(section, Z_TRUE);

        const Zid viewer = Zscroll_viewer_create();
        const Zid stack = Zstack_panel_create(Z_VERTICAL);
        Zscroll_viewer_set_content(viewer, stack);
        Zelement_set_margin(stack, Zthickness(19, 9.5f, 19, 0));

        const Zid title = Ztext_block_create("normal button");
        Ztext_block_set_style(title, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
        Zpanel_add(stack, title);

        const Zid button = Zbutton_create("button");
        Zpanel_add(stack, button);

        Zcontent_control_set_content(section, viewer);
        Zhub_add_section(hub, section);
    }

    /* Section 2 */
    {
        const Zid section = Zhub_section_create("SECTION 2");
        Zhub_section_set_is_header_interactive(section, Z_TRUE);

        const Zid viewer = Zscroll_viewer_create();
        const Zid stack = Zstack_panel_create(Z_VERTICAL);
        Zscroll_viewer_set_content(viewer, stack);
        Zelement_set_margin(stack, Zthickness(19, 9.5f, 19, 0));

        const Zid title = Ztext_block_create("repeat button");
        Ztext_block_set_style(title, Z_TEXT_STYLE_LIST_VIEW_ITEM_SUBHEADER);
        Zpanel_add(stack, title);

        const Zid button = Zrepeat_button_create("hold me");
        Zbutton_on_click(button, on_repeat_button_pressed, NULL);
        Zpanel_add(stack, button);

        Zcontent_control_set_content(section, viewer);
        Zhub_add_section(hub, section);
    }

    return page;
}