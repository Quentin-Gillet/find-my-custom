#ifndef MYFIND_ARGS_PARSING_H
#define MYFIND_ARGS_PARSING_H

#include <stdlib.h>

#include "../errors/errors.h"
#include "../structures.h"
#include "../token/tokens.h"

struct args_input *process_args(int argc, char **argv);

#endif // MYFIND_ARGS_PARSING_H
