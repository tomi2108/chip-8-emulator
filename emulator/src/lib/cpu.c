#include "../include/cpu.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdlib.h>

regs registers = {.R_PC = 0x200, 0};

t_config *cpu_config;
log_t cpu_logger = {.file = "log.log",
                    .process = "CPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void instruction_noop() {}

void instruction_random(u8 reg, u8 nn) {
  registers.R_X[reg] &= (rand() % 0xFF);
}

void instruction_jump(u16 nnn) { registers.R_PC = nnn; }
void instruction_jump_offset_v0(u16 nnn) {
  registers.R_PC = nnn + registers.R_X[0x0];
}
void instruction_jump_offset(u8 reg, u16 nnn) {
  registers.R_PC = nnn + registers.R_X[reg];
}
void instruction_return() {
  u16 *bytes = stack_pop();
  registers.R_PC = *bytes;
}
void instruction_sub_routine(u16 nnn) {
  stack_push(registers.R_PC);
  registers.R_PC = nnn;
}

void instruction_skip_if_reg(u8 reg, u8 nn) {
  if (registers.R_X[reg] == nn)
    registers.R_PC += 2;
}
void instruction_skip_if_not_reg(u8 reg, u8 nn) {
  if (registers.R_X[reg] != nn)
    registers.R_PC += 2;
}
void instruction_skip_if_regs(u8 reg1, u8 reg2) {
  if (registers.R_X[reg1] == registers.R_X[reg2])
    registers.R_PC += 2;
}
void instruction_skip_if_not_regs(u8 reg1, u8 reg2) {
  if (registers.R_X[reg1] != registers.R_X[reg2])
    registers.R_PC += 2;
}
void instruction_skip_if_key(u8 reg) {
  if (keypad_key_is_pressed(registers.R_X[reg]))
    registers.R_PC += 2;
}
void instruction_skip_if_not_key(u8 reg) {
  if (!keypad_key_is_pressed(registers.R_X[reg]))
    registers.R_PC += 2;
}

void instruction_set_reg_d_timer(u8 reg) { registers.R_X[reg] = d_timer_get(); }
void instruction_set_d_timer_reg(u8 reg) { d_timer_set(registers.R_X[reg]); }
void instruction_set_s_timer_reg(u8 reg) { s_timer_set(registers.R_X[reg]); }

void instruction_set_index(u16 nnn) { registers.R_I = nnn; }
void instruction_set_nn(u8 reg_dest, u8 nn) { registers.R_X[reg_dest] = nn; }
void instruction_set_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[reg_dest] = registers.R_X[reg_source];
}

void instruction_or_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[reg_dest] |= registers.R_X[reg_source];
}
void instruction_and_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[reg_dest] &= registers.R_X[reg_source];
}
void instruction_xor_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[reg_dest] ^= registers.R_X[reg_source];
}

void instruction_add_index(u8 reg_dest) {
  if (config_get_int_value(cpu_config, "add_index_carry_flag") &&
      (0x1000 - registers.R_I < registers.R_X[reg_dest]))
    registers.R_X[0xF] = 1;

  registers.R_I += registers.R_X[reg_dest];
}
void instruction_add_nn(u8 reg_dest, u8 nn) { registers.R_X[reg_dest] += nn; }
void instruction_add_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[0xF] = 0;
  if (0xFF - registers.R_X[reg_dest] < registers.R_X[reg_source])
    registers.R_X[0xF] = 1;
  registers.R_X[reg_dest] += registers.R_X[reg_source];
}

void instruction_sub_regs(u8 reg_dest, u8 reg_source) {
  registers.R_X[0xF] = 0;
  if (registers.R_X[reg_dest] > registers.R_X[reg_source])
    registers.R_X[0xF] = 1;
  registers.R_X[reg_dest] = registers.R_X[reg_dest] - registers.R_X[reg_source];
}
void instruction_sub_regs_i(u8 reg_dest, u8 reg_source) {
  registers.R_X[0xF] = 0;
  if (registers.R_X[reg_source] > registers.R_X[reg_dest])
    registers.R_X[0xF] = 1;
  registers.R_X[reg_dest] = registers.R_X[reg_source] - registers.R_X[reg_dest];
}

void instruction_shift_left(u8 reg_dest, u8 reg_source) {
  if (config_get_int_value(cpu_config, "shift_set"))
    registers.R_X[reg_dest] = registers.R_X[reg_source];
  bool bit = registers.R_X[reg_dest] & 0x80;
  registers.R_X[reg_dest] <<= registers.R_X[reg_dest];
  registers.R_X[0xF] = bit;
}
void instruction_shift_right(u8 reg_dest, u8 reg_source) {
  if (config_get_int_value(cpu_config, "shift_set"))
    registers.R_X[reg_dest] = registers.R_X[reg_source];
  bool bit = registers.R_X[reg_dest] & 0x1;
  registers.R_X[reg_dest] >>= registers.R_X[reg_dest];
  registers.R_X[0xF] = bit;
}

void instruction_get_key(u8 reg) {
  bool res = 1;
  u8 key = keypad_get_key(&res);
  if (res == 0)
    registers.R_X[reg] = key;
  else
    registers.R_PC -= 2;
}

