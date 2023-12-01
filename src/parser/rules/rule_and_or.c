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
 * @brief check if and, or is present
 *
 * @param lexer
 * @param ast
 * @param op_type
 * @param nb_op
 * @return int
 */
static int in_rule(struct lexer **lexer, enum operator_type **op_type,
                   size_t nb_op)
{
    int res = 0;
    if (lexer_peek(*lexer)->type == TOKEN_AND)
    {
        res = 1;
        *op_type = xrealloc(*op_type, sizeof(*op_type) * (nb_op + 1));
        (*op_type)[nb_op] = AND;
    }
    else if (lexer_peek(*lexer)->type == TOKEN_OR)
    {
        res = 1;
        *op_type = xrealloc(*op_type, sizeof(*op_type) * (nb_op + 1));
        (*op_type)[nb_op] = OR;
    }
    return res;
}

/**
 * @brief and_or rule
 *
 * and_or: pipeline (('&&'|'||') ('\n')* pipeline)*
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_and_or(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast **pipes = xmalloc(sizeof(*pipes));
    enum operator_type *op_type = NULL;
    size_t nb_pipes = 1;

    pipes[nb_pipes - 1] = parse_pipe(lexer);

    if (pipes[0] != NULL)
    {
        while (in_rule(lexer, &op_type, nb_pipes - 1))
        {
            lexer_pop(*lexer);

            skip_newl(lexer);

            // Check parse_pipe
            struct lexer *lexer_tmp = lexer_new((*lexer)->input);

            struct ast *tmp = parse_pipe(lexer);

            if (tmp != NULL)
            {
                nb_pipes++;
                pipes = xrealloc(pipes, nb_pipes * sizeof(*pipes));
                pipes[nb_pipes - 1] = tmp;
                lexer_free(lexer_tmp);
            }
            else
            {
                lexer_free(*lexer);
                *lexer = lexer_tmp;
            }
        }
        res = (struct ast *)new_and_or_ast(pipes, nb_pipes, op_type);
    }
    else // Free because of error
        free_alloc(pipes);

    return res;
}
