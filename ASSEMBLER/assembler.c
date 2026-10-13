#include "assembler.h"

int main( int argc, char** argv )
{
    FileInfo file = {};

    if (argc > 1 && strcmp(argv[1], "../") != 0)
        PrepareFileForSorting(argv[1], &file);
    else
    {
        fprintf(stderr, "ASSEMBLING ERROR: YOU NEED TO INPUT FILENAME\n");
        exit(1);
    }
    const char* output_filename = NULL;
    if (argc > 2 && strcmp(argv[2], "../") != 0)
        output_filename = argv[2];
    else
        output_filename = DEFAULT_OUTPUT_FILENAME;


    code_arr CodeArray = {};
    CodeArray.code = (int*)calloc(file.FileSize, sizeof(int));

    int labels[NUM_OF_LABELS] = {};
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
        labels[i] = -1;

    Assembler(file.Index, file.NumStrings, &CodeArray, labels);

    CodeArray.size = 0;

    Assembler(file.Index, file.NumStrings, &CodeArray, labels);

    /*
    for (size_t i = 0; i < CodeArray.size; i++)
        printf("[%zu] -> %d\n", i, CodeArray.code[i]);
    */

    FILE* output = fopen(output_filename, "wb");
    assert(output != NULL);
    fwrite(CodeArray.code, sizeof(int), CodeArray.size, output);

    fclose(output);
    free(CodeArray.code);

    return 0;
}

code_arr* Assembler( char** arr_of_asm_str, size_t num_of_str, code_arr* byte_code, int* labels )
{
    char command_name[10] = {};
    int command_arg = 0;
    int sscanf_rv = 0;

    for (size_t i = 0; i < num_of_str; i++)
    {
        if ((sscanf_rv = sscanf(arr_of_asm_str[i], "%s", command_name)) != EOF)
        {
            switch(command_name[0])
            {
                case ':':
                {
                    sscanf_rv = sscanf(arr_of_asm_str[i], "%*s %d", &command_arg);
                    if (sscanf_rv != 1 || command_arg >= NUM_OF_LABELS || command_arg < 0)
                    {
                        fprintf(stderr, "ASSEMBLING ERROR: INCORRECT LABEL\n");
                        exit(1);
                    }
                    labels[command_arg] = (int)byte_code->size;
                    break;
                }
                case 'P':
                {
                    if (strcmp(command_name, "PUSH") == 0)
                    {
                        byte_code->code[byte_code->size++] = PUSH;
                        sscanf(arr_of_asm_str[i], "%*s %d", &command_arg);
                        byte_code->code[byte_code->size++] = command_arg;
                    }
                    else if (strcmp(command_name, "PUSHR") == 0)
                    {
                        sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                        byte_code->code[byte_code->size++] = PUSHR;
                        switch(command_name[0])
                        {
                            case 'A':
                                byte_code->code[byte_code->size++] = A_X;
                                break;
                            case 'B':
                                byte_code->code[byte_code->size++] = B_X;
                                break;
                            case 'C':
                                byte_code->code[byte_code->size++] = C_X;
                                break;
                            case 'D':
                                byte_code->code[byte_code->size++] = D_X;
                                break;
                            default:
                                fprintf(stderr, "ASSEMBLING ERROR: INCORRECT REGISTER NAME\n");
                                exit(1);
                        }
                    }
                    else if (strcmp(command_name, "POP") == 0)
                    {
                        sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                        byte_code->code[byte_code->size++] = POP;
                        switch(command_name[0])
                        {
                            case 'A':
                                byte_code->code[byte_code->size++] = A_X;
                                break;
                            case 'B':
                                byte_code->code[byte_code->size++] = B_X;
                                break;
                            case 'C':
                                byte_code->code[byte_code->size++] = C_X;
                                break;
                            case 'D':
                                byte_code->code[byte_code->size++] = D_X;
                                break;
                            default:
                                fprintf(stderr, "ASSEMBLING ERROR: INCORRECT REGISTER NAME\n");
                                exit(1);
                        }
                    }
                    break;
                }
                case 'A':
                {
                    byte_code->code[byte_code->size++] = ADD;
                    break;
                }
                case 'M':
                {
                    byte_code->code[byte_code->size++] = MUL;
                    break;
                }
                case 'O':
                {
                    byte_code->code[byte_code->size++] = OUT;
                    break;
                }
                case 'D':
                {
                    byte_code->code[byte_code->size++] = DIV;
                    break;
                }
                case 'S':
                {
                    byte_code->code[byte_code->size++] = SUB;
                    break;
                }
                case 'H':
                {
                    byte_code->code[byte_code->size++] = HLT;
                    break;
                }
                case 'I':
                {
                    byte_code->code[byte_code->size++] = IN;
                    break;
                }
                case 'J':
                {
                    if (strcmp(command_name,  "JMP") == 0)
                        byte_code->code[byte_code->size++] = JMP;
                    else if (strcmp(command_name,  "JA") == 0)
                        byte_code->code[byte_code->size++] = JA;
                    else if (strcmp(command_name,  "JAE") == 0)
                        byte_code->code[byte_code->size++] = JAE;
                    else if (strcmp(command_name,  "JB") == 0)
                        byte_code->code[byte_code->size++] = JB;
                    else if (strcmp(command_name,  "JBE") == 0)
                        byte_code->code[byte_code->size++] = JBE;
                    else if (strcmp(command_name,  "JE") == 0)
                        byte_code->code[byte_code->size++] = JE;

                    sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                    if (command_name[0] == ':')
                    {
                        sscanf_rv = sscanf(arr_of_asm_str[i], "%*s %*s %d", &command_arg);
                        if (sscanf_rv != 1 || command_arg >= NUM_OF_LABELS || command_arg < 0)
                        {
                            fprintf(stderr, "ASSEMBLING ERROR: INCORRECT LABEL\n");
                            exit(1);
                        }
                        byte_code->code[byte_code->size++] = labels[command_arg];
                    }
                    else if (isdigit(command_name[0]))
                        byte_code->code[byte_code->size++] = atoi(command_name);
                    else
                    {
                        fprintf(stderr, "ASSEMBLING ERROR: INCORRECT LABEL\n");
                        exit(1);
                    }
                    break;
                }
                default:
                {
                    fprintf(stderr, "ASSEMBLING ERROR: INCORRECT COMMAND\n");
                    exit(1);
                }
            }
        }
    }
    return byte_code;
}

