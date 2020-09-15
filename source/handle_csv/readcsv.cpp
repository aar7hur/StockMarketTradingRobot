#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "readcsv.h"


/********************************************
 * Functors Auxiliares
 ********************************************
 * É considerado boa prática em C++ passar
 * functors ao invés de ponteiros para funções
 ********************************************
 * Recebem: Uma string
 * Retornam: Um dado convertido para o tipo desejado
 ********************************************/

struct _transparente {
    std::string operator()(std::string string) {
        return string;
    }
}
transparente;

struct _stoi {
    int operator()(std::string string) {
        return std::stoi(string);
    }
}
stoi;

namespace Auxiliar{
    struct _stof {
    float operator()(std::string string) {
        return std::stof(string);
    }
}stof;
}

struct _stod {
    double operator()(std::string string) {
        return std::stod(string);
    }
}
stod;

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

std::vector < std::string > quebrarLinha(const std::string linha, char virgula) {
    std::vector < std::string > array;
    std::string token;
    std::stringstream stringFile(linha);
    while (getline(stringFile, token, virgula)) {
        array.push_back(token);
    }

    return array;
};

/********************************************
 * Construtor de Coluna
 ********************************************
 * Inicializa o header da coluna
 ********************************************
 * Recebe: Uma string
 ********************************************/

Coluna::Coluna(std::string headerEntrada) {
    header = headerEntrada;
}

/********************************************
 * Coluna :: getHeader ()
 ********************************************
 * Retorna o Header da Coluna
 ********************************************
 * Recebe: Nada
 * Retorna: Uma string
 ********************************************/

std::string Coluna::getHeader() const {
    return header;
}

/********************************************
 * Coluna :: Operador <<
 ********************************************
 * Insere um dado em uma coluna
 ********************************************
 * Recebe: Uma string
 * Retorna: Uma referencia para a coluna
 ********************************************/

Coluna & Coluna::operator << (std::string entrada) {
    dados.push_back(entrada);
    return *this;
}

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
    size_t Coluna::jogarPraArray(Tipo * & array, Func converter, size_t limite) {

        if (dados.empty()) {
            array = NULL;
            return 0;
        }

        size_t tamanho;

        if (limite < dados.size() && limite != 0) {
            tamanho = limite;
        } else {
            tamanho = dados.size();
        }

        array = new Tipo[tamanho];

        for (size_t i = 0; i < tamanho; i++) {
            array[i] = converter(dados[i]);
        }

        return tamanho;
    }

/********************************************
 * Coluna :: topo()
 ********************************************
 * Retorna o primeiro elemento da Coluna
 ********************************************
 * Recebe: Nada
 * Retorna: String
 ********************************************/
std::string Coluna::topo() {
    return dados.front();
}

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

Coluna_Interface::Coluna_Interface(Coluna * origem, size_t size) {
    coluna = origem;
    requestSize = size;
}

/********************************************
 * Coluna_Interface :: Operador ()
 ********************************************
 * Define o valor interno requestSize que
 * limita a quantidade de elementos que podem
 * ser transferidos para array de saida
 ********************************************
 * Recebe: size_t
 * Retorna: Referencia Coluna_Interface
 ********************************************/
Coluna_Interface & Coluna_Interface::operator()(size_t size) {
    requestSize = size;
    return *this;
}

/********************************************
 * Coluna_Interface :: Operadores >>
 ********************************************
 * Realiza a tranferência para uma array
 ********************************************
 * Recebe: Array de saida (tipo poliformizado)
 * Retorna: size_t
 ********************************************/

size_t Coluna_Interface::operator >> (std::string * & array) {
    return coluna -> jogarPraArray < std::string > (array, transparente, requestSize);
}

size_t Coluna_Interface::operator >> (int * & array) {
    return coluna -> jogarPraArray < int > (array, stoi, requestSize);
}

size_t Coluna_Interface::operator >> (float * & array) {
    return coluna -> jogarPraArray < float > (array, Auxiliar::stof, requestSize);
}

size_t Coluna_Interface::operator >> (double * & array) {
    return coluna -> jogarPraArray < double > (array, stod, requestSize);
}

/********************************************
 * Coluna_Interface :: topo ()
 ********************************************
 * Retornam o primeiro elemento da coluna
 ********************************************
 * Recebe: Nada
 * Retorna: Tipo genérico
 ********************************************/

std::string Coluna_Interface::topo() {
    return coluna -> topo();
}
template < > int Coluna_Interface::topo < int > () {
    return stoi(coluna -> topo());
}
template < > float Coluna_Interface::topo < float > () {
    return stof(coluna -> topo());
}
template < > double Coluna_Interface::topo < double > () {
    return stod(coluna -> topo());
}

/********************************************
 * Construtor de Tabela vazio
 ********************************************
 * Classes que herdam da Tabela chamam este construtor
 * O construtor abaixo recebe uma string e dá erro se não achar o arquivo
 * Este construtor vazio deixa a classe derivada fazer a busca
 ********************************************/

Tabela::Tabela() {
    ;
}

/********************************************
 * Construtor de Tabela
 ********************************************
 * Abre um arquivo e carrega em uma tabela
 ********************************************
 * Recebe: String
 ********************************************/

Tabela::Tabela(std::string arquivo) {
    this -> open(arquivo);
}

