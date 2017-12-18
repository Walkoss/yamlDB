//
// Created by Iliasse Wahbi on 13/12/2017.
//

#include "gtk.h"
#include "../print_color/print_color.h"

/**
 * Gets user input and begins processes
 * @return FieldType
 */
FieldType getEnum(long type)
{
    switch (type)
    {
        case 1:
            return INT;
        case 2:
            return FLOAT;
        case 3:
            return CHAR;
        case 4:
            return VARCHAR;
        default:
            return INT;
    }
    return INT;
}

void onDestroy(GtkWidget *pWidget, gpointer pData)
{
    gtk_main_quit();
}

void updateInformationLabel(GtkDatabase *gtkDatabase, char *message)
{
    gtk_label_set_text(GTK_LABEL(gtkDatabase->pObject[7]), message);
}

void initDefaultValues(GtkDatabase *gtkDatabase)
{
    gtk_entry_set_text(GTK_ENTRY(gtkDatabase->pObject[0]), "");
    gtk_entry_set_text(GTK_ENTRY(gtkDatabase->pObject[2]), "");

    gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);
    gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[9], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[10], FALSE);
}

void onActiveItemChangerComboBox(GtkWidget *comboBox, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;
    char *tableName;
    Table *table;

    getTableSelectedName(gtkDatabase, &tableName);
    table = getTableFromDatabase(gtkDatabase, tableName);
    initTableDataStoreList(gtkDatabase, table, NULL);
}

void runSqlRequestCallback(GtkWidget *pButton, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;

    initTableDataStoreList(gtkDatabase, NULL, (char*)gtk_entry_get_text(GTK_ENTRY(gtkDatabase->pObject[11])));
}

