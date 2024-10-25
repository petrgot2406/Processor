#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

Error_assembler Assembler(File_t program, FILE* fw);

int main()
{
    struct File_t program = {};
    struct File_t code = {};

    program.file_name = "program.asm";
    code.file_name = "code.asm";

    Put_file_to_structure(&program);

    FILE* fw = fopen(code.file_name, "w");

    Error_assembler err_asm = Assembler(program, fw);

    if (err_asm != ASSEMBLED_OK)
    {
        return err_asm;
    }

    return 0;
}

Error_assembler Assembler(File_t program, FILE* fw)
{
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

    return ASSEMBLED_OK;
}