/********************************************
 * Tabela :: Open ()
 ********************************************
 * Abre um arquivo e carrega em uma tabela
 ********************************************
 * Recebe: String
 * Retorna: Referencia para Tabela
 ********************************************/

Tabela & Tabela::open(std::string arquivo) {
    std::string buffer;
    std::vector < std::string > linhas;

    std::ifstream arquivoCSV(arquivo);

    if (arquivoCSV.fail()) {
        std::cerr << "Erro: o arquivo \"" << arquivo << "\" existe?" << std::endl;
        throw "Ação não encontrada!";
    }

    // Le o arquivo e armazena as linhas.
    while (getline(arquivoCSV, buffer)) {
        linhas.push_back(buffer);
    }

    // Seta os metadados do objeto
    headers = quebrarLinha(linhas[0]);
    for (auto i = headers.begin(); i != headers.end(); i++) {
        colunas.push_back(Coluna( * i));
    }

    // Transfere as linhas 
    for (auto i = linhas.begin(); i != --linhas.end(); ++i) {
        * this << quebrarLinha( * i);
    }

    return *this;
}

/********************************************
 * Tabela :: Operador << 
 ********************************************
 * Lê uma linha e transfere cada elemento
 * para a coluna correspondente
 ********************************************
 * Recebe: Vetor de Strings
 * Retorna: Referencia de Tabela
 ********************************************/
Tabela & Tabela::operator << (std::vector < std::string > linha) {
    if (linha.size() != headers.size()) {
        std::cerr << "Tentado inserir uma linha com " << linha.size() << " colunas em uma tabela de " << headers.size() << " colunas." << std::endl;
        throw "Erro ao inserir linha: dimensões invalidas!";
    }

    for (size_t i; i < headers.size(); ++i) {
        colunas[i] << linha[i];
    }

    return *this;
}

/********************************************
 * Tabela :: Operador []
 ********************************************
 * Seleciona qual coluna da tabela se
 * deseja acessar, pelo header
 ********************************************
 * Recebe: String
 * Retorna: Coluna_Interface
 ********************************************/
Coluna_Interface Tabela::operator[](std::string header) {
    for (auto i = colunas.begin(); i != colunas.end(); i++) {
        if (header.compare(i -> getHeader()) == 0) {
            return Coluna_Interface( & * i);
        }
    }

    std::cerr << "Tentado acessar uma coluna chamada \"" << header << "\" de uma Tabela que não tem tal coluna." << std::endl;
    throw "Erro ao acessar uma coluna: coluna inexistente!";
}

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
Ativo::Ativo(std::string acao, std::string tipo) {
    this -> open(acao, tipo);
}

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
Ativo & Ativo::open(std::string acao, std::string tipo) {
    std::string buffer;
    std::vector < std::string > linhas;

    // Abre o CSV correspondente a ação
    // Prefixo d_ para diária, w_ para mensal (de weekly)
    if (tipo != "d" && tipo != "w") {
        throw "Falha ao ler o CSV: Tipo de ação invalida!";
    }

    transform(acao.begin(), acao.end(), acao.begin(), toupper);
    acao = "ativos/" + tipo + "_" + acao + ".SA.csv";
    std::ifstream arquivoCSV(acao.c_str());

    if (arquivoCSV.fail()) {
        std::cerr << "Erro: o arquivo \"" << acao << "\" existe?" << std::endl;
        throw "Ação não encontrada!";
    }

    // Le o arquivo e armazena as linhas.
    while (getline(arquivoCSV, buffer)) {
        linhas.push_back(buffer);
    }

    // Seta os metadados do objeto
    headers = quebrarLinha(linhas[0]);
    for (auto i = headers.begin(); i != headers.end(); i++) {
        colunas.push_back(Coluna( * i));
    }

    // Transfere as linhas 
    for (auto i = linhas.rbegin(); i != --linhas.rend(); ++i) {
        * this << quebrarLinha( * i);
    }

    return *this;
}

/********************************************
 * Ativo :: Operador << 
 ********************************************
 * Lê uma linha e transfere cada elemento
 * para a coluna correspondente
 ********************************************
 * Recebe: Vetor de Strings
 * Retorna: Referencia de Tabela
 ********************************************/
Ativo & Ativo::operator << (std::vector < std::string > linha) {
    if (linha.size() != headers.size()) {
        std::cerr << "Tentado inserir uma linha com " << linha.size() << " colunas em uma tabela de " << headers.size() << " colunas." << std::endl;
        throw "Erro ao inserir linha: dimensões invalidas!";
    }

    for (size_t i; i < headers.size(); ++i) {
        colunas[i] << linha[i];
    }

    return *this;
}

/********************************************
 * Ativo :: Operador []
 ********************************************
 * Seleciona qual coluna da tabela se
 * deseja acessar, pelo header
 ********************************************
 * Recebe: String
 * Retorna: Coluna_Interface
 ********************************************/
Coluna_Interface Ativo::operator[](std::string header) {
    for (auto i = colunas.begin(); i != colunas.end(); i++) {
        if (header.compare(i -> getHeader()) == 0) {
            return Coluna_Interface( & * i);
        }
    }

    std::cerr << "Tentado acessar uma coluna chamada \"" << header << "\" de uma Tabela que não tem tal coluna." << std::endl;
    throw "Erro ao acessar uma coluna: coluna inexistente!";
}
