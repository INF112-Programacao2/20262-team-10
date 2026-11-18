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
std :: string Bezerro:: getIdPai()const{return idPai;}
std :: string Bezerro:: getIdMae()const{return idMae;}
bool Bezerro:: podeSerPromovido() const {if(atingiuIdadeAdulta()){return true;}return false;}
//sets
void Bezerro:: setConsumoLeiteLitros(double c) {consumoLeiteLitros=c;}
void Bezerro:: setGanhoPesoMedioDiario(double g) {ganhoPesoMedioDiario=g;}
void Bezerro:: setIdadeDesmameMeses(int i) {idadeDesmameMeses=i;}
void Bezerro:: setDesmamado(bool d) {desmamado=d;}
void Bezerro:: setIdPai(std::string id) {idPai = id;}
void Bezerro:: setIdMae(std::string id) {idMae = id;}

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

double Bezerro::calcularGastoDiario(double precoLeite, double precoRacao) {
    if(precoLeite <=0 || precoRacao <=0) {
        return 1.5;
    }
    if(!desmamado){ //por hora 1.5 é um custo fixo operacional puramente especulativo
        return (consumoLeiteLitros * precoLeite) + 1.5;
    }
    else { //bezerros consomem em média 2.8% de seu peso corporal
        return peso * precoRacao * 0.028;
    }
}

//Agio : é o percentual de valorização extra pago por arroba (ou quilo) de um animal jovem de reposição 
//— como o bezerro — em relação ao preço base da arroba do boi gordo.
double Bezerro::calcularReceitaDiaria(double valorMercado) {
    double agio = 0.05; // calculo com base na idade do animal ganho de peso diario
    if(idadeMeses<=8) (sexo=='M') ? agio=0.22 : agio=0.15;
    else if(idadeMeses<=14) (sexo=='M') ? agio=0.12 : agio=0.08;
    return (ganhoPesoMedioDiario/30.0) * valorMercado * (1+agio);
}
    