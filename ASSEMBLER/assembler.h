#ifndef _ASSM_H_
#define _ASSM_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "FILE_WORK/file_work.h"
#include "../STACK/stack.h"
#include "../commands.h"
#include "../log.h"


#define DEFAULT_OUTPUT_FILENAME "../a.o"

#define NUM_OF_LABELS 20

struct bytecode_t
{
    stack_el_t* code_array;
    size_t size;
};

struct label_t
{
    char label_name[50];
    int label_address;
};

struct asm_t
{
    bytecode_t bytecode;
    label_t* labels;
    char** array_of_lines;
    char* buffer_ptr;
    size_t num_of_lines;
    bool need_twice_asm;
};

void PreAssembling( int argc, char** argv, asm_t* asm_data );
void Assembler(asm_t* asm_data);
void DoAssembling( asm_t* asm_data );
void PostAssembling( int argc, char** argv, asm_t* asm_data );
int FindLabel( const char* name, label_t* labels_arr );
int SetLabel( const char* name, int address, label_t* labels_arr );
void SwitchRegister( asm_t* asm_data, const char* name );

#endif
