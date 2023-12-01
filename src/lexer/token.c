#include "token.h"

#include <err.h>
#include <stdlib.h>

#include "alloc.h"

/**
 * @brief Create a new token of type token_type. The cmd pointer is set to NULL.
 *
 * @param type
 * @return struct token*
 */
struct token *token_new(enum token_type type)
{
    struct token *new = zalloc(sizeof(struct token));
    new->type = type;
    new->cmd = NULL;
    return new;
}

/**
 * @brief Free the cmd pointer and the token.
 *
 * @param token
 */
void token_free(struct token *token)
{
    if (token)
    {
        if (token->cmd)
        {
            free_alloc(token->cmd);
            token->cmd = NULL;
        }

        free_alloc(token);
    }
}
