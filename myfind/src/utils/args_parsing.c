#include "args_parsing.h"

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

static bool end_of_entries_point(int index, int argc, char **argv)
{
    return index >= argc || argv[index][0] == '-' || argv[index][0] == '('
        || argv[index][0] == '!';
}

static struct entries_point *get_entries_point(int *index, int argc,
                                               char **argv)
{
    struct entries_point *entries_point =
        calloc(1, sizeof(struct entries_point));
    if (entries_point == NULL)
        exit_with(1, "main.c:26 calloc allocation failed");

    while (!end_of_entries_point(*index, argc, argv))
    {
        add_entry(entries_point, argv[*index]);
        *index = *index + 1;
    }

    if (entries_point->size == 0)
        add_entry(entries_point, ".");
    return entries_point;
}

struct options *get_options(int *i, int argc, char **argv)
{
    struct options *options = calloc(1, sizeof(struct options));
    if (options == NULL)
        exit_with(1, "main.c:43 calloc allocation failed");

    options->symlink = SYMLINK_NOFOLLOW;

    for (; *i < argc; *i = *i + 1)
    {
        if (strcmp(argv[*i], "-d") == 0)
            options->post_order = true;
        else if (strcmp(argv[*i], "-L") == 0)
            options->symlink = SYMLINK_FOLLOW;
        else if (strcmp(argv[*i], "-H") == 0)
            options->symlink = SYMLINK_FOLLOW_EXEC;
        else if (strcmp(argv[*i], "-P") == 0)
            options->symlink = SYMLINK_NOFOLLOW;
        else if (argv[*i][0] == '-')
            return NULL;
        else
            return options;
    }
    return options;
}

struct args_input *process_args(int argc, char **argv)
{
    struct args_input *args = calloc(1, sizeof(struct args_input));
    if (args == NULL)
        exit_with(1, "main.c:67 calloc allocation failed");

    int index = 1;

    args->options = get_options(&index, argc, argv);
    if (args->options == NULL)
        exit_with(1, "main.c:81 invalid option '%s'", argv[index]);

    if (argc == 1)
    {
        args->expression = NULL;
        args->entries_points = calloc(1, sizeof(struct entries_point));
        add_entry(args->entries_points, ".");
    }
    else
    {
        args->entries_points = get_entries_point(&index, argc, argv);

        args->expression = calloc(argc - 1, sizeof(char *));
        if (args->expression == NULL)
            exit_with(1, "main.c:49 calloc allocation failed");

        int i = 0;
        for (; index < argc; i++, index++)
            args->expression[i] = argv[index];
        args->expression[i] = NULL;
    }

    return args;
}
