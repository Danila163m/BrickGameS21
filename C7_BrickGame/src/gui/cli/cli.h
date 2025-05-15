#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define FIGURE_SIZE 4

#define WINDOW_TOP 1
#define WINDOW_LEFT 1
#define FIELD_HEIGHT BOARD_HEIGHT
#define FIELD_WIDTH BOARD_WIDTH
#define WINDOW_WIDTH 80

typedef enum {
    ACTION_NONE,
    ACTION_START,
    ACTION_TERMINATE,
    ACTION_PAUSE,
    ACTION_MOVE_LEFT,
    ACTION_MOVE_RIGHT,
    ACTION_ROTATE,
    ACTION_DROP,
    ACTION_MOVE_DOWN
} user_action_t;

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int next_figure[FIGURE_SIZE][FIGURE_SIZE];
    int current_figure[FIGURE_SIZE][FIGURE_SIZE];
    int current_x;
    int current_y;
    int level;
    int score;
    int high_score;
    int speed;
    int tick;
    bool paused;
    bool game_started;
    bool game_over;
} game_info_t;

// GUI functions
void init_gui(void);
void quit_ncurses(void);
void draw_game(const game_info_t *game);
void draw_window(void);
void draw_rectangle(int top, int bottom, int left, int right);
void draw_info(const game_info_t *game);
void draw_next(const int **next_figure);
void draw_field(const int **board);
void draw_current(const game_info_t *game);

// Game control functions (to be implemented in game logic)
game_info_t* get_game(void);
user_action_t get_user_action(void);
void start_game(void);
void handle_user_input(user_action_t action, bool from_timer);
void check_collision(void);
void move_down(void);
void free_game(game_info_t *game);

#endif // CLI_H


