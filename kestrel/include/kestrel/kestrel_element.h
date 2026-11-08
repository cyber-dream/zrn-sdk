/**
 * Kestrel
 * kestrel_element.h : UIElement, FrameworkElement, and the elements that
 * are not controls: panels, Border, Viewbox, TextBlock, Image, Popup.
 *
 * The hierarchy, as in System.Windows:
 *
 *   Element (UIElement + FrameworkElement)
 *     Panel                 StackPanel (VirtualizingStackPanel, ItemsStackPanel), WrapPanel,
 *                           ItemsWrapGrid, Grid, Canvas
 *     Border, Viewbox, TextBlock, Image, Popup
 *     Control               kestrel_controls.h
 *
 * Layout is the two passes of Silverlight. Measure asks each element how
 * much room it wants given the room on offer (bottom-up); Arrange then
 * gives each one its final rectangle (top-down). Margin, Width, Height,
 * MinWidth..MaxHeight and the alignments are common to every element.
 * Kroot_update_layout runs the passes; rendering and input run them when
 * something changed, so a program rarely calls it.
 *
 * Attached properties (Grid.Row, Canvas.Left, Panel.ZIndex) are stored on
 * the child, with Kgrid_set_row(child, ...), and ignored when the parent
 * is not that kind of panel.
 *
 * Ownership: a parent owns its children and destroys them with itself.
 * An element with no parent is owned by the caller until it is added to a
 * panel, set as content or set as the visual of a root.
 */

#ifndef KESTREL_ELEMENT_H
#define KESTREL_ELEMENT_H

#include "kestrel_core.h"
#include "kestrel_resources.h"


/** Routed events that carry no data: read the new state from the sender. */
typedef enum KEvent {
	K_EVENT_CLICK = 0x0400,
	K_EVENT_CHECKED,
	K_EVENT_UNCHECKED,
	K_EVENT_INDETERMINATE,
	K_EVENT_TEXT_CHANGED,
	K_EVENT_SELECTION_CHANGED,
	K_EVENT_DROP_DOWN_OPENED,
	K_EVENT_DROP_DOWN_CLOSED,
	K_EVENT_GOT_FOCUS,
	K_EVENT_LOST_FOCUS,
	K_EVENT_LOADED,
	K_EVENT_POPUP_OPENED,
	K_EVENT_POPUP_CLOSED,
	K_EVENT_NAVIGATED,              /* Frame: a page is shown */
	K_EVENT_NAVIGATED_TO,           /* Page: the frame has shown it */
	K_EVENT_NAVIGATED_FROM,         /* Page: the frame is about to leave it */
	K_EVENT_TOGGLED,                /* ToggleSwitch: IsOn changed */
	K_EVENT_OPENING,                /* FlyoutBase: about to show */
	K_EVENT_OPENED,                 /* AppBar, FlyoutBase, ContentDialog: now open */
	K_EVENT_CLOSED,                 /* AppBar, FlyoutBase, ContentDialog: now closed */
	K_EVENT_PRIMARY_BUTTON_CLICK,   /* ContentDialog */
	K_EVENT_SECONDARY_BUTTON_CLICK, /* ContentDialog */
	K_EVENT_SECTIONS_IN_VIEW_CHANGED, /* Hub: the sections in view are others now */
	K_EVENT_LAST_
} KEvent;

typedef void (*KRoutedCallback)(Kid sender, void* user_data);

/** Easing of an animation: the curve from 0 to 1 over its duration. */
typedef enum KEasing {
	K_EASE_LINEAR = 0,
	K_EASE_OUT,          /* cubic: quick start, soft end */
	K_EASE_IN,           /* cubic: slow start */
	K_EASE_EMPHASIZED    /* (0.2, 0.8, 0.2, 1) as a cubic Bezier: a quick start and a long soft settle */
} KEasing;

/** What Kelement_animate can move. Rotations are degrees. */
typedef enum KAnimatedProperty {
	K_PROP_OPACITY = 0,
	K_PROP_ROTATION_X,
	K_PROP_ROTATION_Y,
	K_PROP_SCALE,        /* ScaleTransform about the centre of rotation */
	K_PROP_TRANSLATE_X,
	K_PROP_TRANSLATE_Y,
	K_PROP_LAST_
} KAnimatedProperty;


