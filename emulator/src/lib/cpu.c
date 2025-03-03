#include "../include/cpu.h"
#include <commons/collections/list.h>
#include <commons/log.h>

#define FILE_LINE_MAX_LENGTH 500

regs registers = {0};

log_t cpu_logger = {.file = "log.log",
                    .process = "CPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

instruction *instruction_decode(u8 byte) {
  instruction *i = get_instruction(byte);
  return i;
}

void instruction_exec(instruction *i) {
  void (*exec_function)(instruction *) = get_instruction_function(i->op);
  exec_function(i);
}

void log_param(void *arg) {
  param *p = (param *)arg;
  log_debug(cpu_logger.logger, "Type: %d", p->type);
  log_debug(cpu_logger.logger, "Value: %s", (char *)p->value);
}

char *to_binary_string(int n, int bits) {
  int num_bits = sizeof(char) * bits;
  char *string = safe_malloc(cpu_logger.logger, num_bits + 1);

  for (int i = num_bits - 1; i >= 0; i--) {
    string[i] = (n & 1) + '0';
    n >>= 1;
  }
  string[num_bits] = '\0';
  return string;
}

void log_registers() {}

void cpu_init() {
  cpu_logger.logger =
      log_create(cpu_logger.file, cpu_logger.process,
                 cpu_logger.is_active_console, cpu_logger.level);
}

void free_instruction(instruction *i) { free(i); }

void cpu_exec(const char *rom_data) {

  // u8 byte = rom_data[0x150 + registers.PC - 1];
  // if (registers.PC == 0)
  //   byte = rom_data[0x100];
  //
  // registers.PC++;
  // instruction *i = instruction_decode(byte);
  // instruction_exec(i);
  //
  // free_instruction(i);
}

void cpu_free() {}
