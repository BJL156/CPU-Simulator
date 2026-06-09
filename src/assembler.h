#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

typedef struct {
  char name[32];
  uint8_t address;
} Label;

int assemble(const char *filename, uint8_t *out, int *out_len);

#endif
