#include "gui/cli/cli.h"

int main() {
  srand(time(NULL));

  init_game();
  game();
  stop_game();

  return 0;
}
