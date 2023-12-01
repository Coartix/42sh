#pragma once

int is_keyword(const char *input, size_t pos);

enum token_type get_keyword(struct lexer *lexer);
