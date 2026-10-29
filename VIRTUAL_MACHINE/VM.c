//#define LOGGING
//#define STEP_BY_STEP
#include "VM.h"
#include "raylib.h"



int main( int argc, char** argv )
{
ON_LOG(STARTLOG())
    const char* code_filename = NULL;
    command_f array_of_commands[] = {[HLT]=hlt, [PUSH]=push, [ADD]=add, [SUB]=sub, [MUL]=mul, [DIV]=div, [OUT]=out, [IN]=in,
                                     [POP]=pop, [JMP]=jmp, [JA]=ja, [JAE]=jae, [JB]=jb, [JBE]=jbe, [JE]=je, [JNE]=jne,
                                     [CALL]=call, [RET]=ret, [SQRT]=sqrt, [DRAW]=draw, [MSG]=msg};
    code_filename = SetFileName(argc, argv);

    spu_t spu = {};
    SPUCtor(&spu, code_filename);
    while (spu.ip < spu.num_of_instructions)
    {
        if (spu.code[spu.ip] == HLT)
        {
            ON_LOG(LOG("Find HLT on ip: %zu", spu.ip))
            hlt(&spu);
            ON_LOG(ENDLOG())
            return 0;
        }
        array_of_commands[(command_t)(spu.code[spu.ip++]) & CMD_MASK](&spu);
        #ifdef STEP_BY_STEP
            getchar();
            SPUDump(&spu);
        #endif

    }
    fprintf(stderr, "RUNTIME ERROR: SEGMENTATION FAULT\n");
    SPUDtor(&spu);
    ON_LOG(ENDLOG();)
    return 1;
}



void push( spu_t* spu )
{
    ON_LOG(LOG("Start push_cmd"))
    command_arg_t res = 0;
    size_t cmd_ip = spu->ip - 1;

    if (spu->code[cmd_ip] & REG_MASK)
    {
        res += spu->reg[spu->code[spu->ip++]];
        ON_LOG(LOG("reg arg: {%cX}", spu->code[spu->ip - 1] + 'A'))
    }
    if (spu->code[cmd_ip] & CNST_MASK)
    {
        stack_el_t value = 0;
        memcpy(&value, &spu->code[spu->ip], sizeof(command_arg_t));
        res += value;
        spu->ip += sizeof(command_arg_t);
        ON_LOG(LOG("const arg: {" OUTPUT_SPECIFIER "}", value))
    }
    if (spu->code[cmd_ip] & RAM_MASK)
    {
        ON_LOG(LOG("ram arg: RAM[%zu], value: {" OUTPUT_SPECIFIER "}",
                    (size_t)res, spu->RAM[(size_t)res]))
        res = spu->RAM[(size_t)res];
    }
    StackPush(&spu->stack, res);
    ON_LOG(LOG("Pushed {" OUTPUT_SPECIFIER "}", res))
}

void pop( spu_t* spu )
{
    command_arg_t* res_ptr = NULL;
    size_t cmd_ip = spu->ip - 1;

    if (!(spu->code[cmd_ip] & RAM_MASK))
    {
        if (spu->code[cmd_ip] & REG_MASK)
        {
            res_ptr = &spu->reg[spu->code[spu->ip++]];
            ON_LOG(LOG("Popped to %cX", spu->code[cmd_ip + 1] + 'A'))
        }
    }
    else
    {
        size_t ram_ip = 0;
        if (spu->code[cmd_ip] & REG_MASK)
            ram_ip += (size_t)spu->reg[spu->code[spu->ip++]];
        if (spu->code[cmd_ip] & CNST_MASK)
        {
            command_arg_t value = 0;
            memcpy(&value, &spu->code[spu->ip], sizeof(command_arg_t));
            ram_ip += (size_t)value;
            spu->ip += sizeof(command_arg_t);
        }
        res_ptr = &spu->RAM[ram_ip];
        ON_LOG(LOG("Popped to RAM[%zu]", ram_ip))
    }
    StackPop(&spu->stack, res_ptr);
}

void add( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[0] + spu->value[1]);
    ON_LOG(LOG("Added {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}",
                spu->value[0], spu->value[1], spu->value[0] + spu->value[1]))
}

void mul( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[0] * spu->value[1]);
    ON_LOG(LOG("Mul {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}",
                spu->value[0], spu->value[1], spu->value[0] * spu->value[1]))
}

void sub( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[1] - spu->value[0]);
    ON_LOG(LOG("Sub {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}",
                spu->value[1], spu->value[0], spu->value[1] - spu->value[0]))
}

void sqrt( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPush(&spu->stack, (stack_el_t)sqrt(spu->value[0]));
    ON_LOG(LOG("Sqrt {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}",
                spu->value[0], (stack_el_t)sqrt(spu->value[0])))

}

void div( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    if (CompareDoubles(spu->value[0], 0) == EQUAL)
    {
        fprintf(stderr, "RUNTIME ERROR: DIVISION BY ZERO");
        exit(1);
    }
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[1] / spu->value[0] );
    ON_LOG(LOG("Div {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}",
                spu->value[1], spu->value[0], spu->value[1] / spu->value[0]))
}



