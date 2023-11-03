#define _DEFAULT_SOURCE 1

#include <main.h>

static void process_entry_point(struct node *ast, const char *path,
                                struct options *options)
{
    DIR *current_dir = opendir(path);
    if (current_dir == NULL)
        return;

    struct dirent *dir_info = readdir(current_dir);
    for (; dir_info; dir_info = readdir(current_dir))
    {
        if (strcmp(dir_info->d_name, "..") == 0
            || strcmp(dir_info->d_name, ".") == 0)
            continue;

        unsigned long size = strlen(path) + strlen(dir_info->d_name) + 2;
        char *filepath = calloc(size, sizeof(char));
        if (path[strlen(path) - 1] == '/')
            snprintf(filepath, size, "%s%s", path, dir_info->d_name);
        else
            snprintf(filepath, size, "%s/%s", path, dir_info->d_name);
        struct file file = { dir_info->d_name, filepath, options->symlink };

        struct stat statbuf;
        if (lstat(filepath, &statbuf) == -1)
            exit_with(1, "no such file or directory: %s", filepath);

        if (!options->post_order)
            evaluate(ast, file);

        if (S_ISDIR(statbuf.st_mode)
            || ((S_ISLNK(statbuf.st_mode)) && options->symlink == SYMLINK_FOLLOW
                && !S_ISREG(statbuf.st_mode)))
            process_entry_point(ast, filepath, options);

        if (options->post_order)
            evaluate(ast, file);
    }

    closedir(current_dir);
}

static int process_entries_points(struct node *ast,
                                  struct entries_point *entries_point,
                                  struct options *options)
{
    for (int i = 0; i < entries_point->size; i++)
    {
        struct file file = { entries_point->entries_point[i],
                             entries_point->entries_point[i],
                             options->symlink };

        struct stat statbuf;
        if (lstat(file.path, &statbuf) == -1)
            exit_with(1, "no such file or directory: %s", file.path);

        if (!options->post_order)
            evaluate(ast, file);

        if (statbuf.st_mode & S_IFDIR
            || ((statbuf.st_mode & S_IFLNK)
                && options->symlink == SYMLINK_FOLLOW))
            process_entry_point(ast, file.path, options);

        if (options->post_order)
            evaluate(ast, file);
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct args_input *args = process_args(argc, argv);
    struct tokens *tokens = parse_tokens(args);
    struct node *ast = build_ast(tokens);

    process_entries_points(ast, args->entries_points, args->options);

    return get_exit_code();
}
