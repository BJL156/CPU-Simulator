JMP main

test_func:
  MOVI R0, 67
  STOR 0x50, R0
  MOVI R0, 41
  LOAD R0, 0x50

  RET

main:
  CALL test_func

  HLT
