#ifndef STRUCT_H
#define STRUCT_H

#include "Constants.h"

struct HashStruct
{
    unsigned long stack_hash;
    unsigned long data_hash;
};

struct Stack_t
{
    canary_type canary_start;
    canary_type* data;
    size_t capacity;
    size_t size;
    HashStruct hash_struct;
    canary_type canary_end;
};

struct Labels_t
{
    size_t ip;
    char* name;
};

struct Table_labels
{
    Labels_t* array;
    size_t size;
    size_t max_len;
};

struct File_t
{
    const char* file_name;
    size_t file_size;
    size_t str_num;
    char* buffer;
    size_t* lineslen;
    char** lines;
};

struct Registers
{
    int ax;
    int bx;
    int cx;
    int dx;
};

struct SPU
{
    Stack_t stack;
    File_t code;
    Labels_t labels;
    Registers registers;
};

#endif
