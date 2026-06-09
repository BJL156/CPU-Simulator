#include "cpu.h"

#include <memory.h>
#include <stdio.h>

void cpu_dump(const CPU *cpu) {
  printf("PC=0x%02X  CARRY=%d  ZERO=%d\n", cpu->pc, cpu->flag_carry, cpu->flag_zero);

  int size = sizeof(cpu->reg) / sizeof(cpu->reg[0]);
  for (int i = 0; i < size; i++) {
    printf("R%d=0x%02X  ", i, cpu->reg[i]);
  }

  putchar('\n');
}

void cpu_reset(CPU *cpu) {
  memset(cpu, 0, sizeof(CPU));
}

void cpu_step(CPU *cpu) {
  uint8_t op = cpu->mem[cpu->pc++];
  switch (op) {
    case 0xFF: {
      cpu->halted = true;
      break;
    }
    case 0x01: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t imm = cpu->mem[cpu->pc++];
      cpu->reg[rd] = imm;
      break;
    }
    case 0x03: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      uint16_t result = cpu->reg[rd] + cpu->reg[rs];
      cpu->flag_carry = (result > 0xFF);
      cpu->flag_zero = ((result & 0xFF) == 0);

      cpu->reg[rd] = result;
      break;
    }
    case 0x05: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      cpu->flag_carry = (cpu->reg[rd] < cpu->reg[rs]);
      uint16_t result = cpu->reg[rd] - cpu->reg[rs];
      cpu->flag_zero = ((result & 0xFF) == 0);

      cpu->reg[rd] = result;
      break;
    }
    case 0x07: {
      uint8_t addr = cpu->mem[cpu->pc++];
      cpu->pc = addr;
      break;
    }
    case 0x09: {
      uint8_t addr = cpu->mem[cpu->pc++];
      if (cpu->flag_zero) {
        cpu->pc = addr;
      }
      break;
    }
    case 0x0B: {
      uint8_t addr = cpu->mem[cpu->pc++];
      if (!cpu->flag_zero) {
        cpu->pc = addr;
      }
      break;
    }
    default: {
      printf("UNKNOWN OPCODE: 0x%02X at PC=0x%02X\n", op, cpu->pc - 1);
      return;
    }
  }
}
