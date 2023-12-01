#pragma once

/**
 * @brief enum containing all different type of node that we can have
 *
 */
enum ast_type
{
    AST_INPUT, // input: list '\n' | list EOF | '\n' | EOF

    AST_LIST, // list: command (';' command)* [;]

    AST_SIMPLE_CMD,

    AST_CMD, // command: simple_command | shell_command

    AST_SH_CMD, // shell_command: rule_if | rule_while | rule_until

    AST_IF, // rule_if:  If compound_list Then compound_list [else_clause] Fi

    AST_ELSE, // else_clause:  Else compound_list | Elif compound_list Then
              // compound_list [else_clause]

    AST_COMPOUND_LIST, // compound_list: ('\n')* and_or ((';'|'\n') ('\n')*
                       // and_or)* [(';'|'\n') ('\n')*]

    AST_DOGROUP, // do_group: Do compound_list Done

    AST_WHILE, // rule_while: While compound_list do_group

    AST_UNTIL, // rule_until: Until compound_list do_group

    AST_AND_OR, // rule_and_or:  pipeline (('&&'|'||') ('\n')* pipeline)*

    AST_PIPE, // pipeline: command ('|' ('\n')* command)*

    AST_NEWL,

    AST_EOF,
};

enum operator_type
{
    AND,
    OR
};

/**
 * @brief The "reduce" version of each node
 * It's the common base between each node
 *
 */
struct ast
{
    enum ast_type type;
    void (*free)(struct ast *);
    void (*print)(struct ast *, char *pre_print);
    int (*evaluate)(struct ast *);
};