void initTableDataStoreList(GtkDatabase *gtkDatabase, Table *table, char *request)
{
    Parser *parser;
    //Database *database;
    int column = 0;
    GType *columnsType;
    GtkCellRenderer *cellDataRenderer;
    GtkTreeViewColumn *columnDataList;
    char *sql;

    // Création de la requete sql si on affiche toutes les données d'une seule table
    if (request == NULL)
    {
        sql = xmalloc(sizeof(char) * (strlen(table->name) + strlen(gtkDatabase->database->name) + 30), __func__);
        strcat(sql, "SELECT * FROM ");
        strcat(sql, table->name);
        strcat(sql, ";");
        parser = parserInit(sql);
        char buf[BUFSIZ];
        setbuf(stderr, buf);
        if (parse(parser, &gtkDatabase->database))
        {
            instanceMessageDialog(gtkDatabase->pTableDataBox, buf);
            updateInformationLabel(gtkDatabase, isSuccessGtk(1));
        }
        else
            updateInformationLabel(gtkDatabase, isSuccessGtk(0));
        parserFree(parser);
        free(sql);
    }
    else
    {
        parser = parserInit(request);
        char buf[BUFSIZ];
        setbuf(stderr, buf);
        if (parse(parser, &gtkDatabase->database))
        {
            instanceMessageDialog(gtkDatabase->pTableDataBox, buf);
            updateInformationLabel(gtkDatabase, isSuccessGtk(1));
        }
        else
            updateInformationLabel(gtkDatabase, isSuccessGtk(0));
        parserFree(parser);
    }

    if (gtkDatabase->database && gtkDatabase->database->selectedData)
    {
        // On récupère la première liste de valeur pour obtenir la key/valuedans le bon ordre
        SelectedData *selectedDataTmp = gtkDatabase->database->selectedData;
        while (selectedDataTmp != NULL && selectedDataTmp->key) {
            if (selectedDataTmp->key)
                column++;
            selectedDataTmp = selectedDataTmp->next;
        }

        // Si une liste est déjà existante, on la detruit pour laisser place à la nouvelle
        if (gtkDatabase->tableDataExist) {
            gtk_widget_destroy(gtkDatabase->pTableDataListView);
            gtkDatabase->tableDataExist = FALSE;
        }

        columnsType = g_new0(GType, column);
        for (int i = 0; i < column; i++)
            columnsType[i] = G_TYPE_STRING;
        gtkDatabase->pTableDataList = gtk_list_store_newv(column, columnsType);

        // On ajoute les données dans la liste en fonction de la position des futurs colonnes
        selectedDataTmp = gtkDatabase->database->selectedData;
        while (selectedDataTmp != NULL) {
            if (selectedDataTmp->key) {
                GtkTreeIter iterColumn;
                gtk_list_store_append(gtkDatabase->pTableDataList, &iterColumn);
                for (int i = 0; (i < column && selectedDataTmp != NULL); i++) {
                    GValue value = G_VALUE_INIT;
                    g_value_init(&value, G_TYPE_STRING);
                    g_value_set_string(&value, selectedDataTmp->value);
                    gtk_list_store_set_value(gtkDatabase->pTableDataList, &iterColumn, i, &value);
                    if (i - 1 < column)
                        selectedDataTmp = selectedDataTmp->next;
                }
            }
            if (selectedDataTmp)
                selectedDataTmp = selectedDataTmp->next;
        }

        gtkDatabase->pTableDataListView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gtkDatabase->pTableDataList));

        // On ajoute les colonnes, on les nomme et on leur donne une position
        selectedDataTmp = gtkDatabase->database->selectedData;
        for (int i = 0; (selectedDataTmp != NULL && selectedDataTmp->key); i++) {
            i = (i == 3) ? 0 : i;
            if (selectedDataTmp->key) {
                cellDataRenderer = gtk_cell_renderer_text_new();
                columnDataList = gtk_tree_view_column_new_with_attributes(selectedDataTmp->key, cellDataRenderer, "text", i, NULL);
                gtk_tree_view_append_column(GTK_TREE_VIEW(gtkDatabase->pTableDataListView), columnDataList);
            }
            selectedDataTmp = selectedDataTmp->next;
        }

        /*Field *fieldTmp = table->fieldHead;
        for(int i = 0; fieldTmp != NULL; i++)
        {
            cellDataRenderer = gtk_cell_renderer_text_new();
            columnDataList = gtk_tree_view_column_new_with_attributes(fieldTmp->name, cellDataRenderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(gtkDatabase->pTableDataListView), columnDataList);
            fieldTmp = fieldTmp->next;
        }*/

        gtk_container_add(GTK_CONTAINER(gtkDatabase->pObject[13]), gtkDatabase->pTableDataListView);
        gtk_widget_show(gtkDatabase->pTableDataListView);
        gtkDatabase->tableDataExist = TRUE;
    }
    else
    {/*
        if (gtkDatabase->tableDataExist) {
            gtk_widget_destroy(gtkDatabase->pTableDataListView);
            gtkDatabase->tableDataExist = FALSE;
        }
        Field *fieldTmp = table->fieldHead;
        while (fieldTmp != NULL)
        {
            column++;
            fieldTmp = fieldTmp->next;
        }
        columnsType = g_new0(GType, column);
        for (int i = 0; i < column; i++)
            columnsType[i] = G_TYPE_STRING;
        gtkDatabase->pTableDataList = gtk_list_store_newv(column, columnsType);
        gtkDatabase->pTableDataListView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gtkDatabase->pTableDataList));
        fieldTmp = table->fieldHead;
        for (int i = 0; fieldTmp != NULL; i++)
        {
            cellDataRenderer = gtk_cell_renderer_text_new();
            columnDataList = gtk_tree_view_column_new_with_attributes(fieldTmp->name, cellDataRenderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(gtkDatabase->pTableDataListView), columnDataList);
            fieldTmp = fieldTmp->next;
        }
        gtk_container_add(GTK_CONTAINER(gtkDatabase->pObject[13]), gtkDatabase->pTableDataListView);
        gtk_widget_show(gtkDatabase->pTableDataListView);
        gtkDatabase->tableDataExist = TRUE;*/
    }
}

