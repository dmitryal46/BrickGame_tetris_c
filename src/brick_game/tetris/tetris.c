#include "tetris.h"

void userInput(UserAction_t action, bool hold) {
  if (hold) printf("hold!");
  GameInfoTotal_t *game_param = update_param(NULL);
  GameState_t state = game_param->state;
  funcPointer FSM_TABLE[STATES_COUNT][SIGNALS_COUNT] = {
      // FINITE STATE MACHINE TABLE
      // State START
      {start_game, NULL, remove_game_param, NULL, NULL, NULL, NULL, NULL},
      // State GAME
      {NULL, pause_game, remove_game_param, move_left, move_right, NULL,
       move_down, rotate},
      // State GAMEOVER
      {start_game, NULL, remove_game_param, NULL, NULL, NULL, NULL, NULL},
  };
  funcPointer func = FSM_TABLE[state][action];
  if (func) func(game_param);
}

GameInfo_t updateCurrentState(void) {
  GameInfoTotal_t *game_param = update_param(NULL);
  shift(game_param);
  return *game_param->data;
}

GameInfoTotal_t *update_param(GameInfoTotal_t *game_param) {
  static GameInfoTotal_t *data;
  if (game_param != NULL) data = game_param;
  return data;
}

void initialize_game_param(GameInfoTotal_t *game_param) {
  game_param->data->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  reset_field(game_param);
  game_param->data->next = create_matrix(FIGURE_HEIGHT, FIGURE_WIDTH);
  game_param->data->score = 0;

  FILE *fp = fopen(DATAFILE_PATH, "r");
  if (!fp) {
    game_param->data->high_score = 0;
    fp = fopen(DATAFILE_PATH, "w");
    fprintf(fp, "0\n");
  } else {
    int highScore;
    fscanf(fp, "%d\n", &highScore);
    game_param->data->high_score = highScore;
  }
  fclose(fp);

  game_param->data->level = LEVEL_MIN;
  game_param->data->speed = SPEED_MIN;
  game_param->data->pause = 0;
  game_param->figure->typeNext = create_rand_figure(game_param->data->next);
  game_param->state = START;
  game_param->active_flag = true;
}

void reset_field(GameInfoTotal_t *game_param) {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      if (i > FIELD_HEIGHT - BORDER_SIZE - 1 || j < BORDER_SIZE ||
          j > FIELD_WIDTH - BORDER_SIZE - 1)
        game_param->data->field[i][j] = 1;
      else
        game_param->data->field[i][j] = 0;
}

void remove_game_param(GameInfoTotal_t *game_param) {
  if (game_param->data->field) {
    for (size_t i = 0; i < FIELD_HEIGHT; i++) {
      free(game_param->data->field[i]);
      game_param->data->field[i] = NULL;
    }
    free(game_param->data->field);
    game_param->data->field = NULL;
  }

  if (game_param->data->next) {
    for (size_t i = 0; i < FIGURE_HEIGHT; i++) {
      free(game_param->data->next[i]);
      game_param->data->next[i] = NULL;
    }
    free(game_param->data->next);
    game_param->data->next = NULL;
  }

  game_param->state = GAMEOVER;
  game_param->active_flag = false;
}

int create_rand_figure(int **next) {
  int figures[FIGURES_COUNT][8] = {
      FIGURE1, FIGURE2, FIGURE3, FIGURE4, FIGURE5, FIGURE6, FIGURE7,
  };
  int type = rand() % FIGURES_COUNT;

  for (int i = 0; i < FIGURE_HEIGHT; i++)
    for (int j = 0; j < FIGURE_WIDTH; j++) next[i][j] = 0;

  for (int i = 1; i < 8; i += 2)
    next[figures[type][i - 1] + 1][figures[type][i] + 1] = type + 1;

  return type;
}

void start_game(GameInfoTotal_t *game_param) {
  reset_field(game_param);
  FILE *fp = fopen(DATAFILE_PATH, "r");
  int highScore;
  fscanf(fp, "%d\n", &highScore);
  game_param->data->high_score = highScore;
  fclose(fp);
  game_param->data->score = 0;
  game_param->data->level = LEVEL_MIN;
  game_param->data->speed = SPEED_MIN;
  game_param->state = GAME;
  spawn_next_figure(game_param);
}

void spawn_next_figure(GameInfoTotal_t *game_param) {
  game_param->figure->type = game_param->figure->typeNext;
  game_param->figure->x = FIELD_WIDTH / 2;
  game_param->figure->y = 2;
  game_param->figure->rotation = 0;
  game_param->figure->typeNext = create_rand_figure(game_param->data->next);
  add_figure(game_param);
}

void add_figure(GameInfoTotal_t *game_param) {
  int figures[FIGURES_COUNT][8] = {
      FIGURE1, FIGURE2, FIGURE3, FIGURE4, FIGURE5, FIGURE6, FIGURE7,
  };
  int y = game_param->figure->y;
  int x = game_param->figure->x;
  int type = game_param->figure->type;
  int rotation = game_param->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));
    game_param->data->field[yy + y][xx + x] = type + 1;
  }
}

