#ifndef _ASSM_H_
#define _ASSM_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "file_work.h"
#include "../commands.h"

#define DEFAULT_OUTPUT_FILENAME "../a.o"

#define NUM_OF_LABELS 20

struct code_arr
{
    int* code;
    size_t size;
};

struct label_t
{
    char label_name[20];
    int label_address;
};

code_arr* Assembler( char** arr_of_asm_str, size_t num_of_str, code_arr* byte_code, label_t* labels );
int FindLabel( const char* name, label_t* labels_arr );
int SetLabel( const char* name, int address, label_t* labels_arr );

#endif
