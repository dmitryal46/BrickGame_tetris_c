#include "tetris_test.h"

START_TEST(test_1) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  update_param(&game_param);
  spawn_next_figure(&game_param);
  GameInfo_t expected_data = updateCurrentState();

  ck_assert_ptr_eq(game_param.data->field, expected_data.field);
  ck_assert_ptr_eq(game_param.data->next, expected_data.next);
  ck_assert_int_eq(game_param.data->high_score, expected_data.high_score);
  ck_assert_int_eq(game_param.data->score, expected_data.score);
  ck_assert_int_eq(game_param.data->level, expected_data.level);
  ck_assert_int_eq(game_param.data->pause, expected_data.pause);
  ck_assert_int_eq(game_param.data->speed, expected_data.speed);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_2) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  GameInfoTotal_t *game_param_expd1 = update_param(&game_param);
  GameInfoTotal_t *game_param_expd2 = update_param(NULL);

  ck_assert_ptr_eq(&game_param, game_param_expd1);
  ck_assert_ptr_eq(&game_param, game_param_expd2);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_3) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);

  ck_assert_int_eq(game_param.data->score, 0);
  ck_assert_int_eq(game_param.data->level, 1);
  ck_assert_int_eq(game_param.data->speed, 1);
  ck_assert_int_eq(game_param.data->pause, 0);
  ck_assert_int_ge(game_param.figure->typeNext, 0);
  ck_assert_int_le(game_param.figure->typeNext, 6);
  ck_assert_int_eq(game_param.state, START);
  ck_assert_int_eq(game_param.active_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_4) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  remove_game_param(&game_param);

  ck_assert_ptr_null(game_param.data->field);
  ck_assert_ptr_null(game_param.data->next);
  ck_assert_int_eq(game_param.state, GAMEOVER);
  ck_assert_int_eq(game_param.active_flag, false);
}
END_TEST

START_TEST(test_5) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  game_param.data = &data;
  game_param.data->field = NULL;
  game_param.data->next = NULL;

  remove_game_param(&game_param);

  ck_assert_ptr_null(game_param.data->field);
  ck_assert_ptr_null(game_param.data->next);
  ck_assert_int_eq(game_param.state, GAMEOVER);
  ck_assert_int_eq(game_param.active_flag, false);
}
END_TEST

START_TEST(test_6) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  start_game(&game_param);

  ck_assert_int_eq(game_param.data->score, 0);
  ck_assert_int_eq(game_param.data->level, 1);
  ck_assert_int_eq(game_param.data->speed, 1);
  ck_assert_int_eq(game_param.state, GAME);
  ck_assert_int_eq(game_param.active_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_7) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.data->field[FIELD_HEIGHT / 2][FIELD_WIDTH / 2] = 1;
  reset_field(&game_param);

  bool field_eq_flag = true;
  for (int row = 0; row < FIELD_HEIGHT && field_eq_flag; row++)
    for (int col = 0; col < FIELD_WIDTH && field_eq_flag; col++) {
      int checkValue = (row > 22 || col < 3 || col > 12) ? 1 : 0;
      field_eq_flag = game_param.data->field[row][col] == checkValue;
    }
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_8) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->typeNext = create_rand_figure(game_param.data->next);

  ck_assert_int_ge(game_param.figure->typeNext, 0);
  ck_assert_int_le(game_param.figure->typeNext, 6);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_9) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->typeNext = 0;
  spawn_next_figure(&game_param);

  ck_assert_int_eq(game_param.figure->type, 0);
  ck_assert_int_eq(game_param.figure->y, 2);
  ck_assert_int_eq(game_param.figure->x, FIELD_WIDTH / 2);
  ck_assert_int_eq(game_param.figure->rotation, 0);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_10) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  move_left(&game_param);

  ck_assert_int_ne(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_11) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  game_param.data->pause = 1;
  move_left(&game_param);

  ck_assert_int_eq(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_12) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = 4;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  move_left(&game_param);

  ck_assert_int_eq(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_13) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  move_right(&game_param);

  ck_assert_int_ne(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_14) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  game_param.data->pause = 1;
  move_right(&game_param);

  ck_assert_int_eq(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_15) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH - 6;
  int prev_x = game_param.figure->x;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  move_right(&game_param);

  ck_assert_int_eq(game_param.figure->x, prev_x);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_16) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->rotation = 0;
  game_param.figure->type = 0;
  int prev_rotate = game_param.figure->rotation;
  rotate(&game_param);

  ck_assert_int_ne(game_param.figure->rotation, prev_rotate);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_17) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  int prev_rotate = game_param.figure->rotation;
  game_param.data->pause = 1;
  rotate(&game_param);

  ck_assert_int_eq(game_param.figure->rotation, prev_rotate);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_18) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH - 3;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  int prev_rotate = game_param.figure->rotation;
  rotate(&game_param);

  ck_assert_int_eq(game_param.figure->rotation, prev_rotate);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_19) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_y = game_param.figure->y;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  move_down(&game_param);

  bool field_eq_flag = true;
  field_eq_flag = game_param.data->field[game_param.figure->y][prev_y] == 0;
  field_eq_flag = game_param.data->field[game_param.figure->y][prev_y] == 0;
  field_eq_flag = game_param.data->field[game_param.figure->y][prev_y] == 0;
  field_eq_flag = game_param.data->field[game_param.figure->y][prev_y] == 0;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_20) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  game_param.data->pause = 1;
  move_down(&game_param);

  bool field_eq_flag = true;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 - 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 + 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT / 2][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_21) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT - 4;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  int prev_y = game_param.figure->y;
  move_down(&game_param);

  bool field_eq_flag = true;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_22) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  int prev_y = game_param.figure->y;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  shift(&game_param);

  ck_assert_int_ne(game_param.figure->y, prev_y);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_23) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT - 4;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  int prev_y = game_param.figure->y;
  shift(&game_param);

  bool field_eq_flag = true;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag = game_param.data->field[prev_y][game_param.figure->x] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 - 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 1] == 1;
  field_eq_flag =
      game_param.data->field[FIELD_HEIGHT - 4][FIELD_HEIGHT / 2 + 2] == 1;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_24) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  for (int col = 0; col < FIELD_WIDTH; col++)
    game_param.data->field[FIELD_HEIGHT - 4][col] = 1;
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, 100);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_25) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    game_param.data->field[FIELD_HEIGHT - 4][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 5][col] = 1;
  }
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, 300);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_26) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    game_param.data->field[FIELD_HEIGHT - 4][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 5][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 6][col] = 1;
  }
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, 700);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_27) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  for (int col = 0; col < FIELD_WIDTH; col++) {
    game_param.data->field[FIELD_HEIGHT - 4][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 5][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 6][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 7][col] = 1;
  }
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, 1500);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_28) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.data->score = 6000;
  for (int col = 0; col < FIELD_WIDTH; col++) {
    game_param.data->field[FIELD_HEIGHT - 4][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 5][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 6][col] = 1;
    game_param.data->field[FIELD_HEIGHT - 7][col] = 1;
  }
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, 7500);
  ck_assert_int_eq(game_param.data->level, 10);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_29) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.data->field[3][FIELD_WIDTH / 2] = 1;
  attach(&game_param);

  ck_assert_int_eq(game_param.state, GAMEOVER);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_30) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.data->score = game_param.data->high_score + 100;
  attach(&game_param);

  ck_assert_int_eq(game_param.data->score, game_param.data->high_score);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_31) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  add_figure(&game_param);

  bool field_eq_flag = true;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x - 1] ==
      game_param.figure->type + 1;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x] ==
      game_param.figure->type + 1;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x + 1] ==
      game_param.figure->type + 1;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x + 2] ==
      game_param.figure->type + 1;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_32) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  bool meet_flag = is_figures_not_meet(&game_param);

  ck_assert_int_eq(meet_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_33) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT - 3;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  bool meet_flag = is_figures_not_meet(&game_param);

  ck_assert_int_eq(meet_flag, false);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_34) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  game_param.figure->y = FIELD_HEIGHT / 2;
  game_param.figure->x = FIELD_WIDTH / 2;
  game_param.figure->type = 0;
  game_param.figure->rotation = 0;
  remove_figure(&game_param);

  bool field_eq_flag = true;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x - 1] ==
      0;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x] == 0;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x + 1] ==
      0;
  field_eq_flag =
      game_param.data->field[game_param.figure->y][game_param.figure->x + 2] ==
      0;
  ck_assert_int_eq(field_eq_flag, true);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_35) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  data.field = NULL;
  data.next = NULL;
  game_param.data = &data;

  data.field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);

  ck_assert_ptr_nonnull(game_param.data->field);
  remove_game_param(&game_param);
}
END_TEST

