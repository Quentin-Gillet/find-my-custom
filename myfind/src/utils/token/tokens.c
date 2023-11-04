#include <tokens.h>

static bool is_terminator(const char *str)
{
    return strcmp(str, ";") == 0 || strcmp(str, "';'") == 0
        || strcmp(str, "\\;") == 0;
}

static void set_exec_value(struct token *token, struct args_input *args)
{
    args->expression_index++;
    unsigned start_index = args->expression_index;
    while (args->expression[args->expression_index] != NULL
           && !is_terminator(args->expression[args->expression_index]))
        args->expression_index++;

    char **data_cpy =
        calloc(args->expression_index - start_index + 2, sizeof(char *));

    int i = 0;
    for (; start_index < args->expression_index; i++, start_index++)
    {
        data_cpy[i] =
            calloc(strlen(args->expression[start_index]) + 1, sizeof(char));
        strcpy(data_cpy[i], args->expression[start_index]);
    }

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
            exit_with(1, "token.c:34 realloc failed");
    }

    tokens->data[tokens->length] = token;
    if (token != NULL)
        tokens->length++;
}

static void set_simple_value(struct token *token, struct args_input *args)
{
    args->expression_index++;
    if (args->expression[args->expression_index] == NULL)
        exit_with(1, "token.c:45 missing argument for %s",
                  args->expression[args->expression_index - 1]);
    token->value.param = args->expression[args->expression_index];
}

static void set_delete(__attribute__((unused)) struct token *token,
                       struct args_input *args)
{
    args->options->post_order = true;
}

struct token *copy_token(struct token *token)
{
    struct token *new_token = calloc(1, sizeof(struct token));
    if (new_token == NULL)
        exit_with(1, "token.c:53 calloc failed");

    new_token->type = token->type;
    new_token->pre = token->pre;
    new_token->reversed = token->reversed;
    new_token->func = token->func;

    if (token->type == ACTION_EXEC || token->type == ACTION_EXECDIR)
    {
        int len = 0;
        for (; token->value.args[len]; len++)
            ;
        new_token->value.args = calloc(len + 1, sizeof(char *));
        if (new_token->value.args == NULL)
            exit_with(1, "token.c:67 calloc failed");
        for (int i = 0; token->value.args[i]; i++)
        {
            new_token->value.args[i] =
                calloc(strlen(token->value.args[i]) + 1, sizeof(char));
            if (new_token->value.args[i] == NULL)
                exit_with(1, "token.c:73 calloc failed");
            strcpy(new_token->value.args[i], token->value.args[i]);
        }
    }
    else if (token->value.param != NULL)
    {
        new_token->value.param =
            calloc(strlen(token->value.param) + 1, sizeof(char));
        if (new_token->value.param == NULL)
            exit_with(1, "token.c:92 calloc failed");
        strcpy(new_token->value.param, token->value.param);
    }
    return new_token;
}

struct token_model *get_token_model(const char *symbol)
{
    if (symbol == NULL || strcmp(symbol, "") == 0)
        return NULL;

    static struct token_model models[] = {
        { "-o", OPERATOR_OR, 1, NULL, NULL },
        { "-a", OPERATOR_AND, 2, NULL, NULL },
        { "!", OPERATOR_NOT, 0, NULL, NULL },
        { "(", L_PARENT, 0, NULL, NULL },
        { ")", R_PARENT, 0, NULL, NULL },

        { "-delete", ACTION_DELETE, 0, set_delete, delete_action },
        { "-print", ACTION_PRINT, 0, NULL, print },
        { "-exec", ACTION_EXEC, -1, set_exec_value, exec },
        { "-execdir", ACTION_EXECDIR, -1, set_exec_value, execdir },

        { "-name", TEST_NAME, 1, set_simple_value, name },
        { "-type", TEST_TYPE, 1, set_simple_value, type },
        { "-newer", TEST_NEWER, 1, set_simple_value, newer },
        { "-perm", TEST_PERM, 1, set_simple_value, perm },
        { "-user", TEST_USER, 1, set_simple_value, user },
        { "-group", TEST_GROUP, 1, set_simple_value, group },
    };

    for (unsigned long i = 0; i < sizeof models / sizeof *models; i++)
    {
        if (strcmp(symbol, models[i].symbol) == 0)
            return &models[i];
    }
    return NULL;
}

static struct token *get_token_from_symbol(const char *symbol,
                                           struct args_input *args)
{
    struct token_model *model = get_token_model(symbol);
    struct token *token = calloc(1, sizeof(struct token));

    if (token == NULL || model == NULL)
        return NULL;

    token->type = model->type;
    token->pre = model->precedence;

    if (model->set_value != NULL)
        model->set_value(token, args);

    token->reversed = false;
    token->func = model->func;
    return token;
}

bool is_operator(struct token *token)
{
    return token->type == OPERATOR_OR || token->type == OPERATOR_AND
        || token->type == OPERATOR_NOT;
}

bool is_action(struct token *token)
{
    return token->type == ACTION_PRINT || token->type == ACTION_EXEC
        || token->type == ACTION_DELETE || token->type == ACTION_EXECDIR;
}

bool is_parenthesis(struct token *token)
{
    return token->type == L_PARENT || token->type == R_PARENT;
}

struct tokens *parse_tokens(struct args_input *args)
{
    if (args->expression == NULL)
        return NULL;

    struct tokens *tokens = calloc(1, sizeof(struct tokens));
    if (tokens == NULL)
        exit_with(1, "token.c:173 calloc error");

    for (; args->expression[args->expression_index]; args->expression_index++)
    {
        struct token *token = get_token_from_symbol(
            args->expression[args->expression_index], args);

        if (token == NULL)
            exit_with(1, "unknown token %s",
                      args->expression[args->expression_index]);

        if (tokens->length != 0 && tokens->data[tokens->length - 1] != NULL
            && tokens->data[tokens->length - 1]->type == OPERATOR_NOT)
        {
            token->reversed = true;
            tokens->length--;
        }

        add_token(token, tokens);
    }
    add_token(NULL, tokens);
    return tokens;
}
