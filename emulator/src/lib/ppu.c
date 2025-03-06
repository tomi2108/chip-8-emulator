#include "../include/ppu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <commons/log.h>
#include <string.h>

log_t ppu_logger = {.file = "log.log",
                    .process = "PPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

SDL_Window *window;
SDL_Renderer *renderer;

bool screen[SCREEN_WIDTH][SCREEN_HEIGHT] = {0};

void screen_clear() { memset(screen, 0, SCREEN_WIDTH * SCREEN_HEIGHT); }

void screen_init() {
  ppu_logger.logger =
      log_create(ppu_logger.file, ppu_logger.process,
                 ppu_logger.is_active_console, ppu_logger.level);

  SDL_InitSubSystem(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, 0,
                              &window, &renderer);
  SDL_RenderSetScale(renderer, SCALE, SCALE);
  SDL_SetRenderDrawColor(renderer, 15, 56, 15, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void screen_draw() {
  SDL_SetRenderDrawColor(renderer, 15, 56, 15, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 155, 188, 15, SDL_ALPHA_OPAQUE);
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      if (screen[x][y])
        SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  SDL_RenderPresent(renderer);
}

void screen_free() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void screen_set(u8 x, u8 y, bool to) { screen[x][y] = to; }
bool screen_get(u8 x, u8 y) { return screen[x][y]; }
