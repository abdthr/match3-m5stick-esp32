#pragma once
#include <stdint.h>
#include "game_state.h"

uint16_t colorFor(const GameState &g, int8_t v);
void drawTriangle(int x, int y, int size, bool up, uint16_t color);

void drawBoardAndCursor(const GameState &g);
void drawEndScreen(const GameState &g);

void drawMenu(const GameState &g);
void showMessage(const char *msg);
