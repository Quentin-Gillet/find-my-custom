#include "main.h"

static void add_entry(struct entries_point *entries_point, char *entry)
{
    if (entries_point->capacity == 0
        || entries_point->size == entries_point->capacity)
    {
        entries_point->capacity =
            entries_point->capacity == 0 ? 10 : entries_point->capacity * 2;
        entries_point->entries_point =
            realloc(entries_point->entries_point,
                    entries_point->capacity * sizeof(char *));
        if (entries_point->entries_point == NULL)
            exit_with(1, "main.c:14 realloc failed");
    }

    entries_point->entries_point[entries_point->size] = entry;
    entries_point->size++;
}

static struct entries_point *get_entries_point(int argc, char **argv)
{
    struct entries_point *entries_point =
        calloc(1, sizeof(struct entries_point));
    if (entries_point == NULL)
        exit_with(1, "main.c:26 calloc allocation failed");

    int i = 1;
    while (argc > i)
        if (get_token_model(argv[i]) == NULL)
            add_entry(entries_point, argv[i++]);
        else
            break;
    if (entries_point->size == 0)
        add_entry(entries_point, ".");
    return entries_point;
}

static struct args_input *process_args(int argc, char **argv)
{
    struct args_input *args = calloc(1, sizeof(struct args_input));
    if (args == NULL)
        exit_with(1, "main.c:40 calloc allocation failed");

    if (argc == 1)
    {
        args->expression = NULL;
        args->entries_points = calloc(1, sizeof(struct entries_point));
        add_entry(args->entries_points, ".");
    }
    else
    {
        args->entries_points = get_entries_point(argc, argv);
        args->expression = calloc(argc - 1, sizeof(char *));
        if (args->expression == NULL)
            exit_with(1, "main.c:49 calloc allocation failed");
        int i = 0;
        for (; i + args->entries_points->size + 1 < argc; i++)
            args->expression[i] = argv[i + args->entries_points->size + 1];
        args->expression[i] = NULL;
    }
    return args;
}

static int process_entry_point(struct node *ast, const char *path)
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
        struct file file = { dir_info->d_name, filepath };
        evaluate(ast, file);

        if (dir_info->d_type == 4)
            process_entry_point(ast, filepath);
    }

    closedir(current_dir);
    return 0;
}

static int process_entries_points(struct node *ast,
                                  struct entries_point *entries_point)
{
    for (int i = 0; i < entries_point->size; i++)
    {
        struct file file = { entries_point->entries_point[i],
                             entries_point->entries_point[i] };
        evaluate(ast, file);
        process_entry_point(ast, file.path);
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct args_input *args = process_args(argc, argv);
    struct tokens *tokens = parse_tokens(args);
    struct node *ast = build_ast(tokens);

    set_exit_code(process_entries_points(ast, args->entries_points));

    return get_exit_code();
}
