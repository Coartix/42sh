#include "skip_space.h"

#include <ctype.h>

/**
 ** \brief Skip all space characters
 */
size_t skip_space(const char *input, size_t i)
{
    while (input[i] != '\0' && input[i] != '\n' && isspace(input[i]))
        i++;
    return i;
}