#ifdef __cplusplus
extern "C" {
#endif

	/* ============================================================================
	 *  CREATION & VISUAL TREE
	 * ============================================================================ */

	/**
	 * @brief Creates an element of a class. Shortcuts per class exist
	 * (Kbutton_create...), this one serves code that picks the class at
	 * run time. Abstract classes (K_CLASS_PANEL...) are refused.
	 */
	KESTREL_API Kid Kelement_create(KClass element_class);

	/** @brief Destroys an element and every element under it. A root's visual is detached first. */
	KESTREL_API void Kelement_destroy(Kid element);
	KESTREL_API int Kelement_is_valid(Kid element);

	/** @return The most derived class of the element, 0 for an invalid handle. */
	KESTREL_API KClass Kelement_get_class(Kid element);
	/** @return K_TRUE when the element is of `element_class` or derives from it (a CheckBox is a ToggleButton, a ButtonBase, a ContentControl, a Control...). */
	KESTREL_API int Kelement_is_a(Kid element, KClass element_class);
	KESTREL_API const char* Kelement_get_class_name(Kid element);

	/** @brief FrameworkElement.Name. FindName searches the subtree. */
	KESTREL_API void Kelement_set_name(Kid element, const char* name);
	KESTREL_API const char* Kelement_get_name(Kid element);
	KESTREL_API Kid Kelement_find_name(Kid scope, const char* name);

	/** @brief FrameworkElement.Tag: a pointer of the program, never touched. */
	KESTREL_API void Kelement_set_tag(Kid element, void* tag);
	KESTREL_API void* Kelement_get_tag(Kid element);

	/**
	 * The visual tree (VisualTreeHelper). A Button has one child, its
	 * content; a ListBox has its scroll viewer, which has the item panel.
	 * To change the contents of a panel use Kpanel_*, to change the content
	 * of a control Kcontent_*, to change the items Kitems_control_*.
	 */
	KESTREL_API Kid Kelement_get_parent(Kid element);
	KESTREL_API int Kelement_get_child_count(Kid element);
	KESTREL_API Kid Kelement_get_child(Kid element, int index);
	/** @return The root showing the element, or K_INVALID_ID. */
	KESTREL_API Kid Kelement_get_root(Kid element);

	/** @brief Transform of a point of the element to the root, and back (TransformToVisual). */
	KESTREL_API KPoint Kelement_to_root(Kid element, KPoint point);
	KESTREL_API KPoint Kelement_from_root(Kid element, KPoint point);

	/** @brief The element under a point given in the coordinates of `scope`, or K_INVALID_ID. */
	KESTREL_API Kid Kelement_hit_test(Kid scope, KPoint point);


	/* ============================================================================
	 *  FRAMEWORKELEMENT: LAYOUT PROPERTIES
	 * ============================================================================ */

	/** @brief Width and Height; K_AUTO sizes to the content (the default). */
	KESTREL_API void Kelement_set_width(Kid element, float width);
	KESTREL_API void Kelement_set_height(Kid element, float height);
	KESTREL_API float Kelement_get_width(Kid element);
	KESTREL_API float Kelement_get_height(Kid element);

	/** @brief MinWidth / MinHeight (default 0), MaxWidth / MaxHeight (default K_INFINITE). */
	KESTREL_API void Kelement_set_min_size(Kid element, float min_width, float min_height);
	KESTREL_API void Kelement_set_max_size(Kid element, float max_width, float max_height);

	KESTREL_API void Kelement_set_margin(Kid element, KThickness margin);
	KESTREL_API KThickness Kelement_get_margin(Kid element);
	/**
	 * @brief {ThemeResource key} for the Margin: a Thickness key (K_RES_PIVOT_ITEM_MARGIN...). The margin follows the key
	 * (the theme, the accent, an override of the root) until Kelement_set_margin sets another one.
	 * @return K_FALSE for an element that is not one or a key that is not a Thickness.
	 */
	KESTREL_API int Kelement_bind_margin(Kid element, KThemeResource key);

	KESTREL_API void Kelement_set_horizontal_alignment(Kid element, KHorizontalAlignment alignment);
	KESTREL_API void Kelement_set_vertical_alignment(Kid element, KVerticalAlignment alignment);
	KESTREL_API KHorizontalAlignment Kelement_get_horizontal_alignment(Kid element);
	KESTREL_API KVerticalAlignment Kelement_get_vertical_alignment(Kid element);

	/** @brief ActualWidth / ActualHeight: the size after the last Arrange, without the margin. */
	KESTREL_API float Kelement_get_actual_width(Kid element);
	KESTREL_API float Kelement_get_actual_height(Kid element);
	/** @brief DesiredSize of the last Measure, with the margin. */
	KESTREL_API KSize Kelement_get_desired_size(Kid element);
	/** @brief The rectangle of the element, margin excluded, in the coordinates of its parent. */
	KESTREL_API KRect Kelement_get_bounds(Kid element);

	/** @brief Visibility. A collapsed element is neither drawn, nor measured, nor hit. */
	KESTREL_API void Kelement_set_visibility(Kid element, KVisibility visibility);
	KESTREL_API KVisibility Kelement_get_visibility(Kid element);

	/** @brief Opacity of the element and its children, 0..1. */
	KESTREL_API void Kelement_set_opacity(Kid element, float opacity);

	/* ---- render transforms and animation ----
	 * A render transform changes how an element and what is in it are drawn, not the layout, so a
	 * rotation or a slide costs no new Measure or Arrange. Only the OpenGL renderer draws them (the
	 * software renderer shows the element untransformed), and Kroot_set_animations decides whether
	 * Kelement_animate moves or jumps. A transformed element is hit tested where layout put it, and
	 * ClipToBounds of what is inside it is not applied while it is rotated.
	 */

	/** @brief PlaneProjection: rotation about the X and Y axes through a point of the element, `center_x` and `center_y` as fractions of its size (0.5, 0.5 is the middle; 0 is the left or top edge). The viewer is a 34 degree field of view away. */
	KESTREL_API void Kelement_set_projection(Kid element, float rotation_x, float rotation_y, float center_x, float center_y);
	/** @brief A ScaleTransform about the centre of the projection. 1 is no scale. */
	KESTREL_API void Kelement_set_render_scale(Kid element, float scale);
	/** @brief A TranslateTransform in logical pixels. */
	KESTREL_API void Kelement_set_translate(Kid element, float x, float y);
	KESTREL_API float Kelement_get_property(Kid element, KAnimatedProperty property);
	/**
	 * @brief Moves a property to `to` over `duration_ms`, after `delay_ms`, from where it is now (a new
	 * animation of the same property replaces the one running). With animations off, or an element not
	 * in a root, the property takes `to` at once.
	 */
	KESTREL_API void Kelement_animate(Kid element, KAnimatedProperty property, float to, float duration_ms, KEasing easing, float delay_ms);
	/** @brief K_TRUE while an animation of the element is running. */
	KESTREL_API int Kelement_is_animating(Kid element);
	/** @brief The easing curve itself: t in [0, 1] to [0, 1]. */
	KESTREL_API float Keasing(KEasing easing, float t);
	/** @brief Tilt: while pressed the element leans toward the finger (WP's TiltEffect). On by default for list items, hyperlinks, check boxes and radio buttons; not for Button. */
	KESTREL_API void Kelement_set_tilt(Kid element, int enabled);
	KESTREL_API float Kelement_get_opacity(Kid element);

	/** @brief IsEnabled. A disabled element ignores input; its descendants are disabled too. */
	KESTREL_API void Kelement_set_enabled(Kid element, int enabled);
	/** @return IsEnabled as set on the element itself. */
	KESTREL_API int Kelement_get_enabled(Kid element);
	/** @return K_TRUE when neither the element nor an ancestor is disabled. */
	KESTREL_API int Kelement_is_effectively_enabled(Kid element);

	/** @brief IsHitTestVisible. K_FALSE lets pointer events fall through the element and its children. */
	KESTREL_API void Kelement_set_hit_test_visible(Kid element, int visible);

	/** @brief UIElement.Clip to the bounds: children are cut where the element ends. */
	KESTREL_API void Kelement_set_clip_to_bounds(Kid element, int clip);

	/** @brief ToolTipService.ToolTip as plain text: shown when the pointer rests on the element. NULL removes it. */
	KESTREL_API void Kelement_set_tooltip(Kid element, const char* text);

	/** @brief Forces the next Measure / Arrange of this element (InvalidateMeasure / InvalidateArrange). */
	KESTREL_API void Kelement_invalidate_measure(Kid element);
	KESTREL_API void Kelement_invalidate_arrange(Kid element);

	/** @brief Runs Measure then Arrange of one element by hand (UpdateLayout of a detached element). */
	KESTREL_API void Kelement_measure(Kid element, KSize available);
	KESTREL_API void Kelement_arrange(Kid element, KRect final_rect);

	/** @brief Gives the keyboard focus to the element (Control.Focus). A control that keeps its text in a part of its own (AutoSuggestBox) hands the focus to it. @return K_FALSE when it cannot take it. */
	KESTREL_API int Kelement_focus(Kid element);
	KESTREL_API int Kelement_has_focus(Kid element);

	/**
	 * @brief Sets the handler of a routed event; one handler per event and
	 * element, NULL removes it. Events with data have their own setters
	 * (Krange_base_on_value_changed, Kscroll_bar_on_scroll...).
	 */
	KESTREL_API void Kelement_on(Kid element, KEvent event, KRoutedCallback callback, void* user_data);


	/* ============================================================================
	 *  PANEL
	 * ============================================================================ */

	/** @brief Appends a child (Panel.Children.Add). @return `child`, so calls nest. */
	KESTREL_API Kid Kpanel_add(Kid panel, Kid child);
	KESTREL_API Kid Kpanel_insert(Kid panel, int index, Kid child);
	/** @brief Detaches a child without destroying it. */
	KESTREL_API void Kpanel_remove(Kid panel, Kid child);
	/** @brief Destroys all the children (Children.Clear). */
	KESTREL_API void Kpanel_clear(Kid panel);
	KESTREL_API int Kpanel_get_count(Kid panel);
	KESTREL_API Kid Kpanel_get_child(Kid panel, int index);

	/** @brief Panel.Background; transparent (a panel that does not catch the pointer) by default. */
	KESTREL_API void Kpanel_set_background(Kid panel, KColor color);

	/** @brief Panel.ZIndex (attached): children with a higher value are drawn above. */
	KESTREL_API void Kpanel_set_zindex(Kid child, int zindex);


	/* ============================================================================
	 *  STACKPANEL, VIRTUALIZINGSTACKPANEL
	 * ============================================================================ */

	KESTREL_API Kid Kstack_panel_create(KOrientation orientation);
	KESTREL_API void Kstack_panel_set_orientation(Kid panel, KOrientation orientation);
	KESTREL_API KOrientation Kstack_panel_get_orientation(Kid panel);

	/**
	 * @brief A StackPanel for the items of a ListBox. This implementation
	 * measures and arranges every container but paints only the ones in
	 * view; with it a list of thousands of rows scrolls well, but Measure
	 * is linear in the number of items.
	 */
	KESTREL_API Kid Kvirtualizing_stack_panel_create(void);


	/**
	 * @brief The ItemsStackPanel of a ListView: a StackPanel for items (Kstack_panel_set_orientation sets its
	 * Orientation, vertical by default). It lays out every container, as the VirtualizingStackPanel does.
	 */
	KESTREL_API Kid Kitems_stack_panel_create(KOrientation orientation);


	/* ============================================================================
	 *  ITEMSWRAPGRID
	 *
	 *  The panel of a GridView: the items go in cells of one size, ItemWidth by ItemHeight, or when those
	 *  are not set the size of the first item. Horizontal fills a row left to right and then starts the next
	 *  row below; Vertical fills a column top to bottom and then starts the next one to the right.
	 *  A row (column) has as many cells as fit, at most MaximumRowsOrColumns. The header item of a group takes a line of its own.
	 * ============================================================================ */

	KESTREL_API Kid Kitems_wrap_grid_create(KOrientation orientation);
	KESTREL_API void Kitems_wrap_grid_set_orientation(Kid grid, KOrientation orientation);
	KESTREL_API KOrientation Kitems_wrap_grid_get_orientation(Kid grid);
	/** @brief MaximumRowsOrColumns: the most cells in a line, along the orientation; -1 (the default) for as many as fit. */
	KESTREL_API void Kitems_wrap_grid_set_maximum_rows_or_columns(Kid grid, int maximum);
	KESTREL_API int Kitems_wrap_grid_get_maximum_rows_or_columns(Kid grid);
	/** @brief ItemWidth and ItemHeight; K_AUTO (the default) takes the size of the first item. */
	KESTREL_API void Kitems_wrap_grid_set_item_width(Kid grid, float item_width);
	KESTREL_API void Kitems_wrap_grid_set_item_height(Kid grid, float item_height);
	KESTREL_API float Kitems_wrap_grid_get_item_width(Kid grid);
	KESTREL_API float Kitems_wrap_grid_get_item_height(Kid grid);


	/* ============================================================================
	 *  WRAPPANEL
	 * ============================================================================ */

	KESTREL_API Kid Kwrap_panel_create(KOrientation orientation);
	KESTREL_API void Kwrap_panel_set_orientation(Kid panel, KOrientation orientation);
	/** @brief ItemWidth / ItemHeight: every child gets this size; K_AUTO keeps its own. */
	KESTREL_API void Kwrap_panel_set_item_size(Kid panel, float item_width, float item_height);


	/* ============================================================================
	 *  GRID
	 * ============================================================================ */

	KESTREL_API Kid Kgrid_create(void);

	/**
	 * @brief Adds a column (ColumnDefinition). `value` is the width in pixels
	 * for K_GRID_PIXEL, the weight for K_GRID_STAR ("2*" is 2), ignored for K_GRID_AUTO.
	 * @return The index of the column.
	 */
	KESTREL_API int Kgrid_add_column(Kid grid, KGridUnitType unit, float value);
	KESTREL_API int Kgrid_add_row(Kid grid, KGridUnitType unit, float value);
	/** @brief MinWidth / MaxWidth of a ColumnDefinition (MinHeight / MaxHeight of a row). */
	KESTREL_API void Kgrid_set_column_limits(Kid grid, int column, float min_width, float max_width);
	KESTREL_API void Kgrid_set_row_limits(Kid grid, int row, float min_height, float max_height);
	KESTREL_API int Kgrid_get_column_count(Kid grid);
	KESTREL_API int Kgrid_get_row_count(Kid grid);
	/** @brief The size of a column / row after the last Arrange. */
	KESTREL_API float Kgrid_get_column_width(Kid grid, int column);
	KESTREL_API float Kgrid_get_row_height(Kid grid, int row);

	/** @brief Grid.Row, Grid.Column, Grid.RowSpan, Grid.ColumnSpan of a child (attached). */
	KESTREL_API void Kgrid_set_row(Kid child, int row);
	KESTREL_API void Kgrid_set_column(Kid child, int column);
	KESTREL_API void Kgrid_set_row_span(Kid child, int span);
	KESTREL_API void Kgrid_set_column_span(Kid child, int span);
	KESTREL_API int Kgrid_get_row(Kid child);
	KESTREL_API int Kgrid_get_column(Kid child);

	/** @brief Adds `child` at a cell: the usual three lines in one. */
	KESTREL_API Kid Kgrid_place(Kid grid, Kid child, int column, int row);


	/* ============================================================================
	 *  CANVAS
	 * ============================================================================ */

	KESTREL_API Kid Kcanvas_create(void);
	/** @brief Canvas.Left / Canvas.Top (attached). K_AUTO is 0. */
	KESTREL_API void Kcanvas_set_left(Kid child, float left);
	KESTREL_API void Kcanvas_set_top(Kid child, float top);
	KESTREL_API Kid Kcanvas_place(Kid canvas, Kid child, float left, float top);


	/* ============================================================================
	 *  BORDER
	 * ============================================================================ */

	KESTREL_API Kid Kborder_create(void);
	/** @brief Border.Child. Replacing a child destroys the old one. */
	KESTREL_API void Kborder_set_child(Kid border, Kid child);
	KESTREL_API Kid Kborder_get_child(Kid border);
	KESTREL_API void Kborder_set_background(Kid border, KColor color);
	KESTREL_API void Kborder_set_border_brush(Kid border, KColor color);
	KESTREL_API void Kborder_set_border_thickness(Kid border, KThickness thickness);
	/** @brief {ThemeResource key}: a color key for the background and the border brush, a Thickness key for the border thickness and the padding. The direct setter removes the binding. */
	KESTREL_API int Kborder_bind_background(Kid border, KThemeResource key);
	KESTREL_API int Kborder_bind_border_brush(Kid border, KThemeResource key);
	KESTREL_API int Kborder_bind_border_thickness(Kid border, KThemeResource key);
	KESTREL_API int Kborder_bind_padding(Kid border, KThemeResource key);
	KESTREL_API void Kborder_set_corner_radius(Kid border, KCornerRadius radius);
	KESTREL_API void Kborder_set_padding(Kid border, KThickness padding);


	/* ============================================================================
	 *  VIEWBOX
	 * ============================================================================ */

	KESTREL_API Kid Kviewbox_create(void);
	KESTREL_API void Kviewbox_set_child(Kid viewbox, Kid child);
	KESTREL_API void Kviewbox_set_stretch(Kid viewbox, KStretch stretch);
	KESTREL_API void Kviewbox_set_stretch_direction(Kid viewbox, KStretchDirection direction);


	/* ============================================================================
	 *  TEXTBLOCK
	 * ============================================================================ */

	KESTREL_API Kid Ktext_block_create(const char* text);
	KESTREL_API void Ktext_block_set_text(Kid text_block, const char* text);
	KESTREL_API const char* Ktext_block_get_text(Kid text_block);
	/**
	 * Font properties are inherited down the tree like Silverlight's:
	 * a TextBlock that was given none takes the ones of the nearest control
	 * above it (a Button gives its content the semibold 22.667 of the
	 * template); a value set here wins.
	 */
	KESTREL_API void Ktext_block_set_font_size(Kid text_block, float size);
	KESTREL_API void Ktext_block_set_font_family(Kid text_block, KFontFamily family);
	KESTREL_API void Ktext_block_set_foreground(Kid text_block, KColor color);
	KESTREL_API void Ktext_block_set_text_wrapping(Kid text_block, KTextWrapping wrapping);
	KESTREL_API void Ktext_block_set_text_alignment(Kid text_block, KTextAlignment alignment);
	KESTREL_API void Ktext_block_set_text_trimming(Kid text_block, KTextTrimming trimming);
	/** @brief MaxLines-like limit; 0 is no limit. */
	KESTREL_API void Ktext_block_set_max_lines(Kid text_block, int max_lines);
	/** @brief LineHeight in pixels; 0 uses the line height of the font. */
	KESTREL_API void Ktext_block_set_line_height(Kid text_block, float line_height);
	/** @brief CharacterSpacing in 1/1000 em (the panorama titles use -35). */
	KESTREL_API void Ktext_block_set_character_spacing(Kid text_block, int spacing);
	/**
	 * @brief {ThemeResource key} for a property of a TextBlock: the foreground (a color key, a brush such as
	 * K_RES_PHONE_FOREGROUND_BRUSH), the font size (a Double key), the character spacing (an Int key such as
	 * K_RES_PIVOT_HEADER_ITEM_CHARACTER_SPACING). The property follows the key when the theme, the accent or an
	 * override of the root changes; its direct setter (and Ktext_block_set_style) removes the binding.
	 * @return K_FALSE for a Kid that is not a TextBlock or a key of another type.
	 */
	KESTREL_API int Ktext_block_bind_foreground(Kid text_block, KThemeResource key);
	KESTREL_API int Ktext_block_bind_font_size(Kid text_block, KThemeResource key);
	KESTREL_API int Ktext_block_bind_character_spacing(Kid text_block, KThemeResource key);
	/**
	 * @brief Applies a text style: font, size, foreground, line height, character spacing, wrapping and
	 * margin of the style (the 8.1 ones carry no margin, the K_TEXT_STYLE_PHONE_* ones 12 on each side).
	 * A value set after the style wins. @return K_FALSE for K_TEXT_STYLE_NONE or a value out of range.
	 */
	KESTREL_API int Ktext_block_set_style(Kid text_block, KTextStyle style);


	/* ============================================================================
	 *  RICHTEXTBLOCK
	 *
	 *  Minimal: paragraphs of plain text under one another (Blocks with Paragraphs that hold a Run), each wrapped at
	 *  the width of the block, with no space between them, in the font, color and alignment that the block has. The
	 *  text properties are those of a TextBlock; the defaults are too, but the text wraps (the 8.1 default). A paragraph
	 *  is a TextBlock owned by the block: Krich_text_block_get_paragraph gives it for what the block has no function for.
	 *  Not made: inlines (Run, Bold, Hyperlink...), OverflowContentTarget and RichTextBlockOverflow, text selection,
	 *  TextIndent, MaxLines, LineStackingStrategy. The space between paragraphs is not in the XAML; there is none here.
	 * ============================================================================ */

	KESTREL_API Kid Krich_text_block_create(void);
	/** @brief Blocks.Append: a paragraph with a text. @return its index. */
	KESTREL_API int Krich_text_block_add_paragraph(Kid rich_text_block, const char* text);
	KESTREL_API void Krich_text_block_set_paragraph_text(Kid rich_text_block, int index, const char* text);
	KESTREL_API const char* Krich_text_block_get_paragraph_text(Kid rich_text_block, int index);
	KESTREL_API int Krich_text_block_get_paragraph_count(Kid rich_text_block);
	/** @return The TextBlock of a paragraph. */
	KESTREL_API Kid Krich_text_block_get_paragraph(Kid rich_text_block, int index);
	KESTREL_API void Krich_text_block_remove_paragraph_at(Kid rich_text_block, int index);
	KESTREL_API void Krich_text_block_clear_paragraphs(Kid rich_text_block);
	KESTREL_API void Krich_text_block_set_font_size(Kid rich_text_block, float size);
	KESTREL_API float Krich_text_block_get_font_size(Kid rich_text_block);
	KESTREL_API void Krich_text_block_set_font_family(Kid rich_text_block, KFontFamily family);
	KESTREL_API void Krich_text_block_set_foreground(Kid rich_text_block, KColor color);
	KESTREL_API void Krich_text_block_set_text_wrapping(Kid rich_text_block, KTextWrapping wrapping);
	KESTREL_API void Krich_text_block_set_text_alignment(Kid rich_text_block, KTextAlignment alignment);
	KESTREL_API void Krich_text_block_set_text_trimming(Kid rich_text_block, KTextTrimming trimming);
	/** @brief LineHeight in pixels; 0 uses the line height of the font. */
	KESTREL_API void Krich_text_block_set_line_height(Kid rich_text_block, float line_height);
	/** @brief CharacterSpacing in 1/1000 em. */
	KESTREL_API void Krich_text_block_set_character_spacing(Kid rich_text_block, int spacing);
	KESTREL_API void Krich_text_block_set_padding(Kid rich_text_block, KThickness padding);
	KESTREL_API KThickness Krich_text_block_get_padding(Kid rich_text_block);
	/** @brief K_TEXT_STYLE_BASE (16 px, line 21.5) or K_TEXT_STYLE_BODY (18 px, line 24). @return K_FALSE for another style. */
	KESTREL_API int Krich_text_block_set_style(Kid rich_text_block, KTextStyle style);


	/* ============================================================================
	 *  IMAGE
	 * ============================================================================ */

	KESTREL_API Kid Kimage_create(void);
	/** @brief Image.Source from a PNG file. @return K_FALSE on failure (the image stays as it was). */
	KESTREL_API int Kimage_set_source_file(Kid image, const char* path);
	/** @brief Image.Source from pixels: width * height RGBA bytes, copied. */
	KESTREL_API int Kimage_set_source_rgba(Kid image, int width, int height, const uint8_t* rgba);
	KESTREL_API void Kimage_set_stretch(Kid image, KStretch stretch);
	KESTREL_API KSize Kimage_get_natural_size(Kid image);


	/* ============================================================================
	 *  POPUP
	 * ============================================================================ */

	/**
	 * A Popup shows its child above the rest of the page, at an offset from
	 * the top left corner of its root (or from `Kpopup_set_placement_target`).
	 * It takes no room in the layout. A ComboBox drops down in one, and the flyouts and the
	 * ContentDialog (kestrel_controls.h) are shown in one.
	 */
	KESTREL_API Kid Kpopup_create(void);
	KESTREL_API void Kpopup_set_child(Kid popup, Kid child);
	KESTREL_API void Kpopup_set_offset(Kid popup, float horizontal_offset, float vertical_offset);
	/** @brief PlacementTarget and Placement: the popup opens next to `target`. */
	KESTREL_API void Kpopup_set_placement_target(Kid popup, Kid target, KPlacementMode mode);
	/** @brief IsOpen. A popup opens only once it is in a tree that belongs to a root. */
	KESTREL_API void Kpopup_set_open(Kid popup, int open);
	KESTREL_API int Kpopup_is_open(Kid popup);
	/** @brief StaysOpen: K_FALSE (the default of ComboBox) closes the popup on a click outside. */
	KESTREL_API void Kpopup_set_stays_open(Kid popup, int stays_open);

#ifdef __cplusplus
}
#endif

#endif
