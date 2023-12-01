#include "ast.h"

#include <err.h>
#include <stdio.h>

/**
 * @brief Print the given ast, it could be NULL
 *
 * @param ast
 */
void print_ast(struct ast *ast)
{
    if (ast != NULL)
        ast->print(ast, "");
    else
        printf("ast is empty\n");
}

/**
 * @brief Free the given ast, it could be NULL
 *
 * @param ast
 */
void free_ast(struct ast *ast)
{
    if (ast != NULL)
        ast->free(ast);
}

/**
 * @brief Evaluate the given ast, it could be NULL
 *
 * @param ast
 * @return int
 */
int evaluate_ast(struct ast *ast)
{
    int res = TRUE;
    if (ast != NULL)
        res = ast->evaluate(ast);
    return res;
}
