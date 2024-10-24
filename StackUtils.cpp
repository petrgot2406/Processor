#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "Struct.h"
#include "StackUtils.h"
#include "StackHash.h"

Error_t DumpStack(Stack_t stack)
{
    Error_t stack_error = CheckStack(&stack);

    if (stack_error != FOUND_OK)
    {
        return stack_error;
    }

    printf("dump\n");
    printf("Capacity = %d\n", stack.capacity);
    printf("Size = %d\n", stack.size);
    printf("Data:\n");

    for (size_t i = 0; i < stack.size; i++)
    {
        printf_stack_element(i, *((stack_element_t*)(stack.data + 1) + i));
    }

    printf("\n");

    return stack_error;
}

Error_t CheckStack(Stack_t* stack)
{
    if (stack->size >= stack->capacity)
    {
        printf("ERROR IN SIZE OF STACK!\n");
        return ERROR_OVERFLOW;
    }

    if (stack->data == NULL)
    {
        printf("ERROR IN STACK!\n");
        return ERROR_DATA;
    }

    if (stack->canary_start != canary || stack->canary_end != canary)
    {
        printf("ERROR IN CANARY STACK!\n");
        return ERROR_CANARY_STACK;
    }

    if (stack->data[0] != canary ||
        *((canary_type*)((char*)stack->data +
        sizeof(canary_type) +
        stack->capacity * sizeof(stack_element_t))) != canary)
    {
        printf("ERROR IN CANARY DATA!\n");
        return ERROR_CANARY_DATA;
    }

    if (Hash((char*)stack,
        sizeof(canary_type) +
        sizeof(canary_type*) +
        2 * sizeof(size_t)) !=
        stack->hash_struct.stack_hash)
    {
        printf("ERROR IN HASH STACK!\n");
        return ERROR_HASH_STACK;
    }

    if (Hash((char*)stack->data,
             2 * sizeof(canary_type) +
             stack->capacity * sizeof(stack_element_t)) !=
             stack->hash_struct.data_hash)
    {
        printf("ERROR IN HASH DATA!\n");
        return ERROR_HASH_DATA;
    }

    return FOUND_OK;
}

Error_t ReallocStackData(Stack_t* stack)
{
    stack->data = (canary_type*)realloc(stack->data,
                                      sizeof(canary_type) * 2 +
                                      stack->capacity * sizeof(stack_element_t));

    stack->data[0] = canary;

    *((canary_type*)((char*)stack->data +
                     sizeof(canary_type) +
                     stack->capacity * sizeof(stack_element_t))) = canary;

    ChangeStackHash(stack);

    Error_t stack_error = CheckStack(stack);

    if (stack_error != FOUND_OK)
    {
        return stack_error;
    }

    return stack_error;
}
