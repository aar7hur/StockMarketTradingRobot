#include "decision.h"
#include "readcsv.h"
#include "movingAverage.h"
#include <string>

/*******************************************************************************
 *	Função: populateData
 *	----------------------
 * 	Função dedicada popular as structs com os dados necessários
 *	recebe: nada
 *	retorna: nada. Apenas popula os atributos da classe
 * OBS: Essa função será chamada uma vez apenas
 ******************************************************************************/
void Decision::populateData(std::string ativo, std::string tipo){

    Tabela meuAtivo(ativo, tipo);

    float *array;
    this->movingAverage.setmovingAverage(array, 20);
    this->decisionData.average_20 = this->movingAverage.getAverage();

    this->movingAverage.setmovingAverage(array, 50);
    this->decisionData.average_50 = this->movingAverage.getAverage();

    //******************************************************
    // IMplementar parte do jefferson para popular float stochastic_8;

    //***********************
    //Popular highdaily e lowDaily da struct

}

/*******************************************************************************
 *	Função: doDecision
 *	----------------------
 * 	Função dedicada a fazer a decisão de compra e de venda
 *	recebe: nada
 *	retorna: nada. apensa seta o booleano "isPurchaseAction
 * OBS: Essa função será chamada uma vez apenas após pupulateData()
 ******************************************************************************/

void Decision::doDecision(void)
{
    if(this->isPurchaseAction() == true)
    {
        this->purchaseAction = true;
    }
    else if(this->isSaleAction() == true)
    {
        this->purchaseAction = false;
    }

    this->managmentRisk();
    
}

/*******************************************************************************
 *	Função: isSaleDecision
 *	----------------------
 * 	Função dedicada a a verificar se a ação deve ser vendida dentro da regra
 *  de negócios estiplada
 *	recebe: nada
 *	retorna: booleano se a ação deve ser vendida
 ******************************************************************************/
bool Decision::isSaleAction(void)
{
    if(this->decisionData.closeWeek < this->decisionData.average_20)
    {
         if(this->decisionData.average_20 < this->decisionData.average_50)
         {
            if(this->decisionData.stochastic_8 > MAX_STOCHASTIC_SALE) 
            {
                return true;
            }
         }
    }
}

/*******************************************************************************
 *	Função: isPurchasecision
 *	----------------------
 * 	Função dedicada a a verificar se a ação pode ser comprada dentro da regra
 *  de negócios estiplada
 *	recebe: nada
 *	retorna: booleano se a ação deve ser comprada.
 ******************************************************************************/
bool Decision::isPurchaseAction(void)
{
    if(this->decisionData.closeWeek >= this->decisionData.average_20)
    {
         if(this->decisionData.average_20 >= this->decisionData.average_50)
         {
            if(this->decisionData.stochastic_8 <= MAX_STOCHASTIC_PURCHASE) 
            {
                return true;
            }
         }
    }
}

/*******************************************************************************
 *	Função: isPurchasecision
 *	----------------------
 * 	Função fazer o gerenciamento de risco estipulando quantidade de ações de compra
 *  ou venda. Popula uma struct de retorno para a classe principal
 *	recebe: nada
 *	retorna: nada
 ******************************************************************************/

void Decision::managmentRisk()
{
    int target, qtdStocks = 1;
    float stopLoss, trigger, riskTrade;
    

    stopLoss = *lowDaily;
	trigger = *highDaily;

    if(this->purchaseAction = true)
    {
        target = ((trigger - stopLoss) * 3) + trigger;
	    while(riskTrade < (0.01*userMoney)){
                
            riskTrade = (trigger - stopLoss) * qtdStocks;
            qtdStocks++;	
        }
        this->outputUser.message  = "Compra";
	}

    else if(this->purchaseAction = false)
    {
        target = trigger - ((stopLoss - trigger) * 3);
		while(riskTrade < (0.01*userMoney)){
			riskTrade = (stopLoss - trigger) * qtdStocks;
			qtdStocks++;	
		}
		this->outputUser.message  = "Venda";
    }

    this->outputUser.stopLoss = stopLoss;
    this->outputUser.trigger = trigger;
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
Decision::userData Decision::getUserData(void)
{
    return this->outputUser;
}


Decision::Decision(){}
Decision::~Decision(){}