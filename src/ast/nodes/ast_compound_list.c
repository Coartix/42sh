#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an AST_COMPOUND_LIST type node
 *
 * @param ast
 * @return int
 */
static int evaluate_compound_list_ast(struct ast *ast)
{
    struct ast_compound_list *ast_compound_list =
        (struct ast_compound_list *)ast;

    int res = TRUE;
    for (size_t i = 0; i < ast_compound_list->nb_cmds && res == TRUE; i++)
        res = ast_compound_list->cmds[i]->evaluate(ast_compound_list->cmds[i]);
    return res;
}

/**
 * @brief Print an AST_COMPOUND_LIST type node
 *
 * @param ast
 */
static void print_compound_list_ast(struct ast *ast, char *pre_print)
{
    struct ast_compound_list *ast_compound_list =
        (struct ast_compound_list *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_compound_list\n", pre_print);
    for (size_t i = 0; i < ast_compound_list->nb_cmds; i++)
        ast_compound_list->cmds[i]->print(ast_compound_list->cmds[i], str);

    free_alloc(str);
}

/**
 * @brief Free an AST_COMPOUND_LIST type node and all his children(s)
 *
 * @param ast
 */
static void free_compound_list_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_compound_list *compound_list =
            (struct ast_compound_list *)ast;
        for (size_t i = 0; i < compound_list->nb_cmds; i++)
        {
            compound_list->cmds[i]->free(compound_list->cmds[i]);
        }
        free_alloc(compound_list->cmds);
        free_alloc(compound_list);
    }
}

/**
 * @brief Create new compound list
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_compound_list*
 */
struct ast_compound_list *new_compound_list_ast(struct ast **cmds,
                                                size_t nb_cmds)
{
    struct ast_compound_list *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_COMPOUND_LIST;
    base->free = free_compound_list_ast;
    base->print = print_compound_list_ast;
    base->evaluate = evaluate_compound_list_ast;

    new->cmds = cmds;
    new->nb_cmds = nb_cmds;

    return new;
}
