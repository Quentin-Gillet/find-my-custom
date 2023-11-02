#include "tokens.h"

static bool is_terminator(const char *str)
{
    return strcmp(str, ";") == 0 || strcmp(str, "';'") == 0
        || strcmp(str, "\\;") == 0;
}

static void set_exec_value(struct token *token, struct args_input *args)
{
    unsigned start_index = args->expression_index;
    while (args->expression[args->expression_index] != NULL
           && !is_terminator(args->expression[args->expression_index]))
        args->expression_index++;
    char **data_cpy =
        calloc(args->expression_index - start_index + 1, sizeof(char *));
    for (int i = 0; start_index < args->expression_index + 1;
         i++, start_index++)
        data_cpy[i] = args->expression[start_index];
    token->value.args = data_cpy;
}

static void add_token(struct token *token, struct tokens *tokens)
{
    if (tokens->capacity == 0 || tokens->length == tokens->capacity)
    {
        tokens->capacity = tokens->capacity == 0 ? 10 : tokens->capacity * 2;
        tokens->data =
            realloc(tokens->data, tokens->capacity * sizeof(struct token *));
        if (tokens->data == NULL)
            exit_with(1, "token.c:41 realloc failed");
    }

    tokens->data[tokens->length] = token;
    if (token != NULL)
        tokens->length++;
}

static void set_simple_value(struct token *token, struct args_input *args)
{
    token->value.param = args->expression[args->expression_index];
}

static struct token *get_token_from_symbol(const char *symbol,
                                           struct args_input *args)
{
    static struct token_model models[] = {
        { "-o", OPERATOR_OR, 3, NULL, NULL },
        { "-a", OPERATOR_AND, 4, NULL, NULL },
        { "'!'", OPERATOR_NOT, 5, NULL, NULL },
        { "'('", OPERATOR_L_PARENTHESIS, 6, NULL, NULL },
        { "')'", OPERATOR_R_PARENTHESIS, 6, NULL, NULL },

        { "-delete", ACTION_DELETE, 1, NULL, NULL },
        { "-print", ACTION_PRINT, 0, NULL, print },
        { "-exec", ACTION_EXEC, 1, set_exec_value, exec },

        { "-name", TEST_NAME, 1, set_simple_value, &name },
        { "-type", TEST_TYPE, 1, set_simple_value, type },
        { "-newer", TEST_NEWER, 1, NULL, NULL },
    };

    struct token *token = calloc(1, sizeof(struct token));
    token->type = VALUE;

    for (unsigned long i = 0; i < sizeof models / sizeof *models; i++)
    {
        if (strcmp(symbol, models[i].symbol) == 0)
        {
            token->type = models[i].type;
            token->precedence = models[i].precedence;
            if (models[i].set_value != NULL)
            {
                args->expression_index++;
                models[i].set_value(token, args);
            }
            token->reversed = false;
            token->func = models[i].func;
        }
    }
    return token;
}

static void add_print_if_necessary(struct tokens *tokens,
                                   struct args_input *args)
{
    bool exist = false;
    for (unsigned i = 0; i < tokens->length; i++)
        if (tokens->data[i]->type == ACTION_PRINT)
            exist = true;

    if (!exist)
        add_token(get_token_from_symbol("-print", args), tokens);
}

bool is_operator(struct token *token)
{
    return token->type == OPERATOR_OR || token->type == OPERATOR_AND
        || token->type == OPERATOR_NOT;
}

struct tokens *parse_tokens(struct args_input *args)
{
    if (args->expression == NULL)
        return NULL;

    struct tokens *tokens = calloc(1, sizeof(struct tokens));
    if (tokens == NULL)
        exit_with(1, "token.c:109 calloc error");

    for (; args->expression[args->expression_index]; args->expression_index++)
    {
        struct token *token = get_token_from_symbol(
            args->expression[args->expression_index], args);
        add_token(token, tokens);
    }
    add_print_if_necessary(tokens, args);
    add_token(NULL, tokens);
    return tokens;
}
