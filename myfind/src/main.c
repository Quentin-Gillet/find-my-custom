#include "main.h"

static struct args_input *process_args(int argc, char **argv)
{
    struct args_input *args = calloc(1, sizeof(struct args_input));
    if (args == NULL)
        exit_with(1, "main.c:7 calloc allocation failed");

    if (argc == 1)
    {
        args->entry_point = ".";
        args->expression = NULL;
    }
    else
    {
        args->entry_point = argv[1];

        args->expression = calloc(argc - 1, sizeof(char *));
        if (args->expression == NULL)
            exit_with(1, "main.c:20 calloc allocation failed");
        int i = 2;
        for (; i < argc; i++)
            args->expression[i - 2] = argv[i];
        args->expression[i - 2] = NULL;
    }
    return args;
}

void free_args(struct args_input *args)
{
    free(args->expression);
    free(args);
}

static int process_files(struct node *ast, struct args_input *args)
{
    DIR *current_dir = opendir(args->entry_point);
    if (current_dir == NULL)
        return 1;

    struct dirent *dir_info = readdir(current_dir);
    for (; dir_info; dir_info = readdir(current_dir))
    {
        if (strcmp(dir_info->d_name, "..") == 0
            || strcmp(dir_info->d_name, ".") == 0)
            continue;

        char *path;
        asprintf(&path, "%s/%s", args->entry_point, dir_info->d_name);
        struct file file = { dir_info->d_name, path};
        evaluate(ast, file);
        if (dir_info->d_type == 4)
        {
            struct args_input *input = calloc(1, sizeof(struct args_input));
            asprintf(&input->entry_point, "%s/%s", args->entry_point,
                     dir_info->d_name);
            process_files(ast, input);
        }
    }

    closedir(current_dir);
    return 0;
}

int main(int argc, char **argv)
{
    struct args_input *args = process_args(argc, argv);
    struct tokens *tokens = parse_tokens(args);
    struct node *ast = build_ast(tokens);

    struct file file = { args->entry_point, args->entry_point };
    evaluate(ast, file);
    set_exit_code(process_files(ast, args));

    free_args(args);
    return get_exit_code();
}
