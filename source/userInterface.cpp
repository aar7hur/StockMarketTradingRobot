



#include <iostream>
#include "../include/userInterface.h"
#include "../include/decision.h"
#include <cstring>
#include <fstream>
#define NDEBUG
#include <cassert>
#include <fstream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*************************************************
 * UserInterface :: UserInterface --> CONSTRUCTOR
 *************************************************
 * Seta os atributos privados da sua classe.
 ********************************************
 * Recebe: nada
 * Retorna: nada
 ************************************************/
UserInterface::UserInterface(int *argc, char ***argv)
{
    // initiate gtk application
    gtk_init(argc, argv);
    
    // Se o usuário clicar no botao x da janela, a interface irá
    // fechar automaticamente e a aplicação será destruida
	this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(this->window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    this->layout = gtk_layout_new(NULL, NULL);
    this->virtual_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    this->horizontal_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	this->button = gtk_button_new_with_label("Start ");
	this->image =  gtk_image_new_from_file("/home/arthur/StockMarket.jpg");
    this->menu_bar = gtk_menu_bar_new();
    this->help_menu = gtk_menu_new();
    this->file_menu = gtk_menu_new();
    this->menu_item_file = gtk_menu_item_new_with_label("File");
    this->menu_item_help = gtk_menu_item_new_with_label("Help");
}

UserInterface::UserInterface(){}
UserInterface::~UserInterface(){}

/**********************************************
 * UserInterface :: destroy
 ***********************************************
 * Destroy o objeto UserInterface toda vez que
 * o usuário clicar em em menu->exit ou no x da 
 * janela.
 ********************************************
 * Recebe: nada
 * Retorna: nada
 **********************************************/
void UserInterface::destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}


/**********************************************
 * UserInterface :: create_menu
 ***********************************************
 * Cria a barra do menu com opções de: New,
 * Check, Exit e adiciona a barra do menu dentro
 * de uma "caixa externa" chamada virtual_box. 
 * Por fim, monta o menu dentro da caixa virtual
 * e adiciona a caixa virtual ao layout da pagina.
 ********************************************
 * Recebe: nada
 * Retorna: nada
 **********************************************/
void UserInterface::create_menu()
{
	const char* file[] = {"New","Check","Exit"};
    const char* help[] = {"About"};

    // adiciona o submenu dentro do menu "file"
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(this->menu_item_file),this->file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->menu_bar), this->menu_item_file);

    for(int i=0;i<3;i++)
    {
        // Cria os items do menu de acordo com o array const char* file[]
        this->menu_item_file = gtk_menu_item_new_with_label(file[i]);
        gtk_menu_shell_append(GTK_MENU_SHELL(this->file_menu),this->menu_item_file);
        g_signal_connect(this->menu_item_file,"activate",G_CALLBACK(menu_event),NULL);
    }

    // seta submenu dentro do menu "help"
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(this->menu_item_help),this->help_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->menu_bar), this->menu_item_help);
    this->menu_item_help = gtk_menu_item_new_with_label(help[0]);
    gtk_menu_shell_append(GTK_MENU_SHELL(this->help_menu),this->menu_item_help);
    g_signal_connect(this->menu_item_help,"activate",G_CALLBACK(menu_event), NULL);
    
    // coloca a barra do menu dentro da caixa virtual
    // adiciona a caixa virtual dentro do layout da pagina
    gtk_box_pack_start(GTK_BOX(this->virtual_box), this->menu_bar, 0, 0, 0);
    gtk_layout_put(GTK_LAYOUT(this->layout), this->virtual_box, 0, 0);
    this->window_status = WINDOW_IS_BEING_CREATED;

}

/********************************************
 * UserInterface :: display_window
 ********************************************
 * Seta o titulo, o tamanho e a posição da 
 * interface para o usuário e mostra o layout 
 * que foi criado.
 ********************************************
 * Recebe: nada
 * Retorna: nada
 ********************************************/

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

/********************************************
 * UserInterface :: create_background_image
 ********************************************
 * Cria a imagem de fundo adicionando a imagem 
 * dentro do layout da página e adiaionando
 * o layout dentro da janela.

 ********************************************
 * Recebe: nada
 * Retorna: nada
 ********************************************/
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


/********************************************
 * UserInterface :: create_user_entry
 ********************************************
 * Cria a caixa de texto que vai solicitar
 * qual ação e o montante de dinheiro que o
 * usuário quer investir/tem. Cria também a
 * barra de progresso demonstrando o status
 * do calculo .
 * Por fim adiciona as caixas de texto no
 * layout da pagina.
 ********************************************
 * Recebe: nada
 * Retorna: nada
 ********************************************/
