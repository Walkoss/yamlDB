#include "database/database.h"

/**
 * display the database, tables and fields to debug
 * @param database
 * @return 0 if success, 1 for error
 */
int displayDatabase(Database *database) {
    Table *currentTable;

    if (database == NULL)
        return 1;

    currentTable = database->tableHead;

    if (strcmp(database->name, "") != 0)
        printf("%d %s:\n", database->isUsed, database->name);

    while (currentTable != NULL) {
        printf("\t%s:\n ", currentTable->name);

        Field *currentField = currentTable->fieldHead;
        while (currentField != NULL) {
            printf("\t\t%s:%d\n", currentField->name, currentField->type);
            currentField = currentField->next;
        }

        printf("\n");
        currentTable = currentTable->next;
    }

    return 0;
}

int main() {
    Database *database;

    Table *table1 = xmalloc(sizeof(Table), __func__);
    Field *field1 = xmalloc(sizeof(Field), __func__);
    Field *field2 = xmalloc(sizeof(Field), __func__);

    table1->name = "test";
    table1->pk = 0;
    table1->fieldHead = field1;
    table1->next = NULL;

    field1->name = "field1";
    field1->type = VARCHAR;
    field1->next = field2;

    field2->name = "field2";
    field2->type = INT;
    field2->next = NULL;

    Table *table2 = xmalloc(sizeof(Table), __func__);
    Field *field3 = xmalloc(sizeof(Field), __func__);

    table2->name = "test2";
    table2->pk = 0;
    table2->fieldHead = field3;
    table2->next = NULL;

    field3->name = "field3";
    field3->type = VARCHAR;
    field3->next = NULL;

    database = initDatabase("database");
    //createDatabase(database);
    useDatabase(database);
    //createTable(database, table1);
    //dropTable(database, table1);
    //displayDatabase(database);
    //dropDatabase(database);
    //displayDatabase(database);

    return 0;
}