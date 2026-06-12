; Array test

; Setup data for the array.
MOVI R0, 10
MOVI R1, 0x50 ; array[0]
STORR R1, R0

MOVI R0, 20
MOVI R1, 0x51 ; array[1]
STORR R1, R0

MOVI R0, 30
MOVI R1, 0x52 ; array[2]
STORR R1, R0

; Initialize array.
MOVI R0, 0x50   ; Pointer to start of array.
MOVI R1, 0      ; int sum = 0;

loop:
  LOADR R3, R0  ; Load current element into R3.
  ADD R1, R3    ; sum = sum + R3;

  ADDI R0, 1    ; Move array point to next element.

  ; Break out of loop if reached element 3.
  MOVI R3, 0x53
  CMP R0, R3
  JNZ loop

HLT

