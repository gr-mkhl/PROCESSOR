#include "VM.h"


int main( int argc, char** argv )
{
ON_LOG(STARTLOG())
    const char* code_filename = NULL;
    command_f array_of_commands[] = {hlt, push, add, sub, mul, div, out, in,
                                     pop, jmp, ja, jae, jb, jbe, je, jne,
                                     call, ret, sqrt, draw, msg};
    code_filename = SetFileName(argc, argv);

    spu_t spu = {};
    SPUCtor(&spu, code_filename);

    while (spu.ip < spu.num_of_instructions)
    {
        if (CompareDoubles(spu.code[spu.ip], HLT) == EQUAL)
        {
ON_LOG(LOG("Find HLT on ip: %zu", spu.ip))
            hlt(&spu);
ON_LOG(ENDLOG())
            return 0;
        }
        array_of_commands[(int)(spu.code[spu.ip++]) & CMD_MASK](&spu);
    }
    fprintf(stderr, "RUNTIME ERROR: SEGMENTATION FAULT\n");
    SPUDtor(&spu);
ON_LOG(ENDLOG();)
    return 1;
}



void push( spu_t* spu )
{
ON_LOG(LOG("Start push_cmd"))
    stack_el_t res = 0;
    size_t cmd_ip = spu->ip - 1;

    if ((int)spu->code[cmd_ip] & CNST_MASK)
    {
        res += spu->code[spu->ip++];
ON_LOG(LOG("const arg: {" OUTPUT_SPECIFIER "}", spu->code[spu->ip - 1]))
    }
    if ((int)spu->code[cmd_ip] & REG_MASK)
    {
        res += spu->reg[((int)spu->code[cmd_ip] & REG_NUM) >> 8];
ON_LOG(LOG("reg arg: {%d}", ((int)spu->code[cmd_ip] & REG_NUM) >> 8))
ON_LOG(LOG("reg arg: {%c%c}", (((int)(spu->code[cmd_ip]) & REG_NUM) >> 8) + 'A', 'X'))
    }
    if ((int)spu->code[cmd_ip] & RAM_MASK)
    {
ON_LOG(LOG("ram arg: RAM[%zu], value: {" OUTPUT_SPECIFIER "}", (size_t)res, spu->RAM[(size_t)res]))
        res = spu->RAM[(size_t)res];
    }
    StackPush(&spu->stack, res);
ON_LOG(LOG("Pushed {" OUTPUT_SPECIFIER "}", res))
}

void pop( spu_t* spu )
{
    stack_el_t* res_ptr = NULL;
    size_t cmd_ip = spu->ip - 1;

    if (!((int)spu->code[cmd_ip] & RAM_MASK))
    {
        if ((int)spu->code[cmd_ip] & REG_MASK)
            res_ptr = &spu->reg[((int)spu->code[cmd_ip] & REG_NUM) >> 8];
    }
    else
    {
        size_t ram_ip = 0;
        if ((int)spu->code[cmd_ip] & REG_MASK)
            ram_ip += (size_t)spu->reg[((int)spu->code[cmd_ip] & REG_NUM) >> 8];
        if ((int)spu->code[cmd_ip] & CNST_MASK)
            ram_ip += (size_t)spu->code[spu->ip++];
        res_ptr = &spu->RAM[ram_ip];
    }
    StackPop(&spu->stack, res_ptr);
}

void add( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[0] + spu->value[1]);
ON_LOG(LOG("Added {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}", spu->value[0], spu->value[1], spu->value[0] + spu->value[1]))
}

void mul( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[0] * spu->value[1]);
ON_LOG(LOG("Mul {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}", spu->value[0], spu->value[1], spu->value[0] * spu->value[1]))
}

void sub( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    StackPush(&spu->stack, spu->value[1] - spu->value[0]);
ON_LOG(LOG("Sub {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}", spu->value[1], spu->value[0], spu->value[1] - spu->value[0]))
}

void sqrt( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPush(&spu->stack, (stack_el_t)sqrt(spu->value[0]));
ON_LOG(LOG("Sqrt {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}", spu->value[0], (stack_el_t)sqrt(spu->value[0])))

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
ON_LOG(LOG("Div {" OUTPUT_SPECIFIER "} with {" OUTPUT_SPECIFIER "}, result: {" OUTPUT_SPECIFIER "}", spu->value[1], spu->value[0], spu->value[1] / spu->value[0]))
}

void draw( spu_t* spu )
{
    for (size_t i = 0; i < (size_t)sqrt(VRAM_SIZE); i++)
    {
        for (size_t j = 0; j < (size_t)sqrt(VRAM_SIZE); j++)
        {
            //printf("[" OUTPUT_SPECIFIER "]", spu->RAM[i * (size_t)sqrt(RAM_SIZE) + j]);

            if (CompareDoubles(spu->RAM[i * (size_t)sqrt(VRAM_SIZE) + j], 0) == EQUAL)
                printf("[.]");
            else
                printf("[*]");

        }
        printf("\n");
    }
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
    StackPush(&spu->ret_stack, (stack_el_t)(spu->ip + 1));
ON_LOG(LOG("Jumped(call) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip]))
    spu->ip = (size_t)spu->code[spu->ip];
}

void ret( spu_t* spu )
{
    StackPop(&spu->ret_stack, &spu->value[0]);
    spu->ip = (size_t)spu->value[0];
ON_LOG(LOG("Return to ip: {" OUTPUT_SPECIFIER "}", spu->value[0]))
}

void jmp( spu_t* spu )
{
ON_LOG(LOG("Jumped from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip]))
    spu->ip = (size_t)spu->code[spu->ip];
}

void jae( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == LESS || CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(jae) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], ">=", spu->value[0]))
    }

    else
        spu->ip++;
}

void je( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(je) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], "==", spu->value[0]))
    }
    else
        spu->ip++;
}

void ja( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == LESS)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(ja) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], ">", spu->value[0]))
    }
    else
        spu->ip++;
}

void jbe( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == MORE || CompareDoubles(spu->value[0], spu->value[1]) == EQUAL)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(jbe) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], "<=", spu->value[0]))
    }
    else
        spu->ip++;
}

void jb( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) == MORE)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(jb) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], "<", spu->value[0]))

    }
    else
        spu->ip++;
}

void jne( spu_t* spu )
{
    StackPop(&spu->stack, &spu->value[0]);
    StackPop(&spu->stack, &spu->value[1]);
    if (CompareDoubles(spu->value[0], spu->value[1]) != EQUAL)
    {
        spu->ip = (size_t)spu->code[spu->ip];
ON_LOG(LOG("Jumped(jne) from ip: {%zu} to ip: {" OUTPUT_SPECIFIER "}, because {" OUTPUT_SPECIFIER "} %s {" OUTPUT_SPECIFIER "}", spu->ip, spu->code[spu->ip], spu->value[1], "!=", spu->value[0]))

    }
    else
        spu->ip++;
}

void hlt( spu_t* spu )
{
    SPUDtor(spu);
ON_LOG(LOG("Halt"))
}

void msg( spu_t* spu )
{
    printf("MESSAGE: %s\n", messages[(size_t)(spu->code[spu->ip++]) - 1]);
ON_LOG(LOG("Msg: {%s}", messages[(size_t)(spu->code[spu->ip - 1]) - 1]))

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


