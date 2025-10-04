#pragma once

#include <deque>

#include "helpers.h"

namespace snakeSize {
constexpr int ROWS = 20;
constexpr int COLS = 10;
};  // namespace snakeSize

enum class CollisionType { None, Apple, Wall, Self };

class Apple {
 private:
  int x;
  int y;

 public:
  Apple()
      : x(randomNumber(0, snakeSize::COLS)),
        y(randomNumber(0, snakeSize::ROWS)) {}

  inline int getX() const noexcept { return x; }
  inline int getY() const noexcept { return y; }
};

class Snake {
 public:
  struct point {
    int x, y;
  };

 private:
  std::deque<point> body;

  CollisionType checkCollision(const Apple& a);
  void grow(point& p);

 public:
  Snake();

  CollisionType move(point& dir, const Apple& a);
  inline std::deque<point> getBody() const noexcept { return body; };
  inline point getHead() const noexcept { return body.front(); }
};
