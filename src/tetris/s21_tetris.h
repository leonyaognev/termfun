/**
 * @file s21_tetris_running.h
 * @brief Handles user input and updates game state for Tetris.
 */

#pragma once

#include <stdbool.h>

#include "s21_figures.h"
#include "s21_tetris_logics.h"

/// Possible user actions in the game.
typedef enum {
  Start,      ///< Start or restart the game
  Pause,      ///< Pause/resume
  Terminate,  ///< End the game
  Left,       ///< Move piece left
  Right,      ///< Move piece right
  Up,         ///< Rotate piece
  Down,       ///< Soft drop
  Action      ///< Hard drop
} UserAction_t;

/// Snapshot of the current game state for UI or logic.
typedef struct {
  int **field;       ///< Pointer to 2D playfield array
  tetromino active;  ///< Active tetromino
  tetromino next;    ///< Next tetromino
  int score;         ///< Current score
  int high_score;    ///< High score
  int level;         ///< Current level
  int speed;         ///< Tick speed modifier
  int state;         ///< Current game state (see enum state)
} GameInfo_t;

/**
 * @brief Process a user action and update game state.
 * @param action The action performed by the user.
 * @param hold Currently unused parameter (reserved for future features).
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Get the current game state snapshot.
 * @return GameInfo_t containing field, scores, level, and active pieces.
 */
GameInfo_t updateCurrentState();
