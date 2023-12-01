#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an ast_cmd type node
 *
 * @param ast
 * @return int
 */
static int evaluate_cmd_ast(struct ast *ast)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    return ast_cmd->cmd->evaluate(ast_cmd->cmd);
}

/**
 * @brief Free an ast_cmd type node
 *
 * @param ast
 */
static void ast_cmd_free(struct ast *ast)
{
    struct ast_cmd *cmd = (struct ast_cmd *)ast;
    if (cmd != NULL)
    {
        cmd->cmd->free(cmd->cmd);
        free_alloc(cmd);
    }
}

/**
 * @brief Print an ast_cmd type node
 *
 * @param ast
 * @param pre_print
 */
static void print_cmd_ast(struct ast *ast, char *pre_print)
{
    struct ast_cmd *cmd = (struct ast_cmd *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_cmd\n", pre_print);

    cmd->cmd->print(cmd->cmd, str);

    free_alloc(str);
}

/**
 * @brief Create an ast_cmd type node
 *
 * @param cmd
 * @return struct ast_cmd
 *
 */
struct ast_cmd *new_cmd_ast(struct ast *cmd)
{
    struct ast_cmd *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_cmd_ast;
    base->free = ast_cmd_free;
    base->evaluate = evaluate_cmd_ast;
    base->type = AST_CMD;

    new->cmd = cmd;

    return new;
}
