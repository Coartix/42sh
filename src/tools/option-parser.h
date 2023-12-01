#pragma once

#include <stdio.h>
#include <unistd.h>

char *read_cmd(void);
char *read_file_chevron(void);
int check_options(char *options);
int check_if_there_is_file(int argc, char *argv[]);
char *process_params(int argc, char *argv[]);
int get_index(char *c, char *argv[], int argc);
char *file_to_buffer(FILE *fd, char *buffer);
size_t total_file_size(FILE *fd);
size_t my_strlen(const char *s);
char *str_skip_shebang(char *str);
