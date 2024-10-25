#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

int main()
{
    struct File program = {};
    struct File code = {};

    program.file_name = "program.asm";

    program.file_size = num_of_symbols_in_file(program.file_name);
    program.str_num = num_of_strings_in_file(program.file_name);

    program.buffer = (char*)calloc(program.file_size + 1, sizeof(char));

    program.lineslen = (size_t*)calloc(program.str_num, sizeof(size_t));

    program.lines = (char**)calloc(program.str_num + 1, sizeof(char*));


    code.file_name = "code.asm";

    FILE* fr = fopen(program.file_name, "r");

    Read_file_to_buffer(program.file_name, program.file_size, program.buffer);
    Put_lineslen_for_all_lines(program.buffer, program.file_size, program.lineslen);
    Put_pointers_to_lines(program.buffer, program.file_size, program.str_num, program.lines);

    fclose(fr);

    FILE* fw = fopen(code.file_name, "w");

    for (size_t i = 0; i < program.str_num; i++)
    {
        char* word = (char*)calloc(program.lineslen[i], sizeof(char));

        for (size_t j = 0; j < program.lineslen[i] + 1; j++)
        {
            word[j] = program.lines[i][j];
        }

        char* func = (char*)calloc(program.lineslen[i], sizeof(char));
        int arg = 0;

        int numb = sscanf(word, "%s %d", func, &arg);

        if (numb == 2)
        {
            if (strcmp(func, "push") == 0)
            {
                fprintf(fw, "%d ", CMD_PUSH);
                fprintf(fw, "%d\n", arg);
            }
        }
        else if (numb == 1)
        {
            if (strcmp(func, "add") == 0)
            {
                fprintf(fw, "%d\n", CMD_ADD);
            }
            else if (strcmp(func, "sub") == 0)
            {
                fprintf(fw, "%d\n", CMD_SUB);
            }
            else if (strcmp(func, "mul") == 0)
            {
                fprintf(fw, "%d\n", CMD_MUL);
            }
            else if (strcmp(func, "div") == 0)
            {
                fprintf(fw, "%d\n", CMD_DIV);
            }
            else if (strcmp(func, "out") == 0)
            {
                fprintf(fw, "%d\n", CMD_OUT);
            }
            else if (strcmp(func, "in") == 0)
            {
                fprintf(fw, "%d\n", CMD_IN);
            }
            else
            {
                fprintf(fw, "ERROR\n");
                printf("ERROR\n");
                free(word);
                return ERROR_ASM;
            }
        }
        else
        {
            fprintf(fw, "ERROR\n");
            printf("ERROR\n");
            free(word);
            return ERROR_ASM;
        }

        free(word);
        free(func);
    }

    fclose(fw);

    free(program.buffer);
    free(program.lineslen);
    free(program.lines);

    return ASSEMBLE_OK;
}
