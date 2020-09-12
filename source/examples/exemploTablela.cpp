#include "readcsv.h"

using namespace std;

int main(){

    /** Usar a Tabela exige apenas 3 passos:
      *
      *  1. Carregar o Ativo
      *  2. Declarar uma array do tipo desejado
      *  3. Selecionar uma coluna e mandar para a Array
      *
      **/

    /* 1. Carregar o Ativo: */
    
    Tabela meuAtivo("ITUB4");
    
    // Você também pode especificar um segundo argumento para definir o periodo:
    //   Tabela meuAtivo("ITUB4", "d") => Retorna intervalo por dias, mesmo que sem argumentos
    //   Tabela meuAtivo("ITUB4", "w") => Retorna intervalo por semana ("w" de Weekly)
    
    // Se o Ativo não existir é levantado uma excessão
    // É uma boa ideia envolver seu código em um try-catch();
    
    /* 2. Declarar uma array do tipo desejado */
    
    float *array;
    
    // Você pode declarar como uma array do tipo string, int, float ou double
    
    /* 3. Selecionar uma coluna e mandar para a Array */
    
    meuAtivo["Close"] >> array;
    
    // A função acima retorna o tamanho da array como um size_t
    // É recomendável armazenar e usar esta informação:
    //   size_t arraySize = meuAtivo["Close"] >> array;
    //   if (arraySize > 9) { funcaoComArray(array); }
    
    return 0;
}

/** Ativos disponíveis:
  *
  * ABEV3
  * AZUL4
  * BBAS3
  * ELET3
  * ITSA4
  * ITUB4
  * PETR4
  * VALE3
  * VVAR3
  * WEGE3
  *
  **/
  
  /** Colunas disponíveis:
  *
  * Date
  * Open
  * High
  * Low
  * Close
  * Adj Close
  * Volume
  **/