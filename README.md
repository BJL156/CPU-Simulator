# CPU-Simulator
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
MOV R0, 5
MOV R1, 1
loop:
  SUB R0, R1
  JNZ loop
HLT
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
- Small byte-code instruction set:
  - [x] MOV (0x01)
  - [x] ADD (0x02)
  - [x] SUB (0x03)
  - [x] JMP (0x04)
  - [x] JZ  (0x05)
  - [x] JNZ (0x06)
  - [x] ADD (0x07)
  - [x] OR  (0x08)
  - [x] XOR (0x09)
  - [x] NOT (0x0A)
  - [x] HLT (0xFF)
- Flags
  - [x] CARRY
  - [x] ZERO
