#include <ncurses.h>

#include "s21_tetris.h"
#include "s21_tui.h"

int s21_recive_input() {
  int ch = getch();
  switch (ch) {
    case 'q':
    case 'Q':
      return 1;
    case KEY_UP:
      userInput(Up, 1);
      break;
    case KEY_DOWN:
      userInput(Down, 1);
      break;
    case KEY_LEFT:
      userInput(Left, 1);
      break;
    case KEY_RIGHT:
      userInput(Right, 1);
      break;
    case '\n':
    case KEY_ENTER:
      userInput(Start, 1);
      break;
    case ' ':
      userInput(Action, 1);
      break;
    case 27:
      userInput(Pause, 1);
      break;
    default:
      break;
  }
  return 0;
}
