#include <M5Unified.h>
#include <EEPROM.h>

#include "menu.h"
#include "render.h"
#include "save.h"
#include "board.h"
#include "game.h"

static const int MENU_COUNT = 4;

void openMenu(GameState &g) {
  g.menuOpen = true;
  g.menuIndex = 0;
  drawMenu(g);
}

void closeMenu(GameState &g) {
  g.menuOpen = false;
  if (g.gameOver) drawEndScreen(g);
  else            drawBoardAndCursor(g);
}

void handleMenuInput(GameState &g) {
  if (M5.BtnA.wasPressed()) {
    g.menuIndex++;
    if (g.menuIndex >= MENU_COUNT) g.menuIndex = 0;
    drawMenu(g);
  }

  if (M5.BtnB.wasPressed()) {
    if (g.menuIndex == 0) {            // SAVE
      saveGame(g);
      showMessage("Saved");
      drawMenu(g);
    } else if (g.menuIndex == 1) {     // LOAD
      if (loadGame(g)) showMessage("Loaded");
      else             showMessage("No save found");

      // After loading, show either endscreen or board
      clampCursor(g);
      if (g.gameOver) drawEndScreen(g);
      else            drawBoardAndCursor(g);

      // back to menu display
      drawMenu(g);
    } else if (g.menuIndex == 2) {     // RESET
      resetBoardSameLevel(g);
      closeMenu(g);
    } else {                           // BACK
      closeMenu(g);
    }
  }
}
