#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

Error_assembler Assembler(File_t program, FILE* fw1, FILE* fw2);

int main()
{
    struct File_t program = {};
    struct File_t code = {};

    program.file_name = "program.asm";
    code.file_name = "code.asm";

    const char* code_array_file_name = "code_array.asm";

    Put_file_to_structure(&program);

    FILE* fw1 = fopen(code.file_name, "w");
    FILE* fw2 = fopen(code_array_file_name, "w");

    Error_assembler err_asm = Assembler(program, fw1, fw2);

    if (err_asm != ASSEMBLED_OK)
    {
        return err_asm;
    }

    fclose(fw1);
    fclose(fw2);

    return 0;
}

Error_assembler Assembler(File_t program, FILE* fw1, FILE* fw2)
{
    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        char* word = (char*)calloc(program.lineslen[i], sizeof(char));

        for (size_t j = 0; j < program.lineslen[i] + 1; j++)
        {
            word[j] = program.lines[i][j];
        }

        char* func = (char*)calloc(program.lineslen[i], sizeof(char));
        int arg = 0;

        int numb = sscanf(word, "%s %d", func, &arg);

        switch(numb)
        {
            case 1: if (strcmp(func, "add") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_ADD);
                        fprintf(fw2, "%d\n", CMD_ADD);
                    }
                    else if (strcmp(func, "sub") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_SUB);
                        fprintf(fw2, "%d\n", CMD_SUB);
                    }
                    else if (strcmp(func, "mul") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_MUL);
                        fprintf(fw2, "%d\n", CMD_MUL);
                    }
                    else if (strcmp(func, "div") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_DIV);
                        fprintf(fw2, "%d\n", CMD_DIV);
                    }
                    else if (strcmp(func, "out") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_OUT);
                        fprintf(fw2, "%d\n", CMD_OUT);
                    }
                    else if (strcmp(func, "in") == 0)
                    {
                        fprintf(fw1, "%d\n", CMD_IN);
                        fprintf(fw2, "%d\n", CMD_IN);
                    }
                    else
                    {
                        fprintf(fw1, "ERROR\n");
                        fprintf(fw2, "ERROR\n");
                        printf("ERROR\n");
                        free(word);
                        free(func);
                        return ERROR_ASM;
                    }
                    break;

            case 2: if (strcmp(func, "push") == 0)
                    {
                        fprintf(fw1, "%d ", CMD_PUSH);
                        fprintf(fw2, "%d\n", CMD_PUSH);

                        fprintf(fw1, "%d\n", arg);
                        fprintf(fw2, "%d\n", arg);
                    }
                    else
                    {
                        fprintf(fw1, "ERROR\n");
                        fprintf(fw2, "ERROR\n");
                        printf("ERROR\n");
                        free(word);
                        free(func);
                        return ERROR_ASM;
                    }
                    break;

            default: fprintf(fw1, "ERROR\n");
                     fprintf(fw2, "ERROR\n");
                     printf("ERROR\n");
                     free(word);
                     free(func);
                     return ERROR_ASM;
        }

        free(word);
        free(func);
    }

    free(program.buffer);
    free(program.lineslen);
    free(program.lines);

    return ASSEMBLED_OK;
}