bool is_figures_not_meet(GameInfoTotal_t *game_param) {
  int figures[FIGURES_COUNT][8] = {
      FIGURE1, FIGURE2, FIGURE3, FIGURE4, FIGURE5, FIGURE6, FIGURE7,
  };
  int y = game_param->figure->y;
  int x = game_param->figure->x;
  int type = game_param->figure->type;
  int rotation = game_param->figure->rotation;

  bool meet_flag = true;
  for (int i = 1; i < 8 && meet_flag; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));
    if (game_param->data->field[yy + y][xx + x]) meet_flag = false;
  }

  return meet_flag;
}

void remove_figure(GameInfoTotal_t *game_param) {
  int figures[FIGURES_COUNT][8] = {
      FIGURE1, FIGURE2, FIGURE3, FIGURE4, FIGURE5, FIGURE6, FIGURE7,
  };
  int y = game_param->figure->y;
  int x = game_param->figure->x;
  int type = game_param->figure->type;
  int rotation = game_param->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(PI_2 * rotation) +
                        figures[type][i - 1] * sin(PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(PI_2 * rotation) +
                        figures[type][i - 1] * cos(PI_2 * rotation));
    game_param->data->field[yy + y][xx + x] = 0;
  }
}

void shift(GameInfoTotal_t *game_param) {
  remove_figure(game_param);
  game_param->figure->y++;
  bool shift_flag = is_figures_not_meet(game_param);

  if (!shift_flag) game_param->figure->y--;

  add_figure(game_param);

  if (!shift_flag) attach(game_param);
}

void attach(GameInfoTotal_t *game_param) {
  int attach_rows = 0;
  for (int row = FIELD_HEIGHT - BORDER_SIZE - 1; row > 2; row--) {
    int row_blocks;
    bool flag = true;
    while (flag) {
      row_blocks = 0;
      for (int col = 3; col < FIELD_WIDTH - 3; col++)
        if (game_param->data->field[row][col]) row_blocks++;
      if (row_blocks == FIELD_WIDTH - 6) {
        attach_rows++;
        for (int i = row; i > 1; i--)
          for (int col = BORDER_SIZE; col < FIELD_WIDTH - BORDER_SIZE; col++)
            game_param->data->field[i][col] =
                game_param->data->field[i - 1][col];
      } else
        flag = false;
    }
  }

  if (attach_rows == 1)
    game_param->data->score += SCORE_1;
  else if (attach_rows == 2)
    game_param->data->score += SCORE_2;
  else if (attach_rows == 3)
    game_param->data->score += SCORE_3;
  else if (attach_rows >= 4)
    game_param->data->score += SCORE_4;

  if (game_param->data->score > game_param->data->high_score) {
    game_param->data->high_score = game_param->data->score;
    FILE *fp = fopen(DATAFILE_PATH, "w");
    fprintf(fp, "%d\n", game_param->data->high_score);
    fclose(fp);
  }

  if (game_param->data->score / LEVEL_UP_SCORE + 1 <= LEVEL_MAX)
    game_param->data->level = game_param->data->score / LEVEL_UP_SCORE + 1;
  else
    game_param->data->level = LEVEL_MAX;

  game_param->data->speed = game_param->data->level;

  spawn_next_figure(game_param);
  remove_figure(game_param);
  game_param->figure->y++;
  bool shift_flag = is_figures_not_meet(game_param);

  if (!shift_flag) {
    game_param->figure->y--;
    game_param->state = GAMEOVER;
  }

  add_figure(game_param);
}

void move_left(GameInfoTotal_t *game_param) {
  if (!game_param->data->pause) {
    remove_figure(game_param);
    game_param->figure->x--;
    bool move_flag = is_figures_not_meet(game_param);

    if (!move_flag) game_param->figure->x++;

    add_figure(game_param);
  }
}

void move_right(GameInfoTotal_t *game_param) {
  if (!game_param->data->pause) {
    remove_figure(game_param);
    game_param->figure->x++;
    bool move_flag = is_figures_not_meet(game_param);

    if (!move_flag) game_param->figure->x--;

    add_figure(game_param);
  }
}

void move_down(GameInfoTotal_t *game_param) {
  if (!game_param->data->pause) {
    remove_figure(game_param);
    bool move_flag = true;
    while (move_flag) {
      game_param->figure->y++;
      move_flag = is_figures_not_meet(game_param);

      if (!move_flag) game_param->figure->y--;
    }

    add_figure(game_param);
    attach(game_param);
  }
}

void rotate(GameInfoTotal_t *game_param) {
  if (!game_param->data->pause) {
    remove_figure(game_param);

    if (game_param->figure->rotation + 1 <= ROTATION_MAX)
      game_param->figure->rotation = game_param->figure->rotation + 1;
    else
      game_param->figure->rotation = ROTATION_MIN;

    bool rotate_flag = is_figures_not_meet(game_param);

    if (!rotate_flag) {
      if (game_param->figure->rotation - 1 >= ROTATION_MIN) {
        game_param->figure->rotation = game_param->figure->rotation - 1;
      } else {
        game_param->figure->rotation = ROTATION_MAX;
      }
    }

    add_figure(game_param);
  }
}

void pause_game(GameInfoTotal_t *game_param) {
  game_param->data->pause = !game_param->data->pause;
}

int **create_matrix(int n_rows, int n_cols) {
  int **arr = (int **)calloc(n_rows, sizeof(int *));
  for (size_t rowIdx = 0; rowIdx < (size_t)n_rows; rowIdx++)
    arr[rowIdx] = (int *)calloc(n_cols, sizeof(int));
  return arr;
}
