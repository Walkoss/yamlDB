#include "database/database.h"

int main() {
    Database *database;

    database = initDatabase("database");
    createDatabase(database);

    if (database != NULL) {
        printf("Database = %s\n", database->name);
        printf("isUsed = %d\n\n", database->isUsed);
    }

    useDatabase(database);

    if (database != NULL) {
        printf("Database = %s\n", database->name);
        printf("isUsed = %d\n\n", database->isUsed);
    }

    //dropDatabase(database);
    freeDatabase(database); // est appelé quand l'utilisateur fait un autre USE;

    if (database != NULL) {
        printf("Database = %s\n", database->name);
        printf("isUsed = %d\n\n", database->isUsed);
    }

    return 0;
}