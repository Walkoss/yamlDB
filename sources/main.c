#include "database/database.h"

int main() {
    Database *database;

    database = initDatabase("test");

    if (database != NULL) {
        printf("db = %s", database->name);
    }

    freeDatabase(database);

    return 0;
}