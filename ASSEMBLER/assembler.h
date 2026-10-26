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

#define MAX_NUM_OF_LABELS 20

struct label_t
{
    uint64_t label_hash;
    int label_address;
};
struct bytecode_t
{
    uint8_t* code_array;
    size_t size;
};


struct asm_t
{
    bytecode_t bytecode;
    label_t* labels;
    size_t num_of_labels;
    char** array_of_lines;
    char* buffer_ptr;
    size_t num_of_lines;
    bool second_asm;
};

void PreAssembling( int argc, char** argv, asm_t* asm_data );
bool Assembler(asm_t* asm_data);
void DoAssembling( asm_t* asm_data );
void PostAssembling( int argc, char** argv, asm_t* asm_data );
int FindLabel( const char* name, label_t* labels_arr, size_t num_of_labels );
int SetLabel( const char* name, int address, label_t* labels_arr, size_t* num_of_labels );

void ParsingArg( const char* command_arg_str, commands comm, asm_t* asm_data);


uint64_t Hash_djb2( const void* beg, size_t len );
int CompareLabelsHashUp( const void* first, const void* second );

#endif
