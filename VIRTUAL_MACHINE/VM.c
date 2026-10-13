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

    processor My = {};
    StackInit(&My.stack, INIT_SIZE);
    struct stat input_file = {};

    int stat_status = stat(input_filename, &input_file);
    assert(stat_status != -1);

    size_t code_size_int = input_file.st_size / sizeof(int);

    My.code = (int*)calloc(code_size_int, sizeof(int));

    FILE* code_file = fopen(input_filename, "rb");
    assert(code_file != NULL);

    fread(My.code, sizeof(int), code_size_int, code_file);

    fclose(code_file);


    while (My.ip < code_size_int)
    {
        switch(My.code[My.ip++])
        {
            case PUSH:
                StackPush(&My.stack, My.code[My.ip++]);
                break;
            case PUSHR:
                StackPush(&My.stack, My.reg[My.code[My.ip++] - 1]);
                break;
            case POP:
                StackPop(&My.stack, &My.reg[My.code[My.ip++] - 1]);
                break;
            case ADD:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                StackPush(&My.stack, My.value[0] + My.value[1]);
                break;
            case MUL:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                StackPush(&My.stack, My.value[0] * My.value[1]);
                break;
            case SUB:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                StackPush(&My.stack, My.value[1] - My.value[0]);
                break;
            case DIV:
                StackPop(&My.stack, &My.value[0]);
                if (My.value[0] == 0)
                {
                    fprintf(stderr, "RUNTIME ERROR: DIVISION BY ZERO");
                    exit(1);
                }
                StackPop(&My.stack, &My.value[1]);
                StackPush(&My.stack, My.value[1] / My.value[0]);
                break;
            case OUT:
                StackPop(&My.stack, &My.value[0]);
                printf("OUTPUT: %d\n", My.value[0]);
                break;
            case IN:
                printf("INPUT: ");
                scanf("%d", &My.value[0]);
                StackPush(&My.stack, My.value[0]);
                break;
            case JMP:
                My.ip = My.code[My.ip];
                break;
            case JA:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                if (My.value[1] > My.value[2])
                    My.ip = My.code[My.ip];
                break;
            case JAE:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                if (My.value[1] >= My.value[2])
                    My.ip = My.code[My.ip];
                break;
            case JB:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                if (My.value[1] < My.value[2])
                    My.ip = My.code[My.ip];
                break;
            case JBE:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                if (My.value[1] <= My.value[2])
                    My.ip = My.code[My.ip];
                break;
            case JE:
                StackPop(&My.stack, &My.value[0]);
                StackPop(&My.stack, &My.value[1]);
                if (My.value[1] == My.value[2])
                    My.ip = My.code[My.ip];
                break;
            case HLT:
                return 0;
            default:
                fprintf(stderr, "RUNTIME ERROR: INCORRECT COMMAND\n");
                exit(1);
        }
    }

    StackDtor(&My.stack);
    free(My.code);

    return 0;
}
