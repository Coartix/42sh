#include "export.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int my_export(char *cmd)
{
    cmd += 6; // skip "export"
    size_t len = strlen(cmd);

    char *name = malloc(sizeof(char) * len);
    char *value = malloc(sizeof(char) * len);

    size_t debut = 0;
    while (cmd[debut] != '\0' && cmd[debut] == ' ')
    {
        debut += 1;
    }

    int is_value = 0;
    int name_index = 0;
    int value_index = 0;

    for (size_t i = debut; cmd[i] != '\0'; cmd++)
    {
        if (cmd[i] == '=' && !is_value)
        {
            is_value = 1;
            name[name_index] = '\0';
        }
        else if (is_value == 0)
        {
            name[name_index] = cmd[i];
            name_index++;
        }
        else
        {
            value[value_index] = cmd[i];
            value_index++;
            if (!value)
            {
                value = "";
            }
        }
    }

    if (setenv(name, value, 1) == -1)
        return 1;

    free(name);
    free(value);

    return 0;
}
