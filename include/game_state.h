#pragma once
#include <stdint.h>
#include "config.h"


struct GameState {
  // Current level settings (change each level)
  int ROWS = 7;
  int COLS = 7;
  int NUM_COLORS = 6;

  // Board storage uses max size, but we only use [0...ROWS-1][0...COLS-1]
  int8_t board[MAX_ROWS][MAX_COLS];

  // Cursor
  int curR = 0;
  int curC = 0;
  bool horizontal = true;
  uint32_t lastMoveMs = 0;

  // Game state
  uint32_t score = 0;
  int movesLeft = 0;
  uint32_t targetScore = 0;
  int level = 1;
  bool gameOver = false;
  bool gameWon  = false;

  // Menu state
  bool menuOpen = false;
  int menuIndex = 0;
};
