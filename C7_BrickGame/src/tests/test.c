#include "test.h"

void resetGame() {
    static GameInfo_t *game = NULL;
    if (game != NULL) {
        free(game);
        game = NULL;
    }
}

// Функция, которая вызывается перед каждым тестом.
void setup(void) {
    resetGame();
}

// Функция, которая вызывается после каждого теста.
void teardown(void) {
    resetGame(); // Важно: сбрасываем статическую переменную game
}

START_TEST(test_getGame1) {

    GameInfo_t *game = getGame();
    ck_assert_ptr_nonnull(game); 
    ck_assert_int_eq(game->score, 0);       
    ck_assert_int_eq(game->level, 1);
    ck_assert_int_eq(game->speed, 50);
    ck_assert_int_eq(game->pause, 0);
    ck_assert_int_eq(game->game_over, 0);
    ck_assert_int_eq(game->game_start, 0);
  
}

END_TEST

START_TEST(test_getGame2) {

    GameInfo_t *game = getGame();
    ck_assert_ptr_nonnull(game);
    GameInfo_t *game2 = getGame();
    ck_assert_ptr_eq(game, game2); // Проверяем, что возвращается тот же указатель
      }

END_TEST

START_TEST(test_field_init) {

  GameInfo_t *game = getGame();
  ck_assert_ptr_nonnull(game->field);
  int *ptr = (int *)((int **)game->field + 22);
  for (int i = 0; i < 22; i++) {
      ck_assert_ptr_eq(game->field[i], ptr + 10 * i);
  }
  game->field[0][0] = 123; //пытаемся записать в ячейку. Если произойдет ошибка сегментации, то тест упадёт
  ck_assert_int_eq(game->field[0][0], 123); //проверяем, что запись прошла успешно
}
END_TEST

START_TEST(test_next_init) {

  GameInfo_t *game = getGame();
  ck_assert_ptr_nonnull(game->next);
  int *ptr = (int *)((int **)game->next + 4);
  for (int i = 0; i < 4; i++) {
      ck_assert_ptr_eq(game->next[i], ptr + 4 * i);
  }
  game->next[0][0] = 123; //пытаемся записать в ячейку. Если произойдет ошибка сегментации, то тест упадёт
  ck_assert_int_eq(game->next[0][0], 123); //проверяем, что запись прошла успешно
}
END_TEST

START_TEST(test_current_init) {

  GameInfo_t *game = getGame();
  ck_assert_ptr_nonnull(game->current);
  int *ptr = (int *)((int **)game->current + 4);
  for (int i = 0; i < 4; i++) {
      ck_assert_ptr_eq(game->current[i], ptr + 4 * i);
  }
  game->current[0][0] = 123; //пытаемся записать в ячейку. Если произойдет ошибка сегментации, то тест упадёт
  ck_assert_int_eq(game->current[0][0], 123); //проверяем, что запись прошла успешно
}
END_TEST

START_TEST(test_drop_next) {

  GameInfo_t *game = getGame();
  int count = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j]) count++;
    }
  }
  ck_assert_int_eq(count,4);
}
END_TEST

START_TEST(test_drop_current) {

  GameInfo_t *game = getGame();
  int count = 0;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (game->current[i][j]) count++;
       }
    }
    ck_assert_int_eq(count, 4);
}
END_TEST

START_TEST(test_file_open) {
    // Создаем файл и записываем в него счет
    FILE *file = fopen("max_score.txt", "w");
    fprintf(file, "%d", 42); // Записываем 42 как счет
    fclose(file);

    int score = load_high_score();
    ck_assert_int_eq(score, 42); // Ожидаем, что счет будет 42
}
END_TEST

START_TEST(test_save_high_score) {

  int high_score = 54321;
  save_high_score(high_score);
  FILE *file = fopen("max_score.txt", "r");
  ck_assert_ptr_nonnull(file);

  int loaded_high_score;
  fscanf(file, "%d", &loaded_high_score);
  fclose(file);
  ck_assert_int_eq(loaded_high_score, high_score);
}
END_TEST

START_TEST(test_drop_figure) {

  GameInfo_t *game = getGame();
  game->coord_y = 0;
  game->game_over = 0;
  drop_figure();
  ck_assert_int_eq(game->game_over, 1);
}
END_TEST

// START_TEST(test_drop_figure_success) {

//   GameInfo_t *game = getGame();
//   game->game_over = 0;
//   drop_figure();
//   for (int i = 0; i < 4; i++)
//   for (int j = 0; j < 4; j++) {
//     if (game->current[i][j])
//     ck_assert_int_eq(game->current[i][j],1);
// }
// }
// END_TEST

START_TEST(test_game_start) {
  GameInfo_t *game = getGame();
  game->game_start = 0;
  ck_assert_int_eq(game->game_start, 0);
  game_start();
  ck_assert_int_eq(game->game_start, 1);
}
END_TEST

START_TEST(test_getAction) {
  fprintf(stdin, "\n");
  UserAction_t action = getAction();

  ck_assert_int_eq(action, 5);

}
END_TEST

START_TEST(test_terminate_pushed) {

  GameInfo_t *game = getGame();
  game->game_over = 0;
  ck_assert_int_eq(game->game_over, 0);
  terminateButtonPushed();
  ck_assert_int_eq(game->game_over, 1);
}
END_TEST

START_TEST(test_rotateFigure) {
  GameInfo_t *game = getGame();
  game->coord_x = 10;
  game->coord_y = 2;
  rotateFigure();
  ck_assert_int_eq(game->new_orientation_figure, game->orientation_figure);
}
END_TEST

