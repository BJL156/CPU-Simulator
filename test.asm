JMP main

test:
  MOVI R2, 4
  MOVI R3, 5
  ADD R2, R3
  RET
main:
  MOVI R0, 3
  MOVI R1, 1
  ADD R0, R1

  CALL test

  MOV R0, R3
  ADDI R0, 3

  HLT
