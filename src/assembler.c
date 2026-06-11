#include "assembler.h"
#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Label labels[64];
int label_count = 0;

void trim(char *line) {
  char *carriage_return = strchr(line, '\r');
  if (carriage_return != NULL) {
    *carriage_return = '\0';
  }

  char *newline = strchr(line, '\n');
  if (newline != NULL) {
    *newline = '\0';
  }

  char *comment = strchr(line, ';');
  if (comment != NULL) {
    *comment = '\0';
  }

  int i = 0;
  while (line[i] == ' ' || line[i] == '\t') {
    i++;
  }
  memmove(line, line + i, strlen(line + i) + 1);

  int len = strlen(line);
  i = len - 1;
  while (i >= 0 && (line[i] == ' ' || line[i] == '\t')) {
    i--;
  }
  line[i + 1] = '\0';
}

int get_label_address(const char *label) {
  for (int i = 0; i < label_count; i++) {
    if (!strcmp(labels[i].name, label)) {
      return labels[i].address;
    }
  }

  return -1;
}

int parse_register(const char *op, int current_line) {
  if (op == NULL || (op[0] != 'R' && op[0] != 'r')) {
    printf("Error on line %d: expected register, got \"%s\".\n", current_line, op ? op : "nothing");
    return -1;
  }

  int index = op[1] - '0';
  if (index < 0 || index > MAX_REG - 1) {
    printf("Error on line %d: invalid register \"%s\".\n", current_line, op);
    return -1;
  }

  return index;
}

int assemble(const char *filename, uint8_t *out, int *out_len) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file \"%s\".\n", filename);
    return 1;
  }

  char buffer[256];
  int current_byte = 0;
  int current_line = 0;
  while (fgets(buffer, 256, file)) {
    current_line++;
    trim(buffer);
    if (buffer[0] == '\0') {
      continue;
    }

    char *colon = strchr(buffer, ':');
    if (colon != NULL) {
      *colon = '\0';
      strcpy(labels[label_count].name, buffer);
      labels[label_count].address = current_byte;
      label_count++;
      continue;
    }

    char *mnem = strtok(buffer, " ,\t");
    if (!strcmp(mnem, "MOV"))        current_byte += 3;
    else if (!strcmp(mnem, "MOVI"))  current_byte += 3;
    else if (!strcmp(mnem, "ADD"))   current_byte += 3;
    else if (!strcmp(mnem, "ADDI"))  current_byte += 3;
    else if (!strcmp(mnem, "SUB"))   current_byte += 3;
    else if (!strcmp(mnem, "JMP"))   current_byte += 2;
    else if (!strcmp(mnem, "JZ"))    current_byte += 2;
    else if (!strcmp(mnem, "JNZ"))   current_byte += 2;
    else if (!strcmp(mnem, "AND"))   current_byte += 3;
    else if (!strcmp(mnem, "OR"))    current_byte += 3;
    else if (!strcmp(mnem, "XOR"))   current_byte += 3;
    else if (!strcmp(mnem, "NOT"))   current_byte += 2;
    else if (!strcmp(mnem, "PUSH"))  current_byte += 2;
    else if (!strcmp(mnem, "POP"))   current_byte += 2;
    else if (!strcmp(mnem, "CALL"))  current_byte += 2;
    else if (!strcmp(mnem, "RET"))   current_byte += 1;
    else if (!strcmp(mnem, "CMP"))   current_byte += 3;
    else if (!strcmp(mnem, "LOAD"))  current_byte += 3;
    else if (!strcmp(mnem, "STOR"))  current_byte += 3;
    else if (!strcmp(mnem, "HLT"))   current_byte += 1;
    else {
      printf("Error on line %d: unknown mnemomic \"%s\".\n", current_line, mnem);
      fclose(file);
      return 1;
    }
  }

  rewind(file);

  current_line = 0;
  while (fgets(buffer, 256, file)) {
    current_line++;
    trim(buffer);
    if (buffer[0] == '\0' || strchr(buffer, ':')) {
      continue;
    }

    char *mnem = strtok(buffer, " ,\t");
    char *op1 = strtok(NULL, " ,\t");
    char *op2 = strtok(NULL, " ,\t");

    if (!strcmp(mnem, "MOV")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_MOV;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "MOVI")) {
      int rd = parse_register(op1, current_line);
      if (rd == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_MOVI;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = atoi(op2);
    } else if (!strcmp(mnem, "ADD")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_ADD;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "ADDI")) {
      int rd = parse_register(op1, current_line); 
      if (rd == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_ADDI;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = atoi(op2);
    } else if (!strcmp(mnem, "SUB")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_SUB;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "JMP")) {
      int addr = get_label_address(op1);
      if (addr == -1) {
        printf("Error on line %d: undefined label \"%s\".\n", current_line, op1);
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_JMP;
      out[(*out_len)++] = (uint8_t)addr;
    } else if (!strcmp(mnem, "JZ")) {
      int addr = get_label_address(op1);
      if (addr == -1) {
        printf("Error on line %d: undefined label \"%s\".\n", current_line, op1);
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_JZ;
      out[(*out_len)++] = (uint8_t)addr;
    } else if (!strcmp(mnem, "JNZ")) {
      int addr = get_label_address(op1);
      if (addr == -1) {
        printf("Error on line %d: undefined label \"%s\".\n", current_line, op1);
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_JNZ;
      out[(*out_len)++] = (uint8_t)addr;
    } else if (!strcmp(mnem, "AND")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_AND;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "OR")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_OR;
      out[(*out_len)++] = rs;
      out[(*out_len)++] = rd;
    } else if (!strcmp(mnem, "XOR")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_XOR;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "NOT")) {
      int rd = parse_register(op1, current_line);
      if (rd == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_NOT;
      out[(*out_len)++] = rd;
    } else if (!strcmp(mnem, "PUSH")) {
      int rs = parse_register(op1, current_line);
      if (rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_PUSH;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "POP")) {
      int rd = parse_register(op1, current_line);
      if (rd == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_POP;
      out[(*out_len)++] = rd;
    } else if (!strcmp(mnem, "CALL")) {
      int addr = get_label_address(op1);
      if (addr == -1) {
        printf("Error on line %d: undefined label \"%s\".\n", current_line, op1);
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_CALL;
      out[(*out_len)++] = (uint8_t)addr;
    } else if (!strcmp(mnem, "RET")) {
      out[(*out_len)++] = OP_RET;
    } else if (!strcmp(mnem, "CMP")) {
      int rd = parse_register(op1, current_line);
      int rs = parse_register(op2, current_line);
      if (rd == -1 || rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_CMP;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "LOAD")) {
      int rd = parse_register(op1, current_line);
      if (rd == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_LOAD;
      out[(*out_len)++] = rd;
      out[(*out_len)++] = (uint8_t)strtol(op2, NULL, 0);
    } else if (!strcmp(mnem, "STOR")) {
      int rs = parse_register(op2, current_line);
      if (rs == -1) {
        fclose(file);
        return 1;
      }

      out[(*out_len)++] = OP_STOR;
      out[(*out_len)++] = (uint8_t)strtol(op1, NULL, 0);
      out[(*out_len)++] = rs;
    } else if (!strcmp(mnem, "HLT")) {
      out[(*out_len)++] = OP_HLT;
    }
  }

  fclose(file);
  return 0;
}