void initTableComboBox(GtkDatabase *gtkDatabase)
{
    Table *tableTmp;

    g_signal_handler_block(G_OBJECT(gtkDatabase->pObject[8]), gtkDatabase->comboxBoxSignalChanged);

    gtk_list_store_clear(gtkDatabase->pTableList);
    if (gtkDatabase->database != NULL && gtkDatabase->database->tableHead == NULL) {
        GtkTreeIter iterTable;
        gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
        gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, "Aucune table existante.", -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);                               // On focus le premier element de la liste des tables
        gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);                                              // Désactive la comboBox
        gtk_widget_set_sensitive(gtkDatabase->pObject[10], FALSE);
    }
    else {
        tableTmp = gtkDatabase->database->tableHead;
        gtk_widget_set_sensitive(gtkDatabase->pObject[8], TRUE);                                             // Désactive la comboBox
        gtk_widget_set_sensitive(gtkDatabase->pObject[10], TRUE);
        while (tableTmp != NULL) {
            GtkTreeIter iterTable;
            gchar *tableName = g_malloc(50);
            strcpy(tableName, tableTmp->name);
            gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
            gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, tableName, -1);
            if (tableTmp->next == NULL)
                initTableDataStoreList(gtkDatabase, tableTmp, NULL);
            tableTmp = tableTmp->next;
            g_free(tableName);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);                    // On focus le premier element de la liste des tables
    }

    g_signal_handler_unblock(G_OBJECT(gtkDatabase->pObject[8]), gtkDatabase->comboxBoxSignalChanged);
}

char *getEnumName(FieldType type)
{
    char *typeName = xmalloc(sizeof(char*), __func__);
    if (!typeName)
        return NULL;
    switch (type)
    {
        case INT:
            strcpy(typeName, "INT");
            return typeName;
        case FLOAT:
            strcpy(typeName, "FLOAT");
            return typeName;
        case CHAR:
            strcpy(typeName, "CHAR");
            return typeName;
        case VARCHAR:
            strcpy(typeName, "VARCHAR");
            return typeName;
        default:
            strcpy(typeName, "Erreur");
            return typeName;
    }
}

void initFieldsComboBox(CreateTableDialog *createTableDialog)
{
    Field *fieldTmp;

    gtk_list_store_clear(createTableDialog->fieldsList);
    if (createTableDialog->table->fieldHead == NULL) {
        GtkTreeIter iterField;
        gtk_list_store_prepend(createTableDialog->fieldsList, &iterField);
        gtk_list_store_set(createTableDialog->fieldsList, &iterField, 0, "Aucun champ n'a été ajouté.", 1, "", -1);
    }
    else {
        fieldTmp = createTableDialog->table->fieldHead;
        while (fieldTmp != NULL) {
            char *fieldType = getEnumName(fieldTmp->type);
            GtkTreeIter pIter;
            gtk_list_store_append(createTableDialog->fieldsList, &pIter);
            gtk_list_store_set(createTableDialog->fieldsList, &pIter, 0, fieldTmp->name , 1, fieldType, -1);

            fieldTmp = fieldTmp->next;
            free(fieldType);
        }
    }
}

void initDatabaseCallback(GtkWidget *pButton, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;
    const gchar *sText;
    int result = 1;

    sText = gtk_entry_get_text(GTK_ENTRY(gtkDatabase->pObject[0]));

    if (gtkDatabase->database) {
        freeDatabase(gtkDatabase->database);
        gtkDatabase->database = NULL;
    }
    if (strlen(sText) > 0)
    {
        Database *database = initDatabase(sText);
        gtkDatabase->database = database;
        if (!(result = useDatabase(gtkDatabase->database))) {
            gtk_label_set_text(GTK_LABEL(gtkDatabase->pObject[6]), gtkDatabase->database->name);              // on met le nom de la base de données dans le label du haut
            gtk_widget_set_sensitive(gtkDatabase->pObject[4], TRUE);                                          // on active le bouton supprimer (pour la base de données)
            gtk_widget_set_sensitive(gtkDatabase->pObject[8], TRUE);                                          // on active la ComboBox listant les tables de la bdd
            gtk_widget_set_sensitive(gtkDatabase->pObject[9], TRUE);
            initTableComboBox(gtkDatabase);
        }
    }
    updateInformationLabel(gtkDatabase, isSuccessGtk(result));
}

