#ifndef FUNC_STACK_H
#define FUNC_STACK_H

#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

Error_t PushStack(Stack_t* stack, stack_element_t elem);
Error_t PopStack(Stack_t* stack);
Error_t InitStack(Stack_t* stack);
Error_t DestroyStack(Stack_t* stack);

#endif
