#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../headers/Constants.h"
#include "../headers/Struct.h"
#include "../headers/ReadFromFile.h"

size_t amount_of_labels(File_t program);
Error_assembler Put_labels_to_structure(File_t program, Labels_t* labels);
void FreeAssembler(File_t* program);
Error_assembler Assembler(File_t program, Labels_t* labels, File_t code);

int main()
{
    struct File_t program = {};
    struct File_t code = {};

    program.file_name = PROGRAM_FILE_NAME;
    code.file_name = CODE_FILE_NAME;

    Put_file_to_structure(&program);

    size_t num_of_labels = amount_of_labels(program);

    Labels_t* labels = (Labels_t*)calloc(num_of_labels + 1, sizeof(Labels_t));

    Error_assembler err_asm = Assembler(program, labels, code);

    if (err_asm != ASSEMBLED_OK)
    {
        free(labels);
        return err_asm;
    }

    free(labels);

    return 0;
}

size_t amount_of_labels(File_t program)
{
    size_t counter = 0;

    for (size_t i = 0; i < program.file_size; i++)
    {
        if (program.buffer[i] == ':')
        {
            counter++;
        }
    }

    return counter;
}

Error_assembler Put_labels_to_structure(File_t program, Labels_t* labels)
{
    size_t counter_of_labels = 0;

    size_t counter = 0;

    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        char* word = (char*)calloc(program.lineslen[i], sizeof(char));

        memcpy(word, program.lines[i], program.lineslen[i]);

        if (word[program.lineslen[i] - 1] == ':')
        {
            counter++;
            labels[counter_of_labels].ip = counter + 1;

            labels[counter_of_labels].name = (char*)calloc
                                             (program.lineslen[i],
                                              sizeof(char));
            memcpy(labels[counter_of_labels].name, word, program.lineslen[i] - 1);

            counter_of_labels++;
        }
        else
        {
            char* func = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            char* args = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            int arg = 0;

            int numsd = sscanf(word, "%s %d", func, &arg);
            int numss = sscanf(word, "%s %s", func, args);

            if (numsd == 1 && numsd == 1)
            {
                counter++;
            }
            else if (numsd == 1 && numss == 2)
            {
                counter += 2;
            }
            else if (numsd == 2 && numss == 2)
            {
                counter += 2;
            }
            else
            {
                printf("ERROR IN LINE %d\n", i + 1);
                return ERROR_ASM;
            }

            free(func);
            free(args);
        }

        free(word);
    }

    /*
    for (size_t i = 0; i < counter_of_labels; i++)
    {
        printf("label %u: ip = %u, name = %s\n", i, labels->ip, labels->name);
    }
    */

    return ASSEMBLED_OK;
}

void FreeAssembler(File_t* program)
{
    free(program->buffer);
    free(program->lineslen);
    free(program->lines);
}

