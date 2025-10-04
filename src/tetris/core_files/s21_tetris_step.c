#include <ncurses.h>
#include <unistd.h>

#include "s21_figures.h"
#include "s21_tetris_logics.h"

void s21_tetris_step(game *g, tetromino *t) {
  tetromino buf = *t;  // Keep a copy in case of collision rollback

  // Falling speed decreases as level grows (faster game at higher levels)
  int step = 100 - g->counter.level * 7;

  usleep(10000);    // Small delay to control update frequency
  g->tick++;        // Increase internal tick counter
  g->tick %= step;  // Wrap tick to keep consistent timing

  if (g->tick == 0) {
    s21_move_y_tetromino(t);  // Move piece one step down

    int collision = s21_check_collision(g, t);
    if (collision) {
      *t = buf;                 // Roll back movement
      s21_fix_figute(g, t);     // Lock piece into the field
      s21_create_tetromino(t);  // Spawn a new tetromino

      collision = s21_check_collision(g, t);
      if (collision) {
        *t = buf;  // If even the new piece collides → game over
        g->state = GAME_OVER;
      }
    }
  }
}
