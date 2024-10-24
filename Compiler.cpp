#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

size_t num_of_symbols_in_file(const char* input_file_name);
void Read_file_to_buffer(const char* input_file_name, size_t file_size, char* buffer);

int main()
{
    const char* program_file_name = "program.asm";
    const char* code_file_name = "code.asm";
    FILE* fr = fopen(program_file_name, "r");
    FILE* fw = fopen(code_file_name, "w");

    size_t file_size = num_of_symbols_in_file(program_file_name);
    char* buffer = (char*)calloc(file_size, sizeof(char));
    Read_file_to_buffer(program_file_name, file_size, buffer);

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

size_t num_of_symbols_in_file(const char* input_file_name)
{
    assert(input_file_name != NULL);

    FILE *fptr = fopen(input_file_name, "r");
    size_t counter = 0;
    while (getc(fptr) != EOF)
    {
        counter++;
    }
    fclose(fptr);
    return counter;
}

void Read_file_to_buffer(const char* input_file_name, size_t file_size, char* buffer)
{
    assert(input_file_name != NULL);

    buffer = (char*)calloc(file_size, sizeof(char));

    FILE* fptr = fopen(input_file_name, "r");
    if (fptr)
    {
        fread(buffer, sizeof(char), file_size, fptr);
        fclose(fptr);
    }
}
