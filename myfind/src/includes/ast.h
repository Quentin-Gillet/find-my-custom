#ifndef MYFIND_AST_H
#define MYFIND_AST_H

#include <stack.h>
#include <stdbool.h>
#include <tokens.h>

struct node *build_ast(struct tokens *tokens);
bool evaluate(struct node *tree, struct file file);

#endif // MYFIND_AST_H
