# ================================
# Compiler & Flags
# ================================
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -x c++ -std=c++20 -lstdc++
GCOV_FLAGS  = -fprofile-arcs -ftest-coverage -I./src/test/include/

OS := $(shell uname -s)

# ================================
# Paths & Headers
# ================================
SRC_DIR     = ./src
TETRIS_DIR  = $(SRC_DIR)/tetris
SNAKE_DIR   = $(SRC_DIR)/snake
TUI_DIR     = $(SRC_DIR)/tui
CORE_DIR    = $(SRC_DIR)/core_files
TEST_DIR    = ./test
BUILD_DIR   = ./build

TETRIS_HEADERS = -I$(TETRIS_DIR)/core_files/figure/include \
                 -I$(TETRIS_DIR)/include \
                 -I$(TETRIS_DIR)
SNAKE_HEADERS  = -I$(SNAKE_DIR)/include -I$(SNAKE_DIR)/ -I./src/vendor/logger/
TUI_HEADERS    = -I$(TUI_DIR)

# ================================
# Source Files
# ================================
SRC_TETRIS := $(wildcard $(TETRIS_DIR)/core_files/figure/core/*.c \
                          $(TETRIS_DIR)/core_files/*.c)
SRC_SNAKE  := $(wildcard $(SNAKE_DIR)/core/*.cpp $(SNAKE_DIR)/*.cpp $(SNAKE_DIR)/helpers/*.cpp)
SRC_TUI    := $(wildcard $(TUI_DIR)/core_files/*.c $(TUI_DIR)/helpers/*.c $(TUI_DIR)/helpers/*.cpp $(TUI_DIR)/core_files/*.cpp)
SRC_CORE   := $(wildcard $(CORE_DIR)/*.c)

TEST_SRC   := $(wildcard $(TEST_DIR)/*.c)

# ================================
# Objects
# ================================
OBJ_TETRIS   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/obj/%.o, $(SRC_TETRIS))
OBJ_SNAKE    := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/obj/%.o, $(SRC_SNAKE))
OBJ_TUI_C    := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/obj/%.o, $(SRC_TUI))
OBJ_TUI_CPP  := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/obj/%.o, $(SRC_TUI))
OBJ_GCOV     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/gcov/%.o, $(SRC_TETRIS))

# ================================
# Targets
# ================================
TETRIS       = $(BUILD_DIR)/s21_tetris_logics_lib.a
SNAKE        = $(BUILD_DIR)/s21_snake_lib.a
TUI          = $(BUILD_DIR)/s21_tui_lib.a
TETRIS_GCOV  = $(BUILD_DIR)/s21_tetris_logics_gcov.a

# ================================
# Progress Bar
# ================================
TERM_WIDTH := $(shell stty size 2>/dev/null | awk '{print $$2}')
TERM_WIDTH := $(if $(TERM_WIDTH),$(TERM_WIDTH),80)
BAR_WIDTH  := $(shell expr $(TERM_WIDTH) \* 80 / 100 - 20)

TOTAL_TETRIS := $(words $(OBJ_TETRIS))
TOTAL_SNAKE  := $(words $(OBJ_SNAKE))
TOTAL_TUI    := $(words $(OBJ_TUI_C) $(OBJ_TUI_CPP))
TOTAL_GCOV   := $(words $(OBJ_GCOV))
DONE_TETRIS  := 0
DONE_SNAKE   := 0
DONE_TUI     := 0
DONE_GCOV    := 0

define print_bar
FILLED=$$(( ($1 * $(BAR_WIDTH)) / $2 )); \
UNFILLED=$$(( $(BAR_WIDTH) - FILLED )); \
BAR=$$(printf "%0.s▓" $$(seq 1 $$FILLED)); \
SP=""; \
if [ $$UNFILLED -gt 0 ]; then \
  SP=$$(printf "%0.s░" $$(seq 1 $$UNFILLED)); \
fi; \
PCT=$$(( 100 * $1 / $2 )); \
printf "\r$3 [$$BAR$$SP] %3d%%" $$PCT; \
if [ $1 -eq $2 ]; then printf "\n"; fi
endef

# ================================
# Rules
# ================================
all: install gcov_report valgrind_test dvi dist

# ---------------- TETRIS ----------------
$(BUILD_DIR)/obj/tetris/%.o: $(TETRIS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(TETRIS_HEADERS) $(CFLAGS) -c $< -o $@
	$(eval DONE_TETRIS := $(shell expr $(DONE_TETRIS) + 1))
	@$(call print_bar,$(DONE_TETRIS),$(TOTAL_TETRIS),Tetris compilation: )

$(TETRIS): mkbuild $(OBJ_TETRIS)
	@ar rcs $@ $(OBJ_TETRIS)

# ---------------- SNAKE ----------------
$(BUILD_DIR)/obj/snake/%.o: $(SNAKE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(SNAKE_HEADERS) $(CFLAGS) -c $< -o $@
	$(eval DONE_SNAKE := $(shell expr $(DONE_SNAKE) + 1))
	@$(call print_bar,$(DONE_SNAKE),$(TOTAL_SNAKE),Snake compilation: )

$(SNAKE): mkbuild $(OBJ_SNAKE)
	@ar rcs $@ $(OBJ_SNAKE)

# ---------------- TUI ----------------
$(BUILD_DIR)/obj/tui/%.o: $(TUI_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(TUI_HEADERS) $(TETRIS_HEADERS) $(SNAKE_HEADERS) $(CFLAGS) -c $< -o $@
	$(eval DONE_TUI := $(shell expr $(DONE_TUI) + 1))
	@$(call print_bar,$(DONE_TUI),$(TOTAL_TUI),TUI compilation: )

$(BUILD_DIR)/obj/tui/%.o: $(TUI_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(TUI_HEADERS) $(TETRIS_HEADERS) $(SNAKE_HEADERS) $(CFLAGS) -c $< -o $@
	$(eval DONE_TUI := $(shell expr $(DONE_TUI) + 1))
	@$(call print_bar,$(DONE_TUI),$(TOTAL_TUI),TUI compilation: )

$(TUI): mkbuild $(OBJ_TUI_C) $(OBJ_TUI_CPP)
	@ar rcs $@ $(OBJ_TUI_C) $(OBJ_TUI_CPP)

# ---------------- Install ----------------
install: $(TETRIS) $(SNAKE) $(TUI)
	@$(CC) $(CFLAGS) $(TETRIS_HEADERS) $(TUI_HEADERS) $(SNAKE_HEADERS) ./tetris/main.cpp \
		-L$(BUILD_DIR)/ -l:s21_tui_lib.a \
		-L$(BUILD_DIR)/ -l:s21_tetris_logics_lib.a \
		-L$(BUILD_DIR)/ -l:s21_snake_lib.a \
		-lncurses -o ./build/tetris

uninstall:
	@rm -rf ~/.cache/tetris
	@rm -rf ./build/tetris

# ---------------- Tests & Coverage ----------------
mkbuild:
	@mkdir -p $(BUILD_DIR)/coverage_html $(BUILD_DIR)/obj $(BUILD_DIR)/gcov $(BUILD_DIR)/dist

$(BUILD_DIR)/gcov/tetris/%.o: $(TETRIS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(GCOV_FLAGS) $(CFLAGS) $(TETRIS_HEADERS) -c $< -o $@
	$(eval DONE_GCOV := $(shell expr $(DONE_GCOV) + 1))
	@$(call print_bar,$(DONE_GCOV),$(TOTAL_GCOV),Tetris GCOV: )

$(TETRIS_GCOV): mkbuild $(OBJ_GCOV)
	@ar rcs $@ $(OBJ_GCOV)

test: $(TETRIS_GCOV)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) -O0 \
		$(TEST_DIR)/test.c \
		-L$(BUILD_DIR) -l:s21_tetris_logics_gcov.a \
		$(TETRIS_HEADERS) -lcheck -o $(BUILD_DIR)/gcov/test

run_test: test
	@echo
	@echo "Run tests..."
	@./build/gcov/test > /tmp/s21_matrix_test.log 2>&1 || true
	@if grep -q "Failures: 0, Errors: 0" /tmp/s21_matrix_test.log; then \
		echo "SUCCESFUL! ✅"; \
	else \
		echo "FAIL! 💥"; \
		cat /tmp/s21_matrix_test.log; \
	fi
	@rm -f /tmp/s21_matrix_test.log
	@echo

gcov_report: run_test
	@echo "Generating coverage report..."
	@lcov --capture --directory $(BUILD_DIR)/gcov/ --output-file $(BUILD_DIR)/coverage_html/base.info > /dev/null 2>&1
	@genhtml $(BUILD_DIR)/coverage_html/base.info --output-directory $(BUILD_DIR)/coverage_html > /dev/null 2>&1
	@echo "Coverage report: build/coverage_html/index.html ✅"
	@echo

valgrind_test: test
	@if [ "$(OS)" = "Linux" ]; then \
		echo "Run tests via valgrind..."; \
		CK_FORK=no valgrind --error-exitcode=1 --tool=memcheck --leak-check=yes $(BUILD_DIR)/gcov/test > /tmp/valgrind.log 2>&1 || true; \
	else \
		echo "Run tests via leaks..."; \
		leaks -atExit -- ./build/test > /tmp/valgrind.log 2>&1 || true; \
	fi
	@if grep -q "ERROR SUMMARY: 0 errors" /tmp/valgrind.log; then \
		echo "SUCCESFUL! ✅"; \
	else \
		echo "FAIL! 💥"; \
		cat /tmp/valgrind.log; \
	fi
	@rm -f /tmp/valgrind.log
	@echo

# ---------------- Docs & Dist ----------------
dvi: mkbuild
	@doxygen > /dev/null 2>&1 || true
	@echo "Documentation created: build/docs ✅"

dist: mkbuild
	@tar -czf $(BUILD_DIR)/dist/tetris.tar.gz $(SRC_DIR) $(TEST_DIR) ./Makefile ./Doxyfile || true
	@echo "Archive created: /build/dist/tetris.tar.gz ✅"

# ---------------- Maintenance ----------------
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf ./app.log ./best_score.bin

rebuild: clean all

.PHONY: all install uninstall test run_test mkbuild gcov_report valgrind_test dvi dist clean rebuild
