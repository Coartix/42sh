#include "ast.h"
#include "rules.h"

/**
 * @brief shell_command: rule_if
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_sh_cmd(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast *rule = NULL;
    struct lexer *lexer_tmp = NULL;

    lexer_tmp = lexer_new((*lexer)->input);
    if (rule == NULL && (rule = parse_rule_if(lexer)) != NULL)
    {
        res = (struct ast *)new_sh_cmd_ast(rule);
        lexer_free(lexer_tmp);
    }
    else
    {
        lexer_free(*lexer);
        *lexer = lexer_tmp;
    }

    lexer_tmp = lexer_new((*lexer)->input);
    if (rule == NULL && (rule = parse_while(lexer)) != NULL)
    {
        res = (struct ast *)new_sh_cmd_ast(rule);
        lexer_free(lexer_tmp);
    }
    else
    {
        lexer_free(*lexer);
        *lexer = lexer_tmp;
    }

    lexer_tmp = lexer_new((*lexer)->input);
    if (rule == NULL && (rule = parse_until(lexer)) != NULL)
    {
        res = (struct ast *)new_sh_cmd_ast(rule);
        lexer_free(lexer_tmp);
    }
    else
    {
        lexer_free(*lexer);
        *lexer = lexer_tmp;
    }

    return res;
}
