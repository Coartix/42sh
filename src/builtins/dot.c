#include "dot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alloc.h"

int my_dot(char *cmd)
{
    /*
    **  Reproduce dot function
    **  BUGS TO FIX : error may be bad handled + code is awful
    */

    cmd += 2; // to skip "."

    int i = 0;

    while (cmd[i] != '\0' && cmd[i] == ' ') // i = debut of filename
    {
        i++;
    }

    int x = i;

    while (cmd[x] != '\0' && cmd[x] != ' ') // x = end of filename
    {
        x++;
    }

    char *file = calloc(x + 1, sizeof(char));
    // int go_to_PATH = 1;

    for (int i_copy = 0; i_copy + i < x; i_copy++) // copy filename into "file"
    {
        file[i_copy] = cmd[i_copy + i];
        /*if (file[i_copy] == '/')
            go_to_PATH = 0;*/
    }

    if (!file) // if calloc failed
    {
        fprintf(stderr, ": No such file or directory\n");
        return 1;
    }

    size_t file_len = strlen(file);

    // bad way of checking if the file is a .sh -- needs to be changed
    if (file_len < 4 || file[file_len - 3] != '.' || file[file_len - 2] != 's'
        || file[file_len - 1] != 'h')
    {
        if (fopen(file, "r") != NULL)
        {
            fprintf(stderr, "%s : cannot execute binary file\n", file);
            free(file);
            return 1;
        }
        char *path = getenv("PATH");
        chdir(path);

        if (fopen(file, "r") != NULL)
        {
            fprintf(stderr, "%s : cannot execute binary file\n", file);
            free(file);
            return 1;
        }
        else
        {
            fprintf(stderr, "%s : No such file or directory\n", file);
            free(file);
            return 1;
        }
    }

    cmd += x;

    char *new_path = xmalloc(sizeof(char) * 1000);
    new_path[0] = '\0';

    new_path = getcwd(new_path, 1000);

    new_path = strcat(new_path, "/");
    new_path[strlen(new_path) + 1] = '\0';

    new_path = strcat(new_path, file);

    int res = 0;

    if (execlp(new_path, file, cmd, (char *)0) == -1)
    {
        char *path = getenv("PATH");
        chdir(path);

        char *second_path = xmalloc(sizeof(char) * 1000);
        second_path[0] = '\0';

        second_path = getcwd(second_path, 1000);

        second_path = strcat(second_path, "/");
        second_path[strlen(second_path) + 1] = '\0';

        second_path = strcat(second_path, file);

        res = execlp(second_path, file, cmd, (char *)0);

        free_alloc(second_path);
        ;
    }

    free(file);
    free_alloc(new_path);

    return res;
}
