#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t reg[4];
  uint8_t pc;
  uint8_t mem[256];
  bool flag_carry;
  bool flag_zero;
  bool halted;
} CPU;

void cpu_dump(const CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
