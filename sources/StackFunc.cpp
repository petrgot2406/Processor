#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../headers/Constants.h"
#include "../headers/Struct.h"
#include "../headers/StackFunc.h"
#include "../headers/StackUtils.h"
#include "../headers/StackHash.h"

Error_stack PushStack(Stack_t* stack, stack_element_t new_elem)
{
    Error_stack stack_error = CheckStack(stack);

    if (stack_error != FOUND_OK)
    {
        return stack_error;
    }

    if (stack->size + 2 > stack->capacity)
    {
        stack->capacity *= 2;
        ReallocStackData(stack);
    }

    *((stack_element_t*)(stack->data + 1) + stack->size) = new_elem;

    stack->size++;

    ChangeStackHash(stack);

    return stack_error;
}

Error_stack PopStack(Stack_t* stack)
{
    Error_stack stack_error = CheckStack(stack);

    if (stack_error != FOUND_OK)
    {
        return stack_error;
    }

    if (4 * (stack->size - 2) < stack->capacity &&
                  stack->size >= 3)
    {
        stack->capacity /= 2;
        ReallocStackData(stack);
    }

    *((stack_element_t*)(stack->data + 1) + stack->size) = 0;

    stack->size--;

    ChangeStackHash(stack);

    return stack_error;
}

Error_stack InitStack(Stack_t* stack)
{
    if (stack == NULL)
    {
        printf("ERROR IN ADDRESS OF STACK!\n");
        return ERROR_ADDRESS;
    }

    stack->canary_start = canary;
    stack->canary_end = canary;

    stack->capacity = 8;
    stack->size = 0;

    stack->data = (canary_type*)calloc(sizeof(canary_type) * 2 +
                                       stack->capacity *
                                       sizeof(stack_element_t), 1);

    stack->data[0] = canary;
    *((canary_type*)((char*)stack->data +
                     sizeof(canary_type) +
                     stack->capacity *
                     sizeof(stack_element_t))) = canary;

    ChangeStackHash(stack);

    return FOUND_OK;
}

Error_stack DestroyStack(Stack_t* stack)
{
    Error_stack stack_error = CheckStack(stack);

    if (stack_error != FOUND_OK)
    {
        return stack_error;
    }

    stack->capacity = 0;
    stack->size = 0;
    stack->hash_struct.stack_hash = 0;
    stack->hash_struct.data_hash = 0;

    free(stack->data);

    return stack_error;
}

stack_element_t PeekStack(Stack_t stack)
{
    assert(stack.size > 0);

    return *((stack_element_t*)(stack.data + 1) +
             stack.size - 1);
}
