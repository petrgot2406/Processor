#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../headers/Constants.h"
#include "../headers/Struct.h"
#include "../headers/Stack.h"
#include "../headers/ReadFromFile.h"

Error_processor Put_code_to_array(SPU* spu);
Error_processor Processor(SPU* spu);

int main()
{
    struct SPU spu = {};

    spu.code.file_name = CODE_FILE_NAME;

    InitStack(&spu.stack);

    Put_file_to_structure(&spu.code);

    Put_code_to_array(&spu);

    Error_processor err_proc = Processor(&spu);

    if (err_proc != PROCESSED_OK)
    {
        return err_proc;
    }

    free(spu.code_array);

    return 0;
}

Error_processor Put_code_to_array(SPU* spu)
{
    spu->code_array = (int*)calloc(spu->code.str_num, sizeof(int));

    for (size_t i = 0; i < spu->code.str_num; i++)
    {
        char* word = (char*)calloc(spu->code.lineslen[i], sizeof(char));

        memcpy(word, spu->code.lines[i], spu->code.lineslen[i]);

        int element;
        int scan_num = sscanf(word, "%d", &element);

        free(word);

        if (scan_num == 1)
        {
            spu->code_array[i] = element;
        }
        else
        {
            printf("ERROR IN INPUT\n");

            FreeFile(&spu->code);

            return ERROR_PROCESS;
        }
    }

    FreeFile(&spu->code);

    return PROCESSED_OK;
}

Error_processor Processor(SPU* spu)
{
    for (size_t i = 0; i < spu->code.str_num; i++)
    {
        if (spu->code_array[i] == CMD_LABEL)
        {
            //DO NOTHING;
        }
        else if (spu->code_array[i] == CMD_PUSH)
        {
            stack_element_t elem = spu->code_array[i + 1];
            PushStack(&spu->stack, elem);
            i++;
        }
        else if (spu->code_array[i] == CMD_PUSHR)
        {
            int arg = spu->code_array[i + 1];
            i++;
            if (arg == AX)
            {
                stack_element_t elem = spu->registers.ax;
                PushStack(&spu->stack, elem);
            }
            else if (arg == BX)
            {
                stack_element_t elem = spu->registers.bx;
                PushStack(&spu->stack, elem);
            }
            else if (arg == CX)
            {
                stack_element_t elem = spu->registers.cx;
                PushStack(&spu->stack, elem);
            }
            else if (arg == DX)
            {
                stack_element_t elem = spu->registers.dx;
                PushStack(&spu->stack, elem);
            }
            else
            {
                DestroyStack(&spu->stack);
                printf("ERROR IN LINE %lu\n", i + 1);
                return ERROR_PROCESS;
            }
        }
        else if (spu->code_array[i] == CMD_ADD)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b + a);
        }
        else if (spu->code_array[i] == CMD_SUB)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b - a);
        }
        else if (spu->code_array[i] == CMD_MUL)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b * a);
        }
        else if (spu->code_array[i] == CMD_DIV)
        {
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            PushStack(&spu->stack, b / a);
        }
        else if (spu->code_array[i] == CMD_OUT)
        {
            stack_element_t a = PeekStack(spu->stack);
            printf("%d\n", a);
        }
        else if (spu->code_array[i] == CMD_IN)
        {
            stack_element_t elem;
            scanf("%d", &elem);
            PushStack(&spu->stack, elem);
        }
        else if (spu->code_array[i] == CMD_POP)
        {
            PopStack(&spu->stack);
        }
        else if (spu->code_array[i] == CMD_POPR)
        {
            int arg = spu->code_array[i + 1];
            i++;
            if (arg == AX)
            {
                spu->registers.ax = PeekStack(spu->stack);
                PopStack(&spu->stack);
            }
            else if (arg == BX)
            {
                spu->registers.bx = PeekStack(spu->stack);
                PopStack(&spu->stack);
            }
            else if (arg == CX)
            {
                spu->registers.cx = PeekStack(spu->stack);
                PopStack(&spu->stack);
            }
            else if (arg == DX)
            {
                spu->registers.dx = PeekStack(spu->stack);
                PopStack(&spu->stack);
            }
            else
            {
                DestroyStack(&spu->stack);
                printf("ERROR IN LINE %lu\n", i + 1);
                return ERROR_PROCESS;
            }
        }
        else if (spu->code_array[i] == CMD_JA)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a > b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JAE)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a >= b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JB)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a < b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JBE)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a <= b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JE)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a == b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JNE)
        {
            int arg = spu->code_array[i + 1];
            stack_element_t b = PeekStack(spu->stack);
            PopStack(&spu->stack);
            stack_element_t a = PeekStack(spu->stack);
            PopStack(&spu->stack);
            if (a != b)
            {
                i = (size_t)arg;
            }
            else
            {
                i++;
            }
        }
        else if (spu->code_array[i] == CMD_JMP)
        {
            int arg = spu->code_array[i + 1];
            i = (size_t)arg;
        }
        else if (spu->code_array[i] == CMD_HLT)
        {
            DestroyStack(&spu->stack);
            printf("\n");
            printf("Program successfully finished\n");
            return PROCESSED_OK;
        }
        else
        {
            printf("ERROR\n");
            return ERROR_PROCESS;
        }
    }

    return PROCESSED_OK;
}
