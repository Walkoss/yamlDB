#include "database/database.h"
#include "interface/interface.h"

int main() {
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

    Database *database;
    database = initDatabase("database");
/*    createDatabase(database);
    useDatabase(database);
    createTable(database, table1);
    createTable(database, table2);
    debugDatabase(database);
    dropTable(database, table1);
    dropTable(database, table2);
    dropDatabase(database);*/
    /*Database *database;
    database = initDatabase("database");
    createDatabase(database);*/
    //createTable(database, table1);

    userInterface();

    return 0;
}