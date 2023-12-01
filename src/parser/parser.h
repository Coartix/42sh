#pragma once

#include "ast.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 * @brief
 *
 * @param ast
 * @param input
 * @return enum parser_status
 */
enum parser_status parse(struct ast **ast, char *input);
