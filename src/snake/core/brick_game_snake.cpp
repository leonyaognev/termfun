#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "logger.h"
#include "snake.h"

// =======================
// scoreManager
// =======================

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

// =======================
// GameSnake
// =======================

void GameSnake::userInput(UserAction_s action, bool hold) {
  log_info("userInput called with action: %d", static_cast<int>(action));
  (void)hold;  // suppress unused warning

  switch (action) {
    case UserAction_s::Start:
    case UserAction_s::Pause:
    case UserAction_s::Terminate:
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
  CollisionType collision = snake.move(dir, apple);
  if (collision == CollisionType::Wall || collision == CollisionType::Self) {
    apple = Apple();
    ++score;
  }
}
