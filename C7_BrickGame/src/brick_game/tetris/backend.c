#include "backend.h"

static const int figures[7][4][FIGURE_SIZE][FIGURE_SIZE] = {
    // I
    {
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}}
    },
    // J
    {
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}
    },
    // L
    {
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    // O
    {
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}
    },
    // S
    {
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}
    },
    // T
    {
        {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    // Z
    {
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}
    }
};

static game_info_t *game_instance = NULL;

static int** alloc_2d_int(int rows, int cols) {
    int **arr = malloc(rows * sizeof(int*));
    if (!arr) return NULL;
    arr[0] = calloc(rows * cols, sizeof(int));
    if (!arr[0]) {
        free(arr);
        return NULL;
    }
    for (int i = 1; i < rows; i++) {
        arr[i] = arr[0] + i * cols;
    }
    return arr;
}

static void free_2d_int(int **arr) {
    if (!arr) return;
    free(arr[0]);
    free(arr);
}

game_info_t *get_game(void) {
    if (!game_instance) {
        game_instance = malloc(sizeof(game_info_t));
        if (!game_instance) terminate_with_error("Failed to allocate game_info_t");

        game_instance->board = alloc_2d_int(BOARD_HEIGHT, BOARD_WIDTH);
        game_instance->next_figure = alloc_2d_int(FIGURE_SIZE, FIGURE_SIZE);
        game_instance->current_figure = alloc_2d_int(FIGURE_SIZE, FIGURE_SIZE);
        if (!game_instance->board || !game_instance->next_figure || !game_instance->current_figure)
            terminate_with_error("Failed to allocate game arrays");

        game_instance->score = 0;
        game_instance->high_score = load_high_score();
        game_instance->level = 1;
        game_instance->speed = 50;
        game_instance->paused = false;
        game_instance->game_over = false;
        game_instance->game_started = false;
        game_instance->current_x = BOARD_WIDTH / 2 - 2;
        game_instance->current_y = 0;
        game_instance->current_orientation = 0;
        game_instance->next_orientation = 0;
        game_instance->current_type = 0;
        game_instance->next_type = 0;
        game_instance->tick = 0;

        drop_next_figure();
        drop_current_figure();
    }
    return game_instance;
}

bool check_collision(void) {
    game_info_t *game = get_game();  // Получаем текущее состояние игры
    tetromino_t *t = &game->current_tetromino;

    for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
            if (t->shape[y][x]) {
                int field_x = t->x + x;
                int field_y = t->y + y;

                if (field_x < 0 || field_x >= FIELD_WIDTH || field_y < 0 || field_y >= FIELD_HEIGHT) {
                    return true;  // Столкновение с границей
                }

                if (game->field[field_y][field_x]) {
                    return true;  // Столкновение с занятым полем
                }
            }
        }
    }

    return false;  // Столкновений нет
}

void init_board(void) {
    game_info_t *game = get_game();
    for (int r = 0; r < BOARD_HEIGHT; r++)
        for (int c = 0; c < BOARD_WIDTH; c++)
            game->board[r][c] = 0;
}

void init_next_figure(void) {
    game_info_t *game = get_game();
    for (int i = 0; i < FIGURE_SIZE; i++)
        for (int j = 0; j < FIGURE_SIZE; j++)
            game->next_figure[i][j] = 0;
}

void init_current_figure(void) {
    game_info_t *game = get_game();
    for (int i = 0; i < FIGURE_SIZE; i++)
        for (int j = 0; j < FIGURE_SIZE; j++)
            game->current_figure[i][j] = 0;
}

void drop_next_figure(void) {
    game_info_t *game = get_game();
    game->next_type = rand() % 7;
    game->next_orientation = rand() % 4;
    for (int i = 0; i < FIGURE_SIZE; i++)
        for (int j = 0; j < FIGURE_SIZE; j++)
            game->next_figure[i][j] = figures[game->next_type][game->next_orientation][i][j];
}

void drop_current_figure(void) {
    game_info_t *game = get_game();
    game->current_type = game->next_type;
    game->current_orientation = game->next_orientation;
    for (int i = 0; i < FIGURE_SIZE; i++)
        for (int j = 0; j < FIGURE_SIZE; j++)
            game->current_figure[i][j] = game->next_figure[i][j];

    drop_next_figure();
    game->current_x = BOARD_WIDTH / 2 - 2;
    game->current_y = 0;
}

static bool can_place_figure(int x, int y, int type, int orientation) {
    game_info_t *game = get_game();
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if (figures[type][orientation][i][j]) {
                int bx = x + j;
                int by = y + i;
                if (bx < 0 || bx >= BOARD_WIDTH || by >= BOARD_HEIGHT) return false;
                if (by >= 0 && game->board[by][bx]) return false;
            }
        }
    }
    return true;
}

