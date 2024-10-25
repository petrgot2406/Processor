#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "Struct.h"
#include "StackHash.h"

unsigned long Hash(char* data, size_t size)
{
    unsigned long hash = 1234;
    char* str = data;
    char c = 0;

    for (size_t i = 0; i < size; i++)
    {
        c = str[i];
        hash = ~(hash << 3) ^ c;
    }

    return hash;
}

Error_stack ChangeStackHash(Stack_t* stack)
{
    if (stack == NULL)
    {
        printf("ERROR IN HASH ADDRESS!\n");
        return ERROR_HASH_ADDRESS;
    }

    stack->hash_struct.stack_hash = Hash((char*)stack,
                                       sizeof(canary_type) +
                                       sizeof(canary_type*) +
                                       2 * sizeof(size_t));

    stack->hash_struct.data_hash = Hash((char*)stack->data,
                                      2 * sizeof(canary_type) +
                                      stack->capacity * sizeof(stack_element_t));

    return FOUND_OK;
}
