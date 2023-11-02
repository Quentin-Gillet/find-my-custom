#include "operations.h"

bool print(struct token *token, struct file file)
{
    printf("%s\n", file.path);
    return token == NULL ? true : !token->reversed;
}

static bool exec_command(const char *command)
{
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1)
        return 1;

    if (child_pid == 0)
    {
        execl("/bin/bash", "bash", "-c", command, NULL);
        exit(1);
    }
    else
    {
        wait(&status);
        if (WIFEXITED(status))
            return false;
        else
            return true;
    }
}

bool exec(struct token *token, struct file file)
{
    int count = sizeof(token->value.args) / sizeof(token->value.args[0]);
    unsigned long totalLength = 0;
    for (int i = 0; i < count; i++)
        totalLength += strlen(token->value.args[i]) + 1;

    char *command = (char *)malloc(totalLength * 100);
    if (command == NULL)
        return NULL;
    command[0] = '\0';

    for (int i = 0; i < count; i++)
    {
        if (strcmp(token->value.args[i], "{}") == 0)
            strcat(command, file.filename);
        else
            strcat(command, token->value.args[i]);
        if (i < count - 1)
            strcat(command, " ");
    }
    return exec_command(command);
}

bool name(struct token *token, struct file file)
{
    if (fnmatch(token->value.param, file.filename, 0) == 0)
        return !token->reversed;
    return token->reversed;
}

bool type(struct token *token, struct file file)
{
    if (strlen(token->value.param) != 1)
        return false;

    static struct type_stat mode_ref[] = { { "b", S_IFBLK }, { "d", S_IFDIR },
                                           { "c", S_IFCHR }, { "f", S_IFREG },
                                           { "l", S_IFLNK }, { "p", S_IFIFO },
                                           { "g", S_IFSOCK } };

    int wanted_mode = -1;
    for (int i = 0; i < 7; i++)
        if (strcmp(token->value.param, mode_ref[i].symbol) == 0)
            wanted_mode = mode_ref[i].mode;
    if (wanted_mode == -1)
        return false;

    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return false;

    int result_code = stat(file.path, stat_info);
    if (result_code == 0 && (stat_info->st_mode & S_IFMT) == wanted_mode)
        return true;

    return false;
}
