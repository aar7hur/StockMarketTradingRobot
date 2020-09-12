#include "movingAverage.h"

/*******************************************************************************
 *	Função: movingAverage
 *	----------------------
 * 	Função dedicada ao cálculo da média móvel dos dados.
 *	recebe: vetor de Preços de Fechamento Semanal e o Período p/ cálculo 
 *	retorna: inteiro com o Preço da Média Móvel
 * OBS: Essa função será chamada duas vezes pois o algoritmo exige 2 médias móveis
 com períodos de 20 e 50.
 ******************************************************************************/
void MovingAverage::setmovingAverage(float *priceCloseWeek, int period){

	float aux, sum;
	sum = 0;
	for(aux = period; aux > 0; aux--){
		sum += *priceCloseWeek;
		priceCloseWeek--;
	}
	this->average = (sum)/period;
}

float MovingAverage::getAverage(void){
	return this->average;
}

MovingAverage::MovingAverage(float *priceCloseWeek, int period){}
MovingAverage::~MovingAverage(){}
