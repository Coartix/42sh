#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an AST_AND_OR type node
 *
 * @param ast
 * @return int
 */
static int evaluate_and_or_ast(struct ast *ast)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    int res = ast_and_or->pipes[0]->evaluate(ast_and_or->pipes[0]);

    for (size_t i = 1; i < ast_and_or->nb_pipes; i++)
    {
        if ((res == TRUE && ast_and_or->op_type[i - 1] == AND)
            || (res == FALSE && ast_and_or->op_type[i - 1] == OR))
        {
            res = ast_and_or->pipes[i]->evaluate(ast_and_or->pipes[i]);
        }
    }

    return res;
}

/**
 * @brief Print an AST_AND_OR type node
 *
 * @param ast
 */
static void print_and_or_ast(struct ast *ast, char *pre_print)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_and_or\n", pre_print);
    for (size_t i = 0; i < ast_and_or->nb_pipes; i++)
    {
        ast_and_or->pipes[i]->print(ast_and_or->pipes[i], str);

        if (i < ast_and_or->nb_pipes - 1)
        {
            if (ast_and_or->op_type[i] == AND)
                printf("%s&&\n", pre_print);
            else
                printf("%s||\n", pre_print);
        }
    }
    free_alloc(str);
}

/**
 * @brief Free an AST_AND_OR type node and all his childrens
 *
 * @param ast
 */
static void free_and_or_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_and_or *and_or = (struct ast_and_or *)ast;
        for (size_t i = 0; i < and_or->nb_pipes; i++)
        {
            and_or->pipes[i]->free(and_or->pipes[i]);
        }
        free_alloc(and_or->pipes);
        free_alloc(and_or->op_type);
        free_alloc(and_or);
    }
}

/**
 * @brief create node ast and_or
 *
 * @param pipes
 * @param nb_pipes
 * @param op_type
 * @return struct ast_and_or*
 */
struct ast_and_or *new_and_or_ast(struct ast **pipes, size_t nb_pipes,
                                  enum operator_type *op_type)
{
    struct ast_and_or *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_AND_OR;
    base->free = free_and_or_ast;
    base->print = print_and_or_ast;
    base->evaluate = evaluate_and_or_ast;

    new->pipes = pipes;
    new->nb_pipes = nb_pipes;
    new->op_type = op_type;

    return new;
}
