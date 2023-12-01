#include "exit.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

static int isnumber(char *str)
{
    int res = 1;
    for (size_t i = 0; str[i] != '\0' && res; i++)
        res = isdigit(str[i]);

    return res;
}

int my_exit(char *cmd)
{
    /*
    **  Reproduce the exit builtin behaviour
    */

    size_t index_start_number = remove_spaces(cmd);
    int number_len = (strlen(cmd) - index_start_number);
    char *value_str = xmalloc(sizeof(char) * number_len + 1);
    value_str[number_len] = '\0';

    for (size_t counter = 0; counter + index_start_number <= strlen(cmd) - 1;
         counter += 1)
    {
        value_str[counter] = cmd[counter + index_start_number];
    }

    int value = 2;
    if (isnumber(value_str))
        value = atoi(value_str);
    else
        fprintf(stderr, "Syntax error\n");

    free_alloc(value_str);
    destroy_global();
    exit(value);
    return value;
}

int remove_spaces(char *cmd)
{
    /*
    **  Go to the next index while there are some spaces. Return the first index
    **  which is not a space
    */

    int counter = 4;
    while (cmd[counter] == ' ')
        counter += 1;
    return counter;
}
