; add two numbers
init: LDA# q1   ; load direct "q1" address to A
      STA ptr   ; store address to ptr
prt1: LDA ptr   ; load ptr to A
      OUT* ptr  ; output indirect value at ptr
      INC       ; increment ptr
      STA ptr   ; store new position to ptr
      LDA* ptr  ; load value in ptr
      CMP# 00   ; compare to end of string
      BNE prt1  ; loop if not equal
wt1:  INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ wt1   ; wait until input has a value
      SEC       ; set carry flag
      SBC 30    ; convert to decimal
      STA x     ; store in x
      LDA# q2   ; load direct "q2" address to A
      STA ptr   ; store address to ptr
prt2: LDA ptr   ; load ptr to A
      OUT* ptr  ; output indirect value at ptr
      INC       ; increment ptr
      STA ptr   ; store new position to ptr
      LDA* ptr  ; load value in ptr
      CMP# 00   ; compare to end of string
      BNE prt2  ; loop if not equal
wt2:  INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ wt2   ; wait until input has a value
      SEC       ; set carry flag
      SBC 30    ; convert to decimal
      STA y     ; store in y
add:  LDA x     ; load x
      CLC       ; clear carry flag
      ADC y     ; add y
      STA z     ; store in z
      ADC 30    ; convert to ascii
      STA answ  ; store answer
prt:  LDA# resp ; load resp ptr to A
      STA ptr   ; store in ptr
res2: LDA ptr   ; load pointer
      OUT* ptr  ; print response
      INC       ; increment to next address
      STA ptr   ; store new position
      LDA* ptr  ; load value in ptr
      CMP# 00   ; compare to new line
      BNE res2  ; if not, print next character
      JMP init  ; go again
x:    00
y:    00
z:    00
ptr:  00
q1:   "Enter first number:"
      00
q2:   "Enter second number:"
      00
resp: "result:"
answ: 00
      00
