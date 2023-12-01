#pragma once

#include "base_ast.h"

/**
 * @brief Node containing the node cmd to run
 *
 */
struct ast_cmd
{
    struct ast base;
    struct ast *cmd;
};

/**
 * @brief Node containing the node sh_rule to run
 *
 */
struct ast_sh_cmd
{
    struct ast base;
    struct ast *sh_rule;
};

/**
 * @brief Node containing the nodes condition, comand and possibly else clause
 * to run
 *
 */
struct ast_if
{
    struct ast base;
    struct ast *cond;
    struct ast *cmd;
    struct ast *else_clause;
};

/**
 * @brief Node containing the nodes condition (possibly NULL),
 * comand and possibly else clause to run
 *
 */
struct ast_else
{
    struct ast base;
    struct ast *cond;
    struct ast *cmd;
    struct ast *else_clause;
};

/**
 * @brief Rule node having list of commands ending with ;
 *
 */
struct ast_list
{
    struct ast base;
    size_t nb_cmds;
    struct ast **cmds;
};

/**
 * @brief Rule input, that could have up to two children.
 * The first is the command to run (if there is one).
 * The second is the delimiter ('\n' or '\0').
 *
 */
struct ast_input
{
    struct ast base;
    struct ast *list; // peut etre NULL ou list
    struct ast *end; // peut etre '\n' ou '\0'
};

/**
 * @brief Rule compound list
 *
 */
struct ast_compound_list
{
    struct ast base;
    size_t nb_cmds;
    struct ast **cmds; // list of cmd node not NULL
};

/**
 * @brief Rule do_group
 *
 */
struct ast_do_group
{
    struct ast base;
    struct ast *comp_list;
};

/**
 * @brief rule while
 *
 */
struct ast_while
{
    struct ast base;
    struct ast *cond;
    struct ast *do_group;
};

/**
 * @brief rule until
 *
 */
struct ast_until
{
    struct ast base;
    struct ast *cond;
    struct ast *do_group;
};

/**
 * @brief rule and_or
 *
 */
struct ast_and_or
{
    struct ast base;
    size_t nb_pipes;
    struct ast **pipes;
    enum operator_type *op_type; // nb_pipes - 1 operators in array
};

/**
 * @brief rule pipe
 *
 */
struct ast_pipe
{
    struct ast base;
    size_t nb_cmds;
    struct ast **cmds;
};
