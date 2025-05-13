#include "tetris_test.h"

int main() {
  SRunner *runner = srunner_create(tetris_suite());

  srunner_run_all(runner, CK_NORMAL);
  int res = srunner_ntests_failed(runner);
  srunner_free(runner);
  if (res != 0) res = 1;
  return res;
}