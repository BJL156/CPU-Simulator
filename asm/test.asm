JMP main

test_func:
  MOVI R0, 2
  SHL R0
  SHL R0
  SHL R0
.jump:
  RET
main:
  JMP .jump
.idk:
  MOVI R0, 4
  STOR 0x51, R0
  JMP .hlt
.jump:
  CALL test_func
  JMP .idk
.hlt:
  HLT
