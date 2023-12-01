#pragma once

#include "leaf_ast.h"
#include "node_ast.h"

/**
 * @brief Create a new line node.
 *
 * @return struct ast_newl*
 */
struct ast_newl *new_newl_ast(void);

/**
 * @brief Create a simple command node.
 *
 * @param cmd_str
 * @return struct ast_cmd*
 */
struct ast_simple_cmd *new_simple_cmd_ast(char **cmd_str);

/**
 * @brief Create a command node.
 *
 * @param cmd
 * @return struct ast_cmd*
 */
struct ast_cmd *new_cmd_ast(struct ast *cmd);

/**
 * @brief Create a shell command node.
 *
 * @param sh_rule
 * @return struct ast_shell_cmd*
 */
struct ast_sh_cmd *new_sh_cmd_ast(struct ast *sh_rule);

/**
 * @brief Create a EOF node.
 *
 * @return struct ast_eof*
 */
struct ast_eof *new_eof_ast(void);

/**
 * @brief Create a list node.
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_list*
 */
struct ast_list *new_list_ast(struct ast **cmds, size_t nb_cmds);

/**
 * @brief Create an input node.
 * List is the command to run, could be NULL.
 * End is the delimiter node, could ast_oef or ast_newl.
 *
 * @param list
 * @param end
 * @return struct ast_input*
 */
struct ast_input *new_input_ast(struct ast *list, struct ast *end);

/**
 * @brief Create an ast_if type node with cond, cmd and else_clause
 * for children. Else_clause can be NULL.
 *
 * @param cond
 * @param cmd
 * @param else_clause
 * @return struct ast_if*
 */
struct ast_if *new_if_ast(struct ast *cond, struct ast *cmd,
                          struct ast *else_clause);

/**
 * @brief Create an ast_else type node with cond, cmd and else_clause
 * for children. Else_clause can be NULL.
 *
 * @param cond
 * @param cmd
 * @param else_clause
 * @return struct ast_else*
 */
struct ast_else *new_else_ast(struct ast *cond, struct ast *cmd,
                              struct ast *else_clause);

/**
 * @brief Create new compound list with list of cmds to execute
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_compound_list*
 */
struct ast_compound_list *new_compound_list_ast(struct ast **cmds,
                                                size_t nb_cmds);

/**
 * @brief Create new do_group node pointing to a compound list
 *
 * @param comp_list
 * @return struct ast_do_group*
 */
struct ast_do_group *new_do_group_ast(struct ast *comp_list);

/**
 * @brief Create new while node pointing to a compound list and a do group
 *
 * @param comp_list
 * @param do_group
 * @return struct ast_while*
 */
struct ast_while *new_while_ast(struct ast *cond, struct ast *do_group);

/**
 * @brief Create new until node pointing to a compound list and a do group
 *
 * @param comp_list
 * @param do_group
 * @return struct ast_until*
 */
struct ast_until *new_until_ast(struct ast *cond, struct ast *do_group);

/**
 * @brief create node ast and_or
 *
 * @param pipes
 * @param nb_pipes
 * @param op_type
 * @return struct ast_and_or*
 */
struct ast_and_or *new_and_or_ast(struct ast **pipes, size_t nb_pipes,
                                  enum operator_type *op_type);

/**
 * @brief Create ast_pipe
 *
 * @param cmds
 * @param nb_cmds
 * @return struct ast_pipe*
 */
struct ast_pipe *new_pipe_ast(struct ast **cmds, size_t nb_cmds);
