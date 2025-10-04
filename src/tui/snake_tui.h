#pragma once

#include <ncurses.h>

#include "snake.h"

struct SnakeUI {
  WINDOW* field;
  WINDOW* counter;
};

SnakeUI snake_init_ui(void);

void snake_deinit_ui(SnakeUI* ui);

void draw(SnakeUI* ui, const Snake& snake, const Apple& apple);

int snake_recive_input(GameSnake& game);
