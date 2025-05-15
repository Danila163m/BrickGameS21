#include "backend.h"

static const int figures[7][4][FIGURE_SIZE][FIGURE_SIZE] = {
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} },
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }
    },
    {
        { {0,1,0,0}, {0,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,1,1,0}, {0,0,0,0} }
    },
    {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,0,0} }
    },
    {
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }
    },
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }
    },
    {
        { {0,1,1,0}, {0,0,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,0,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }
    },
    {
        { {0,0,1,0}, {0,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,1}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,1,1}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }
    }
};

static game_info_t *game_instance = NULL;

game_info_t *get_game(void) {
    if (game_instance == NULL) {
        game_instance = malloc(sizeof(game_info_t));
        if (game_instance == NULL) {
            terminate_with_error("Memory allocation failed");
        }
        init_board();
        init_next_figure();
        init_current_figure();
        game_instance->score = 0;
        game_instance->high_score = load_high_score();
        game_instance->level = 1;
        game_instance->speed = 50;
        game_instance->paused = false;
        game_instance->game_over = false;
        game_instance->game_started = false;
        game_instance->current_x = BOARD_WIDTH / 2;
        game_instance->current_y = 0;
        game_instance->current_orientation = 0;
        game_instance->next_orientation = 0;
        game_instance->current_type = 0;
        game_instance->next_type = 0;
        game_instance->tick = 0;
    }
    return game_instance;
}

void init_board(void) {
    game_info_t *game = get_game();
    if (game->board == NULL) {
        game->board = malloc(BOARD_HEIGHT * sizeof(int *));
        if (game->board == NULL) {
            terminate_with_error("Memory allocation failed");
        }
        game->board[0] = calloc(BOARD_HEIGHT * BOARD_WIDTH, sizeof(int));
        if (game->board[0] == NULL) {
            free(game->board);
            terminate_with_error("Memory allocation failed");
        }
        for (int i = 1; i < BOARD_HEIGHT; i++) {
            game->board[i] = game->board[0] + i * BOARD_WIDTH;
        }
    }
}

void init_next_figure(void) {
    game_info_t *game = get_game();
    if (game->next_figure == NULL) {
        game->next_figure = malloc(FIGURE_SIZE * sizeof(int *));
        if (game->next_figure == NULL) {
            terminate_with_error("Memory allocation failed");
        }
        game->next_figure[0] = calloc(FIGURE_SIZE * FIGURE_SIZE, sizeof(int));
        if (game->next_figure[0] == NULL) {
            free(game->next_figure);
            terminate_with_error("Memory allocation failed");
        }
        for (int i = 1; i < FIGURE_SIZE; i++) {
            game->next_figure[i] = game->next_figure[0] + i * FIGURE_SIZE;
        }
    }
}

void init_current_figure(void) {
    game_info_t *game = get_game();
    if (game->current_figure == NULL) {
        game->current_figure = malloc(FIGURE_SIZE * sizeof(int *));
        if (game->current_figure == NULL) {
            terminate_with_error("Memory allocation failed");
        }
        game->current_figure[0] = calloc(FIGURE_SIZE * FIGURE_SIZE, sizeof(int));
        if (game->current_figure[0] == NULL) {
            free(game->current_figure);
            terminate_with_error("Memory allocation failed");
        }
        for (int i = 1; i < FIGURE_SIZE; i++) {
            game->current_figure[i] = game->current_figure[0] + i * FIGURE_SIZE;
        }
    }
}

void drop_next_figure(void) {
    game_info_t *game = get_game();
    game->next_type = rand() % 7;
    game->next_orientation = rand() % 4;
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            game->next_figure[i][j] = figures[game->next_type][game->next_orientation][i][j];
        }
    }
}

void drop_current_figure(void) {
    game_info_t *game = get_game();
    game->current_type = game->next_type;
    game->current_orientation = game->next_orientation;
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            game->current_figure[i][j] = game->next_figure[i][j];
        }
    }
    drop_next_figure();
    game->current_x = BOARD_WIDTH / 2 - 2;
    game->current_y = 0;
}

void rotate_current_figure(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) {
        return;
    }
    int new_orientation = (game->current_orientation + 1) % 4;
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if (game->current_figure[i][j] &&
                game->board[game->current_y + i][game->current_x + j] &&
                figures[game->current_type][new_orientation][i][j]) {
                return;
            }
        }
    }
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            game->current_figure[i][j] = figures[game->current_type][new_orientation][i][j];
        }
    }
    game->current_orientation = new_orientation;
}

