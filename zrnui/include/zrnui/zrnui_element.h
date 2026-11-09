/**
 * zrnui
 * zrnui_element.h : UIElement, FrameworkElement, and the elements that
 * are not controls: panels, Border, Viewbox, TextBlock, Image, Popup.
 *
 * The hierarchy, as in System.Windows:
 *
 *   Element (UIElement + FrameworkElement)
 *     Panel                 StackPanel (VirtualizingStackPanel, ItemsStackPanel), WrapPanel,
 *                           ItemsWrapGrid, Grid, Canvas
 *     Border, Viewbox, TextBlock, Image, Popup
 *     Control               zrnui_controls.h
 *
 * Layout is the two passes of Silverlight. Measure asks each element how
 * much room it wants given the room on offer (bottom-up); Arrange then
 * gives each one its final rectangle (top-down). Margin, Width, Height,
 * MinWidth..MaxHeight and the alignments are common to every element.
 * Zroot_update_layout runs the passes; rendering and input run them when
 * something changed, so a program rarely calls it.
 *
 * Attached properties (Grid.Row, Canvas.Left, Panel.ZIndex) are stored on
 * the child, with Zgrid_set_row(child, ...), and ignored when the parent
 * is not that kind of panel.
 *
 * Ownership: a parent owns its children and destroys them with itself.
 * An element with no parent is owned by the caller until it is added to a
 * panel, set as content or set as the visual of a root.
 */

#ifndef ZRNUI_ELEMENT_H
#define ZRNUI_ELEMENT_H

#include "zrnui_core.h"
#include "zrnui_resources.h"


/** Routed events that carry no data: read the new state from the sender. */
typedef enum ZEvent {
	Z_EVENT_CLICK = 0x0400,
	Z_EVENT_CHECKED,
	Z_EVENT_UNCHECKED,
	Z_EVENT_INDETERMINATE,
	Z_EVENT_TEXT_CHANGED,
	Z_EVENT_SELECTION_CHANGED,
	Z_EVENT_DROP_DOWN_OPENED,
	Z_EVENT_DROP_DOWN_CLOSED,
	Z_EVENT_GOT_FOCUS,
	Z_EVENT_LOST_FOCUS,
	Z_EVENT_LOADED,
	Z_EVENT_POPUP_OPENED,
	Z_EVENT_POPUP_CLOSED,
	Z_EVENT_NAVIGATED,              /* Frame: a page is shown */
	Z_EVENT_NAVIGATED_TO,           /* Page: the frame has shown it */
	Z_EVENT_NAVIGATED_FROM,         /* Page: the frame is about to leave it */
	Z_EVENT_TOGGLED,                /* ToggleSwitch: IsOn changed */
	Z_EVENT_OPENING,                /* FlyoutBase: about to show */
	Z_EVENT_OPENED,                 /* AppBar, FlyoutBase, ContentDialog: now open */
	Z_EVENT_CLOSED,                 /* AppBar, FlyoutBase, ContentDialog: now closed */
	Z_EVENT_PRIMARY_BUTTON_CLICK,   /* ContentDialog */
	Z_EVENT_SECONDARY_BUTTON_CLICK, /* ContentDialog */
	Z_EVENT_SECTIONS_IN_VIEW_CHANGED, /* Hub: the sections in view are others now */
	Z_EVENT_LAST_
} ZEvent;

typedef void (*ZRoutedCallback)(Zid sender, void* user_data);

/** Easing of an animation: the curve from 0 to 1 over its duration. */
typedef enum ZEasing {
	Z_EASE_LINEAR = 0,
	Z_EASE_OUT,          /* cubic: quick start, soft end */
	Z_EASE_IN,           /* cubic: slow start */
	Z_EASE_EMPHASIZED    /* (0.2, 0.8, 0.2, 1) as a cubic Bezier: a quick start and a long soft settle */
} ZEasing;

