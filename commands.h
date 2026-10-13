#ifndef _COMMANDS_H_
#define _COMMANDS_H_


enum commands
{
    HLT = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    OUT = 6,
    IN = 7,
    PUSHR = 8,
    POP = 9,
    JMP = 10,
    JA = 11,
    JAE = 12,
    JB = 13,
    JBE = 14,
    JE = 15
};

enum registers
{
    A_X = 1,
    B_X = 2,
    C_X = 3,
    D_X = 4
};


#endif
