#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>

#define printf_stack_element(number, element) printf("stack[%d] = %d\n", number, element)

typedef long long int canary_type;
typedef int stack_element_t;

const canary_type canary = 0xDEDDEDDEDDEDDEDF;

enum Error_t
{
    FOUND_OK = 0,
    ERROR_ADDRESS = 1,
    ERROR_OVERFLOW = 1 << 1,
    ERROR_DATA = 1 << 2,
    ERROR_CANARY_STACK = 1 << 3,
    ERROR_CANARY_DATA = 1 << 4,
    ERROR_HASH_ADDRESS = 1 << 5,
    ERROR_HASH_STACK = 1 << 6,
    ERROR_HASH_DATA = 1 << 7
};

enum Commands
{
    CMD_PUSH = 1,
    CMD_ADD = 2,
    CMD_SUB = 3,
    CMD_MUL = 4,
    CMD_DIV = 5,
    CMD_OUT = 6,
    CMD_IN = 7
};

enum Error_assembler
{
    ASSEMBLE_OK = 0,
    ERROR_ASM = 1
};


#endif
