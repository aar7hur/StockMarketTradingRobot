#ifndef READCSV_HEADER_GUARD
#define READCSV_HEADER_GUARD

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

std::vector<std::string> quebrarLinha(const std::string linha, char virgula=',');

class Coluna{
    private:
    std::string header;
    std::vector <std::string> dados;

    public:
    Coluna(std::string headerEntrada);
    
    std::string getHeader() const;

    Coluna& operator<<(std::string entrada);

    size_t operator>>(std::string *&array);
    size_t operator>>(int *&array);
    size_t operator>>(float *&array);
    size_t operator>>(double *&array);
};


class Tabela{
    private:
    std::vector<std::string> headers;
    std::vector<Coluna> colunas;
    
    public:
    Tabela(std::string acao, std::string tipo="d");
    
    Tabela& operator<<(std::vector<std::string> linha);
   
    Coluna& operator[](std::string header);
};

#endif