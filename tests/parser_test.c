#include <criterion/criterion.h>

#include "alloc.h"
#include "parser.h"

Test(Parser, basic, .disabled = false)
{
    char *input = "ls";
    struct ast *ast = NULL;
    enum parser_status res = parse(&ast, input);
    struct ast_input *ast_input = (struct ast_input *)ast;
    struct ast_compound_list *ast_cmp_list =
        (struct ast_compound_list *)ast_input->list;
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast_cmp_list->cmds[0];
    struct ast_pipe *ast_pipe = (struct ast_pipe *)ast_and_or->pipes[0];
    struct ast_simple_cmd *ast_simple_cmd =
        (struct ast_simple_cmd *)ast_pipe->cmds[0];

    cr_assert_neq(ast, NULL);
    cr_assert_eq(ast->type, AST_INPUT);
    cr_assert_eq(ast_input->list->type, AST_COMPOUND_LIST);
    cr_assert_str_eq(ast_simple_cmd->cmd, "ls");
    cr_assert_eq(ast_cmp_list->nb_cmds, 1);
    cr_assert_eq(ast_input->end->type, AST_EOF);
    cr_assert_eq(res, PARSER_OK);

    ast->free(ast);
    destroy_global();
}

Test(Parser, basic_newl, .disabled = false)
{
    char *input = "ls\n";
    struct ast *ast = NULL;
    enum parser_status res = parse(&ast, input);
    struct ast_input *ast_input = (struct ast_input *)ast;
    struct ast_compound_list *ast_cmp_list =
        (struct ast_compound_list *)ast_input->list;
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast_cmp_list->cmds[0];
    struct ast_pipe *ast_pipe = (struct ast_pipe *)ast_and_or->pipes[0];
    struct ast_simple_cmd *ast_simple_cmd =
        (struct ast_simple_cmd *)ast_pipe->cmds[0];

    cr_assert_neq(ast, NULL);
    cr_assert_eq(ast->type, AST_INPUT);
    cr_assert_eq(ast_input->list->type, AST_COMPOUND_LIST);
    cr_assert_str_eq(ast_simple_cmd->cmd, "ls");
    cr_assert_eq(ast_cmp_list->nb_cmds, 1);
    cr_assert_eq(ast_input->end->type, AST_EOF);
    cr_assert_eq(res, PARSER_OK);

    ast->free(ast);
    destroy_global();
}

Test(Parser, basic_empty, .disabled = false)
{
    char *input = "";
    struct ast *ast = NULL;
    enum parser_status res = parse(&ast, input);
    struct ast_input *ast_input = (struct ast_input *)ast;

    cr_assert_neq(ast, NULL);
    cr_assert_eq(ast->type, AST_INPUT);
    cr_assert_eq(ast_input->list, NULL);
    cr_assert_eq(ast_input->end->type, AST_EOF);
    cr_assert_eq(res, PARSER_OK);

    ast->free(ast);
    destroy_global();
}

Test(Parser, basic_newl_only, .disabled = false)
{
    char *input = "\n";
    struct ast *ast = NULL;
    enum parser_status res = parse(&ast, input);
    struct ast_input *ast_input = (struct ast_input *)ast;

    cr_assert_neq(ast, NULL);
    cr_assert_eq(ast->type, AST_INPUT);
    cr_assert_eq(ast_input->list, NULL);
    cr_assert_eq(ast_input->end->type, AST_NEWL);
    cr_assert_eq(res, PARSER_OK);

    ast->free(ast);
    destroy_global();
}

Test(Parser, basic_compound_list, .disabled = false)
{
    char *input = "ls;ls;\n";
    struct ast *ast = NULL;
    enum parser_status res = parse(&ast, input);
    struct ast_input *ast_input = (struct ast_input *)ast;
    struct ast_compound_list *ast_cmp_list =
        (struct ast_compound_list *)ast_input->list;
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast_cmp_list->cmds[0];
    struct ast_pipe *ast_pipe = (struct ast_pipe *)ast_and_or->pipes[0];
    struct ast_simple_cmd *ast_simple_cmd =
        (struct ast_simple_cmd *)ast_pipe->cmds[0];

    cr_assert_neq(ast, NULL);
    cr_assert_eq(ast->type, AST_INPUT);
    cr_assert_neq(ast_input->list, NULL);
    cr_assert_eq(ast_input->end->type, AST_EOF);
    cr_assert_eq(res, PARSER_OK);

    cr_assert_eq(ast_cmp_list->base.type, AST_COMPOUND_LIST);
    cr_assert_eq(ast_cmp_list->nb_cmds, 2);

    cr_assert_str_eq(ast_simple_cmd->cmd, "ls");

    ast->free(ast);
    destroy_global();
}
