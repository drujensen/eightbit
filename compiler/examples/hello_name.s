; print hello, Dru
init: LDA# ques ; load direct "ques" address to A
      STA ptr   ; store address to ptr
loop: LDA ptr   ; load ptr to A
      OUT* ptr  ; output indirect value at ptr
      INC       ; increment ptr
      STA ptr   ; store new position to ptr
      LDA* ptr  ; load value in ptr
      CMP# 00   ; compare to end of string
      BNE loop  ; loop if not equal
      LDA# answ ; load direct "answer" address to A
      STA ptr   ; store to ptr
wait: INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ wait  ; wait until input has a value
      OUTA      ; echo input
      STA* ptr  ; indirectly store in answ
      CMP# 0A   ; check for new line
      BEQ prnt  ; print results if done
      LDA ptr   ; load pos
      INC       ; increment to next address location
      STA ptr   ; save pos
      JMP wait  ; get next character
prnt: LDA# 00   ; set A to 00
      STA* ptr  ; terminate string with 00
      LDA# resp ; load response ptr to A
      STA ptr   ; store in ptr
res2: LDA ptr   ; load pointer
      OUT* ptr  ; print response
      INC       ; increment to next address
      STA ptr   ; store new position
      LDA* ptr  ; load value in ptr
      CMP# 00   ; compare to new line
      BNE res2  ; if not, print next character
      JMP init  ; go again
ptr:  00        ; Pointer to string
ques: "What is your name? "
      00
resp: "Hello, "
answ: 00
