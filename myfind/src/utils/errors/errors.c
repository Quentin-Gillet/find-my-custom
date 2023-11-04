#include <errors.h>

void exit_with(int code, char *message, ...)
{
    char *buffer = calloc(strlen(message) + 10, sizeof(char));
    if (buffer == NULL)
        exit_with(1, "exit_with: calloc error");

    snprintf(buffer, strlen(message) + 10, "myfind: %s\n", message);

    va_list args;
    va_start(args, message);
    vfprintf(stderr, buffer, args);
    va_end(args);

    free(buffer);
    exit(code);
}
