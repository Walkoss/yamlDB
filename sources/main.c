#include "database/database.h"

int main() {
    Database *database;

    /* Tables de test */
    Table *table1 = xmalloc(sizeof(Table), __func__);
    Field *field1 = xmalloc(sizeof(Field), __func__);
    Field *field2 = xmalloc(sizeof(Field), __func__);
    Data *data1 = xmalloc(sizeof(Data), __func__);
    Data *data2 = xmalloc(sizeof(Data), __func__);
    Data *data3 = xmalloc(sizeof(Data), __func__);
    Data *data4 = xmalloc(sizeof(Data), __func__);
    Condition *condition1 = xmalloc(sizeof(Condition), __func__);

    condition1->key = "field1";
    condition1->value = "1998";

    table1->name = "table1";
    table1->pk = 0;
    table1->fieldHead = field1;
    table1->next = NULL;

    field1->name = "field1";
    field1->type = INT;
    field1->next = field2;

    field2->name = "field2";
    field2->type = VARCHAR;
    field2->next = NULL;

    data1->value = "1994";
    data1->field = field1;
    data1->next = data2;

    data2->value = "test";
    data2->field = field2;
    data2->next = NULL;

    data3->value = "2000";
    data3->field = field1;
    data3->next = data4;

    data4->value = "blabla";
    data4->field = field2;
    data4->next = NULL;

    Table *table2 = xmalloc(sizeof(Table), __func__);
    Field *field3 = xmalloc(sizeof(Field), __func__);
    Data *data5 = xmalloc(sizeof(Data), __func__);

    table2->name = "table2";
    table2->pk = 0;
    table2->fieldHead = field3;
    table2->next = NULL;

    field3->name = "field3";
    field3->type = INT;
    field3->next = NULL;

    data5->value = "2017";
    data5->field = field3;
    data5->next = NULL;
    /* Fin des tables de test */

    database = initDatabase("database");
    //createDatabase(database);
    useDatabase(database);
    //createTable(database, table1);
    //createTable(database, table2);
    //addData(database, table1, data1);
    removeData(database, table1, condition1);
    //updateData(database, table1, data2, condition1);
    //debugDatabase(database);
    //dropTable(database, table1);
    //dropTable(database, table2);
    //dropDatabase(database);

    return 0;
}