void instanceMessageDialog(GtkWidget *parent, const char *message)
{
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new (GTK_WINDOW(gtk_widget_get_toplevel(parent)), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "\n%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void addFieldNewTableCallback(GtkWidget *pButton, gpointer data)
{
    CreateTableDialog *createTableDialog = (CreateTableDialog*)data;
    Field *field = xmalloc(sizeof(Field), __func__);
    GSList *radioList;
    const gchar *fieldName;
    const gchar *tableName;
    long i = 1;

    if (!field) {
        instanceMessageDialog(pButton, "Erreur d'allocation mémoire.");
        return;
    }
    fieldName = gtk_entry_get_text(GTK_ENTRY(createTableDialog->pObject[0]));
    tableName = gtk_entry_get_text(GTK_ENTRY(createTableDialog->pObject[1]));
    if (strlen(fieldName) <= 0 || strlen(tableName) <= 0) {
        instanceMessageDialog(pButton, "Entrez le nom de la table et du champ");
        return;
    }

    // Vérification de la présence d'un champ avec un nom identique
    Field *fieldTmp = createTableDialog->table->fieldHead;
    while (fieldTmp != NULL)
    {
        if (!strcmp(fieldTmp->name, fieldName))
        {
            instanceMessageDialog(pButton, "Un champ avec le même nom à déjà été ajouté.");
            return;
        }
        fieldTmp = fieldTmp->next;
    }

    radioList = gtk_radio_button_get_group(GTK_RADIO_BUTTON(createTableDialog->pObject[2]));
    while (radioList)
    {
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radioList->data))) {
            field->name = xmalloc(sizeof(char *), __func__);
            if (!field->name) {
                instanceMessageDialog(pButton, "Erreur d'allocation mémoire.");
                return;
            }
            strcpy(field->name, fieldName);
            field->type = getEnum(i);
            field->next = NULL;
            if (createTableDialog->table->fieldHead == NULL)
                createTableDialog->table->fieldHead = field;
            else
            {
                Field *tmp = createTableDialog->table->fieldHead;
                while (tmp->next != NULL)
                    tmp = tmp->next;
                tmp->next = field;
            }
            createTableDialog->fieldCount++;
            radioList = NULL;
        }
        else {
            radioList = g_slist_next(radioList);
            i++;
        }
    }
    initFieldsComboBox(createTableDialog);
}

