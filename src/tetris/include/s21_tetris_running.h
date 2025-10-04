/**
 * @file s21_tetris.h
 * @brief Game step execution and line checking for Tetris.
 */

#pragma once

#include "s21_tetris_logics.h"

/**
 * @brief Perform one game step (gravity + collision + new piece).
 * @param g Pointer to game state.
 * @param t Pointer to active tetromino.
 */
void s21_tetris_step(game *g, tetromino *t);

/**
 * @brief Check and clear filled lines, update score/level.
 * @param g Pointer to game state.
 */
void check_line(game *g);
