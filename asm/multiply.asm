; Multiply test

JMP main

; Parameters
; arg1:   R0 = int
; arg2:   R1 = int
; return: R0 = int
multiply:
  PUSH R2
  PUSH R3
  XOR R2, R2
  MOVI R3, 1
  .loop:
    ADD R2, R1
    SUB R0, R3
    JNZ .loop
  MOV R0, R2
  POP R3
  POP R2
  RET

main:
  MOVI R0, 3
  MOVI R1, 2
  CALL multiply

  HLT
