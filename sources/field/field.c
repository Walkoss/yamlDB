/*
**  Filename: field.c
**
**  Made by: Walkoss on 15/11/2017.
**
**  Description: 
*/

#include "../database/database.h"

/**
 * Initialize the fields in Table structure
 * @param database
 * @param table
 * @return 0 if success, 1 for error
 */
int initFieldsInStruct(FILE *file, Table *table, Field *field) {
    FieldType type;
    char currentLine[BUFFER_SIZE];
    char *name;

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        field = xmalloc(sizeof(Field), __func__);
        name = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!field || !name)
            return 1;

        fscanf(file, "%s %d", name, &type);
        name[strlen(name) - 1]  = '\0'; // To remove the ":"

        if (strcmp(name, "data") == 0) {
            break;
        }
        else if (strcmp(name, "") != 0) {
            field->name = name;
            field->type = type;
            field->next = table->fieldHead;
            table->fieldHead = field;
        }
    }

    return 0;
}

/**
 * Catching error before initializing the fields
 * @param database
 * @param table
 * @return 0 if success, 1 for error
 */
int initFields(Database *database, Table *table) {
    FILE *file;
    Field *field;

    if (!database || !table)
        return 1;

    file = fopen(getTablePath(database->name, table->name), "r");
    if (file != NULL) {
        initFieldsInStruct(file, table, field);
        fclose(file);
    } else {
        fprintf(stderr, "An error has occured when init fields '%s': "
                "%s\n", table->name, strerror(errno));
        return 1;
    }

    return 0;
}

/**
 * Write fields in table file
 * @param database
 * @param table
 * @param file
 * @return 0 if success, 1 for error
 */
int addFieldsInFile(Database *database, Table *table) {
    Field *field;
    FILE *file;
    char *path;

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    file = fopen(path, "w+");
    if (!file) {
        fprintf(stderr, "An error has occured when creating table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        return 1;
    }

    field = table->fieldHead;
    fprintf(file, "fields:\n");

    while (field != NULL) {
        fprintf(file, "\t%s: %d\n", field->name, field->type);
        field = field->next;
    }

    fprintf(file, "data:\n");
    initFields(database, table);

    return 0;
}

/**
 * Free the fields in a table node
 * @param table
 * @return 0 if success, 1 for error
 */
int freeFields(Table *table) {
    Field *currentField;
    Field *fieldToFree;

    if (!table)
        return 1;

    currentField = table->fieldHead;

    while (currentField->next != NULL) {
        fieldToFree = currentField->next;
        table->fieldHead = currentField;
        currentField = currentField->next;
        free(fieldToFree);
    }

    table->fieldHead = NULL;
    free(currentField);

    return 0;
}