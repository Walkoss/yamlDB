//
// Created by Iliasse Wahbi on 13/12/2017.
//

#include "gtk.h"

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

    //initTableComboBox(gtkDatabase);
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);
    gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);
}

void initTableComboBox(GtkDatabase *gtkDatabase)
{
    Table *tableTmp;

    gtk_list_store_clear(gtkDatabase->pTableList);
    if (gtkDatabase->database != NULL && gtkDatabase->database->tableHead == NULL) {
        GtkTreeIter iterTable;
        gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
        gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, "Aucune table existante.", -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);                               // On focus le premier element de la liste des tables
        gtk_widget_set_sensitive(gtkDatabase->pObject[8], 0);                                              // Désactive la comboBox
    }
    else {
        tableTmp = gtkDatabase->database->tableHead;
        gtk_widget_set_sensitive(gtkDatabase->pObject[8], TRUE);
        while (tableTmp != NULL) {
            GtkTreeIter iterTable;
            gchar *tableName = g_malloc(50);
            strcpy(tableName, tableTmp->name);
            gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
            gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, tableName, -1);
            tableTmp = tableTmp->next;
            g_free(tableName);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);                    // On focus le premier element de la liste des tables
    }
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
    if (gtkDatabase->database)
        freeDatabase(gtkDatabase->database);
    if (strlen(sText) > 0)
    {
        gtkDatabase->database = initDatabase(sText);
        if (!(result = useDatabase(gtkDatabase->database))) {
            gtk_label_set_text(GTK_LABEL(gtkDatabase->pObject[6]), gtkDatabase->database->name);              // on met le nom de la base de données dans le label du haut
            gtk_widget_set_sensitive(gtkDatabase->pObject[4], TRUE);                                          // on active le bouton supprimer (pour la base de données)
            gtk_widget_set_sensitive(gtkDatabase->pObject[8], TRUE);                                          // on active la ComboBox listant les tables de la bdd
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

    dialog = gtk_dialog_new_with_buttons ("Message", GTK_WINDOW(gtk_widget_get_toplevel(pButton)), GTK_DIALOG_MODAL, "Annuler", GTK_RESPONSE_CANCEL, "Créer la table", GTK_RESPONSE_OK, NULL);
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

    // Ensure that the dialog box is destroyed when the user responds

    //g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);

    gtk_window_set_default_size(&(GTK_DIALOG(dialog)->window), 500, 350);

    // Ajout liste de gauche affichant les champs de la table à créer

    fieldsListStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

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
            printf("Rien ?\n");
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
        isSuccess(dropTable(gtkDatabase->database, table));
        //initDatabaseCallback(NULL, gtkDatabase);
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
            gtk_widget_set_sensitive(gtkDatabase->pObject[4], TRUE);
            gtk_widget_set_sensitive(gtkDatabase->pObject[8], TRUE);
            initTableComboBox(gtkDatabase);
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
    pBox[11] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    pBox[12] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 850, 400);
    gtk_window_set_title(GTK_WINDOW(pWindow), "yamlDB");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(onDestroy), NULL);

    gtkDatabase->pTableList = gtk_list_store_new(1, G_TYPE_STRING);
    gtkDatabase->pObject[8] = gtk_combo_box_new_with_model(GTK_TREE_MODEL(gtkDatabase->pTableList));
    pTableCellRenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkDatabase->pObject[8]), pTableCellRenderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkDatabase->pObject[8]), pTableCellRenderer, "text",  0, NULL);
    gtk_list_store_prepend(gtkDatabase->pTableList, &iterTable);
    gtk_list_store_set(gtkDatabase->pTableList, &iterTable, 0, "Liste des tables", -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(gtkDatabase->pObject[8]), 0);

    gtkDatabase->pObject[5] = gtk_label_new("Base de données: ");
    gtkDatabase->pObject[6] = gtk_label_new("Aucune bdd sélectionnée");
    gtkDatabase->pObject[7] = gtk_label_new(NULL);

    gtkDatabase->pObject[1] = gtk_button_new_with_label("Selectionner");
    gtkDatabase->pObject[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gtkDatabase->pObject[0]), "Selectionner une base de données");

    gtkDatabase->pObject[3] = gtk_button_new_with_label("Créer");
    gtkDatabase->pObject[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gtkDatabase->pObject[2]), "Créer une base de données");

    gtkDatabase->pObject[9] = gtk_button_new_with_label("Créer une table");
    gtkDatabase->pObject[10] = gtk_button_new_with_label("Supprimer la table");

    gtkDatabase->pObject[4] = gtk_button_new_with_label("Supprimer la base de données");

    gtk_container_add(GTK_CONTAINER(pWindow), pBox[0]);

    gtk_box_pack_start(GTK_BOX(pBox[0]), pBox[1], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[0]), pBox[2], FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[3], FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[4], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[5], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[6], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[7], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[8], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[9], FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(pBox[1]), pBox[10], FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(pBox[2]), pBox[11], FALSE, FALSE, 0);
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

    g_signal_connect(G_OBJECT(gtkDatabase->pObject[1]), "clicked", G_CALLBACK(initDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[3]), "clicked", G_CALLBACK(createDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[4]), "clicked", G_CALLBACK(removeDatabaseCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[9]), "clicked", G_CALLBACK(createTableCallback), gtkDatabase);
    g_signal_connect(G_OBJECT(gtkDatabase->pObject[10]), "clicked", G_CALLBACK(removeTableCallback), gtkDatabase);

    gtk_widget_set_sensitive(gtkDatabase->pObject[4], FALSE);
    gtk_widget_set_sensitive(gtkDatabase->pObject[8], FALSE);
    gtk_widget_show_all(pWindow);
    return pWindow;
}

void run(int argc, char **argv, Database* database)
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