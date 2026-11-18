#include <iostream>
#include "touro.hpp"

Touro::Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double vDS) : Animal(id, s, p, iM, qtdF, qtdV) , valorDoseSemen(vDS) {}

Touro::~Touro(){}

//gets
double Touro:: getValorDoseSemen()const {return valorDoseSemen;}
double Touro:: getMerito()const {return meritoGeneticoPTA;}
int Touro:: getDosesEstoque()const {return dosesEstoque;}
int Touro:: getQtdCoberturas()const {return qtdCoberturas;}
std::string Touro::getTipo() const {return "touro";}
bool Touro::podeSerPromovido() const {return false;} 
//sets
void Touro:: setValorDoseSemen(double v) {valorDoseSemen=v;}
void Touro:: setMerito(double m) {meritoGeneticoPTA=m;}
void Touro:: setDosesEstoque(int d) {dosesEstoque=d;}
void Touro:: setQtdCoberturas(int q) {qtdCoberturas=q;}

double Touro::calcularGastoDiario(double precoLeite, SacaRacao** relatorioRacao) {
    double precoRacao = 0;
    double pesoSaco = 0;
    for (int i = 0; i < 3; i++) {
        if (relatorioRacao[i]->name == "Racao Avulsa") {
            precoRacao = relatorioRacao[i]->preco;
            pesoSaco = relatorioRacao[i]->peso;
        }
    }
    return (peso * 0.020)*precoRacao/pesoSaco;
}

double Touro::calcularReceitaDiaria(double valorMercado) {return 0.0;}