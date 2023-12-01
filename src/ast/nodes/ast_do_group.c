#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an ast_do_group type node
 *
 * @param ast
 * @return int
 */
static int evaluate_do_group_ast(struct ast *ast)
{
    struct ast_do_group *ast_do_group = (struct ast_do_group *)ast;

    return ast_do_group->comp_list->evaluate(ast_do_group->comp_list);
}

/**
 * @brief Free an ast_do_group type node
 *
 * @param ast
 */
static void ast_do_group_free(struct ast *ast)
{
    struct ast_do_group *do_group = (struct ast_do_group *)ast;
    if (do_group != NULL)
    {
        do_group->comp_list->free(do_group->comp_list);
        free_alloc(do_group);
    }
}

/**
 * @brief Print an ast_do_group type node
 *
 * @param ast
 * @param pre_print
 */
static void print_do_group_ast(struct ast *ast, char *pre_print)
{
    struct ast_do_group *do_group = (struct ast_do_group *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_do_group\n", pre_print);

    do_group->comp_list->print(do_group->comp_list, str);

    free_alloc(str);
}

/**
 * @brief Create an ast_do_group type node
 *
 * @param comp_list
 * @return struct ast_do_group
 *
 */
struct ast_do_group *new_do_group_ast(struct ast *comp_list)
{
    struct ast_do_group *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_do_group_ast;
    base->free = ast_do_group_free;
    base->evaluate = evaluate_do_group_ast;
    base->type = AST_DOGROUP;

    new->comp_list = comp_list;

    return new;
}
