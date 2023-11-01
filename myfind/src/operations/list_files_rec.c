#include "operations.h"

void list_files_rec(struct args_input *args)
{
    DIR *current_dir = opendir(args->entry_point);
    if (current_dir == NULL)
        return;

    struct dirent *dir_info = readdir(current_dir);
    for (; dir_info; dir_info = readdir(current_dir))
    {
        if (strcmp(dir_info->d_name, "..") == 0
            || strcmp(dir_info->d_name, ".") == 0)
            continue;

        printf("%s/%s\n", args->entry_point, dir_info->d_name);
        fprintf(stderr, "%s/%s\n", args->entry_point, dir_info->d_name);
        if (dir_info->d_type == 4)
        {
            struct args_input *input = calloc(1, sizeof(struct args_input));
            asprintf(&input->entry_point, "%s/%s", args->entry_point,
                     dir_info->d_name);
            list_files_rec(input);
        }
    }

    closedir(current_dir);
}
