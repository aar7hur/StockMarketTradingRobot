#include "../include/decision.h"
#include "../include/readcsv.h"
#include "../include/movingAverage.h"
#include "gtk/gtk.h"
#include <string>
#include <algorithm>
#include <iostream>

/*******************************************************************************
 *	Função: populateData
 *	----------------------
 * 	Função dedicada popular as structs com os dados necessários
 *	recebe: nada
 *	retorna: nada. Apenas popula os atributos da classe
 * OBS: Essa função será chamada uma vez apenas
 ******************************************************************************/
void Decision::populateData(std::string ativo){

    try{

        this->populateAverage(ativo);
        this->populateCloseWeek(ativo);
        this->populateStochastic(ativo);
    }
    catch(const char* msg)
    {
        std::cerr << msg << std::endl;
        std::cout << "Erro! Não existe esse ativo no sistema";
        exit(-1);
    }
}

/*******************************************************************************
 *	Decision:: populateAverage
 *	----------------------
 * 	Faz as medias moveis de 20 e 50 dados e popula as variaveis da struct
 *  com os resultados das médias
 *  receb: std::string Ativo. Qual ativo irá buscar no banco de dados
 *  retorna: nada
 ******************************************************************************/
void Decision::populateAverage(std::string ativo){

    Ativo meuAtivo(ativo, "w");
    float *array;
    meuAtivo["Close"] >> array;
    
    this->movingAverage.setmovingAverage(array, 20);
    this->decisionData.average_20 = this->movingAverage.getAverage();

    this->movingAverage.setmovingAverage(array, 50);
    this->decisionData.average_50 = this->movingAverage.getAverage();
    g_print("\nAverage 20: %f", this->decisionData.average_20);
    g_print("\nAverage 50 %f", this->decisionData.average_50);
    // Foi testado aqui, lendo e calculando average corretamente, mexer daqui pra frente
    
}

/*******************************************************************************
 *	Decision:: popuCloseWeek
 *	----------------------
 * 	Busca no banco de dados o fechamento do preço da ação da semana e 
 *  populaa variavel da struct que corresponde a isso.
 *  receb: std::string Ativo. Qual ativo irá buscar no banco de dados
 *  retorna: nada
 ******************************************************************************/
void Decision::populateCloseWeek(std::string ativo)
{
    Ativo meuAtivoClosed(ativo, "w");
    float *arrayClose;
    meuAtivoClosed["Close"] >> arrayClose; //ver quantas posições retorna no array
    this->decisionData.closeWeek = arrayClose[0];
    g_print("\nCloseWeek: %f\n", this->decisionData.closeWeek);

}


/*******************************************************************************
 *	Decision:: populateStochastic
 *	----------------------
 * 	Busca no banco de dados e faz os calculos estocasticos populando 
 *  a struct decisionData com os dados necessários para fazer a decisão
 *  de compra ou venda.
 *  recebe: nada
 *  retorna: nada
 ******************************************************************************/
void Decision::populateStochastic(std::string ativo)
{
    // Máximo de dados do CSV que devem ser usados
    enum {magic_periodo = 8}; 
    
    // Le a coluna de Close
    Ativo meuAtivoSto(ativo, "d");
   
    
    // Popula arrayClose
    gfloat *arrayClose; size_t sizeClose;
    sizeClose = meuAtivoSto["Close"](magic_periodo) >> arrayClose;
    gfloat closeMaisRecente = arrayClose[0];
    
    // Ler Baixas
    gfloat *arrayLow; size_t sizeLow;
    sizeLow = meuAtivoSto["Low"](magic_periodo) >> arrayLow;
    gfloat minimaDoPeriodo = *( std::min_element(arrayLow, &arrayLow[sizeLow]) );
    
    // Ler Altas
    gfloat *arrayHigh; size_t sizeHigh;
    sizeHigh = meuAtivoSto["High"](magic_periodo) >> arrayHigh;
    gfloat maximaDoPeriodo = *( std::max_element(arrayHigh, &arrayHigh[sizeHigh]) );
    
    // Calcular %K
    
    gfloat kValue = ( closeMaisRecente - minimaDoPeriodo)/(maximaDoPeriodo - minimaDoPeriodo);
    kValue = 100.00*kValue;

    this->decisionData.lowDaily = minimaDoPeriodo;
    this->decisionData.highDaily = maximaDoPeriodo;
    this->decisionData.stochastic_8 = kValue;
}

