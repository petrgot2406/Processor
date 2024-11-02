#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "Stack.h"
#include "ReadFromFile.h"

Error_processor Processor(SPU* spu);

int main()
{
    struct SPU spu = {};

    spu.code.file_name = CODE_FILE_NAME;

    InitStack(&spu.stack);

    Put_file_to_structure(&spu.code);

    Error_processor err_proc = Processor(&spu);

    if (err_proc != PROCESSED_OK)
    {
        return err_proc;
    }

    return 0;
}

Error_processor Processor(SPU* spu)
{
    for (size_t i = 0; i < spu->code.str_num; i++)
    {
        char* word = (char*)calloc(spu->code.lineslen[i],
                                   sizeof(char));

        for (size_t j = 0; j < spu->code.lineslen[i]; j++)
        {
            word[j] = spu->code.lines[i][j];
        }

        int func, arg;

        int numb = sscanf(word, "%d %d", &func, &arg);

        if (numb == EOF)
        {
            //DO NOTHING;
        }
        else if (numb == 1)
        {
            if (func == CMD_LABEL)
            {
                //DO NOTHING;
            }
            else if (func == CMD_ADD)
            {
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                PushStack(&spu->stack, b + a);
            }
            else if (func == CMD_SUB)
            {
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                PushStack(&spu->stack, b - a);
            }
            else if (func == CMD_MUL)
            {
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                PushStack(&spu->stack, b * a);
            }
            else if (func == CMD_DIV)
            {
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                PushStack(&spu->stack, b / a);
            }
            else if (func == CMD_OUT)
            {
                stack_element_t a = PeekStack(spu->stack);
                printf("%d\n", a);
            }
            else if (func == CMD_IN)
            {
                stack_element_t elem;
                scanf("%d", &elem);
                PushStack(&spu->stack, elem);
            }
            else if (func == CMD_POP)
            {
                PopStack(&spu->stack);
            }
            else if (func == CMD_HLT)
            {
                DestroyStack(&spu->stack);
                free(word);
                printf("\n");
                printf("Program successfully finished\n");
                return PROCESSED_OK;
            }
            else
            {
                DestroyStack(&spu->stack);
                free(word);
                printf("ERROR IN LINE %d\n", i + 1);
                return ERROR_PROCESS;
            }
        }
        else if (numb == 2)
        {
            if (func == CMD_PUSH)
            {
                stack_element_t elem = arg;
                PushStack(&spu->stack, elem);
            }
            else if (func == CMD_PUSHF)
            {
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
                    free(word);
                    printf("ERROR IN LINE %d\n", i + 1);
                    return ERROR_PROCESS;
                }
            }
            else if (func == CMD_POPF)
            {
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
                    free(word);
                    printf("ERROR IN LINE %d\n", i + 1);
                    return ERROR_PROCESS;
                }
            }
            else if (func == CMD_JA)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a > b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JAE)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a >= b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JB)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a < b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JBE)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a <= b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JE)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a == b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JNE)
            {
                stack_element_t b = PeekStack(spu->stack);
                PopStack(&spu->stack);
                stack_element_t a = PeekStack(spu->stack);
                PopStack(&spu->stack);
                if (a != b)
                {
                    i = arg - 1;
                }
            }
            else if (func == CMD_JMP)
            {
                i = arg - 1;
            }
            else
            {
                DestroyStack(&spu->stack);
                free(word);
                printf("ERROR IN LINE %d\n", i + 1);
                return ERROR_PROCESS;
            }
        }
        else
        {
            DestroyStack(&spu->stack);
            free(word);
            printf("ERROR IN LINE %d\n", i + 1);
            return ERROR_PROCESS;
        }
    }

    return PROCESSED_OK;
}
