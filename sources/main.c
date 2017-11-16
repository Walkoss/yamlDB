#include "database/database.h"

int main() {
    Database *database;

    createDatabase("Database");
    database = initDatabase("myDatabase");

    if (database != NULL) {
        printf("Database = %s\n", database->name);
    }

    freeDatabase(database);
    //removeDatabase("Database");

    return 0;
}