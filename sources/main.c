#include "interface/interface.h"
#include "gtk/gtk.h"

int main(int argc, char **argv) {
    Database* database = NULL;
    //userInterface();
    run(argc, argv, database);
    return 0;
}