#include <ast.h>

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
    token->pre = 4;
    return token;
}

static struct token *create_print_token(void)
{
    struct token *token = calloc(1, sizeof(struct token));
    token->type = ACTION_PRINT;
    token->func = print;
    token->pre = 1;
    return token;
}

static void process_operator(struct stack **operators_stack,
                             struct stack **operands_stack)
{
    struct node *top_node = stack_pop_value(operators_stack);
    struct node *right = stack_pop_value(operands_stack);
    struct node *left = stack_pop_value(operands_stack);

    if (left == NULL || right == NULL)
        exit_with(1, "ast.c:47 wrong input format");

    top_node->left = left;
    top_node->right = right;
    *operands_stack = stack_push(*operands_stack, top_node);
}

static struct node *get_top_node(struct tokens *tokens,
                                 struct stack **operands_stack)
{
    if (stack_peek(*operands_stack) == NULL)
        return create_node(create_print_token());

    bool exist = false;
    for (unsigned i = 0; i < tokens->length; i++)
        if (is_action(tokens->data[i]))
            exist = true;

    if (!exist)
    {
        struct node *and_node = create_node(create_and_token());

        struct node *right = create_node(create_print_token());
        struct node *left = stack_pop_value(operands_stack);

        and_node->left = left;
        and_node->right = right;

        return and_node;
    }

    return stack_peek(*operands_stack);
}

struct node *check_top_operator(struct stack *operators_stack)
{
    if (stack_peek(operators_stack) == NULL)
        exit_with(1, "missing '(' in input");
    return stack_peek(operators_stack);
}

struct node *process_end_of_ast(struct stack **operators_stack,
                                struct stack **operands_stack,
                                struct tokens *tokens)
{
    while (stack_peek(*operators_stack) != NULL)
        if (stack_peek(*operators_stack)->token->type == L_PARENT)
            exit_with(1, "too many ')' in input");
        else
            process_operator(operators_stack, operands_stack);

    struct node *top_node = get_top_node(tokens, operands_stack);
    free(*operators_stack);
    free(*operands_stack);
    return top_node;
}

struct node *build_ast(struct tokens *tokens)
{
    struct stack *operators_stack = stack_init();
    struct stack *operands_stack = stack_init();

    for (unsigned i = 0; tokens != NULL && i < tokens->length; i++)
    {
        struct token *token = copy_token(tokens->data[i]);

        if (token->type == L_PARENT)
            operators_stack = stack_push(operators_stack, create_node(token));
        else if (token->type == R_PARENT)
        {
            struct node *top_node;
            while ((top_node = check_top_operator(operators_stack)) != NULL
                   && top_node->token->type != L_PARENT)
                process_operator(&operators_stack, &operands_stack);

            if (top_node != NULL && top_node->token->reversed)
                stack_peek(operands_stack)->token->reversed = true;

            if (stack_peek(operators_stack)->token->type != L_PARENT)
                exit_with(1, "111: missing ')' in input");

            operators_stack = stack_pop(operators_stack);
            free(token);
        }
        else if (!is_operator(token))
        {
            operands_stack = stack_push(operands_stack, create_node(token));
            if (tokens->data[i + 1] != NULL && !is_operator(tokens->data[i + 1])
                && !is_parenthesis(tokens->data[i + 1]))
                operators_stack = stack_push(operators_stack,
                                             create_node(create_and_token()));
        }
        else
        {
            while (stack_peek(operators_stack) != NULL
                   && (stack_peek(operators_stack)->token->pre > token->pre))
                process_operator(&operators_stack, &operands_stack);
            operators_stack = stack_push(operators_stack, create_node(token));
        }
    }

    return process_end_of_ast(&operators_stack, &operands_stack, tokens);
}

static bool revert_bool(bool original, bool reversed)
{
    return reversed == !original;
}

bool evaluate(struct node *tree, struct file file)
{
    struct token *token = tree->token;

    if (!file.print)
    {
        if (is_operator(token))
        {
            evaluate(tree->left, file);
            evaluate(tree->right, file);
            return true;
        }
        else
            return token->func(token, file);
    }
    else if (token->type == OPERATOR_OR)
        return revert_bool(evaluate(tree->left, file)
                               || evaluate(tree->right, file),
                           token->reversed);
    else if (token->type == OPERATOR_AND)
        return revert_bool(evaluate(tree->left, file)
                               && evaluate(tree->right, file),
                           token->reversed);
    else
        return token->func(token, file);
}
