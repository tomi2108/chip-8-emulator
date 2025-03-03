#ifndef CART_H_
#define CART_H_

#include "common.h"
#include <commons/buffer.h>

typedef struct {
} cartdrige_header;

typedef struct {
  int rom_size;
  char *rom_data;
} cartdrige;

cartdrige *cartdrige_load(const char *filename);

const char *get_rom_type(u8 code);

#endif
