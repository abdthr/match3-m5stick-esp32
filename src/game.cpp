#include <Arduino.h>
#include "game.h"
#include "board.h"
#include "match.h"
#include "render.h"

void startLevel(GameState &g, int newLevel) {
  g.level = newLevel;

  // Board size grows with level
  if (g.level <= 1)      g.ROWS = 5;
  else if (g.level == 2) g.ROWS = 6;
  else if (g.level == 3) g.ROWS = 7;
  else if (g.level == 4) g.ROWS = 8;
  else                   g.ROWS = 9;

  g.COLS = 7;

  // Colors increase later to make it harder
  if (g.level <= 2)      g.NUM_COLORS = 5;
  else if (g.level <= 4) g.NUM_COLORS = 6;
  else                   g.NUM_COLORS = 7;

  g.targetScore = (uint32_t)(g.ROWS * g.COLS * 10 + g.level * 100);
  g.movesLeft   = 12 + g.level * 2;

  g.curR = 0;
  g.curC = 0;
  g.horizontal = true;
  clampCursor(g);

  initBoardRandom(g);

  // Clear any free matches at the start
  resolveBoard(g);

  g.score = 0;
  g.gameOver = false;
  g.gameWon  = false;

  drawBoardAndCursor(g);
}

void checkWinLose(GameState &g) {
  if (g.score >= g.targetScore) {
    g.gameOver = true;
    g.gameWon  = true;
    drawEndScreen(g);
    return;
  }

  if (g.movesLeft <= 0) {
    g.gameOver = true;
    g.gameWon  = false;
    drawEndScreen(g);
    return;
  }
}

void resetBoardSameLevel(GameState &g) {
  initBoardRandom(g);
  resolveBoard(g);
  clampCursor(g);
  drawBoardAndCursor(g);
}
