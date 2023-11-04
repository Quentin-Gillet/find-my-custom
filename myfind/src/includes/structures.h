#ifndef MYFIND_STRUCTURES_H
#define MYFIND_STRUCTURES_H

#include <stdbool.h>

enum symlink
{
    SYMLINK_FOLLOW,
    SYMLINK_NOFOLLOW,
    SYMLINK_FOLLOW_EXEC,
};

struct file
{
    char *filename;
    char *path;
    const char *parent;
    enum symlink symlink;
    bool print;
};

enum token_type
{
    // OPERATOR
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_NOT,
    L_PARENT,
    R_PARENT,

    // ACTION
    ACTION_DELETE,
    ACTION_PRINT,
    ACTION_EXEC,
    ACTION_EXECDIR,

    // TEST
    TEST_NAME,
    TEST_TYPE,
    TEST_NEWER,
    TEST_PERM,
    TEST_USER,
    TEST_GROUP,
};

struct node
{
    struct token *token;
    struct node *left;
    struct node *right;
};

struct tokens
{
    struct token **data;
    unsigned length;
    unsigned capacity;
};

struct entries_point
{
    char **entries_point;
    unsigned index;
    int size;
    int capacity;
};

struct options
{
    enum symlink symlink;
    bool post_order;
    bool print;
};

struct to_free
{
    struct node *ast;
    struct tokens *tokens;
    struct args_input *args;
};

struct args_input
{
    struct entries_point *entries_points;
    struct options *options;
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
    int pre;
};

#endif // MYFIND_STRUCTURES_H
