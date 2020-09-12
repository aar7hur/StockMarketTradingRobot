#ifndef MENU_HEADER_GUARD
#define MENU_HEADER_GUARD

#include <gtk/gtk.h>
#include <string>


class Menu
{
    protected:
        GtkWidget *menu_bar;
		GtkWidget *help_menu;
        GtkWidget *file_menu;
		GtkWidget *menu_item_file;
		GtkWidget *menu_item_help;
		GtkWidget *menu;
      
        void create_menu(void);
};

#endif