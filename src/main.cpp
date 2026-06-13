#include <M5Unified.h>
#include <EEPROM.h>

#include "game_state.h"
#include "board.h"
#include "match.h"
#include "render.h"
#include "input.h"
#include "menu.h"
#include "save.h"
#include "game.h"

static const int EEPROM_SIZE = 512;
GameState g;

void setup() {
  M5.begin();
  M5.Imu.init();
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);

  randomSeed((uint32_t)esp_random());

  startLevel(g, 1);
}

void loop() {
  M5.update();

  // Open the menu if both buttons are held down
  if (!g.menuOpen && M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
    openMenu(g);
    delay(250);
    return;
  }

  if (g.menuOpen) {
    handleMenuInput(g);
    delay(10);
    return;
  }

  if (!g.gameOver) {
    handleTiltMove(g);

    // Button A: rotate selection (horizontal <-> vertical)
    if (M5.BtnA.wasPressed()) {
      g.horizontal = !g.horizontal;
      clampCursor(g);
      drawBoardAndCursor(g);
    }
  }

  // Button B: commit swap / continue from endscreen
  if (M5.BtnB.wasPressed()) {
    doSwapAction(g);
  }

  delay(10);
}
