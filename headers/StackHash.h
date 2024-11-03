#ifndef STACK_HASH_H
#define STACK_HASH_H

#include "Constants.h"
#include "Struct.h"
#include "StackFunc.h"
#include "StackUtils.h"
#include "StackHash.h"

unsigned long Hash(char* data, size_t size);
Error_stack ChangeStackHash(Stack_t* stack);

#endif
