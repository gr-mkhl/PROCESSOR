#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../STACK/stack.h"
#include "../commands.h"
#include "../log.h"
#define DEFAULT_INPUT_FILENAME "../a.o"
#define INIT_SIZE 10
#define RAM_SIZE 10000
#define VRAM_SIZE 3600
struct spu_t
{
    stack_t stack;
    stack_t ret_stack;

    stack_el_t* code;
    size_t ip;

    stack_el_t* RAM;

    size_t num_of_instructions;
    stack_el_t reg[4];

    stack_el_t value[2];
};


void SPUCtor( spu_t* spu, const char* input_filename );
void SPUDtor( spu_t* spu );
const char* SetFileName( int argc, char** argv );

typedef void (*command_f)(spu_t*);

void hlt( spu_t* spu );
void push( spu_t* spu );
void add( spu_t* spu );
void sub( spu_t* spu );
void mul( spu_t* spu );
void div( spu_t* spu );
void out( spu_t* spu );
void in( spu_t* spu );
void pop( spu_t* spu );
void jmp( spu_t* spu );
void ja( spu_t* spu );
void jae( spu_t* spu );
void jb( spu_t* spu );
void jbe( spu_t* spu );
void je( spu_t* spu );
void jne( spu_t* spu );
void call( spu_t* spu );
void ret( spu_t* spu );
void sqrt( spu_t* spu );
void draw( spu_t* spu );
void msg( spu_t* spu );

#define EPS 1E-8
enum SIGNS {UNDEF = -67, LESS = -1, EQUAL = 0, MORE = 1};
int CompareDoubles( const double a, const double b );



#endif
