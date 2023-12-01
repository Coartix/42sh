#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "ast.h"

/**
 * @brief Evaluate an ast_sh_cmd type node
 *
 * @param ast
 * @return int
 */
static int evaluate_sh_cmd_ast(struct ast *ast)
{
    struct ast_sh_cmd *ast_sh_cmd = (struct ast_sh_cmd *)ast;

    return ast_sh_cmd->sh_rule->evaluate(ast_sh_cmd->sh_rule);
}

/**
 * @brief Free an ast_sh_cmd type node
 *
 * @param ast
 */
static void ast_sh_cmd_free(struct ast *ast)
{
    struct ast_sh_cmd *sh_rule = (struct ast_sh_cmd *)ast;
    if (sh_rule != NULL)
    {
        sh_rule->sh_rule->free(sh_rule->sh_rule);
        free_alloc(sh_rule);
    }
}

/**
 * @brief Print an ast_sh_cmd type node
 *
 * @param ast
 * @param pre_print
 */
static void print_sh_cmd_ast(struct ast *ast, char *pre_print)
{
    struct ast_sh_cmd *sh_rule = (struct ast_sh_cmd *)ast;

    char *str = xmalloc(strlen(pre_print) + 2);
    sprintf(str, "%s\t", pre_print);

    printf("%snode_sh_cmd\n", pre_print);

    sh_rule->sh_rule->print(sh_rule->sh_rule, str);

    free_alloc(str);
}

/**
 * @brief Create an ast_sh_cmd type node
 *
 * @param sh_rule
 * @return struct ast_sh_cmd
 *
 */
struct ast_sh_cmd *new_sh_cmd_ast(struct ast *sh_rule)
{
    struct ast_sh_cmd *new = xmalloc(sizeof(*new));

    struct ast *base = (struct ast *)&new->base;
    base->print = print_sh_cmd_ast;
    base->free = ast_sh_cmd_free;
    base->evaluate = evaluate_sh_cmd_ast;
    base->type = AST_SH_CMD;

    new->sh_rule = sh_rule;

    return new;
}
