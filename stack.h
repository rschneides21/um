
#ifndef STACK_H
#define STACK_H

#include "stdbool.h"

typedef struct Stack_T *Stack_T;

extern unsigned Stack_pop(Stack_T stack);
extern Stack_T Stack_push(Stack_T stack, unsigned val);
extern bool Stack_is_empty(Stack_T stack);
extern Stack_T Stack_init(int max_size);
extern void Stack_free(Stack_T stack);

#endif