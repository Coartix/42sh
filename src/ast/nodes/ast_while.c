#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate node while
 *
 * @param ast
 * @return int
 */
static int evaluate_while_ast(struct ast *ast)
{
    struct ast_while *ast_while = (struct ast_while *)ast;

    int res = TRUE;
    while (ast_while->cond->evaluate(ast_while->cond) == TRUE)
        res = ast_while->do_group->evaluate(ast_while->do_group);
    return res;
}

/**
 * @brief Print an ast_while type node
 *
 * @param ast
 */
static void print_while_ast(struct ast *ast, char *pre_print)
{
    struct ast_while *ast_while = (struct ast_while *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_while\n", pre_print);
    ast_while->cond->print(ast_while->cond, str);
    ast_while->do_group->print(ast_while->do_group, str);

    free_alloc(str);
}

/**
 * @brief Free a ast_while type node and all his childrens
 *
 * @param ast
 */
static void free_while_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_while *ast_while = (struct ast_while *)ast;
        ast_while->cond->free(ast_while->cond);
        ast_while->do_group->free(ast_while->do_group);
        free_alloc(ast_while);
    }
}

/**
 * @brief Create an ast_while node and link his childrens
 *
 * @param cond
 * @param do_group
 * @return struct ast_while*
 */
struct ast_while *new_while_ast(struct ast *cond, struct ast *do_group)
{
    struct ast_while *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_WHILE;
    base->free = free_while_ast;
    base->print = print_while_ast;
    base->evaluate = evaluate_while_ast;

    new->cond = cond;
    new->do_group = do_group;

    return new;
}