void createTableCallback(GtkWidget *pButton, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;
    GtkWidget *dialog;
    GtkWidget *tableNameEntry;
    GtkWidget *fieldNameEntry;
    GtkWidget *content_area;
    GtkWidget *addFieldButton;
    GtkWidget *contentHbox;
    GtkWidget *intRadio;
    GtkWidget *floatRadio;
    GtkWidget *charRadio;
    GtkWidget *varcharRadio;
    GtkWidget *tableInformationsVbox;
    GtkWidget *tableShowingInformationsHbox;
    GtkWidget *radioLabel;
    GtkWidget *radioHbox1;
    GtkWidget *fieldListView;
    GtkWidget *fieldsListScrollbar;
    GtkListStore *fieldsListStore;
    GtkTreeViewColumn *fieldsColumn;
    GtkCellRenderer *fieldsCellRenderer;
    CreateTableDialog *createTableDialog;

    createTableDialog = xmalloc(sizeof(CreateTableDialog), __func__);
    if (!createTableDialog)
    {
        updateInformationLabel(gtkDatabase, "Impossible de créer une table.");
        return;
    }

    dialog = gtk_dialog_new_with_buttons ("Création de table", GTK_WINDOW(gtk_widget_get_toplevel(pButton)), GTK_DIALOG_MODAL, "Annuler", GTK_RESPONSE_CANCEL, "Créer la table", GTK_RESPONSE_OK, NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    // Initialisation de l'element "GTK_ENTRY" permettant d'entrer le nom de la table
    tableNameEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(tableNameEntry), "Nom de la table");

    // Initialisation de l'element "GTK_ENTRY" permettant d'entrer le nom du champ à ajouter
    fieldNameEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fieldNameEntry), "Nom du champ");

    radioLabel = gtk_label_new("Choisissez le type du champ");

    addFieldButton = gtk_button_new_with_label("Ajouter le champ dans la table");

    varcharRadio = gtk_radio_button_new_with_label(NULL, "VARCHAR");
    charRadio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(varcharRadio), "CHAR");
    floatRadio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(varcharRadio), "FLOAT");
    intRadio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(varcharRadio), "INT");

    contentHbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    tableInformationsVbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    tableShowingInformationsHbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    radioHbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    //g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);

    gtk_window_set_default_size(&(GTK_DIALOG(dialog)->window), 500, 350);

    // Ajout liste de gauche affichant les champs de la table à créer

    fieldsListStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    gtkDatabase->pTableDataList = NULL;

    fieldListView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(fieldsListStore));

    // Ajout de la colonne "champ"
    fieldsCellRenderer = gtk_cell_renderer_text_new();
    fieldsColumn = gtk_tree_view_column_new_with_attributes("Champ", fieldsCellRenderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(fieldListView), fieldsColumn);

    // Ajout de la colonne "type"
    fieldsCellRenderer = gtk_cell_renderer_text_new();
    fieldsColumn = gtk_tree_view_column_new_with_attributes("Type", fieldsCellRenderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(fieldListView), fieldsColumn);

    // Ajout d'un element ScrollBar si il y a beaucoup de champ dans la table à créer
    fieldsListScrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(fieldsListScrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(fieldsListScrollbar), fieldListView);

    gtk_container_add(GTK_CONTAINER (content_area), contentHbox);

    // Encastre la GTK_BOX "contentHbox" dans son parent meme si l'espace necessaire etait deja suffisant
    gtk_box_set_child_packing(GTK_BOX(content_area), contentHbox, TRUE, TRUE, 0, GTK_PACK_START);
    gtk_box_set_homogeneous(GTK_BOX(contentHbox), TRUE);
    gtk_box_pack_start(GTK_BOX(contentHbox), tableShowingInformationsHbox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(contentHbox), tableInformationsVbox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(tableShowingInformationsHbox), fieldsListScrollbar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(tableInformationsVbox), tableNameEntry, FALSE, FALSE, 15);
    gtk_box_pack_start(GTK_BOX(tableInformationsVbox), fieldNameEntry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(tableInformationsVbox), radioLabel, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(tableInformationsVbox), radioHbox1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(tableInformationsVbox), addFieldButton, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(radioHbox1), intRadio, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(radioHbox1), floatRadio, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(radioHbox1), charRadio, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(radioHbox1), varcharRadio, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(addFieldButton), "clicked", G_CALLBACK(addFieldNewTableCallback), createTableDialog);

    createTableDialog->pObject[0] = fieldNameEntry;
    createTableDialog->pObject[1] = tableNameEntry;
    createTableDialog->pObject[2] = intRadio;
    createTableDialog->pObject[3] = tableShowingInformationsHbox;
    createTableDialog->fieldsList = fieldsListStore;
    createTableDialog->table = xmalloc(sizeof(Table), __func__);
    createTableDialog->table->name = xmalloc(sizeof(char*), __func__);
    createTableDialog->table->fieldHead = NULL;
    createTableDialog->table->pk = 0;
    createTableDialog->table->next = NULL;
    createTableDialog->fieldCount = 0;

    initFieldsComboBox(createTableDialog);

    gtk_widget_show_all(dialog);

    switch (gtk_dialog_run(GTK_DIALOG(dialog)))
    {
        case GTK_RESPONSE_OK:
            if (createTableDialog->fieldCount == 0)
                instanceMessageDialog(pButton, "Vous n'avez enregistré aucun champ. La table ne sera pas créée.");
            else {
                int result;
                strcpy(createTableDialog->table->name, gtk_entry_get_text(GTK_ENTRY(createTableDialog->pObject[1])));
                result = createTable(gtkDatabase->database, createTableDialog->table);
                // réinitialisation de la database
                Database *database = initDatabase(gtkDatabase->database->name);
                useDatabase(database);
                freeDatabase(gtkDatabase->database);
                gtkDatabase->database = database;
                // fin de la réinitialisation
                updateInformationLabel(gtkDatabase, isSuccessGtk(result));
                initTableComboBox(gtkDatabase);
            }
            gtk_widget_destroy(dialog);
            break;
        case GTK_RESPONSE_CANCEL:
            freeTable(gtkDatabase->database, createTableDialog->table);
            gtk_widget_destroy(dialog);
            break;
        default:
            gtk_widget_destroy(dialog);
            break;
    }
}

