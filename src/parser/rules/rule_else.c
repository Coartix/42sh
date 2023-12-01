#include "ast.h"
#include "rules.h"

/**
 * @brief rule else_clause
 *
 * else_clause: Else compound_list | Elif compound_list Then compound_list
 * [else_clause]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_else_clause(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast *cond = NULL;
    struct ast *cmd = NULL;
    struct ast *else_clause = NULL;

    if (lexer_peek(*lexer)->type == TOKEN_ELSE)
    {
        lexer_pop(*lexer);
        cmd = parse_compound_list(lexer);

        if (cmd != NULL)
            res = (struct ast *)new_else_ast(NULL, cmd, NULL);
    }
    else if (lexer_peek(*lexer)->type == TOKEN_ELIF)
    {
        lexer_pop(*lexer);
        cond = parse_compound_list(lexer);

        if (cond != NULL && lexer_peek(*lexer)->type == TOKEN_THEN)
        {
            lexer_pop(*lexer);
            cmd = parse_compound_list(lexer);

            if (cmd != NULL)
            {
                struct lexer *lexer_tmp = lexer_new((*lexer)->input);
                else_clause = parse_else_clause(lexer); // try to parse else

                if (else_clause == NULL)
                {
                    lexer_free(*lexer);
                    *lexer = lexer_tmp;
                }
                else
                    lexer_free(lexer_tmp);

                res = (struct ast *)new_else_ast(cond, cmd, else_clause);
            }
        }
    }

    // Free everything in case of error
    if (res == NULL)
    {
        if (cond != NULL)
            cond->free(cond);
        if (cmd != NULL)
            cmd->free(cmd);
        if (else_clause != NULL)
            else_clause->free(else_clause);
    }

    return res;
}
