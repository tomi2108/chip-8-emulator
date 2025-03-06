#include "../include/emu.h"
#include <commons/log.h>
#include <pthread.h>
#include <unistd.h>

#define FRAMERATE 120

log_t emu_logger = {.file = "log.log",
                    .process = "EMU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void *input_handler(void *arg) {
  bool *stop = (bool *)arg;
  keypad_init();
  while (1) {
    keypad_handle_input(stop);
    if (*stop == 1)
      break;
    usleep(1000 * 1000 * 1 / FRAMERATE);
  }
  keypad_free();
}

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

  pthread_t input_handle_thread;
  bool *stop = safe_malloc(emu_logger.logger, sizeof(bool));
  *stop = 0;
  pthread_create(&input_handle_thread, NULL, &input_handler, stop);

  while (!*stop) {
    cpu_exec();
    screen_draw();

    s_timer_tick();
    d_timer_tick();
    usleep(1000 * 1000 * 1 / FRAMERATE);
  }

  pthread_join(input_handle_thread, NULL);
  free(stop);
  screen_free();
  cpu_free();
  memory_free();

  return EXIT_SUCCESS;
}
