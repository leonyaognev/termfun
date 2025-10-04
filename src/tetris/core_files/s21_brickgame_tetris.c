#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "s21_figures.h"
#include "s21_tetris.h"
#include "s21_tetris_logics.h"
#include "s21_tetris_running.h"

static game g;
static tetromino t = {FIGURE_I, FIGURE_I, 0, {{0, 0}}, {0, 0}};

static GameInfo_t info;
static int field_buffer[ROWS][COLS];

__attribute__((constructor)) static void s21_init_tetris() {
  const char *home = getenv("HOME");
  if (!home) home = ".";

  char path[512];

  strncpy(path, home, sizeof(path));
  path[sizeof(path) - 1] = '\0';
  strncat(path, "/.cache/tetris", sizeof(path) - strlen(path) - 1);

  mkdir(path, 0755);  // Create cache directory if it doesn't exist

  char file[512];
  strncpy(file, path, sizeof(file));
  file[sizeof(file) - 1] = '\0';
  strncat(file, "/best_score.bin", sizeof(file) - strlen(file) - 1);

  FILE *f = fopen(file, "rb");
  if (!f) {
    g.counter.highscore = 0;  // No file yet → set highscore to 0
  } else {
    if (fseek(f, 0, SEEK_END) == 0) {
      long sz = ftell(f);
      if (sz >= (long)sizeof(unsigned int)) {
        // If file has enough bytes, read highscore
        rewind(f);
        fread(&g.counter.highscore, sizeof(unsigned int), 1, f);
      } else {
        g.counter.highscore = 0;  // Corrupt or too small file
      }
    } else {
      g.counter.highscore = 0;  // Failed to seek
    }
    fclose(f);
  }

  srand(time(NULL));  // Seed random generator
  g.state = START;    // Initial game state

  // Spawn tetromino outside visible area until game starts
  t.position.row = -10;
  t.position.col = -10;

  // Allocate game field pointer array
  info.field = (int **)malloc(sizeof(int *) * ROWS);
  for (int i = 0; i < ROWS; i++) {
    info.field[i] = field_buffer[i];  // Point to static buffer rows
  }
}

__attribute__((destructor)) static void s21_deinit_tetris() {
  const char *home = getenv("HOME");
  if (!home) home = ".";

  char path[512];

  strncpy(path, home, sizeof(path));
  path[sizeof(path) - 1] = '\0';
  strncat(path, "/.cache/tetris", sizeof(path) - strlen(path) - 1);

  mkdir(path, 0755);  // Make sure directory exists
  char file[512];

  strncpy(file, path, sizeof(file));
  file[sizeof(file) - 1] = '\0';
  strncat(file, "/best_score.bin", sizeof(file) - strlen(file) - 1);

  FILE *f = fopen(file, "wb");
  if (f) {
    // Save current highscore to file
    unsigned int hs = g.counter.highscore;
    fwrite(&hs, sizeof(unsigned int), 1, f);
    fclose(f);
  }

  free(info.field);  // Release memory for field pointers
}

static void s21_new_field() {
  s21_create_tetromino(&t);  // Prepare first tetromino

  int hight = g.counter.highscore;
  memset(&g, 0, sizeof(game));  // Reset game struct
  g.counter.highscore = hight;  // Restore highscore
}

void userInput(UserAction_t action, bool hold) {
  if ((g.state == PAUSE && action != Pause) ||
      (g.state == GAME_OVER && action != Start) ||
      ((g.state == START) && action != Start) ||
      (g.state != START && g.state != GAME_OVER && action == Start)) {
    return;  // Ignore invalid actions for current state
  }
  (void)hold;  // Suppress unused warning

  tetromino buf = t;  // Backup current tetromino

  switch (action) {
    case Start:
      s21_new_field();   // Reset game
      g.state = MOVING;  // Enter active state
      break;
    case Pause:
      // Toggle between pause and moving
      g.state = (g.state == PAUSE ? MOVING : PAUSE);
      break;
    case Terminate:
      g.state = GAME_OVER;
      break;
    case Left:
      s21_move_x_tetromino(&t, -1);  // Try move left
      if (s21_check_collision(&g, &t))
        s21_move_x_tetromino(&t, +1);  // Undo if blocked
      break;
    case Right:
      s21_move_x_tetromino(&t, +1);  // Try move right
      if (s21_check_collision(&g, &t))
        s21_move_x_tetromino(&t, -1);  // Undo if blocked
      break;
    case Down:
      s21_move_y_tetromino(&t);  // Soft drop
      if (s21_check_collision(&g, &t)) {
        t = buf;                   // Restore position if hit something
        s21_fix_figute(&g, &t);    // Lock piece in place
        s21_create_tetromino(&t);  // Spawn new piece
      }
      break;
    case Up:
      s21_rotate_tetromino(&t);  // Rotate clockwise
      if (s21_check_collision(&g, &t)) {
        // Rotate back (3 times = one counter-clockwise)
        for (int i = 0; i < 3; i++) s21_rotate_tetromino(&t);
      }
      break;
    case Action:
      // Hard drop until collision
      while (!s21_check_collision(&g, &t)) {
        s21_move_y_tetromino(&t);
      }
      t.position.row--;          // Step back from collision point
      s21_fix_figute(&g, &t);    // Lock tetromino
      s21_create_tetromino(&t);  // Spawn new one
      break;
  }
}

GameInfo_t updateCurrentState() {
  tetromino next = t;
  info.score = g.counter.score;
  info.high_score = g.counter.highscore;
  info.level = g.counter.level;
  info.speed = 1;  // Speed is constant (can be adjusted later)
  info.state = g.state;
  info.active = t;

  s21_create_tetromino(&next);  // Predict next piece
  info.next = next;

  if (!info.state) {
    s21_tetris_step(&g, &t);  // Advance game logic
    check_line(&g);           // Clear full lines
  }

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      field_buffer[i][j] = g.field[i][j];  // Copy board state to buffer
    }
  }

  return info;
}
