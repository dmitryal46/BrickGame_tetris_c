#include "cli.h"

void init_game(void) {
  setlocale(LC_ALL, "");

  initscr();
  noecho();              // canceling char print
  cbreak();              // canceling raw() - kayboard control
  curs_set(0);           // cursor start position
  keypad(stdscr, true);  // using functionals keys
  timeout(DELAY);        // timeout sequence of keypress
}

void stop_game(void) {
  printw("\n GAME OVER...\n");
  refresh();  // output from buffer abd clear buffer
  sleep(1);
  clear();
  refresh();
  endwin();  // ending ncurses and exit
}

void game(void) {
  GameInfoTotal_t game_param;
  GameInfo_t data;
  Figure_t figure;
  game_param.data = &data;
  game_param.figure = &figure;

  UserAction_t action;
  int key;
  double counter = 0.;
  bool hold = false;

  initialize_game_param(&game_param);
  update_param(&game_param);

  while (game_param.active_flag) {
    if (counter >= 1.50 - game_param.data->speed * SPEED_RATE) {
      if (game_param.state == GAME && !game_param.data->pause) {
        updateCurrentState();
      }
      counter = 0.;
    }

    counter += DELAY * EPS3;

    if (game_param.state == START) {
      draw_start_screen(game_param.data);
    } else if (game_param.state == GAME) {
      draw_interface_1();
      draw_legend(game_param.data);
      draw_field(game_param.data->field);
    } else if (game_param.state == GAMEOVER) {
      draw_end_screen(game_param.data);
    }

    if (game_param.data->pause) {
      mvprintw(1 + FIELD_SIZE_V / 2, FIELD_SIZE_H - 1, "PAUSE");
      move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
    }

    key = getch();
    action = get_action(key);
    if (action != Up) {
      userInput(action, hold);
    }
  }
}

UserAction_t get_action(int key) {
  UserAction_t action = Up;
  if (key == 10)
    action = Start;
  else if (key == 'P' || key == 'p')
    action = Pause;
  else if (key == 27)
    action = Terminate;
  else if (key == KEY_LEFT)
    action = Left;
  else if (key == KEY_RIGHT)
    action = Right;
  else if (key == KEY_DOWN)
    action = Down;
  else if (key == ' ')
    action = Action;

  return action;
}

void draw_interface_1(void) {
  clear();

  mvhline(0, 0, ACS_HLINE, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 2);
  mvhline(FIELD_SIZE_V + 1, 0, ACS_HLINE,
          FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 2);
  mvhline(FIELD_SIZE_V - 6, FIELD_SIZE_H * 2 + 2, ACS_HLINE, LEGEND_SIZE_H * 2);
  mvvline(1, 0, ACS_VLINE, FIELD_SIZE_V);
  mvvline(1, FIELD_SIZE_H * 2 + 1, ACS_VLINE, FIELD_SIZE_V);
  mvvline(1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 2, ACS_VLINE, FIELD_SIZE_V);

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 2, ACS_URCORNER);
  mvaddch(FIELD_SIZE_V + 1, 0, ACS_LLCORNER);
  mvaddch(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 2,
          ACS_LRCORNER);
  mvaddch(0, FIELD_SIZE_H * 2 + 1, ACS_TTEE);
  mvaddch(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + 1, ACS_BTEE);

  move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
}

void draw_field(int **field) {
  for (int row = 0; row < FIELD_SIZE_V; row++)
    for (int col = 0; col < FIELD_SIZE_H; col++)
      if (field[row + 3][col + 3]) {
        mvaddch(1 + row, 1 + col * 2, ':');
        mvaddch(1 + row, 1 + col * 2 + 1, ':');
      }
  move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
}

void draw_legend(GameInfo_t *data) {
  mvprintw(2, FIELD_SIZE_H * 2 + 3, "High score: %d", data->high_score);
  mvprintw(4, FIELD_SIZE_H * 2 + 3, "Score: %d", data->score);
  mvprintw(6, FIELD_SIZE_H * 2 + 3, "Level: %d", data->level);
  mvprintw(9, FIELD_SIZE_H * 2 + 3, "Next figure");
  for (int row = 0; row < FIGURE_HEIGHT; row++)
    for (int col = 0; col < FIGURE_WIDTH; col++) {
      if (data->next[row][col]) {
        mvaddch(11 + row, FIELD_SIZE_H * 2 + 6 + col * 2, ':');
        mvaddch(11 + row, FIELD_SIZE_H * 2 + 6 + col * 2 + 1, ':');
      }
    }

  mvprintw(15, FIELD_SIZE_H * 2 + 5, "P   - pause game");
  mvaddwstr(16, FIELD_SIZE_H * 2 + 5, L"←   - move left");
  mvaddwstr(17, FIELD_SIZE_H * 2 + 5, L"→   - move right");
  mvaddwstr(18, FIELD_SIZE_H * 2 + 5, L"↓   - move down");
  mvaddwstr(19, FIELD_SIZE_H * 2 + 3, L"space - rotate");
  mvaddwstr(20, FIELD_SIZE_H * 2 + 4, L"Esc  - exit");
  move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
}

void draw_start_screen(GameInfo_t *data) {
  draw_interface_1();
  draw_legend(data);

  mvprintw(1 + FIELD_SIZE_V / 2, 3, "<Enter> to start");

  move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
}

void draw_end_screen(GameInfo_t *data) {
  draw_interface_1();
  draw_legend(data);
  draw_field(data->field);

  mvprintw(FIELD_SIZE_V / 2, 6, "GAME OVER");
  mvprintw(FIELD_SIZE_V / 2 + 2, 3, "<Enter> to restart");

  move(FIELD_SIZE_V + 1, FIELD_SIZE_H * 2 + LEGEND_SIZE_H * 2 + 3);
}
