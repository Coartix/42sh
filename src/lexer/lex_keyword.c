#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

#define NB_KEYWORDS 9

static char *keywords[NB_KEYWORDS] = { "if",    "then",  "fi", "else", "elif",
                                       "while", "until", "do", "done" };

int is_keyword(const char *input, size_t pos)
{
    int check = 1;

    size_t len_input = 0;
    while (isalpha(input[pos + len_input]))
        len_input++;

    for (size_t i = 0; i < NB_KEYWORDS && check; i++)
    {
        if (len_input == strlen(keywords[i]))
            check = strncmp(keywords[i], input + pos, len_input);
    }
    return !check;
}

enum token_type get_keyword(struct lexer *lexer)
{
    enum token_type check = TOKEN_ERROR;

    size_t len_input = 0;
    while (isalpha(lexer->input[lexer->pos + len_input]))
        len_input++;

    for (size_t i = 0; i < NB_KEYWORDS && check == TOKEN_ERROR; i++)
    {
        if (len_input == strlen(keywords[i])
            && !strncmp(keywords[i], lexer->input + lexer->pos, len_input))
        {
            check = i;
            lexer->pos += len_input;
        }
    }
    return check;
}
