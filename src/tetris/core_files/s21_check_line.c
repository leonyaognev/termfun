#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "s21_tetris_logics.h"

static void set_score(game *g, int lines) {
  switch (lines) {
    case 1:
      g->counter.score += 100;  // Single line clear
      break;
    case 2:
      g->counter.score += 300;  // Double line clear
      break;
    case 3:
      g->counter.score += 700;  // Triple line clear
      break;
    case 4:
      g->counter.score += 1500;  // Tetris (4 lines at once)
      break;
  }

  // Update high score if beaten
  if (g->counter.score > g->counter.highscore) {
    g->counter.highscore = g->counter.score;
  }

  // Level increases every ~600 points, capped at level 10
  if (g->counter.level < 10) {
    g->counter.level = g->counter.score / 600;
  }
}

void check_line(game *g) {
  int lines = 0;

  for (int row = 0; row < ROWS; row++) {
    int check = 1;
    for (int col = 0; col < COLS; col++) {
      check = check &&
              g->field[row][col];  // Row is full only if all cells are non-zero
    }

    if (check) {
      // Clear the full row
      memset(g->field[row], 0, COLS * sizeof(int));

      // Shift everything above down by one
      for (int r = row; r >= 0; r--) {
        memcpy(g->field[r], g->field[r - 1], COLS * sizeof(int));
      };

      // Clear the top row after shifting
      memset(g->field[0], 0, COLS * sizeof(int));

      lines++;  // Count how many rows cleared in this step
    }
  }

  // Apply score and level changes if any lines were cleared
  if (lines) {
    set_score(g, lines);
  }
}
