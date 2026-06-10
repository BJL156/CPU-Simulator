#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  OP_MOV  = 0x01,
  OP_MOVI = 0x02,
  OP_ADD  = 0x03,
  OP_ADDI = 0x04,
  OP_SUB  = 0x05,
  OP_JMP  = 0x06,
  OP_JZ   = 0x07,
  OP_JNZ  = 0x08,
  OP_AND  = 0x09,
  OP_OR   = 0x0A,
  OP_XOR  = 0x0B,
  OP_NOT  = 0x0C,
  OP_PUSH = 0x0D,
  OP_POP  = 0x0E,
  OP_CALL = 0x0F,
  OP_RET  = 0x10,
  OP_HLT  = 0xFF
} Opcode;

typedef struct {
  uint8_t reg[4];
  uint8_t pc;
  uint8_t mem[256];
  uint8_t sp;
  bool flag_carry;
  bool flag_zero;
  bool halted;
} CPU;

void cpu_disasm(const CPU *cpu);
void cpu_dump(const CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
