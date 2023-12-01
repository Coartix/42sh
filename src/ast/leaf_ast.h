#pragma once

#include "base_ast.h"

/**
 * @brief Leaf containing the cmd to run
 *
 */
struct ast_simple_cmd
{
    struct ast base;
    char *cmd;
    char **args;
    char *all_cmd;
};

/**
 * @brief Leaf indicating a new line
 *
 */
struct ast_newl
{
    struct ast base;
};

/**
 * @brief Leaf indicating the end of the input
 *
 */
struct ast_eof
{
    struct ast base;
};