void rotate_current_figure(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) return;
    int new_orientation = (game->current_orientation + 1) % 4;
    if (can_place_figure(game->current_x, game->current_y, game->current_type, new_orientation)) {
        game->current_orientation = new_orientation;
        for (int i = 0; i < FIGURE_SIZE; i++)
            for (int j = 0; j < FIGURE_SIZE; j++)
                game->current_figure[i][j] = figures[game->current_type][new_orientation][i][j];
    }
}

void move_left(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) return;
    int new_x = game->current_x - 1;
    if (can_place_figure(new_x, game->current_y, game->current_type, game->current_orientation))
        game->current_x = new_x;
}

void move_right(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) return;
    int new_x = game->current_x + 1;
    if (can_place_figure(new_x, game->current_y, game->current_type, game->current_orientation))
        game->current_x = new_x;
}

void move_down(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) return;
    int new_y = game->current_y + 1;
    if (can_place_figure(game->current_x, new_y, game->current_type, game->current_orientation)) {
        game->current_y = new_y;
    } else {
        for (int i = 0; i < FIGURE_SIZE; i++) {
            for (int j = 0; j < FIGURE_SIZE; j++) {
                if (game->current_figure[i][j]) {
                    int bx = game->current_x + j;
                    int by = game->current_y + i;
                    if (by >= 0 && by < BOARD_HEIGHT && bx >= 0 && bx < BOARD_WIDTH)
                        game->board[by][bx] = 1;
                }
            }
        }
        check_and_remove_lines();
        drop_current_figure();
        if (!can_place_figure(game->current_x, game->current_y, game->current_type, game->current_orientation))
            set_game_over();
    }
}

void hard_drop(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) return;
    while (can_place_figure(game->current_x, game->current_y + 1, game->current_type, game->current_orientation))
        game->current_y++;
    move_down();
}

bool is_line_full(int row) {
    game_info_t *game = get_game();
    for (int c = 0; c < BOARD_WIDTH; c++) {
        if (game->board[row][c] == 0) return false;
    }
    return true;
}

void remove_line(int row) {
    game_info_t *game = get_game();
    for (int r = row; r > 0; r--)
        for (int c = 0; c < BOARD_WIDTH; c++)
            game->board[r][c] = game->board[r - 1][c];
    for (int c = 0; c < BOARD_WIDTH; c++)
        game->board[0][c] = 0;
}

void check_and_remove_lines(void) {
    game_info_t *game = get_game();
    int lines_cleared = 0;
    for (int r = 0; r < BOARD_HEIGHT; r++) {
        if (is_line_full(r)) {
            remove_line(r);
            lines_cleared++;
            r--; // Проверяем ту же строку снова, так как она сдвинулась вниз
        }
    }

    switch (lines_cleared) {
        case 1: game->score += 100 * game->level; break;
        case 2: game->score += 300 * game->level; break;
        case 3: game->score += 500 * game->level; break;
        case 4: game->score += 800 * game->level; break;
    }

    int new_level = game->score / 1000 + 1;
    if (new_level > game->level) {
        game->level = new_level;
        game->speed = 50 - (game->level - 1) * 5;
        if (game->speed < 10) game->speed = 10;
    }

    if (game->score > game->high_score) {
        game->high_score = game->score;
        save_high_score(game->high_score);
    }
}

void handle_user_input(user_action_t action, bool hold) {
    (void)hold;
    game_info_t *game = get_game();
    if (game->game_over) return;
    switch (action) {
        case action_left: move_left(); break;
        case action_right: move_right(); break;
        case action_down: move_down(); break;
        case action_hard_drop: hard_drop(); break;
        case action_rotate: rotate_current_figure(); break;
        case action_pause: toggle_pause(); break;
        case action_start: start_game(); break;
        case action_terminate: set_game_over(); break;
        default: break;
    }
}

user_action_t get_user_action(void) {
    return action_start; // Заглушка
}

int load_high_score(void) {
    FILE *f = fopen("highscore.dat", "rb");
    if (!f) return 0;
    int highscore = 0;
    fread(&highscore, sizeof(int), 1, f);
    fclose(f);
    return highscore;
}

void save_high_score(int high_score) {
    FILE *f = fopen("highscore.dat", "wb");
    if (!f) return;
    fwrite(&high_score, sizeof(int), 1, f);
    fclose(f);
}

void toggle_pause(void) {
    game_info_t *game = get_game();
    if (!game->game_started) return;
    game->paused = !game->paused;
}

void start_game(void) {
    game_info_t *game = get_game();
    init_board();
    drop_next_figure();
    drop_current_figure();
    game->score = 0;
    game->level = 1;
    game->speed = 50;
    game->paused = false;
    game->game_over = false;
    game->game_started = true;
}

void set_game_over(void) {
    game_info_t *game = get_game();
    game->game_over = true;
    game->game_started = false;
}

void free_game(game_info_t *game) {
    if (!game) return;
    free_2d_int(game->board);
    free_2d_int(game->next_figure);
    free_2d_int(game->current_figure);
    free(game);
    game_instance = NULL;
}

void terminate_with_error(const char *msg) {
    endwin();
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void quit_ncurses(void) {
    endwin();
}

