#include "../include/emu.h"
#include <commons/log.h>
#include <unistd.h>

log_t emu_logger = {.file = "log.log",
                    .process = "EMU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

t_config *emu_config;

int running;
cartdrige *rom;

int main(const int argc, const char *argv[]) {
  emu_logger.logger =
      log_create(emu_logger.file, emu_logger.process,
                 emu_logger.is_active_console, emu_logger.level);

  // if (argc < 2)
  //   exit_not_enough_arguments_error(emu_logger);
  //
  // char *config_path =
  //     "/home/tsanchen/personal/gbc-emulator/emulator/emulator.config";
  // emu_config = config_create(config_path);
  //
  // if (emu_config == NULL)
  //   exit_enoent_error(emu_logger, config_path);
  //
  // const char *rom_path = argv[1];
  //
  // rom = cartdrige_load(rom_path);

  // state *emu_state = safe_malloc(emu_logger, sizeof(state));
  //
  // emu_state->ticks = 0;
  // emu_state->running = 1;
  //
  // cpu_init();
  // while (emu_state->running) {
  //   sleep(1);
  //   cpu_exec(rom->rom_data);
  //
  //   emu_state->ticks++;
  // }

  screen_init();
  screen_draw();
  sleep(2);
  screen_free();

  return EXIT_SUCCESS;
}
