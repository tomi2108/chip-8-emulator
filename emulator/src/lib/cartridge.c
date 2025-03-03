#include "../include/cartridge.h"
#include <stdio.h>

log_t cartdrige_logger = {.file = "log.log",
                          .process = "CARTRIDGE",
                          .level = LOG_LEVEL_DEBUG,
                          .is_active_console = 1};

t_log_level log_level = LOG_LEVEL_DEBUG;

cartdrige *cartdrige_load(const char *filename) {
  cartdrige *c = safe_malloc(NULL, sizeof(cartdrige));

  FILE *file = safe_fopen(NULL, filename, "r");
  rewind(file);

  fseek(file, 0, SEEK_END);

  c->rom_size = ftell(file);
  rewind(file);

  c->rom_data = safe_malloc(NULL, c->rom_size);

  fread(c->rom_data, c->rom_size, 1, file);

  cartdrige_logger.logger =
      log_create(cartdrige_logger.file, cartdrige_logger.process,
                 cartdrige_logger.is_active_console, cartdrige_logger.level);

  log_debug(cartdrige_logger.logger, "File size: %d Bytes", c->rom_size);

  fclose(file);
  log_destroy(cartdrige_logger.logger);
  return c;
}
