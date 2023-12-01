#include "option-parser.h"

#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alloc.h"

int POSSIBLE_OPTIONS_NUMBER = 3;

char *read_cmd(void)
{
    /*
    **  Temporary function
    **  Receive the stdin entry
    **  Will be deleted soon
    */

    char *line = NULL;
    size_t len = 0;
    int res = getline(&line, &len, stdin);
    if (res == -1)
    {
        free(line);
        return NULL;
    }
    return line;
}

char *read_file_chevron(void)
{
    /*
    **  Function to handle 42sh < option
    **  Receive the stdin entry
    */
    char *line = NULL;
    size_t len = 0;
    int res = 0;
    char *buffer = calloc(50000, sizeof(char));
    buffer[49999] = '\0';

    while ((res = getline(&line, &len, stdin)) != -1)
    {
        if (line[0] == '#')
            continue;
        buffer = strcat(buffer, line);
    }

    if (line)
        free(line);
    return buffer;
}

int check_options(char *options)
{
    /*
    **  Check if the options string is correct
    **  so if there is at once one correct arg
    */

    if (strchr(options, 'c') == NULL && strchr(options, 'v') == NULL
        && strchr(options, 'p') == NULL)
    {
        return 1;
    }
    return 0;
}

int get_index(char *c, char *argv[], int argc)
{
    /*
    **  Return the index of a -X arg
    */
    int counter = 1;
    while (counter < argc)
    {
        if (strstr(argv[counter], c) != NULL)
        {
            return counter + 1;
        }
        counter += 1;
    }
    return 0;
}

int check_if_there_is_file(int argc, char *argv[])
{
    /*
    **  Check if there is a .sh file given in parameters
    **  Be sure that the .sh file is the last argument
    */

    if (strstr(argv[argc - 1], ".sh") != NULL)
    {
        return 1;
    }
    return 0;
}

char *process_params(int argc, char *argv[])
{
    /*
    **  Main function to process and parse options
    **  Will use getopt_long to handle --option options type
    **  Return a string of 3 chars max, res = cvp if they are used
    */

    char *res = xmalloc(sizeof(char) * (POSSIBLE_OPTIONS_NUMBER + 1));
    res[POSSIBLE_OPTIONS_NUMBER] = '\0';

    struct option longopts[] = { { "pretty-print", optional_argument, NULL,
                                   'p' },
                                 { "verbose", optional_argument, NULL, 'v' },
                                 { "c", optional_argument, NULL, 'c' },
                                 { 0 } };

    while (1)
    {
        int opt = getopt_long(argc, argv, "c:vp", longopts, 0);
        size_t counter = 0;
        if (opt == -1)
            break;

        switch (opt)
        {
        case 'v':
            res[counter] = 'v';
            counter += 1;
            break;
        case 'p':
            res[counter] = 'p';
            counter += 1;
            break;
        case 'c':
            res[counter] = 'c';
            counter += 1;
            break;
        default:
            break;
        }
    }
    return res;
}

size_t my_strlen(const char *s)
{
    /*
    ** Reproduce strlen function
    */

    if (!s)
        return 0;
    size_t i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

size_t total_file_size(FILE *fd)
{
    /*
    **  Returns the number of characters in a file
    */

    char *line = NULL;
    size_t len = 0;
    int res = 0;
    size_t total_size = 0;
    while ((res = getline(&line, &len, fd)) != -1)
    {
        total_size += my_strlen(line);
    }
    if (line)
        free(line);

    fseek(fd, 0, SEEK_SET);
    return total_size;
}

char *file_to_buffer(FILE *fd, char *buffer)
{
    /*
    **  Transfer the file content in the buffer given in argument
    **  Returns the buffer
    */
    fseek(fd, 0, SEEK_SET);
    char *line = NULL;
    size_t len = 0;
    int res = 0;

    while ((res = getline(&line, &len, fd)) != -1)
    {
        if (line[0] == '#')
            continue;
        buffer = strcat(buffer, line);
    }

    if (line)
        free(line);
    return buffer;
}

char *str_skip_shebang(char *str)
{
    /*
    **  Skip the shebang part of the str
    */

    while (*str == ' ')
        str++;

    if (*str == '#')
    {
        while (*str != ' ' && *str != '\0')
            str++;

        // size_t start_pos = counter;
        // size_t len = strlen(str) - start_pos;
        // char *new_str = xmalloc(sizeof(char) * len + 1);
        // new_str[len] = '\0';
        // for (size_t counter_2 = 0; counter_2 < len; counter++)
        // {
        //     new_str[counter_2] = str[counter];
        //     counter += 1;
        // }
        // return new_str;
    }
    return str;
}