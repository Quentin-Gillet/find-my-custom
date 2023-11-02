#ifndef MYFIND_STRUCTURES_H
#define MYFIND_STRUCTURES_H

#include <stdbool.h>

struct file
{
    char *filename;
    char *path;
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
    TEST_PERM,
    TEST_USER,
    TEST_GROUP,
};

struct exit_info
{
    int exit_code;
} exit_info;

struct entries_point
{
    char **entries_point;
    unsigned index;
    int size;
    int capacity;
};

struct args_input
{
    struct entries_point *entries_points;
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
