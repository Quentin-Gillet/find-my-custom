#include "tokens.h"

static void set_exec_value(struct token *token, struct args_input *args)
{
    unsigned start_index = args->expression_index;
    while (args->expression[args->expression_index] != NULL
           && strcmp(args->expression[args->expression_index], "';'") != 0)
        args->expression_index++;
    char** data_cpy =
        calloc(args->expression_index++ - start_index + 1, sizeof(char *));
    memmove(data_cpy, args->expression[start_index],
            (args->expression_index++ - start_index) * sizeof(char *));
    token->value.args = data_cpy;
}

static void set_name_value(struct token *token, struct args_input *args)
{
    token->value.str = args->expression[args->expression_index];
}

static struct token *get_token_from_symbol(const char *symbol,
                                           struct args_input *args)
{
    static struct token_model models[] = {
        { "-o", OPERATOR_OR, 3, NULL },
        { "-a", OPERATOR_AND, 4, NULL },
        { "'!'", OPERATOR_NOT, 5, NULL },
        { "'('", OPERATOR_L_PARENTHESIS, 6, NULL },
        { "')'", OPERATOR_R_PARENTHESIS, 6, NULL },

        { "-delete", ACTION_DELETE, 1, NULL },
        { "-print", ACTION_PRINT, 1, NULL },
        { "-exec", ACTION_EXEC, 1, set_exec_value },

        { "-name", TEST_NAME, 1, set_name_value },
        { "-newer", TEST_NEWER, 1, NULL },
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
        }
    }
    return token;
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
    tokens->length++;
}

struct tokens *parse_tokens(struct args_input *args)
{
    struct tokens *tokens = calloc(1, sizeof(struct tokens));
    if (tokens == NULL)
        exit_with(1, "token.c:52 calloc error");

    for (; args->expression[args->expression_index]; args->expression_index++)
    {
        struct token *token = get_token_from_symbol(
            args->expression[args->expression_index], args);
        add_token(token, tokens);
    }

    return tokens;
}
