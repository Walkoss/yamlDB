/*
**  Filename: gtk.h
**
**  Made by: Iliasse WAHBI on 13/12/2017.
**
**  Description:
*/

#include <gtk/gtk.h>
#include "../interface/interface.h"

typedef struct SGtkDatabase {
    Database *database;
    GtkListStore *pTableList;
    GtkWidget *pObject[11];
} GtkDatabase;

typedef struct SCreateTableDialog {
    Table *table;
    GtkWidget *pObject[5];
    int fieldCount;
    GtkListStore *fieldsList;
} CreateTableDialog;

void onDestroy(GtkWidget *pWidget, gpointer pData);
void run(int argc, char **argv, Database* database);
void initDatabaseCallback(GtkWidget *pButton, gpointer data);
void createDatabaseCallback(GtkWidget *pButton, gpointer data);
void removeDatabaseCallback(GtkWidget *pButton, gpointer data);
void initTableComboBox(GtkDatabase *gtkDatabase);
void removeTableCallback(GtkWidget *pButton, gpointer data);
void getTableSelectedName(GtkDatabase *gtkDatabase, gchar **text);
void createTableCallback(GtkWidget *pButton, gpointer data);
void instanceMessageDialog(GtkWidget *parent, const char *message);
void addFieldNewTableCallback(GtkWidget *pButton, gpointer data);
void updateInformationLabel(GtkDatabase *gtkDatabase, char *message);
void initFieldsComboBox(CreateTableDialog *createTableDialog);
char *getEnumName(FieldType type);
Table *getTableFromDatabase(GtkDatabase *gtkDatabase, char *tableName);