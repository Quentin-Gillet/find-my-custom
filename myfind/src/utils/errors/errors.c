#include "errors.h"

static struct exit_info info = {
    .exit_code = 0,
};

void exit_with(int code, char *message, ...)
{
    char *buffer = calloc(strlen(message) + 10, sizeof(char));
    if (buffer == NULL)
        exit_with(1, "errors.c:exit_with: calloc error");

    snprintf(buffer, strlen(message) + 10, "myfind: %s\n", message);

    va_list args;
    va_start(args, message);
    vfprintf(stderr, buffer, args);
    va_end(args);

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
