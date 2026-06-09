#include "cpu.h"

#include <memory.h>

int main(void) {
  uint8_t program[] = {
    OP_MOV, 0, 5,   // MOV R0, 5
    OP_MOV, 1, 1,   // MOV R1, 1
    OP_SUB, 0, 1,   // SUB R0, R1 (R0 = R0 - R1)
    OP_JNZ, 0x06,   // JNZ (to SUB)
    OP_HLT          // HLT
  };

  CPU cpu;
  cpu_reset(&cpu);
  int size = sizeof(program) / sizeof(program[0]);
  memcpy(cpu.mem, program, size * sizeof(program[0]));

  while (!cpu.halted) {
    cpu_disasm(&cpu);
    cpu_step(&cpu);
  }

  cpu_dump(&cpu); 

  return 0;
}
