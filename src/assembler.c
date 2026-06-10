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

int assemble(const char *filename, uint8_t *out, int *out_len) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file: %s\n", filename);
    return 1;
  }

  char buffer[256];
  int current_byte = 0;
  while (fgets(buffer, 256, file)) {
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
    else if (!strcmp(mnem, "HLT"))   current_byte += 1;
  }

  rewind(file);

  while (fgets(buffer, 256, file)) {
    trim(buffer);
    if (buffer[0] == '\0' || strchr(buffer, ':')) {
      continue;
    }

    char *mnem = strtok(buffer, " ,\t");
    char *op1 = strtok(NULL, " ,\t");
    char *op2 = strtok(NULL, " ,\t");

    if (!strcmp(mnem, "MOV")) {
      out[(*out_len)++] = OP_MOV;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "MOVI")) {
      out[(*out_len)++] = OP_MOVI;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = atoi(op2);
    } else if (!strcmp(mnem, "ADD")) {
      out[(*out_len)++] = OP_ADD;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "ADDI")) {
      out[(*out_len)++] = OP_ADDI;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = atoi(op2);
    } else if (!strcmp(mnem, "SUB")) {
      out[(*out_len)++] = OP_SUB;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "JMP")) {
      out[(*out_len)++] = OP_JMP;
      out[(*out_len)++] = get_label_address(op1);
    } else if (!strcmp(mnem, "JZ")) {
      out[(*out_len)++] = OP_JZ;
      out[(*out_len)++] = get_label_address(op1);
    } else if (!strcmp(mnem, "JNZ")) {
      out[(*out_len)++] = OP_JNZ;
      out[(*out_len)++] = get_label_address(op1);
    } else if (!strcmp(mnem, "AND")) {
      out[(*out_len)++] = OP_AND;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "OR")) {
      out[(*out_len)++] = OP_OR;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "XOR")) {
      out[(*out_len)++] = OP_XOR;
      out[(*out_len)++] = op1[1] - '0';
      out[(*out_len)++] = op2[1] - '0';
    } else if (!strcmp(mnem, "NOT")) {
      out[(*out_len)++] = OP_NOT;
      out[(*out_len)++] = op1[1] - '0';
    } else if (!strcmp(mnem, "PUSH")) {
      out[(*out_len)++] = OP_PUSH;
      out[(*out_len)++] = op1[1] - '0';
    } else if (!strcmp(mnem, "POP")) {
      out[(*out_len)++] = OP_POP;
      out[(*out_len)++] = op1[1] - '0';
    } else if (!strcmp(mnem, "CALL")) {
      out[(*out_len)++] = OP_CALL;
      out[(*out_len)++] = get_label_address(op1);
    } else if (!strcmp(mnem, "RET")) {
      out[(*out_len)++] = OP_RET;
    } else if (!strcmp(mnem, "HLT")) {
      out[(*out_len)++] = OP_HLT;
    }
  }

  fclose(file);
  return 0;
}
