JMP main

test_func:
  MOVI R0, 2
  SHL R0
  SHL R0
  SHL R0

  RET
main:
  CALL test_func

  HLT
