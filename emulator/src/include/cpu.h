#ifndef CPU_H_
#define CPU_H_

#include "common.h"
#include "instructions.h"
#include <commons/buffer.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/sys/exit.h>
#include <commons/sys/file.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  u8 R_PC;
  u8 R_I;
  u8 R_0;
  u8 R_1;
  u8 R_2;
  u8 R_3;
  u8 R_4;
  u8 R_5;
  u8 R_6;
  u8 R_7;
  u8 R_8;
  u8 R_9;
  u8 R_A;
  u8 R_B;
  u8 R_C;
  u8 R_D;
  u8 R_E;
  u8 R_F;
} regs;

void cpu_init();

void cpu_free();
void cpu_exec(const char *rom_data);

#endif
