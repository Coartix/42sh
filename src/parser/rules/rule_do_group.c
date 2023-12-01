#include "ast.h"
#include "rules.h"

/**
 * @brief do_group: Do compound_list Done
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_do_group(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast *cmp_list = NULL;

    if (lexer_peek(*lexer)->type == TOKEN_DO)
    {
        lexer_pop(*lexer);
        cmp_list = parse_compound_list(lexer);
        if (cmp_list != NULL && lexer_peek(*lexer)->type == TOKEN_DONE)
        {
            lexer_pop(*lexer);
            res = (struct ast *)new_do_group_ast(cmp_list);
        }
        else if (cmp_list != NULL)
            cmp_list->free(cmp_list);
    }
    return res;
}
