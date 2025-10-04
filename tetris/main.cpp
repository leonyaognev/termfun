#include <ncurses.h>

#include <thread>

#include "snake.h"
#include "snake_tui.h"

int main(void) {
  GameUI ui = snake_init_ui();  ///< Initialize UI
  GameSnake game;

  int run = 0;  ///< Flag to control game loop
  while (!run) {
    game.updateCurrentState();
    draw(&ui, game.snake, game.apple);  ///< Draw current game state
    run = snake_recive_input(game);     ///< Get user input
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  snake_deinit_ui(&ui);  ///< Clean up UI
  return 0;              ///< Exit program
}
