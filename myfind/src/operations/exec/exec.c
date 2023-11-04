#include <operations.h>

static bool exec_command(char *command, char **args, const char *filepath)
{
    int status;

    pid_t child_pid = fork();

    if (child_pid == -1)
        return 1;

    if (child_pid == 0)
    {
        if (filepath != NULL)
            chdir(filepath);
        execvp(command, args);
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

    int size = 0;
    for (int i = 0; token->value.args[i]; i++)
        size++;
    char **command_args = calloc(size + 1, sizeof(char *));

    for (int i = 0; token->value.args[i]; i++)
        if (strcmp(token->value.args[i], "{}") == 0)
        {
            unsigned long len = strlen(file.path) + 1;
            char *input = calloc(len, sizeof(char));
            snprintf(input, len, "%s", file.path);
            command_args[i] = input;
        }
        else
        {
            command_args[i] =
                calloc(strlen(token->value.args[i]) + 1, sizeof(char));
            strcpy(command_args[i], token->value.args[i]);
        }

    bool exit_code = exec_command(command_args[0], command_args, NULL);

    for (int i = 0; command_args[i]; i++)
        free(command_args[i]);
    free(command_args);

    return exit_code;
}

bool execdir(struct token *token, struct file file)
{
    if (token->value.args == NULL)
        return false;

    int size = 0;
    for (int i = 0; token->value.args[i]; i++)
        size++;
    char **command_args = calloc(size + 1, sizeof(char *));

    for (int i = 0; token->value.args[i]; i++)
        if (strcmp(token->value.args[i], "{}") == 0)
        {
            unsigned long len = strlen(file.filename) + 3;
            char *input = calloc(len, sizeof(char));
            snprintf(input, len, "./%s", file.filename);
            command_args[i] = input;
        }
        else
        {
            command_args[i] =
                calloc(strlen(token->value.args[i]) + 1, sizeof(char));
            strcpy(command_args[i], token->value.args[i]);
        }

    bool exit_code = exec_command(command_args[0], command_args, file.parent);

    for (int i = 0; command_args[i]; i++)
        free(command_args[i]);
    free(command_args);

    return exit_code;
}
