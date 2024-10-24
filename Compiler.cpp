#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

size_t num_of_symbols_in_file(const char* file_name);
size_t num_of_strings_in_file(const char* file_name);
void Read_file_to_buffer(const char* file_name, size_t file_size, char* buffer);
void Put_lineslen_for_all_lines(char* buffer, size_t file_size, size_t str_num, size_t* lineslen);
void Put_pointers_to_lines(char* buffer, size_t file_size, size_t str_num, char** lines);

int main()
{
    const char* program_file_name = "program.asm";
    const char* code_file_name = "code.asm";
    FILE* fr = fopen(program_file_name, "r");
    FILE* fw = fopen(code_file_name, "w");

    size_t file_size = num_of_symbols_in_file(program_file_name);
    size_t str_num = num_of_strings_in_file(program_file_name);

    char* buffer = (char*)calloc(file_size, sizeof(char));

    size_t* lineslen = (size_t*)calloc(str_num, sizeof(size_t));

    char** lines = (char**)calloc(str_num + 1, sizeof(char*));

    Read_file_to_buffer(program_file_name, file_size, buffer);
    Put_lineslen_for_all_lines(buffer, file_size, str_num, lineslen);
    Put_pointers_to_lines(buffer, file_size, str_num, lines);

    char* word = strtok(buffer, " \r\n");

    while (word != NULL)
    {
        if (strcmp(word, "push") == 0)
        {
            fprintf(fw, "%d\n", CMD_PUSH);
        }
        if (strcmp(word, "add") == 0)
        {
            fprintf(fw, "%d\n", CMD_ADD);
        }
        if (strcmp(word, "sub") == 0)
        {
            fprintf(fw, "%d\n", CMD_SUB);
        }
        if (strcmp(word, "mul") == 0)
        {
            fprintf(fw, "%d\n", CMD_MUL);
        }
        if (strcmp(word, "div") == 0)
        {
            fprintf(fw, "%d\n", CMD_DIV);
        }
        if (strcmp(word, "out") == 0)
        {
            fprintf(fw, "%d\n", CMD_OUT);
        }
        if (strcmp(word, "in") == 0)
        {
            fprintf(fw, "%d\n", CMD_IN);
        }
        else
        {
            return 1;
        }
        word = strtok(buffer, " \r\n");
    }

    fclose(fr);
    fclose(fw);
    return 0;
}

size_t num_of_symbols_in_file(const char* file_name)
{
    assert(file_name != NULL);

    FILE *fptr = fopen(file_name, "r");
    size_t counter = 0;
    while (getc(fptr) != EOF)
    {
        counter++;
    }
    fclose(fptr);
    return counter;
}

size_t num_of_strings_in_file(const char* file_name)
{
    assert(file_name != NULL);

    FILE *fptr = fopen(file_name, "r");
    size_t counter = 0;
    int ch = getc(fptr);
    while (ch != EOF)
    {
        if (ch == '\n')
        {
            counter++;
        }
        ch = getc(fptr);
    }
    fclose(fptr);
    return counter;
}

void Read_file_to_buffer(const char* file_name, size_t file_size, char* buffer)
{
    assert(file_name != NULL);

    buffer = (char*)calloc(file_size, sizeof(char));

    FILE* fptr = fopen(file_name, "r");
    if (fptr)
    {
        fread(buffer, sizeof(char), file_size, fptr);
        fclose(fptr);
    }
}

void Put_lineslen_for_all_lines(char* buffer, size_t file_size, size_t str_num, size_t* lineslen)
{
    assert(buffer != NULL);

    lineslen = (size_t*)calloc(str_num, sizeof(size_t));

    size_t num_of_the_str = 0;
    size_t count_sym_in_str = 0;
    for (size_t i = 0; i < file_size; i++)
    {
        count_sym_in_str++;
        if (buffer[i] == '\n' ||
            buffer[i] == '\0' ||
            buffer[i] == EOF)
        {
            lineslen[num_of_the_str] = count_sym_in_str - 1;
            num_of_the_str++;
            count_sym_in_str = 0;
        }
    }
}

void Put_pointers_to_lines(char* buffer, size_t file_size, size_t str_num, char** lines)
{
    assert(buffer != NULL);

    lines = (char**)calloc(str_num + 1, sizeof(char*));

    size_t num_of_the_str = 1;
    lines[0] = &buffer[0];
    for (size_t i = 1; i < file_size; i++)
    {
        if (buffer[i] == '\n')
        {
            assert(num_of_the_str <= str_num);

            lines[num_of_the_str] = &buffer[i + 1];
            num_of_the_str++;
        }
    }
}
