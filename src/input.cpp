#include <M5Unified.h>
#include "input.h"
#include "board.h"
#include "match.h"
#include "render.h"
#include "game.h"

void handleTiltMove(GameState &g) {
  uint32_t now = millis();
  if (now - g.lastMoveMs < MOVE_COOLDOWN_MS) return;

  float ax, ay, az;
  M5.Imu.getAccel(&ax, &ay, &az);

  int dR = 0, dC = 0;

  if (ax >  TILT_THRESHOLD) dC = -1;
  if (ax < -TILT_THRESHOLD) dC =  1;
  if (ay >  TILT_THRESHOLD) dR =  1;
  if (ay < -TILT_THRESHOLD) dR = -1;

  if (dR || dC) {
    g.curR += dR;
    g.curC += dC;
    clampCursor(g);
    g.lastMoveMs = now;
    drawBoardAndCursor(g);
  }
}

void doSwapAction(GameState &g) {
  // On end screen: pressing B just continues (next level or restart)
  if (g.gameOver) {
    if (g.gameWon) startLevel(g, g.level + 1);
    else          startLevel(g, 1);
    return;
  }

  // Swap, resolve, and if it was a useless move, swap back.
  swapSelected(g);
  int removed = resolveBoard(g);

  if (removed == 0) {
    swapSelected(g); // undo
  } else {
    g.movesLeft--;
    g.score += (uint32_t)removed * 10;
  }

  drawBoardAndCursor(g);
  checkWinLose(g);
}
