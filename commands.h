#ifndef _COMMANDS_H_
#define _COMMANDS_H_

//TODO сделать ABRT, как яд
enum commands
{
    HLT    = 0x00,
    PUSH   = 0x01,
    ADD    = 0x02,
    SUB    = 0x03,
    MUL    = 0x04,
    DIV    = 0x05,
    OUT    = 0x06,
    IN     = 0x07,
    POP    = 0x08,
    JMP    = 0x09,
    JA     = 0x0A,
    JAE    = 0x0B,
    JB     = 0x0C,
    JBE    = 0x0D,
    JE     = 0x0E,
    JNE    = 0x0F,
    CALL   = 0x10,
    RET    = 0x11,
    SQRT   = 0x12,
    DRAW   = 0x13,
    MSG    = 0x14 //test version
};

#define CMD_MASK 0x1F
#define CNST_MASK 0x20
#define REG_MASK 0x40
#define REG_NUM 0x300
#define RAM_MASK 0x80

enum registers
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

const char* messages[] = {"NO ROOTS", "INFINITE ROOTS"};
#endif
