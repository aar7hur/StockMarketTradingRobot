/**** TODO
 * --> CREATE METHODS TO HANDLE WITH USER INPUT.
 * --> CREATE EVENTS FOR INTERFACE MENUS
 * 
 * */

#include <iostream>
#include "userInterface.h"
#include <cstring>
#define NDEBUG
#include <cassert>

/******************************************s
*
* Constructor used to instantiate private variables
* in userInterface class
*
******************************************/
UserInterface::UserInterface(int *argc, char ***argv)
{
    // initiate gtk application
    gtk_init(argc, argv);
    
    // if user clicks on 'x' the application will destroy
    // and closed the window
	this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(this->window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    this->layout = gtk_layout_new(NULL, NULL);
    this->virtual_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    this->horizontal_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	this->button = gtk_button_new_with_label("Start ");
	this->image =  gtk_image_new_from_file("/home/arthur/Downloads/StockMarket.jpg");
    this->menu_bar = gtk_menu_bar_new();
    this->help_menu = gtk_menu_new();
    this->file_menu = gtk_menu_new();
    this->menu_item_file = gtk_menu_item_new_with_label("File");
    this->menu_item_help = gtk_menu_item_new_with_label("Help");
}

UserInterface::UserInterface(){}
UserInterface::~UserInterface(){}
/******************************************
*
* Method used to destroy UserInterface Object and 
* also close the window
*
******************************************/
void UserInterface::destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

void UserInterface::create_menu()
{
	const char* file[] = {"New","Check","Exit"};
    const char* help[] = {"About"};

    // set submenu inside file_menu
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(this->menu_item_file),this->file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->menu_bar), this->menu_item_file);

    for(int i=0;i<3;i++)
    {
        this->menu_item_file = gtk_menu_item_new_with_label(file[i]);
        gtk_menu_shell_append(GTK_MENU_SHELL(this->file_menu),this->menu_item_file);
        g_signal_connect(this->menu_item_file,"activate",G_CALLBACK(menu_event),NULL);
    }

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(this->menu_item_help),this->help_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->menu_bar), this->menu_item_help);
    this->menu_item_help = gtk_menu_item_new_with_label(help[0]);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->help_menu),this->menu_item_help);
    g_signal_connect(this->menu_item_help,"activate",G_CALLBACK(menu_event), NULL);
    

    gtk_box_pack_start(GTK_BOX(this->virtual_box), this->menu_bar, 0, 0, 0);
    gtk_layout_put(GTK_LAYOUT(this->layout), this->virtual_box, 0, 0);
    this->window_status = WINDOW_IS_BEING_CREATED;

}

void UserInterface::display_window()
{
    try
    {
        gtk_window_set_title(GTK_WINDOW(this->window),"Stock Trading Robot");
        gtk_window_set_default_size(GTK_WINDOW(this->window), 711, 473);
        gtk_window_set_position(GTK_WINDOW(this->window), GTK_WIN_POS_CENTER);
        gtk_widget_show_all(this->window);
        gtk_main();
        this->window_status = WINDOW_IS_BEING_SHOWN;
    }
    catch(...)
    {
        this->window_status = WINDOW_ERROR;
    }
}

void UserInterface::create_background_image()
{
    try
    {
        this->layout = gtk_layout_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER (this->window), this->layout);
        gtk_widget_show(this->layout);
        gtk_layout_put(GTK_LAYOUT(this->layout), this->image, 0, 0);
        this->window_status = WINDOW_IS_BEING_CREATED;  
    }
    catch(...)
    {
        this->window_status = WINDOW_ERROR;
    }
}

void UserInterface::create_user_entry()
{
    GtkWidget* aux;
    gdk_rgba_parse(color, "rgba(255,255,255,2)");
    
    /* Create a 1x2 table */
    this->table = gtk_grid_new();

    gtk_layout_put(GTK_LAYOUT(this->layout), this->table, 70, 120);
    
    //create a text box
    this->entries[0] = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(this->entries[0]),10);
    gtk_widget_set_size_request(this->entries[0],20,20);
    gtk_grid_attach (GTK_GRID (this->table), this->entries[0], 0, 0, 1, 1);

    //create label for text box
    this->labels[0] = gtk_label_new("Qual ação deseja avaliar");
    gtk_grid_attach (GTK_GRID (this->table),this->labels[0], 1, 0, 1, 1);
    gtk_widget_override_color(this->labels[0],  GTK_STATE_FLAG_NORMAL, this->color);
  

    this->entries[1] = gtk_entry_new();
    gtk_grid_attach (GTK_GRID (this->table), this->entries[1], 0, 1, 1, 1);


    //create label for text box
    this->labels[1] = gtk_label_new("Qual o montante que você investirá");
    gtk_grid_attach (GTK_GRID (this->table),this->labels[1], 1, 1, 1, 1);
    gtk_widget_override_color(this->labels[1],  GTK_STATE_FLAG_NORMAL, this->color);

    
    //create button to perform calculations
    this->button = gtk_button_new_with_mnemonic("RUN!");
    g_signal_connect_swapped (this->button, "clicked", G_CALLBACK (entry_submit), this->entries);
    gtk_grid_attach (GTK_GRID (this->table), this->button, 0, 2, 1, 1);
    aux = this->create_progress_bar();

    gtk_widget_set_size_request(aux,30,30);
    gtk_grid_attach (GTK_GRID (this->table), aux, 0, 3, 1, 1);
    g_timeout_add( 100, (GSourceFunc)this->update_progress_bar, aux);
}

GtkWidget* UserInterface::create_progress_bar(void)
{
    std::cout << "teste";
    GtkWidget *progress_bar;
    progress_bar = gtk_progress_bar_new();

    return progress_bar;
}

void UserInterface::run_window(void)
{   
    this->create_background_image();
    assert (this->window_status == WINDOW_IS_BEING_CREATED);
    this->create_menu();
    assert (this->window_status == WINDOW_IS_BEING_CREATED);
    this->create_user_entry();
    this->display_window();
    assert (this->window_status.value == WINDOW_IS_BEING_SHOWN);
}


void UserInterface::menu_event(GtkWidget *menu_item, gpointer data)
{
    if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Exit") == 0)
    {
        g_print("Exiting...");
        gtk_main_quit();
    }
    if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "About") == 0)
    {
        g_print("About...");
    }
}

void UserInterface::entry_submit(GtkWidget **entry, GtkWidget *widget)
{
    GtkWidget *entry_ptr_action = entry[0];
    GtkWidget *entry_ptr_money = entry[1];

    const gchar *action, *money;

    action = gtk_entry_get_text(GTK_ENTRY (entry_ptr_action));
    g_print("A ação que vocẽ deseja avaliar é: %s\n", action);
    money = gtk_entry_get_text(GTK_ENTRY (entry_ptr_money));
    g_print("O montante de dinheiro que voce tem é: %s\n", money);
    
}

gboolean UserInterface::update_progress_bar(GtkProgressBar* progress_bar)
{
    gtk_progress_bar_pulse(progress_bar);
    gtk_progress_bar_get_pulse_step(progress_bar);
    gtk_progress_bar_set_show_text(progress_bar, FALSE);
    
    return TRUE;
}

