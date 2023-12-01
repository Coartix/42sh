#include "ast.h"
#include "rules.h"

/**
 * @brief rule_while: While compound_list do_group
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_while(struct lexer **lexer)
{
    struct ast *res = NULL;
    struct ast *cond = NULL;
    struct ast *do_group = NULL;

    if (lexer_peek(*lexer)->type == TOKEN_WHILE)
    {
        lexer_pop(*lexer);
        cond = parse_compound_list(lexer);
        do_group = parse_do_group(lexer);

        if (cond != NULL && do_group != NULL)
            res = (struct ast *)new_while_ast(cond, do_group);
        else
        {
            if (cond != NULL)
                cond->free(cond);
            if (do_group != NULL)
                do_group->free(do_group);
        }
    }
    return res;
}
