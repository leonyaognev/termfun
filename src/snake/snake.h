#pragma once

#include <filesystem>

#include "snake_core.h"

/// Possible user actions in the game.
enum class UserAction_s {
  Start,      ///< Start or restart the game
  Pause,      ///< Pause/resume
  Terminate,  ///< End the game
  Left,       ///< Move piece left
  Right,      ///< Move piece right
  Up,         ///< Rotate piece
  Down,       ///< Soft drop
  Action      ///< Hard drop
};

/// Current state of the game.
enum class gameState {
  MOVING,     ///< Piece is falling.
  PAUSE,      ///< Game is paused.
  GAME_OVER,  ///< No more moves left.
  START,      ///< Waiting to start.
  TERMINATED  ///< Exit game.
};

/// Class responsible for managing score, high score and game level
class scoreManager {
 public:
  int score;      ///< Current score
  int highScore;  ///< High score
  int level;      ///< Current level
  int speed;      ///< Tick speed modifier

  scoreManager();
  ~scoreManager();

  void operator++();

 private:
  std::filesystem::path file;
};

/// Main Snake game class
class GameSnake {
 public:
  Snake snake;
  Apple apple;
  gameState state = gameState::START;  ///< Current game state

  void userInput(UserAction_s action, bool hold);
  void updateCurrentState();

 private:
  scoreManager score;
  Snake::point dir = {1, 0};
};
