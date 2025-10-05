#pragma once

#include <filesystem>

#include "snake_core.h"

/**
 * @brief Possible user actions during the game.
 */
enum class UserAction_s {
  Start,     /**< Start or restart the game */
  Pause,     /**< Pause or resume the game */
  Terminate, /**< Terminate the game and exit */
  Left,      /**< Move snake left */
  Right,     /**< Move snake right */
  Up,        /**< Move snake up */
  Down,      /**< Move snake down */
  Action     /**< Reserved for future use (hard drop / special action) */
};

/**
 * @brief Current state of the game.
 */
enum class gameState {
  MOVING,    /**< The snake is moving, game is running */
  PAUSE,     /**< Game is paused */
  GAME_OVER, /**< Snake collided, game is over */
  START,     /**< Waiting for the game to start */
  TERMINATED /**< Game terminated by the player */
};

/**
 * @brief Manages score, high score, level and speed progression.
 */
class scoreManager {
 public:
  int score;     /**< Current score */
  int highScore; /**< Saved high score */
  int level;     /**< Current game level */
  int speed;     /**< Current tick speed modifier */

  /**
   * @brief Default constructor.
   *
   * Loads saved high score from file if available.
   */
  scoreManager();

  /**
   * @brief Destructor.
   *
   * Stores high score to file if needed.
   */
  ~scoreManager();

  /**
   * @brief Increment score operator.
   *
   * Updates score and adjusts level/speed when thresholds are reached.
   */
  void operator++();

 private:
  /**
   * @brief Path to file used for storing high score.
   */
  std::filesystem::path file;
};

/**
 * @brief Main Snake game class.
 *
 * Handles snake movement, collisions, score management, and input.
 */
class GameSnake {
 public:
  Snake snake;                        /**< Snake instance */
  Apple apple;                        /**< Apple instance */
  gameState state = gameState::START; /**< Current game state */

  /**
   * @brief Process user input.
   *
   * @param action The user action
   * @param hold True if key is held, false if it’s a single press
   */
  void userInput(UserAction_s action, bool hold);

  /**
   * @brief Update current game state.
   *
   * Handles movement, collision checks and apple consumption.
   */
  void updateCurrentState();

 private:
  scoreManager score;        /**< Score manager instance */
  Snake::point dir = {1, 0}; /**< Current snake direction vector */
};
