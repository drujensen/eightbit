strt: LDA# 00   ; load 0
      STA dgt   ; initialize to 0
      STA cnt   ; initialize to 0
      STA res   ; initialize to 0
      LDA# resp ; load resp address
      STA ptr   ; store to ptr variable

inpt: INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ inpt  ; wait until input has a value
      OUTA      ; echo input
      STA* ptr  ; indirectly store in answ
      CMP# 0A   ; check for new line
      BEQ term  ; branch is enter was pressed
      LDA ptr   ; load ptr
      INC       ; increment to next address
      STA ptr   ; store ptr
      LDA dgt   ; load digit count
      INC       ; increment count
      STA dgt   ; store digit count
      JMP inpt  ; get next character

term: LDA# resp ; load resp address to A
      STA ptr   ; reset pointer to start of resp
      LDA dgt   ; load number of digits
      CMP# 01   ; one digit?
      BEQ ones  ; jump to ones
      CMP# 02   ; two digits?
      BEQ tens  ; branch to tens
      CMP# 03   ; three digits?
      BEQ hund  ; branch to hundreds
      OUT# 45   ; show E for error
      JMP strt  ; branch to start

hund: LDA* ptr  ; load value
      SUB 30    ; convert to number
      STA cnt   ; store in cnt
      LDA ptr   ; load ptr
      INC       ; move to next character
      STA ptr   ; store address to pointer
h_lp: LDA cnt   ; load count
      CMP 00    ; compare to 0
      BEQ tens  ; done with hundreds
      DEC       ; decrement
      STA cnt   ; store count
      LDA res   ; load the res
      ADD# 64   ; add 100
      STA res   ; store result
      JMP h_lp  ; go again

tens: LDA* ptr  ; load value
      SUB 30    ; convert to number
      STA cnt   ; store in cnt
      LDA ptr   ; load ptr
      INC       ; move to next character
      STA ptr   ; store address to pointer
t_lp: LDA cnt   ; load count
      CMP 00    ; compare to 0
      BEQ ones  ; done with tens
      DEC       ; decrement
      STA cnt   ; store count
      LDA res   ; load the res
      ADD# 0A   ; add 10
      STA res   ; store result
      JMP t_lp  ; go again

ones: LDA* ptr  ; load value in response
      SUB 30    ; convert to number
      ADD res   ; add result
      STA res   ; store result

      HLT       ; end program

; variables
dgt:  00
cnt:  00
res:  00
ptr:  00
resp: 00
