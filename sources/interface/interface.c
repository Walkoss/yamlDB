/*
**  Filename: interface.c
**
**  Made by: Alexis PETRILLO on 17/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

/**
 * Read user input
 * @param size
 * @return char* if success, NULL for error
 */
char *getUserInput(int size)
{
    char *text;
    int c;
    int i = 0;

    if ((text = xmalloc(sizeof(char) * size + 1, __FUNCTION__)) == NULL)
        return NULL;

    printf("> ");
    while (!i) {
        while ((c = getchar()) != '\n' && c != EOF) {
            if (i < size) {
                text[i] = (char) c;
                i++;
            }
        }
    }
    text[i] = '\0';
    printf("\n");

    return text;
}