#ifndef GUI_CLI_H
#define GUI_CLI_H

#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <wchar.h>

#include "../../brick_game/tetris/tetris.h"

#define DELAY 50
#define FIELD_SIZE_H 10  // horizontal size of field
#define FIELD_SIZE_V 20  // vertical size of field
#define LEGEND_SIZE_H 10
#define LEGEND_SIZE_V 20
#define SPEED_RATE 0.15

#define EPS3 (1E-3)

void init_game(void);
void stop_game(void);
void game(void);  // Game main function

UserAction_t get_action(int key);

void draw_interface_1(void);
void draw_field(int **field);
void draw_legend(GameInfo_t *data);
void draw_start_screen(GameInfo_t *data);
void draw_end_screen(GameInfo_t *data);

#endif