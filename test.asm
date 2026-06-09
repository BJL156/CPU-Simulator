; Loop test

MOV R0, 5
MOV R1, 1
loop:
  SUB R0, R1  ; R0 = R0 - R1
  JNZ loop    ; Jump to loop if the SUB operation is NOT 0.
HLT           ; Halt CPU.
