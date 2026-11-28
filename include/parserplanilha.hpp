#ifndef PARSERPLANILHA_HPP
#define PARSERPLANILHA_HPP

#include <iosfwd>
#include <sstream>
#include <string>

//class declaration for evitar dependência circular(chamar varias vzs a mesma class)

class Farm;
class Animal;
class SnapshotAncestral;

struct ResumoIportacao {
    int areas = 0;
    int animais = 0;
    int ancestrais = 0; // registros lidos (linhas ANCESTRAL + os anexados no fim de linhas ANIMAL)
};

class ParserPlanilha {
    private:
        Farm& fazenda; // referencia para nn sobrecarregar o parser com posse da fazenda
        std::istringstream entrada; //linha atual reaproveitada a cada linha, nao cria stream por linha
        int numLinha;
        ResumoIportacao resumo;
}


#endif 