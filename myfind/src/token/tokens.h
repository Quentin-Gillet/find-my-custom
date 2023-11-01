#ifndef MYFIND_TOKENS_H
#define MYFIND_TOKENS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../structures.h"
#include "../errors/errors.h"

union token_value
{
    char *str;
    char **args;
    void (*func)(void);
};

enum token_type
{
    // OPERATOR
    OPERATOR,
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_NOT,
    OPERATOR_L_PARENTHESIS,
    OPERATOR_R_PARENTHESIS,

    // ACTION
    ACTION,
    ACTION_DELETE,
    ACTION_PRINT,
    ACTION_EXEC,

    // TEST
    TEST,
    TEST_NAME,
    TEST_NEWER,

    // OTHER
    VALUE,
};

struct token
{
    enum token_type type;
    union token_value value;

    // Characteristics
    bool unitary;
    int precedence;
};

struct token_model
{
    const char* symbol;
    enum token_type type;
    int precedence;

    void(*set_value)(struct token *token, struct args_input *args);
};

struct tokens
{
    struct token **data;
    unsigned length;
    unsigned capacity;
};

struct tokens *parse_tokens(struct args_input *args);

#endif // MYFIND_TOKENS_H
