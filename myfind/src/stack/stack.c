#include "stack.h"

struct stack *stack_push(struct stack *s, struct node *node)
{
    if (s == NULL)
        s = stack_init();

    if (s->node == NULL)
    {
        s->node = node;
        return s;
    }

    struct stack *st = calloc(1, sizeof(struct stack));

    st->next = s;
    st->node = node;

    if (s == NULL)
        return st;

    return st;
}

int is_empty(struct stack *s)
{
    if (s == NULL)
        return 1;
    return s->node == NULL ? 1 : 0;
}

struct stack *stack_init(void)
{
    struct stack *stack = calloc(1, sizeof(struct stack));
    stack->node = NULL;
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

struct stack *stack_pop_tail(struct stack *s)
{
    if (s == NULL)
        return NULL;
    if (s->next == NULL)
    {
        free(s);
        return NULL;
    }
    struct stack *t = s;
    while (t->next->next != NULL)
        t = t->next;
    free(t->next);
    t->next = NULL;
    return s;
}

struct node *stack_peek(struct stack *s)
{
    if (s == NULL)
        return NULL;
    return s->node;
}

struct node *stack_peek_tail(struct stack *s)
{
    if (s == NULL)
        return NULL;
    while (s->next != NULL)
        s = s->next;
    return s->node;
}
