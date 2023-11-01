#ifndef MYFIND_STRUCTURES_H
#define MYFIND_STRUCTURES_H

struct exit_info
{
    int exit_code;
} exit_info;

struct args_input
{
    char* entry_point;
    char** expression;

    unsigned expression_index;
};

enum function_list
{
    LIST_FILES_REC,
};

struct operation_function
{
    enum function_list name;
    void(*func)(struct args_input *args);
};

#endif // MYFIND_STRUCTURES_H
