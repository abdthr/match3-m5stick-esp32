#pragma once
#include "game_state.h"

void saveGame(const GameState &g);
bool loadGame(GameState &g);
