#include "../include/keypad.h"
#include <commons/log.h>

u8 keys[0x10] = {0};

log_t keypad_logger = {.file = "log.log",
                       .process = "KEYPAD",
                       .level = LOG_LEVEL_DEBUG,
                       .is_active_console = 1};

u8 keycodes[0x10] = {
    SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_q, SDLK_w, SDLK_e, SDLK_r,
    SDLK_a, SDLK_s, SDLK_d, SDLK_f, SDLK_z, SDLK_x, SDLK_c, SDLK_v,
};

void keypad_press_key(u8 key) { keys[key] = 1; }
void keypad_release_key(u8 key) { keys[key] = 0; }

bool keypad_key_is_pressed(u8 key) { return keys[key]; }

u8 keypad_get_key(bool *res) {
  for (u8 i = 0; i < 0x10; i++) {
    if (keys[i])
      return i;
  }
  *res = 0;
  return 0;
}

u8 get_keycode(u8 target, bool *res) {
  for (int i = 0; i < 0x10; i++) {
    if (keycodes[i] == target)
      return i;
  }
  *res = 0;
  return 0;
}

void keypad_handle_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    log_debug(keypad_logger.logger, "Getting keycode for: %c",
              event.key.keysym.sym);
    bool res = 1;
    u8 key = get_keycode(event.key.keysym.sym, &res);
    log_debug(keypad_logger.logger, "Got key: %x", key);
    if (res == 0)
      continue;

    switch (event.type) {
    case SDL_KEYDOWN:
      keypad_press_key(key);
      break;

    case SDL_KEYUP:
      keypad_release_key(key);
      break;

    default:
      break;
    }
  }
}

void keypad_init() {
  keypad_logger.logger =
      log_create(keypad_logger.file, keypad_logger.process,
                 keypad_logger.is_active_console, keypad_logger.level);
}

void keypad_free() { log_destroy(keypad_logger.logger); }
