#include <iostream>
#include "boicorte.hpp"

Boicorte::Boicorte(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {} 

Boicorte::~Boicorte() {}

//gets
double Boicorte:: getrendimentoCarcaca()const {return rendimentoCarcaca;}
double Boicorte:: getmetaAbatePeso()const {return metaAbatePeso;}
bool Boicorte:: getFaseEngorda()const {return faseEngorda;}
std::string Boicorte:: getTipo() const {return "boicorte";}
bool Boicorte:: podeSerPromovido() const {return false;}
//sets
void Boicorte:: setrendimentoCarcaca(double r) {rendimentoCarcaca=r;}
void Boicorte:: setmetaAbatePeso(double m) {metaAbatePeso=m;}
void Boicorte:: setFaseEngorda(bool f) {faseEngorda=f;}

double Boicorte::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    double precoRacaoEngorda = 0;
    double pesoSacoEngorda = 0;
    double precoRacaoAvulsa = 0;
    double pesoSacoAvulsa = 0;
    for (int i = 0; i < 3; i++) {
    if (relatorioRacao[i]->name == "Racao Engorda") {
            precoRacaoEngorda = relatorioRacao[i]->preco;
            pesoSacoEngorda = relatorioRacao[i]->peso;
        }
    if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacaoAvulsa = relatorioRacao[i]->preco;
            pesoSacoAvulsa = relatorioRacao[i]->peso;
        }
    }
    
    if(faseEngorda){
        return (peso * 0.026) * precoRacaoEngorda/pesoSacoEngorda;
    }
    return (peso * 0.021)*precoRacaoAvulsa/pesoSacoAvulsa;
}

double Boicorte::calcularReceitaDiaria(double valorMercado) { //precoLeite alias precoArroba
    if(faseEngorda){
        return ((rendimentoCarcaca*1.4)/15) * valorMercado;
    }
    return ((rendimentoCarcaca*0.7)/15) * valorMercado;
}