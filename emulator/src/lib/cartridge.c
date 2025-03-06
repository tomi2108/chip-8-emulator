#include "../include/cartridge.h"
#include <stdio.h>

log_t cartdrige_logger = {.file = "log.log",
                          .process = "CARTRIDGE",
                          .level = LOG_LEVEL_DEBUG,
                          .is_active_console = 1};

t_log_level log_level = LOG_LEVEL_DEBUG;

void cartdrige_load(const char *filename) {
  FILE *file = safe_fopen(NULL, filename, "r");
  rewind(file);

  fseek(file, 0, SEEK_END);

  u16 size = ftell(file);
  rewind(file);

  u8 *stream = safe_malloc(NULL, size);

  fread(stream, size, sizeof(u8), file);
  ram_write(stream, size, 0x200);
  free(stream);

  fclose(file);
}
