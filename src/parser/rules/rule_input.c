#include "ast.h"
#include "rules.h"

/**
 * @brief input rule
 *
 * input:  list '\n'  |  list EOF | '\n' | EOF
 *
 * @param ast
 * @param lexer
 * @return enum parser_status
 */
struct ast *parse_input(struct lexer **lexer)
{
    struct lexer *lexer_tmp = lexer_new((*lexer)->input);

    struct ast *list = parse_compound_list(lexer);

    if (list == NULL)
    {
        lexer_free(*lexer);
        *lexer = lexer_tmp;
    }
    else
        lexer_free(lexer_tmp);

    struct ast *res = NULL;
    if ((*lexer)->current_tok->type == TOKEN_EOF)
        res = (struct ast *)new_input_ast(list, (struct ast *)new_eof_ast());
    else if ((*lexer)->current_tok->type == TOKEN_NEWL)
        res = (struct ast *)new_input_ast(list, (struct ast *)new_newl_ast());
    else if (list != NULL)
        list->free(list);

    return res;
}
