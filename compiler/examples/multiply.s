; multiply age by count
init: LDA# 00
      STA idx  ; init to 0
      STA res  ; init to 0
loop: LDA res  ; load the last result
      ADD age  ; add the age
      STA res  ; store the result
      LDA idx  ; load the index
      INC      ; increment
      STA idx  ; store the index
      CMP cnt  ; compare to the count
      BNE loop ; if not done, go again
      OUT res  ; print the results
      HLT      ; halt
cnt:  05
age:  09
idx:  00
res:  00
