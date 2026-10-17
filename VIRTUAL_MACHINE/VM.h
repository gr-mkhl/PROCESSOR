#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "STACK/stack.h"
#include "../commands.h"

#define DEFAULT_INPUT_FILENAME "../a.o"
#define INIT_SIZE 10

struct spu_t
{
    stack_t stack;
    stack_t ret_stack;

    int* code;
    size_t ip;

    int* RAM;

    size_t num_of_instructions;
    stack_el_t reg[4];

    stack_el_t value[2];
};

#define COEFF 1000
void SPUCtor( spu_t* spu, const char* input_filename );
void SPUDtor( spu_t* spu );
const char* SetFileName( int argc, const char* const * argv );

#endif
