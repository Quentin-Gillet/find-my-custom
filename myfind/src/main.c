#include "main.h"

static struct args_input *process_args(int argc, char **argv)
{
    struct args_input *args = calloc(1, sizeof(struct args_input));
    if (args == NULL)
        exit_with(1, "main.c:7 calloc allocation failed");

    if (argc == 1)
    {
        args->entry_point = ".";
        fprintf(stderr, "%s\n", args->entry_point);
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
        {
            args->expression[i - 2] = argv[i];
            fprintf(stderr, "%s\n", argv[i]);
        }
        args->expression[i - 2] = NULL;
    }
    args->expression_index = 0;
    return args;
}

void free_args(struct args_input *args)
{
    free(args->expression);
    free(args);
}

int main(int argc, char **argv)
{
    struct args_input *args = process_args(argc, argv);
    fprintf(stderr, "===========\n");
    list_files_rec(args);
    return 9;
    struct tokens *tokens = parse_tokens(args);
    for (unsigned i = 0; i < tokens->length; i++)
        printf("%d - ", tokens->data[i]->type);

    free_args(args);
    return get_exit_code();
}
