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
void FreeLabels(Labels_t* labels, size_t num_of_labels);
Error_assembler PrintJump(Labels_t* labels, char* args, FILE* fw, size_t num_of_labels, int CMD);
Error_assembler PrintWithoutArg(File_t program, FILE* fw, char* func, char* args, size_t i);
Error_assembler PrintRegister(File_t program, FILE* fw, char* func, char* args, size_t i);
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
        FreeLabels(labels, num_of_labels);
        free(labels);
        return err_asm;
    }

    FreeLabels(labels, num_of_labels);
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

        if (program.lines[i][program.lineslen[i] - 1] == ':')
        {
            counter++;
            labels[counter_of_labels].ip = counter;

            labels[counter_of_labels].name = (char*)calloc
                                             (program.lineslen[i],
                                              sizeof(char));

            memcpy(labels[counter_of_labels].name, program.lines[i], program.lineslen[i] - 1);

            counter_of_labels++;
        }
        else
        {
            char* func = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            char* args = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            int arg = 0;

            program.lines[i][program.lineslen[i]] = '\0';

            int numsd = sscanf(program.lines[i], "%s %d", func, &arg);
            int numss = sscanf(program.lines[i], "%s %s", func, args);

            if (numsd == 1)
            {
                counter++;
            }
            else if (numsd == 2)
            {
                counter += 2;
            }
            else if (numsd == 1 && numss == 2)
            {
                counter += 2;
            }
            else
            {
                printf("ERROR IN LINE %lu\n", i + 1);

                free(func);
                free(args);

                return ERROR_ASM;
            }

            free(func);
            free(args);
        }
    }

    return ASSEMBLED_OK;
}

void FreeAssembler(File_t* program)
{
    free(program->buffer);
    free(program->lineslen);
    free(program->lines);
}

void FreeLabels(Labels_t* labels, size_t num_of_labels)
{
    for (size_t i = 0; i < num_of_labels; i++)
    {
        free(labels[i].name);
    }
}

Error_assembler PrintJump(Labels_t* labels, char* args, FILE* fw, size_t num_of_labels, int CMD)
{
    fprintf(fw, "%d\n", CMD);

    for (size_t j = 0; j < num_of_labels; j++)
    {
        if (strcmp(args, labels[j].name) == 0)
        {
            fprintf(fw, "%lu\n", labels[j].ip);
            break;
        }
    }

    return ASSEMBLED_OK;
}

Error_assembler PrintWithoutArg(File_t program, FILE* fw, char* func, char* args, size_t i)
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
        printf("ERROR IN LINE %lu\n", i + 1);

        fclose(fw);

        free(func);
        free(args);

        FreeAssembler(&program);

        return ERROR_ASM;
    }

    return ASSEMBLED_OK;
}

Error_assembler PrintRegister(File_t program, FILE* fw, char* func, char* args, size_t i)
{
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
        printf("ERROR IN LINE %lu\n", i + 1);

        fclose(fw);

        free(func);
        free(args);

        FreeAssembler(&program);

        return ERROR_ASM;
    }

    return ASSEMBLED_OK;
}

Error_assembler Assembler(File_t program, Labels_t* labels, File_t code)
{
    Error_assembler err_asm = Put_labels_to_structure(program, labels);

    if (err_asm != ASSEMBLED_OK)
    {
        return err_asm;
    }

    FILE* fw = fopen(code.file_name, "w");

    size_t num_of_labels = amount_of_labels(program);

    for (size_t i = 0; i < program.str_num; i++)
    {
        while (program.lineslen[i] == 0)
        {
            i++;
        }

        if (program.lines[i][program.lineslen[i] - 1] == ':')
        {
            fprintf(fw, "%d\n", CMD_LABEL);
        }
        else
        {
            char* func = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            char* args = (char*)calloc(program.lineslen[i] + 1, sizeof(char));
            int arg = 0;

            program.lines[i][program.lineslen[i]] = '\0';
            int numsd = sscanf(program.lines[i], "%s %d", func, &arg);
            int numss = sscanf(program.lines[i], "%s %[^\n]", func, args);

            if (numsd == 1 && numss == 1)
            {
                if (PrintWithoutArg(program, fw, func, args,  i))
                {
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
                    printf("ERROR IN LINE %lu\n", i + 1);

                    fclose(fw);

                    free(func);
                    free(args);

                    FreeAssembler(&program);

                    return ERROR_ASM;
                }
            }
            else if (numsd == 1 && numss == 2)
            {
                if (strcmp(func, "pushr") == 0)
                {
                    fprintf(fw, "%d\n", CMD_PUSHR);

                    if (PrintRegister(program, fw, func, args, i))
                    {
                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func, "popr") == 0)
                {
                    fprintf(fw, "%d\n", CMD_POPR);

                    if (PrintRegister(program, fw, func, args, i))
                    {
                        return ERROR_ASM;
                    }
                }
                else if (strcmp(func, "ja") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JA);
                }
                else if (strcmp(func, "jae") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JAE);
                }
                else if (strcmp(func, "jb") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JB);
                }
                else if (strcmp(func, "jbe") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JBE);
                }
                else if (strcmp(func, "je") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JE);
                }
                else if (strcmp(func, "jne") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JNE);
                }
                else if (strcmp(func, "jmp") == 0)
                {
                    PrintJump(labels, args, fw, num_of_labels, CMD_JMP);
                }
                else
                {
                    fprintf(fw, "ERROR\n");
                    printf("ERROR IN LINE %lu\n", i + 1);

                    fclose(fw);

                    free(func);
                    free(args);

                    FreeAssembler(&program);

                    return ERROR_ASM;
                }
            }
            else
            {
                fprintf(fw, "ERROR\n");
                printf("ERROR IN LINE %lu\n", i + 1);

                fclose(fw);

                free(func);
                free(args);

                FreeAssembler(&program);

                return ERROR_ASM;
            }

            free(func);
            free(args);
        }
    }

    fclose(fw);

    FreeAssembler(&program);

    printf("Compilation in OK\n");

    return ASSEMBLED_OK;
}
