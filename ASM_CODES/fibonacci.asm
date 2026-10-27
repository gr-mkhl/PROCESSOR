IN
CALL : FIBONACCI
OUT
HLT


: FIBONACCI
    POP AX

    PUSH AX
    PUSH 0
    JE : RET_0

    PUSH AX
    PUSH 1
    JE : RET_1


    PUSH AX
    PUSH AX
    PUSH 1
    SUB
    CALL : FIBONACCI

    POP BX
    POP AX
    PUSH BX
    PUSH AX
    PUSH 2
    SUB
    CALL : FIBONACCI

    ADD
    RET

: RET_0
    PUSH 0
    RET

: RET_1
    PUSH 1
    RET
