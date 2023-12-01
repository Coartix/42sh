#include "alloc.h"
#include "ast.h"
#include "rules.h"

static void skip_newl(struct lexer **lexer)
{
    while (lexer_peek(*lexer)->type == TOKEN_NEWL)
        lexer_pop(*lexer);
}

static int in_rule(struct lexer **lexer, struct ast *ast)
{
    int res = lexer_peek(*lexer)->type == TOKEN_NEWL;
    res |= lexer_peek(*lexer)->type == TOKEN_SMCL;
    return res && ast != NULL;
}

/**
 * @brief coumpound_list rule
 *
 * compound_list: compound_list: ('\n')* and_or ((';'|'\n') ('\n')* and_or)*
 * [(';'|'\n') ('\n')*]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_compound_list(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast **cmds = xmalloc(sizeof(*cmds));
    size_t nb_cmds = 1;

    // First part: ('\n')* and_or

    skip_newl(lexer);

    cmds[nb_cmds - 1] = parse_and_or(lexer);

    // Second part:  ((';'|'\n') ('\n')* and_or)* [(';'|'\n') ('\n')*]

    if (cmds[0] != NULL)
    {
        while (in_rule(lexer, cmds[nb_cmds - 1]))
        {
            lexer_pop(*lexer);

            skip_newl(lexer);

            // Check parse_and_or
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
        res = (struct ast *)new_compound_list_ast(cmds, nb_cmds);
    }
    else // Free because of error
        free_alloc(cmds);

    return res;
}
