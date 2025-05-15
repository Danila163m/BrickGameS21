#ifndef CLI_H
#define CLI_H

#include "../../brick_game/tetris/backend.h"

#define WINDOW_TOP 1
#define WINDOW_LEFT 7
#define WINDOW_HEIGHT 24
#define WINDOW_WIDTH 41

#define FIELD_HEIGHT 22
#define FIELD_WIDTH 24

void init_gui(void);
void draw_game(const game_info_t *game);
void draw_window(void);
void draw_rectangle(int top, int bottom, int left, int right);
void draw_info(const game_info_t *game);
void draw_next(const int **next_figure);
void draw_field(const int **board);
void draw_current(const game_info_t *game);

#endif
