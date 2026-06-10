JMP main

test_func:
  loop:
    ADDI R0, 1
    CMP R0, R1
    JNZ loop
  RET
main:
  MOVI R0, 0
  MOVI R1, 7
  CALL test_func

  HLT
