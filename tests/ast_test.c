#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "alloc.h"
#include "ast.h"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(ast, ast_newl, .disabled = false, .init = cr_redirect_stdout)
{
    struct ast_newl *tmp = new_newl_ast();

    cr_assert_eq(tmp->base.type, AST_NEWL);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_newl\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[2] = { "", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: \n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_null, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[1] = { NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd:\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_single_quote, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "'truc'", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: echo truc\ntruc\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "'truc machin'", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");

    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo truc machin\ntruc machin\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote_tricky, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "'truc'machin", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo trucmachin\n"
                            "trucmachin\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote_backslash, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "'\\'truc", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo \\truc\n"
                            "\\truc\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote2_backslash2, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\''\\'truc", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo \'\\truc\n"
                            "\'\\truc\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote2_backslash3, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\''\\'\\", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo '\\\\\n"
                            "'\\\\\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_single_quote_backslash_hard, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\''\\n\\t\\\\'\\'", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo '\\n\\t\\\\'\n"
                            "'\\n\\t\\\\'\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_backslash2, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\\\oui", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo \\oui\n"
                            "\\oui\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_backslash3, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\\\\\oui", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo \\oui\n"
                            "\\oui\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_all_backslash_quote_very_hard, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "\\oui\\ non\\'quo''te", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);

    cr_assert_stdout_eq_str("node_simple_cmd: echo oui non'quote\n"
                            "oui non'quote\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_ls, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[2] = { "ls", NULL };

    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: ls\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_ls_la, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[3] = { "ls", "-la", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: ls -la\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_ls_many_args, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[5] = { "ls", "-la", "-la", "machin", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: ls -la -la machin\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_simple_cmd_ls_multiple_spaces, .disabled = false,
     .init = cr_redirect_stdout)
{
    char *tab[6] = { "ls", "-la", "-la", "\t\v\r", "machin", NULL };
    struct ast_simple_cmd *tmp = new_simple_cmd_ast(tab);

    cr_assert_eq(tmp->base.type, AST_SIMPLE_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_simple_cmd: ls -la -la \t\v\r machin\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_eof, .disabled = false, .init = cr_redirect_stdout)
{
    struct ast_eof *tmp = new_eof_ast();

    cr_assert_eq(tmp->base.type, AST_EOF);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_EOF\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_input, .disabled = false, .init = cr_redirect_stdout)
{
    struct ast_eof *eof = new_eof_ast();
    struct ast_input *tmp = new_input_ast(NULL, (struct ast *)eof);

    cr_assert_eq(tmp->base.type, AST_INPUT);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);
    cr_assert_eq(tmp->list, NULL);
    cr_assert_eq((struct ast_eof *)tmp->end, eof);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_input\n\tnode_EOF\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_input_test_child, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[3] = { "echo", "test", NULL };
    struct ast_input *tmp = new_input_ast((struct ast *)new_simple_cmd_ast(tab),
                                          (struct ast *)new_eof_ast());

    cr_assert_neq(tmp->list, NULL);
    cr_assert_neq(tmp->end, NULL);

    cr_assert_eq(tmp->base.type, AST_INPUT);
    cr_assert_eq(tmp->list->type, AST_SIMPLE_CMD);
    cr_assert_eq(tmp->end->type, AST_EOF);

    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);
    cr_assert_neq(tmp->list->free, NULL);
    cr_assert_neq(tmp->end->print, NULL);
    cr_assert_neq(tmp->list->free, NULL);
    cr_assert_neq(tmp->list->print, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_input\n\tnode_simple_cmd: echo test\n\tnode_EOF\n");

    tmp->base.free(&tmp->base);
    destroy_global();
}

Test(ast, ast_cmd, .disabled = false, .init = cr_redirect_stdout)
{
    char *tab[2] = { "ls", NULL };
    struct ast_simple_cmd *ast_simple_cmd = new_simple_cmd_ast(tab);
    struct ast_cmd *tmp = new_cmd_ast((struct ast *)ast_simple_cmd);

    cr_assert_eq(tmp->base.type, AST_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_cmd\n\tnode_simple_cmd: ls\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_sh_cmd, .disabled = false, .init = cr_redirect_stdout)
{
    // Here we give a simple_command to a shell_rule only for testing purposes
    char *tab[3] = { "ls", "-la", NULL };
    struct ast_simple_cmd *ast_simple_cmd = new_simple_cmd_ast(tab);
    struct ast_sh_cmd *tmp = new_sh_cmd_ast((struct ast *)ast_simple_cmd);

    cr_assert_eq(tmp->base.type, AST_SH_CMD);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_sh_cmd\n\tnode_simple_cmd: ls -la\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_if_cmd_else_not_null, .disabled = false,
     .init = cr_redirect_stdout)
{
    // Here we give a simple_command to a if_rule only for testing purposes
    char *tab[3] = { "ls", "-la", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd3 = new_simple_cmd_ast(tab);
    struct ast_if *tmp =
        new_if_ast((struct ast *)ast_simple_cmd1, (struct ast *)ast_simple_cmd2,
                   (struct ast *)ast_simple_cmd3);

    cr_assert_eq(tmp->base.type, AST_IF);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_if\n\tnode_simple_cmd: ls -la\n\tnode_simple_cmd: ls "
        "-la\n\tnode_simple_cmd: ls -la\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_if_cmd_else_null, .disabled = false, .init = cr_redirect_stdout)
{
    // Here we give a simple_command to a if_rule only for testing purposes
    char *tab[3] = { "ls", "-la", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab);
    struct ast_if *tmp = new_if_ast((struct ast *)ast_simple_cmd1,
                                    (struct ast *)ast_simple_cmd2, NULL);

    cr_assert_eq(tmp->base.type, AST_IF);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_if\n\tnode_simple_cmd: ls -la\n\tnode_simple_cmd: ls -la\n");
    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_else_cmd_else_null, .disabled = false, .init = cr_redirect_stdout)
{
    // Here we give a simple_command to a else_rule only for testing purposes
    char *tab[3] = { "echo", "coucou", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab);
    struct ast_else *tmp = new_else_ast((struct ast *)ast_simple_cmd1,
                                        (struct ast *)ast_simple_cmd2, NULL);

    cr_assert_eq(tmp->base.type, AST_ELSE);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);
    cr_assert_stdout_eq_str("node_else\n\tnode_simple_cmd: echo "
                            "coucou\n\tnode_simple_cmd: echo coucou\n"
                            "coucou\ncoucou\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_else_cmd_cond_NULL, .disabled = false,
     .init = redirect_all_stdout)
{
    // Here we give a simple_command to a else_rule only for testing purposes
    char *tab[3] = { "ech", "coucou", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_else *tmp =
        new_else_ast(NULL, (struct ast *)ast_simple_cmd1, NULL);

    cr_assert_eq(tmp->base.type, AST_ELSE);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_else\n\tnode_simple_cmd: ech coucou\n");

    int res = tmp->base.evaluate(&tmp->base);
    cr_assert_eq(res, 127);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_compound_true, .disabled = false, .init = redirect_all_stdout)
{
    char *tab[3] = { "ls", "-a", NULL };
    char *tab2[2] = { "ls", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab2);

    struct ast **cmds = xmalloc(2 * sizeof(*cmds));
    cmds[0] = (struct ast *)ast_simple_cmd1;
    cmds[1] = (struct ast *)ast_simple_cmd2;
    struct ast_compound_list *tmp = new_compound_list_ast(cmds, 2);

    cr_assert_eq(tmp->base.type, AST_COMPOUND_LIST);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");

    fflush(stdout);
    cr_assert_stdout_eq_str("node_compound_list\n\tnode_simple_cmd: ls -a"
                            "\n\tnode_simple_cmd: ls\n");

    int res = tmp->base.evaluate(&tmp->base);
    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_compound_false, .disabled = false, .init = redirect_all_stdout)
{
    char *tab[3] = { "ech", "coucou", NULL };
    char *tab2[] = { "ls", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab2);

    struct ast **cmds = xmalloc(2 * sizeof(*cmds));
    cmds[0] = (struct ast *)ast_simple_cmd1;
    cmds[1] = (struct ast *)ast_simple_cmd2;
    struct ast_compound_list *tmp = new_compound_list_ast(cmds, 2);

    cr_assert_eq(tmp->base.type, AST_COMPOUND_LIST);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    fflush(stdout);
    cr_assert_stdout_eq_str("node_compound_list\n\tnode_simple_cmd: ech "
                            "coucou\n\tnode_simple_cmd: ls\n");

    int res = tmp->base.evaluate(&tmp->base);
    cr_assert_eq(res, 127);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_do_group, .disabled = false, .init = redirect_all_stdout)
{
    // Here we give a simple_command to a else_rule only for testing purposes
    char *tab[3] = { "echo", "coucou", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_do_group *tmp = new_do_group_ast((struct ast *)ast_simple_cmd1);

    cr_assert_eq(tmp->base.type, AST_DOGROUP);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_do_group\n\tnode_simple_cmd: echo coucou\ncoucou\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_while_false, .disabled = false, .init = redirect_all_stdout)
{
    // Here we give a simple_command to a else_rule only for testing purposes
    char *tab[3] = { "echo", "coucou", NULL };
    char *tab2[3] = { "ech", "coucou", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab2);
    struct ast_do_group *do_group =
        new_do_group_ast((struct ast *)ast_simple_cmd1);
    struct ast_while *tmp =
        new_while_ast((struct ast *)ast_simple_cmd2, (struct ast *)do_group);

    cr_assert_eq(tmp->base.type, AST_WHILE);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");

    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_while\n\tnode_simple_cmd: ech "
        "coucou\n\tnode_do_group\n\t\tnode_simple_cmd: echo coucou\n");

    int res = tmp->base.evaluate(&tmp->base);
    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}

Test(ast, ast_until_false, .disabled = false, .init = redirect_all_stdout)
{
    // Here we give a simple_command to a else_rule only for testing purposes
    char *tab[3] = { "echo", "coucou", NULL };
    struct ast_simple_cmd *ast_simple_cmd1 = new_simple_cmd_ast(tab);
    struct ast_simple_cmd *ast_simple_cmd2 = new_simple_cmd_ast(tab);
    struct ast_do_group *do_group =
        new_do_group_ast((struct ast *)ast_simple_cmd1);
    struct ast_until *tmp =
        new_until_ast((struct ast *)ast_simple_cmd2, (struct ast *)do_group);

    cr_assert_eq(tmp->base.type, AST_UNTIL);
    cr_assert_neq(tmp->base.free, NULL);
    cr_assert_neq(tmp->base.print, NULL);
    cr_assert_neq(tmp->base.evaluate, NULL);

    tmp->base.print(&tmp->base, "");
    int res = tmp->base.evaluate(&tmp->base);
    fflush(stdout);
    cr_assert_stdout_eq_str(
        "node_until\n\tnode_simple_cmd: echo "
        "coucou\n\tnode_do_group\n\t\tnode_simple_cmd: echo coucou\ncoucou\n");

    cr_assert_eq(res, TRUE);

    tmp->base.free(&tmp->base);

    destroy_global();
}
