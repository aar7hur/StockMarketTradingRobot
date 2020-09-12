#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "readcsv.h"

Coluna::Coluna(std::string headerEntrada){
    header = headerEntrada;
}

std::string Coluna::getHeader() const{
    return header;
}
    
Coluna& Coluna::operator<<(std::string entrada){
    dados.push_back(entrada);
    return *this;
}

// Array de std::strings
size_t Coluna::operator>>(std::string *&array){

    if (dados.empty()){
        array = NULL;
        return 0;
    }
    
    size_t tamanho = dados.size();
    array = new std::string[tamanho];
    
    for(size_t i=0; i<tamanho; i++){
        array[i] = dados[i];
    }
    
    return tamanho;
}

// Array de Ints
size_t Coluna::operator>>(int *&array){

    if (dados.empty()){
        array = NULL;
        return 0;
    }
    
    size_t tamanho = dados.size();
    array = new int[tamanho];
    
    for(size_t i=0; i < tamanho; ++i){
        array[i] = std::stoi(dados[i]);
    }
    
    return tamanho;
}

// Array de Floats	
size_t Coluna::operator>>(float *&array){

    if (dados.empty()){
        array = NULL;
        return 0;
    }
    
    size_t tamanho = dados.size();
    array = new float[tamanho];
    
    for(size_t i=0; i < tamanho; ++i){
        array[i] = std::stof(dados[i]);
    }
    
    return tamanho;
}

// Array de Double
size_t Coluna::operator>>(double *&array){

    if (dados.empty()){
        array = NULL;
        return 0;
    }
    
    size_t tamanho = dados.size();
    array = new double[tamanho];
    
    for(size_t i=0; i < tamanho; ++i){
        array[i] = std::stod(dados[i]);
    }
    
    return tamanho;
}

Tabela::Tabela(std::string acao, std::string tipo){
    std::string buffer;
    std::vector<std::string> linhas;
    
    // Abre o CSV correspondente a ação
    // Prefixo d_ para diária, w_ para mensal (de weekly)
    if (tipo != "d" && tipo != "w"){
        throw "Falha ao ler o CSV: Tipo de ação invalida!";
    }        

    transform(acao.begin(), acao.end(), acao.begin(), toupper);
    acao = "ativos/" + tipo + "_" + acao + ".SA.csv";
    std::ifstream arquivoCSV(acao.c_str());

    if(arquivoCSV.fail()){
        throw "Ação não encontrada!";
    }

    // Le o arquivo e armazena as linhas.
    while(getline(arquivoCSV, buffer)){
        linhas.push_back(buffer);
    }
    
    // Seta os metadados do objeto
    headers = quebrarLinha(linhas[0]);
    for(auto i = headers.begin(); i != headers.end(); i++){
            colunas.push_back( Coluna(*i) );
    }
    
    // Transfere as linhas 
    for(auto i = linhas.rbegin(); i != --linhas.rend(); ++i){
        *this << quebrarLinha(*i);
    }

}

Tabela& Tabela::operator<<(std::vector<std::string> linha){
    if (linha.size() != headers.size()) {
        std::cerr << "Tentado inserir uma linha com " << linha.size() << " colunas em uma tabela de " << headers.size() << " colunas." << std::endl;
        throw "Erro ao inserir linha: dimensões invalidas!";
    }

    for(size_t i; i < headers.size(); ++i){
        colunas[i] << linha[i];
    }
    
    return *this;
}

Coluna& Tabela::operator[](std::string header){
    for(auto i = colunas.begin(); i != colunas.end(); i++){
        if (header.compare(i->getHeader()) == 0){
            return *i;
        }
    }
    
    std::cerr << "Tentado acessar uma coluna chamada \""<< header << "\" de uma Tabela que não tem tal coluna." << std::endl;
    throw "Erro ao acessar uma coluna: coluna inexistente!";
}

std::vector<std::string> quebrarLinha(const std::string linha, char virgula){
    std::vector<std::string> array;
    std::string token;
    std::stringstream stringFile(linha);
    while(getline(stringFile, token, virgula)){
        array.push_back(token);
    }

    return array;
}