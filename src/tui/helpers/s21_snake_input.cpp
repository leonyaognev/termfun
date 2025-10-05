#include <ncurses.h>

#include "snake.h"

int snake_recive_input(GameSnake& game) {
  int ch = getch();
  switch (ch) {
    case 'q':
    case 'Q':
      game.userInput(UserAction_s::Terminate, 0);
      break;
    case KEY_UP:
      game.userInput(UserAction_s::Up, 0);
      break;
    case KEY_DOWN:
      game.userInput(UserAction_s::Down, 0);
      break;
    case KEY_LEFT:
      game.userInput(UserAction_s::Left, 0);
      break;
    case KEY_RIGHT:
      game.userInput(UserAction_s::Right, 0);
      break;
    case '\n':
    case KEY_ENTER:
      game.userInput(UserAction_s::Start, 0);
      break;
    case ' ':
      game.userInput(UserAction_s::Action, 0);
      break;
    case 27:
      game.userInput(UserAction_s::Pause, 0);
      break;
  }
  return 0;
}
