#ifndef YAMLDB_PRINT_COLOR_H
#define YAMLDB_PRINT_COLOR_H

#include <stdio.h>

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

void print_color(char *, char *);
void print_error_color(char *);

#endif //YAMLDB_PRINT_COLOR_H
