#include <Arduino.h>
#include "board.h"

// Tiny helpers
static int minInt(int a, int b) { return (a < b) ? a : b; }
static int maxInt(int a, int b) { return (a > b) ? a : b; }

void clampCursor(GameState &g) {
  // Cursor always selects 2 tiles
  // So we must stay inside bounds
  if (g.horizontal) {
    if (g.curC < 0) g.curC = 0;
    if (g.curC > g.COLS - 2) g.curC = g.COLS - 2;   // need room for (c+1)
    if (g.curR < 0) g.curR = 0;
    if (g.curR > g.ROWS - 1) g.curR = g.ROWS - 1;
  } else {
    if (g.curC < 0) g.curC = 0;
    if (g.curC > g.COLS - 1) g.curC = g.COLS - 1;
    if (g.curR < 0) g.curR = 0;
    if (g.curR > g.ROWS - 2) g.curR = g.ROWS - 2;   // need room for (r+1)
  }
}

void getSecondTile(const GameState &g, int *r2, int *c2) {
  if (g.horizontal) { *r2 = g.curR;     *c2 = g.curC + 1; }
  else              { *r2 = g.curR + 1; *c2 = g.curC; }
}

void swapSelected(GameState &g) {
  int r2, c2;
  getSecondTile(g, &r2, &c2);
  int8_t tmp = g.board[g.curR][g.curC];
  g.board[g.curR][g.curC] = g.board[r2][c2];
  g.board[r2][c2] = tmp;
}

void initBoardRandom(GameState &g) {
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      g.board[r][c] = makeGem((uint8_t)random(g.NUM_COLORS));
    }
  }
}

void applyGravity(GameState &g) {
  // Drop everything down in each column
  for (int c = 0; c < g.COLS; c++) {
    int write = g.ROWS - 1;
    for (int r = g.ROWS - 1; r >= 0; r--) {
      if (g.board[r][c] != EMPTY) {
        g.board[write][c] = g.board[r][c];
        if (write != r) g.board[r][c] = EMPTY;
        write--;
      }
    }
    // Fill the rest above with EMPTY
    for (int r = write; r >= 0; r--) g.board[r][c] = EMPTY;
  }
}

void refillBoard(GameState &g) {
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      if (g.board[r][c] == EMPTY) {
        g.board[r][c] = makeGem((uint8_t)random(g.NUM_COLORS));
      }
    }
  }
}
