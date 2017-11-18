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
int initFields(Database *database, Table *table) {
    Field *field;
    FILE* file;
    char currentLine[BUFFER_SIZE];
    char *name;
    FieldType type;

    if (!database || !table)
        return 1;

    file = fopen(getTablePath(database->name, table->name), "r");

    if (file != NULL) {
        while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
            field = xmalloc(sizeof(Field), __func__);
            name = xmalloc(sizeof(char) * 128, __func__);

            if (!field || !name)
                return 1;

            fscanf(file, "%s %d", name, &type);

            field->name = name;
            field->type = type;
            field->next = table->fieldHead;
            table->fieldHead = field;
        }

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
int addFieldsInFile(Database *database, Table *table, FILE *file) {
    Field *field;

    if (!database || !table || !file)
        return 1;

    field = table->fieldHead;
    fprintf(file, "fields:");

    while (field != NULL) {
        fprintf(file, "\n\t%s: %d", field->name, field->type);
        field = field->next;
    }

    table->next = database->tableHead;
    initFields(database, table);
    database->tableHead = table;

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