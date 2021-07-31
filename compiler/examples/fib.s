; print fibanocci numbers using 24 bit number
; largest number is 36th position
; DEC: 14,930,352
; HEX: E3,D1,B0
start: LDA# 00
       STA fib1l
       STA fib1h
       STA fib1d
       STA fib2h
       STA fib2d
       STA fib3h
       STA fib3d
       INC
       STA fib2l
calc:  LDA fib1l
       ADD fib2l
       STA fib3l
       LDA fib1h
       ADC fib2h
       STA fib3h
       LDA fib1d
       ADC fib2d
       STA fib3d
       OUT fib3d
       OUT fib3h
       OUT fib3l
save:  LDA fib2l
       STA fib1l
       LDA fib2h
       STA fib1h
       LDA fib2d
       STA fib1d
       LDA fib3l
       STA fib2l
       LDA fib3h
       STA fib2h
       LDA fib3d
       STA fib2d
       JMP calc
fib1l:  00
fib1h:  00
fib1d:  00
fib2l:  00
fib2h:  00
fib2d:  00
fib3l:  00
fib3h:  00
fib3d:  00
