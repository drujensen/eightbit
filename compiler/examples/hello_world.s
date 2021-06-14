; print hello world
init: LDA# txt  ; load direct "txt" address to A
      STA ptr   ; store address to ptr
loop: OUT* ptr  ; output indirect value at ptr
      INC       ; increment ptr
      STA ptr   ; store new position to ptr
      CMP# end  ; compare to end of string
      BNE loop  ; loop if not equal
      JMP init  ; endless loop
ptr:  00        ; Pointer to String
txt:  "Hello World!"
      0D
      0A
end:  00