void removeTableCallback(GtkWidget *pButton, gpointer data)
{
    char *tableName = NULL;

    GtkDatabase *gtkDatabase = (GtkDatabase*)data;
    if (gtkDatabase->database == NULL)
        return;
    getTableSelectedName(gtkDatabase, &tableName);
    if (tableName != NULL)
    {
        Table *table = getTableFromDatabase(gtkDatabase, tableName);
        table->name = tableName;
        updateInformationLabel(gtkDatabase, isSuccessGtk(dropTable(gtkDatabase->database, table)));
        initTableComboBox(gtkDatabase);
    }
}

void createDatabaseCallback(GtkWidget *pButton, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;
    GtkWidget *pTempEntry;
    GList *pEntryList;
    GList *pList;
    const gchar *sText;

    pList = gtk_container_get_children(GTK_CONTAINER(gtk_widget_get_parent(gtk_widget_get_parent(pButton))));
    pList = g_list_next(pList);
    pList = g_list_next(pList);
    pEntryList = gtk_container_get_children(GTK_CONTAINER(pList->data));
    pTempEntry = GTK_WIDGET(pEntryList->data);
    sText = gtk_entry_get_text(GTK_ENTRY(pTempEntry));
    if (strlen(sText) > 0)
    {
        gtkDatabase->database = initDatabase(sText);
        if (!createDatabase(gtkDatabase->database)) {
            useDatabase(gtkDatabase->database);
            gtk_label_set_text(GTK_LABEL(gtkDatabase->pObject[6]), gtkDatabase->database->name);
            initTableComboBox(gtkDatabase);
            gtk_widget_set_sensitive(gtkDatabase->pObject[9], TRUE);
            gtk_widget_set_sensitive(gtkDatabase->pObject[4], TRUE);
        }
    }
    g_list_free(pList);
}

void removeDatabaseCallback(GtkWidget *pButton, gpointer data)
{
    GtkDatabase *gtkDatabase = (GtkDatabase*)data;

    if (gtkDatabase->database != NULL)
    {
        if (!dropDatabase(gtkDatabase->database)) {
            gtkDatabase->database = NULL;
            gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
            gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);
            gtk_label_set_text(GTK_LABEL(gtkDatabase->pObject[6]), "Aucune bdd sélectionnée");
            initDefaultValues(gtkDatabase);
        }
    }
}

void getTableSelectedName(GtkDatabase *gtkDatabase, gchar **text)
{
    GtkTreeModel *pTableModel;
    GtkTreeIter iter;

    pTableModel = gtk_combo_box_get_model(GTK_COMBO_BOX(gtkDatabase->pObject[8]));
    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(gtkDatabase->pObject[8]), &iter);
    if (pTableModel != NULL)
        gtk_tree_model_get(pTableModel, &iter, 0, text, -1);
}

Table *getTableFromDatabase(GtkDatabase *gtkDatabase, char *tableName)
{
    Table *table;

    table = gtkDatabase->database->tableHead;
    if (table == NULL)
        return NULL;
    while (table != NULL)
    {
        if (!strcmp(table->name, tableName)) {
            return table;
        }
        table = table->next;
    }
    return NULL;
}

