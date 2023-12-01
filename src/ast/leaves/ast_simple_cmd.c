#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "alloc.h"
#include "ast.h"
#include "cd.h"
#include "dot.h"
#include "echo.h"
#include "exit.h"
#include "export.h"
#include "lexer.h"
#include "process_args.h"

#define NB_BUILTINS 5

enum type_builtin
{
    NOT_BUILTIN,
    ECHO,
    DOT,
    EXPORT,
    EXIT,
    CD
};

static char *builtins[NB_BUILTINS] = { "echo", ".", "export", "exit", "cd" };

/**
 * @brief
 *
 * @param input
 * @param pos
 * @return int
 */
static enum type_builtin is_builtins(const char *input)
{
    enum type_builtin res = NOT_BUILTIN;

    size_t len_input = 0;
    if (input[0] == '.')
        len_input = 1;
    else
        while (isalpha(input[len_input]))
            len_input++;

    for (size_t i = 0; i < NB_BUILTINS && res == NOT_BUILTIN; i++)
    {
        if (len_input == strlen(builtins[i])
            && !strncmp(builtins[i], input, len_input))
            res = i + 1;
    }

    return res;
}

/**
 * @brief Evaluate an AST_SIMPLE_CMD type node
 *
 * @param ast
 * @return int
 */
static int evaluate_simple_cmd_ast(struct ast *ast)
{
    struct ast_simple_cmd *ast_simple_cmd = (struct ast_simple_cmd *)ast;

    int pid;
    int res = TRUE;
    enum type_builtin cmd_type = NOT_BUILTIN;

    if ((cmd_type = is_builtins(ast_simple_cmd->args[0])) == NOT_BUILTIN)
    {
        if ((pid = fork()) < 0)
            return FALSE;
        if (pid == 0)
        {
            execvp(ast_simple_cmd->cmd, ast_simple_cmd->args);
            err(127, "failed to exec command");
        }

        int wstatus;

        if (waitpid(pid, &wstatus, 0) < 0)
            res = FALSE;
        else
            res = WEXITSTATUS(wstatus);
    }
    else
    {
        switch (cmd_type)
        {
        case ECHO:
            res = my_echo(ast_simple_cmd->args);
            break;
        case DOT:
            res = my_dot(ast_simple_cmd->all_cmd);
            break;
        case EXPORT:
            res = my_export(ast_simple_cmd->all_cmd);
            break;
        case EXIT:
            res = my_exit(ast_simple_cmd->all_cmd);
            break;
        case CD:
            res = my_cd(ast_simple_cmd->all_cmd);
            break;
        default:
            fprintf(stderr, "Builtin not found\n");
            break;
        }
    }

    return res;
}

/**
 * @brief Print an AST_SIMPLE_CMD type node
 *
 * @param ast
 */
static void ast_simple_cmd_free(struct ast *ast)
{
    struct ast_simple_cmd *cmd = (struct ast_simple_cmd *)ast;
    if (cmd != NULL)
    {
        for (size_t i = 0; cmd->args[i] != NULL; i++)
            free_alloc(cmd->args[i]);
        free_alloc(cmd->args);
        free_alloc(cmd->all_cmd);
        free_alloc(cmd);
    }
}

/**
 * @brief Free an AST_SIMPLE_CMD type node
 *
 * @param ast
 */
static void print_simple_cmd_ast(struct ast *ast, char *pre_print)
{
    struct ast_simple_cmd *cmd = (struct ast_simple_cmd *)ast;
    printf("%snode_simple_cmd:", pre_print);
    for (size_t i = 0; cmd->args[i] != NULL; i++)
        printf(" %s", cmd->args[i]);
    printf("\n");
}

/**
 * @brief Create an AST_SIMPLE_CMD type node
 *
 * @param cmd_str
 * @return struct ast_simple_cmd*
 */
struct ast_simple_cmd *new_simple_cmd_ast(char **cmd_str)
{
    struct ast_simple_cmd *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_simple_cmd_ast;
    base->free = ast_simple_cmd_free;
    base->evaluate = evaluate_simple_cmd_ast;
    base->type = AST_SIMPLE_CMD;

    size_t len = 0;
    while (cmd_str[len] != NULL)
        len++;

    new->args = xmalloc(sizeof(*new->args) * (len + 1));
    for (size_t i = 0; i < len; i++)
    {
        new->args[i] = xmalloc(strlen(cmd_str[i]) + 1);
        memcpy(new->args[i], cmd_str[i], strlen(cmd_str[i]) + 1);
    }
    new->args[len] = NULL;

    new->args = process_args(new->args);
    new->cmd = new->args[0];

    new->all_cmd = NULL;
    size_t pos = 0;

    for (size_t i = 0; new->args[i] != NULL; i++)
    {
        size_t len = strlen(new->args[i]);
        new->all_cmd = xrealloc(new->all_cmd, pos + len + 1);
        memmove(new->all_cmd + pos, new->args[i], len);
        pos += len;
        if (new->args[i + 1] != NULL)
            new->all_cmd[pos++] = ' ';
        else
            new->all_cmd[pos] = '\0';
    }

    return new;
}
