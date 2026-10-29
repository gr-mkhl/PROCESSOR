#include "VM_dump.h"

void SPUDump( spu_t* spu )
{
    CodeDump(spu);
    SPUStackDump("Stack", &spu->stack);
    SPUStackDump("Ret_Stack", &spu->ret_stack);
    RegDump(spu);
}

void SPUStackDump( const char* stack_name, stack_t* stk )
{
    printf("__stack_dump__: %s\n", stack_name);
    printf("=========================================\n");
    ssize_t top = stk->size >= 1 ? stk->size : 0;
    ssize_t last = stk->size > 3 ? top - 3 : 0;
    if (top == 0)
    {
        printf("<empty>\n");
        printf("\n\n\n");
        printf("=========================================\n\n");
        return;
    }
    for (ssize_t i = top - 1; i >= last; i--)
        printf("[%zu]: " OUTPUT_SPECIFIER "\n", i, stk->data[i]);
    for (ssize_t j = 0; j < 4 - (top - last) && stk->size <= 3; j++)
        putchar('\n');
    if (stk->size > 3)
        printf(". . .\n");
    printf("=========================================\n\n");
}

void RegDump( spu_t* spu )
{
    printf("__register_dump__\n");
    printf("=========================================\n");
    printf("register name\t| value\n");
    printf("_________________________________________\n");
    for (size_t i = 0; i < NUM_OF_RX; i++)
        printf("%cX\t\t| " OUTPUT_SPECIFIER "\n", (int)('A' + i), spu->reg[i]);
    printf("=========================================\n\n");
}

void CodeDump( spu_t* spu )
{
    size_t last_dump_ip = (spu->ip > spu->num_of_instructions - 3 ? spu->num_of_instructions - 1 : spu->ip + 2);
    size_t first_dump_ip = last_dump_ip < 5 ? 0 : last_dump_ip - 4;
    printf("__code_dump__\n");
    printf("=========================================\n");
    printf("ip\t\t| byte\t\t| command\n");
    printf("_________________________________________\n");
    for (size_t i = first_dump_ip; i <= last_dump_ip; i++)
    {
        if (i == spu->ip)
        {
            printf("0x%llX\t\t| [%d]\t\t| ", i, spu->code[i]);
            DisasmInstruction(spu);
        }
        else
            printf("0x%llX\t\t| %d\t\t|\n", i, spu->code[i]);
    }
    printf("=========================================\n\n");
}

int DisasmInstruction( spu_t* spu )
{

    command_t com = spu->code[spu->ip];
    size_t arg_ip = spu->ip + 1;

    switch(com & CMD_MASK)
    {
        case PUSH:
        {
            printf("PUSH ");
            if (com & RAM_MASK)
                putchar('[');
            if ((com & REG_MASK) && (com & CNST_MASK))
            {
                uint8_t reg_name = (uint8_t)(spu->code[arg_ip++] + 'A');
                printf("%cX + ", reg_name);
                command_arg_t const_arg = 0;
                memcpy(&const_arg, &spu->code[arg_ip], sizeof(command_arg_t));
                arg_ip += sizeof(command_arg_t);
                printf(OUTPUT_SPECIFIER, const_arg);
            }
            else if (com & REG_MASK)
            {
                uint8_t reg_name = (uint8_t)(spu->code[arg_ip++] + 'A');
                printf("%cX", reg_name);
            }
            else if (com & CNST_MASK)
            {
                command_arg_t const_arg = 0;
                memcpy(&const_arg, &spu->code[arg_ip], sizeof(command_arg_t));
                arg_ip += sizeof(command_arg_t);
                printf(OUTPUT_SPECIFIER, const_arg);
            }
            if (com & RAM_MASK)
                putchar(']');
            putchar('\n');
            break;
        }
        case POP:
        {
            printf("POP ");
            if (com & RAM_MASK)
                putchar('[');
            if ((com & REG_MASK) && (com & CNST_MASK))
            {
                uint8_t reg_name = (uint8_t)(spu->code[arg_ip++] + 'A');
                printf("%cX + ", reg_name);
                command_arg_t const_arg = 0;
                memcpy(&const_arg, &spu->code[arg_ip], sizeof(command_arg_t));
                arg_ip += sizeof(command_arg_t);
                printf(OUTPUT_SPECIFIER, const_arg);
            }
            else if (com & REG_MASK)
            {
                uint8_t reg_name = (uint8_t)(spu->code[arg_ip++] + 'A');
                printf("%cX", reg_name);
            }
            else if (com & CNST_MASK)
            {
                command_arg_t const_arg = 0;
                memcpy(&const_arg, &spu->code[arg_ip], sizeof(command_arg_t));
                arg_ip += sizeof(command_arg_t);
                printf(OUTPUT_SPECIFIER, const_arg);
            }
            if (com & RAM_MASK)
                putchar(']');
            putchar('\n');
            break;
        }
        case JMP:
        {
            printf("JMP ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JA:
        {
            printf("JA ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JAE:
        {
            printf("JAE ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JE:
        {
            printf("JE ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JBE:
        {
            printf("JBE ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JB:
        {
            printf("JB ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case JNE:
        {
            printf("JNE ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case CALL:
        {
            printf("CALL ");
            int address = 0;
            memcpy(&address, &spu->code[arg_ip], sizeof(int));
            arg_ip += sizeof(int);
            printf("%d\n", address);
            break;
        }
        case ADD:
        {
            printf("ADD\n");
            break;
        }
        case SUB:
        {
            printf("SUB\n");
            break;
        }
        case MUL:
        {
            printf("MUL\n");
            break;
        }
        case DIV:
        {
            printf("DIV\n");
            break;
        }
        case IN:
        {
            printf("IN\n");
            break;
        }
        case OUT:
        {
            printf("OUT\n");
            break;
        }
        case HLT:
        {
            printf("HLT\n");
            break;
        }
        case MSG:
        {
            printf("MSG\n");
            break;
        }
        case RET:
        {
            printf("RET\n");
            break;
        }
        case SQRT:
        {
            printf("SQRT\n");
            break;
        }
        case DRAW:
        {
            printf("DRAW\n");
            break;
        }
        default:
        {
            printf("UNKNOWN COMMAND\n");
            break;
        }
    }
    return 1;
}

// void RAMDump( spu_t* spu )
// {
//     for (size_t i = 0; i < VRAM_SIZE; i++)
//     {
//         printf("[%zu]:", i);
//         uint8_t* ptr = (uint8_t*)&spu->RAM[i];
//         for (size_t j = 0; j < sizeof(ram_t); j++)
//             printf("%3X", *(ptr + j));
//         putchar('\n');
//     }
// }
