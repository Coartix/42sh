#include <criterion/criterion.h>
#include <stdio.h>

#include "alloc.h"
#include "lexer.h"
#include "process_args.h"

Test(lexer, lexer_new, .disabled = false)
{
    char *input = "ls";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(input, lexer->input);
    cr_assert_neq(lexer->pos, 0);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, input);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_new_empty_input, .disabled = false)
{
    char *input = "";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(input, lexer->input);
    cr_assert_neq(lexer->pos, 0);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);
    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek, .disabled = false)
{
    char *input = "ls";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(input, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek_multiple_times, .disabled = false)
{
    char *input = "ls";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(input, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek_EOF, .disabled = false)
{
    char *input = "";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek_ERROR, .disabled = false)
{
    char *input = "£";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq("£", lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek_ERROR_with_cmd, .disabled = false)
{
    char *input = "ls;";
    char *expected = "ls";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, expected);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_all, .disabled = false)
{
    char *input = "ls";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(input, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_ERROR, .disabled = false)
{
    char *input = "ls\a";
    char *expected = "ls\a";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, expected);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_EOF, .disabled = false)
{
    char *input = "";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_big2, .disabled = false)
{
    char *input = "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls";
    struct lexer *lexer = lexer_new(input);

    while (lexer_peek(lexer)->type != TOKEN_EOF)
        lexer_pop(lexer);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_big2_ERROR, .disabled = false)
{
    char *input = "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls ls "
                  "ls ls ls ls ls ls ls ls ls ls ls ls;";
    struct lexer *lexer = lexer_new(input);

    while (lexer_peek(lexer)->type != TOKEN_EOF)
        lexer_pop(lexer);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_peek_NEWL, .disabled = false)
{
    char *input = "\n";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_pop_NEWL, .disabled = false)
{
    char *input = "ls\n";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq("ls", lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_free(lexer);

    destroy_global();
}

Test(lexer, lexer_pop_NEWLx2, .disabled = false)
{
    char *input = "ls\n\n";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq("ls", lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);
    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);
    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);
    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_HARD, .disabled = false)
{
    char *input = "echo test;\n ls \n \a ";
    struct lexer *lexer = lexer_new(input);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "test");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_SMCL);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "ls");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\a");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_if, .disabled = false)
{
    char *input = "if";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_IF);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_then, .disabled = false)
{
    char *input = "then";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_THEN);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_fi, .disabled = false)
{
    char *input = "fi";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_FI);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_else, .disabled = false)
{
    char *input = "else";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ELSE);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_elif, .disabled = false)
{
    char *input = "elif";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ELIF);

    lexer_pop(lexer);

    cr_assert_eq(NULL, lexer_peek(lexer)->cmd);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_if_cond, .disabled = false)
{
    char *input = "if echo -e toto; then echo truc\n fi";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_IF);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "-e");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "toto");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_SMCL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_THEN);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "truc");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_FI);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_if_then, .disabled = false)
{
    char *input = "if then";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_IF);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_THEN);
    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_get_keyword_basic_If_HARD, .disabled = false)
{
    char *input = "if echo -e toto; then echo truc\n elif ls; else tree \n fi";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_IF);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "-e");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "toto");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_SMCL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_THEN);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "truc");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ELIF);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "ls");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_SMCL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ELSE);

    lexer_pop(lexer);

    cr_assert_str_eq(lexer_peek(lexer)->cmd, "tree");
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_NEWL);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_FI);

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);
    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_EOF);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_single_quote_basic, .disabled = false)
{
    char *input = "\'test\'";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\'test\'");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_single_quote_error, .disabled = false)
{
    char *input = "\'test";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ERROR);
    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_single_quote_escaped, .disabled = false)
{
    char *input = "\\\'\'test\'";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\\\'\'test\'");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_quote_basic, .disabled = false)
{
    char *input = "\"test\"";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\"test\"");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_quote_error, .disabled = false)
{
    char *input = "\"test";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ERROR);
    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_quote_escaped, .disabled = false)
{
    char *input = "\\\"\"test\"";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\\\"\"test\"");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_single_quote, .disabled = false)
{
    char *input = "\"'test\"";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "\"'test\"");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_single_quote_escaped_hard, .disabled = false)
{
    char *input = "echo '\\'\"'\"\\'\"test \"";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "'\\'\"'\"\\'\"test \"");

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_double_single_quote_escaped_hard_error, .disabled = false)
{
    char *input = "echo '\\'\"\"\\'\\\"test\"";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_WORD);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "echo");

    lexer_pop(lexer);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_ERROR);
    cr_assert_eq(lexer_peek(lexer)->cmd, NULL);

    lexer_free(lexer);
    destroy_global();
}

Test(lexer, lexer_AND, .disabled = false)
{
    char *input = "&&";
    struct lexer *lexer = lexer_new(input);

    cr_assert_eq(lexer_peek(lexer)->type, TOKEN_AND);
    cr_assert_str_eq(lexer_peek(lexer)->cmd, "&&");

    lexer_free(lexer);
    destroy_global();
}
