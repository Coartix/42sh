#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief evaluate node input
 *
 * @param ast
 * @return int
 */
static int evaluate_input_ast(struct ast *ast)
{
    struct ast_input *ast_input = (struct ast_input *)ast;
    int res = 0;
    if (ast_input->list != NULL)
        res = ast_input->list->evaluate(ast_input->list);
    return res;
}

/**
 * @brief Print an AST_INPUT type node
 *
 * @param ast
 */
static void print_input_ast(struct ast *ast, char *pre_print)
{
    struct ast_input *ast_input = (struct ast_input *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_input\n", pre_print);
    if (ast_input->list != NULL)
        ast_input->list->print(ast_input->list, str);
    ast_input->end->print(ast_input->end, str);

    free_alloc(str);
}

/**
 * @brief Free a AST_INPUT type node and all his children
 *
 * @param ast
 */
static void free_input_ast(struct ast *ast)
{
    if (ast != NULL)
    {
        struct ast_input *input = (struct ast_input *)ast;
        if (input->list)
            input->list->free(input->list);
        input->end->free(input->end);
        free_alloc(ast);
    }
}

/**
 * @brief Create a AST_INPUT type node
 *
 * @param list
 * @param end
 * @return struct ast_input*
 */
struct ast_input *new_input_ast(struct ast *list, struct ast *end)
{
    struct ast_input *new = xmalloc(sizeof(*new));
    struct ast *base = (struct ast *)&new->base;

    base->type = AST_INPUT;
    base->free = free_input_ast;
    base->print = print_input_ast;
    base->evaluate = evaluate_input_ast;

    new->list = list;
    new->end = end;

    return new;
}
