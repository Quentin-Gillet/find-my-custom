#ifndef MYFIND_STRUCTURES_H
#define MYFIND_STRUCTURES_H

#include <stdbool.h>

struct file
{
    const char* filename;
    const char* path;
};

enum token_type
{
    // OPERATOR
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_NOT,
    OPERATOR_L_PARENTHESIS,
    OPERATOR_R_PARENTHESIS,

    // ACTION
    ACTION_DELETE,
    ACTION_PRINT,
    ACTION_EXEC,

    // TEST
    TEST_NAME,
    TEST_TYPE,
    TEST_NEWER,

    // OTHER
    VALUE,
};

struct exit_info
{
    int exit_code;
} exit_info;

struct args_input
{
    char *entry_point;
    char **expression;

    unsigned expression_index;
};

union token_value
{
    char *param;
    char **args;
};

struct token
{
    enum token_type type;
    union token_value value;
    bool (*func)(struct token *token, struct file file);

    // Characteristics
    bool reversed;
    int precedence;
};

#endif // MYFIND_STRUCTURES_H
