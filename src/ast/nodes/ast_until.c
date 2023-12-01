#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate node until
 *
 * @param ast
 * @return int
 */
static int evaluate_until_ast(struct ast *ast)
{
    struct ast_until *ast_until = (struct ast_until *)ast;

    int res = TRUE;
    while (ast_until->cond->evaluate(ast_until->cond) == FALSE)
    {
        res = ast_until->do_group->evaluate(ast_until->do_group);
    }

    return res;
}

/**
 * @brief Print an ast_until type node
 *
 * @param ast
 */
static void print_until_ast(struct ast *ast, char *pre_print)
{
    struct ast_until *ast_until = (struct ast_until *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_until\n", pre_print);
    ast_until->cond->print(ast_until->cond, str);
    ast_until->do_group->print(ast_until->do_group, str);

    free_alloc(str);
}

/**
 * @brief Free a ast_until type node and all his childrens
 *
 * @param ast
 */
static void free_until_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_until *ast_until = (struct ast_until *)ast;
        ast_until->cond->free(ast_until->cond);
        ast_until->do_group->free(ast_until->do_group);
        free_alloc(ast_until);
    }
}

/**
 * @brief Create an ast_until node and link his childrens
 *
 * @param cond
 * @param do_group
 * @return struct ast_until*
 */
struct ast_until *new_until_ast(struct ast *cond, struct ast *do_group)
{
    struct ast_until *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_UNTIL;
    base->free = free_until_ast;
    base->print = print_until_ast;
    base->evaluate = evaluate_until_ast;

    new->cond = cond;
    new->do_group = do_group;

    return new;
}
