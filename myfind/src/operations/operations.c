#define _DEFAULT_SOURCE 1

#include <operations.h>

bool print(struct token *token, struct file file)
{
    printf("%s\n", file.path);
    return token == NULL ? true : !token->reversed;
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
        return token->reversed;

    static struct type_stat mode_ref[] = { { "b", S_IFBLK }, { "d", S_IFDIR },
                                           { "c", S_IFCHR }, { "f", S_IFREG },
                                           { "l", S_IFLNK }, { "p", S_IFIFO },
                                           { "g", S_IFSOCK } };

    unsigned int wanted_mode = 999;
    for (int i = 0; i < 7; i++)
        if (strcmp(token->value.param, mode_ref[i].symbol) == 0)
            wanted_mode = mode_ref[i].mode;
    if (wanted_mode == 999)
        return token->reversed;

    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return token->reversed;

    int result_code = file.symlink == SYMLINK_FOLLOW
        ? stat(file.path, stat_info)
        : lstat(file.path, stat_info);
    if (result_code == 0 && (stat_info->st_mode & S_IFMT) == wanted_mode)
        return !token->reversed;

    return token->reversed;
}

bool newer(struct token *token, struct file file)
{
    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return token->reversed;

    int result_code = stat(file.path, stat_info);
    if (result_code != 0)
        return token->reversed;

    struct stat *stat_info_param = calloc(1, sizeof(struct stat));
    if (stat_info_param == NULL)
        return token->reversed;

    result_code = stat(token->value.param, stat_info_param);
    if (result_code != 0)
        return token->reversed;

    int result = stat_info->st_mtime > stat_info_param->st_mtime;
    return token->reversed == !result;
}

bool perm(struct token *token, struct file file)
{
    // get permission for file
    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return token->reversed;

    // get stat of file
    if (stat(file.path, stat_info) != 0)
        return token->reversed;

    char *file_perm = calloc(10, sizeof(char));
    int mode = stat_info->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    snprintf(file_perm, 10, "%03o", mode);

    if (token->value.param[0] == '-')
    {
        for (int i = 0; i < 3; i++)
        {
            if (token->value.param[i + 1] == '0')
                continue;

            int t = ((file_perm[i] - '0') & (token->value.param[i + 1] - '0'));
            if (t == 0
                || (token->value.param[i + 1] - '0') > (file_perm[i] - '0'))
                return token->reversed;
        }
        return !token->reversed;
    }
    else if (token->value.param[0] == '/' || token->value.param[0] == '+')
    {
        for (int i = 0; i < 3; i++)
        {
            if (token->value.param[i + 1] == '0')
                continue;
            if (((file_perm[i] - '0') & (token->value.param[i + 1] - '0')) != 0)
                return !token->reversed;
        }
        return token->reversed;
    }
    else if (strtol(token->value.param, NULL, 10)
             == strtol(file_perm, NULL, 10))
        return !token->reversed;

    return token->reversed;
}

static bool is_number(const char *str)
{
    for (unsigned i = 0; i < strlen(str); i++)
        if (!isdigit(str[i]))
            return false;
    return true;
}

bool user(struct token *token, struct file file)
{
    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return token->reversed;

    if (stat(file.path, stat_info) != 0)
        return token->reversed;

    if ((is_number(token->value.param)
         && strtol(token->value.param, NULL, 10) == stat_info->st_gid))
        return !token->reversed;

    struct passwd *passwd_info = getpwuid(stat_info->st_uid);
    if (passwd_info == NULL)
        exit_with(1, "no such user: %s", token->value.param);

    if (strcmp(passwd_info->pw_name, token->value.param) == 0
        || passwd_info->pw_uid == strtol(token->value.param, NULL, 10))
        return !token->reversed;

    if (getpwnam(token->value.param) == NULL
        && (!is_number(token->value.param)
            || getpwuid(strtol(token->value.param, NULL, 10)) == NULL))
        exit_with(1, "no such user: %s", token->value.param);

    return token->reversed;
}

bool group(struct token *token, struct file file)
{
    struct stat *stat_info = calloc(1, sizeof(struct stat));
    if (stat_info == NULL)
        return token->reversed;

    if (stat(file.path, stat_info) != 0)
        return token->reversed;

    if ((is_number(token->value.param)
         && strtol(token->value.param, NULL, 10) == stat_info->st_gid))
        return !token->reversed;

    struct group *group_info = getgrgid(stat_info->st_gid);
    if (group_info == NULL)
        exit_with(1, "no such group: %s", token->value.param);

    if (strcmp(group_info->gr_name, token->value.param) == 0
        || (is_number(token->value.param)
            && group_info->gr_gid == strtol(token->value.param, NULL, 10)))
        return !token->reversed;

    if (getgrnam(token->value.param) == NULL
        && (!is_number(token->value.param)
            || getgrgid(strtol(token->value.param, NULL, 10)) == NULL))
        exit_with(1, "no such group: %s", token->value.param);

    return token->reversed;
}
