#include <iostream>
#include "vaca.hpp"

Vaca::Vaca(std::string id, char s, double p, int iM, int qtdF, int qtdV) : Animal(id, s, p, iM, qtdF, qtdV) {}

Vaca::~Vaca(){

}

//gets
double Vaca:: getProducaoLeiteDia()const {return producaoLeiteDia;}
bool Vaca:: getEmLactacao()const {return emLactacao;}
std::string Vaca::getTipo() const {return "vaca";}
bool Vaca::podeSerPromovido() const {return false;}
//sets
void Vaca:: setProducaoLeiteDia(double p) {producaoLeiteDia=p;}
void Vaca:: setEmLactacao(bool l) {emLactacao=l;}

double Vaca::calcularGastoDiario([[maybe_unused]] double precoLeite, SacaRacao** relatorioRacao) {
    double precoRacao = 0;
    double pesoSaco = 0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            pesoSaco = relatorioRacao[i]->peso;
        }
    }
    if(emLactacao){ //vacas em lactacao tem consumo por kilo corporal mais elevado
        return (peso * 0.028)*precoRacao/pesoSaco;
    }
    return (peso*0.022)*precoRacao/pesoSaco;
}

double Vaca::calcularReceitaDiaria(double valorMercado) {
    if(emLactacao){
        return producaoLeiteDia * valorMercado;
    }
    return 0.0;
}