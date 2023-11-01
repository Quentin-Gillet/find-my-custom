#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdlib.h>

#include "../structures.h"

struct stack
{
    struct token *token;
    struct stack *next;
};

// STACK TOKEN
struct stack *stack_push(struct stack *s, struct token *token);
struct stack *stack_pop(struct stack *s);
struct token *stack_peek(struct stack *s);
struct stack *stack_init(void);
int is_empty(struct stack *s);

#endif /* !STACK_H */
