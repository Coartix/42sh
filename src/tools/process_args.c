#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "alloc.h"
#include "skip_space.h"

/**
 * @brief Split str array in multiple args
 *
 * @param str
 * @return char**
 */
/*
static char **split_args(const char *str)
{
    size_t nb_words = 0;
    size_t pos = 0;
    size_t end_word = 0;

    int quote = 0;
    int dquote = 0;

    char **res = xmalloc(1 * sizeof(*res));
    res[0] = NULL;

    while (str[pos] != '\0')
    {
        pos = skip_space(str, pos);
        end_word = pos;

        while (str[end_word] != '\0'
               && (!isspace(str[end_word]) || quote == 1 || dquote == 1))
        {
            if (str[end_word] == '"' && quote == 0)
                dquote = !dquote;
            if (str[end_word] == '\'' && dquote == 0)
                quote = !quote;
            if (quote == 0 && dquote == 0 && str[end_word] == '\\'
                && str[end_word + 1] != '\0')
                end_word++;
            end_word++;
        }

        if (end_word > pos)
        {
            nb_words++;
            res = xrealloc(res, (nb_words + 1) * sizeof(*res));
            res[nb_words] = NULL;
            res[nb_words - 1] = xmalloc((end_word - pos + 1) * sizeof(char));

            memcpy(res[nb_words - 1], str + pos, end_word - pos);
            res[nb_words - 1][end_word - pos] = '\0';
        }

        pos = end_word;
    }
    return res;
}
*/
/**
 * @brief Remove the first element of str
 *
 * @param str
 */
static void remove_first(char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        str[i] = str[i + 1];
}

/**
 * @brief process the single quotes
 *
 * @param str
 * @param pos
 * @return size_t
 */
static size_t process_quote(char **str, size_t pos)
{
    remove_first((*str) + pos);

    while ((*str)[pos] != '\'')
        pos++;

    remove_first((*str) + pos);

    *str = xrealloc(*str, strlen(*str) + 1);

    return pos;
}

/**
 * @brief process backslash outside double quotes
 *
 * @param str
 * @param pos
 * @return size_t
 */
static size_t process_backsl(char **str, size_t pos)
{
    if ((*str)[pos + 1] != '\0')
    {
        remove_first((*str) + pos);

        if ((*str)[pos] == '\n')
            remove_first((*str) + pos);

        *str = xrealloc(*str, strlen(*str) + 1);
    }

    if ((*str)[pos] != '\0')
        pos++;

    return pos;
}

/**
 * @brief process backslash in double quotes
 *
 * @param str
 * @param pos
 * @return size_t
 */
static size_t process_backsl2(char **str, size_t pos)
{
    if ((*str)[pos + 1] == '"' || (*str)[pos + 1] == '$'
        || (*str)[pos + 1] == '`' || (*str)[pos + 1] == '\\'
        || (*str)[pos + 1] == '\n')
    {
        remove_first((*str) + pos);
        if ((*str)[pos] == '\n')
        {
            remove_first((*str) + pos);
            pos--;
        }

        *str = xrealloc(*str, strlen(*str) + 1);
    }

    if ((*str)[pos] != '\0')
        pos++;

    return pos;
}

/**
 * @brief process the double quotes
 *
 * @param str
 * @param pos
 * @return size_t
 */
static size_t process_dquote(char **str, size_t pos)
{
    remove_first((*str) + pos);

    while ((*str)[pos] != '\"')
    {
        if ((*str)[pos] == '$')
        {
            pos++; // to do
        }
        else if ((*str)[pos] == '\\')
            pos = process_backsl2(str, pos);
        else
            pos++;
    }

    remove_first((*str) + pos);

    *str = xrealloc(*str, strlen(*str) + 1);

    return pos;
}

/**
 * @brief Split the incoming str in tab of strings, the separator is space(s)
 *
 * @param str
 * @return char**
 */
char **process_args(char **str)
{
    // char **res = split_args(str);

    for (size_t i = 0; str[i] != NULL; i++)
    {
        size_t j = 0;
        while (str[i][j] != '\0')
        {
            if (str[i][j] == '\'')
                j = process_quote(str + i, j);
            else if (str[i][j] == '"')
                j = process_dquote(str + i, j);
            else if (str[i][j] == '\\')
                j = process_backsl(str + i, j);
            // else if (str[i][j] == '$')
            //    j = process_var(str + i, j);
            else
                j++;
        }
    }

    return str;
}
