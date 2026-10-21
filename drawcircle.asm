IN
POP AX ; радиус круга (в клетках) хранится в AX

CALL : FILL_MEM
DRAW
HLT

: FILL_MEM
    PUSH 0
    POP BX
    : FOR1
        PUSHR BX
        PUSH 1
        ADD
        POP BX
        PUSHRM BX
        OUT
        PUSHR BX
        PUSH 10
        JB : FOR1
    RET

