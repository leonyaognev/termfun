#include <ncurses.h>

#include "snake.h"

int snake_recive_input(GameSnake& game) {
  int ch = getch();
  switch (ch) {
    case 'q':
    case 'Q':
      return 1;
    case KEY_UP:
      game.userInput(UserAction_s::Up, 1);
      break;
    case KEY_DOWN:
      game.userInput(UserAction_s::Down, 1);
      break;
    case KEY_LEFT:
      game.userInput(UserAction_s::Left, 1);
      break;
    case KEY_RIGHT:
      game.userInput(UserAction_s::Right, 1);
      break;
    case '\n':
    case KEY_ENTER:
      game.userInput(UserAction_s::Start, 1);
      break;
    case ' ':
      game.userInput(UserAction_s::Action, 1);
      break;
    case 27:
      game.userInput(UserAction_s::Pause, 1);
      break;
  }
  return 0;
}
