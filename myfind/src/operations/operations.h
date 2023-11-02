#ifndef MYFIND_OPERATIONS_H
#define MYFIND_OPERATIONS_H

#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../errors/errors.h"
#include "../structures.h"

struct type_stat
{
    char* symbol;
    int mode;
};

// TOKENS ACTION OPERATION
bool print(struct token *token, struct file file);
bool exec(struct token *token, struct file file);

// TOKENS TEST OPERATION
bool name(struct token *token, struct file file);
bool type(struct token *token, struct file file);

#endif // MYFIND_OPERATIONS_H