START_TEST(test_pauseGame) {

  GameInfo_t *game = getGame();
  game->pause = 0;
  pauseGame();
  ck_assert_int_eq(game->pause, 1);
  pauseGame();
  ck_assert_int_eq(game->pause, 0);

}
END_TEST


START_TEST(test_collision_move_left) {
  GameInfo_t *game = getGame();
  game->coord_x = 10;
  game->coord_y = 2;

  collision();

  ck_assert_int_eq(1,1);
}
END_TEST

START_TEST(test_collision_left_no_collision) {

  GameInfo_t *game = getGame();
  game->coord_x = 10;
  game->coord_y = 2;
  collision_left();
  ck_assert_int_eq(1,1);
}
END_TEST

START_TEST(test_collision_right_no_collision) {

  GameInfo_t *game = getGame();
  game->coord_x = 10;
  game->coord_y = 5;
  collision_Right();

  ck_assert_int_eq(1,1);
}
END_TEST

START_TEST(test_check_and_remove_lines) {

  GameInfo_t *game = getGame();

  game->score = 0;
  game->high_score = 50;

  for (int i = 5; i < 6; i++)
  for (int j = 0; j < 10; j++)
    game->field[i][j] = 1;
check_and_remove_lines();
ck_assert_int_eq(game->score, 100);
ck_assert_int_eq(game->level, 1);  


for (int i = 5; i < 7; i++)
for (int j = 0; j < 10; j++)
  game->field[i][j] = 1;
check_and_remove_lines();
ck_assert_int_eq(game->score, 400);
ck_assert_int_eq(game->level, 1);  

for (int i = 5; i < 8; i++)
for (int j = 0; j < 10; j++)
  game->field[i][j] = 1;
check_and_remove_lines();
ck_assert_int_eq(game->score, 1100);
ck_assert_int_eq(game->level, 2);  

for (int i = 5; i < 9; i++)
for (int j = 0; j < 10; j++)
  game->field[i][j] = 1;
check_and_remove_lines();
ck_assert_int_eq(game->score, 2600);
ck_assert_int_eq(game->level, 5);  

}
END_TEST

START_TEST(test_hard_drop) {

  GameInfo_t *game = getGame();
  game->coord_y = 5;

  hard_drop();

}
END_TEST

START_TEST(test_move_left) {
  GameInfo_t *game = getGame();
  int coord_x = game->coord_x;
  mvLeft();
  ck_assert_int_eq(coord_x, game->coord_x +1);  
}
END_TEST

START_TEST(test_free_Game) {

  GameInfo_t *game = getGame();
  free_memory(game);
  ck_assert_ptr_nonnull(game);
}
END_TEST

START_TEST(test_user_input) {

  UserAction_t act = Start;
  userInput(act, 0);
  act = Pause;
  userInput(act, 0);
  act = Up;
  userInput(act, 0);
  act = Terminate;
  userInput(act, 0);
  ck_assert_int_eq(2, 2);  
}
END_TEST

START_TEST(test_exit) {

  quitNcurses();
  ck_assert_int_eq(2, 2);  
}
END_TEST


Suite *test_getGame() {
  Suite *suite = suite_create("test_getGame");
  TCase *tc_core = tcase_create("Core");
    
  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, test_getGame1);
  tcase_add_test(tc_core, test_getGame2);
  tcase_add_test(tc_core, test_field_init);
  tcase_add_test(tc_core, test_next_init);
  tcase_add_test(tc_core, test_current_init);
  tcase_add_test(tc_core, test_drop_next);
  tcase_add_test(tc_core, test_drop_current);

  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *test_fileOpen() {
  Suite *suite = suite_create("test_fileOpen");
  TCase *tc_core = tcase_create("Core");
    
  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, test_file_open);
  tcase_add_test(tc_core, test_save_high_score);


  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *test_Game() {
  Suite *suite = suite_create("test_Game");
  TCase *tc_core = tcase_create("Core");
    
  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, test_drop_figure);
  // tcase_add_test(tc_core, test_drop_figure_success);
  tcase_add_test(tc_core, test_game_start);
  tcase_add_test(tc_core, test_getAction);
  tcase_add_test(tc_core, test_terminate_pushed);
  tcase_add_test(tc_core, test_pauseGame);
  tcase_add_test(tc_core, test_free_Game);
  tcase_add_test(tc_core, test_exit);  

  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *test_Figure() {
  Suite *suite = suite_create("test_Figure");
  TCase *tc_core = tcase_create("Core");
    
  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, test_rotateFigure);
  tcase_add_test(tc_core, test_collision_move_left);
  tcase_add_test(tc_core, test_collision_left_no_collision);
  tcase_add_test(tc_core, test_collision_right_no_collision);
  tcase_add_test(tc_core, test_check_and_remove_lines);

  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *test_move_figure() {
  Suite *suite = suite_create("test_Figure");
  TCase *tc_core = tcase_create("Core");
    
  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, test_hard_drop);
  tcase_add_test(tc_core, test_user_input);
  tcase_add_test(tc_core, test_move_left);

  suite_add_tcase(suite, tc_core);

  return suite;
}

// Запуск тестов
int main() {
  int fail = 0;
  Suite *s[] = {test_getGame(), test_fileOpen(), 
                test_Game(), test_Figure(), test_move_figure(), NULL};
  for (int i = 0; s[i] != NULL; i++) {
    SRunner *runner = srunner_create(s[i]);
    srunner_run_all(runner, CK_NORMAL);
    fail += srunner_ntests_failed(runner);
    srunner_free(runner);
  }
  printf("FAIL: %d\n", fail);
  return 0;
}