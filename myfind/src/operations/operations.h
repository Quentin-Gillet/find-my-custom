#ifndef MYFIND_OPERATIONS_H
#define MYFIND_OPERATIONS_H

#include <dirent.h>
#include <string.h>

#include "../errors/errors.h"
#include "../structures.h"

void list_files_rec(struct args_input *args);
void print_current_file(struct args_input *args);

#endif // MYFIND_OPERATIONS_H
