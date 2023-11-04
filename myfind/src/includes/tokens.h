#ifndef MYFIND_TOKENS_H
#define MYFIND_TOKENS_H

#ifndef _DEFAULT_SOURCE
#    define _DEFAULT_SOURCE
#endif /* ! _DEFAULT_SOURCE */

#include <errors.h>
#include <operations.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <structures.h>

struct token_model
{
    const char *symbol;
    enum token_type type;
    int precedence;

    void (*set_value)(struct token *token, struct args_input *args,
                      struct tokens *tokens);
    bool (*func)(struct token *token, struct file file);
};

bool is_operator(struct token *token);
bool is_action(struct token *token);
bool is_parenthesis(struct token *token);

struct token *copy_token(struct token *token);
struct tokens *parse_tokens(struct args_input *args);
struct token_model *get_token_model(const char *symbol);

#endif // MYFIND_TOKENS_H
