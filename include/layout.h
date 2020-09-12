#ifndef LAYOUT_HEADER_GUARD
#define LAYOUT_HEADER_GUARD

#include <gtk/gtk.h>


class Layout
{
    protected:
        GtkWidget *window;
        GtkWidget *button;
        GtkWidget *image;
        GtkWidget *virtual_box;
        GtkWidget *horizontal_box;
        GtkWidget *layout;
        GtkWidget **entries;
        GtkWidget *table;
        GtkWidget **labels;
        GdkRGBA *color;
        
   
        void create_background_image(void);
        void display_window(void);
        void create_user_entry(void);
        GtkWidget *create_progress_bar(void);
};

#endif