#include "../operations.h"

static bool exec_command(char *command, char **args)
{
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1)
        return 1;

    if (child_pid == 0)
    {
        execvp(command, args);
        perror("execvp");
        exit(1);
    }
    else
    {
        wait(&status);
        if (WIFEXITED(status))
            return true;
        else
            return false;
    }
}

bool exec(struct token *token, struct file file)
{
    if (token->value.args == NULL)
        return false;

    // get size of args
    int size = 0;
    for (int i = 0; token->value.args[i]; i++)
        size++;
    char **command_args = calloc(size + 1, sizeof(char *));

    for (int i = 0; token->value.args[i]; i++)
        if (strcmp(token->value.args[i], "{}") == 0)
        {
            char *input;
            asprintf(&input, "%s", file.path);
            command_args[i] = input;
        }
        else
        {
            command_args[i] =
                calloc(strlen(token->value.args[i]) + 1, sizeof(char));
            strcpy(command_args[i], token->value.args[i]);
        }

    bool exit_code = exec_command(command_args[0], command_args);
    return exit_code;
}
