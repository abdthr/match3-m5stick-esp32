#pragma once
#include "game_state.h"

void clampCursor(GameState &g);
void getSecondTile(const GameState &g, int *r2, int *c2);
void swapSelected(GameState &g);

void initBoardRandom(GameState &g);
void applyGravity(GameState &g);
void refillBoard(GameState &g);
