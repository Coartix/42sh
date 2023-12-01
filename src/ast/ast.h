#pragma once

#include <unistd.h>

#include "ast_constructors.h"
#include "defines.h"

/**
 * @brief print the all ast
 *
 * @param ast
 */
void print_ast(struct ast *ast);

/**
 * @brief free the all ast
 *
 * @param ast
 */
void free_ast(struct ast *ast);

/**
 * @brief evaluate all ast
 *
 * @param ast
 * @return int
 */
int evaluate_ast(struct ast *ast);
