// SPDX-License-Identifier: LGPL-2.1-or-later
/* What the widgets share: the overlay, the pickers, the tiles and the hub keep
 * their own state in their own file and meet here. Internal to libzrnui. */
#ifndef ZUI_WIDGETS_H
#define ZUI_WIDGETS_H
#include "zui-internal.h"

#define M (app->style->m)

#define PICK_ROW 40

#define PICK_OPEN_MS 250

#define PICK_CLOSE_MS 200

enum { OV_MENU, OV_DIALOG, OV_BOARD, OV_DATE, OV_TIME };

struct col {
	int n, lo, pad;		/* n values from lo, shown with at least pad digits */
	const char *const *names;	/* or these */
	bool loop;
	float pos, target, v;	/* pos: which value is in the middle, between values while it moves */
	float x, w;
};

struct ov {
	int kind;
	uint32_t t0, t_close, last;
	bool closing;
	zui_choice_fn cb;
	void *data;
	char *title, *msg;
	char **item;
	bool *live;
	int count;
	float y;			/* a menu's top; a board's top */
	int pressed;			/* what the finger is on: item, button, cell */
	float down_x, down_y;
	bool moved;
	/* pickers */
	struct col col[3];
	int ncol, drag;
	float drag_y0, drag_pos0, last_y;
	uint32_t last_t;
	zui_node *source;
};

/* ---- the geometry of each ---- */
#define BOARD_COLS 4

#define BOARD_GAP 9

#define BOARD_FONT 27

#define BOARD_INSET 9

#define BOARD_STAGGER 12.0f

#define SWIVEL_IN_MS 300.0f

#define SWIVEL_OUT_MS 200.0f

/* pickers: a bar of two round buttons at the bottom, columns over it */
#define BAR_H 70

#define ROW_H 62

/* the widgets' own files */
/* common.c */
bool dotted(const zui_app *app);
float clamp01(float v);
char *dupstr(const char *s);
void text_at(struct zt_slot *slot, const char *text, int font, float size, float x, float y,
		uint32_t color, float max_w);
float text_now(const char *text, int font, float size, float x, float y, uint32_t color, float max_w);
bool in_footer(const zui_node *n);
float screen_y(const zui_node *n);
/* hub.c */
float layout_hub(zui_node *hub);
void draw_hub(zui_app *app, zui_node *hub, const struct zg_xf *base, uint32_t now);
/* check.c */
void draw_check(zui_app *app, zui_node *n, uint32_t now);
void radio_chosen(zui_node *n);
/* ring.c */
void draw_ring(zui_app *app, zui_node *n, uint32_t now);
bool has_ring(zui_node *n);
/* field.c */
void draw_suggestions(zui_app *app);
/* picker.c */
float layout_picker(zui_node *n);
void draw_picker(zui_app *app, zui_node *n, uint32_t now);
void picker_tapped(zui_app *app, zui_node *n);
bool pickers_moving(zui_node *n, uint32_t now);
/* tiles.c */
float layout_tiles(zui_node *g, float x, float y, float w);
void draw_tile(zui_app *app, zui_node *n, const struct zg_xf *base, uint32_t now);
bool has_flip(zui_node *n, uint32_t now);
/* overlay.c */
void ov_free(struct ov *o);
struct ov *ov_open(zui_app *app, int kind);
void ov_items(struct ov *o, const char *const *items, int count);
bool ov_moving(const struct ov *o, uint32_t now);
void ov_step(zui_app *app, struct ov *o, uint32_t now);
/* menu.c */
int menu_row_at(zui_app *app, struct ov *o, float x, float y);
void draw_menu(zui_app *app, struct ov *o, float a);
/* dialog.c */
int dlg_button_at(zui_app *app, struct ov *o, float x, float y);
void draw_dialog(zui_app *app, struct ov *o, float a, uint32_t now);
/* jump_list.c */
int board_cell_at(zui_app *app, struct ov *o, float x, float y);
void draw_board(zui_app *app, struct ov *o, uint32_t now);
/* datetime.c */
int pick_button_at(zui_app *app, float x, float y);
void open_picker(zui_node *n);
void picker_row_tapped(zui_node *n, void *data);
void pick_commit(zui_app *app, struct ov *o);
void pick_fit_days(struct ov *o);
void draw_pick(zui_app *app, struct ov *o, float a, uint32_t now);
/* freeing what a node keeps in ext, and forgetting it */
extern const struct zui_class zui_picker_class, zui_tile_class, zui_tiles_class, zui_hub_class, zui_hubsec_class,
	zui_check_class, zui_radio_class, zui_ring_class;
void picker_free(zui_node *n);
void tile_free(zui_node *n);
void hub_free(zui_node *n);
void stamp_free(zui_node *n);
void suggest_free(zui_node *n);
void zui_overlay_forget(zui_app *app, const zui_node *n);

#endif