void draw( spu_t* spu )
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WIDTH, HEIGHT, "draw");

    Image MyImage = GenImageColor(WIDTH, HEIGHT, WHITE);
    MyImage.data = (Color*)spu->RAM;
    Texture MyTexture = LoadTextureFromImage(MyImage);

    UnloadImage(MyImage);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(MyTexture, 0, 0, WHITE);
        EndDrawing();
    }

    UnloadTexture(MyTexture);
    CloseWindow();

    ON_LOG(LOG("Drew RAM"))
}


void out( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    printf("OUTPUT: " OUTPUT_SPECIFIER "\n", spu->value[0]);
    ON_LOG(LOG("Out: {" OUTPUT_SPECIFIER "}", spu->value[0]))
}

void in( spu_t* spu )
{
    printf("INPUT: ");
    scanf(OUTPUT_SPECIFIER, &spu->value[0]);
    StackPush(&spu->stack, spu->value[0]);
    ON_LOG(LOG("In: {" OUTPUT_SPECIFIER "}", spu->value[0]))
}

void call( spu_t* spu )
{
    StackPush(&spu->ret_stack, (stack_el_t)(spu->ip + sizeof(int)));
    int label_address = 0;
    memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
    ON_LOG(LOG("Jumped(call) from ip: {%zu} to ip: {%d}", spu->ip, label_address))
    spu->ip = (size_t)label_address;
}

void ret( spu_t* spu )
{
    StackPop(&spu->ret_stack, &spu->value[0]);
    spu->ip = (size_t)spu->value[0];
    ON_LOG(LOG("Return to ip: {" OUTPUT_SPECIFIER "}", spu->value[0]))
}

void jmp( spu_t* spu )
{
    int label_address = 0;
    memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
    ON_LOG(LOG("Jumped from ip: {%zu} to ip: {%d}", spu->ip, label_address))
    spu->ip = (size_t)label_address;
}

void jae( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == LESS ||
        CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(jae) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], ">=", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void je( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(je) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], "==", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void ja( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == LESS)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(je) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], ">", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void jbe( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == MORE || CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(jbe) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], "<=", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void jb( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == MORE)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(jb) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], "<", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void jne( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) != EQUAL)
    {
        int label_address = 0;
        memcpy(&label_address, &spu->code[spu->ip], sizeof(int));
        ON_LOG(LOG("Jumped(jne) from ip: {%zu} to ip: {%d}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}",
                    spu->ip, label_address, spu->value[1], "!=", spu->value[0]))
        spu->ip = (size_t)label_address;
    }
    else
        spu->ip += sizeof(int);
}

void hlt( spu_t* spu )
{
    SPUDtor(spu);
    ON_LOG(LOG("Halt"))
}

void msg( spu_t* spu )
{
    command_arg_t arg = 0;
    memcpy(&arg, &spu->code[spu->ip], sizeof(command_arg_t));
    spu->ip += sizeof(command_arg_t);
    printf("MESSAGE: %s\n", messages[(size_t)(arg - 1)]);
    ON_LOG(LOG("Msg: {%s}", messages[(size_t)(arg - 1)]))
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

    spu->num_of_instructions = (size_t)(input_file.st_size - sizeof(file_header));

    spu->code = (uint8_t*)calloc(spu->num_of_instructions, sizeof(uint8_t));
    spu->RAM = (ram_t*)calloc(RAM_SIZE, sizeof(ram_t));
    FILE* code_file = fopen(input_filename, "rb");
    assert(code_file != NULL);

    file_header head = {};
    fread(&head, sizeof(file_header), 1, code_file);
    fread(spu->code, sizeof(uint8_t), spu->num_of_instructions, code_file);
    CheckHead(&head, spu->code, spu->num_of_instructions);

    fclose(code_file);
}

void CheckHead( file_header* head, uint8_t* code_array, size_t code_size )
{
    for (size_t i = 0; i < 4; i++)
    {
        if (head->SIGN[i] != VM_header.SIGN[i])
        {
            printf("Incorrect sign, cant work with this file\n");
            exit(1);
        }
    }
    uint64_t hash = Hash_djb2(code_array, code_size);
    if (hash != head->hash)
    {
        printf("Incorrect hash, file can be corrupted. Cant work with this file\n");
        exit(1);
    }
    if (head->major != VM_header.major || (head->major == VM_header.major && head->minor >  VM_header.minor))
    {
        printf("Incorrect version, cant work with this file\n"
               "file version: %u.%u\n"
                "spu version: %u.%u\n",
                head->major, head->minor, VM_header.major, VM_header.minor);
        exit(1);
    }

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

int CompareDoubles( const double a, const double b )
{
    if (isnan(a) == 1 || isnan(b) == 1)
        return UNDEF;
    if ((a - b) > EPS)
        return MORE;
    else if ((a - b) < -EPS)
        return LESS;
    else if ((a - b) < EPS)
        return EQUAL;
    return UNDEF;
}

uint64_t Hash_djb2( const void* beg, size_t len )
{
    uint64_t hash = 5381;

    for (const uint8_t* i = (const uint8_t*)beg; i <= (const uint8_t*)beg + len; i++)
        hash =  (hash << 5) + hash + *i;
    return hash;
}


