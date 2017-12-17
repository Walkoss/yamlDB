#include "print_color.h"

void print_color(char *color, char *message) {
    printf("%s", color);
    printf("%s", message);
    printf("%s", COLOR_RESET);
}

void print_error_color(char *message) {
    fprintf(stderr, "%s", COLOR_RED);
    fprintf(stderr, "%s", message);
    fprintf(stderr, "%s", COLOR_RESET);
}