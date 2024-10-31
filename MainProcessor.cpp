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

    InitStack(&spu.stack);

    spu.code.file_name = "code.asm";

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
        char* word = (char*)calloc(spu->code.lineslen[i], sizeof(char));

        for (size_t j = 0; j < spu->code.lineslen[i]; j++)
        {
            word[j] = spu->code.lines[i][j];
        }

        int func, arg1, arg2;

        int numb = sscanf(word, "%d %d %d", &func, &arg1, &arg2);
        printf("%d\n", numb);
        printf("%s\n", word);
        printf("%d %d %d\n", func, arg1, arg2);


        if (numb == 1)
        {
            if (func == CMD_ADD)
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
            else
            {
                free(word);
                printf("ERROR\n");
                return ERROR_PROCESS;
            }

        }
        else if (numb == 2)
        {
            if (func == CMD_PUSH)
            {
                stack_element_t elem = arg1;
                PushStack(&spu->stack, elem);
            }
            else if (func == CMD_POP)
            {
                if (arg1 == AX)
                {
                    spu->registers.ax = PeekStack(spu->stack);
                    PopStack(&spu->stack);
                }
                else if (arg1 == BX)
                {
                    spu->registers.bx = PeekStack(spu->stack);
                    PopStack(&spu->stack);
                }
                else if (arg1 == CX)
                {
                    spu->registers.cx = PeekStack(spu->stack);
                    PopStack(&spu->stack);
                }
                else if (arg1 == DX)
                {
                    spu->registers.dx = PeekStack(spu->stack);
                    PopStack(&spu->stack);
                }
                else
                {
                    free(word);
                    printf("ERROR\n");
                    return ERROR_PROCESS;
                }
            }
            else if (func == CMD_JMP)
            {
                i = arg1;
            }
            else
            {
                free(word);
                printf("ERROR\n");
                return ERROR_PROCESS;
            }
        }
        else if (numb == 3)
        {
            if (func == CMD_PUSH)
            {
                if (arg1 == AX && arg2 == 228)
                {
                    PushStack(&spu->stack, spu->registers.ax);
                }
                else if (arg1 == BX && arg2 == 228)
                {
                    PushStack(&spu->stack, spu->registers.bx);
                }
                else if (arg1 == CX && arg2 == 228)
                {
                    PushStack(&spu->stack, spu->registers.cx);
                }
                else if (arg1 == DX && arg2 == 228)
                {
                    PushStack(&spu->stack, spu->registers.dx);
                }
                else
                {
                    free(word);
                    printf("ERROR\n");
                    return ERROR_PROCESS;
                }
            }
            else
            {
                free(word);
                printf("ERROR\n");
                return ERROR_PROCESS;
            }
        }
        else
        {
            free(word);
            printf("ERROR\n");
            return ERROR_PROCESS;
        }
    }

    return PROCESSED_OK;
}
