#include <freememory.h>

static void free_args(struct args_input *args)
{
    if (args == NULL)
        return;
    free(args->expression);
    free(args->options);
    free(args->entries_points->entries_point);
    free(args->entries_points);
    free(args);
}

static void free_token(struct token *token, bool free_args)
{
    if (token == NULL)
        return;
    if (token->type == ACTION_EXEC || token->type == ACTION_EXECDIR)
    {
        for (int i = 0; token->value.args[i] != NULL; i++)
            free(token->value.args[i]);
        free(token->value.args);
    }
    else if (token->value.param != NULL && free_args)
        free(token->value.param);
    free(token);
}

static void free_tokens(struct tokens *tokens)
{
    if (tokens == NULL)
        return;
    for (unsigned i = 0; i < tokens->length; i++)
        free_token(tokens->data[i], false);
    free(tokens->data);
    free(tokens);
}

static void free_ast(struct node *ast)
{
    if (ast == NULL)
        return;
    free_ast(ast->left);
    free_ast(ast->right);
    free_token(ast->token, true);
    free(ast);
}

void free_all(struct node *ast, struct tokens *tokens, struct args_input *args)
{
    free_ast(ast);
    free_tokens(tokens);
    free_args(args);
}
