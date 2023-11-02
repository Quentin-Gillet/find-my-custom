#ifndef MYFIND_ERRORS_H
#define MYFIND_ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#include "../structures.h"

void exit_with(int code, char *message);
int get_exit_code(void);
void set_exit_code(int code);

#endif // MYFIND_ERRORS_H
