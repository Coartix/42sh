#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an ast_else type node
 *
 * @param ast
 * @return int
 */
static int evaluate_else_ast(struct ast *ast)
{
    struct ast_else *else_ast = (struct ast_else *)ast;

    int res = TRUE;
    // if condition is NULL, it is an else statement
    // otherwise it is an elif statement
    if (else_ast->cond == NULL
        || else_ast->cond->evaluate(else_ast->cond) == TRUE)
        res = else_ast->cmd->evaluate(else_ast->cmd);
    else if (else_ast->else_clause != NULL)
        res = else_ast->else_clause->evaluate(else_ast->else_clause);

    return res;
}

/**
 * @brief Free an ast_else type node
 *
 * @param ast
 */
static void ast_else_free(struct ast *ast)
{
    struct ast_else *else_ast = (struct ast_else *)ast;
    if (else_ast != NULL)
    {
        if (else_ast->cond != NULL)
            else_ast->cond->free(else_ast->cond);
        else_ast->cmd->free(else_ast->cmd);
        if (else_ast->else_clause != NULL)
            else_ast->else_clause->free(else_ast->else_clause);
        free_alloc(else_ast);
    }
}

/**
 * @brief Print an ast_else type node
 *
 * @param ast
 * @param pre_print
 */
static void print_else_ast(struct ast *ast, char *pre_print)
{
    struct ast_else *else_ast = (struct ast_else *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_else\n", pre_print);

    if (else_ast->cond != NULL)
        else_ast->cond->print(else_ast->cond, str);
    else_ast->cmd->print(else_ast->cmd, str);
    if (else_ast->else_clause != NULL)
        else_ast->else_clause->print(else_ast->else_clause, str);

    free_alloc(str);
}

/**
 * @brief Create an ast_else type node with cond, cmd and else_clause
 * for children. Else_clause can be NULL.
 *
 * @param cond
 * @param cmd
 * @param else_clause
 * @return struct ast_else*
 */
struct ast_else *new_else_ast(struct ast *cond, struct ast *cmd,
                              struct ast *else_clause)
{
    struct ast_else *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_else_ast;
    base->free = ast_else_free;
    base->evaluate = evaluate_else_ast;
    base->type = AST_ELSE;

    new->cond = cond;
    new->cmd = cmd;
    new->else_clause = else_clause;

    return new;
}
