#ifndef USERINTERFACE_HEADER_GUARD
#define USERINTERFACE_HEADER_GUARD

#include <gtk/gtk.h>
#include "menu.h"
#include "decision.h"
#include "layout.h"
#include <string>

enum Window
{
	WINDOW_IS_BEING_CREATED,
	WINDOW_IS_BEING_SHOWN,
	WINDOW_CLOSED,
	WINDOW_ERROR,
};

/******************************************s
*
* Classe usada para criar a interface com o 
* usuário, receber os dados de entrada e 
* utilizar classes auxiliares para fazer
* os cálculos nessecários.
*
******************************************/
class UserInterface: public Menu, public Layout
{
	private:
		std::string user_input;
		enum Window window_status;
		static void menu_event(GtkWidget *menu_event, gpointer data);
		static void entry_submit(GtkWidget** entry, GtkWidget *widget);
		static void destroy(GtkWidget *widget, gpointer data);
		static gboolean update_progress_bar(GtkProgressBar* progress_bar);
		static void printResult();
	
	protected:
		void create_menu(void);
		void create_background_image(void);
		void display_window(void);
		void create_user_entry(void);
		GtkWidget* create_progress_bar(void);

	public:
		UserInterface();
		UserInterface(int *argc, char ***argv);
		~UserInterface();
		std::string get_user_input(void);
		void run_window(void);
			
};

#endif