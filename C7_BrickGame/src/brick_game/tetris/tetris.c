#include "../../gui/cli/cli.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

int main(void) {
    init_gui();
    srand((unsigned int)time(NULL));

    game_info_t *game = get_game();
    user_action_t action = ACTION_START;

    // Wait for user to start the game or exit
    while ((action = get_user_action()) != ACTION_TERMINATE) {
        draw_game(game);
        if (action == ACTION_START) {
            start_game();
            break;
        }
        napms(50);
    }

    while (game->game_started && action != ACTION_TERMINATE) {
        game->tick = 20;
        while (game->tick > 0) {
            game->tick--;
            action = get_user_action();
            handle_user_input(action, false);
            check_collision();

            if (game->game_over || action == ACTION_TERMINATE) {
                mvprintw(14, 16, "GAME OVER");
                refresh();
                napms(1000);
                action = ACTION_TERMINATE;
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
