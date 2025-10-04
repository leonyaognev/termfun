#include "s21_tetris_logics.h"

int s21_check_collision(const game *const g, tetromino *const t) {
  for (point *p = t->blocks; p - t->blocks < BLOCKS_PER_FIGURE; p++) {
    int row = p->row + t->position.row, col = p->col + t->position.col;

    // If piece goes out of bottom or sides
    if (row >= ROWS || col >= COLS || col < 0) {
      return 1;
    }

    // If piece overlaps an already occupied cell
    if (g->field[row][col]) {
      return 1;
    }
  }
  return 0;  // No collision
}

void s21_fix_figute(game *const g, tetromino *const t) {
  for (point *p = t->blocks; p - t->blocks < BLOCKS_PER_FIGURE; p++) {
    int row = p->row + t->position.row, col = p->col + t->position.col;

    // Mark cell as filled with a number based on tetromino type (1–7)
    g->field[row][col] = (t->type % 7) + 1;
  }
}
