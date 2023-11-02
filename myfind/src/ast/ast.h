#ifndef MYFIND_AST_H
#define MYFIND_AST_H

#include <stdbool.h>

#include "../token/tokens.h"
#include "../stack/stack.h"

struct node {
    struct token *token;
    struct node *left;
    struct node *right;
};

struct node *build_ast(struct tokens *tokens);
bool evaluate(struct node *tree, struct file file);

#endif // MYFIND_AST_H
