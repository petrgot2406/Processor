#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "Stack.h"
#include "ReadFromFile.h"

Error_processor PushCodeToArray(SPU* spu, File_t code);
Error_processor Processor(SPU* spu, File_t code);

int main()
{
    struct SPU spu = {};

    InitStack(&spu.stack);

    struct File_t code = {};

    code.file_name = "code_array.asm";

    Put_file_to_structure(&code);

    spu.arrcode = (int*)calloc(code.str_num, sizeof(int));

    PushCodeToArray(&spu, code);

    Error_processor err_proc = Processor(&spu, code);

    if (err_proc != PROCESSED_OK)
    {
        return err_proc;
    }

    return 0;
}

Error_processor PushCodeToArray(SPU* spu, File_t code)
{
    for (size_t i = 0; i < code.str_num; i++)
    {
        char* word = (char*)calloc(code.lineslen[i], sizeof(char));

        for (size_t j = 0; j < code.lineslen[i] + 1; j++)
        {
            word[j] = code.lines[i][j];
        }

        if (sscanf(word, "%d", &spu->arrcode[i]) != 1)
        {
            printf("ERROR\n");
            return ERROR_PROCESS;
        }

        free(word);
    }

    return PROCESSED_OK;
}

Error_processor Processor(SPU* spu, File_t code)
{

    for (size_t i = 0; i < code.str_num; i++)
    {
        if (spu->arrcode[i] == 1)
        {
            stack_element_t elem = spu->arrcode[i + 1];
            PushStack(&spu->stack, elem);
        }
        else if (spu->arrcode[i] == 2)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, a + b);
        }
        else if (spu->arrcode[i] == 3)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b - a);
        }
        else if (spu->arrcode[i] == 4)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, a * b);
        }
        else if (spu->arrcode[i] == 5)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b / a);
        }
        else if (spu->arrcode[i] == 6)
        {
            stack_element_t a = PeekStack(spu->stack);
            printf("%d\n", a);
        }
        else if (spu->arrcode[i] == 7)
        {
            stack_element_t elem;
            scanf("%d", &elem);
            PushStack(&spu->stack, elem);
        }
        else
        {
            if (spu->arrcode[i - 1] != 1 &&
                spu->arrcode[i - 1] != 2 &&
                spu->arrcode[i - 1] != 3 &&
                spu->arrcode[i - 1] != 4 &&
                spu->arrcode[i - 1] != 5 &&
                spu->arrcode[i - 1] != 6 &&
                spu->arrcode[i - 1] != 7)
            {
                printf("ERROR\n");
                return ERROR_PROCESS;
            }
        }
    }

    return PROCESSED_OK;
}
