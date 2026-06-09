# CPU-Simulator
A small CPU Simulator that emulates a 256 byte processer using a fetch-decode-execute cycle.

# Build
Clone the repository and change into its directory:
```
git clone https://github.com/BJL156/CPU-Simulator
cd CPU-Simulator
```
Then run the `makefile` which will generate an `app` executable:
```
make
```

# Features
- 256 bytes of memory.
- 4 registers (R0 - R3).
- Small byte-code instruction set:
  - [x] MOV (0x01)
  - [x] ADD (0x03)
  - [x] SUB (0x05)
  - [x] JMP (0x07)
  - [x] JZ  (0x09)
  - [x] JNZ (0x0B)
  - [x] HLT (0xFF)
- Flags
  - [x] CARRY
  - [x] ZERO
