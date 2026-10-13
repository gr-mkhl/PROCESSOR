#include "VM.h"


int main( int argc, char** argv )
{
    const char* input_filename = NULL;

    if (argc > 1 && strcmp(argv[1], "../") != 0)
        input_filename = argv[1];
    else
    {
        input_filename = DEFAULT_INPUT_FILENAME;
    }

    processor Proc = {};
    StackInit(&Proc.stack, INIT_SIZE);
    StackInit(&Proc.ret_stack, INIT_SIZE);
    struct stat input_file = {};

    int stat_status = stat(input_filename, &input_file);
    assert(stat_status != -1);

    size_t code_size_int = input_file.st_size / sizeof(int);

    Proc.code = (int*)calloc(code_size_int, sizeof(int));

    FILE* code_file = fopen(input_filename, "rb");
    assert(code_file != NULL);

    fread(Proc.code, sizeof(int), code_size_int, code_file);

    fclose(code_file);


    while (Proc.ip < code_size_int)
    {
        printf("AX: %d\nBX: %d\n", Proc.reg[0], Proc.reg[1]);
        switch(Proc.code[Proc.ip++])
        {
            case PUSH:
            printf("PUSH\n");
                StackPush(&Proc.stack, Proc.code[Proc.ip++]);
                break;
            case PUSHR:
            printf("PUSHR\n");
                StackPush(&Proc.stack, Proc.reg[Proc.code[Proc.ip++] - 1]);
                break;
            case POP:
            printf("POP\n");
                StackPop(&Proc.stack, &Proc.reg[Proc.code[Proc.ip++] - 1]);
                break;
            case ADD:
            printf("ADD\n");
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                StackPush(&Proc.stack, Proc.value[0] + Proc.value[1]);
                break;
            case MUL:
            printf("MUL\n");
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                StackPush(&Proc.stack, Proc.value[0] * Proc.value[1]);
                break;
            case SUB:
            printf("SUB\n");
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                StackPush(&Proc.stack, Proc.value[1] - Proc.value[0]);
                break;
            case DIV:
            printf("DIV\n");
                StackPop(&Proc.stack, &Proc.value[0]);
                if (Proc.value[0] == 0)
                {
                    fprintf(stderr, "RUNTIME ERROR: DIVISION BY ZERO");
                    exit(1);
                }
                StackPop(&Proc.stack, &Proc.value[1]);
                StackPush(&Proc.stack, Proc.value[1] / Proc.value[0]);
                break;
            case OUT:
            printf("OUT\n");
                StackPop(&Proc.stack, &Proc.value[0]);
                printf("OUTPUT: %d\n", Proc.value[0]);
                break;
            case IN:
            printf("IN\n");
                printf("INPUT: ");
                scanf("%d", &Proc.value[0]);
                StackPush(&Proc.stack, Proc.value[0]);
                break;
            case CALL:
            printf("CALL\n");
                StackPush(&Proc.ret_stack, (int)(Proc.ip + 1));
                Proc.ip = Proc.code[Proc.ip];
                break;
            case RET:
            printf("RET\n");
                StackPop(&Proc.ret_stack, &Proc.value[0]);
                Proc.ip = Proc.value[0];
                break;
            case JMP:
            printf("JMP\n");
                Proc.ip = Proc.code[Proc.ip];
                break;
            case JA:
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                if (Proc.value[1] > Proc.value[2])
                    Proc.ip = Proc.code[Proc.ip];
                break;
            case JAE:
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                if (Proc.value[1] >= Proc.value[2])
                    Proc.ip = Proc.code[Proc.ip];
                break;
            case JB:
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                if (Proc.value[1] < Proc.value[2])
                    Proc.ip = Proc.code[Proc.ip];
                break;
            case JBE:
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                if (Proc.value[1] <= Proc.value[2])
                    Proc.ip = Proc.code[Proc.ip];
                break;
            case JE:
                StackPop(&Proc.stack, &Proc.value[0]);
                StackPop(&Proc.stack, &Proc.value[1]);
                if (Proc.value[1] == Proc.value[2])
                    Proc.ip = Proc.code[Proc.ip];
                break;
            case HLT:
            printf("HLT\n");
                StackDtor(&Proc.stack);
                free(Proc.code);
                return 0; //это плохо
            default:
                fprintf(stderr, "RUNTIME ERROR: INCORRECT COMMAND\n");
                exit(1);
        }
    }
    if (Proc.ip == code_size_int)
    {
        fprintf(stderr, "RUNTIME ERROR: SEGMENTATION FAULT\n");
        StackDtor(&Proc.stack);
        free(Proc.code);
        return 0;
    }

}
