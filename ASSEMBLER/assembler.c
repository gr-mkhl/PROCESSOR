#define LOGGING
#include "assembler.h"

int main( int argc, char** argv )
{
ON_LOG(STARTLOG())
    asm_t AsmData = {};

    PreAssembling(argc, argv, &AsmData);

    Preprocessing(&AsmData);
    DoAssembling(&AsmData);

    PostAssembling(argc, argv, &AsmData);
ON_LOG(ENDLOG())
    return 0;
}

bool Assembler( asm_t* asm_data )
{
    char command_name[20] = {};
    stack_el_t command_arg = 0;
    bool need_second_asm = false;

    for (size_t i = 0; i < asm_data->num_of_lines; i++)
    {
        if (sscanf(asm_data->array_of_lines[i], "%s", command_name) != EOF)
        {
            switch(command_name[0])
            {
                case '\n':
                    break;
                case ';':
                    break;
                case '$':
                    break;
                case ':':
                {
                    if (asm_data->second_asm == false)
                    {
                        sscanf(asm_data->array_of_lines[i], "%*s %s", command_name);
                        SetLabel(command_name, (int)asm_data->bytecode.size, asm_data->labels, &asm_data->num_of_labels);
                    }
                    break;
                }
                case 'P':
                {
                    if (strcmp(command_name, "PUSH") == 0)
                    {
ON_LOG(LOG("Push:"))
                        ParsingArg(asm_data->array_of_lines[i], PUSH, asm_data);
                    }
                    else if (strcmp(command_name, "POP") == 0)
                    {
ON_LOG(LOG("Pop:"))
                        ParsingArg(asm_data->array_of_lines[i], POP, asm_data);
                    }
                    break;
                }
                case 'C':
                {
                    if (strcmp(command_name, "CALL") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)CALL;
                        int ret_value = ParsingLabelArg(asm_data->array_of_lines[i], asm_data);
                        if (ret_value == 0)
                            need_second_asm = true;
                        else if (ret_value == -1)
                        {
                            fprintf(stderr, "ASSEMBLING ERROR: INCORRECT ARGUMENT FOR CALL IN LINE:%zu\n", i);
                            exit(1);
                        }
                    }
                    break;
                }
                case 'M':
                {
                    if (strcmp(command_name, "MSG") == 0)
                    {
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)MSG;
                        sscanf(asm_data->array_of_lines[i], "%*s" OUTPUT_SPECIFIER, &command_arg);
                        memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &command_arg, sizeof(stack_el_t));
                        asm_data->bytecode.size += sizeof(stack_el_t);
                    }
                    else if (strcmp(command_name, "MUL") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)MUL;
                    break;
                }
                case 'O':
                {
                    if (strcmp(command_name,  "OUT") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)OUT;
                    break;
                }
                case 'A':
                {
                    if (strcmp(command_name,  "ADD") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)ADD;
                    break;
                }
                case 'R':
                {
                    if (strcmp(command_name,  "RET") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)RET;
                    break;
                }
                case 'D':
                {
                    if (strcmp(command_name, "DIV") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)DIV;
                    else if (strcmp(command_name, "DRAW") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)DRAW;
                    break;
                }
                case 'S':
                {
                    if (strcmp(command_name, "SUB") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)SUB;
                    else if (strcmp(command_name, "SQRT") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)SQRT;
                    break;
                }
                case 'H':
                {
                    if (strcmp(command_name,  "HLT") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)HLT;
                    break;
                }
                case 'I':
                {
                    if (strcmp(command_name,  "IN") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)IN;
                    break;
                }
                case 'J':
                {
                    if (strcmp(command_name,  "JMP") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JMP;
                    else if (strcmp(command_name,  "JA") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JA;
                    else if (strcmp(command_name,  "JAE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JAE;
                    else if (strcmp(command_name,  "JB") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JB;
                    else if (strcmp(command_name,  "JBE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JBE;
                    else if (strcmp(command_name,  "JE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JE;
                    else if (strcmp(command_name,  "JNE") == 0)
                        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)JNE;

                    int ret_value = ParsingLabelArg(asm_data->array_of_lines[i], asm_data);
                    if (ret_value == 0)
                        need_second_asm = true;
                    else if (ret_value == -1)
                    {
                        fprintf(stderr, "ASSEMBLING ERROR: INCORRECT ARGUMENT FOR JMP IN LINE:%zu\n", i);
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
    return need_second_asm;
}

void Preprocessing( asm_t* asm_data )
{
    for (size_t i = 0; i < asm_data->num_of_lines; i++)
    {
        if (asm_data->array_of_lines[i][0] == '$')
        {
            if (asm_data->num_of_macros == MAX_NUM_OF_MACROS)
            {
                fprintf(stderr, "PREPROCESSING ERROR: TOO MANY MACROS, YOU CANT SET MORE");
                exit(1);
            }
ON_LOG(LOG("Set new macro, line:%zu", i))
            char new_macro[20] = {};
            sscanf(asm_data->array_of_lines[i], "$%19s", new_macro);
ON_LOG(LOG("macro_name: %s", new_macro))
            uint64_t new_macro_hash = Hash_djb2(new_macro, strlen(new_macro));
            asm_data->macros[asm_data->num_of_macros].macro_hash = new_macro_hash;
ON_LOG(LOG("macro_hash: 0x%llX", new_macro_hash))
            sscanf(asm_data->array_of_lines[i], "%*s %19[^\n]", new_macro);
            char* new_macro_value = (char*)calloc(strlen("COMMAND ") + strlen(new_macro) + 1, sizeof(char));
            strcpy(new_macro_value, "COMMAND ");
            strcat(new_macro_value, new_macro);
ON_LOG(LOG("macro_value: %s", new_macro_value))
            asm_data->macros[asm_data->num_of_macros++].macro_value = new_macro_value;
        }
    }
    #ifdef LOGGING
    LOG("Num of macros: %zu", asm_data->num_of_macros);
    LOG("__macros_array before qsort()__");
    for (size_t i = 0; i < asm_data->num_of_macros; i++)
        LOG("macro[%zu]: value: %s hash: 0x%llX", i, asm_data->macros[i].macro_value, asm_data->macros[i].macro_hash);
    #endif

    qsort(asm_data->macros, asm_data->num_of_macros, sizeof(macro_t), CompareMacrosHashUp);

    #ifdef LOGGING
    LOG("__macros_array after qsort()__");
        for (size_t i = 0; i < asm_data->num_of_macros; i++)
            LOG("macro[%zu]: value: %s hash: 0x%llX", i, asm_data->macros[i].macro_value, asm_data->macros[i].macro_hash);
    #endif

}

int ParsingLabelArg( const char* arg, asm_t* asm_data )
{
    char command_name[20] = {};
    sscanf(arg, "%*s %s", command_name);
    if (command_name[0] == ':')
    {

        sscanf(arg, "%*s %*s %s", command_name);

        int address = FindLabel(command_name, asm_data->labels, asm_data->num_of_labels);
        if (address == -1)
        {
            asm_data->bytecode.size += sizeof(int);
            return 0;
        }
        else
        {
            memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &address, sizeof(int));
            asm_data->bytecode.size += sizeof(int);
            return 1;
        }
    }
    return -1;
}

char* FindDefinition( char* macro_name, asm_t* asm_data )
{
ON_LOG(LOG("macro_name: %s", macro_name))
    uint64_t new_macro_hash = Hash_djb2(macro_name, strlen(macro_name));

    macro_t need_to_find = {};
    need_to_find.macro_hash = new_macro_hash;

    macro_t* result = (macro_t*)bsearch(&need_to_find, asm_data->macros, asm_data->num_of_macros, sizeof(macro_t), CompareMacrosHashUp);
    if (result != NULL)
    {
ON_LOG(LOG("Find macro: %s, value: %s, hash: 0x%llX", macro_name, result->macro_value, result->macro_hash))
        return result->macro_value;
    }
ON_LOG(LOG("Didn't find macro: %s", macro_name))
    return NULL;
}


void ParsingArg( char* arg, commands comm, asm_t* asm_data )
{
ON_LOG(LOG("arg: %s", arg))

    stack_el_t command_arg = 0;
    char command_arg_str[10] = {};

    if (sscanf(arg, "%*s $%s", command_arg_str) == 1)
    {
        arg = FindDefinition(command_arg_str, asm_data);
        if (arg == NULL)
        {
            fprintf(stderr, "ASSEMBLING ERROR: NO DEFINITION OF MACRO $%s", command_arg_str);
            exit(1);
        }
    }
    if ((sscanf(arg, "%*s [%[A-H]X + " OUTPUT_SPECIFIER "]", command_arg_str, &command_arg) == 2) ||
        (sscanf(arg, "%*s [" OUTPUT_SPECIFIER " + %[A-H]X]", &command_arg, command_arg_str) == 2))
    {
        uint8_t reg_num = (uint8_t)(command_arg_str[0] - 'A');
ON_LOG(LOG("reg_num: 0x%X", reg_num))
ON_LOG(LOG("const_arg: {" OUTPUT_SPECIFIER "}", command_arg))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)(comm | RAM_MASK | REG_MASK | CNST_MASK);
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = reg_num;
        memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &command_arg, sizeof(stack_el_t));
        asm_data->bytecode.size += sizeof(stack_el_t);
    }
    else if (comm == PUSH && ((sscanf(arg, "%*s %[A-H]X + " OUTPUT_SPECIFIER, command_arg_str, &command_arg) == 2) ||
        (sscanf(arg, "%*s" OUTPUT_SPECIFIER " + %[A-H]X", &command_arg, command_arg_str) == 2)))
    {
        uint8_t reg_num = (uint8_t)(command_arg_str[0] - 'A');
ON_LOG(LOG("reg_num: 0x%X", reg_num))
ON_LOG(LOG("const_arg: {" OUTPUT_SPECIFIER "}", command_arg))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)(comm | REG_MASK | CNST_MASK);
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = reg_num;
        memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &command_arg, sizeof(stack_el_t));
        asm_data->bytecode.size += sizeof(stack_el_t);
        return;
    }
    else if (sscanf(arg, "%*s [%[A-H]X]", command_arg_str) == 1)
    {
        uint8_t reg_num = (uint8_t)(command_arg_str[0] - 'A');
ON_LOG(LOG("reg_num: 0x%X", reg_num))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = uint8_t(comm | RAM_MASK | REG_MASK);
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = reg_num;
        return;
    }
    else if (sscanf(arg, "%*s [" OUTPUT_SPECIFIER "]", &command_arg) == 1)
    {
ON_LOG(LOG("const_arg: {" OUTPUT_SPECIFIER "}", command_arg))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)(comm | RAM_MASK | CNST_MASK);
        memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &command_arg, sizeof(stack_el_t));
        asm_data->bytecode.size += sizeof(stack_el_t);
        return;
    }
    else if (sscanf(arg, "%*s %[A-H]X", command_arg_str) == 1)
    {
        uint8_t reg_num = (uint8_t)(command_arg_str[0] - 'A');
ON_LOG(LOG("reg_num: 0x%X", reg_num))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)(comm | REG_MASK);
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = reg_num;
        return;
    }
    else if (comm == PUSH && sscanf(arg, "%*s" OUTPUT_SPECIFIER, &command_arg) == 1)
    {
ON_LOG(LOG("const_arg: {" OUTPUT_SPECIFIER "}", command_arg))
        asm_data->bytecode.code_array[asm_data->bytecode.size++] = (uint8_t)(comm | CNST_MASK);
        memcpy(&asm_data->bytecode.code_array[asm_data->bytecode.size], &command_arg, sizeof(stack_el_t));
        asm_data->bytecode.size += sizeof(stack_el_t);
        return;
    }
    else
    {
        ON_LOG(LOG("INCORRECT_ARG"))
        return;
    }
}


void PreAssembling( int argc, char** argv, asm_t* asm_data )
{
    FileInfo file = {};

    if (argc > 1 && strcmp(argv[1], "../ASM_CODES/") != 0)
        PrepareFileForSorting(argv[1], &file);
    else
    {
        fprintf(stderr, "ASSEMBLING ERROR: YOU NEED TO INPUT FILENAME\n");
        exit(1);
    }

    asm_data->array_of_lines = file.Index;
    asm_data->buffer_ptr = file.Buffer;
    asm_data->num_of_lines = file.NumStrings;
    asm_data->bytecode.code_array = (uint8_t*)calloc(20 * file.NumStrings, sizeof(uint8_t));
    asm_data->labels = (label_t*)calloc(MAX_NUM_OF_LABELS, sizeof(label_t));
    asm_data->macros = (macro_t*)calloc(MAX_NUM_OF_MACROS, sizeof(macro_t));
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

    fwrite(asm_data->bytecode.code_array, sizeof(uint8_t), asm_data->bytecode.size, output);

    fclose(output);

    free(asm_data->buffer_ptr);
    free(asm_data->array_of_lines);
    free(asm_data->labels);
    free(asm_data->macros);
    free(asm_data->bytecode.code_array);
}


void DoAssembling( asm_t* asm_data )
{

ON_LOG(LOG("First assembling"))
    if (Assembler(asm_data) == true)
    {
        #ifdef LOGGING
        LOG("Num of labels: %zu", asm_data->num_of_labels);
        LOG("__labels_array before qsort()__");
        for (size_t i = 0; i < asm_data->num_of_labels; i++)
            LOG("label[%zu]: address: %d hash: 0x%llX", i, asm_data->labels[i].label_address, asm_data->labels[i].label_hash);
        #endif

        qsort(asm_data->labels, asm_data->num_of_labels, sizeof(label_t), CompareLabelsHashUp);



    #ifdef LOGGING
    LOG("__labels_array after qsort()__");
    for (size_t i = 0; i < asm_data->num_of_labels; i++)
        LOG("label[%zu]: address: %d hash: 0x%llX", i, asm_data->labels[i].label_address, asm_data->labels[i].label_hash);
    #endif
ON_LOG(LOG("Second assembling"))

    asm_data->bytecode.size = 0;
    asm_data->second_asm = true;
    Assembler(asm_data);
    }
}

int CompareLabelsHashUp( const void* first, const void* second )
{
    uint64_t hash1 = ((const label_t*)first)->label_hash;
    uint64_t hash2 = ((const label_t*)second)->label_hash;

    if (hash1 > hash2)
        return 1;
    else if (hash1 == hash2)
        return 0;
    return -1;
}

int CompareMacrosHashUp( const void* first, const void* second )
{
    uint64_t hash1 = ((const macro_t*)first)->macro_hash;
    uint64_t hash2 = ((const macro_t*)second)->macro_hash;

    if (hash1 > hash2)
        return 1;
    else if (hash1 == hash2)
        return 0;
    return -1;
}

int FindLabel( const char* name, label_t* labels_arr, size_t num_of_labels )
{
    uint64_t new_label_hash = Hash_djb2(name, strlen(name));
    label_t need_to_find = {};
    need_to_find.label_hash = new_label_hash;

    label_t* result = (label_t*)bsearch(&need_to_find, labels_arr, num_of_labels, sizeof(label_t), CompareLabelsHashUp);
    if (result != NULL)
    {
ON_LOG(LOG("Find label: %s, address:%d hash: %llX", name, result->label_address, result->label_hash))
        return result->label_address;
    }
ON_LOG(LOG("Didn't find label: %s", name))
    return -1;
}



int SetLabel( const char* name, int address, label_t* labels_arr, size_t* num_of_labels )
{
    if (*num_of_labels == MAX_NUM_OF_LABELS)
    {
        fprintf(stderr, "ASSEMBLING ERROR: TOO MANY LABELS, YOU CANT SET MORE");
        exit(1);
    }
    uint64_t newlabel_hash = Hash_djb2(name, strlen(name));

    labels_arr[*num_of_labels].label_hash = newlabel_hash;
    labels_arr[*num_of_labels].label_address = address;
    (*num_of_labels)++;
ON_LOG(LOG("Set label: %s, address: %d, hash: 0x%llX", name, address, newlabel_hash))
    return 1;
}

uint64_t Hash_djb2( const void* beg, size_t len )
{
    uint64_t hash = 5381;

    for (const uint8_t* i = (const uint8_t*)beg; i <= (const uint8_t*)beg + len; i++)
    {
        hash =  (hash << 5) + hash + *i;
    }
    return hash;
}
