#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "Stack.h"
#include "ReadFromFile.h"

int main()
{
    struct File code = {};

    struct Stack_t stack = {};

    InitStack(&stack);

    code.file_name = "code.asm";

    code.file_size = num_of_symbols_in_file(code.file_name);
    code.str_num = num_of_strings_in_file(code.file_name);

    code.buffer = (char*)calloc(code.file_size + 1, sizeof(char));

    code.lineslen = (size_t*)calloc(code.str_num, sizeof(size_t));

    code.lines = (char**)calloc(code.str_num + 1, sizeof(char*));

    FILE* fr = fopen(code.file_name, "r");

    Read_file_to_buffer(code.file_name, code.file_size, code.buffer);
    Put_lineslen_for_all_lines(code.buffer, code.file_size, code.lineslen);
    Put_pointers_to_lines(code.buffer, code.file_size, code.str_num, code.lines);

    fclose(fr);

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
                PushStack(&stack, arg);
            }
        }
        else if (numb == 1)
        {
            switch(func)
            {
                case CMD_PUSH: PushStack(&stack, 60);

                               break;

                case CMD_ADD: a = PeekStack(stack);
                              PopStack(&stack);

                              b = PeekStack(stack);
                              PopStack(&stack);

                              PushStack(&stack, a + b);

                              break;

                case CMD_SUB: a = PeekStack(stack);
                              PopStack(&stack);

                              b = PeekStack(stack);
                              PopStack(&stack);

                              PushStack(&stack, a - b);

                              break;

                case CMD_MUL: a = PeekStack(stack);
                              PopStack(&stack);

                              b = PeekStack(stack);
                              PopStack(&stack);

                              PushStack(&stack, a * b);

                              break;

                case CMD_DIV: a = PeekStack(stack);
                              PopStack(&stack);

                              b = PeekStack(stack);
                              PopStack(&stack);

                              PushStack(&stack, a / b);

                              break;

                case CMD_OUT: DumpStack(stack);

                              break;

                case CMD_IN: stack_element_t elem;
                             printf("Input new element: ");
                             scanf("%d", &elem);
                             PushStack(&stack, elem);

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
