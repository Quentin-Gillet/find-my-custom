#ifndef MYFIND_OPERATIONS_H
#define MYFIND_OPERATIONS_H

#ifndef _DEFAULT_SOURCE
#    define _DEFAULT_SOURCE
#endif /* ! _DEFAULT_SOURCE */
#if defined(__APPLE__)
#    define st_atim st_atimespec
#    define st_ctim st_ctimespec
#    define st_mtim st_mtimespec
#endif

#include <ctype.h>
#include <dirent.h>
#include <errors.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <structures.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct type_stat
{
    char *symbol;
    int mode;
};

// TOKENS ACTION OPERATION
bool print(struct token *token, struct file file);
bool exec(struct token *token, struct file file);
bool execdir(struct token *token, struct file file);
bool delete_action(struct token *token, struct file file);

// TOKENS TEST OPERATION
bool name(struct token *token, struct file file);
bool type(struct token *token, struct file file);
bool newer(struct token *token, struct file file);
bool perm(struct token *token, struct file file);
bool group(struct token *token, struct file file);
bool user(struct token *token, struct file file);

#endif // MYFIND_OPERATIONS_H
