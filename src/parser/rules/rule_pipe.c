#include <string.h>

#include "alloc.h"
#include "ast.h"
#include "rules.h"

/**
 * @brief skip all newlines in a row from lexer
 *
 * @param lexer
 */
static void skip_newl(struct lexer **lexer)
{
    while (lexer_peek(*lexer)->type == TOKEN_NEWL)
        lexer_pop(*lexer);
}

/**
 * @brief pipe rule
 *
 * pipeline: command ('|' ('\n')* command)*
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_pipe(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast **cmds = xmalloc(sizeof(*cmds));
    size_t nb_cmds = 1;

    cmds[nb_cmds - 1] = parse_cmd(lexer);

    if (cmds[0] != NULL)
    {
        while (lexer_peek(*lexer)->type == TOKEN_PIPE)
        {
            lexer_pop(*lexer);

            skip_newl(lexer);

            // Check parse_cmd
            struct lexer *lexer_tmp = lexer_new((*lexer)->input);

            struct ast *tmp = parse_cmd(lexer);

            if (tmp != NULL)
            {
                nb_cmds++;
                cmds = xrealloc(cmds, nb_cmds * sizeof(*cmds));
                cmds[nb_cmds - 1] = tmp;
                lexer_free(lexer_tmp);
            }
            else
            {
                lexer_free(*lexer);
                *lexer = lexer_tmp;
            }
        }
        res = (struct ast *)new_pipe_ast(cmds, nb_cmds);
    }
    else // Free because of error
        free_alloc(cmds);

    return res;
}
