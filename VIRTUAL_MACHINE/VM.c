#include "VM.h"


int main( int argc, char** argv )
{
    const char* code_filename = NULL;

    code_filename = SetFileName(argc, argv);

    spu_t spu = {};
    SPUCtor(&spu, code_filename);

    while (spu.ip < spu.num_of_instructions)
    {
        switch((int)(spu.code[spu.ip++]))
        {
            case PUSH:
                StackPush(&spu.stack, spu.code[spu.ip++]);
                break;
            case PUSHR:
                StackPush(&spu.stack, spu.reg[(size_t)(spu.code[spu.ip++]) - 1]);
                break;
            case PUSHM:
                StackPush(&spu.stack, spu.RAM[(size_t)(spu.code[spu.ip++])]);
                break;
            case POP:
                StackPop(&spu.stack, &spu.reg[(size_t)(spu.code[spu.ip++]) - 1]);
                break;
            case POPM:
                StackPop(&spu.stack, &spu.RAM[(size_t)(spu.code[spu.ip++])]);
                break;
            case ADD:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[0] + spu.value[1]);
                break;
            case MUL:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[0] * spu.value[1]);
                break;
            case MSG:
                printf("MESSAGE: %s", messages[(size_t)(spu.code[spu.ip++]) - 1]);
                break;
            case SUB:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[1] - spu.value[0]);
                break;
            case SQRT:
                StackPop(&spu.stack, &spu.value[0]);
                StackPush(&spu.stack, (stack_el_t)sqrt(spu.value[0]));
                break;
            case DIV:
                StackPop(&spu.stack, &spu.value[0]);
                if (spu.value[0] == 0)
                {
                    fprintf(stderr, "RUNTIME ERROR: DIVISION BY ZERO");
                    exit(1);
                }
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[1] / spu.value[0] );
                break;
            case OUT:
                StackPop(&spu.stack, &spu.value[0]);
                printf("OUTPUT:" OUTPUT_SPECIFIER "\n", spu.value[0]);
                break;
            case IN:
                printf("INPUT: ");
                scanf(OUTPUT_SPECIFIER, &spu.value[0]);
                StackPush(&spu.stack, spu.value[0]);
                break;
            case CALL:
                StackPush(&spu.ret_stack, (stack_el_t)(spu.ip + 1));
                spu.ip = (size_t)spu.code[spu.ip];
                break;
            case RET:
                StackPop(&spu.ret_stack, &spu.value[0]);
                spu.ip = (size_t)spu.value[0];
                break;
            case JMP:
                spu.ip = (size_t)spu.code[spu.ip];
                break;
            case JA:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] < spu.value[1])
                    spu.ip = (size_t)spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JAE:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] <= spu.value[1])
                    spu.ip = (size_t)spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JB:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] > spu.value[1])
                    spu.ip = (size_t)spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JBE:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] >= spu.value[1])
                    spu.ip = (size_t)spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JE:
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] == spu.value[1])
                    spu.ip = (size_t)spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case HLT:
                SPUDtor(&spu);
                return 0;
            default:
                fprintf(stderr, "RUNTIME ERROR: INCORRECT COMMAND\n");
                exit(1);
        }
    }
    if (spu.ip == spu.num_of_instructions)
    {
        fprintf(stderr, "RUNTIME ERROR: SEGMENTATION FAULT\n");
        SPUDtor(&spu);
        return 1;
    }

}

void SPUDtor( spu_t* spu )
{
    StackDtor(&spu->stack);
    StackDtor(&spu->ret_stack);
    free(spu->code);
}

void SPUCtor( spu_t* spu, const char* input_filename)
{
    StackInit(&spu->stack, INIT_SIZE);
    StackInit(&spu->ret_stack, INIT_SIZE);
    struct stat input_file = {};

    int stat_status = stat(input_filename, &input_file);
    assert(stat_status != -1);

    spu->num_of_instructions = input_file.st_size / sizeof(stack_el_t);

    spu->code = (stack_el_t*)calloc(spu->num_of_instructions, sizeof(stack_el_t));
    spu->RAM = (stack_el_t*)calloc(RAM_SIZE, sizeof(stack_el_t));
    FILE* code_file = fopen(input_filename, "rb");
    assert(code_file != NULL);

    fread(spu->code, sizeof(stack_el_t), spu->num_of_instructions, code_file);

    fclose(code_file);
}

const char* SetFileName( int argc, char** argv )
{
    const char* filename = NULL;
    if (argc > 1 && strcmp(argv[1], "../") != 0)
        filename = argv[1];
    else
        filename = DEFAULT_INPUT_FILENAME;

    return filename;
}
