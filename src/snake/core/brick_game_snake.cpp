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

  std::filesystem::path path = base + "/.cache/termfun";
  std::filesystem::create_directory(path);
  file = path / "best_score.bin";

  std::ifstream in(file, std::ios::binary);
  if (!in) {
    log_warn("Error opening file to reading best result.");
    throw std::runtime_error("Error opening file.");
  }

  if (!in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore))) {
    highScore = 0;
  }
}

scoreManager::~scoreManager() {
  std::ofstream out(file, std::ios::binary);
  if (!out) {
    log_warn("Error opening file to record best result.");
    return;
  }
  out.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));
}

void scoreManager::operator++() {
  ++score;
  if (score > highScore) {
    highScore = score;
  }
  if (score % 5 == 0) {
    ++level;
    ++speed;
  }
}

void GameSnake::userInput(UserAction_s action, bool hold) {
  log_info("userInput called with action: %d", static_cast<int>(action));
  (void)hold;

  if ((state == gameState::PAUSE && action != UserAction_s::Pause) ||
      (state == gameState::GAME_OVER && action != UserAction_s::Start) ||
      ((state == gameState::START) && action != UserAction_s::Start) ||
      (state != gameState::START && state != gameState::GAME_OVER &&
       action == UserAction_s::Start)) {
    return;  // Ignore invalid actions for current state
  }

  switch (action) {
    case UserAction_s::Start:
      *this = GameSnake();
      state = gameState::MOVING;
      break;
    case UserAction_s::Pause:
      state = state == gameState::PAUSE ? gameState::MOVING : gameState::PAUSE;
      break;
    case UserAction_s::Terminate:
      state = gameState::TERMINATED;
      break;
    case UserAction_s::Left:
      dir = {-1, 0};
      break;
    case UserAction_s::Right:
      dir = {1, 0};
      break;
    case UserAction_s::Up:
      dir = {0, -1};
      break;
    case UserAction_s::Down:
      dir = {0, 1};
      break;
    case UserAction_s::Action:
      break;
    default:
      log_warn("incorrect action");
  }
}

void GameSnake::updateCurrentState() {
  if (state == gameState::MOVING) {
    CollisionType collision = snake.move(dir, apple);
    switch (collision) {
      case CollisionType::Wall:
      case CollisionType::Self:
        state = gameState::GAME_OVER;
        break;
      case CollisionType::Apple:
        apple = Apple();
        ++score;
        break;
      default:
        break;
    }
  }
}