START_TEST(test_36) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  initialize_game_param(&game_param);
  pause_game(&game_param);

  ck_assert_int_eq(game_param.data->pause, 1);
  remove_game_param(&game_param);
}
END_TEST

Suite *tetris_suite() {
  Suite *s = suite_create("tetris");
  TCase *tcases_ok;

  tcases_ok = tcase_create("tetris_ok");
  tcase_add_test(tcases_ok, test_1);
  tcase_add_test(tcases_ok, test_2);
  tcase_add_test(tcases_ok, test_3);
  tcase_add_test(tcases_ok, test_4);
  tcase_add_test(tcases_ok, test_5);
  tcase_add_test(tcases_ok, test_6);
  tcase_add_test(tcases_ok, test_7);
  tcase_add_test(tcases_ok, test_8);
  tcase_add_test(tcases_ok, test_9);
  tcase_add_test(tcases_ok, test_10);
  tcase_add_test(tcases_ok, test_11);
  tcase_add_test(tcases_ok, test_12);
  tcase_add_test(tcases_ok, test_13);
  tcase_add_test(tcases_ok, test_14);
  tcase_add_test(tcases_ok, test_15);
  tcase_add_test(tcases_ok, test_16);
  tcase_add_test(tcases_ok, test_17);
  tcase_add_test(tcases_ok, test_18);
  tcase_add_test(tcases_ok, test_19);
  tcase_add_test(tcases_ok, test_20);
  tcase_add_test(tcases_ok, test_21);
  tcase_add_test(tcases_ok, test_22);
  tcase_add_test(tcases_ok, test_23);
  tcase_add_test(tcases_ok, test_24);
  tcase_add_test(tcases_ok, test_25);
  tcase_add_test(tcases_ok, test_26);
  tcase_add_test(tcases_ok, test_27);
  tcase_add_test(tcases_ok, test_28);
  tcase_add_test(tcases_ok, test_29);
  tcase_add_test(tcases_ok, test_30);
  tcase_add_test(tcases_ok, test_31);
  tcase_add_test(tcases_ok, test_32);
  tcase_add_test(tcases_ok, test_33);
  tcase_add_test(tcases_ok, test_34);
  tcase_add_test(tcases_ok, test_35);
  tcase_add_test(tcases_ok, test_36);

  suite_add_tcase(s, tcases_ok);

  return s;
}