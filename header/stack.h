#ifndef STATC_H
#define STATC_H

#include <stdlib.h>
#include "linkedlist.h"
#include <stdint.h>

#define STACK_SUCCESS 0

#define STACK_OVERFLOW 1
#define STACK_UNDERFLOW 2
#define STACK_MAXCOUNT 100

struct stack
{
    size_t unit_size;
    int16_t count;
    struct linkedlist *ll;
};

struct stack *createStack(size_t unit_size);
int push(struct stack *stack, void *value);
int pop(struct stack *stack, void *out);
void freeStack(struct stack *stack);
#endif
