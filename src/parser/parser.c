#include "parser.h"

#include "alloc.h"
#include "rules.h"

/**
 * @brief
 *
 * @param ast
 * @param lexer
 * @return enum parser_status
 */
enum parser_status parse(struct ast **ast, char *input)
{
    struct lexer *lexer = lexer_new(input);
    enum parser_status res = PARSER_OK;

    *ast = parse_input(&lexer);
    if (*ast == NULL)
        res = PARSER_UNEXPECTED_TOKEN;

    lexer_free(lexer);
    return res;
}
