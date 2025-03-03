#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "common.h"
#include <commons/buffer.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/net/status.h>
#include <stdint.h>
#include <string.h>

typedef enum {NOOP, UNKNOWN_INSTRUCTION } instruction_op;

typedef enum { REGISTER, EXTENDED_REGISTER, NUMBER, CONDITION_CODE } param_type;

typedef struct {
  param_type type;
  void *value;
} param;

typedef struct {
  instruction_op op;
  param source;
  param destination;
} instruction;

void instruction_noop();

void *get_instruction_function(instruction_op op);

instruction *get_instruction(u8 byte);

#endif
