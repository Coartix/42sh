#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void remove_first(char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        str[i] = str[i + 1];
}

/**
 * @brief interpret the \n \t \\
 *
 * @param str
 * @param pos
 */
static void interpret(char **str, size_t pos)
{
    for (; str[pos] != NULL; pos++)
    {
        for (size_t i = 0; str[pos][i] != '\0'; i++)
        {
            if (str[pos][i] == '\\' && str[pos][i + 1] != '\0')
            {
                char c = str[pos][i + 1];
                int remove = 1;
                switch (c)
                {
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case '\\':
                    c = '\\';
                    break;
                default:
                    remove = 0;
                    break;
                }
                if (remove)
                {
                    str[pos][i + 1] = c;
                    remove_first(str[pos] + i);
                }
            }
        }
    }
}

/**
 * @brief echo builtin
 *
 * @param str
 * @return int
 */
int my_echo(char **str)
{
    int n_opt = 0;
    int e_opt = 0;
    size_t i = 1;

    size_t len = 0;
    while (str[len] != NULL)
        len++;

    if (len > 1)
    {
        while (str[i] != NULL
               && (!strncmp(str[i], "-e", 3) || !strncmp(str[i], "-n", 3)
                   || !strncmp(str[i], "-en", 4) || !strncmp(str[i], "-ne", 4)))
        {
            if (!strncmp(str[i], "-e", 3))
                e_opt = 1;
            else if (!strncmp(str[i], "-n", 3))
                n_opt = 1;
            else
            {
                e_opt = 1;
                n_opt = 1;
            }
            i++;
        }

        if (e_opt == 1)
            interpret(str, i);

        if (i < len)
            printf("%s", str[i++]);

        for (; i < len; i++)
            printf(" %s", str[i]);
    }

    if (n_opt == 0)
        printf("\n");

    return 0;
}