GtkWidget *initWindow(GtkDatabase* gtkDatabase)
{
    GtkWidget *pWindow;
    GtkWidget *pBox[13];
    GtkTreeIter iterTable;
    GtkCellRenderer *pTableCellRenderer;

    pBox[0] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[1] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    pBox[2] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    pBox[3] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[4] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[5] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[6] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[7] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[8] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[9] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[10] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtkDatabase->pTableDataBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[12] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 850, 400);
    gtk_window_set_title(GTK_WINDOW(pWindow), "yamlDB");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(onDestroy), NULL);

    gtkDatabase->pObject[13] = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkDatabase->pObject[13]), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtkDatabase->pTableList = gtk_list_store_new(1, G_TYPE_STRING);
    gtkDatabase->pObject[8] = gtk_combo_box_new_with_model(GTK_TREE_MODEL(gtkDatabase->pTableList));
    pTableCellRenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkDatabase->pObject[8]), pTableCellRenderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkDatabase->pObject[8]), pTableCellRenderer, "text",  0, NULL);
    gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
    gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, "Liste des tables", -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);

    gtkDatabase->pTableDataList = NULL;
    gtkDatabase->pTableDataListView = gtk_tree_view_new_with_model(NULL);
    gtkDatabase->tableDataExist = TRUE;

    gtkDatabase->pObject[5] = gtk_label_new("Base de données: ");
    gtkDatabase->pObject[6] = gtk_label_new("Aucune bdd sélectionnée");
    gtkDatabase->pObject[7] = gtk_label_new(NULL);

    gtkDatabase->pObject[1] = gtk_button_new_with_label("Selectionner");
    gtkDatabase->pObject[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gtkDatabase->pObject[0]), "Selectionner une base de données");

    gtkDatabase->pObject[3] = gtk_button_new_with_label("Créer");
    gtkDatabase->pObject[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gtkDatabase->pObject[2]), "Créer une base de données");

    gtkDatabase->pObject[12] = gtk_button_new_with_label("Lancer");
    gtkDatabase->pObject[11] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gtkDatabase->pObject[11]), "Tapez votre requête SQL");

    gtkDatabase->pObject[9] = gtk_button_new_with_label("Créer une table");
    gtkDatabase->pObject[10] = gtk_button_new_with_label("Supprimer la table");

    gtkDatabase->pObject[4] = gtk_button_new_with_label("Supprimer la base de données");

    gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[9], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[10], FALSE);

    gtk_container_add(GTK_CONTAINER(pWindow), pBox[0]);

    gtk_box_pack_start(GTK_BOX(pBox[0]), pBox[1], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[0]), pBox[2], FALSE, FALSE, 5);
    // Encastre la vue des resultats de droit en utilisant tout l'espace libre meme si l'espace était déjà suffisant pour afficher les données
    gtk_box_set_child_packing(GTK_BOX(pBox[0]), pBox[2], TRUE, TRUE, 0, GTK_PACK_START);

    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[3], FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[4], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[5], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[6], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[7], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[8], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[9], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[10], FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[2]), gtkDatabase->pTableDataBox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(gtkDatabase->pTableDataBox), gtkDatabase->pObject[13], TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(gtkDatabase->pObject[13]), gtkDatabase->pTableDataListView);
    gtk_box_pack_start(GTK_BOX(pBox[2]), pBox[12], FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pBox[3]), gtkDatabase->pObject[5], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[3]), gtkDatabase->pObject[6], FALSE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[4]), gtkDatabase->pObject[0], TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[4]), gtkDatabase->pObject[1], FALSE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[5]), gtkDatabase->pObject[2], TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[5]), gtkDatabase->pObject[3], FALSE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[6]), gtkDatabase->pObject[4], TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[7]), gtkDatabase->pObject[8], TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[8]), gtkDatabase->pObject[9], TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[9]), gtkDatabase->pObject[10], TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[10]), gtkDatabase->pObject[7], TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[12]), gtkDatabase->pObject[11], TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[12]), gtkDatabase->pObject[12], FALSE, FALSE, 5);

    g_signal_connect(G_OBJECT(gtkDatabase->pObject[1]), "clicked", G_CALLBACK(initDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[3]), "clicked", G_CALLBACK(createDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[4]), "clicked", G_CALLBACK(removeDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[9]), "clicked", G_CALLBACK(createTableCallback), gtkDatabase);
    gtkDatabase->comboxBoxSignalChanged = g_signal_connect(G_OBJECT(gtkDatabase->pObject[8]), "changed", G_CALLBACK(onActiveItemChangerComboBox), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[10]), "clicked", G_CALLBACK(removeTableCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[12]), "clicked", G_CALLBACK(runSqlRequestCallback), gtkDatabase);

    gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);
    gtk_widget_show_all(pWindow);
    return pWindow;
}

void run(int argc, char **argv)
{
    GtkDatabase *gtkDatabase = xmalloc(sizeof(GtkDatabase), __func__);
    gtkDatabase->database = NULL;
    /* Initialisation de GTK+ */
    gtk_init(&argc, &argv);

    /* Initialisation de la fenêtre */
    GtkWidget *p_window = initWindow(gtkDatabase);

    /* Lancement de la boucle principale */
    gtk_main ();
}