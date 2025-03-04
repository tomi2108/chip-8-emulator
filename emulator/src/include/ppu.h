#ifndef PPU_H_
#define PPU_H_

#include "common.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE 20

void screen_init();
void screen_free();

void screen_set(u8 x, u8 y, bool to);
bool screen_get(u8 x, u8 y);

void screen_draw();
void screen_clear();

#endif
