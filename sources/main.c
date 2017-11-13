#include <stdio.h>
#include "database/database.h"
#include "table/table.h"

int main() {
    addDatabase("database");
    Table *table;
    table = initialization();
    newField(table, "id", "int"); // For the tests
    newField(table, "name", "char*"); // For the tests
    newField(table, "surname", "char*"); // For the tests
    addTable("database", "table", table);
    return 0;
}