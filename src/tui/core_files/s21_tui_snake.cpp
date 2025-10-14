#include <ncurses.h>

#include <chrono>

#include "snake.h"
#include "snake_core.h"
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

SnakeUI snake_init_ui() {
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

  SnakeUI ui;
  ui.field = newwin(snakeSize::ROWS + 2, snakeSize::COLS * 2 + 2, 1, 1);
  ui.counter = newwin(5, 20, 1, snakeSize::COLS * 2 + 4);
  ui.control = newwin(7, 37, 7, snakeSize::COLS * 2 + 4);
  return ui;
}

void snake_deinit_ui(SnakeUI* ui) {
  delwin(ui->field);
  delwin(ui->counter);
  endwin();
}

static void draw_scoreboard(WINDOW* counter, const GameSnake& snake) {
  werase(counter);
  draw_fancy_box(counter);

  wattron(counter, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(counter, 1, 2, "Level: %d", (int)snake.score.level);
  mvwprintw(counter, 2, 2, "Length: %d", snake.score.score);
  mvwprintw(counter, 3, 2, "Max length: %d", (int)snake.score.highScore);
  wattroff(counter, COLOR_PAIR(3) | A_BOLD);

  wrefresh(counter);
}

static void draw_controlbord(WINDOW* control) {
  werase(control);
  draw_fancy_box(control);

  wattron(control, COLOR_PAIR(3) | A_BOLD);
  mvwprintw(control, 1, 2, "'enter' - start/restart game");
  mvwprintw(control, 2, 2, "'esc' - pause");
  mvwprintw(control, 3, 2, "'q/Q' - exit game");
  mvwprintw(control, 4, 2, "'arrows' - movement");
  mvwprintw(control, 5, 2, "'space' - one-time speed increase");
  wattroff(control, COLOR_PAIR(3) | A_BOLD);

  wrefresh(control);
}

static void draw_state_messages(WINDOW* field, const gameState& state) {
  switch (state) {
    case gameState::GAME_OVER:
      mvwaddstr(field, snakeSize::ROWS / 2, snakeSize::COLS - 5,
                "[[GAME OVER]]");
      mvwaddstr(field, snakeSize::ROWS / 2 + 1, snakeSize::COLS - 10,
                "press ENTER to restart");
      break;
    case gameState::PAUSE:
      mvwaddstr(field, snakeSize::ROWS / 2, snakeSize::COLS - 4, "[[PAUSE]]");
      break;
    case gameState::START:
      mvwaddstr(field, snakeSize::ROWS / 2, snakeSize::COLS - 9,
                "press ENTER to start");
      break;
    default:
      break;
  }
  wattroff(field, COLOR_PAIR(7) | A_BOLD);
}

void draw(SnakeUI* ui, const GameSnake& game) {
  werase(ui->field);
  draw_fancy_box(ui->field);

  // Яблоко
  draw_block(ui->field, game.apple.getY() + 1, game.apple.getX() * 2 + 1, 3);

  // Змейка
  bool first = true;
  for (auto& p : game.snake.getBody()) {
    int color = first ? 1 : 2;
    draw_block(ui->field, p.y + 1, p.x * 2 + 1, color);
    first = false;
  }

  draw_state_messages(ui->field, game.state);

  wrefresh(ui->field);
  draw_scoreboard(ui->counter, game);
  draw_controlbord(ui->control);
}
