JMP main

test:
  MOV R2, 4
  MOV R3, 5
  ADD R2, R3
  RET
main:
  MOV R0, 3
  MOV R1, 1

  CALL test

HLT
