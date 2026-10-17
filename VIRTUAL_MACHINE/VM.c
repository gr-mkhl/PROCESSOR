#include "VM.h"


int main( int argc, char** argv )
{
    const char* code_filename = NULL;

    code_filename = SetFileName(argc, argv);

    spu_t spu = {};
    SPUCtor(&spu, code_filename);

    while (getchar() && spu.ip < spu.num_of_instructions)
    {
printf("AX: %d\nBX: %d\nCX: %d\nDX: %d\n", spu.reg[0], spu.reg[1], spu.reg[2], spu.reg[3]);
        switch(spu.code[spu.ip++])
        {
            case PUSH:
printf("PUSH\n");
                StackPush(&spu.stack, spu.code[spu.ip++] * COEFF);
                break;
            case PUSHR:
printf("PUSHR\n");
                StackPush(&spu.stack, spu.reg[spu.code[spu.ip++] - 1]);
                break;
            case POP:
printf("POP\n");
                StackPop(&spu.stack, &spu.reg[spu.code[spu.ip++] - 1]);
                break;
            case ADD:
printf("ADD\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[0] + spu.value[1]);
                break;
            case MUL:
printf("MUL\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[0] * spu.value[1] / COEFF);
                break;
            case MSG:
printf("MSG\n");
                printf("MESSAGE: %s", messages[spu.code[spu.ip++] - 1]);
                break;
            case SUB:
printf("SUB\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[1] - spu.value[0]);
                break;
            case SQRT:
printf("SQRT\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPush(&spu.stack, (int)sqrt(spu.value[0] * COEFF));
                break;
            case DIV:
printf("DIV\n");
                StackPop(&spu.stack, &spu.value[0]);
                if (spu.value[0] == 0)
                {
                    fprintf(stderr, "RUNTIME ERROR: DIVISION BY ZERO");
                    exit(1);
                }
                StackPop(&spu.stack, &spu.value[1]);
                StackPush(&spu.stack, spu.value[1] * COEFF / spu.value[0] );
                break;
            case OUT:
printf("OUT\n");
                StackPop(&spu.stack, &spu.value[0]);
                printf("OUTPUT: %d.%d\n", spu.value[0] / COEFF, (int)fabs(spu.value[0] % COEFF));
                break;
            case IN:
printf("IN\n");
                printf("INPUT: ");
                scanf("%d", &spu.value[0]);
                StackPush(&spu.stack, spu.value[0] * COEFF);
                break;
            case CALL:
printf("CALL\n");
                StackPush(&spu.ret_stack, (int)(spu.ip + 1));
                spu.ip = spu.code[spu.ip];
                break;
            case RET:
printf("RET\n");
                StackPop(&spu.ret_stack, &spu.value[0]);
                spu.ip = spu.value[0];
                break;
            case JMP:
printf("JMP\n");
                spu.ip = spu.code[spu.ip];
                break;
            case JA:
printf("JA\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] < spu.value[1])
                    spu.ip = spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JAE:
printf("JAE\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] <= spu.value[1])
                    spu.ip = spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JB:
printf("JB\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] > spu.value[1])
                    spu.ip = spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JBE:
printf("JBE\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] >= spu.value[1])
                    spu.ip = spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case JE:
printf("JE\n");
                StackPop(&spu.stack, &spu.value[0]);
                StackPop(&spu.stack, &spu.value[1]);
                if (spu.value[0] == spu.value[1])
                    spu.ip = spu.code[spu.ip];
                else
                    spu.ip++;
                break;
            case HLT:
printf("HLT\n");
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

    spu->num_of_instructions = input_file.st_size / sizeof(int);

    spu->code = (int*)calloc(spu->num_of_instructions, sizeof(int));

    FILE* code_file = fopen(input_filename, "rb");
    assert(code_file != NULL);

    fread(spu->code, sizeof(int), spu->num_of_instructions, code_file);

    fclose(code_file);
}

const char* SetFileName( int argc, const char* const * argv )
{
    const char* filename = NULL;
    if (argc > 1 && strcmp(argv[1], "../") != 0)
        filename = argv[1];
    else
        filename = DEFAULT_INPUT_FILENAME;

    return filename;
}
