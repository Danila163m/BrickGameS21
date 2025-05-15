#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "backend.h"

// Прототипы функций из твоего GUI-модуля
void init_gui(void);
void quit_ncurses(void);
user_action_t get_user_action(void);
void handle_user_input(user_action_t action, bool hold);
void draw_game(game_info_t *game);
void start_game(void);
void move_down(void);
bool check_collision(void);
void free_game(game_info_t *game);

int main(void) {
    init_gui();
    srand((unsigned int)time(NULL));

    game_info_t *game = get_game();
    user_action_t action = action_start;

    // Ждем, пока пользователь нажмет старт или выйти
    while ((action = get_user_action()) != action_terminate) {
        draw_game(game);
        if (action == action_start) {
            start_game();
            break;
        }
        napms(50); // Пауза 50 мс
    }

    while (game->game_started && action != action_terminate) {
        game->tick = 20;
        while (game->tick > 0) {
            game->tick--;
            action = get_user_action();
            handle_user_input(action, false);
            check_collision();

            if (game->game_over || action == action_terminate) {
                mvprintw(14, 16, "GAME OVER");
                refresh();
                napms(1000);
                action = action_terminate;
                game->game_started = false;
                game->game_over = false;
                break;
            }

            napms(game->speed - game->level * 3);
            erase();
            draw_game(game);
        }
        if (!game->game_over) {
            move_down();
        }
        refresh();
    }

    free_game(game);
    quit_ncurses();
    return 0;
}
