#ifndef _ASSM_H_
#define _ASSM_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "file_work.h"
#include "../commands.h"

#define DEFAULT_OUTPUT_FILENAME "../a.o"

#define NUM_OF_LABELS 20

struct code_arr
{
    int* code;
    size_t size;
};


code_arr* Assembler( char** arr_of_asm_str, size_t num_of_str, code_arr* byte_code, int* labels );

#endif
