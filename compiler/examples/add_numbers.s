waix: INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ waix  ; wait until input has a value
      SEC       ; set carry flag
      SBC# 30   ; convert to number
      STA x     ; store in x
      OUT# 2B   ; print +
waiy: INA       ; get char from input
      CMP# 00   ; compare input with 0
      BEQ waiy  ; wait until input has a value
      SEC       ; set carry flag
      SBC# 30   ; convert to number
      STA y     ; store in y
      CLC       ; clear carry flag
      ADC x     ; add x to y
      CMP# 0A   ; compare to 10
      OUT# 3D   ; print =
      BEQ tens  ; equal to 10
      BCC ones  ; less than 10
tens: OUT# 31   ; output 1 for 10's spot
      SEC       ; set carry flag
      SBC# 0A   ; subtract 10
ones: CLC       ; clear carry flag
      ADC# 30   ; convert to ascii
      STA z     ; store z
      OUT z     ; print results
      OUT# 0D   ; print cr
      OUT# 0A   ; print nl
      JMP waix  ; start over
x:    00
y:    00
z:    00
