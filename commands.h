#ifndef _COMMANDS_H_
#define _COMMANDS_H_

//TODO сделать ABRT, как яд
enum commands
{
    HLT    = 0,
    PUSH   = 1,
    ADD    = 2,
    SUB    = 3,
    MUL    = 4,
    DIV    = 5,
    OUT    = 6,
    IN     = 7,
    PUSHR  = 8,
    POP    = 9,
    JMP    = 10,
    JA     = 11,
    JAE    = 12,
    JB     = 13,
    JBE    = 14,
    JE     = 15,
    JNE    = 16,
    CALL   = 17,
    RET    = 18,
    SQRT   = 19,
    PUSHM  = 20,
    POPM   = 21,
    POPMR  = 22,
    PUSHMR = 23,
    DRAW   = 24,
    MSG    = 25 //test version
};



enum registers
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

const char* messages[] = {"NO ROOTS\n", "INFINITE ROOTS\n"};
#endif
