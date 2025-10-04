#include "helpers.h"

#include <random>

int randomNumber(int low, int high) {
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> dist(low, high - 1);
  return dist(gen);
}
