#include "../include/emu.h"
#include <commons/log.h>
#include <unistd.h>

#define FRAMERATE 60

log_t emu_logger = {.file = "log.log",
                    .process = "EMU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

int running;

int main(const int argc, const char *argv[]) {
  emu_logger.logger =
      log_create(emu_logger.file, emu_logger.process,
                 emu_logger.is_active_console, emu_logger.level);

  if (argc < 2)
    exit_not_enough_arguments_error(emu_logger.logger);

  const char *rom_path = argv[1];

  state *emu_state = safe_malloc(emu_logger.logger, sizeof(state));

  emu_state->ticks = 0;
  emu_state->running = 1;

  memory_init();
  cartdrige_load(rom_path);

  cpu_init();
  screen_init();

  while (emu_state->running) {
    cpu_exec();
    screen_draw();

    emu_state->ticks++;
    usleep(1000 * 1000 * 1 / FRAMERATE);
  }

  screen_free();
  cpu_free();
  memory_free();

  return EXIT_SUCCESS;
}
