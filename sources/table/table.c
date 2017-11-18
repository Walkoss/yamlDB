/*
**  Filename: table.c
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Functions for manage tables
*/

#include "../database/database.h"

/**
 * Initialize the tables in Database structure
 * @param database
 */
int initTables(Database *database) {
    struct dirent *file;
    DIR *dir;
    Table *table;

    dir = opendir(getDatabasePath(database->name));

    if (!dir) {
        fprintf(stderr, "An error has occured when opening database '%s': "
                "%s\n", database->name, strerror(errno));
        return 1;
    }

    while ((file = readdir(dir))) {
        if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0) {
            table = xmalloc(sizeof(Table), __func__);

            if (!table)
                return 1;

            file->d_name[strlen(file->d_name) - 4]  = '\0'; // To remove the ".yml"
            table->name = file->d_name;
            //TODO : vérifier l'id le plus grand dans les données et rajouter +1
            table->pk = 0;
            table->fieldHead = NULL;
            table->next = database->tableHead;

            initFields(database, table);
            database->tableHead = table;
        }
    }

    closedir(dir);

    return 0;
}

/**
 * Free the tables in a Database structure
 * @param database
 * @return
 */
// TODO: Vérifier les fonctions free
int freeTables(Database *database) {
    Table *currentTable;
    Table *tableToFree;

    if (!database)
        return 1;

    currentTable = database->tableHead;

    while (currentTable != NULL) {
        tableToFree = currentTable->next;
        database->tableHead = currentTable;
        currentTable = currentTable->next;
        freeFields(tableToFree);
        free(tableToFree);
    }

    database->tableHead = NULL;
    free(currentTable);

    return 0;
}

/**
 * Find a table in a Database structure
 * @param database
 * @param tableName
 * @return database if success, NULL for error
 */
Table *findTable(Database *database, char *tableName) {
    Table *currentTable;

    if (database == NULL)
        return NULL;

    currentTable =  database->tableHead;

    while (currentTable != NULL)
    {
        if (strcmp(currentTable->name, tableName) == 0)
            return currentTable;
        currentTable = currentTable->next;
    }

    return NULL;
}

/**
 * Free a table in a Database structure
 * @param database
 * @param table
 * @return  0 if success, 1 for error
 */
// TODO: Vérifier les fonctions free
int freeTable(Database *database, Table *table) {
    Table *tableToFree;
    Table *currentTable;

    if (!database)
        return 1;

    currentTable = database->tableHead;

    while (currentTable != NULL) {
        if (currentTable->next == table) {
            tableToFree = currentTable->next;
            currentTable->next = currentTable->next->next;
            free(tableToFree);
        }
        else if (currentTable == table) {
            tableToFree = currentTable;
            database->tableHead = currentTable->next;
            free(tableToFree);
        }
        currentTable = currentTable->next;
    }

    return 0;
}

/**
 * Create a table file
 * @param database
 * @param table
 * @return 0 if success, 1 for error
 */
int createTable(Database *database, Table *table) {
    char *path;
    FILE *file;

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    if (fopen(path,"w") == NULL) { // Si la table n'existe pas
        file = fopen(path, "w+");
        if (!file) {
            fprintf(stderr, "An error has occured when creating table '%s': "
                    "%s\n", table->name, strerror(errno));
            free(path);
            return 1;
        }
        addFieldsInFile(database, table, file);

        fclose(file);
    } else {
        fprintf(stderr, "The table already exist '%s': "
                "%s\n", table->name, strerror(errno));
    }
    free(path);

    return 0;
}

/**
 * Remove file 'table->name'
 * Free the Table and fields
 * @param database
 * @param table
 * @return
 */
int dropTable(Database *database, Table *table) {
    char *path;

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    if (remove(path) == -1) {
        fprintf(stderr, "An error has occured when removing table '%s': "
                "%s\n", table->name, strerror(errno));

        free(path);
        return 1;
    }

    freeTable(database, findTable(database, table->name));

    free(path);
    return 0;
}

/**
 * Based on RESOURCES_DIR and databaseName, this function returns an absolute
 * path to the file
 * @param databaseName
 * @param tableName
 * @return
 */
char *getTablePath(const char *databaseName, const char *tableName) {
    char *path;

    path = xmalloc(sizeof(char) * (strlen(RESOURCES_DIR) + strlen
            (databaseName) + strlen(tableName) + 6), __func__);
    if (!path)
        return NULL;

    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".yml");

    return path;
}