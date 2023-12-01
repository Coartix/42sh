#include <stdio.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate the node eof
 *
 * @param ast
 * @return int
 */
static int evaluate_eof_ast(struct ast *ast)
{
    ast++;
    return TRUE;
}

/**
 * @brief Printf a AST_EOF type node
 *
 * @param ast
 */
static void print_eof_ast(struct ast *ast, char *pre_print)
{
    ast++;
    printf("%snode_EOF\n", pre_print);
}

/**
 * @brief Free a AST_EOF type node
 *
 * @param ast
 */
static void free_eof_ast(struct ast *ast)
{
    if (ast != NULL)
        free_alloc(ast);
}

/**
 * @brief Create a AST_EOF type node
 *
 * @return struct ast_eof*
 */
struct ast_eof *new_eof_ast(void)
{
    struct ast_eof *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_EOF;
    base->free = free_eof_ast;
    base->print = print_eof_ast;
    base->evaluate = evaluate_eof_ast;

    return new;
}
