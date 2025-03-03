#include "../include/instructions.h"

void set_register(u8 *reg, u8 value) { *reg = value; }
void set_extended_register(u16 *reg, u16 value) { *reg = value; }

void instruction_ld(t_list *params) {
  param *first_param = list_get(params, 0);
  param *second_param = list_get(params, 1);

  if (first_param->type == REGISTER) {
    u8 *destination = first_param->value;
    if (second_param->type == REGISTER) {
      u8 *source = second_param->value;
      set_register(destination, *source);
    }
    if (second_param->type == NUMBER) {
      u8 *source = second_param->value;
      set_register(destination, *source);
    }
  }
}

void instruction_noop() {}

void (*instructions[0xFF])(t_list *) = {[NOOP] = &instruction_noop};

instruction instruction_table[0xFF] = {
    [0x00] = {NOOP},
};

void *get_instruction_function(instruction_op op) { return instructions[op]; }

instruction *get_instruction(u8 byte) { return &instruction_table[byte]; }
