#include "s21_tui.h"

#include <locale.h>
#include <ncurses.h>

#include "s21_tetris.h"

static void draw_fancy_box(WINDOW *w) {
  setlocale(LC_ALL, "");  // Enable UTF-8 locale so ncurses handles box chars

  // Draw a border around the window using ACS characters
  wborder(w, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
          ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
}

static void draw_block(WINDOW *w, int y, int x, int color_pair) {
  wattron(w, COLOR_PAIR(color_pair) | A_REVERSE);   // Turn on color + reverse
  mvwaddch(w, y, x, ACS_CKBOARD);                   // Draw a checker block
  wattroff(w, COLOR_PAIR(color_pair) | A_REVERSE);  // Turn off attributes
}

GameUI init_ui(void) {
  initscr();              // Start ncurses mode
  cbreak();               // Disable line buffering
  noecho();               // Don't echo user input
  keypad(stdscr, TRUE);   // Enable special keys
  curs_set(0);            // Hide cursor
  nodelay(stdscr, TRUE);  // Non-blocking input
  ESCDELAY = 0;           // Remove ESC key delay

  if (has_colors()) {
    start_color();
    use_default_colors();  // Allow default terminal background
    // Define color pairs for tetromino pieces
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_MAGENTA, -1);
    init_pair(4, COLOR_GREEN, -1);
    init_pair(5, COLOR_RED, -1);
    init_pair(6, COLOR_BLUE, -1);
    init_pair(7, COLOR_WHITE, -1);
  }

  GameUI ui;
  // Main playfield window with borders
  ui.field = newwin(ROWS + 2, COLS * 2 + 2, 1, 1);
  // Next tetromino preview window
  ui.next = newwin(5, 10, 2, COLS * 2 + 4);
  // Scoreboard / stats window
  ui.counter = newwin(8, 24, ROWS - 5, COLS * 2 + 4);
  return ui;
}

void deinit_ui(GameUI *ui) {
  delwin(ui->field);    // Destroy playfield window
  delwin(ui->next);     // Destroy preview window
  delwin(ui->counter);  // Destroy scoreboard window
  endwin();             // Exit ncurses mode
}

static void draw_scoreboard(WINDOW *counter, GameInfo_t *game) {
  werase(counter);          // Clear window
  draw_fancy_box(counter);  // Draw border

  // Print high score, current score, and level
  wattron(counter, COLOR_PAIR(7) | A_BOLD);
  mvwprintw(counter, 1, 2, "[HIGH]  %05d", game->high_score);
  mvwprintw(counter, 2, 2, "[SCORE] %05d", game->score);
  mvwprintw(counter, 3, 2, "[LVL]   %03d", game->level);
  wattroff(counter, COLOR_PAIR(7) | A_BOLD);
}

static void draw_field(WINDOW *field, GameInfo_t *game) {
  werase(field);
  draw_fancy_box(field);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      int cell = game->field[i][j];
      mvwaddch(field, i + 1, j * 2 + 1, '.');  // Draw empty cell marker
      if (cell) {
        int x = 1 + j * 2;
        int y = 1 + i;
        // Draw filled cell as two blocks (for width balance)
        draw_block(field, y, x, cell);
        draw_block(field, y, x + 1, cell);
      }
    }
  }
}

static void draw_tetromino(WINDOW *field, tetromino *t, int base_row,
                           int base_col) {
  int color = (t->type % 7) + 1;  // Pick color based on type
  for (int k = 0; k < BLOCKS_PER_FIGURE; k++) {
    point p = t->blocks[k];
    int row = p.row + base_row + 1;
    int col = p.col + base_col;
    int x = 1 + col * 2;
    // Draw each block as two characters wide
    draw_block(field, row, x, color);
    draw_block(field, row, x + 1, color);
  }
}

static void draw_state_messages(WINDOW *field, GameInfo_t *game) {
  wattron(field, COLOR_PAIR(7) | A_BOLD);
  switch (game->state) {
    case GAME_OVER:
      mvwaddstr(field, ROWS / 2, COLS - 5, "[[GAME OVER]]");
      mvwaddstr(field, ROWS / 2 + 1, COLS - 10, "press ENTER to restart");
      break;
    case PAUSE:
      mvwaddstr(field, ROWS / 2, COLS - 4, "[[PAUSE]]");
      break;
    case START:
      mvwaddstr(field, ROWS / 2, COLS - 9, "press ENTER to start");
      break;
    default:
      break;
  }
  wattroff(field, COLOR_PAIR(7) | A_BOLD);
}

void s21_write_run(GameUI *ui) {
  GameInfo_t game = updateCurrentState();  // Get updated game state

  draw_field(ui->field, &game);         // Draw main field
  draw_scoreboard(ui->counter, &game);  // Draw scoreboard

  werase(ui->next);  // Clear preview window
  draw_fancy_box(ui->next);
  draw_tetromino(ui->next, &game.next, 0, 0);  // Draw "next" tetromino

  draw_tetromino(ui->field, &game.active,  // Draw current falling piece
                 game.active.position.row, game.active.position.col);

  draw_state_messages(ui->field, &game);  // Overlay pause/game-over text

  // Refresh all windows to display changes
  wrefresh(ui->field);
  wrefresh(ui->next);
  wrefresh(ui->counter);
}
