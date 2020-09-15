#include "../include/main.h"
#include "../include/decision.h"
#include <iostream>

void noInterface(void);
int getCmdOptions(int argc, char* argv[], const std::string& option);
void printUsage(void);


int main(int argc, char * argv[])

{
    if(getCmdOptions(argc, argv, "--no_interface") == 0)
    {
        noInterface();
    }

    else if(getCmdOptions(argc, argv, "--interface") == 0 )
    {
        UserInterface *user = new UserInterface(&argc, &argv);
        user->run_window();
    }
    else
    {
        printUsage();
        exit(-1);
    }
    
}

/*******************************************************************************
 *	printUsage
 *	----------------------
 * 	Printa para o usuário as instruções para utilizar o robo
 *  retorna: nada
 *  recebe: nada
 *
 ******************************************************************************/
void printUsage(void)
{
    std::cout << "Você paderá usar o robo de duas maneiras: \n\t" 
    "1. Através da interface gráfica" << "\n\t 2. Através do terminal " << std::endl;
    std::cout << "Para rodar com interface gráfica basta passar o argumento --interface"
    "na linha de comando ao rodar o script \n Para rodar sem a interface gráfica basta"
    "passar o argumento --no_interface para a linha de comando.\n\n" << std::endl;

}

/*******************************************************************************
 *	getCmdOptions
 *	----------------------
 * 	Compara as escolhas do usuario pela linha de comando e retorna
 *  0 se a string comparada for igual.populaa variavel da struct que corresponde a isso.
 *  recebe: quandidade de comandos passados pela linha de comando. 
 *  Os comandos passados e a string para comparar
 *  retorna: int 0 ->igual 1->diferente
 ******************************************************************************/
int getCmdOptions(int argc, char* argv[], const std::string& option)
{
    std::string cmd;
     for( int i = 0; i < argc; ++i)
     {
          std::string arg = argv[i];
          if(0 == arg.find(option))
          {
               std::size_t found = arg.find_last_of(option);
               cmd =arg.substr(found + 1);
               return 0;
          }
     }
     return 1;
}


/*******************************************************************************
 *	noInterface
 *	----------------------
 * 	Caso nao solicitado o uso de interface grafica, o programa irá solicitar
 *  a ação e o montande de dinheiro para o usuário e fará os calculos
 *  retorna: nada
 *  recebe: nada
 *
 ******************************************************************************/
void noInterface(void)
{
    std::string acao;
    float montante;
    std::cout << "Entre com a ação que deseja escolher: ";
    std::cin >> acao;
    std::cout << "Entre com o montante de dinheiro que você usará para a aplicação: ";
    std::cin >> montante;

    Decision decision;
    decision.setUserMoney(montante);

    // Popula os dados necessários para fazer os calculos.
    // Ver documentação do metodo na classe em: Decision.cpp
    decision.populateData(acao);

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
                                "!\nQtdStocks: " + qtdStocks + "\n";

    std::cout << outputUser;
}