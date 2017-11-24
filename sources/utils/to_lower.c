/*
**  Filename: to_lower.c
**
**  Made by: Walkoss on 23/11/2017.
**
**  Description: 
*/

#include "to_lower.h"

void strToLower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) tolower(str[i]);
    }
}