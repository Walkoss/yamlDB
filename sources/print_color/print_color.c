#include <stdlib.h>
#include "print_color.h"
#include "../database/database.h"

void print_color(char *color, char *message) {
    printf("%s", color);
    printf("%s", message);
    printf("%s", COLOR_RESET);
}

void print_error_color(char *message) {
    sprintf(error, "%s", COLOR_RED);
    sprintf(error, "%s", message);
    sprintf(error, "%s", COLOR_RESET);
}

char *retrieveErrorMessage() {
    char *buffer = 0;
    long length;

    FILE *f = stderr;

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    return buffer;
}