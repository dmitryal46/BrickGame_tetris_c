#ifndef TETRIS_H
#define TETRIS_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.1415926535897932384626433832795
#define PI_2 PI / 2.0

#define FIELD_HEIGHT 26
#define FIELD_WIDTH 16
#define BORDER_SIZE 3
#define FIGURE_HEIGHT 2
#define FIGURE_WIDTH 4
#define PIXEL_EMPTY 0

#define STATES_COUNT 3
#define SIGNALS_COUNT 8
#define FIGURES_COUNT 7

#define DATAFILE_PATH "./file"

#define LEVEL_MIN 1
#define LEVEL_MAX 10
#define SPEED_MIN 1
#define SPEED_MAX 10
#define LEVEL_UP_SCORE 600

#define SCORE_1 100
#define SCORE_2 300
#define SCORE_3 700
#define SCORE_4 1500

#define ROTATION_MIN 0
#define ROTATION_MAX 3

#define FIGURE1 \
  { 0, -1, 0, 0, 0, 1, 0, 2 }
#define FIGURE2 \
  { -1, -1, 0, -1, 0, 0, 0, 1 }
#define FIGURE3 \
  { 0, -1, 0, 0, 0, 1, -1, 1 }
#define FIGURE4 \
  { -1, 0, -1, 1, 0, 0, 0, 1 }
#define FIGURE5 \
  { 0, -1, 0, 0, -1, 0, -1, 1 }
#define FIGURE6 \
  { 0, -1, 0, 0, -1, 0, 0, 1 }
#define FIGURE7 \
  { -1, -1, -1, 0, 0, 0, 0, 1 }

typedef enum {
  START = 0,
  GAME,
  GAMEOVER,
} GameState_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
} UserAction_t;  // !! обязательный тип по ТЗ

typedef struct {
  int **field;
  int **next;  // next spawn figure
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;  // !! обязательный тип по ТЗ

typedef struct {
  int typeNext;
  int type;
  int rotation;  // [0..3]
  int x;
  int y;
} Figure_t;

typedef struct {
  GameInfo_t *data;
  GameState_t state;
  bool active_flag;
  Figure_t *figure;
} GameInfoTotal_t;

typedef void (*funcPointer)(GameInfoTotal_t *game_param);

void userInput(UserAction_t action,
               bool hold);  // !!! обязательная функция по ТЗ

GameInfo_t updateCurrentState(void);  // !!! обязательная функция по ТЗ

GameInfoTotal_t *update_param(GameInfoTotal_t *game_param);
void initialize_game_param(GameInfoTotal_t *game_param);
void remove_game_param(GameInfoTotal_t *game_param);
void start_game(GameInfoTotal_t *game_param);
void pause_game(GameInfoTotal_t *game_param);

int **create_matrix(int n_rows, int n_cols);
void reset_field(GameInfoTotal_t *game_param);

int create_rand_figure(int **next);
void spawn_next_figure(GameInfoTotal_t *game_param);

void move_left(GameInfoTotal_t *game_param);
void move_right(GameInfoTotal_t *game_param);
void rotate(GameInfoTotal_t *game_param);
void move_down(GameInfoTotal_t *game_param);

void shift(GameInfoTotal_t *game_param);
void attach(
    GameInfoTotal_t *game_param);  // del rows, shift rows, update, spawn
void add_figure(GameInfoTotal_t *game_param);
bool is_figures_not_meet(GameInfoTotal_t *game_param);
void remove_figure(GameInfoTotal_t *game_param);

#endif
