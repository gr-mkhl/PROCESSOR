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

    /*
    int labels[NUM_OF_LABELS] = {};
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
        labels[i] = -1;
    */



    label_t labels[NUM_OF_LABELS] = {};
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
        labels[i].label_address = -1;


    Assembler(file.Index, file.NumStrings, &CodeArray, labels);

    CodeArray.size = 0;

    Assembler(file.Index, file.NumStrings, &CodeArray, labels);


    FILE* output = fopen(output_filename, "wb");
    assert(output != NULL);
    fwrite(CodeArray.code, sizeof(int), CodeArray.size, output);

    fclose(output);
    free(CodeArray.code);

    return 0;
}

code_arr* Assembler( char** arr_of_asm_str, size_t num_of_str, code_arr* byte_code, label_t* labels )
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
                    sscanf_rv = sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                    SetLabel(command_name, (int)byte_code->size, labels);
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
                case 'C':
                {
                    if (strcmp(command_name, "CALL") == 0)
                    {
                        byte_code->code[byte_code->size++] = CALL;
                        sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                        if (command_name[0] == ':')
                        {
                            sscanf_rv = sscanf(arr_of_asm_str[i], "%*s %*s %s", command_name);

                            int address = FindLabel(command_name, labels);
                            byte_code->code[byte_code->size++] = address;
                        }
                        else
                        {
                            fprintf(stderr, "ASSEMBLING ERROR: INCORRECT LABEL\n");
                            exit(1);
                        }
                    }
                    break;
                }
                case 'O':
                {
                    byte_code->code[byte_code->size++] = OUT;
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
                case 'R':
                {
                    byte_code->code[byte_code->size++] = RET;
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
                    else if (strcmp(command_name,  "JNE") == 0)
                        byte_code->code[byte_code->size++] = JNE;

                    sscanf(arr_of_asm_str[i], "%*s %s", command_name);
                    if (command_name[0] == ':')
                    {
                        sscanf_rv = sscanf(arr_of_asm_str[i], "%*s %*s %s", command_name);

                        int address = FindLabel(command_name, labels);
                        byte_code->code[byte_code->size++] = address;
                    }
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

int FindLabel( const char* name, label_t* labels_arr )
{
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
    {
        if (strcmp(labels_arr[i].label_name, name) == 0)
            return labels_arr[i].label_address;
    }

    return -1;
}

int SetLabel( const char* name, int address, label_t* labels_arr )
{
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
    {
        if (strcmp(labels_arr[i].label_name, "") == 0)
        {
            strcpy(labels_arr[i].label_name, name);
            labels_arr[i].label_address = address;
            return 1;
        }
    }

    return 0;
}
