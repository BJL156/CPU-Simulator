#include "assembler.h"
#include "cpu.h"

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  const char *filename;
  bool step;
} Arguments;

Arguments parse_arguments(int argc, char *argv[]) {
  Arguments arguments;
  arguments.filename = NULL;
  arguments.step = false;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--step")) {
      arguments.step = true;
      continue;
    }

    if (argv[i][0] != '-' && !arguments.filename) {
      arguments.filename = argv[i];
      continue;
    }

    printf("Unknown argument: %s\n", argv[i]);
  }

  return arguments;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: %s <file.asm> [--step]\n", argv[0]);
    printf("  <file.asm>  assembly file to run.\n");
    printf("  --step      step through instructions interactively.\n");
    return 1;
  }

  Arguments arguments = parse_arguments(argc, argv);
  if (!arguments.filename) {
    printf("Add an input ASM file.\n");
    return 1;
  }

  uint8_t program[256];
  memset(program, 0, sizeof(program));
  int program_len = 0;
  int result = assemble(arguments.filename, program, &program_len);
  if (result) {
    printf("Failed to assemble \"%s\".\n", arguments.filename);
    return 1;
  }

  CPU cpu;
  cpu_reset(&cpu);
  memcpy(cpu.mem, program, program_len / sizeof(uint8_t));

  while (!cpu.halted) {
    if (arguments.step) {
      cpu_disasm(&cpu);
      cpu_step(&cpu);
      cpu_dump(&cpu);
      getchar();
    } else {
      cpu_disasm(&cpu);
      cpu_step(&cpu);
    }
  }

  if (!arguments.step) {
    cpu_dump(&cpu); 
  }

  return 0;
}
