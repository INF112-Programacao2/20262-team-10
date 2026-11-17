#include "parserplanilha.hpp"
#include "farm.hpp"
#include "area.hpp"
#include "vaca.hpp"
#include "touro.hpp"
#include "boicorte.hpp"
#include "bezerro.hpp"
#include <fstream>
#include <stdexcept>
#include <cstdlib>


std::string ParserPlanilha::splitManual(std::string linha, char delim, int tamOut){
    int colunaAtual = 0;
    std::string token = "";

    for(size_t i = 0; i < linha.length(); i++) {
        if(linha[i] == '\r' || linha[i] == '\n') continue;

        if (linha[i] == delim) {
            if (colunaAtual == tamOut) return token;
            colunaAtual++;
            token = "";
        }else {
            token = linha[i];
        }

    }
    if(colunaAtual == tamOut) return token;
    return "";
}
int ParserPlanilha::importarCSV(std::string arquivo, Farm* f) 
{
    if (f == nullptr) {
        throw std::invalid_argument("Ptr nulo de farm ao importar");
    }
    std::ifstream in(arquivo);
    if(!in.is_open()){
        throw std::runtime_error("Nn abriu o csv" + arquivo);
    }
}