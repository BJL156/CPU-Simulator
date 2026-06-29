# CPU Simulator
[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg)](https://www.linux.org/)
[![Instructions](https://img.shields.io/badge/instructions-25-success.svg)](#instruction-set)

A custom CPU architecture along with the assembler and bytecode interpreter both written in C. Supports registers, flags, branching, subroutines, stack and memory operations.

This project was a stepping stone toward writing for a real ISA. After designing my own instruction set here, I moved on to the [AArch64 Assembler](https://github.com/BJL156/ARM-Assembler).

## Build

Clone the repository and change into its directory:

```bash
git clone https://github.com/BJL156/CPU-Simulator
cd CPU-Simulator
```

Then use CMake:

```bash
cmake -B build
cmake --build build
```

The executable is written to `build/cpu_sim` on Linux, or `build\Debug\cpu_sim.exe` on Windows.

## Usage
```bash
./cpu_sim  [--step]
    Assembly file to run.
  --step      Step through instructions interactively.
```

## Example
### Input (`asm/loop.asm`)
```
; Loop test

MOVI R0, 5    ; R0 = 5
MOVI R1, 1    ; R1 = 1
loop:
  SUB R0, R1  ; R0 = R0 - R1
  JNZ loop    ; Jump to loop if the result of SUB is NOT 0.
HLT           ; Halt CPU.
```
### Output
```
0x00  MOVI R0, 5
0x03  MOVI R1, 1
0x06  SUB R0, R1
0x09  JNZ 0x06
0x06  SUB R0, R1
0x09  JNZ 0x06
0x06  SUB R0, R1
0x09  JNZ 0x06
0x06  SUB R0, R1
0x09  JNZ 0x06
0x06  SUB R0, R1
0x09  JNZ 0x06
0x0B  HLT
PC=0x0C  SP=0xFF  CARRY=0  ZERO=1
R0=0x00  R1=0x01  R2=0x00  R3=0x00
```

## Features
- Assembler
  - [x] Reads `.asm` files and converts them into bytecode.
  - [x] Ignores whitespace, tabs, carriage returns, and comments.
  - [x] Error handling.
  - [x] Handles scoped and unscoped labels.
- Virtual Machine
  - [x] 256 bytes of memory.
  - [x] 4 general-purpose registers (`R0`–`R3`).
  - [x] CARRY and ZERO flags.
- Interactive step-through debugger (`--step`)
  - [x] Press Enter - advance one instruction.
  - [x] `m 0x01` - hex dump at a memory address.
  - [x] `q` - quit.
 
## Instruction Set
| Opcode | Mnemonic | Syntax       | Description |
|:-------:|:--------:|:------------:|:------------|
| 0x01 | MOV  | `MOV RD, RS` | Copy RS into RD. |
| 0x02 | MOVI  | `MOVI RD, imm` | Copy imm into RD. |
| 0x03 | ADD  | `ADD RD, RS` | RD = RD + RS. Updates flags. |
| 0x04 | ADDI  | `ADDI RD, imm` | RD = RD + imm. Updates flags. |
| 0x05 | SUB  | `SUB RD, RS` | RD = RD - RS. Updates flags. |
| 0x06 | SUBI  | `SUBI RD, imm` | RD = RD - imm. Updates flags. |
| 0x07 | JMP  | `JMP label` | Jump to label. |
| 0x08 | JZ   | `JZ label`  | Jump to label if zero flag is set. |
| 0x09 | JNZ  | `JNZ label` | Jump to label if zero flag is clear. |
| 0x0A | AND  | `AND RD, RS` | RD = RD & RS. Updates flags. |
| 0x0B | OR   | `OR RD, RS`  | RD = RD \| RS. Updates flags. |
| 0x0C | XOR  | `XOR RD, RS` | RD = RD ^ RS. Updates flags. |
| 0x0D | NOT  | `NOT RD`     | RD = ~RD. Updates flags. |
| 0x0E | PUSH | `PUSH RS`    | Push RS onto stack. |
| 0x0F | POP  | `POP RD`     | Pop stack into register. |
| 0x10 | CALL | `CALL label` | Push return address and jump to label. |
| 0x11 | RET  | `RET`        | Return from current subroutine. |
| 0x12 | CMP  | `CMP RD, RS` | Executes RD - RS; however, doesn't save the result. Updates flags. |
| 0x13 | LOAD  | `LOAD RD, addr` | returns the contents of addr to RD. |
| 0x14 | LOADR  | `LOADR RD, RS` | returns the contents of RS to RD. |
| 0x15 | STOR  | `STOR addr, RS` | Saves the contents of RS to addr. |
| 0x16 | STORR  | `STORR RD, RS` | Saves the contents of RS to RD. |
| 0x17 | SHL  | `SHL RD` | Shifts bits in RD to the left by 1. |
| 0x18 | SHR  | `SHR RD` | Shifts bits in RD to the right by 1. |
| 0xFF | HLT  | `HLT`        | Halt execution by terminating the CPU. Programs MUST use this to terminate. |
