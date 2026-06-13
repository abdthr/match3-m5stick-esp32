#include <EEPROM.h>
#include "save.h"

static const int EEPROM_SIZE = 512;
static const uint8_t SAVE_MAGIC = 0xB7;

// EEPROM helpers: store 32-bit numbers byte-by-byte
static void writeU32(int &addr, uint32_t v) {
  EEPROM.write(addr++, (uint8_t)(v & 0xFF));
  EEPROM.write(addr++, (uint8_t)((v >> 8) & 0xFF));
  EEPROM.write(addr++, (uint8_t)((v >> 16) & 0xFF));
  EEPROM.write(addr++, (uint8_t)((v >> 24) & 0xFF));
}

static uint32_t readU32(int &addr) {
  uint32_t v = 0;
  v |= (uint32_t)EEPROM.read(addr++);
  v |= (uint32_t)EEPROM.read(addr++) << 8;
  v |= (uint32_t)EEPROM.read(addr++) << 16;
  v |= (uint32_t)EEPROM.read(addr++) << 24;
  return v;
}

void saveGame(const GameState &g) {
  int addr = 0;

  EEPROM.write(addr++, SAVE_MAGIC);

  EEPROM.write(addr++, (uint8_t)g.ROWS);
  EEPROM.write(addr++, (uint8_t)g.COLS);
  EEPROM.write(addr++, (uint8_t)g.NUM_COLORS);
  EEPROM.write(addr++, (uint8_t)g.level);

  writeU32(addr, g.score);
  writeU32(addr, g.targetScore);

  EEPROM.write(addr++, (uint8_t)g.movesLeft);

  EEPROM.write(addr++, (uint8_t)g.curR);
  EEPROM.write(addr++, (uint8_t)g.curC);
  EEPROM.write(addr++, g.horizontal ? 1 : 0);

  EEPROM.write(addr++, g.gameOver ? 1 : 0);
  EEPROM.write(addr++, g.gameWon  ? 1 : 0);

  // Save board (only the active size)
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      EEPROM.write(addr++, (uint8_t)g.board[r][c]);
    }
  }

  EEPROM.commit();
}

bool loadGame(GameState &g) {
  int addr = 0;

  uint8_t magic = EEPROM.read(addr++);
  if (magic != SAVE_MAGIC) return false;

  g.ROWS = (int)EEPROM.read(addr++);
  g.COLS = (int)EEPROM.read(addr++);
  g.NUM_COLORS = (int)EEPROM.read(addr++);
  g.level = (int)EEPROM.read(addr++);

  g.score = readU32(addr);
  g.targetScore = readU32(addr);

  g.movesLeft = (int)EEPROM.read(addr++);

  g.curR = (int)EEPROM.read(addr++);
  g.curC = (int)EEPROM.read(addr++);
  g.horizontal = EEPROM.read(addr++) ? true : false;

  g.gameOver = EEPROM.read(addr++) ? true : false;
  g.gameWon  = EEPROM.read(addr++) ? true : false;

  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      g.board[r][c] = (int8_t)EEPROM.read(addr++);
    }
  }

  return true;
}
