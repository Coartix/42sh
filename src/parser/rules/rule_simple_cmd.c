#include "alloc.h"
#include "ast.h"
#include "rules.h"
#include "string.h"

/**
 * @brief simple_command rule
 *
 * simple_command: WORD+
 *
 * @param ast
 * @param lexer
 * @return enum parser_status
 */
struct ast *parse_simple_cmd(struct lexer **lexer)
{
    size_t nb_words = 1;
    char **words = xmalloc(nb_words * sizeof(*words));
    words[nb_words - 1] = NULL;
    struct ast *res = NULL;
    size_t len = 0;

    if (lexer_peek(*lexer)->type == TOKEN_WORD)
    {
        len = strlen(lexer_peek(*lexer)->cmd);

        words = xrealloc(words, (nb_words + 1) * sizeof(*words));
        words[nb_words] = NULL;
        words[nb_words - 1] = xmalloc(len + 1);
        memcpy(words[nb_words - 1], lexer_peek(*lexer)->cmd, len + 1);
        nb_words++;

        lexer_pop(*lexer);

        while (lexer_peek(*lexer)->type == TOKEN_WORD)
        {
            len = strlen(lexer_peek(*lexer)->cmd);

            words = xrealloc(words, (nb_words + 1) * sizeof(*words));
            words[nb_words] = NULL;
            words[nb_words - 1] = xmalloc(len + 1);
            memcpy(words[nb_words - 1], lexer_peek(*lexer)->cmd, len + 1);
            nb_words++;
            lexer_pop(*lexer);
        }
    }

    if (words[0] != NULL && lexer_peek(*lexer)->type != TOKEN_ERROR)
        res = (struct ast *)new_simple_cmd_ast(words);

    for (size_t i = 0; i < nb_words - 1; i++)
        free_alloc(words[i]);
    free_alloc(words);

    return res;
}
