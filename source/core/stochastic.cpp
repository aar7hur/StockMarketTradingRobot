#include "../include/stochastic.h"
#include <iostream> //debug
#include <cstring>

Stochastic::Stochastic(){}
Stochastic::~Stochastic(){}

/*******************************************************************************
 *	Stochastic:: setHighDailyPrice
 *	----------------------
 * 	Le os dados e verifica o maior preço da ação no intervalo
 *  de dados estipulados.
 *	recebe:: nada. Apenas popula os atributos da classe
 *  retorna: nada
 * OBS: Essa função será chamada uma vez apenas
 ******************************************************************************/
void Stochastic::setHighDailyPrice(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
{
    float priceCloseDaily[MAX_ARRAY_SIZE], price[MAX_ARRAY_SIZE+16];
    float  bestprice;
    float priceHighDaily[MAX_ARRAY_SIZE];
    int n = 8; // n = intervalo que deseja analisar

    for(int i = 0; i < MAX_ARRAY_SIZE; i ++) {
        priceCloseDaily[i] = price[i];
    }

    for(int i = 0; i < MAX_ARRAY_SIZE; i++) {
        if(i > 8) {
            n++;
        }
        bestprice = price[i];

        for(int j = i; j < i + n; j++) {
            if(price[j] > bestprice) {
                bestprice = price[j];
            }
        }
        this->stochasticData.priceHighDaily[i] = bestprice;

    }
    this->highPriceDaily = priceHighDaily[0];
}


/*******************************************************************************
 *	Stochastic:: KVaues
 *	----------------------
 * 	Le os dados e verifica o maior preço da ação no intervalo
 *  de dados estipulados.
 *	recebe:: nada. Apenas popula os atributos da classe
 *  retorna: nada
 *  OBS: Essa função será chamada uma vez apenas
 ******************************************************************************/
size_t Stochastic::kValues(float * output, float * arrayMin, float* arrayMax, float* arrayClose, size_t sizeClose){
    enum { magic_periodos = 8};
    float kArray[sizeClose]; size_t kSize=0;
    float k;
    
    for(size_t i=magic_periodos; i < sizeClose; ++i){
        k = ( arrayClose[i] - arrayMin[i-magic_periodos] ) / ( arrayMax[i-magic_periodos] - arrayMin[i-magic_periodos] );
        k *= 100;
        kArray[kSize++] = k;
    }
    
    output = new float[kSize];
    memcpy(output, kArray, kSize);
    
    return kSize;
}

/*******************************************************************************
 *	Stochastic:: setLowDAyPrice
 *	----------------------
 * 	Le os dados e verifica o menor preço da ação no intervalo
 *  de dados estipulados.
 *	recebe:: nada. Apenas popula os atributos da classe
 *  retorna: nada
 * OBS: Essa função será chamada uma vez apenas
 ******************************************************************************/
void Stochastic::setLowDailyPrice(void)
{
    int n = 8;
    float worstprice;
    float priceLowDaily[MAX_ARRAY_SIZE];

    for(int i = 0; i < MAX_ARRAY_SIZE; i++) {
        if(i > 8) {
            n++;
        }
        worstprice = this->stochasticData.price[i];
        for(int j = i; j < i + n; j++) {
            if(this->stochasticData.price[j] < worstprice) {
                worstprice = this->stochasticData.price[j];
            }
        }
        this->stochasticData.priceLowDaily[i] = worstprice;
    }
    this->lowPriceDaily = priceLowDaily[0];

}

void Stochastic::getKcurve(void)
{
    float Kcurve[MAX_ARRAY_SIZE];

    for(int i = 0; i < MAX_ARRAY_SIZE + 2; i++) {
        this->stochasticData.Kcurve[i] = 100 * 
        ((this->stochasticData.price[i] - this->stochasticData.priceLowDaily[i])
        /(this->stochasticData.priceLowDaily[i]-this->stochasticData.priceLowDaily[i]));
    }
    this->stochasticResult = this->stochasticData.Kcurve[0];
}


/*******************************************************************************
 *	Stochastic:: averageKcurve
 *	----------------------
 * 	Calcula a média da curvaK e guarda num dado da struct
 *	recebe:: nada. 
 *  retorna: nada Apenas popula os atributos da classe
 ******************************************************************************/
void Stochastic::averageKcurve(void)
{
    for(int i = 0; i < MAX_ARRAY_SIZE; i++) {
        this->stochasticData.KcurveAverage[i] = this->stochasticData.Kcurve[i] + 
        this->stochasticData.Kcurve[i + 1] + this->stochasticData.Kcurve[i + 2];
    }
}

/*******************************************************************************
 *	Stochastic:: averageKcurve
 *	----------------------f
 * 	Calcula a média da curvaK e guarda num dado da struct
 *	recebe:: nada. 
 *  retorna: nada Apenas popula os atributos da classe
 ******************************************************************************/
void Stochastic::setPrice(float* price, size_t arraySize)
{
    for(int i = 0; i < arraySize; i++)
    {
        this->stochasticData.price[i] = price[i];
    }
}

/*******************************************************************************
 *	Stochastic:: getResult
 *	----------------------
 * 	Acessa o atributo privado stochasticResult e retorna seu conteudo
 *	recebe:: nada. 
 *  retorna: this->stochasticResult
 ******************************************************************************/
float Stochastic::getResult(void)
{
    return this->stochasticResult;
}

/*******************************************************************************
 *	Stochastic:: getLowDailyPrice
 *	----------------------
 * 	Acessa o atributo privado lowPriceDaily retorna seu conteudo
 *	recebe:: nada. 
 *  retorna: this->lowPriceDaily
 ******************************************************************************/
float Stochastic::getLowDailyPrice(void)
{
    return this->lowPriceDaily;
}

/*******************************************************************************
 *	Stochastic:: geHighPriceDaily
 *	----------------------
 * 	Acessa o atributo privado highPriceDAily retorna seu conteudo
 *	recebe:: nada. 
 *  retorna: this->highPriceDaily
 ******************************************************************************/
float Stochastic::getHighDailyPrice(void)
{
    return this->highPriceDaily;
}