Error_assembler Assembler(File_t program, Labels_t* labels, File_t code)
{
    Error_assembler err_asm = Put_labels_to_structure(program, labels);

    if (err_asm != ASSEMBLED_OK)
    {
        return err_asm;
    }

    FILE* fw = fopen(code.file_name, "w");

    size_t labels_num = amount_of_labels(program);

    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        char* word = (char*)calloc(program.lineslen[i] + 1, sizeof(char));

        for (size_t j = 0; j < program.lineslen[i]; j++)
        {
            word[j] = program.lines[i][j];
        }

        if (word[program.lineslen[i] - 1] == ':')
        {
            fprintf(fw, "%d\n", CMD_LABEL);
        }
        else
        {
            char* func = (char*)calloc(program.lineslen[i], sizeof(char));
            char* args = (char*)calloc(program.lineslen[i], sizeof(char));
            int arg = 0;

            int numsd = sscanf(word, "%s %d", func, &arg);
            int numss = sscanf(word, "%s %s", func, args);

            if (numsd == 1 && numss == 1)
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
                else if (strcmp(func, "pop") == 0)
                {
                    fprintf(fw, "%d\n", CMD_POP);
                }
                else if (strcmp(func, "hlt") == 0)
                {
                    fprintf(fw, "%d\n", CMD_HLT);
                }
                else
                {
                    fprintf(fw, "ERROR\n");
                    printf("ERROR IN LINE %d\n", i + 1);

                    fclose(fw);

                    free(word);
                    free(func);
                    free(args);

                    FreeAssembler(&program);

                    return ERROR_ASM;
                }
            }
            else if (numsd == 2)
            {
                if (strcmp(func, "push") == 0)
                {
                    fprintf(fw, "%d\n", CMD_PUSH);
                    fprintf(fw, "%d\n", arg);
                }
                else
                {
                    fprintf(fw, "ERROR\n");
                    printf("ERROR IN LINE %d\n", i + 1);

                    fclose(fw);

                    free(word);
                    free(func);
                    free(args);

                    FreeAssembler(&program);

                    return ERROR_ASM;
                }
            }
            else if (numsd == 1 && numss == 2)
            {
                if (strcmp(func, "pushf") == 0)
                {
                    fprintf(fw, "%d\n", CMD_PUSHF);

                    if (strcmp(args, "ax") == 0)
                    {
                        fprintf(fw, "%d\n", AX);
                    }
                    else if (strcmp(args, "bx") == 0)
                    {
                        fprintf(fw, "%d\n", BX);
                    }
                    else if (strcmp(args, "cx") == 0)
                    {
                        fprintf(fw, "%d\n", CX);
                    }
                    else if (strcmp(args, "dx") == 0)
                    {
                        fprintf(fw, "%d\n", DX);
                    }
                    else
                    {
                        fprintf(fw, "ERROR\n");
                        printf("ERROR IN LINE %d\n", i + 1);

                        fclose(fw);

                        free(word);
                        free(func);
                        free(args);

                        FreeAssembler(&program);

                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func, "popf") == 0)
                {
                    fprintf(fw, "%d\n", CMD_POPF);

                    if (strcmp(args, "ax") == 0)
                    {
                        fprintf(fw, "%d\n", AX);
                    }
                    else if (strcmp(args, "bx") == 0)
                    {
                        fprintf(fw, "%d\n", BX);
                    }
                    else if (strcmp(args, "cx") == 0)
                    {
                        fprintf(fw, "%d\n", CX);
                    }
                    else if (strcmp(args, "dx") == 0)
                    {
                        fprintf(fw, "%d\n", DX);
                    }
                    else
                    {
                        fprintf(fw, "ERROR\n");
                        printf("ERROR IN LINE %d\n", i + 1);

                        fclose(fw);

                        free(word);
                        free(func);
                        free(args);

                        FreeAssembler(&program);

                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func, "ja") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JA);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "jae") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JAE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "jb") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JB);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "jbe") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JBE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "je") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "jne") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JNE);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else if (strcmp(func, "jmp") == 0)
                {
                    fprintf(fw, "%d\n", CMD_JMP);

                    for (size_t j = 0; j < labels_num; j++)
                    {
                        if (strcmp(args, labels[j].name) == 0)
                        {
                            fprintf(fw, "%u\n", labels[j].ip);
                            break;
                        }
                    }
                }
                else
                {
                    fprintf(fw, "ERROR\n");
                    printf("ERROR IN LINE %d\n", i + 1);

                    fclose(fw);

                    free(word);
                    free(func);
                    free(args);

                    FreeAssembler(&program);

                    return ERROR_ASM;
                }
            }
            else
            {
                fprintf(fw, "ERROR\n");
                printf("ERROR IN LINE %d\n", i + 1);

                fclose(fw);

                free(word);
                free(func);
                free(args);

                FreeAssembler(&program);

                return ERROR_ASM;
            }

            free(word);
            free(func);
            free(args);
        }
    }

    fclose(fw);

    FreeAssembler(&program);

    return ASSEMBLED_OK;
}
