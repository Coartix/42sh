#pragma once

#include "lexer.h"

/**
 * @brief input rule
 *
 * input:  list '\n'  |  list EOF | '\n' | EOF
 *
 * @param ast
 * @param lexer
 * @return enum parser_status
 */
struct ast *parse_input(struct lexer **lexer);

/**
 * @brief
 *
 * list : command (';' command)* [;]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_list(struct lexer **lexer);

/**
 * @brief command: simple_command | shell_command
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_cmd(struct lexer **lexer);

/**
 * @brief shell_command: rule_if
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_sh_cmd(struct lexer **lexer);

/**
 * @brief rule_if: If compound_list Then compound_list [else_clause] Fi
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_rule_if(struct lexer **lexer);

/**
 * @brief rule else_clause
 *
 * else_clause: Else compound_list | Elif compound_list Then compound_list
 * [else_clause]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_else_clause(struct lexer **lexer);

/**
 * @brief coumpound_list rule
 *
 * compound_list: compound_list: ('\n')* and_or ((';'|'\n') ('\n')* and_or)*
 * [(';'|'\n') ('\n')*]
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_compound_list(struct lexer **lexer);

/**
 * @brief simple_command rule
 *
 * simple_command: WORD+
 *
 * @param ast
 * @param lexer
 * @return enum parser_status
 */
struct ast *parse_simple_cmd(struct lexer **lexer);

/**
 * @brief do_group rule
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_do_group(struct lexer **lexer);

/**
 * @brief while rule
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_while(struct lexer **lexer);

/**
 * @brief until rule
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_until(struct lexer **lexer);

/**
 * @brief and_or rule
 *
 * and_or: pipeline (('&&'|'||') ('\n')* pipeline)*
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_and_or(struct lexer **lexer);

/**
 * @brief pipe rule
 *
 * pipeline: command ('|' ('\n')* command)*
 *
 * @param lexer
 * @return struct ast*
 */
struct ast *parse_pipe(struct lexer **lexer);
