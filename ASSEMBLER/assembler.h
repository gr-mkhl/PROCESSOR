#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "FILE_WORK/file_work.h"
#include "../STACK/stack.h"
#include "../commands.h"
#include "../log.h"

const file_header ASM_header =
{
    .SIGN = {'M', 'K', 'H', 'L'},
    .major = 1,
    .minor = 0
};

#define DEFAULT_OUTPUT_FILENAME "../a.o"

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

struct macro_t
{
    uint64_t macro_hash;
    char* macro_value;
};

#define INIT_NUM_OF_LABELS 20
#define INIT_NUM_OF_MACROS 20

struct asm_t
{
    bytecode_t bytecode;

    label_t* labels;
    size_t num_of_labels;
    size_t labels_capacity;

    macro_t* macros;
    size_t num_of_macros;
    size_t macros_capacity;

    char** array_of_lines;
    size_t num_of_lines;
    char* buffer_ptr;
    bool second_asm;
};

void PreAssembling( int argc, char** argv, asm_t* asm_data );
void PreProcessing( asm_t* asm_data );
bool Assembler(asm_t* asm_data);
void DoAssembling( asm_t* asm_data );
void PostAssembling( int argc, char** argv, asm_t* asm_data );
int FindLabel( const char* name, asm_t* asm_data );
int SetLabel( const char* name, int address, asm_t* asm_data );
void SetHeader( asm_t* asm_data, file_header* head );
void ParsingArg( char* arg, commands comm, asm_t* asm_data);
int ParsingLabelArg( const char* arg, asm_t* asm_data );

uint64_t Hash_djb2( const void* beg, size_t len );
int CompareLabelsHashUp( const void* first, const void* second );
int CompareMacrosHashUp( const void* first, const void* second );
char* FindDefinition( char* macro_name, asm_t* asm_data );
//ssize_t binary_search( const void* target, const void* a, size_t n, size_t el_size, int (*comparator)(const void* a, const void* b) );
#endif
