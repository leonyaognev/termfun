/**
 * @file s21_tetris_logics.h
 * @brief Core game logic structures and functions for Tetris.
 */

#pragma once

#include "s21_figures.h"

/// Number of rows in the game field.
#define ROWS 20
/// Number of columns in the game field.
#define COLS 10

/// Current state of the game.
typedef enum {
  MOVING,     ///< Piece is falling.
  PAUSE,      ///< Game is paused.
  GAME_OVER,  ///< No more moves left.
  START       ///< Waiting to start.
} game_state;

/// Score, highscore and level tracking.
typedef struct {
  unsigned int score;      ///< Current score.
  unsigned int highscore;  ///< Best score saved.
  int level;               ///< Current level (affects speed).
} game_counter;

/// Full game state including field, counter and status.
typedef struct {
  int field[ROWS][COLS];  ///< Playfield grid.
  unsigned int tick;      ///< Internal tick counter.
  game_counter counter;   ///< Score and level info.
  game_state state;       ///< Current game state.
} game;

/**
 * @brief Check if a tetromino collides with walls, floor or blocks.
 * @param g Pointer to current game state.
 * @param t Pointer to tetromino.
 * @return 1 if collision detected, 0 otherwise.
 */
int s21_check_collision(const game *const g, tetromino *const t);

/**
 * @brief Fix tetromino blocks into the playfield after landing.
 * @param g Pointer to current game state.
 * @param t Pointer to tetromino.
 */
void s21_fix_figute(game *const g, tetromino *const t);
