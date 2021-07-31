waix: INA       ; get char from input
      CMP# 00   ; compare input with 0
      OUTA      ; echo input
      BEQ waix  ; wait until input has a value
      SUB# 30   ; convert to number
      STA x     ; store in x
      OUT# 2B   ; print +
waiy: INA       ; get char from input
      CMP# 00   ; compare input with 0
      OUTA      ; echo input
      BEQ waiy  ; wait until input has a value
      SUB# 30   ; convert to number
      STA y     ; store in y
      ADD x     ; add x to y
      CMP# 0A   ; compare to 10
      OUT# 3D   ; print =
      BEQ tens  ; equal to 10
      BCC ones  ; less than 10
tens: OUT# 31   ; output 1 for 10's spot
      SUB# 0A   ; subtract 10
ones: ADD# 30   ; convert to ascii
      OUTA      ; print results
      OUT# 0D   ; print cr
      OUT# 0A   ; print nl
      JMP waix  ; start over
x:    00
y:    00
