#include <iostream>
#include "bezerro.hpp"

Bezerro::Bezerro(std::string id, char s, double p) : Animal(id, s, p) {
    desmamado = false;
    idadeDesmameMeses = 0;
}

Bezerro::Bezerro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double cLL, double gPMD, int iDM, bool d) : Animal(id, s, p, iM, qtdF, qtdV) , consumoLeiteLitros(cLL), ganhoPesoMedioDiario(gPMD), idadeDesmameMeses(iDM), desmamado(d) {}

Bezerro::~Bezerro() {}

//gets
double Bezerro:: getConsumoLeiteLitros()const {return consumoLeiteLitros;}
double Bezerro:: getGanhoPesoMedioDiario()const {return ganhoPesoMedioDiario;}
int Bezerro:: getIdadeDesmameMeses()const {return idadeDesmameMeses;}
bool Bezerro:: getDesmamado()const {return desmamado;}
std::string Bezerro:: getTipo() const {return "bezerro";}
bool Bezerro:: podeSerPromovido() const {if(atingiuIdadeAdulta()){return true;}return false;}
//sets
void Bezerro:: setConsumoLeiteLitros(double c) {consumoLeiteLitros=c;}
void Bezerro:: setGanhoPesoMedioDiario(double g) {ganhoPesoMedioDiario=g;}
void Bezerro:: setIdadeDesmameMeses(int i) {idadeDesmameMeses=i;}
void Bezerro:: setDesmamado(bool d) {desmamado=d;}

void Bezerro::realizarDesmame() {
    if(idadeMeses >= 7){
        desmamado = true;
        idadeDesmameMeses = idadeMeses;
    }
    else{
        std :: cout << "O bezerro nao esta com a idade adequada para o desmame\n";
    }
}

bool Bezerro::atingiuIdadeAdulta() const {
    if(idadeMeses >=12){
        return true;
    }
    else{
        return false;
    }
}