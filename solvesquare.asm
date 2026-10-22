IN
POP AX
IN
POP BX
IN
POP CX

PUSH 1
CALL : SOLVE_SQUARE
HLT


: SOLVE_SQUARE
    PUSH 0
    PUSH AX
    JE : SOLVE_LINEAR        ; if (a == 0) -> linear equation

    CALL : COUNT_DISCR       ; else        -> classic square equation

    PUSH DX
    PUSH 0
    JB : NO_ROOTS

    PUSH DX
    PUSH 0
    JE : ONE_ROOT

    PUSH DX
    PUSH 0
    JA : TWO_ROOTS

: COUNT_DISCR
    PUSH BX
    PUSH BX
    MUL
    PUSH 4
    PUSH AX
    MUL
    PUSH CX
    MUL
    SUB
    POP DX
    RET

: ONE_ROOT
    PUSH -1
    PUSH BX
    MUL
    PUSH 2
    PUSH AX
    MUL
    DIV
    OUT
    HLT

: TWO_ROOTS
    PUSH -1
    PUSH BX
    MUL
    PUSH DX
    SQRT
    ADD
    PUSH 2
    PUSH AX
    MUL
    DIV
    OUT

    PUSH -1
    PUSH BX
    MUL
    PUSH DX
    SQRT
    SUB
    PUSH 2
    PUSH AX
    MUL
    DIV
    OUT

    HLT

: SOLVE_LINEAR
    PUSH BX
    PUSH 0
    JE : SOLVE_SIMPLE   ; if (b == 0) -> simple equation

    PUSH CX            ; else        -> classic linear equation
    PUSH -1
    MUL
    PUSH BX
    DIV
    OUT                 ; out(-c / b)
    HLT

: SOLVE_SIMPLE
    PUSH CX
    PUSH 0
    JE : INF_ROOTS       ; if (c == 0) -> infinite roots

    JMP : NO_ROOTS       ; else        -> no roots
    HLT

: INF_ROOTS
    MSG 2
    HLT

: NO_ROOTS
    MSG 1
    HLT

