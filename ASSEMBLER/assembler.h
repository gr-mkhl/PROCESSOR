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
#define MAX_NUM_OF_MACROS 20

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

struct asm_t
{
    bytecode_t bytecode;

    label_t* labels;
    size_t num_of_labels;

    macro_t* macros;
    size_t num_of_macros;

    char** array_of_lines;
    size_t num_of_lines;
    char* buffer_ptr;
    bool second_asm;
};

void PreAssembling( int argc, char** argv, asm_t* asm_data );
void Preprocessing( asm_t* asm_data );
bool Assembler(asm_t* asm_data);
void DoAssembling( asm_t* asm_data );
void PostAssembling( int argc, char** argv, asm_t* asm_data );
int FindLabel( const char* name, label_t* labels_arr, size_t num_of_labels );
int SetLabel( const char* name, int address, label_t* labels_arr, size_t* num_of_labels );

void ParsingArg( char* arg, commands comm, asm_t* asm_data);
int ParsingLabelArg( const char* arg, asm_t* asm_data );

uint64_t Hash_djb2( const void* beg, size_t len );
int CompareLabelsHashUp( const void* first, const void* second );
int CompareMacrosHashUp( const void* first, const void* second );
char* FindDefinition( char* macro_name, asm_t* asm_data );
#endif
