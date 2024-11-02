#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>

#define printf_stack_element(number, element) printf("stack[%u] = %d\n", number, element)

#define PROGRAM_FILE_NAME  "program.asm"
#define CODE_FILE_NAME  "code.asm"

typedef long long int canary_type;
typedef int stack_element_t;

const canary_type canary = 0xDEDDEDDEDDEDF;

enum Error_stack
{
    FOUND_OK           = 0,
    ERROR_ADDRESS      = 1 << 0,
    ERROR_OVERFLOW     = 1 << 1,
    ERROR_DATA         = 1 << 2,
    ERROR_CANARY_STACK = 1 << 3,
    ERROR_CANARY_DATA  = 1 << 4,
    ERROR_HASH_ADDRESS = 1 << 5,
    ERROR_HASH_STACK   = 1 << 6,
    ERROR_HASH_DATA    = 1 << 7
};

enum Error_assembler
{
    ASSEMBLED_OK = 0,
    ERROR_ASM    = 1
};

enum Error_processor
{
    PROCESSED_OK  = 0,
    ERROR_PROCESS = 1
};

enum Commands
{
    CMD_LABEL = 0,
    CMD_PUSH  = 1,
    CMD_ADD   = 2,
    CMD_SUB   = 3,
    CMD_MUL   = 4,
    CMD_DIV   = 5,
    CMD_OUT   = 6,
    CMD_IN    = 7,
    CMD_POP   = 8,
    CMD_JA    = 9,
    CMD_JAE   = 10,
    CMD_JB    = 11,
    CMD_JBE   = 12,
    CMD_JE    = 13,
    CMD_JNE   = 14,
    CMD_JMP   = 15,
    CMD_HLT   = 16
};

enum Registers_t
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3
};

#endif
