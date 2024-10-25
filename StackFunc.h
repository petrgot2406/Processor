#ifndef FUNC_STACK_H
#define FUNC_STACK_H

#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

Error_stack PushStack(Stack_t* stack, stack_element_t elem);
Error_stack PopStack(Stack_t* stack);
Error_stack InitStack(Stack_t* stack);
Error_stack DestroyStack(Stack_t* stack);

stack_element_t PeekStack(Stack_t stack);

#endif
