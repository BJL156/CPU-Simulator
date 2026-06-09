#include "assembler.h"
#include "cpu.h"

#include <memory.h>

int main(void) {
  uint8_t program[256];
  memset(program, 0, sizeof(program));
  int program_len = 0;
  assemble("test.asm", program, &program_len);

  CPU cpu;
  cpu_reset(&cpu);
  memcpy(cpu.mem, program, program_len / sizeof(uint8_t));

  while (!cpu.halted) {
    cpu_disasm(&cpu);
    cpu_step(&cpu);
  }

  cpu_dump(&cpu); 

  return 0;
}
