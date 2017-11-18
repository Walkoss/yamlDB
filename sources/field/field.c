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
    char *currentLine;
    char *name;
    int type;

    if (!database || !table)
        return 1;

    currentLine = malloc(sizeof(char) * 200); // A CHANGER
    file = fopen(getTablePath(database->name, table->name), "r");

    if (file != NULL) {
        while (fgets(currentLine, 100, file) != NULL) { // A CHANGER
            field = xmalloc(sizeof(Field), __func__);
            name = malloc(sizeof(char) * 100); // A CHANGER
            fscanf(file, "%s %d", name, &type);
            name[strlen(name) - 1]  = '\0'; // To remove the ":"

            field->name = name;
            field->type = type;
            field->next = table->fieldHead;
            table->fieldHead = field;
        }

        fclose(file);
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

    while (currentField != NULL) {
        fieldToFree = currentField->next;
        table->fieldHead = currentField;
        currentField = currentField->next;
        free(fieldToFree);
    }

    table->fieldHead = NULL;
    free(currentField);

    return 0;
}