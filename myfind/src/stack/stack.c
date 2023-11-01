#include "stack.h"

struct stack *stack_push(struct stack *s, struct token *token)
{
    if (s == NULL)
        s = stack_init();

    if (s->token == NULL)
    {
        s->token = token;
        return s;
    }

    struct stack *st = calloc(1, sizeof(struct stack));

    st->next = s;
    st->token = token;

    if (s == NULL)
        return st;

    return st;
}

int is_empty(struct stack *s)
{
    if (s == NULL)
        return 1;
    return s->token == NULL ? 1 : 0;
}

struct stack *stack_init(void)
{
    struct stack *stack = calloc(1, sizeof(struct stack));
    stack->token = NULL;
    stack->next = NULL;
    return stack;
}

struct stack *stack_pop(struct stack *s)
{
    if (s == NULL)
        return NULL;
    struct stack *t = s->next;
    free(s);
    return t;
}

struct token *stack_peek(struct stack *s)
{
    if (s == NULL)
        return NULL;
    return s->token;
}
