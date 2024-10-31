#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

size_t amount_of_labels(File_t program);
Error_assembler Put_labels_to_structure(File_t program, Labels_t* labels);
Error_assembler Assembler(File_t program, Labels_t* labels, FILE* fw1, FILE* fw2);

int main()
{
    struct File_t program = {};
    struct File_t code = {};
    struct File_t code_array = {};

    program.file_name = "program.asm";
    code.file_name = "code.asm";
    code_array.file_name = "code_array.asm";

    Put_file_to_structure(&program);

    FILE* fw1 = fopen(code.file_name, "w");
    FILE* fw2 = fopen(code_array.file_name, "w");


    size_t dota2 = amount_of_labels(program);

    Labels_t* labels = (Labels_t*)calloc(dota2 + 10, sizeof(Labels_t));

    Error_assembler err_asm = Assembler(program, labels, fw1, fw2);

    if (err_asm != ASSEMBLED_OK)
    {
        fclose(fw1);
        fclose(fw2);
        free(labels);
        return err_asm;
    }

    printf("errasm\n");

    fclose(fw1);
    fclose(fw2);

    printf("close\n");

    free(labels);
    printf("okeee\n");

    return 0;
}

size_t amount_of_labels(File_t program)
{
    FILE* fr = fopen(program.file_name, "r");
    size_t counter = 0;

    Read_file_to_buffer(&program);

    for (size_t i = 0; i < program.file_size; i++)
    {
        if (program.buffer[i] == ':')
        {
            counter++;
        }
    }

    printf("THERE ARE %u LABELS BLYAAAA\n", counter);
    fclose(fr);

    return counter;
}

Error_assembler Put_labels_to_structure(File_t program, Labels_t* labels)
{
    size_t counter_of_labels = 0;

    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        char* word = (char*)calloc(program.lineslen[i], sizeof(char));

        for (size_t j = 0; j < program.lineslen[i]; j++)
        {
            word[j] = program.lines[i][j];
        }

        if (word[program.lineslen[i] - 1] == ':')
        {
            word = (char*)realloc(word, program.lineslen[i] * sizeof(char));

            for (size_t j = 0; j < program.lineslen[i]; j++)
            {
                word[j] = program.lines[i][j];
            }

            labels[counter_of_labels].ip = i + 2;

            labels[counter_of_labels].name = (char*)calloc(program.lineslen[i] - 1, sizeof(char));

            for (size_t j = 0; j < program.lineslen[i] - 1; j++)
            {
                labels[counter_of_labels].name[j] = word[j];
            }

            printf("%u %s %u\n", counter_of_labels, labels[counter_of_labels].name, labels[counter_of_labels].ip);

            counter_of_labels++;
        }

        free(word);
    }
    return ASSEMBLED_OK;
}

