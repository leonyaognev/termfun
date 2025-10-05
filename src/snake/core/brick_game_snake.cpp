#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "logger.h"
#include "snake.h"
#include "snake_core.h"

scoreManager::scoreManager() : score(0), highScore(0), level(0), speed(1) {
  const char* home = std::getenv("HOME");
  std::string base = home ? home : ".";

  /** Create directory for storing high score data */
  std::filesystem::path path = base + "/.cache/termfun";
  std::filesystem::create_directory(path);
  file = path / "best_score.bin";

  /** Try to open the file with the best score */
  std::ifstream in(file, std::ios::binary);
  if (!in) {
    log_warn("Error opening file to read best result.");
    throw std::runtime_error("Error opening file.");
  }

  /** Read best score, if failed set to 0 */
  if (!in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore))) {
    highScore = 0;
  }

  log_info("scoreManager initialized: highScore = %d", highScore);
}

scoreManager::~scoreManager() {
  /** Save high score at game exit */
  std::ofstream out(file, std::ios::binary);
  if (!out) {
    log_warn("Error opening file to record best result.");
    return;
  }
  out.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));
  log_info("High score saved: %d", highScore);
}

void scoreManager::operator++() {
  /** Increase score */
  ++score;
  log_trace("Score incremented: %d", score);

  /** Update high score if broken */
  if (score > highScore) {
    highScore = score;
    log_info("New high score: %d", highScore);
  }

  /** Every 5 points increase level and speed */
  if (score % 5 == 0) {
    ++level;
    ++speed;
    log_info("Level increased to %d, Speed increased to %d", level, speed);
  }
}

void GameSnake::userInput(UserAction_s action, bool hold) {
  log_info("userInput called with action: %d", static_cast<int>(action));
  (void)hold;

  /** Validate action against current game state */
  if ((state == gameState::PAUSE && action != UserAction_s::Pause) ||
      (state == gameState::GAME_OVER && action != UserAction_s::Start) ||
      ((state == gameState::START) && action != UserAction_s::Start) ||
      (state != gameState::START && state != gameState::GAME_OVER &&
       action == UserAction_s::Start)) {
    log_warn("Invalid action %d for current state %d", static_cast<int>(action),
             static_cast<int>(state));
    return;  ///< Ignore invalid actions
  }

  /** Handle player actions */
  switch (action) {
    case UserAction_s::Start:
      *this = GameSnake();       /**< Restart the game */
      state = gameState::MOVING; /**< Start moving */
      log_info("Game started");
      break;
    case UserAction_s::Pause:
      state = state == gameState::PAUSE ? gameState::MOVING : gameState::PAUSE;
      log_info("Game %s", state == gameState::PAUSE ? "paused" : "resumed");
      break;
    case UserAction_s::Terminate:
      state = gameState::TERMINATED;
      log_info("Game terminated by player");
      break;
    case UserAction_s::Left:
      dir = {-1, 0};
      log_trace("Direction set to LEFT");
      break;
    case UserAction_s::Right:
      dir = {1, 0};
      log_trace("Direction set to RIGHT");
      break;
    case UserAction_s::Up:
      dir = {0, -1};
      log_trace("Direction set to UP");
      break;
    case UserAction_s::Down:
      dir = {0, 1};
      log_trace("Direction set to DOWN");
      break;
    case UserAction_s::Action:
      log_info("Action button pressed (no effect)");
      break;
    default:
      log_warn("Incorrect action received: %d", static_cast<int>(action));
  }
}

void GameSnake::updateCurrentState() {
  /** Update game only if snake is moving */
  if (state == gameState::MOVING) {
    log_trace("Updating game state: snake is moving");

    /** Check snake collision */
    CollisionType collision = snake.move(dir, apple);
    switch (collision) {
      case CollisionType::Wall:
        log_error("Snake collided with wall, game over");
        state = gameState::GAME_OVER;
        break;
      case CollisionType::Self:
        log_error("Snake collided with itself, game over");
        state = gameState::GAME_OVER;
        break;
      case CollisionType::Apple:
        log_info("Apple eaten, spawning new apple");
        apple = Apple(); /**< Spawn new apple */
        ++score;         /**< Increase score */
        break;
      case CollisionType::None:
        log_trace("No collision detected");
        break;
      default:
        log_warn("Unexpected collision type");
        break;
    }
  }
}
