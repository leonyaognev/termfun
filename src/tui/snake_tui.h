#pragma once

#include <ncurses.h>

#include "snake.h"

struct GameUI {
  WINDOW* field;
  WINDOW* counter;
};

GameUI snake_init_ui(void);

void snake_deinit_ui(GameUI* ui);

void draw(GameUI* ui, const Snake& snake, const Apple& apple);

int snake_recive_input(GameSnake& game);
