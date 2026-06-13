#include "match.h"
#include "board.h"

bool findMatches(GameState &g, bool marks[MAX_ROWS][MAX_COLS], int8_t specials[MAX_ROWS][MAX_COLS]) {
  bool any = false;

  // Clear marks + clear planned specials
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      marks[r][c] = false;
      specials[r][c] = (int8_t)EMPTY; // EMPTY = no special planned here
    }
  }

  // ---- horizontal runs ----
  for (int r = 0; r < g.ROWS; r++) {
    int c = 0;
    while (c < g.COLS) {
      int8_t v = g.board[r][c];
      if (isEmpty(v)) { c++; continue; }

      uint8_t col = gemColor(v);
      int start = c;

      while (c < g.COLS && !isEmpty(g.board[r][c]) && gemColor(g.board[r][c]) == col) c++;
      int len = c - start;

      if (len >= 3) {
        any = true;
        for (int k = start; k < start + len; k++) marks[r][k] = true;

        if (len >= 4) {
          // Plan a horizontal special in the middle-ish of the run
          int pos = start + (len / 2);
          specials[r][pos] = makeSpecialGem(col, GEM_HORZ);
        }
      }
    }
  }

  // ---- vertical runs ----
  for (int c = 0; c < g.COLS; c++) {
    int r = 0;
    while (r < g.ROWS) {
      int8_t v = g.board[r][c];
      if (isEmpty(v)) { r++; continue; }

      uint8_t col = gemColor(v);
      int start = r;

      while (r < g.ROWS && !isEmpty(g.board[r][c]) && gemColor(g.board[r][c]) == col) r++;
      int len = r - start;

      if (len >= 3) {
        any = true;
        for (int k = start; k < start + len; k++) marks[k][c] = true;

        if (len >= 4) {
          // Plan a vertical special in the middle-ish of the run
          int pos = start + (len / 2);

          // If a horizontal special was already planned here -> keep it
          if (specials[pos][c] == (int8_t)EMPTY) {
            specials[pos][c] = makeSpecialGem(col, GEM_VERT);
          }
        }
      }
    }
  }

  return any;
}

void expandSpecialActivations(GameState &g, bool marks[MAX_ROWS][MAX_COLS]) {
  // If a special gem is part of a match, it triggers immediately
  // That trigger can mark new specials, so we loop until nothing new gets added
  bool changed = true;

  while (changed) {
    changed = false;

    for (int r = 0; r < g.ROWS; r++) {
      for (int c = 0; c < g.COLS; c++) {
        if (!marks[r][c]) continue;

        int8_t v = g.board[r][c];
        if (!isSpecial(v)) continue;

        uint8_t t = gemType(v);

        if (t == GEM_HORZ) {
          for (int cc = 0; cc < g.COLS; cc++) {
            if (!marks[r][cc]) { marks[r][cc] = true; changed = true; }
          }
        } else if (t == GEM_VERT) {
          for (int rr = 0; rr < g.ROWS; rr++) {
            if (!marks[rr][c]) { marks[rr][c] = true; changed = true; }
          }
        }
      }
    }
  }
}

int removeMarked(GameState &g, bool marks[MAX_ROWS][MAX_COLS], int8_t specials[MAX_ROWS][MAX_COLS]) {
  int removed = 0;

  // First: remove matched gems
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      if (marks[r][c]) {
        g.board[r][c] = EMPTY;
        removed++;
      }
    }
  }

  // Second: place planned specials so they survive the clear
  for (int r = 0; r < g.ROWS; r++) {
    for (int c = 0; c < g.COLS; c++) {
      if (specials[r][c] != (int8_t)EMPTY) {
        g.board[r][c] = specials[r][c];
      }
    }
  }

  return removed;
}

int resolveBoard(GameState &g) {
  int totalRemoved = 0;
  bool marks[MAX_ROWS][MAX_COLS];
  int8_t specials[MAX_ROWS][MAX_COLS];

  // Keep resolving until the board stabilizes
  // Matches -> clear -> gravity -> refill can create new matches.
  while (true) {
    if (!findMatches(g, marks, specials)) break;

    expandSpecialActivations(g, marks);

    int removed = removeMarked(g, marks, specials);
    totalRemoved += removed;

    applyGravity(g);
    refillBoard(g);
  }

  return totalRemoved;
}
