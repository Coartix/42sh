#pragma once

/**
 * @brief This enum contain all the different token type that the lexer could
 * recognize.
 *
 */
enum token_type
{
    /// The order is VERY important dont touch it unless you know what youre
    /// doing
    TOKEN_IF, ///< if token
    TOKEN_THEN, ///< then token
    TOKEN_FI, ///< fi token
    TOKEN_ELSE, ///< else token
    TOKEN_ELIF, ///< elif token
    TOKEN_WHILE, ///< while token
    TOKEN_UNTIL, ///< until token
    TOKEN_DO, ///< token do
    TOKEN_DONE, ///< token done
    TOKEN_SMCL, ///< semicolon token
    TOKEN_WORD, ///< simple word
    TOKEN_AND, ///< token and
    TOKEN_OR, ///< token or
    TOKEN_PIPE, ///< token pipe
    TOKEN_NEWL, ///< newline token
    TOKEN_EOF, ///< end of input marker
    TOKEN_ERROR ///< tt's not a real token, it's returned in case of invalid
                ///< input
};

/**
 * @brief The structure of a token.
 *
 */
struct token
{
    enum token_type type; ///< The kind of token
    char *cmd; ///< has a value only if it is a simple command
};

/**
 * @brief Create a new token of type token_type. The cmd pointer is set to NULL.
 *
 * @param type
 * @return struct token*
 */
struct token *token_new(enum token_type type);

/**
 * @brief Free the cmd pointer and the token.
 *
 * @param token
 */
void token_free(struct token *token);
