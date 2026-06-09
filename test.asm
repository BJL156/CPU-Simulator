MOV R0, 5
MOV R1, 1
loop:
  SUB R0, R1
  JNZ loop
HLT
