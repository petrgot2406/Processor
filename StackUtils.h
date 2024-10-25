#ifndef STACK_UTILS_H
#define STACK_UTILS_H

#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

Error_stack DumpStack(Stack_t stack);
Error_stack CheckStack(Stack_t* stack);
Error_stack ReallocStackData(Stack_t* stack);

#endif
