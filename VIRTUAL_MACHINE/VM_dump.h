#ifndef _VM_DUMP_H_
#define _VM_DUMP_H_

#include "VM.h"

void SPUDump( spu_t* spu );
void CodeDump( spu_t* spu );
void SPUStackDump( const char* stack_name, stack_t* stk );
int DisasmInstruction( spu_t* spu );
void RegDump( spu_t* spu );
//void RAMDump( spu_t* spu );

#endif
