#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate node list
 *
 * @param ast
 * @return int
 */
static int evaluate_list_ast(struct ast *ast)
{
    struct ast_list *ast_list = (struct ast_list *)ast;

    int res = TRUE;
    for (size_t i = 0; i < ast_list->nb_cmds && res == TRUE; i++)
        res = ast_list->cmds[i]->evaluate(ast_list->cmds[i]);
    return res;
}

/**
 * @brief Print a AST_LIST type node
 *
 * @param ast
 */
static void print_list_ast(struct ast *ast, char *pre_print)
{
    struct ast_list *ast_list = (struct ast_list *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_list\n", pre_print);
    for (size_t i = 0; i < ast_list->nb_cmds; i++)
        ast_list->cmds[i]->print(ast_list->cmds[i], str);

    free_alloc(str);
}

/**
 * @brief Free a AST_LIST type node and all his children
 *
 * @param ast
 */
static void free_list_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_list *list = (struct ast_list *)ast;
        for (size_t i = 0; i < list->nb_cmds; i++)
        {
            list->cmds[i]->free(list->cmds[i]);
        }
        free_alloc(list->cmds);
        free_alloc(list);
    }
}

/**
 * @brief
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_list*
 */
struct ast_list *new_list_ast(struct ast **cmds, size_t nb_cmds)
{
    struct ast_list *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_LIST;
    base->free = free_list_ast;
    base->print = print_list_ast;
    base->evaluate = evaluate_list_ast;

    new->cmds = cmds;
    new->nb_cmds = nb_cmds;

    return new;
}
