; радиус круга будет в DX, центр круга - пиксель [30][30] (нумерация с нуля)

IN
POP DX
CALL : FILL_MEM
DRAW
HLT

: FILL_MEM
    PUSH 0
    POP AX
    PUSH 0
    POP BX
    CALL : BEG_SC
    RET

: BEG_SC
    PUSH AX
    PUSH 60
    JB : SC
    RET

: SC
    CALL : BEG_FC
    PUSH 0
    POP BX
    PUSH AX
    PUSH 1
    ADD
    POP AX
    JMP : BEG_SC

: BEG_FC
    PUSH BX
    PUSH 60
    JB : FC
    RET

: FC
    CALL : IF_CIRCLE
    PUSH BX
    PUSH 1
    ADD
    POP BX
    JMP : BEG_FC


: IF_CIRCLE
    PUSH BX
    PUSH 30
    SUB
    PUSH BX
    PUSH 30
    SUB
    MUL
    PUSH AX
    PUSH 30
    SUB
    PUSH AX
    PUSH 30
    SUB
    MUL
    ADD
    SQRT
    PUSH DX
    JBE : YES
    RET

: YES
    PUSH AX
    PUSH 60
    MUL
    PUSH BX
    ADD
    POP CX
    PUSH 67
    POP [CX]
    RET



