#ifndef MYFIND_TOKENS_H
#define MYFIND_TOKENS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../errors/errors.h"
#include "../operations/operations.h"
#include "../structures.h"

struct token_model
{
    const char *symbol;
    enum token_type type;
    int precedence;

    void (*set_value)(struct token *token, struct args_input *args);
    bool (*func)(struct token *token, struct file file);
};

struct tokens
{
    struct token **data;
    unsigned length;
    unsigned capacity;
};

bool is_operator(struct token *token);
bool is_action(struct token *token);

struct tokens *parse_tokens(struct args_input *args);
struct token_model *get_token_model(const char *symbol);

#endif // MYFIND_TOKENS_H
