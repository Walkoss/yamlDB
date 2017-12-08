/*
**  Filename: strsplit.c
**
**  Made by: Alexis PETRILLO on 08/12/2017.
**
**  Description: 
*/

#include "strsplit.h"

char** strSplit(char* str, const char delimiter)
{
    char** result;
    char delim[2];
    int i;
    delim[0] = delimiter;
    delim[1] = 0;

    i = 0;
    result = malloc(sizeof(char*) * 2); // 3 car on sait que les string a parser auront deux élément + un pour le 0
    if (result) {
        char* token = strtok(str, delim); // parse la string

        while (token) {
            result[i++] = strdup(token); // retourne un pointeur sur une nouvelle chaine
            token = strtok(0, delim); // parse la string jusqu'au \0
        }
    }

    return result;
}