#include <ncurses.h>

#include "s21_tui.h"

int main(void) {
  GameUI ui = init_ui();  ///< Initialize UI

  int run = 0;  ///< Flag to control game loop
  while (!run) {
    s21_write_run(&ui);        ///< Draw current game state
    run = s21_recive_input();  ///< Get user input
  }

  deinit_ui(&ui);  ///< Clean up UI
  return 0;        ///< Exit program
}
