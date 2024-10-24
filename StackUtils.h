#ifndef STACK_UTILS_H
#define STACK_UTILS_H

#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

Error_t DumpStack(Stack_t stack);
Error_t CheckStack(Stack_t* stack);
Error_t ReallocStackData(Stack_t* stack);

#endif
