#include "cpu.h"

#include <memory.h>

int main(void) {
  uint8_t program[] = {
    0x01, 0, 5,   // MOV R0, 5
    0x01, 1, 1,   // MOV R1, 1
    0x05, 0, 1,   // SUB R0, R1 (R0 = R0 - R1)
    0x0B, 0x06,   // JNZ (to SUB)
    0xFF          // HLT
  };

  CPU cpu;
  cpu_reset(&cpu);
  int size = sizeof(program) / sizeof(program[0]);
  memcpy(cpu.mem, program, size * sizeof(program[0]));

  while (!cpu.halted) {
    cpu_step(&cpu);
  }

  cpu_dump(&cpu); 

  return 0;
}
