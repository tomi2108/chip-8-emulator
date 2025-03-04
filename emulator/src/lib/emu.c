#include "../include/emu.h"
#include <commons/log.h>
#include <unistd.h>

#define FRAMERATE 90

log_t emu_logger = {.file = "log.log",
                    .process = "EMU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

int main(const int argc, const char *argv[]) {
  emu_logger.logger =
      log_create(emu_logger.file, emu_logger.process,
                 emu_logger.is_active_console, emu_logger.level);

  if (argc < 2)
    exit_not_enough_arguments_error(emu_logger.logger);

  const char *rom_path = argv[1];

  memory_init();
  cartdrige_load(rom_path);

  timer_init();
  cpu_init();
  screen_init();
  keypad_init();

  while (1) {
    cpu_exec();
    screen_draw();

    keypad_handle_input();
    s_timer_tick();
    d_timer_tick();
    usleep(1000 * 1000 * 1 / FRAMERATE);
  }

  keypad_free();
  screen_free();
  cpu_free();
  memory_free();

  return EXIT_SUCCESS;
}