/*******************************************************************************
 *	Função: doDecision
 *	----------------------
 * 	Função dedicada a fazer a decisão de compra e de venda
 *	recebe: nada
 *	retorna: nada. apensa seta o booleano "isPurchaseAction
 * OBS: Essa função será chamada uma vez apenas após pupulateData()
 ******************************************************************************/

calculation Decision::doDecision(void)
{   
    this->calculation_process = WAITING_FOR_CALCULATION;

    if(this->isPurchaseAction() == true)
    {
        this->purchaseAction = true;
    }
    else if(this->isSaleAction() == true)
    {
        this->purchaseAction = false;
    }

    this->managementRisk();
    
    this->calculation_process = CALCULATION_IS_OVER;

    return this->calculation_process;
    
}

/*******************************************************************************
 *	Função: isSaleDecision
 *	----------------------
 * 	Função dedicada a a verificar se a ação deve ser vendida dentro da regra
 *  de negócios estipulada
 *	recebe: nada
 *	retorna: booleano se a ação deve ser vendida
 ******************************************************************************/
bool Decision::isSaleAction(void)
{
    if( // Verdadeiro se todas estas condições são verdadeiras:
        (this->decisionData.closeWeek < this->decisionData.average_20) &&
        (this->decisionData.average_20 < this->decisionData.average_50) &&
        (this->decisionData.stochastic_8 > MAX_STOCHASTIC_SALE)
    )  {return true;}
      
      return false;
}

/*******************************************************************************
 *	Função: isPurchasecision
 *	----------------------
 * 	Função dedicada a a verificar se a ação pode ser comprada dentro da regra
 *  de negócios estipulada
 *	recebe: nada
 *	retorna: booleano se a ação deve ser comprada.
 ******************************************************************************/
bool Decision::isPurchaseAction(void)
{
    if( // Verdadeiro se todas estas condições são verdadeiras:
        (this->decisionData.closeWeek >= this->decisionData.average_20) &&
        (this->decisionData.average_20 >= this->decisionData.average_50) &&
        (this->decisionData.stochastic_8 <= MAX_STOCHASTIC_PURCHASE)
    ) {return true;}
    
    return false;
}

/*******************************************************************************
 *	Função: isPurchasecision
 *	----------------------
 * 	Função fazer o gerenciamento de risco estipulando quantidade de ações de compra
 *  ou venda. Popula uma struct de retorno para a classe principal
 *	recebe: nada
 *	retorna: nada
 ******************************************************************************/

void Decision::managementRisk()
{
    int target, qtdStocks = 1;
    float stopLoss, trigger, riskTrade;
    	
    
    if(this->purchaseAction == true)
    {
        stopLoss = this->decisionData.lowDaily;
        trigger = this->decisionData.highDaily;
        target = ((trigger - stopLoss) * 3) + trigger;
        
	    while(riskTrade < (0.01*this->userMoney)){
                
            riskTrade = (trigger - stopLoss) * qtdStocks;
            qtdStocks++;
        }
       this->outputUser.message  = "Compra";
	}

    else if(this->purchaseAction == false)
    {
        stopLoss = this->decisionData.highDaily;
        trigger = this->decisionData.lowDaily;
        target = trigger - ((stopLoss - trigger) * 3);
		
		while(riskTrade < (0.01*this->userMoney)){
			riskTrade = (stopLoss - trigger) * qtdStocks;
			qtdStocks++;
            
		}
	     this->outputUser.message  = "Venda";
    }
    this->outputUser.stopLoss = stopLoss;
    this->outputUser.trigger = trigger;
    this->outputUser.target = target;
    this->outputUser.qtdStocks = qtdStocks;
}

/*******************************************************************************
 *	Função: getUserMoney
 *	----------------------
 * 	Retorna variavel privada chamada UserMOney
 *  de negócios estiplada
 *	recebe: nada
 *	retorna: float 
 ******************************************************************************/	
float Decision::getUserMoney(void)
{
    return this->userMoney;
}

/*******************************************************************************
 *	Função: setUserMoney
 *	----------------------
 * 	Dá acesso à variavel privada userMoney a classes externas
 *  de negócios estiplada
 *	recebe: float
 *	retorna: nada
 ******************************************************************************/	
void Decision::setUserMoney(float money)
{
    this->userMoney = money;
}

/*******************************************************************************
 *	Função: getUserData
 *	----------------------
 * 	Retorna struct privada chamada outputUser
 *	recebe: nada
 *	retorna: struct outputUser 
 ******************************************************************************/
userData Decision::getUserData(void)
{   
    return this->outputUser;
}

Decision::Decision(){}
Decision::~Decision(){}
