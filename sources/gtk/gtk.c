//
// Created by Iliasse Wahbi on 13/12/2017.
//

#include "gtk.h"

void onDestroy(GtkWidget *pWidget, gpointer pData)
{
    /* Arrêt de la boucle événementielle */
    gtk_main_quit();
}

void run(int argc, char **argv)
{
    GtkWidget *p_window = NULL;

    /* Initialisation de GTK+ */
    gtk_init (&argc, &argv);

    /* Creation de la fenetre principale de notre application */
    p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* Affichage de la fenetre principale */
    gtk_widget_show (p_window);
    /* Titre de la fenetre */
    gtk_window_set_title(GTK_WINDOW(p_window), "yamlDB");

    g_signal_connect(G_OBJECT(p_window), "destroy", G_CALLBACK(OnDestroy), NULL);

    gtk_window_set_position(GTK_WINDOW(p_window), GTK_WIN_POS_CENTER);

    /* Lancement de la boucle principale */
    gtk_main ();
}