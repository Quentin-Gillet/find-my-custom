#include "errors.h"

static struct exit_info info = {
    .exit_code = 0,
};

void exit_with(int code, char *message)
{
    fprintf(stderr, "myfind: %s", message);
    exit(code);
}

int get_exit_code(void)
{
    return info.exit_code;
}

void set_exit_code(int code)
{
    info.exit_code = code;
}
