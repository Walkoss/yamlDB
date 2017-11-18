#include "database/database.h"

int displayDatabase(Database *database) {
    if (database == NULL)
        return 1;

    Table *currentTable = database->tableHead;

    while (currentTable != NULL) {
        printf("%s :\n ", currentTable->name);

        Field *currentField = currentTable->fieldHead;
        while (currentField != NULL) {
            printf("\t%s: %d\n ", currentField->name, currentField->type);
            currentField = currentField->next;
        }

        printf("\n");

        currentTable = currentTable->next;
    }

    return 0;
}

int main() {
    Database *database = NULL;

    Table *table = xmalloc(sizeof(Table), __func__);
    Field *field1 = xmalloc(sizeof(Field), __func__);
    Field *field2 = xmalloc(sizeof(Field), __func__);

    table->name = "test";
    table->pk = 0;
    table->fieldHead = field1;
    table->next = NULL;

    field1->name = "field1";
    field1->type = VARCHAR;
    field1->next = field2;

    field2->name = "field2";
    field2->type = INT;
    field2->next = NULL;

    database = initDatabase("database");

    database = useDatabase(database);

    createTable(database, table);
    displayDatabase(database);
    return 0;
}