//
// Created by Iliasse Wahbi on 14/11/2017.
//

#include "interface.h"

char *getUserInput(int size)
{
    char *text;

    text = xmalloc(sizeof(char) * size, __FUNCTION__);
    if (!text)
        return NULL;
    fgets(text, 20, stdin);

    return text;
}

int userInterface()
{
    while (1) {
        char *text = getUserInput(20);
        printf("%s", text);
        free(text);
    }
}