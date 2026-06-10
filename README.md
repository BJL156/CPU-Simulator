# CPU Simulator
A small CPU Simulator that emulates a 256 byte processer using a fetch-decode-execute cycle.

# Build
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
If you're on Linux, the executable was written to `build/cpu_sim`. While if you're on Windows check inside of the `build/Debug` directory.

# Example
### Input (`test.asm`)
```
; Loop test

MOV R0, 5
MOV R1, 1
loop:
  SUB R0, R1  ; R0 = R0 - R1
  JNZ loop    ; Jump to loop if the result of SUB is NOT 0.
HLT           ; Halt CPU.
```
### Output
```
0x00    MOV R0, 5
0x03    MOV R1, 1
0x06    SUB R0, R1
0x09    JNZ 0x06
0x06    SUB R0, R1
0x09    JNZ 0x06
0x06    SUB R0, R1
0x09    JNZ 0x06
0x06    SUB R0, R1
0x09    JNZ 0x06
0x06    SUB R0, R1
0x09    JNZ 0x06
0x0B    HLT
PC=0x0C  CARRY=0  ZERO=1
R0=0x00  R1=0x01  R2=0x00  R3=0x00
```

# Features
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
 
# Instruction Set
| Opcode | Mnemonic | Syntax       | Description |
|:-------:|:--------:|:------------:|:------------|
| 0x01 | MOV  | `MOV RD, imm` | Copy imm into RD. |
| 0x02 | ADD  | `ADD RD, RS` | RD = RD + RS. Updates flags. |
| 0x03 | SUB  | `SUB RD, RS` | RD = RD - RS. Updates flags. |
| 0x04 | JMP  | `JMP label` | Jump to label. |
| 0x05 | JZ   | `JZ label`  | Jump to label if zero flag is set. |
| 0x06 | JNZ  | `JNZ label` | Jump to label if zero flag is clear. |
| 0x07 | AND  | `AND RD, RS` | RD = RD & RS. Updates flags. |
| 0x08 | OR   | `OR RD, RS`  | RD = RD \| RS. Updates flags. |
| 0x09 | XOR  | `XOR RD, RS` | RD = RD ^ RS. Updates flags. |
| 0x0A | NOT  | `NOT RD`     | RD = ~RD. Updates flags. |
| 0x0B | PUSH | `PUSH RD`    | Push RD onto stack. |
| 0x0C | POP  | `POP RD`     | Pop stack into register. |
| 0x0D | CALL | `CALL label` | Push return address and jump to label. |
| 0x0E | RET  | `RET`        | Return from current subroutine. |
| 0xFF | HLT  | `HLT`        | Halt execution by terminating the CPU. |
