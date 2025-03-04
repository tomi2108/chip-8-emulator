#include "../include/cpu.h"
#include <commons/collections/list.h>
#include <commons/log.h>

regs registers = {.R_PC = 0x200, 0};

log_t cpu_logger = {.file = "log.log",
                    .process = "CPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void instruction_noop() {}

void instruction_jump(u16 n) { registers.R_PC = n; }

void instruction_set(u8 reg_dest, u8 n) { registers.R_X[reg_dest] = n; }

void instruction_add(u8 reg_dest, u8 n) { registers.R_X[reg_dest] += n; }

void instruction_set_index(u16 n) { registers.R_I = n; }

void instruction_draw(u8 reg_x, u8 reg_y, u8 n) {
  registers.R_X[0xF] = 0;
  u8 y = registers.R_X[reg_y] & (SCREEN_HEIGHT - 1);

  for (u8 i = 0; i < n; i++) {
    u8 x = registers.R_X[reg_x] & (SCREEN_WIDTH - 1);
    u8 sprite_byte = ram_read(registers.R_I + i);

    for (int j = 0; j < 8; j++) {

      bool bit = (sprite_byte >> (7 - j)) & 0x1;
      bool pixel = screen_get(x, y);

      if (bit && pixel) {
        screen_set(x, y, 0);
        registers.R_X[0xF] = 1;
      }
      if (bit && !pixel)
        screen_set(x, y, 1);

      if (x > SCREEN_WIDTH)
        break;
      x++;
    }
    y++;
    if (y > SCREEN_HEIGHT)
      break;
  }
}

void instruction_exec(u16 bytes) {
  u8 nibble_1 = (0xF000 & bytes) >> 12;
  u8 nibble_2 = (0x0F00 & bytes) >> 8;
  u8 nibble_3 = (0x00F0 & bytes) >> 4;
  u8 nibble_4 = (0x000F & bytes) >> 0;

  u8 NN = 0x00FF & bytes;
  u16 NNN = 0x0FFF & bytes;

  switch (nibble_1) {
  case 0x0:
    if (nibble_2 == 0 && nibble_3 == 0xE && nibble_4 == 0)
      return screen_clear();
    else
      return instruction_noop();

  case 0x1:
    return instruction_jump(NNN);

  case 0x6:
    return instruction_set(nibble_2, NN);

  case 0x7:
    return instruction_add(nibble_2, NN);

  case 0xA:
    return instruction_set_index(NNN);

  case 0xD:
    return instruction_draw(nibble_2, nibble_3, nibble_4);
  default:
    instruction_noop();
  }
}

void cpu_init() {
  cpu_logger.logger =
      log_create(cpu_logger.file, cpu_logger.process,
                 cpu_logger.is_active_console, cpu_logger.level);
}

void cpu_exec() {
  u8 byte_1 = ram_read(registers.R_PC);
  u8 byte_2 = ram_read(registers.R_PC + 1);
  u16 bytes = (byte_1 << 8) | byte_2;

  registers.R_PC += 2;

  instruction_exec(bytes);
}

void cpu_free() {}
