#include <M5Unified.h>
#include "render.h"
#include "board.h"

// Small helpers for drawing the cursor box
static int minInt(int a, int b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

static int maxInt(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

uint16_t colorFor(const GameState &g, int8_t v) {
  if (v < 0) return BLACK;

  uint8_t c = gemColor(v);

  // Defensive: if NUM_COLORS changes, modulo keeps the switch safe.
  switch (c % (uint8_t)g.NUM_COLORS) {
    case 0: return RED;
    case 1: return GREEN;
    case 2: return BLUE;
    case 3: return YELLOW;
    case 4: return CYAN;
    case 5: return MAGENTA;
    case 6: return WHITE;    // only used if you ever go to 7 colors
    default: return WHITE;
  }
}

void drawTriangle(int x, int y, int size, bool up, uint16_t color) {
  // Special gems: we draw a triangle instead of a square
  if (up) {
    // ▲ horizontal-clear
    M5.Lcd.fillTriangle(
      x + size / 2, y,
      x, y + size,
      x + size, y + size,
      color
    );
  } else {
    // ▼ vertical-clear
    M5.Lcd.fillTriangle(
      x, y,
      x + size, y,
      x + size / 2, y + size,
      color
    );
  }
}

void drawBoardAndCursor(const GameState &g) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(1);

  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      int x = ORGX + c * (CELL + GAP);
      int y = ORGY + r * (CELL + GAP);

      int8_t v = g.board[r][c];

      if (isSpecial(v)) {
        bool up = (gemType(v) == GEM_HORZ);
        drawTriangle(x, y, CELL, up, colorFor(g, v));
      } else {
        M5.Lcd.fillRect(x, y, CELL, CELL, colorFor(g, v));
      }

      // Thin outline so tiles are readable
      M5.Lcd.drawRect(x, y, CELL, CELL, WHITE);
    }
  }

  // Draw selection cursor (2-tile box)
  int r2, c2;
  getSecondTile(g, &r2, &c2);

  int x1 = ORGX + g.curC * (CELL + GAP);
  int y1 = ORGY + g.curR * (CELL + GAP);
  int x2 = ORGX + c2 * (CELL + GAP);
  int y2 = ORGY + r2 * (CELL + GAP);

  int left   = minInt(x1, x2);
  int right  = maxInt(x1, x2);
  int top    = minInt(y1, y2);
  int bottom = maxInt(y1, y2);

  int w = (right - left) + CELL;
  int h = (bottom - top) + CELL;

  // 3px thick border
  for (int t = 0; t < 3; t++) {
    M5.Lcd.drawRect(left - t, top - t, w + 2*t, h + 2*t, WHITE);
  }

  int infoY = ORGY + g.ROWS * (CELL + GAP) + 6;

  M5.Lcd.setCursor(ORGX, infoY);
  M5.Lcd.printf(
    "\n"
    "  Level: %d\n\n"
    "  Score: %lu/%lu\n\n"
    "  Moves left: %d",
    g.level,
    (unsigned long)g.score,
    (unsigned long)g.targetScore,
    g.movesLeft
  );
}

void drawEndScreen(const GameState &g) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 30);

  if (g.gameWon) M5.Lcd.printf("YOU WIN");
  else          M5.Lcd.printf("GAME OVER");

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.printf("Score: %lu", (unsigned long)g.score);
  M5.Lcd.setCursor(10, 90);
  M5.Lcd.printf("Press B to continue");
}

void drawMenu(const GameState &g) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("MENU");

  const char *items[] = {"SAVE", "LOAD", "RESET", "BACK"};

  for (int i = 0; i < 4; i++) {
    M5.Lcd.setCursor(10, 40 + i * 25);
    if (i == g.menuIndex) M5.Lcd.printf("> %s", items[i]);
    else                 M5.Lcd.printf("  %s", items[i]);
  }

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 140);
  M5.Lcd.printf("A: next   B: select");
}

void showMessage(const char *msg) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.printf("%s", msg);
  delay(700);
}