void move_down(void) {
    game_info_t *game = get_game();
    if (!game->paused && !game->game_over) {
        game->current_y++;
    }
}

void move_left(void) {
    game_info_t *game = get_game();
    if (!game->paused) {
        game->current_x--;
    }
}

void move_right(void) {
    game_info_t *game = get_game();
    if (!game->paused) {
        game->current_x++;
    }
}

void hard_drop(void) {
    game_info_t *game = get_game();
    if (game->paused || game->game_over) {
        return;
    }
    while (true) {
        game->current_y++;
        for (int i = 0; i < FIGURE_SIZE; i++) {
            for (int j = 0; j < FIGURE_SIZE; j++) {
                if (game->current_figure[i][j] &&
                    (game->current_y + i >= BOARD_HEIGHT ||
                     game->board[game->current_y + i][game->current_x + j])) {
                    game->current_y--;
                    return;
                }
            }
        }
    }
}

void check_collision(void) {
    game_info_t *game = get_game();
    if (game->current_x < 0) {
        game->current_x = 0;
    }
    if (game->current_x + FIGURE_SIZE > BOARD_WIDTH) {
        game->current_x = BOARD_WIDTH - FIGURE_SIZE;
    }
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if (game->current_figure[i][j]) {
                int y = game->current_y + i;
                int x = game->current_x + j;
                if (y >= BOARD_HEIGHT || (y >= 0 && game->board[y][x])) {
                    for (int m = 0; m < FIGURE_SIZE; m++) {
                        for (int n = 0; n < FIGURE_SIZE; n++) {
                            if (game->current_figure[m][n] && y - i + m >= 0) {
                                game->board[y - i + m][x - j + n] = game->current_figure[m][n];
                            }
                        }
                    }
                    drop_current_figure();
                    check_and_remove_lines();
                    return;
                }
            }
        }
    }
}

void check_and_remove_lines(void) {
    game_info_t *game = get_game();
    int lines_removed = 0;
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        if (is_line_full(row)) {
            remove_line(row);
            lines_removed++;
            row--;
        }
    }
    switch (lines_removed) {
        case 1:
            game->score += 100;
            break;
        case 2:
            game->score += 300;
            break;
        case 3:
            game->score += 700;
            break;
        case 4:
            game->score += 1500;
            break;
        default:
            break;
    }
    if (game->score > game->high_score) {
        save_high_score(game->score);
        game->high_score = game->score;
    }
    game->level = game->score / 600 + 1;
    if (game->level > 10) {
        game->level = 10;
    }
}

bool is_line_full(int row) {
    game_info_t *game = get_game();
    for (int col = 0; col < BOARD_WIDTH; col++) {
        if (game->board[row][col] == 0) {
            return false;
        }
    }
    return true;
}

void remove_line(int row) {
    game_info_t *game = get_game();
    for (int r = row; r > 0; r--) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
            game->board[r][c] = game->board[r - 1][c];
        }
    }
    for (int c = 0; c < BOARD_WIDTH; c++) {
        game->board[0][c] = 0;
    }
}

int load_high_score(void) {
    FILE *file = fopen("max_score.txt", "r");
    int high_score = 0;
    if (file != NULL) {
        fscanf(file, "%d", &high_score);
        fclose(file);
    }
    return high_score;
}

void save_high_score(int high_score) {
    FILE *file = fopen("max_score.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", high_score);
        fclose(file);
    }
}

void toggle_pause(void) {
    game_info_t *game = get_game();
    game->paused = !game->paused;
}

void start_game(void) {
    game_info_t *game = get_game();
    game->game_started = true;
    game->game_over = false;
    game->score = 0;
    game->level = 1;
    game->speed = 50;
    game->paused = false;
    drop_next_figure();
    drop_current_figure();
}

void set_game_over(void) {
    game_info_t *game = get_game();
    game->game_over = true;
}

void free_game(game_info_t *game) {
    if (game == NULL) {
        return;
    }
    if (game->board != NULL) {
        free(game->board[0]);
        free(game->board);
    }
    if (game->next_figure != NULL) {
        free(game->next_figure[0]);
        free(game->next_figure);
    }
    if (game->current_figure != NULL) {
        free(game->current_figure[0]);
        free(game->current_figure);
    }
    free(game);
    game_instance = NULL;
}

void terminate_with_error(const char *msg) {
    quit_ncurses();
    perror(msg);
    exit(EXIT_FAILURE);
}

void quit_ncurses(void) {
    endwin();
}
