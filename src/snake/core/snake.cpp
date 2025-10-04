#include "snake.h"

Snake::Snake() {
  body.push_front({0, 0});
  body.push_front({0, 1});
  body.push_front({0, 2});
}

CollisionType Snake::checkCollision(const Apple& a) {
  point head = getHead();
  if (head.x < 0 || head.y < 0 || head.x >= snakeSize::COLS ||
      head.y >= snakeSize::ROWS) {
    return CollisionType::Wall;
  }

  for (auto it = ++body.begin(); it != body.end(); ++it) {
    if (head.x == it->x && head.y == it->y) {
      return CollisionType::Self;
    }
  }

  if (a.getX() == head.x && a.getY() == head.y) {
    return CollisionType::Apple;
  }

  return CollisionType::None;
}

CollisionType Snake::move(point& dir, const Apple& a) {
  point newHead = {body.front().x + dir.x, body.front().y + dir.y};

  CollisionType collision = checkCollision(a);
  switch (collision) {
    case CollisionType::Self:
    case CollisionType::Wall:
      break;
    case CollisionType::Apple:
      body.push_front(newHead);
      break;
    default:
      body.pop_back();
      body.push_front(newHead);
      break;
  }
  return collision;
}
