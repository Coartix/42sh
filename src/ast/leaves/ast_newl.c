#include <stdio.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate the node newl
 *
 * @param ast
 * @return int
 */
static int evaluate_newl_ast(struct ast *ast)
{
    ast++;
    return TRUE;
}

/**
 * @brief Print a AST_NEWl type node
 *
 * @param ast
 */
static void print_newl_ast(struct ast *ast, char *pre_print)
{
    ast++;
    printf("%snode_newl\n", pre_print);
}

/**
 * @brief Free a AST_NEWl type node
 *
 * @param ast
 */
static void free_newl_ast(struct ast *ast)
{
    if (ast != NULL)
        free_alloc(ast);
}

/**
 * @brief Create a AST_NEWl type node
 *
 * @return struct ast_newl*
 */
struct ast_newl *new_newl_ast(void)
{
    struct ast_newl *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_NEWL;
    base->free = free_newl_ast;
    base->print = print_newl_ast;
    base->evaluate = evaluate_newl_ast;

    return new;
}
