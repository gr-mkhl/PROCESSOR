#ifndef _COMMANDS_H_
#define _COMMANDS_H_


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
    MSG    = 0x14
};

#define CMD_MASK 0x1F
#define CNST_MASK 0x20
#define REG_MASK 0x40
#define RAM_MASK 0x80

typedef stack_el_t command_arg_t;
typedef uint8_t command_t;

const char* messages[] = {"NO ROOTS", "INFINITE ROOTS"};

struct file_header
{
    char SIGN[4];
    uint8_t major;
    uint8_t minor;
    uint64_t hash;
};

#endif
