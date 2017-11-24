/*
**  Filename: file.c
**
**  Made by: Walkoss on 19/11/2017.
**
**  Description: 
*/

#include "file.h"

File *fileInit() {
    File *file;

    file = xmalloc(sizeof(File), __func__);
    if (file != NULL) {
        file->size = 0;
        file->cursor = 0;
        file->buf = NULL;
    }
    return file;
}

void fileFree(File *file) {
    if (file != NULL) {
        if (file->buf != NULL) {
            free(file->buf);
        }
        free(file);
    }
}

void fileRead(File *file) {
    int character;

    file->buf = xmalloc(MAX_READ_SIZE, __func__);
    for (int i = 0; i < MAX_READ_SIZE; i++) {
        character = getchar();
        if (character == EOF) {
            return;
        }
        file->buf[file->size] = (char) character;
        file->size++;
        if (i == (MAX_READ_SIZE - 1)) {
            file->buf = realloc(file->buf, file->size + MAX_READ_SIZE);
            i = 0;
        }
    }
}

int fileNextChar(File *file) {
    if (file->buf != NULL) {
        fileRead(file);
    }
    if (file->cursor < file->size) {
        return file->buf[file->cursor++];
    }
    return EOF;
}