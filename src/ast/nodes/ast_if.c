#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an ast_if type node
 *
 * @param ast
 * @return int
 */
static int evaluate_if_ast(struct ast *ast)
{
    struct ast_if *if_ast = (struct ast_if *)ast;

    int res = TRUE;
    if (if_ast->cond->evaluate(if_ast->cond) == TRUE)
        res = if_ast->cmd->evaluate(if_ast->cmd);
    else if (if_ast->else_clause != NULL)
        res = if_ast->else_clause->evaluate(if_ast->else_clause);

    return res;
}

/**
 * @brief Free an ast_if type node
 *
 * @param ast
 */
static void ast_if_free(struct ast *ast)
{
    struct ast_if *if_ast = (struct ast_if *)ast;
    if (if_ast != NULL)
    {
        if_ast->cond->free(if_ast->cond);
        if_ast->cmd->free(if_ast->cmd);
        if (if_ast->else_clause != NULL)
            if_ast->else_clause->free(if_ast->else_clause);
        free_alloc(if_ast);
    }
}

/**
 * @brief Print an ast_if type node
 *
 * @param ast
 * @param pre_print
 */
static void print_if_ast(struct ast *ast, char *pre_print)
{
    struct ast_if *if_ast = (struct ast_if *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_if\n", pre_print);

    if_ast->cond->print(if_ast->cond, str);
    if_ast->cmd->print(if_ast->cmd, str);
    if (if_ast->else_clause != NULL)
        if_ast->else_clause->print(if_ast->else_clause, str);

    free_alloc(str);
}

/**
 * @brief Create an ast_if type node with cond, cmd and else_clause
 * for children. Else_clause can be NULL.
 *
 * @param cond
 * @param cmd
 * @param else_clause
 * @return struct ast_if*
 */
struct ast_if *new_if_ast(struct ast *cond, struct ast *cmd,
                          struct ast *else_clause)
{
    struct ast_if *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_if_ast;
    base->free = ast_if_free;
    base->evaluate = evaluate_if_ast;
    base->type = AST_IF;

    new->cond = cond;
    new->cmd = cmd;
    new->else_clause = else_clause;

    return new;
}
