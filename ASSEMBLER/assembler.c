#include "assembler.h"



int main( int argc, char** argv )
{
    asm_t AsmData = {};

    PreAssembling(argc, argv, &AsmData);

    DoubleAssembling(&AsmData);

    PostAssembling(argc, argv, &AsmData);

    return 0;
}



void Assembler( asm_t* asm_data )
{
    char command_name[10] = {};
    int command_arg = 0;
    int sscanf_rv = 0;

    for (size_t i = 0; i < asm_data->num_of_lines; i++)
    {
        if ((sscanf_rv = sscanf(asm_data->array_of_lines[i], "%s", command_name)) != EOF)
        {
            switch(command_name[0])
            {
                case '\n':
                    break;
                case ';':
                    break;
                case ':':
                {
                    sscanf_rv = sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                    SetLabel(command_name, (int)asm_data->bytecode.size, asm_data->labels);
                    break;
                }
                case 'P':
                {
                    if (strcmp(command_name, "PUSH") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = PUSH;
                        sscanf(asm_data->array_of_lines[i], "%*s %d", &command_arg);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = command_arg;
                    }
                    else if (strcmp(command_name, "PUSHR") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = PUSHR;
                        sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                        switch(command_name[0])
                        {
                            case 'A':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = AX;
                                break;
                            case 'B':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = BX;
                                break;
                            case 'C':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = CX;
                                break;
                            case 'D':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = DX;
                                break;
                            default:
                                fprintf(stderr, "ASSEMBLING ERROR: INCORRECT REGISTER NAME\n");
                                exit(1);
                        }
                    }
                    else if (strcmp(command_name, "PUSHM") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = PUSHM;
                        sscanf(asm_data->array_of_lines[i], "%*s [%d]", &command_arg);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = command_arg;
                    }
                    else if (strcmp(command_name, "POPM") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = POPM;
                        sscanf(asm_data->array_of_lines[i], "%*s [%d]", &command_arg);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = command_arg;
                    }
                    else if (strcmp(command_name, "POP") == 0)
                    {
                        sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = POP;
                        switch(command_name[0])
                        {
                            case 'A':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = AX;
                                break;
                            case 'B':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = BX;
                                break;
                            case 'C':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = CX;
                                break;
                            case 'D':
                                asm_data->bytecode.code_array[asm_data->bytecode.size++] = DX;
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
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = CALL;
                        sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                        if (command_name[0] == ':')
                        {
                            sscanf_rv = sscanf(asm_data->array_of_lines[i], "%*s %*s %s", command_name);

                            int address = FindLabel(command_name, asm_data->labels);
                            asm_data->bytecode.code_array[asm_data->bytecode.size++] = address;
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
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = OUT;
                    break;
                }
                case 'A':
                {
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = ADD;
                    break;
                }
                case 'M':
                {
                    if (strcmp(command_name, "MSG") == 0) //TODO - это сложно
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = MSG;
                        sscanf(asm_data->array_of_lines[i], "%*s %d", &command_arg);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = command_arg;

                    }
                    else if (strcmp(command_name, "MUL") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = MUL;
                    }
                    break;
                }
                case 'R':
                {
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = RET;
                    break;
                }
                case 'D':
                {
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = DIV;
                    break;
                }
                case 'S':
                {
                    if (strcmp(command_name, "SUB") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = SUB;
                    else if (strcmp(command_name, "SQRT") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = SQRT;
                    break;
                }
                case 'H':
                {
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = HLT;
                    break;
                }
                case 'I':
                {
                    asm_data->bytecode.code_array[asm_data->bytecode.size++] = IN;
                    break;
                }
                case 'J':
                {
                    if (strcmp(command_name,  "JMP") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JMP;
                    else if (strcmp(command_name,  "JA") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JA;
                    else if (strcmp(command_name,  "JAE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JAE;
                    else if (strcmp(command_name,  "JB") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JB;
                    else if (strcmp(command_name,  "JBE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JBE;
                    else if (strcmp(command_name,  "JE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JE;
                    else if (strcmp(command_name,  "JNE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = JNE;

                    sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                    if (command_name[0] == ':')
                    {
                        sscanf_rv = sscanf(asm_data->array_of_lines[i], "%*s %*s %s", command_name);

                        int address = FindLabel(command_name, asm_data->labels);
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = address;
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
}

void PreAssembling( int argc, char** argv, asm_t* asm_data )
{
    FileInfo file = {};

    if (argc > 1 && strcmp(argv[1], "../") != 0)
        PrepareFileForSorting(argv[1], &file);
    else
    {
        fprintf(stderr, "ASSEMBLING ERROR: YOU NEED TO INPUT FILENAME\n");
        exit(1);
    }

    asm_data->array_of_lines = file.Index;
    asm_data->buffer_ptr = file.Buffer;
    asm_data->num_of_lines = file.NumStrings;
    asm_data->bytecode.code_array = (int*)calloc(3 * file.NumStrings, sizeof(int));
    asm_data->labels = (label_t*)calloc(NUM_OF_LABELS, sizeof(label_t));
    for (size_t i = 0; i < NUM_OF_LABELS; i++)
    {
        asm_data->labels[i].label_address = -1;
    }
}

void PostAssembling( int argc, char** argv, asm_t* asm_data )
{
    const char* output_filename = NULL;
    if (argc > 2 && strcmp(argv[2], "../") != 0)
        output_filename = argv[2];
    else
        output_filename = DEFAULT_OUTPUT_FILENAME;

    FILE* output = fopen(output_filename, "wb");
    assert(output != NULL);

    fwrite(asm_data->bytecode.code_array, sizeof(int), asm_data->bytecode.size, output);

    fclose(output);

    free(asm_data->buffer_ptr);
    free(asm_data->array_of_lines);
    free(asm_data->labels);
    free(asm_data->bytecode.code_array);
}

void DoubleAssembling( asm_t* asm_data )
{
    Assembler(asm_data);
    asm_data->bytecode.size = 0;
    Assembler(asm_data);
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
