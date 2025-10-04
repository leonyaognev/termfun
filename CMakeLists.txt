cmake_minimum_required(VERSION 3.25)
project(tetris_snake LANGUAGES C CXX)

# ------------------------------
# Компиляторы и флаги
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -lstdc++")

# ------------------------------
# Пути к исходникам
# ------------------------------
set(TETRIS_SRC
    src/tetris/core_files/figure/core/s21_figures.c
    src/tetris/core_files/s21_brickgame_tetris.c
    src/tetris/core_files/s21_check_line.c
    src/tetris/core_files/s21_tetris_logics.c
    src/tetris/core_files/s21_tetris_step.c
)

set(SNAKE_SRC
    src/snake/core/brick_game_snake.cpp
    src/snake/core/snake.cpp
    src/snake/helpers/helpers.cpp
)

set(TUI_SRC
    src/tui/core_files/s21_tui_snake.cpp
    src/tui/core_files/s21_tui_tetris.c
    src/tui/helpers/s21_snake_input.cpp
    src/tui/helpers/s21_tetris_input.c
)

add_library(tui_lib STATIC ${TUI_SRC})

# ------------------------------
# Заголовочные пути
# ------------------------------
include_directories(
    src/tetris
    src/tetris/include
    src/tetris/core_files/figure/include
    src/snake
    src/snake/include
    src/snake/helpers
    src/vendor/logger
    src/tui
)

# ------------------------------
# Статические библиотеки
# ------------------------------
add_library(tetris_lib STATIC ${TETRIS_SRC})
add_library(snake_lib STATIC ${SNAKE_SRC})
add_library(tui_lib STATIC ${TUI_SRC})

# ------------------------------
# Исполняемый файл
# ------------------------------
add_executable(tetris_app tetris/main.cpp)
target_link_libraries(tetris_app PRIVATE tetris_lib snake_lib tui_lib ncurses)

# ------------------------------
# Тесты (если нужны)
# ------------------------------
enable_testing()
add_executable(test_app test/test.c)
target_link_libraries(test_app PRIVATE tetris_lib check)
add_test(NAME run_tests COMMAND test_app)
