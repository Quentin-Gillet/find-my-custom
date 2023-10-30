#include <dirent.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    char *dir_str;
    if (argc == 1)
        dir_str = ".";
    else
        dir_str = argv[1];

    DIR *dir_input = opendir(dir_str);
    if (dir_input == NULL)
    {
        fprintf(stderr, "%s: No such file or directory", argv[1]);
        return 1;
    }

    struct dirent *dir_info = readdir(dir_input);
    for (; dir_info; dir_info = readdir(dir_input))
    {
        if (dir_info->d_type == 4 || dir_info->d_name[0] == '.')
            continue;
        printf("%s\n", dir_info->d_name);
    }

    closedir(dir_input);
    return 0;
}

