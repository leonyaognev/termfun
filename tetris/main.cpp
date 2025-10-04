#include <ncurses.h>

#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "s21_tui.h"
#include "snake.h"
#include "snake_tui.h"

void snake() {
  SnakeUI ui = snake_init_ui();
  GameSnake game;

  int run = 0;
  while (!run) {
    game.updateCurrentState();
    draw(&ui, game.snake, game.apple);
    run = snake_recive_input(game);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  snake_deinit_ui(&ui);
}

void tetris() {
  GameUI ui = init_ui();

  int run = 0;
  while (!run) {
    s21_write_run(&ui);
    run = s21_recive_input();
  }

  deinit_ui(&ui);
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  std::vector<std::string> items = {"tetris", "snake", "exit"};
  std::vector<std::function<void()>> actions = {tetris, snake, []() {
                                                  endwin();
                                                  exit(0);
                                                }};
  int highlight = 0;

  while (true) {
    clear();
    mvprintw(0, 0, "=== MENU ===");

    for (size_t i = 0; i < items.size(); ++i) {
      if ((int)i == highlight) attron(A_REVERSE);
      mvprintw(i + 2, 2, items[i].c_str());
      if ((int)i == highlight) attroff(A_REVERSE);
    }

    int ch = getch();
    if (ch == KEY_UP) {
      highlight--;
      if (highlight < 0) highlight = items.size() - 1;
    } else if (ch == KEY_DOWN) {
      highlight++;
      if (highlight >= (int)items.size()) highlight = 0;
    } else if (ch == 10 || ch == KEY_ENTER) {
      actions[highlight]();
    }
  }

  endwin();
  return 0;
}
