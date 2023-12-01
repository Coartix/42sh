#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "cd.h"
#include "echo.h"
#include "option-parser.h"
#include "parser.h"

int PRETTY_PRINT = 0;
int VERBOSE = 0;
int PRINT_DEBUG = 0;

static char *parse_args(int argc, char *argv[])
{
    // To read from args

    if (argc == 1)
    {
        char *cmd;
        cmd = read_file_chevron();
        if (cmd != NULL && strcmp(cmd, "\n") != 0)
        {
            return cmd;
        }
        else
        {
            free(cmd);
            fprintf(stderr,
                    "Usage : ./42sh -c --verbose --pretty-print [COMMAND] "
                    "[SCRIPT]\n");
            return NULL;
        }
        free(cmd);

        exit(1);
    }
    if (argc >= 2)
    {
        char *options = process_params(argc, argv);
        int are_options = check_options(options);
        if (are_options == 0)
        {
            if (strchr(options, 'c') != NULL)
            {
                int command_pos = get_index("-c", argv, argc);
                if (command_pos == 0)
                {
                    fprintf(
                        stderr,
                        "Usage : ./42sh -c --verbose --pretty-print [COMMAND] "
                        "[SCRIPT]\n");
                    free_alloc(options);
                    return NULL;
                }
                free_alloc(options);

                // Traiter argv[command_pos] pour skip shebang
                // char *cmd = xmalloc(sizeof(char) * strlen(argv[command_pos])
                // + 1);
                char *cmd = argv[command_pos];
                cmd = str_skip_shebang(cmd);
                size_t cmd_len = strlen(cmd);
                char *cmd_malloc = malloc(sizeof(char) * cmd_len + 1);
                memcpy(cmd_malloc, cmd, cmd_len + 1);
                cmd_malloc[cmd_len] = '\0';

                // Change to 1 just before dementor
                PRETTY_PRINT = 0;

                // free_alloc(cmd);

                // change to 1 for dementor debug
                // PRINT_DEBUG = 0;
                return cmd_malloc;
            }

            if (strchr(options, 'v') != NULL)
            {
                VERBOSE = 1;
            }

            if (strchr(options, 'p') != NULL)
            {
                PRETTY_PRINT = 1;
            }
        }
        else
        {
            FILE *fd = fopen(argv[argc - 1], "r");
            if (fd != NULL)
            {
                size_t total_size = total_file_size(fd);
                char *file_content = calloc((total_size + 1), sizeof(char));

                file_content = file_to_buffer(fd, file_content);
                file_content[total_size] = '\0';
                fclose(fd);
                free_alloc(options);

                return file_content;
            }
            else
            {
                fprintf(stderr,
                        "Usage : ./42sh -c --verbose --pretty-print [COMMAND] "
                        "[SCRIPT]\n");
                free_alloc(options);
                return NULL;
            }
        }

        free_alloc(options);
    }

    // To read from stdin
    else
    {
        char *cmd;
        cmd = read_cmd();
        if (cmd != NULL)
        {
            return cmd;
        }
        else
        {
            fprintf(stderr,
                    "Usage : ./42sh -c --verbose --pretty-print [COMMAND] "
                    "[SCRIPT]");
            return NULL;
        }
        free(cmd);
    }
    return NULL;
}

static int parse_and_eval(char *cmd)
{
    int res_parser = 0;
    int res_eval = 0;
    int res = 0;

    if (PRINT_DEBUG == 1)
        printf("%s\n", cmd);

    if (cmd != NULL)
    {
        struct ast *ast = NULL;
        res_parser = parse(&ast, cmd);

        if (res_parser == PARSER_OK)
        {
            if (PRETTY_PRINT || VERBOSE || PRINT_DEBUG)
            {
                print_ast(ast);
            }

            res_eval = evaluate_ast(ast);
        }
        else
        {
            if (PRINT_DEBUG == 0)
                fprintf(stderr, "Syntax error\n");
        }

        free_ast(ast);
    }

    if (res_parser == PARSER_OK)
        res = res_eval;
    else
        res = 2;

    return res;
}

int main(int argc, char *argv[])
{
    char *cmd = parse_args(argc, argv);
    int res = parse_and_eval(cmd);

    free(cmd);
    destroy_global();
    return res;
}
