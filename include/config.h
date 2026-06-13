#pragma once
#include <stdint.h>

// Keep arrays fixed-size
// Actual ROWS/COLS can change per level
static const int MAX_ROWS = 9;
static const int MAX_COLS = 9;

// Drawing
static const int CELL = 14;   // tile size in pixels
static const int GAP  = 2;    // spacing between tiles
static const int ORGX = 10;   // board top-left x
static const int ORGY = 20;   // board top-left y

// Tilt
static const float TILT_THRESHOLD      = 0.35f;
static const uint32_t MOVE_COOLDOWN_MS = 200;

// Board
static const int EMPTY = -1;

// Special gem encoding (stored inside int8_t)
// bits 0..2 : color (0..7)
// bits 3..4 : type  (normal / row-clear / col-clear)
static const uint8_t GEM_COLOR_MASK = 0x07;   // 00000111
static const uint8_t GEM_TYPE_MASK  = 0x18;   // 00011000

static const uint8_t GEM_NORMAL = 0x00;
static const uint8_t GEM_HORZ   = 0x08;       // clears entire row when activated
static const uint8_t GEM_VERT   = 0x10;       // clears entire column when activated

static inline bool isEmpty(int8_t v) { return v == EMPTY; }
static inline uint8_t gemColor(int8_t v) { return (uint8_t)v & GEM_COLOR_MASK; }
static inline uint8_t gemType(int8_t v)  { return (uint8_t)v & GEM_TYPE_MASK; }

static inline bool isSpecial(int8_t v) {
  return (!isEmpty(v)) && (gemType(v) != GEM_NORMAL);
}

// Build a normal gem or a special gem.
static inline int8_t makeGem(uint8_t color) {
  return (int8_t)(color & GEM_COLOR_MASK);
}
static inline int8_t makeSpecialGem(uint8_t color, uint8_t type) {
  return (int8_t)((color & GEM_COLOR_MASK) | type);
}
