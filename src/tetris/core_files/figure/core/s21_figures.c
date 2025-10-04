#include "s21_figures.h"

#include <stdlib.h>
#include <time.h>

#include "s21_tetris_logics.h"

const point tetromino_shapes[FIGURE_COUNT][MAX_ROTATIONS][BLOCKS_PER_FIGURE] = {
    // FIGURE_I
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}},
     {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
     {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
     {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},

    // FIGURE_O
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},

    // FIGURE_T
    {{{0, 0}, {0, 1}, {0, 2}, {1, 1}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 0}}},

    // FIGURE_L
    {{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
     {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{1, 0}, {1, 1}, {1, 2}, {0, 2}}},

    // FIGURE_J
    {{{0, 1}, {1, 1}, {2, 1}, {2, 0}},
     {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 0}, {0, 1}, {1, 0}, {2, 0}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 2}}},

    // FIGURE_S
    {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
     {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},

    // FIGURE_Z
    {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 0}},
     {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 0}}}};

void s21_create_tetromino(tetromino *t) {
  t->type = t->next;  // Take the "next" piece as current
  t->next = (figure_type)(rand() %
                          FIGURE_COUNT);  // Generate a new random "next" piece
  t->rotation = 0;                        // Start with default rotation (0)

  for (int i = 0; i < BLOCKS_PER_FIGURE; i++) {
    // Copy shape blocks from the template based on type and rotation
    t->blocks[i] = tetromino_shapes[t->type][t->rotation][i];
  }

  t->position.row = 0;               // Spawn at the top row
  t->position.col = (COLS / 2) - 2;  // Center horizontally on the board
}

void s21_rotate_tetromino(tetromino *t) {
  t->rotation = (t->rotation + 1) % MAX_ROTATIONS;  // Cycle rotation 0–3

  for (int i = 0; i < BLOCKS_PER_FIGURE; i++) {
    // Update blocks to match new rotation
    t->blocks[i] = tetromino_shapes[t->type][t->rotation][i];
  }
}

void s21_move_y_tetromino(tetromino *t) {
  t->position.row++;  // Move piece one row down
}

void s21_move_x_tetromino(tetromino *t, int move) {
  t->position.col += move;  // Shift piece horizontally (left = -1, right = +1)
}
