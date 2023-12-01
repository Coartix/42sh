#include "alloc.h"
#include "ast.h"
#include "rules.h"

/**
 * @brief
 *
 * list : and_or (';' and_or)* [;]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_list(struct lexer **lexer)
{
    size_t nb_cmds = 1;
    struct ast **cmds = xmalloc(nb_cmds * sizeof(*cmds));
    struct ast *res = NULL;

    cmds[nb_cmds - 1] = parse_and_or(lexer);

    if (cmds[0] != NULL)
    {
        while (lexer_peek(*lexer)->type == TOKEN_SMCL
               && cmds[nb_cmds - 1] != NULL)
        {
            lexer_pop(*lexer);
            struct lexer *lexer_tmp = lexer_new((*lexer)->input);

            struct ast *tmp = parse_and_or(lexer);

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

        res = (struct ast *)new_list_ast(cmds, nb_cmds);
    }
    else
        free_alloc(cmds);

    return res;
}
