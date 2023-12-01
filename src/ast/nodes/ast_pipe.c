#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief
 *
 * @param argv_left
 * @param argv
 * @return int
 *
 * We close many FD (especially the dupped ones : save_in & save_out) by
 * safety, maybe we can keep some up if needed
 */
static int exec_pipe(struct ast *cmd, int fdin, int fdout)
{
    int save_in = dup(STDIN_FILENO);
    int save_out = dup(STDOUT_FILENO);

    dup2(fdin, STDIN_FILENO);
    dup2(fdout, STDOUT_FILENO);

    int pid = fork();
    int status = 0;

    if (pid == 0)
    {
        close(save_in);
        close(save_out);

        exit(cmd->evaluate(cmd));
    }
    else if (pid > 0)
    {
        dup2(save_in, STDIN_FILENO);
        dup2(save_out, STDOUT_FILENO);

        close(save_in);
        close(save_out);

        waitpid(pid, &status, 0);
    }
    else
        exit(1);

    return WEXITSTATUS(status);
}

/**
 * @brief Evaluate an AST_pipe type node
 *
 * @param ast
 * @return int
 */
static int evaluate_pipe_ast(struct ast *ast)
{
    struct ast_pipe *ast_pipe = (struct ast_pipe *)ast;

    if (ast_pipe->nb_cmds == 1)
        return ast_pipe->cmds[0]->evaluate(ast_pipe->cmds[0]);

    int pipe_tmp[2];
    if (pipe(pipe_tmp) < 0)
        exit(1);

    int res = exec_pipe(ast_pipe->cmds[0], STDIN_FILENO, pipe_tmp[1]);

    for (size_t i = 1; i < ast_pipe->nb_cmds - 2; i++)
    {
        int fdin_temp = dup(pipe_tmp[0]);

        close(pipe_tmp[1]);
        close(pipe_tmp[0]);

        if (pipe(pipe_tmp) < 0)
            exit(1);

        res = exec_pipe(ast_pipe->cmds[i], fdin_temp, pipe_tmp[1]);
        close(fdin_temp);
    }

    close(pipe_tmp[1]);

    res = exec_pipe(ast_pipe->cmds[ast_pipe->nb_cmds - 1], pipe_tmp[0],
                    STDOUT_FILENO);

    close(pipe_tmp[0]);

    return res;
}

/**
 * @brief Print an AST_PIPE type node
 *
 * @param ast
 */
static void print_pipe_ast(struct ast *ast, char *pre_print)
{
    struct ast_pipe *ast_pipe = (struct ast_pipe *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_pipe\n", pre_print);
    for (size_t i = 0; i < ast_pipe->nb_cmds; i++)
        ast_pipe->cmds[i]->print(ast_pipe->cmds[i], str);

    free_alloc(str);
}

/**
 * @brief Free an AST_PIPE type node and all his childrens
 *
 * @param ast
 */
static void free_pipe_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_pipe *pipe = (struct ast_pipe *)ast;
        for (size_t i = 0; i < pipe->nb_cmds; i++)
        {
            pipe->cmds[i]->free(pipe->cmds[i]);
        }
        free_alloc(pipe->cmds);
        free_alloc(pipe);
    }
}

/**
 * @brief Create ast_pipe
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_pipe*
 */
struct ast_pipe *new_pipe_ast(struct ast **cmds, size_t nb_cmds)
{
    struct ast_pipe *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_PIPE;
    base->free = free_pipe_ast;
    base->print = print_pipe_ast;
    base->evaluate = evaluate_pipe_ast;

    new->cmds = cmds;
    new->nb_cmds = nb_cmds;

    return new;
}
