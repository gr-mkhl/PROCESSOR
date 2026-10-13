#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "../commands.h"

#define DEFAULT_INPUT_FILENAME "../a.o"
#define INIT_SIZE 10

struct processor
{
    stack_t stack;
    stack_t ret_stack;
    int* code;
    size_t ip;
    stack_el_t reg[4];

    stack_el_t value[2];
};

#endif
