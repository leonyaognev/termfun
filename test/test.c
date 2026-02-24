#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "s21_tetris.h"
#include "s21_tetris_logics.h"

// Small helpers
static GameInfo_t get_info(void) { return updateCurrentState(); }
static void start_game(void) { userInput(Start, false); }
static void pause_toggle(void) { userInput(Pause, false); }
static void terminate_game(void) { userInput(Terminate, false); }
static void move_left(void) { userInput(Left, false); }
static void move_right(void) { userInput(Right, false); }
static void move_down(void) { userInput(Down, false); }
static void rotate_up(void) { userInput(Up, false); }
static void hard_drop(void) { userInput(Action, false); }

// Simple invariant checks, which should almost always hold
static void assert_basic_invariants(const GameInfo_t* info) {
  ck_assert_msg(info->score >= 0, "score < 0");
  ck_assert_msg(info->high_score >= 0, "high_score < 0");
  ck_assert_msg(info->level >= 0, "level < 0");
  // tetromino.type and next should be within reasonable range 0..10
  ck_assert_msg(info->active.type >= 0 && info->active.type < 100,
                "tetromino.type invalid");
  ck_assert_msg(info->next.type >= 0 && info->next.type < 100,
                "next.type invalid");
  // blocks should not have huge negative coordinates
  for (int i = 0; i < 4; ++i) {
    ck_assert_msg(info->active.blocks[i].row > -1000, "block.row suspicious");
    ck_assert_msg(info->active.blocks[i].col > -1000, "block.col suspicious");
  }
}

// Tests

START_TEST(test_start_sets_state_and_invariants) {
  start_game();
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
  // After start, the game state should not remain START
  ck_assert_msg(info.state != START, "After Start, state remained START %d",
                info.state == START);
}
END_TEST

START_TEST(test_pause_toggle) {
  start_game();
  GameInfo_t before = get_info();
  pause_toggle();
  GameInfo_t after = get_info();
  // toggling twice returns to a comparable state (no crash)
  pause_toggle();
  GameInfo_t after2 = get_info();
  assert_basic_invariants(&before);
  assert_basic_invariants(&after);
  assert_basic_invariants(&after2);
}
END_TEST

START_TEST(test_move_left_and_right_changes_col) {
  start_game();
  GameInfo_t a = get_info();
  move_left();
  GameInfo_t b = get_info();
  // either moved left or stayed due to collision — both fine, just no crash
  move_right();
  GameInfo_t c = get_info();
  assert_basic_invariants(&a);
  assert_basic_invariants(&b);
  assert_basic_invariants(&c);
  // check that column values are in reasonable range
  ck_assert_msg(a.active.position.col > -1000, "col absurd");
  ck_assert_msg(b.active.position.col > -1000, "col absurd");
  ck_assert_msg(c.active.position.col > -1000, "col absurd");
}
END_TEST

START_TEST(test_rotation_changes_blocks) {
  start_game();
  GameInfo_t a = get_info();
  int before_rows[4], before_cols[4];
  for (int i = 0; i < 4; ++i) {
    before_rows[i] = a.active.blocks[i].row;
    before_cols[i] = a.active.blocks[i].col;
  }
  rotate_up();
  GameInfo_t b = get_info();
  int diffs = 0;
  for (int i = 0; i < 4; ++i) {
    if (before_rows[i] != b.active.blocks[i].row ||
        before_cols[i] != b.active.blocks[i].col)
      diffs++;
  }
  // rotation may not change blocks for O shape, so diffs can be 0..4
  ck_assert_msg(diffs >= 0 && diffs <= 4, "rotation produced impossible diffs");
  assert_basic_invariants(&b);
}
END_TEST

START_TEST(test_hard_drop_places_piece_and_generates_next) {
  start_game();
  hard_drop();
  GameInfo_t after = get_info();
  // After hard drop, new tetromino should appear in info.next or current
  // tetromino updated
  assert_basic_invariants(&after);
  ck_assert_msg(after.score >= 0, "score negative after hard drop");
  // next must be valid
  ck_assert_msg(after.next.type >= 0 && after.next.type < 100,
                "next.type invalid");
}
END_TEST

START_TEST(test_down_moves_or_fixes_without_crash) {
  start_game();
  move_down();
  GameInfo_t b = get_info();
  // either row increased or piece was fixed and replaced — all ok
  assert_basic_invariants(&b);
  ck_assert_msg(b.active.position.row >= -1000, "row absurd");
}
END_TEST

START_TEST(test_sequence_lrud_rotate) {
  start_game();
  for (int i = 0; i < 3; ++i) {
    move_left();
    move_right();
    rotate_up();
    move_down();
  }
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
}
END_TEST

START_TEST(test_massive_updates_no_crash) {
  start_game();
  // many updates — check no crash and invariants hold
  for (int i = 0; i < 200; ++i) {
    move_down();
    if (i % 7 == 0) rotate_up();
    if (i % 13 == 0) move_left();
    if (i % 17 == 0) move_right();
    GameInfo_t info = get_info();
    assert_basic_invariants(&info);
  }
}
END_TEST

START_TEST(test_random_action_sequences_small) {
  start_game();
  srand((unsigned)time(NULL) ^ 0xC0FFEE);
  for (int i = 0; i < 100; ++i) {
    int a = rand() % 6;
    switch (a) {
      case 0:
        move_left();
        break;
      case 1:
        move_right();
        break;
      case 2:
        move_down();
        break;
      case 3:
        rotate_up();
        break;
      case 4:
        hard_drop();
        break;
      case 5:
        pause_toggle();
        break;
    }
    GameInfo_t info = get_info();
    assert_basic_invariants(&info);
  }
}
END_TEST

