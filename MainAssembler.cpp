#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

int main()
{
    const char* program_file_name = "program.asm";
    const char* code_file_name = "code.asm";

    FILE* fr = fopen(program_file_name, "r");
    FILE* fw = fopen(code_file_name, "w");

    size_t file_size = num_of_symbols_in_file(program_file_name);
    size_t str_num = num_of_strings_in_file(program_file_name);

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));

    size_t* lineslen = (size_t*)calloc(str_num, sizeof(size_t));

    char** lines = (char**)calloc(str_num + 1, sizeof(char*));

    Read_file_to_buffer(program_file_name, file_size, buffer);
    Put_lineslen_for_all_lines(buffer, file_size, lineslen);
    Put_pointers_to_lines(buffer, file_size, str_num, lines);

    for (size_t i = 0; i < str_num; i++)
    {
        char* word = (char*)calloc(lineslen[i], sizeof(char));

        for (size_t j = 0; j < lineslen[i] + 1; j++)
        {
            word[j] = lines[i][j];
        }

        char* func = (char*)calloc(lineslen[i], sizeof(char));
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

    free(buffer);
    free(lineslen);
    free(lines);

    fclose(fr);
    fclose(fw);

    return ASSEMBLE_OK;
}
