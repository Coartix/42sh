#include "rules.h"

/**
 * @brief command: simple_command | shell_command
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_cmd(struct lexer **lexer)
{
    struct lexer *lexer_tmp = lexer_new((*lexer)->input);

    struct ast *res = parse_simple_cmd(lexer);

    if (res == NULL)
    {
        lexer_free(*lexer);
        *lexer = lexer_tmp;
        res = parse_sh_cmd(lexer);
    }
    else
        lexer_free(lexer_tmp);

    return res;
}
