#include "ast.h"

static struct node *create_node(struct token *token)
{
    struct node *node = calloc(1, sizeof(struct node));
    if (node == NULL)
        exit_with(1, "ast.c:7 calloc failed");
    node->token = token;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static struct node *stack_pop_value(struct stack **stack)
{
    struct node *node = stack_peek(*stack);
    *stack = stack_pop(*stack);
    return node;
}

static struct token *create_and_token(void)
{
    struct token *token = calloc(1, sizeof(struct token));
    token->type = OPERATOR_AND;
    token->precedence = 4;
    return token;
}

static struct token *create_print_token(void)
{
    struct token *token = calloc(1, sizeof(struct token));
    token->type = ACTION_PRINT;
    token->func = print;
    token->precedence = 1;
    return token;
}

static void process_operator(struct stack **operators_stack,
                             struct stack **operands_stack)
{
    struct node *top_node = stack_pop_value(operators_stack);
    struct node *right = stack_pop_value(operands_stack);
    struct node *left = stack_pop_value(operands_stack);

    if (left == NULL || right == NULL)
        exit_with(1, "ast.c:29 wrong input format");

    top_node->left = left;
    top_node->right = right;
    *operands_stack = stack_push(*operands_stack, top_node);
}

struct node *build_ast(struct tokens *tokens)
{
    struct stack *operators_stack = stack_init();
    struct stack *operands_stack = stack_init();

    for (unsigned i = 0; tokens != NULL && i < tokens->length; i++)
    {
        struct token *token = tokens->data[i];
        if (!is_operator(token))
        {
            operands_stack = stack_push(operands_stack, create_node(token));
            if (tokens->data[i + 1] != NULL
                && !is_operator(tokens->data[i + 1]))
                operators_stack = stack_push(operators_stack,
                                             create_node(create_and_token()));
        }
        else
        {
            while (stack_peek(operators_stack) != NULL
                   && (stack_peek(operators_stack)->token->precedence
                       > token->precedence))
                process_operator(&operators_stack, &operands_stack);
            operators_stack = stack_push(operators_stack, create_node(token));
        }
    }

    while (stack_peek(operators_stack) != NULL)
        process_operator(&operators_stack, &operands_stack);
    struct node *top_node = stack_peek(operands_stack);

    if (top_node == NULL)
        top_node = create_node(create_print_token());

    free(operands_stack);
    free(operators_stack);
    return top_node;
}

bool evaluate(struct node *tree, struct file file)
{
    if (tree->token->type == OPERATOR_OR)
        return evaluate(tree->left, file) || evaluate(tree->right, file);
    else if (tree->token->type == OPERATOR_AND)
        return evaluate(tree->left, file) && evaluate(tree->right, file);
    else
        return tree->token->func(tree->token, file);
}
