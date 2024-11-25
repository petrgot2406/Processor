#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../headers/Constants.h"
#include "../headers/Struct.h"
#include "../headers/ReadFromFile.h"

size_t num_of_symbols_in_file(File_t file)
{
    assert(file.file_name != NULL);

    FILE *fptr = fopen(file.file_name, "r");
    size_t counter = 0;
    while (getc(fptr) != EOF)
    {
        counter++;
    }
    fclose(fptr);
    return counter;
}

size_t num_of_strings_in_file(File_t file)
{
    assert(file.file_name != NULL);

    FILE *fptr = fopen(file.file_name, "r");
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

void Init_file(File_t* file)
{
    file->file_size = num_of_symbols_in_file(*file);
    file->str_num   = num_of_strings_in_file(*file);

    file->buffer    = (char*)calloc(file->file_size + 1, sizeof(char));

    file->lineslen  = (size_t*)calloc(file->str_num, sizeof(size_t));

    file->lines     = (char**)calloc(file->str_num + 1, sizeof(char*));
}

void Read_file_to_buffer(File_t* file)
{
    assert(file->file_name != NULL);

    FILE* fptr = fopen(file->file_name, "r");
    if (fptr)
    {
        fread(file->buffer, sizeof(char), file->file_size, fptr);
        fclose(fptr);
    }
}

void Put_lineslen_for_all_lines(File_t* file)
{
    assert(file->buffer != NULL);

    size_t num_of_the_str = 0;
    size_t count_sym_in_str = 0;
    for (size_t i = 0; i < file->file_size; i++)
    {
        count_sym_in_str++;
        if (file->buffer[i] == '\n' ||
            file->buffer[i] == '\0' ||
            file->buffer[i] == '\r' ||
            file->buffer[i] == EOF)
        {
            file->lineslen[num_of_the_str] = count_sym_in_str - 1;
            num_of_the_str++;
            count_sym_in_str = 0;
        }
    }
}

void Put_pointers_to_lines(File_t* file)
{
    assert(file->buffer != NULL);

    size_t num_of_the_str = 1;
    file->lines[0] = &file->buffer[0];
    for (size_t i = 1; i < file->file_size; i++)
    {
        if (file->buffer[i] == '\n')
        {
            assert(num_of_the_str <= file->str_num);

            file->lines[num_of_the_str] = &file->buffer[i + 1];
            num_of_the_str++;
        }
    }
}

void Put_file_to_structure(File_t* file)
{
    Init_file(file);

    FILE* fr = fopen(file->file_name, "r");

    Read_file_to_buffer(file);
    Put_lineslen_for_all_lines(file);
    Put_pointers_to_lines(file);

    fclose(fr);
}
