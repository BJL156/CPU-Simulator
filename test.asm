JMP main

test_func:
  MOVI R3, 0x40

  MOVI R0, 67
  STORR R3, R0
  MOVI R0, 41
  LOADR R0, R3

  RET

main:
  CALL test_func

  HLT
