#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Constants.h"
#include "Struct.h"
#include "ReadFromFile.h"

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

    FILE* fptr = fopen(file_name, "r");
    if (fptr)
    {
        fread(buffer, sizeof(char), file_size, fptr);
        fclose(fptr);
    }
}

void Put_lineslen_for_all_lines(char* buffer, size_t file_size, size_t* lineslen)
{
    assert(buffer != NULL);

    size_t num_of_the_str = 0;
    size_t count_sym_in_str = 0;
    for (size_t i = 0; i < file_size; i++)
    {
        count_sym_in_str++;
        if (buffer[i] == '\n' ||
            buffer[i] == '\0' ||
            buffer[i] == '\r' ||
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
