#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"
#include "ReadFromFile.h"

int main()
{
    struct Stack_t stack = {};

    InitStack(&stack);

    const char* code_file_name = "code.asm";

    FILE* fr = fopen(code_file_name, "r");

    size_t file_size = num_of_symbols_in_file(code_file_name);
    size_t str_num = num_of_strings_in_file(code_file_name);

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));

    size_t* lineslen = (size_t*)calloc(str_num, sizeof(size_t));

    char** lines = (char**)calloc(str_num + 1, sizeof(char*));

    Read_file_to_buffer(code_file_name, file_size, buffer);
    Put_lineslen_for_all_lines(buffer, file_size, lineslen);
    Put_pointers_to_lines(buffer, file_size, str_num, lines);

    for (size_t i = 0; i < str_num; i++)
    {
        char* word = (char*)calloc(lineslen[i], sizeof(char));

        for (size_t j = 0; j < lineslen[i] + 1; j++)
        {
            word[j] = lines[i][j];
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
                         return 1;
            }
        }
        else
        {
            printf("ERROR\n");
        }

        free(word);
    }

    fclose(fr);

    return 0;
}
