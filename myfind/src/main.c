#include "main.h"

static int process_entry_point(struct node *ast, const char *path,
                               struct options *options)
{
    DIR *current_dir = opendir(path);
    if (current_dir == NULL)
        return 1;

    struct dirent *dir_info = readdir(current_dir);
    for (; dir_info; dir_info = readdir(current_dir))
    {
        if (strcmp(dir_info->d_name, "..") == 0
            || strcmp(dir_info->d_name, ".") == 0)
            continue;

        char *filepath;
        asprintf(&filepath, "%s/%s", path, dir_info->d_name);
        struct file file = { dir_info->d_name, filepath, options->symlink };

        if (!options->post_order)
            evaluate(ast, file);

        if (dir_info->d_type == 4
            || (dir_info->d_type == 10 && options->symlink == SYMLINK_FOLLOW))
            process_entry_point(ast, filepath, options);

        if (options->post_order)
            evaluate(ast, file);
    }

    closedir(current_dir);
    return 0;
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
        if (!options->post_order)
            evaluate(ast, file);

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

    set_exit_code(
        process_entries_points(ast, args->entries_points, args->options));

    return get_exit_code();
}
