#pragma once

#include <ncurses.h>

#include "snake.h"

struct SnakeUI {
  WINDOW* field;
  WINDOW* counter;
  WINDOW* control;
};

SnakeUI snake_init_ui(void);

void snake_deinit_ui(SnakeUI* ui);

void draw(SnakeUI* ui, const GameSnake& game);

int snake_recive_input(GameSnake& game);
