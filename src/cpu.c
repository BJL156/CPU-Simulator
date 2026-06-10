#include "cpu.h"

#include <memory.h>
#include <stdio.h>

void cpu_disasm(const CPU *cpu) {
  uint8_t op = cpu->mem[cpu->pc];
  printf("0x%02X  ", cpu->pc);
  switch (op) {
    case OP_MOV: {
      printf("MOV R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_MOVI: {
      printf("MOVI R%d, %d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_ADD: {
      printf("ADD R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_ADDI: {
      printf("ADDI R%d, %d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_SUB: {
      printf("SUB R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_JMP: {
      printf("JMP 0x%02X\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_JZ: {
      printf("JZ 0x%02X\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_JNZ: {
      printf("JNZ 0x%02X\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_AND: {
      printf("AND R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_OR: {
      printf("OR R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_XOR: {
      printf("XOR R%d, R%d\n", cpu->mem[cpu->pc + 1], cpu->mem[cpu->pc + 2]);
      break;
    }
    case OP_NOT: {
      printf("NOT R%d\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_PUSH: {
      printf("PUSH R%d\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_POP: {
      printf("POP R%d\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_CALL: {
      printf("CALL 0x%02X\n", cpu->mem[cpu->pc + 1]);
      break;
    }
    case OP_RET: {
      printf("RET\n");
      break;
    }
    case OP_HLT: {
      printf("HLT\n");
      break;
    }
    default: {
      printf("??? (0x%02X)\n", op);
      return;
    }
  }
}

void cpu_dump(const CPU *cpu) {
  printf("PC=0x%02X  SP=0x%02X  CARRY=%d  ZERO=%d\n", cpu->pc, cpu->sp, cpu->flag_carry, cpu->flag_zero);

  int size = sizeof(cpu->reg) / sizeof(cpu->reg[0]);
  for (int i = 0; i < size; i++) {
    printf("R%d=0x%02X  ", i, cpu->reg[i]);
  }

  putchar('\n');
}

void cpu_reset(CPU *cpu) {
  memset(cpu, 0, sizeof(CPU));
  cpu->sp = 0xFF;
}

void cpu_step(CPU *cpu) {
  uint8_t op = cpu->mem[cpu->pc++];
  switch (op) {
    case OP_MOV: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      cpu->reg[rd] = cpu->reg[rs];
      break;
    }
    case OP_MOVI: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t imm = cpu->mem[cpu->pc++];
      cpu->reg[rd] = imm;
      break;
    }
    case OP_ADD: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      uint16_t result = cpu->reg[rd] + cpu->reg[rs];
      cpu->flag_carry = (result > 0xFF);
      cpu->flag_zero = ((result & 0xFF) == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_ADDI: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t imm = cpu->mem[cpu->pc++];
      uint16_t result = cpu->reg[rd] + imm;
      cpu->flag_carry = (result > 0xFF);
      cpu->flag_zero = ((result & 0xFF) == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_SUB: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      cpu->flag_carry = (cpu->reg[rd] < cpu->reg[rs]);
      uint16_t result = cpu->reg[rd] - cpu->reg[rs];
      cpu->flag_zero = ((result & 0xFF) == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_JMP: {
      uint8_t addr = cpu->mem[cpu->pc++];
      cpu->pc = addr;
      break;
    }
    case OP_JZ: {
      uint8_t addr = cpu->mem[cpu->pc++];
      if (cpu->flag_zero) {
        cpu->pc = addr;
      }
      break;
    }
    case OP_JNZ: {
      uint8_t addr = cpu->mem[cpu->pc++];
      if (!cpu->flag_zero) {
        cpu->pc = addr;
      }
      break;
    }
    case OP_AND: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      uint8_t result = cpu->reg[rd] & cpu->reg[rs];
      cpu->flag_zero = (result == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_OR: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      uint8_t result = cpu->reg[rd] | cpu->reg[rs];
      cpu->flag_zero = (result == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_XOR: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t rs = cpu->mem[cpu->pc++];
      uint8_t result = cpu->reg[rd] ^ cpu->reg[rs];
      cpu->flag_zero = (result == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_NOT: {
      uint8_t rd = cpu->mem[cpu->pc++];
      uint8_t result = ~cpu->reg[rd];
      cpu->flag_zero = (result == 0);
      cpu->reg[rd] = result;
      break;
    }
    case OP_PUSH: {
      uint8_t rs = cpu->mem[cpu->pc++];
      cpu->mem[--cpu->sp] = cpu->reg[rs];
      break;
    }
    case OP_POP: {
      uint8_t rd = cpu->mem[cpu->pc++];
      cpu->reg[rd] = cpu->mem[cpu->sp++];
      break;
    }
    case OP_CALL: {
      uint8_t addr = cpu->mem[cpu->pc++];
      cpu->mem[--cpu->sp] = cpu->pc;
      cpu->pc = addr;
      break;
    }
    case OP_RET: {
      uint8_t return_addr = cpu->mem[cpu->sp++];
      cpu->pc = return_addr;
      break;
    }
    case OP_HLT: {
      cpu->halted = true;
      break;
    }
    default: {
      printf("UNKNOWN OPCODE: 0x%02X at PC=0x%02X\n", op, cpu->pc - 1);
      return;
    }
  }
}
