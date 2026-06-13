#pragma once
#include "game_state.h"

bool findMatches(GameState &g, bool marks[MAX_ROWS][MAX_COLS], int8_t specials[MAX_ROWS][MAX_COLS]);
void expandSpecialActivations(GameState &g, bool marks[MAX_ROWS][MAX_COLS]);
int removeMarked(GameState &g, bool marks[MAX_ROWS][MAX_COLS], int8_t specials[MAX_ROWS][MAX_COLS]);

int resolveBoard(GameState &g);
