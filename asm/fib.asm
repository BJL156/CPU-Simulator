; Fibonacci test

JMP main

fibonacci:
  MOV R3, R0
  MOVI R0, 0
  MOVI R1, 1
  .loop:
    MOV R2, R0
    ADD R2, R1
    MOV R0, R1
    MOV R1, R2
    SUBI R3, 1
    JNZ .loop
  MOV R0, R2
  RET
main:
  MOVI R0, 6
  CALL fibonacci
  HLT