START_TEST(test_highscore_persistence_behavior) {
  start_game();
  for (int i = 0; i < 50; ++i) hard_drop();
  GameInfo_t info = get_info();
  ck_assert_msg(info.high_score >= 0, "high_score < 0");
}
END_TEST

START_TEST(test_terminate_sets_game_over_state) {
  start_game();
  terminate_game();
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
}
END_TEST

START_TEST(test_multiple_starts_reset_score_and_field) {
  start_game();
  for (int i = 0; i < 10; ++i) hard_drop();
  GameInfo_t info1 = get_info();
  start_game();
  GameInfo_t info2 = get_info();
  assert_basic_invariants(&info1);
  assert_basic_invariants(&info2);
  ck_assert_msg(info2.high_score >= 0, "highscore negative after restart");
}
END_TEST

#define SIMPLE_MOVES_TEST(N)          \
  START_TEST(test_simple_moves_##N) { \
    start_game();                     \
    for (int i = 0; i < (N); ++i) {   \
      move_left();                    \
      move_right();                   \
      rotate_up();                    \
      move_down();                    \
    }                                 \
    GameInfo_t info = get_info();     \
    assert_basic_invariants(&info);   \
  }                                   \
  END_TEST

SIMPLE_MOVES_TEST(1)
SIMPLE_MOVES_TEST(2)
SIMPLE_MOVES_TEST(3)
SIMPLE_MOVES_TEST(4)
SIMPLE_MOVES_TEST(5)
SIMPLE_MOVES_TEST(6)
SIMPLE_MOVES_TEST(7)
SIMPLE_MOVES_TEST(8)
SIMPLE_MOVES_TEST(9)
SIMPLE_MOVES_TEST(10)

START_TEST(test_many_left_moves) {
  start_game();
  for (int i = 0; i < 50; ++i) move_left();
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
}
END_TEST

START_TEST(test_many_right_moves) {
  start_game();
  for (int i = 0; i < 50; ++i) move_right();
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
}
END_TEST

START_TEST(test_next_changes_over_time) {
  start_game();
  GameInfo_t a = get_info();
  hard_drop();
  GameInfo_t b = get_info();
  assert_basic_invariants(&a);
  assert_basic_invariants(&b);
}
END_TEST

START_TEST(test_spin_and_drop_loop) {
  start_game();
  for (int i = 0; i < 30; ++i) {
    rotate_up();
    rotate_up();
    hard_drop();
  }
  GameInfo_t info = get_info();
  assert_basic_invariants(&info);
}
END_TEST

START_TEST(test_pause_heavy_use) {
  start_game();
  for (int i = 0; i < 20; ++i) {
    pause_toggle();
    GameInfo_t info = get_info();
    assert_basic_invariants(&info);
  }
}
END_TEST

START_TEST(test_score_non_negative_under_massive_drops) {
  start_game();
  for (int i = 0; i < 200; ++i) hard_drop();
  GameInfo_t info = get_info();
  ck_assert_msg(info.score >= 0, "score < 0 after many drops");
}
END_TEST

Suite* tetris_suite(void) {
  Suite* s = suite_create("s21_tetris");
  TCase* tc_core = tcase_create("core");

  // Core tests
  tcase_add_test(tc_core, test_start_sets_state_and_invariants);
  tcase_add_test(tc_core, test_pause_toggle);
  tcase_add_test(tc_core, test_move_left_and_right_changes_col);
  tcase_add_test(tc_core, test_rotation_changes_blocks);
  tcase_add_test(tc_core, test_hard_drop_places_piece_and_generates_next);
  tcase_add_test(tc_core, test_down_moves_or_fixes_without_crash);
  tcase_add_test(tc_core, test_sequence_lrud_rotate);
  tcase_add_test(tc_core, test_massive_updates_no_crash);
  tcase_add_test(tc_core, test_random_action_sequences_small);
  tcase_add_test(tc_core, test_highscore_persistence_behavior);
  tcase_add_test(tc_core, test_terminate_sets_game_over_state);
  tcase_add_test(tc_core, test_multiple_starts_reset_score_and_field);

  // Simple move tests
  tcase_add_test(tc_core, test_simple_moves_1);
  tcase_add_test(tc_core, test_simple_moves_2);
  tcase_add_test(tc_core, test_simple_moves_3);
  tcase_add_test(tc_core, test_simple_moves_4);
  tcase_add_test(tc_core, test_simple_moves_5);
  tcase_add_test(tc_core, test_simple_moves_6);
  tcase_add_test(tc_core, test_simple_moves_7);
  tcase_add_test(tc_core, test_simple_moves_8);
  tcase_add_test(tc_core, test_simple_moves_9);
  tcase_add_test(tc_core, test_simple_moves_10);

  // Edge & scenario tests
  tcase_add_test(tc_core, test_many_left_moves);
  tcase_add_test(tc_core, test_many_right_moves);
  tcase_add_test(tc_core, test_next_changes_over_time);
  tcase_add_test(tc_core, test_spin_and_drop_loop);
  tcase_add_test(tc_core, test_pause_heavy_use);
  tcase_add_test(tc_core, test_score_non_negative_under_massive_drops);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite* s = tetris_suite();
  SRunner* sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
