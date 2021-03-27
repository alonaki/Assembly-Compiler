;test for first pass
;missing operand
        sub #5

;missing operand
        sub #5,

;missing operand
        red

;invalid target operand (immediate)
        add  #5,#6

;invalid operand (immediate)
        inc  #50

;undefined instruction name
        and  r1,r2

;undefined instruction name (case sensitivity)
        jSr  Start

;invalid characters (,r2)
        cmp  r1,,r2

;invalid characters (,r3)
        add  #5,r1,r3

;invalid characters (blabla)S
        prn  r1 blabla

;invalid operand (undefined addressing mode)
        prn  48

;invalid operand (incomplete immediate #)  
        prn  # 48

;invalid characters (r3)
        cmp  r1,#r3

;invalid source operand (register)   
        lea  r3,r1

;invalid source operand (immediate)   
        lea  #3,r5

;invalid operand (or invalid characters (Start))
        stop Start

;invalid characters (400) 
Z1:     .data   200 400

;invalid characters (,3)
Z2:     .data   1, ,3

;invalid character (,)
Z3:     .data   4,

;invalid characters (#123)
Z4:     .data   #123

;invalid characters (.4)
Z5:     .data   12.4

;invalid characters (-5)
Z6:     .data   --5

;invalid label (cannot be an instruction name)
mov:    .data   5

;invalid label (cannot be a register name)
r6:     .data   200

;invalid label(non-alphabetic first character)
1DATA:  .data   300

;invalid symbol (DATA2 is not an instruction name and not a valid label)
DATA2   .data   4

;invalid characters (blabla is not a string)
Z7:     .string blabla

;invalid characters (blabla)
Z8:     .string "abcdefg" blabla

;invalid label (too long)
SuperCalifragilisticExpiAlidocious: .data 4
          
;missing argument in directive
Z9:     .data

; missing argument in directive
        .entry

;undefined directive
Z10:    .invalid 85, 90


;invalid operand (or invalid characters (Next))
        rts Next






