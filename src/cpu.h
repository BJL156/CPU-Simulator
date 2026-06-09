#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  OP_MOV = 0x01,
  OP_ADD = 0x02,
  OP_SUB = 0x03,
  OP_JMP = 0x04,
  OP_JZ  = 0x05,
  OP_JNZ = 0x06,
  OP_AND = 0x07,
  OP_OR  = 0x08,
  OP_XOR = 0x09,
  OP_NOT = 0x0A,
  OP_HLT = 0xFF
} Opcode;

typedef struct {
  uint8_t reg[4];
  uint8_t pc;
  uint8_t mem[256];
  bool flag_carry;
  bool flag_zero;
  bool halted;
} CPU;

void cpu_disasm(const CPU *cpu);
void cpu_dump(const CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
