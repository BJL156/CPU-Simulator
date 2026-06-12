; Loop test

MOVI R0, 5    ; R0 = 5
MOVI R1, 1    ; R1 = 1
loop:
  SUB R0, R1  ; R0 = R0 - R1
  JNZ loop    ; Jump to loop if the result of SUB is NOT 0.
HLT           ; Halt CPU.
