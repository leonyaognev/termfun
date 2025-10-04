#include "../include/snake.h"

Snake::Snake() {
  body.push_front({0, 0});
  body.push_front({0, 1});
  body.push_front({0, 2});
}

int Snake::checkCollision(Apple& a) {
  point head = getHead();
  if (head.x < 0 || head.y < 0 || head.x >= snakeSize::COLS ||
      head.y >= snakeSize::ROWS) {
    return 1;
  }

  for (auto it = ++body.begin(); it != body.end(); ++it) {
    if (head.x == it->x && head.y == it->y) {
      return 1;
    }
  }

  if (a.getX() == head.x && a.getY() == head.y) {
    a = Apple();
    return 2;
  }

  return 0;
}

void Snake::move(point& dir, Apple& a) {
  point newHead = {body.front().x + dir.x, body.front().y + dir.y};

  switch (checkCollision(a)) {
    case 1:
      throw penis();
    case 2:
      break;
    default:
      body.pop_back();
      break;
  }
  body.push_front(newHead);
}
