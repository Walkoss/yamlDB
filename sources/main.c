#include "database/database.h"

int main() {
    Database *database;

    Table *table1 = xmalloc(sizeof(Table), __func__);
    Field *field1 = xmalloc(sizeof(Field), __func__);
    Field *field2 = xmalloc(sizeof(Field), __func__);
    Field *field3 = xmalloc(sizeof(Field), __func__);
    Field *field4 = xmalloc(sizeof(Field), __func__);
    Data *data1 = xmalloc(sizeof(Data), __func__);
    Data *data2 = xmalloc(sizeof(Data), __func__);
    Data *data3 = xmalloc(sizeof(Data), __func__);
    Data *data4 = xmalloc(sizeof(Data), __func__);
    Data *data5 = xmalloc(sizeof(Data), __func__);
    Data *data6 = xmalloc(sizeof(Data), __func__);
    Condition *condition1 = xmalloc(sizeof(Condition), __func__);

    table1->name = "user";
    table1->fieldHead = field1;
    table1->next = NULL;

    field1->name = "name";
    field1->type = VARCHAR;
    field1->next = field2;

    field2->name = "surname";
    field2->type = VARCHAR;
    field2->next = field3;

    field3->name = "age";
    field3->type = INT;
    field3->next = NULL;

    data1->value = "Alexis";
    data1->field = field1;
    data1->next = data2;

    data2->value = "Petrillo";
    data2->field = field2;
    data2->next = data3;

    data3->value = "21";
    data3->field = field3;
    data3->next = NULL;

    data4->value = "Walid";
    data4->field = field1;
    data4->next = data5;

    data5->value = "El bouchiki";
    data5->field = field2;
    data5->next = data6;

    data6->value = "20";
    data6->field = field3;
    data6->next = NULL;

    field4->name = NULL;
    field4->next = NULL;

    condition1->key = "age";
    condition1->value = "21";

    database = initDatabase("database");
    //createDatabase(database);
    useDatabase(database);
    //createTable(database, table1);
    //addData(database, table1, data1);
    //addData(database, table1, data4);
    openFilesForUpdating(database, table1, data1, NULL);
    //openFilesForRemoving(database, table1, NULL);
    //selectData(database, table1, field1, NULL); // Mettre un field->name à NULL si on select *, Mettre condition = NULL si on veut pas de condition
    //dropTable(database, table1);
    //dropDatabase(database);

    return 0;
}