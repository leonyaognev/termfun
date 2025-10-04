/**
 * @file s21_figures.h
 * @brief Tetromino shapes and operations for Tetris.
 */

#pragma once

/// Each tetromino is made of 4 blocks.
#define BLOCKS_PER_FIGURE 4

/// Number of rotation states (0–3).
#define MAX_ROTATIONS 4

/// Coordinate on the playfield.
typedef struct {
  int row;  ///< Row index.
  int col;  ///< Column index.
} point;

/// All possible tetromino types.
typedef enum {
  FIGURE_I,     ///< Line shape
  FIGURE_O,     ///< Square shape
  FIGURE_T,     ///< T shape
  FIGURE_L,     ///< L shape
  FIGURE_J,     ///< Mirrored L
  FIGURE_S,     ///< S shape
  FIGURE_Z,     ///< Z shape
  FIGURE_COUNT  ///< Total count
} figure_type;

/// Tetromino instance with type, rotation and position.
typedef struct tetromino {
  figure_type type;                 ///< Current figure type.
  figure_type next;                 ///< Next figure (preview).
  int rotation;                     ///< Rotation state 0–3.
  point blocks[BLOCKS_PER_FIGURE];  ///< Coordinates of 4 blocks.
  point position;                   ///< Top-left offset on field.
} tetromino;

/// Predefined shapes [type][rotation][block].
extern const point tetromino_shapes[FIGURE_COUNT][MAX_ROTATIONS]
                                   [BLOCKS_PER_FIGURE];

/**
 * @brief Initialize a tetromino with random type and position.
 * @param t Pointer to tetromino.
 */
void s21_create_tetromino(tetromino *t);

/**
 * @brief Rotate tetromino clockwise (updates block positions).
 * @param t Pointer to tetromino.
 */
void s21_rotate_tetromino(tetromino *t);

/**
 * @brief Move tetromino one row down.
 * @param t Pointer to tetromino.
 */
void s21_move_y_tetromino(tetromino *t);

/**
 * @brief Move tetromino horizontally.
 * @param t Pointer to tetromino.
 * @param move Direction: -1 for left, +1 for right.
 */
void s21_move_x_tetromino(tetromino *t, int move);
