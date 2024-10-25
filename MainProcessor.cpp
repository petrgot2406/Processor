#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "Stack.h"
#include "ReadFromFile.h"

Error_processor Processor(File_t code, Stack_t* stack);

int main()
{
    struct Stack_t stack = {};

    InitStack(&stack);

    struct File_t code = {};

    code.file_name = "code.asm";

    Put_file_to_structure(&code);

    Error_processor err_proc = Processor(code, &stack);

    if (err_proc != PROCESSED_OK)
    {
        return err_proc;
    }

    return 0;
}

Error_processor Processor(File_t code, Stack_t* stack)
{
    for (size_t i = 0; i < code.str_num; i++)
    {
        char* word = (char*)calloc(code.lineslen[i], sizeof(char));

        for (size_t j = 0; j < code.lineslen[i] + 1; j++)
        {
            word[j] = code.lines[i][j];
        }

        stack_element_t a, b;

        int func, arg;

        int numb = sscanf(word, "%d %d", &func, &arg);

        if (numb == 2)
        {
            if (func == CMD_PUSH)
            {
                PushStack(stack, arg);
            }
        }
        else if (numb == 1)
        {
            switch(func)
            {
                case CMD_PUSH: PushStack(stack, 60);

                               break;

                case CMD_ADD: a = PeekStack(*stack);
                              PopStack(stack);

                              b = PeekStack(*stack);
                              PopStack(stack);

                              PushStack(stack, a + b);

                              break;

                case CMD_SUB: a = PeekStack(*stack);
                              PopStack(stack);

                              b = PeekStack(*stack);
                              PopStack(stack);

                              PushStack(stack, a - b);

                              break;

                case CMD_MUL: a = PeekStack(*stack);
                              PopStack(stack);

                              b = PeekStack(*stack);
                              PopStack(stack);

                              PushStack(stack, a * b);

                              break;

                case CMD_DIV: a = PeekStack(*stack);
                              PopStack(stack);

                              b = PeekStack(*stack);
                              PopStack(stack);

                              PushStack(stack, a / b);

                              break;

                case CMD_OUT: DumpStack(*stack);

                              break;

                case CMD_IN: stack_element_t elem;
                             printf("Input new element: ");
                             scanf("%d", &elem);
                             PushStack(stack, elem);

                             break;

                default: printf("ERROR\n");
                         return ERROR_PROCESS;
            }
        }
        else
        {
            printf("ERROR\n");
            return ERROR_PROCESS;
        }

        free(word);
    }

    return PROCESSED_OK;
}
