#include "database/database.h"

int main() {
    Database *database;

    /* Tables de test */
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
    /* Fin des tables de test */

    database = initDatabase("database");
    createDatabase(database);
    useDatabase(database);
    createTable(database, table1);
    createTable(database, table2);
    displayDatabase(database);
    dropTable(database, table1);
    dropTable(database, table2);
    dropDatabase(database);

    return 0;
}