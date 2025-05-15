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

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];         // Игровое поле
    int next_figure[FIGURE_SIZE][FIGURE_SIZE];    // Следующая фигура
    int current_figure[FIGURE_SIZE][FIGURE_SIZE]; // Текущая фигура
    int current_x;                                 // Координата X текущей фигуры
    int current_y;                                 // Координата Y текущей фигуры
    int level;                                    // Текущий уровень
    int score;                                    // Текущий счет
    int high_score;                               // Рекордный счет
    bool paused;                                  // Флаг паузы
    bool game_started;                            // Флаг начала игры
} game_info_t;

// Инициализация ncurses и графического интерфейса
void init_gui(void);

// Отрисовка всего игрового окна и интерфейса
void draw_game(const game_info_t *game);

// Отрисовка окна (границ)
void draw_window(void);

// Отрисовка прямоугольника с заданными координатами (границы окна и панелей)
void draw_rectangle(int top, int bottom, int left, int right);

// Отрисовка игровой информации (уровень, счет, пауза, инструкции)
void draw_info(const game_info_t *game);

// Отрисовка следующей фигуры
void draw_next(const int **next_figure);

// Отрисовка игрового поля
void draw_field(const int **board);

// Отрисовка текущей активной фигуры
void draw_current(const game_info_t *game);

#endif // CLI_H