Error_assembler Assembler(File_t program, Labels_t* labels, FILE* fw1, FILE* fw2)
{
    Error_assembler err_asm = Put_labels_to_structure(program, labels);

    if (err_asm != ASSEMBLED_OK)
    {
        return err_asm;
    }

    size_t labels_num = amount_of_labels(program);

    printf("%u\n", labels_num);

    for (size_t i = 0; i < labels_num; i++)
    {
        printf("%s %u\n", labels[i].name, labels[i].ip);
    }

    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        char* word = (char*)calloc(program.lineslen[i], sizeof(char));

        for (size_t j = 0; j < program.lineslen[i]; j++)
        {
            word[j] = program.lines[i][j];
        }

        if (word[program.lineslen[i] - 1] != ':')
        {
            char* func1 = (char*)calloc(program.lineslen[i], sizeof(char));
            char* func2 = (char*)calloc(program.lineslen[i], sizeof(char));
            int arg = 0;

            int numsd = sscanf(word, "%s %d", func1, &arg);
            int numss = sscanf(word, "%s %s", func1, func2);

            if (numsd == 1 && numss == 1)
            {
                if (strcmp(func1, "add") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_ADD);
                    fprintf(fw2, "%d\n", CMD_ADD);
                }
                else if (strcmp(func1, "sub") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_SUB);
                    fprintf(fw2, "%d\n", CMD_SUB);
                }
                else if (strcmp(func1, "mul") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_MUL);
                    fprintf(fw2, "%d\n", CMD_MUL);
                }
                else if (strcmp(func1, "div") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_DIV);
                    fprintf(fw2, "%d\n", CMD_DIV);
                }
                else if (strcmp(func1, "out") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_OUT);
                    fprintf(fw2, "%d\n", CMD_OUT);
                }
                else if (strcmp(func1, "in") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_IN);
                    fprintf(fw2, "%d\n", CMD_IN);
                }
                else if (strcmp(func1, "pop") == 0)
                {
                    fprintf(fw1, "%d\n", CMD_POP);
                    fprintf(fw2, "%d\n", CMD_POP);
                }
                else
                {
                    fprintf(fw1, "ERROR\n");
                    fprintf(fw2, "ERROR\n");
                    printf("ERRORrrr\n");
                    free(word);
                    free(func1);
                    free(func2);
                    return ERROR_ASM;
                }
            }
            else if (numsd == 2)
            {
                if (strcmp(func1, "push") == 0)
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
                    free(func1);
                    free(func2);
                    return ERROR_ASM;
                }
            }
            else if (numsd == 1 && numss == 2)
            {
                if (strcmp(func1, "push") == 0)
                {
                    fprintf(fw1, "%d ", CMD_PUSH);
                    fprintf(fw2, "%d\n", CMD_PUSH);

                    if (strcmp(func2, "ax") == 0)
                    {
                        fprintf(fw1, "%d ", AX);
                        fprintf(fw2, "%d\n", AX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "bx") == 0)
                    {
                        fprintf(fw1, "%d ", BX);
                        fprintf(fw2, "%d\n", BX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "cx") == 0)
                    {
                        fprintf(fw1, "%d ", CX);
                        fprintf(fw2, "%d\n", CX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "dx") == 0)
                    {
                        fprintf(fw1, "%d ", DX);
                        fprintf(fw2, "%d\n", DX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else
                    {
                        fprintf(fw1, "ERROR\n");
                        fprintf(fw2, "ERROR\n");
                        printf("ERROR\n");
                        free(word);
                        free(func1);
                        free(func2);
                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func1, "pop") == 0)
                {
                    fprintf(fw1, "%d ", CMD_POP);
                    fprintf(fw2, "%d\n", CMD_POP);

                    if (strcmp(func2, "ax") == 0)
                    {
                        fprintf(fw1, "%d ", AX);
                        fprintf(fw2, "%d\n", AX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "bx") == 0)
                    {
                        fprintf(fw1, "%d ", BX);
                        fprintf(fw2, "%d\n", BX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "cx") == 0)
                    {
                        fprintf(fw1, "%d ", CX);
                        fprintf(fw2, "%d\n", CX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else if (strcmp(func2, "dx") == 0)
                    {
                        fprintf(fw1, "%d ", DX);
                        fprintf(fw2, "%d\n", DX);

                        fprintf(fw1, "%d\n", 0);
                        fprintf(fw2, "%d\n", 0);
                    }
                    else
                    {
                        fprintf(fw1, "ERROR\n");
                        fprintf(fw2, "ERROR\n");
                        printf("ERROR\n");
                        free(word);
                        free(func1);
                        free(func2);
                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func1, "ja") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JA);
                    fprintf(fw2, "%d\n", CMD_JA);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "jae") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JAE);
                    fprintf(fw2, "%d\n", CMD_JAE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "jb") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JB);
                    fprintf(fw2, "%d\n", CMD_JB);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "jbe") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JBE);
                    fprintf(fw2, "%d\n", CMD_JBE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "je") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JE);
                    fprintf(fw2, "%d\n", CMD_JE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "jne") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JNE);
                    fprintf(fw2, "%d\n", CMD_JNE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func1, "jmp") == 0)
                {
                    fprintf(fw1, "%d ", CMD_JMP);
                    fprintf(fw2, "%d\n", CMD_JMP);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(func2, labels[j].name) == 0)
                        {
                            fprintf(fw1, "%u\n", labels[j].ip);
                            fprintf(fw2, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else
                {
                    fprintf(fw1, "ERROR\n");
                    fprintf(fw2, "ERROR\n");
                    printf("ERROR\n");
                    free(word);
                    free(func1);
                    free(func2);
                    return ERROR_ASM;
                }
            }
            else
            {
                fprintf(fw1, "ERROR\n");
                fprintf(fw2, "ERROR\n");
                printf("ERROR\n");
                free(word);
                free(func1);
                free(func2);
                return ERROR_ASM;
            }

            free(word);
            free(func1);
            free(func2);
        }
    }

    free(program.buffer);
    free(program.lineslen);
    free(program.lines);

    return ASSEMBLED_OK;
}
