; радиус круга будет в DX, центр круга - пиксель [300][300] (нумерация с нуля)

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
    PUSH 600
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
    PUSH 600
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
    PUSH 300
    SUB
    PUSH BX
    PUSH 300
    SUB
    MUL
    PUSH AX
    PUSH 300
    SUB
    PUSH AX
    PUSH 300
    SUB
    MUL
    ADD
    SQRT
    PUSH DX
    JBE : YES
    RET

: YES
    PUSH AX
    PUSH 600
    MUL
    PUSH BX
    ADD
    POP CX
    PUSH 67
    POP [CX]
    RET



