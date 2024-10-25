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

    //struct File_t code = {};

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

        for (size_t j = 0; j < spu->code.lineslen[i] + 1; j++)
        {
            word[j] = spu->code.lines[i][j];
        }

        stack_element_t a, b;

        int func, arg;

        int numb = sscanf(word, "%d %d", &func, &arg);

        if (numb == 1)
        {
            switch(func)
            {
                case CMD_ADD: a = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              b = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              PushStack(&spu->stack, a + b);
                              break;

                case CMD_SUB: a = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              b = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              PushStack(&spu->stack, a - b);
                              break;

                case CMD_MUL: a = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              b = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              PushStack(&spu->stack, a * b);
                              break;

                case CMD_DIV: a = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              b = PeekStack(spu->stack);
                              PopStack(&spu->stack);

                              PushStack(&spu->stack, b / a);
                              break;

                case CMD_OUT: a = PeekStack(spu->stack);
                              printf("Result = %d\n", a);
                              break;

                case CMD_IN: stack_element_t elem;
                             printf("Input new element: ");
                             scanf("%d", &elem);
                             PushStack(&spu->stack, elem);
                             break;

                default: printf("ERROR\n");
                         return ERROR_PROCESS;
            }
        }
        else if (numb == 2)
        {
            if (func == CMD_PUSH)
            {
                PushStack(&spu->stack, arg);
            }
            else
            {
                printf("ERROR\n");
                return ERROR_PROCESS;
            }
        }
        else
        {
            printf("ERROR\n");
            free(word);
            return ERROR_PROCESS;
        }

        free(word);
    }

    return PROCESSED_OK;
}
