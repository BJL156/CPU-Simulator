; Function test

JMP _start

; Adds two registers together.
; arg1:   R0 = int
; arg2:   R1 = int
; return: R0 = int
add:
  ADD R0, R1
  RET

main:
  MOVI R0, 6
  MOVI R1, 7
  CALL add

  RET       ; Pops PC then jumps back to _start.

_start:
  CALL main ; Pushes PC to the stack.
  HLT
