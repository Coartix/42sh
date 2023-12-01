#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "alloc.h"
#include "lex_keyword.h"
#include "skip_space.h"

static int first = 1;

/**
 * \brief Creates a new lexer given an input string.
 */
struct lexer *lexer_new(const char *input)
{
    struct lexer *lex = xmalloc(sizeof(struct lexer));
    lex->input = input;
    lex->pos = 0;
    lex->current_tok = lexer_peek(lex);
    return lex;
}

/**
 ** \brief Free the given lexer, but not its input.
 */
void lexer_free(struct lexer *lexer)
{
    token_free(lexer->current_tok);
    free_alloc(lexer);
}

/**
 ** \brief Allocate a string for the command
 */
static char *get_command(struct lexer *lexer)
{
    char *cmd = NULL;
    int quote = 0;
    int dquote = 0;
    size_t len = 0;

    while (lexer->input[lexer->pos + len] != '\0'
           && ((lexer->input[lexer->pos + len] != '\n'
                && lexer->input[lexer->pos + len] != ';'
                && lexer->input[lexer->pos + len] != ' ')
               || quote != 0 || dquote != 0))
    {
        if (lexer->input[lexer->pos + len] == '\"' && quote == 0)
            dquote = !dquote;
        if (lexer->input[lexer->pos + len] == '\'' && dquote == 0)
            quote = !quote;
        if (quote == 0 && lexer->input[lexer->pos + len] == '\\'
            && lexer->input[lexer->pos + len + 1] != '\0')
            len++;
        len++;
    }

    if (quote == 0 && dquote == 0)
    {
        cmd = xmalloc((len + 1) * sizeof(*cmd));

        for (size_t i = 0; i < len; i++)
            cmd[i] = lexer->input[lexer->pos + i];

        cmd[len] = '\0';
    }
    lexer->pos += len;

    lexer->pos = skip_space(lexer->input, lexer->pos);

    return cmd;
}

/**
 * \brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token *lexer_peek(struct lexer *lexer)
{
    if (lexer->pos != 0)
        return lexer->current_tok;

    lexer->pos = skip_space(lexer->input, lexer->pos);

    if (first && is_keyword(lexer->input, lexer->pos))
    {
        lexer->current_tok = token_new(get_keyword(lexer));
    }
    /*else if (isalpha(lexer->input[lexer->pos])
             || lexer->input[lexer->pos] == '\''
             || lexer->input[lexer->pos] == '\"'
             || lexer->input[lexer->pos] == '\\'
             || lexer->input[lexer->pos] == '.'
             || lexer->input[lexer->pos] == '/'
             || lexer->input[lexer->pos] == '-')*/
    else if (lexer->input[lexer->pos] != '\n'
             && lexer->input[lexer->pos] != '\0'
             && lexer->input[lexer->pos] != ';')
    {
        char *cmd = get_command(lexer);
        first = 0;

        if (cmd != NULL)
        {
            if (strncmp(cmd, "&&", 3) == 0)
                lexer->current_tok = token_new(TOKEN_AND);
            else if (strncmp(cmd, "||", 3) == 0)
                lexer->current_tok = token_new(TOKEN_OR);
            else if (strncmp(cmd, "|", 2) == 0)
                lexer->current_tok = token_new(TOKEN_PIPE);
            else
                lexer->current_tok = token_new(TOKEN_WORD);
        }
        else
            lexer->current_tok = token_new(TOKEN_ERROR);
        lexer->current_tok->cmd = cmd;
    }
    else
    {
        char c = lexer->input[lexer->pos];
        enum token_type token = TOKEN_ERROR;

        if (c == '\0')
            token = TOKEN_EOF;
        else if (c == '\n')
            token = TOKEN_NEWL;
        else if (c == ';')
            token = TOKEN_SMCL;

        lexer->current_tok = token_new(token);
        lexer->pos++;
        lexer->current_tok->cmd = NULL;

        first = 1;
    }

    return lexer->current_tok;
}

/**
 * \brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
void lexer_pop(struct lexer *lexer)
{
    if (lexer->current_tok->type != TOKEN_EOF)
    {
        lexer->input = lexer->input + lexer->pos;
        lexer->pos = 0;
        token_free(lexer->current_tok);
        lexer->current_tok = NULL;
        lexer_peek(lexer);
    }
}
