#ifndef CPU_H_
#define CPU_H_

#include "common.h"
#include "memory.h"
#include "ppu.h"
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
  u8 R_X[0x10];
  u16 R_PC;
  u16 R_I;
} regs;

void cpu_init();

void cpu_free();
void cpu_exec();

#endif
