#pragma once

#include <deque>

#include "helpers.h"

/**
 * @namespace snakeSize
 * @brief Defines the fixed dimensions of the Snake game board.
 */
namespace snakeSize {
constexpr int ROWS = 20; /**< Number of rows in the board */
constexpr int COLS = 10; /**< Number of columns in the board */
};  // namespace snakeSize

/**
 * @enum CollisionType
 * @brief Represents possible collision outcomes for the snake.
 */
enum class CollisionType {
  None,  /**< No collision */
  Apple, /**< Snake collided with an apple */
  Wall,  /**< Snake collided with a wall */
  Self   /**< Snake collided with itself */
};

/**
 * @class Apple
 * @brief Represents an apple on the game board.
 *
 * Stores position coordinates of the apple and provides accessors.
 */
class Apple {
 private:
  int x; /**< X coordinate of the apple */
  int y; /**< Y coordinate of the apple */

 public:
  /**
   * @brief Default constructor.
   *
   * Places apple at a random position within board bounds.
   */
  Apple()
      : x(randomNumber(0, snakeSize::COLS)),
        y(randomNumber(0, snakeSize::ROWS)) {}

  /**
   * @brief Get the X coordinate of the apple.
   * @return X position
   */
  inline int getX() const noexcept { return x; }

  /**
   * @brief Get the Y coordinate of the apple.
   * @return Y position
   */
  inline int getY() const noexcept { return y; }
};

/**
 * @class Snake
 * @brief Represents the snake entity in the game.
 *
 * Manages the snake's body, movement, growth, and collision detection.
 */
class Snake {
 public:
  /**
   * @struct point
   * @brief Represents a coordinate (x, y) on the board.
   */
  struct point {
    int x; /**< X coordinate */
    int y; /**< Y coordinate */
  };

 private:
  std::deque<point> body; /**< Container for snake body segments */

  /**
   * @brief Check for collision of the snake with the board, itself, or an
   * apple.
   * @param a Reference to the apple instance
   * @return CollisionType Result of the collision check
   */
  CollisionType checkCollision(const Apple& a);

  /**
   * @brief Grow the snake by adding a new point.
   * @param p Reference to point where new segment will be added
   */
  void grow(point& p);

 public:
  /**
   * @brief Default constructor.
   *
   * Initializes snake body with default starting position.
   */
  Snake();

  /**
   * @brief Move the snake in the given direction.
   *
   * Updates the snake's body, checks collisions and apple consumption.
   *
   * @param dir Direction vector (point) to move
   * @param a Reference to the apple instance
   * @return CollisionType Collision result after movement
   */
  CollisionType move(point& dir, const Apple& a);

  /**
   * @brief Get the full body of the snake.
   * @return Deque of snake body points
   */
  inline std::deque<point> getBody() const noexcept { return body; }

  /**
   * @brief Get the position of the snake's head.
   * @return Point representing the head position
   */
  inline point getHead() const noexcept { return body.front(); }
};
