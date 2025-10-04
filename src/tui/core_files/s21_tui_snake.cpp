#include <ncurses.h>

#include <chrono>

#include "snake.h"
#include "snake_tui.h"

static void draw_fancy_box(WINDOW* w) {
  wborder(w, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
}

static void draw_block(WINDOW* w, int y, int x, int color_pair) {
  wattron(w, COLOR_PAIR(color_pair) | A_REVERSE);
  mvwaddch(w, y, x, ACS_CKBOARD);
  mvwaddch(w, y, x + 1, ACS_CKBOARD);  // два символа шириной
  wattroff(w, COLOR_PAIR(color_pair) | A_REVERSE);
}

GameUI snake_init_ui() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  nodelay(stdscr, TRUE);
  ESCDELAY = 0;

  if (has_colors()) {
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);   // змейка голова
    init_pair(2, COLOR_YELLOW, -1);  // змейка тело
    init_pair(3, COLOR_RED, -1);     // яблоко
  }

  GameUI ui;
  ui.field = newwin(snakeSize::ROWS + 2, snakeSize::COLS * 2 + 2, 1, 1);
  ui.counter = newwin(5, 20, 1, snakeSize::COLS * 2 + 4);
  return ui;
}

void snake_deinit_ui(GameUI* ui) {
  delwin(ui->field);
  delwin(ui->counter);
  endwin();
}

static void draw_scoreboard(WINDOW* counter, const Snake& snake) {
  werase(counter);
  draw_fancy_box(counter);

  wattron(counter, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(counter, 1, 2, "Length: %d", (int)snake.getBody().size());
  wattroff(counter, COLOR_PAIR(3) | A_BOLD);

  wrefresh(counter);
}

void draw(GameUI* ui, const Snake& snake, const Apple& apple) {
  werase(ui->field);
  draw_fancy_box(ui->field);

  // Яблоко
  draw_block(ui->field, apple.getY() + 1, apple.getX() * 2 + 1, 3);

  // Змейка
  bool first = true;
  for (auto& p : snake.getBody()) {
    int color = first ? 1 : 2;
    draw_block(ui->field, p.y + 1, p.x * 2 + 1, color);
    first = false;
  }

  wrefresh(ui->field);
  draw_scoreboard(ui->counter, snake);
}
