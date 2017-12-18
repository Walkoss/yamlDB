/*
**  Filename: field.c
**
**  Made by: Walkoss on 15/11/2017.
**
**  Description: 
*/

#include "../database/database.h"
#include "../print_color/print_color.h"

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
        name[strlen(name) - 1] = '\0'; // To remove the ":"

        if (strcmp(name, "data") == 0) {
            break;
        } else if (strcmp(name, "") != 0) {
            field->name = xmalloc(sizeof(char*), __func__);
            strcpy(field->name, name);
            field->type = type;
            field->next = table->fieldHead;
            table->fieldHead = field;
        }
    }

    fieldListReverse(&table->fieldHead);

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
        fprintf(stderr, "%sAn error has occured when init fields '%s': "
                "%s\n%s", COLOR_RED, table->name, strerror(errno), COLOR_RESET);
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
        fclose(file);
        return 1;
    }

    field = table->fieldHead;
    fprintf(file, "fields:\n");

    while (field != NULL) {
        fprintf(file, "\t%s: %d\n", field->name, field->type);
        field = field->next;
    }

    fprintf(file, "data:\n");
    fclose(file);
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

    while (currentField != NULL) {
        fieldToFree = currentField;
        currentField = currentField->next;
        free(fieldToFree);
    }

    table->fieldHead = NULL;
    free(currentField);
    return 0;
}

Field *fieldListLast(Field *node) {
    while (node->next != NULL)
        node = node->next;
    return (node);
}

void fieldListAppend(Field **node, Field *newNode) {
    if (*node != NULL)
        fieldListLast(*node)->next = newNode;
    else
        *node = newNode;
}


void fieldListReverse(Field **begin) {
    Field *prev_node;
    Field *curr_node;
    Field *next_node;

    prev_node = NULL;
    curr_node = *begin;
    while (curr_node != NULL) {
        next_node = curr_node->next;
        curr_node->next = prev_node;
        prev_node = curr_node;
        curr_node = next_node;
    }
    *begin = prev_node;
}

Field *findField(Table *table, const char *fieldName) {
    Field *currentField;

    if (table == NULL)
        return NULL;

    currentField = table->fieldHead;

    while (currentField != NULL) {
        if (strcmp(currentField->name, fieldName) == 0)
            return currentField;
        currentField = currentField->next;
    }

    return NULL;
}