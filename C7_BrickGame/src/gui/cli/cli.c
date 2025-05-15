#include "cli.h"
#include <ncurses.h>
#include <stdlib.h>

void init_gui(void) {
    initscr();
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_GREEN, -1);

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

void draw_game(const game_info_t *game) {
    draw_window();
    draw_field((const int **)game->board);
    draw_next((const int **)game->next_figure);
    draw_current(game);
    draw_info(game);
    refresh();
}

void draw_window(void) {
    draw_rectangle(WINDOW_TOP, WINDOW_TOP + FIELD_HEIGHT + 1, WINDOW_LEFT, WINDOW_LEFT + FIELD_WIDTH + 1);
    draw_rectangle(WINDOW_TOP + 1, WINDOW_TOP + FIELD_HEIGHT, WINDOW_LEFT + 2, WINDOW_LEFT + FIELD_WIDTH);
    draw_rectangle(WINDOW_TOP + 1, WINDOW_TOP + FIELD_HEIGHT, WINDOW_LEFT + FIELD_WIDTH + 3, WINDOW_LEFT + FIELD_WIDTH + 20);

    draw_rectangle(3, 5, WINDOW_LEFT + FIELD_WIDTH + 4, WINDOW_LEFT + FIELD_WIDTH + 20);
    draw_rectangle(7, 9, WINDOW_LEFT + FIELD_WIDTH + 4, WINDOW_LEFT + FIELD_WIDTH + 20);
    draw_rectangle(11, 17, WINDOW_LEFT + FIELD_WIDTH + 4, WINDOW_LEFT + FIELD_WIDTH + 20);
    draw_rectangle(19, 21, WINDOW_LEFT + FIELD_WIDTH + 4, WINDOW_LEFT + FIELD_WIDTH + 20);

    attron(A_BOLD);
    mvprintw(3, WINDOW_LEFT + FIELD_WIDTH + 10, "LEVEL");
    mvprintw(7, WINDOW_LEFT + FIELD_WIDTH + 10, "SCORE");
    mvprintw(11, WINDOW_LEFT + FIELD_WIDTH + 10, "NEXT");
    mvprintw(19, WINDOW_LEFT + FIELD_WIDTH + 9, "RECORD");
    attroff(A_BOLD);
}

void draw_rectangle(int top, int bottom, int left, int right) {
    mvaddch(top, left, ACS_ULCORNER);
    for (int x = left + 1; x < right; x++) {
        mvaddch(top, x, ACS_HLINE);
    }
    mvaddch(top, right, ACS_URCORNER);

    for (int y = top + 1; y < bottom; y++) {
        mvaddch(y, left, ACS_VLINE);
        mvaddch(y, right, ACS_VLINE);
    }

    mvaddch(bottom, left, ACS_LLCORNER);
    for (int x = left + 1; x < right; x++) {
        mvaddch(bottom, x, ACS_HLINE);
    }
    mvaddch(bottom, right, ACS_LRCORNER);
}

void draw_info(const game_info_t *game) {
    attron(COLOR_PAIR(3));
    mvprintw(1, WINDOW_WIDTH / 2 + 8, "TETRIS");
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));
    mvprintw(4, WINDOW_LEFT + FIELD_WIDTH + 10, "%d", game->level);
    mvprintw(8, WINDOW_LEFT + FIELD_WIDTH + 10, "%d", game->score);
    mvprintw(20, WINDOW_LEFT + FIELD_WIDTH + 10, "%d", game->high_score);
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(2));
    if (game->paused) {
        mvprintw(4 + rand() % 18, 14, "GAME IS PAUSED");
        napms(200);
    }
    if (!game->game_started) {
        mvprintw(11, 14, "PRESS \"ENTER\"");
        mvprintw(12, 12, "TO START THE GAME");
        mvprintw(13, 18, "OR");
        mvprintw(14, 15, "PRESS \"ESC\"");
        mvprintw(15, 12, "TO EXIT THE GAME");
    }
    attroff(COLOR_PAIR(2));

    mvprintw(25, 8, "Press:");
    mvprintw(25, 14, "Start: 'Enter'");
    mvprintw(26, 14, "Pause: 'p'");
    mvprintw(27, 14, "Exit: 'q'");
    mvprintw(28, 14, "Arrows to move: '<' '>'");
    mvprintw(29, 14, "Space to rotate: 'Space'");
    mvprintw(30, 14, "Arrow down to plant: 'v'");
}

void draw_next(const int **next_figure) {
    if (next_figure == NULL) return;
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if (next_figure[i][j]) {
                mvprintw(14 + i, WINDOW_LEFT + FIELD_WIDTH + 10 + 2 * j, "[]");
            }
        }
    }
}

void draw_field(const int **board) {
    if (board == NULL) return;
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j]) {
                mvprintw(WINDOW_TOP + 2 + i, WINDOW_LEFT + 4 + 2 * j, "[]");
            }
        }
    }
}

void draw_current(const game_info_t *game) {
    if (game == NULL) return;
    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if (game->current_figure[i][j]) {
                mvprintw(game->current_y + WINDOW_TOP + 2 + i,
                         game->current_x * 2 + WINDOW_LEFT + 3 + 2 * j,
                         "[]");
            }
        }
    }
}
