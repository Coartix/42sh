#include "cd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alloc.h"

int change_directory(char *path, char *cmd)
{
    /*
    ** Change the directory, an handle some errors
    */

    char *buf = xmalloc(10000 * sizeof(char));
    char *old_path = getcwd(buf, 10000);

    if (old_path == NULL)
        return 1;

    if (setenv("OLDPWD", old_path, 1) == -1)
        return 1;

    free(old_path);

    if (setenv("PWD", path, 1) == -1)
        return 1;

    if (chdir(path) == -1)
        fprintf(stderr, "cd: %s: No such file or directory\n", cmd);

    return 0;
}

int my_cd(char *cmd)
{
    /*
    **  Reproduce cd builtin
    **  Bug : cd - : double free
    */

    cmd += 2; // to skip "cd"
    int return_code = 0;

    while (*cmd != '\0' && *cmd == ' ')
    {
        cmd++;
    }

    if (*cmd == '\0')
    {
        // case : cd
        char *home = getenv("HOME");
        return change_directory(home, cmd);
    }
    if (*cmd == '-')
    {
        // case : cd -

        char *oldpath = getenv("OLDPWD");

        if (!oldpath)
        {
            char *new_oldpath = getenv("PWD");

            if (setenv("OLDPWD", new_oldpath, 1) == -1)
                return 1;

            return_code = change_directory(new_oldpath, cmd);
        }
        else
        {
            return_code = change_directory(oldpath, cmd);
        }

        fprintf(stdout, "%s\n", oldpath);

        return return_code;
    }
    else if (*cmd == '/')
    {
        // case : cd /....
        return change_directory(cmd, cmd);
    }
    else if (*cmd == '.')
    {
        if (*(cmd + 1) == '.')
        {
            return change_directory(cmd, cmd);
        }
    }
    else
    {
        char *new_path = xmalloc(sizeof(char) * 10000);
        new_path[0] = '\0';

        new_path = getcwd(new_path, 10000);

        new_path = strcat(new_path, "/");
        new_path[strlen(new_path) + 1] = '\0';

        new_path = strcat(new_path, cmd);

        return_code = change_directory(new_path, cmd);

        free_alloc(new_path);
    }
    return return_code;
}
