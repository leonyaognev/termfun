/**
 * @file s21_tui.h
 * @brief Terminal UI handling for Tetris using ncurses.
 */

#pragma once

#include <ncurses.h>

/// Represents all windows used in the Tetris UI.
typedef struct {
  WINDOW *field;    ///< Main playfield window
  WINDOW *next;     ///< Next tetromino preview window
  WINDOW *counter;  ///< Score/level display window
} GameUI;

/**
 * @brief Initialize the ncurses UI and create windows.
 * @return GameUI struct containing initialized windows.
 */
GameUI init_ui(void);

/**
 * @brief Deinitialize the UI and free windows.
 * @param ui Pointer to GameUI struct.
 */
void deinit_ui(GameUI *ui);

/**
 * @brief Draw the current game state in the UI.
 * @param ui Pointer to GameUI struct.
 */
void s21_write_run(GameUI *ui);

/**
 * @brief Capture user input from the terminal.
 * @return Integer representing key/action pressed.
 */
int s21_recive_input();
