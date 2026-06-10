# CPU Simulator
A custom CPU architecture, assembler, and bytecode interpreter written in C. Supports registers, flags, branching, subroutines, stack and memory operations.

## Build
Clone the repository and change into its directory:
```
git clone https://github.com/BJL156/CPU-Simulator
cd CPU-Simulator
```
Then use CMake:
```
cmake -B build
cmake --build build
```
On Linux the executable gets written to `build/cpu_sim`. While if on Windows check inside of `build/Debug`.

### Usage
```
./cpu_sim <file.asm> [--step]
  <file.asm>  assembly file to run.
  --step      step through instructions interactively.
```

## Example
### Input (`test.asm`)
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
- Assembler.
  - [x] Reads `.asm` files.
  - [x] Converts files into bytecode.
  - [x] Ignores whitespace, tabs, carriage return, and comments.
- 256 bytes of memory.
- 4 registers (R0 - R3).
- Small byte-code instruction set. See [Instruction Set](#instruction-set).
- Flags
  - [x] CARRY
  - [x] ZERO
 
## Instruction Set
| Opcode | Mnemonic | Syntax       | Description |
|:-------:|:--------:|:------------:|:------------|
| 0x01 | MOV  | `MOV RD, RS` | Copy RS into RD. |
| 0x02 | MOVI  | `MOVI RD, imm` | Copy imm into RD. |
| 0x03 | ADD  | `ADD RD, RS` | RD = RD + RS. Updates flags. |
| 0x04 | ADDI  | `ADDI RD, imm` | RD = RD + imm. Updates flags. |
| 0x05 | SUB  | `SUB RD, RS` | RD = RD - RS. Updates flags. |
| 0x06 | JMP  | `JMP label` | Jump to label. |
| 0x07 | JZ   | `JZ label`  | Jump to label if zero flag is set. |
| 0x08 | JNZ  | `JNZ label` | Jump to label if zero flag is clear. |
| 0x09 | AND  | `AND RD, RS` | RD = RD & RS. Updates flags. |
| 0x0A | OR   | `OR RD, RS`  | RD = RD \| RS. Updates flags. |
| 0x0B | XOR  | `XOR RD, RS` | RD = RD ^ RS. Updates flags. |
| 0x0C | NOT  | `NOT RD`     | RD = ~RD. Updates flags. |
| 0x0D | PUSH | `PUSH RD`    | Push RD onto stack. |
| 0x0E | POP  | `POP RD`     | Pop stack into register. |
| 0x0F | CALL | `CALL label` | Push return address and jump to label. |
| 0x10 | RET  | `RET`        | Return from current subroutine. |
| 0x11 | CMP  | `CMP RD, RS` | Executes RD - RS; however, doesn't save the result. Updates flags. |
| 0x12 | LOAD  | `LOAD RD, addr` | returns the contents of addr to RD. |
| 0x13 | STOR  | `STOR addr, RS` | Saves the contents of RS to addr. |
| 0xFF | HLT  | `HLT`        | Halt execution by terminating the CPU. |
