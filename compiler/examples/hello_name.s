; print hello, Dru
init: LDA# ques ; load direct "ques" address to A
      STA ptr   ; store address to ptr
loop: OUT* ptr  ; output indirect value at ptr
      INC       ; increment ptr
      STA ptr   ; store new position to ptr
      CMP# end1 ; compare to end of string
      BNE loop  ; loop if not equal
      LDA# answ ; load direct "answer" address to A
      STA ptr   ; store to ptr
wait: INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ wait  ; wait until input has a value
      STA* ptr  ; indirectly store in answ
      CMP# 0A   ; check for new line
      BEQ resu  ; print results
      LDA ptr   ; load pos
      INC       ; increment to next address location
      STA ptr   ; save pos
      JMP wait  ; get next character
resu: LDA# resp ; load response ptr to A
      STA ptr   ; store in ptr
res2: OUT* ptr  ; print response
      INC       ; increment to next address
      STA ptr   ; store new position
      CMP# end2 ; compare to new line
      BNE res2  ; if not, print next character
      JMP init  ; go again
ptr:  00        ; Pointer to string
ques: "What is your name? "
end1: NOP
resp: "Hello, "
answ: "          "
      0D
      0A
end2: NOP