/** What Zelement_animate can move. Rotations are degrees. */
typedef enum ZAnimatedProperty {
	Z_PROP_OPACITY = 0,
	Z_PROP_ROTATION_X,
	Z_PROP_ROTATION_Y,
	Z_PROP_SCALE,        /* ScaleTransform about the centre of rotation */
	Z_PROP_TRANSLATE_X,
	Z_PROP_TRANSLATE_Y,
	Z_PROP_LAST_
} ZAnimatedProperty;


#ifdef __cplusplus
extern "C" {
#endif

	/* ============================================================================
	 *  CREATION & VISUAL TREE
	 * ============================================================================ */

	/**
	 * @brief Creates an element of a class. Shortcuts per class exist
	 * (Zbutton_create...), this one serves code that picks the class at
	 * run time. Abstract classes (Z_CLASS_PANEL...) are refused.
	 */
	ZRNUI_API Zid Zelement_create(ZClass element_class);

	/** @brief Destroys an element and every element under it. A root's visual is detached first. */
	ZRNUI_API void Zelement_destroy(Zid element);
	ZRNUI_API int Zelement_is_valid(Zid element);

	/** @return The most derived class of the element, 0 for an invalid handle. */
	ZRNUI_API ZClass Zelement_get_class(Zid element);
	/** @return Z_TRUE when the element is of `element_class` or derives from it (a CheckBox is a ToggleButton, a ButtonBase, a ContentControl, a Control...). */
	ZRNUI_API int Zelement_is_a(Zid element, ZClass element_class);
	ZRNUI_API const char* Zelement_get_class_name(Zid element);

	/** @brief FrameworkElement.Name. FindName searches the subtree. */
	ZRNUI_API void Zelement_set_name(Zid element, const char* name);
	ZRNUI_API const char* Zelement_get_name(Zid element);
	ZRNUI_API Zid Zelement_find_name(Zid scope, const char* name);

	/** @brief FrameworkElement.Tag: a pointer of the program, never touched. */
	ZRNUI_API void Zelement_set_tag(Zid element, void* tag);
	ZRNUI_API void* Zelement_get_tag(Zid element);

	/**
	 * The visual tree (VisualTreeHelper). A Button has one child, its
	 * content; a ListBox has its scroll viewer, which has the item panel.
	 * To change the contents of a panel use Zpanel_*, to change the content
	 * of a control Zcontent_*, to change the items Zitems_control_*.
	 */
	ZRNUI_API Zid Zelement_get_parent(Zid element);
	ZRNUI_API int Zelement_get_child_count(Zid element);
	ZRNUI_API Zid Zelement_get_child(Zid element, int index);
	/** @return The root showing the element, or Z_INVALID_ID. */
	ZRNUI_API Zid Zelement_get_root(Zid element);

	/** @brief Transform of a point of the element to the root, and back (TransformToVisual). */
	ZRNUI_API ZPoint Zelement_to_root(Zid element, ZPoint point);
	ZRNUI_API ZPoint Zelement_from_root(Zid element, ZPoint point);

	/** @brief The element under a point given in the coordinates of `scope`, or Z_INVALID_ID. */
	ZRNUI_API Zid Zelement_hit_test(Zid scope, ZPoint point);


	/* ============================================================================
	 *  FRAMEWORKELEMENT: LAYOUT PROPERTIES
	 * ============================================================================ */

	/** @brief Width and Height; Z_AUTO sizes to the content (the default). */
	ZRNUI_API void Zelement_set_width(Zid element, float width);
	ZRNUI_API void Zelement_set_height(Zid element, float height);
	ZRNUI_API float Zelement_get_width(Zid element);
	ZRNUI_API float Zelement_get_height(Zid element);

	/** @brief MinWidth / MinHeight (default 0), MaxWidth / MaxHeight (default Z_INFINITE). */
	ZRNUI_API void Zelement_set_min_size(Zid element, float min_width, float min_height);
	ZRNUI_API void Zelement_set_max_size(Zid element, float max_width, float max_height);

	ZRNUI_API void Zelement_set_margin(Zid element, ZThickness margin);
	ZRNUI_API ZThickness Zelement_get_margin(Zid element);
	/**
	 * @brief {ThemeResource key} for the Margin: a Thickness key (Z_RES_PIVOT_ITEM_MARGIN...). The margin follows the key
	 * (the theme, the accent, an override of the root) until Zelement_set_margin sets another one.
	 * @return Z_FALSE for an element that is not one or a key that is not a Thickness.
	 */
	ZRNUI_API int Zelement_bind_margin(Zid element, ZThemeResource key);

	ZRNUI_API void Zelement_set_horizontal_alignment(Zid element, ZHorizontalAlignment alignment);
	ZRNUI_API void Zelement_set_vertical_alignment(Zid element, ZVerticalAlignment alignment);
	ZRNUI_API ZHorizontalAlignment Zelement_get_horizontal_alignment(Zid element);
	ZRNUI_API ZVerticalAlignment Zelement_get_vertical_alignment(Zid element);

	/** @brief ActualWidth / ActualHeight: the size after the last Arrange, without the margin. */
	ZRNUI_API float Zelement_get_actual_width(Zid element);
	ZRNUI_API float Zelement_get_actual_height(Zid element);
	/** @brief DesiredSize of the last Measure, with the margin. */
	ZRNUI_API ZSize Zelement_get_desired_size(Zid element);
	/** @brief The rectangle of the element, margin excluded, in the coordinates of its parent. */
	ZRNUI_API ZRect Zelement_get_bounds(Zid element);

	/** @brief Visibility. A collapsed element is neither drawn, nor measured, nor hit. */
	ZRNUI_API void Zelement_set_visibility(Zid element, ZVisibility visibility);
	ZRNUI_API ZVisibility Zelement_get_visibility(Zid element);

	/** @brief Opacity of the element and its children, 0..1. */
	ZRNUI_API void Zelement_set_opacity(Zid element, float opacity);

	/* ---- render transforms and animation ----
	 * A render transform changes how an element and what is in it are drawn, not the layout, so a
	 * rotation or a slide costs no new Measure or Arrange. Only the OpenGL renderer draws them (the
	 * software renderer shows the element untransformed), and Zroot_set_animations decides whether
	 * Zelement_animate moves or jumps. A transformed element is hit tested where layout put it, and
	 * ClipToBounds of what is inside it is not applied while it is rotated.
	 */

	/** @brief PlaneProjection: rotation about the X and Y axes through a point of the element, `center_x` and `center_y` as fractions of its size (0.5, 0.5 is the middle; 0 is the left or top edge). The viewer is a 34 degree field of view away. */
	ZRNUI_API void Zelement_set_projection(Zid element, float rotation_x, float rotation_y, float center_x, float center_y);
	/** @brief A ScaleTransform about the centre of the projection. 1 is no scale. */
	ZRNUI_API void Zelement_set_render_scale(Zid element, float scale);
	/** @brief A TranslateTransform in logical pixels. */
	ZRNUI_API void Zelement_set_translate(Zid element, float x, float y);
	ZRNUI_API float Zelement_get_property(Zid element, ZAnimatedProperty property);
	/**
	 * @brief Moves a property to `to` over `duration_ms`, after `delay_ms`, from where it is now (a new
	 * animation of the same property replaces the one running). With animations off, or an element not
	 * in a root, the property takes `to` at once.
	 */
	ZRNUI_API void Zelement_animate(Zid element, ZAnimatedProperty property, float to, float duration_ms, ZEasing easing, float delay_ms);
	/** @brief Z_TRUE while an animation of the element is running. */
	ZRNUI_API int Zelement_is_animating(Zid element);
	/** @brief The easing curve itself: t in [0, 1] to [0, 1]. */
	ZRNUI_API float Zeasing(ZEasing easing, float t);
	/** @brief Tilt: while pressed the element leans toward the finger (WP's TiltEffect). On by default for list items, hyperlinks, check boxes and radio buttons; not for Button. */
	ZRNUI_API void Zelement_set_tilt(Zid element, int enabled);
	ZRNUI_API float Zelement_get_opacity(Zid element);

	/** @brief IsEnabled. A disabled element ignores input; its descendants are disabled too. */
	ZRNUI_API void Zelement_set_enabled(Zid element, int enabled);
	/** @return IsEnabled as set on the element itself. */
	ZRNUI_API int Zelement_get_enabled(Zid element);
	/** @return Z_TRUE when neither the element nor an ancestor is disabled. */
	ZRNUI_API int Zelement_is_effectively_enabled(Zid element);

	/** @brief IsHitTestVisible. Z_FALSE lets pointer events fall through the element and its children. */
	ZRNUI_API void Zelement_set_hit_test_visible(Zid element, int visible);

	/** @brief UIElement.Clip to the bounds: children are cut where the element ends. */
	ZRNUI_API void Zelement_set_clip_to_bounds(Zid element, int clip);

	/** @brief ToolTipService.ToolTip as plain text: shown when the pointer rests on the element. NULL removes it. */
	ZRNUI_API void Zelement_set_tooltip(Zid element, const char* text);

	/** @brief Forces the next Measure / Arrange of this element (InvalidateMeasure / InvalidateArrange). */
	ZRNUI_API void Zelement_invalidate_measure(Zid element);
	ZRNUI_API void Zelement_invalidate_arrange(Zid element);

	/** @brief Runs Measure then Arrange of one element by hand (UpdateLayout of a detached element). */
	ZRNUI_API void Zelement_measure(Zid element, ZSize available);
	ZRNUI_API void Zelement_arrange(Zid element, ZRect final_rect);

	/** @brief Gives the keyboard focus to the element (Control.Focus). A control that keeps its text in a part of its own (AutoSuggestBox) hands the focus to it. @return Z_FALSE when it cannot take it. */
	ZRNUI_API int Zelement_focus(Zid element);
	ZRNUI_API int Zelement_has_focus(Zid element);

	/**
	 * @brief Sets the handler of a routed event; one handler per event and
	 * element, NULL removes it. Events with data have their own setters
	 * (Zrange_base_on_value_changed, Zscroll_bar_on_scroll...).
	 */
	ZRNUI_API void Zelement_on(Zid element, ZEvent event, ZRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  PANEL
	 * ============================================================================ */

	/** @brief Appends a child (Panel.Children.Add). @return `child`, so calls nest. */
	ZRNUI_API Zid Zpanel_add(Zid panel, Zid child);
	ZRNUI_API Zid Zpanel_insert(Zid panel, int index, Zid child);
	/** @brief Detaches a child without destroying it. */
	ZRNUI_API void Zpanel_remove(Zid panel, Zid child);
	/** @brief Destroys all the children (Children.Clear). */
	ZRNUI_API void Zpanel_clear(Zid panel);
	ZRNUI_API int Zpanel_get_count(Zid panel);
	ZRNUI_API Zid Zpanel_get_child(Zid panel, int index);

	/** @brief Panel.Background; transparent (a panel that does not catch the pointer) by default. */
	ZRNUI_API void Zpanel_set_background(Zid panel, ZColor color);

	/** @brief Panel.ZIndex (attached): children with a higher value are drawn above. */
	ZRNUI_API void Zpanel_set_zindex(Zid child, int zindex);


	/* ============================================================================
	 *  STACKPANEL, VIRTUALIZINGSTACKPANEL
	 * ============================================================================ */

	ZRNUI_API Zid Zstack_panel_create(ZOrientation orientation);
	ZRNUI_API void Zstack_panel_set_orientation(Zid panel, ZOrientation orientation);
	ZRNUI_API ZOrientation Zstack_panel_get_orientation(Zid panel);

	/**
	 * @brief A StackPanel for the items of a ListBox. This implementation
	 * measures and arranges every container but paints only the ones in
	 * view; with it a list of thousands of rows scrolls well, but Measure
	 * is linear in the number of items.
	 */
	ZRNUI_API Zid Zvirtualizing_stack_panel_create(void);


	/**
	 * @brief The ItemsStackPanel of a ListView: a StackPanel for items (Zstack_panel_set_orientation sets its
	 * Orientation, vertical by default). It lays out every container, as the VirtualizingStackPanel does.
	 */
	ZRNUI_API Zid Zitems_stack_panel_create(ZOrientation orientation);


	/* ============================================================================
	 *  ITEMSWRAPGRID
	 *
	 *  The panel of a GridView: the items go in cells of one size, ItemWidth by ItemHeight, or when those
	 *  are not set the size of the first item. Horizontal fills a row left to right and then starts the next
	 *  row below; Vertical fills a column top to bottom and then starts the next one to the right.
	 *  A row (column) has as many cells as fit, at most MaximumRowsOrColumns. The header item of a group takes a line of its own.
	 * ============================================================================ */

	ZRNUI_API Zid Zitems_wrap_grid_create(ZOrientation orientation);
	ZRNUI_API void Zitems_wrap_grid_set_orientation(Zid grid, ZOrientation orientation);
	ZRNUI_API ZOrientation Zitems_wrap_grid_get_orientation(Zid grid);
	/** @brief MaximumRowsOrColumns: the most cells in a line, along the orientation; -1 (the default) for as many as fit. */
	ZRNUI_API void Zitems_wrap_grid_set_maximum_rows_or_columns(Zid grid, int maximum);
	ZRNUI_API int Zitems_wrap_grid_get_maximum_rows_or_columns(Zid grid);
	/** @brief ItemWidth and ItemHeight; Z_AUTO (the default) takes the size of the first item. */
	ZRNUI_API void Zitems_wrap_grid_set_item_width(Zid grid, float item_width);
	ZRNUI_API void Zitems_wrap_grid_set_item_height(Zid grid, float item_height);
	ZRNUI_API float Zitems_wrap_grid_get_item_width(Zid grid);
	ZRNUI_API float Zitems_wrap_grid_get_item_height(Zid grid);


	/* ============================================================================
	 *  WRAPPANEL
	 * ============================================================================ */

	ZRNUI_API Zid Zwrap_panel_create(ZOrientation orientation);
	ZRNUI_API void Zwrap_panel_set_orientation(Zid panel, ZOrientation orientation);
	/** @brief ItemWidth / ItemHeight: every child gets this size; Z_AUTO keeps its own. */
	ZRNUI_API void Zwrap_panel_set_item_size(Zid panel, float item_width, float item_height);


	/* ============================================================================
	 *  GRID
	 * ============================================================================ */

	ZRNUI_API Zid Zgrid_create(void);

	/**
	 * @brief Adds a column (ColumnDefinition). `value` is the width in pixels
	 * for Z_GRID_PIXEL, the weight for Z_GRID_STAR ("2*" is 2), ignored for Z_GRID_AUTO.
	 * @return The index of the column.
	 */
	ZRNUI_API int Zgrid_add_column(Zid grid, ZGridUnitType unit, float value);
	ZRNUI_API int Zgrid_add_row(Zid grid, ZGridUnitType unit, float value);
	/** @brief MinWidth / MaxWidth of a ColumnDefinition (MinHeight / MaxHeight of a row). */
	ZRNUI_API void Zgrid_set_column_limits(Zid grid, int column, float min_width, float max_width);
	ZRNUI_API void Zgrid_set_row_limits(Zid grid, int row, float min_height, float max_height);
	ZRNUI_API int Zgrid_get_column_count(Zid grid);
	ZRNUI_API int Zgrid_get_row_count(Zid grid);
	/** @brief The size of a column / row after the last Arrange. */
	ZRNUI_API float Zgrid_get_column_width(Zid grid, int column);
	ZRNUI_API float Zgrid_get_row_height(Zid grid, int row);

	/** @brief Grid.Row, Grid.Column, Grid.RowSpan, Grid.ColumnSpan of a child (attached). */
	ZRNUI_API void Zgrid_set_row(Zid child, int row);
	ZRNUI_API void Zgrid_set_column(Zid child, int column);
	ZRNUI_API void Zgrid_set_row_span(Zid child, int span);
	ZRNUI_API void Zgrid_set_column_span(Zid child, int span);
	ZRNUI_API int Zgrid_get_row(Zid child);
	ZRNUI_API int Zgrid_get_column(Zid child);

	/** @brief Adds `child` at a cell: the usual three lines in one. */
	ZRNUI_API Zid Zgrid_place(Zid grid, Zid child, int column, int row);


	/* ============================================================================
	 *  CANVAS
	 * ============================================================================ */

	ZRNUI_API Zid Zcanvas_create(void);
	/** @brief Canvas.Left / Canvas.Top (attached). Z_AUTO is 0. */
	ZRNUI_API void Zcanvas_set_left(Zid child, float left);
	ZRNUI_API void Zcanvas_set_top(Zid child, float top);
	ZRNUI_API Zid Zcanvas_place(Zid canvas, Zid child, float left, float top);


	/* ============================================================================
	 *  BORDER
	 * ============================================================================ */

	ZRNUI_API Zid Zborder_create(void);
	/** @brief Border.Child. Replacing a child destroys the old one. */
	ZRNUI_API void Zborder_set_child(Zid border, Zid child);
	ZRNUI_API Zid Zborder_get_child(Zid border);
	ZRNUI_API void Zborder_set_background(Zid border, ZColor color);
	ZRNUI_API void Zborder_set_border_brush(Zid border, ZColor color);
	ZRNUI_API void Zborder_set_border_thickness(Zid border, ZThickness thickness);
	/** @brief {ThemeResource key}: a color key for the background and the border brush, a Thickness key for the border thickness and the padding. The direct setter removes the binding. */
	ZRNUI_API int Zborder_bind_background(Zid border, ZThemeResource key);
	ZRNUI_API int Zborder_bind_border_brush(Zid border, ZThemeResource key);
	ZRNUI_API int Zborder_bind_border_thickness(Zid border, ZThemeResource key);
	ZRNUI_API int Zborder_bind_padding(Zid border, ZThemeResource key);
	ZRNUI_API void Zborder_set_corner_radius(Zid border, ZCornerRadius radius);
	ZRNUI_API void Zborder_set_padding(Zid border, ZThickness padding);


	/* ============================================================================
	 *  VIEWBOX
	 * ============================================================================ */

	ZRNUI_API Zid Zviewbox_create(void);
	ZRNUI_API void Zviewbox_set_child(Zid viewbox, Zid child);
	ZRNUI_API void Zviewbox_set_stretch(Zid viewbox, ZStretch stretch);
	ZRNUI_API void Zviewbox_set_stretch_direction(Zid viewbox, ZStretchDirection direction);


	/* ============================================================================
	 *  TEXTBLOCK
	 * ============================================================================ */

	ZRNUI_API Zid Ztext_block_create(const char* text);
	ZRNUI_API void Ztext_block_set_text(Zid text_block, const char* text);
	ZRNUI_API const char* Ztext_block_get_text(Zid text_block);
	/**
	 * Font properties are inherited down the tree like Silverlight's:
	 * a TextBlock that was given none takes the ones of the nearest control
	 * above it (a Button gives its content the semibold 22.667 of the
	 * template); a value set here wins.
	 */
	ZRNUI_API void Ztext_block_set_font_size(Zid text_block, float size);
	ZRNUI_API void Ztext_block_set_font_family(Zid text_block, ZFontFamily family);
	ZRNUI_API void Ztext_block_set_foreground(Zid text_block, ZColor color);
	ZRNUI_API void Ztext_block_set_text_wrapping(Zid text_block, ZTextWrapping wrapping);
	ZRNUI_API void Ztext_block_set_text_alignment(Zid text_block, ZTextAlignment alignment);
	ZRNUI_API void Ztext_block_set_text_trimming(Zid text_block, ZTextTrimming trimming);
	/** @brief MaxLines-like limit; 0 is no limit. */
	ZRNUI_API void Ztext_block_set_max_lines(Zid text_block, int max_lines);
	/** @brief LineHeight in pixels; 0 uses the line height of the font. */
	ZRNUI_API void Ztext_block_set_line_height(Zid text_block, float line_height);
	/** @brief CharacterSpacing in 1/1000 em (the panorama titles use -35). */
	ZRNUI_API void Ztext_block_set_character_spacing(Zid text_block, int spacing);
	/**
	 * @brief {ThemeResource key} for a property of a TextBlock: the foreground (a color key, a brush such as
	 * Z_RES_PHONE_FOREGROUND_BRUSH), the font size (a Double key), the character spacing (an Int key such as
	 * Z_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING). The property follows the key when the theme, the accent or an
	 * override of the root changes; its direct setter (and Ztext_block_set_style) removes the binding.
	 * @return Z_FALSE for a Zid that is not a TextBlock or a key of another type.
	 */
	ZRNUI_API int Ztext_block_bind_foreground(Zid text_block, ZThemeResource key);
	ZRNUI_API int Ztext_block_bind_font_size(Zid text_block, ZThemeResource key);
	ZRNUI_API int Ztext_block_bind_character_spacing(Zid text_block, ZThemeResource key);
	/**
	 * @brief Applies a text style: font, size, foreground, line height, character spacing, wrapping and
	 * margin of the style (the 8.1 ones carry no margin, the Z_TEXT_STYLE_PHONE_* ones 12 on each side).
	 * A value set after the style wins. @return Z_FALSE for Z_TEXT_STYLE_NONE or a value out of range.
	 */
	ZRNUI_API int Ztext_block_set_style(Zid text_block, ZTextStyle style);


	/* ============================================================================
	 *  RICHTEXTBLOCK
	 *
	 *  Minimal: paragraphs of plain text under one another (Blocks with Paragraphs that hold a Run), each wrapped at
	 *  the width of the block, with no space between them, in the font, color and alignment that the block has. The
	 *  text properties are those of a TextBlock; the defaults are too, but the text wraps (the 8.1 default). A paragraph
	 *  is a TextBlock owned by the block: Zrich_text_block_get_paragraph gives it for what the block has no function for.
	 *  Not made: inlines (Run, Bold, Hyperlink...), OverflowContentTarget and RichTextBlockOverflow, text selection,
	 *  TextIndent, MaxLines, LineStackingStrategy. The space between paragraphs is not in the XAML; there is none here.
	 * ============================================================================ */

	ZRNUI_API Zid Zrich_text_block_create(void);
	/** @brief Blocks.Append: a paragraph with a text. @return its index. */
	ZRNUI_API int Zrich_text_block_add_paragraph(Zid rich_text_block, const char* text);
	ZRNUI_API void Zrich_text_block_set_paragraph_text(Zid rich_text_block, int index, const char* text);
	ZRNUI_API const char* Zrich_text_block_get_paragraph_text(Zid rich_text_block, int index);
	ZRNUI_API int Zrich_text_block_get_paragraph_count(Zid rich_text_block);
	/** @return The TextBlock of a paragraph. */
	ZRNUI_API Zid Zrich_text_block_get_paragraph(Zid rich_text_block, int index);
	ZRNUI_API void Zrich_text_block_remove_paragraph_at(Zid rich_text_block, int index);
	ZRNUI_API void Zrich_text_block_clear_paragraphs(Zid rich_text_block);
	ZRNUI_API void Zrich_text_block_set_font_size(Zid rich_text_block, float size);
	ZRNUI_API float Zrich_text_block_get_font_size(Zid rich_text_block);
	ZRNUI_API void Zrich_text_block_set_font_family(Zid rich_text_block, ZFontFamily family);
	ZRNUI_API void Zrich_text_block_set_foreground(Zid rich_text_block, ZColor color);
	ZRNUI_API void Zrich_text_block_set_text_wrapping(Zid rich_text_block, ZTextWrapping wrapping);
	ZRNUI_API void Zrich_text_block_set_text_alignment(Zid rich_text_block, ZTextAlignment alignment);
	ZRNUI_API void Zrich_text_block_set_text_trimming(Zid rich_text_block, ZTextTrimming trimming);
	/** @brief LineHeight in pixels; 0 uses the line height of the font. */
	ZRNUI_API void Zrich_text_block_set_line_height(Zid rich_text_block, float line_height);
	/** @brief CharacterSpacing in 1/1000 em. */
	ZRNUI_API void Zrich_text_block_set_character_spacing(Zid rich_text_block, int spacing);
	ZRNUI_API void Zrich_text_block_set_padding(Zid rich_text_block, ZThickness padding);
	ZRNUI_API ZThickness Zrich_text_block_get_padding(Zid rich_text_block);
	/** @brief Z_TEXT_STYLE_BASE (16 px, line 21.5) or Z_TEXT_STYLE_BODY (18 px, line 24). @return Z_FALSE for another style. */
	ZRNUI_API int Zrich_text_block_set_style(Zid rich_text_block, ZTextStyle style);


	/* ============================================================================
	 *  IMAGE
	 * ============================================================================ */

	ZRNUI_API Zid Zimage_create(void);
	/** @brief Image.Source from a PNG file. @return Z_FALSE on failure (the image stays as it was). */
	ZRNUI_API int Zimage_set_source_file(Zid image, const char* path);
	/** @brief Image.Source from pixels: width * height RGBA bytes, copied. */
	ZRNUI_API int Zimage_set_source_rgba(Zid image, int width, int height, const uint8_t* rgba);
	ZRNUI_API void Zimage_set_stretch(Zid image, ZStretch stretch);
	ZRNUI_API ZSize Zimage_get_natural_size(Zid image);


	/* ============================================================================
	 *  POPUP
	 * ============================================================================ */

	/**
	 * A Popup shows its child above the rest of the page, at an offset from
	 * the top left corner of its root (or from `Zpopup_set_placement_target`).
	 * It takes no room in the layout. A ComboBox drops down in one, and the flyouts and the
	 * ContentDialog (zrnui_controls.h) are shown in one.
	 */
	ZRNUI_API Zid Zpopup_create(void);
	ZRNUI_API void Zpopup_set_child(Zid popup, Zid child);
	ZRNUI_API void Zpopup_set_offset(Zid popup, float horizontal_offset, float vertical_offset);
	/** @brief PlacementTarget and Placement: the popup opens next to `target`. */
	ZRNUI_API void Zpopup_set_placement_target(Zid popup, Zid target, ZPlacementMode mode);
	/** @brief IsOpen. A popup opens only once it is in a tree that belongs to a root. */
	ZRNUI_API void Zpopup_set_open(Zid popup, int open);
	ZRNUI_API int Zpopup_is_open(Zid popup);
	/** @brief StaysOpen: Z_FALSE (the default of ComboBox) closes the popup on a click outside. */
	ZRNUI_API void Zpopup_set_stays_open(Zid popup, int stays_open);

#ifdef __cplusplus
}
#endif

#endif
