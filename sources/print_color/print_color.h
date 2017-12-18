#ifndef YAMLDB_PRINT_COLOR_H
#define YAMLDB_PRINT_COLOR_H

#include <stdio.h>

#define COLOR_RED ""
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET ""

void print_color(char *, char *);
void print_error_color(char *);
char *retrieveErrorMessage();

#endif //YAMLDB_PRINT_COLOR_H