void instruction_font_character(u8 reg) {
  u8 character = (0x000F & registers.R_X[reg]) >> 0;
  registers.R_I = 0x50 + character * 5;
}

void instruction_binary_decimal_conversion(u8 reg) {
  u8 byte = registers.R_X[reg];

  u8 digit_1 = (byte / 100) % 10;
  u8 digit_2 = (byte / 10) % 10;
  u8 digit_3 = byte % 10;

  ram_write(&digit_1, 1, registers.R_I);
  ram_write(&digit_2, 1, registers.R_I + 1);
  ram_write(&digit_3, 1, registers.R_I + 2);
}

void instruction_memory_write(u8 reg) {
  for (u8 i = 0; i <= reg; i++) {
    ram_write(&registers.R_X[reg], 1, registers.R_I + i);
  }

  if (config_get_int_value(cpu_config, "memory_access_modify_index"))
    registers.R_I += reg + 1;
}
void instruction_memory_read(u8 reg) {
  for (u8 i = 0; i <= reg; i++) {
    u8 byte = ram_read(registers.R_I + i);
    registers.R_X[reg] = byte;
  }
  if (config_get_int_value(cpu_config, "memory_access_modify_index"))
    registers.R_I += reg + 1;
}

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
    if (nibble_2 == 0 && nibble_3 == 0xE)
      if (nibble_4 == 0)
        return screen_clear();
    if (nibble_4 == 0xE)
      return instruction_return();

  case 0x1:
    return instruction_jump(NNN);

  case 0x2:
    return instruction_sub_routine(NNN);

  case 0x3:
    return instruction_skip_if_reg(nibble_2, NN);

  case 0x4:
    return instruction_skip_if_not_reg(nibble_2, NN);

  case 0x5:
    return instruction_skip_if_regs(nibble_2, nibble_3);

  case 0x6:
    return instruction_set_nn(nibble_2, NN);

  case 0x7:
    return instruction_add_nn(nibble_2, NN);

  case 0x8:
    switch (nibble_4) {
    case 0x0:
      return instruction_set_regs(nibble_2, nibble_3);
    case 0x1:
      return instruction_or_regs(nibble_2, nibble_3);
    case 0x2:
      return instruction_and_regs(nibble_2, nibble_3);
    case 0x3:
      return instruction_xor_regs(nibble_2, nibble_3);
    case 0x4:
      return instruction_add_regs(nibble_2, nibble_3);
    case 0x5:
      return instruction_sub_regs(nibble_2, nibble_3);
    case 0x6:
      return instruction_shift_right(nibble_2, nibble_3);
    case 0x7:
      return instruction_sub_regs_i(nibble_2, nibble_3);
    case 0xE:
      return instruction_shift_left(nibble_2, nibble_3);
    }

  case 0x9:
    return instruction_skip_if_not_regs(nibble_2, nibble_3);

  case 0xA:
    return instruction_set_index(NNN);
  case 0xB:
    if (config_get_int_value(cpu_config, "jump_offset_v0"))
      return instruction_jump_offset_v0(NNN);
    else
      return instruction_jump_offset(nibble_2, NNN);

  case 0xC:
    return instruction_random(nibble_2, NN);

  case 0xD:
    return instruction_draw(nibble_2, nibble_3, nibble_4);

  case 0xE:
    if (nibble_3 == 0x9 && nibble_4 == 0xE)
      return instruction_skip_if_key(nibble_2);
    if (nibble_3 == 0xA && nibble_4 == 0x1)
      return instruction_skip_if_not_key(nibble_2);

  case 0xF:
    if (nibble_3 == 0x0) {
      if (nibble_4 == 0x7)
        return instruction_set_reg_d_timer(nibble_2);
      if (nibble_4 == 0xA)
        return instruction_get_key(nibble_2);
    }

    if (nibble_3 == 0x1) {
      if (nibble_4 == 0x5)
        return instruction_set_d_timer_reg(nibble_2);
      if (nibble_4 == 0x8)
        return instruction_set_s_timer_reg(nibble_2);
      if (nibble_4 == 0xE)
        return instruction_add_index(nibble_2);
    }

    if (nibble_3 == 0x2 && nibble_4 == 0x9)
      return instruction_font_character(nibble_2);

    if (nibble_3 == 0x3 && nibble_4 == 0x3)
      return instruction_binary_decimal_conversion(nibble_2);

    if (nibble_3 == 0x5 && nibble_4 == 0x5)
      return instruction_memory_write(nibble_2);

    if (nibble_3 == 0x6 && nibble_4 == 0x5)
      return instruction_memory_read(nibble_2);

  default:
    instruction_noop();
  }
}

void cpu_init() {
  srand(time(NULL));
  cpu_logger.logger =
      log_create(cpu_logger.file, cpu_logger.process,
                 cpu_logger.is_active_console, cpu_logger.level);
  cpu_config = config_create("cpu.config");
  if (!cpu_config)
    exit_enoent_error(cpu_logger.logger, "cpu.config");
}

void cpu_exec() {
  u8 byte_1 = ram_read(registers.R_PC);
  u8 byte_2 = ram_read(registers.R_PC + 1);
  u16 bytes = (byte_1 << 8) | byte_2;

  registers.R_PC += 2;

  instruction_exec(bytes);
}

void cpu_free() {}