void UserInterface::create_user_entry()
{
    GtkWidget* progress_bar_aux;
    gdk_rgba_parse(color, "rgba(255,255,255,2)");
    
    // Cria a tabela onde será adicionado a entrada do usuário
    // e as labels para descrição
    this->table = gtk_grid_new();

    gtk_layout_put(GTK_LAYOUT(this->layout), this->table, 70, 120);
    
    // Cria entrada para qual ação o usário quer selecionar
    this->entries[0] = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(this->entries[0]),10);
    gtk_widget_set_size_request(this->entries[0],20,20);
    gtk_grid_attach (GTK_GRID (this->table), this->entries[0], 0, 0, 1, 1);

    // Cria label para a caixa de texto para a ação.
    this->labels[0] = gtk_label_new("Qual ação deseja avaliar");
    gtk_grid_attach (GTK_GRID (this->table),this->labels[0], 1, 0, 1, 1);
    gtk_widget_override_color(this->labels[0],  GTK_STATE_FLAG_NORMAL, this->color);
  
    // Cria etnrada para o montande de dinheiro do usuário
    this->entries[1] = gtk_entry_new();
    gtk_grid_attach (GTK_GRID (this->table), this->entries[1], 0, 1, 1, 1);


    // Cria label para a caixa de texto do montande de dinheiro
    this->labels[1] = gtk_label_new("Qual o montante que você investirá");
    gtk_grid_attach (GTK_GRID (this->table),this->labels[1], 1, 1, 1, 1);
    gtk_widget_override_color(this->labels[1],  GTK_STATE_FLAG_NORMAL, this->color);

    
    // Cria o botão de entrada para o usuário começar os calculos
    this->button = gtk_button_new_with_mnemonic("RUN!");
    g_signal_connect_swapped (this->button, "clicked", G_CALLBACK (entry_submit), this->entries);
    gtk_grid_attach (GTK_GRID (this->table), this->button, 0, 2, 1, 1);
    progress_bar_aux = this->create_progress_bar();

    gtk_widget_set_size_request(progress_bar_aux,30,30);
    gtk_grid_attach (GTK_GRID (this->table), progress_bar_aux, 0, 3, 1, 1);
    g_timeout_add( 100, (GSourceFunc)this->update_progress_bar, progress_bar_aux);
}


/********************************************
 * UserInterface :: create_progress_bar
 ********************************************
 * Cria a barra de progresso a ser visualizada
 * pelo usuário
 ********************************************
 * Recebe: nada
 * Retorna: ponteiro para a barra de progresso
 ********************************************/
GtkWidget* UserInterface::create_progress_bar(void)
{
    GtkWidget *progress_bar;
    progress_bar = gtk_progress_bar_new();

    return progress_bar;
}

/********************************************
 * UserInterface :: run_window
 ********************************************
 * Cria a imagem de fundo, o menu, a entrada
 * do usuário e mostra a interface na tela

 ********************************************
 * Recebe: nada
 * Retorna: nada
 ********************************************/
void UserInterface::run_window(void)
{   
    this->create_background_image();
    //this->create_menu();
    this->create_user_entry();
    this->display_window();
}

/***********************************************
 * UserInterface :: menu Event
 ***********************************************
 * Toda vez que o usuario clicar em alguma opção
 * do menu, essa função é chamada e, de acordo com
 * o menu escolhido, algumas ações são tomadas
 ***********************************************
 * Recebe: ponteiro para o menu, ponteiro para o dado
 * Retorna: nada
 **********************************************/
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

/***********************************************
 * UserInterface :: entry_submit
 ***********************************************
 * Toda vez que o usuario clicar entrar com dados
 * nas caixas de texto e clinar no botao RUN!, 
 * essa função é chamada e é ela que comandará 
 * a busca pelas ações no banco de dados e chamará
 * as classes auxiliares para executar os calculos
 * e retornar os dados para o usuário
 ***********************************************
 * Recebe: ponteiro apontando para um ponteiro
 * onde é armazenado as caixas de texto. Ponteiro
 * para o widget que gerou a chamada desse método,
 * nesse caso o botão RUN!.
 * Retorna: nada
 **********************************************/
void UserInterface::entry_submit(GtkWidget **entry, GtkWidget *widget)
{
    // Instancia objeto da classe decisão 
    Decision decision;
    const gchar *action, *money;

    GtkWidget *entry_ptr_action = entry[0];
    GtkWidget *entry_ptr_money = entry[1];

    // Pega a ação de entrada do usuário e guarda 
    action = gtk_entry_get_text(GTK_ENTRY (entry_ptr_action));
    g_print("A ação que vocẽ deseja avaliar é: %s\n", action);

    // Pega o montande de dinheiro de entrada do usuário e guarda 
    money = gtk_entry_get_text(GTK_ENTRY (entry_ptr_money));
    g_print("O montante de dinheiro que voce tem é: %s\n", money);

    // Converte *gchar para string
    std::string action_(action);


    // Converte opara float
    float user_money = (float)(strtod(money,NULL));
    g_print("User money: %f", user_money);

    //std::cout << typeid(user_money).name();
    // Seta a quantidade de dinheiro que o usuário passou
    // na interface para a classe decisão.
    decision.setUserMoney(user_money);

    // Popula os dados necessários para fazer os calculos.
    // Ver documentação do metodo na classe em: Decision.cpp
    decision.populateData(action_);

    // Faz a decisão de vender, comprar, ou aguardar.
    while(decision.doDecision() != CALCULATION_IS_OVER);

    // Cria uma struct para guardar os dados a serem mostrados
    // para o usuário
    struct userData user;

    // Pega os dados privados da classe Decision para mostrar ao
    // usuário
    user = decision.getUserData();

    // Converte os dados de float para string
    std::string stopLoss = std::to_string(user.stopLoss);
    std::string trigger = std::to_string(user.trigger); 
    std::string target = std::to_string(user.target);
    std::string qtdStocks = std::to_string(user.qtdStocks);
    // Printa os dados para o usuário no terminal
    std::string outputUser = user.message + "!!\n" + "stopLoss:" + stopLoss +
                            "!\nTrigger: " + trigger + "!\nTarget: " + target +
                            "!\nQtdStocks: " + qtdStocks;
    
   
    std::cout << outputUser;
    
}
/***********************************************
 * UserInterface :: entry_submit
 ***********************************************
 * Atualiza a barra de progresso com o andamento
 * dos calculos de compra e venda
 ***********************************************
 * Recebe: nada
 * Retorna: nada
 **********************************************/
gboolean UserInterface::update_progress_bar(GtkProgressBar* progress_bar)
{
    gtk_progress_bar_pulse(progress_bar);
    gtk_progress_bar_get_pulse_step(progress_bar);
    gtk_progress_bar_set_show_text(progress_bar, FALSE);
    
    return TRUE;
}
