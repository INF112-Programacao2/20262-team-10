#include <iostream>
#include "touro.hpp"

Touro::Touro(std::string id, char s, double p, int iM, int qtdF, int qtdV, double vDS) : Animal(id, s, p, iM, qtdF, qtdV) , valorDoseSemen(vDS) {}

Touro::~Touro(){}

//gets
double Touro:: getValorDoseSemen()const {return valorDoseSemen;}
double Touro:: getMerito()const {return meritoGeneticoPTA;}
int Touro:: getDosesEstoque()const {return dosesEstoque;}
int Touro:: getQtdCoberturas()const {return qtdCoberturas;}
//sets
void Touro:: setValorDoseSemen(double v) {valorDoseSemen=v;}
void Touro:: setMerito(double m) {meritoGeneticoPTA=m;}
void Touro:: setDosesEstoque(int d) {dosesEstoque=d;}
void Touro:: setQtdCoberturas(int q) {qtdCoberturas=q;}