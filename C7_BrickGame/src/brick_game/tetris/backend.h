#ifndef BACKEND_H
#define BACKEND_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define FIGURE_SIZE 4

typedef enum {
    action_start,
    action_pause,
    action_terminate,
    action_left,
    action_right,
    action_rotate,
    action_down,
    action_hard_drop
} user_action_t;

typedef struct {
    int **board;
    int **next_figure;
    int **current_figure;
    int score;
    int high_score;
    int level;
    int speed;
    bool paused;
    int current_orientation;
    int next_orientation;
    int current_type;
    int next_type;
    int current_x;
    int current_y;
    int tick;
    bool game_over;
    bool game_started;
} game_info_t;

bool check_collision(void);
void init_board(void);
void init_next_figure(void);
void init_current_figure(void);
game_info_t *get_game(void);
void handle_user_input(user_action_t action, bool hold);
user_action_t get_user_action(void);
void drop_next_figure(void);
void drop_current_figure(void);
void rotate_current_figure(void);
void move_down(void);
void move_left(void);
void move_right(void);
void hard_drop(void);
void check_and_remove_lines(void);
bool is_line_full(int row);
void remove_line(int row);
int load_high_score(void);
void save_high_score(int high_score);
void toggle_pause(void);
void start_game(void);
void set_game_over(void);
void free_game(game_info_t *game);
void terminate_with_error(const char *msg);
void quit_ncurses(void);

#endif
