#ifndef READCSV_HEADER_GUARD
#define READCSV_HEADER_GUARD

#include <cstddef>
#include <string>
#include <vector>

/********************************************
 * Functors Auxiliares
 ********************************************
 * É considerado boa prática em C++ passar
 * functors ao invés de ponteiros para funções
 ********************************************
 * Recebem: Uma string
 * Retornam: Um dado convertido para o tipo desejado
 ********************************************/
struct _transparente;
struct _stoi;
struct _stof;
struct _stod;

/********************************************
 * quebrarLinha ()
 ********************************************
 * Função auxiliar que separa uma string
 * em um vetor com base em um delimitador
 * "1,2,3" => <"1", "2", "3">
 ********************************************
 * Recebe: Uma string
 * Retorna: Um vetor de strings
 ********************************************/
std::vector < std::string > quebrarLinha(const std::string linha, char virgula = ',');

class Coluna {
    private:
        std::string header;
    std::vector < std::string > dados;

    public:
        /********************************************
         * Construtor de Coluna
         ********************************************
         * Inicializa o header da coluna
         ********************************************
         * Recebe: Uma string
         ********************************************/
        Coluna(std::string headerEntrada);

    /********************************************
     * Coluna :: getHeader ()
     ********************************************
     * Retorna o Header da Coluna
     ********************************************
     * Recebe: Nada
     * Retorna: Uma string
     ********************************************/
    std::string getHeader() const;

    /********************************************
     * Coluna :: Operador <<
     ********************************************
     * Insere um dado em uma coluna
     ********************************************
     * Recebe: Uma string
     * Retorna: Uma referencia para a coluna
     ********************************************/
    Coluna & operator << (std::string entrada);

    /********************************************
     * Template <class Tipo, class Func>
     * Coluna :: jogarPraArray () 
     ********************************************
     * Um template para funções que transferem
     * o conteudo de uma coluna para uma array
     * de um tipo arbitrário, retornando a
     * quantidade de elementos transferidos
     ********************************************
     * Recebe: Array de saida (tipo genérico), função de conversão, limite máximo de elementos (opcional)
     * Retorna: size_t
     ********************************************/
    template < class Tipo, class Func >
        size_t jogarPraArray(Tipo * & array, Func converter, size_t limite = 0);

    /********************************************
     * Coluna :: topo()
     ********************************************
     * Retorna o primeiro elemento da Coluna
     ********************************************
     * Recebe: Nada
     * Retorna: String
     ********************************************/
    std::string topo();

};

class Coluna_Interface {
    private: Coluna * coluna;
    public: size_t requestSize;

    public:
        /********************************************
         * Construtor de Coluna_Interface
         ********************************************
         * A classe Coluna_interface serve como
         * "açucar sintático" para facilitar
         * as operações de coluna. Aqui está o 
         * construtor da classe
         ********************************************
         * Recebe: Coluna, size_t
         ********************************************/
        Coluna_Interface(Coluna * origem, size_t size = 0);

    /********************************************
     * Coluna_Interface :: Opearador ()
     ********************************************
     * Define o valor interno requestSize que
     * limita a quantidade de elementos que podem
     * ser transferidos para array de saida
     ********************************************
     * Recebe: size_t
     * Retorna: Referencia Coluna_Interface
     ********************************************/
    Coluna_Interface & operator()(size_t size);

    /********************************************
     * Coluna_Interface :: Operadores >>
     ********************************************
     * Realiza a tranferência para uma array
     ********************************************
     * Recebe: Array de saida (tipo poliformizado)
     * Retorna: size_t
     ********************************************/
    size_t operator >> (std::string * & array);
    size_t operator >> (int * & array);
    size_t operator >> (float * & array);
    size_t operator >> (double * & array);

    /********************************************
     * Coluna_Interface :: topo ()
     ********************************************
     * Retornam o primeiro elemento da coluna
     ********************************************
     * Recebe: Nada
     * Retorna: Tipo genérico
     ********************************************/
    std::string topo();

    template < class T >
        T topo();
};

class Tabela {

    protected:
        std::vector < std::string > headers;
    std::vector < Coluna > colunas;

    /********************************************
     * Construtor de Tabela vazio
     ********************************************
     * Classes que herdam da Tabela chamam este construtor
     * O construtor abaixo recebe uma string e dá erro se não achar o arquivo
     * Este construtor vazio deixa a classe derivada fazer a busca
     ********************************************/
    Tabela(); // Construtor vazio, penas para heranças.

    public:

        /********************************************
         * Construtor de Tabela
         ********************************************
         * Abre um arquivo e carrega em uma tabela
         ********************************************
         * Recebe: String
         ********************************************/
        Tabela(std::string arquivo);

    /********************************************
     * Tabela :: Open ()
     ********************************************
     * Abre um arquivo e carrega em uma tabela
     ********************************************
     * Recebe: String
     * Retorna: Referencia para Tabela
     ********************************************/
    virtual Tabela & open(std::string arquivo);

    /********************************************
     * Tabela :: Operador << 
     ********************************************
     * Lê uma linha e transfere cada elemento
     * para a coluna correspondente
     ********************************************
     * Recebe: Vetor de Strings
     * Retorna: Referencia de Tabela
     ********************************************/
    Tabela & operator << (std::vector < std::string > linha);

    /********************************************
     * Tabela :: Operador []
     ********************************************
     * Seleciona qual coluna da tabela se
     * deseja acessar, pelo header
     ********************************************
     * Recebe: String
     * Retorna: Coluna_Interface
     ********************************************/
    Coluna_Interface operator[](std::string header);
};

class Ativo: public Tabela {

    public:
        /********************************************
         * Construtor de Ativo
         ********************************************
         * Esta classe herda da classe Tabela
         * O construtor recebe o nome da ação e um tipo.
         * Tipo pode ser "d" para diario ou "w" para semanal
         * A função localiza o arquivo no sistema de diretorios
         ********************************************
         * Recebe: Duas strings
         ********************************************/
        Ativo(std::string acao, std::string tipo = "d");

    /********************************************
     * Ativo :: Open ()
     ********************************************
     * A função recebe o nome da ação e um tipo.
     * Tipo pode ser "d" para diario ou "w" para semanal
     * A função localiza o arquivo no sistema de diretorios
     ********************************************
     * Recebe: Duas strings
     * Retorna: Referencia para Ativo
     ********************************************/
    Ativo & open(std::string acao, std::string tipo = "d");

    /********************************************
     * Ativo :: Operador << 
     ********************************************
     * Lê uma linha e transfere cada elemento
     * para a coluna correspondente
     ********************************************
     * Recebe: Vetor de Strings
     * Retorna: Referencia de Tabela
     ********************************************/
    Ativo & operator << (std::vector < std::string > linha);

    /********************************************
     * Ativo :: Operador []
     ********************************************
     * Seleciona qual coluna da tabela se
     * deseja acessar, pelo header
     ********************************************
     * Recebe: String
     * Retorna: Coluna_Interface
     ********************************************/
    Coluna_Interface operator[](std::string header);

};

#endif
