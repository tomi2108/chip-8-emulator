#include "../include/memory.h"
#include <commons/log.h>
#include <string.h>

t_list *stack;

log_t memory_logger = {.file = "log.log",
                       .process = "MEMORY",
                       .level = LOG_LEVEL_DEBUG,
                       .is_active_console = 1};

u8 ram[0xFFFF] = {0};

void ram_write(const u8 *stream, u16 length, u16 offset) {
  memcpy(&ram[offset], stream, length);
}

u8 ram_read(u16 offset) { return ram[offset]; }

void stack_push(u16 bytes) {
  u16 *entry = safe_malloc(NULL, sizeof(u16));
  *entry = bytes;
  list_add_in_index(stack, list_size(stack) - 1, entry);
}

u16 stack_pop() {
  u16 *bytes = list_get(stack, list_size(stack) - 1);
  u16 ret = *bytes;
  free(bytes);
  return ret;
}

u8 font[0xFF] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void memory_init() {
  memory_logger.logger =
      log_create(memory_logger.file, memory_logger.process,
                 memory_logger.is_active_console, memory_logger.level);
  stack = list_create();
  ram_write(font, 5 * 0xF, 0x50);
}

void memory_free() {
  log_destroy(memory_logger.logger);
  memset(ram, 0, 0xFFFF);
  list_destroy(stack);
}
