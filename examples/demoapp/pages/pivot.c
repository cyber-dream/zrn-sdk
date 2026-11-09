// SPDX-License-Identifier: MIT
#include "pivot.h"

#include <zrnui.h>

Zid page_pivot(const Zid frame, void *parameter) {
    (void) frame;
    (void) parameter;

    const Zid page = Zpage_create();
    const Zid pivot = Zpivot_create();
    Zpivot_set_title(pivot, "SETTINGS");
    Zpage_set_content(page, pivot);

    /* item 1 */
    {
        const Zid item = Zpivot_item_create("brightness");
        const Zid viewer = Zscroll_viewer_create();
        const Zid stack = Zstack_panel_create(Z_VERTICAL);
        Zscroll_viewer_set_content(viewer, stack);
        Zelement_set_margin(stack, Zthickness(19, 9.5f, 19, 0));

        const Zid toggle = Ztoggle_switch_create();
        Ztoggle_switch_set_header(toggle, "Automatically adjust");
        Zpanel_add(stack, toggle);

        const Zid combo = Zcombo_box_create();
        Zcombo_box_set_header(combo, "Level");
        Zitems_control_add_text(combo, "low");
        Zitems_control_add_text(combo, "medium");
        Zitems_control_add_text(combo, "high");
        Zpanel_add(stack, combo);

        Zcontent_control_set_content(item, viewer);
        Zpivot_add_item(pivot, item);
    }

    /* item 2 */
    {
        const Zid item = Zpivot_item_create("theme");
        const Zid viewer = Zscroll_viewer_create();
        const Zid stack = Zstack_panel_create(Z_VERTICAL);
        Zscroll_viewer_set_content(viewer, stack);
        Zelement_set_margin(stack, Zthickness(19, 9.5f, 19, 0));

        const Zid cb = Zcheck_box_create("Dark theme");
        Zpanel_add(stack, cb);

        Zcontent_control_set_content(item, viewer);
        Zpivot_add_item(pivot, item);
    }

    return page;
}